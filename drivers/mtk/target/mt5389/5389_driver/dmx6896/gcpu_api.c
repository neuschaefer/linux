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
#define AES_KEY_LEN_WRAPPER(len) (len == KEY_BIT_LEN_128)?0:(len == KEY_BIT_LEN_192)?1:2
#define DES_KEY_LEN_WRAPPER(len) (len == KEY_BIT_LEN_64)?0:(len == KEY_BIT_LEN_128)?1:(len == KEY_BIT_LEN_192)?2:3

#define IsValidSecureSlot(u4Ptr)    ((u4Ptr >= GCPU_SECURESLOT_PTR) && \
                                     (u4Ptr < GCPU_SECURESLOT_PTR + GCPU_SECURESLOT_NUM))

#define ADDR_INCR_IN_RING(addr, incr, ringstart, ringend)      \
    ((((addr) + (incr)) < (ringend)) ? ((addr) + (incr)) : (((addr) + (incr)) - ((ringend) - (ringstart))))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static CRIT_STATE_T _rGcpuLock;
#endif
static BOOL _fgGcpuInit = FALSE;
static GCPU_SECURE_SLOT_INFO _arSecureSlotInfo[GCPU_SECURESLOT_NUM];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static UINT32 _Gcpu_Align(UINT32 u4Addr, UINT32 u4Alignment)
{
	UINT32 u4Unaligned;

	if (u4Alignment <= 1)
	{
		return u4Addr;
	}

	u4Unaligned = u4Addr % u4Alignment;
	if (u4Unaligned != 0)
	{
		u4Addr += u4Alignment - u4Unaligned;
	}

	return u4Addr;
}


static BOOL _Gcpu_IsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
	return u4Addr == _Gcpu_Align(u4Addr, u4Alignment);
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _GCPU_Lock
 *  Enter gcpu critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _GCPU_Lock(void)
{
#ifndef CC_MTK_LOADER
    //Disable All Interrupt
    _rGcpuLock = x_crit_start();
#endif
}

//-----------------------------------------------------------------------------
/** _GCPU_Unlock
 *  Leave gcpu critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _GCPU_Unlock(void)
{
#ifndef CC_MTK_LOADER
    //Enable All Interrupt
    x_crit_end(_rGcpuLock);
#endif
}

UINT32 _GCPU_CopyRingBuffer(UINT32 u4Dst, UINT32 u4DstStart, UINT32 u4DstEnd,
    UINT32 u4Src, UINT32 u4SrcStart, UINT32 u4SrcEnd, UINT32 u4Size)
{
    UINT32 i;
    UINT8 *pSrc, *pDst;

    ASSERT((u4Dst >= u4DstStart) && (u4Dst < u4DstEnd));
    ASSERT((u4Src >= u4SrcStart) && (u4Src < u4SrcEnd));

    pSrc = (UINT8*)u4Src;
    pDst = (UINT8*)u4Dst;

    for (i = 0; i < u4Size; i++)
    {
        *pDst++ = *pSrc++;

        if ((UINT32)pDst >= u4DstEnd)
        {
            pDst = (UINT8*)u4DstStart;
        }

        if ((UINT32)pSrc >= u4SrcEnd)
        {
            pSrc = (UINT8*)u4SrcStart;
        }
    }

    return u4Dst;
}


// u4Size in byte
UINT32 _GCPU_AllocSecureSlot(UINT32 u4Size)
{
    UINT32 i, j, u4GetHWCnt = 0, u4SlotStartIndex = 0;
    UINT32 u4SlotPtr, u4SlotSize;

    u4SlotSize = (u4Size + 3) / 4;
    u4SlotPtr = 0;

    for (i = 0; i < GCPU_SECURESLOT_NUM; i++)
    {
        if (FALSE == _arSecureSlotInfo[i].fgUsed)
        {
            if (u4GetHWCnt == 0)
            {
                u4SlotStartIndex = i;
            }
            u4GetHWCnt++;
        }

        if (TRUE == _arSecureSlotInfo[i].fgUsed)
        {
            u4GetHWCnt = 0;
        }

        if (u4GetHWCnt >= u4SlotSize)
        {
            u4SlotPtr = u4SlotStartIndex + GCPU_SECURESLOT_PTR;
            _arSecureSlotInfo[u4SlotStartIndex].u1Size = u4SlotSize;

            for (j = u4SlotStartIndex; j < u4SlotStartIndex + u4SlotSize; j++)
            {
                _arSecureSlotInfo[j].fgUsed = TRUE;
            }

            break;
        }
    }

    return u4SlotPtr;
}

INT32 _GCPU_FreeSecureSlot(UINT32 u4SlotPtr)
{
    UINT32 i, u4SecureSlotSize = 0;
    UINT32 u4SecureSlotIdx = u4SlotPtr - GCPU_SECURESLOT_PTR;

    if (GCPU_SECURESLOT_NUM <= u4SecureSlotIdx)
    {
        return E_GCPU_PARAM_WRONG;
    }

    if ((FALSE == _arSecureSlotInfo[u4SecureSlotIdx].fgUsed) ||
        (0 == _arSecureSlotInfo[u4SecureSlotIdx].u1Size))
    {
        return E_GCPU_PARAM_WRONG;
    }

    u4SecureSlotSize = _arSecureSlotInfo[u4SecureSlotIdx].u1Size;

    for (i = u4SecureSlotIdx; i < u4SecureSlotIdx + u4SecureSlotSize; i++)
    {
        if(i < GCPU_SECURESLOT_NUM)
        {
            _arSecureSlotInfo[i].fgUsed = FALSE;
        }
        else
        {
            break;
        }
    }
    _arSecureSlotInfo[u4SecureSlotIdx].u1Size = 0;

    return S_GCPU_OK;
}


INT32 _GCPU_SetSecureSlot(UINT32 u4SlotPtr, UINT8* pu1Data, UINT32 u4Size)
{
    UINT8 i, *p, u1Loop;

    if(u4Size > 32)
    {
        return E_GCPU_PARAM_WRONG;
    }

    if(u4SlotPtr > GCPU_SECURESLOT_PTR + GCPU_SECURESLOT_NUM)
    {
        return E_GCPU_PARAM_WRONG;
    }

    u1Loop = (u4Size + 3) / 4;
    for(i = 0; i < u1Loop; i++)
    {
        p = pu1Data + i * 4;
        _GCPU_Hw_SetSlot((UINT8)(u4SlotPtr + i), KEY_WRAPPER(p));
    }

    return S_GCPU_OK;
}


INT32 _GCPU_Init(UINT32 u4Mode)
{
    INT32 i4Ret;

    if (_fgGcpuInit)
    {
        return S_GCPU_OK;
    }

    i4Ret = _GCPU_Hw_Init(u4Mode);

    _fgGcpuInit = TRUE;

    return i4Ret;
}


INT32 _GCPU_AES_CmdEx(UINT32 u4Handle, UINT32 u4Id, void* prParam, BOOL fgIOMMU)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;
    GCPU_STRUCT_T *prGcpu;

    UNUSED(i4Ret);

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_AES:
        {
            AES_PARAM_T *prLParam = (AES_PARAM_T *)prParam;
            AES_PARAM_T rLPParam = *prLParam;

            if(!fgIOMMU)
            {
                //traslate to physical memory
                rLPParam.u4SrcStartAddr = PHYSICAL(rLPParam.u4SrcStartAddr);
                rLPParam.u4SrcBufStart = PHYSICAL(rLPParam.u4SrcBufStart);
                rLPParam.u4SrcBufEnd = PHYSICAL(rLPParam.u4SrcBufEnd);
                rLPParam.u4DstStartAddr = PHYSICAL(rLPParam.u4DstStartAddr);
                rLPParam.u4DstBufStart = PHYSICAL(rLPParam.u4DstBufStart);
                rLPParam.u4DstBufEnd = PHYSICAL(rLPParam.u4DstBufEnd);
            }

            if(u4Handle != 0)
            {
                prGcpu = (GCPU_STRUCT_T *)u4Handle;
                LOG(7, "Use GCPU handler\n");
            }
            else
            {
                prGcpu = NULL;
            }

            if (!_Gcpu_IsAligned(rLPParam.u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLPParam.u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if ((!_GCPU_IsInRegion(rLPParam.u4SrcStartAddr, rLPParam.u4SrcBufStart,
                    rLPParam.u4SrcBufEnd)) ||
                (!_GCPU_IsInRegion(rLPParam.u4DstStartAddr, rLPParam.u4DstBufStart,
                    rLPParam.u4DstBufEnd)))
            {
                return E_GCPU_PARAM_WRONG;
            }

            // This is SW workaround of rev.A IC for gcpu address issue bit[31] = 1 && src start address [7:4] is not multiply of 4
            if((BSP_GetIcVersion() == IC_VER_5396_AA) || (BSP_GetIcVersion() == IC_VER_5368_AA))
            {
                // intend to use prLParam and virtual memory
                if(((prLParam->u4SrcStartAddr & 0x80000000) != 0) &&
                    (((prLParam->u4SrcStartAddr & 0xf0) >> 4) % 4 != 0))
                {
                    #ifndef CC_UBOOT
                    BOOL fgRet;
                    fgRet = _GCPU_AES_SW_Ring(prLParam);
                    return (fgRet == TRUE)?S_GCPU_OK:E_GCPU_UNEXPECT;
                    #else
                    ASSERT(0);
                    #endif
                }
            }

            // setup command
            switch(rLPParam.eMode)
            {
            case BLOCK_CIPHER_MODE_ECB:
                if(rLPParam.fgEncrypt)
                {
                    rHwCmd.u4Param[0] = AES_EPAK;
                }
                else
                {
                    rHwCmd.u4Param[0] = AES_DPAK;
                }
                break;
            case BLOCK_CIPHER_MODE_CTR:
                // the same operation for encryption and decryption
                rHwCmd.u4Param[0] = AES_CTR;
                break;
            case BLOCK_CIPHER_MODE_OFB:
                // the same operation for encryption and decryption
                rHwCmd.u4Param[0] = AES_OFB;
                break;
            case BLOCK_CIPHER_MODE_CBC:
                if(rLPParam.fgEncrypt)
                {
                    rHwCmd.u4Param[0] = AES_ECBC;
                }
                else
                {
                    rHwCmd.u4Param[0] = AES_DCBC;
                }
                break;

            default:
                return E_GCPU_UNEXPECT;
            }

            // setup parameter
            rHwCmd.u4Param[1] = rLPParam.u4SrcStartAddr;
            rHwCmd.u4Param[2] = rLPParam.u4DstStartAddr;
            if(rLPParam.eMode == BLOCK_CIPHER_MODE_CBC ||
                rLPParam.eMode == BLOCK_CIPHER_MODE_ECB)
            {
                rHwCmd.u4Param[3] = rLPParam.u4DatLen / 16;
            }
            else
            {
                rHwCmd.u4Param[3] = rLPParam.u4DatLen;
            }

            rHwCmd.u4Param[4] = AES_KEY_LEN_WRAPPER(rLPParam.eKeyBitLen);

            if(prGcpu != NULL)
            {
                if(prGcpu->u4AesKeyPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesKeyPtr))
                    {
                        LOG(7, "Use secure aes key ptr\n");
                        rHwCmd.u4Param[5] = prGcpu->u4AesKeyPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes key slot ptr\n");
                        rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes key slot ptr\n");
                    rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                }

                if(prGcpu->u4AesIvPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesIvPtr))
                    {
                        LOG(7, "Use secure aes iv ptr\n");
                        rHwCmd.u4Param[6] = prGcpu->u4AesIvPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes iv slot ptr\n");
                        rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes iv slot ptr\n");
                    rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                }

                if(prGcpu->u4AesRetIvPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesRetIvPtr))
                    {
                        LOG(7, "Use secure aes ret iv ptr\n");
                        rHwCmd.u4Param[7] = prGcpu->u4AesRetIvPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes ret iv slot ptr\n");
                        rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes ret iv slot ptr\n");
                    rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
                }
            }
            else
            {
                rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
            }

            rHwCmd.u4Param[8] = rLPParam.u4SrcBufStart;
            rHwCmd.u4Param[9] = rLPParam.u4SrcBufEnd;
            rHwCmd.u4Param[10] = rLPParam.u4DstBufStart;
            rHwCmd.u4Param[11] = rLPParam.u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
            {
                p = rLPParam.au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            //iv
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = rLPParam.au1Iv + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            if(fgIOMMU)
            {
                _GCPU_EnableIOMMU(rLPParam.u4SrcBufStart, rLPParam.u4SrcBufEnd, 0, 0);
            }
            else
            {
                _GCPU_DisableIOMMU();
            }

            HalFlushInvalidateDCache();

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalFlushInvalidateDCache();

            //return result
            if(prGcpu == NULL || (prGcpu != NULL && prGcpu->u4AesRetIvPtr == 0))
            {
                for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
                {
                    u4Tmp = rHwCmd.u4Param[GCPU_PARAM_AES_RET_IV_PTR + i];
                    prLParam->au1Iv[i * 4] = (u4Tmp & 0xff);
                    prLParam->au1Iv[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                    prLParam->au1Iv[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                    prLParam->au1Iv[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
                }
            }
        }
        break;
    case GCPU_AES_D:
    case GCPU_AES_E:
    case GCPU_AES_G:
        {
            SIM_AES_PARAM_T *prLParam = (SIM_AES_PARAM_T *)prParam;

            if(u4Id == GCPU_AES_D)
            {
                rHwCmd.u4Param[0] = AES_D;
            }
            else if(u4Id == GCPU_AES_E)
            {
                rHwCmd.u4Param[0] = AES_E;
            }
            else if(u4Id == GCPU_AES_G)
            {
                rHwCmd.u4Param[0] = AES_G;
            }

            // setup parameter
            rHwCmd.u4Param[1] = AES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);
            if(prLParam->u4SecureKeyPtr == 0)
            {
                rHwCmd.u4Param[2] = GCPU_PARAM_AES_KEY_PTR;
            }
            else
            {
                rHwCmd.u4Param[2] = prLParam->u4SecureKeyPtr;
            }
            rHwCmd.u4Param[3] = GCPU_PARAM_AES_IV_PTR;
            rHwCmd.u4Param[4] = GCPU_PARAM_RET_PTR;

            //key
            if(prLParam->u4SecureKeyPtr == 0)
            {
                for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
                {
                    p = prLParam->au1Key + i * 4;
                    rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
                }
            }

            //iv
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = prLParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
                prLParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_AES_D_CMP:
        {
            AES_DCMP_PARAM_T *prLParam = (AES_DCMP_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = AES_D_CMP;
            rHwCmd.u4Param[1] = AES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);
            rHwCmd.u4Param[2] = GCPU_PARAM_AES_KEY_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_AES_IV_PTR;
            rHwCmd.u4Param[4] = GCPU_PARAM_RET_PTR;

            // key
            for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            // data
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = prLParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            prLParam->u4CmpRes = rHwCmd.u4Param[5];
            prLParam->u4Value = rHwCmd.u4Param[6];

            //return result
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
                prLParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return S_GCPU_OK;
}


INT32 _GCPU_AES_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;
    GCPU_STRUCT_T *prGcpu;

    UNUSED(i4Ret);

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_AES:
        {
            AES_PARAM_T *prLParam = (AES_PARAM_T *)prParam;

            if(u4Handle != 0)
            {
                prGcpu = (GCPU_STRUCT_T *)u4Handle;
                LOG(7, "Use GCPU handler\n");
            }
            else
            {
                prGcpu = NULL;
            }

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if ((!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd)) ||
                (!_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd)))
            {
                return E_GCPU_PARAM_WRONG;
            }

            // This is SW workaround of rev.A IC for gcpu address issue bit[31] = 1 && src start address [7:4] is not multiply of 4
            if((BSP_GetIcVersion() == IC_VER_5396_AA) || (BSP_GetIcVersion() == IC_VER_5368_AA))
            {
                if(((prLParam->u4SrcStartAddr & 0x80000000) != 0) &&
                    (((prLParam->u4SrcStartAddr & 0xf0) >> 4) % 4 != 0))
                {
                    #ifndef CC_UBOOT
                    BOOL fgRet;
                    fgRet = _GCPU_AES_SW_Ring(prLParam);
                    return (fgRet == TRUE)?S_GCPU_OK:E_GCPU_UNEXPECT;
                    #else
                    ASSERT(0);
                    #endif
                }
            }

            // setup command
            switch(prLParam->eMode)
            {
            case BLOCK_CIPHER_MODE_ECB:
                if(prLParam->fgEncrypt)
                {
                    rHwCmd.u4Param[0] = AES_EPAK;
                }
                else
                {
                    rHwCmd.u4Param[0] = AES_DPAK;
                }
                break;
            case BLOCK_CIPHER_MODE_CTR:
                // the same operation for encryption and decryption
                rHwCmd.u4Param[0] = AES_CTR;
                break;
            case BLOCK_CIPHER_MODE_OFB:
                // the same operation for encryption and decryption
                rHwCmd.u4Param[0] = AES_OFB;
                break;
            case BLOCK_CIPHER_MODE_CBC:
                if(prLParam->fgEncrypt)
                {
                    rHwCmd.u4Param[0] = AES_ECBC;
                }
                else
                {
                    rHwCmd.u4Param[0] = AES_DCBC;
                }
                break;

            default:
                return E_GCPU_UNEXPECT;
            }

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            if(prLParam->eMode == BLOCK_CIPHER_MODE_CBC ||
                prLParam->eMode == BLOCK_CIPHER_MODE_ECB)
            {
                rHwCmd.u4Param[3] = prLParam->u4DatLen / 16;
            }
            else
            {
                rHwCmd.u4Param[3] = prLParam->u4DatLen;
            }

            rHwCmd.u4Param[4] = AES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);

            if(prGcpu != NULL)
            {
                if(prGcpu->u4AesKeyPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesKeyPtr))
                    {
                        LOG(7, "Use secure aes key ptr\n");
                        rHwCmd.u4Param[5] = prGcpu->u4AesKeyPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes key slot ptr\n");
                        rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes key slot ptr\n");
                    rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                }

                if(prGcpu->u4AesIvPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesIvPtr))
                    {
                        LOG(7, "Use secure aes iv ptr\n");
                        rHwCmd.u4Param[6] = prGcpu->u4AesIvPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes iv slot ptr\n");
                        rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes iv slot ptr\n");
                    rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                }

                if(prGcpu->u4AesRetIvPtr != 0)
                {
                    if(IsValidSecureSlot(prGcpu->u4AesRetIvPtr))
                    {
                        LOG(7, "Use secure aes ret iv ptr\n");
                        rHwCmd.u4Param[7] = prGcpu->u4AesRetIvPtr;
                    }
                    else
                    {
                        LOG(3, "Wrong seucre aes ret iv slot ptr\n");
                        rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
                    }
                }
                else
                {
                    LOG(5, "Use normal aes ret iv slot ptr\n");
                    rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
                }
            }
            else
            {
                rHwCmd.u4Param[5] = GCPU_PARAM_AES_KEY_PTR;
                rHwCmd.u4Param[6] = GCPU_PARAM_AES_IV_PTR;
                rHwCmd.u4Param[7] = GCPU_PARAM_AES_RET_IV_PTR;
            }

            rHwCmd.u4Param[8] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[9] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[10] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[11] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            //iv
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = prLParam->au1Iv + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);
#if 0
            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
#endif
            HalFlushInvalidateDCache();

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalFlushInvalidateDCache();
#if 0
            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
#endif
            //return result
            if(prGcpu == NULL || (prGcpu != NULL && prGcpu->u4AesRetIvPtr == 0))
            {
                for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
                {
                    u4Tmp = rHwCmd.u4Param[GCPU_PARAM_AES_RET_IV_PTR + i];
                    prLParam->au1Iv[i * 4] = (u4Tmp & 0xff);
                    prLParam->au1Iv[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                    prLParam->au1Iv[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                    prLParam->au1Iv[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
                }
            }
        }
        break;
    case GCPU_AES_D:
    case GCPU_AES_E:
    case GCPU_AES_G:
        {
            SIM_AES_PARAM_T *prLParam = (SIM_AES_PARAM_T *)prParam;

            if(u4Id == GCPU_AES_D)
            {
                rHwCmd.u4Param[0] = AES_D;
            }
            else if(u4Id == GCPU_AES_E)
            {
                rHwCmd.u4Param[0] = AES_E;
            }
            else if(u4Id == GCPU_AES_G)
            {
                rHwCmd.u4Param[0] = AES_G;
            }

            // setup parameter
            rHwCmd.u4Param[1] = AES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);
            if(prLParam->u4SecureKeyPtr == 0)
            {
                rHwCmd.u4Param[2] = GCPU_PARAM_AES_KEY_PTR;
            }
            else
            {
                rHwCmd.u4Param[2] = prLParam->u4SecureKeyPtr;
            }
            rHwCmd.u4Param[3] = GCPU_PARAM_AES_IV_PTR;
            rHwCmd.u4Param[4] = GCPU_PARAM_RET_PTR;

            //key
            if(prLParam->u4SecureKeyPtr == 0)
            {
                for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
                {
                    p = prLParam->au1Key + i * 4;
                    rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
                }
            }

            //iv
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = prLParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
                prLParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_AES_D_CMP:
        {
            AES_DCMP_PARAM_T *prLParam = (AES_DCMP_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = AES_D_CMP;
            rHwCmd.u4Param[1] = AES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);
            rHwCmd.u4Param[2] = GCPU_PARAM_AES_KEY_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_AES_IV_PTR;
            rHwCmd.u4Param[4] = GCPU_PARAM_RET_PTR;

            // key
            for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            // data
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                p = prLParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            prLParam->u4CmpRes = rHwCmd.u4Param[5];
            prLParam->u4Value = rHwCmd.u4Param[6];

            //return result
            for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
                prLParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return S_GCPU_OK;
}


INT32 _GCPU_AES_CmdSw(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    BOOL fgRet = TRUE;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_AES:
        {
            AES_PARAM_T *prLParam = (AES_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if ((!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd)) ||
                (!_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd)))
            {
                return E_GCPU_PARAM_WRONG;
            }

            #ifndef CC_UBOOT
            fgRet = _GCPU_AES_SW_Ring(prLParam);
            #endif
            i4Ret = (fgRet == TRUE)?S_GCPU_OK:E_GCPU_UNEXPECT;
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}

#ifndef CC_MTK_LOADER
INT32 _GCPU_DES_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    UINT8 i, *p;
    INT32 i4Ret;
    GCPU_HW_CMD_T rHwCmd;
    UINT32 u4Tmp;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_TDES:
        {
            DES_PARAM_T *prLParam = (DES_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            // setup command
            switch(prLParam->eMode)
            {
            case BLOCK_CIPHER_MODE_ECB:
                if(prLParam->fgEncrypt)
                {
                    rHwCmd.u4Param[0] = TDES_DMA_E;
                }
                else
                {
                    rHwCmd.u4Param[0] = TDES_DMA_D;
                }
                break;
            case BLOCK_CIPHER_MODE_CBC:
                if(prLParam->fgEncrypt)
                {
                    rHwCmd.u4Param[0] = TDES_CBC_E;
                }
                else
                {
                    rHwCmd.u4Param[0] = TDES_CBC_D;
                }
                break;

            default:
                return E_GCPU_UNEXPECT;
            }

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = prLParam->u4DatLen / 8;

            rHwCmd.u4Param[4] = DES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);
            rHwCmd.u4Param[5] = GCPU_PARAM_DES_KEY_PTR;
            p = prLParam->au1Iv;
            rHwCmd.u4Param[6] = KEY_WRAPPER(p);
            p = prLParam->au1Iv + 4;
            rHwCmd.u4Param[7] = KEY_WRAPPER(p);

            rHwCmd.u4Param[8] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[9] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[10] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[11] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_DES_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_DES_KEY_PTR + i] = KEY_WRAPPER(p);
                if(prLParam->eKeyBitLen == KEY_BIT_LEN_40)
                {
                    rHwCmd.u4Param[GCPU_PARAM_DES_KEY_PTR + i] &= 0xFF0FFF0F;
                }
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            //return result
            for(i = 0; i < GCPU_PARAM_DES_IV_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_DES_IV_PTR + i];
                prLParam->au1Iv[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Iv[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Iv[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Iv[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_TDES_D:
    case GCPU_TDES_E:
        {
            SIM_DES_PARAM_T *prLParam = (SIM_DES_PARAM_T *)prParam;

            if(u4Id == GCPU_TDES_D)
            {
                rHwCmd.u4Param[0] = TDES_D;
            }
            else if(u4Id == GCPU_TDES_E)
            {
                rHwCmd.u4Param[0] = TDES_E;
            }

            rHwCmd.u4Param[1] = DES_KEY_LEN_WRAPPER(prLParam->eKeyBitLen);
            rHwCmd.u4Param[2] = GCPU_PARAM_DES_KEY_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_DES_DAT_PTR;
            rHwCmd.u4Param[4] = GCPU_PARAM_DES_RET_PTR;

            //key
            for(i = 0; i < GCPU_PARAM_DES_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_DES_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            //data
            for(i = 0; i < 2; i++)
            {
                p = prLParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_DES_DAT_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < 2; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_DES_RET_PTR + i];
                prLParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }

        }
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}


INT32 _GCPU_CSS_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 u4Tmp;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    UNUSED(i4Ret);

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_CSS_DEC_DK:
        {
            CSS_DK_PARAM_T *prDkParam = (CSS_DK_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = CSS_DEC_DK;
            rHwCmd.u4Param[1] = KEY_WRAPPER(prDkParam->au1RDK);      //RDK [39:8]
            rHwCmd.u4Param[2] = ((prDkParam->au1RDK[4]) & 0xFF);    //RDK [7:0]
            rHwCmd.u4Param[3] = KEY_WRAPPER(prDkParam->au1EDK);      //EDK [39:8]
            rHwCmd.u4Param[4] = ((prDkParam->au1EDK[4]) & 0xFF);    //EDK [7:0]

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            u4Tmp = rHwCmd.u4Param[1];
            prDkParam->au1Res[0] = (u4Tmp & 0xff);
            prDkParam->au1Res[1] = ((u4Tmp >> 8) & 0xff);
            prDkParam->au1Res[2] = ((u4Tmp >> 16) & 0xff);
            prDkParam->au1Res[3] = ((u4Tmp >> 24) & 0xff);
        }
        break;
    case GCPU_CSS_DEC_TK:
        {
            CSS_TK_PARAM_T *prDkParam = (CSS_TK_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = CSS_DEC_TK;
            rHwCmd.u4Param[1] = KEY_WRAPPER(prDkParam->au1ETK);      //ETK [39:8]
            rHwCmd.u4Param[2] = ((prDkParam->au1ETK[4]) & 0xFF);    //ETK [7:0]

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);
        }
        break;
    case GCPU_CSS_DSC_AV:
        {
            CSS_AV_PARAM_T *prLParam = (CSS_AV_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = CSS_DSC_AV;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = (prLParam->u4DatLen + 2047) / 2048;
            // check size
            if(rHwCmd.u4Param[3] * 2048 > prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart ||
               rHwCmd.u4Param[3] * 2048 > prLParam->u4DstBufEnd - prLParam->u4DstBufStart )
            {
                return E_GCPU_PARAM_WRONG;
            }
            rHwCmd.u4Param[4] = prLParam->eMode;
            rHwCmd.u4Param[5] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[6] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[7] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[8] = prLParam->u4DstBufEnd;

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;

    case GCPU_CSS_AUTH_DRV:
    case GCPU_CSS_AUTH_DEC:
    case GCPU_CSS_AUTH_BK:
        {
            CSS_AUTH_PARAM_T *prAuthParam = (CSS_AUTH_PARAM_T *)prParam;

            if(u4Id == GCPU_CSS_AUTH_DRV)
            {
                rHwCmd.u4Param[0] = CSS_AUTH_DRV;
            }
            else if(u4Id == GCPU_CSS_AUTH_DEC)
            {
                rHwCmd.u4Param[0] = CSS_AUTH_DEC;
            }
            else if(u4Id == GCPU_CSS_AUTH_BK)
            {
                rHwCmd.u4Param[0] = CSS_AUTH_BK;
            }

            rHwCmd.u4Param[1] = KEY_WRAPPER(prAuthParam->au1CD);      //RDK [39:8]
            rHwCmd.u4Param[2] = ((prAuthParam->au1CD[4]) & 0xFF);    //RDK [7:0]
            p = prAuthParam->au1CD + 5;
            rHwCmd.u4Param[3] = KEY_WRAPPER(p);      //EDK [39:8]
            rHwCmd.u4Param[4] = ((prAuthParam->au1CD[9]) & 0xFF);    //EDK [7:0]
            rHwCmd.u4Param[5] = prAuthParam->u4ACC;

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            u4Tmp = rHwCmd.u4Param[1];
            prAuthParam->au1RD[0] = (u4Tmp & 0xff);
            prAuthParam->au1RD[1] = ((u4Tmp >> 8) & 0xff);
            prAuthParam->au1RD[2] = ((u4Tmp >> 16) & 0xff);
            prAuthParam->au1RD[3] = ((u4Tmp >> 24) & 0xff);
            prAuthParam->au1RD[4] = ((rHwCmd.u4Param[2]) & 0xff);
        }
        break;

    default:
        return E_GCPU_UNEXPECT;
    }

    return S_GCPU_OK;
}

INT32 _GCPU_CPRM_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_CPPM_DPAK:
    case GCPU_CPRM_DPAK:
    case GCPU_CPRM_EPAK:
        {
            CPRM_PARAM_T *prLParam = (CPRM_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_CPPM_DPAK)
            {
                rHwCmd.u4Param[0] = CPPM_DPAK;
            }
            else if(u4Id == GCPU_CPRM_DPAK)
            {
                rHwCmd.u4Param[0] = CPRM_DPAK;
            }
            else if(u4Id == GCPU_CPRM_EPAK)
            {
                rHwCmd.u4Param[0] = CPRM_EPAK;
            }

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = prLParam->u4DatLen / GCPU_CPRM_PACKET_SIZE;

            rHwCmd.u4Param[4] = GCPU_PARAM_CPRM_KEY_PTR;
            rHwCmd.u4Param[5] = prLParam->eMode;
            rHwCmd.u4Param[6] = prLParam->u4APSTB;
            rHwCmd.u4Param[7] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[8] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[9] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[10] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_CPRM_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_C2_H:
        {
            C2_HASH_PARAM_T *prLParam = (C2_HASH_PARAM_T *)prParam;
            UINT8 u1PadFlag = 0;
            static UINT8 *au1PadBuffer = NULL;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = C2_H;

            if (prLParam->fgFirstPacket)
            {
                rHwCmd.u4Param[3] = HASH_FIRST_PACKET_MODE;
                // seed
                for(i = 0; i < 2; i++)
                {
                    p = prLParam->au1Hash + i * 4;
                    rHwCmd.u4Param[4 + i] = 0;
                }
            }
            else
            {
                rHwCmd.u4Param[3] = HASH_SUCCESSIVE_PACKET_MODE;
                // seed
                for(i = 0; i < 2; i++)
                {
                    p = prLParam->au1Hash + i * 4;
                    rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
                }
            }

            //Padding Check
            if ((prLParam->fgFirstPacket == TRUE) && (prLParam->fgLastPacket == TRUE))
            {
                //One Packet Case
                u1PadFlag = 1;
            }
            else if (prLParam->fgLastPacket == TRUE)
            {
                //Last Packet Case
                u1PadFlag = 1;
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            if (u1PadFlag == 0)
            {
                //No Padding
                rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
                rHwCmd.u4Param[2] = prLParam->u4DatLen / GCPU_C2H_PACKET_SIZE;
                rHwCmd.u4Param[7] = prLParam->u4SrcBufStart;
                rHwCmd.u4Param[8] = prLParam->u4SrcBufEnd;

                HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

                i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

                HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            }
            else
            {
                UINT32 u4OptCnt, u4RemSize, u4RemBufOffset, u4PadBufSize;

                //Padding
                u4OptCnt = prLParam->u4DatLen / GCPU_C2H_PACKET_SIZE;
                u4RemSize = prLParam->u4DatLen - (u4OptCnt * GCPU_C2H_PACKET_SIZE);
                u4RemBufOffset = ADDR_INCR_IN_RING(prLParam->u4SrcStartAddr,
                    u4OptCnt * GCPU_C2H_PACKET_SIZE, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd);

                if (u4OptCnt)
                {
                    rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
                    rHwCmd.u4Param[2] = u4OptCnt;
                    rHwCmd.u4Param[7] = prLParam->u4SrcBufStart;
                    rHwCmd.u4Param[8] = prLParam->u4SrcBufEnd;

                    HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                        prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

                    i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

                    HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                        prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

                    // res
                    for(i = 0; i < 2; i++)
                    {
                        u4Tmp = rHwCmd.u4Param[4 + i];
                        prLParam->au1Hash[i * 4] = (u4Tmp & 0xff);
                        prLParam->au1Hash[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                        prLParam->au1Hash[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                        prLParam->au1Hash[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
                    }
                }

                //Padding SHA1
                u4PadBufSize = GCPU_C2H_PACKET_SIZE;

                if(au1PadBuffer == NULL)
                {
                    #ifndef CC_MTK_LOADER
                    au1PadBuffer = (UINT8 *)BSP_AllocAlignedDmaMemory(GCPU_C2H_PACKET_SIZE,
                        GCPU_FIFO_ALIGNMENT);
                    au1PadBuffer = (UINT8 *)VIRTUAL((UINT32)au1PadBuffer);
                    #else
                    au1PadBuffer = x_mem_alloc(GCPU_C2H_PACKET_SIZE);
                    #endif
                    if(au1PadBuffer == NULL)
                    {
                        return E_GCPU_NO_RES;
                    }
          	        HalInvalidateDCacheMultipleLine((UINT32)au1PadBuffer, GCPU_C2H_PACKET_SIZE);
                }

                x_memset(au1PadBuffer, 0x0, u4PadBufSize);
                _GCPU_CopyRingBuffer((UINT32)au1PadBuffer, (UINT32)au1PadBuffer,
                    (UINT32)au1PadBuffer + GCPU_C2H_PACKET_SIZE, u4RemBufOffset,
                    prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, u4RemSize);

                //Add one bit
                au1PadBuffer[u4RemSize] = 0x80;

                if (u4OptCnt)
                {
                    rHwCmd.u4Param[3] = HASH_SUCCESSIVE_PACKET_MODE;
                    for(i = 0; i < 2; i++)
                    {
                        p = prLParam->au1Hash + i * 4;
                        rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
                    }
                }

                rHwCmd.u4Param[1] = (UINT32)au1PadBuffer;
                rHwCmd.u4Param[2] = (UINT32)u4PadBufSize / GCPU_C2H_PACKET_SIZE;
                rHwCmd.u4Param[7] = (UINT32)au1PadBuffer;
                rHwCmd.u4Param[8] = (UINT32)au1PadBuffer + GCPU_C2H_PACKET_SIZE;

                _GCPU_EnableIOMMU((UINT32)au1PadBuffer,
                    (UINT32)au1PadBuffer + GCPU_C2H_PACKET_SIZE, 0, 0);

                HalDmaBufferStart((void *)au1PadBuffer,
                    GCPU_C2H_PACKET_SIZE, HAL_DMA_TO_DEVICE);

                i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);
                HalDmaBufferDone((void *)au1PadBuffer,
                    GCPU_C2H_PACKET_SIZE, HAL_DMA_TO_DEVICE);
            }

            // res
            for(i = 0; i < 2; i++)
            {
                u4Tmp = rHwCmd.u4Param[4 + i];
                prLParam->au1Hash[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Hash[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Hash[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Hash[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;

    case GCPU_CPRM_DCI_VFY:
        {
            CPRM_DCICCI_PARAM_T *prDciParam = (CPRM_DCICCI_PARAM_T *)prParam;
            rHwCmd.u4Param[0] = CPRM_DCI_VFY;
            rHwCmd.u4Param[1] = GCPU_PARAM_CPRM_KEY_PTR;
            rHwCmd.u4Param[2] = GCPU_PARAM_CPRM_DAT_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_CPRM_DAT_PTR + 2;
            rHwCmd.u4Param[4] = GCPU_PARAM_CPRM_RET_PTR;

            // key
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                p = prDciParam->au1KT + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_CPRM_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            // dci
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                p = prDciParam->au1DCI+ i * 4;
                rHwCmd.u4Param[GCPU_PARAM_CPRM_DAT_PTR + i] = KEY_WRAPPER(p);
            }

            // dtkc
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                p = prDciParam->au1DTKC + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_CPRM_DAT_PTR + 2 + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_CPRM_RET_PTR + i];
                prDciParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prDciParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prDciParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prDciParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_C2_D:
    case GCPU_C2_E:
    case GCPU_C2_G:
        {
            C2_PARAM_T *prC2Param = (C2_PARAM_T *)prParam;

            if(u4Id == GCPU_C2_D)
            {
                rHwCmd.u4Param[0] = C2_D;
            }
            else if(u4Id == GCPU_C2_E)
            {
                rHwCmd.u4Param[0] = C2_E;
            }
            else if(u4Id == GCPU_C2_G)
            {
                rHwCmd.u4Param[0] = C2_G;
            }

            rHwCmd.u4Param[1] = GCPU_PARAM_CPRM_KEY_PTR;
            rHwCmd.u4Param[2] = GCPU_PARAM_CPRM_DAT_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_CPRM_RET_PTR;

            // key
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                p = prC2Param->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_CPRM_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            // data
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                p = prC2Param->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_CPRM_DAT_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < GCPU_PARAM_CPRM_KEY_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_CPRM_RET_PTR + i];
                prC2Param->au1Res[i * 4] = (u4Tmp & 0xff);
                prC2Param->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prC2Param->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prC2Param->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;

    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}

INT32 _GCPU_VCPS_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_VCPS_HASH:
        {
            VCPS_HASH_PARAM_T *prHashParam = (VCPS_HASH_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = VCPS_H;

            // setup parameter
            rHwCmd.u4Param[1] = 0;
            rHwCmd.u4Param[2] = GCPU_PARAM_VPCS_KEY_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_VPCS_DAT_PTR;
            rHwCmd.u4Param[4] = GCPU_PARAM_VPCS_RET_PTR;

            //key
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                p = prHashParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_VPCS_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            //key
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                p = prHashParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_VPCS_DAT_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_VPCS_RET_PTR + i];
                prHashParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prHashParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prHashParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prHashParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_VCPS_DPAK:
    case GCPU_VCPS_EPAK:
        {
            VCPS_PARAM_T *prLParam = (VCPS_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_VCPS_DPAK)
            {
                rHwCmd.u4Param[0] = VCPS_DPAK;
            }
            else if(u4Id == GCPU_VCPS_EPAK)
            {
                rHwCmd.u4Param[0] = VCPS_EPAK;
            }

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = prLParam->u4DatLen / GCPU_VCPS_PACKET_SIZE;
            rHwCmd.u4Param[4] = prLParam->eMode;
            rHwCmd.u4Param[5] = GCPU_PARAM_VPCS_KEY_PTR;

            rHwCmd.u4Param[6] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[7] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[8] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[9] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_VPCS_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_VCPS_DKBH:
        {
            VCPS_DKB_HASH_PARAM_T *prLParam = (VCPS_DKB_HASH_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = VCPS_DKBH;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = (prLParam->u4DatLen + 15) / 16;
            rHwCmd.u4Param[3] = prLParam->eMode;
            // hash
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                p = prLParam->au1HashValue + i * 4;
                rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
            }

            rHwCmd.u4Param[8] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[9] = prLParam->u4SrcBufEnd;

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

            //return result
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                u4Tmp = rHwCmd.u4Param[4 + i];
                prLParam->au1HashValue[i * 4] = (u4Tmp & 0xff);
                prLParam->au1HashValue[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1HashValue[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1HashValue[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_VCPS_DCBC:
    case GCPU_VCPS_ECBC:
        {
            VCPS_AES_CBC_PARAM_T *prCBCParam = (VCPS_AES_CBC_PARAM_T *)prParam;

            if(u4Id == GCPU_VCPS_DCBC)
            {
                rHwCmd.u4Param[0] = VCPS_DCBC;
            }
            else
            {
                rHwCmd.u4Param[0] = VCPS_ECBC;
            }

            // data
            for(i = 0; i < 8; i++)
            {
                p = prCBCParam->au1Dat + i * 4;
                rHwCmd.u4Param[1 + i] = KEY_WRAPPER(p);
            }

            //key
            for(i = 0; i < GCPU_PARAM_VPCS_KEY_NUM; i++)
            {
                p = prCBCParam->au1Key + i * 4;
                rHwCmd.u4Param[9 + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < 8; i++)
            {
                u4Tmp = rHwCmd.u4Param[1 + i];
                prCBCParam->au1Dat[i * 4] = (u4Tmp & 0xff);
                prCBCParam->au1Dat[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prCBCParam->au1Dat[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prCBCParam->au1Dat[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_VCPS_DHDK:
        {
            VCPS_DHDK_PARAM_T *prLParam = (VCPS_DHDK_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = VCPS_DHDK;
            rHwCmd.u4Param[1] = prLParam->u4SecureDatPtr;
            rHwCmd.u4Param[2] = prLParam->u4SecureResPtr;

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);
        }
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}

INT32 _GCPU_AACS_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_AACS_DBD:
    case GCPU_AACS_EBD:
    case GCPU_AACS_DTN:
    case GCPU_AACS_ETN:
        {
            AACS_AV_PARAM_T *prLParam = (AACS_AV_PARAM_T *)prParam;
            UINT32 u4PacketSize = 0;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_AACS_DBD)
            {
                rHwCmd.u4Param[0] = AACS_DBD;
                u4PacketSize = GCPU_AACS_BDMV_PACKET_SIZE;
            }
            else if(u4Id == GCPU_AACS_EBD)
            {
                rHwCmd.u4Param[0] = AACS_EBD;
                u4PacketSize = GCPU_AACS_BDMV_PACKET_SIZE;
            }
            else if(u4Id == GCPU_AACS_DTN)
            {
                rHwCmd.u4Param[0] = AACS_DTN;
                u4PacketSize = GCPU_AACS_BDTHUMB_PACKET_SIZE;
            }
            else if(u4Id == GCPU_AACS_ETN)
            {
                rHwCmd.u4Param[0] = AACS_ETN;
                u4PacketSize = GCPU_AACS_BDTHUMB_PACKET_SIZE;
            }

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = (prLParam->u4DatLen + u4PacketSize - 1) / u4PacketSize;
            if(rHwCmd.u4Param[3] * u4PacketSize > prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart ||
               rHwCmd.u4Param[3] * u4PacketSize > prLParam->u4DstBufEnd - prLParam->u4DstBufStart )
            {
                return E_GCPU_PARAM_WRONG;
            }
            rHwCmd.u4Param[4] = GCPU_PARAM_AACS_KEY_PTR;
            rHwCmd.u4Param[5] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[6] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[7] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[8] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_AACS_KEY_NUM; i++)
            {
                p = prLParam->au1Uk + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AACS_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_AACS_DHD:
    case GCPU_AACS_EHD:
        {
            AACS_HDDVD_PARAM_T *prLParam = (AACS_HDDVD_PARAM_T *)prParam;
            UINT32 u4PacketSize;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_AACS_DHD)
            {
                rHwCmd.u4Param[0] = AACS_DHD;
            }
            else if(u4Id == GCPU_AACS_EHD)
            {
                rHwCmd.u4Param[0] = AACS_EHD;
            }

            u4PacketSize = GCPU_AACS_HDVD_PACKET_SIZE;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = (prLParam->u4DatLen + u4PacketSize - 1) / u4PacketSize;
            if(rHwCmd.u4Param[3] * u4PacketSize > prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart ||
               rHwCmd.u4Param[3] * u4PacketSize > prLParam->u4DstBufEnd - prLParam->u4DstBufStart )
            {
                return E_GCPU_PARAM_WRONG;
            }
            rHwCmd.u4Param[4] = DESCRAMBLE_MODE_ON;
            rHwCmd.u4Param[5] = GCPU_PARAM_AACS_KEY_PTR;

            // CPI
            for(i = 0; i < 3; i++)
            {
                p = prLParam->au1Cpi + i * 4;
                rHwCmd.u4Param[6 + i] = KEY_WRAPPER(p);
            }
            rHwCmd.u4Param[9] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[10] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[11] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[12] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_AACS_KEY_NUM; i++)
            {
                p = prLParam->au1Kt + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AACS_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_AACS_DV_CALC:
        {
            AACS_DVCALC_PARAM_T *prDvParam = (AACS_DVCALC_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = AACS_DV_CALC;

            rHwCmd.u4Param[1] = prDvParam->u4KmsiSecureSlotPtr;
            rHwCmd.u4Param[2] = GCPU_PARAM_AACS_KEY_PTR;
            rHwCmd.u4Param[3] = GCPU_PARAM_AACS_KEY_PTR + 4;
            rHwCmd.u4Param[4] = prDvParam->u4DvSecureSlotPtr;
            rHwCmd.u4Param[5] = prDvParam->u4Mode;

            // xfcr
            for(i = 0; i < 4; i++)
            {
                p = prDvParam->au1Xfcr + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AACS_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            // dkder
            for(i = 0; i < 4; i++)
            {
                p = prDvParam->au1Dkder + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_AACS_KEY_PTR + 4 + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            prDvParam->u4DVLSB10 = rHwCmd.u4Param[6];
            prDvParam->u4Cmp = rHwCmd.u4Param[7];
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}


INT32 _GCPU_BDRE_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_BDRE_DBD:
    case GCPU_BDRE_EBD:
    case GCPU_BDRE_DTN:
    case GCPU_BDRE_ETN:
        {
            BDRE_PARAM_T *prLParam = (BDRE_PARAM_T *)prParam;
            UINT32 u4PacketSize = 0;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_BDRE_DBD)
            {
                rHwCmd.u4Param[0] = BDRE_DBD;
                u4PacketSize = GCPU_BDRE_PACKET_SIZE;
            }
            else if(u4Id == GCPU_BDRE_EBD)
            {
                rHwCmd.u4Param[0] = BDRE_EBD;
                u4PacketSize = GCPU_BDRE_PACKET_SIZE;
            }
            else if(u4Id == GCPU_BDRE_DTN)
            {
                rHwCmd.u4Param[0] = BDRE_DTN;
                u4PacketSize = GCPU_BDRE_THUMB_PACKET_SIZE;
            }
            else if(u4Id == GCPU_BDRE_ETN)
            {
                rHwCmd.u4Param[0] = BDRE_ETN;
                u4PacketSize = GCPU_BDRE_THUMB_PACKET_SIZE;
            }

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = (prLParam->u4DatLen + u4PacketSize - 1) / u4PacketSize;
            if(rHwCmd.u4Param[3] * u4PacketSize > prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart ||
               rHwCmd.u4Param[3] * u4PacketSize > prLParam->u4DstBufEnd - prLParam->u4DstBufStart )
            {
                return E_GCPU_PARAM_WRONG;
            }
            rHwCmd.u4Param[4] = GCPU_PARAM_BDRE_KEY_PTR;
            rHwCmd.u4Param[5] = prLParam->u4Vbkg;
            rHwCmd.u4Param[6] = prLParam->eMode;
            rHwCmd.u4Param[7] = prLParam->u4Cci;
            rHwCmd.u4Param[8] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[9] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[10] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[11] = prLParam->u4DstBufEnd;

            //key
            for(i = 0; i < GCPU_PARAM_BDRE_KEY_NUM; i++)
            {
                p = prLParam->au1Krec + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_BDRE_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_BDRE_BE:
        {
            BDRE_BE_PARAM_T *prBeParam = (BDRE_BE_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = BDRE_BE;

            rHwCmd.u4Param[1] = GCPU_PARAM_BDRE_RET_PTR;

            // dat
            for(i = 0; i < 2; i++)
            {
                p = prBeParam->au1Dat + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_BDRE_RET_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < 2; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_BDRE_RET_PTR + i];
                prBeParam->au1Dat[i * 4] = (u4Tmp & 0xff);
                prBeParam->au1Dat[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prBeParam->au1Dat[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prBeParam->au1Dat[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}
#endif

INT32 _GCPU_MISC_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    INT32 i4Ret;
    UINT32 u4Tmp, i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_MEMCPY:
        {
            MEMCPY_PARAM_T *prLParam = (MEMCPY_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = MEMCPY;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[3] = prLParam->u4DatLen;
            rHwCmd.u4Param[4] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[5] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[6] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[7] = prLParam->u4DstBufEnd;

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_DMA:
        {
            DMA_PARAM_T *prLParam = (DMA_PARAM_T *)prParam;

            if (!_Gcpu_IsAligned(prLParam->u4DramBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DramBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4DramStartAddr, prLParam->u4DramBufStart,
                    prLParam->u4DramBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = DMA;

            rHwCmd.u4Param[1] = prLParam->u4DramStartAddr;
            rHwCmd.u4Param[2] = prLParam->u4SramStartAddr;
            rHwCmd.u4Param[3] = prLParam->u4DatLen;
            rHwCmd.u4Param[4] = prLParam->eMode;
            rHwCmd.u4Param[5] = prLParam->u4DramBufStart;
            rHwCmd.u4Param[6] = prLParam->u4DramBufEnd;

            _GCPU_EnableIOMMU(prLParam->u4DramBufStart, prLParam->u4DramBufEnd, 0, 0);

            // u4DramBufStart is possible a source or destination address,
            // so handle this buffer twice
            HalDmaBufferStart((void *)prLParam->u4DramBufStart,
                prLParam->u4DramBufEnd - prLParam->u4DramBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DramBufStart,
                prLParam->u4DramBufEnd - prLParam->u4DramBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4DramBufStart,
                prLParam->u4DramBufEnd - prLParam->u4DramBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DramBufStart,
                prLParam->u4DramBufEnd - prLParam->u4DramBufStart, HAL_DMA_FROM_DEVICE);
        }
        break;
    case GCPU_MEM_XOR:
        {
            XOR_PARAM_T *prLParam = (XOR_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = MEM_XOR;

            rHwCmd.u4Param[1] = GCPU_PARAM_RET_PTR;
            p = prLParam->au1Xor;
            rHwCmd.u4Param[2] = KEY_WRAPPER(p);

            // dat
            p = prLParam->au1Dat;
            rHwCmd.u4Param[GCPU_PARAM_RET_PTR] = KEY_WRAPPER(p);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR];
            prLParam->au1Dat[0] = (u4Tmp & 0xff);
            prLParam->au1Dat[1] = ((u4Tmp >> 8) & 0xff);
            prLParam->au1Dat[2] = ((u4Tmp >> 16) & 0xff);
            prLParam->au1Dat[3] = ((u4Tmp >> 24) & 0xff);
        }
        break;
    case GCPU_ROM_BIST:
        {
            ROBBIST_PARAM_T *prLParam = (ROBBIST_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = ROM_BIST;
            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            prLParam->u4Res = rHwCmd.u4Param[1];
        }
        break;
    case GCPU_RNG:
        {
            RNG_PARAM_T *prLParam = (RNG_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = RNG;
            rHwCmd.u4Param[1] = GCPU_PARAM_RET_PTR;

            // seed
            for(i = 0; i < 4; i++)
            {
                p = prLParam->au1Seed + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            // res
            for(i = 0; i < 4; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
                prLParam->au1Res[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_SHA_1:
    case GCPU_SHA_256:
    case GCPU_SHA_224:
    case GCPU_MD5:
        {
            MD_PARAM_T *prLParam = (MD_PARAM_T *)prParam;
            UINT64 u8DataLenBitCnt = 0;
            UINT8 u1PadFlag = 0;
            static UINT8 *au1PadBuffer = NULL;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if(u4Id == GCPU_SHA_1)
            {
                rHwCmd.u4Param[0] = SHA_1;
            }
            else if(u4Id == GCPU_SHA_224)
            {
                rHwCmd.u4Param[0] = SHA_224;
            }
            else if(u4Id == GCPU_SHA_256)
            {
                rHwCmd.u4Param[0] = SHA_256;
            }
            else if(u4Id == GCPU_MD5)
            {
                rHwCmd.u4Param[0] = MD5;
            }

            if (prLParam->fgFirstPacket)
            {
                rHwCmd.u4Param[3] = HASH_FIRST_PACKET_MODE;
                // seed
                for(i = 0; i < 8; i++)
                {
                    p = prLParam->au1Hash + i * 4;
                    rHwCmd.u4Param[4 + i] = 0;
                }
            }
            else
            {
                rHwCmd.u4Param[3] = HASH_SUCCESSIVE_PACKET_MODE;
                // seed
                for(i = 0; i < 8; i++)
                {
                    p = prLParam->au1Hash + i * 4;
                    rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
                }
            }

            //Padding Check
            if ((prLParam->fgFirstPacket == TRUE) && (prLParam->fgLastPacket == TRUE))
            {
                //One Packet Case
                u8DataLenBitCnt = (UINT64) (prLParam->u4DatLen * 8);    //Byte to Bit
                u1PadFlag = 1;
            }
            else if (prLParam->fgLastPacket == TRUE)
            {
                //Last Packet Case
                u8DataLenBitCnt = (UINT64) ((prLParam->u4DatLen * 8) + (prLParam->u8BitCnt));    //Byte to Bit
                u1PadFlag = 1;
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            if (u1PadFlag == 0)
            {
                //No Padding
                rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
                rHwCmd.u4Param[2] = prLParam->u4DatLen / GCPU_SHA_1_PACKET_SIZE;
                rHwCmd.u4Param[12] = prLParam->u4SrcBufStart;
                rHwCmd.u4Param[13] = prLParam->u4SrcBufEnd;

                HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

                i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

                HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            }
            else
            {
                UINT32 u4OptCnt, u4RemSize, u4RemBufOffset, u4PadOffset, u4PadBufSize, u4BitCnt;

                //Padding
                u4OptCnt = prLParam->u4DatLen/GCPU_SHA_1_PACKET_SIZE;
                u4RemSize = prLParam->u4DatLen - (u4OptCnt*GCPU_SHA_1_PACKET_SIZE);
                u4RemBufOffset = ADDR_INCR_IN_RING(prLParam->u4SrcStartAddr,
                    u4OptCnt * GCPU_SHA_1_PACKET_SIZE, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd);

                if (u4OptCnt)
                {
                    rHwCmd.u4Param[1] = prLParam->u4SrcStartAddr;
                    rHwCmd.u4Param[2] = u4OptCnt;
                    rHwCmd.u4Param[12] = prLParam->u4SrcBufStart;
                    rHwCmd.u4Param[13] = prLParam->u4SrcBufEnd;

                    HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                        prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

                    i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

                    HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                        prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);

                    // res
                    for(i = 0; i < 8; i++)
                    {
                        u4Tmp = rHwCmd.u4Param[4 + i];
                        prLParam->au1Hash[i * 4] = (u4Tmp & 0xff);
                        prLParam->au1Hash[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                        prLParam->au1Hash[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                        prLParam->au1Hash[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
                    }
                }

                //Padding SHA1
                if (u4RemSize + 9 > GCPU_SHA_1_PACKET_SIZE)
                {
                    u4PadOffset = 32;
                    u4PadBufSize = GCPU_SHA_1_PACKET_SIZE * 2;
                }
                else
                {
                    u4PadOffset = 16;
                    u4PadBufSize = GCPU_SHA_1_PACKET_SIZE;
                }

                if(au1PadBuffer == NULL)
                {
                    #ifndef CC_MTK_LOADER
                    au1PadBuffer = (UINT8 *)BSP_AllocAlignedDmaMemory(GCPU_SHA_1_PACKET_SIZE * 2,
                        GCPU_FIFO_ALIGNMENT);
                    au1PadBuffer = (UINT8 *)VIRTUAL((UINT32)au1PadBuffer);
                    #else
					// this is a workaround for GCPU address issue.
    	            static UINT8 *au1OrgBuffer = NULL;
                    au1OrgBuffer = x_mem_alloc(GCPU_SHA_1_PACKET_SIZE * 2 + 0x40);
                    au1PadBuffer = (UINT8 *)(((UINT32)au1OrgBuffer) + 0x40 - (((UINT32)au1OrgBuffer) % 0x40));
                    #endif
                    if(au1PadBuffer == NULL)
                    {
                        return E_GCPU_NO_RES;
                    }
          	        HalInvalidateDCacheMultipleLine((UINT32)au1PadBuffer, GCPU_SHA_1_PACKET_SIZE * 2);
                }

                x_memset(au1PadBuffer, 0x0, u4PadBufSize);
                _GCPU_CopyRingBuffer((UINT32)au1PadBuffer, (UINT32)au1PadBuffer,
                    (UINT32)(au1PadBuffer + GCPU_SHA_1_PACKET_SIZE * 2), u4RemBufOffset,
                    prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, u4RemSize);

                //Add one bit
                au1PadBuffer[u4RemSize] = 0x80;

                if(u4Id == GCPU_MD5)
                {
                    u4BitCnt = (UINT32) (((UINT32) (u8DataLenBitCnt & 0xFFFFFFFF)));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 8), (BYTE*)&u4BitCnt, 4);
                    u4BitCnt = (UINT32) (UINT32) (((UINT32) ((u8DataLenBitCnt >> 32) & 0xFFFFFFFF)));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 4), (BYTE*)&u4BitCnt, 4);
                }
                else
                {
                    u4BitCnt = (UINT32) (SwitchValue((UINT32) (u8DataLenBitCnt >> 32) & 0xFFFFFFFF));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 8), (BYTE*)&u4BitCnt, 4);
                    u4BitCnt = (UINT32) (SwitchValue((UINT32) (u8DataLenBitCnt & 0xFFFFFFFF)));
                    x_memcpy(au1PadBuffer + (u4PadOffset * 4 - 4), (BYTE*)&u4BitCnt, 4);
                }


                if (u4OptCnt)
                {
                    rHwCmd.u4Param[3] = HASH_SUCCESSIVE_PACKET_MODE;
                    for(i = 0; i < 8; i++)
                    {
                        p = prLParam->au1Hash + i * 4;
                        rHwCmd.u4Param[4 + i] = KEY_WRAPPER(p);
                    }
                }

                rHwCmd.u4Param[1] = (UINT32)au1PadBuffer;
                rHwCmd.u4Param[2] = (UINT32)u4PadBufSize / GCPU_SHA_1_PACKET_SIZE;
                rHwCmd.u4Param[12] = (UINT32)au1PadBuffer;
                rHwCmd.u4Param[13] = (UINT32)au1PadBuffer + GCPU_SHA_1_PACKET_SIZE * 2;

                HalDmaBufferStart((void *)au1PadBuffer,
                    GCPU_SHA_1_PACKET_SIZE * 2, HAL_DMA_TO_DEVICE);

                i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

                HalDmaBufferDone((void *)au1PadBuffer,
                    GCPU_SHA_1_PACKET_SIZE * 2, HAL_DMA_TO_DEVICE);
            }

            // res
            for(i = 0; i < 8; i++)
            {
                u4Tmp = rHwCmd.u4Param[4 + i];
                prLParam->au1Hash[i * 4] = (u4Tmp & 0xff);
                prLParam->au1Hash[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->au1Hash[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->au1Hash[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_RC4_KSA:
        {
            RC4_KEY_PARAM_T *prLParam = (RC4_KEY_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = RC4KSA;
            rHwCmd.u4Param[1] = GCPU_PARAM_RC4_KEY_PTR;
            rHwCmd.u4Param[2] = prLParam->u4KeyLen;
            rHwCmd.u4Param[3] = prLParam->u4keyAddr;
            rHwCmd.u4Param[4] = GCPU_PARAM_RC4_S_PTR;

            // seed
            for(i = 0; i < GCPU_PARAM_RC4_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_RC4_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);
        }
        break;

    case GCPU_RC4:
        {
            RC4_PARAM_T *prLParam = (RC4_PARAM_T *)prParam;
            UINT32 u4SecurePtr;

            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = RC4DPAK;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[2] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[3] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[4] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[5] = prLParam->u4DstBufEnd;
            rHwCmd.u4Param[6] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[7] = prLParam->u4DatLen;
            rHwCmd.u4Param[8] = prLParam->u4ResidualLen;
            rHwCmd.u4Param[9] = prLParam->u4keyAddr;
            rHwCmd.u4Param[10] = GCPU_PARAM_RC4_S_PTR;
            rHwCmd.u4Param[12] = GCPU_PARAM_RC4_KEY_PTR;
            rHwCmd.u4Param[13] = prLParam->u4InitIJ;
            rHwCmd.u4Param[14] = prLParam->au4ResidualBytes[0];
            rHwCmd.u4Param[15] = prLParam->au4ResidualBytes[1];

            // key
            for(i = 0; i < GCPU_PARAM_RC4_KEY_NUM; i++)
            {
                p = prLParam->au1Key + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_RC4_KEY_PTR + i] = KEY_WRAPPER(p);
            }

            // s array - 30
            for(i = 0; i < (48 - 16 - 2); i++)
            {
                p = ((UINT8 *)(prLParam->u4keyAddr)) + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_RC4_S_PTR + i] = KEY_WRAPPER(p);
            }

            u4SecurePtr = _GCPU_AllocSecureSlot((64 - 30) * 4);

            // s array - 34
            for(i = 30; i < 64; i++)
            {
                p = ((UINT8 *)(prLParam->u4keyAddr)) + i * 4;
                _GCPU_Hw_SetSlot(u4SecurePtr + i - 30, KEY_WRAPPER(p));
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            HalDmaBufferStart((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferStart((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalDmaBufferDone((void *)prLParam->u4SrcBufStart,
                prLParam->u4SrcBufEnd - prLParam->u4SrcBufStart, HAL_DMA_TO_DEVICE);
            HalDmaBufferDone((void *)prLParam->u4DstBufStart,
                prLParam->u4DstBufEnd - prLParam->u4DstBufStart, HAL_DMA_FROM_DEVICE);

            prLParam->u4ResidualLen = rHwCmd.u4Param[8];
            prLParam->u4InitIJ = rHwCmd.u4Param[13];
            prLParam->au4ResidualBytes[0] = rHwCmd.u4Param[14];
            prLParam->au4ResidualBytes[1] = rHwCmd.u4Param[15];

            _GCPU_FreeSecureSlot(u4SecurePtr);
        }
        break;
    case GCPU_PTX:
        {
            PTX_PARAM_T *prLParam = (PTX_PARAM_T *)prParam;

            rHwCmd.u4Param[0] = PTX;

            // setup parameter
            rHwCmd.u4Param[1] = (prLParam->u4CmdAddr >> 4);
            rHwCmd.u4Param[2] = prLParam->u4CmdNum;
            rHwCmd.u4Param[3] = GCPU_PARAM_PTR;
            rHwCmd.u4Param[4] = prLParam->u4IntBufStart;
            rHwCmd.u4Param[5] = prLParam->u4IntBufEnd;
            rHwCmd.u4Param[6] = prLParam->u4IntStartAddr;
            rHwCmd.u4Param[7] = prLParam->eMode;

            for(i = 0; i < 19; i++)
            {
                rHwCmd.u4Param[GCPU_PARAM_PTR + i] = prLParam->u4PicStruct[i];
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            prLParam->u4IntStartAddr = rHwCmd.u4Param[6];
            prLParam->u4PicNum = rHwCmd.u4Param[9];

            for(i = 0; i < 19; i++)
            {
                 prLParam->u4PicStruct[i] = rHwCmd.u4Param[GCPU_PARAM_PTR + i];
            }
        }
        break;
    case GCPU_TSDESC:
        {
            static UINT8 *pu1WorkBuf;
            TSDESC_PARAM_T *prLParam = (TSDESC_PARAM_T *)prParam;
            if (!_Gcpu_IsAligned(prLParam->u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(prLParam->u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(prLParam->u4SrcStartAddr, prLParam->u4SrcBufStart,
                    prLParam->u4SrcBufEnd) ||
                !_GCPU_IsInRegion(prLParam->u4DstStartAddr, prLParam->u4DstBufStart,
                    prLParam->u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = TS_DESC;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4SrcBufStart;
            rHwCmd.u4Param[2] = prLParam->u4SrcBufEnd;
            rHwCmd.u4Param[3] = prLParam->u4SrcStartAddr;
            rHwCmd.u4Param[4] = prLParam->u4DstBufStart;
            rHwCmd.u4Param[5] = prLParam->u4DstBufEnd;
            rHwCmd.u4Param[6] = prLParam->u4DstStartAddr;
            rHwCmd.u4Param[7] = prLParam->u4DatLen;

            rHwCmd.u4Param[8] = ((prLParam->eForm) & 0x1) | ((prLParam->eMode << 1) & 0x2);
            rHwCmd.u4Param[9] = 2;
            rHwCmd.u4Param[10] = GCPU_PARAM_PTR;

            if(pu1WorkBuf == NULL)
            {
                #ifndef CC_MTK_LOADER
                pu1WorkBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(GCPU_TSDESC_WORKBUF_SIZE,
                    GCPU_FIFO_ALIGNMENT);
                pu1WorkBuf = (UINT8 *)VIRTUAL((UINT32)pu1WorkBuf);
                #else
                pu1WorkBuf = x_mem_alloc(GCPU_TSDESC_WORKBUF_SIZE);
                #endif
                if(pu1WorkBuf == NULL)
                {
                    return E_GCPU_NO_RES;
                }
          	    HalInvalidateDCacheMultipleLine((UINT32)pu1WorkBuf, GCPU_TSDESC_WORKBUF_SIZE);
            }
            rHwCmd.u4Param[11] = (UINT32)pu1WorkBuf;
            rHwCmd.u4Param[12] = prLParam->u4TsState[0];
            rHwCmd.u4Param[13] = prLParam->u4TsState[1];
            rHwCmd.u4Param[14] = prLParam->u4TsState[2];
            rHwCmd.u4Param[15] = prLParam->u4TsState[3];

            // even key
            rHwCmd.u4Param[GCPU_PARAM_PTR + 0] = 0x80000047;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 1] = 0xC0000000;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 2] = (prLParam->eScheme << 28) | GCPU_PARAM_TSDESC_EVEN;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 3] = 0;

            for(i = 0; i < 4; i++)
            {
                p = prLParam->au1Key[0] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_EVEN + i] = KEY_WRAPPER(p);
            }

            for(i = 0; i < 4; i++)
            {
                p = prLParam->au1Iv[0] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_EVEN + 4 + i] = KEY_WRAPPER(p);
            }

            // odd key
            rHwCmd.u4Param[GCPU_PARAM_PTR + 4] = 0xC0000047;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 5] = 0xC0000000;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 6] = (prLParam->eScheme << 28) | GCPU_PARAM_TSDESC_ODD;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 7] = 0;

            for(i = 0; i < 4; i++)
            {
                p = prLParam->au1Key[1] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_ODD + i] = KEY_WRAPPER(p);
            }

            for(i = 0; i < 4; i++)
            {
                p = prLParam->au1Iv[1] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_ODD + 4 + i] = KEY_WRAPPER(p);
            }

            _GCPU_EnableIOMMU(prLParam->u4SrcBufStart, prLParam->u4SrcBufEnd, 0, 0);

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            prLParam->u4TsState[0] = rHwCmd.u4Param[12];
            prLParam->u4TsState[1] = rHwCmd.u4Param[13];
            prLParam->u4TsState[2] = rHwCmd.u4Param[14];
            prLParam->u4TsState[3] = rHwCmd.u4Param[15];
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}

INT32 _GCPU_MISC_CmdEx(UINT32 u4Handle, UINT32 u4Id, void* prParam, BOOL fgIOMMU)
{
    INT32 i4Ret;
    UINT32 i;
    GCPU_HW_CMD_T rHwCmd;
    UINT8 *p;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_TSDESC:
        {
            static UINT8 *pu1WorkBuf;
            TSDESC_PARAM_T *prLParam = (TSDESC_PARAM_T *)prParam;
            TSDESC_PARAM_T rLParam = *prLParam;

            if(!fgIOMMU)
            {
                rLParam.u4SrcBufStart = PHYSICAL(rLParam.u4SrcBufStart);
                rLParam.u4SrcBufEnd = PHYSICAL(rLParam.u4SrcBufEnd);
                rLParam.u4SrcStartAddr = PHYSICAL(rLParam.u4SrcStartAddr);
                rLParam.u4DstBufStart = PHYSICAL(rLParam.u4DstBufStart);
                rLParam.u4DstBufEnd = PHYSICAL(rLParam.u4DstBufEnd);
                rLParam.u4DstStartAddr = PHYSICAL(rLParam.u4DstStartAddr);
            }

            if (!_Gcpu_IsAligned(rLParam.u4SrcBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLParam.u4SrcBufEnd, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLParam.u4DstBufStart, GCPU_FIFO_ALIGNMENT) ||
                !_Gcpu_IsAligned(rLParam.u4DstBufEnd, GCPU_FIFO_ALIGNMENT))
            {
                return E_GCPU_PARAM_WRONG;
            }

            if (!_GCPU_IsInRegion(rLParam.u4SrcStartAddr, rLParam.u4SrcBufStart,
                    rLParam.u4SrcBufEnd) ||
                !_GCPU_IsInRegion(rLParam.u4DstStartAddr, rLParam.u4DstBufStart,
                    rLParam.u4DstBufEnd))
            {
                return E_GCPU_PARAM_WRONG;
            }

            rHwCmd.u4Param[0] = TS_DESC;

            // setup parameter
            rHwCmd.u4Param[1] = rLParam.u4SrcBufStart;
            rHwCmd.u4Param[2] = rLParam.u4SrcBufEnd;
            rHwCmd.u4Param[3] = rLParam.u4SrcStartAddr;
            rHwCmd.u4Param[4] = rLParam.u4DstBufStart;
            rHwCmd.u4Param[5] = rLParam.u4DstBufEnd;
            rHwCmd.u4Param[6] = rLParam.u4DstStartAddr;
            rHwCmd.u4Param[7] = rLParam.u4DatLen;

            rHwCmd.u4Param[8] = ((rLParam.eForm) & 0x1) | ((rLParam.eMode << 1) & 0x2);
            rHwCmd.u4Param[9] = 2;
            rHwCmd.u4Param[10] = GCPU_PARAM_PTR;

            if(pu1WorkBuf == NULL)
            {
                #ifndef CC_MTK_LOADER
                pu1WorkBuf = (UINT8 *)BSP_AllocAlignedDmaMemory(GCPU_TSDESC_WORKBUF_SIZE,
                    GCPU_FIFO_ALIGNMENT);
                pu1WorkBuf = (UINT8 *)VIRTUAL((UINT32)pu1WorkBuf);
                #else
                pu1WorkBuf = x_mem_alloc(GCPU_TSDESC_WORKBUF_SIZE);
                #endif
                if(pu1WorkBuf == NULL)
                {
                    return E_GCPU_NO_RES;
                }
          	    HalInvalidateDCacheMultipleLine((UINT32)pu1WorkBuf, GCPU_TSDESC_WORKBUF_SIZE);
            }
            rHwCmd.u4Param[11] = (UINT32)pu1WorkBuf;
            rHwCmd.u4Param[12] = prLParam->u4TsState[0];
            rHwCmd.u4Param[13] = prLParam->u4TsState[1];
            rHwCmd.u4Param[14] = prLParam->u4TsState[2];
            rHwCmd.u4Param[15] = prLParam->u4TsState[3];

            // even key
            rHwCmd.u4Param[GCPU_PARAM_PTR + 0] = 0x80000047;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 1] = 0xC0000000;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 2] = (rLParam.eScheme << 28) | GCPU_PARAM_TSDESC_EVEN;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 3] = 0;

            for(i = 0; i < 4; i++)
            {
                p = rLParam.au1Key[0] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_EVEN + i] = KEY_WRAPPER(p);
            }

            for(i = 0; i < 4; i++)
            {
                p = rLParam.au1Iv[0] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_EVEN + 4 + i] = KEY_WRAPPER(p);
            }

            // odd key
            rHwCmd.u4Param[GCPU_PARAM_PTR + 4] = 0xC0000047;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 5] = 0xC0000000;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 6] = (rLParam.eScheme << 28) | GCPU_PARAM_TSDESC_ODD;
            rHwCmd.u4Param[GCPU_PARAM_PTR + 7] = 0;

            for(i = 0; i < 4; i++)
            {
                p = rLParam.au1Key[1] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_ODD + i] = KEY_WRAPPER(p);
            }

            for(i = 0; i < 4; i++)
            {
                p = rLParam.au1Iv[1] + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_TSDESC_ODD + 4 + i] = KEY_WRAPPER(p);
            }

            if(fgIOMMU)
            {
                _GCPU_EnableIOMMU(rLParam.u4SrcBufStart, rLParam.u4SrcBufEnd, 0, 0);
            }
            else
            {
                _GCPU_DisableIOMMU();
            }

            HalFlushInvalidateDCache();

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            HalFlushInvalidateDCache();

            prLParam->u4TsState[0] = rHwCmd.u4Param[12];
            prLParam->u4TsState[1] = rHwCmd.u4Param[13];
            prLParam->u4TsState[2] = rHwCmd.u4Param[14];
            prLParam->u4TsState[3] = rHwCmd.u4Param[15];
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}


#ifndef CC_MTK_LOADER
//#ifdef CC_EMULATION
INT32 _GCPU_AES_Emu_Cmd(AES_EMU_PARAM_T* prAesParam)
{
    UINT8 i, *p;
    INT32 i4Ret;
    GCPU_HW_CMD_T rHwCmd;
    UINT32 u4Tmp;

    if(prAesParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(prAesParam->eMode)
    {
    case BLOCK_CIPHER_EMU_MODE_ECB:
        if(prAesParam->fgEncrypt)
        {
            rHwCmd.u4Param[0] = AES_E;
        }
        else
        {
            rHwCmd.u4Param[0] = AES_D;
        }
        break;

    case BLOCK_CIPHER_EMU_MODE_GEN:
        if(prAesParam->fgEncrypt)
        {
            return E_GCPU_UNEXPECT;
        }
        else
        {
            rHwCmd.u4Param[0] = AES_G;
        }
        break;

    case BLOCK_CIPHER_EMU_MODE_CMAC:

        rHwCmd.u4Param[0] = AES_CMAC;
        rHwCmd.u4Param[1] = prAesParam->u4SecureKeyPtr;
        rHwCmd.u4Param[2] = GCPU_PARAM_AES_IV_PTR;
        rHwCmd.u4Param[3] = GCPU_PARAM_RET_PTR;
        rHwCmd.u4Param[4] = prAesParam->eCmacMode;

        //iv
        for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
        {
            p = prAesParam->au1Dat + i * 4;
            rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
        }

        i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

        //return result
        for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
        {
            u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
            prAesParam->au1Res[i * 4] = (u4Tmp & 0xff);
            prAesParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
            prAesParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
            prAesParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
        }
        return i4Ret;
    default:
        return E_GCPU_UNEXPECT;
    }

    // setup parameter
    rHwCmd.u4Param[1] = AES_KEY_LEN_WRAPPER(prAesParam->eKeyBitLen);
    rHwCmd.u4Param[2] = GCPU_PARAM_AES_KEY_PTR;
    rHwCmd.u4Param[3] = GCPU_PARAM_AES_IV_PTR;
    rHwCmd.u4Param[4] = GCPU_PARAM_RET_PTR;

    //key
    for(i = 0; i < GCPU_PARAM_AES_KEY_NUM; i++)
    {
        p = prAesParam->au1Key + i * 4;
        rHwCmd.u4Param[GCPU_PARAM_AES_KEY_PTR + i] = KEY_WRAPPER(p);
    }

    //iv
    for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
    {
        p = prAesParam->au1Dat + i * 4;
        rHwCmd.u4Param[GCPU_PARAM_AES_IV_PTR + i] = KEY_WRAPPER(p);
    }

    i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

    //return result
    for(i = 0; i < GCPU_PARAM_AES_IV_NUM; i++)
    {
        u4Tmp = rHwCmd.u4Param[GCPU_PARAM_RET_PTR + i];
        prAesParam->au1Res[i * 4] = (u4Tmp & 0xff);
        prAesParam->au1Res[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
        prAesParam->au1Res[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
        prAesParam->au1Res[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
    }

    return i4Ret;
}


INT32 _GCPU_EF_Cmd(UINT32 u4Handle, UINT32 u4Id, void* prParam)
{
    UINT8 i, *p;
    INT32 i4Ret;
    GCPU_HW_CMD_T rHwCmd;
    GCPU_IF_PARAM_T *prLParam;
    UINT32 u4Tmp;

    if(prParam == NULL)
    {
        return E_GCPU_PARAM_WRONG;
    }

    prLParam = (GCPU_IF_PARAM_T *)prParam;

    if (prLParam->u4ADR >= 0xc000)      // the max sram address
    {
        return E_GCPU_PARAM_WRONG;
    }

    if (prLParam->u1DataLen / 4 > 44)   // the max slot availabe
    {
        return E_GCPU_PARAM_WRONG;
    }

    // setup command
    switch(u4Id)
    {
    case GCPU_IF_READ:
        {
            // setup command
            rHwCmd.u4Param[0] = GCPU_READ;

            // setup parameter
            rHwCmd.u4Param[1] = prLParam->u4ADR;
            rHwCmd.u4Param[2] = GCPU_PARAM_IF_PTR;
            rHwCmd.u4Param[3] = prLParam->u1DataLen / 4;        // in word

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);

            //return result
            for(i = 0; i < prLParam->u1DataLen / 4; i++)
            {
                u4Tmp = rHwCmd.u4Param[GCPU_PARAM_IF_PTR + i];
                prLParam->pu1Data[i * 4] = (u4Tmp & 0xff);
                prLParam->pu1Data[i * 4 + 1] = ((u4Tmp >> 8) & 0xff);
                prLParam->pu1Data[i * 4 + 2] = ((u4Tmp >> 16) & 0xff);
                prLParam->pu1Data[i * 4 + 3] = ((u4Tmp >> 24) & 0xff);
            }
        }
        break;
    case GCPU_IF_WRITE:
        {
            // setup command
            rHwCmd.u4Param[0] = GCPU_WRITE;

            // setup parameter
            rHwCmd.u4Param[1] = GCPU_PARAM_IF_PTR;
            rHwCmd.u4Param[2] = prLParam->u4ADR;
            rHwCmd.u4Param[3] = prLParam->u1DataLen / 4;        // in word

            for(i = 0; i < prLParam->u1DataLen / 4; i++)
            {
                p = ((UINT8 *)(prLParam->pu1Data)) + i * 4;
                rHwCmd.u4Param[GCPU_PARAM_IF_PTR + i] = KEY_WRAPPER(p);
            }

            i4Ret = _GCPU_Hw_CmdExe(&rHwCmd);
        }
        break;
    default:
        return E_GCPU_UNEXPECT;
    }

    return i4Ret;
}
#endif

INT32 _GCPU_Cmd(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam)
{
    switch(u4Cmd & 0x0000FF00)
    {
    case AES_CMD_SID:
        return _GCPU_AES_Cmd(u4Handle, u4Cmd, pvParam);
#ifndef CC_MTK_LOADER
    case CSS_CMD_SID:
        return _GCPU_CSS_Cmd(u4Handle, u4Cmd, pvParam);
    case CPPM_CMD_SID:
        return _GCPU_CPRM_Cmd(u4Handle, u4Cmd, pvParam);

    case VCPS_CMD_SID:
        return _GCPU_VCPS_Cmd(u4Handle, u4Cmd, pvParam);
    case AACS_CMD_SID:
        return _GCPU_AACS_Cmd(u4Handle, u4Cmd, pvParam);
    case TDES_CMD_SID:
        return _GCPU_DES_Cmd(u4Handle, u4Cmd, pvParam);
    case BDRE_CMD_SID:
        return _GCPU_BDRE_Cmd(u4Handle, u4Cmd, pvParam);
    case EFUSE_CMD_SID:
      return _GCPU_EF_Cmd(u4Handle, u4Cmd, pvParam);
#endif
    case MISC_CMD_SID:
        return _GCPU_MISC_Cmd(u4Handle, u4Cmd, pvParam);
    default:
        return E_GCPU_PARAM_WRONG;
    }
}

INT32 _GCPU_CmdEx(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam, BOOL fgIOMMU)
{
    switch(u4Cmd & 0x0000FF00)
    {
    case AES_CMD_SID:
        return _GCPU_AES_CmdEx(u4Handle, u4Cmd, pvParam, fgIOMMU);
    case MISC_CMD_SID:
        return _GCPU_MISC_CmdEx(u4Handle, u4Cmd, pvParam, fgIOMMU);
    default:
        return E_GCPU_PARAM_WRONG;
    }
}

INT32 _GCPU_CmdSw(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam)
{
    switch(u4Cmd & 0x0000FF00)
    {
    case AES_CMD_SID:
        return _GCPU_AES_CmdSw(u4Handle, u4Cmd, pvParam);
    default:
        return E_GCPU_PARAM_WRONG;
    }
}

UINT32 GCPU_CreateHandle()
{
    void *p;

    LOG(9, "Create GCPU handler.\n");
    p = x_mem_alloc(sizeof(GCPU_STRUCT_T));
    if(p == NULL)
    {
        return 0;
    }
    x_memset(p, 0, sizeof(GCPU_STRUCT_T));
    return (UINT32)p;
}

INT32 GCPU_ClostHandle(UINT32 u4Handle)
{
    if(u4Handle == 0)
    {
        return E_GCPU_PARAM_WRONG;
    }

    LOG(9, "Close GCPU handler.\n");
    x_mem_free((void *)u4Handle);

    return S_GCPU_OK;
}

