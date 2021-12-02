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

#include "x_typedef.h"
#include "x_bim.h"
#include "x_dram.h"
#include "x_pdwnc.h"
#include "x_ckgen.h"
#include "drvcust_if.h"
#include "ddr.h"
#include "chip_txt.h"

extern void CLK_SetDdr(void);
extern void CHIP_BimInit(void);
#ifndef CC_MTK_LOADER
extern void CHIP_CheckBoard(void);
#endif /* CC_MTK_LOADER */
extern void CHIP_DisplayChar(UINT32 u4Char);
extern void CHIP_ClockSetting(void);
extern void CHIP_1stBootRegInit(void);


void CHIP_NotifyT8032(void)
{
    UINT32 u4Val = 0;        
    UINT32 i = 0, j = 0;
    UINT32 u1Ack = 0;
    #define ARM_FORCE_TAKE_BACK_UART0 0    
    
    PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_ENTER_NORMAL | (1 << 8));

    while(1)
    {          
        vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x1, FLD_AUX_INTEN);
        vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
        
        while(i < 100000)
        {
            //CHIP_Delay_us(100);                  
            if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
            {
                u4Val = PDWNC_T8032CMD_GET_RESPONSE();
                if((u4Val & 0xff) != ((UINT8)(PDWNC_CMD_ARMT8032_ENTER_NORMAL)))
                {
                        continue;
                }
                u1Ack = 1;
                break;                
            }      
            i++;
        }
                             
        if(i < 10000)
        {
            CHIP_DisplayString("Preloader T8032 ack !\n");
            vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
            u1Ack = 1;
            break;
        }            
        else
        {
            vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
            i = 0;
        }
        
        if(++j > 2 /*T8032_ACK_TIMEOUT*/)
        {
            CHIP_DisplayString("Preloader T8032 NO ack!\n");
            // Ack timeout
            u1Ack = 0;
              break;
        }
      
    }

    if(((u4Val & 0xff000000) == ARM_FORCE_TAKE_BACK_UART0) || (u1Ack == 0))
    {
//        CHIP_DisplayString("U0==>ARM!\n");
#ifdef CC_MT5389
		vIO32Write4B(PDWNC_PINMUX0,u4IO32Read4B(PDWNC_PINMUX0) & 0xfffffcff);
#else
        vIO32Write4B(PDWNC_PINMUX0,u4IO32Read4B(PDWNC_PINMUX0) & 0xfffcffff);
#endif
    }
    else
    {
    //    CHIP_DisplayString("U0==>T8032!\n");
    }
}


void CHIP_BootEntry0()
{
    if ((BIM_READ32(REG_RW_TIMER_CTRL) & (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN)) == 0)
    {
        // Start timer 2 for bootup time counter enable.
        BIM_WRITE32(REG_RW_TIMER2_LLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_LOW , 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HLMT, 0xffffffff);
        BIM_WRITE32(REG_RW_TIMER2_HIGH, 0xffffffff);
        BIM_ClrTimeLog(0);
        BIM_ClrTimeLog(1);
        BIM_ClrTimeLog(2);
        BIM_ClrTimeLog(3);
        BIM_WRITE32(REG_RW_TIMER_CTRL, (TMR2_CNTDWN_EN | TMR2_AUTOLD_EN));
    }

    // ; Show first 'o' to note that i-cache enable is ok.
    // MOV     r0, #0x6f
    // BL      CHIP_DisplayChar
    CHIP_DisplayChar('o');

    // ; Setup BIM initial register
    // BL      CHIP_BimInit
    CHIP_BimInit();

#ifndef CC_MTK_LOADER
    // ; Setup board clock
    // BL      CHIP_CheckBoard
    CHIP_CheckBoard();
#endif /* CC_MTK_LOADER */

    // ; Show second 'o' to note that BIM enable is ok.
    // MOV     r0, #0x6f
    // BL      CHIP_DisplayChar
    CHIP_DisplayChar('o');

    if (!CHIP_IsRemap() && !BSP_IsFPGA())
    {
    // ; Check if remapped, initialize BIM and DRAM for the first time only
    // BL      CHIP_ClockSetting
        CHIP_ClockSetting();
    }

    // ; Show 't' to note that clock initialization is ok.
    // MOV     r0, #0x74
    // BL      CHIP_DisplayChar
    CHIP_DisplayChar('t');

    // ; Check Dram initialized
    // BL      CHIP_IsRemap
    // CMP     r0, #0
    // BNE     SoftInit
    if (!CHIP_IsRemap())
    {
        // MOV     r0, #0x2d
        // BL      CHIP_DisplayChar
        CHIP_DisplayChar('-');

        // ; Setup 1st boot register initialization
        // BL      CHIP_1stBootRegInit
        CHIP_1stBootRegInit();

        //BIM_SetTimeLog(0);
#ifndef CC_FPGA
        if(u4IO32Read4B(PDWNC_PDSTAT))

        {
            CHIP_NotifyT8032();
        }
#endif
        //BIM_SetTimeLog(1);
        // ; Setup Dram
        // BL      CLK_SetDdr
        CLK_SetDdr();
    }
}

