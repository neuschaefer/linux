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
 * $RCSfile: dmx_descrambler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_descrambler.c
 *  Demux driver - descrambler
 */

#ifdef __MODEL_slt__
#undef __MODEL_slt__//build error
#endif


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_debug.h"
#include "fvr.h"
#include "x_assert.h"
#include "x_os.h"
#include "dmx_drm_drvif.h"

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

//-----------------------------------------------------------------------------
/** _DMX_DMEM_CA_Init
 */
//-----------------------------------------------------------------------------
void _DMX_DMEM_CA_Init(void)
{
    x_memset((void*)DMX_DMEM_CA_KEY_BASE, 0, 96 * DMX_DMEM_CA_KEY_NUM);
    x_memset((void*)DMX_DMEM_CA_CTRL_BASE, 0, 4 * DMX_DMEM_CA_KEY_NUM);
}


//-----------------------------------------------------------------------------
/** _DMX_SetCA_Mode
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetCA_Mode(UINT8 u1Idx, DMX_CA_MODE_T eMode, DMX_CA_FEEDBACK_MODE_T eFBMode, BOOL fgEncrypt)
{
    UINT32 u4Ctrl;

    if(u1Idx >= DMX_DMEM_CA_KEY_NUM)
    {
        return FALSE;
    }

    u4Ctrl = DMX_DMEM_CA_CTRL(u1Idx);
    switch(eMode)
    {
    case DMX_CA_MODE_DES:
        u4Ctrl &= ~0xFF00FF4F;
        u4Ctrl |=  (((fgEncrypt ? 1 : 0) << 1) |       // Dec:0, Enc:1
                   (0 << 2) |                          // DES:0, 3DES:1
                   (1 << 27));
        break;

    case DMX_CA_MODE_3DES:
        u4Ctrl &= ~0xFF00FF4F;
        u4Ctrl |=  (((fgEncrypt ? 1 : 0) << 1) |       // Dec:0, Enc:1
                   (1 << 2)  |                         // DES:0, 3DES:1
                   (1 << 27));
        break;

    case DMX_CA_MODE_DVB:
        ASSERT(!fgEncrypt);
        u4Ctrl = 0x10000100;
        break;

    case DMX_CA_MODE_DVB_CONF:
        ASSERT(!fgEncrypt);
        u4Ctrl = 0x10000101;
        break;

    case DMX_CA_MODE_MULTI2_BIG:
    case DMX_CA_MODE_MULTI2_LITTLE:
        ASSERT(!fgEncrypt);
        u4Ctrl &= ~0xFF000000;
        u4Ctrl |= 0x20000100;
        break;

    case DMX_CA_MODE_AES:
        u4Ctrl &= ~0xFF00FF43;
        u4Ctrl |=  (((fgEncrypt ? 1 : 0) << 1) |       // Dec:0, Enc:1
                   (1 << 24));
        break;

    case DMX_CA_MODE_SMS4:
        u4Ctrl &= ~0xFF00FFFF;
        u4Ctrl |=  (((fgEncrypt ? 1 : 0) << 1) |
                   (1 << 30));
        break;

    case DMX_CA_MODE_NONE:
        break;

    default:
        return FALSE;
    }

    switch(eFBMode)
    {
    case DMX_CA_FB_ECB:
        break;

    case DMX_CA_FB_CBC:
        u4Ctrl |= 1;
        break;

    case DMX_CA_FB_CFB:
        u4Ctrl |= (3 |
                  (0xBC << 8) |                      // CFB
                  ((fgEncrypt ? 1 : 0) << 14));       // CFB encrypt or decrypt
        break;

    case DMX_CA_FB_OFB:
        u4Ctrl |= (3 |
                  (0xDC << 8) |                      // OFB
                  ((fgEncrypt ? 1 : 0) << 13));       // OFB encrypt or decrypt
        break;

    case DMX_CA_FB_CTR:
        u4Ctrl |= ((1 << 8) | (1 << 6));
        break;

    default:
        return FALSE;
    }

    DMX_DMEM_CA_CTRL(u1Idx) = u4Ctrl;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetCA_CTR
 */
//-----------------------------------------------------------------------------
void _DMX_SetCA_CTR(UINT8 u1Idx, UINT64 u8Nonce, UINT64 u8Counter, BOOL fgEven)
{
    if(u1Idx >= DMX_DMEM_CA_KEY_NUM)
    {
        return;
    }

    if(fgEven)
    {
        DMX_DMEM_CA_EVEN_IV(u1Idx, 0) = (UINT32)(u8Counter & 0xFFFFFFFF);
        DMX_DMEM_CA_EVEN_IV(u1Idx, 1) = (UINT32)((u8Counter >> 32) & 0xFFFFFFFF);
        DMX_DMEM_CA_EVEN_IV(u1Idx, 2) = (UINT32)(u8Nonce & 0xFFFFFFFF);
        DMX_DMEM_CA_EVEN_IV(u1Idx, 3) = (UINT32)((u8Nonce >> 32) & 0xFFFFFFFF);
    }
    else
    {
        DMX_DMEM_CA_ODD_IV(u1Idx, 0) = (UINT32)(u8Counter & 0xFFFFFFFF);
        DMX_DMEM_CA_ODD_IV(u1Idx, 1) = (UINT32)((u8Counter >> 32) & 0xFFFFFFFF);
        DMX_DMEM_CA_ODD_IV(u1Idx, 2) = (UINT32)(u8Nonce & 0xFFFFFFFF);
        DMX_DMEM_CA_ODD_IV(u1Idx, 3) = (UINT32)((u8Nonce >> 32) & 0xFFFFFFFF);
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetCTR_Rtb
 */
//-----------------------------------------------------------------------------
void _DMX_SetCTR_Rtb(UINT8 u1Idx, BOOL fgDoRtb)
{
    UINT32 u4Ctrl;

    u4Ctrl = DMX_DMEM_CA_CTRL(u1Idx);
    u4Ctrl &= ~0x180;
    if(fgDoRtb)
    {
        u4Ctrl |= 0x100;
    }

    DMX_DMEM_CA_CTRL(u1Idx) = u4Ctrl;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDesIVEx2
 *  Set initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetDesIVEx2(UINT8 u1Idx, UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven)
{
    if(u1Idx >= DMX_DMEM_CA_KEY_NUM)
    {
        return;
    }

    if(fgEven)
    {
        DMX_DMEM_CA_EVEN_IV(u1Idx, 2) = u4IvLo;
        DMX_DMEM_CA_EVEN_IV(u1Idx, 3) = u4IvHi;
    }
    else
    {
        DMX_DMEM_CA_ODD_IV(u1Idx, 2) = u4IvLo;
        DMX_DMEM_CA_ODD_IV(u1Idx, 3) = u4IvHi;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetDesKey
 *  Set a desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven, const UINT32 au4Key[6])
{
    UINT8 i;
    //UINT32 au4DesMap[6] = { 0, 1, 2, 3, 4, 5};
    //UINT32 au43DesMap[6] = { 0, 1, 2, 3, 4, 5};  //{ 4, 5, 2, 3, 0, 1};  // Swap key-1 and key-3
    //UINT32 u4MappedIndex;

    ASSERT(au4Key != NULL);

    if(u1Index >= DMX_DMEM_CA_KEY_NUM)
    {
        return FALSE;
    }

    for(i=0;i<6;i++)
    {
        //u4MappedIndex = fg3Des ? au43DesMap[i] : au4DesMap[i];
        if(fgEven)
        {
            DMX_DMEM_CA_EVEN_KEY(u1Index, 7 - i) = au4Key[i];
        }
        else
        {
            DMX_DMEM_CA_ODD_KEY(u1Index, 7 - i) = au4Key[i];
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDesRtbMode
 *  Set residual termination mode of DES
 *
 *  @param  DMX_DESC_RTB_MODE_T          CLEAR/CTS/SCTE-52
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
void _DMX_SetDesRtbMode(UINT8 u1Idx, DMX_DESC_RTB_MODE_T eMode)
{
    UINT32 u4Ctrl;

    if(u1Idx >= DMX_DMEM_CA_KEY_NUM)
    {
        return;
    }

    u4Ctrl = DMX_DMEM_CA_CTRL(u1Idx);
    u4Ctrl &= 0xFF00FFFF;

    switch(eMode)
    {
    case DMX_DESC_RTB_MODE_CTS:
        u4Ctrl |= (1<<17);
        break;

    case DMX_DESC_RTB_MODE_SCTE52:
        u4Ctrl |= (1<<16);
        break;

    case DMX_DESC_RTB_MODE_CLEAR:
    default:
        break;
    }

    DMX_DMEM_CA_CTRL(u1Idx) = u4Ctrl;
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesIV
 */
//-----------------------------------------------------------------------------
void _DMX_SetAesIVEx2(UINT8 u1Idx, const UINT32 au4Iv[4], BOOL fgEven)
{
    UINT8 i;

    if(u1Idx >= DMX_DMEM_CA_KEY_NUM)
    {
        return;
    }

    for(i=0;i<4;i++)
    {
        if(fgEven)
        {
            DMX_DMEM_CA_EVEN_IV(u1Idx, 3 - i) = au4Iv[i];
        }
        else
        {
            DMX_DMEM_CA_ODD_IV(u1Idx, 3 - i) = au4Iv[i];
        }
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesKeyLen
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetAesKeyLen(UINT8 u1Idx, UINT32 u4KeyLen)
{
    UINT32 u4Ctrl;

    if(u1Idx >= DMX_DMEM_CA_KEY_NUM)
    {
        return FALSE;
    }

    if((u4KeyLen != 128) && (u4KeyLen != 192) && (u4KeyLen != 256))
    {
        return FALSE;
    }

    u4Ctrl = DMX_DMEM_CA_CTRL(u1Idx);
    u4Ctrl &= ~0xC;

    if(u4KeyLen == 128)
    {
        u4Ctrl |= 0x0;
    }
    else if(u4KeyLen == 192)
    {
        u4Ctrl |= 0x4;
    }
    else if(u4KeyLen == 256)
    {
        u4Ctrl |= 0x8;
    }

    DMX_DMEM_CA_CTRL(u1Idx) = u4Ctrl;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesKey
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8])
{
    UINT8 i;

    ASSERT(au4Key != NULL);

    if(u1Index >= DMX_DMEM_CA_KEY_NUM)
    {
        return FALSE;
    }

    for(i=0;i<8;i++)
    {
        if(fgEven)
        {
            DMX_DMEM_CA_EVEN_KEY(u1Index, 7 - i) = au4Key[i];
        }
        else
        {
            DMX_DMEM_CA_ODD_KEY(u1Index, 7 - i) = au4Key[i];
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetSMS4Key
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetSMS4Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[4])
{
    UINT8 i;

    ASSERT(au4Key != NULL);

    if(u1Index >= DMX_DMEM_CA_KEY_NUM)
    {
        return FALSE;
    }

    for(i=0;i<4;i++)
    {
        if(fgEven)
        {
            DMX_DMEM_CA_EVEN_KEY(u1Index, 7 - i) = au4Key[i];
        }
        else
        {
            DMX_DMEM_CA_ODD_KEY(u1Index, 7 - i) = au4Key[i];
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetSMS4IV
 */
//-----------------------------------------------------------------------------
void _DMX_SetSMS4IV(UINT8 u1Idx, const UINT32 au4Iv[4], BOOL fgEven)
{
    _DMX_SetAesIVEx2(u1Idx, au4Iv, fgEven);
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesRtbMode
 */
//-----------------------------------------------------------------------------
void _DMX_SetAesRtbMode(UINT8 u1Idx, DMX_DESC_RTB_MODE_T eMode)
{
    _DMX_SetDesRtbMode(u1Idx, eMode);
}


#ifdef CC_DMX_SUPPORT_MULTI2
//-----------------------------------------------------------------------------
/** _DMX_SetMulti2Key
 *  Set a Multi2 desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2])
{
    UINT8 i;

    ASSERT(au4Key != NULL);

    if(u1Index >= DMX_DMEM_CA_KEY_NUM)
    {
        return FALSE;
    }

    for(i=0;i<2;i++)
    {
        if(fgEven)
        {
            DMX_DMEM_CA_KEY(u1Index, 9 - i) = au4Key[i];
        }
        else
        {
            DMX_DMEM_CA_KEY(u1Index, 21 - i) = au4Key[i];
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2CbcIV
 *  Set initial vector of MULTI-2 CBC mode
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2CbcIV(UINT8 u1Index,UINT32 u4IvHi, UINT32 u4IvLo)
{
    DMX_DMEM_CA_KEY(u1Index, 10) = u4IvLo;        // Even
    DMX_DMEM_CA_KEY(u1Index, 11) = u4IvHi;        // Even
    DMX_DMEM_CA_KEY(u1Index, 22) = u4IvLo;        // Odd
    DMX_DMEM_CA_KEY(u1Index, 23) = u4IvHi;        // Odd
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2OfbIV
 *  Set initial vector of MULTI-2 OFB mode
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2OfbIV(UINT8 u1Index,UINT32 u4IvHi, UINT32 u4IvLo)
{
    // Not test yet, org
	//DMXCMD_WRITE32(DMX_REG_M2_OFB_IV_LO, u4IvLo);
	//DMXCMD_WRITE32(DMX_REG_M2_OFB_IV_HI, u4IvHi);

    // 5368 new
    DMX_DMEM_CA_KEY(u1Index, 10) = u4IvLo;        // Even
    DMX_DMEM_CA_KEY(u1Index, 11) = u4IvHi;        // Even
    DMX_DMEM_CA_KEY(u1Index, 22) = u4IvLo;        // Odd
    DMX_DMEM_CA_KEY(u1Index, 23) = u4IvHi;        // Odd
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2SysKey
 *  Set system keys of MULTI-2
 *
 *  @param  au4Key          The system keys
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2SysKey(UINT8 u1Idx, const UINT32 au4Key[8])
{
	UINT32 i;

    ASSERT(au4Key != NULL);

	for (i = 0; i < 8; i++)
	{
	    DMX_DMEM_CA_KEY(u1Idx, i) = au4Key[i];      // Even
	    DMX_DMEM_CA_KEY(u1Idx, 12 + i) = au4Key[i];     // Odd
	}
}


//-----------------------------------------------------------------------------
/** _DMX_SetMulti2Config
 *  Set MULTI-2 configurations
 *
 *  @param  fgOfbFreeRun    Is OFB free-running mode
 *  @param  u2Iteration     Number of iterations
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetMulti2Config(UINT8 u1Index,BOOL fgOfbFreeRun, UINT16 u2Iteration)
{
    UINT32 u4Ctrl;
	UINT8 u1OfbFreeRun;

	u1OfbFreeRun = (UINT8)(fgOfbFreeRun ? 1 : 0);

	//DMXCMD_WRITE32(DMX_REG_M2_CFG, (u1OfbFreeRun << 16) | u2Iteration);

	u4Ctrl = DMX_DMEM_CA_CTRL(u1Index) & 0xFF000000;
	u4Ctrl |= ((u1OfbFreeRun << 15) | (UINT32)u2Iteration);     // ????
	DMX_DMEM_CA_CTRL(u1Index) = u4Ctrl;
}
#endif  // CC_DMX_SUPPORT_MULTI2


//#ifdef CC_VUDU_SUPPORT
const UINT8 au1CommonSecret[] = {0x03, 0x00, 0x5e, 0xe1, 0x0e, 0xe1, 0x91, 0x37,
                              0x0c, 0x51, 0x80, 0x27, 0x0c, 0xe1, 0x80, 0x37};


static BOOL _VuduUnwrapKey(const UINT8 * pu1UniqueKey, UINT32 u4Len,
    const UINT8 *pu1Wrapped, UINT8 *pu1Unwrapped)
{
    DES3_KS ks;

    // unwrap unique key using common key into key A
    UINT8 keyA[16];
    UINT8 keyBuf[24];
    UINT8 au1Temp[32];
    UINT32 i;

    if (u4Len > 16)
    {
        return FALSE;
    }

    ASSERT(pu1UniqueKey != NULL);

    x_memcpy(keyBuf, au1CommonSecret+8, 8);
    x_memcpy(keyBuf+8, au1CommonSecret, 8);
    x_memcpy(keyBuf+16, au1CommonSecret+8, 8);
    x_memcpy(au1Temp, pu1UniqueKey, 16);
    for (i = 0; i < 2; i++)
    {
        des3key(ks, keyBuf, 0);
        des3(ks, au1Temp + i * 8);
    }
    x_memcpy(keyA, au1Temp, 16);

#if defined(CC_EMULATION) || defined(__MODEL_slt__)
    LOG(3, "unique key:\n");
    _DumpBuffer(pu1UniqueKey, 16, 16);
    LOG(3, "key A:\n");
    _DumpBuffer(keyA, 16, 16);
#endif

    x_memcpy(keyBuf, keyA+8, 8);
    x_memcpy(keyBuf+8, keyA, 8);
    x_memcpy(keyBuf+16, keyA+8, 8);

    x_memcpy(au1Temp, pu1Wrapped, u4Len);
    // unwrap content key with key A
    for (i = 0; i < u4Len/8; i++) {
        des3key(ks, keyBuf, 0);
        des3(ks, au1Temp + i * 8);
    }
    x_memcpy(pu1Unwrapped, au1Temp, u4Len);

#if defined(CC_EMULATION) || defined(__MODEL_slt__)
    LOG(3, "wrapped:\n");
    _DumpBuffer(pu1Wrapped, u4Len, 16);
    LOG(3, "unwrapped:\n");
    _DumpBuffer(pu1Unwrapped, u4Len, 16);
#endif

    return TRUE;
}


BOOL _DMX_VUDU_SetKey(DMX_VUDU_KEY_T *prVuduKey)
{
    BOOL fgRet = FALSE;
    UINT32 i, j;
    UINT8* p;

    // clear key and clear IV
    UINT8 au1KeyC[32] = {};
    UINT8 au1IvC[16] = {};

    // key and iv for register
    UINT32 au4Iv[4];
    UINT32 au4Key[8];

    if(!((prVuduKey->u1KeyLen <= 16) &&  (prVuduKey->u1IvLen <= 16)))
    {
        return FALSE;
    }

    if(prVuduKey->u1Index >= 8)
    {
        return FALSE;
    }

    if(prVuduKey->fgWrappedKey)
    {
        if (!_VuduUnwrapKey(prVuduKey->pu1UniqueKey, prVuduKey->u1KeyLen,
            prVuduKey->pu1Key, au1KeyC)) {
            LOG(3, "Failed to unwrap key");
            return fgRet;
        }
    }
    else
    {
        x_memcpy(au1KeyC, prVuduKey->pu1Key, prVuduKey->u1KeyLen);
    }

    if(prVuduKey->fgWarppedIV)
    {
        if (!_VuduUnwrapKey(prVuduKey->pu1UniqueKey, prVuduKey->u1IvLen,
            prVuduKey->pu1IV, au1IvC)) {
            LOG(3, "Failed to unwrap key");
            return fgRet;
        }
    }
    else
    {
        x_memcpy(au1IvC, prVuduKey->pu1IV, prVuduKey->u1IvLen);
    }

    if(prVuduKey->eMode >= DMX_DESC_MODE_DES_ECB &&
        prVuduKey->eMode <= DMX_DESC_MODE_3DES_CBC)
    {
        // We don't support DES/3DES now
        ASSERT(0);

        au4Iv[0] = (au1IvC[0] << 24) | (au1IvC[1] << 16) | (au1IvC[2] << 8) | au1IvC[3];
        au4Iv[1] = (au1IvC[4] << 24) | (au1IvC[5] << 16) | (au1IvC[6] << 8) | au1IvC[7];
        _DMX_SetDesIVEx2(prVuduKey->u1Index, au4Iv[0] , au4Iv[1], prVuduKey->fgEven);


        for (i = 0; i < 6; i++)
        {
            p = au1KeyC + (i * 4);
            au4Key[i] = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
        }

        _DMX_SetDesKey(prVuduKey->u1Index,
            ((prVuduKey->eMode == DMX_DESC_MODE_3DES_ECB ||
            prVuduKey->eMode == DMX_DESC_MODE_3DES_CBC)?TRUE:FALSE),
            prVuduKey->fgEven, au4Key);

        _DMX_SetDesRtbMode(prVuduKey->u1Index, prVuduKey->eRtbMode);
    }
    else if(prVuduKey->eMode == DMX_DESC_MODE_AES_ECB ||
        prVuduKey->eMode == DMX_DESC_MODE_AES_CBC)
    {
        for (i = 0; i < 4; i++)
        {
            au4Iv[i] = 0;

            for (j = 0; j < 4; j++)
            {
                au4Iv[i] |= (au1IvC[(i * 4) + j] << ((3 - j) * 8));
            }
        }
        _DMX_SetAesIVEx2(prVuduKey->u1Index, au4Iv, prVuduKey->fgEven);
        _DMX_SetAesKeyLen(prVuduKey->u1Index, (UINT32)(prVuduKey->u1KeyLen * 8));

        for (i = 0; i < 8; i++)
        {
            p = au1KeyC + (i * 4);
            au4Key[i] = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
        }
        _DMX_SetAesKey(prVuduKey->u1Index, prVuduKey->fgEven, au4Key);
        _DMX_SetAesRtbMode(prVuduKey->u1Index, prVuduKey->eRtbMode);
    }

    fgRet = TRUE;

    return fgRet;
}
//#endif //  CC_VUDU_SUPPORT


//-----------------------------------------------------------------------------
/** _DMX_SetCaOutputBuffer
 *  Setup descrambler output buffer
 *
 *  @param  fgEnable        Enable or disable output buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetCaOutputBuffer(BOOL fgEnable, BOOL fgIsPs, UINT32 u4BufStart,
    UINT32 u4BufEnd, UINT32 u4Threshold)
{
	UINT32 u4Ctrl, u4BufSize;
	UINT8 u1IsPs;

    if (fgEnable)
    {
        // Check if buffer is correctly aligned
        //if (!_DMX_IsAligned(u4BufStart, DMX_CA_BUFFER_ALIGNMENT) ||
        //    !_DMX_IsAligned(u4BufEnd, DMX_CA_BUFFER_ALIGNMENT))
        //{
        //    return FALSE;
        //}

        // Check if buffer size and threshold are adequate
        u4BufSize = u4BufEnd - u4BufStart;
        if ((u4BufSize == 0) || (u4BufSize < u4Threshold))
        {
            return FALSE;
        }

        // Setup buffer
    	DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_START, u4BufStart);
    	DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_END, u4BufEnd - 1);
    	DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_RP, u4BufStart);
    	DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_WP, u4BufStart);

        // Enable output buffer
    	u1IsPs = (UINT8)(fgIsPs ? 1 : 0);
    	u4Ctrl = DMXCMD_READ32(DMX_REG_CA_CTRL);
    	u4Ctrl &= ~(1 << 1);
    	u4Ctrl |= (u1IsPs << 1) | (1 << 7);
    	DMXCMD_WRITE32(DMX_REG_CA_CTRL, u4Ctrl);

        // Set interrupt threshold
    	DMXCMD_WRITE32(DMX_REG_CA_OUT_BUF_THRESHOLD, u4Threshold);

    	// Enable interrupt for output buffer
    	DMXCMD_REG32(DMX_REG_CA_CTRL) |= (1 << 15);
    }
    else
    {
        // Disable output buffer and its interrupt
        u4Ctrl = DMXCMD_REG32(DMX_REG_CA_CTRL);
		u4Ctrl &= ~((1 << 7) | (1 << 15));
		DMXCMD_WRITE32(DMX_REG_CA_CTRL, u4Ctrl); 
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetCaInputBuffer
 *  Setup descrambler input buffer
 *
 *  @param  fgEnable        Enable or disable input buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetCaInputBuffer(BOOL fgEnable, BOOL fgIsPs, UINT32 u4BufStart,
    UINT32 u4BufEnd, UINT32 u4Threshold)
{
	UINT32 u4Ctrl, u4BufSize;
	UINT8 u1IsPs;

    if (fgEnable)
    {
        // Check if buffer is correctly aligned
        if (!_DMX_IsAligned(u4BufStart, DMX_CA_BUFFER_ALIGNMENT) ||
            !_DMX_IsAligned(u4BufEnd, DMX_CA_BUFFER_ALIGNMENT))
        {
            return FALSE;
        }

        // Check if buffer size and threshold are adequate
        u4BufSize = u4BufEnd - u4BufStart;
        if ((u4BufSize == 0) || (u4BufSize < u4Threshold))
        {
            return FALSE;
        }

        // Setup buffer
    	DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_START, u4BufStart);
    	DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_END, u4BufEnd - 1);
    	DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_RP, u4BufStart);
    	DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_WP, u4BufStart);

        // Set interrupt threshold
    	DMXCMD_WRITE32(DMX_REG_CA_IN_BUF_THRESHOLD, u4Threshold);

        // Enable input buffer
    	u1IsPs = (UINT8)(fgIsPs ? 1 : 0);
    	u4Ctrl = DMXCMD_READ32(DMX_REG_CA_CTRL);
    	u4Ctrl &= ~(1 << 0);
    	u4Ctrl |= (u1IsPs << 0) | (1 << 6);
    	DMXCMD_WRITE32(DMX_REG_CA_CTRL, u4Ctrl);

    	// Enable interrupt for input buffer
    	DMXCMD_REG32(DMX_REG_CA_CTRL) |= (1 << 14);
    }
    else
    {
        // Disable intput buffer and its interrupt
        DMXCMD_REG32(DMX_REG_CA_CTRL) &= ~((1 << 6) | (1 << 14));
    }

    return TRUE;
}


