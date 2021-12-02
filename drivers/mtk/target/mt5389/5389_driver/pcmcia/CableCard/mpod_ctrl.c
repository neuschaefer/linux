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
 * $RCSfile: mpod_ctrl.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
  
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "x_util.h"
LINT_EXT_HEADER_END

#include "../pcmcia_if.h"
#include "../pcmcia_ctrl.h"
#include "../pcmcia_hw.h"


#include "mpod_ctrl.h"
#include "../SPI/spi_if.h"
#include "../NFI/nfi_if.h"

#ifdef PCMCIA_ENABLED

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#define _MPOD_EnableNullHR()    PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD)|0x4000)
#define _MPOD_DisableNullHR()   PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD)&(~0x4000))
#define _MPOD_EnableHR()        PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)|0x40)
#define _MPOD_DisableHR()       PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)&(~0x40))
#define _MPOD_EnableEC()        PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)|0x20)
#define _MPOD_DisableEC()       PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)&(~0x20))
#define _MPOD_EnableL()         PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)|0x10)
#define _MPOD_DisableL()        PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)&(~0x10))
#define _MPOD_EnableF()         PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)|0x08)
#define _MPOD_DisableF()        PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)&(~0x08))
#define _MPOD_EnableDA()        PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)|0x04)
#define _MPOD_DisableDA()       PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)&(~0x04))
#define _MPOD_EnableER()        PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)|0x02)
#define _MPOD_DisableER()       PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, PCMCIAHW_READ32(REG_MPOD_WRITE_CMD_HDR)&(~0x02))


static INT32 _MPOD_WaitForWriteComplete(void)
{
    UINT32 i=0;
    while(1)
    {
        if ( (PCMCIAHW_READ32(REG_MPOD_INTR_STATUS_REG) & 0x1) != 0 )
        {
            (void)PCMCIAHW_WRITE32(REG_MPOD_INTR_STATUS_REG, 0x1);
            return 0;
        }
        i++;
        if ( i == 0xffff )
        {
            LOG(0, "_MPOD_WaitForWriteComplete timeout ! --------------\n");
            return -1;
        }
    }
}

static INT32 _MPOD_WaitForReadComplete(void)
{
    UINT32 i=0;
    while(1)
    {
        if ( (PCMCIAHW_READ32(REG_MPOD_INTR_STATUS_REG) & 0x2) != 0 )
        {
            (void)PCMCIAHW_WRITE32(REG_MPOD_INTR_STATUS_REG, 0x2);
            return 0;
        }
        i++;
        if ( i == 0xffff )
        {
            LOG(0, "_MPOD_WaitForReadComplete timeout ! --------------\n");
            return -1;
        }
    }
}

static void _MPOD_PrintReadCmdStatus(void)
{
    UINT8 MRSNCLL=0, MRSNCLM=0, MRSLCS=0, MRSSTS=0, MRSNCQ=0;

    MRSNCLL = (UINT8)(PCMCIAHW_READ32(REG_MPOD_READ_CMD_STATUS) & 0xff000000) >> 24;
    MRSNCLM = (UINT8)(PCMCIAHW_READ32(REG_MPOD_READ_CMD_STATUS) & 0x00ff0000) >> 16;
    MRSNCQ =  (UINT8)(PCMCIAHW_READ32(REG_MPOD_READ_CMD_STATUS) & 0x0000ff00) >> 8;
    MRSLCS =  (UINT8)(PCMCIAHW_READ32(REG_MPOD_READ_CMD_STATUS) & 0x00000008) >> 3;
    MRSSTS =  (UINT8)(PCMCIAHW_READ32(REG_MPOD_READ_CMD_STATUS) & 0x00000007);

    LOG(1, "Read Command Status: \n");
    LOG(1, "    Length LSB = 0x%X\n", MRSNCLL);
    LOG(1, "    Length MSB = 0x%X\n", MRSNCLM);
    LOG(1, "    Query Byte = 0x%X\n", MRSNCQ);
    LOG(1, "    Length Check = 0x%X\n", MRSLCS);
    LOG(1, "    Read Status = 0x%X\n", MRSSTS);
}

static INT32 _MPOD_CheckFifoSettingForMpod(void)
{
    if ( (PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & FIFO_MODE) != 0 &&
         (PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & FIFO4FLASH) == 0 )
    {
        return 0;
    }
    else
    {
        LOG(0, "MPOD is not initialized ! Fifo is not set for MPOD ! 0x%X\n", PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0));
        return -1;
    }
}

static INT32 _MPOD_Init4TS(UINT32 u4Case)
{
    UINT32 u4Reg;
    // u4Case:  0: 4TS to M-Card,
    //          1: 3TS to M-Card, 1TS to CMP Merge
    //          2: 2TS to M-Card, 2TS to CMP Merge
    //          3: 1TS to M-Card, 3TS to CMP Merge
    //          4: 4TS to CMP Merge

    // Change CKGEN to CMP output
    u4Reg = PCMCIAHW_READ32(MPOD_CKGEN_PAD_CFG0);
    u4Reg &= ~(TSOP_SEL | TSO1_SEL);
    u4Reg |= 1 << TSOP_SEL_SHIFT;
    u4Reg |= 1 << TSO1_SEL_SHIFT;
    u4Reg &= ~(3<<2);
    u4Reg |= (1 << 2);
    (void)PCMCIAHW_WRITE32(MPOD_CKGEN_PAD_CFG0, u4Reg);
    while ((PCMCIAHW_READ32(MPOD_CKGEN_PAD_CFG0) & (TSOP_SEL | TSO1_SEL)) != (TSOP_SEL | TSO1_SEL))
    {
        LOG(0, "Set to CKGEN MPOD_CKGEN_PAD_CFG0 error.\n");
        (void)PCMCIAHW_WRITE32(MPOD_CKGEN_PAD_CFG0, u4Reg);
    }

    // Change to parallel output
    u4Reg = PCMCIAHW_READ32(MPOD_CKGEN_DATAPATH_CFG0);
    u4Reg &= ~(TS_OUT_PARALLEL);
    u4Reg |= 1 << TS_OUT_PARALLEL_SHIFT;
    (void)PCMCIAHW_WRITE32(MPOD_CKGEN_DATAPATH_CFG0, u4Reg);
    while ((PCMCIAHW_READ32(MPOD_CKGEN_DATAPATH_CFG0) & (TS_OUT_PARALLEL)) != (TS_OUT_PARALLEL))
    {
        LOG(0, "Set to CKGEN MPOD_CKGEN_DATAPATH_CFG0 error.\n");
        (void)PCMCIAHW_WRITE32(MPOD_CKGEN_DATAPATH_CFG0, u4Reg);
    }

    // Switch to MPOD mode, 1
    (void)PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL)|MPODE);

    // Set MPOD FIFO_MODE
// avoid compile error
//    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, ((PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&~FIFO4FLASH)|FIFO_MODE)&~BYPASS_EN);

    // Set LTSID, 257 (3, 2, 1, 0)
    (void)PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_1, 0x03020100);

    // Enable TS Setting, 65
    (void)PCMCIAHW_WRITE32(REG_MPOD_POWER_SET, PCMCIAHW_READ32(REG_MPOD_POWER_SET)|0x10);
    (void)PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL)|0x000c0000);

    // Interrupt Mask Register, 265
    (void)PCMCIAHW_WRITE32(MPOD_INTERRUPT_MASK_REGISTER, 0x0000000b);

    // Enable MMX Framer 0, 272
    switch ( u4Case )
    {
        case 0: //          0: 4TS to M-Card,
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x055fdddd);
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x0550dddd);
            break;

        case 1: //          1: 3TS to M-Card, 1TS to CMP Merge
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x056fdddd);
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x0560dddd);
            break;
        
        case 2: //          2: 2TS to M-Card, 2TS to CMP Merge
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x05afdddd);
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x05a0dddd);
            break;
        
        case 3: //          3: 1TS to M-Card, 3TS to CMP Merge
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x06afdddd);
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x06a0dddd);
            break;
        
        case 4: //          4: 4TS to CMP Merge
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x0aafdddd);
            (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x0aa0dddd);
            break;
    }

    // Reset MMX DBM & enable, 276
    (void)PCMCIAHW_WRITE32(MPOD_MUX_DBM_CONTROL_REGISTER, 0xc0000010);
    (void)PCMCIAHW_WRITE32(MPOD_MUX_DBM_CONTROL_REGISTER, 0x40000010);

    // Reset & Enable CMP, 448
    (void)PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 0x031677c8);
    (void)PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 0x011633c8);

    // Set MUX input source, 11305
    (void)PCMCIAHW_WRITE32(0xb0a4, 0x04000000);
    while (PCMCIAHW_READ32(0xb0a4) != 0x04000000)
    {
        LOG(0, "Set to CKGEN 0xb0a4 error.\n");
        (void)PCMCIAHW_WRITE32(0xb0a4, 0x04000000);
    }

    // Reset MDMX, 384
    (void)PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 0x00210009);
    (void)PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 0x00200009);

    // Run MMX/DMX, 256
    (void)PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x0);
    while (PCMCIAHW_READ32(MPOD_GLOBAL_CONTROL_REGISTER_0) != 0)
    {
        LOG(0, "Set to MPOD_GLOBAL_CONTROL_REGISTER_0 error.\n");
        (void)PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x0);
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 MPOD_InitTs(INT32 i4Mode)
{
    UINT32 u4Reg;

    switch (i4Mode)
    {
    case 1:
        // 2TS via MDMX-P2S output

        // Change CKGEN to serial TS output
        u4Reg = PCMCIAHW_READ32(MPOD_CKGEN_PAD_CFG0);
        u4Reg &= ~(TSOP_SEL | TSO1_SEL);
        (void)PCMCIAHW_WRITE32(MPOD_CKGEN_PAD_CFG0, u4Reg);

        // Change to serial output
        u4Reg = PCMCIAHW_READ32(MPOD_CKGEN_DATAPATH_CFG0);
        u4Reg &= ~(TS_OUT_PARALLEL);
        (void)PCMCIAHW_WRITE32(MPOD_CKGEN_DATAPATH_CFG0, u4Reg);

        // Switch to MPOD mode, 1
        (void)PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL)|MPODE);
        
        // Set MPOD FIFO_MODE
// avoid compile error
//        (void)PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, ((PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&~FIFO4FLASH)|FIFO_MODE)&~BYPASS_EN);

        // Set LTSID, 257 (3,2,1,0)
        (void)PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_1, 0x03020100);

        // Enable TS Setting, 65
        (void)PCMCIAHW_WRITE32(REG_MPOD_POWER_SET, PCMCIAHW_READ32(REG_MPOD_POWER_SET)|0x10);
        (void)PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL)|0x000c0000);

        // Interrupt Mask Register, 265
        (void)PCMCIAHW_WRITE32(MPOD_INTERRUPT_MASK_REGISTER, 0x0003000b);

        // Reset MMX DBM & enable, 276
        (void)PCMCIAHW_WRITE32(MPOD_MUX_DBM_CONTROL_REGISTER, 0xc0000008);
        (void)PCMCIAHW_WRITE32(MPOD_MUX_DBM_CONTROL_REGISTER, 0x40000008);

        // Enable MMX Framer 0, 272
        (void)PCMCIAHW_WRITE32(MPOD_MUX_FRAMER_CONTROL_REGISTER, 0x005000dd);

        // Reset & Enable CMP, 448
        (void)PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 0x031677c8);
        (void)PCMCIAHW_WRITE32(MPOD_CMP_CONFIGURATION_REGISTER, 0x011633c8);

        // Set MUX input source, 11305
        (void)PCMCIAHW_WRITE32(0xb0a4, 0x04000000);
        while (PCMCIAHW_READ32(0xb0a4) != 0x04000000)
        {
            LOG(0, "Set to CKGEN 0xb0a4 error.\n");
            (void)PCMCIAHW_WRITE32(0xb0a4, 0x04000000);
        }

        // Reset MDMX, 384
        (void)PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 0x00210009);
        (void)PCMCIAHW_WRITE32(MPOD_DEMUX_FRAMER_CONTROL_REGISTER, 0x00200009);

        // Run MMX/DMX, 256
        (void)PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x0);
        while (PCMCIAHW_READ32(MPOD_GLOBAL_CONTROL_REGISTER_0) != 0)
        {
            LOG(0, "Set to MPOD_GLOBAL_CONTROL_REGISTER_0 error.\n");
            (void)PCMCIAHW_WRITE32(MPOD_GLOBAL_CONTROL_REGISTER_0, 0x0);
        }
        break;

    case 2:
        LOG(0, "4TS to M-Card.\n");
        _MPOD_Init4TS(0);
        break;
    case 3:
        LOG(0, "3TS to M-Card, 1TS to CMP Merge.\n");
        _MPOD_Init4TS(1);
        break;
    case 4:
        LOG(0, "2TS to M-Card, 2TS to CMP Merge.\n");
        _MPOD_Init4TS(2);
        break;
    case 5:
        LOG(0, "1TS to M-Card, 3TS to CMP Merge.\n");
        _MPOD_Init4TS(3);
        break;
    case 6:
        LOG(0, "4TS to CMP Merge.\n");
        _MPOD_Init4TS(4);
        break;
    default:
        break;
    }

    return 0;
}

INT32 MPOD_PrintInfo(void)
{
    UINT32 u4Framer0StatusRegister2;
    UINT32 u4Framer1StatusRegister2;
    UINT32 u4Framer2StatusRegister2;
    UINT32 u4Framer3StatusRegister2;
    UINT32 u4DemuxPacketCountRegister;
    UINT32 u4InterruptStatusRegister;
    UINT32 u4CMPFramer0StatusRegister;
    UINT32 u4CMPFramer1StatusRegister;
    u4Framer0StatusRegister2 = PCMCIAHW_READ32(MPOD_MUX_FRAMER_0_STATUS_REGISTER2);
    u4Framer1StatusRegister2 = PCMCIAHW_READ32(MPOD_MUX_FRAMER_1_STATUS_REGISTER2);
    u4Framer2StatusRegister2 = PCMCIAHW_READ32(MPOD_MUX_FRAMER_2_STATUS_REGISTER2);
    u4Framer3StatusRegister2 = PCMCIAHW_READ32(MPOD_MUX_FRAMER_3_STATUS_REGISTER2);
    u4DemuxPacketCountRegister = PCMCIAHW_READ32(MPOD_DEMUX_PACKET_COUNT_REGISTER);
    u4InterruptStatusRegister = PCMCIAHW_READ32(MPOD_INTERRUPT_STATUS_REGISTER);
    u4CMPFramer0StatusRegister = PCMCIAHW_READ32(MPOD_CMP_FRAMER_0_STATUS_REGISTER);
    u4CMPFramer1StatusRegister = PCMCIAHW_READ32(MPOD_CMP_FRAMER_1_STATUS_REGISTER);

    LOG(0, "MMX Framer 0, Receive : %d, Drop :%d \n", (u4Framer0StatusRegister2&0x0000ffff), ((u4Framer0StatusRegister2&0xffff0000)>>16));
    LOG(0, "MMX Framer 1, Receive : %d, Drop :%d \n", (u4Framer1StatusRegister2&0x0000ffff), ((u4Framer1StatusRegister2&0xffff0000)>>16));
    LOG(0, "MMX Framer 2, Receive : %d, Drop :%d \n", (u4Framer2StatusRegister2&0x0000ffff), ((u4Framer2StatusRegister2&0xffff0000)>>16));
    LOG(0, "MMX Framer 3, Receive : %d, Drop :%d \n\n", (u4Framer3StatusRegister2&0x0000ffff), ((u4Framer3StatusRegister2&0xffff0000)>>16));
    LOG(0, "CMP Framer 0, Receive : %d, Drop :%d \n", (u4CMPFramer0StatusRegister&0x0000ffff), ((u4CMPFramer0StatusRegister&0xffff0000)>>16));
    LOG(0, "CMP Framer 1, Receive : %d, Drop :%d \n\n", (u4CMPFramer1StatusRegister&0x0000ffff), ((u4CMPFramer1StatusRegister&0xffff0000)>>16));
    LOG(0, "MDMX Framer, Receive  : %d, Drop :%d \n", (u4DemuxPacketCountRegister&0x0000ffff), ((u4DemuxPacketCountRegister&0xffff0000)>>16));

    if ( u4InterruptStatusRegister & 0x00020000 )
    {
        LOG(0, "MPOD Demux Status Interrupt: 0x%X\n", PCMCIAHW_READ32(MPOD_DEMUX_STATUS_INTERRUPT_REASON_REGISTER));
        LOG(0, "Impossible case!\n");
        ASSERT(0);
    }
    if ( u4InterruptStatusRegister & 0x00010000 )
    {
        LOG(0, "MPOD Mux Status Interrupt: 0x%X\n", PCMCIAHW_READ32(MPOD_MUX_STATUS_INTERRUPT_REASON_REGISTER));
        LOG(0, "Impossible case!\n");
        ASSERT(0);
    }
    if ( u4InterruptStatusRegister & 0x00000008 )
    {
        LOG(0, "CMP Error Interrupt: 0x%X\n", PCMCIAHW_READ32(MPOD_CMP_ERROR_INTERRUPT_REASON_REGISTER));
        (void)PCMCIAHW_WRITE32(MPOD_CMP_ERROR_INTERRUPT_REASON_REGISTER, 0);
    }
    if ( u4InterruptStatusRegister & 0x00000002 )
    {
        LOG(0, "MPOD Demux Error Interrupt: 0x%X\n", PCMCIAHW_READ32(MPOD_DEMUX_ERROR_INTERRUPT_REASON_REGISTER));
        (void)PCMCIAHW_WRITE32(MPOD_DEMUX_ERROR_INTERRUPT_REASON_REGISTER, 0);
    }
    if ( u4InterruptStatusRegister & 0x00000001 )
    {
        LOG(0, "MPOD Mux Error Interrupt: 0x%X\n", PCMCIAHW_READ32(MPOD_MUX_ERROR_INTERRUPT_REASON_REGISTER));
        (void)PCMCIAHW_WRITE32(MPOD_MUX_ERROR_INTERRUPT_REASON_REGISTER, 0);
    }

    return 0;
}

INT32 MPOD_Read(UINT8* u1Ptr, UINT32 u4Len)
{
    INT32 i4Result=0;

    if ( _MPOD_CheckFifoSettingForMpod() )
    {
        return -1;
    }

    // Enable Read Command
    (void)PCMCIAHW_WRITE32(REG_MPOD_READ_CMD, 0x17);
    (void)PCMCIAHW_WRITE32(REG_MPOD_READ_DMA_START_ADDR, 0x0000);
    (void)PCMCIAHW_WRITE32(REG_MPOD_READ_DMA_END_ADDR, 0x1001);
    (void)PCMCIAHW_WRITE32(REG_MPOD_READ_DMA_THRESHOLD, 0x1000);

    // Enable HR
    (void)_MPOD_EnableNullHR();

    // Wait for read complete
    i4Result = _MPOD_WaitForReadComplete();

    // Disable HR
    (void)_MPOD_DisableNullHR();

    // Print Read Command Status Register
    _MPOD_PrintReadCmdStatus();

    (void)SPI_DMARead((UINT32*)u1Ptr, 0, u4Len);
    
    x_thread_delay(1);

    LOG(1, "R-DMA Start Address : %d \n", PCMCIAHW_READ32(REG_MPOD_READ_DMA_START_ADDR));
    LOG(1, "R-DMA End Address : %d \n", PCMCIAHW_READ32(REG_MPOD_READ_DMA_END_ADDR));
    LOG(1, "Current byte count in 8295 fifo : %d \n", PCMCIAHW_READ32(REG_MPOD_READ_DMA_BYTE_CNT));
    LOG(1, "R-DMA Write Pointer : %d \n", PCMCIAHW_READ32(REG_MPOD_READ_DMA_DRAM_WRITE_PTR));
    LOG(1, "R-DMA Read Pointer : %d \n", PCMCIAHW_READ32(REG_MPOD_READ_DMA_DRAM_READ_PTR));
    LOG(1, "R-DMA Threshold : %d \n", PCMCIAHW_READ32(REG_MPOD_READ_DMA_THRESHOLD));

    if ( i4Result == 0 &&
         PCMCIAHW_READ32(REG_MPOD_INTR_STATUS_REG) == 0 &&
         PCMCIAHW_READ32(REG_MPOD_READ_CMD) == 0x16 &&
         PCMCIAHW_READ32(REG_MPOD_DEV_CHANNEL_STATUS) == 0x40 )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

INT32 MPOD_Write(UINT8* u1Ptr, UINT32 u4Len)
{
    UINT32 u4WriteCmdHeader=0;
    INT32 i4Result = 0;

    // Error Handling ==========================================================
    if ( _MPOD_CheckFifoSettingForMpod() )
    {
        return -1;
    }

    if ( u4Len > 4096 )
    {
        LOG(0, "MPOD Write size limitation : 4096. (%d), u4Len\n");
        return -1;
    }
    // =========================================================================


    SPI_DMAWrite( (const UINT32 *) u1Ptr, 0, u4Len);

    // Set Write DMA Address
    (void)PCMCIAHW_WRITE32(REG_MPOD_WRITE_DMA_DRAM_ADDR, 0);

    // Set Write DMA Byte Count
    (void)PCMCIAHW_WRITE32(REG_MPOD_WRITE_DMA_BYTE_CNT, u4Len);

    // Set Write Command Header
    u4WriteCmdHeader |= ((u4Len & 0x000000FF)<<16); // LSB
    u4WriteCmdHeader |= ((u4Len & 0x0000FF00)<<8);  // MSB
    u4WriteCmdHeader |= 0x5C;                       // Query Byte
    (void)PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD_HDR, u4WriteCmdHeader);

    // Set Null Write Command
    (void)PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD, (PCMCIAHW_READ32(REG_MPOD_WRITE_CMD)&0xffffff00)|0x07);

    // Wait for command done
    i4Result = _MPOD_WaitForWriteComplete();

    // Set Null Write Command
    (void)PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD, (PCMCIAHW_READ32(REG_MPOD_WRITE_CMD)&0xffffff00)|0x04);
    x_thread_delay(1);

    if ( i4Result == 0 &&
         PCMCIAHW_READ32(REG_MPOD_WRITE_CMD) == 0x4 &&
         PCMCIAHW_READ32(REG_MPOD_INTR_STATUS_REG) == 0 &&
         PCMCIAHW_READ32(REG_MPOD_DEV_CHANNEL_STATUS) == 0x40 )
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

INT32 MPOD_InitM(void)
{

    UINT8 u1LocalBuf[512]={0};
    UINT8 u1ResourceId0,u1ResourceId1,u1ResourceId2,u1ResourceId3;
    // M-CARD Detect Mode

    // Set PinMux for OOB
    (void)PCMCIAHW_WRITE32(0xB03C, 0x40000010);
    (void)PCMCIAHW_WRITE32(0xB040, 0x00140000);

    // Switch to MPOD mode
    (void)PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL)|MPODE);

    // Enable MPOD Interrupt
    (void)PCMCIAHW_WRITE32(REG_MPOD_EXTRA_CTRL, PCMCIAHW_READ32(REG_MPOD_EXTRA_CTRL)|CMD_MPOD_MWCIE|CMD_MPOD_MRCIE|CMD_MPOD_MCRIE|CMD_MPOD_MCERIE|CMD_MPOD_MRDBFIE|CMD_MPOD_MWDDFFIE|CMD_MPOD_MRDFFIE);

    // Set MPOD FIFO_MODE
// avoid compile error
//    (void)PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, ((PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&~FIFO4FLASH)|FIFO_MODE)&~BYPASS_EN);

    // Pull-Up RESET
    (void)PCMCIA_Reset(1);

    // Pull-Down RESET
    (void)PCMCIA_Reset(0);

    // Enable Null Write Command & HR On (parepare for receive open_session_request() )
    (void)PCMCIAHW_WRITE32(REG_MPOD_WRITE_CMD, 0x00000004);

    // Read open_session_request from 8295 fifo to main chip DRAM
    (void)MPOD_Read(u1LocalBuf, 6);
    LOG(1, "open_session_request: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X \n", u1LocalBuf[0], u1LocalBuf[1], u1LocalBuf[2], u1LocalBuf[3], u1LocalBuf[4], u1LocalBuf[5]);

    // Send open_session_response to M-Card
    u1ResourceId0 = u1LocalBuf[2];
    u1ResourceId1 = u1LocalBuf[3];
    u1ResourceId2 = u1LocalBuf[4];
    u1ResourceId3 = u1LocalBuf[5];
    u1LocalBuf[0] = (UINT8)0x92;
    u1LocalBuf[1] = (UINT8)0x07;
    u1LocalBuf[2] = (UINT8)0x00;
    u1LocalBuf[3] = u1ResourceId0;
    u1LocalBuf[4] = u1ResourceId1;
    u1LocalBuf[5] = u1ResourceId2;
    u1LocalBuf[6] = u1ResourceId3;
    u1LocalBuf[7] = (UINT8)0x00;
    u1LocalBuf[8] = (UINT8)0x01;
    (void)MPOD_Write(u1LocalBuf, 9);
    LOG(1, "open_session_response: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n", u1LocalBuf[0], u1LocalBuf[1], u1LocalBuf[2], u1LocalBuf[3], u1LocalBuf[4], u1LocalBuf[5], u1LocalBuf[6], u1LocalBuf[7], u1LocalBuf[8]);

    if ( PCMCIAHW_READ32(REG_MPOD_WRITE_CMD) == 0x4 &&
         PCMCIAHW_READ32(REG_MPOD_INTR_STATUS_REG) == 0 &&
         PCMCIAHW_READ32(REG_MPOD_READ_CMD) == 0x16 &&
         PCMCIAHW_READ32(REG_MPOD_DEV_CHANNEL_STATUS) == 0x40 )
    {
        LOG(0, "MPOD Init M-Mode Done.\n");
        return 0;
    }
    else
    {
        return -1;
    }
}


INT32 MPOD_InitS(void)
{
    LOG(0, "MPOD Init S-Mode OK\n");
    return 0;
}

#endif
#endif
