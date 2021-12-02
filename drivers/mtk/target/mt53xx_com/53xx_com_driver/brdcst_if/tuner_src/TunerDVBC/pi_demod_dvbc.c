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
 * $Author: xingjie.gu $
 * $Date: 2013/01/03 $
 * $RCSfile: $
 * $Revision: #9 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod.c
 *  DVBC demod related API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_demod.h"
#include "mt5135c_com_defs.h"
#include "pd_common.h"
#include "pd_API.h"
#if defined(CC_MT5135) || defined(CC_MT5135DVBC) 
//lei120601 for customer code size issue;
//#include "code_img_MT5135C.h"
//#include "code_img_MT5135C_LEG.h"
#include "code_img_MT5135T.h"
#include "code_img_MT5135T_LEG.h"
#elif defined(CC_MT5396)|| defined(CC_MT5368) //EC_code_dB330: For MT5396DVBTC demod
#include "code_img_MT5396C.h"
#include "code_img_MT5396C_LEG.h"
#endif

#if defined(CC_MT5135) || defined(CC_MT5135DVBC) 
    #define fg5135_GPIOOVERWRITE_DIS    0
#else
    #define fg5135_GPIOOVERWRITE_DIS    1
#endif

#include "ctrl_bus.h"  //add by liuqu,20090429
#include "spi_if.h"
#include "drvcust_if.h"
#include "x_gpio.h"
//-----------------------------------------------------------------------------
// Compile option define
#if defined(CC_MT5396)|| defined(CC_MT5368)  //Lei_code_dB0413: For MT5396DVBTC demod
     #define cRISC_DVBC_BASE          (IO_VIRT + 0x2A000) 
     #define cRISC_ACD_BASE           (IO_VIRT + 0x61000) 
     #ifndef CKGEN_BASE
          #define CKGEN_BASE               (IO_VIRT + 0x0d000) 
     #endif
#endif

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)

//-----------------------------------------------------------------------------
static void  vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len);

// Configurations
//-----------------------------------------------------------------------------
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcADD_HOSTCMD_CHKSUM    0


/*******************************************************/
#define ESTIMATE_SYMBOL_MAX     7000       // 7MBaud/s
#define ESTIMATE_SYMBOL_MIN     4000       // 4MMBaud/s
#define ESTIMATE_SYMBOL_OFFSET  300        //300KBaud/s

#define ESTIMATE_SYMBOL_ACT_MAX  ((ESTIMATE_SYMBOL_MAX) + (ESTIMATE_SYMBOL_OFFSET))
#define ESTIMATE_SYMBOL_ACT_MIN  ((ESTIMATE_SYMBOL_MIN) - (ESTIMATE_SYMBOL_OFFSET))

/*******************************************************/
#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\
//lei120601 for customer code size issue;
#if defined(CC_MT5135) || defined(CC_MT5135DVBC)
EXTERN UINT8 ucDvbtCodeImage[];
EXTERN UINT8 ucDvbtCodeImageLeg[];
#endif

/***********************************************************************/
/*                  Private (static) Function Prototypes                                                     */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  DVBC_ChipInit
 *  This function initialize DVB-C related registers in the chip.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_ChipInit(DEMOD_CTX_T *psDemodCtx)
{

#if defined(CC_MT5396)|| defined(CC_MT5368) //Lei_code_dB0413: For MT5396DVBTC demod
	UINT32 u4Temp;
    UINT32 u4GpioforRFAGCSwitch;

// R/W register mode :in-direct  to direct
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	u4Temp|=0x000f0000;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);
//ACD  settting
//w 0xf0061500 0x00000100
//w 0xf0061504 0x0008A694
//0x0008A894--->0x0008A694,change for CC Hou,just for the THD of the low power(3V) more margin
//w 0xf0061400 0x00AAA882
	u4Temp = 0x00000100;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[2:1]==2'b00,PGA power on
	u4Temp = 0x0008A694;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x504, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//bit[0]=1'b0,ADC power on

	//0xE01[4]=1'b0,reset;=1'b1,enable; CDC ,lei110901;
    //first set 0,reset
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0xE00);
	u4Temp &= 0xFFFFEFFF;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xE00, u4Temp);
	//sencond set 1,enable
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0xE00);
	u4Temp |= 0x00001000;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xE00, u4Temp);
	mcSHOW_DBG_MSG(("Have reset and enable CDC.\n"));
//Demod clock  settting
//w 0xf000d204 0x00000100
//w 0xf000d304 0x00000001
//w 0xf000d218 0x00000001
//w 0xf000d21c 0x00000002
//w 0xf000d300 0x00000002
//w 0xf000d308 0x00000003
//w 0xf000d30c 0x00000104
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x204);
	u4Temp &= 0x8FFFF8FF;
	u4Temp |= 0x00000100;
	mcSET_SHOW_REG(CKGEN_BASE, 0x204, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x304);
	u4Temp &= 0xFFFFFF0C;
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x304, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x218);
	u4Temp &= 0xFFFFFF08;
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x218, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x21c);
	u4Temp &= 0xFFFFFF0c;
	u4Temp |= 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x21c, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x300);
	u4Temp &= 0xFFFFFF0c;
	u4Temp |= 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x300, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x308);
	u4Temp &= 0xFFFFFF08;
	u4Temp |= 0x00000003;
	mcSET_SHOW_REG(CKGEN_BASE, 0x308, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x30c);
	u4Temp &= 0xFFFFFE00;
	u4Temp |= 0x00000104;
	mcSET_SHOW_REG(CKGEN_BASE, 0x30c, u4Temp);
//MT5396
// IFAGC pin mux2 [27] =  1, pinmux2=0x408
// RFAGC, pin mux1[19] = 1, pinmux1=0x404
//MT568
// IFAGC, pin mux2 [20:19] = 01, pinmux2=0x408
// RFAGC, pin mux2[22:21] = 01, pinmux2=0x408
		
	#if defined(CC_MT5396)
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
			u4Temp |= 0x08000000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x404);
			u4Temp |= 0x00080000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x404, u4Temp);		
	#elif defined(CC_MT5368)
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
			u4Temp &= 0xFFEFFFFF;
			u4Temp |= 0x00080000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
			u4Temp &= 0xFFBFFFFF;
			u4Temp |= 0x00200000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
	#endif

//Select DVBC
//w 0xf002ae00 0x00001001  //EU mode DVBC
    u4Temp = 0x00001003;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xe00, u4Temp);
	mcSHOW_USER_MSG(("select DVBC Mode\n"));
//RF AGC switch
	if(DRVCUST_OptQuery(eTunerRFAGCSwitchGpio,&u4GpioforRFAGCSwitch)==0)
	{
		 GPIO_SetOut(u4GpioforRFAGCSwitch,1);//d20110503_Lei,:Gpio set high for DTV
		 mcSHOW_DBG_MSG(("GPIO %d set high to switch RF AGC control\n",u4GpioforRFAGCSwitch));
	}
	

// RFAGC internal control by tuner, set RFAGC pin to be HiZ
#if 0	
	UINT8 ucData;
	DVBT_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	mcSHOW_DBG_MSG(("0xf62=%02x\n",ucData));
    mcCLR_BIT(ucData, 3);
	mcSET_BIT(ucData, 2);
    DVBT_SetReg(psDemodCtx, 0xf62, &ucData, 1);
	DVBT_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	mcSHOW_DBG_MSG(("0xf62=%02x\n",ucData));
#endif

#else
UINT8 ucData;
    if (!psDemodCtx)
    {
       return 1;
    }

#if 1
    /* Temp mark. Hui @ 20100308 */
    // PowerOn setting
    ucData = 0x20;
    DVBC_SetReg(psDemodCtx, 0xE90, &ucData, 1);
    ucData = 0x01;
    DVBC_SetReg(psDemodCtx, 0xE91, &ucData, 1);

	//lei120816 for 5135 xtal cause 27M harmonic issue;
	//change the crystal swing to smallest;
	#if 1
	ucData = 0x0f;
    DVBC_SetReg(psDemodCtx, 0xE92, &ucData, 1);
	#endif
	
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0x09;
    //DVBC_SetReg(psDemodCtx, 0xE9B, &ucData, 1);
    //ucData = 0x27;
    //DVBC_SetReg(psDemodCtx, 0xE9E, &ucData, 1);

    ucData = 0x45;
    DVBC_SetReg(psDemodCtx, 0xEAA, &ucData, 1);
    ucData = 0x45;
    DVBC_SetReg(psDemodCtx, 0xEAC, &ucData, 1);
//    ucData = 0x00;
//    DVBC_SetReg(psDemodCtx, 0xE51, &ucData, 1);
//    ucData = 0x01;
//    DVBC_SetReg(psDemodCtx, 0xE52, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE50, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE5D, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE5E, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE53, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE55, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE57, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE5B, &ucData, 1);
#endif

#if 0
    DVBC_GetReg(psDemodCtx, 0x880, &ucData, 1);
    if(ucData == 0x02)
    {
        ucData = 0x03;
        DVBC_SetReg(psDemodCtx, 0x880, &ucData, 1);
    }
    else if(ucData == 0x03)
    {
    }
    else
#endif
    {
#if 0
        // Move CLKOUT to MTK_Loader.
        // set 5365 clock to 5135
        //w 0xf00280dc 0x0d000084: bit25=0, bit24=1, bit2=1
        if (BSP_GetIcVersion() != IC_VER_5365_AB)   // for ECO IC, not set this register for audio test
            mcSET_SHOW_REG(0xf0028000, 0x0dc, 0x0d000084);
#endif
        // Analog AD setting
        ucData = 0x82;
        DVBC_SetReg(psDemodCtx, 0xE93, &ucData, 1);
        ucData = 0xA8;
        DVBC_SetReg(psDemodCtx, 0xE94, &ucData, 1);
        ucData = 0x5A;
        DVBC_SetReg(psDemodCtx, 0xE95, &ucData, 1);
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE96, &ucData, 1);
        // Analog PGA power on
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE97, &ucData, 1);
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE98, &ucData, 1);

        // Ckgen
        // set demod_54m_up_ck to 54m
        ucData = 0x41;   // b[7]: DVBC clk PD, b[6]: DVBT clk PD, b[5]: DVBTC clk PD
        DVBC_SetReg(psDemodCtx, 0xE51, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE52, &ucData, 1);
        // set demod_216m_mux_ck to 216m
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE53, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE54, &ucData, 1);
        // set demod_86p4m_mux_ck to 54m
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE55, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE56, &ucData, 1);
        // set demod_36m_ck to 54m
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE57, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE58, &ucData, 1);

        {
        UINT32 u4Clock = 0;
        DRVCUST_OptQuery(eCISysClock, &u4Clock);
        switch (u4Clock)
        {
            case 144:
                ucData = 0x7; // 144 MHz
                break;
            case 108:
                ucData = 0x6;
                break;
            case 86: // 86.4
                ucData = 0x5;
                break;
            case 62: // 61.7
                ucData = 0x4;
                break;
            case 54:
                ucData = 0x3;
                break;
            case 48:
                ucData = 0x2;
                break;
            case 72:
                ucData = 0x1;
                break;
            case 36:
                ucData = 0xf;
                break;
            case 27:
                ucData = 0x13;
                break;
            default:
                ucData = 0x7;
                break;
        }
        DVBC_SetReg(psDemodCtx, 0xE59, &ucData, 1);   // CI SYS CLK
        ucData = 0x1;
        DVBC_SetReg(psDemodCtx, 0xE5A, &ucData, 1);

        u4Clock = 0;
        DRVCUST_OptQuery(eCITsClock, &u4Clock);
        switch (u4Clock)
        {
            case 48:
                ucData = 0x7; // 144 MHz
                break;
            case 62: //61.7
                ucData = 0x6;
                break;
            case 72: 
                ucData = 0x5;
                break;
            case 86:  //86.4
                ucData = 0x4;
                break;
            case 108:
                ucData = 0x3;
                break;
            case 144:
                ucData = 0x2;
                break;
            case 54:
                ucData = 0x1;
                break;
            case 36:
                ucData = 0xf;
                break;
            case 27:
                ucData = 0x13;
                break;
            default:
                ucData = 0x5;
                break;
        }
        DVBC_SetReg(psDemodCtx, 0xE5B, &ucData, 1);   // CI SYS CLK
        ucData = 0x1;
        DVBC_SetReg(psDemodCtx, 0xE5C, &ucData, 1);
        }

        // Padmux
       // ucData = 0x01;                                          //Mark by xiongfeng for serial system do not need TS function
       // DVBC_SetReg(psDemodCtx, 0xF8C, &ucData, 1);
        ucData = 0x03;
        DVBC_SetReg(psDemodCtx, 0x880, &ucData, 1);
    }

    // RFAGC internal control by tuner, Ken

    // set RFAGC pin to be HiZ
    DVBC_GetReg(psDemodCtx, 0xf75, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcSET_BIT(ucData, 2);  // gpio enable
    mcCLR_BIT(ucData, 0);  // gpio as input mode
    DVBC_SetReg(psDemodCtx, 0xf75, &ucData, 1);

#endif

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetNormalMode
 *  Set demod to normal mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_SetNormalMode(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 fgRet = 0;
    UINT8 ucData = 0x00;
    
    //U32   u4Temp;

    mcSHOW_DBG_MSG(("PI DVBC Set Normal Mode!!!\n"));
   
    // set RFAGC pin to be output (not GPIO)
    /*DVBC_GetReg(psDemodCtx, 0xf75, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcCLR_BIT(ucData, 2);   // gpio disable
    DVBC_SetReg(psDemodCtx, 0xf75, &ucData, 1);*/
if(psDemodCtx->psTunerCtx->fgRFTuner)
{    // Set IFAGC pad for GPIO control RFAGC switch due to 5365/66 HiZ not perfect
    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcSET_BIT(ucData, 2);   // gpio enable
    mcSET_BIT(ucData, 0);   // gpio as output mode
    mcSET_BIT(ucData, 1);   // gpio as output high
    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);
}   
else
{    // set IFAGC pin to be normal function for legacy
    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcCLR_BIT(ucData, 2);   // gpio disable
    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);

	//lei_code_db110713
	ucData=psDemodCtx->psTunerCtx->uSLD_DefaultPGAIndex;
	DVBC_SetPGAGainIndex(psDemodCtx,ucData);
	mcSHOW_DBG_MSG(("psDemodCtx->sTunerCtx->uSLD_DefaultPGAIndex = 0x%X\n",psDemodCtx->psTunerCtx->uSLD_DefaultPGAIndex));
}
    return fgRet;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_Connect
 *  Start demod module to acquire to a channel.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 Mode, UINT16 Rate)
{
  UINT8 fgRet = 0;  
    UINT16  u2symbolRate = 0;   
    UINT8   u1mode = 0; 

    // Chip init
    DVBC_ChipInit(psDemodCtx);
    #if defined (CC_MT5135) || defined(CC_MT5135DVBC) ||defined(CC_MT5396)|| defined(CC_MT5368) //EC_code_dB330: For MT5396DVBTC demod
    DVBC_FwSwitch(psDemodCtx);/* For Remap Use */
    #endif
    // stop driver
    DVBC_DriverStop(psDemodCtx);
    // set Parameter
    DVBC_SetNormalMode(psDemodCtx);         
    //Convert symbol rate
    u2symbolRate = Rate;
    //Convert QAM mode
    switch (Mode)
    {   
        case MOD_QAM_16:
            u1mode = 0x00;
            break;
        case MOD_QAM_32:
            u1mode = 0x01;
            break;
        case MOD_QAM_64:
            u1mode = 0x02;
            break;
        case MOD_QAM_128:
            u1mode = 0x03;
            break;
        case MOD_QAM_256:
            u1mode = 0x04;
            break;
        case MOD_UNKNOWN:
            u1mode = 0x05;
            break;
        default:
            u1mode = 0x02;
            mcSHOW_DBG_MSG(("Invalid input modulation(%d) to DVBC_TunerAcq!\n", u1mode));
            break;
    }    
    mcSHOW_DBG_MSG(("symbol rate = %d\n", u2symbolRate));
    mcSHOW_DBG_MSG(("Mode = %d\n", u1mode));    
    //Do Tuner Connect and Cal Cost Time        
    //ask pi to work
    // set QAM
    DVBC_SetQamMode(psDemodCtx, u1mode);
    //Set Symbol Rate
    DVBC_SetSymbolRate(psDemodCtx, u2symbolRate);
    // driver start
    DVBC_DriverStart(psDemodCtx);
    //Start Acq Start
    DVBC_AcqStart(psDemodCtx);
    // Change TS Path from Parallel to Serial
    mcDELAY_MS(10);
    DVBC_TSParallelSerialSwitch(psDemodCtx,TRUE);
    fgRet = 0;
    return fgRet;
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_DisConnect
 *  power down demod module.  
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 */
//-----------------------------------------------------------------------------
VOID DVBC_DisConnect(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR ucData = 0x00;
	#if defined(CC_MT5396)||defined(CC_MT5368)
	U32   u4Temp;
	#endif

    mcSHOW_DBG_MSG(("PI DVBC Disconnect!!\n"));

    // stop driver
    DVBC_DriverStop(psDemodCtx);

    //DVBC_SetTsOutput(psDemodCtx, FALSE);   /*wenming 12-2-27: for MTFE, this may need a define to mark...*/

#if defined(CC_MT5396)||defined(CC_MT5368)
	//U32   u4Temp;
	//ADC and PGA power down,Lei110816;
	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x500);
	u4Temp |= 0x00000006;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[2:1]==2'b11,PGA power down

	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x400);
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//bit[0]=1'b1,ADC power down

	// RFAGC internal control by tuner, set RFAGC pin to be HiZ
	DVBC_GetReg(psDemodCtx, 0xf62, &ucData, 1);
    mcSET_BIT(ucData, 3);
	mcSET_BIT(ucData, 2);
    DVBC_SetReg(psDemodCtx, 0xf62, &ucData, 1);
#else
    // set RFAGC pin to be HiZ
    /*DVBC_GetReg(psDemodCtx, 0xf75, &ucData, 1);
       mcCLR_BIT(ucData, 3);   // gpio control by demod
       mcSET_BIT(ucData, 2);  // gpio enable
       mcCLR_BIT(ucData, 0);  // gpio as input mode
       DVBC_SetReg(psDemodCtx, 0xf75, &ucData, 1);*/
	if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
	 {   // Set IFAGC pad for GPIO control RFAGC switch due to 5365/66 HiZ not perfect
	    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
	    mcCLR_BIT(ucData, 3);   // gpio control by demod
	    mcSET_BIT(ucData, 2);  // gpio enable
	    mcSET_BIT(ucData, 0);  // gpio as output mode
	    mcCLR_BIT(ucData, 1);  // gpio as output low
	    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);
	 }  
	else
	{    // set IFAGC pin to be HiZ for legacy
	    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
	    mcCLR_BIT(ucData, 3);   // gpio control by demod
	    mcSET_BIT(ucData, 2);  // gpio enable
	    mcCLR_BIT(ucData, 0);  // gpio as input mode
	    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);
	}
	#if 1

#if fg5135_GPIOOVERWRITE_DIS
    // b[7]: DVBC clk PD, b[6]: DVBT clk PD, b[5]: DVBTC clk PD, b[1:0]: 54M clk select (XTAL/2)
    ucData = 0xC2;   // b[7]: DVBC clk PD, b[6]: DVBT clk PD, b[5]: DVBTC clk PD
    DVBC_SetReg(psDemodCtx, 0xE51, &ucData, 1);
    // apply 54M clk selection change
    ucData = 0x01;
    DVBC_SetReg(psDemodCtx, 0xE52, &ucData, 1);
    // b[7]: ci_sys_clk power down, b[6:5]: ci_sys test clock selection, b[4:0]: ci_sys clock selection
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE59, &ucData, 1);
    // apply ci_sys clock select
    ucData = 0x01;
    DVBC_SetReg(psDemodCtx, 0xE5A, &ucData, 1);
    // demod216m_ck power down
    ucData = 0x40;
    DVBC_SetReg(psDemodCtx, 0xE53, &ucData, 1);
    // demod86p4m_ck power down
    DVBC_SetReg(psDemodCtx, 0xE55, &ucData, 1);
    // demod36m_ck power down
    DVBC_SetReg(psDemodCtx, 0xE57, &ucData, 1);
    // ?? test_ck0 ??
    DVBC_SetReg(psDemodCtx, 0xE5B, &ucData, 1);

    // Power down of selected pll clock
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0x01;
    //DVBC_SetReg(psDemodCtx, 0xE50, &ucData, 1);
    // Power down of selected dmsspll clock
    DVBC_SetReg(psDemodCtx, 0xE5D, &ucData, 1);
    // Power down of selected dddspll clock
    DVBC_SetReg(psDemodCtx, 0xE5E, &ucData, 1);
    // b[7]: RSSI input buffer power down
    ucData = 0xA0;
    DVBC_SetReg(psDemodCtx, 0xE90, &ucData, 1);
    // b[0]: RSSI ADC power down
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE91, &ucData, 1);
    // b[3]: IFTESTBUF power down, b[2]: IFBG power down
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0x0D;
    //DVBC_SetReg(psDemodCtx, 0xE9B, &ucData, 1);
    // b[7]: IF VCO power down, b[6]: IF PLL power down
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0xE7;
    //DVBC_SetReg(psDemodCtx, 0xE9E, &ucData, 1);
#endif

    // AD power down
    // b[0]: ADC PWD DEMOD
    ucData = 0x83;
    DVBC_SetReg(psDemodCtx, 0xE93, &ucData, 1);
    // b[1]: Disable ADC ckgen
    ucData = 0x02;
    DVBC_SetReg(psDemodCtx, 0xE96, &ucData, 1);
#if fg5135_GPIOOVERWRITE_DIS
    ucData = 0x57;
    DVBC_SetReg(psDemodCtx, 0xEAA, &ucData, 1);
    DVBC_SetReg(psDemodCtx, 0xEAC, &ucData, 1);
	#endif
#endif
#endif
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_DemodCtxCreate
 *  This function creates demod context and returns the context pointer.
 *  @param  void.
 *  @retval   DEMOD_CTX_T * : pointer of demod context.   
 */
//-----------------------------------------------------------------------------
DEMOD_CTX_T *DVBC_DemodCtxCreate(void)
{
    DEMOD_CTX_T *p = NULL;

    CREATE_OBJECT(p, DEMOD_CTX_T);

    // error handling
    if (!p)
       return NULL;

    // zero memory
    memset(p, 0, sizeof(DEMOD_CTX_T));

    return p;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DemodCtxDestroy
 *  This function destroys context you have created and releases the memory.
 *  @param  p  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DemodCtxDestroy(DEMOD_CTX_T *p)
{
    DESTROY_OBJECT(p);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDemodReg
 *  This function will read demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_GetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ii = 0;

    if (!psDemodCtx)
        return 1;
    if ( (ucLen == 0) || (ucLen > 4) )
        return 1;
    if (!pucBuf)
        return 1;

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucLen;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE( u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucLen - 1, ccOFST_REG1_SIZE+1))
    {
        for (ii = 0; ii < ucLen; ii++)
             *(pucBuf+ii) = mccGET_QUEUE(aucData, ccOFST_REG1_DATA+ii);
         return 0;
    }

    return 1;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetDemodReg
 *  This function will write demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_SetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ii = 0;

    if (!psDemodCtx)
        return 1;
    if ( (ucLen == 0) || (ucLen > 4) )
        return 1;
    if (!pucBuf)
        return 1;

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucLen;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE( u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    for (ii = 0; ii < ucLen; ii++)
        mccSET_QUEUE2(aucData, ccOFST_REG1_DATA+ii) = *(pucBuf+ii);

    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_REG1_SIZE+ucLen-1+1))
         return 0;

    return 1;
}

/***********************************************************************/
/*                  Private (static) Function Prototypes               */
/***********************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetReg
 *  This function set demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    //if (u1I2cDemWrite(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))  
    if(ICtrlBus_DemodWrite(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG(("Demod Write Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetReg
 *  This function get demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    //if (u1I2cDemRead(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    if(ICtrlBus_DemodRead(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG((" Demod Read Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/** download image

 *  @param  
 *  @param  
 *  @retval   others : Error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_Loadcode(DEMOD_CTX_T *psDemodCtx, UINT8 *pu1Buffer, UINT16 u2CurRomLen)
{
    if(ICtrlBus_DemodLoadCode(psDemodCtx->I2cAddress, DVBC_REG_uP_IRData, pu1Buffer, u2CurRomLen))
    {
        /*fail case*/
        mcSHOW_POP_MSG(("Download Code fail!\n"));
        return 1;
    }

    return 0;
}

/************************************************************************/
#define cMAX_TRY_CNT            2
#define cMAX_WAIT_LOOP          10
//#define cWAIT_TIME_INTERVAL     200         // ms
#define cWAIT_TIME_INTERVAL     10         // ms

//----------------------------------------------------------------------------- 
/*
 *  fgHostCmd
 *  Host command interface API between host and demod.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  aucCmdData  Data buffer.
 *  @param  ucReadParacnt  Read parameter byte counts.
 *  @param  ucWriteCmdcnt  Write command byte counts.
 *  @retval   TRUE : Host command success.
 *  @retval   FALSE : Host command fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgHostCmdDVBC(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
{
UCHAR   ucWriteParacnt;
UINT16  ii;
UINT8   ucValue;
UCHAR   ucParaStart, ucRegSetOfst;
BOOL    fgRetStatus = TRUE;
#if fcMEAS_HOST_CMD_TIME
UINT32  u4TickStart;
UINT32  u4TickCnt;
#endif
UCHAR   ucReadChkCnt;
#if fcADD_HOSTCMD_CHKSUM
UCHAR   ucChkSum;
#endif
UCHAR   jj;

#if fcMEAS_HOST_CMD_TIME
    u4TickStart = mcGET_SYS_TICK();
#endif

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucValue, 1))
    {
    //    psDemodCtx->u1I2cNakCnt ++;
        mcSHOW_HW_MSG((" _I2_ "));

        return FALSE;                       // I2C fail
    }
    if (!mcTEST_BIT(ucValue, ccpUP_ENABLE))
    {
        psDemodCtx->u1UpChkCnt ++;
    //    mcSHOW_HW_MSG(("uP not enabled!\n"));
        mcSHOW_HW_MSG((" _UE_ "));

        return FALSE;
    }
    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1))
    {
        return FALSE;                       // I2C fail
    }
    if (ucValue != ccFW_RUN_TAG)
    {
        psDemodCtx->u1UpChkCnt ++;
        if (ucValue == ccFW_WDT_TAG)
        {
        //    mcSHOW_HW_MSG(("Watchdog reset!\n"));
            mcSHOW_HW_MSG((" _WR_ "));
        }
        else
        {
        //    mcSHOW_HW_MSG(("Fw not running!\n"));
            mcSHOW_HW_MSG((" _FR_ "));
        }

        return FALSE;
    }   
    if (ucWriteCmdcnt)
    {
        ucWriteParacnt = ucWriteCmdcnt - 1;
        for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
        {
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
            // This while loop will cause dead-lock for Combo-Tuner (when I2C_Block = TRUE)
        #if 1
            jj = 0;
            while (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
            {
                DVBC_GetReg(psDemodCtx, DVBC_REG_RSICIF_RSP7, &ucValue, 1);
                DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
                jj ++;
                if (jj >= cMAX_WAIT_LOOP)
                    break;
            }
        #endif
            if (!mcTEST_BIT(ucValue, ccpUP_CMD_RX))
                break;
            mcDELAY_MS(cWAIT_TIME_INTERVAL);
        }

        #if fcMEAS_HOST_CMD_TIME
        u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
        mcSHOW_TIME_MSG((" ** Cmd%02X %d (%3u)", mccSET_CMD2(aucCmdData),
                                           ii, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif

        if (ucWriteParacnt > ccCMD_REG_NUM - 1)
        {
            mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d > %d!",
                                              mccSET_CMD2(aucCmdData), ucWriteParacnt, ccCMD_REG_NUM - 1));
            ucWriteParacnt = ccCMD_REG_NUM - 1;
        }
        #if fcADD_HOSTCMD_CHKSUM
        if (ucWriteParacnt == (ccCMD_REG_NUM - 1))
        {
            mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d == %d!",
            mccSET_CMD2(aucCmdData), ucWriteParacnt, ccCMD_REG_NUM - 1));
        }
        else
        {
            ucChkSum = 0;
            for (ii = 0; ii < ccCMD_REG_NUM - 1; ii ++)
            {
                ucChkSum ^= aucCmdData[ccCMD_REG_NUM - 1 - ii + 1];
            }
            aucCmdData[1] = ucChkSum;
            ucWriteParacnt = ccCMD_REG_NUM - 1;
        }
        #endif

        if (ii == cMAX_WAIT_LOOP)
            psDemodCtx->u1UpNakCnt ++;

        ucParaStart = ccCMD_REG_NUM - ucWriteParacnt - 1;
        ucRegSetOfst = 1;

        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_RSICIF_CMD0 + ucParaStart),
                                      aucCmdData + ucParaStart + ucRegSetOfst,
                                                          ucWriteParacnt + 1);
        #if fcMEAS_HOST_CMD_TIME
        u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
        mcSHOW_TIME_MSG((" Para %d (%3u)", ucWriteParacnt, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif
        #if fcSHOW_HOST_CMD
        if (fgShowHostCmd)
        {
            //    mcSHOW_USER_MSG(("HostCmd: (%d) -->", ucParaStart));
            mcSHOW_USER_MSG(("HostCmd -->"));
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
            {
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
            }
            mcSHOW_USER_MSG(("\n           "));
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR00 + ucParaStart,
                                                         aucCmdData + ucParaStart + ucRegSetOfst,
                                                         ucWriteParacnt + 1);
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
            {
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
            }
            mcSHOW_USER_MSG(("\n"));
        }
        #endif
    }

    if (ucReadParacnt)
    {
        UCHAR   ucCmdId;

        ucCmdId = mccSET_CMD2(aucCmdData);
        fgRetStatus = FALSE;
        for (jj = 0; jj < cMAX_TRY_CNT; jj ++)
        {
            for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
            {
                DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
                if (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
                    break;
                mcDELAY_MS(cSHORT_SLEEP_TIME);
            }

            if (ucReadParacnt > ccCMD_REG_NUM - 1)
            {
                mcSHOW_HW_MSG(("Rsp %02X Para Cnt %d > %d!",
                                                  mccSET_CMD2(aucCmdData), ucReadParacnt, ccCMD_REG_NUM - 1));
                ucReadParacnt = ccCMD_REG_NUM - 1;
            }
            ucReadChkCnt = ucReadParacnt;
            #if fcADD_HOSTCMD_CHKSUM
            if (ucReadChkCnt < ccCMD_REG_NUM - 1)
                ucReadChkCnt ++;
            #endif
            if (ii < cMAX_WAIT_LOOP)
            {
                UINT16  u2AddrOfst = 0;

                #if fcMEAS_HOST_CMD_TIME
                u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
                mcSHOW_TIME_MSG((" Rsp %d_%d (%3u)", jj, ii, u4TickCnt * mcGET_TICK_PERIOD()));
                #endif
                ucParaStart = ccCMD_REG_NUM - ucReadChkCnt - 1;

                if (DVBC_GetReg(psDemodCtx,
                                            (UINT16) (DVBC_REG_RSICIF_RSP0 + u2AddrOfst + ucParaStart),
                                            aucCmdData + ucParaStart, ucReadChkCnt + 1))
                {
                    return FALSE;
                }

                #if fcMEAS_HOST_CMD_TIME
                u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
                mcSHOW_TIME_MSG((" Para %d (%3u)", ucReadChkCnt, u4TickCnt * mcGET_TICK_PERIOD()));
                #endif

                if (mccGET_CMD(aucCmdData) == ucCmdId)
                {
                    fgRetStatus = TRUE;
                    #if fcSHOW_HOST_CMD
                    if (fgShowHostCmd)
                    {
                        mcSHOW_USER_MSG(("HostRsp -->"));
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                        {
                            mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                        }
                        mcSHOW_USER_MSG(("\n"));
                    }
                    #endif
                    
                    #if fcADD_HOSTCMD_CHKSUM
                    if (ucReadParacnt < ccCMD_REG_NUM - 1)
                    {
                        ucChkSum = 0;
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                        {
                            ucChkSum ^= aucCmdData[ccCMD_REG_NUM - 1 - ii];
                        }
                        if (ucChkSum)
                        {
                            fgRetStatus = FALSE;
                            mcSHOW_HW_MSG((" _@CS_ "));
                            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_RSPR00 + ucParaStart,
                                                   aucCmdData + ucParaStart, ucReadChkCnt + 1);
                            mcSHOW_USER_MSG(("-->"));
                            for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                            {
                                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                            }
                            mcSHOW_USER_MSG(("\n"));
                            psDemodCtx->u1UpMisCnt ++;
                        }
                    }
                    #endif
                    break;
                }
                else
                    psDemodCtx->u1UpMisCnt ++;
            }
            else
            {
                psDemodCtx->u1UpNakCnt ++;
                break;
            }
        }
        //    mcSHOW_HW_MSG(("Error uP Rsp!"));
    }

    #if fcMEAS_HOST_CMD_TIME
    u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
    //    mcSHOW_DBG_MSG((" %3uH ", u4TickCnt * mcGET_TICK_PERIOD()));
    mcSHOW_TIME_MSG((" %3uH ", u4TickCnt * mcGET_TICK_PERIOD()));
    mcSHOW_TIME_MSG(("\n"));
    #endif

    return fgRetStatus;
}

//----------------------------------------------------------------------------- 
/*
 *  fgDVBCGetFwVars
 *  Get firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDVBCGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if(ucCnt>(ccCMD_REG_NUM-2-ccOFST_REG1_DATA+1))/*Prevent aucData Buffer Overflow  */ 
    {
        return FALSE;
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_VAR;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    //mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = 0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    {
        for (ii = 0; ii < ucCnt; ii ++)
            pucData[ii] = mccGET_QUEUE(aucData, ccOFST_REG1_DATA + ii);
    }
    else
        return FALSE;
    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDvbcNotExist
 *  Check if really no DVB-C signal exists.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE  : It's confirmed that no DVB-C signal exists.   
 *  @retval   FALSE : DVB-C signal exists or it's not confirmed yet.
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetDvbcNotExist(DEMOD_CTX_T *psDemodCtx)
{
    if (!psDemodCtx)
        return FALSE;

    mcSHOW_DBG_MSG((".DVBC ReAcq Cnt %d, QCN Fail Cnt %d, ASR Fail Cnt %d!\n", 
        psDemodCtx->ucReAcqCnt, psDemodCtx->ucQCNFailState, psDemodCtx->ucASRFailState));

//    if ( (psDemodCtx->ucASRFailState>0) || (psDemodCtx->ucQCNFailState>0) )
    if ( (psDemodCtx->ucASRFailState>0) )  // removed  the QCN fail for early break
    {
        return TRUE;
    }
	else if ((psDemodCtx->ucReAcqCnt > 1 ))
	{    
        DVBC_UpdateSymbol(psDemodCtx);

        mcSHOW_DBG_MSG(("DVBC ASR Ready : %d\n", psDemodCtx->fgAsrReady));

        if (psDemodCtx->fgAsrReady)
        {
		
            mcSHOW_DBG_MSG(("DVBC esimate QAM: %d , RS: %d!\n", psDemodCtx->ucQamMode, psDemodCtx->u2SymbolRate));
            if (psDemodCtx->u2SymbolRate == 0)
            {
                mcSHOW_DBG_MSG(("Esimate symbol rate is 0, not yet\n"));
                return FALSE;
            }
            else if ((psDemodCtx->u2SymbolRate<(psDemodCtx->u2SymbolrateMin-ESTIMATE_SYMBOL_OFFSET))||
                       (psDemodCtx->u2SymbolRate>(psDemodCtx->u2SymbolrateMax+ESTIMATE_SYMBOL_OFFSET)))
            {   
                mcSHOW_DBG_MSG(("Esimate symbol rate is  %dkbaud/s, out of range [%dkbaud/s, %dKbaud/s], need early break\n",
                                                 psDemodCtx->u2SymbolRate, (psDemodCtx->u2SymbolrateMin-ESTIMATE_SYMBOL_OFFSET), (psDemodCtx->u2SymbolrateMax+ESTIMATE_SYMBOL_OFFSET)));
                return TRUE;
            }
            else
            {
                mcSHOW_DBG_MSG(("Esimate symbol rate is  %dkbaud/s, in range [%dkbaud/s, %dKbaud/s], ok!\n",
                                                 psDemodCtx->u2SymbolRate, (psDemodCtx->u2SymbolrateMin-ESTIMATE_SYMBOL_OFFSET), (psDemodCtx->u2SymbolrateMax+ESTIMATE_SYMBOL_OFFSET)));
                return FALSE;
            }
        }
        else
        {
            mcSHOW_DBG_MSG(("DVBC ASR Not Ready\n"));
            return FALSE;
        }
    }
    else
        return FALSE;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDvbcTsNotExist
 *  Check if RSH lock but PER>80% or TotalPkt=0.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Lock   : DVB-C TS really ok
 *  @retval   Unknown: DVB-C TS not confirmed yet.
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetDvbcTsNotExist(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1TsState = DVBC_DEMOD_STATE_UNKNOWN;

    if (!psDemodCtx)
        return FALSE;

    if (psDemodCtx->ucRSHState)
    {
        UCHAR   aucData[cMAX_READ_NUM + 1];
        UCHAR   uc_data[2];
        UINT16  u2ErrPkt=0;
        UINT16  u2TotPkt=0;

        mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO0;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO0_SIZE, 1))
        {
            //total error packets
            uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT0);
            uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT1);
            u2ErrPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8); 
            //total packets
            uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT0);
            uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT1);
            u2TotPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        }
        // If TS is locked, but error rate is high; not report locked, it may be QAM error (eg: 16QAM signal locked with 256QAM)
        if ((u2TotPkt != 0) && ((u2TotPkt * 9) > (u2ErrPkt * 10)))
        {
            u1TsState = DVBC_DEMOD_STATE_LOCK;
        }
        else
        {
            u1TsState = DVBC_DEMOD_STATE_UNSTABLE;
            mcSHOW_DBG_MSG(("..DVBC TS lock, but TotalPkt %d, ErrorPkt %d. Continue try..!\n", u2TotPkt, u2ErrPkt));
        }
    }

    return u1TsState;
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSync
 *  Get demod lock status.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   DVBC_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DVBC_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DVBC_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DVBC_GetSync(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1DvbcDemodState = DVBC_DEMOD_STATE_UNKNOWN;
#if 0
    // for debug
    UCHAR   aucData[cMAX_READ_NUM + 1];
#endif

    DVBC_UpdateStatus(psDemodCtx); // update SC state, FFT & TS lock
   
	//Mingsheng 0802. Remove to tuner acq to decrease the scan time ;
    //DVBC_UpdateInfo(psDemodCtx); // Iven, for Symbol rate update 20110317
    if (DVBC_GetDvbcNotExist(psDemodCtx))
    {
        u1DvbcDemodState = DVBC_DEMOD_STATE_UNLOCK;
    }
    else
    {
        u1DvbcDemodState = DVBC_GetDvbcTsNotExist(psDemodCtx);
    }

#if 0
    // for debug
    if (u1DbgLevel > 1)
    {
        mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO6;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO6_SIZE, 1))
        {
            psDemodCtx->ucAQM_Seq[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_00);
            psDemodCtx->ucAQM_Seq[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_01);
            psDemodCtx->ucAQM_Seq[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_02);
            psDemodCtx->ucAQM_Seq[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_03);
            psDemodCtx->ucAQM_Seq[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_04);
            psDemodCtx->ucAQM_ID = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_Index);
        }
        mcSHOW_DBG_MSG(("AQM Sequence: %d-%d-%d-%d-%d, %d\n", psDemodCtx->ucAQM_Seq[0], psDemodCtx->ucAQM_Seq[1], psDemodCtx->ucAQM_Seq[2], psDemodCtx->ucAQM_Seq[3], psDemodCtx->ucAQM_Seq[4], psDemodCtx->ucAQM_ID));
    }
#endif
    
    return u1DvbcDemodState;
}

//----------------------------------------------------------------------------- 
/*
 *  ucGetFwVar
 *  Get firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @retval   1-byte data.   
 */
//-----------------------------------------------------------------------------
UINT8 ucDVBCGetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
    UCHAR   ucValue = 0;

    fgDVBCGetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
    return ucValue;
}
//----------------------------------------------------------------------------- 
/*
 *  fgDVBCSetFwVars
 *  Set firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDVBCSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if(ucCnt>(ccCMD_REG_NUM-2-ccOFST_REG1_DATA+2))/*Prevent aucData Buffer Overflow  */ 
    {
        return FALSE;
    }
    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_VAR;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    //mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
        mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = 0;
    for (ii = 0; ii < ucCnt; ii ++)
        mccSET_QUEUE2(aucData, ccOFST_REG1_DATA + ii) = pucData[ii];
    //if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_REG1_SIZE + ucCnt -1 + 1))
    {
        return TRUE;
    }
    return FALSE;
}
//----------------------------------------------------------------------------- 
/*
 *  s4LoadCode_patch
 *  Download firmware code for MT5396 up sram share sen.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u1CodeImage  Pointer of firmware code image.
 *  @param  u2CurRomLen  Firmware code size in byte.
 *  @param  u2MaxI2cLen  Maximum I2C length per transfer.
 *  @param  fgClk50  Flag to switch if 48MHz or 24MHz clock is used.
 *  @retval   cUP_LOAD_OK : Load code OK.
 *  @retval   cUP_LOAD_ERR_I2C : I2C error.
 *  @retval   cUP_LOAD_ERR_HW_RDY : HW not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_RDY : Checksum not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_OK : Checksum not OK.   
 */
//----------------------------------------------------------------------------- 

#if defined(CC_MT5396)|| defined(CC_MT5368)
#if (ATD_NOT_USE==FALSE)
static INT32 s4LoadCode_patch(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{
    UCHAR   ucValue, ucUpCtrl, ii;
	UINT16 u2SramSize;
#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
    UCHAR   ucI2cAuto;
#endif
    UINT16  /*u2CurI2cLen,*/ u2CurRamAdr;
    /*UCHAR   ucI2cAddr;*/
#if fcLOAD_CODE_TIME
    TIME_TICK_T u4TickStart, u4TickCur;
    TIME_DIFF_T u4TickCnt;
#endif

#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    /* Confirm Load FW Source Code */

	// for 5396 ES SRAM sharing patch
    #if defined(CC_MT5135) || defined (CC_MT5135DVBC)
	    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
	    if(ucUpCtrl == 0x66)
	    {
			mcSHOW_POP_MSG(("Have not download code,direct return!\n"));
			return cUP_LOAD_OK;
	    }
	#elif defined(CC_MT5396)|| defined(CC_MT5368)
		ucUpCtrl = 0;	
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		
		ucUpCtrl = 0x00;
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
	#endif
	
	ucUpCtrl = 0;
	DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
	ucUpCtrl = mcBIT(ccpUP_RESTART_CHKSUM) | mcBIT(ccpUP_ROMCODE_CHKSUM);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not ready\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not OK\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_OK;
    }
	
	
#endif

//#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368)
    // SRAM clear
    ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcDELAY_MS(10);
#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368) 
    // set ad clock phase for more margin case
    DVBC_GetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);
    mcCLR_BIT(ucUpCtrl, 5);
    DVBC_SetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1))
        return cUP_LOAD_ERR_I2C;
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
    {
        return cUP_LOAD_ERR_HW_RDY;
    }
#endif

    ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    ucValue = 0;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);

    for (ii = 0; ii < 4; ii ++)
    {
        DVBC_GetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
    }

// set max code size.
#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    ucValue = 0x9F;
#else
    ucValue = 0x6F;
#endif
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_H, &ucValue, 1);
    ucValue = 0xFF;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_L, &ucValue, 1);

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn off I2C auto-increment
    DVBC_GetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
    mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    //UINT16 u2SramSize;
#if defined(CC_MT5135) || defined(CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    u2SramSize = 110*1024/10;
#else
    u2SramSize = 0;
#endif
    u2CurRamAdr = u2SramSize;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
    // for 40k SRAM, IR_RD is 0x00[7]
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2CurRamAdr);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_L, &ucValue, 1);

    /*ucI2cAddr = psDemodCtx->I2cAddress;*/
    if (DVBC_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
    {
        mcSHOW_POP_MSG(("Download FW fail! (%d)\n"));
        return 1;
    }
	mcSHOW_POP_MSG(("Download FW code finished!\n"));

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn on I2C auto-increment
    mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

    mcSET_BIT(ucUpCtrl, ccpUP_RESTART_CHKSUM);
    mcCLR_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        return cUP_LOAD_ERR_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        return cUP_LOAD_ERR_CHKSUM_OK;
    }

    ucUpCtrl = mcBIT(ccpUP_ENABLE);          // 50 MHz Clock
    if (!fgClk50)
        ucUpCtrl = mcBIT(ccpUP_ENABLE) | mcBIT(ccpUP_DN_FREQ);    // 25 MHz Clock
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);

    // delay for waiting FW initialization done, may have I2C timeout at first
    for (ii = 0; ii < 5; ii++)
    {
        mcDELAY_MS(3);
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
        if (ucValue == ccFW_RUN_TAG)
            break;
    }

#if defined(CC_MT5135) || defined(CC_MT5135DVBC) //|| defined(CC_MT5396)
    // enable HOST_EN for host to be able to access cmd/rsp register
    ucValue = 0x01;
    DVBC_SetReg(psDemodCtx, DVBC_REG_RFA4A0, &ucValue, 1);
#endif

#if fcLOAD_CODE_TIME
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    u4TickCnt = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" (%3u ms) ", u4TickCnt));
    if (u4TickCnt > 0)
        return -((INT32) u4TickCnt);
    else
#endif

    return cUP_LOAD_OK;
}
#endif
#endif

//----------------------------------------------------------------------------- 
BOOL DVBC_FwSwitch(DEMOD_CTX_T *psDemodCtx)
{
#if defined (CC_MT5135) || defined(CC_MT5135DVBC) ||defined(CC_MT5396)|| defined(CC_MT5368) //EC_code_dB330: For MT5396DVBTC demod
    UCHAR ucValue;
    /*
    UCHAR ucUpCtrl = 1;// Enable uP //EC_debug_dB506
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcSHOW_DBG_MSG(("enable DVBC UP!\n"));    
    mcDELAY_MS(10);
	*/

	#if defined(CC_MT5396)|| defined(CC_MT5368)// for 5396 ES SRAM sharing patch 

    UCHAR ucData = 0x00;
    
    #if (ATD_NOT_USE==FALSE)
	
	UINT8 * pCodeImage=NULL;
	UINT16 u2CodeImageLen;
    //UCHAR ucData = 0x00;
	
	//Reset SLD REG //Mingsheng110616
	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);

	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	
    if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
    {
          pCodeImage=ucDvbcCodeImage;
          u2CodeImageLen=sizeof(ucDvbcCodeImage);
    }
    else//legacy tuner
    {
          pCodeImage=ucDvbcCodeImageLeg;
          u2CodeImageLen=sizeof(ucDvbcCodeImageLeg);
    }

		mcSHOW_USER_MSG(("Re-download FW start!!\n"));
		
    if (s4LoadCode_patch(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
    {
            mcSHOW_USER_MSG(("Re-download FW fail!!\n"));
    }
	
	#endif
    
	#endif
    
    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG07, &ucValue, 1);
    if(0x01 != ucValue)
    {
        UCHAR aucData[cMAX_READ_NUM + 1], ii;

        mccSET_CMD2(aucData) = ccCMD_ID_DVBTC_SWITCH;
        mccSET_QUEUE2(aucData, ccOFST_DVBTC_SWITCH) = cCONN_TYPE_DVBC;//0x01:DVBC, 0x02: DVBT
        mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBTC_SWITCH_RSIZE, ccOFST_DVBTC_SWITCH_SIZE+1);

        // delay for waiting FW initialization done, may have I2C timeout at first
        for (ii = 0; ii < 5; ii++)
        {
            mcDELAY_MS(3);
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
        }

        mcSHOW_DBG_MSG2(("FW switch to DVBC!"));
    }

	#if defined(CC_MT5396)|| defined(CC_MT5368)
    // RFAGC internal control by tuner, set RFAGC pin to be HiZ
    if(!psDemodCtx->psTunerCtx->fgRFTuner)//legacy tuner
    {
		DVBC_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	    mcSET_BIT(ucData, 3);
		mcSET_BIT(ucData, 2);
	    DVBC_SetReg(psDemodCtx, 0xf62, &ucData, 1);
    }
	#endif
	
    // set agc parameters
    DVBC_SetIF(psDemodCtx);     // set IF when FW switch /*wenming 12-2-27: for MTFE, this may need a define to mark...*/
#endif

    return TRUE;
}

//-----------------------------------------------------------------------------
VOID DVBC_TSParallelSerialSwitch(DEMOD_CTX_T *psDemodCtx, BOOL IsSerial)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_TS_MODE;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_TS_MODE) = (UCHAR)IsSerial;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_TS_MODE_RSIZE, ccOFST_DVBC_SET_TS_MODE_SIZE+1);
}

/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  s4LoadCode
 *  Download firmware code.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u4I2cClkDiv  Unused.
 *  @param  u1CodeImage  Pointer of fimware code image.
 *  @param  u2CurRomLen  Firmware code size in byte.
 *  @param  u2MaxI2cLen  Maximum I2C length per transfer.
 *  @param  fgClk50  Flag to switch if 48MHz or 24MHz clock is used.
 *  @retval   cUP_LOAD_OK : Load code OK.
 *  @retval   cUP_LOAD_ERR_I2C : I2C error.
 *  @retval   cUP_LOAD_ERR_HW_RDY : HW not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_RDY : Checksum not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_OK : Checksum not OK.   
 */
//-----------------------------------------------------------------------------
static INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{
    UCHAR   ucValue, ucUpCtrl, ii;
	UINT16 u2SramSize;
#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
    UCHAR   ucI2cAuto;
#endif
    UINT16  /*u2CurI2cLen,*/ u2CurRamAdr;
    /*UCHAR   ucI2cAddr;*/
#if fcLOAD_CODE_TIME
    TIME_TICK_T u4TickStart, u4TickCur;
    TIME_DIFF_T u4TickCnt;
#endif

#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    /* Confirm Load FW Source Code */
    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
    if(ucUpCtrl == 0x66)
    {
        mcSHOW_POP_MSG(("Have not download code,direct return!\n"));
        return cUP_LOAD_OK;
    }
	ucUpCtrl = 0;
	DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
	ucUpCtrl = mcBIT(ccpUP_RESTART_CHKSUM) | mcBIT(ccpUP_ROMCODE_CHKSUM);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not ready\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not OK\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_OK;
    }
#endif

//#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368)
    // SRAM clear
    ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcDELAY_MS(10);
#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368) 
    // set ad clock phase for more margin case
    DVBC_GetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);
    mcCLR_BIT(ucUpCtrl, 5);
    DVBC_SetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1))
        return cUP_LOAD_ERR_I2C;
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
    {
        return cUP_LOAD_ERR_HW_RDY;
    }
#endif

    ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    ucValue = 0;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);

    for (ii = 0; ii < 4; ii ++)
    {
        DVBC_GetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
    }

// set max code size.
#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    ucValue = 0x9F;
#else
    ucValue = 0x6F;
#endif
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_H, &ucValue, 1);
    ucValue = 0xFF;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_L, &ucValue, 1);

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn off I2C auto-increment
    DVBC_GetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
    mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    //UINT16 u2SramSize;
#if defined(CC_MT5135) || defined(CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    u2SramSize = 110*1024/10;
#else
    u2SramSize = 0;
#endif
    u2CurRamAdr = u2SramSize;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
    // for 40k SRAM, IR_RD is 0x00[7]
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2CurRamAdr);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_L, &ucValue, 1);

    /*ucI2cAddr = psDemodCtx->I2cAddress;*/
    if (DVBC_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
    {
        mcSHOW_POP_MSG(("Download FW fail! (%d)\n"));
        return 1;
    }
	mcSHOW_POP_MSG(("Download FW code finished!\n"));

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn on I2C auto-increment
    mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

    mcSET_BIT(ucUpCtrl, ccpUP_RESTART_CHKSUM);
    mcCLR_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        return cUP_LOAD_ERR_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        return cUP_LOAD_ERR_CHKSUM_OK;
    }

    ucUpCtrl = mcBIT(ccpUP_ENABLE);          // 50 MHz Clock
    if (!fgClk50)
        ucUpCtrl = mcBIT(ccpUP_ENABLE) | mcBIT(ccpUP_DN_FREQ);    // 25 MHz Clock
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);

    // delay for waiting FW initialization done, may have I2C timeout at first
    for (ii = 0; ii < 5; ii++)
    {
        mcDELAY_MS(3);
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
        if (ucValue == ccFW_RUN_TAG)
            break;
    }

#if defined(CC_MT5135) || defined(CC_MT5135DVBC) //|| defined(CC_MT5396)
    // enable HOST_EN for host to be able to access cmd/rsp register
    ucValue = 0x01;
    DVBC_SetReg(psDemodCtx, DVBC_REG_RFA4A0, &ucValue, 1);
#endif

#if fcLOAD_CODE_TIME
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    u4TickCnt = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" (%3u ms) ", u4TickCnt));
    if (u4TickCnt > 0)
        return -((INT32) u4TickCnt);
    else
#endif

    return cUP_LOAD_OK;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DriverStart
 *  Start demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u1Mon1Sel  Monitor1 driver select. (0: None, 1: Auto-Reacquisition).
 *  @param  u1Mon2Sel  Monitor2 driver select. (0: None, 1: TPS sync, 2: SPS sync auto, 3: SPS sync manual
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DriverStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_REACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_REACQ_START_RSIZE, 1);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DriverStop(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_REACQ_STOP;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_REACQ_STOP_RSIZE, 1);
}


void DVBC_SetIF(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   ucPara[ccCMD_REG_NUM];
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

    // set agc parameters
    ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA1;
    ucPara[1] = pEuTunerCtx->m_aucPara[7];   // IF taget level, reserved
    ucPara[2] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
    ucPara[3] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
    vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA1_SIZE+1);
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 ucI2cAddress, BOOL fgLoadCode)
{
    //UCHAR   ucPara[ccCMD_REG_NUM];
    //UCHAR   ii;
    UCHAR   aucData[cMAX_READ_NUM + 1];
    //SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);
    UINT8 *pCodeImage=NULL;
    UINT16 u2CodeImageLen;
	#if (ATD_NOT_USE==TRUE)
    UCHAR ucData = 0x00;
	#endif

    psDemodCtx->u2ErrorPkt = 0;
    psDemodCtx->u2TotalPkt = 0;
    psDemodCtx->u4CorrBit = 0;
    psDemodCtx->ucEnFpga = FALSE;
    psDemodCtx->I2cAddress = ucI2cAddress;
    psDemodCtx->ucConnType = PORT_CFG_MTK;
    psDemodCtx->ucTsIfFormat = 0x8C;   //changed from 0x0C to 0x8C due to MT5360B, and no impact to MT539x and MT5360A
    psDemodCtx->ucTsIfCtrl = 0x00;
    psDemodCtx->ucTsIfRate = 0x07;
    psDemodCtx->ucSramSize = 32;
    psDemodCtx->m_TunerMode = 0;
    psDemodCtx->i4BerTh = 430;   // JC, 090202
    psDemodCtx->u2SymbolrateMax = ESTIMATE_SYMBOL_ACT_MAX;
	psDemodCtx->u2SymbolrateMin = ESTIMATE_SYMBOL_ACT_MIN;

//rest SLD before load FW.lei110805
#if defined(CC_MT5396)|| defined(CC_MT5368)
#if (ATD_NOT_USE==TRUE)

    //UCHAR ucData = 0x00;
    //Reset SLD REG 
	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);

	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);

	
#endif
#endif

    if (fgLoadCode)
    {
		#if defined(CC_MT5135) || defined(CC_MT5135DVBC)
			if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
	        {
	            pCodeImage=ucDvbtCodeImage;
	            u2CodeImageLen=sizeof(ucDvbtCodeImage);
	        }
	        else//legacy tuner
	        {
	             pCodeImage=ucDvbtCodeImageLeg;
	             u2CodeImageLen=sizeof(ucDvbtCodeImageLeg);
	        }
		#else
			if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
	        {
	            pCodeImage=ucDvbcCodeImage;
	            u2CodeImageLen=sizeof(ucDvbcCodeImage);
	        }
	        else//legacy tuner
	        {
	             pCodeImage=ucDvbcCodeImageLeg;
	             u2CodeImageLen=sizeof(ucDvbcCodeImageLeg);
	        }
		#endif

        if (s4LoadCode(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
        {
            return (DRVAPI_TUNER_ERROR);
        }

        // Set TS IF
        DVBC_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);

        // AGC crrve setting
        //ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA0;
        //for (ii = 0; ii < 4; ii++)
        //    ucPara[ii+1] = pEuTunerCtx->m_aucPara[ii+2];  // write m_aucPara[2-5]
        //vSetIntfPara(psDemodCtx, ucPara, ccOFST_DVBC_SET_TUNER_PARA0_SIZE+1);

        // set agc parameters
       DVBC_SetIF(psDemodCtx);  /*wenming 12-2-27: for MTFE, this may need a define to mark...*/
        /*
        ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA1;
        ucPara[1] = pEuTunerCtx->m_aucPara[7];   // IF taget level, reserved
        ucPara[2] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
        ucPara[3] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
        vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA1_SIZE+1);
        */
        // set to blind search mode
        //vSetChSearchMode(psDemodCtx); /* Zhang Chi mark, and will be added in the later time */

        // Ken, GetVersion for previous host command complete, 090114
        mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
        {
            mcSHOW_USER_MSG(("DVBC_Initialize OK!\n"));
        }
    }     

    DVBC_ChipInit(psDemodCtx);

    return (DRVAPI_TUNER_OK);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetTotPkt(DEMOD_CTX_T *psDemodCtx)  
{
    return (UINT32)psDemodCtx->u2TotalPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetErrPkt(DEMOD_CTX_T *psDemodCtx)  
{
    return (UINT32)psDemodCtx->u2ErrorPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPER
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPER(DEMOD_CTX_T *psDemodCtx)  // return value = PER*10^5
{
    INT32 RSTotPkt, RSErrPkt;
        
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;

    if (RSTotPkt <= 0)
        return -1;
    else
        return (INT32) ((RSErrPkt * 100000) / RSTotPkt);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetTsLock(DEMOD_CTX_T *psDemodCtx)
{
    if (psDemodCtx->ucRSHState)
        psDemodCtx->fgTsLock = TRUE;
    else
        psDemodCtx->fgTsLock = FALSE;

    return psDemodCtx->fgTsLock;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSignalLevel
 *  Get signal level from 0 (lowest) to 100 (highest).
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) 0~100.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetSignalLevel(DEMOD_CTX_T *psDemodCtx)
{
    INT8 s1AgcMax;
    INT8 s1AgcMin;
    INT16 s2SignalStrength;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

      
    //s1AgcMax = (INT8)psDemodCtx->sTunerCtx.m_aucPara[4];
    //s1AgcMin = (INT8)psDemodCtx->sTunerCtx.m_aucPara[5];
    s1AgcMax = (INT8)pEuTunerCtx->m_ifAgc_lvl_max;
    s1AgcMin = (INT8)pEuTunerCtx->m_ifAgc_lvl_min;

    if (s1AgcMax != s1AgcMin)
#if defined(CC_MT5135) || defined(CC_MT5396)|| defined(CC_MT5368) //EC_code_dB3330: For MT5396DVBTC demod
        s2SignalStrength = ((INT16)(psDemodCtx->ucSldPraGainIdx-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
#else
        s2SignalStrength = ((INT16)(psDemodCtx->s1IfAgc-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
#endif
    else
        s2SignalStrength = 0;

    if ( ((INT8)pEuTunerCtx->m_aucPara[2]) > 0)
        s2SignalStrength = 100 - s2SignalStrength;

    //if (psDemodCtx->fgTsLock == TRUE)
    {
        if      (s2SignalStrength < 0)
            return 0;
        else if (s2SignalStrength > 100)
            return 100;
        else
            return (UINT8)s2SignalStrength;
    }
    //return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) SNR in dB.   
 */
//-----------------------------------------------------------------------------
UINT16 DVBC_GetSignalSNR(DEMOD_CTX_T *psDemodCtx)
{
    INT16 s2_data;

    if (psDemodCtx->fgTsLock == TRUE)
    {
//        s2_data = (psDemodCtx->s2Snr) / 256;
        s2_data = (psDemodCtx->s2Snr) / 25; // return 0.1 dB
        if (s2_data < 0)
            return 0;
        else
            return  s2_data;
    }
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  vSetIntfPara
 *  Set host command interface API.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  para  Pointer of parameters buffer. (para[0] : CmdId, para[1~(len-1)] : parameters).
 *  @param  len  Length of parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
static void vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   ii;

    if(len>(cMAX_READ_NUM +1))/*Prevent aucData Buffer Overflow*/
    {
        return;
    }
    mccSET_CMD2(aucData) = para[0];
    if (len > 1)
    {
        for (ii = 0; ii < (len-1); ii++)
            mccSET_QUEUE2(aucData, ii) = para[ii+1];
    }
    
    mcHOST_CMD(psDemodCtx, aucData, 1, len);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetTsIf
 *  Set TS interface parameters.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  tsif_format  TS interface format.
 *  TsIfFormat:
 *  b7  Reserve
 *  b6  VALID   Active polarity selection of tsif_val
 *      0 -> tsif_val is high active
 *      1 -> tsif_val is low active
 *  b5  ERR Active polarity selection of tsif_err
 *      0 -> tsif_err is high active
 *      1 -> tsif_err is low active
 *  b4  SYNC    Active polarity selection of tsif_sync
 *      0 -> tsif_sync is high active
 *      1 -> tsif_sync is low active
 *  b3  TEI Transport stream error indicator
 *      0 -> TEI bit will not be changed by tsif
 *      1 -> TEI bit will automatic set by tsif 
 *  b2  SP  Parrallel / Serial mode selection
 *      0 -> Serial mode
 *      1 -> Parralle mode
 *  b1  CLK_GATED   Output clock gated selection of tsif_clk
 *      0 -> tsif_clk is gated
 *      1 -> tsif_clk is non-gated
 *  b0  CLK_INV Output clock edge selection of tsif_clk
 *      0 -> tsif_data is valid at positive edge clock
 *      1 -> tsif_data is valid at negative edge clock
 *  @param  tsif_ctrl  TS interface control.
 *  TsIfControl:
 *  b7  Reserve
 *  b6  FRACTIONAL_PKT  Allow MPEG fractional packet generated
 *      0 -> Output packet is always 188 bytes
 *      1 -> Output packet may not be 188 bytes when channel changed
 *  b5  SERIAL_SYNC  Control the width of TSIF_SYNC signal. Only valid in serial mode
 *      0 -> SYNC duration = first 8 bit
 *      1 -> SYNC duration = first 1 bit
 *  b4  DISABLE_HDR  Disable output header 'h47
 *      0 -> Normal
 *      1 -> Disable header 'h47 in TS
 *  b3  DATA_FLIP  Control data flip of TSIF_DATA[7:0]
 *      0 -> Normal
 *      1 -> Data flip of TSIF_DATA[7:0]
 *  b2  ERR_DROP_PKT  Drop packet if error packet occur
 *      0 -> Normal
 *      1 -> Drop packet if TSIF_ERR = 1'b1
 *  b1  ERR_SHORT  Define TSIF_ERR duration
 *      0 -> Normal, TSIF_ERR duration is 188 bytes
 *      1 -> TSIF_ERR duration is 1 byte
 *  b0  ERR_GATED  Define TSIF_ERR gated
 *      0 -> TSIF_ERR is gated by data enable
 *      1 -> TSIF_ERR is not gated between packet
 *  @param  tsif_rate  TS interface clock rate. Output data rate = (48MHz/(TSIF_RATE+1)). The value should be odd number.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate)
{
}

//----------------------------------------------------------------------------- 
/** DVBC_SetTsOutputMode
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel)
{
    UCHAR ucData;

    DVBC_GetDemodReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
    mcSHOW_DBG_MSG(("DVBT_SetTsOutputMode: fgParallel = %d\n", fgParallel));
    mcSHOW_DBG_MSG(("DVBT_SetTsOutputMode: read 0xC38 = 0x%02X\n", ucData));

    if (fgParallel)
        ucData |= 0x04;
    else
        ucData &= 0xfB;

    mcSHOW_DBG_MSG(("DVBC_SetTsOutputMode: write 0xC38 = 0x%02X\n", ucData));
    DVBC_SetDemodReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);

    //Wait TS mode change
    mcDELAY_MS(1);
}

//----------------------------------------------------------------------------- 
/** DVBC_SetQTREarlyONOFF
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetQTREarlyONOFF(DEMOD_CTX_T *psDemodCtx, BOOL fgQTREarly)
{
	UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_QTRER_EN;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_QTRER_ENABLE) = fgQTREarly;

    if(mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_QTRER_RSIZE, ccOFST_DVBC_SET_QTRER_TSIZE+1)== FALSE)
    {
        mcSHOW_USER_MSG(("DVBC Set QTR Earlybreak enable flag!\n"));
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_SetTsPathMode
 *  Set TS path mode
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable)
{
#if defined(CC_MT5135) || defined (CC_MT5135DVBC)
	UINT32 u4Temp;

    SPI_SDARead(&u4Temp, 0x0001);
    if (fgEnable)
        u4Temp |= 0x000C0000;
    else
        u4Temp &= 0xFFF3FFFF;
    SPI_SDAWrite(&u4Temp, 0x0001);
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPostVBer(DEMOD_CTX_T *psDemodCtx) // return value = PostVBer * 10^5
{
    INT32 RSTotPkt, RSErrPkt, RSCorrBit;

    if (!psDemodCtx)
        return -1;

    // PostVBer = (RSCorBit + RSErrPkt * ERR_BITS_PER_ERR_PKT) / (RSTotPkt *188*8)
    // 66 = 10^5 / (188*8)
    
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;
    RSCorrBit = (INT32)psDemodCtx->u4CorrBit;
    
    if (RSTotPkt <= 0)
        return (-1);
    else
        return ( (RSCorrBit + RSErrPkt*ERR_BITS_PER_ERR_PKT) * 66 / RSTotPkt );
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetConnType
 *  Set connection type.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  type  Connection type. (Default is 0).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DVBC_SetConnType(DEMOD_CTX_T *psDemodCtx, UINT8 type)
{
    psDemodCtx->ucConnType = type;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetPowerMode
 *  Set power mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucMode  Unused.
 *  @retval   0. 
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_SetPowerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode)
{
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DumpRegisters
 *  Dump Demod registers.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  bank_no  Register bank number.
 *  @param  max_i2c_len  Max I2C read byte at one time
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len)
{
UINT16  u2CurI2cLen, u2CurAddr, u2StartAddr = 0;
UINT16  u2CurLen = 256;

    if (!psDemodCtx)
        return 1;
    if (!buffer)
        return 1;
    if (bank_no > 2)
        return 1;

    u2CurAddr = 0;
    u2StartAddr = (UINT16) bank_no << 8;
    while (u2CurLen)
    {
        u2CurI2cLen = u2CurLen;
        if (u2CurI2cLen > max_i2c_len)
            u2CurI2cLen = max_i2c_len;

        if (DVBC_GetReg(psDemodCtx, u2StartAddr + u2CurAddr,
                                   buffer + u2CurAddr, u2CurI2cLen))
        {
            return 1;
        }
        u2CurAddr += u2CurI2cLen;
        u2CurLen  -= u2CurI2cLen;
    }

    return 0;
}


//----------------------------------------------------------------------------- 
/** DVBC_SetQamMode
 *  Set QAM Mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  Mode  QAM mode selection.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetQamMode(DEMOD_CTX_T *psDemodCtx, UINT8 Mode)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_QAM;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_QAM_MODE) = Mode;

    if(mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_QAM_RSIZE, ccOFST_DVBC_SET_QAM_TSIZE+1)== FALSE)
    {
        mcSHOW_USER_MSG(("DVBC Set Qam Mode Fail!\n"));
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_SetSymbolRate
 *  Set Symbol Rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  Rate  symbol rate in kSym/sec.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetSymbolRate(DEMOD_CTX_T *psDemodCtx, UINT16 Rate)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_SYMBOL_RATE;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_SYMBOL_RATE_H) = mcHIGH_BYTE(Rate);
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_SYMBOL_RATE_L) =  mcLOW_BYTE(Rate);

    if(mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_SYMBOL_RATE_RSIZE, ccOFST_DVBC_SET_SYMBOL_RATE_TSIZE+1) == FALSE)
    {
        mcSHOW_USER_MSG(("DVBC Set Symbol Rate Fail!\n"));
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_AcqStart
 *  Start Acquistion.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_AcqStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_ACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_ACQ_START_RSIZE, 1);
}

void DVBC_AdvAQM(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvAQM)
{
#if 1
    UCHAR   aucData[cMAX_READ_NUM + 1];

    /* bfgAdvAQM=TRUE, do AdvAQM. */
    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_ADV_AQM;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_ADV_AQM_ENABLE) = (UINT8) bfgAdvAQM;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_ADV_AQM_RSIZE, ccOFST_DVBC_SET_ADV_AQM_TSIZE+1);
#endif
}

void DVBC_AdvASR(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvASR)
{
#if 1
    UCHAR   aucData[cMAX_READ_NUM + 1];

    /* bfgAdvASR=TRUE, do AdvASR. */
    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_ADV_ASR;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_ADV_ASR_ENABLE) = (UINT8) bfgAdvASR;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_ADV_ASR_RSIZE, ccOFST_DVBC_SET_ADV_ASR_TSIZE+1);
#endif
}

//----------------------------------------------------------------------------- 
/** DVBC_TunerI2c
 *  Switch Tuner I2C.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  fgSwitchOn  Switch ON/OFF of tuner I2C
 *  @retval   S16.
 */
//-----------------------------------------------------------------------------
S16 DVBC_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn)
{
    UCHAR   aucData[cMAX_READ_NUM + 1]; 
    U8      status;

    if (!psDemodCtx)
        return -2;

    if (fgSwitchOn)
    {
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 1;
        if (psDemodCtx->ucEnFpga)
            mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x04;
        else
            mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0x50; 
    }
    else 
    {
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 0;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0x50;
    }

    status = mcHOST_CMD(psDemodCtx, aucData, ccOFST_TUNER_I2C_ONOFF_RSIZE, ccOFST_TUNER_I2C_ONOFF_SIZE+1);
    if (status)
    {
        mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
            return 0;
        else
            return -1;
    }
    else
        return -1;
}

//----------------------------------------------------------------------------- 
/** DVBC_UpdateStatus
 *  Get Status.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateStatus(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ucData[2];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_STATUS0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_STATUS0_SIZE, 1))
    {
        ucData[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS00);

        psDemodCtx->ucPraLock     = mcGET_FIELD(ucData[0], 0x03, 0);
        psDemodCtx->ucQNIRState   = mcGET_FIELD(ucData[0], 0x04, 2);
        psDemodCtx->ucQTRState    = mcGET_FIELD(ucData[0], 0x08, 3);
        psDemodCtx->ucQCRFftState = mcGET_FIELD(ucData[0], 0x10, 4);
        psDemodCtx->ucQCRState    = mcGET_FIELD(ucData[0], 0x20, 5);
        psDemodCtx->ucRSHState    = mcGET_FIELD(ucData[0], 0x40, 6);

        psDemodCtx->ucQCNFailState = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_QCN_FAIL_CNT);
        psDemodCtx->ucASRFailState = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_ASR_FAIL_CNT);
        psDemodCtx->ucReAcqCnt     = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_REACQ_CNT);
    }
}


//----------------------------------------------------------------------------- 
/** DVBC_UpdateSymbol
 *  Get demod information and update demod symbol rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateSymbol(DEMOD_CTX_T *psDemodCtx)

{   
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   uc_data[5];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO1_SIZE, 1))
    {   
    
	    psDemodCtx->ucQamMode = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_QAM_MODE);
	   
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_H);
        psDemodCtx->u2SymbolRate = mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		mcSHOW_DBG_MSG3(("[MS DEBUG]ccCMD_ID_DVBC_GET_INFO1 symbole rate is = %d\n", psDemodCtx->u2SymbolRate));   

        psDemodCtx->fgAsrReady = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_RDY);
    }	
}



//----------------------------------------------------------------------------- 
/** DVBC_UpdateInfo
 *  Get Info.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateInfo(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   uc_data[5];
    U32 u4temp=0;

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO0_SIZE, 1))
    {
        //corrected error bits
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT2);
        psDemodCtx->u4CorrBit = uc_data[0];
        psDemodCtx->u4CorrBit += (U32)mcUNSIGN_2BYTE(uc_data[2], uc_data[1], 7, 8) * 256;
        //total error packets
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT1);
        psDemodCtx->u2ErrorPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8); 
        //total packets
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT1);
        psDemodCtx->u2TotalPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO1_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_AGC_GAIN_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_AGC_GAIN_H);
        psDemodCtx->s2AgcGain = mcSIGN_2BYTE(uc_data[1], uc_data[0], 6, 8);   // Gain * 2^13

        psDemodCtx->ucQamMode = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_QAM_MODE);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_H);
        psDemodCtx->u2SymbolRate = mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

        psDemodCtx->fgAsrReady = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_RDY);

        psDemodCtx->ucDsmSel = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_DSM_SEL);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO2;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO2_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET3);

    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO3 = %d\n", uc_data[3]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO2 = %d\n", uc_data[2]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO1 = %d\n", uc_data[1]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO0 = %d\n", uc_data[0]));    //Iven, 20110303    


       // psDemodCtx->s4Cfo = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
       // psDemodCtx->s4Cfo += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 5, 8) * 256 * 256;
        psDemodCtx->s4QCR_CFO = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);    // Iven, 20110303
        psDemodCtx->s4QCR_CFO += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 5, 8) * 256 * 256;   

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR1);
        psDemodCtx->s2Snr = mcSIGN_2BYTE(uc_data[1], uc_data[0], 7, 8);   // SNR * 256

        psDemodCtx->u1Bw = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_QCR_DD_BW);
    }

    

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO4;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO4_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_3);
        uc_data[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_4);

        psDemodCtx->s4TimingOffset = ((S32)uc_data[4]) << 8;
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[3])) <<8;        
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[2])) <<8;                
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[1])) <<8;                
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[0])) ;                        
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO5;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO5_SIZE, 1))
    {
        psDemodCtx->ucASR_CFOflag = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CONFIDENCE);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO3);

        u4temp = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        u4temp += (U32)mcUNSIGN_2BYTE(uc_data[3], uc_data[2], 8, 8) * 256 * 256;

    if(mcCHK_BIT1(psDemodCtx->ucASR_CFOflag, 4))    // CFO_Con = 2, valid
    {
         if(mcCHK_BIT1(psDemodCtx->ucASR_CFOflag, 0))
         {
             psDemodCtx->s4ASR_CFO =(S32)( 0 - u4temp );
         }
         else
         {
             psDemodCtx->s4ASR_CFO = (S32)u4temp;
         }
    }
    else
    {
        psDemodCtx->s4ASR_CFO = 0;
    }

	mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO3;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO3_SIZE, 1))
    {
        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_SYSC);
        psDemodCtx->ucQcnState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCN);
        psDemodCtx->ucQcrReturnRC = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCR_RETURN_RC);
        psDemodCtx->ucFftSearchResult = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QTR_FFT_SEARCH_RESULT);
        psDemodCtx->ucPraState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_PRA);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_BN_MONITOR);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_CARRIER_INVERSE);
		mcSHOW_DBG_MSG3(("b_QCR_Spectrum_inverse = 0x%x\n", uc_data[1]));     
        psDemodCtx->ucBNflag = mcGET_FIELD(uc_data[0], 0x40, 6);
		if (uc_data[1] == 0x01)
		{
			psDemodCtx->s4QCR_CFO = 0 - psDemodCtx->s4QCR_CFO;
			psDemodCtx->s4ASR_CFO = 0 - psDemodCtx->s4ASR_CFO;
		}
				
    }

    mcSHOW_DBG_MSG3(("psDemodCtx->ucASR_CFOflag = 0x%x\n", psDemodCtx->ucASR_CFOflag));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO3 = 0x%x\n", uc_data[3]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO2 = 0x%x\n", uc_data[2]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO1 = 0x%x\n", uc_data[1]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO0 = 0x%x\n", uc_data[0]));    //Iven, 20110303    
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO6;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO6_SIZE, 1))
    {
        psDemodCtx->ucAQM_Seq[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_00);
        psDemodCtx->ucAQM_Seq[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_01);
        psDemodCtx->ucAQM_Seq[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_02);
        psDemodCtx->ucAQM_Seq[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_03);
        psDemodCtx->ucAQM_Seq[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_04);
        psDemodCtx->ucAQM_ID = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_Index);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO7;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO7_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SpecInv_QCN_State);
        psDemodCtx->u1QSpecInv = mcGET_FIELD(uc_data[0], 0x80, 7);
        psDemodCtx->ucQcnState = uc_data[0] & 0x07;

        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SYSC_State);
        psDemodCtx->ucCurrentAcqState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_ucCurAcqState);
        psDemodCtx->ucDev_AWGN_PN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN_PN);
        psDemodCtx->ucDev_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN);
        psDemodCtx->ucCor_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uCor_AWGN);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO8;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO8_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_JTD_00);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_JTD_01);
        psDemodCtx->u2JTD_Result = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_LockTime_00);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_LockTime_01);
        psDemodCtx->u2AcqTime = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO9;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO9_SIZE, 1))
    {
//        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_STATUS_MONITOR);
//        psDemodCtx->ucSldPraState = mcGET_FIELD(uc_data[0], 0x60, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraRfagcCtrlWord = (S16) mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraIfagcCtrlWord = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_PGA_GAIN_INDEX);
        psDemodCtx->ucSldPraGainIdx = mcGET_FIELD(uc_data[0], 0x1f, 0);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFOA;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFOA_SIZE, 1))
    {
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_L);
        psDemodCtx->s2SldPraOut = (S16) mcSIGN_2BYTE(uc_data[1], uc_data[0], 3, 8);
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_GetCFO
 *  Get CFO.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   S32 CFO in kHz * 10^5.
 */
//-----------------------------------------------------------------------------
S32 DVBC_GetCFO(DEMOD_CTX_T *psDemodCtx)
{
    S32 s4Cfo_value;

    DVBC_UpdateInfo(psDemodCtx);// iven, 20110303
//    s4Cfo_value = psDemodCtx->s4Cfo * 10;   // (reg/2^29*54*1000) * 10^5

    psDemodCtx->s4Cfo =  psDemodCtx->s4QCR_CFO* 10 + psDemodCtx->s4ASR_CFO*10;   // (reg/2^29*54*1000) * 10^5
    s4Cfo_value = psDemodCtx->s4Cfo;
    mcSHOW_DBG_MSG3(("DVBC_GetCFO(s)_QCR_CFO = %dkhz\n", psDemodCtx->s4QCR_CFO/10000));    //Iven, 20110303    
    mcSHOW_DBG_MSG3(("DVBC_GetCFO(s)_ASR_CFO = %dkhz\n", psDemodCtx->s4ASR_CFO/10000));    //Iven, 20110303    
    mcSHOW_DBG_MSG3(("DVBC_GetCFO(s)_s4Cfo = %dkhz\n", psDemodCtx->s4Cfo/100000));    //Iven, 20110303

    return s4Cfo_value;
}

//----------------------------------------------------------------------------- 
/** DVBC_GetTFO
 *  Get TFO.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   UINT32 TFO in ppm.
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetTFO(DEMOD_CTX_T *psDemodCtx)
{
#if 1
    // Hui mark for Linux build error. 20100402
    INT32  s4TFO;

    s4TFO = ((((( (psDemodCtx->s4TimingOffset) * 37) >> 18) * ((S32)psDemodCtx->u2SymbolRate)) << (psDemodCtx->ucDsmSel)) >> 11);


    return s4TFO;
#else
    return 0;
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DVBC_GetTsFmt(DEMOD_CTX_T *psDemodCtx)
{
UCHAR    ucData, ucClk;
TS_FMT_T fgTsFmt;

#if defined(CC_MT5135) || defined (CC_MT5135DVBC)
    UINT32   u4Temp;
#endif
    
    DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);

    if (ucData & 0x04)
        fgTsFmt.SerOrPar = SP_PARALLEL;
    else
        fgTsFmt.SerOrPar = SP_SERIAL;

    if (ucData & 0x02)
        fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;
    else
        fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;

    if (ucData & 0x01)
        fgTsFmt.fgGatedClock = BOOL_FALSE;
    else
        fgTsFmt.fgGatedClock = BOOL_TRUE;

#if defined(CC_MT5135) || defined (CC_MT5135DVBC)
    SPI_SDARead(&u4Temp, 0x0004);
    if (u4Temp & 0x00080000)
        fgTsFmt.fgTristate = BOOL_FALSE;
    else
        fgTsFmt.fgTristate = BOOL_TRUE;
#elif defined(CC_MT5396) || defined (CC_MT5368)//for MT5396,lei_code_110513;
	fgTsFmt.fgTristate = BOOL_FALSE;
#else   
	fgTsFmt.fgTristate = BOOL_FALSE;
#endif
    DVBC_GetReg(psDemodCtx, 0xC39, &ucClk, 1);
    if (ucData & 0x04)
    {
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 54000/2/(ucClk+1);
    }
    else
    {
        /* Serial */
        if (ucClk >= 0x07)
        {
            fgTsFmt.u4TsClockInKHz = 54000/2/(ucClk+1);
        }
        else if (0x05 == ucClk)
        {
            fgTsFmt.u4TsClockInKHz = 36000;
        }
        else
        {
            fgTsFmt.u4TsClockInKHz = 54000;
        }
    }

    /* In-house DTD demod,  fgExternalDemod is BOOL_FALSE! */
    fgTsFmt.fgExternalDemod = BOOL_FALSE;

    return fgTsFmt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSQI
 *  calculate Signal Quality Indicator.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetSQI(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1SignalSNR = 0;
    UINT8 u1SignalSQI = 0;

    if(DVBC_GetTsLock(psDemodCtx) == TRUE)
    {
        u1SignalSNR = (UINT8)(DVBC_GetSignalSNR(psDemodCtx)/10);
    }
    else
    {
        u1SignalSNR = 0;
    }

    if((u1SignalSNR>0) && (u1SignalSNR<=36))
    {
        u1SignalSQI = u1SignalSNR*25/9;
    }
    else if(u1SignalSNR>36)
    {
        u1SignalSQI = 100;
    }
    else
    {
        u1SignalSQI = 0;
    }
    return u1SignalSQI;
}

/***********************************************************************/
CHAR *DVBC_GetVer(DEMOD_CTX_T *psDemodCtx)
{
   CHAR * cString1;
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = TRUE;
#endif
    vDVBC_GetAsicVer(psDemodCtx);
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = FALSE;
#endif
    ITuner_OP(psDemodCtx->psTunerCtx,itGetVer,0,&cString1);
    mcSHOW_USER_MSG(("%s\n", cString1));

    //return szGetDrvVer();
    return NULL;
}
/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
VOID vDVBC_GetAsicVer(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    #if defined(CC_MT5135)|| defined(CC_MT5135DVBC)
    DVBC_GetDemodReg(psDemodCtx, MT5135_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("MT5135 Chip ID: %02x\n", aucData[0]));
	#endif


    DVBC_GetDemodReg(psDemodCtx, DVBC_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("DVBC Chip ID: %02x\n", aucData[0]));


    mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
    {
        mcSHOW_USER_MSG(("FW build ver: %02X%02X-%02X%02X\n",
                         mccGET_QUEUE(aucData, ccOFST_VER_YEAR_MONTH), mccGET_QUEUE(aucData, ccOFST_VER_DATE),
                         mccGET_QUEUE(aucData, ccOFST_VER_HOUR), mccGET_QUEUE(aucData, ccOFST_VER_MINUTE)));
    }
	//Mingsheng 110802; add it for FW version control;
	mccSET_CMD2(aucData) = ccCMD_ID_GET_LABEL;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_LABEL_SIZE, 1))
    {
        mcSHOW_USER_MSG(("FW update ver: %c%02X%02X\n",
                         mccGET_QUEUE(aucData, ccOFST_LABEL_BRANCH_TYPE), mccGET_QUEUE(aucData, ccOFST_LABEL_ID1),
                         mccGET_QUEUE(aucData, ccOFST_LABEL_ID2)));
    }

}

BOOL DVBC_CheckLocked(DEMOD_CTX_T *psDemodCtx)
{

	INT32 s4Freq;

    /* If ASR_CFO<+-500kHz, Locked. */ 
    DVBC_UpdateInfo(psDemodCtx);

    s4Freq = (INT32)(psDemodCtx->s4ASR_CFO)*27/(1024*16)*1000/4/4096;

    mcSHOW_USER_MSG(("DVBC_CheckLocked(s)_u4Freq = %dkhz\n", s4Freq));            
    

    if((s4Freq<501) && (s4Freq>-501))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

VOID DVBC_SetTsOutput(DEMOD_CTX_T *psDemodCtx, BOOL fgOutputEnable)
{
    UCHAR    aucData[cMAX_READ_NUM + 1];

#if defined(CC_MT5396) // Mingsheng 0929; Set TS Pad output mode for TS high clk issue.
	UINT32   u4Temp;

    if (fgOutputEnable) //// Enable  ts pad
    {       
            u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	      u4Temp|=0x000f0000;
	      mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);

		u4Temp = mcRISC_Reg(CKGEN_BASE, 0x40c);//mingsheng add
		u4Temp&=0xffffff3f;
		u4Temp|=0x000f0040;
		mcSET_SHOW_REG(CKGEN_BASE, 0x40c, u4Temp);
    }
#endif
    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_TS_OUTPUT;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_TS_OUTPUT_ENABLE) = (UINT8) fgOutputEnable;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_SET_TS_OUTPUT_RSIZE, ccOFST_DVBC_SET_TS_OUTPUT_TSIZE+1);
#if defined(CC_MT5396) // Mingsheng 0929; Set TS Pad input mode for TS high clk issue.
    if (!fgOutputEnable)  // closed ts pad
    {

		u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	      u4Temp|=0x000f0000;
	      mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);

    	      u4Temp = mcRISC_Reg(CKGEN_BASE, 0x40c);//mingsheng add
		u4Temp&=0xffffff3f;
		mcSET_SHOW_REG(CKGEN_BASE, 0x40c, u4Temp);
    }
#endif
}

VOID DVBC_SetDescramblerLock(DEMOD_CTX_T *psDemodCtx, BOOL fgDetect)
{
UINT8 ucData;

    if (fgDetect)
    {
        // Restart Descrambler, always detect
        DVBC_GetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
        mcCLR_BIT(ucData, 4);
        DVBC_SetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
    }
    else
    {
        // lock one time
        DVBC_GetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
        mcSET_BIT(ucData, 4);
        DVBC_SetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
    }
}

void DVBC_SetPGAGainIndex(DEMOD_CTX_T *psDemodCtx, UCHAR ucPGAGainIndex)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_SET_PGAGAININDEX;
    mccSET_QUEUE2(aucData, ccOFST_FAST_SET_PGAGAININDEX) = ucPGAGainIndex;   
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_FAST_SET_PGAGAININDEX_RSIZE, ccOFST_FAST_SET_PGAGAININDEX_TSIZE + 1);
}

// AGC crrve setting

void DVBC_SetIfAgcCurve(DEMOD_CTX_T *psDemodCtx)
{
	UCHAR   ucPara[ccCMD_REG_NUM];
	UCHAR   ii;
	SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

	ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA0;
	for (ii = 0; ii < 4; ii++)
		ucPara[ii+1] = pEuTunerCtx->m_aucPara[ii+2];  // write m_aucPara[2-5]
	vSetIntfPara(psDemodCtx, ucPara, ccOFST_DVBC_SET_TUNER_PARA0_SIZE+1);
	
}

void DVBC_SetBNProtection(DEMOD_CTX_T *psDemodCtx, UINT32 Freq)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
	
#if defined (CC_MT5135) || defined(CC_MT5135DVBC) 
	if ((Freq == 794000) || (Freq == 826000) || (Freq == 858000))
	{
		mcSHOW_DBG_MSG(("----Close BN protection for LTE test-----\n" ));
		
		mccSET_CMD2(aucData) = cCMD_ID_DVBC_SET_BNPORTESTION_EN;
		mccSET_QUEUE2(aucData, cOFST_DVBC_SET_BNPORTESTION_ENABLE) = FALSE;
		
		mcHOST_CMD(psDemodCtx, aucData, cOFST_DVBC_SET_BNPORTESTION_RSIZE, cOFST_DVBC_SET_BNPORTESTION_TSIZE +1);
	}
	else
	{
		mccSET_CMD2(aucData) = cCMD_ID_DVBC_SET_BNPORTESTION_EN;
		mccSET_QUEUE2(aucData, cOFST_DVBC_SET_BNPORTESTION_ENABLE) = TRUE;

		mcHOST_CMD(psDemodCtx, aucData, cOFST_DVBC_SET_BNPORTESTION_RSIZE, cOFST_DVBC_SET_BNPORTESTION_TSIZE +1);
	}
#endif
	
}


#if defined (CC_MT5135) || defined (CC_MT5135DVBC)
#if 1
//move nim.c and nim.o to the function,lei120726 for ESD issue;
INT32 DVBC_Recover(DEMOD_CTX_T *psDemodCtx)
{
    UINT8   ucData1;
	ITuner_OP(psDemodCtx->psTunerCtx, resettuner, 0, &ucData1);
	if (DVBC_Initialize(psDemodCtx, DEFAULT_I2C_ADDR, TRUE))
	{
        mcSHOW_DBG_MSG(("DVBT_Initialize failed\n"));
	}
	DVBC_SetTsPathMode(psDemodCtx, FALSE);
	DVBC_DisConnect(psDemodCtx);	    
    return (DRVAPI_TUNER_OK);
}
#endif
#endif



