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

#ifdef DRAM_DEBUG_TOOL
//lint --e{717} do ... while (0);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

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
    INT32 i4Min;
    INT32 i4Max;
    UINT32 u4Size;
    UINT32 u4DQRDSEL;
    UINT32 u4MaxDQRDSEL;
    UINT32 u4DQSRDSEL;
    UINT32 u4MaxDQSRDSEL;
    UINT32 i;

    DRAM_WRITE32(0x398, 0);
    DRAM_WRITE32(0x39C, 0);

    for (i=0; i<20; i++)
    {
        // Start from DQRDSEL.
        for (u4Byte=0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte++)
        {
            // Fix DQxRDSEL[3:2] = 0, search DQxRDSEL[1:0].
            u4MaxSize = 0;
            for (u4DQRDSEL=0; u4DQRDSEL<4; u4DQRDSEL++)
            {        
                u4Val = DRAM_READ32(0x39C);
                u4Val &= ~(0x3F << (8*u4Byte));
                u4Val |= (u4DQRDSEL << (8*u4Byte)); 
                DRAM_WRITE32(0x39C, u4Val);

                //Printf("DQRDSEL=0x%X, 0x739C=0x%08X.\n", u4DQRDSEL, u4Val);
                
                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    u4Rec = HAL_READ32(RECORD_DQS_REG + u4Byte * 4);	        
                    i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF-(u4Rec >> 24)+1) : (u4Rec >> 24);
                    i4Max = (u4Rec >> 16) & 0xFF;
                    u4Size = (UINT32)(i4Max- i4Min + 1);
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQRDSEL = u4DQRDSEL;
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
            for (u4DQRDSEL=1; u4DQRDSEL<4; u4DQRDSEL++)
            {
                u4Val = DRAM_READ32(0x39C);
                u4Val &= ~(0x3F << (8*u4Byte));
                u4Val |= ((u4DQRDSEL << 2) << (8*u4Byte));            
                DRAM_WRITE32(0x39C, u4Val);

                //Printf("DQRDSEL=0x%X, 0x739C=0x%08X.\n", (u4DQRDSEL << 2), u4Val);

                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    u4Rec = HAL_READ32(RECORD_DQS_REG + u4Byte * 4);	        
                    i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF-(u4Rec >> 24)+1) : (u4Rec >> 24);                    
                    i4Max = (u4Rec >> 16) & 0xFF;
                    u4Size = (UINT32)(i4Max- i4Min + 1);
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQRDSEL = (u4DQRDSEL << 2);
                    }
                }
                else
                {
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                }
            }                        

            u4Val = DRAM_READ32(0x39C);
            u4Val &= ~(0x3F << (8*u4Byte));
            u4Val |= (u4MaxDQRDSEL << (8*u4Byte));            
            DRAM_WRITE32(0x39C, u4Val);

            //Printf("Byte %d : MaxDQSSize = %d, MaxDQRDSEL = 0x%X.\n", u4Byte, u4MaxSize, u4MaxDQRDSEL);
        }

        // Next to search DQSRDSEL.
        for (u4Byte=0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte++)
        {
            // Fix DQSxRDSEL[3:2] = 0, search DQSxRDSEL[1:0].
            u4MaxSize = 0;
            for (u4DQSRDSEL=0; u4DQSRDSEL<4; u4DQSRDSEL++)
            {
                u4Val = DRAM_READ32(0x398);
                u4Val &= ~(0x3F << (8*u4Byte));
                u4Val |= (u4DQSRDSEL << (8*u4Byte));            
                DRAM_WRITE32(0x398, u4Val);

                //Printf("DQSRDSEL=0x%X, 0x7398=0x%08X.\n", u4DQSRDSEL, u4Val);

                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    u4Rec = HAL_READ32(RECORD_DQS_REG + u4Byte * 4);	        
                    i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF-(u4Rec >> 24)+1) : (u4Rec >> 24);                    
                    i4Max = (u4Rec >> 16) & 0xFF;
                    u4Size = (UINT32)(i4Max- i4Min + 1);
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQSRDSEL = u4DQSRDSEL;
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
            for (u4DQSRDSEL=1; u4DQSRDSEL<4; u4DQSRDSEL++)
            {
                u4Val = DRAM_READ32(0x398);
                u4Val &= ~(0x3F << (8*u4Byte));
                u4Val |= ((u4DQSRDSEL << 2) << (8*u4Byte));            
                DRAM_WRITE32(0x398, u4Val);

                //Printf("DQSRDSEL=0x%X, 0x7398=0x%08X.\n", (u4DQRDSEL << 2), u4Val);

                CHIP_Delay_us(300);
                
                if (DDR_CalibrateDqs() == 0)
                {
                    u4Rec = HAL_READ32(RECORD_DQS_REG + u4Byte * 4);	        
                    i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF-(u4Rec >> 24)+1) : (u4Rec >> 24);                    
                    i4Max = (u4Rec >> 16) & 0xFF;
                    u4Size = (UINT32)(i4Max- i4Min + 1);
                    if (u4Size > u4MaxSize)
                    {
                        u4MaxSize = u4Size;
                        u4MaxDQSRDSEL = (u4DQSRDSEL << 2);
                    }
                }
                else
                {
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                }
            }                        

            u4Val = DRAM_READ32(0x398);
            u4Val &= ~(0x3F << (8*u4Byte));
            u4Val |= (u4MaxDQSRDSEL << (8*u4Byte));            
            DRAM_WRITE32(0x398, u4Val);

            //Printf("Byte %d : MaxDQSSize = %d, MaxDQSRDSEL = 0x%X.\n", u4Byte, u4MaxSize, u4MaxDQSRDSEL);
        }    

        Printf("Result(%2d): DQSRDSEL = 0x%08X, DQRDSEL = 0x%08X.\n", 
            i, DRAM_READ32(0x398), DRAM_READ32(0x39C));    
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

    u4Reg360[0] = DRAM_READ32(0x360);
    u4Reg360[1] = DRAM_READ32(0x364);
    u4Reg360[2] = DRAM_READ32(0x368);
    u4Reg360[3] = DRAM_READ32(0x36C);
    u4Reg354 = DRAM_READ32(0x354);
    
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
        Printf("****** Byte=%d ******.\n", u4Byte);
        Printf("    DQ : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");

        DRAM_WRITE32(0x360, u4Reg360[0]);
        DRAM_WRITE32(0x364, u4Reg360[1]);
        DRAM_WRITE32(0x368, u4Reg360[2]);
        DRAM_WRITE32(0x36C, u4Reg360[3]);        
        DRAM_WRITE32(0x354, u4Reg354);                

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
    
    DRAM_WRITE32(0x360, u4Reg360[0]);
    DRAM_WRITE32(0x364, u4Reg360[1]);
    DRAM_WRITE32(0x368, u4Reg360[2]);
    DRAM_WRITE32(0x36C, u4Reg360[3]);        
    DRAM_WRITE32(0x354, u4Reg354);                

    return u4Ret;
}
#endif /* #ifdef DRAM_WRITE_DQS_CALIBRATION */

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
        DRAM_WRITE32(0x380, u4Val);
        DRAM_WRITE32(0x384, u4Val);
        
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
    DRAM_WRITE32(0x380, u4DQSCtrlSet);
    DRAM_WRITE32(0x384, u4DQSCtrlSet);

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

    DRAM_WRITE32(0x380, u4DQSCtrlSet);
    DRAM_WRITE32(0x384, u4DQSCtrlSet);
    u4Reg388 = DRAM_READ32(0x388);   

    // Reset PHY to recover DQS ring counter error.
    _DDR_ResetPhy();
    
    Printf("Current DQSCTL=0x%08X, DQSIEN=0x%08X.\n", u4DQSCtrlSet, u4Reg388);   
   
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
        u4OldVal[u4Byte] = (u4Reg388 >> (8*u4Byte)) & 0x7F;
        
        for(u4Step=0; u4Step<0x80; u4Step=u4Step+3)
        {       
            u4Val = (u4OldVal[u4Byte] + u4Step) & 0x7F;
            u4SetVal = DRAM_READ32(0x388);
            u4SetVal &= ~ (0x7F << (8*u4Byte));
            u4SetVal |= (u4Val << (8*u4Byte));
            DRAM_WRITE32(0x388, u4SetVal); // Need to do early	

            // DRAM read/write test, u4Bin parameter is no meaning.
            if (DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_NO_CHANGE_TYPE, u4ChipNum))
            {
                // Restore default gating window before PHY reset.
                DRAM_WRITE32(0x388, u4Reg388);

                // Reset PHY to recover DQS ring counter error.
                _DDR_ResetPhy();
                break;
            }
            else
            {
                u4Max[u4Byte] = u4Val;
            }
        }
       
        for(u4Step=0; u4Step<0x80; u4Step=u4Step+3)
        {
            if (u4OldVal[u4Byte] > u4Step)
            {
                u4Val = (u4OldVal[u4Byte] - u4Step) & 0x7F;
                u4SetVal = DRAM_READ32(0x388);
                u4SetVal &= ~ (0x7F << (8*u4Byte));
                u4SetVal |= (u4Val << (8*u4Byte));
                DRAM_WRITE32(0x388, u4SetVal); // Need to do early	

                // DRAM read/write test, u4Bin parameter is no meaning.
                if (DDR_IsDqsFail(u4Byte, 0, DRAM_DQS_NO_CHANGE_TYPE, u4ChipNum))
                {
                    // Restore default gating window before PHY reset.
                    DRAM_WRITE32(0x388, u4Reg388);
                    
                    // Reset PHY to recover DQS ring counter error.
                    _DDR_ResetPhy();
                    break;                    
                }
                else
                {
                    u4Min[u4Byte] = u4Val;                  
                }                
            }
        }        
    }

    u4Reg388 = 0;
    for (u4Byte = 0; u4Byte<(u4ChipNum*BYTE_PER_CHIP); u4Byte ++)	
    {
        // Get Max and Min => Get Avg.
        u4Avg[u4Byte] = (u4Max[u4Byte] + u4Min[u4Byte]) / 2;

        u4Reg388 |= u4Avg[u4Byte] << (u4Byte * 8);

        Printf("Byte %d : Gating (%2d ~ %2d), Size %d, Set = %d.\n", 
            u4Byte, u4Min[u4Byte], u4Max[u4Byte], 
            ((u4Max[u4Byte] - u4Min[u4Byte]) + 1), u4Avg[u4Byte]);
    }
    Printf("Set DQSCTL=0x%08X, DQSIEN=0x%08X.\n", 
        (DRAM_READ32(0x380) & 0xFFF), u4Reg388);   
    
    DRAM_WRITE32(0x388, u4Reg388);

    return;
}
#endif /* #ifdef DRAM_GATING_DQS_CALIBRATION */

#ifdef DRAM_CS_DELAY_CALIBRATION
static void _DdrCalCsDelay(void)
{
    UINT16 u2GrayBinaryCoded[32] = {
        0x00, 0x01, 0x03, 0x02, 0x07, 0x06, 0x04, 0x05, 0x0F, 0x0E, 0x0C, 0x0D, 0x08, 0x09, 0x0B, 0x0A,
        0x1F, 0x1E, 0x1C, 0x1D, 0x18, 0x19, 0x1B, 0x1A, 0x10, 0x11, 0x13, 0x12, 0x17, 0x16, 0x14, 0x15};
    UINT32 u4ClkDelay;
    UINT32 u4CsDelay;
    UINT32 u4SetClkDelay;
    UINT32 u4SetCsDelay;
    UINT32 u4Val;
    UINT32 u4Backup[2];

    Printf("  CLK : 0 1 2 3 4 5 6 7 8 9 A B C D E F.\n");
       
    for (u4CsDelay = 0; u4CsDelay<=0x1F; u4CsDelay++)	
    {       
        // Set chip select delay.
        u4SetCsDelay = u2GrayBinaryCoded[u4CsDelay];
        u4Val = 0;
        u4Val |= u4SetCsDelay;
        u4SetCsDelay = (u4Val << 24)|(u4Val << 16)|(u4Val << 8)|(u4Val);

        // Set command delay.
        DRAM_WRITE32(0x320, u4SetCsDelay);
        DRAM_WRITE32(0x324, u4SetCsDelay);
        DRAM_WRITE32(0x328, u4SetCsDelay);
        DRAM_WRITE32(0x32C, u4SetCsDelay);
        DRAM_WRITE32(0x330, u4SetCsDelay); // CS is here.
        DRAM_WRITE32(0x334, u4SetCsDelay);
   
        Printf("CS=%02X : ", u4CsDelay);

        for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
        {
            // Set clock delay.    
            u4SetClkDelay = u2GrayBinaryCoded[u4ClkDelay];
            u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
            u4Val = DRAM_READ32(0x304);
            u4Val &= ~ 0xFF;
            u4Val |= u4SetClkDelay;
            DRAM_WRITE32(0x304, u4Val);

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
    
    for (u4CsDelay = 0; u4CsDelay<=0x1F; u4CsDelay++)	
    {       
        // Set chip select delay.
        u4SetCsDelay = u2GrayBinaryCoded[u4CsDelay];
        u4Val = 0;        
        u4Val |= (0x80 | u4SetCsDelay);
        u4SetCsDelay = (u4Val << 24)|(u4Val << 16)|(u4Val << 8)|(u4Val);

        // Set command delay.
        DRAM_WRITE32(0x320, u4SetCsDelay);
        DRAM_WRITE32(0x324, u4SetCsDelay);
        DRAM_WRITE32(0x328, u4SetCsDelay);
        DRAM_WRITE32(0x32C, u4SetCsDelay);
        DRAM_WRITE32(0x330, u4SetCsDelay); // CS is here.
        DRAM_WRITE32(0x334, u4SetCsDelay);
   
        Printf("CS=%02X : ", (0x80 |u4CsDelay));

        for (u4ClkDelay = 0; u4ClkDelay<=0x0F; u4ClkDelay++)	
        {
            // Set clock delay.    
            u4SetClkDelay = u2GrayBinaryCoded[u4ClkDelay];
            u4SetClkDelay = (u4SetClkDelay << 4) | u4SetClkDelay;
            u4Val = DRAM_READ32(0x304);
            u4Val &= ~ 0xFF;
            u4Val |= u4SetClkDelay;
            DRAM_WRITE32(0x304, u4Val);

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

#ifdef DRAM_MAIN_IC_ODT_CALIBRATION
static UINT32 _DdrCalMainIcODT(UINT32 u4ChipNum)
{
    UINT32 u4Val;    
    UINT32 u4Rec;
    INT32 i4Min;
    INT32 i4Max;
    UINT32 u4Size;
    UINT32 u4MainChipODT[8]= {0, 75, 150, 50, 37,25, 30, 21};
    UINT32 i;
    UINT32 u4Byte;

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
                u4Rec = HAL_READ32(RECORD_DQS_REG + (u4Byte * 4));
                i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF-(u4Rec >> 24)+1) : (u4Rec >> 24);                    
                i4Max = (u4Rec >> 16) & 0xFF;
                u4Size = (UINT32)(i4Max- i4Min + 1);
                Printf("Byte %d : DQS(%2d ~ %2d), Size %d.\n", 
                    u4Byte, i4Min, i4Max, ((i4Max - i4Min) + 1));
            }
        }
    }
}
#endif /* #ifdef DRAM_MAIN_IC_ODT_CALIBRATION */

#ifdef DRAM_DRAM_ODT_CALIBRATION
static UINT32 _DdrCalDramODT(UINT32 u4ChipNum)
{
    UINT32 u4Val;    
    UINT32 u4Rec;
    INT32 i4Min;
    INT32 i4Max;
    UINT32 u4Size;
    UINT32 i;
    UINT32 u4Byte;
#ifdef CC_SUPPORT_DDR2    
    UINT32 u4EMRS[4]= {0x02, 0x42, 0x46, 0x06};
#endif
#ifdef CC_SUPPORT_DDR3
    UINT32 u4EMRS[4]= {0x00, 0x04, 0x40, 0x44};
#endif

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
                u4Rec = HAL_READ32(RECORD_DQS_REG + (u4Byte * 4));
                i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF-(u4Rec >> 24)+1) : (u4Rec >> 24);                    
                i4Max = (u4Rec >> 16) & 0xFF;
                u4Size = (UINT32)(i4Max- i4Min + 1);
                Printf("Byte %d : DQS(%2d ~ %2d), Size %d.\n", 
                    u4Byte, i4Min, i4Max, ((i4Max - i4Min) + 1));
            }
        }
    }
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
void _DDR_ToolMain(void)
{
    UINT32 u4ChipNum = (IS_FORCE32()) ? 1 : 2;
    UINT32 u4DQSCtrlSet;

#ifdef DRAM_GATING_DQS_CALIBRATION
    Printf("Show DQS Gating Window...\n");
    u4DQSCtrlSet = _DdrCalGatingDqsCtrl();
    (void)_DdrCalGatingDqs(u4ChipNum, u4DQSCtrlSet);
    (void)_DdrCalGatingDqs(u4ChipNum, (u4DQSCtrlSet >> 1));    
#endif

#ifdef DRAM_CS_DELAY_CALIBRATION
    Printf("Show CLK to CS Map.\n");
    _DdrCalCsDelay();
#endif

#ifdef DRAM_DUTY_DQS_CALIBRATION
    Printf("Show DQ/DQS RDSEL.\n");
    _DdrCalDuty(u4ChipNum);
#endif

#ifdef DRAM_WRITE_DQS_CALIBRATION
    Printf("Show Write DQS to DQ Map...\n");
    (void)_DdrCalOutDqs(u4ChipNum);
#endif

#ifdef DRAM_MAIN_IC_ODT_CALIBRATION
    Printf("Show Main IC side ODT result...\n");
    (void)_DdrCalMainIcODT(u4ChipNum);
#endif

#ifdef DRAM_DRAM_ODT_CALIBRATION
    Printf("Show Dram side ODT result...\n");
    (void)_DdrCalDramODT(u4ChipNum);
#endif

}
#endif /* DRAM_DEBUG_TOOL */
