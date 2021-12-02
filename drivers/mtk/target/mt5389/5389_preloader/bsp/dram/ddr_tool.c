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
/** @file ddr_tool.c
 *  Dram parameter checking tool.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "ddr.h"
#include "x_bim.h"
#include "x_dram.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#ifdef NDEBUG
#define Printf(fmt...)
#endif

#define DQS_GATING_OFFSET (0)
#define DQS_GATING_STEP (5)
//#define DQS_GATING_ONE_BYTE_MODE
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifdef DRAM_WDQS_CALIBRATION
#define WDQS_CALIB_STEP     (3)
#define MAX_WDQS_DELAY      (16)            
#define MAX_WDQ_DELAY       (16)
#endif
//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef DRAM_DUTY_DQS_CALIBRATION
static void _DdrCalDuty(UINT32 u4ChipNum)
{
    UINT32 u4Byte;
    UINT32 u4MaxSize;
    UINT32 u4Val;    
    UINT32 u4Rec;
    UINT32 u4Size;
    UINT32 u4MapBit01[8] = {0x00, 0x01, 0x02, 0x03, 0x10, 0x11, 0x12, 0x13};
    UINT32 u4MapBit23[7] = {0x04, 0x08, 0x0C, 0x20, 0x24, 0x28, 0x2C};
    UINT32 u4MaxDQRDSEL;
    UINT32 u4MaxDQSRDSEL;
    UINT32 i;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    UINT32 j;

    DRAM_DDRPHY_WRITE32(0x398, 0);
    DRAM_DDRPHY_WRITE32(0x39C, 0);

    for (i=0; i<20; i++)
    {
        // Start from DQRDSEL.
        for (u4Byte=0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte++)
        {
            u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
        
            // Fix DQxRDSEL[3:2] = 0, search DQxRDSEL[1:0].
            u4MaxSize = 0;
            for (j=0; j<8; j++)
            {        
                u4Val = DRAM_DDRPHY_READ32(0x39C);
                u4Val &= ~(0x3F << (8*u4HwDQSth));
                u4Val |= (u4MapBit01[j] << (8*u4HwDQSth)); 
                DRAM_DDRPHY_WRITE32(0x39C, u4Val);

                //Printf("DQRDSEL: 0x739C=0x%08X.\n", u4Val);
                
                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    // Get result from SRAM.
                    u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4Byte /2)*4));
                    u4Size = ((u4Rec >> (((u4Byte & 0x1)*16) + 8)) & 0xFF) + 
                        ((u4Rec >> ((u4Byte & 0x1)*16)) & 0xFF) + 1;
                    
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQRDSEL = u4MapBit01[j];
                        //Printf("DQS%d: u4MaxSize=%d.\n", u4Byte, u4MaxSize);
                    }
                }
                else
                {
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                }
            }
        
            // Fix DQxRDSEL[1:0] = 0, search DQxRDSEL[3:2].
            u4MaxSize = 0;
            for (j=0; j<7; j++)
            {
                u4Val = DRAM_DDRPHY_READ32(0x39C);
                u4Val &= ~(0x3F << (8*u4HwDQSth));
                u4Val |= (u4MapBit23[j]<< (8*u4HwDQSth));            
                DRAM_DDRPHY_WRITE32(0x39C, u4Val);

                //Printf("DQRDSEL: 0x739C=0x%08X.\n", u4Val);

                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    // Get result from SRAM.
                    u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4Byte /2)*4));
                    u4Size = ((u4Rec >> (((u4Byte & 0x1)*16) + 8)) & 0xFF) + 
                        ((u4Rec >> ((u4Byte & 0x1)*16)) & 0xFF) + 1;
                    
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQRDSEL = u4MapBit23[j];
                        //Printf("DQS%d: u4MaxSize=%d.\n", u4Byte, u4MaxSize);                        
                    }
                }
                else
                {
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                }
            }                        

            u4Val = DRAM_DDRPHY_READ32(0x39C);
            u4Val &= ~(0x3F << (8*u4HwDQSth));
            u4Val |= (u4MaxDQRDSEL << (8*u4HwDQSth));            
            DRAM_DDRPHY_WRITE32(0x39C, u4Val);

            //Printf("Byte %d : MaxDQSSize = %d, MaxDQRDSEL = 0x%X.\n", u4Byte, u4MaxSize, u4MaxDQRDSEL);
        }

        // Next to search DQSRDSEL.
        for (u4Byte=0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte++)
        {
            u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
        
            // Fix DQSxRDSEL[5, 3, 2] = 0, search DQSxRDSEL[1:0].
            u4MaxSize = 0;
            for (j=0; j<8; j++)
            {
                u4Val = DRAM_DDRPHY_READ32(0x398);
                u4Val &= ~(0x3F << (8*u4HwDQSth));
                u4Val |= (u4MapBit01[j] << (8*u4HwDQSth));            
                DRAM_DDRPHY_WRITE32(0x398, u4Val);

                //Printf("DQSRDSEL: 0x7398=0x%08X.\n", u4Val);

                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    // Get result from SRAM.
                    u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4Byte /2)*4));
                    u4Size = ((u4Rec >> (((u4Byte & 0x1)*16) + 8)) & 0xFF) + 
                        ((u4Rec >> ((u4Byte & 0x1)*16)) & 0xFF) + 1;
                    
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQSRDSEL = u4MapBit01[j];
                        //Printf("DQS%d: u4MaxSize=%d.\n", u4Byte, u4MaxSize);
                    }
                }
                else
                {
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                }
            }

            // Fix DQxRDSEL[1:0] = 0, search DQxRDSEL[3:2].
            u4MaxSize = 0;
            for (j=0; j<7; j++)
            {
                u4Val = DRAM_DDRPHY_READ32(0x398);
                u4Val &= ~(0x3F << (8*u4HwDQSth));
                u4Val |= (u4MapBit23[j] << (8*u4HwDQSth));            
                DRAM_DDRPHY_WRITE32(0x398, u4Val);

                //Printf("DQSRDSEL: 0x7398=0x%08X.\n", u4Val);

                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    // Get result from SRAM.
                    u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4Byte /2)*4));
                    u4Size = ((u4Rec >> (((u4Byte & 0x1)*16) + 8)) & 0xFF) + ((u4Rec >> ((u4Byte & 0x1)*16)) & 0xFF) + 1;
                    
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQSRDSEL = u4MapBit23[j];
                        //Printf("DQS%d: u4MaxSize=%d.\n", u4Byte, u4MaxSize);                        
                    }
                }
                else
                {
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                }
            }                        

            u4Val = DRAM_DDRPHY_READ32(0x398);
            u4Val &= ~(0x3F << (8*u4HwDQSth));
            u4Val |= (u4MaxDQSRDSEL << (8*u4HwDQSth));            
            DRAM_DDRPHY_WRITE32(0x398, u4Val);

            //Printf("Byte %d : MaxDQSSize = %d, MaxDQSRDSEL = 0x%X.\n", u4Byte, u4MaxSize, u4MaxDQSRDSEL);
        }    

        Printf("Result(%2d): DQSRDSEL = 0x%08X, DQRDSEL = 0x%08X.\n", 
            i, DRAM_DDRPHY_READ32(0x398), DRAM_DDRPHY_READ32(0x39C));    
    }
}
#endif /* #ifdef DRAM_DUTY_DQS_CALIBRATION */

#ifdef DRAM_WRITE_DQS_CALIBRATION
static UINT32 _DdrCalOutDqs(UINT32 u4ChipNum)
{
    UINT32 u4Ret = 1;
    UINT32 u4Byte;
    UINT32 u4DQS;
    UINT32 u4DQ;
    UINT32 u4Reg360[4];
    UINT32 u4Reg354;

    u4Reg360[0] = DRAM_DDRPHY_READ32(0x360);
    u4Reg360[1] = DRAM_DDRPHY_READ32(0x364);
    u4Reg360[2] = DRAM_DDRPHY_READ32(0x368);
    u4Reg360[3] = DRAM_DDRPHY_READ32(0x36C);
    u4Reg354 = DRAM_DDRPHY_READ32(0x354);
    
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
        Printf("****** Byte=%d ******.\n", u4Byte);
        Printf("    DQ : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");

        DRAM_DDRPHY_WRITE32(0x360, u4Reg360[0]);
        DRAM_DDRPHY_WRITE32(0x364, u4Reg360[1]);
        DRAM_DDRPHY_WRITE32(0x368, u4Reg360[2]);
        DRAM_DDRPHY_WRITE32(0x36C, u4Reg360[3]);        
        DRAM_DDRPHY_WRITE32(0x354, u4Reg354);                

        for (u4DQS=0; u4DQS<16; u4DQS++)	
        {
            Printf("DQS=%02X : ", u4DQS);
            
            for (u4DQ=0; u4DQ<16; u4DQ++)	
            {
                _DDR_SetOutDQ(u4Byte, u4DQ);
                if (DDR_IsDqsFail(u4Byte, u4DQS, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
                {
                    Printf("%c ", 'X');                
                }
                else
                {
                    Printf("%c ", 'O');
                }
            }

            // go to next DQS.
            Printf("\n");
        }
    }
    
    DRAM_DDRPHY_WRITE32(0x360, u4Reg360[0]);
    DRAM_DDRPHY_WRITE32(0x364, u4Reg360[1]);
    DRAM_DDRPHY_WRITE32(0x368, u4Reg360[2]);
    DRAM_DDRPHY_WRITE32(0x36C, u4Reg360[3]);        
    DRAM_DDRPHY_WRITE32(0x354, u4Reg354);                

    return u4Ret;
}
#endif /* #ifdef DRAM_WRITE_DQS_CALIBRATION */

#ifdef DRAM_CLOCK_WRITE_DQS_CALIBRATION
static UINT32 _DdrCalClkAndOutDqs(UINT32 u4ChipNum)
{
    UINT32 u4Ret = 1;
    UINT32 u4Byte;
    UINT32 u4DQS;
    UINT32 u4DQ;
    UINT32 u4Reg360[4];
    UINT32 u4Reg354;
    UINT32 u4ClkDelay;
    UINT32 u4SetClkDelay;        
    UINT32 u4Val;        

    u4Reg360[0] = DRAM_DDRPHY_READ32(0x360);
    u4Reg360[1] = DRAM_DDRPHY_READ32(0x364);
    u4Reg360[2] = DRAM_DDRPHY_READ32(0x368);
    u4Reg360[3] = DRAM_DDRPHY_READ32(0x36C);
    u4Reg354 = DRAM_DDRPHY_READ32(0x354);

    for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
    {
        // Set clock delay.    
        u4SetClkDelay = u4ClkDelay;
        u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
        u4Val = DRAM_DDRPHY_READ32(0x304);
        u4Val &= ~ 0xFF;
        u4Val |= u4SetClkDelay;
        DRAM_DDRPHY_WRITE32(0x304, u4Val);

        Printf(">>>>> CLK delay=%d <<<<<<.\n", u4ClkDelay);

        for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
        {
            Printf("****** Byte=%d ******.\n", u4Byte);
            Printf("    DQ : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");

            DRAM_DDRPHY_WRITE32(0x360, u4Reg360[0]);
            DRAM_DDRPHY_WRITE32(0x364, u4Reg360[1]);
            DRAM_DDRPHY_WRITE32(0x368, u4Reg360[2]);
            DRAM_DDRPHY_WRITE32(0x36C, u4Reg360[3]);        
            DRAM_DDRPHY_WRITE32(0x354, u4Reg354);                

            for (u4DQS=0; u4DQS<16; u4DQS++)	
            {
                Printf("DQS=%02X : ", u4DQS);
                
                for (u4DQ=0; u4DQ<16; u4DQ++)	
                {
                    _DDR_SetOutDQ(u4Byte, u4DQ);
                    if (DDR_IsDqsFail(u4Byte, u4DQS, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
                    {
                        Printf("%c ", 'X');                
                    }
                    else
                    {
                        Printf("%c ", 'O');
                    }
                }

                // go to next DQS.
                Printf("\n");
            }
        }

    }
       
    DRAM_DDRPHY_WRITE32(0x360, u4Reg360[0]);
    DRAM_DDRPHY_WRITE32(0x364, u4Reg360[1]);
    DRAM_DDRPHY_WRITE32(0x368, u4Reg360[2]);
    DRAM_DDRPHY_WRITE32(0x36C, u4Reg360[3]);        
    DRAM_DDRPHY_WRITE32(0x354, u4Reg354);                

    return u4Ret;
}
#endif /* #ifdef DRAM_CLOCK_WRITE_DQS_CALIBRATION */

#ifdef DRAM_GATING_DQS_CALIBRATION
static UINT32 _DdrCalGatingDqsCtrl(void)
{
    UINT32 u4Val;    
    UINT32 u4DQSCtrl = 0;
    UINT32 u4DQSCtrlSet = 0;

    // First find the DQSCTL base on DQSIEN = 0.
    DRAM_WRITE32(0x388, 0);

    while (u4DQSCtrl <= 0x1000)
    {       
        u4Val = (u4DQSCtrl << 16) | u4DQSCtrl;
        DRAM_DDRPHY_WRITE32(0x380, u4Val);
        DRAM_DDRPHY_WRITE32(0x384, u4Val);
        
        Printf("Set DQSCTL=0x%08X, DQSIEN=0...", u4Val);   

        // Start reset phy and redo dram auto init.
        _DDR_ResetPhy();

        // Do DQS calibration to get DQS window average value. 
        if (DDR_CalibrateDqs() > 0)
        {
            Printf("Fail.\n");
        }
        else
        {
            Printf("Pass.\n");
            // We find a usable value to be candidate.
            u4DQSCtrlSet = u4Val;
        }           

        u4DQSCtrl = (u4DQSCtrl == 0) ? 1 : (u4DQSCtrl << 1);
    }

    // First find the DQSCTL base on DQSIEN = 0.
    DRAM_WRITE32(0x388, 0);
    DRAM_DDRPHY_WRITE32(0x380, u4DQSCtrlSet);
    DRAM_DDRPHY_WRITE32(0x384, u4DQSCtrlSet);

    // Start reset phy and redo dram auto init.
    _DDR_ResetPhy();

    // Do DQS calibration to get DQS window average value. 
    if (DDR_CalibrateDqs() > 0)
    {
        Printf("Still fail at DQSCTRL = 0x%08X.\n", u4DQSCtrlSet);                
    }

    return u4DQSCtrlSet;
}

static void _DdrCalGatingDqs(UINT32 u4ChipNum, UINT32 u4DQSCtrlSet)
{
    UINT32 u4Byte;
    UINT32 u4OldVal[4] = {0, 0, 0, 0};
    UINT32 u4Max[4] = {0, 0, 0, 0};
    UINT32 u4Min[4] = {0, 0, 0, 0};
    UINT32 u4Avg[4] = {0, 0, 0, 0};
    UINT32 u4Val;    
    UINT32 u4SetVal = 0;    
    UINT32 u4Reg388;
    UINT32 u4Step;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;

    DRAM_DDRPHY_WRITE32(0x380, u4DQSCtrlSet);
    DRAM_DDRPHY_WRITE32(0x384, u4DQSCtrlSet);
    u4Reg388 = DRAM_READ32(0x388);   

    // Reset PHY to recover DQS ring counter error.
    _DDR_ResetPhy();
    
    Printf("Current DQSCTL=0x%08X, DQSIEN=0x%08X.\n", u4DQSCtrlSet, u4Reg388);   
   
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
    
        u4OldVal[u4Byte] = (u4Reg388 >> (8*u4HwDQSth)) & 0x7F;
        
        for(u4Step=0; u4Step<0x60; u4Step=u4Step+3)
        {       
            u4Val = (u4OldVal[u4Byte] + u4Step) & 0x7F;
            if (u4Val > 0x60)
            {
                // Restore default gating window before PHY reset.
                DRAM_WRITE32(0x388, u4Reg388);
                // Reset PHY to recover DQS ring counter error.
                _DDR_ResetPhy();                
                break;
            }

            u4SetVal = DRAM_READ32(0x388);
            u4SetVal &= ~ (0x7F << (8*u4HwDQSth));
            u4SetVal |= (u4Val << (8*u4HwDQSth));
            
            //Printf("(%d) 0x388=0x%08X...", u4Byte, u4SetVal);   
            
            DRAM_WRITE32(0x388, u4SetVal); // Need to do early	

            // DRAM read/write test, u4Bin parameter is no meaning.
            if (DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_NO_CHANGE_TYPE, u4ChipNum))
            {
                //Printf("Fail.\n");   
            
                // Restore default gating window before PHY reset.
                DRAM_WRITE32(0x388, u4Reg388);

                // Reset PHY to recover DQS ring counter error.
                _DDR_ResetPhy();

                break;
            }
            else
            {
                u4Max[u4Byte] = u4Val;
                //Printf("Pass..Max=%d.\n", u4Val);   
            }
        }
       
        for(u4Step=0; u4Step<0x60; u4Step=u4Step+3)
        {
            if (u4OldVal[u4Byte] > u4Step)
            {
                u4Val = (u4OldVal[u4Byte] - u4Step) & 0x7F;
                u4SetVal = DRAM_READ32(0x388);
                u4SetVal &= ~ (0x7F << (8*u4HwDQSth));
                u4SetVal |= (u4Val << (8*u4HwDQSth));

                //Printf("(%d) 0x388=0x%08X...", u4Byte, u4SetVal);   
                
                DRAM_WRITE32(0x388, u4SetVal); // Need to do early	

                // DRAM read/write test, u4Bin parameter is no meaning.
                if (DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_NO_CHANGE_TYPE, u4ChipNum))
                {
                    //Printf("Fail.\n");   
                
                    // Restore default gating window before PHY reset.
                    DRAM_WRITE32(0x388, u4Reg388);
                    
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();

                    break;                    
                }
                else
                {
                    u4Min[u4Byte] = u4Val;                  
                    //Printf("Pass..Min=%d.\n", u4Val);                       
                }                
            }
        }        
    }

    u4Reg388 = 0;
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
    
        // Get Max and Min => Get Avg.
        u4Avg[u4Byte] = (u4Max[u4Byte] + u4Min[u4Byte]) / 2;

        u4Reg388 |= u4Avg[u4Byte] << (u4HwDQSth * 8);

        Printf("Byte %d : Gating (%2d ~ %2d), Size %d, Set = %d.\n", 
            u4Byte, u4Min[u4Byte], u4Max[u4Byte], 
            ((u4Max[u4Byte] - u4Min[u4Byte]) + 1), u4Avg[u4Byte]);
    }
    Printf("Set DQSCTL=0x%08X, DQSIEN=0x%08X.\n", 
        (DRAM_DDRPHY_READ32(0x380) & 0xFFF), u4Reg388);   
    
    DRAM_WRITE32(0x388, u4Reg388);

    return;
}
#endif /* #ifdef DRAM_GATING_DQS_CALIBRATION */

#ifdef DRAM_CS_DELAY_CALIBRATION
static void _DdrCalCsDelay(void)
{
    UINT32 u4ClkDelay;
    UINT32 au4CsDelay[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    UINT32 u4SetClkDelay;
    UINT32 u4SetCsDelay;
    UINT32 u4Val;
    UINT32 i;
    
    Printf("  CLK : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");
       
    for (i = 0; i<=0x0F; i++)	
    {       
        // Set chip select delay.
        u4SetCsDelay = au4CsDelay[i];
        u4Val = 0;
        u4Val |= u4SetCsDelay;
        u4SetCsDelay = (u4Val << 24)|(u4Val << 16)|(u4Val << 8)|(u4Val);

        // Set command delay.
        DRAM_DDRPHY_WRITE32(0x320, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x324, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x328, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x32C, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x330, u4SetCsDelay); // CS is here.
        DRAM_DDRPHY_WRITE32(0x334, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x338, u4SetCsDelay);
   
        Printf("CS=%02X : ", au4CsDelay[i]);

        for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
        {
            // Set clock delay.    
            u4SetClkDelay = u4ClkDelay;
            u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
            u4Val = DRAM_DDRPHY_READ32(0x304);
            u4Val &= ~ 0xFF;
            u4Val |= u4SetClkDelay;
            DRAM_DDRPHY_WRITE32(0x304, u4Val);

            // Start reset phy and redo dram auto init.
            _DDR_ResetPhy();

            // Do DQS calibration to get DQS window average value. 
            if (DDR_CalibrateDqs() > 0)
            {
                Printf("%c ", 'X');                
            }
            else
            {
                Printf("%c ", 'O');
            }           
        }

        // go to next CLK.
        Printf("\n");        
    }    

    Printf("\nCS set T/2 early.\n");
    Printf("  CLK : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");
    
    for (i = 0; i<=0x0F; i++)	
    {       
        // Set chip select delay.
        u4SetCsDelay = au4CsDelay[i];
        u4Val = 0;        
        u4Val |= (0x80 | u4SetCsDelay);
        u4SetCsDelay = (u4Val << 24)|(u4Val << 16)|(u4Val << 8)|(u4Val);

        // Set command delay.
        DRAM_DDRPHY_WRITE32(0x320, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x324, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x328, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x32C, u4SetCsDelay);
        DRAM_DDRPHY_WRITE32(0x330, u4SetCsDelay); // CS is here.
        DRAM_DDRPHY_WRITE32(0x334, u4SetCsDelay);
		DRAM_DDRPHY_WRITE32(0x338, u4SetCsDelay);
   
        Printf("CS=%02X : ", (0x80 |au4CsDelay[i]));

        for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
        {
            // Set clock delay.    
            u4SetClkDelay = u4ClkDelay;
            u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
            u4Val = DRAM_DDRPHY_READ32(0x304);
            u4Val &= ~ 0xFF;
            u4Val |= u4SetClkDelay;
            DRAM_DDRPHY_WRITE32(0x304, u4Val);

            // Start reset phy and redo dram auto init.
            _DDR_ResetPhy();

            // Do DQS calibration to get DQS window average value. 
            if (DDR_CalibrateDqs() > 0)
            {
                Printf("%c ", 'X');                
            }
            else
            {
                Printf("%c ", 'O');
            }           
        }

        // go to next CLK.
        Printf("\n");        
    }    
}
#endif /* #ifdef DRAM_CS_DELAY_CALIBRATION */

#ifdef DRAM_CMD_DELAY_CALIBRATION
static void _DdrCalCmdDelay(UINT32 u4CmdOffset, UINT32 u4CmdShift)
{
    UINT32 u4ClkDelay;
    UINT32 au4CsDelay[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
                                                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    UINT32 u4SetClkDelay;
    UINT32 u4SetCmdDelay;
    UINT32 u4Val;
    UINT32 u4Backup;
    UINT32 i;
    
    Printf("  CLK    : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");

    u4Backup = DRAM_DDRPHY_READ32(u4CmdOffset);
    
    for (i = 0; i<=0x0F; i++)	
    {       
        // Set CMD delay.
        u4SetCmdDelay = DRAM_DDRPHY_READ32(u4CmdOffset);
        u4SetCmdDelay &= ~ (0xFF << u4CmdShift);
        u4SetCmdDelay |= (au4CsDelay[i] << u4CmdShift);
        DRAM_DDRPHY_WRITE32(u4CmdOffset, u4SetCmdDelay);
   
        Printf("0x%X=%02X : ", (u4CmdOffset+(u4CmdShift/8)), au4CsDelay[i]);

        for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
        {
            // Set clock delay.    
            u4SetClkDelay = u4ClkDelay;
            u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
            u4Val = DRAM_DDRPHY_READ32(0x304);
            u4Val &= ~ 0xFF;
            u4Val |= u4SetClkDelay;
            DRAM_DDRPHY_WRITE32(0x304, u4Val);

            // Start reset phy and redo dram auto init.
            _DDR_ResetPhy();

            // Do DQS calibration to get DQS window average value. 
            if (DDR_CalibrateDqs() > 0)
            {
                Printf("%c ", 'X');                
            }
            else
            {
                Printf("%c ", 'O');
            }           
        }

        // go to next CLK.
        Printf("\n");        
    }    

    Printf("\nCMD set T/2 early.\n");
    Printf("  CLK    : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");
    
    for (i = 0; i<=0x0F; i++)	
    {       
        // Set CMD delay.
        u4SetCmdDelay = DRAM_DDRPHY_READ32(u4CmdOffset);
        u4SetCmdDelay &= ~ (0xFF << u4CmdShift);
        u4SetCmdDelay |= ((0x80|au4CsDelay[i]) << u4CmdShift);
        DRAM_DDRPHY_WRITE32(u4CmdOffset, u4SetCmdDelay);
   
        Printf("0x%X=%02X : ", (u4CmdOffset+(u4CmdShift/8)), (0x80 |au4CsDelay[i]));

        for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
        {
            // Set clock delay.    
            u4SetClkDelay = u4ClkDelay;
            u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
            u4Val = DRAM_DDRPHY_READ32(0x304);
            u4Val &= ~ 0xFF;
            u4Val |= u4SetClkDelay;
            DRAM_DDRPHY_WRITE32(0x304, u4Val);

            // Start reset phy and redo dram auto init.
            _DDR_ResetPhy();

            // Do DQS calibration to get DQS window average value. 
            if (DDR_CalibrateDqs() > 0)
            {
                Printf("%c ", 'X');                
            }
            else
            {
                Printf("%c ", 'O');
            }           
        }

        // go to next CLK.
        Printf("\n");        
    }    

    DRAM_DDRPHY_WRITE32(u4CmdOffset, u4Backup);    
}
#endif /* #ifdef DRAM_CS_DELAY_CALIBRATION */

#ifdef DRAM_MAIN_IC_ODT_CALIBRATION
static UINT32 _DdrCalMainIcODT(UINT32 u4ChipNum)
{
    UINT32 u4Rec;
    INT32 i4Min;
    INT32 i4Max;
    UINT32 u4MainChipODT[8]= {0, 75, 150, 50, 37,25, 30, 21};
    UINT32 i;
    UINT32 u4Byte;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    INT32 i4Avg;

    for (i=0; i<8; i++)
    {
        Printf("\n(%d) Main Chip ODT = %d, calibration.\n", i, u4MainChipODT[i]);   

        // Start reset phy and redo dram auto init.
        _DDR_ResetPhy();

        // Set main chip ODT.
        _DDR_SetRTT(u4MainChipODT[i]);

        CHIP_Delay_us(1000);

        Printf("0x7310=0x%08X, 0x7318=0x%08X.\n", 
            DRAM_READ32(0x310), DRAM_READ32(0x318));

        // Do DQS calibration to get DQS window average value. 
        if (DDR_CalibrateDqs() > 0)
        {
            Printf("......Fail.\n");
        }
        else
        {
            Printf("......Pass.\n");

            // Show calibration result.
            for (u4Byte=0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte++)
            {
                u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
            
                i4Avg = (DRAM_READ32(0x3A0) >> (u4HwDQSth*8)) & 0xFF;
                u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4Byte /2)*4));
                i4Min = i4Avg - (u4Rec >> ((u4Byte & 0x1)*16)) & 0xFF;
                i4Max = i4Avg + (u4Rec >> (((u4Byte & 0x1)*16) + 8)) & 0xFF;
                Printf("Byte %d : DQS(%2d ~ %2d), Size %d.\n", 
                    u4Byte, i4Min, i4Max, ((i4Max - i4Min) + 1));
            }
        }
    }

    return 0;
}
#endif /* #ifdef DRAM_MAIN_IC_ODT_CALIBRATION */

#ifdef DRAM_DRAM_ODT_CALIBRATION
static UINT32 _DdrCalDramODT(UINT32 u4ChipNum)
{
    UINT32 u4Val;    
    UINT32 u4Rec;
    INT32 i4Min;
    INT32 i4Max;
    UINT32 i;
    UINT32 u4Byte;
#ifdef CC_SUPPORT_DDR2    
    UINT32 u4EMRS[4]= {0x02, 0x42, 0x46, 0x06};
#endif
#ifdef CC_SUPPORT_DDR3
    UINT32 u4EMRS[4]= {0x00, 0x04, 0x40, 0x44};
#endif
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    INT32 i4Avg;

    for (i=0; i<4; i++)
    {
        Printf("\n(%d) DRAM EMRS = 0x%X, calibration.\n", i, u4EMRS[i]);   

        // Set DRAM ODT on EMRS.
        u4Val = DRAM_READ32(0x10);
        u4Val &= 0xFFFF;
        u4Val |= (u4EMRS[i] << 16);
        DRAM_WRITE32(0x10, u4Val);        

        // Start reset phy and redo dram auto init.
        _DDR_ResetPhy();

        CHIP_Delay_us(1000);

        Printf("0x7010=0x%08X.\n", DRAM_READ32(0x10));

        // Do DQS calibration to get DQS window average value. 
        if (DDR_CalibrateDqs() > 0)
        {
            Printf("......Fail.\n");
        }
        else
        {
            Printf("......Pass.\n");

            // Show calibration result.
            for (u4Byte=0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte++)
            {
                u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
            
                i4Avg = (DRAM_READ32(0x3A0) >> (u4HwDQSth*8)) & 0xFF;
                u4Rec = HAL_READ32(RECORD_DQS_REG + ((u4Byte /2)*4));
                i4Min = i4Avg - (u4Rec >> ((u4Byte & 0x1)*16)) & 0xFF;
                i4Max = i4Avg + (u4Rec >> (((u4Byte & 0x1)*16) + 8)) & 0xFF;
                
                Printf("Byte %d : DQS(%2d ~ %2d), Size %d.\n", 
                    u4Byte, i4Min, i4Max, ((i4Max - i4Min) + 1));
            }
        }
    }

    return 0;
}
#endif /* #ifdef DRAM_MAIN_IC_ODT_CALIBRATION */

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** DDR_ToolMain() to search DRAM parameter.
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
#ifdef DRAM_DEBUG_TOOL
void _DDR_ToolMain(void)
{
    UINT32 u4ChipNum = (IS_FORCE32()) ? 1 : 2;
#ifdef DRAM_GATING_DQS_CALIBRATION
    UINT32 u4DQSCtrlSet;
    UINT32 u4Gating1;
    UINT32 u4Gating2;
    UINT32 u4Gating3;    
#endif
#ifdef DRAM_CMD_DELAY_CALIBRATION
    UINT32 u4CmdOffset;
    UINT32 u4CmdShift;
#endif

#ifdef DRAM_GATING_DQS_CALIBRATION
    Printf("Show DQS Gating Window...\n");
    // Backup old setting.
    u4Gating1 = DRAM_READ32(0x388);
    u4Gating2 = DRAM_DDRPHY_READ32(0x380);
    u4Gating3 = DRAM_DDRPHY_READ32(0x384);

    u4DQSCtrlSet = _DdrCalGatingDqsCtrl();
    (void)_DdrCalGatingDqs(u4ChipNum, u4DQSCtrlSet);
    (void)_DdrCalGatingDqs(u4ChipNum, (u4DQSCtrlSet >> 1));    
    (void)_DdrCalGatingDqs(u4ChipNum, (u4DQSCtrlSet >> 2));    

    _DDR_ResetPhy();
    DRAM_WRITE32(0x388, u4Gating1);
    DRAM_DDRPHY_WRITE32(0x380, u4Gating2);
    DRAM_DDRPHY_WRITE32(0x384, u4Gating3);
#endif

#ifdef DRAM_CS_DELAY_CALIBRATION
    Printf("Show CLK to CS Map.\n");
    _DdrCalCsDelay();
#endif

#ifdef DRAM_CMD_DELAY_CALIBRATION
    Printf("Show CLK to CMD Map.\n");

    for (u4CmdOffset = 0x320; u4CmdOffset<=0x338; u4CmdOffset+=4)
    {
        for (u4CmdShift = 0; u4CmdShift<=24; u4CmdShift+=8)
        {
            Printf("\nShow CLK to CMD=0x%X Map.\n", (u4CmdOffset + (u4CmdShift/8)));
        
            _DdrCalCmdDelay(u4CmdOffset, u4CmdShift);
        }
    }
#endif

#ifdef DRAM_DUTY_DQS_CALIBRATION
    Printf("Show DQ/DQS RDSEL.\n");
    _DdrCalDuty(u4ChipNum);
#endif

#ifdef DRAM_WRITE_DQS_CALIBRATION
    Printf("Show Write DQS to DQ Map...\n");

    if (DDR_CalibrateDqs() == 0)
    {
        (void)_DdrCalOutDqs(u4ChipNum);
    }    
#endif

#ifdef DRAM_CLOCK_WRITE_DQS_CALIBRATION
    Printf("Show Clock to Write DQS to DQ Map...\n");

    if (DDR_CalibrateDqs() == 0)
    {
        (void)_DdrCalClkAndOutDqs(u4ChipNum);
    }    
#endif

#ifdef DRAM_MAIN_IC_ODT_CALIBRATION
    Printf("Show Main IC side ODT result...\n");
    (void)_DdrCalMainIcODT(u4ChipNum);
#endif

#ifdef DRAM_DRAM_ODT_CALIBRATION
    Printf("Show Dram side ODT result...\n");
    (void)_DdrCalDramODT(u4ChipNum);
#endif

    UNUSED(u4ChipNum);
}
#endif /* DRAM_DEBUG_TOOL */

#ifdef DRAM_WDQS_CALIBRATION
UINT32 DDR_CalibrateOutDqs(void)
{
    UINT32  u4ChipNum = (IS_FORCE32()) ? 1 : 2;
    UINT32  u4Byte;
    UINT32  u4DQS;
    UINT32  u4DQ;
    INT32   i4Max=0,i4Min=0,i4Avg=0;
    INT32   i4MinL, i4MaxL;
    BOOL    fgCalibrationFail = FALSE;

    CHIP_DisplayString("DRAM Output DQS Calibration.\n");

    DRAM_DDRPHY_WRITE32(0x360, 0);
    DRAM_DDRPHY_WRITE32(0x364, 0);
    DRAM_DDRPHY_WRITE32(0x368, 0);
    DRAM_DDRPHY_WRITE32(0x36C, 0);        
    DRAM_DDRPHY_WRITE32(0x354, 0);

    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {                
        _DDR_SetOutDQ(u4Byte, 0);
        for (u4DQS=0; u4DQS<MAX_WDQS_DELAY; u4DQS+=WDQS_CALIB_STEP)	
        {
            if (DDR_IsDqsFail(u4Byte, u4DQS, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
            {
            
                if (WDQS_CALIB_STEP > 1)
                {   
                    i4MaxL = u4DQS - WDQS_CALIB_STEP;
                    for (; u4DQS > i4MaxL ; u4DQS--)
                    {                         
                        if (!DDR_IsDqsFail(u4Byte, u4DQS, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
                        {
                            break;
                        }
                    }
                 }
                 break;
            }
        }
        
        if(u4DQS>=MAX_WDQS_DELAY)
        {
            i4Max=u4DQS-WDQS_CALIB_STEP;
        }
        else
        {
            i4Max=u4DQS;
        }
        
        for (u4DQ=0; u4DQ<MAX_WDQ_DELAY; u4DQ+=WDQS_CALIB_STEP)
        {
            _DDR_SetOutDQ(u4Byte, u4DQ);
            if (DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
            {                
                if (WDQS_CALIB_STEP > 1)
                {   
                    i4MinL = u4DQ - WDQS_CALIB_STEP;
                    for (; u4DQ > i4MinL ; u4DQ--)
                    {                         
                        _DDR_SetOutDQ(u4Byte, u4DQ);
                        if (!DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
                        {
                            break;
                        }
                    }
                 }
                 break;
            }
        }
        
        if(u4DQ>=MAX_WDQ_DELAY)
        {
            i4Min=-(u4DQ-WDQS_CALIB_STEP);
        }
        else
        {
            i4Min=-u4DQ;
        }

        i4Avg = (i4Min + i4Max) / 2;
        if(i4Avg >= 0)
        {
            _DDR_SetOutDQS(u4Byte, i4Avg);
            _DDR_SetOutDQ(u4Byte,0);
        }
        else
        {
            _DDR_SetOutDQS(u4Byte, 0);
            _DDR_SetOutDQ(u4Byte,-i4Avg);
        }
        
        if (i4Min <= i4Max)
        {
            CHIP_DisplayString("Byte ");
            CHIP_DisplayInteger(u4Byte);
            CHIP_DisplayString(" : DQS(");
            CHIP_DisplayInteger(i4Min);
            CHIP_DisplayString(" ~ ");
            CHIP_DisplayInteger(i4Max);
            CHIP_DisplayString("), Size ");
            CHIP_DisplayInteger(((i4Max - i4Min) + 1));
            CHIP_DisplayString(", Set ");
            CHIP_DisplayInteger(i4Avg);        
            CHIP_DisplayString(".\n");
        }
        else
        {
            CHIP_DisplayString("Byte ");
            CHIP_DisplayInteger(u4Byte);
            CHIP_DisplayString(" : DQS(");
            CHIP_DisplayInteger(i4Min);
            CHIP_DisplayString(" ~ ");
            CHIP_DisplayInteger(i4Max);
            CHIP_DisplayString("), Error !\n");
            fgCalibrationFail = TRUE;
        }
        
    }  
        
    return (UINT32)fgCalibrationFail;
}
#endif

#ifdef DRAM_GATING_SCAN
void _DDR_GatingScan(void)
{
    UINT32 u4Byte = 0;
    UINT32 u4OldVal = 0;
    UINT32 u4Max = 0;
    UINT32 u4Min = 0;
    UINT32 u4Avg = 0;
    UINT32 u4Val;    
    UINT32 u4SetVal = 0;    
    UINT32 u4Reg388 = DRAM_READ32(0x388);   
    UINT32 u4Step;
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    UINT32 u4TestVal; 
    UINT32 u4TimeOut; 
    UINT32 u4GatingStep = DQS_GATING_STEP;
#ifndef DQS_GATING_ONE_BYTE_MODE    
    UINT32 u4ChipNum = (IS_FORCE32()) ? 1 : 2;
#endif

    //Printf("Current DQSIEN=0x%08X.\n", u4Reg388);   

#ifndef DQS_GATING_ONE_BYTE_MODE   
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
#endif    
    {
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4Byte] : u4Byte;
        u4OldVal = (u4Reg388 >> (8*u4HwDQSth)) & 0x7F;

        // Check upper bound.        
        for(u4Step=u4GatingStep; u4Step<0x60; u4Step+=u4GatingStep)
        {       
            u4Val = (u4OldVal + u4Step) & 0x7F;
            if (u4Val > 0x60)
            {
                // Restore default gating window before PHY reset.
                DRAM_WRITE32(0x388, u4Reg388);
                // Reset PHY to recover DQS ring counter error.
                _DDR_ResetPhyOnly();                
                break;
            }

            u4SetVal = DRAM_READ32(0x388);
            u4SetVal &= ~ (0x7F << (8*u4HwDQSth));
            u4SetVal |= (u4Val << (8*u4HwDQSth));
            
            //Printf("(%d) 0x388=0x%08X...", u4Byte, u4SetVal);   
            
            DRAM_WRITE32(0x388, u4SetVal); // Need to do early	

            // Enable test agent.
            u4TestVal = DRAM_READ32(0x218);
            u4TestVal |= 0x30000000;
            DRAM_WRITE32(0x218, u4TestVal); // Need to do early	

            u4TimeOut = 0;
            while((u4TimeOut ++) < 1000)
            {
                CHIP_Delay_us(50);    
            
                u4TestVal = DRAM_READ32(0x280);
                if ((u4TestVal & 0x303) == 0x303)
                {
                    break;
                }
            }
            if (u4TimeOut >= 1000)
            {
                CHIP_DisplayString("Test agent fail.\n");
                // Use default setting.
                _DDR_ResetPhyOnly();                
                DRAM_WRITE32(0x388, u4Reg388);
                return;
            }

            // Disable test agent.
            u4TestVal = DRAM_READ32(0x218);
            u4TestVal &= ~0x30000000;
            DRAM_WRITE32(0x218, u4TestVal); // Need to do early	

            u4TestVal = (0xFF << (u4HwDQSth * 8));
            if (((DRAM_READ32(0x2A8) & u4TestVal) > 0)
                ||((DRAM_READ32(0x2AC) & u4TestVal) > 0))
            {
                //Printf("Fail.\n");   
                u4Max = (u4Val > u4OldVal) ? (u4Val - u4GatingStep) : u4OldVal;
                // Use default setting.
                _DDR_ResetPhyOnly();                
                DRAM_WRITE32(0x388, u4Reg388);
                break;
            }
            else
            {
                u4Max = u4Val;
                //Printf("Pass..Max=%d.\n", u4Val);   
            }            
        }

        // Check lower bound.
        for(u4Step=u4GatingStep; u4Step<0x60; u4Step=u4Step+u4GatingStep)
        {
            if (u4OldVal > u4Step)
            {
                u4Val = (u4OldVal - u4Step) & 0x7F;
                u4SetVal = DRAM_READ32(0x388);
                u4SetVal &= ~ (0x7F << (8*u4HwDQSth));
                u4SetVal |= (u4Val << (8*u4HwDQSth));

                //Printf("(%d) 0x388=0x%08X...", u4Byte, u4SetVal);   
                
                DRAM_WRITE32(0x388, u4SetVal); // Need to do early	

                // Enable test agent.
                u4TestVal = DRAM_READ32(0x218);
                u4TestVal |= 0x30000000;
                DRAM_WRITE32(0x218, u4TestVal); // Need to do early	

                u4TimeOut = 0;
                while((u4TimeOut ++) < 1000)
                {
                    CHIP_Delay_us(50);    
                
                    u4TestVal = DRAM_READ32(0x280);
                    if ((u4TestVal & 0x303) == 0x303)
                    {
                        break;
                    }
                }
                if (u4TimeOut >= 1000)
                {
                    CHIP_DisplayString("Test agent fail.\n");
                    // Use default setting.
                    _DDR_ResetPhyOnly();                
                    DRAM_WRITE32(0x388, u4Reg388);
                    return;
                }

                // Disable test agent.
                u4TestVal = DRAM_READ32(0x218);
                u4TestVal &= ~0x30000000;
                DRAM_WRITE32(0x218, u4TestVal); // Need to do early	

                u4TestVal = (0xFF << (u4HwDQSth * 8));
                if (((DRAM_READ32(0x2A8) & u4TestVal) > 0)
                    ||((DRAM_READ32(0x2AC) & u4TestVal) > 0))
                {
                    //Printf("Fail.\n");   
                    u4Min = (u4Val < u4OldVal) ? (u4Val + u4GatingStep) : u4OldVal;                    
                    // Use default setting.
                    _DDR_ResetPhyOnly();                
                    DRAM_WRITE32(0x388, u4Reg388);
                    break;                    
                }
                else
                {
                    u4Min = u4Val;      
                    //Printf("Pass..Min=%d.\n", u4Val);                       
                }                
            }
        }        

        // Update 0x388 register new value.
        // Get Max and Min => Get Avg.
        u4Avg = (u4Max + u4Min) / 2;

        if (u4Min >= u4Max)
        {
            u4Val = u4OldVal;           
        }
        else
        {
            u4Val = ((u4Avg + DQS_GATING_OFFSET) < 96) ?  
                (u4Avg + DQS_GATING_OFFSET) : 95;           
        }

        CHIP_DisplayString("Byte ");
        CHIP_DisplayInteger(u4Byte);
        CHIP_DisplayString(" : Gating(");
        CHIP_DisplayInteger(u4Min);
        CHIP_DisplayString(" ~ ");
        CHIP_DisplayInteger(u4Max);
        CHIP_DisplayString("), Size=");
        CHIP_DisplayInteger(((u4Max - u4Min) + 1));
        CHIP_DisplayString(", Mid=");
        CHIP_DisplayInteger(u4Avg);        
        CHIP_DisplayString(", Set=");
        CHIP_DisplayInteger(u4Val);
        CHIP_DisplayString(".\n");

        if ((u4Max <= u4Min) ||
            (((u4Max - u4Min) + 1) < 30))
        {
            CHIP_DisplayString("Gating size too small !!\n");        
            // Use default setting.
            _DDR_ResetPhyOnly();                
            DRAM_WRITE32(0x388, u4Reg388);
            return;
        }        

#ifndef DQS_GATING_ONE_BYTE_MODE   
        u4Reg388 &= ~(0xFF << (u4HwDQSth * 8));
        u4Reg388 |= u4Val << (u4HwDQSth * 8);
#endif        
    }

#ifdef DQS_GATING_ONE_BYTE_MODE   
    u4Reg388 = (u4Val << 24) | (u4Val << 16) | (u4Val << 8) | (u4Val);
#endif

    DRAM_WRITE32(0x388, u4Reg388);

    return;
}
#endif

#ifdef DRAM_WDQS_PERBITCALIBRATION
void DDR_PerbitCalibrateOutDqs(void)
{
    UINT32   u4Val,u4Val2;
    INT32   u4PerBitErrMin[32];
    INT32   u4PerBitErrMax[32];
    INT32   u4PerBitErrMid[32];
    INT32   u4PerBitErrDly;
    INT32   u4PerBitErrMidMax[4];
    UINT32   u4DQDelay=0,u4DQSDelay=0;
    UINT32   i;
    UINT32   u4Reg218;
    UINT32   u4Reg280;
    UINT32   u4Reg280_ori;
    UINT32   u4Reg284;
    
    for(i=0;i<32;i++)
    {
        u4PerBitErrMin[i]=-15;
        u4PerBitErrMax[i]=15;
    }
    #if 0// SW DQS check
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
          if (DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_OUTPUT_TYPE, u4ChipNum))
           {
                    Printf("****** Byte=%d   Per Bit Start NG******.\n", u4Byte);
                    
            }
                else
            {
                    Printf("****** Byte=%d    Per Bit Start OK******.\n", u4Byte);
            }
            
    }
    #endif
    Printf("TX HW DQS  DQ Calibration Start: \n");
    
    u4Reg218 = DRAM_READ32(0x218);
    DRAM_WRITE32(0x218, u4Reg218 &0xFF0FFFFF);//SY recommand to set 0x7218[23:20]=0 when TX calibration
    //left margin
    DRAM_WRITE32(0x354,0);
    
    for(u4DQDelay=1;u4DQDelay<0x10;u4DQDelay++)
    {
        u4Val =(u4DQDelay<<28)|(u4DQDelay<<24)|(u4DQDelay<<20)|(u4DQDelay<<16)|(u4DQDelay<<12)|(u4DQDelay<<8)|(u4DQDelay<<4)|u4DQDelay;
        DRAM_DDRPHY_WRITE32(0x360,u4Val);
        DRAM_DDRPHY_WRITE32(0x364,u4Val);
        DRAM_DDRPHY_WRITE32(0x368,u4Val);
        DRAM_DDRPHY_WRITE32(0x36C,u4Val);
        
        //Enable test agent-2
        u4Val2 = DRAM_READ32(0x218);
        u4Val2 &= ~0xF0000000;
        DRAM_WRITE32(0x218, u4Val2 |0x80000000);

        do
        {
            u4Reg280_ori= DRAM_READ32(0x280);
            u4Reg280 = u4Reg280_ori & 0x444;
        }while((u4Reg280 != 0x404) && (u4Reg280 != 0x444));
        
       //Printf("Min Cal,DQ delay = 0x%08X ; Test agent status=0x%08X .\n",u4Val,u4Reg280_ori);
        
        if(u4Reg280 == 0x444)
            {
                u4Val2 = DRAM_READ32(0x218);
                u4Val2 &= ~0xF0000000;
                DRAM_WRITE32(0x218, u4Val2);
                
                u4Val2 = DRAM_READ32(0x218);
                u4Val2 &= ~0xF0000000;
                DRAM_WRITE32(0x218, u4Val2 | 0x40000000);

                do
                {
                    u4Reg280_ori= DRAM_READ32(0x280);
                    u4Reg280 = u4Reg280_ori & 0x444;
                }while((u4Reg280 != 0x404) && (u4Reg280 != 0x444));
  
                u4Reg284 = DRAM_READ32(0x284);

                for(i=0 ; i<32 ; i++)
                    {
                        if(((u4Reg284 >> i) & (0x00000001)) != 0)
                            {
                                u4PerBitErrMin[i]++;
                            }
                    }

                //Printf("     -----Test agent2 DQ Calibration Error,u4Reg284 = 0x%08X .\n",u4Reg284);
            }
        
        //Disable test agent2
        u4Val2 = DRAM_READ32(0x218);
        u4Val2 &= ~0xF0000000;
        DRAM_WRITE32(0x218, u4Val2);
    }

    //right margin
    DRAM_DDRPHY_WRITE32(0x360,0);
    DRAM_DDRPHY_WRITE32(0x364,0);
    DRAM_DDRPHY_WRITE32(0x368,0);
    DRAM_DDRPHY_WRITE32(0x36C,0);
    
    for(u4DQSDelay=1;u4DQSDelay<0x10;u4DQSDelay++)
    {
        u4Val =(u4DQSDelay<<12)|(u4DQSDelay<<8)|(u4DQSDelay<<4)|u4DQSDelay;
        
        DRAM_DDRPHY_WRITE32(0x354,u4Val);       
        
         //Enable Test agent-2
        u4Val2 = DRAM_READ32(0x218);
        u4Val2 &= ~0xF0000000;
        DRAM_WRITE32(0x218, u4Val2 |0x80000000);

        //wait cal complete
        do
        {
            u4Reg280_ori= DRAM_READ32(0x280);
            u4Reg280 = u4Reg280_ori & 0x444;
         }while((u4Reg280 != 0x404) && (u4Reg280 != 0x444));
       // Printf("Max Cal,DQS delay = 0x%08X ; Test agent status=0x%08X .\n",u4Val,u4Reg280_ori);

        //read DQ calibration error register
        if(u4Reg280 == 0x444)
            {
                u4Val2 = DRAM_READ32(0x218);
                u4Val2 &= ~0xF0000000;
                DRAM_WRITE32(0x218, u4Val2);
                
                u4Val2 = DRAM_READ32(0x218);
                u4Val2 &= ~0xF0000000;
                DRAM_WRITE32(0x218, u4Val2 | 0x40000000);

                do
                {
                    u4Reg280_ori= DRAM_READ32(0x280);
                    u4Reg280 = u4Reg280_ori & 0x444;
                 }while((u4Reg280 != 0x404) && (u4Reg280 != 0x444));
  
                u4Reg284 = DRAM_READ32(0x284);

                for(i=0 ; i<32 ; i++)
                    {
                        if(((u4Reg284 >> i) & (0x00000001)) != 0)
                            {
                                u4PerBitErrMax[i]--;
                            }
                    }
                    //Printf("     -----Test agent2 DQ Calibration Error,u4Reg284 = 0x%08X .\n",u4Reg284);
            }
        
        //Disable Test agent-2
        u4Val2 = DRAM_READ32(0x218);
        u4Val2 &= ~0xF0000000;
        DRAM_WRITE32(0x218, u4Val2);
        
    }
    //output per bit margin
    for(i=0;i<32;i++)
    {
         u4PerBitErrMid[i]=(u4PerBitErrMin[i]+u4PerBitErrMax[i])/2;
    	//  Printf("bit %d margin:%d~%d,mid centre=%d.\n",i,u4PerBitErrMin[i],u4PerBitErrMax[i],u4PerBitErrMid[i]);
    }
    
    //----------------------------------start per bit deskew---------------------------
    u4PerBitErrMidMax[0] = 0;
    u4PerBitErrMidMax[1] = 0;
    u4PerBitErrMidMax[2] = 0;
    u4PerBitErrMidMax[3] = 0;
    //find max margin centre for per byte
    for(i=0;i<32;i++)
    {
        u4PerBitErrMid[i]=(u4PerBitErrMax[i] + u4PerBitErrMin[i])/2;
        if(u4PerBitErrMid[i] > u4PerBitErrMidMax[i/8])
            u4PerBitErrMidMax[i/8]=u4PerBitErrMid[i];
    }
    for(i=0;i<4;i++)
        {
            if(u4PerBitErrMidMax[i]<0)
            u4PerBitErrMidMax[i]=0;
            
        //    Printf("Byte %d max middle margin = %d.\n",i,u4PerBitErrMidMax[i]);
        }
    
    //--------------------------per bit deskew-----------------------
    //set DQ delay
    DRAM_DDRPHY_WRITE32(0x360,0);
    DRAM_DDRPHY_WRITE32(0x364,0);
    DRAM_DDRPHY_WRITE32(0x368,0);
    DRAM_DDRPHY_WRITE32(0x36C,0);
    for(i=0;i<32;i++)
    {
        u4PerBitErrDly=u4PerBitErrMidMax[i/8]-u4PerBitErrMid[i];
       // u4RegDly=u4PerBitErrDly;
        if(u4PerBitErrMidMax[i/8] < 0)
        	{
        		  u4PerBitErrDly = u4PerBitErrDly - u4PerBitErrMidMax[i/8];
        	}
        u4PerBitErrDly=u4PerBitErrDly << ((i%8)*4);
        DRAM_DDRPHY_WRITE32(0x360 + (i/8)*4,DRAM_DDRPHY_READ32(0x360 + (i/8)*4) | u4PerBitErrDly);
        //Printf("bit %d margin:%d~%d,mid centre=%d,DQ delay setting=%d.\n",i,u4PerBitErrMin[i],u4PerBitErrMax[i],u4PerBitErrMid[i],u4RegDly);
    }
    Printf("DQ BYTE 0 TX Delay=0x%08X.\n",DRAM_DDRPHY_READ32(0x360));
    Printf("DQ BYTE 1 TX Delay=0x%08X.\n",DRAM_DDRPHY_READ32(0x364));
    Printf("DQ BYTE 2 TX Delay=0x%08X.\n",DRAM_DDRPHY_READ32(0x368));
    Printf("DQ BYTE 3 TX Delay=0x%08X.\n",DRAM_DDRPHY_READ32(0x36c));
    //set DQS delay
    DRAM_DDRPHY_WRITE32(0x354,0);
    u4DQSDelay=0;
    for(i=0;i<4;i++)
    {
    	  if(u4PerBitErrMidMax[i]>0)
    	  	{
    	  		  u4DQSDelay|=u4PerBitErrMidMax[i] << (4*i);
    	  		 
    	  	}
    }
     DRAM_DDRPHY_WRITE32(0x354,u4DQSDelay);
     Printf("DQSDelay=0x%08X.\n",u4DQSDelay);
    //revert DQS & DQ delay setting
    //DRAM_WRITE32(0x354, u4Reg354);
    //DRAM_WRITE32(0x360, u4Reg360);
    //DRAM_WRITE32(0x364, u4Reg364);
    //DRAM_WRITE32(0x368, u4Reg368);
    //DRAM_WRITE32(0x36C, u4Reg36C);

    DRAM_WRITE32(0x218, u4Reg218);//load back 0x7218 setting
}
#endif

