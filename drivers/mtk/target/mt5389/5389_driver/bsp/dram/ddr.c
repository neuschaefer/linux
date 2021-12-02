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
/** @file ddr.c
 *  Dram control and setting.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "ddr.h"
#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_serial.h"
#include "x_dram.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef CC_MTK_PRELOADER

static void _DdrSetDramBeforeAI(void)
{

#ifdef CC_SUPPORT_SDR
    _DDR_SetSdrBeforeAI();
#endif /* #ifdef CC_SUPPORT_SDR */

#ifdef CC_SUPPORT_DDR1
    _DDR_SetDdr1BeforeAI();
#endif /* #ifdef CC_SUPPORT_DDR1 */

#ifdef CC_SUPPORT_DDR2
    _DDR_SetDdr2BeforeAI();
#endif /* #ifdef CC_SUPPORT_DDR2 */

#ifdef CC_SUPPORT_DDR3
     _DDR_SetDdr3BeforeAI();
#endif /* #ifdef CC_SUPPORT_DDR3 */

    CHIP_Delay_us(100);
}

static void _DdrSetDramRefresh(void)
{
    UINT32 u4Clk = TCMGET_DDR_CLK();
    UINT32 u4MemClk;
    UINT32 u4RefreshCount;

    u4MemClk = u4Clk/2;

    // 78: tREFI = 7.8 us. 39: tREFI = 3.9 us.
    // Set 50: tREFI = 5.0 us.
    u4RefreshCount = ((u4MemClk / 1000000) * 50) / 160;       

    // Full frequency mode when DDRI, SDR and frequency < 400Mhz.
    u4RefreshCount = u4RefreshCount / 2;

    u4RefreshCount &= 0xFF;
    
    // Set REFCNT by tREFI and enable ADVREFEN.
    DRAM_WRITE32(0x08, 0x200 |u4RefreshCount);
}

static void _DdrSetDramAfterAI(void)
{

#ifdef CC_SUPPORT_SDR
    _DDR_SetSdrAfterAI();
#endif /* #ifdef CC_SUPPORT_SDR */

#ifdef CC_SUPPORT_DDR1
    _DDR_SetDdr1AfterAI();
#endif /* #ifdef CC_SUPPORT_DDR1 */

#ifdef CC_SUPPORT_DDR2
    _DDR_SetDdr2AfterAI();
#endif /* #ifdef CC_SUPPORT_DDR2 */

#ifdef CC_SUPPORT_DDR3
     _DDR_SetDdr3AfterAI();
#endif /* #ifdef CC_SUPPORT_DDR3 */

    CHIP_Delay_us(100);
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
void _DDR_SetDdrMode(UINT32 fgPostWrite)
{
    UINT32 u4Val;
    UINT32 u4DramType;
    
    u4DramType = TCMGET_DRAMTYPE();

    if ((u4DramType >= DDR_III_x1) && (u4DramType <= DDR_III_x4))
    {
        // Enable advanced precharge with post write.
        u4Val = (fgPostWrite) ? 0x97000088: 0x90000008;
    }
    else
    {
        // Enable advanced precharge with post write.
        u4Val = (fgPostWrite) ? 0x93000088: 0x90000008;
    }
    // 701C[27]=1 to improve DRAM performance 
    u4Val &= ~0x08000000;
    DRAM_WRITE32(0x1C, u4Val);
}

void _DDR_SetRTT(UINT32 u4MainChipODT)
{
    DDR_RTT_T eRODT = DDR_RTT_150OHM;
    UINT32 u4Val;

    // Read ODT setting.
    switch (u4MainChipODT)
    {
        case 0: 
            eRODT = DDR_RTT_DISABLE;
            break;
        case 75: 
            eRODT = DDR_RTT_75OHM;
            break;
        case 50: 
            eRODT = DDR_RTT_50OHM;
            break;
        case 150: 
            eRODT = DDR_RTT_150OHM;
            break;
        case 37: 
            eRODT = DDR_RTT_37OHM;
            break;            
        case 25: 
            eRODT = DDR_RTT_25OHM;
            break;            
        case 30: 
            eRODT = DDR_RTT_30OHM;
            break;            
        case 21: 
            eRODT = DDR_RTT_21OHM;            
            break;
            
        default:             
            eRODT = DDR_RTT_150OHM;            
            break;            
    }

    // Set DQS default ODTP, ODTN, RTT.
    u4Val = DRAM_READ32(0x310);
    u4Val &= ~ (0x00770007);
    //RTT setting
    u4Val |= ((UINT32)eRODT);
    //ODTN ODTP setting
    u4Val |= (((UINT32)DEFAULT_DRAM_RODTNP)<<16);
    DRAM_WRITE32(0x310, u4Val);  
	DRAM_DDRPHY_WRITE32(0x310, u4Val); 

    // Set DQ default ODTP, ODTN, RTT.
    u4Val = DRAM_READ32(0x318);
    u4Val &= ~ (0x00770007);
    //RTT setting
    u4Val |= ((UINT32)eRODT);
    //ODTN ODTP setting
    u4Val |= (((UINT32)DEFAULT_DRAM_RODTNP)<<16);
    DRAM_WRITE32(0x318, u4Val); 
	DRAM_DDRPHY_WRITE32(0x318, u4Val); 
}

#ifdef DRAM_DEBUG_TOOL
void _DDR_ResetPhy(void)
{
    UINT32 u4RefCnt;
    UINT32 u4OldVal;
    UINT32 u4Reg33C;

    // Set CKE fix off.
    u4OldVal = DRAM_READ32(0x1C);
    u4OldVal &= ~ 0x00060000;
    u4OldVal |= 0x00040000;    
    DRAM_WRITE32(0x1C, u4OldVal);    
    CHIP_Delay_us(1000);

    // Set DRAM reset. Only DDR3 is need.
    u4OldVal = DRAM_READ32(0x0C);
    u4OldVal &= ~ 0x0100;
    DRAM_WRITE32(0x0C, u4OldVal);    
    CHIP_Delay_us(1000);
    u4OldVal = DRAM_READ32(0x0C);
    u4OldVal |= 0x0100;
    DRAM_WRITE32(0x0C, u4OldVal);    
    
    u4OldVal = DRAM_READ32(0x224);
    DRAM_WRITE32(0x224, (u4OldVal |0x03));    
    CHIP_Delay_us(1000);
    DRAM_WRITE32(0x224, u4OldVal);

    // Set not alway CKE fix off.
    u4OldVal = DRAM_READ32(0x1C);
    u4OldVal &= ~ 0x00060000;
    DRAM_WRITE32(0x1C, u4OldVal);    
    CHIP_Delay_us(1000);
    
    // Disable refresh before autoinit.
    u4RefCnt = DRAM_READ32(0x08);
    DRAM_WRITE32(0x08, (u4RefCnt & (~(0xFF))));

    //WODT must be disabled before AINIT.
    u4Reg33C = DRAM_READ32(0x33C); 
    DRAM_WRITE32(0x33C, (u4Reg33C & ~0x1));

    // Trigger auto initialization.
    u4OldVal = DRAM_READ32(0x0C);
    DRAM_WRITE32(0x0C, u4OldVal |0x200);
    CHIP_Delay_us(100);
    DRAM_WRITE32(0x0C, u4OldVal);

    //After AINIT, restore 
    DRAM_WRITE32(0x33C, u4Reg33C);

    // Restore autoinit.
    DRAM_WRITE32(0x08, u4RefCnt);
    
    CHIP_Delay_us(1000);
}
#endif /* #ifdef DRAM_DEBUG_TOOL */

void _DDR_ResetPhyOnly(void)
{
    UINT32 u4OldVal;

    u4OldVal = DRAM_READ32(0x224);
    DRAM_WRITE32(0x224, (u4OldVal |0x03));    
    CHIP_Delay_us(50);
    DRAM_WRITE32(0x224, u4OldVal);
    CHIP_Delay_us(50);    
}

UINT32 _DDR_CheckSize(BOOL fgChBActive)
{   
    register UINT32 u4DramSize = 0x4000000;   // 64 Mbytes.
#ifdef DRAM_64M
    UNUSED(fgChBActive);
#else
    register UINT32 u4CheckAddr1;
    register UINT32 u4CheckAddr2;  
    register UINT32 u4BaseAddr;      
    register UINT32 u4Val1;
    //register UINT32 u4MaxAddr = 0x20000000;  // 512 Mbytes.
    register UINT32 u4MaxAddr = 0x40000000;  // 1 Gbytes.
    BOOL fgSizingDone = FALSE;
     
    if (fgChBActive)
    {
        u4BaseAddr = INIT_DRAM_B_CHB_BASE;
    }
    else
    {
        u4BaseAddr = INIT_DRAM_B_BASE;
    }

    while ((!fgSizingDone) && (u4DramSize <= u4MaxAddr))
    {
        u4CheckAddr1 = u4BaseAddr + (u4DramSize*1) - 4;
        *((volatile UINT32 *)u4CheckAddr1) = 0x12345678;
        u4CheckAddr2 = u4BaseAddr + (u4DramSize*2) - 4;
        *((volatile UINT32 *)u4CheckAddr2) = 0x55AACC33;
        
        u4Val1 = *((volatile UINT32 *)u4CheckAddr1);
        
        if (u4Val1 == 0x12345678)
        {
            u4DramSize *= 2;
        }
        else            
        {
            fgSizingDone = TRUE;
        }
    }

#endif
    return u4DramSize;
}

#ifdef CC_SUPPORT_DDR2
BOOL _DDR_CheckColumnAddress(void)
{
    UINT32 u4Reg;
    volatile UINT32 u4Val0; 
    volatile UINT32 u4Val1;
    UINT32 u4CheckOffset;

    u4CheckOffset = (IS_FORCE32()) ? 0x400: 0x800;

    MEM_WRITE32(0, 0x12345678);
    u4Val0 = MEM_READ32(0);
    MEM_WRITE32(4, 0x55AA33CC);
    u4Val1 = MEM_READ32(4);

    CHIP_Delay_us(100);

    // Set column address = 10 bits.
    u4Reg = DRAM_READ32(0x18);
    u4Reg &= ~ 0x3000;
    u4Reg |= 0x2000;
    DRAM_WRITE32(0x18, u4Reg);       

    CHIP_Delay_us(100);

    // Check value at u4CheckOffset to identify if 9 bits or 10 bits column address.
    u4Val0 = MEM_READ32(u4CheckOffset+0x0);
    u4Val1 = MEM_READ32(u4CheckOffset+0x4);

    if ((u4Val0 == 0x12345678) && (u4Val1 == 0x55AA33CC))
    {
        // This is column address = 9 bits DRAM.
        u4Reg = DRAM_READ32(0x18);
        u4Reg &= ~ 0x3000;
        u4Reg |= 0x1000;
        DRAM_WRITE32(0x18, u4Reg);       
        
        LOG(1, "DRAM: 9 bits column address.\n");

        // fgColAddr9bits = TRUE;
        return TRUE;
    }
    else
    {
        LOG(1, "DRAM: 10 bits column address.\n");

        // fgColAddr9bits = FALSE;
        return FALSE;
    }
}

void _DDR_CheckBankAddress(BOOL fgColAddr9bits)
{
    UINT32 u4Reg;
    volatile UINT32 u4Val0; 
    volatile UINT32 u4Val1;
    UINT32 u4CheckOffset;

    if (fgColAddr9bits)
    {
        u4CheckOffset = (IS_FORCE32()) ? 0x1000: 0x2000;
    }
    else
    {
        u4CheckOffset = (IS_FORCE32()) ? 0x2000: 0x4000;
    }    

    // Set Default value to offset 0 at DRAM.
    MEM_WRITE32(0, 0x87654321);
    u4Val0 = MEM_READ32(0);
    MEM_WRITE32(4, 0xDD779922);
    u4Val1 = MEM_READ32(4);
    
    CHIP_Delay_us(100);
    
    // Set bank address = 3 bits.
    u4Reg = DRAM_READ32(0x18);
    u4Reg |= 0x4000;
    DRAM_WRITE32(0x18, u4Reg);

    CHIP_Delay_us(100);

    // Check value at u4CheckOffset to identify if 3 bits bank address is supported.
    u4Val0 = MEM_READ32(u4CheckOffset+0x0);
    u4Val1 = MEM_READ32(u4CheckOffset+0x4);

    if ((u4Val0 == 0x87654321) && (u4Val1 == 0xDD779922))
    {
        // This is 2 bits bank address DRAM.
        u4Reg = DRAM_READ32(0x18);
        u4Reg &= ~0x4000;
        DRAM_WRITE32(0x18, u4Reg);  

        LOG(1, "DRAM: 2 bits bank address.\n");
    }
    else
    {
        LOG(1, "DRAM: 3 bits bank address.\n");
    }
}
#endif /* #ifdef CC_SUPPORT_DDR2 */

void _DDR_QueryDramScrambleDataKey(void)
{
#if 0
    INT32 i;
    UCHAR uc;
    
    Printf("\nPress ESC to disable scramble data ... \n");
    SerTransparent();
    uc = 0;
    for (i=0; i<(200000000 >> 15); i++)
    {
        Printf("\r");
        switch(i & 0x3) {
            case 0:
                Printf("|"); break;
            case 1:
                Printf("/"); break;
            case 2:
                Printf("-"); break;
            case 3:
                Printf("\\"); break;
            default:
                continue;
        }

        if (SerInByte(&uc))
        {
            if (uc == 0x1B) // KEY_ESCAPE = 0x1B.
            {
                break;
            }
        }
    }

    SerNormalMode();

    // Without ESC, just return.
    if (uc == 0x1B) // KEY_ESCAPE = 0x1B.
    {
        Printf("\nDisable scramble dram.\n");
        return ;
    }     

    DRAM_WRITE32(0x24, 0x156);		
    Printf("\nEnable scramble, 0x7024 = 0x%08X.\n", DRAM_READ32(0x24));

#else

    //CHIP_DisplayString("scramble\n");
    DRAM_WRITE32(0x24, 0x156);

#endif

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/* DDR_SetDramController() to set DRAM controller parameter
 *  @param u4Clk    Data rate divided by 2
 */
//----------------------------------------------------------------------------
void DDR_SetDramController(void)
{
    UINT32 u4OldVal;
    UINT32 u4Reg33C;
    
    LOG(1, "SetDramCtrlr(Clk:%d)\n", TCMGET_DDR_CLK());

    _DdrSetDramBeforeAI();

#if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)
    CLK_QueryDramSetting("Before dram auto-init:", DRAM_CHANGE_DDR_BEFORE_AUTOINIT);
#endif /* DRAM_DEBUG */

    //WODT must be disabled before AINIT.
    u4Reg33C = DRAM_READ32(0x33C); 
    DRAM_WRITE32(0x33C, (u4Reg33C & ~0x1));

    // Trigger auto initialization.
    u4OldVal = DRAM_READ32(0x0C);
    DRAM_WRITE32(0x0C, u4OldVal |0x200);
    CHIP_Delay_us(100);
    DRAM_WRITE32(0x0C, u4OldVal);

    //After AINIT, restore 
    DRAM_WRITE32(0x33C, u4Reg33C);
    
    // Refresh must be set after the end of auto init.
    _DdrSetDramRefresh();

    _DdrSetDramAfterAI();

    // Set post write disable.
    _DDR_SetDdrMode(FALSE); 

#if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)
    CLK_QueryDramSetting("End of dram init:", DRAM_CHANGE_DDR_AFTER_AUTOINIT);
#endif /* DRAM_DEBUG */

    if (IS_DDR_DMSSON())
    {
        // Enable DRAM clock spread spectrum.
        CKGEN_WRITE32(0x194, CKGEN_READ32(0x194) | 0x4000);
        LOG(1, "DRAM DMSS start.\n");
    }
}

#else /* #ifdef CC_MTK_PRELOADER */

//-----------------------------------------------------------------------------
/** DDR_SetAgentPriority() Set dram group 1 agent priorities
 *  @param pu1Priorities   Priorities of aud, dmx, fci, vbi, osd, pscan,
 *                         b2r, cpu, scpos
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetAgentPriority(const UINT8 *pu1Prio)
{
    UINT64 u8Priority = 0;
    INT32 i;
    UINT8 u1Prio;

    ASSERT(pu1Prio != NULL);

    for (i = 0; i < 16; i++)
    {
        u1Prio = pu1Prio[i];
        // '1' ~ '9' ==> Pri 1 ~ 9
        if ((u1Prio <= (UINT8)'9') && (u1Prio >= (UINT8)'0'))
        {
            u1Prio -= (UINT8)'0';
        }
        // 'A' ==> Pri 10
        else if ((u1Prio == (UINT8)'A') || (u1Prio == (UINT8)'a'))
        {
            u1Prio = 10;
        }
        // 'B' ==> Pri 11
        else if ((u1Prio == (UINT8)'B') || (u1Prio == (UINT8)'b'))
        {
            u1Prio = 11;
        }
        // 'C' ==> Pri 12
        else if ((u1Prio == (UINT8)'C') || (u1Prio == (UINT8)'c'))
        {
            u1Prio = 12;
        }
        else if ((u1Prio == (UINT8)'D') || (u1Prio == (UINT8)'d'))
        {
            u1Prio = 13;
        }
        else if ((u1Prio == (UINT8)'E') || (u1Prio == (UINT8)'e'))
        {
            u1Prio = 14;
        }
        else if ((u1Prio == (UINT8)'F') || (u1Prio == (UINT8)'f'))
        {
            u1Prio = 15;
        }
    //    else if (i == 9)    // backward compatible, for only 9 agents
    //    {
   //         u1Prio = 9;
    //    }
        else
        {
            ASSERT(0);
            return;
        }

        u8Priority |= (((UINT64)u1Prio) << (4 * i));
    }

    // Set agent 0 ~ 7 of group 1 static priority.
    IO_WRITE32(DRAM_CHB_BASE, 0x100, (UINT32)(u8Priority & 0xffffffff));
    IO_WRITE32(DRAM_BASE, 0x100, (UINT32)(u8Priority & 0xffffffff));

    // Set agent 8 ~ 15 of group 1 static priority.
    IO_WRITE32(DRAM_CHB_BASE, 0x104, (UINT32)((u8Priority>>32) & 0xffffffff));
    IO_WRITE32(DRAM_BASE, 0x104, (UINT32)((u8Priority>>32) & 0xffffffff));

    // Set dynamic priority control.
    // dynamic priority on NR: 0x20007128[13:0] <= 14'h2031
    // DRAM_WRITE32(0x128, (DRAM_READ32(0x128) & ~(0x3fffU << 0)) | (0x2031U << 0));
    // dynamic priority on Scalar: 0x2000712c[29:16] <= 14'h2031
    // DRAM_WRITE32(0x12c, (DRAM_READ32(0x12c) & ~(0x3fffU << 16)) | (0x2031U << 16));
    // dynamic priority on VDEC: 0x20007130[13:0] <= 14'h2031
    // DRAM_WRITE32(0x130, (DRAM_READ32(0x130) & ~(0x3fffU << 0)) | (0x2031U << 0));
    // Enable dynamic control.
    //DRAM_WRITE32(0x150, (DRAM_READ32(0x150) | 0x10000));
}

//-----------------------------------------------------------------------------
/** DDR_SetBurstLen() Set dram group 1 agent burst length
 *  @param u4BurstLen1     Burst length of agent 0 ~ 7
 *  @param u4BurstLen2     Burst length of agent 8 ~ 16
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetBurstLen(UINT32 u4BurstLen1, UINT32 u4BurstLen2)
{
    DRAM_WRITE32(0x110, u4BurstLen1);
    DRAM_WRITE32(0x114, u4BurstLen2);
}

//-----------------------------------------------------------------------------
/** DDR_SetArbitorTime() Set dram group arbiter time
 *  @param u1Group         Dram agent group (1,2,3)
 *  @param u1Time          Arbitor time (0 ~ 15)
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetArbiterTime(UINT8 u1Group, UINT8 u1Time)
{
    UINT32 u4RegVal;

    if (u1Time > 15)
    {
        u1Time = 15;
    }

    u4RegVal = DRAM_READ32(0x150);
    switch(u1Group)
    {
    case 1:
        u4RegVal &= 0xff0fffff;
        u4RegVal |= (((UINT32)u1Time) << 20);
        break;
    case 2:
        u4RegVal &= 0xf0ffffff;
        u4RegVal |= (((UINT32)u1Time) << 24);
        break;
    case 3:
        u4RegVal &= 0x0fffffff;
        u4RegVal |= (((UINT32)u1Time) << 28);
        break;
    default:
        return;
    }
    DRAM_WRITE32(0x150, u4RegVal);
}
#endif /* #ifdef CC_MTK_PRELOADER */   

