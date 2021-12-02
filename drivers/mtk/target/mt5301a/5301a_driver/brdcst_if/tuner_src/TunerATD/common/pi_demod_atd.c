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
 * $RCSfile: pi_tuner_teqe3.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// **** REMOVE after common/specific part done!!!  *** //

#include "pi_demod_atd.h"
#include "pi_def_atd.h"
//#include "pd_common.h"
#include "PD_Def_dbg.h"  /*need include RISC IF macro, if re-ware RISC IF , remove this*/
#include "drvcust_if.h"
#include "drv_tvd.h"
#include "x_ckgen.h"
#include "drv_pwm.h"

//New RISC IF
#include "ctrl_bus.h"


//For sound carrier shift and OVM
#include "aud_if.h"

/***********************************************************************/
/*                  Defines                                            */
/***********************************************************************/

#define cATD_CR_AUTO_REACQ_TIME  70

//-----------------------------------------------------------------------------
// RISC IF Related Implementation
//-----------------------------------------------------------------------------
#define cRISC_ATD_BASE           (IO_VIRT + 0x2B000)
#define cRISC_TVD_BASE           (IO_VIRT + 0x22000)


STATIC PARAM_RISC_T sATDRiscCtx;
volatile UINT32 *puATD_Reg = (volatile UINT32 *) cRISC_ATD_BASE;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define RegWrite8(Addr, Value)    \
{                                 \
    ucData = Value; ATD_SetReg(psDemodCtx, (Addr+0x800), &ucData, 1);  \
}

#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_USER_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
} \

/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/

/***********************************************************************/
/*                  Private Types                                      */
/***********************************************************************/

/***********************************************************************/
/*                  Global variables                                   */
/***********************************************************************/
static UINT32 TestItem = 0;
static UINT32 QLabCondition[2] = {700,5};

static UINT8 Weak_Sinal_Indicator=0, weak_count = 0;	
static UINT8 Strong_Chroma_Indicator=0;
//static UINT8 fgAudCFO=0;
static UINT8 SmallAGC=0;

static S8 CCI_Indicator=0;
//static UINT8 CCITest=0;
static UINT8 NoOVMCounter0=5;
static UINT8 PNCounter=5;	
static UINT8 u1CRPatchLoopCnt = 0;

UINT8 CCI_Tone_BW3[4][3] = {{0x0F,0xFE,0x00},{0x0F,0xFD,0xFD},{0x0F,0xFD,0xF9},{0x0F,0xFD,0xCF}};
UINT8 CCI_Tone_BW2[4][3] = {{0x0F,0xFE,0x00},{0x0F,0xFB,0xFD},{0x0F,0xFB,0xF9},{0x0F,0xFB,0xCF}};


/***********************************************************************/
ATD_CTX_T *ATD_DemodCtxCreate(void)
{
    ATD_CTX_T *p = NULL;

	CREATE_OBJECT(p, ATD_CTX_T);

    // error handling
    if (!p)
       return NULL;

   	// zero memory
	memset(p, 0, sizeof(ATD_CTX_T));

	return p;
}

void ATD_DemodCtxDestroy(ATD_CTX_T *p)
{
    DESTROY_OBJECT(p);
}


/***********************************************************************/
/*                      Private (static) Function                      */
/***********************************************************************/
/***********************************************************************/
UINT8 ATD_SetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
#ifndef CC_MT8223
    if (ICtrlBus_RiscDemWrite(&sATDRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount))
#else
    if (ICtrlBus_RiscDemWrite(&sATDRiscCtx, u2RegAddr-0x800, pu1Buffer, u2ByteCount))
#endif
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Write %d byte Error: Reg[%03X] = 0x%02X!\n",
                       u2ByteCount, u2RegAddr, *pu1Buffer));

		return 1;
    }

	return 0;
}

/************************************************************************/
/* Read consecutive registers */
UINT8 ATD_GetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
#ifndef CC_MT8223
    if (ICtrlBus_RiscDemRead(&sATDRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount))
#else
	if (ICtrlBus_RiscDemRead(&sATDRiscCtx, u2RegAddr-0x800, pu1Buffer, u2ByteCount))
#endif
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Read %d byte Error: Reg[%03X] = 0x%02X!\n",
                       u2ByteCount, u2RegAddr, *pu1Buffer));

        return 1;
    }

    return 0;
}



/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/

/**********************************************************************/
/*                  Initialization APIs                               */
/**********************************************************************/

/**********************************************************************/
/*                  Configuration APIs                                */
/**********************************************************************/


/**********************************************************************/
INT32 ATD_Initialize(ATD_CTX_T *psDemodCtx, UINT8 ucI2cAddress, BOOL fgLoadCode)
{
    if (!psDemodCtx)
        return DRVAPI_TUNER_ERROR;

    psDemodCtx->I2cAddress = ucI2cAddress;
    psDemodCtx->fgDemodReset = FALSE;
    psDemodCtx->u1SubSysIdx = MOD_PAL_BG;  // PAL_BG
    psDemodCtx->fgPRALock = FALSE;
    psDemodCtx->fgCRLock = FALSE;
    psDemodCtx->fgAutoSearch = FALSE;
    psDemodCtx->fgAFT = TRUE;
    psDemodCtx->u1PF = 0x01;
    psDemodCtx->u2VOPCpo = 0;
    psDemodCtx->s4Cfo = 0;
    psDemodCtx->u1VIFLock = cATD_VIF_UNLOCK;
    psDemodCtx->u1UpChkCnt = 0;
    psDemodCtx->u1UpNakCnt = 0;
    psDemodCtx->u1UpMisCnt = 0;
    psDemodCtx->u4Frequency = 181250; //default frequency

    psDemodCtx->s1IFmin = 0x80;
    psDemodCtx->s1RFmin = 0x80;

   // psDemodCtx->pDigiTunerCtx = GetDigiTunerCtx();

    //For new RISC IF       
    sATDRiscCtx.pu4RegBase = puATD_Reg;


	// create semaphore for HostCmd Interface
    if (mcSEMA_CREATE(&psDemodCtx->hHostCmdLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hHostCmdLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);

    if (mcSEMA_CREATE(&psDemodCtx->hLogLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hLogLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK(psDemodCtx->hLogLock);

	if (mcSEMA_CREATE(&psDemodCtx->hRegLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hRegLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }
    mcSEMA_UNLOCK(psDemodCtx->hRegLock);

	if (!psDemodCtx->fgDemodInitFlag)
    {
        psDemodCtx->fgDemodInitFlag = TRUE;
    //    if (mcSEMA_CREATE(&hDemodAcq, X_SEMA_STATE_UNLOCK) != OSR_OK)
        if (mcSEMA_CREATE(&psDemodCtx->hDemodAcq, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hDemodAcq) Fail!\n"));
            return (DRVAPI_TUNER_ERROR);
        }
        mcSEMA_UNLOCK(psDemodCtx->hDemodAcq);
    }

    ATD_ChipInit(psDemodCtx);

// According to MW/Albert, Extinguish operation after opening.
    mcBLOCK_I2C(TRUE);


    return (DRVAPI_TUNER_OK);
}

UINT8 ATD_ChipInit(ATD_CTX_T *psDemodCtx)
{
#ifndef CC_MT8223

#ifdef CC_MT5363 //Justin TODO

// Select Clock Source
// CKGEN_BASE: 0x2000dxxx
// ATD_BASE:   0x2002axxx
// DVBT_BASE:  0x2002bxxx
    UINT32 u4Temp;
    UINT8 ucData;

    // for 27M Xtal TVDPLL to 540MHz
    //IO_WRITE32(0x2000d000, 0x130, 0x00099401);

    // ADC setting & sampling from PLL
    if (GET_XTAL_CLK() == CLK_54MHZ) { // for 54M Xtal
        IO_WRITE32(CKGEN_BASE, 0x1d0, 0x00aa4000); // from Xtal
    }
    else{ // for 27M Xtal
        IO_WRITE32(CKGEN_BASE, 0x1d0, 0x00aa4100); // from ADCLK
    }
    IO_WRITE32(CKGEN_BASE, 0x1d4, 0x00000054);

    // Demod AD clock enable
    u4Temp = IO_READ32(CKGEN_BASE, 0x12c);
    u4Temp |=  ((U32)0x01) << 26; //2000d12c[26]=1'b1
    IO_WRITE32(CKGEN_BASE, 0x12c, u4Temp);

    // set PLL clock = 108M for AD
    u4Temp = IO_READ32(CKGEN_BASE, 0x134);
    u4Temp |=  (((U32)0x01) << 4); //2000d134[4]=1'b1
    u4Temp &= ~(((U32)0x03) << 5); //2000d134[6:5]=2'b00
    u4Temp &= ~(((U32)0x0F) << 8); //2000d134[11:8]=4'b0100
    u4Temp |=  (((U32)0x04) << 8);
    // ADC reset (while new setting) 2000d134[12]=1'b0 2000d134[14]=1'b0
    u4Temp &= ~((((U32)0x01) << 12) | (((U32)0x01) << 14));
    IO_WRITE32(CKGEN_BASE, 0x134, u4Temp);
    // ADC release (while new setting)2000d134[12]=1'b1 2000d134[14]=1'b1
    u4Temp |=  ((((U32)0x01) << 12) | (((U32)0x01) << 14));
    IO_WRITE32(CKGEN_BASE, 0x134, u4Temp);

    // for demod60m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, 0x218);
    u4Temp |=  ((U32)0x03); // 2000d218[1:0]=2'b11
    IO_WRITE32(CKGEN_BASE, 0x218, u4Temp);


    // for demod54m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, 0x21C);
    u4Temp |=  ((U32)0x03); // 2000d21c[1:0]=2'b11
    IO_WRITE32(CKGEN_BASE, 0x21C, u4Temp);

    // for demod27m_ck = 27Mz (TVDPLL)
    u4Temp = IO_READ32(CKGEN_BASE, 0x204);
    u4Temp &= ~(((U32)0x03) << 8); //2000d204[9:8]=2'b01
    u4Temp |=  (((U32)0x01) << 8);    
    IO_WRITE32(CKGEN_BASE, 0x204, u4Temp);

    // set byte mask
    u4Temp = IO_READ32(cRISC_ATD_BASE, 0x4b4);
    u4Temp |=  (((U32)0x0F) << 16);//u4Temp |= 0x000f0000;    
    IO_WRITE32(cRISC_ATD_BASE, 0x4b4, u4Temp);

    // power on clk
    RegWrite8(REG_DEMOD_CTL_TEST, 0x00); //0x932, all clk power on

    //Need to set RISC wait time => to correct get multi-regiser bug
    ucData = 0x1e; 
    ATD_SetReg(psDemodCtx, 0x4a3, &ucData, 1);       

    // RFAGC, IFAGC pin mux: [29:28] = 00 //EC_code_d9826 for eIntATD_ExtDTD
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
    u4Temp &= 0xcFFFFFFF;
    mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
    
    return 0;

#else

    UINT32 u4Temp;
    UINT8 ucData;


    IO_WRITE32(CKGEN_BASE,0x210, 0x00000001);	
    IO_WRITE32(CKGEN_BASE,0x214, 0x00000001);            	
    IO_WRITE32(CKGEN_BASE,0x218, 0x00000002);            	
    IO_WRITE32(CKGEN_BASE,0x21c, 0x00000001);               	

    //1. Write DTD 111H=02h (disable dtd sampling rate switch)
    //IO_WRITE32(0x2002b000, 0x4b0, 0x00000200);               	
    //IO_WRITE32(0x2002b000, 0x4b4, 0x40020044);                   
    ucData = 0x02;
	ATD_SetReg(psDemodCtx, 0x111, &ucData, 1);

    //IO_WRITE32(0x2002b000,0x4b0, 0x00AA4100);               	
    //IO_WRITE32(0x2002b000,0x4b4, 0x400f0046);


    //IO_WRITE32(0x2002b000,0x4b0, 0x00000000);               	
    //IO_WRITE32(0x2002b000,0x4b4, 0x40010046);
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, 0x118, &ucData, 1);


    //2. Write DTD 119H=41h
    //IO_WRITE32(0x2002b000,0x4b0, 0x00004100);
    //IO_WRITE32(0x2002b000,0x4b4, 0x40020046);
    ucData = 0x41;
    ATD_SetReg(psDemodCtx, 0x119, &ucData, 1);

    //Write DTD 11AH=AAh (same as default)
    //IO_WRITE32(0x2002b000,0x4b0, 0x00AA0000);               	
    //IO_WRITE32(0x2002b000,0x4b4, 0x40040046);                   
    ucData = 0xAA;
    ATD_SetReg(psDemodCtx, 0x11A, &ucData, 1);


    //Write DTD 11BH=00h  (same as default)
    //IO_WRITE32(0x2002b000,0x4b0, 0x00000000);               	
    //IO_WRITE32(0x2002b000,0x4b4, 0x40080046);               	
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, 0x11B, &ucData, 1);

    //3. Write DTD 11CH=94h  (same as default)
    //IO_WRITE32(0x2002b000, 0x4b0, 0x00000094);               	
    //IO_WRITE32(0x2002b000, 0x4b4, 0x40010047);               	
    //set AD input swing from 1.8Vp_p t 2Vp_p
    //IO_WRITE32(0x2002b000, 0x4b0, 0x00000054);
    ucData = 0x54;
    ATD_SetReg(psDemodCtx, 0x11C, &ucData, 1);
               	
    if(psDemodCtx->pTCtx->fgRFAGC == TRUE)
    {
    //IO_WRITE32(0x2000d000, 0x400, 0x00000300);  
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
    ATD_SetReg(psDemodCtx, 0x924, &ucData, 1);
    ucData = 0x23;
    ATD_SetReg(psDemodCtx, 0x9E0, &ucData, 1);

    //IO_WRITE32(0x2000d2a8) bit[13:12] = 2'b10 Demod clock to VDAC from clock demod60m_ck
    u4Temp = IO_READ32(CKGEN_BASE, 0x2a8);
    u4Temp |= 0x00002000; 
    IO_WRITE32(CKGEN_BASE, 0x2a8, u4Temp);

    if (IS_XTAL_54MHZ())
    {
        //(set for 54MXTAL)
        IO_WRITE32(CKGEN_BASE, 0x218, 0x00000000); 
        //set ADC clock from xtal: write DTD 119H = 40h
        //IO_WRITE32(cRISC_ATD_BASE, 0x4b0, 0x00004000);
        //IO_WRITE32(cRISC_ATD_BASE, 0x4b4, 0x40020046);
        ucData = 0x40;
        ATD_SetReg(psDemodCtx, 0x119, &ucData, 1);    
    }
    
    //Reset ADC divider & release
    IO_WRITE32(CKGEN_BASE,0x134, 0x00086410);               	
    IO_WRITE32(CKGEN_BASE,0x134, 0x00087410);


    // 4. Reset & release ADC latch data FF: (Set Demod adr 0x110 to be 0x00, then set to be 0x02)
    //u4Temp = IO_READ32(CKGEN_BASE, 0x1d4);
    //u4Temp |= 0xE0000000; 
    //IO_WRITE32(CKGEN_BASE, 0x1d4, u4Temp);

    IO_WRITE32(cRISC_ATD_BASE,0x4b0, 0x00000000);               	
    IO_WRITE32(cRISC_ATD_BASE,0x4b4, 0x40010044);               	
    IO_WRITE32(cRISC_ATD_BASE,0x4b0, 0x00000002);               	
    mcDELAY_MS(10);
    IO_WRITE32(cRISC_ATD_BASE,0x4b4, 0x40010044);


    //5. APLL setting for 2.54MHz spur solution and it should be setted on loader//EC_code_d9417   
    /*IO_WRITE32(0x2000d000, 0x100, 0x004a19C4);
    IO_WRITE32(0x2000d000, 0x138, 0x004a19C4);
    IO_WRITE32(0x2000d000, 0x104, 0x03A4114B);
    IO_WRITE32(0x2000d000, 0x13C, 0x03A4114B);    
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

#else


	if(IS_XTAL_54MHZ())
	{
    	vDrvSetPWM(SrcPWM0, 0x00, 0x0e, 0x31);
		//printf("IS_XTAL_54MHZ set pwm 30v");
	}else// if(IS_XTAL_27MHZ())
	{
    	vDrvSetPWM(SrcPWM0, 0x00, 0x5d, 0x95);
		//printf("IS_XTAL_27MHZ set pwm 30v");
	}

	IO_WRITE32(0x2000d000, 0x21c, 0x00030001);

    return 0;
	
#endif

}

void ATD_SetSystemAudio(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
    UINT8   ucData;      

    switch (u1SubSysIdx)
    {
    case MOD_PAL_BG: 
        mcSHOW_DBG_MSG(("Set System = PAL BG 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);               
        ucData = 0x33; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);               
        ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);        
#endif
        //ucData = 0xE0; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
        //ucData = 0x78; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
        //ucData = 0x9C; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
        ucData = 0xE1; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
        ucData = 0xF8; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
        ucData = 0x73; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
        // set AVS/AUC audio filter
        ucData = 0x94; ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0x20; ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);        
        break;
    
    case MOD_PAL_DK: 
        mcSHOW_DBG_MSG(("Set System = PAL DK 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);               
        ucData = 0x35; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);               
        ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);  
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
#endif
        // set AVS filter
        ucData = 0xDB; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
        ucData = 0x78; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
        ucData = 0x25; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
        break;
    
    case MOD_PAL_I: 
        mcSHOW_DBG_MSG(("Set System = PAL I 54M/39M\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);               
        ucData = 0x34; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);               
        ucData = 0x00; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);               
        ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
#ifdef CC_TVE_POWERDOWN_HALF
        ucData = 0x2E; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDB; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);       
#else               
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x38; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
#endif
        // set AVS filter
        //ucData = 0xD6; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
        //ucData = 0xF8; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
        //ucData = 0x9A; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
        ucData = 0xD8; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
        ucData = 0x78; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
        ucData = 0x70; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);

        break;
    
    case MOD_SECAM_L: 
        mcSHOW_DBG_MSG(("Set System = SECAM L	\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);               
        ucData = 0x35; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);               
        ucData = 0x0C; ATD_SetReg(psDemodCtx, 0x90D, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);               
        ucData = 0x03; ATD_SetReg(psDemodCtx, 0x928, &ucData, 1);               
        ucData = 0x99; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
        //ucData = 0x11; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        //ucData = 0x9E; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);
        
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x10; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
        ucData = 0x84; ATD_SetReg(psDemodCtx, 0xA37, &ucData, 1);  
        ucData = 0x00; ATD_SetReg(psDemodCtx, 0xA38, &ucData, 1);  
        
        break;
    
    case MOD_SECAM_L1: 
        mcSHOW_DBG_MSG(("Set System = SECAM L'	\n"));
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);               
        ucData = 0x95; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);               
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);               
        ucData = 0x0C; ATD_SetReg(psDemodCtx, 0x90D, &ucData, 1);               
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);               
        ucData = 0x03; ATD_SetReg(psDemodCtx, 0x928, &ucData, 1);               
        ucData = 0x99; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
        //ucData = 0x11; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        //ucData = 0x9E; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);               
        
        //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
        ucData = 0x10; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
        ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
        ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
        ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
        ucData = 0x84; ATD_SetReg(psDemodCtx, 0xA37, &ucData, 1);  
        ucData = 0x00; ATD_SetReg(psDemodCtx, 0xA38, &ucData, 1);  
        
        break;
#if 0    
    case MOD_NTSC_M_60M_44M: // NTSC M/N 60M/44M
        mcSHOW_DBG_MSG(("Set System = NTSC M/N 60M/44M\n"));			
        ucData = 0x02; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);	    
        ucData = 0x02; ATD_SetReg(psDemodCtx, 0x902, &ucData, 1);		
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x920, &ucData, 1);			
        ucData = 0x11; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);			
        ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);		
        ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);			
        ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
        ucData = 0x4E; ATD_SetReg(psDemodCtx, 0x9CF, &ucData, 1);
        ucData = 0x3F; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);
        ucData = 0xE4; ATD_SetReg(psDemodCtx, 0x96A, &ucData, 1);
        ucData = 0x9C; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);			
        break;
#endif    
    case MOD_NTSC_M:
    
        if (psDemodCtx->pTCtx->u2IF_Freq == 44000)
        {
            mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/44M\n"));
            //TV System Setting
            ucData = 0x03; ATD_SetReg(psDemodCtx, 0x93E, &ucData, 1);               
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);
            ucData = 0x22; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);               
            ucData = 0x66; ATD_SetReg(psDemodCtx, 0x9CF, &ucData, 1); 
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, 0x96A, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);	           
#else
             //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);	           
        }
        else
        {            
          if (psDemodCtx->pTCtx->u2IF_Freq == 36167)  //forTWN  NTSC 38Mhz,DVBT + ATDNA
          {
            mcSHOW_DBG_MSG(("Set System = TWN NTSC M/N 54M/38M\n"));
		    ucData = 0x03; ATD_SetReg(psDemodCtx, 0x93E, &ucData, 1);  
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);
            ucData = 0x42; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
		    ucData = 0x66; ATD_SetReg(psDemodCtx, 0x9CF, &ucData, 1); 
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, 0x96A, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);	
#else
            //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);


		  }
		  else
		  { mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/36M\n"));
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x903, &ucData, 1);
            ucData = 0x32; ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);
            ucData = 0x0E; ATD_SetReg(psDemodCtx, 0x900, &ucData, 1);
            ucData = 0x01; ATD_SetReg(psDemodCtx, 0x907, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, 0x93B, &ucData, 1);
            ucData = 0x98; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
#ifdef CC_TVE_POWERDOWN_HALF
            ucData = 0x2E; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);
            ucData = 0x00; ATD_SetReg(psDemodCtx, 0x96A, &ucData, 1);
            ucData = 0xDB; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1);	
#else
            //EC_code_d9918: Fine tune TVD_IF interface for CR [DTV00065325]
            ucData = 0x38; ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);               
            ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA45, &ucData, 1); 
            ucData = 0x40; ATD_SetReg(psDemodCtx, 0xA08, &ucData, 1);  
            ucData = 0xB0; ATD_SetReg(psDemodCtx, 0xA48, &ucData, 1);  
#endif            
            ucData = 0xF4; ATD_SetReg(psDemodCtx, 0x9C0, &ucData, 1);
            ucData = 0xF8; ATD_SetReg(psDemodCtx, 0x9C1, &ucData, 1);
            ucData = 0x70; ATD_SetReg(psDemodCtx, 0x9C9, &ucData, 1);
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
        RegWrite8(REG_DEMOD_PRA_LP1_CFG11,0x61); //0x14A
        RegWrite8(REG_DEMOD_PRA_LP1_CFG04,0x42); //0x143
        RegWrite8(REG_DEMOD_PRA_LP1_CFG03,0x19); //0x142
        RegWrite8(REG_DEMOD_PRA_LP1_CFG13,0x8D); //0x14C
        RegWrite8(REG_DEMOD_PRA_IF_SLOPE,0x7F);  //0x154
        RegWrite8(REG_DEMOD_PRA_CFG_1,0x1E);     //0x159
        RegWrite8(REG_DEMOD_PRA_DR_SEL,0x0B);    //0x15F
        RegWrite8(REG_DEMOD_DRO_PROBE_CFG,0x20); //0x16C
        
        //CR
        RegWrite8(REG_DEMOD_TRAN_STG,0x36);    //0x198
        RegWrite8(REG_DEMOD_PFLL_CFG,0x8c);    //0x199
        RegWrite8(REG_DEMOD_PFLL_CFG2,0x29);   //0x19A
        RegWrite8(REG_DEMOD_PFLT_SS,0x47);     //0x1A8
        RegWrite8(REG_DEMOD_PFLT_TRAN,0x46);   //0x1A9
        RegWrite8(REG_DEMOD_CR_CFG,0xf8);	   //0x1AA
        RegWrite8(REG_DEMOD_PRR_PRFK,0x36);    //0x1B2
        RegWrite8(REG_DEMOD_PRR_DAMRK,0x46);   //0x1B3
        RegWrite8(REG_DEMOD_PE_CFG1,0x7C);     //0x1B5
        RegWrite8(REG_DEMOD_PE_CFOK,0x04);     //0x1B6
        RegWrite8(REG_DEMOD_PJ_CFG,0x2C);      //0x1B7
        RegWrite8(REG_DEMOD_ADVPS_CFG2,0x30);  //0x1BB
        RegWrite8(REG_DEMOD_TRAN_CFG,0x94);    //0x1BF
        RegWrite8(0x10D,0x0C);
        //DRO
        RegWrite8(REG_DEMOD_DRO_CVBS_LPF,0x7F); //0x174
        RegWrite8(REG_DEMOD_DRO_LP_CFG01,0xA4); //0x164, async mode, default 0xD4, GA FT issue 0xA4
        RegWrite8(REG_DEMOD_DRO_LP_CFG02,0x08);  //0x165, defualt 0x18, GA FT issue 0x08
        RegWrite8(REG_DEMOD_DRO_REF_COEF,0xD2); //0x17A
        RegWrite8(REG_DEMOD_DRO_RSP_CFG,0x0F);  //0x17E
        RegWrite8(REG_DEMOD_DRO_LP_CFG05,0x87); //0x168
        RegWrite8(REG_DEMOD_DRO_REF_CFG2,0x68); //0x17C
        RegWrite8(REG_DEMOD_POA_TARG_LVH,0x20); //0x183
        RegWrite8(0x234, 0xA5);
        RegWrite8(0x235, 0x12);
        
        //FPGA Interpolartor			
        RegWrite8(0x106,0x11);
#ifdef CC_TVE_POWERDOWN_HALF
        RegWrite8(0x16A,0x00);
        RegWrite8(0x248,0xBC);
        RegWrite8(0x208,0x40);
#else
        RegWrite8(0x16A,0xE4);
#endif
        RegWrite8(0x214, 0x17);
        RegWrite8(0x255, 0x36);
        break;

    case MOD_SECAM_L:
    case MOD_SECAM_L1:
		//PRA
        RegWrite8(REG_DEMOD_PRA_LP1_CFG01,0x1A); //0x140

#if  0
        RegWrite8(REG_DEMOD_PRA_LP1_CFG03,0x3E); //0x142
        RegWrite8(REG_DEMOD_PRA_LP1_CFG04,0x42); //0x143
        RegWrite8(REG_DEMOD_PRA_LP1_CFG07,0x5C); //0x146
        RegWrite8(REG_DEMOD_PRA_LP1_CFG08,0x8C); //0x147
#else
        //To fix CR DTV00138738
        RegWrite8(REG_DEMOD_PRA_LP1_CFG03,0x54); //0x142
        RegWrite8(REG_DEMOD_PRA_LP1_CFG04,0x22); //0x143
        RegWrite8(REG_DEMOD_PRA_LP1_CFG07,0x59); //0x146
        RegWrite8(REG_DEMOD_PRA_LP1_CFG08,0xCC); //0x147
        RegWrite8(REG_DEMOD_PRA_LP1_CFG09,0xFF); //0x148
        RegWrite8(REG_DEMOD_PRA_LP1_CFG10,0x33); //0x149
#endif
		
        RegWrite8(REG_DEMOD_PRA_LP1_CFG11,0x61); //0x14A
        //To fix CR DTV00138738
        //RegWrite8(REG_DEMOD_PRA_LP1_CFG13,0x2D); //0x14C
        RegWrite8(REG_DEMOD_PRA_LP1_CFG13,0x1D); //0x14C
        RegWrite8(REG_DEMOD_PRA_IF_SLOPE,0x7F);  //0x154
        RegWrite8(REG_DEMOD_PRA_CFG_1,0x1F);     //0x159
        RegWrite8(REG_DEMOD_PRA_DR_SEL,0x0B);    //0x15F
        RegWrite8(REG_DEMOD_DRO_PROBE_CFG,0x20); //0x16C
        //RegWrite8(REG_DEMOD_DRO_WADC_5,0x32);    //0x173
        
        //DRO
        RegWrite8(REG_DEMOD_DRO_LP_CFG02,0x58);  //0x165
        //RegWrite8(REG_DEMOD_DRO_LP_CFG03,0x11);  //0x166
        RegWrite8(REG_DEMOD_DRO_LP_CFG04,0xD9);  //0x167
        RegWrite8(REG_DEMOD_DRO_LP_CFG05,0x87);  //0x168
        RegWrite8(REG_DEMOD_DRO_CVBS_LPF,0x7F);  //0x174
        RegWrite8(REG_DEMOD_DRO_LP_CFG01,0xDE);  //0x164, sync mode
        //RegWrite8(REG_DEMOD_DRO_LP_CFG01,0xD6);  //0x164, async mode
        RegWrite8(REG_DEMOD_DRO_REF_COEF,0x2D);  //0x17A
        RegWrite8(REG_DEMOD_DRO_REF_CFG2,0x68);  //0x17C
        RegWrite8(REG_DEMOD_DRO_RSP_CFG,0x0F);   //0x17E
        //RegWrite8(REG_DEMOD_DRO_MASK_CFG,0x27);  //0x17D
        RegWrite8(REG_DEMOD_ATD_POA_CFG,0xE8);  //0x180
        RegWrite8(REG_DEMOD_POA_TARG_LVH,0x1A); //0x183
        RegWrite8(REG_DEMOD_POA_IIRCFG,0x37);  //0x18A
        RegWrite8(0x234, 0xA6);
        RegWrite8(0x235, 0x2D);
        
        //CR
        //RegWrite8(REG_DEMOD_ACR_PN_CFG0,0x1C); //0x190
        //RegWrite8(REG_DEMOD_ACR_PN_CFG3,0xD9); //0x193
        //RegWrite8(REG_DEMOD_ACR_QCNF,0x5F);    //0x196
        //RegWrite8(REG_DEMOD_ACR_CFG,0x59);     //0x197
        RegWrite8(REG_DEMOD_TRAN_STG,0x36);    //0x198
        RegWrite8(REG_DEMOD_PFLL_CFG,0x8c);    //0x199
        RegWrite8(REG_DEMOD_PFLL_CFG2,0x29);   //0x19A
        RegWrite8(REG_DEMOD_PFLT_SS,0x47);     //0x1A8
        RegWrite8(REG_DEMOD_PFLT_TRAN,0x46);   //0x1A9
        RegWrite8(REG_DEMOD_CR_CFG,0xf8);	   //0x1AA
        RegWrite8(REG_DEMOD_PRR_PRFK,0x36);    //0x1B2
        RegWrite8(REG_DEMOD_PRR_DAMRK,0x46);   //0x1B3
        //RegWrite8(REG_DEMOD_PE_CFG0,0x14);     //0x1B4
        RegWrite8(REG_DEMOD_PE_CFG1,0x7C);     //0x1B5
        RegWrite8(REG_DEMOD_PE_CFOK,0x84);     //0x1B6
        RegWrite8(REG_DEMOD_PJ_CFG,0x24);      //0x1B7
        RegWrite8(REG_DEMOD_ADVPS_CFG2,0x30);  //0x1BB
        RegWrite8(REG_DEMOD_TRAN_CFG,0x94);    //0x1BF
        RegWrite8(0x106,0x11);
        RegWrite8(0x16A,0xA0);
        //RegWrite8(0x210, 0x3F);		
        //RegWrite8(0x212, 0xC0);
        //RegWrite8(0x248, 0xF0);
        RegWrite8(0x255, 0x18);
        RegWrite8(REG_DEMOD_ACR_PN_CFG2, 0x37); //0x192
        break;
    
    case MOD_NTSC_M:
    //case MOD_NTSC_M_60M_44M:
    //case MOD_NTSC_M_54M_44M:
    //case MOD_NTSC_M_54M_36M:
        //PRA
        ucData = 0x61;
        ATD_SetReg(psDemodCtx, 0x94A, &ucData, 1);    	
        ucData = 0x19;
        ATD_SetReg(psDemodCtx, 0x942, &ucData, 1);    	
        ucData = 0x42;
        ATD_SetReg(psDemodCtx, 0x943, &ucData, 1);    	
        ucData = 0x8D;
        ATD_SetReg(psDemodCtx, 0x94C, &ucData, 1);    	
        ucData = 0x7F;
        ATD_SetReg(psDemodCtx, 0x954, &ucData, 1);    	
        ucData = 0x1E;
        ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);    	
        ucData = 0x20;
        ATD_SetReg(psDemodCtx, 0x96C, &ucData, 1);    	
        ucData = 0x0B;
        ATD_SetReg(psDemodCtx, 0x95F, &ucData, 1);
        
        //CR
        ucData = 0x36;                
        ATD_SetReg(psDemodCtx, 0x998, &ucData, 1); 
        ucData = 0x8C;
        ATD_SetReg(psDemodCtx, 0x999, &ucData, 1);    	
        ucData = 0x29;
        ATD_SetReg(psDemodCtx, 0x99A, &ucData, 1);    	
        ucData = 0x47;
        ATD_SetReg(psDemodCtx, 0x9A8, &ucData, 1);    	
        ucData = 0x46;
        ATD_SetReg(psDemodCtx, 0x9A9, &ucData, 1); 
        ucData = 0xF8;
        ATD_SetReg(psDemodCtx, 0x9AA, &ucData, 1);    	
        ucData = 0x36;
        ATD_SetReg(psDemodCtx, 0x9B2, &ucData, 1);    	
        ucData = 0x46;
        ATD_SetReg(psDemodCtx, 0x9B3, &ucData, 1);    	
        ucData = 0x7C;
        ATD_SetReg(psDemodCtx, 0x9B5, &ucData, 1); 
        ucData = 0x04;
        ATD_SetReg(psDemodCtx, 0x9B6, &ucData, 1);    	
        ucData = 0x2C;
        ATD_SetReg(psDemodCtx, 0x9B7, &ucData, 1);    	
        ucData = 0x30;
        ATD_SetReg(psDemodCtx, 0x9BB, &ucData, 1);    	
        ucData = 0x94;
        ATD_SetReg(psDemodCtx, 0x9BF, &ucData, 1);    	
        ucData = 0x0C;                
        ATD_SetReg(psDemodCtx, 0x90D, &ucData, 1);    	
        //DRO
        ucData = 0x7F;
        ATD_SetReg(psDemodCtx, 0x974, &ucData, 1);    	
        ucData = 0xA4; //default 0xD4, GA FT issue 0xA4
        ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); 
#ifdef CC_DALI2K10 
        ucData = 0x19;
#else
        ucData = 0x08; //default 0x18, GA FT issue 0x08
#endif
        ATD_SetReg(psDemodCtx, 0x965, &ucData, 1); 
        ucData = 0xD2;
        ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);    	
        ucData = 0x0F;
        ATD_SetReg(psDemodCtx, 0x97E, &ucData, 1);    	
        ucData = 0x87;
        ATD_SetReg(psDemodCtx, 0x968, &ucData, 1);    	
//        ucData = 0x2C;
//        ATD_SetReg(psDemodCtx, 0x966, &ucData, 1); 
        ucData = 0x68;
        ATD_SetReg(psDemodCtx, 0x97C, &ucData, 1);    	
        ucData = 0xA5;
        ATD_SetReg(psDemodCtx, 0xA34, &ucData, 1);    	
        ucData = 0x12;
        ATD_SetReg(psDemodCtx, 0xA35, &ucData, 1);    	
        ucData = 0x11;
        ATD_SetReg(psDemodCtx, 0x906, &ucData, 1);    	
        ucData = 0xA9;
        ATD_SetReg(psDemodCtx, 0xA40, &ucData, 1);    	
        ucData = 0x17;
        ATD_SetReg(psDemodCtx, 0xA14, &ucData, 1);    	
        ucData = 0x36;
        ATD_SetReg(psDemodCtx, 0xA55, &ucData, 1);    	
  //      ucData = 0x3A;
  //      ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);
  //      ucData = 0xF0;
  //      ATD_SetReg(psDemodCtx, 0x96A, &ucData, 1);
        
        RegWrite8(REG_DEMOD_DRO_REF_COEF, 0xD2); //0x17A
        RegWrite8(0x10D, 0x0C);	
                
    	break;            

    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }

    RegWrite8(REG_DEMOD_ADCIF_CFG, 0x01);  //0x120 
    RegWrite8(REG_DEMOD_DRO_MASK_CFG,0x37);  //0x17D
    RegWrite8(0x24C,0x03);
    RegWrite8(REG_DEMOD_DRO_WADC_5,0x12);    //0x173
    //VOP
    RegWrite8(REG_DEMOD_VOP_CFG,0x35);     //0x1c8

    RegWrite8(0x13E,0x03);  

    //RegWrite8(0x245,0x9F);  //0xDF for higher CVBS swing
    RegWrite8(0x249,0x04);
    RegWrite8(0x24D,0x80);
    RegWrite8(0x254,0x0C);
    RegWrite8(0x210, 0x3F);		
    RegWrite8(0x212, 0xC0);
#ifdef CC_TVE_POWERDOWN_HALF
    RegWrite8(0x208,0x40);
    RegWrite8(0x248,0xBC);
#endif
	// CR
    RegWrite8(REG_DEMOD_ACR_PN_CFG0,0x0C); //0x190
    RegWrite8(REG_DEMOD_ACR_PN_CFG3,0x19); //0x193  //For PN
    RegWrite8(REG_DEMOD_ACR_SCNF,0x5F);    //0x195
    RegWrite8(REG_DEMOD_ACR_QCNF,0xDF);    //0x196
    //RegWrite8(REG_DEMOD_ACR_CFG,0xF9);     //0x197  // CR auto config ON    
    RegWrite8(REG_DEMOD_ACR_CFG,0x39);     //0x197  // CR auto config  OFF
    RegWrite8(REG_DEMOD_PE_CFG0,0x27);     //0x1B4
#ifdef CC_MT5363 //For MT5363    
    RegWrite8(REG_DEMOD_ADVPS_CFG0,0x1C);  //0x1B9
#else //For MT5388
    RegWrite8(REG_DEMOD_ADVPS_CFG0,0x3C);  //0x1B9
#endif
    RegWrite8(REG_DEMOD_VOP_PREROT, 0x66); //0x1CF, nyquist filter on
    //VOP
    RegWrite8(0x1CA,0x10); 
    // level switch HW patch parameter 
    RegWrite8(REG_DEMOD_PRA_LP1_CFG02,0xA4); //0x141
    //To fix CR DTV00138738
    if((u1SubSysIdx==MOD_SECAM_L)||(u1SubSysIdx==MOD_SECAM_L1))
        RegWrite8(REG_DEMOD_PRA_LP1_CFG02,0x42); //0x141
        
    RegWrite8(REG_DEMOD_PRA_TRA_CTL1, 0x83); //0x160
    RegWrite8(REG_DEMOD_PRA_TRA_CTL3, 0x1A); //0x162

}

void ATD_SetIF(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{
    UINT8 ucData;

    if (psDemodCtx->pTCtx->u2IF_Freq == 38000)
    {
        switch(u1SubSysIdx)
        {
        case MOD_PAL_BG:
        case MOD_PAL_DK:
        case MOD_PAL_I:
        case MOD_SECAM_L:
        case MOD_NTSC_M:
        //case MOD_NTSC_M_54M_36M:
            ATD_GetReg(psDemodCtx, 0x930, &ucData, 1);
            ucData = (ucData & 0x0F) | 0x40;
            ATD_SetReg(psDemodCtx, 0x930, &ucData, 1);
            break;
        default:
            break;
        }
    }
}

VOID ATD_SetAgc(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    ATD_AGC_CTX* psAtdAgcPara = &(psDemodCtx->pTCtx->sAtdAgcPara);
    UINT8 u1Ifmin=0;
    //RF_AGC external +5V
     #ifdef CC_DALI2K10 //Philips DALI2K10 project.RF_AGC use extern +5v
    	  ucData=0x0E;
       ATD_SetReg(psDemodCtx, 0x924, &ucData, 1);
       //mcSHOW_USER_MSG(("RF_AGC use Externel +5V\n"));
     #endif
    
    
    // AGC
    RegWrite8(REG_DEMOD_PRA_RF_BIAS, psAtdAgcPara->AgcRfBias);	//RF BIAS
    RegWrite8(REG_DEMOD_PRA_IF_BIAS, 0x00);	                    //IF BIAS
    RegWrite8(REG_DEMOD_PRA_RF_SLOPE, 0x80);	                //RF SLOPE
    RegWrite8(REG_DEMOD_PRA_IF_SLOPE, 0x80);	                //IF SLOPE
    RegWrite8(REG_DEMOD_PRA_RF_MAX, 0x7F);                      //RF MAX
    RegWrite8(REG_DEMOD_PRA_IF_MAX, 0x7F);                      //IF MAX
    RegWrite8(REG_DEMOD_PRA_RF_MIN, psAtdAgcPara->AgcRfMin);	//RF MIN

    //    depart  the tuner-dependent part to [ITUNER] layer , Jack Mei 2009/11/3    
    ucData = psDemodCtx->u1SubSysIdx;
   if (ITuner_OP(psDemodCtx->pTCtx, itSetIfMin,ucData, &u1Ifmin) == ITUNER_OK)
   	    {
            RegWrite8(REG_DEMOD_PRA_IF_MIN, u1Ifmin);
        }
   else
   	    {
            RegWrite8(REG_DEMOD_PRA_IF_MIN, psAtdAgcPara->AgcIfMin);	//for other tuner IF MIN
            u1Ifmin=psAtdAgcPara->AgcIfMin;
        }
   
/*
	#ifndef CC_MT5363//For MT5388
    if(psDemodCtx->u1TunerType==cPANASONIC_ENV57S){ //only for MT5388+Panasonic tuner
        mcSHOW_DBG_MSG(("ENV57S TOP Setting\n"));
    	switch(psDemodCtx->u1SubSysIdx){
    	    case MOD_PAL_BG:
    	    case MOD_PAL_I:
    	        RegWrite8(REG_DEMOD_PRA_IF_MIN, 0xE0);
    	        u1Ifmin=0xE0;
    	        break;
    	    case MOD_SECAM_L:
            case MOD_SECAM_L1:
            case MOD_PAL_DK:
                u1Ifmin=0xF0;
                RegWrite8(REG_DEMOD_PRA_IF_MIN, 0xF0);
    		break;
    	    default:
    	        mcSHOW_DBG_MSG(("TOP No change\n"));
    	        break;
        }
    }
    else  //for other tuner of MT5388
    {
        RegWrite8(REG_DEMOD_PRA_IF_MIN, psAtdAgcPara->AgcIfMin);	//IF MIN
        u1Ifmin=psAtdAgcPara->AgcIfMin;
    }
    #else
    if(psDemodCtx->u1TunerType==cNuTune_FT2607){ 
            mcSHOW_DBG_MSG(("NuTune_FT2607 TOP Setting\n"));
            switch(psDemodCtx->u1SubSysIdx){
                case MOD_PAL_BG:
                case MOD_PAL_I:                    
                case MOD_PAL_DK:
                    RegWrite8(REG_DEMOD_PRA_IF_MIN, 0xE3);
                    u1Ifmin=0xE3;
                    break;
                case MOD_SECAM_L:
                case MOD_SECAM_L1:
                    u1Ifmin=0xF3;
                    RegWrite8(REG_DEMOD_PRA_IF_MIN, 0xF3);
                break;
                default:
                    mcSHOW_DBG_MSG(("TOP No change\n"));
                    break;
            }
        }

    else if (psDemodCtx->u1TunerType==cLG_TDTWS810D){ 
                mcSHOW_DBG_MSG(("TDTWS810D TOP Setting\n"));
                switch(psDemodCtx->u1SubSysIdx){
                    case MOD_PAL_BG:
                    case MOD_PAL_I:                    
                    case MOD_PAL_DK:
                        RegWrite8(REG_DEMOD_PRA_IF_MIN, 0xDC);
                        u1Ifmin=0xDC;
                        break;
                    case MOD_SECAM_L:
                    case MOD_SECAM_L1:
                        u1Ifmin=0xE4;
                        RegWrite8(REG_DEMOD_PRA_IF_MIN, 0xE4);
                    break;
                    default:
                        mcSHOW_DBG_MSG(("TOP No change\n"));
                        break;
                }
            }        
    else 
    {    	
        RegWrite8(REG_DEMOD_PRA_IF_MIN, psAtdAgcPara->AgcIfMin);	//IF MIN
        u1Ifmin=psAtdAgcPara->AgcIfMin;
     }
    #endif

*/
    RegWrite8(REG_DEMOD_PRA_DR_SEL, psAtdAgcPara->AgcDrSel);	//PRA Dynamic Range
    //RegWrite8(REG_DEMOD_PRA_LP1_CFG13, 0x4D); //0x94C	    
    RegWrite8(REG_DEMOD_PRA_LP1_CFG12, 0x83); //0x94B, disable PRA auto-target
   //To fix CR DTV00138738
    if((psDemodCtx->u1SubSysIdx==MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx==MOD_SECAM_L1))
        RegWrite8(REG_DEMOD_PRA_LP1_CFG12, 0x43); //0x94B, disable PRA auto-target	
  
    switch (psAtdAgcPara->AgcDrSel) //mapping PRA target based on dynamic range value
    {
        case 0x00: ucData = 0x60; break;
        case 0x01: ucData = 0x70; break;
        case 0x02: ucData = 0x80; break;
        case 0x03: ucData = 0x88; break;
        case 0x04: ucData = 0x90; break;
        case 0x05: ucData = 0x98; break;
        case 0x06: ucData = 0xA0; break;
        case 0x07: ucData = 0xA8; break;
        case 0x08: ucData = 0xB0; break;
        case 0x09: ucData = 0xB8; break;
        case 0x0A: ucData = 0xC0; break;
        case 0x0B: ucData = 0xC8; break;
        case 0x0C: ucData = 0xD0; break;
        case 0x0D: ucData = 0xE0; break;
        case 0x0E: ucData = 0xF0; break;
        case 0x0F: ucData = 0xFF; break;
        default:   ucData = 0x80; break;
    }
    RegWrite8(REG_DEMOD_PRA_LP1_CFG05, ucData); //0x944                         

    psDemodCtx->s1IFmin = u1Ifmin;
    psDemodCtx->s1RFmin = psAtdAgcPara->AgcRfMin;
}

void ATD_SetSystem(ATD_CTX_T *psDemodCtx, UINT8 u1SubSysIdx)
{   
    UINT8 ucData;
    // reset ATD datapath and registers
    ucData = 0x00; ATD_SetReg(psDemodCtx, 0x931, &ucData, 1);               

    psDemodCtx->u1SubSysIdx = u1SubSysIdx;

#ifndef CC_MT8223


    ATD_SetSystemAudio(psDemodCtx, u1SubSysIdx);
    ATD_SetSystemVideo(psDemodCtx, u1SubSysIdx);
    ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
    ATD_SetIF(psDemodCtx, u1SubSysIdx);   
    ATD_SetAgc(psDemodCtx);
 #else

  switch(u1SubSysIdx)
        {
        case MOD_PAL_BG:
      mcSHOW_DBG_MSG(("Set System = PAL BG 54M/39M\n"));
      RegWrite8(0x131, 0x00);
	    RegWrite8(0x100, 0x82);
	    RegWrite8(0x126, 0x09);
	    RegWrite8(0x129, 0x87);
      RegWrite8(0x12A, 0x22);
	    RegWrite8(0x130, 0x33);
	    RegWrite8(0x13B, 0x00);
	    RegWrite8(0x13E, 0x03);
      RegWrite8(0x164, 0x14);
	    RegWrite8(0x165, 0x09);
	    RegWrite8(0x166, 0xB6);
	    RegWrite8(0x167, 0x9D);
      RegWrite8(0x168, 0x87);
	    RegWrite8(0x16A, 0x00);
	    RegWrite8(0x16C, 0x20);
	    RegWrite8(0x173, 0x12);
      RegWrite8(0x174, 0x55);
	    RegWrite8(0x17A, 0xD2);
	    RegWrite8(0x17C, 0x68);
	    RegWrite8(0x17D, 0x37);
      RegWrite8(0x17E, 0x7F);
	    RegWrite8(0x180, 0x08);
	    RegWrite8(0x183, 0x20);
	    RegWrite8(0x18A, 0x20);
      RegWrite8(0x190, 0x0C);
	    RegWrite8(0x192, 0x54);
	    RegWrite8(0x193, 0x19);
	    RegWrite8(0x194, 0x51);
      RegWrite8(0x195, 0x5F);
	    RegWrite8(0x196, 0xDF);
	    RegWrite8(0x197, 0xF9);
	    RegWrite8(0x198, 0x36);
      RegWrite8(0x199, 0x8C);
	    RegWrite8(0x19A, 0x29);
	    RegWrite8(0x1A8, 0x47);
	    RegWrite8(0x1A9, 0x46);
      RegWrite8(0x1AA, 0xF8);
	    RegWrite8(0x1B2, 0x36);
	    RegWrite8(0x1B3, 0x46);
	    RegWrite8(0x1B4, 0x37);//comment from Justin for null region and constant delay
	    RegWrite8(0x1B5, 0x7C);
	    RegWrite8(0x1B6, 0x04);
      RegWrite8(0x1B7, 0x2C);
	    RegWrite8(0x1B9, 0x2C);
	    RegWrite8(0x1BA, 0x04);  // enable CR auto reacq clear ECO
	    RegWrite8(0x1BB, 0x30);
	    RegWrite8(0x1BF, 0x94);
	    RegWrite8(0x1C4, 0x04);//for ECO IC can output AV out
      RegWrite8(0x1C8, 0x35);
	    RegWrite8(0x1C9, 0x17);
	    RegWrite8(0x1CA, 0x10);
	    RegWrite8(0x1CF, 0x66);
      RegWrite8(0x1E1, 0x67);
	    RegWrite8(0x208, 0x58);
	    RegWrite8(0x210, 0x3F);
	    RegWrite8(0x212, 0xC0);
      RegWrite8(0x214, 0x17);
	    RegWrite8(0x220, 0x01);
	    RegWrite8(0x221, 0xB0);
	    RegWrite8(0x234, 0xA5);
      RegWrite8(0x235, 0x12);
	    RegWrite8(0x23D, 0x4F);
	    RegWrite8(0x244, 0x42);
	    RegWrite8(0x245, 0xDF);//orignal value=0xDC, fine tune to meet av out=1Vp-p
	    RegWrite8(0x248, 0xDC);
	    RegWrite8(0x249, 0x1D);
      RegWrite8(0x24C, 0xA3);
	    RegWrite8(0x24D, 0x80);
	    RegWrite8(0x254, 0x0C);
	    RegWrite8(0x255, 0x36);
      RegWrite8(0x25e, 0x84);
	    RegWrite8(0x270, 0x99);
	    RegWrite8(0x274, 0xcd);
	    RegWrite8(0x275, 0x62);
      RegWrite8(0x277, 0x42);
	    RegWrite8(0x280, 0x41);
	    RegWrite8(0x282, 0x10);
	    RegWrite8(0x283, 0x20);
      RegWrite8(0x291, 0x40);
	    RegWrite8(0x293, 0x04);
	    RegWrite8(0x294, 0x48);
	    RegWrite8(0x296, 0xc8);
      RegWrite8(0x297, 0x22);
	    RegWrite8(0x29a, 0x7a);
	    RegWrite8(0x29b, 0x0a);
	    RegWrite8(0x29c, 0x21);
	    RegWrite8(0x29d, 0x1c);    
	    RegWrite8(0x29e, 0x80);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29e, 0xa0);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);    
	    RegWrite8(0x2a4, 0x38);
	    RegWrite8(0x2b0, 0x28);
	    RegWrite8(0x2b2, 0x03);
	    RegWrite8(0x2b3, 0x60);
      RegWrite8(0x2b5, 0x40);
	    RegWrite8(0x2b7, 0x15);
	    RegWrite8(0x2b8, 0x00);
	    RegWrite8(0x2b9, 0x11);
      RegWrite8(0x2c0, 0x7f);
	    RegWrite8(0x2c1, 0x0a);
	    RegWrite8(0x2c2, 0x21);
	    RegWrite8(0x2c3, 0x34);
	    RegWrite8(0x2ca, 0x77);
      RegWrite8(0x2cb, 0x00);//disable SAT
	    RegWrite8(0x2cc, 0x14);
	    RegWrite8(0x2d4, 0x04);
	    RegWrite8(0x2d5, 0xc2);
      RegWrite8(0x2d6, 0xc2);
	    RegWrite8(0x2d7, 0xec);
	    RegWrite8(0x2d8, 0xb3);
	    RegWrite8(0x2d9, 0xb3);
	    RegWrite8(0x2da, 0x7f);
      RegWrite8(0x2db, 0x7a);
	    RegWrite8(0x2dc, 0x80);
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0xd4);//enable mag_ref from DRO
	    RegWrite8(0x2e3, 0x6b);
	#if 0   
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0x54);
	   RegWrite8(0x129, 0x87);
	    RegWrite8(0x12a, 0x22);
	#endif
            break;
			
        case MOD_PAL_DK:
	mcSHOW_DBG_MSG(("Set System = PAL DK 54M/39M\n"));
      RegWrite8(0x131, 0x00);
	    RegWrite8(0x100, 0x82);
	    RegWrite8(0x126, 0x02);
	    RegWrite8(0x129, 0x87);
      RegWrite8(0x12A, 0x22);
	    RegWrite8(0x130, 0x35);
	    RegWrite8(0x13B, 0x00);
	    RegWrite8(0x13E, 0x03);
      RegWrite8(0x164, 0x14);
	    RegWrite8(0x165, 0x09);
	    RegWrite8(0x166, 0xB6);
	    RegWrite8(0x167, 0x9D);
      RegWrite8(0x168, 0x87);
	    RegWrite8(0x16A, 0x00);
	    RegWrite8(0x16C, 0x20);
	    RegWrite8(0x173, 0x12);
      RegWrite8(0x174, 0x55);
	    RegWrite8(0x17A, 0xD2);
	    RegWrite8(0x17C, 0x68);
	    RegWrite8(0x17D, 0x37);
      RegWrite8(0x17E, 0x7F);
	    RegWrite8(0x180, 0x08);
	    RegWrite8(0x183, 0x20);
	    RegWrite8(0x18A, 0x20);
      RegWrite8(0x190, 0x0C);
	    RegWrite8(0x192, 0x54);
	    RegWrite8(0x193, 0x19);
	    RegWrite8(0x194, 0x51);
      RegWrite8(0x195, 0x5F);
	    RegWrite8(0x196, 0xDF);
	    RegWrite8(0x197, 0xF9);
	    RegWrite8(0x198, 0x36);
      RegWrite8(0x199, 0x8C);
	    RegWrite8(0x19A, 0x29);
	    RegWrite8(0x1A8, 0x47);
	    RegWrite8(0x1A9, 0x46);
      RegWrite8(0x1AA, 0xF8);
	    RegWrite8(0x1B2, 0x36);
	    RegWrite8(0x1B3, 0x46);
	    RegWrite8(0x1B4, 0x37);//comment from Justin for null region and constant delay
	    RegWrite8(0x1B5, 0x7C);
	    RegWrite8(0x1B6, 0x04);
      RegWrite8(0x1B7, 0x2C);
	    RegWrite8(0x1B9, 0x2C);
	    RegWrite8(0x1BA, 0x04);  // enable CR auto reacq clear ECO
	    RegWrite8(0x1BB, 0x30);
	    RegWrite8(0x1BF, 0x94);
	    RegWrite8(0x1C4, 0x04);//for ECO IC can output AV out
      RegWrite8(0x1C8, 0x35);
	    RegWrite8(0x1C9, 0x19);
	    RegWrite8(0x1CA, 0x50);
	    RegWrite8(0x1CF, 0x66);
      RegWrite8(0x1E1, 0x60);
	    RegWrite8(0x208, 0x58);
	    RegWrite8(0x210, 0x3F);
	    RegWrite8(0x212, 0xC0);
      RegWrite8(0x214, 0x17);
	    RegWrite8(0x220, 0x00);
	    RegWrite8(0x221, 0x00);
	    RegWrite8(0x234, 0xA5);
      RegWrite8(0x235, 0x12);
	    RegWrite8(0x23D, 0x4F);
	    RegWrite8(0x244, 0x42);
	    RegWrite8(0x245, 0xDF);//orignal value=0xDC, fine tune to meet av out=1Vp-p
	    RegWrite8(0x248, 0xDC);
	    RegWrite8(0x249, 0x1D);
      RegWrite8(0x24C, 0xA3);
	    RegWrite8(0x24D, 0x80);
	    RegWrite8(0x254, 0x0C);
	    RegWrite8(0x255, 0x36);
      RegWrite8(0x25e, 0x84);
	    RegWrite8(0x270, 0x99);
	    RegWrite8(0x274, 0xcd);
	    RegWrite8(0x275, 0x62);
      RegWrite8(0x277, 0x42);
	    RegWrite8(0x280, 0x41);
	    RegWrite8(0x282, 0x10);
	    RegWrite8(0x283, 0x20);
      RegWrite8(0x291, 0x40);
	    RegWrite8(0x293, 0x04);
	    RegWrite8(0x294, 0x48);
	    RegWrite8(0x296, 0xc8);
      RegWrite8(0x297, 0x22);
	    RegWrite8(0x29a, 0x7a);
	    RegWrite8(0x29b, 0x0a);
	    RegWrite8(0x29c, 0x21);
	    RegWrite8(0x29d, 0x1c);          
	    RegWrite8(0x29e, 0x80);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29e, 0xa0);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);                        
	    RegWrite8(0x2a4, 0x38);
	    RegWrite8(0x2b0, 0x28);
	    RegWrite8(0x2b2, 0x03);
	    RegWrite8(0x2b3, 0x60);
      RegWrite8(0x2b5, 0x40);
	    RegWrite8(0x2b7, 0x15);
	    RegWrite8(0x2b8, 0x00);
	    RegWrite8(0x2b9, 0x11);
      RegWrite8(0x2c0, 0x7f);
	    RegWrite8(0x2c1, 0x0a);
	    RegWrite8(0x2c2, 0x21);
	    RegWrite8(0x2c3, 0x34);
	    RegWrite8(0x2ca, 0x77);
      RegWrite8(0x2cb, 0x00);//disable SAT
	    RegWrite8(0x2cc, 0x14);
	    RegWrite8(0x2d4, 0x04);
	    RegWrite8(0x2d5, 0xc2);
      RegWrite8(0x2d6, 0xc2);
	    RegWrite8(0x2d7, 0xec);
	    RegWrite8(0x2d8, 0xb3);
	    RegWrite8(0x2d9, 0xb3);
	    RegWrite8(0x2da, 0x7f);
      RegWrite8(0x2db, 0x7a);
	    RegWrite8(0x2dc, 0x82);
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0xd4);//enable mag_ref from DRO
	    RegWrite8(0x2e3, 0x6b);
	#if 0    
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
	    RegWrite8(0x2e1, 0x54);
	   RegWrite8(0x129, 0x87);
	    RegWrite8(0x12a, 0x22);
	#endif    
		break;
		
        case MOD_PAL_I:
	 mcSHOW_DBG_MSG(("Set System = PAL I 54M/39M\n"));
      RegWrite8(0x131, 0x00);
	    RegWrite8(0x100, 0x82);
	    RegWrite8(0x126, 0x09);
	    RegWrite8(0x129, 0x87);
      RegWrite8(0x12A, 0x22);
	    RegWrite8(0x130, 0x34);
	    RegWrite8(0x13B, 0x00);
	    RegWrite8(0x13E, 0x03);
      RegWrite8(0x164, 0x14);
	    RegWrite8(0x165, 0x09);
	    RegWrite8(0x166, 0xB6);
	    RegWrite8(0x167, 0x9D);
      RegWrite8(0x168, 0x87);
	    RegWrite8(0x16A, 0x00);
	    RegWrite8(0x16C, 0x20);
	    RegWrite8(0x173, 0x12);
      RegWrite8(0x174, 0x55);
	    RegWrite8(0x17A, 0xD2);
	    RegWrite8(0x17C, 0x68);
	    RegWrite8(0x17D, 0x37);
      RegWrite8(0x17E, 0x7F);
	    RegWrite8(0x180, 0x08);
	    RegWrite8(0x183, 0x20);
	    RegWrite8(0x18A, 0x20);
      RegWrite8(0x190, 0x0C);
	    RegWrite8(0x192, 0x54);
	    RegWrite8(0x193, 0x19);
	    RegWrite8(0x194, 0x51);
      RegWrite8(0x195, 0x5F);
	    RegWrite8(0x196, 0xDF);
	    RegWrite8(0x197, 0xF9);
	    RegWrite8(0x198, 0x36);
      RegWrite8(0x199, 0x8C);
	    RegWrite8(0x19A, 0x29);
	    RegWrite8(0x1A8, 0x47);
	    RegWrite8(0x1A9, 0x46);
      RegWrite8(0x1AA, 0xF8);
	    RegWrite8(0x1B2, 0x36);
	    RegWrite8(0x1B3, 0x46);
	    RegWrite8(0x1B4, 0x37);//comment from Justin for null region and constant delay
	    RegWrite8(0x1B5, 0x7C);
	    RegWrite8(0x1B6, 0x04);
      RegWrite8(0x1B7, 0x2C);
	    RegWrite8(0x1B9, 0x2C);
	    RegWrite8(0x1BA, 0x04);  // enable CR auto reacq clear ECO
	    RegWrite8(0x1BB, 0x30);
	    RegWrite8(0x1BF, 0x94);
	    RegWrite8(0x1C4, 0x04);//for ECO IC can output AV out
      RegWrite8(0x1C8, 0x35);
	    RegWrite8(0x1C9, 0x3E);
	    RegWrite8(0x1CA, 0xB0);
	    RegWrite8(0x1CF, 0x66);
      RegWrite8(0x1E1, 0x67);
	    RegWrite8(0x208, 0x58);
	    RegWrite8(0x210, 0x3F);
	    RegWrite8(0x212, 0xC0);
      RegWrite8(0x214, 0x17);
	    RegWrite8(0x220, 0x00);
	    RegWrite8(0x221, 0x00);
	    RegWrite8(0x234, 0xA5);
      RegWrite8(0x235, 0x12);
	    RegWrite8(0x23D, 0x4F);
	    RegWrite8(0x244, 0x42);
	    RegWrite8(0x245, 0xDF);//orignal value=0xDC, fine tune to meet av out=1Vp-p
	    RegWrite8(0x248, 0xDC);
	    RegWrite8(0x249, 0x1D);
      RegWrite8(0x24C, 0xA3);
	    RegWrite8(0x24D, 0x80);
	    RegWrite8(0x254, 0x0C);
	    RegWrite8(0x255, 0x36);
      RegWrite8(0x25e, 0x84);
	    RegWrite8(0x270, 0x99);
	    RegWrite8(0x274, 0xcd);
	    RegWrite8(0x275, 0x62);
      RegWrite8(0x277, 0x42);
	    RegWrite8(0x280, 0x41);
	    RegWrite8(0x282, 0x10);
	    RegWrite8(0x283, 0x20);
      RegWrite8(0x291, 0x40);
	    RegWrite8(0x293, 0x04);
	    RegWrite8(0x294, 0x48);
	    RegWrite8(0x296, 0xc8);
      RegWrite8(0x297, 0x22);
	    RegWrite8(0x29a, 0x7a);
	    RegWrite8(0x29b, 0x0a);
	    RegWrite8(0x29c, 0x21);
	    RegWrite8(0x29d, 0x1c);	    
	    RegWrite8(0x29e, 0x80);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29e, 0xa0);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);                                    
	    RegWrite8(0x2a4, 0x38);
	    RegWrite8(0x2b0, 0x28);
	    RegWrite8(0x2b2, 0x03);
	    RegWrite8(0x2b3, 0x60);
      RegWrite8(0x2b5, 0x40);
	    RegWrite8(0x2b7, 0x15);
	    RegWrite8(0x2b8, 0x00);
	    RegWrite8(0x2b9, 0x11);
      RegWrite8(0x2c0, 0x7f);
	    RegWrite8(0x2c1, 0x0a);
	    RegWrite8(0x2c2, 0x21);
	    RegWrite8(0x2c3, 0x34);
	    RegWrite8(0x2ca, 0x77);
      RegWrite8(0x2cb, 0x00);//disable SAT
	    RegWrite8(0x2cc, 0x14);
	    RegWrite8(0x2d4, 0x04);
	    RegWrite8(0x2d5, 0xc2);
      RegWrite8(0x2d6, 0xc2);
	    RegWrite8(0x2d7, 0xec);
	    RegWrite8(0x2d8, 0xb3);
	    RegWrite8(0x2d9, 0xb3);
	    RegWrite8(0x2da, 0x7f);
      RegWrite8(0x2db, 0x7a);
	    RegWrite8(0x2dc, 0x85);
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0xd4);//enable mag_ref from DRO
	    RegWrite8(0x2e3, 0x6b);	    
#if 0	    
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0x54);
	   RegWrite8(0x129, 0x87);
	    RegWrite8(0x12a, 0x22);
#endif
		break;
			
        case MOD_SECAM_L:
      mcSHOW_DBG_MSG(("Set System = SECAM L	\n"));
      RegWrite8(0x131, 0x00);
	    RegWrite8(0x100, 0x82);
	    RegWrite8(0x126, 0x09);
      RegWrite8(0x129, 0x87);//ASIC IF38MHz
	    RegWrite8(0x12A, 0x22);
	    RegWrite8(0x130, 0x35);
	    RegWrite8(0x13B, 0x01);
	    RegWrite8(0x13E, 0x03);
      RegWrite8(0x164, 0xD4);
	    RegWrite8(0x165, 0x4D);
	    RegWrite8(0x166, 0x8D);
	    RegWrite8(0x167, 0x7F);
      RegWrite8(0x168, 0x87);
	    RegWrite8(0x16A, 0x00);
	    RegWrite8(0x16C, 0x20);
	    RegWrite8(0x173, 0x12);
      RegWrite8(0x174, 0xA5);
	    RegWrite8(0x17A, 0x2D);
	    RegWrite8(0x17C, 0x68);
	    RegWrite8(0x17D, 0x37);
      RegWrite8(0x17E, 0x7F);
	    RegWrite8(0x180, 0xE8);
	    RegWrite8(0x183, 0x13);//update for ECO,POA gain for SECAM 
	    RegWrite8(0x184, 0x80);//update for ECO,POA gain for SECAM 
	    RegWrite8(0x18A, 0x37);
      RegWrite8(0x190, 0x0C);
	    RegWrite8(0x192, 0x37);
	    RegWrite8(0x193, 0x19);
	    RegWrite8(0x195, 0x5F);
      RegWrite8(0x196, 0xDF);
	    RegWrite8(0x197, 0xF9);
	    RegWrite8(0x198, 0x36);
	    RegWrite8(0x199, 0x8C);
      RegWrite8(0x19A, 0x29);
	    RegWrite8(0x1A8, 0x47);
	    RegWrite8(0x1A9, 0x46);
	    RegWrite8(0x1AA, 0xF8);
      RegWrite8(0x1B2, 0x36);
	    RegWrite8(0x1B3, 0x46);
	    RegWrite8(0x1B4, 0x37);//comment from Justin for null region and constant delay
	    RegWrite8(0x1B5, 0x7C);
	    RegWrite8(0x1B6, 0x84);
	    RegWrite8(0x1B7, 0x24);
      RegWrite8(0x1B9, 0x1C);
	    RegWrite8(0x1BA, 0x04);  // enable CR auto reacq clear ECO
	    RegWrite8(0x1BB, 0x30);
	    RegWrite8(0x1BF, 0x94);
	    RegWrite8(0x1C4, 0x04);//for ECO IC can output AV out
	    RegWrite8(0x1C8, 0x35);
      RegWrite8(0x1C9, 0x17);
	    RegWrite8(0x1CA, 0xD0);
	    RegWrite8(0x1CF, 0x66);
	    RegWrite8(0x1E1, 0x67);
      RegWrite8(0x208, 0x58);
	    RegWrite8(0x210, 0x3F);
	    RegWrite8(0x212, 0xC0);
	    RegWrite8(0x214, 0x14);
      RegWrite8(0x220, 0x00);
	    RegWrite8(0x221, 0x00);
	    RegWrite8(0x234, 0xA6);
	    RegWrite8(0x235, 0x2D);
      RegWrite8(0x23D, 0x14);
	    RegWrite8(0x244, 0x37);
	    RegWrite8(0x245, 0xDF);
	    RegWrite8(0x248, 0xDC);//update for ECO,DRO i2c_target_blank
	    RegWrite8(0x249, 0x1D);
	    RegWrite8(0x24C, 0xA3);
      RegWrite8(0x24D, 0x80);
	    RegWrite8(0x254, 0x0C);
	    RegWrite8(0x255, 0x18);
	    RegWrite8(0x25e, 0x84);
      RegWrite8(0x270, 0x99);
      RegWrite8(0x274, 0xcf);//DRO
      RegWrite8(0x275, 0xa2);
      RegWrite8(0x276, 0x56);	    
      RegWrite8(0x277, 0x42);
	    RegWrite8(0x280, 0x41);
	    RegWrite8(0x282, 0x10);
	    RegWrite8(0x283, 0x20);
      RegWrite8(0x290, 0xc8);
      RegWrite8(0x291, 0x45);				
	    RegWrite8(0x293, 0x42);				
      RegWrite8(0x294, 0x02);				
      RegWrite8(0x296, 0xc8);
	    RegWrite8(0x297, 0x22);
	    RegWrite8(0x29a, 0x7a);
	    RegWrite8(0x29b, 0x0a);
	    RegWrite8(0x29c, 0x11);  
      RegWrite8(0x29d, 0x0a);
	    RegWrite8(0x29e, 0x80);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29e, 0xa0);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);                                                                      
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x2a4, 0x01);
      RegWrite8(0x2a5, 0x40);	    
	    RegWrite8(0x2b0, 0x28);
	    RegWrite8(0x2b1, 0x10);//update for ECO,64->16
      RegWrite8(0x2b2, 0x87);//update for ECO,5->40 line
	    RegWrite8(0x2b3, 0x00);//update for ECO,5->40 line
	    RegWrite8(0x2b5, 0x45);//SLD_PRA
      RegWrite8(0x2b6, 0x6D);
	    RegWrite8(0x2b7, 0x01);//update for ECO,target H (MSB)
      RegWrite8(0x2b8, 0xC4);//update for ECO,target LSB
	    RegWrite8(0x2b9, 0x01);      
	    RegWrite8(0x2bd, 0x2d);
	    RegWrite8(0x2be, 0x23);
	    RegWrite8(0x2bf, 0x1e);
      RegWrite8(0x2c0, 0x7a);
	    RegWrite8(0x2c1, 0x0a);
	    RegWrite8(0x2c2, 0x3f);//update for ECO,DROP thd, 0x11->0x3F for level switch 72<->40
	    RegWrite8(0x2c3, 0x0a);
      RegWrite8(0x2c9, 0x12);
	    RegWrite8(0x2ca, 0x77);
	    RegWrite8(0x2cb, 0x3f);
	    RegWrite8(0x2cc, 0x3f);
	    RegWrite8(0x2d4, 0x04);
      RegWrite8(0x2d5, 0xe0);
	    RegWrite8(0x2d6, 0xe0);
	    RegWrite8(0x2d7, 0xec);
	    RegWrite8(0x2d8, 0xd0);
      RegWrite8(0x2d9, 0xd0);
	    RegWrite8(0x2da, 0x7f);
      RegWrite8(0x2db, 0x7a);
	    RegWrite8(0x2dc, 0x80);			
      RegWrite8(0x2dd, 0xe0);			
	    RegWrite8(0x2de, 0xe0);			
      RegWrite8(0x2e1, 0x34);			
	    RegWrite8(0x2e3, 0x20);//update for ECO,no v-sync			
      RegWrite8(0x2fb, 0x02);			
		break;
		
        case MOD_SECAM_L1:
	mcSHOW_DBG_MSG(("Set System = SECAM L'	\n"));
      RegWrite8(0x131, 0x00);
	    RegWrite8(0x100, 0x82);
	    RegWrite8(0x126, 0x09);
	    RegWrite8(0x129, 0x9F);
      //RegWrite8(0x12A, 0xDC);
      RegWrite8(0x12A, 0x85);//modify secam l' IF freq
	    RegWrite8(0x130, 0x95);
	    RegWrite8(0x13B, 0x01);
	    RegWrite8(0x13E, 0x03);
      RegWrite8(0x164, 0xD4);
	    RegWrite8(0x165, 0x4D);
	    RegWrite8(0x166, 0x8a);//update for ECO,88->8a LSB 5 bit is target. (0x90/0x88 cause white/yellow color distortion)
	    RegWrite8(0x167, 0xD9);
      RegWrite8(0x168, 0x87);
	    RegWrite8(0x16A, 0x00);
	    RegWrite8(0x16C, 0x20);
	    RegWrite8(0x173, 0x12);
      RegWrite8(0x174, 0xA5);
	    RegWrite8(0x17A, 0x2D);
	    RegWrite8(0x17C, 0x68);
	    RegWrite8(0x17D, 0x37);
      RegWrite8(0x17E, 0x7F);
	    RegWrite8(0x180, 0xE8);
	    RegWrite8(0x183, 0x13);//update for ECO,POA gain for SECAM 
	    RegWrite8(0x184, 0x80);//update for ECO,POA gain for SECAM 
	    RegWrite8(0x18A, 0x37);
      RegWrite8(0x190, 0x0C);
	    RegWrite8(0x192, 0x37);
	    RegWrite8(0x193, 0x19);
	    RegWrite8(0x195, 0x5F);
      RegWrite8(0x196, 0xDF);
	    RegWrite8(0x197, 0xF9);
	    RegWrite8(0x198, 0x36);
	    RegWrite8(0x199, 0x8C);
      RegWrite8(0x19A, 0x29);
	    RegWrite8(0x1A8, 0x47);
	    RegWrite8(0x1A9, 0x46);
	    RegWrite8(0x1AA, 0xF8);
      RegWrite8(0x1B2, 0x36);
	    RegWrite8(0x1B3, 0x46);
	    RegWrite8(0x1B4, 0x37);//comment from Justin for null region and constant delay
	    RegWrite8(0x1B5, 0x7C);
	    RegWrite8(0x1B6, 0x84);
	    RegWrite8(0x1B7, 0x24);
      RegWrite8(0x1B9, 0x1C);
	    RegWrite8(0x1BA, 0x04);  // enable CR auto reacq clear ECO
	    RegWrite8(0x1BB, 0x30);
	    RegWrite8(0x1BF, 0x94);
	    RegWrite8(0x1C4, 0x04);//for ECO IC can output AV out
	    RegWrite8(0x1C8, 0x35);
      RegWrite8(0x1C9, 0x17);
	    RegWrite8(0x1CA, 0xD0);
	    RegWrite8(0x1CF, 0x66);
	    RegWrite8(0x1E1, 0x67);
      RegWrite8(0x208, 0x58);
	    RegWrite8(0x210, 0x3F);
	    RegWrite8(0x212, 0xC0);
	    RegWrite8(0x214, 0x14);
      RegWrite8(0x220, 0x00);
	    RegWrite8(0x221, 0x00);
	    RegWrite8(0x234, 0xA6);
	    RegWrite8(0x235, 0x2D);
	     RegWrite8(0x237, 0x00);//update for ECO
	    RegWrite8(0x238, 0x00);
      RegWrite8(0x23D, 0x14);
	    RegWrite8(0x244, 0x37);
	    RegWrite8(0x245, 0xDF);
	    RegWrite8(0x248, 0xDC);//update for ECO,DRO i2c_target_blank
	    RegWrite8(0x249, 0x1D);
	    RegWrite8(0x24C, 0xA3);
      RegWrite8(0x24D, 0x80);
	    RegWrite8(0x254, 0x0C);
	    RegWrite8(0x255, 0x18);
	    RegWrite8(0x25e, 0x84);
      RegWrite8(0x270, 0x99);
	    RegWrite8(0x274, 0x2f);
	    RegWrite8(0x275, 0xa2);
	    RegWrite8(0x276, 0x00);
      RegWrite8(0x277, 0x42);
	    RegWrite8(0x280, 0x41);
	    RegWrite8(0x282, 0x10);
	    RegWrite8(0x283, 0x20);
      RegWrite8(0x290, 0xc8);
	    RegWrite8(0x291, 0x41);
	    RegWrite8(0x293, 0x42);
	    RegWrite8(0x294, 0x08);
      RegWrite8(0x296, 0xc8);
	    RegWrite8(0x297, 0x22);
	    RegWrite8(0x29a, 0x7a);
	    RegWrite8(0x29b, 0x0a);
	    RegWrite8(0x29c, 0x21);	    	    	    	    	    
	    RegWrite8(0x29d, 0x1c);
	    RegWrite8(0x29e, 0x80);
      RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
      RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
      RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
      RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
      RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29f, 0xe4);
	    RegWrite8(0x29e, 0xa0);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
      RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
      RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
      RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
      RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x29f, 0x1e);
      RegWrite8(0x29f, 0x1e);                                   
	    RegWrite8(0x29f, 0x1e);
	    RegWrite8(0x2a4, 0x08);
	    RegWrite8(0x2b0, 0x28);
	    RegWrite8(0x2b1, 0x10);//update for ECO,64->16
      RegWrite8(0x2b2, 0x87);//update for ECO,5->40 line
	    RegWrite8(0x2b3, 0x00);//update for ECO,5->40 line
	    RegWrite8(0x2b5, 0x41);
	    RegWrite8(0x2b7, 0x01);//update for ECO,target H (MSB)
      RegWrite8(0x2b8, 0xC4);//update for ECO,target LSB
	    RegWrite8(0x2b9, 0x05);
	    RegWrite8(0x2bd, 0x2d);
	    RegWrite8(0x2be, 0x23);
	    RegWrite8(0x2bf, 0x1e);
      RegWrite8(0x2c0, 0x7a);
	    RegWrite8(0x2c1, 0x0a);
	    RegWrite8(0x2c2, 0x3f);//update for ECO,DROP thd, 0x11->0x3F for level switch 72<->40
	    RegWrite8(0x2c3, 0x34);
      RegWrite8(0x2c9, 0x12);
	    RegWrite8(0x2ca, 0x77);
	    RegWrite8(0x2cb, 0x3f);
	    RegWrite8(0x2cc, 0x3f);
	    RegWrite8(0x2d4, 0x04);
      RegWrite8(0x2d5, 0xe0);
	    RegWrite8(0x2d6, 0xe0);
	    RegWrite8(0x2d7, 0xec);
	    RegWrite8(0x2d8, 0xd0);
      RegWrite8(0x2d9, 0xd0);
	    RegWrite8(0x2da, 0x7f);
      RegWrite8(0x2db, 0x7a);
	    RegWrite8(0x2dc, 0x80);			
      RegWrite8(0x2dd, 0xe0);			
	    RegWrite8(0x2de, 0xe0);			
      RegWrite8(0x2e1, 0x34);			
	    RegWrite8(0x2e3, 0x20);//update for ECO,no v-sync				
      RegWrite8(0x2fb, 0x02);			
	    RegWrite8(0x290, 0xc8);	//SLD_DAGC		
      RegWrite8(0x291, 0x45);				
	    RegWrite8(0x293, 0x42);				
      RegWrite8(0x294, 0x02);				
	    RegWrite8(0x296, 0xc8);
      RegWrite8(0x297, 0x22);	
	    RegWrite8(0x29a, 0x7a);  
      RegWrite8(0x29b, 0x0a);	
	    RegWrite8(0x29c, 0x11);  
      RegWrite8(0x29d, 0x0a);
	    RegWrite8(0x2a4, 0x01);
      RegWrite8(0x2a5, 0x40);
	    RegWrite8(0x2b5, 0x45);//SLD_PRA 
      RegWrite8(0x2b6, 0x6D);
	   // RegWrite8(0x2b7, 0x02);
     // RegWrite8(0x2b8, 0x04);
	    RegWrite8(0x2b9, 0x01);
	   // RegWrite8(0x2c2, 0x11);
	    RegWrite8(0x2c3, 0x0a);
      RegWrite8(0x274, 0xcf);//DRO 
      RegWrite8(0x276, 0x56);
	    RegWrite8(0x174, 0xa5);
      RegWrite8(0x275, 0xa2);
		break;
		
        case MOD_NTSC_M:
	mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/44M\n"));
      RegWrite8(0x131, 0x00);
	    RegWrite8(0x100, 0x82);
	    RegWrite8(0x126, 0x10);
	    RegWrite8(0x129, 0x87);
      RegWrite8(0x12A, 0x22);
	    RegWrite8(0x130, 0x32);
	    RegWrite8(0x13B, 0x00);
	    RegWrite8(0x13E, 0x03);
      RegWrite8(0x164, 0x04);
	    RegWrite8(0x165, 0x09);
	    RegWrite8(0x166, 0xB6);
	    RegWrite8(0x167, 0x9D);
      RegWrite8(0x168, 0x87);
	    RegWrite8(0x16A, 0x00);
	    RegWrite8(0x16C, 0x20);
	    RegWrite8(0x173, 0x12);
      RegWrite8(0x174, 0x55);
	    RegWrite8(0x17A, 0xD2);
	    RegWrite8(0x17C, 0x68);
	    RegWrite8(0x17D, 0x37);
      RegWrite8(0x17E, 0x7F);
	    RegWrite8(0x180, 0x08);
	    RegWrite8(0x183, 0x20);
	    RegWrite8(0x18A, 0x20);
      RegWrite8(0x190, 0x0C);
	    RegWrite8(0x192, 0x54);
	    RegWrite8(0x193, 0x19);
	    RegWrite8(0x194, 0x51);
      RegWrite8(0x195, 0x5F);
	    RegWrite8(0x196, 0xDF);
	    RegWrite8(0x197, 0xF9);
	    RegWrite8(0x198, 0x36);
      RegWrite8(0x199, 0x8C);
	    RegWrite8(0x19A, 0x29);
	    RegWrite8(0x1A8, 0x47);
	    RegWrite8(0x1A9, 0x46);
      RegWrite8(0x1AA, 0xF8);
	    RegWrite8(0x1B2, 0x36);
	    RegWrite8(0x1B3, 0x46);
	    RegWrite8(0x1B4, 0x37);//comment from Justin for null region and constant delay
	    RegWrite8(0x1B5, 0x7C);
	    RegWrite8(0x1B6, 0x04);
      RegWrite8(0x1B7, 0x2C);
	    RegWrite8(0x1B9, 0x2C);
	    RegWrite8(0x1BA, 0x04);  // enable CR auto reacq clear ECO
	    RegWrite8(0x1BB, 0x30);
	    RegWrite8(0x1BF, 0x94);
	    RegWrite8(0x1C4, 0x04);//for ECO IC can output AV out
      RegWrite8(0x1C8, 0x35);
	    RegWrite8(0x1C9, 0xCB);
	    RegWrite8(0x1CA, 0xD0);
	    RegWrite8(0x1CF, 0x66);
      RegWrite8(0x1E1, 0x67);
	    RegWrite8(0x208, 0x58);
	    RegWrite8(0x210, 0x3F);
	    RegWrite8(0x212, 0xC0);
      RegWrite8(0x214, 0x17);
	    RegWrite8(0x220, 0x00);
	    RegWrite8(0x221, 0x00);
	    RegWrite8(0x234, 0xA5);
      RegWrite8(0x235, 0x12);
	    RegWrite8(0x23D, 0x42);
	    RegWrite8(0x244, 0x42);
	    RegWrite8(0x245, 0xDF);//orignal value=0xDC, fine tune to meet av out=1Vp-p
	    RegWrite8(0x248, 0xDC);
	    RegWrite8(0x249, 0x1D);
      RegWrite8(0x24C, 0xA3);
	    RegWrite8(0x24D, 0x80);
	    RegWrite8(0x254, 0x0C);
	    RegWrite8(0x255, 0x36);
      RegWrite8(0x25e, 0x84);
	    RegWrite8(0x270, 0x80);
	    RegWrite8(0x274, 0xcd);
	    RegWrite8(0x275, 0x62);
      RegWrite8(0x277, 0x37);
	    RegWrite8(0x280, 0x41);
	    RegWrite8(0x282, 0x10);
	    RegWrite8(0x283, 0x20);
      RegWrite8(0x291, 0x40);
	    RegWrite8(0x293, 0x04);
	    RegWrite8(0x294, 0x48);
	    RegWrite8(0x296, 0xc8);
      RegWrite8(0x297, 0x22);
	    RegWrite8(0x29a, 0x7a);
	    RegWrite8(0x29b, 0x0a);
	    RegWrite8(0x29c, 0x21);
	    RegWrite8(0x29d, 0x1c);	    	    	    	       
	    RegWrite8(0x29e, 0x80);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
      RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29f, 0xef);
	    RegWrite8(0x29e, 0xa0);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
	    RegWrite8(0x29f, 0x12);
      RegWrite8(0x29f, 0x12);                                  
	    RegWrite8(0x2a4, 0x38);
	    RegWrite8(0x2b0, 0x28);
	    RegWrite8(0x2b2, 0x03);
	    RegWrite8(0x2b3, 0x60);
      RegWrite8(0x2b5, 0x40);
	    RegWrite8(0x2b7, 0x15);
	    RegWrite8(0x2b8, 0x00);
	    RegWrite8(0x2b9, 0x11);
      RegWrite8(0x2c0, 0x7f);
	    RegWrite8(0x2c1, 0x0a);
	    RegWrite8(0x2c2, 0x21);
	    RegWrite8(0x2c3, 0x34);
	    RegWrite8(0x2ca, 0x77);
      RegWrite8(0x2cb, 0x00);//disable SAT
	    RegWrite8(0x2cc, 0x14);
	    RegWrite8(0x2d4, 0x04);
	    RegWrite8(0x2d5, 0xc2);
      RegWrite8(0x2d6, 0xc2);
	    RegWrite8(0x2d7, 0xec);
	    RegWrite8(0x2d8, 0xb3);
	    RegWrite8(0x2d9, 0xb3);
	    RegWrite8(0x2da, 0x7f);
      RegWrite8(0x2db, 0x7a);
	    RegWrite8(0x2dc, 0x88);
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0xd4);//enable mag_ref from DRO
	    RegWrite8(0x2e3, 0x6b);
	    
#if 0	    
	    RegWrite8(0x2dd, 0x80);
	    RegWrite8(0x2de, 0x80);
      RegWrite8(0x2e1, 0x54);
	    RegWrite8(0x129, 0x87);
	    RegWrite8(0x12a, 0x22);
#endif
		break;				
			
        default:
            break;
        }

    ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
    //ATD_SetAgc(psDemodCtx);
    ATD_SetNormalMode(psDemodCtx);
 #endif

 
}


UINT8 ATD_StartAcq(ATD_CTX_T *psDemodCtx, UINT8 fgScanMode)
{

#ifndef CC_MT8223


#ifdef CC_MT5363 // do not reset demod in MT5363 to avoid video flash when channel scan
    UINT8 ii, ucData;//, ucData2;
                     
    if(fgScanMode)
    {
    	ATD_SetScanMode(psDemodCtx);
    }
    ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);

    
    ucData = 0x00; //EC_code_d9928: Enable MT5363 reset demod to replace ATD_CR_Auto-reacq
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);
	
//add for CR DTV00138738 (the root cause is as the same with Eboda field SECAM issue)
//For some SECAM L/L' pattern RF/IF AGC is not stable
    ATD_GetReg(psDemodCtx, 0x959, &ucData, 1);
    ucData = ucData & 0xE7;
    ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);
    
    ucData = 0x80;    
    ATD_SetReg(psDemodCtx, 0x94D, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x94F, &ucData, 1);
    ucData = 0x00;    
    ATD_SetReg(psDemodCtx, 0x94E, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x950, &ucData, 1);
    
    mcDELAY_MS(10);

    ATD_GetReg(psDemodCtx, 0x959, &ucData, 1);
    ucData = ucData | 0x18;
    ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);
//above is added CR DTV00138738 

    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);
    
    //ZY, 100126, for SECAL L scan loss issue
    if(fgScanMode)     	
    {   
    	if(psDemodCtx->u1SubSysIdx==MOD_SECAM_L || psDemodCtx->u1SubSysIdx==MOD_SECAM_L1)
    	{
    	  ATD_GetReg(psDemodCtx, 0x959, &ucData, 1);
          ucData = ucData & 0xFE;   
          ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);  //0x959[0]=0, fast speed in both ACQ/TRA
        }
    }
    
    mcDELAY_MS(cATD_CR_AUTO_REACQ_TIME);
    
#else     
    UINT8 ii, ucData;//, ucData2;

    if(fgScanMode)
    {
    	ATD_SetScanMode(psDemodCtx);
    }
    ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);
	
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);

    //Release PRA,....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    //pucChipReg[REG_DEMOD_CTL_RST1] = 0xFF;

    //PRA LOOP
    //CheckTmp = 0;
    for (ii = 0; ii < 20; ii ++)
    {
        mcDELAY_MS(10);
        ATD_GetReg(psDemodCtx, 0x95E, &ucData, 1);
        if ((ucData & 0x40) == 0x40) {
            //mcSHOW_DBG_MSG(("PRA lock!, cnt = %d\n", ii));
            break;
        }
    }

    if(ii==20)
    {
        Printf("PRA acquisition fail!\n");
        return 1;
    }

    //pucChipReg[REG_uP_GP_REG03] = ii;
    //Release CRP....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);
    //pucChipReg[REG_DEMOD_CTL_RST2] = 0xFF;

#endif

    //CPR LOOP
    for (ii = 0; ii < 20; ii ++)
    {
        ATD_GetReg(psDemodCtx, 0x9A0, &ucData, 1);
        //ATD_GetReg(psDemodCtx, 0x1A1, &ucData2, 1);
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
            break;
        }
        mcDELAY_MS(10);
    }

    if(ii==20)
    {
        Printf("CR acquisition fail!\n");
        return 1;
    }

    return 0; 
#else

UINT8 ii, ucData;//, ucData2;

if(fgScanMode)
{
	ATD_SetScanMode(psDemodCtx);
}
ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);


ucData = 0x00;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);

    //Release PRA,....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);


    //PRA LOOP
    //CheckTmp = 0;
    for (ii = 0; ii < 20; ii ++)
    {
        mcDELAY_MS(10);
        ATD_GetReg(psDemodCtx, 0xAE4, &ucData, 1);//for 8223 2E4[6:5]=10 means pra tracking state
        if ((ucData & 0x60) == 0x40) {
            //mcSHOW_DBG_MSG(("PRA lock!, cnt = %d\n", ii));
            break;
        }
    }
	
    if(ii==20)
    {
        Printf("PRA acquisition fail!\n");
        return 1;
    }

    //Release CRP....
    ucData = 0xFF;
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);

    //CPR LOOP
    for (ii = 0; ii < 20; ii ++)
    {
        mcDELAY_MS(10);
        ATD_GetReg(psDemodCtx, 0x9A0, &ucData, 1);
        if ((ucData & 0x03) == 0x03)
        {
            mcSHOW_DBG_MSG(("ATD acquisition OK!\n"));
        
            Weak_Sinal_Indicator=0;  // initialize driver patch variables
            Strong_Chroma_Indicator=0;
            weak_count=0;
          //  CCI_Indicator=0;//8223 no CCI patch
            //CCITest=0;                  	                   
            NoOVMCounter0=5;
            PNCounter=5;	       	           	    
            break;
        }
    }

    if(ii==20)
    {
        Printf("CR acquisition fail!\n");
        return 1;
    }

    return 0; 
	
#endif

}

// Set CR pilot filter
// u1PF= 0x00 ==> Disable pilot filter
// u1PF= 0x01 ==> กำ1.05 MHz.
// u1PF= 0x10 ==> กำ2.11 MHz.
// u1PF= 0x11 ==> กำ6.25 MHz.
void ATD_SetPF(ATD_CTX_T *psDemodCtx, UINT8 u1PF)
{

    UINT8 ucData;

    psDemodCtx->u1PF = u1PF;
    ATD_GetReg(psDemodCtx, ATD_REG_DEMOD_PS_CFG, &ucData, 1);

    ucData = (ucData & 0xFC) | u1PF;

    ATD_SetReg(psDemodCtx, ATD_REG_DEMOD_PS_CFG, &ucData, 1);

}

//INT8 ATD_SetFreq(UINT32 u4Freq);

BOOL ATD_SetFreq(ATD_CTX_T *psDemodCtx, UINT32 u4FreqInKHz, UINT8 fgAutoSearch)
{
    BOOL fgReturn;
    PARAM_SETFREQ_T ATD_param;    

    mcSHOW_DBG_MSG(("Before SetFreq\n"));

    psDemodCtx->u4Frequency = u4FreqInKHz;
    psDemodCtx->fgAutoSearch = fgAutoSearch;
    
    ATD_param.Freq = u4FreqInKHz;
    ATD_param.Modulation = psDemodCtx->u1SubSysIdx;
    ATD_param.fgAutoSearch = fgAutoSearch;

    DigTunerBypassI2C (TRUE); //EC_code_d9826 for eIntATD_ExtDTD
    fgReturn = ITuner_SetFreq(psDemodCtx->pTCtx, &ATD_param);  
    DigTunerBypassI2C (FALSE);

    mcSHOW_DBG_MSG(("After SetFreq\n"));
    
    if (fgReturn)
        fgReturn = FALSE;
    else
        fgReturn = TRUE;

    return fgReturn;

}

void ATD_SetScanMode(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;

    switch (psDemodCtx->u1SubSysIdx)
    {
    case MOD_SECAM_L:  // to speed up PRA 
    case MOD_SECAM_L1:
        mcSHOW_DBG_MSG(("Set scan mode parameter for SECAM L/L'\n"));
        
        //ZY, 100126, SECAM L scan loss issue. PRA speed-up at channel scan
        ucData = 0x84; ATD_SetReg(psDemodCtx, 0x941, &ucData, 1); //0x42->0x84, fast loop speed
        ATD_GetReg(psDemodCtx, 0x946, &ucData, 1);
        ucData = ucData & 0xEF;   //0x946[4]=0, middle-term
        ATD_SetReg(psDemodCtx, 0x946, &ucData, 1); 
        
        
	#if  0
        ucData = 0x19;  // 0x142 = 0x19
        ATD_SetReg(psDemodCtx, 0x942, &ucData, 1);
        ATD_GetReg(psDemodCtx, 0x94B, &ucData, 1);
        ucData &= 0xF8;  // 0x14B[2:0] = 1
        ucData |= 0x01;  
        ATD_SetReg(psDemodCtx, 0x94B, &ucData, 1);
        ucData = 0x8D;  // 0x14C = 0x8D
        ATD_SetReg(psDemodCtx, 0x94C, &ucData, 1);
        ucData = 0x1E;  // 0x159 = 0x1E
        ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);
       #endif
       //ZY 100126, remove small DRO gain setting
       //liuqu,20100108,for secaml L scan issue. 
       //ucData = 0x04;
	   // ATD_SetReg(psDemodCtx, 0x966, &ucData, 1);
	   break;

    case MOD_PAL_BG:  // to enlarge audio filter in scan mode for TV spec detection
    case MOD_PAL_DK:
    case MOD_PAL_I:
        ucData = 0x8C;
        ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0x20;
        ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
        ucData = 0x81;
        ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0xD0;
        ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);
        break;

    default:
        break;
    }
}

void ATD_SetDelayLine(ATD_CTX_T *psDemodCtx,UINT8 IsSetDelay)
{
UINT8 ucData;

    if(IsSetDelay)//set delay a line for scramble signal 
    {
    	switch(psDemodCtx->u1SubSysIdx)
    	{
	case MOD_PAL_BG:
	case MOD_NTSC_M:
		mcSHOW_DBG_MSG(("Set PAL BG/M delay line\n"));
              //set AVS and delay line enable
       	ucData = 0x90; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1); 
		break;
	case MOD_PAL_DK:
	case MOD_PAL_I:
		mcSHOW_DBG_MSG(("Set PAL BG/I delay line\n"));
              //set AVS and delay line enable
       	ucData = 0x10; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1); 
		break;		
    	}
	RegWrite8(0x1c5, 0x60);	
    }
    else
    {
    	switch(psDemodCtx->u1SubSysIdx)
    	{
	case MOD_PAL_BG:
	case MOD_NTSC_M:
		mcSHOW_DBG_MSG(("PAL BG/M No delay line\n"));
              //set AVS and delay line disable
       	ucData = 0x94; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1); 
		break;
	case MOD_PAL_DK:
	case MOD_PAL_I:
		mcSHOW_DBG_MSG(("PAL BG/I No delay line\n"));
              //set AVS and delay line disable
       	ucData = 0x14; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1); 
		break;	
    	}

	RegWrite8(0x1c5, 0x00);   
        }
	
}

void ATD_SetNormalMode(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;

#ifndef CC_MT8223

 
    switch (psDemodCtx->u1SubSysIdx)
    {
    case MOD_SECAM_L:  // to speed up PRA 
    case MOD_SECAM_L1:
        mcSHOW_DBG_MSG(("Set scan mode parameter for SECAM L/L'\n"));
	#if  0
        ucData = 0x3E;  // 0x142 = 0x3E
        ATD_SetReg(psDemodCtx, 0x942, &ucData, 1);
        ATD_GetReg(psDemodCtx, 0x94B, &ucData, 1);
        ucData &= 0xF8;  // 0x14B[2:0] = 3
        ucData |= 0x03;  
        ATD_SetReg(psDemodCtx, 0x94B, &ucData, 1);
        ucData = 0x2D;  // 0x14C = 0x2D
        ATD_SetReg(psDemodCtx, 0x94C, &ucData, 1);
        ucData = 0x1F;  // 0x159 = 0x1F
        ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);
	#endif
	
        break;

    case MOD_PAL_BG:  // set SIF filter of BG back to 5.7MHz   
        ucData = 0x94;
        ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0x20;
        ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
        ucData = 0x81;
        ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0xB0;
        ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);
        break;
    case MOD_PAL_DK:  // set SIF filetr of DKI back to defualt setting
    case MOD_PAL_I:
        ucData = 0x00;
        ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);        
        ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);     
        ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);	
    default:
        break;
    }
#else
   switch (psDemodCtx->u1SubSysIdx)
    {
    case MOD_PAL_BG: 
        mcSHOW_DBG_MSG(("Set System = PAL BG 54M/39M\n"));
        // set AVS/AUC audio filter,center:5.65M and BW(3dB)=0.9M
        ucData = 0x94; ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0xE8; ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
	    ucData = 0x94; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0xAD; ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);        
        break;
    
    case MOD_PAL_DK: 
        mcSHOW_DBG_MSG(("Set System = PAL DK 54M/39M\n"));
        // set AVS/AUC audio filter,center:6.22M and BW(3dB)=1.256M
        ucData = 0x8A; ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0x20; ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
        ucData = 0x14; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0xD8; ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);
        break;
    
    case MOD_PAL_I: 
        mcSHOW_DBG_MSG(("Set System = PAL I 54M/39M\n"));
        // set AVS/AUC audio filter,center:6.31M and BW(3dB)=1.256M
        ucData = 0x88; ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0x60; ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
        ucData = 0x14; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0xDF; ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);

        break;
    
    case MOD_SECAM_L: 
        mcSHOW_DBG_MSG(("Set System = SECAM L	\n"));             
        break;
    
    case MOD_SECAM_L1: 
        mcSHOW_DBG_MSG(("Set System = SECAM L'	\n"));             
        break;  
    case MOD_NTSC_M:
    mcSHOW_DBG_MSG(("Set System = NTSC M/N 54M/44M\n"));
        // set AVS/AUC audio filter,center:4.5M and BW(3dB)=0.9M
        ucData = 0xAA; ATD_SetReg(psDemodCtx, 0x9C2, &ucData, 1);
        ucData = 0xE8; ATD_SetReg(psDemodCtx, 0x9C3, &ucData, 1);
        ucData = 0x94; ATD_SetReg(psDemodCtx, 0x9C4, &ucData, 1);
        ucData = 0x81; ATD_SetReg(psDemodCtx, 0xA20, &ucData, 1);
        ucData = 0x55; ATD_SetReg(psDemodCtx, 0xA21, &ucData, 1);
        break;         
    default:
        mcSHOW_DBG_MSG(("Set System = DEFAULT!!\n"));
        break;
    }
//AVS register can not be set real time and need to reset POA
    ATD_GetReg(psDemodCtx, 0x935, &ucData, 1);
    ucData &= 0xFB;
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);
    ucData |= 0x04;
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);


    ATD_SetDelayLine(psDemodCtx,FALSE);

#endif
    
}

/**********************************************************************/
INT32 ATD_DemodReset(ATD_CTX_T *psDemodCtx)  // need further implementation
{
#if 0

#endif

	return (DRVAPI_TUNER_OK);
}

void ATD_DisConnect(ATD_CTX_T *psDemodCtx)
{
#ifdef CC_MT5363
    UINT8 ucData;
    UINT32 u4Temp;

	// AD cannot be power down for uP 
	// power down AD clock
    //u4Temp = IO_READ32(0x2000d000, 0x1d0);
    //u4Temp |= 0x000000e0;
    //IO_WRITE32(0x2000d000, 0x1d0, u4Temp);
    
    RegWrite8(REG_DEMOD_CTL_TEST, 0xFF); //0x932, all clk power down

    // for demod60m_ck = TVDPLL/9 (power down ADCLK) //EC_code_d9A09 for power down ADC setting for eIntATD_ExtDTD
    u4Temp = IO_READ32(CKGEN_BASE, 0x218);
    u4Temp |=  ((U32)0x01); // 2000d218[1:0]=2'b01
    IO_WRITE32(CKGEN_BASE, 0x218, u4Temp);
    // for demod54m_ck = TVDPLL/10 (power down ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, 0x21C);
    u4Temp |=  ((U32)0x01); // 2000d21c[1:0]=2'b01
    IO_WRITE32(CKGEN_BASE, 0x21C, u4Temp);

    // power down AD clock
    u4Temp = IO_READ32(CKGEN_BASE, 0x1d0);
    u4Temp |= 0x000000e0;
    IO_WRITE32(CKGEN_BASE, 0x1d0, u4Temp);

    
    //Set MT5363 RFAGC and IFAGC to high-Z   (pin mux: [29:28] = 11, GPIO: [9:8] = 00) //EC_code_d9826 for eIntATD_ExtDTD
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
    u4Temp |= 0x30000000;
    mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
    
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x514);
    u4Temp &= 0xFFFFFCFF;
    mcSET_SHOW_REG(CKGEN_BASE, 0x514, u4Temp);

#else
 #if 0
    // for demod60m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, 0x218);
    u4Temp &= ~((U32)0x03); // 2000d218[1:0]=2'b01
    u4Temp |=  ((U32)0x01); 
    IO_WRITE32(CKGEN_BASE, 0x218, u4Temp);


    // for demod54m_ck = 54Mz (ADCLK)
    u4Temp = IO_READ32(CKGEN_BASE, 0x21C);
    u4Temp &= ~((U32)0x03); // 2000d21c[1:0]=2'b01
    u4Temp |=  ((U32)0x03); 
    IO_WRITE32(CKGEN_BASE, 0x21C, u4Temp);

    //disbale ATD
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(0x924)));

     UINT8 ucData;
    UINT32 uData;

    // Enable DVBT and Disable ATD, LC, write 0x1b4 to 0x02
    //mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b0, 0x00000002);
    //mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b4, 0x4001006d);
    /*DVBT_GetReg(psDemodCtx, MT5391_REG_ANA_PD_MODE, &ucData, 1);
    ucData &= 0xEF;   // b'4 = 0
    DVBT_SetReg(psDemodCtx, MT5391_REG_ANA_PD_MODE, &ucData, 1); */

   
    //disbale ATD
    ucData = 0x00;
    ATD_SetReg(psDemodCtx, 0x934, &ucData, 1);
    ATD_SetReg(psDemodCtx, 0x935, &ucData, 1);
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(0x924)));
    ATD_GetReg(psDemodCtx, 0x924, &ucData, 1);
    ucData &= 0xFD;
    ATD_SetReg(psDemodCtx, 0x924, &ucData, 1);
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module 0x%X\n",GetATDReg(0x924)));
    mcSHOW_DBG_MSG(("[Atuner] Disable ATD Module\n"));


    //Close ADC Circuit
    uData = IO_READ32(CKGEN_BASE, 0x1d0);
    uData |= 0x80000001;
    IO_WRITE32(CKGEN_BASE, 0x1d0, uData);

    uData = IO_READ32(CKGEN_BASE, 0x1d4);
    uData |= 0xE0000000;
    IO_WRITE32(CKGEN_BASE, 0x1d4, uData);  
#endif   
#endif   
    
}




STATIC BOOL TVD_CCI_Indicator()
{		
    mcSHOW_DBG_MSG2(("TVD CCI_Indicator 0x%X\n",(IO_READ16(VIDEO_IN0_BASE, 0x60) >> 1) & 0x3FF));

    if(((IO_READ16(VIDEO_IN0_BASE, 0x60) >> 1) & 0x3FF) > 0x0020) // CCI Present
    {
    	return TRUE;
    }
    else
    {
    	return FALSE;
    }
}

STATIC UINT16 ATD_CheckToneMag(ATD_CTX_T *psDemodCtx,UINT8 CCIidx)
{
    UINT8 ucValue;
    UINT16 ToneMag = 0;
    //Load Tone Detection Script
    //1. Set Tone Detection BW3
    ATD_GetReg(psDemodCtx, 0xA4D, &ucValue, 1);
    ucValue &= 0xF0;
    ucValue |= 0x0D;
    ATD_SetReg(psDemodCtx, 0xA4D, &ucValue, 1);
    //2. Load Tone Script
    ATD_SetReg(psDemodCtx, 0xA1C, &(CCI_Tone_BW3[CCIidx][0]), 1);
    ATD_SetReg(psDemodCtx, 0xA1D, &(CCI_Tone_BW3[CCIidx][1]), 1);
    ATD_SetReg(psDemodCtx, 0xA1E, &(CCI_Tone_BW3[CCIidx][2]), 1);
    //3. Get Tone Amp
    //mcDELAY_MS(100);
    mcDELAY_MS(10);
    ATD_GetReg(psDemodCtx, 0xA15, &ucValue, 1);
    ToneMag += ucValue << 8;
    ATD_GetReg(psDemodCtx, 0xA16, &ucValue, 1);
    ToneMag += ucValue;
    mcSHOW_DBG_MSG2(("TVD CCI Tone Mag 0x%X, %d\n",ToneMag, CCIidx));
    return (UINT16)ToneMag;
}

STATIC void ATD_LoadCCIScript(ATD_CTX_T *psDemodCtx,UINT8 CCIidx)
{
    UINT8 ucValue;
    /*//Load Tone Detection Script
    //1. Set Tone Rejection BW3
    ATD_GetReg(psDemodCtx, 0xA4D, &ucValue, 1);
    ucValue &= 0x0F;
    ucValue |= 0xE0; //0xF0 for BW3 , 0xE0 for BW2
    ATD_SetReg(psDemodCtx, 0xA4D, &ucValue, 1);
    //2. Load Tone Script
    ATD_SetReg(psDemodCtx, 0xA19, &(CCI_Tone_BW2[CCIidx][0]), 1);
    ATD_SetReg(psDemodCtx, 0xA1A, &(CCI_Tone_BW2[CCIidx][1]), 1);
    ATD_SetReg(psDemodCtx, 0xA1B, &(CCI_Tone_BW2[CCIidx][2]), 1);*/

    // for Q-lab CCI case, disable CVBS scaling, fine-tune CR loop BW    
    //ATD_SetReg(psDemodCtx, 0x00a, &ucValue, 1);
    //ATD_SetReg(psDemodCtx, 0x00f, &ucValue, 1);
    ATD_GetReg(psDemodCtx, 0xA45, &ucValue, 1);
    ucValue = ucValue & 0x7F;
    ATD_SetReg(psDemodCtx, 0xA45, &ucValue, 1);
    ucValue = 0x2C;
    ATD_SetReg(psDemodCtx, 0x966, &ucValue, 1);
    ATD_GetReg(psDemodCtx, 0x96A, &ucValue, 1);
    ucValue = ucValue & 0x7F;
    ATD_SetReg(psDemodCtx, 0x96A, &ucValue, 1);
    //ucValue = 0xC9;
    ucValue = 0x09; //EC_code_d9B06: Correct Disable CR auto-reacq
    ATD_SetReg(psDemodCtx, 0x997, &ucValue, 1);
    ucValue = 0x37;
    ATD_SetReg(psDemodCtx, 0x998, &ucValue, 1);
    ucValue = 0x47;
    ATD_SetReg(psDemodCtx, 0x9A9, &ucValue, 1);
    ucValue = 0x37;
    ATD_SetReg(psDemodCtx, 0x9B2, &ucValue, 1);
    ucValue = 0x47;
    ATD_SetReg(psDemodCtx, 0x9B3, &ucValue, 1);
    ucValue = 0x37;
    ATD_SetReg(psDemodCtx, 0x9B4, &ucValue, 1);
}

STATIC void ATD_Patch_CCI(ATD_CTX_T *psDemodCtx)
{    	
    UINT8 ii;
    UINT8 ucValue;

    if(TVD_CCI_Indicator()) // CCI Present
    {
    	mcSHOW_DBG_MSG(("TVD Enter CCI\n"));
        //for(ii=1;ii<2;ii++)
        for(ii=1;ii<4;ii++)
        {
            if( ATD_CheckToneMag(psDemodCtx,ii) > 0x0200)
            {
                CCI_Indicator = 6;
                mcSHOW_DBG_MSG(("Load CCI %d\n", ii));
                ATD_LoadCCIScript(psDemodCtx, ii);
                //CCITest = ii;
        	break;
            }      	
        }
        if( ii == 4)
     	{
            mcSHOW_DBG_MSG(("Can't Findout any tone\n"));
     	}	                 	
    }       
    else
    {	            	
    	if(CCI_Indicator > 1)
        {
            CCI_Indicator--;
        }
        else if(CCI_Indicator == 1)		
        {	
        //if(CheckToneMag(psDemodCtx,CCITest) < 0x0180)
        //{	
            mcSHOW_DBG_MSG(("TVD Exit CCI\n"));
        //Diable Tone Rejection
        //1. Set Tone Rejection BW3
      /*ATD_GetReg(psDemodCtx, 0x24D, &ucValue, 1);
    ucValue &= 0x0F;						
    ATD_SetReg(psDemodCtx, 0x24D, &ucValue, 1);*/
      
            ATD_GetReg(psDemodCtx, 0xA45, &ucValue, 1);
            ucValue = ucValue | 0x80;
            ATD_SetReg(psDemodCtx, 0xA45, &ucValue, 1);
            ucValue = 0x38;
            ATD_SetReg(psDemodCtx, 0x966, &ucValue, 1);
            ATD_GetReg(psDemodCtx, 0x96A, &ucValue, 1);
            ucValue = ucValue | 0x80;
            ATD_SetReg(psDemodCtx, 0x96A, &ucValue, 1);
            CCI_Indicator = 0;
            //CCITest = 0;
     //}		
        }	
    }	
}


STATIC void ATD_Patch_Sensitivity(ATD_CTX_T *psDemodCtx)
{	
    //UINT8 ucData;
    UINT8 ucTVDstate;
            
    UINT8 ucValue, ucThreshold1, ucThreshold2; 
    
    if((psDemodCtx->u1SubSysIdx == MOD_SECAM_L) || (psDemodCtx->u1SubSysIdx == MOD_SECAM_L1))
    {
    	ucThreshold1 = 0x60;
    	ucThreshold2 = 0x40;
    }
    else
    {
    	ucThreshold1 = 0x60;
    	ucThreshold2 = 0x50;
    }
    ATD_GetReg(psDemodCtx, 0xA42, &ucValue, 1);
    mcSHOW_DBG_MSG2(("ATD noise level = 0x%02x\n", ucValue));    
    
    ucTVDstate = IO_READ8(VIDEO_IN0_BASE, 0x9D);    
    //GetSignalPower(psDemodCtx);
    mcSHOW_DBG_MSG2(("TVD color catch = 0x%02x\n", (ucTVDstate & 0x01)));
    mcSHOW_DBG_MSG2(("weak signal indicator = %d\n", Weak_Sinal_Indicator));
    mcSHOW_DBG_MSG2(("weak signal counter = %d\n", weak_count));
    
    if( (ucValue > ucThreshold1) && (weak_count < 5) )
    {
        {
    	    weak_count=weak_count+1;
        }
        if((weak_count == 5) && (Weak_Sinal_Indicator == 0) && ((ucTVDstate & 0x01)==0x00) ) 
        {
    	    //ucData = 0x16;
    	    //ATD_SetReg(psDemodCtx, 0x1b4, &ucData, 1);
    	    ATD_SetEQ(psDemodCtx, itGetEqScriptWeak);
    	    Weak_Sinal_Indicator = 1;
    	    mcSHOW_DBG_MSG(("Enter weak signal mode!\n"));
        }
    }
    else
    {    	
    	if( (ucValue < ucThreshold2) && (weak_count > 0) )    	
    	{
    	    weak_count=weak_count-1;
    	}
    	
    	if((weak_count == 0) && (Weak_Sinal_Indicator == 1)) 
    	{
    	    //ucData = 0x36;
    	    //ATD_SetReg(psDemodCtx, 0x1b4, &ucData, 1);
    	    ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
    	    Weak_Sinal_Indicator = 0;
    	    mcSHOW_DBG_MSG(("Escape weak signal mode!\n"));
        }
    }    
}

STATIC void ATD_Patch_CR(ATD_CTX_T *psDemodCtx)
{
	
    UINT8 ucData, ucCRstate0, ucCRstate1, ucNoiselevel, ucCmetrix;
    
    // new PN patch, when noise level is large (signal<60dBuV), turn off PN auto-config, wide CR BW
    // NTSC or PAL only
    // LC 080505
    // Modify for all mode
    // LC 080515
    
    ATD_GetReg(psDemodCtx, 0x9A0, &ucData, 1);    
    ucCRstate0 = ucData;
    
    ATD_GetReg(psDemodCtx, 0xA42, &ucData, 1);    
    ucNoiselevel = ucData;
         
    if(u1CRPatchLoopCnt<5)
        u1CRPatchLoopCnt++;
         
    if(u1CRPatchLoopCnt == 5)
    {
        //ucData = 0x19;            
        //ATD_SetReg(psDemodCtx, 0x993, &ucData, 1);    
        ucData = 0x1C;            
        ATD_SetReg(psDemodCtx, 0x9B9, &ucData, 1);    
    }	
         
    if( ((ucCRstate0 & 0x03) == 0x03))// && (u1CRPatchLoopCnt==5) )  //CR in tracking state
    {
        if(ucNoiselevel > 0x19)  //signal < 25dBuV
        {		                
            ATD_GetReg(psDemodCtx, 0x997, &ucData, 1);             
            //ucData = ucData & 0xCF;            
            ucData = ucData & 0x0F; //EC_code_d9B06: Correct Disable CR auto-reacq
            ATD_SetReg(psDemodCtx, 0x997, &ucData, 1);  //turn-off PN & FN auto-config            
            NoOVMCounter0 = 5;
	    //if ((pucChipReg[REG_DEMOD_CR_STATE1] & 0x0C) == 0x00)  // no PN/FN
	    
            ATD_GetReg(psDemodCtx, 0x99C, &ucData, 1);    
            ucCmetrix = ucData;
	    
	    mcSHOW_DBG_MSG2(("CMetrix = 0x%02X\n", ucCmetrix));
	    
    	    if (ucCmetrix < 0x20)  // CMetrix <= 0x2000, weak PN
    	    {
    	        if (PNCounter < 5)
    	        {
    	            PNCounter = PNCounter + 1;
    	        }
    	        if (PNCounter == 5 )
    	        {
    	            ucData = 0x37;  ATD_SetReg(psDemodCtx, 0x998, &ucData, 1);  //narrow BW
    	            ucData = 0x47;  ATD_SetReg(psDemodCtx, 0x9A9, &ucData, 1);
    	            ucData = 0x37;  ATD_SetReg(psDemodCtx, 0x9B2, &ucData, 1);
    	            ucData = 0x47;  ATD_SetReg(psDemodCtx, 0x9B3, &ucData, 1);
    	            ucData = 0x17;  ATD_SetReg(psDemodCtx, 0x9B4, &ucData, 1);  //lower null-region for CR weak signal mode
    	            ATD_GetReg(psDemodCtx, 0x9B5, &ucData, 1);             
                    if(ucData != 0x7C)            
                    {
                    	ucData = 0x7C;            
                        ATD_SetReg(psDemodCtx, 0x9B5, &ucData, 1);  //FIFO delay            	    		    		 
                    }    
    	             ATD_GetReg(psDemodCtx, 0x9B6, &ucData, 1); //GA FT issue    
    	             if((ucData & 0x04) != 0x04)  
    	             {
                        ucData = ucData | 0x04; 
    	                 ATD_SetReg(psDemodCtx, 0x9B6, &ucData, 1);
    	             } 
                    mcSHOW_DBG_MSG2(("CR enter weak signal mode!\n"));
                }
    	    }
    	    else
    	    {
    	        PNCounter = 0;
                ucData = 0x35;  ATD_SetReg(psDemodCtx, 0x998, &ucData, 1);  //a little wider BW
                ucData = 0x45;  ATD_SetReg(psDemodCtx, 0x9A9, &ucData, 1);
                ucData = 0x35;  ATD_SetReg(psDemodCtx, 0x9B2, &ucData, 1);
                ucData = 0x45;  ATD_SetReg(psDemodCtx, 0x9B3, &ucData, 1);
                ucData = 0x27;  ATD_SetReg(psDemodCtx, 0x9B4, &ucData, 1);  //higher null-region for CR weak signal+PN mode
                ATD_GetReg(psDemodCtx, 0x9B5, &ucData, 1);             
                if(ucData != 0x4D)            
                {
                    ucData = 0x4D;	            
                    ATD_SetReg(psDemodCtx, 0x9B5, &ucData, 1);  //FIFO delay     
                }    
                ATD_GetReg(psDemodCtx, 0x9B6, &ucData, 1); //GA FT issue    
    	        if((ucData & 0x04) == 0x04)  
    	        {
                    ucData = ucData & 0x80; 
    	            ATD_SetReg(psDemodCtx, 0x9B6, &ucData, 1);
    	        }     
                mcSHOW_DBG_MSG2(("CR enter weak signal+PN mode!\n"));                
            }				
    	}
    	else if(ucNoiselevel < 0x15)  // 25dBuV<signal
        {           
            ATD_GetReg(psDemodCtx, 0x997, &ucData, 1);             
            ucData = ucData | 0x30;            
            ATD_SetReg(psDemodCtx, 0x997, &ucData, 1);  //turn-on PN auto-config                
                       
            PNCounter = 5;
    	    
    	    ucData = 0x27;  ATD_SetReg(psDemodCtx, 0x9B4, &ucData, 1);  //higher null-region for CR strong signal mode
    	    
            ATD_GetReg(psDemodCtx, 0x9B6, &ucData, 1); //GA FT issue    
    	    if((ucData & 0x04) != 0x04)  
            {
                ucData = ucData | 0x04; 
    	        ATD_SetReg(psDemodCtx, 0x9B6, &ucData, 1);
    	    } 

    	    ATD_GetReg(psDemodCtx, 0x9A1, &ucData, 1);    
            ucCRstate1 = ucData;
    	    
    	    mcSHOW_DBG_MSG2(("PN/FN = 0x%02X\n", (ucCRstate1&0x0C)>>2 ));
    	    
            if ((ucCRstate1 & 0x0C) == 0x00)  // no PN/FN		
            {
                ATD_GetReg(psDemodCtx, 0x9B5, &ucData, 1);             
                if(ucData != 0x7C)
                {            
                    ucData = 0x7C;
                    ATD_SetReg(psDemodCtx, 0x9B5, &ucData, 1);  //FIFO delay       
    	        }    	
                
                if((ucCRstate0 & 0x70) >= 0x20)  // OVM_IDX >= 2
    	        {
                    ucData = 0x37;  ATD_SetReg(psDemodCtx, 0x998, &ucData, 1);  //narrow BW
                    ucData = 0x47;  ATD_SetReg(psDemodCtx, 0x9A9, &ucData, 1);
                    ucData = 0x37;  ATD_SetReg(psDemodCtx, 0x9B2, &ucData, 1);
                    ucData = 0x47;  ATD_SetReg(psDemodCtx, 0x9B3, &ucData, 1);                                            
                    NoOVMCounter0 = 0;
                    mcSHOW_DBG_MSG2(("CR enter OVM mode!\n"));
                }
                else
                {
                    if (NoOVMCounter0 < 5)
                        NoOVMCounter0 = NoOVMCounter0 + 1;
                    if (NoOVMCounter0 == 5 )
                    {
                        ucData = 0x36;  ATD_SetReg(psDemodCtx, 0x998, &ucData, 1);  //wide BW
                        ucData = 0x46;  ATD_SetReg(psDemodCtx, 0x9A9, &ucData, 1);
                        ucData = 0x36;  ATD_SetReg(psDemodCtx, 0x9B2, &ucData, 1);
                        ucData = 0x46;  ATD_SetReg(psDemodCtx, 0x9B3, &ucData, 1);  
                        mcSHOW_DBG_MSG2(("CR in normal mode!\n"));			
          		    }
                }
            }
            else  // PN/FN exists
            {
                if((ucCRstate1 & 0x0C) == 0x04)  //only FN exists
                {
                ATD_GetReg(psDemodCtx, 0x9B5, &ucData, 1);             
                if(ucData != 0x0C)
                {            
                    ucData = 0x0C;
                    ATD_SetReg(psDemodCtx, 0x9B5, &ucData, 1);  //FIFO delay       
    	        }    	
    	        }    	
                
                ucData = 0x36;  ATD_SetReg(psDemodCtx, 0x998, &ucData, 1);  //wide BW
                ucData = 0x46;  ATD_SetReg(psDemodCtx, 0x9A9, &ucData, 1);
                ucData = 0x36;  ATD_SetReg(psDemodCtx, 0x9B2, &ucData, 1);
                ucData = 0x46;  ATD_SetReg(psDemodCtx, 0x9B3, &ucData, 1);                                  
                NoOVMCounter0 = 5;
                mcSHOW_DBG_MSG2(("CR in PN/FN mode!\n"));
    	    }            
    	}
    }
}

STATIC void ATD_Patch_StrongChroma(ATD_CTX_T *psDemodCtx)
{
    //only patch PAL-BG
    if(psDemodCtx->u1SubSysIdx != MOD_PAL_BG)
        return;
        
    UINT32 u4Temp;
    UINT16 u2ChromaGain;

    u4Temp = mcRISC_Reg(cRISC_TVD_BASE, 0x084);
    u2ChromaGain =(UINT16)((u4Temp & 0xFFF00000) >> 20);
    mcSHOW_DBG_MSG2(("TVD chroma gain = 0x%04X\n", u2ChromaGain));
    
    if((u2ChromaGain <= 0x0030) && (Strong_Chroma_Indicator == 0))
    {
        ATD_SetEQ(psDemodCtx, itGetEqScriptStrongChroma);
        Strong_Chroma_Indicator = 1;
        mcSHOW_DBG_MSG(("Enter strong chroma mode!\n"));
    }
    else if((u2ChromaGain >= 0x00A0) && (Strong_Chroma_Indicator == 1)) //0x7A>0xA0 for India FT issue
    {
        ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
        Strong_Chroma_Indicator = 0;
        mcSHOW_DBG_MSG(("Leave strong chroma mode!\n"));
    }    
    
    if(Strong_Chroma_Indicator)
    {
        mcSHOW_DBG_MSG2(("In strong chroma mode!\n"));
    }
    else
    {
    	mcSHOW_DBG_MSG2(("In normal chroma mode!\n"));
    }	
        
}

void ATD_Patch_AGC(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    S8    s1RFgain, s1IFgain;
    
    ATD_GetReg(psDemodCtx, 0x94D, &ucData, 1);
    s1RFgain = (S8)ucData;
    ATD_GetReg(psDemodCtx, 0x94F, &ucData, 1);
    s1IFgain = (S8)ucData;

    if (SmallAGC) { // if now in SmallAGC mode
        if (s1IFgain > psDemodCtx->s1IFmin) {
            ucData = 0x00;
            ATD_SetReg(psDemodCtx, 0x952, &ucData, 1);

            ucData = psDemodCtx->s1IFmin;
            ATD_SetReg(psDemodCtx, 0x958, &ucData, 1);        


            ATD_GetReg(psDemodCtx, 0x959, &ucData, 1);
            ucData = ucData | 0x08;
            ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);

            SmallAGC = 0;
            mcSHOW_DBG_MSG(("Leave small AGC mode!\n"));
        }
    }
    else {// if now NOT in SmallAGC mode
        if ((s1RFgain <= psDemodCtx->s1RFmin) && (s1IFgain <= psDemodCtx->s1IFmin)) {
            ATD_GetReg(psDemodCtx, 0x959, &ucData, 1);
            ucData = ucData & 0xF7;
            ATD_SetReg(psDemodCtx, 0x959, &ucData, 1);

            ucData = 0xAB;
            ATD_SetReg(psDemodCtx, 0x958, &ucData, 1);

            ucData = 0x26;
            ATD_SetReg(psDemodCtx, 0x952, &ucData, 1);

            SmallAGC = 1;
            mcSHOW_DBG_MSG(("Enter small AGC mode!\n"));
        }
    }    
}

STATIC void ATD_Patch_SoundBeat(ATD_CTX_T *psDemodCtx)
{

    INT32 s4AudCFO, s4AudOVM;
    UINT8 ucData1,ucData2,ucData3;
    S16 s2Data1;    

    s4AudCFO = AUD_MTS_CFO_IDX();    
    s4AudOVM = AudCmdAtvHdevDetQuery();
   
    mcSHOW_DBG_MSG2(("Audio CFO = %d, audio OVM = %d\n", s4AudCFO, s4AudOVM));
    
    if((s4AudCFO == 999) || (s4AudOVM == 999))
    {
        mcSHOW_DBG_MSG2(("Audio DSP status error!!\n"));
        return;
    }    
    else if((s4AudCFO > 20) || (s4AudCFO<-20))
    {
        mcSHOW_DBG_MSG2(("Audio CFO too large\n"));
        return;
    }
    
    if((s4AudOVM == 1) && (s4AudCFO >= -5) && (s4AudCFO <= 5))  //sound OVM case
    {
    switch(psDemodCtx->u1SubSysIdx)
    {
    	case MOD_PAL_BG:
    	        ucData1=0xE1; 
    	        ucData2=0xF8; 
    	        ucData3=0x73;
    	        break;
    	    case MOD_PAL_DK:
    	        ucData1=0xE2; 
    	        ucData2=0x38; 
    	        ucData3=0x5A;
    	        break;
    	    case MOD_PAL_I:
    	        ucData1=0xE7; 
    	        ucData2=0x38; 
    	        ucData3=0xC4;
    	        break;    	    
    	    case MOD_NTSC_M:
    	        ucData1=0xF4; 
    	        ucData2=0xF8; 
    	        ucData3=0x70;
    	        break;
    	    default:
    	        ucData1=0xE1; 
    	        ucData2=0xF8; 
    	        ucData3=0x73;
    	        break;            
        }
        mcSHOW_DBG_MSG2(("Audio OVM mode, fine-tune AVS setting!\n"));
    }    	
    else  //sound no-OVM, need consider sound carrier shift case
    {
        switch(psDemodCtx->u1SubSysIdx)
        {
            case MOD_PAL_BG:
                ucData1=0xE1; 
                ucData2=0xF8; 
                ucData3=0x73;
    	        break;
    	    case MOD_PAL_DK:
    	        ucData1=0xDB; 
    	        ucData2=0x78; 
    	        ucData3=0x25;
    	        break;
    	    case MOD_PAL_I:
                ucData1=0xD8; 
                ucData2=0x78; 
                ucData3=0x70;
                break;
    	    case MOD_NTSC_M:
    	        ucData1=0xF4; 
    	        ucData2=0xF8; 
    	        ucData3=0x70;
    	        break;
    	    default:
                ucData1=0xE1; 
                ucData2=0xF8; 
                ucData3=0x73;
    	        break;            
    }
    	    
    if(s4AudCFO > 0)
    {
            if(psDemodCtx->u1SubSysIdx >= MOD_NTSC_M)
            {
            	if(s4AudCFO >= 3)
            	{
            	    ucData1=0xF7; 
    	            ucData2=0x38; 
    	            ucData3=0x27;
    	        }
    	    }
    	    else
    	    {                
                s2Data1 = (S16)s4AudCFO/3;
                ucData1 = (UINT8)ucData1 + s2Data1;
                ucData2 = (UINT8)ucData2 - 0x40;        
                ucData3 = (UINT8)ucData3 - (s2Data1*4-1)*38/5;
            }
            mcSHOW_DBG_MSG2(("Audio negative shift, fine-tune AVS setting!\n"));
            /*if(fgAudCFO)
        {
            ATD_SetEQ(psDemodCtx);
        }
            fgAudCFO = 0;*/                             
    }
    else if(s4AudCFO < 0)
    {
            if(psDemodCtx->u1SubSysIdx >= MOD_NTSC_M)
            {
            	if(s4AudCFO <= -3)
            	{
            	    ucData1=0xF1; 
    	            ucData2=0x78; 
    	            ucData3=0xD4;    
    	        }            	
    	    }
            else if(psDemodCtx->u1SubSysIdx == MOD_PAL_DK)
            {
                mcSHOW_DBG_MSG2(("D/K not shift AVS for positive audio CFO!\n"));
                //return;  //D/K not tracking for positive CFO
            }    
            else
            {
                s4AudCFO = (INT32)(-1) * s4AudCFO;
                s2Data1 = (S16)s4AudCFO/3;        
                ucData1 = (UINT8)ucData1 - s2Data1;
                if(psDemodCtx->u1SubSysIdx == MOD_PAL_BG)
                {
                    ucData1 = ucData1 + 0x01;
                }    
                ucData2 = (UINT8)ucData2 + 0x40;        
                ucData3 = (UINT8)ucData3 + (s2Data1*4-1)*38/5;
            }
            mcSHOW_DBG_MSG2(("Audio positive shift, fine-tune AVS setting!\n"));
            /*if(!fgAudCFO)
            {
            if( ((psDemodCtx->u1SubSysIdx == 0) || (psDemodCtx->u1SubSysIdx == 2)) && (s4AudCFO >= 3) )
            {            
                mcSHOW_DBG_MSG(("Set new EQ for B/G/I & positive audio CFO!\n"));
                ATD_SetEQ_AudioCFO(psDemodCtx);
                fgAudCFO = 1;
            }
            else
            {
                fgAudCFO = 0;
            }
            }*/                                 
    }
    else
    {
            mcSHOW_DBG_MSG2(("Audio no shift, set default AVS setting!\n"));
            //no operation
            /*if(fgAudCFO)
        {
            ATD_SetEQ(psDemodCtx);
        }
            fgAudCFO = 0;*/            
        }
    }
        
    ATD_SetReg(psDemodCtx, 0x9C0, &ucData1, 1);                
    ATD_SetReg(psDemodCtx, 0x9C1, &ucData2, 1);
    ATD_SetReg(psDemodCtx, 0x9C9, &ucData3, 1);
    
    //mcSHOW_DBG_MSG(("0x9C0 = 0x%02X, 0x9C1 = 0x%02X, 0x9C9 = 0x%02X\n", ucData1, ucData2, ucData3));
      
}


STATIC void ATD_Patch_DRO(ATD_CTX_T *psDemodCtx)
{
    UINT8  ucData, ucData1, ucData2, ucIFgain, ucNoiselevel;
    UINT16 u2GainJitter;//, u2SigSwing, u2SyncHeight;
                
    //get IF gain
    ATD_GetReg(psDemodCtx, 0x94F, &ucData, 1);    
    ucIFgain = ucData;        	
        
    //get DRO gain jitter
    ucData = 0x2E;
    ATD_SetReg(psDemodCtx, 0x96C, &ucData, 1); // DRO probe sel: gain_jitter    
    ATD_GetReg(psDemodCtx, 0x96D, &ucData1, 1); 
    ATD_GetReg(psDemodCtx, 0x96E, &ucData2, 1); 
    u2GainJitter = (UINT16)(ucData1 << 8) | ucData2;        
        
    //get signal swing
    //ATD_GetReg(psDemodCtx, 0xA39, &ucData1, 1); 
    //ATD_GetReg(psDemodCtx, 0xA3A, &ucData2, 1); 
    //u2SigSwing = (UINT16)(ucData1 << 8) | ucData2;        
                                                
    //get sync height                        
    //ATD_GetReg(psDemodCtx, 0xA3B, &ucData1, 1); 
    //ATD_GetReg(psDemodCtx, 0xA3C, &ucData2, 1); 
    //u2SyncHeight = (UINT16)(ucData1 << 8) | ucData2;     
    
    ATD_GetReg(psDemodCtx, 0xA42, &ucData, 1);    
    ucNoiselevel = ucData;
    
    switch(psDemodCtx->u1SubSysIdx)
    {
        case MOD_NTSC_M:
        if ( u2GainJitter > 0x1500 )
        {
            ucData = 0x03;  ATD_SetReg(psDemodCtx, 0x960, &ucData, 1);  //disable level-switch patch
        }  
        else if ( u2GainJitter < 0x1000 )
        {
            ucData = 0x83;  ATD_SetReg(psDemodCtx, 0x960, &ucData, 1);  //enable level-switch patch
        }
                
        if (ucNoiselevel < 0x04 )  // signal > 45dBuV
        {	                    
            if ( u2GainJitter > 0x2500 )  // larger gain_jitter: AM-Fv case
            {
                //default 0xDC, GA FT issue 0xAC
                ucData = 0xAC;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO sync. mode, fast W_ADC                
                ucData = 0xC0;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1); //fast ref_0,ref_1 leaky              
                mcSHOW_DBG_MSG2(("DRO in AM-Fv mode!\n"));
            }
            else if ( u2GainJitter < 0x1400 ) // non AM-Fv case
            {
                //if ( u2SyncHeight < (u2SigSwing >> 3) )       // Low-sync. case
                /*{
                    //defualt 0xD4, GA FT issue
    	            ucData = 0xA4;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO async. mode, slow W_ADC
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);           
                    mcSHOW_DBG_MSG2(("DRO in low-sync mode!\n"));    	                	            
                }
                else if ( u2SyncHeight > (u2SigSwing >> 2) )*/   // non Low-sync. case
                {
                    //default 0xD4, GA FT issue 0xA4
    	            ucData = 0xA4;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO sync. mode
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
    	            mcSHOW_DBG_MSG2(("DRO in normal mode!\n"));       	            
                }	
            }       					
        }
        else if ( ucNoiselevel > 0x07 ) //signal < 39dBuV
        {
            ucData = 0xD6;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO async. mode, slow W_ADC
            ucData = 0xC9;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
            mcSHOW_DBG_MSG2(("DRO in weak signal mode!\n"));               					
        }
        else
        {
        }    
    	break;
    	case MOD_PAL_BG:
    	case MOD_PAL_DK:
    	case MOD_PAL_I:   		
    	if ( u2GainJitter > 0x1500 )
        {
            ucData = 0x03;  ATD_SetReg(psDemodCtx, 0x960, &ucData, 1);  //disable level-switch patch
        }  
        else if ( u2GainJitter < 0x1000 )
        {
            ucData = 0x83;  ATD_SetReg(psDemodCtx, 0x960, &ucData, 1);  //enable level-switch patch
        }
    	
    	if( (ucNoiselevel > 0x0D)) // weak signal priority is higher than CCI
        {
    	    ucData = 0xD6;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO weak signal mode    
            ucData = 0xED;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
    	    ucData = 0xA6;  ATD_SetReg(psDemodCtx, 0xA34, &ucData, 1); 
            ucData = 0x2D;  ATD_SetReg(psDemodCtx, 0xA35, &ucData, 1);
    	    mcSHOW_DBG_MSG2(("DRO in weak signal mode!\n"));      	    				
    	}  
    	else if (ucNoiselevel < 0x04 )  // signal > 45dBuV
        {	                                    				
            if ( u2GainJitter > 0x2500 )  // larger gain_jitter: AM-Fv case
            {
                //default 0xDC, GA FT issue 0xAC 
                ucData = 0xAC;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO sync. mode, fast W_ADC   
                ucData = 0xC0;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1); //fast ref_0,ref_1 leaky
                mcSHOW_DBG_MSG2(("DRO in AM-Fv mode!\n"));                                
            }
            else if ( u2GainJitter < 0x1400 ) // non AM-Fv case
            {
                /*if ( u2SyncHeight < (u2SigSwing >> 3) )       // Low-sync. case
                {
    	            ucData = 0xD4;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO async. mode, slow W_ADC  
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
    	            mcSHOW_DBG_MSG2(("DRO in low-sync mode!\n"));      	                	       
                }
                else if ( u2SyncHeight > (u2SigSwing >> 2) )*/   // non Low-sync. case
                {
                    //default 0xD4, GA FT issue 0xA4
    	            ucData = 0xA4;  ATD_SetReg(psDemodCtx, 0x964, &ucData, 1); //DRO async. mode, slow W_ADC
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
    	            mcSHOW_DBG_MSG2(("DRO in normal mode!\n"));    	                	            
                }	
            }
            ucData = 0xA5;  ATD_SetReg(psDemodCtx, 0xA34, &ucData, 1); 
            ucData = 0x12;  ATD_SetReg(psDemodCtx, 0xA35, &ucData, 1);
        }                                                       		
    	break;
    	case MOD_SECAM_L:
    	case MOD_SECAM_L1:    		        
        if((ucIFgain >= 0x20) & (ucIFgain <= 0x7F)) // Weak Signal        
        {
    	    ucData = 0x2D;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
    	    ucData = 0x2D;  ATD_SetReg(psDemodCtx, 0xA35, &ucData, 1);
    	    mcSHOW_DBG_MSG2(("DRO in weak signal mode!\n"));       	    
        }
    	else if((ucIFgain <= 0x10) | (ucIFgain >= 0x80))
        {
    	    ucData = 0x1B;  ATD_SetReg(psDemodCtx, 0x97A, &ucData, 1);
    	    ucData = 0x1B;  ATD_SetReg(psDemodCtx, 0xA35, &ucData, 1);
    	    mcSHOW_DBG_MSG2(("DRO in normal mode!\n"));    	        	    
    	}    	
    	break;
    }
}

/*****
 ATD_Patch_LNA(ATD_CTX_T *) is implemented for  ATD monitor LNA , and
 it replace the method of reading  tuner register by reading IF AGC and
 RF AGC to judge signal power.
  implement by Jack Mei ,2008/10/22
 ******/
#ifdef   LNA_MONITOR   
STATIC void ATD_Patch_LNA(ATD_CTX_T *psDemodCtx)
{
   
   UINT16 ucDataRf,ucDataIf;
   BOOL Lna;
   UINT8 ucData1,ucData2;
   	
   ucDataRf = ATD_GetRFAGC(psDemodCtx);
   ucDataIf = ATDPI_GetIFAGC(psDemodCtx);
   ATD_GetReg(psDemodCtx, (REG_DEMOD_PRA_IF_MIN + 0x800), &ucData1, 1);
   ATD_GetReg(psDemodCtx, (REG_DEMOD_PRA_RF_MAX + 0x800), &ucData2, 1);
   UINT16 IFAgcMIN = (UINT16)(ucData1<<8);
   UINT16 RFAgcMAX = (UINT16)(ucData2<<8);
     
   if (( psDemodCtx->u1SubSysIdx >= MOD_PAL_BG )&&(psDemodCtx->u1SubSysIdx <= MOD_PAL_I))
   {
     ITuner_OP(psDemodCtx->pTCtx, itGetLNA, NULL, &Lna);
	 if(Lna)
	 	{
		 mcSHOW_DBG_MSG(("PAL:The LNA status is ON!\n "));
		 
	    }
	 else
	 	{
		 mcSHOW_DBG_MSG(("PAL:The LNA status is OFF!\n "));
		 
	    }
	 	
     if((ucDataRf == RFAgcMAX)&&((ucDataIf < IFAgcMIN )||(ucDataIf >= 0xe9ff))&& (!Lna)) // make sure Signal level <=50dBv LNA on
        {
 
          ATDPI_SetLNA(psDemodCtx, TRUE);
        }
       else if((ucDataIf == IFAgcMIN)&&((ucDataRf <= 0x16ff)||(ucDataRf > RFAgcMAX))&&(Lna))  //make sure Signal level > 70dBv LNA off
         {

          ATDPI_SetLNA(psDemodCtx, FALSE);
        
         }
     }


   if (( psDemodCtx->u1SubSysIdx >= MOD_SECAM_L )&&(psDemodCtx->u1SubSysIdx <= MOD_SECAM_L1))
    {
        ITuner_OP(psDemodCtx->pTCtx, itGetLNA, NULL, &Lna);
	   if(Lna)
	   	{
         mcSHOW_DBG_MSG(("SECAM:The LNA status is ON!\n "));
	    }
	     
	   else
	   	{
	   	
		   mcSHOW_DBG_MSG(("SECAM:The LNA status is OFF!\n "));
        }
    
       if((ucDataRf == RFAgcMAX)&&((ucDataIf < IFAgcMIN )||(ucDataIf > 0xebff))&& (! Lna))  // Signal level <=50dBv
         {
         
           ATDPI_SetLNA(psDemodCtx, TRUE);    
         }
       else if((ucDataIf == IFAgcMIN)&&((ucDataRf <= 0x0aff)||(ucDataRf > RFAgcMAX))&& (Lna)) //Signal level > 70dBv
         {
         
           ATDPI_SetLNA(psDemodCtx, FALSE);
         }
     }

}

#endif
void ATD_Patch(ATD_CTX_T *psDemodCtx)
{
    UINT8 PatchSwitch = psDemodCtx->pTCtx->u1AtdPatchSwitch;

   //ATD_SetNormalMode(psDemodCtx); //because AVS reset will cause video flash,so mark it and remove
                                   // to searchnearby and monitor when TVD lock
   //EC_code_d9922: [DTV00140179] turn-on CR pilot stability and quality auto re-acq  
    
    //QLabIndicator(psDemodCtx);
    Qlab_PrintSelectedIndicator(psDemodCtx);

    // Justin TODO
    if (0)//if( (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) != 0x10 )
    {
        mcSHOW_DBG_MSG(("[ATuner] VIF lock, but Vpress unlock!\n"));

        ATD_SetSystem(psDemodCtx, psDemodCtx->u1SubSysIdx);
        mcDELAY_MS(50);
        ATD_StartAcq(psDemodCtx, FALSE);

    }
    else
    {       
        //u1PatchLoopCnt++;
        //if(mcCHK_MASK(PatchSwitch, MASK_PATCH_AGC))  ATD_Patch_AGC(psDemodCtx);        
        //if (u1PatchLoopCnt >= 5) {
            if(mcCHK_MASK(PatchSwitch, MASK_PATCH_CR))   ATD_Patch_CR(psDemodCtx);
            if(mcCHK_MASK(PatchSwitch, MASK_PATCH_DRO))  ATD_Patch_DRO(psDemodCtx);
            if(mcCHK_MASK(PatchSwitch, MASK_PATCH_CCI))  ATD_Patch_CCI(psDemodCtx);
            if(mcCHK_MASK(PatchSwitch, MASK_PATCH_SENS)) ATD_Patch_Sensitivity(psDemodCtx);
            if(mcCHK_MASK(PatchSwitch, MASK_PATCH_STCHR)) ATD_Patch_StrongChroma(psDemodCtx);
            if(mcCHK_MASK(PatchSwitch, MASK_PATCH_SBEAT)) ATD_Patch_SoundBeat(psDemodCtx);
//Add LNA to  monitor process			
#ifdef   LNA_MONITOR
		    ATD_Patch_LNA(psDemodCtx);
#endif
			
        //}
    }        	
}
    

BOOL Qlab_SetMonIndicator(UINT32 TestItems,UINT32 ucPar1,UINT32 ucPar2)
{      
	if (TestItems > 0)
	{
		Printf("START Monitor Indicator\n");
	}
	else if (TestItems == 0)
	{
		Printf("STOP Monitor Indicator\n");
	}
	
	QLabCondition[0] = ucPar1; // H Sync Loss Threshold : 700
	QLabCondition[1] = ucPar2; // V Sync Loss Threshold : 5
	TestItem = TestItems;

    Printf("Qlab_SetMonIndicator 0x%x %d %d \n",TestItems,QLabCondition[0],QLabCondition[1]);

	return TRUE;
}


void Qlab_PrintSelectedIndicator(ATD_CTX_T *psDemodCtx)
{

	UINT32 CheckItem = TestItem;
	UINT32 MaskItem = 0x1;
	UINT8  Temp = 0;	
	UINT16 Value[7] = {0};

	while (CheckItem > 0)
	{		
		switch(CheckItem & MaskItem)
		{
			case Q_Lab_Sync_Loss:
				{				
					if((IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) == 0x10)
						Printf("##SyncCatch");
					else
						Printf("##SyncKill");
				}			
				break;
			
			case Q_Lab_Color_Loss:
				{
					if(IO_READ8(0x20022000, 0x9D) & 0x01)
						Printf("##ColorCatch");
					else
						Printf("##ColorKill");
				}
				break;

			case Q_Lab_CC:
				{				
					//VBIIndicator();
				}			
				break;				

			case Q_Lab_Sync_Indicator:
				{
					Qlab_Indicator(psDemodCtx,Value);
				}
				break;
				
			case Q_Lab_HSync_Loss:
				{
					if(Value[1] < QLabCondition[0])
						Printf("##HLock");
					else
						Printf("##HShark");
				}
				break;
								
			case Q_Lab_VSync_Loss:
				{
					if(Value[0] < QLabCondition[1])
						Printf("##VLock");
					else
						Printf("##VShark");
				}
				break;

			case Q_Lab_HVSync_Loss:
				{
					if((Value[0] < QLabCondition[1]) && (Value[1] < QLabCondition[0]))
						Printf("##HVLock");
					else	
						Printf("##HVShark");
				}
			break;
				
			case Q_Lab_HSync_Toggle:
				{
					if(Value[4] < 1)
						Printf("##HStable");
					else
						Printf("##HToggle");
				}
				break;				
				
			case Q_Lab_VSync_Toggle:
				{
					if(Value[3] < 1)
						Printf("##VStable");
					else
						Printf("##VToggle");
				}			
				break;				
		}

		CheckItem &= ~MaskItem;
		MaskItem = (MaskItem << 1)  + 1;
		Temp = 1;
	}

	if(Temp == 1)
	{
		Printf("\n");
	}
}

void Qlab_Indicator(ATD_CTX_T *psDemodCtx,UINT16* Values)
{
	UINT32 RegTemp = 0;
	UINT32 dRegTemp = 0;	
            		
	//1 1.Enable Catch Sel
	RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x5EC);
	RegTemp |= 0x00008000; //Bit 15
	IO_WRITE32(VIDEO_IN0_BASE, 0x5EC, RegTemp);

	//1 2.Select Probe Signal
	RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x61C);
	RegTemp &= 0x0FFFFFFF; //Bit 31 ~28
	RegTemp |= 0xB0000000; //Bit 31 ~28
	IO_WRITE32(VIDEO_IN0_BASE, 0x61C, RegTemp);
	mcDELAY_MS(100);

	//1 3. Get Indicator
	dRegTemp = IO_READ32(VIDEO_IN0_BASE, 0x0A8);
	//mcSHOW_USER_MSG(("Reg 0x%x\n",dRegTemp));
	//mcSHOW_USER_MSG(("V Sync Loss sum during 64 fields %d\n", dRegTemp & 0x3F));
	Values[0] = dRegTemp & 0x3F;
	//mcSHOW_USER_MSG(("H Sync Loss sum during 64 fields %d\n", (dRegTemp >> 6) & 0x3FFF));
	Values[1] = (dRegTemp >> 6) & 0x3FFF;
	//mcSHOW_USER_MSG(("VCR status toggle during 64 fields %d\n", (dRegTemp >> 20) & 0x1));
	Values[2] = (dRegTemp >> 21) & 0x1;
	//mcSHOW_USER_MSG(("VPRES status toggle during 64 fields %d\n", (dRegTemp >> 21) & 0x1));            		
	IO_WRITE32(VIDEO_IN0_BASE, 0x0A8, 0x0);
			
	RegTemp = IO_READ32(VIDEO_IN0_BASE, 0x61C);
	RegTemp &= 0x0FFFFFFF; //Bit 31 ~28			
	RegTemp |= 0xA0000000; //Bit 31 ~28
	IO_WRITE32(VIDEO_IN0_BASE, 0x61C, RegTemp);
	//mcSHOW_USER_MSG(("Setting Reg 0x%x\n",RegTemp));	
			
	dRegTemp = IO_READ32(VIDEO_IN0_BASE, 0x0A8);
	//mcSHOW_USER_MSG(("Reg 0x%x\n",dRegTemp)); 
	//mcSHOW_USER_MSG(("V Lock Status toggle during 64 fields %d\n", dRegTemp & 0x3F));
	Values[3] = dRegTemp & 0x3F;
	//mcSHOW_USER_MSG(("H Lock Status toggle during 64 fields %d\n", (dRegTemp >> 6) & 0x3F));
	Values[4] = (dRegTemp >> 6) & 0x3F;
	//mcSHOW_USER_MSG(("B Lock Status toggle during 64 fields %d\n", (dRegTemp >> 12) & 0x3F));
	Values[5] = (dRegTemp >> 12) & 0x3F;
	
	IO_WRITE32(VIDEO_IN0_BASE, 0x0A8, 0x0);			
}

void ATD_LoadEQCoef(ATD_CTX_T *psDemodCtx, UINT8* pucAtdEQScript)
{
    UINT8  ucData, ii;

    ucData= 0x80;  // Set tap index auto-inc when write
    ATD_SetReg(psDemodCtx, 0x92C, &ucData, 1);
    ucData= 0x00;  // Set tap index to 0
    ATD_SetReg(psDemodCtx, 0x92D, &ucData, 1);
    for (ii=0; ii<103; ii++)
    {
        ATD_SetReg(psDemodCtx, 0x92E, (pucAtdEQScript+ii*2), 1);
        ATD_SetReg(psDemodCtx, 0x92F, (pucAtdEQScript+ii*2+1), 1);
    }
    mcSHOW_DBG_MSG(("Finish EQ coefficient dump!!\n"));

    ucData= 0x81;  // Enable EQ
    ATD_SetReg(psDemodCtx, 0x92C, &ucData, 1);
  
}

void ATD_SetEQ(ATD_CTX_T *psDemodCtx, ITUNEROP_T EqScriptType)
{
    UINT8* pucAtdEQScript = NULL;
    UINT8 u1EqScriptSysIdx = psDemodCtx->u1SubSysIdx - MOD_ANA_TYPE_BEGIN;

    // get EQ script
    ITuner_OP(psDemodCtx->pTCtx, EqScriptType, u1EqScriptSysIdx, &pucAtdEQScript);

    if (pucAtdEQScript != NULL) {
        ATD_LoadEQCoef(psDemodCtx, (pucAtdEQScript));
    }
}


/**********************************************************************/
/*                  Query APIs                                        */
/**********************************************************************/

INT32 ATD_GetCFOKHz(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
{
    UINT8 ucData[2];
    S32  CFO_Tmp;

    ATD_GetReg(psDemodCtx, 0x9A2, &ucData[0], 1);
    ATD_GetReg(psDemodCtx, 0x9A3, &ucData[1], 1);

    CFO_Tmp = (S32)mcSIGN_2BYTE(ucData[0], ucData[1], 7, 8);
    //mcSHOW_DBG_MSG2(("ATD CFO Tmp = 0x%04x, ", CFO_Tmp));

    CFO_Tmp = (S32)(CFO_Tmp * 45776 / 100000);
    psDemodCtx->s4Cfo = CFO_Tmp;

    if(fgLogMessage)
    {
    mcSHOW_DBG_MSG(("CFO = %d Khz\n", psDemodCtx->s4Cfo));
    }

    return (psDemodCtx->s4Cfo);

}

UINT16 ATD_GetVOPCpo(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;//, ucData_pre=255, ii, jj=0;

    /*ATD_GetReg(psDemodCtx, ATD_REG_DEMOD_VOP_HW_CPOH, &ucData_pre, 1);
    for(ii=0;ii<10;ii++)
    {
    	mcDELAY_MS(1);*/
        ATD_GetReg(psDemodCtx, ATD_REG_DEMOD_VOP_HW_CPOH, &ucData, 1);
        /*if(ucData == ucData_pre)
        {
            jj++;
        }
        else
        {
            jj=0;
        }
        ucData_pre = ucData;
    }

    if(jj>=5)
    {*/
        psDemodCtx->u2VOPCpo = (UINT16)(ucData * 360 / 256);
        mcSHOW_DBG_MSG(("ATD VOP CPO = %d degree, 0x%02x\n", psDemodCtx->u2VOPCpo, ucData));
    /*}
    else
    {
    	psDemodCtx->u2VOPCpo = 0;
    	mcSHOW_DBG_MSG(("ATD VOP CPO unstable!, cnt=%d\n", jj));
    }*/
    return (psDemodCtx->u2VOPCpo);

}

UINT8 ATD_GetPRALock(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;

    ATD_GetReg(psDemodCtx, ATD_REG_DEMOD_PRA_ST, &ucData, 1);
    if( (ucData & 0x40) != 0x00 )
    {
        psDemodCtx->fgPRALock = TRUE;
    }
    else
    {
    	psDemodCtx->fgPRALock = FALSE;
    }
    return (psDemodCtx->fgPRALock);
}

UINT8 ATD_GetCRLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
{
#if 0
    ATD_GetVOPCpo(psDemodCtx);

    if ( psDemodCtx->u1SubSysIdx >= 3 )  // positive modulation
    {
    	if((psDemodCtx->u2VOPCpo >= 200) && (psDemodCtx->u2VOPCpo <= 230) )
    	    psDemodCtx->fgCRLock = TRUE;
    	else
    	    psDemodCtx->fgCRLock = FALSE;
    }
    else  // negative modulation
    {
    	if((psDemodCtx->u2VOPCpo >= 20) && (psDemodCtx->u2VOPCpo <= 50) )
    	    psDemodCtx->fgCRLock = TRUE;
    	else
    	    psDemodCtx->fgCRLock = FALSE;
    }

    return (psDemodCtx->fgCRLock);
#else
    UINT8 ucData, ucData2;

    ATD_GetReg(psDemodCtx, 0x9A0, &ucData, 1);
    ATD_GetReg(psDemodCtx, 0x9A1, &ucData2, 1);
    
    if(fgLogMessage)
    {
        mcSHOW_DBG_MSG(("Pilot Strength = 0x%02x\n", (ucData & 0x0C) >> 2  ));
        mcSHOW_DBG_MSG(("Pilot Stability = 0x%02x\n", (ucData2 & 0x20) >> 5 ));
        mcSHOW_DBG_MSG(("Pilot Quality = 0x%02x\n", (ucData2 & 0x10) >> 4 ));
    }

    if( ((ucData & 0x0C) != 0x00) || ((ucData2 & 0x30) == 0x30) ) //&& ((ucData2 & 0x33) == 0x03) )
    {
        psDemodCtx->fgCRLock = TRUE;
    }
    else
    {
    	mcSHOW_DBG_MSG(("Bad pilot quality!\n"));
    	psDemodCtx->fgCRLock = FALSE;
    }
    return (psDemodCtx->fgCRLock);
#endif
}

UINT8 ATD_GetVIFLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
{
   // meirui,20100119,show RF and IF gain when channel  scan
   
   if (fgLogMessage)
    {
      ATD_GetRFAGC(psDemodCtx);
	  ATDPI_GetIFAGC(psDemodCtx);
   	}
    ATD_GetCRLock(psDemodCtx, fgLogMessage);
    ATD_GetCFOKHz(psDemodCtx, fgLogMessage);
    //GetPilotPower(psDemodCtx);

    if( (psDemodCtx->fgCRLock == FALSE) && (psDemodCtx->fgCheckCR == TRUE) )
    {
        psDemodCtx->u1VIFLock = cATD_VIF_UNLOCK;
    }
    else
    {
    	if( (psDemodCtx->s4Cfo > (-cATD_VIF_FULL_LOCK_RANGE)) && 
            (psDemodCtx->s4Cfo <   cATD_VIF_FULL_LOCK_RANGE)   )
    	{
    	    psDemodCtx->u1VIFLock = cATD_VIF_FULL_LOCK;
    	}
    	else if( (psDemodCtx->s4Cfo >= (-cATD_VIF_PARTIAL_LOCK_RANGE)) && 
                 (psDemodCtx->s4Cfo <=   cATD_VIF_PARTIAL_LOCK_RANGE)   )
    	{
    	    psDemodCtx->u1VIFLock = cATD_VIF_PARTIAL_LOCK;
        }
        else
        {
            psDemodCtx->u1VIFLock = cATD_VIF_UNLOCK;
        }
    }
    return (psDemodCtx->u1VIFLock);
}

UINT8 ATD_GetSignalLevel(ATD_CTX_T *psDemodCtx)
{
	/*
	INT8 s1AgcMax = (INT8)psDemodCtx->sTunerCtx.m_aucPara[4];
	INT8 s1AgcMin = (INT8)psDemodCtx->sTunerCtx.m_aucPara[5];

	INT8 s1SignalStrength = (psDemodCtx->s1IfAgc-s1AgcMin)*100 / (s1AgcMax-s1AgcMin); // IF AGC gain

	s1SignalStrength = 100 - s1SignalStrength;

	if (psDemodCtx->fgTsLock == TRUE)
	{
		if      (s1SignalStrength < 0)
			return 0;
		else if (s1SignalStrength > 100)
			return 100;
		else
			return (UINT8)s1SignalStrength;
	}
	*/
	return 0;
}

UINT16 ATD_GetRFAGC(ATD_CTX_T *psDemodCtx)
{
    	
    UINT8 ucData1, ucData2;
    UINT16 u2RFAGC;

    ATD_GetReg(psDemodCtx, 0x94D, &ucData1, 1);
    ATD_GetReg(psDemodCtx, 0x94E, &ucData2, 1);
    
    u2RFAGC = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
    mcSHOW_DBG_MSG(("RF gain = 0x%04X\n", u2RFAGC));	
    return u2RFAGC;
}

UINT32 ATDPI_GetNoiseLevel(ATD_CTX_T *psDemodCtx)
{
    	
    UINT8 ucData1, ucData2, ucData3;
    UINT32 u4NoiseLevel;

    ATD_GetReg(psDemodCtx, 0xA42, &ucData1, 1);
    ATD_GetReg(psDemodCtx, 0xA43, &ucData2, 1);
    ATD_GetReg(psDemodCtx, 0xA44, &ucData3, 1);

    u4NoiseLevel = (UINT32) (ucData1 << 11);
    u4NoiseLevel += (UINT32) (ucData2 << 3);
    u4NoiseLevel += (UINT32) (ucData3 >> 5);
    	
    mcSHOW_DBG_MSG(("Get ATD noise level = 0x%08X\n", u4NoiseLevel));
    	
    return u4NoiseLevel;
}

UINT16 ATDPI_GetIFAGC(ATD_CTX_T *psDemodCtx)
{
    	
    UINT8 ucData1, ucData2;
    UINT16 u2IFAGC;

    ATD_GetReg(psDemodCtx, 0x94F, &ucData1, 1);
    ATD_GetReg(psDemodCtx, 0x950, &ucData2, 1);
    
    u2IFAGC = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
    	
    mcSHOW_DBG_MSG(("IF gain = 0x%04X\n", u2IFAGC));
    	
    return u2IFAGC;
}

BOOL ATDPI_SetLNA(ATD_CTX_T *psDemodCtx, BOOL fgLNA)
{

    BOOL fgReturn;
    UINT8 ucData1;
    
    mcSHOW_DBG_MSG(("Set LNA = %d\n", fgLNA));
    
   fgReturn = ITuner_OP(psDemodCtx->pTCtx, itSetLNA, fgLNA, &ucData1);
    //fgReturn = ATD_SetFreq(psDemodCtx,  psDemodCtx->u4Frequency, psDemodCtx->fgAutoSearch);
    return fgReturn;
    
}

BOOL fgGetTVDSts(ATD_CTX_T *psDemodCtx,UINT16 u1ChkTime)
{
   ITUNER_CTX_T * pTCtx=psDemodCtx->pTCtx;
   
   if(!fgDrvTvdCheckTVDLock(u1ChkTime))//TVD unlock first time
  {
     mcSHOW_DBG_MSG(("[ATuner]TVD can not lock at 1st time\n"));
    if(0==(pTCtx->u2RetryTVDTm))
	{
	  mcSHOW_DBG_MSG(("[ATuner]not re-check TVD more(Delay time is 0)\n"));
	  return FALSE;
	}
	mcDELAY_MS(pTCtx->u2RetryTVDTm);
    mcSHOW_DBG_MSG(("[ATuner]2nd check TVD after %d ms delay\n", pTCtx->u2RetryTVDTm));	
	return fgDrvTvdCheckTVDLock(u1ChkTime);
  }
  else//First time lock
  {
     mcSHOW_DBG_MSG(("[ATuner]TVD lock at 1st time\n"));
     return TRUE;
  }   
}
