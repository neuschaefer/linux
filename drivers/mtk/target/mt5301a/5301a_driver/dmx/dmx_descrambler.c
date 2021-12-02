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

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "fvr.h"
#include "x_assert.h"


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
/** _DMX_SetDesIV
 *  Set initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo)
{
	DMXCMD_WRITE32(DMX_REG_DES_IV_LO, u4IvLo);
	DMXCMD_WRITE32(DMX_REG_DES_IV_HI, u4IvHi);
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
BOOL _DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
    const UINT32 au4Key[6])
{
    UINT32 i;
    UINT32* pu4CaKeys;
    UINT32 au4DesMap[6] = { 0, 1, 2, 3, 4, 5};
    UINT32 au43DesMap[6] = { 4, 5, 2, 3, 0, 1};  // Swap key-1 and key-3

    ASSERT(au4Key != NULL);

    _DMX_Lock();

    if (fgEven)
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_EVEN_KEY;
    }
    else
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_ODD_KEY;
    }

    for (i = 0; i < 6; i++)
    {
        UINT32 u4MappedIndex;

        u4MappedIndex = fg3Des ? au43DesMap[i] : au4DesMap[i];
        pu4CaKeys[(u1Index * 8) + i] = au4Key[u4MappedIndex];
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesIV
 */
//-----------------------------------------------------------------------------
void _DMX_SetAesIV(const UINT32 au4Iv[4])
{
    UINT32 i;

    if(au4Iv != NULL)
    {
        for (i = 0; i < 4; i++)
        {
            DMXCMD_WRITE32(DMX_REG_AES_IV_0 + (3 - i), au4Iv[i]);
        }
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesKeyLen
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetAesKeyLen(UINT32 u4KeyLen)
{
    UINT32 u4Ctrl;

    if((u4KeyLen != 128) && (u4KeyLen != 192) && (u4KeyLen != 256))
    {
        return FALSE;
    }

    if(u4KeyLen == 128)
    {
        u4Ctrl = 0x0;
    }
    else if(u4KeyLen == 192)
    {
        u4Ctrl = 0x2;
    }
    else if(u4KeyLen == 256)
    {
        u4Ctrl = 0x4;
    }

    DMXCMD_WRITE32(DMX_REG_AES_CTRL, u4Ctrl);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetAesKey
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8])
{
    UINT32 i;
    UINT32* pu4CaKeys;
    UINT32 au4AesMap[8] = { 0, 1, 2, 3, 4, 5, 6, 7};

    ASSERT(au4Key != NULL);

    _DMX_Lock();

    if (fgEven)
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_EVEN_KEY;
    }
    else
    {
        pu4CaKeys = (UINT32*)DMX_SECTION_CA_ODD_KEY;
    }

    for (i = 0; i < 8; i++)
    {
        UINT32 u4MappedIndex;

        u4MappedIndex = au4AesMap[i];
        pu4CaKeys[(u1Index * 8) + i] = au4Key[u4MappedIndex];
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDmemAesIV
 */
//-----------------------------------------------------------------------------
void _DMX_SetDmemAesIV(UINT8 u1Pidx, const UINT32 au4Iv[4])
{
    UINT32 i;

    ASSERT(u1Pidx < DMX_MAX_AES_PIDX);

    if(au4Iv != NULL)
    {
        for (i = 0; i < 4; i++)
        {
            *(volatile UINT32*)(FVR_PER_PID_IV_BASE + (4 * i)) = au4Iv[3 - i];
        }
    }
}


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
        DMXCMD_REG32(DMX_REG_CA_CTRL) &= ~((1 << 7) | (1 << 15));
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

