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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pcmcia_hw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
  
 
 //-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_timer.h"
#include "x_pinmux.h"
LINT_EXT_HEADER_END

#include "pcmcia_if.h"
#include "pcmcia_ctrl.h"
#include "pcmcia_hw.h"

#include "CableCard/podci_ctrl.h"
#include "SPI/spi_if.h"
#include "NFI/nfi_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_MT5363
static PCMCIA_IF_TYPE_T fgPcmciaAccessMode = PCMCIA_IF_TYPE_SPI;
#else
static PCMCIA_IF_TYPE_T fgPcmciaAccessMode = PCMCIA_IF_TYPE_NFI;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Query PBus status.
 */
//-----------------------------------------------------------------------------
UINT32 PCMCIAHW_QueryPbusSts(UINT32 u4Bits)
{
    return (PCMCIAHW_READ32(REG_PBUS_STS) & u4Bits);
}

//-----------------------------------------------------------------------------
/** This API gets the current operation mode.
 */
//-----------------------------------------------------------------------------
PCMCIA_IF_TYPE_T PCMCIAHW_GetIfType(void)
{
    return fgPcmciaAccessMode;
}

//-----------------------------------------------------------------------------
/** This API sets the operation mode.
 */
//-----------------------------------------------------------------------------
void PCMCIAHW_SetIfType(PCMCIA_IF_TYPE_T eOpMode)
{
    if ( eOpMode == PCMCIA_IF_TYPE_SPI )
    {
        LOG(0, "Set PCMCIA Access IF : SPI\n");
    }
    else
    {
        LOG(0, "Set PCMCIA Access IF : NFI\n");
    }

    fgPcmciaAccessMode = eOpMode;
}

//-----------------------------------------------------------------------------
/** Register 8bit read (Register on 8295)
 */
//-----------------------------------------------------------------------------
UINT8 PCMCIAHW_READ8(UINT32 u4Addr)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return IO_READ8(PCMCIA_BASE, u4Addr);
    }
    else
    {
        UINT32 u4Data;
        if ( fgPcmciaAccessMode == PCMCIA_IF_TYPE_SPI)
        {
            SPI_SDARead(&u4Data, u4Addr>>2);
        }
        else
        {
            NFI_SDARead(u4Addr>>2, &u4Data);
        }

        switch( u4Addr & 0x03)
        {
            case 0:
                u4Data = (u4Data&0x000000ff);
                break;
            case 1:
                u4Data = ((u4Data&0x0000ff00)>>8);
                break;
            case 2:
                u4Data = ((u4Data&0x00ff0000)>>16);
                break;
            case 3:
                u4Data = ((u4Data&0xff000000)>>24);
                break;
            default:
                ASSERT(0);
        }
        return (UINT8)u4Data;
    }
}

//-----------------------------------------------------------------------------
/** Register 32bit read (Register on 8295)
 */
//-----------------------------------------------------------------------------
UINT32 PCMCIAHW_READ32(UINT32 u4Addr)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return IO_READ32(PCMCIA_BASE, u4Addr);
    }
    else
    {
        UINT32 u4Data;

        if ( fgPcmciaAccessMode == PCMCIA_IF_TYPE_SPI)
        {
                SPI_SDARead( (UINT32*) &u4Data, u4Addr>>2);
        }
        else
        {
                NFI_SDARead(u4Addr>>2, &u4Data);
        }

        return u4Data;
    }
}
//-----------------------------------------------------------------------------
/** Register 32bit write (Register on 8295)
 */
//-----------------------------------------------------------------------------
INT32 PCMCIAHW_WRITE32(UINT32 u4Addr, UINT32 value)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        return IO_WRITE32(PCMCIA_BASE, u4Addr, value);
    }
    else
    {
        INT32 i4ret;

        if ( fgPcmciaAccessMode == PCMCIA_IF_TYPE_SPI)
        {
            i4ret = SPI_SDAWrite( (const UINT32*)&value, u4Addr>>2);
        }
        else
        {
            i4ret = NFI_SDAWrite(u4Addr>>2,value);
        }

        return i4ret;
    }
}

//-----------------------------------------------------------------------------
/** PCMCIAHW_DMA_READ32
 *      Read operation use DMA between main chip & 8295.
 *      (Not DMA between 8295 & CAM)
 */
//-----------------------------------------------------------------------------
INT32 PCMCIAHW_DMA_READ32(UINT32 u4DRAMAddr,UINT32 u4FlashAddr,UINT32 u4ByteCount)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        // not implemented yet
        ASSERT(0);
        return 0;
    }
    else
    {
        INT32 i4ret;

        if ( fgPcmciaAccessMode == PCMCIA_IF_TYPE_SPI)
        {
            i4ret = SPI_DMARead((UINT32 * )u4DRAMAddr,u4FlashAddr>>2, (u4ByteCount+3)>>2);
        }
        else
        {
            i4ret = NFI_BurstRead( u4DRAMAddr, u4FlashAddr>>2, (u4ByteCount+3));
        }
        return i4ret;
    }
}

//-----------------------------------------------------------------------------
/** PCMCIAHW_DMA_WRITE32
 *      Write operation use DMA between main chip & 8295.
 *      (Not DMA between 8295 & CAM)
 */
//-----------------------------------------------------------------------------
INT32 PCMCIAHW_DMA_WRITE32(UINT32 u4DRAMAddr, UINT32 u4FlashAddr, UINT32 u4ByteCount)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_INTERNAL )
    {
        // not implemented yet
        ASSERT(0);
        return 0;
    }
    else
    {
        INT32 i4ret;

        if ( fgPcmciaAccessMode == PCMCIA_IF_TYPE_SPI)
        {
            i4ret = SPI_DMAWrite((const UINT32 *) u4DRAMAddr, u4FlashAddr>>2,(u4ByteCount+3)>>2);
        }
        else
        {
            i4ret = NFI_BurstWrite( u4DRAMAddr, u4FlashAddr>>2, (u4ByteCount+3));
        }

        return i4ret;
    }
}

