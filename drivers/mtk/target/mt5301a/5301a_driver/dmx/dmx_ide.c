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
 * $RCSfile: dmx_ide.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_ide.c
 *  Demux driver - IDE test port
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_ide.h"
#include "dmx_if.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// IDE in-band messages
//
#define IDE_MSG_MAGIC                   0x5371
#define IDE_CMD_PACKET_BEGIN            0x88
#define IDE_CMD_PACKET_END              0x89

// IDE out-of-band messages
//
#define IDE_OUT_MSG_PACKET_ACK          0x51
#define IDE_IN_MSG_FLOW_ACK             0x61


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// IDE in-band message
typedef struct
{
    UINT32 u4Data;
    UINT8 u1PacketIndex;
    UINT8 u1Cmd;
    UINT16 u2Magic;
} IDE_INBAND_MESSAGE_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Is IDE enabled
static BOOL _fgEnableIde = FALSE;

/// Buffer of IDETPOUT. Be allocated at first time use
static UINT8* _pu1IdeBuf = NULL;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DmxIdeWrite
 *  Write data to IDETPOUT
 *
 *  @param  u4Addr          The begin address of data to be written
 *  @param  u4Size          The size of data in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIdeWrite(UINT32 u4Addr, UINT32 u4Size)
{
    CRIT_STATE_T rState;

    if (_pu1IdeBuf == NULL)
    {
        return FALSE;
    }

    rState = x_crit_start();

    while (u4Size > 0)
    {
        UINT32 u4WriteSize;

        // Note that a memory copy is required due to the alignment limitation
        // of IDE buffer
        u4WriteSize = MIN(u4Size, IDE_BUFFER_SIZE);
        x_memcpy(_pu1IdeBuf, (void*)u4Addr, u4WriteSize);

        // Flush cache
        HalFlushDCache();

        // Trigger IDE
        IDE_DMA_SOURCE = PHYSICAL((UINT32)_pu1IdeBuf);
        IDE_DMA_LEN = u4WriteSize;
        IDE_DMA_CONTROL |= 1;

        // Wait for completion
        while ((IDE_DMA_CONTROL & 0x1) != 0)
        {
        }

        // Update left size and data pointer
        u4Size -= u4WriteSize;
        u4Addr += u4WriteSize;
    }

    x_crit_end(rState);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** DMX_IDE_Enable
 *  Enable or disable IDETP
 *
 *  @param  fgEnable        Enable flag
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
void _DMX_IDE_Enable(BOOL fgEnable)
{  
    if (fgEnable)
    {
        // IDETPIN and IDETPOUT TS input selection
        UINT32 u4Ctrl;
        u4Ctrl = CKGEN_READ32(0x24C) & 0xFFFFFFF8;
        u4Ctrl |= 0x1;
        CKGEN_WRITE32(0x24C, u4Ctrl);
        
        // pinmux
        u4Ctrl = (CKGEN_READ32(0x400) & ~0x3FF000) | 0x2AA000;
        CKGEN_WRITE32(0x400, u4Ctrl);

        u4Ctrl = (CKGEN_READ32(0x404) & ~0x3F00) | 0x1500;
        CKGEN_WRITE32(0x404, u4Ctrl);

        u4Ctrl = (CKGEN_READ32(0x408) & ~0x7707) | 0x2203;
        CKGEN_WRITE32(0x408, u4Ctrl);

        u4Ctrl = (CKGEN_READ32(0x414) & ~0x3) | 0x3;
        CKGEN_WRITE32(0x414, u4Ctrl);
                
        // Init IDE DMA control
        IDE_DMA_CONTROL = 0;
        IDE_OUT_ENABLE
        IDE_SELECT_ENABLE

        // Clear out-of-band messages
        IDE_IN_MSG = 0;
        IDE_OUT_MSG1 = 0;
        IDE_OUT_MSG2 = 0;

        // Enable IDE-W
        IDE_SELECT_IN
        IDE_IN_ENABLE

        if (_pu1IdeBuf == NULL)
        {
            _pu1IdeBuf = (UINT8*)BSP_AllocAlignedDmaMemory(IDE_BUFFER_SIZE,
                32);
            ASSERT(_pu1IdeBuf != NULL);
            _pu1IdeBuf = (UINT8*)(VIRTUAL((UINT32)_pu1IdeBuf));
        }
    }
    else
    {
        // Disable IDE-W
        IDE_IN_DISABLE
//      IDE_SELECT_OUT

        // Free IDETPOUT buffer
        if (_pu1IdeBuf != NULL)
        {
            VERIFY(BSP_FreeAlignedDmaMemory(_pu1IdeBuf));
            _pu1IdeBuf = NULL;
        }
    }

    _fgEnableIde = fgEnable;
}


//-----------------------------------------------------------------------------
/** DMX_IDE_IsEnabled
 *  Is IDETP enabled or not
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Not enabled
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IDE_IsEnabled(void)
{
    return _fgEnableIde;
}


//-----------------------------------------------------------------------------
/** _DMX_IDE_Write
 *  Write data to IDETPOUT
 *
 *  @param  u4Addr          The begin address of data to be written
 *  @param  u4Size          The size of data in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IDE_Send(UINT32 u4Addr, UINT32 u4Size)
{
    BOOL fgRet;

    IDE_IN_DISABLE

    fgRet = _DmxIdeWrite(u4Addr, u4Size);

    IDE_IN_ENABLE

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_IDE_SendPacket
 *  Send packet in a ring buffer to IDETPOUT
 *
 *  @param  u1Idx           Packet index (PID index typically)
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Rp            Current read pointer
 *  @param  u4Wp            Current write pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IDE_SendPacket(UINT8 u1Idx, UINT32 u4BufStart,
    UINT32 u4BufEnd, UINT32 u4Rp, UINT32 u4Wp)
{
    UINT32 u4DataSize;
    IDE_INBAND_MESSAGE_T _rMsg;

    // Disable IDE-IN (flow control)
    IDE_IN_MSG = 0;
    IDE_IN_DISABLE

    // Waiting for ACK message from IDEW
    while (1)
    {
        UINT8 u1Msg;

        u1Msg = IDE_IN_MSG;
        if ((u1Msg & (1 << 7)) != 0)
        {
            u1Msg &= 0x7f;

            if (u1Msg != IDE_IN_MSG_FLOW_ACK)
            {
                LOG(5, "    Invalid message: 0x%02x\n", u1Msg);
            }

            break;
        }
    }

#if 1
    // Switch to IDE-OUT (IDER in PC side)
    IDE_SELECT_OUT

    // Clear IDE-OUT message
    IDE_OUT_MSG1 = 0;
#endif

    // Prepare IDE command message for the PID
    u4DataSize = DATASIZE(u4Rp, u4Wp, u4BufEnd - u4BufStart);
    _rMsg.u1Cmd = IDE_CMD_PACKET_BEGIN;
    _rMsg.u1PacketIndex = u1Idx;
    _rMsg.u2Magic = IDE_MSG_MAGIC;
    _rMsg.u4Data = u4DataSize;

    // Send IDE command message
    if (!_DmxIdeWrite((UINT32)&_rMsg, sizeof (IDE_INBAND_MESSAGE_T)))
    {
        return FALSE;
    }

    // Send lower part data in FIFO first
    if (u4Rp > u4Wp)
    {
        if (!_DmxIdeWrite(u4Rp, u4BufEnd - u4Rp))
        {
            return FALSE;
        }
        u4Rp = u4BufStart;
    }

    // Send write upper part data in FIFO
    if (u4Rp < u4Wp)
    {
        if (!_DmxIdeWrite(u4Rp, u4Wp - u4Rp))
        {
            return FALSE;
        }
        u4Rp = u4Wp;
    }

#if 1
    // Waiting for ACK message from IDER
    while (1)
    {
        UINT8 u1Msg;

        u1Msg = IDE_OUT_MSG1;
        if ((u1Msg & (1 << 7)) != 0)
        {
            u1Msg &= 0x7f;

            if (u1Msg != IDE_OUT_MSG_PACKET_ACK)
            {
                LOG(5, "    Invalid message: 0x%02x\n", u1Msg);
            }

            break;
        }
    }

    // Switch back to IDE-IN (IDEW in PC side)
    IDE_SELECT_IN
#endif

    // Re-enable IDE-IN
    IDE_IN_ENABLE

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_IDE_Read
 */
//-----------------------------------------------------------------------------
BOOL _DMX_IDE_Read(UINT32 u4Addr, UINT32 u4Size)
{
    CRIT_STATE_T rState;

    rState = x_crit_start();

    while (u4Size > 0)
    {
        // Flush cache
        HalFlushInvalidateDCache();

        // Trigger IDE
        IDE_DMA_SOURCE = u4Addr;
        IDE_DMA_LEN = u4Size;
        IDE_DMA_CONTROL |= 0x08;

        // Wait for completion
        while ((IDE_DMA_CONTROL & 0x08) != 0)
        {
        }

        // Flush cache
        HalFlushInvalidateDCache();

        u4Size -= u4Size;
    }

    x_crit_end(rState);

    return TRUE;
}


