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
 * $RCSfile: pi_demod_atd_lagacy.c$
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


/***********************************************************************/

extern UINT8 u1NullRegion;   // liuqu added,20100313

#define cLOW_IF_BOUNDARY 		10000

UINT8 ATD_ChipInit(ATD_CTX_T *psDemodCtx)
{
#ifdef CC_MT5363 //Justin TODO

// Select Clock Source
// CKGEN_BASE: REG_ATD_DRO_OBS20dxxx
// ATD_BASE:   REG_ATD_DRO_OBS22axxx
// DVBT_BASE:  REG_ATD_DRO_OBS22bxxx
// IO_WRITE32MSK(0xF000D000, 0x204, 0x00000300, 0x00000300); // C27M Clock, 11: c27m_ck = xtal_ck

    UINT32 u4Temp;
    UINT8 ucData;

    // for 27M Xtal TVDPLL to 540MHz
    //IO_WRITE32(REG_ATD_DRO_OBS20d000, REG_ATD_CTL_SYS1, 0x00099401);

    // ADC setting & sampling from PLL
    if (GET_XTAL_CLK() == CLK_54MHZ) { // for 54M Xtal
        IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_EXT_FREQ1, 0x00aa4000); // from Xtal
    }
    else{ // for 27M Xtal
        IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_EXT_FREQ1, 0x00aa4100); // from ADCLK
    }
    IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_CFO_1, 0x00000054);

    // Demod AD clock enable
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_EQ_CFG);
    u4Temp |=  ((U32)0x01) << 26; //2000d12c[26]=1'b1
    IO_WRITE32(CKGEN_BASE, REG_ATD_EQ_CFG, u4Temp);

    // set PLL clock = 108M for AD
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_CTL_RST1);
    u4Temp |=  (((U32)0x01) << 4); //2000d134[4]=1'b1
    u4Temp &= ~(((U32)0x03) << 5); //2000d134[6:5]=2'b00
    u4Temp &= ~(((U32)0x0F) << 8); //2000d134[11:8]=4'b0100
    u4Temp |=  (((U32)0x04) << 8);
    // ADC reset (while new setting) 2000d134[12]=1'b0 2000d134[14]=1'b0
    u4Temp &= ~((((U32)0x01) << 12) | (((U32)0x01) << 14));
    IO_WRITE32(CKGEN_BASE, REG_ATD_CTL_RST1, u4Temp);
    // ADC release (while new setting)2000d134[12]=1'b1 2000d134[14]=1'b1
    u4Temp |=  ((((U32)0x01) << 12) | (((U32)0x01) << 14));
    IO_WRITE32(CKGEN_BASE, REG_ATD_CTL_RST1, u4Temp);

    // for demod60m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_TD_MIN_1);
    u4Temp &= ~((U32)0x03); //EC_code_dA408 add mask
    u4Temp |=  ((U32)0x03); // 2000d218[1:0]=2'b11
    IO_WRITE32(CKGEN_BASE, REG_ATD_TD_MIN_1, u4Temp);


    // for demod54m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_TD_BPF_MUL_COEF_0);
    u4Temp &= ~((U32)0x03); //EC_code_dA408 add mask
    u4Temp |=  ((U32)0x03); // 2000d21c[1:0]=2'b11
    IO_WRITE32(CKGEN_BASE, REG_ATD_TD_BPF_MUL_COEF_0, u4Temp);

    // for demod27m_ck = 27Mz (TVDPLL)
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_DRO_REF1_1);
    u4Temp &= ~(((U32)0x03) << 8); //2000d204[9:8]=2'b01
    u4Temp |=  (((U32)0x01) << 8);    
    IO_WRITE32(CKGEN_BASE, REG_ATD_DRO_REF1_1, u4Temp);

    // set byte mask
    u4Temp = IO_READ32(cRISC_ATD_BASE, 0x4b4);
    u4Temp |=  (((U32)0x0F) << 16);//u4Temp |= 0x000f0000;    
    IO_WRITE32(cRISC_ATD_BASE, 0x4b4, u4Temp);

    // power on clk
    RegWrite8(REG_ATD_CTL_TEST, 0x00); //REG_ATD_CTL_TEST, all clk power on

    //Need to set RISC wait time => to correct get multi-regiser bug
    ucData = 0x1e; 
    Demod_SetReg(psDemodCtx, 0x4a3, &ucData, 1);       

    // RFAGC, IFAGC pin mux: [29:28] = 00 //EC_code_d9826 for eIntATD_ExtDTD
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
    u4Temp &= 0xcFFFFFFF;
    IO_WRITE32(CKGEN_BASE, 0x408, u4Temp);
    
    return 0;

#else

    UINT32 u4Temp;
    UINT8 ucData;


    IO_WRITE32(CKGEN_BASE,REG_ATD_DRO_CVBS_UP_BUD_0, 0x00000001);	
    IO_WRITE32(CKGEN_BASE,REG_ATD_DRO_LSYNC_CTRL, 0x00000001);            	
    IO_WRITE32(CKGEN_BASE,REG_ATD_TD_MIN_1, 0x00000002);            	
    IO_WRITE32(CKGEN_BASE,REG_ATD_TD_BPF_MUL_COEF_0, 0x00000001);               	

    //1. Write DTD 111H=02h (disable dtd sampling rate switch)
    //IO_WRITE32(REG_ATD_DRO_OBS22b000, 0x4b0, 0x00000200);               	
    //IO_WRITE32(REG_ATD_DRO_OBS22b000, 0x4b4, 0x40020044);                   
    ucData = 0x02;
    Demod_SetReg(psDemodCtx, REG_DEMODADC_1, &ucData, 1);

    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b0, 0x00AA4100);               	
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b4, 0x400f0046);


    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b0, 0x00000000);               	
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b4, 0x40010046);
    ucData = 0x00;
    Demod_SetReg(psDemodCtx, REG_DEMODADC_8, &ucData, 1);


    //2. Write DTD 119H=41h
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b0, 0x00004100);
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b4, 0x40020046);
    ucData = 0x41;
    Demod_SetReg(psDemodCtx, REG_DEMODADC_9, &ucData, 1);

    //Write DTD 11AH=AAh (same as default)
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b0, 0x00AA0000);               	
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b4, 0x40040046);                   
    ucData = 0xAA;
    Demod_SetReg(psDemodCtx, REG_DEMODADC_A, &ucData, 1);


    //Write DTD 11BH=00h  (same as default)
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b0, 0x00000000);               	
    //IO_WRITE32(REG_ATD_DRO_OBS22b000,0x4b4, 0x40080046);               	
    ucData = 0x00;
    Demod_SetReg(psDemodCtx, REG_DEMODADC_B, &ucData, 1);

    //3. Write DTD 11CH=94h  (same as default)
    //IO_WRITE32(REG_ATD_DRO_OBS22b000, 0x4b0, 0x00000094);               	
    //IO_WRITE32(REG_ATD_DRO_OBS22b000, 0x4b4, 0x40010047);               	
    //set AD input swing from 1.8Vp_p t 2Vp_p
    //IO_WRITE32(REG_ATD_DRO_OBS22b000, 0x4b0, 0x00000054);
    ucData = 0x54;
    Demod_SetReg(psDemodCtx, REG_DEMODADC_C, &ucData, 1);
               	
    if(psDemodCtx->pTCtx->fgRFAGC == TRUE)
    {
    //IO_WRITE32(REG_ATD_DRO_OBS20d000, 0x400, 0x00000300);  
    u4Temp = IO_READ32(CKGEN_BASE, 0x400);
    u4Temp |= 0x00000300; 
    IO_WRITE32(CKGEN_BASE, 0x400, u4Temp);
    }

    //IO_WRITE32(CKGEN_BASE, 0x408, 0x00110030);
    u4Temp = IO_READ32(CKGEN_BASE, 0x408);
    u4Temp |= 0x00110030; 
    IO_WRITE32(CKGEN_BASE, 0x408, u4Temp);

//    The following register bit setting is dedicated for CVBS output to VDAC where source comes from ATD path2
//    (Note: ATD path 1 is "non-scaling" and dedicated to TVD; ATD path 2 is "scaling" and dedicated to VDAC ). For Demod ATD, make sure 
//     

    ucData = 0x0c;
    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    ucData = 0x23;
    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG4, &ucData, 1);

    //IO_WRITE32(REG_ATD_DRO_OBS20d2a8) bit[13:12] = 2'b10 Demod clock to VDAC from clock demod60m_ck
    u4Temp = IO_READ32(CKGEN_BASE, 0x2a8);
    u4Temp |= 0x00002000; 
    IO_WRITE32(CKGEN_BASE, 0x2a8, u4Temp);

    if (IS_XTAL_54MHZ())
    {
        //(set for 54MXTAL)
        IO_WRITE32(CKGEN_BASE, REG_ATD_TD_MIN_1, 0x00000000); 
        //set ADC clock from xtal: write DTD 119H = 40h
        //IO_WRITE32(cRISC_ATD_BASE, 0x4b0, 0x00004000);
        //IO_WRITE32(cRISC_ATD_BASE, 0x4b4, 0x40020046);
        ucData = 0x40;
        Demod_SetReg(psDemodCtx, REG_DEMODADC_9, &ucData, 1);    
    }
    
    //Reset ADC divider & release
    IO_WRITE32(CKGEN_BASE,REG_ATD_CTL_RST1, 0x00086410);               	
    IO_WRITE32(CKGEN_BASE,REG_ATD_CTL_RST1, 0x00087410);


    // 4. Reset & release ADC latch data FF: (Set Demod adr 0x110 to be 0x00, then set to be 0x02)
    //u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_CPR_CFO_1);
    //u4Temp |= 0xE0000000; 
    //IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_CFO_1, u4Temp);

    IO_WRITE32(cRISC_ATD_BASE,0x4b0, 0x00000000);               	
    IO_WRITE32(cRISC_ATD_BASE,0x4b4, 0x40010044);               	
    IO_WRITE32(cRISC_ATD_BASE,0x4b0, 0x00000002);               	
    mcDELAY_MS(10);
    IO_WRITE32(cRISC_ATD_BASE,0x4b4, 0x40010044);


    //5. APLL setting for 2.54MHz spur solution and it should be setted on loader//EC_code_d9417   
    /*IO_WRITE32(REG_ATD_DRO_OBS20d000, 0x100, 0x004a19C4);
    IO_WRITE32(REG_ATD_DRO_OBS20d000, REG_ATD_CTL_REGRST1, 0x004a19C4);
    IO_WRITE32(REG_ATD_DRO_OBS20d000, 0x104, 0x03A4114B);
    IO_WRITE32(REG_ATD_DRO_OBS20d000, REG_ATD_CTL_DBGSEL1, 0x03A4114B);    
    */
    
    if (IS_XTAL_27MHZ())
    {
        mcSHOW_HW_MSG(("Finished MT5387 Chip Init w/ 27M\n"));
    }
    else if (IS_XTAL_54MHZ())
    {
        mcSHOW_HW_MSG(("Finished MT5387 Chip Init w/ 54M\n"));
    }
    else
    {
        mcSHOW_HW_MSG(("Finished MT5387 Chip Init w/ XXM\n"));
    }

    return 0;
#endif
}

//for AUD detect system type
/*
void ATD_AudSetSystemAudio(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
    UINT8   ucData;      

    switch (u1SubSysIdx)
    {
    case MOD_PAL_BG: 
        mcSHOW_DBG_MSG(("Set System = PAL BG 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x33; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        //ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);        
#endif       
        //ucData = 0xE1; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
        //ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
        //ucData = 0x73; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
        // set AVS/AUC audio filter
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ucData = 0x20; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);        
        break;
    
    case MOD_PAL_DK: 
        mcSHOW_DBG_MSG(("Set System = PAL DK 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x35; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        //ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);  
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif
        // set AVS filter
        //ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
        //ucData = 0x78; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
        //ucData = 0x25; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
        break;
    
    case MOD_PAL_I: 
        mcSHOW_DBG_MSG(("Set System = PAL I 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x34; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        //ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);       
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif
        // set AVS filter       
        //ucData = 0xD8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
        //ucData = 0x78; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
        //ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);

        break;
    
    case MOD_SECAM_L: 
        mcSHOW_DBG_MSG(("Set System = SECAM L	\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x35; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x0C; ATD_SetReg(psDemodCtx, REG_ATD_AREG13, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_ATD_DC_RFA2, &ucData, 1);               
        ucData = 0x99; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //ucData = 0x11; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        //ucData = 0x9E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);
        
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x0F; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);  //ZY, 0x10->0x0F, to solve level switch mute & OVM 150%             
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG1, &ucData, 1);  //ZY, 0x84->0x81, to solve level switch mute & OVM 150%
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG2, &ucData, 1);  
        
        break;
    
    case MOD_SECAM_L1: 
        mcSHOW_DBG_MSG(("Set System = SECAM L'	\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x95; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x0C; ATD_SetReg(psDemodCtx, REG_ATD_AREG13, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_ATD_DC_RFA2, &ucData, 1);               
        ucData = 0x99; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //ucData = 0x11; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        //ucData = 0x9E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);               
        
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x0F; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);  //ZY, 0x10->0x0F, to solve level switch mute & OVM 150%              
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG1, &ucData, 1);  //ZY, 0x84->0x81, to solve level switch mute & OVM 150%
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG2, &ucData, 1);  
        
        break;

    case MOD_NTSC_M:
    
        if (psDemodCtx->pTCtx->u2IF_Freq == 44000)
        {
            mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/44M\n"));
            //TV System Setting
            ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_CTL_MISC_SEL, &ucData, 1);               
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);
            ucData = 0x22; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
            ucData = 0x66; ATD_SetReg(psDemodCtx, REG_ATD_VOP_RESERVE3, &ucData, 1); 
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);	           
#else
             //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);	           
        }
        else
        {            
          if (psDemodCtx->pTCtx->u2IF_Freq == 36167)  //forTWN  NTSC 38Mhz,DVBT + ATDNA
          {
            mcSHOW_DBG_MSG(("Set System = TWN NTSC M/N 54M/38M\n"));
		    ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_CTL_MISC_SEL, &ucData, 1);  
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);
            ucData = 0x42; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
		    ucData = 0x66; ATD_SetReg(psDemodCtx, REG_ATD_VOP_RESERVE3, &ucData, 1); 
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);	
#else
            //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);


		  }
		  else
		  { mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/36M\n"));
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);
            ucData = 0x32; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);	
#else
            //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
		  }
        }
        break;         
    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }
}
*/
void ATD_SetSystemAudio(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
    UINT8   ucData;      

    switch (u1SubSysIdx)
    {
    case MOD_PAL_BG: 
        mcSHOW_DBG_MSG(("Set System = PAL BG 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x33; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        //if(psDemodCtx->fgAudScan==FALSE)
        //{        	
        //  ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //}
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);        
#endif
        //ucData = 0xE0; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
        //ucData = 0x78; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
        //ucData = 0x9C; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM        
        if(psDemodCtx->fgAudScan==FALSE)
#endif        	
        {
         ucData = 0xE1; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
         ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
         ucData = 0x73; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
        } 
        // set AVS/AUC audio filter
        ucData = 0x94; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ucData = 0x20; ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);        
        break;
    
    case MOD_PAL_DK: 
        mcSHOW_DBG_MSG(("Set System = PAL DK 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x35; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        //if(psDemodCtx->fgAudScan==FALSE)
        //{
        //  ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //}  
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);  
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif
        // set AVS filter
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM        
        if(psDemodCtx->fgAudScan==FALSE)
#endif 
        {
         ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
         ucData = 0x78; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
         ucData = 0x25; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
        } 
        break;
    
    case MOD_PAL_I: 
        mcSHOW_DBG_MSG(("Set System = PAL I 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x34; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        //if(psDemodCtx->fgAudScan==FALSE)
        //{
        // ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //} 
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);       
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif
        // set AVS filter
        //ucData = 0xD6; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
        //ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
        //ucData = 0x9A; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
#ifdef CC_SUPPORT_AUTO_DETECT_AUD_SYSTEM        
        if(psDemodCtx->fgAudScan==FALSE)
#endif 
        {
         ucData = 0xD8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
         ucData = 0x78; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
         ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
        }

        break;
    
    case MOD_SECAM_L: 
        mcSHOW_DBG_MSG(("Set System = SECAM L	\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x35; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x0C; ATD_SetReg(psDemodCtx, REG_ATD_AREG13, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_ATD_DC_RFA2, &ucData, 1);               
        ucData = 0x99; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //ucData = 0x11; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        //ucData = 0x9E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);
        
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x0F; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);  //ZY, 0x10->0x0F, to solve level switch mute & OVM 150%             
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG1, &ucData, 1);  //ZY, 0x84->0x81, to solve level switch mute & OVM 150%
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG2, &ucData, 1);  
        
        break;
    
    case MOD_SECAM_L1: 
        mcSHOW_DBG_MSG(("Set System = SECAM L'	\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);               
        ucData = 0x95; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);               
        ucData = 0x0C; ATD_SetReg(psDemodCtx, REG_ATD_AREG13, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);               
        ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_ATD_DC_RFA2, &ucData, 1);               
        ucData = 0x99; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        //ucData = 0x11; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
        //ucData = 0x9E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);               
        
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x0F; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);  //ZY, 0x10->0x0F, to solve level switch mute & OVM 150%              
        ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
        ucData = 0x81; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG1, &ucData, 1);  //ZY, 0x84->0x81, to solve level switch mute & OVM 150%
        ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_CLAMP_CFG2, &ucData, 1);  
        
        break;
#if 0    
    case MOD_NTSC_M_60M_44M: // NTSC M/N 60M/44M
        mcSHOW_DBG_MSG(("Set System = NTSC M/N 60M/44M\n"));			
        ucData = 0x02; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);	    
        ucData = 0x02; ATD_SetReg(psDemodCtx, 0x902, &ucData, 1);		
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x920, &ucData, 1);			
        ucData = 0x11; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);			
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);		
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);			
        ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
        ucData = 0x4E; ATD_SetReg(psDemodCtx, REG_ATD_VOP_RESERVE3, &ucData, 1);
        ucData = 0x3F; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
        ucData = 0xE4; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
        ucData = 0x9C; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);			
        break;
#endif    
    case MOD_NTSC_M:
    
        if (psDemodCtx->pTCtx->u2IF_Freq == 44000)
        {
            mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/44M\n"));
            //TV System Setting
            ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_CTL_MISC_SEL, &ucData, 1);               
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);
            ucData = 0x22; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);               
            ucData = 0x66; ATD_SetReg(psDemodCtx, REG_ATD_VOP_RESERVE3, &ucData, 1); 
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);	           
#else
             //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);	           
        }
        else
        {            
          if (psDemodCtx->pTCtx->u2IF_Freq == 36167)  //forTWN  NTSC 38Mhz,DVBT + ATDNA
          {
            mcSHOW_DBG_MSG(("Set System = TWN NTSC M/N 54M/38M\n"));
		    ucData = 0x03; ATD_SetReg(psDemodCtx, REG_ATD_CTL_MISC_SEL, &ucData, 1);  
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);
            ucData = 0x42; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
		    ucData = 0x66; ATD_SetReg(psDemodCtx, REG_ATD_VOP_RESERVE3, &ucData, 1); 
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);	
#else
            //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);


		  }
		  else
		  { mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/36M\n"));
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG03, &ucData, 1);
            ucData = 0x32; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, REG_ATD_AREG00, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, REG_ATD_AREG07, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS2, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1);	
#else
            //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_7, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData, 1);
		  }
        }
        break;         
    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }
}

void ATD_SetSystemVideo(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
    UINT8   ucData;    

    switch (u1SubSysIdx)
    {
    case MOD_PAL_BG:
    case MOD_PAL_DK:
    case MOD_PAL_I:
        //PRA
        RegWrite8(REG_ATD_PRA_LP1_CFG11,0x61); //REG_ATD_PRA_LP1_CFG11
        RegWrite8(REG_ATD_PRA_LP1_CFG04,0x42); //REG_ATD_PRA_LP1_CFG04
        RegWrite8(REG_ATD_PRA_LP1_CFG03,0x19); //REG_ATD_PRA_LP1_CFG03
        RegWrite8(REG_ATD_PRA_LP1_CFG13,0x8D); //REG_ATD_PRA_LP1_CFG13
        RegWrite8(REG_ATD_PRA_IF_SLOPE,0x7F);  //REG_ATD_PRA_IF_SLOPE
#if ((fcTUNER_TYPE == cSONY_RE201)||(fcTUNER_TYPE == cSONY_RE205))	
        RegWrite8(REG_ATD_PRA_CFG_1,0x5E); 
#else
		RegWrite8(REG_ATD_PRA_CFG_1,0x1E);     //REG_ATD_PRA_CFG_1
#endif
		RegWrite8(REG_ATD_PRA_DR_SEL,0x0B);    //REG_ATD_PRA_DR_SEL
        RegWrite8(REG_ATD_DRO_PROBE_CFG,0x20); //REG_ATD_DRO_PROBE_CFG
        
        //CR
        RegWrite8(REG_ATD_TRAN_STG,0x36);    //REG_ATD_TRAN_STG
        RegWrite8(REG_ATD_PFLL_CFG,0x8c);    //REG_ATD_PFLL_CFG
        RegWrite8(REG_ATD_PFLL_CFG2,0x29);   //REG_ATD_PFLL_CFG2
        RegWrite8(REG_ATD_PRR_SS,0x47);     //REG_ATD_PRR_SS
        RegWrite8(REG_ATD_PRR_TRAN,0x46);   //REG_ATD_PRR_TRAN
        RegWrite8(REG_ATD_CR_CFG,0xf8);	   //REG_ATD_CR_CFG
        RegWrite8(REG_ATD_PRR_PRFK,0x36);    //REG_ATD_PRR_PRFK
        RegWrite8(REG_ATD_PRR_DAMRK,0x46);   //REG_ATD_PRR_DAMRK
        RegWrite8(REG_ATD_PE_CFG1,0x7C);     //REG_ATD_PE_CFG1
        RegWrite8(REG_ATD_PE_CFOK,0x04);     //REG_ATD_PE_CFOK
        RegWrite8(REG_ATD_PJ_CFG,0x2C);      //REG_ATD_PJ_CFG
        RegWrite8(REG_ATD_ADVPS_CFG2,0x30);  //REG_ATD_ADVPS_CFG2
        RegWrite8(REG_ATD_TRAN_CFG,0x94);    //REG_ATD_TRAN_CFG
        RegWrite8(REG_ATD_AREG13,0x0C);
        //DRO
        RegWrite8(REG_ATD_DRO_CVBS_LPF,0x7F); //REG_ATD_DRO_CVBS_LPF
        RegWrite8(REG_ATD_DRO_LP_CFG01,0xA4); //REG_ATD_DRO_LP_CFG01, async mode, default 0xD4, GA FT issue 0xA4
        RegWrite8(REG_ATD_DRO_LP_CFG02,0x08);  //REG_ATD_DRO_LP_CFG02, defualt 0x18, GA FT issue 0x08
        RegWrite8(REG_ATD_DRO_REF_COEF,0xD2); //REG_ATD_DRO_REF_COEF
        RegWrite8(REG_ATD_DRO_RSP_CFG,0x0F);  //REG_ATD_DRO_RSP_CFG
        RegWrite8(REG_ATD_DRO_LP_CFG05,0x87); //REG_ATD_DRO_LP_CFG05
        RegWrite8(REG_ATD_DRO_REF_CFG2,0x68); //REG_ATD_DRO_REF_CFG2
        RegWrite8(REG_ATD_POA_TARG_LVH,0x20); //REG_ATD_POA_TARG_LVH
        RegWrite8(REG_ATD_DRO_CCI_CFG3, 0xA5);
        RegWrite8(REG_ATD_DRO_CCI_CFG4, 0x12);
        
        //FPGA Interpolartor			
        RegWrite8(REG_ATD_AREG06,0x11);
#ifdef CC_TVE_POWERDOWN_HALF
        RegWrite8(REG_ATD_DRO_DCR_EXT1,0x00);
        RegWrite8(REG_ATD_DRO_SCAL_3,0xBC);
        RegWrite8(REG_ATD_DRO_SCAL_7,0x40);
#else
        RegWrite8(REG_ATD_DRO_DCR_EXT1,0xE4);
#endif
        RegWrite8(REG_ATD_DRO_LSYNC_CTRL, 0x17);
        RegWrite8(REG_ATD_IPS_MODE2, 0x36);
        break;

    case MOD_SECAM_L:
    case MOD_SECAM_L1:
		//PRA
        RegWrite8(REG_ATD_PRA_LP1_CFG01,0x1A); //REG_ATD_PRA_LP1_CFG01

#if  0
        RegWrite8(REG_ATD_PRA_LP1_CFG03,0x3E); //REG_ATD_PRA_LP1_CFG03
        RegWrite8(REG_ATD_PRA_LP1_CFG04,0x42); //REG_ATD_PRA_LP1_CFG04
        RegWrite8(REG_ATD_PRA_LP1_CFG07,0x5C); //REG_ATD_PRA_LP1_CFG07
        RegWrite8(REG_ATD_PRA_LP1_CFG08,0x8C); //REG_ATD_PRA_LP1_CFG08
#else
        //To fix CR DTV00138738
        RegWrite8(REG_ATD_PRA_LP1_CFG03,0x54); //REG_ATD_PRA_LP1_CFG03
        RegWrite8(REG_ATD_PRA_LP1_CFG04,0x22); //REG_ATD_PRA_LP1_CFG04
        RegWrite8(REG_ATD_PRA_LP1_CFG07,0x59); //REG_ATD_PRA_LP1_CFG07
        RegWrite8(REG_ATD_PRA_LP1_CFG08,0xCC); //REG_ATD_PRA_LP1_CFG08
        RegWrite8(REG_ATD_PRA_LP1_CFG09,0xFF); //REG_ATD_PRA_LP1_CFG09
        RegWrite8(REG_ATD_PRA_LP1_CFG10,0x33); //REG_ATD_PRA_LP1_CFG10
#endif
		
        RegWrite8(REG_ATD_PRA_LP1_CFG11,0x61); //REG_ATD_PRA_LP1_CFG11
        //To fix CR DTV00138738
        //RegWrite8(REG_ATD_PRA_LP1_CFG13,0x2D); //REG_ATD_PRA_LP1_CFG13
        RegWrite8(REG_ATD_PRA_LP1_CFG13,0x1D); //REG_ATD_PRA_LP1_CFG13
        RegWrite8(REG_ATD_PRA_IF_SLOPE,0x7F);  //REG_ATD_PRA_IF_SLOPE
#if (fcTUNER_TYPE == cSONY_RE201)
        RegWrite8(REG_ATD_PRA_CFG_1,0x5F);     //REG_ATD_PRA_CFG_1
#else
        RegWrite8(REG_ATD_PRA_CFG_1,0x1F);     //REG_ATD_PRA_CFG_1
#endif
		RegWrite8(REG_ATD_PRA_DR_SEL,0x0B);    //REG_ATD_PRA_DR_SEL
        RegWrite8(REG_ATD_DRO_PROBE_CFG,0x20); //REG_ATD_DRO_PROBE_CFG
        //RegWrite8(REG_ATD_DRO_WADC_5,0x32);    //REG_ATD_DRO_WADC_5
        
        //DRO
        RegWrite8(REG_ATD_DRO_LP_CFG02,0x58);  //REG_ATD_DRO_LP_CFG02
        //RegWrite8(REG_ATD_DRO_LP_CFG03,0x11);  //REG_ATD_DRO_LP_CFG03
        RegWrite8(REG_ATD_DRO_LP_CFG04,0xD9);  //REG_ATD_DRO_LP_CFG04
        RegWrite8(REG_ATD_DRO_LP_CFG05,0x87);  //REG_ATD_DRO_LP_CFG05
        RegWrite8(REG_ATD_DRO_CVBS_LPF,0x7F);  //REG_ATD_DRO_CVBS_LPF
        RegWrite8(REG_ATD_DRO_LP_CFG01,0xDE);  //REG_ATD_DRO_LP_CFG01, sync mode
        //RegWrite8(REG_ATD_DRO_LP_CFG01,0xD6);  //REG_ATD_DRO_LP_CFG01, async mode
        RegWrite8(REG_ATD_DRO_REF_COEF,0x2D);  //REG_ATD_DRO_REF_COEF
        RegWrite8(REG_ATD_DRO_REF_CFG2,0x68);  //REG_ATD_DRO_REF_CFG2
        RegWrite8(REG_ATD_DRO_RSP_CFG,0x0F);   //REG_ATD_DRO_RSP_CFG
        //RegWrite8(REG_ATD_DRO_MASK_CFG,0x27);  //REG_ATD_DRO_MASK_CFG
        RegWrite8(REG_ATD_POA_CFG,0xE8);  //REG_ATD_POA_CFG
        RegWrite8(REG_ATD_POA_TARG_LVH,0x1A); //REG_ATD_POA_TARG_LVH
        RegWrite8(REG_ATD_POA_IIRCFG,0x37);  //REG_ATD_POA_IIRCFG
        RegWrite8(REG_ATD_DRO_CCI_CFG3, 0xA6);
        RegWrite8(REG_ATD_DRO_CCI_CFG4, 0x2D);
        
        //CR
        //RegWrite8(REG_ATD_ACR_PN_CFG0,0x1C); //REG_ATD_ACR_PN_CFG0
        //RegWrite8(REG_ATD_ACR_PN_CFG3,0xD9); //REG_ATD_ACR_PN_CFG3
        //RegWrite8(REG_ATD_ACR_QCNF,0x5F);    //REG_ATD_ACR_QCNF
        //RegWrite8(REG_ATD_ACR_CFG,0x59);     //REG_ATD_ACR_CFG
        RegWrite8(REG_ATD_TRAN_STG,0x36);    //REG_ATD_TRAN_STG
        RegWrite8(REG_ATD_PFLL_CFG,0x8c);    //REG_ATD_PFLL_CFG
        RegWrite8(REG_ATD_PFLL_CFG2,0x29);   //REG_ATD_PFLL_CFG2
        RegWrite8(REG_ATD_PRR_SS,0x47);     //REG_ATD_PRR_SS
        RegWrite8(REG_ATD_PRR_TRAN,0x46);   //REG_ATD_PRR_TRAN
        RegWrite8(REG_ATD_CR_CFG,0xf8);	   //REG_ATD_CR_CFG
        RegWrite8(REG_ATD_PRR_PRFK,0x36);    //REG_ATD_PRR_PRFK
        RegWrite8(REG_ATD_PRR_DAMRK,0x46);   //REG_ATD_PRR_DAMRK
        //RegWrite8(REG_ATD_PE_CFG0,0x14);     //REG_ATD_PE_CFG0
        RegWrite8(REG_ATD_PE_CFG1,0x7C);     //REG_ATD_PE_CFG1
        RegWrite8(REG_ATD_PE_CFOK,0x84);     //REG_ATD_PE_CFOK
        RegWrite8(REG_ATD_PJ_CFG,0x24);      //REG_ATD_PJ_CFG
        RegWrite8(REG_ATD_ADVPS_CFG2,0x30);  //REG_ATD_ADVPS_CFG2
        RegWrite8(REG_ATD_TRAN_CFG,0x94);    //REG_ATD_TRAN_CFG
        RegWrite8(REG_ATD_AREG06,0x11);
        RegWrite8(REG_ATD_DRO_DCR_EXT1,0xA0);
        //RegWrite8(REG_ATD_DRO_CVBS_UP_BUD_0, 0x3F);		
        //RegWrite8(REG_ATD_DRO_CVBS_LOW_BUD_0, 0xC0);
        //RegWrite8(REG_ATD_DRO_SCAL_3, 0xF0);
        RegWrite8(REG_ATD_IPS_MODE2, 0x18);
        RegWrite8(REG_ATD_ACR_PN_CFG2, 0x37); //REG_ATD_ACR_PN_CFG2
        break;
    
    case MOD_NTSC_M:
    //case MOD_NTSC_M_60M_44M:
    //case MOD_NTSC_M_54M_44M:
    //case MOD_NTSC_M_54M_36M:
        //PRA
        ucData = 0x61;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG11, &ucData, 1);    	
        ucData = 0x19;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG03, &ucData, 1);    	
        ucData = 0x42;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG04, &ucData, 1);    	
        ucData = 0x8D;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG13, &ucData, 1);    	
        ucData = 0x7F;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_SLOPE, &ucData, 1);    	
        ucData = 0x1E;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);    	
        ucData = 0x20;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1);    	
        ucData = 0x0B;
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_DR_SEL, &ucData, 1);
        
        //CR
        ucData = 0x36;                
        ATD_SetReg(psDemodCtx, REG_ATD_TRAN_STG, &ucData, 1); 
        ucData = 0x8C;
        ATD_SetReg(psDemodCtx, REG_ATD_PFLL_CFG, &ucData, 1);    	
        ucData = 0x29;
        ATD_SetReg(psDemodCtx, REG_ATD_PFLL_CFG2, &ucData, 1);    	
        ucData = 0x47;
        ATD_SetReg(psDemodCtx, REG_ATD_PRR_SS, &ucData, 1);    	
        ucData = 0x46;
        ATD_SetReg(psDemodCtx, REG_ATD_PRR_TRAN, &ucData, 1); 
        ucData = 0xF8;
        ATD_SetReg(psDemodCtx, REG_ATD_CR_CFG, &ucData, 1);    	
        ucData = 0x36;
        ATD_SetReg(psDemodCtx, REG_ATD_PRR_PRFK, &ucData, 1);    	
        ucData = 0x46;
        ATD_SetReg(psDemodCtx, REG_ATD_PRR_DAMRK, &ucData, 1);    	
        ucData = 0x7C;
        ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1); 
        ucData = 0x04;
        ATD_SetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);    	
        ucData = 0x2C;
        ATD_SetReg(psDemodCtx, REG_ATD_PJ_CFG, &ucData, 1);    	
        ucData = 0x30;
        ATD_SetReg(psDemodCtx, REG_ATD_ADVPS_CFG2, &ucData, 1);    	
        ucData = 0x94;
        ATD_SetReg(psDemodCtx, REG_ATD_TRAN_CFG, &ucData, 1);    	
        ucData = 0x0C;                
        ATD_SetReg(psDemodCtx, REG_ATD_AREG13, &ucData, 1);    	
        //DRO
        ucData = 0x7F;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_LPF, &ucData, 1);    	
        ucData = 0xA4; //default 0xD4, GA FT issue 0xA4
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); 
        ucData = 0x08; //default 0x18, GA FT issue 0x08
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG02, &ucData, 1); 
        ucData = 0xD2;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);    	
        ucData = 0x0F;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_RSP_CFG, &ucData, 1);    	
        ucData = 0x87;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG05, &ucData, 1);    	
//        ucData = 0x2C;
//        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1); 
        ucData = 0x68;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_CFG2, &ucData, 1);    	
        ucData = 0xA5;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG3, &ucData, 1);    	
        ucData = 0x12;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG4, &ucData, 1);    	
        ucData = 0x11;
        ATD_SetReg(psDemodCtx, REG_ATD_AREG06, &ucData, 1);    	
        ucData = 0xA9;
        ATD_SetReg(psDemodCtx, REG_ATD_SNR_CFG_0, &ucData, 1);    	
        ucData = 0x17;
        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LSYNC_CTRL, &ucData, 1);    	
        ucData = 0x36;
        ATD_SetReg(psDemodCtx, REG_ATD_IPS_MODE2, &ucData, 1);    	
  //      ucData = 0x3A;
  //      ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
  //      ucData = 0xF0;
  //      ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucData, 1);
        
        RegWrite8(REG_ATD_DRO_REF_COEF, 0xD2); //REG_ATD_DRO_REF_COEF
        RegWrite8(REG_ATD_AREG13, 0x0C);	
                
    	break;            

    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }

    RegWrite8(REG_ATD_ADCIF_CFG, 0x01);  //0x120 
    RegWrite8(REG_ATD_DRO_MASK_CFG,0x37);  //REG_ATD_DRO_MASK_CFG
    RegWrite8(REG_ATD_DRO_MASK_CFG2,0x03);
    RegWrite8(REG_ATD_DRO_WADC_5,0x12);    //REG_ATD_DRO_WADC_5
    //VOP
    RegWrite8(REG_ATD_VOP_FWCPOIH,0x35);     //REG_ATD_VOP_FWCPOIH

    RegWrite8(REG_ATD_CTL_MISC_SEL,0x03);  

    //RegWrite8(REG_ATD_DRO_SCAL_0,0x9F);  //0xDF for higher CVBS swing
    RegWrite8(REG_ATD_DRO_SCAL_4,0x04);
    RegWrite8(REG_ATD_TD_CFG,0x80);
    RegWrite8(REG_ATD_IPS_MODE1,0x0C);
    RegWrite8(REG_ATD_DRO_CVBS_UP_BUD_0, 0x3F);		
    RegWrite8(REG_ATD_DRO_CVBS_LOW_BUD_0, 0xC0);
#ifdef CC_TVE_POWERDOWN_HALF
    RegWrite8(REG_ATD_DRO_SCAL_7,0x40);
    RegWrite8(REG_ATD_DRO_SCAL_3,0xBC);
#endif
	// CR
    RegWrite8(REG_ATD_ACR_PN_CFG0,0x0C); //REG_ATD_ACR_PN_CFG0
    RegWrite8(REG_ATD_ACR_PN_CFG3,0x19); //REG_ATD_ACR_PN_CFG3  //For PN
    RegWrite8(REG_ATD_ACR_SCNF,0x5F);    //REG_ATD_ACR_SCNF
    RegWrite8(REG_ATD_ACR_QCNF,0xDF);    //REG_ATD_ACR_QCNF
    //RegWrite8(REG_ATD_ACR_CFG,0xF9);     //REG_ATD_ACR_CFG  // CR auto config ON    
    RegWrite8(REG_ATD_ACR_CFG,0x39);     //REG_ATD_ACR_CFG  // CR auto config  OFF
    RegWrite8(REG_ATD_PE_CFG0,0x27);     //REG_ATD_PE_CFG0
#ifdef CC_MT5363 //For MT5363    
    RegWrite8(REG_ATD_ADVPS_CFG0,0x1C);  //REG_ATD_CR_PNDEC_CFG
#else //For MT5388
    RegWrite8(REG_ATD_ADVPS_CFG0,0x3C);  //REG_ATD_CR_PNDEC_CFG
#endif
    RegWrite8(REG_ATD_VOP_RESERVE3, 0x66); //REG_ATD_VOP_RESERVE3, nyquist filter on
    //VOP
    RegWrite8(REG_ATD_VOP_FWCPOQH,0x10); 
    // level switch HW patch parameter 
    RegWrite8(REG_ATD_PRA_LP1_CFG02,0xA4); //REG_ATD_PRA_LP1_CFG02
    //To fix CR DTV00138738
    if((u1SubSysIdx==MOD_SECAM_L)||(u1SubSysIdx==MOD_SECAM_L1))
        RegWrite8(REG_ATD_PRA_LP1_CFG02,0x42); //REG_ATD_PRA_LP1_CFG02
        
    RegWrite8(REG_ATD_PRA_TRA_CTL1, 0x83); //REG_ATD_PRA_TRA_CTL1
    RegWrite8(REG_ATD_PRA_TRA_CTL3, 0x1A); //REG_ATD_PRA_TRA_CTL3

}

void ATD_SetIF(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
    UINT8 ucData;

    if ((psDemodCtx->pTCtx->u2IF_Freq == 38000) ||(psDemodCtx->pTCtx->u2IF_Freq_A == 38000))
    {
        switch(u1SubSysIdx)
        {
        case MOD_PAL_BG:
        case MOD_PAL_DK:
        case MOD_PAL_I:
        case MOD_SECAM_L:
        case MOD_NTSC_M:
        //case MOD_NTSC_M_54M_36M:
            ATD_GetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            ucData = (ucData & 0x0F) | 0x40;
            ATD_SetReg(psDemodCtx, REG_ATD_CTL_SYS1, &ucData, 1);
            break;
        default:
            break;
        }
    }
	if(psDemodCtx->pTCtx->u2IF_Freq_A < cLOW_IF_BOUNDARY)
	{
		UINT16 TempFreq=0;
		INT16 s2IfCtrlWord = 0;

		// calculate IF ctrl word
		if (u1SubSysIdx == MOD_SECAM_L1) {
			ucData = 0x98; // spectrum inverse
			TempFreq = 3375 +psDemodCtx->pTCtx->u2IF_Freq_A; // freq shift in DC
		}
		else {
			ucData = 0x80;
			TempFreq =	psDemodCtx->pTCtx->u2IF_Freq_A-3375; // freq shift in DC
		}
		s2IfCtrlWord = (INT16)((TempFreq << 10) /27000); 
		mcSHOW_DBG_MSG(("IfCtrlWord = 0x%x , IF_Freq_A = %d\n", s2IfCtrlWord, psDemodCtx->pTCtx->u2IF_Freq_A));
	
		// write IF ctrl word to DC
		ucData |= ((UINT8)(s2IfCtrlWord>>8) & 0x07);
		RegWrite8(0x129, ucData);
		RegWrite8(0x12a, (UINT8)s2IfCtrlWord);	

	}
}



void ATD_SetSystem(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{   
    UINT8 ucData;
    // reset ATD datapath and registers
    ucData = 0x00; ATD_SetReg(psDemodCtx, REG_ATD_CTL_CFG, &ucData, 1);               

    psDemodCtx->u1SubSysIdx = u1SubSysIdx;

    ATD_SetSystemAudio(psDemodCtx, u1SubSysIdx);
    ATD_SetSystemVideo(psDemodCtx, u1SubSysIdx);
    ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
    ATD_SetIF(psDemodCtx, u1SubSysIdx);   
    ATD_SetAgc(psDemodCtx);
}


UINT8 ATD_StartAcq(ATD_CTX_T *psDemodCtx, UINT8 fgScanMode)
{
    ATD_SetAgc(psDemodCtx);  //100317, ZY, update freq dependent parameter after SetFreq()
#ifdef CC_MT5363 // do not reset demod in MT5363 to avoid video flash when channel scan
    UINT8 ii, ucData;//, ucData2;
                     
    if(fgScanMode)
    {
    	ATD_SetScanMode(psDemodCtx);
    }
    ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);

    
    ucData = 0x00; //EC_code_d9928: Enable MT5363 reset demod to replace ATD_CR_Auto-reacq
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
	
//add for CR DTV00138738 (the root cause is as the same with Eboda field SECAM issue)
//For some SECAM L/L' pattern RF/IF AGC is not stable
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
    ucData = ucData & 0xE7;
    ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
    
    ucData = 0x80;    
    ATD_SetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_1, &ucData, 1);
    ucData = 0x00;    
    ATD_SetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_2, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_2, &ucData, 1);
    
    mcDELAY_MS(10);

    ATD_GetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
    ucData = ucData | 0x18;
    ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
//above is added CR DTV00138738 

    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    
    //ZY, 100126, for SECAL L scan loss issue
    if(fgScanMode)     	
    {   
    	if(psDemodCtx->u1SubSysIdx==MOD_SECAM_L || psDemodCtx->u1SubSysIdx==MOD_SECAM_L1)
    	{
    	  ATD_GetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
          ucData = ucData & 0xFE;   
          ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);  //REG_ATD_PRA_CFG_1[0]=0, fast speed in both ACQ/TRA
        }
    }
    
    mcDELAY_MS(cATD_CR_AUTO_REACQ_TIME);
    
#else     
    UINT8 ii, ucData,ucData1;//, ucData2;
	UINT16 u2RfCtrl,u2IfCtrl;
	
    if(fgScanMode)
    {
    	ATD_SetScanMode(psDemodCtx);
    }
    ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);
	
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);

    //Release PRA,....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    //pucChipReg[REG_ATD_CTL_RST1] = 0xFF;

    //PRA LOOP
    //CheckTmp = 0;
    for (ii = 0; ii < 20; ii ++)
    {
        mcDELAY_MS(10);
		ATD_GetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_1, &ucData, 1);
		ATD_GetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_2, &ucData1, 1);
		
		u2RfCtrl=(UINT16)(ucData<<8)+ucData1;
		
		ATD_GetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_1, &ucData, 1);
		ATD_GetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_2, &ucData1, 1);
		
		u2IfCtrl=(UINT16)(ucData<<8)+ucData1;
		
		mcSHOW_DBG_MSG(("RF_GAIN=0x%04X,IF_GAIN=0x%04X\n",u2RfCtrl,u2IfCtrl))
        ATD_GetReg(psDemodCtx, REG_ATD_PRA_ST, &ucData, 1);
        if ((ucData & 0x40) == 0x40) {
            //mcSHOW_DBG_MSG(("PRA lock!, cnt = %d\n", ii));
            break;
        }
    }

    if(ii==20)
    {
        mcSHOW_USER_MSG(("PRA acquisition fail!\n"));
        return 1;
    }

    //pucChipReg[REG_uP_GP_REG03] = ii;
    //Release CRP....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    //pucChipReg[REG_ATD_CTL_RST2] = 0xFF;

#endif

    //CPR LOOP
    for (ii = 0; ii < 20; ii ++)
    {
        ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucData, 1);
        //ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData2, 1);
        if ((ucData & 0x03) == 0x03)
        {
            mcSHOW_DBG_MSG(("ATD acquisition OK!\n"));
        
            Weak_Sinal_Indicator=0;  // initialize driver patch variables
            Strong_Chroma_Indicator=0;
            weak_count=0;
            CCI_Indicator=0;
            //CCITest=0;                  	                   
            NoOVMCounter0=5;
            PNCounter=5;	       	           	    
            u1CRPatchLoopCnt=0;
            ATD_GetReg(psDemodCtx, REG_ATD_PE_CFG0, &u1NullRegion, 1);            
            break;
        }
        mcDELAY_MS(10);
    }

    if(ii==20)
    {
        mcSHOW_USER_MSG(("CR acquisition fail!\n"));
        return 1;
    }

    return 0;    
}


void ATD_SetNormalMode(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    //RegWrite8(REG_DEMOD_ACR_CFG,0x39);     //EC_code_d100305: Reg0x197[b7-b6] set to 00 Turn-off CR auto Re-acq config
    ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
    ucData = ucData & 0x3F;
    ATD_SetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);

    switch (psDemodCtx->u1SubSysIdx)
    {
    case MOD_SECAM_L:  // to speed up PRA 
    case MOD_SECAM_L1:
        mcSHOW_DBG_MSG(("Set normal mode parameter for SECAM L/L'\n"));
	#if  0
        ucData = 0x3E;  // REG_ATD_PRA_LP1_CFG03 = 0x3E
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG03, &ucData, 1);
        ATD_GetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG12, &ucData, 1);
        ucData &= 0xF8;  // REG_ATD_PRA_LP1_CFG12[2:0] = 3
        ucData |= 0x03;  
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG12, &ucData, 1);
        ucData = 0x2D;  // REG_ATD_PRA_LP1_CFG13 = 0x2D
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG13, &ucData, 1);
        ucData = 0x1F;  // REG_ATD_PRA_CFG_1 = 0x1F
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
	#endif
	
        break;

    case MOD_PAL_BG:  // set SIF filter of BG back to 5.7MHz   
        ucData = 0x94;
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ucData = 0x20;
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);
        ucData = 0x81;
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xB0;
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;
    case MOD_PAL_DK:  // set SIF filetr of DKI back to defualt setting
    case MOD_PAL_I:
        ucData = 0x00;
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);        
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);     
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);	
    default:
        break;
    }
   
}


void ATD_DisConnect(ATD_CTX_T *psDemodCtx)
{
#ifdef CC_MT5363
    //UINT8 ucData;
    UINT32 u4Temp;

    // AD cannot be power down for uP 
    // power down AD clock
    //u4Temp = IO_READ32(REG_ATD_DRO_OBS20d000, REG_ATD_CPR_EXT_FREQ1);
    //u4Temp |= 0x000000e0;
    //IO_WRITE32(REG_ATD_DRO_OBS20d000, REG_ATD_CPR_EXT_FREQ1, u4Temp);
    
    //RegWrite8(REG_ATD_CTL_TEST, 0x7F); //REG_ATD_CTL_TEST, all clk power down; 
    // Please don't power down CTL_TEST due to TVD module still refer to demod54m_ck/demod60m_ck
    
    //disbale ATD (b4 power down ADCLK)
    //ucData = 0x00;
    //ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    //ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    //mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(REG_ATD_TVDIF_CFG0)));

    // for demod60m_ck = TVDPLL/9 (b4 power down ADCLK) //EC_code_d9A09 for power down ADC setting for eIntATD_ExtDTD
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_TD_MIN_1);
    u4Temp &= ~((U32)0x03); //EC_code_dA408 to add mask for ADC power down correct setting
    u4Temp |=  ((U32)0x01); // 2000d218[1:0]=2'b01
    IO_WRITE32(CKGEN_BASE, REG_ATD_TD_MIN_1, u4Temp);
    
    // for demod54m_ck = TVDPLL/10 (b4 power down ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_TD_BPF_MUL_COEF_0);
    u4Temp &= ~((U32)0x03); //EC_code_dA408 to add mask
    u4Temp |=  ((U32)0x01); // 2000d21c[1:0]=2'b01
    IO_WRITE32(CKGEN_BASE, REG_ATD_TD_BPF_MUL_COEF_0, u4Temp);

    // power down AD clock
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_CPR_EXT_FREQ1);
    u4Temp |= 0x000000e0;
    IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_EXT_FREQ1, u4Temp);

    
    //Set MT5363 RFAGC and IFAGC to high-Z   (pin mux: [29:28] = 11, GPIO: [9:8] = 00) //EC_code_d9826 for eIntATD_ExtDTD
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
    u4Temp |= 0x30000000;
    IO_WRITE32(CKGEN_BASE, 0x408, u4Temp);
    
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x514);
    u4Temp &= 0xFFFFFCFF;
    IO_WRITE32(CKGEN_BASE, 0x514, u4Temp);

#else
 #if 0
    // for demod60m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_TD_MIN_1);
    u4Temp &= ~((U32)0x03); // 2000d218[1:0]=2'b01
    u4Temp |=  ((U32)0x01); 
    IO_WRITE32(CKGEN_BASE, REG_ATD_TD_MIN_1, u4Temp);


    // for demod54m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, REG_ATD_TD_BPF_MUL_COEF_0);
    u4Temp &= ~((U32)0x03); // 2000d21c[1:0]=2'b01
    u4Temp |=  ((U32)0x03); 
    IO_WRITE32(CKGEN_BASE, REG_ATD_TD_BPF_MUL_COEF_0, u4Temp);

    //disbale ATD
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(REG_ATD_TVDIF_CFG0)));

     UINT8 ucData;
    UINT32 uData;

    // Enable DVBT and Disable ATD, LC, write REG_ATD_PE_CFG0 to 0x02
    //mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b0, 0x00000002);
    //mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b4, 0x4001006d);
    /*DVBT_GetReg(psDemodCtx, MT5391_REG_ANA_PD_MODE, &ucData, 1);
    ucData &= 0xEF;   // b'4 = 0
    DVBT_SetReg(psDemodCtx, MT5391_REG_ANA_PD_MODE, &ucData, 1); */

   
    //disbale ATD
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST1, &ucData, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(REG_ATD_TVDIF_CFG0)));
    ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    ucData &= 0xFD;
    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG0, &ucData, 1);
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(REG_ATD_TVDIF_CFG0)));
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module\n"));


    //Close ADC Circuit
    uData = IO_READ32(CKGEN_BASE, REG_ATD_CPR_EXT_FREQ1);
    uData |= 0x80000001;
    IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_EXT_FREQ1, uData);

    uData = IO_READ32(CKGEN_BASE, REG_ATD_CPR_CFO_1);
    uData |= 0xE0000000;
    IO_WRITE32(CKGEN_BASE, REG_ATD_CPR_CFO_1, uData);  
#endif   
#endif   
    
}

void ATD_SetSnowMode(ATD_CTX_T *psDemodCtx, UINT8 fgSnowOn)
{
}
void ATD_BypassSIF2Aud(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{

}
void ATD_DumpRegister(ATD_CTX_T *psDemodCtx, UINT16 u2ModuleMask)
{

}


