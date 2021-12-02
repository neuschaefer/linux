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
 * $RCSfile: pi_demod_dvbt.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod.c
 *  DvbT demod related API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_8g80_dtmb.h"
#include "pi_demod.h"
#include "com_defs.h" 
//#include "pi_tuner.h"  // marked by liuqu,20090624
#include "pd_common.h"
#include "code_img_MT5360.h"
#include "PD_Def_dbg.h"  /*need include RISC IF macro, if re-ware RISC IF , remove this*/
#include"x_hal_5381.h"
#include "ctrl_bus.h"  //add by liuqu,20090429

#if (!defined(CC_MT5365)) && (!defined(CC_MT5395))
#include "drvcust_if.h"   
#include "x_gpio.h"

#define GPIO_OUTPUT_ON     1
#define GPIO_OUTPUT_OFF    0
#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MT5391_DVBT_SW_VER      "1.000"

//#define fcADD_REG_RESET         1
//#define fcCHK_TUNER_PLL         0
#define fcUP_50M_CLK            1
//#define fcPLL_POLLING_TIME      10          //  in ms
#define fcMEASURE_DRIVER_TIME   0
#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)||1

#define cMAX_TRY_CNT            2
#define cMAX_WAIT_LOOP          10
//#define cWAIT_TIME_INTERVAL     200         // ms
#define cWAIT_TIME_INTERVAL     10         // ms

//decided by hardware
#define cLGS8G8X_ADDRESS1  0x3A 
#define cLGS8G8X_ADDRESS2  0x3E 

#define SUPPORT_DISABLE_DEMOD_CLOCK 0
//-----------------------------------------------------------------------------
// RISC IF Related Implementation
//-----------------------------------------------------------------------------
#define cRISC_DVBT_BASE          (IO_VIRT + 0x2B000)
#define cRISC_ATD_BASE           (IO_VIRT + 0x2B000)

volatile UINT32 *pu4DVBT_Reg = (volatile UINT32 *) cRISC_DVBT_BASE;
STATIC PARAM_RISC_T sRiscDvbtCtx;  //add by liuqu,20090429


#define DVBT_RISC_IF            1
#define fcRISCIF_DVBT_DIRECT_RW 1

#define mcSET_RISCIF_ACT(enb)                                         \
{                                                                   \
    mcSET_BIT(u4RegSetting, cpRISCIF_CTRL_##enb##_EN);                \
    mcRISC_Reg(pu4DVBT_Reg, MT5391_REG_RISCIF_CTRL) = u4RegSetting;    \
}\


#define mcRISCIF_CTRL_MASK(u2CurByteCount, u1ByteOfst)    \
    mcRISC_BYTE_MASK(u2CurByteCount, u1ByteOfst, cpRISCIF_CTRL_B0_EN)\
    

#define mcSET_RISCIF_CTRL(enb)                                            \
{                                                                       \
/*    mcSET_BIT(u4RegSetting, u1ByteOfst + cpRISC_CTRL_B0_EN); */       \
    mcSET_MASK(u4RegSetting, mcRISCIF_CTRL_MASK(u2CurByteCount, u1ByteOfst)); \
    mcSET_RISCIF_ACT(enb);                                                \
}\


#define mcRISCIF_RD_OK(u4RegSetting, stat)    \
    (mcGET_FIELD_CMB(u4RegSetting, RISCIF_STAT_##stat) == cRISCIF_STAT_##stat##_ATTN)
#define mcRISCIF_WR_OK(u4RegSetting, stat)    \
    (mcGET_FIELD_CMB(u4RegSetting, RISCIF_STAT_##stat) == cRISCIF_STAT_##stat##_RDY)

#define cMAX_WAIT_LOOP_RISCIF   6

#define mcCHK_RISCIF_STS(ChkAct, stat)                                    \
{                                                                       \
    for (ii = 0; ii < cMAX_WAIT_LOOP_RISCIF; ii ++)                     \
    {                                                                   \
        u4RegSetting = mcRISC_Reg(pu4DVBT_Reg, MT5391_REG_RISCIF_STAT); \
        if (mcRISCIF_##ChkAct##_OK(u4RegSetting, stat))                   \
            break;                                                      \
        mcDELAY_MS(cWAIT_TIME_INTERVAL);                                \
    }                                                                   \
    if (!mcRISCIF_##ChkAct##_OK(u4RegSetting, stat))                      \
    {                                   /* Read / Write Fail */         \
        return 1;/*DVBT return 1 for fail case*/                                                       \
    }                                                                   \
}\


#define mcRISCIF_DATA_READ(pu1Buffer)                                 \
{                                                                   \
    u4RegValue = mcRISC_Reg(pu4DTD_Reg, MT5391_REG_RISCIF_RDATA);     \
    mcRISC_ENDIAN_READ(pu1Buffer, u1ByteOfst, u2CurByteCount);      \
}\


#define mcRISCIF_DATA_WRITE(pu1Buffer, u1ByteOfst)                    \
{                                                                   \
    mcRISC_ENDIAN_WRITE(pu1Buffer, u1ByteOfst, u2CurByteCount);     \
    mcRISC_Reg(pu4DVBT_Reg, MT5391_REG_RISCIF_WDATA) = u4RegValue;     \
}\


#define mcRISCIF_DATA_WRITE_ALGN(pu1Buffer)   \
    (mcRISC_Reg(pu4DVBT_Reg, MT5391_REG_RISCIF_WDATA) = *((UINT32 *) (pu1Buffer)))


#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\

extern BOOL fgBreak;

static UINT16 LastErrorPkt = 0;
static UINT16 ErrorPkt = 0;


UINT8	Reg_3A_07;
UINT8	Reg_3A_19;
UINT8	Reg_3A_1C;
UINT8	Reg_3A_08;
UINT8	Reg_3A_09;
UINT8	Reg_3A_0A;
UINT8	Reg_3A_0B;
UINT8	Reg_3A_10;
UINT8	Reg_3A_11;
UINT8	Reg_3A_12;

UINT8	Reg_3A_15;
UINT8	Reg_3A_16;
UINT8	Reg_3A_17;

UINT8	Reg_3A_4B;
UINT8	Reg_3A_4C;
UINT8	Reg_3A_4D;
UINT8	Reg_3A_7E;
UINT8	Reg_3A_7F;

UINT8	Reg_3E_0A_B0;
UINT8	Reg_3E_0A_B1;
UINT8	Reg_3E_0A_B3;
UINT8	Reg_3E_0A_B2;
UINT8	Reg_3E_0A_B4;
UINT8	Reg_3E_0A_B5;
UINT8	Reg_3E_0A_B7;
UINT8	Reg_3E_0A_B6;
UINT8	Reg_3E_0A_B9;
UINT8	Reg_3E_0A_B8;
UINT8	Reg_3E_0A_BB;
UINT8	Reg_3E_0A_BA;

UINT8	Reg_3E_0C_22;
UINT8	Reg_3E_0C_42;
UINT8	Reg_3E_0C_4C;
UINT8	Reg_3E_0C_24;
UINT8	Reg_3E_0C_44;
UINT8	Reg_3E_0C_4E;
UINT8	Reg_3E_0C_26;
UINT8	Reg_3E_0C_46;
UINT8	Reg_3E_0C_50;
UINT8	Reg_3E_0C_48;
UINT8	Reg_3E_0C_52;
UINT8	Reg_3E_0C_20;
UINT8	Reg_3E_0C_2A;
UINT8	Reg_3E_0C_2C;
UINT8	Reg_3E_0C_2E;
UINT8	Reg_3E_0C_30;
UINT8	Reg_3E_0C_1A;
UINT8	Reg_3E_0C_1C;
UINT8	Reg_3E_0C_00;
UINT8	Reg_3E_0C_02;

static U8 u1AfterAcq = 0;
static void  vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len);
//static void  vSetChSearchMode(DEMOD_CTX_T *psDemodCtx);
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcADD_HOSTCMD_CHKSUM    0

/***********************************************************************/
/*                  Private (static) Function Prototypes               */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  DVBT_ChipAnaInit
 *  This function initialize analog part registers in the chip for DVB-T.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_ChipAnaInit(VOID)
{ 
    return 0;
}
//----------------------------------------------------------------------------- 
/*
 *  DVBT_ChipInit
 *  This function initialize DVB-T related registers in the chip.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_ChipInit(VOID)
{
UINT32 u4Temp;
// Select Clock Source
// CKGEN_BASE: 0x2000dxxx
// BIM_BASE:   0x20008000
// DVBT_BASE:   0x2002bxxx
// ATD_BASE:  0x2002bxxx

// TVD PLL setting (540MHz, with xtal 27M)  xtal * 20 [13:8]
//w 0x2000d130 0x00099401
    //mcSET_SHOW_REG(CKGEN_BASE, 0x130, 0x00099401);
    
//clock divider setting
//w 0x2000d134[4]=1'b1 //(sel TVD clock)
//w 0x2000d134[6:5]=2'b00 // (sel mux /1)
//w 0x2000d134[11:8]=4'b0100 //(sel TVD/5 as AD input clock)
//w 0x2000d134[12]=1'b1 //(enable clock divider), 0: reset
//w 0x2000d12c[26]=1'b1 //(enable clock into ADC) 
////w 0x2000d134 0x00085410
////w 0x2000d12c 0x0400080a
#if 1
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x134);
    u4Temp &= 0xFFFFE08F;
    u4Temp |= 0x00001410;
    mcSET_SHOW_REG(CKGEN_BASE, 0x134, u4Temp);
    
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x12c);
    u4Temp |= 0x04000000;
    mcSET_SHOW_REG(CKGEN_BASE, 0x12c, u4Temp);
#else
    mcSET_SHOW_REG(CKGEN_BASE, 0x134, 0x00085410);
    mcSET_SHOW_REG(CKGEN_BASE, 0x12c, 0x0400080a);
#endif

//adc setting (clock from PLL)
////w 0x2000d1d0 0x00aa4100  b[7]=0, AD power up b4 clk sel
////w 0x2000d1d4 0x00000054
    mcSET_SHOW_REG(CKGEN_BASE, 0x1d0, 0x00aa4100);   
    mcSET_SHOW_REG(CKGEN_BASE, 0x1d4, 0x00000054);

//54M & 36M clock mux setting
//w 0x2000d21c[1:0]=2'b11   //(54M) 11: from AD, 00: from XTAL
//w 0x2000d21c[17:16]=2'b10 //(48M, for CI)
////w 0x2000d21c 0x00020003
#if 1
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x21c);
    u4Temp &= 0xFFFcFFFc;
    u4Temp |= 0x00020003;
    mcSET_SHOW_REG(CKGEN_BASE, 0x21c, u4Temp);

    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x218);
    u4Temp &= 0xFFFFFFFc;
    u4Temp |= 0x00000003;
    mcSET_SHOW_REG(CKGEN_BASE, 0x218, u4Temp);
#else
    mcSET_SHOW_REG(CKGEN_BASE, 0x21c, 0x00020003);
#endif

// RFAGC, IFAGC pin mux: [29:28] = 00
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
    u4Temp &= 0xcFFFFFFF;
    mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
    
//---------change to dvbt mode----0x93e=0x02
//w 0x2002b4b0 0x00020000
//w 0x2002b4b4 0x4004024f
// write 0x93E (0x24f*4) to 0x00 -> DVBT mode, (0x01 ATD mode)
// indirect write
    mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b0, 0x00020000);
    mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b4, 0x4004024f);

    // set byte mask
    u4Temp = mcRISC_Reg(cRISC_DVBT_BASE, 0x4b4);
    u4Temp |= 0x000f0000;
    mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b4, u4Temp);
    
//---------set risc wait time
    mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4a0, 0x1ec87800);
    
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetReg
 *  This function set demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
//temp5391, static UINT8 DVBT_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
 //   if (u1I2cDemWriteDVBT(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount)) 
   if(ICtrlBus_RiscDemWrite(&sRiscDvbtCtx, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
                       psDemodCtx->I2cAddress, u2RegAddr));

		return 1;
    }

	return 0;
}
UINT8 DTMB_ExDemodSetReg(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
//UINT8 ucData;
  //  if(u1I2cAddr!=0x3E)
    {
		if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, u2RegAddr, pu1Buffer, u2ByteCount))
		{
			mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
						u1I2cAddr, u2RegAddr));
	
			return 1;
		}
    }
  #if 0
	else//section2:u1I2cAddr==0x3E
	{
		if(u2RegAddr==0x84)//page 2
		{   
		    mcSHOW_USER_MSG(("section2 and page2"));
			ucData=0x0C;
			if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, 0xFF, &ucData, u2ByteCount))
			{
				mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
							u1I2cAddr, 0xFF));	
				return 1;
			}
			else
			{
			    if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, u2RegAddr, &ucData, u2ByteCount))
			    {
					mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
								u1I2cAddr, u2RegAddr));	
					return 1;
			    }
			}
		}
		else//page 3
		{
			mcSHOW_USER_MSG(("section2 and page3"));
			ucData=0x0D;
			if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, 0xFF, &ucData, u2ByteCount))
			{
				mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
							u1I2cAddr, 0xFF));	
				return 1;
			}
			else
			{
				if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, u2RegAddr, &ucData, u2ByteCount))
				{
					mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
								u1I2cAddr, u2RegAddr));	
					return 1;
				}
			}
		
		}
	
	}
#endif
	return 0;
}
UINT8 DTMB_ExDemodGetReg(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr,UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
//UINT8 ucData;

   // if(u1I2cAddr!=0x3E)
    {
		if(ICtrlBus_I2cDemodRead(0x100, u1I2cAddr, u2RegAddr, pu1Buffer, u2ByteCount))
		{
			mcSHOW_USER_MSG((" _@IF_IIC Bus Read Error: %02X-%03X!\n",
						u1I2cAddr, u2RegAddr));
	
			return 1;
		}
    }
 #if 0
	else//section2:u1I2cAdd==0x3E
	{
		if(u2RegAddr==0x84)//page 2
		{   
		    mcSHOW_USER_MSG(("section2 and page2"));
			ucData=0x0C;
			if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, 0xFF, &ucData, u2ByteCount))
			{
				mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
							u1I2cAddr, 0xFF));	
				return 1;
			}
			else
			{
			    if(ICtrlBus_I2cDemodRead(0x100, u1I2cAddr, u2RegAddr, &ucData, u2ByteCount))
			    {
					mcSHOW_USER_MSG((" _@IF_IIC Bus Read Error: %02X-%03X!\n",
								u1I2cAddr, u2RegAddr));	
					return 1;
			    }
			}
		}
		else//page 3
		{
			mcSHOW_USER_MSG(("section2 and page3"));
			ucData=0x0D;
			if(ICtrlBus_I2cDemodWrite(0x100, u1I2cAddr, 0xFF, &ucData, u2ByteCount))
			{
				mcSHOW_USER_MSG((" _@IF_IIC Bus Write Error: %02X-%03X!\n",
							u1I2cAddr, 0xFF));	
				return 1;
			}
			else
			{
				if(ICtrlBus_I2cDemodRead(0x100, u1I2cAddr, u2RegAddr, &ucData, u2ByteCount))
				{
					mcSHOW_USER_MSG((" _@IF_IIC Bus Read Error: %02X-%03X!\n",
								u1I2cAddr, u2RegAddr));	
					return 1;
				}
			}
		
		}
	
	}
#endif
	return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetReg
 *  This function get demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
// temp5391, static UINT8 DVBT_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
	
// if (u1I2cDemReadDVBT(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    if(ICtrlBus_RiscDemRead(&sRiscDvbtCtx, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG((" _@IF_IIC Bus Read Error: %02X-%02X!\n",
                       psDemodCtx->I2cAddress, u2RegAddr));
		
		return 1;
    }

	return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_DemodCtxCreate
 *  This function creates demod context and returns the context pointer.
 *  @param  void.
 *  @retval   DEMOD_CTX_T * : pointer of demod context.   
 */
//-----------------------------------------------------------------------------
DEMOD_CTX_T *DVBT_DemodCtxCreate(void)
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
 *  DVBT_DemodCtxDestroy
 *  This function destroys context you have created and releases the memory.
 *  @param  p  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBT_DemodCtxDestroy(DEMOD_CTX_T *p)
{
    DESTROY_OBJECT(p);
}

#define mcSET_REG_PAGE(ucAddr)              \
{                                           \
    ucAddr = mcHIGH_BYTE(u2RegAddr);        \
/*    if (ucPrevPage != ucAddr) */          \
    {                                       \
    /*    ucPrevPage = ucAddr;  */          \
        if (ucI2cWrite(u1I2cAddr, MT5391_REG_PAGE_NUM, &ucAddr, 1))   \
            u1RetSts = 1;                   \
    }                                       \
}\



//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetDemodReg
 *  This function will read demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBT_GetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR	ii = 0;

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
 *  DVBT_SetDemodReg
 *  This function will write demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBT_SetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR	ii = 0;

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



//justin
//----------------------------------------------------------------------------- 
/** download image

 *  @param  
 *  @param  
 *  @retval   others : Error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 ucI2cWriteSpeed(DEMOD_CTX_T *psDemodCtx, UINT8 *pu1Buffer, UINT16 u2CurRomLen){

//#ifdef DVBT_RISC_IF /*RISC IF*/
#if 0

    UINT32  u4RegSetting;
    STATIC  UINT8 u1AlignWarn = 0;
    UINT8   u1AlignChk = 0;
    UINT16  u2CurByteCount;
    UINT32  u4RegValue;/*for mcRISCIF_DATA_WRITE usage*/
    UINT8   ii;

    /*enable download mode*/
    mcCALC_RISC_ADDR_BASE(MT5391_REG_uP_IRData);
    mcSET_BIT(u4RegSetting, cpRISCIF_CTRL_B0_EN);
    mcSET_RISCIF_ACT(DL); 

    mcCALC_RISC_ADDR_CNT(u2CurRomLen, 0);
    if ((u2CurByteCount != cRISC_WORD_LEN) || ((((UINT32) (pu1Buffer)) & mcMASK(cRISC_WORD_WID)) != 0))
    {
        if (!u1AlignWarn)
            mcSHOW_DBG_MSG(("\n ****** RISC_Write not aligned! (%d, 0x%X) ******\n",
                            u2CurByteCount, (UINT32) (pu1Buffer)));
        u1AlignWarn = 1;
        u1AlignChk = 1;
    //    u1RetSts = 1;
    //    break;
    }
   while(u2CurRomLen){
        mcCALC_RISC_ADDR_CNT(u2CurRomLen, 0);

        if (!u1AlignChk)
            mcRISCIF_DATA_WRITE_ALGN(pu1Buffer);
        else
            mcRISCIF_DATA_WRITE(pu1Buffer, 0);

        mcCHK_RISCIF_STS(WR, DSTAT);

        if (u2CurByteCount == 0)
        {
            //u1RetSts = 1;
            //u1RetSts = 0; //ok
            break;
        }
        u2CurRomLen -= u2CurByteCount;
        pu1Buffer += u2CurByteCount;
    }
    mcRISC_Reg(pu4DVBT_Reg, MT5391_REG_RISCIF_CTRL) = 0;   // Disable download mode

    return 0;/*download finish*/

#else

    UINT16  u2CurI2cLen=1;// u2CurRamAdr=0;
    UINT32  ii;
	UINT8   uValue;

    uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU off,MCU reset disable
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x3A, &uValue, u2CurI2cLen);//set address=0
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x3B, &uValue, u2CurI2cLen);

	for(ii=0; ii<u2CurRomLen; ii++)
	{    
	    uValue = (UINT8)ii;
	    if(ICtrlBus_I2cDemodWrite(0x40, cLGS8G8X_ADDRESS1, 0x3A, &uValue, u2CurI2cLen ))
		{
            /*fail case*/
			mcSHOW_DBG_MSG(("ucI2cWriteSpeed Download I2C fail(0x3A)!\n"));
			return 1;
    	}
		
		uValue = (UINT8)(ii>>8);
		if(ICtrlBus_I2cDemodWrite(0x40, cLGS8G8X_ADDRESS1, 0x3B, &uValue, u2CurI2cLen ))
		{
            /*fail case*/
			mcSHOW_DBG_MSG(("ucI2cWriteSpeed Download I2C fail(0x3B)!\n"));
			return 1;
    	}
		if(ICtrlBus_I2cDemodWrite(0x40, cLGS8G8X_ADDRESS1, 0x3C, pu1Buffer + ii, u2CurI2cLen ))
		{
            /*fail case*/
			mcSHOW_DBG_MSG(("ucI2cWriteSpeed Download I2C fail(0x3C)!\n"));
			return 1;
    	}
		else
		{  //success
			//mcSHOW_USER_MSG((" %d value == 0x%02x\n", ii, *(pu1Buffer + ii)));
		}
		//u2CurRamAdr += u2CurI2cLen;
		//u2CurRomLen -= u2CurI2cLen;		
	}

	if(ii==u2CurRomLen)
	{
		mcSHOW_USER_MSG(("load code success @@@@@@@@@@@@@@@@@@@"));
	}
	else
	{
		mcSHOW_USER_MSG(("load code fail ******************"));
	}

    uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU off,MCU reset disable
	uValue=0x02;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU on,MCU reset disable
	uValue=0x03;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, u2CurI2cLen);//MCU on,MCU reset enable




	
    return 0; /*download finish*/
#endif
}
/************************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  fgHostCmdDVBT
 *  Host command interface API between host and demod.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  aucCmdData  Data buffer.
 *  @param  ucReadParacnt  Read parameter byte counts.
 *  @param  ucWriteCmdcnt  Write command byte counts.
 *  @retval   TRUE : Host command success.
 *  @retval   FALSE : Host command fail.   
 */
//-----------------------------------------------------------------------------
//BOOL fgHostCmd(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
//rename to fix build error "API multiply defined" when build with DTD on iDTV
BOOL fgHostCmdDVBT(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
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
/*#if fcCHIP_TYPE
UCHAR   ucChipID;
#endif*/
#if fcADD_HOSTCMD_CHKSUM
UCHAR   ucChkSum;
#endif
UCHAR   jj;

#if fcMEAS_HOST_CMD_TIME
    u4TickStart = mcGET_SYS_TICK();
#endif

    if (DVBT_GetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucValue, 1))
    {
    //    psDemodCtx->u1I2cNakCnt ++;
        mcSHOW_HW_MSG((" _I2_ "));
		
        return FALSE;                       // I2C fail
    }
    if (!mcTEST_BIT(ucValue, ccpUP_ENABLE))
    {
        psDemodCtx->u1UpChkCnt ++;
    //    mcSHOW_HW_MSG(("uP not enabled!\n"));

	


	//mcSHOW_HW_MSG((" _UE_ "));


	
		
        return FALSE;
    }
    if (DVBT_GetReg(psDemodCtx, MT5391_REG_uP_GP_REG06, &ucValue, 1))
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

	/*
	#if fcCHIP_TYPE
		// Get Chip Type
		DVBT_GetReg(psDemodCtx, MT5391_REG_Chip_ID, &ucChipID, 1);
		ucChipID &= mcMASKS(4, 4);
	#endif
	*/
		if (ucWriteCmdcnt)
		{
			ucWriteParacnt = ucWriteCmdcnt - 1;
			for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
			{
			#if 1
				DVBT_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1);
			#else
				if (DVBT_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1))
				{                               // I2C fail
					if (ii < (cMAX_WAIT_LOOP - 1))
					{
						mcDELAY_MS(cWAIT_TIME_INTERVAL);
						continue;
					}
				
					return FALSE;
				}
			#endif
			// This while loop will cause dead-lock for Combo-Tuner (when I2C_Block = TRUE)
			#if 1
				jj = 0;
				while (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
				{
				/*#if fcCHIP_TYPE
					if (ucChipID == CHIP_TYPE_1)
						DVBT_GetReg(psDemodCtx, MT5391_REG_RFA4CF, &ucValue, 1);
					else
				#endif*/
						DVBT_GetReg(psDemodCtx, MT5391_REG_RSICIF_RSP7, &ucValue, 1);
					DVBT_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1);
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

		/*#if fcCHIP_TYPE
			if (ucChipID == CHIP_TYPE_1)
				DVBT_SetReg(psDemodCtx, MT5391_REG_RFA4C0 + ucParaStart,
							  aucCmdData + ucParaStart + ucRegSetOfst,
							  ucWriteParacnt + 1);
			else
		#endif*/
				DVBT_SetReg(psDemodCtx, (UINT16) (MT5391_REG_RSICIF_CMD0 + ucParaStart),
							  aucCmdData + ucParaStart + ucRegSetOfst,
							  ucWriteParacnt + 1);
		#if fcMEAS_HOST_CMD_TIME
			u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
			mcSHOW_TIME_MSG((" Para %d (%3u)", ucWriteParacnt, u4TickCnt * mcGET_TICK_PERIOD()));
		#endif
		#if 0//fcSHOW_HOST_CMD
			if (fgShowHostCmd)
			{
			//    mcSHOW_USER_MSG(("HostCmd: (%d) -->", ucParaStart));
				mcSHOW_USER_MSG(("HostCmd -->"));
				for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
					mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
				mcSHOW_USER_MSG(("\n           "));
				DVBT_GetReg(psDemodCtx, MT5391_REG_uP_CMDR00 + ucParaStart,
						  aucCmdData + ucParaStart + ucRegSetOfst,
						  ucWriteParacnt + 1);
				for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
					mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
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
	//        for (jj = 0; jj < 1; jj ++)
			{
				for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
				{
				#if 1
					DVBT_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1);
				#else
					if (DVBT_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1))
					{                           // I2C fail
						if (ii < (cMAX_WAIT_LOOP - 1))
						{
							mcDELAY_MS(cSHORT_SLEEP_TIME);
							continue;
						}
						return FALSE;
					}
				#endif
					if (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
						break;
					//mcDELAY_MS(cSHORT_SLEEP_TIME);
                    mcDELAY_MS(10);
                    //HAL_Delay_us(10000);// temp5391
				}
			//    Get(MT5391_REG_uP_RSPR00 + cPARA_NUM_POS, &ucReadParacnt, 1);
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

				/*#if fcCHIP_TYPE
					if (ucChipID == CHIP_TYPE_1)
						u2AddrOfst = MT5391_REG_RFA4C8 - MT5391_REG_uP_RSPR00;
				#endif*/
					if (DVBT_GetReg(psDemodCtx,
									  (UINT16) (MT5391_REG_RSICIF_RSP0 + u2AddrOfst + ucParaStart),
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
				#if 0//fcSHOW_HOST_CMD
						if (fgShowHostCmd)
						{
							mcSHOW_USER_MSG(("HostRsp -->"));
							for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
								mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
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
                                mcSHOW_USER_MSG((" _@CS_ "));
								DVBT_GetReg(psDemodCtx, MT5391_REG_uP_RSPR00 + ucParaStart,
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

/*void vGetAsicVer(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    MT5112_GetReg(psDemodCtx, MT5112_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("MT5112 ASIC Ver %d.%d",
                    mcGET_FIELD_CMB(aucData[0], CHIP_ID_GEN) + 1,   // Chip Generation, 0 = MT5111
                    mcGET_FIELD_CMB(aucData[0], CHIP_ID_MASK) + 1));// Mask Rev., 1 = BO62350A

    mccSET_CMD2(aucData) = cCMD_ID_GET_LABEL;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_LABEL_SIZE, 1))
    {
        mcSHOW_USER_MSG((" %c%02X%02X-%02X%02X",
                        mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_TYPE), 
                        mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_ID_1), mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_ID_2),
                        mcGET_QUEUE(aucData, cOFST_LABEL_ID_1), mcGET_QUEUE(aucData, cOFST_LABEL_ID_2)));
    }

    mccSET_CMD2(aucData) = cCMD_ID_GET_VER;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_VER_SIZE, 1))
    {
        mcSHOW_USER_MSG((" %02X%02X-%02X%02X\n",
                         mcGET_QUEUE(aucData, cOFST_VER_YEAR_MONTH), mcGET_QUEUE(aucData, cOFST_VER_DATE),
                         mcGET_QUEUE(aucData, cOFST_VER_HOUR), mcGET_QUEUE(aucData, cOFST_VER_MINUTE)));
    }

}*/

/**********************************************************************/
/*BOOL fgGetRegs(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{

UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    {
        for (ii = 0; ii < ucCnt; ii ++)
            pucData[ii] = mccGET_QUEUE(aucData, ccOFST_REG1_DATA + ii);
    }
    else
        return FALSE;

    return TRUE;
}

UINT8 ucGetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{

UCHAR   ucValue = 0;

    fgGetRegs(psDemodCtx, u2Addr, &ucValue, 1);
    return ucValue;
}*/

//----------------------------------------------------------------------------- 
/*
 *  fgGetFwVars
 *  Get firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if (ucCnt > 4) { // for klocwork warning
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
 *  ucGetFwVarDVBT
 *  Get firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @retval   1-byte data.   
 */
//-----------------------------------------------------------------------------
//rename to fix build error "API multiply defined" when build with DTD on iDTV
UINT8 ucGetFwVarDVBT(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
UCHAR   ucValue = 0;

    fgGetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
    return ucValue;
}

//----------------------------------------------------------------------------- 
/*
 *  fgSetFwVars
 *  Set firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if (ucCnt > 4) { // for klocwork warning
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
 *  vSetFwVar
 *  Set firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  ucValue  data to be set
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
//void vSetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 ucValue)
//rename to fix build error "API multiply defined" when build with DTD on iDTV
void vSetFwVarDVBT(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 ucValue)
{
    fgSetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
}

/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/

/**********************************************************************/
/*                  Initialization APIs                               */
/**********************************************************************/
#if 0
char *cMsgUpErr[] =
{
    "",
    cMSG_UP_ERR_I2C,
    cMSG_UP_ERR_HW_RDY,
    cMSG_UP_ERR_CHKSUM_RDY,
    cMSG_UP_ERR_CHKSUM_OK
};
#endif
//----------------------------------------------------------------------------- 
/*
 *  s4LoadCode
 *  Download firmware code.
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
static INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage, UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{

#if 0
UCHAR   ucValue, ucI2cAuto, ucUpCtrl, ii;
//UINT16  u2CurI2cLen;
UINT16  u2CurRamAdr;
//UCHAR   ucI2cAddr;
#if fcLOAD_CODE_TIME
TIME_TICK_T u4TickStart, u4TickCur;
TIME_DIFF_T u4TickCnt;
#endif

    // SRAM clear
    ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcDELAY_MS(10);
    
    // set ad clock phase for more margin case
    DVBT_GetReg(psDemodCtx, MT5391_REG_PGA_CTRL_1, &ucUpCtrl, 1);
    mcCLR_BIT(ucUpCtrl, 5);
    DVBT_SetReg(psDemodCtx, MT5391_REG_PGA_CTRL_1, &ucUpCtrl, 1);
    
    
    if (DVBT_GetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucUpCtrl, 1))
        return cUP_LOAD_ERR_I2C;
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
    {
        return cUP_LOAD_ERR_HW_RDY;
    }

	ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucUpCtrl, 1);
    ucValue = 0;
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_GP_REG06, &ucValue, 1);

	for (ii = 0; ii < 4; ii ++)
    {
        DVBT_GetReg(psDemodCtx, (UINT16) (MT5391_REG_uP_PNGP_H + ii), &ucValue, 1);
        DVBT_SetReg(psDemodCtx, (UINT16) (MT5391_REG_uP_PNGP_H + ii), &ucValue, 1);
	}

// set max code size
	ucValue = 0x6F;
	DVBT_SetReg(psDemodCtx, MT5391_REG_uP_LPA_H, &ucValue, 1);
	ucValue = 0xFF;
	DVBT_SetReg(psDemodCtx, MT5391_REG_uP_LPA_L, &ucValue, 1);
	
// Turn off I2C auto-increment
    DVBT_GetReg(psDemodCtx, MT5391_REG_I2C_Cfg, &ucI2cAuto, 1);
    mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBT_SetReg(psDemodCtx, MT5391_REG_I2C_Cfg, &ucI2cAuto, 1);

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    u2CurRamAdr = 0;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
    
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_IRA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2CurRamAdr);
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_IRA_L, &ucValue, 1);
    
    //ucI2cAddr = psDemodCtx->I2cAddress;

	
    //if (u1I2cWriteSpeed(psDemodCtx, ucI2cAddr, MT5391_REG_uP_IRData, u1CodeImage + u2CurRamAdr, u2CurI2cLen))
#if  1 //liuqu_test    
    if (ucI2cWriteSpeed(psDemodCtx, u1CodeImage, u2CurRomLen))
#else		
    if(ICtrlBus_Risc_LoadCode(&sRiscDvbtCtx, MT5391_REG_uP_IRData, u1CodeImage, u2CurRomLen))	
#endif		
    {
        mcSHOW_POP_MSG(("Download I2C fail! (%d)\n"));
        return 1;
    }
#if 0
	while (u2CurRomLen)
	{
		u2CurI2cLen = u2CurRomLen;
		if (u2CurI2cLen > u2MaxI2cLen)
			u2CurI2cLen = u2MaxI2cLen;

		if (ucI2cWriteSpeed(psDemodCtx, ucI2cAddr, MT5391_REG_uP_IRData,
					  u1CodeImage + u2CurRamAdr, u2CurI2cLen))
		{
			mcSHOW_POP_MSG(("Download I2C fail! (%d)\n", u4I2cClkDiv));
			break;
		}
		u2CurRamAdr += u2CurI2cLen;
		u2CurRomLen -= u2CurI2cLen;
	}
#endif	
// Turn on I2C auto-increment
    mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBT_SetReg(psDemodCtx, MT5391_REG_I2C_Cfg, &ucI2cAuto, 1);

    mcSET_BIT(ucUpCtrl, ccpUP_RESTART_CHKSUM);
    mcCLR_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE);
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBT_GetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucUpCtrl, 1);
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
    DVBT_SetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucUpCtrl, 1);

	// delay for waiting FW initialization done, may have I2C timeout at first
	for (ii = 0; ii < 5; ii++)
	{
		mcDELAY_MS(3);
		DVBT_GetReg(psDemodCtx, MT5391_REG_uP_GP_REG06, &ucValue, 1);
		if (ucValue == ccFW_RUN_TAG)
			break;
	}

    mcSHOW_DBG_MSG(("uP Startup!"));

#if fcLOAD_CODE_TIME
    mcGET_SYS_TIME(u4TickCur);
    u4TickCnt = mcGET_DIFF_TIME(u4TickStart, u4TickCur);
    u4TickCnt = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" (%3u ms) ", u4TickCnt));
    if (u4TickCnt > 0)
        return -((INT32) u4TickCnt);
    else
#endif

        return cUP_LOAD_OK;

#else

//UCHAR    ii;
//UINT16  u2CurRamAdr;

#if fcLOAD_CODE_TIME
TIME_TICK_T u4TickStart, u4TickCur;
TIME_DIFF_T u4TickCnt;
#endif

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_USER_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    //ii = 0;
#endif
    
    if (ucI2cWriteSpeed(psDemodCtx, u1CodeImage, u2CurRomLen))	
    {
        mcSHOW_POP_MSG(("Download fail! (%d)\n"));
        return 1;
    }
	
    // Turn on I2C auto-increment
	// delay for waiting FW initialization done, may have I2C timeout at first
    mcSHOW_USER_MSG(("uP Startup!"));

#if fcLOAD_CODE_TIME
    //mcGET_SYS_TIME(u4TickCur);
    //u4TickCnt = mcGET_DIFF_TIME(u4TickStart, u4TickCur);
    //u4TickCnt = mcCONV_SYS_TIME(u4TickCnt);
    //mcSHOW_DBG_MSG((" (%3u ms) ", u4TickCnt));
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    mcSHOW_USER_MSG((" (Download %3u ms$#$%#$%%$$%%$##$%^%$#)", mcCONV_SYS_TIME(u4TickCnt)));
   // if (u4TickCnt > 0)
   //     return -((INT32) u4TickCnt);
   // else
#endif

        return cUP_LOAD_OK;
#endif

}

//----------------------------------------------------------------------------- 
/*
 *  vDTMBGetStatus
 *  Get demod status and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vDTMBGetStatus(DEMOD_CTX_T *psDemodCtx)
{
//UCHAR   ucValue = 0;
#if 0
UCHAR   aucData[cMAX_READ_NUM + 1];

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_STATUS1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_STATUS1_SIZE, 1))
	{	
		// Signal 
		psDemodCtx->s1IfAgc = (S8)mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_IF_AGC);
		// state
		psDemodCtx->u1ControlStatus = mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_SC_STATE);
		psDemodCtx->fgAgcLock = mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_AGC_LOCK);
		// removed for Toolx only, 080826, JC
		//if (psDemodCtx->fgAgcLock == FALSE)
		//	psDemodCtx->u1ControlStatus = 0;
		// MD state
		psDemodCtx->ucMdFftMode = mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_MD_FFTMODE);
		// TPS state    
		psDemodCtx->fgTpsLock = mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_TPS_LOCK);
		// TS state
		psDemodCtx->fgTsLock = mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_TS_LOCK);
		// RSSI
		psDemodCtx->u1Rssi = mccGET_QUEUE(aucData, ccOFST_DEMOD_STATUS1_RSSI);
	}
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  vDTMBGetInfo
 *  Get demod information and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vDTMBGetInfo(DEMOD_CTX_T *psDemodCtx)
{
UINT8	uValue;

#if 0
UINT8   aucData[cMAX_READ_NUM + 1];
UINT8   uc_data[4];

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO3;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_INFO3_SIZE, 1))
	{
		//*********************  AGC  ***********************
		//psDemodCtx->s1RfAgc = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO3_AGC_RF_OP2);
		//psDemodCtx->s1IfAgc = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO3_AGC_IF_OP2);
			
		//**************** Pre-Viterbi BER *********************
	    uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO3_PREV_BER0);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO3_PREV_BER1);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO3_PREV_BER2);

        psDemodCtx->u2VdTotBit = mcUNSIGN_2BYTE(uc_data[2], uc_data[1], 8, 8);//*16;
        psDemodCtx->u2VdErrBit = (UINT16)uc_data[0];//*4;
	}

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO4;
	if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_INFO4_SIZE, 1))
	{
		//corrected error bits
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_COR_BIT0);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_COR_BIT1);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_COR_BIT2);
		psDemodCtx->u4CorrBit = uc_data[0];
		psDemodCtx->u4CorrBit += (U32)mcUNSIGN_2BYTE(uc_data[2], uc_data[1], 8, 8) * 256;
		//total error packets
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_ERR_PKT0);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_ERR_PKT1);
		psDemodCtx->u2ErrorPkt = (UINT16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);	
		//total packets
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_TOT_PKT0);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO4_RS_TOT_PKT1);
		psDemodCtx->u2TotalPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
	}

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO6;
	if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_INFO6_SIZE, 1))
	{
		//***************** CFO Initial *********************
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_IFS_ADJ_LO);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_IFS_ADJ_HI);
		psDemodCtx->s2Ifo = (S16)mcSIGN_2BYTE(uc_data[1], uc_data[0], 3, 8);  // 11 bits for 5391

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_MD_STATUS0);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_MD_STATUS1);
		psDemodCtx->s2Ffo = (S16)mcSIGN_2BYTE(uc_data[0], uc_data[1], 4, 8);
		//***************** CFO Tracking *********************
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_FTT_LF_FREQ0);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_FTT_LF_FREQ1);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO6_FTT_LF_FREQ2);
		psDemodCtx->s4Cfo = uc_data[0];
		psDemodCtx->s4Cfo += (S32)mcSIGN_2BYTE(uc_data[2], uc_data[1], 7, 8)*256;
	}

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO10;
	if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_INFO10_SIZE, 1))
	{
		// SNR, snr*512
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_SNR_CSI_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_SNR_CSI_H);
		psDemodCtx->s2Snr = (S16) mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		// Cell ID
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_CID_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_CID_H);
		psDemodCtx->u2CellId = (U16) mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		// wideband power, inband power (for silicon tuner)
		psDemodCtx->m_s1AciPow = (S8)mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_RFARC_WBPD);
		psDemodCtx->m_s1InbPow = (S8)mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_RFARC_INB);
	}

#endif
	psDemodCtx->u2TotalPkt=0;
    LastErrorPkt = ErrorPkt;

	//stop pkt counter
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );
	uValue &= 0xEF;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );

	//get total pkt count
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x2B, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;
	psDemodCtx->u2TotalPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x2A, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;
	psDemodCtx->u2TotalPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x29, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;
	psDemodCtx->u2TotalPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x28, &uValue, 1 );
	psDemodCtx->u2TotalPkt |= uValue;

	//get error pkt count
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x2F, &uValue, 1 );
	ErrorPkt |= uValue; 
	ErrorPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x2D, &uValue, 1 );
	ErrorPkt |= uValue; 
	ErrorPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x2E, &uValue, 1 );
	ErrorPkt |= uValue; 
	ErrorPkt <<= 8;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x2C, &uValue, 1 );
	ErrorPkt |= uValue; 

	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );
	uValue |= 0x10;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );

	//mcSHOW_DBG_MSG2(("LastErrorPkt = %d, ErrorPkt=%d\n ", LastErrorPkt,ErrorPkt));

	if(LastErrorPkt<=ErrorPkt)
		psDemodCtx->u2ErrorPkt = (ErrorPkt - LastErrorPkt);
	else
		psDemodCtx->u2ErrorPkt = ErrorPkt;

#if 1
    if( (psDemodCtx->u2TotalPkt>7000) && (psDemodCtx->u2ErrorPkt<10))
    {
		//clear BCH error count 
		ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );
		uValue |= 0x08;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );
		uValue &= 0xF7;
		ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );
    }
#endif	
    mcSHOW_DBG_MSG2(("u2ErrorPkt = %d, u2TotalPkt=%d\n ", psDemodCtx->u2ErrorPkt,psDemodCtx->u2TotalPkt));
}

//----------------------------------------------------------------------------- 
/*
 *  vDTMBGetPara
 *  Get demod system parameters and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vDTMBGetPara(DEMOD_CTX_T *psDemodCtx)
{
#if 0
UINT8   aucData[cMAX_READ_NUM + 1];

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO1;
	if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_INFO1_SIZE, 1))
	{
		psDemodCtx->u1Ver1    = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_VER1);
		psDemodCtx->u1Ver0    = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_VER0);
		psDemodCtx->u1Fft     = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_MD_STATUS0), 
			                                MT5391_MASK_MD_FFT_MODE, 6);
		psDemodCtx->u1GI      = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_MD_STATUS0), 
			                                MT5391_MASK_MD_GI_MODE, 4);
		psDemodCtx->u1CR      = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG5), 
			                                MT5391_MASK_SYS_CR, 5);
		psDemodCtx->u1Mod     = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG0), 
			                                MT5391_MASK_TPS_CON, 0);
		psDemodCtx->u1Hier    = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG0), 
			                                MT5391_MASK_TPS_HIER, 2);
		psDemodCtx->u1HpLp    = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG1), 
			                                MT5391_MASK_TPS_HP_sel, 6);
		psDemodCtx->u1LpCR    = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG1), 
							                MT5391_MASK_TPS_LP_CR, 3);
		psDemodCtx->u1HpCR    = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG1),
			                                MT5391_MASK_TPS_HP_CR, 0);
		psDemodCtx->u1Indepth = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_TPS_REG0),
			                                MT5391_MASK_TPS_INDEPTH, 4);
		psDemodCtx->u1Bw      = mcGET_FIELD(mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO1_CHANL_SEL),
			                                MT5391_MASK_CHANL_SEL, 0);  
	}
#endif
	
UINT8 uValue;

	//ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x0C, &uValue, 1);
	//psDemodCtx->u1GI=(uValue&0x03);//00->PN420, 01->PN595, 10->PN945 

	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x13, &uValue, 1);
	psDemodCtx->u1GI=(uValue&0x60)>>5;//00->PN420, 01->PN595, 10->PN945 

	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x19, &uValue, 1);
	psDemodCtx->u1TimDeInterl=((uValue&0x40)>>6);//0->M=240, 1->M=720
	psDemodCtx->u1Mod=((uValue&0x38)>>3); //000->4QAM, 001->4QAM-NR, 010->16QAM, 011->32QAM, 100->64QAM
	psDemodCtx->u1FecRate=((uValue&0x06)>>1); //00->0.4, 01->0.6, 10->0.8
	psDemodCtx->u1CarrierMode=(uValue&0x01);//0->Muticarrier, 1->Singal carrier

    ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x18, &uValue, 1);
	psDemodCtx->u1PnNumber=(uValue&0x01);//0->Variable phase for PN sequence, 1->Constant phase
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_UpdateInfo
 *  Get demod status, information and system parameters 
 *  and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_UpdateInfo(DEMOD_CTX_T *psDemodCtx)
{
	vDTMBGetStatus(psDemodCtx);
	vDTMBGetInfo(psDemodCtx);
	vDTMBGetPara(psDemodCtx);
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_TunerI2c
 *  Switch tuner bypass mode 
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  fgSwitchOn  Flag to switch bypass mode on/off (TRUE: ON, FALSE: OFF)
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn)
{

//UCHAR   ii;
#if 0
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if (fgSwitchOn)                         // Access Tuner
    {
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
		mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 1;
		if (psDemodCtx->ucEnFpga)
			mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x04; // FPGA:0x04
		else
			mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07; // ASIC:0x07
		mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0xB2; // tune for MT5391 ASIC
    }
    else                                    // Bypass Tuner
    {
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
		mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 0;
		mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
		mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0xB2;
    }

    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_TUNER_I2C_ONOFF_SIZE+1))
    {
    	mcDELAY_MS(20);
    }

#else



#if fgIssharp2403

#if !defined(CC_NO_TUNER_BYPASS)

	UINT8		uValue, RegAddr;

	if (fgSwitchOn) // // Access Tuner
	{
		uValue = 0xE1;//tuner i2c addrss is 0xC2
		RegAddr = 0x01;
		if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		{
		    mcSHOW_DBG_MSG3(("Detach Write I2C fail(enable tuner)!\n"));	
		}
		else
		{
			mcSHOW_DBG_MSG3(("Detach Write I2C success(enable tuner)!\n"));	
		}
	}
	else
	{
		uValue = 0x61;
		RegAddr = 0x01;
		if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		{
		    mcSHOW_DBG_MSG3(("Detach Write I2C fail(disable tuner)!\n"));
		}
		else
		{
			mcSHOW_DBG_MSG3(("Detach Write I2C success(disable tuner)!\n"));	
		}
	
	}
#endif
#endif

#endif




}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_DriverStart
 *  Start demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  u1Mon1Sel  Monitor1 driver select. (0: None, 1: Auto-Reacquisition).
 *  @param  u1Mon2Sel  Monitor2 driver select. (0: None, 1: TPS sync, 2: SPS sync auto, 3: SPS sync manual
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBT_DriverStart(DEMOD_CTX_T *psDemodCtx, UINT8 u1Mon1Sel, UINT8 u1Mon2Sel)
{
UCHAR   aucData[cMAX_READ_NUM + 1];

	mccSET_CMD2(aucData) = ccCMD_ID_START_DRIVER;
	// 0:none 
	// 1:Adv ReAcq
	mccSET_QUEUE2(aucData, ccOFST_SET_DRIVER_MON1) = u1Mon1Sel;   
	// 0:none
	// 1:TPS sync
	// 2:SPS sync auto
	// 3:SPS sync manual
	mccSET_QUEUE2(aucData, ccOFST_SET_DRIVER_MON2) = u1Mon2Sel;   
	mcHOST_CMD(psDemodCtx, aucData, ccOFST_SET_DRIVER_RSIZE, ccOFST_SET_DRIVER_SIZE + 1);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBT_DriverStop(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];

	mccSET_CMD2(aucData) = ccCMD_ID_STOP_DRIVER;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_STOP_DRIVER_RSIZE, 1);
}

UINT8 DTMB_SetIFFreq(DEMOD_CTX_T *psDemodCtx,UINT16 Ifreq)//(IF freq with KHz uint)
{
	return 1;//default setting is ok
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_DemodReset
 *  Demod Reset.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
#if defined(CC_NEED_RESETHW) || SUPPORT_DISABLE_DEMOD_CLOCK
INT32 DTMB_DemodReset(DEMOD_CTX_T *psDemodCtx)
{
	UINT32 DEMOD_RESET = DRVCUST_OptGet(eDemodResetGpio);
	UINT32 reset_time;
	reset_time = 250;
	
		mcSHOW_USER_MSG(("RESET DTMB DEMOD //Set (GPIO%d)low delay %d ms,set high delay %dms,keep high!\n", DEMOD_RESET,reset_time,reset_time));
        GPIO_SetOut(DEMOD_RESET, 0);
        mcDELAY_MS(250);
		GPIO_SetOut(DEMOD_RESET, 1);
        mcDELAY_MS(250);
	
	psDemodCtx->fgDemodReset = TRUE;

	return (DRVAPI_TUNER_OK);
}
#endif

//----------------------------------------------------------------------------- 
/*
 *  DTMB_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32 DTMB_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 ucI2cAddress, BOOL fgLoadCode)
{
//UCHAR   ucPara[ccCMD_REG_NUM];
//UCHAR	ii;
//UCHAR   aucData[cMAX_READ_NUM + 1];

//SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

    sRiscDvbtCtx.pu4RegBase = pu4DVBT_Reg;

    psDemodCtx->u2ErrorPkt = 0;
    psDemodCtx->u2TotalPkt = 0;
    psDemodCtx->u4CorrBit = 0;
    psDemodCtx->u1Bw = MT5391_BW_8MHz;
    psDemodCtx->fgEn24MClk = FALSE;
    psDemodCtx->ucEnFpga = FALSE;
    psDemodCtx->I2cAddress = ucI2cAddress;
    psDemodCtx->ucConnType = PORT_CFG_MTK;
    psDemodCtx->fgDemodReset = FALSE;
    psDemodCtx->ucTsIfFormat = 0x8A;   //changed from 0x0A to 0x8A due to MT5360B, and no impact to MT539x and MT5360A
    psDemodCtx->ucTsIfCtrl = 0x00;
    psDemodCtx->ucTsIfRate = 0x07;
    psDemodCtx->u1ChSearchMode = CH_SEARCH_SCAN;
    psDemodCtx->fgSelectANIR = TRUE;
    psDemodCtx->u1MdMode = 0xF0;
    psDemodCtx->s1FreqBias40khz = 0;
    psDemodCtx->u1TunerAcqType = ACQ_ACTION_DEFAULT;
    psDemodCtx->i4BerTh = 430;   // JC, 090202
    psDemodCtx->u1MWIntf = cTURNKEY_IF;
	
	psDemodCtx->m_TunerMode = 0;


	#if defined(CC_NEED_RESETHW)
	DTMB_DemodReset(psDemodCtx);
	#endif

	//DVBT_ChipInit();
    UINT8 uValue;

    mcSHOW_USER_MSG(("Before Download FW!!!!!!!!!!##########%%%%%%%%%%\n"));
    if (fgLoadCode)
    {
    
        if (s4LoadCode(psDemodCtx, ucDvbtCodeImage, sizeof(ucDvbtCodeImage), cMAX_I2C_LEN, TRUE))
        {
            return (DRVAPI_TUNER_ERROR);
        }
        
    }  
    mcSHOW_USER_MSG(("After Download FW!!!!!!!!!!###############&&&&&&&&&&\n"));

	#if 1
    uValue=0x11;//seclect internal ADC type:
    if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x07, &uValue, 1 ))
	{
        /*fail case*/
		mcSHOW_USER_MSG(("ucI2cWriteSpeed Download I2C fail(0x07)!\n"));
		return 1;
    }
	#endif
    #if 1
	//set serial when open
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );
    uValue |= 0x21;
    ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 );

	//set TS clock
	//ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, 0xC1, &uValue, 1 );
    //uValue |= 0x80;
    //ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0xC1, &uValue, 1 );
	//uValue =  0x44;
	//ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0xC2, &uValue, 1 );
	
    #endif	
	
	//set IF freq
	if(!DTMB_SetIFFreq(psDemodCtx,36167))//IF freq is 36.167MHz
	{
		mcSHOW_USER_MSG(("Set IF freq fail!\n"));
		return 1;
	}
	
	return (DRVAPI_TUNER_OK);



}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_DemodReset
 *  Demod Reset.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32 DVBT_DemodReset(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];

	if (!psDemodCtx)
		return DRVAPI_TUNER_ERROR;

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_RESET;
    if (!mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_RESET_SIZE, 1))
	{
    	return DRVAPI_TUNER_ERROR;
    }
	
	psDemodCtx->fgDemodReset = TRUE;

	return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
/*                  Query APIs                                        */
/**********************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetIfAgcVol
 *  Get IF AGC voltage.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT16) IF AGC voltage in mV.   
 */
//-----------------------------------------------------------------------------
UINT16 DTMB_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx)
{
    UINT16 u2IfAgcVol = 0;
    INT32  s4Temp;

    s4Temp = (INT32)(psDemodCtx->s1IfAgc) * 3300;
    s4Temp /= 256;
    u2IfAgcVol = (UINT16)(s4Temp + 1650);

    return u2IfAgcVol;
}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetSignalLevel
 *  Get signal level from 0 (lowest) to 100 (highest).
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT8) 0~100.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_GetSignalLevel(DEMOD_CTX_T *psDemodCtx)
{
#if 0
	INT8 s1AgcMax;	
	INT8 s1AgcMin;
	INT16 s2SignalStrength;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

      
	//s1AgcMax = (INT8)psDemodCtx->sTunerCtx.m_aucPara[4];	
	//s1AgcMin = (INT8)psDemodCtx->sTunerCtx.m_aucPara[5];
	s1AgcMax = (INT8)pEuTunerCtx->m_ifAgc_lvl_max;	
	s1AgcMin = (INT8)pEuTunerCtx->m_ifAgc_lvl_min;

	if (s1AgcMax != s1AgcMin)
		s2SignalStrength = ((INT16)(psDemodCtx->s1IfAgc-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
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
#endif
	UINT8 uValue, s2SignalStrength;
    uValue = 0x0D;
    ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, 0xFF, &uValue, 1 );
	uValue = 0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, 0x54, &uValue, 1 );
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS2, 0x55, &s2SignalStrength, 1 );

	
	if( (psDemodCtx->u2TotalPkt < 7000) || (psDemodCtx->u2ErrorPkt>100) )
		s2SignalStrength = 0;
	else if( (psDemodCtx->u2ErrorPkt!=0)&&(psDemodCtx->u2ErrorPkt< 30)&&(psDemodCtx->u2TotalPkt > 8000) )
	    s2SignalStrength = 2;
	else
		s2SignalStrength += 5;

	mcSHOW_DBG_MSG2(("SignalStrength = %d", s2SignalStrength));

	return s2SignalStrength;

}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT16) SNR in 0.1dB.   
 */
//-----------------------------------------------------------------------------
UINT16 DTMB_GetSignalSNR(DEMOD_CTX_T *psDemodCtx)
{
#if 0
	INT16 s2_data;

	if (psDemodCtx->fgTsLock == TRUE)
	{
//		s2_data = (psDemodCtx->s2Snr) / 512;
        s2_data = (psDemodCtx->s2Snr) / 51; // return value in 0.1 dB
		if (s2_data < 0)
			return 0;
		else
			return (UINT16) s2_data;
	}
	return 0;
#endif
	UINT8 uValue;
	uValue=0x0D;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, 0xFF, &uValue, 1);
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS2, 0x5D, &uValue, 1);

	psDemodCtx->s2Snr=uValue;
	mcSHOW_DBG_MSG2(("Current SNR = %d dB", psDemodCtx->s2Snr));

	if (psDemodCtx->s2Snr < 0)
		return 0;
	else
		return (UINT8) psDemodCtx->s2Snr;

}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetTotPkt(DEMOD_CTX_T *psDemodCtx)  
{
	return (UINT32)psDemodCtx->u2TotalPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetErrPkt(DEMOD_CTX_T *psDemodCtx)  
{
	return (UINT32)psDemodCtx->u2ErrorPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetPER
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBT_GetPER(DEMOD_CTX_T *psDemodCtx)  // return value = PER*10^5
{
    INT32 RSTotPkt, RSErrPkt;
		
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;

    //mcSHOW_DBG_MSG2(("RSTotPkt==%d!!!!!!!!\n",RSTotPkt));
	
	if (RSTotPkt <= 0)
	{
		//mcSHOW_DBG_MSG2(("Return -1@@@@@@@@@@\n"));
		return 100000;
	}
	
	//mcSHOW_DBG_MSG2(("Code Size = %d\n", (RSErrPkt * 100) / RSTotPkt));

    else if (RSErrPkt > 21000) // to prevent *100000 overflow, 21276 pkts/sec when 32Mbps 
        return (INT32) (((RSErrPkt * 10000) / RSTotPkt) *10);
    else
        return (INT32) ((RSErrPkt * 100000) / RSTotPkt);


}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetTpsLock
 *  Get TPS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL DVBT_GetTpsLock(DEMOD_CTX_T *psDemodCtx)
{
	return psDemodCtx->fgTpsLock;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL DVBT_GetTsLock(DEMOD_CTX_T *psDemodCtx)
{
	return psDemodCtx->fgTsLock;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetRfAgcWidebandPow
 *  Get wide-band power.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (INT8) value.   
 */
//-----------------------------------------------------------------------------
INT8 DVBT_GetRfAgcWidebandPow(DEMOD_CTX_T *psDemodCtx)
{
	return psDemodCtx->m_s1AciPow;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetCellId
 *  Get Cell ID.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (UINT16) Cell ID.   
 */
//-----------------------------------------------------------------------------
UINT16 DVBT_GetCellId(DEMOD_CTX_T *psDemodCtx)
{
	return psDemodCtx->u2CellId;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetFreqOffset_kHz
 *  Get frequency offset in kHz.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   (INT32) freq_offset * 256.   
 */
//-----------------------------------------------------------------------------
INT32 DVBT_GetFreqOffset_kHz(DEMOD_CTX_T *psDemodCtx)
{
	S32 s4_data;
	U8	Nfft, Mt5391_BW;

	if (!psDemodCtx)
		return 0;
	
	// CFO Tracking
	s4_data = psDemodCtx->s4Cfo << 4;
	// IFO Initial
	s4_data += ((S32)psDemodCtx->s2Ifo << 18);
	// FFO Initial
	s4_data += ((S32)psDemodCtx->s2Ffo << 6);

	switch (psDemodCtx->u1Bw)
	{ 
		case 0:  Mt5391_BW = 6;  break;
		case 1:  Mt5391_BW = 7;  break;
		case 2:  Mt5391_BW = 8;  break;
		case 3:  Mt5391_BW = 5;  break;
		default: Mt5391_BW = 8;  break;
	}

	switch (psDemodCtx->u1Fft)
	{
		case 0:	Nfft = 2;	break;
		case 1:	Nfft = 8;	break;
		case 2:	Nfft = 4;	break;
		default:Nfft = 1;	break;
	}
	s4_data = s4_data * Mt5391_BW / Nfft *2/1835;

	return (-s4_data);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetBW
 *  Get channel bandwidth.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : 6M.
 *  @retval   1 : 7M.
 *  @retval   2 : 8M.
 *  @retval   3 : 5M.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetBW(DEMOD_CTX_T *psDemodCtx)
{
	return (UINT8) psDemodCtx->u1Bw;
}

//----------------------------------------------------------------------------- 
/*
 *  vSetIntfPara
 *  Set host command interface API.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  para  Pointer of parameters buffer. (para[0] : CmdId, para[1~(len-1)] : parameters).
 *  @param  len  Length of parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
static void vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR	ii;

    if (len > 8) { // for klocwork warning
        return;
    }

	mccSET_CMD2(aucData) = para[0];
	if (len > 1)
	{
		for (ii = 0; ii < (len-1); ii++)
			mccSET_QUEUE2(aucData, ii) = para[ii+1];
	}
	
	mcHOST_CMD(psDemodCtx, aucData, 0, len);
}

//----------------------------------------------------------------------------- 
/*
 *  vSetChSearchMode
 *  Set channel search mode according to demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
#if 0
static void vSetChSearchMode(DEMOD_CTX_T *psDemodCtx)
{
	UCHAR   aucData[cMAX_READ_NUM + 1];

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_CH_SEARCH;
	mccSET_QUEUE2(aucData, ccOFST_SET_CH_SEARCH_MODE)     = psDemodCtx->u1ChSearchMode;
	mccSET_QUEUE2(aucData, ccOFST_SET_CH_SEARCH_MD_MODE)  = psDemodCtx->u1MdMode;
	mccSET_QUEUE2(aucData, ccOFST_SET_CH_SEARCH_TPS_CR)   = psDemodCtx->u1TpsCr;
	mccSET_QUEUE2(aucData, ccOFST_SET_CH_SEARCH_TPS_CONS) = psDemodCtx->u1TpsCons;
	mccSET_QUEUE2(aucData, ccOFST_SET_CH_SEARCH_TPS_HIER) = psDemodCtx->u1TpsHier;
	mccSET_QUEUE2(aucData, ccOFST_SET_CH_SEARCH_FREQ_BIAS_40KHZ) = psDemodCtx->s1FreqBias40khz;
	mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_CH_SEARCH_SIZE + 1);
}
#endif
//----------------------------------------------------------------------------- 
/*
 *  vEnable24MHzClk
 *  Enable 24MHz related settings of demod.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
#if 0
static void vEnable24MHzClk(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR	ii;

//                     0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x32, 0x33, 0x3F, 0x18, 0x19, 0x28
UCHAR uc_data_8M1[] = {0x00, 0x00, 0x40, 0x05, 0x33, 0x02, 0x62, 0x18, 0x48, 0x0C, 0x01, 0x60},
      uc_data_7M1[] = {0x00, 0x00, 0x00, 0x06, 0x33, 0x01, 0x56, 0x55, 0x50, 0x0C, 0x01, 0x50},
      uc_data_6M1[] = {0x00, 0x00, 0x00, 0x07, 0x33, 0x00, 0x4A, 0x92, 0x40, 0x14, 0x01, 0x40},
      uc_data_5M1[] = {0x66, 0x66, 0x66, 0x08, 0x33, 0x03, 0x3D, 0xCF, 0x3C, 0x15, 0x08, 0x30};

	switch (ucBW)
	{
		case MT5391_BW_8MHz:
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW1;
			for (ii = 0; ii < 7; ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW1_PARA1+ii) = uc_data_8M1[ii];
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW1_SIZE + 1);
			
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW2;
			for (ii = 7; ii < sizeof(uc_data_8M1); ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW2_PARA8+ii-7) = uc_data_8M1[ii];
			mccSET_QUEUE2(aucData, ccOFST_SET_BW2_BW) = ucBW;
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW2_SIZE + 1);
			break;
		case MT5391_BW_7MHz:
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW1;
			for (ii = 0; ii < 7; ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW1_PARA1+ii) = uc_data_7M1[ii];
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW1_SIZE + 1);
			
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW2;
			for (ii = 7; ii < sizeof(uc_data_7M1); ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW2_PARA8+ii-7) = uc_data_7M1[ii];
			mccSET_QUEUE2(aucData, ccOFST_SET_BW2_BW) = ucBW;
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW2_SIZE + 1);
			break;
		case MT5391_BW_6MHz:
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW1;
			for (ii = 0; ii < 7; ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW1_PARA1+ii) = uc_data_6M1[ii];
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW1_SIZE + 1);
			
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW2;
			for (ii = 7; ii < sizeof(uc_data_6M1); ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW2_PARA8+ii-7) = uc_data_6M1[ii];
			mccSET_QUEUE2(aucData, ccOFST_SET_BW2_BW) = ucBW;
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW2_SIZE + 1);
			break;
		case MT5391_BW_5MHz:
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW1;
			for (ii = 0; ii < 7; ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW1_PARA1+ii) = uc_data_5M1[ii];
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW1_SIZE + 1);
			
			mccSET_CMD2(aucData) = ccCMD_ID_SET_BW2;
			for (ii = 7; ii < sizeof(uc_data_5M1); ii++)
				mccSET_QUEUE2(aucData, ccOFST_SET_BW2_PARA8+ii-7) = uc_data_5M1[ii];
			mccSET_QUEUE2(aucData, ccOFST_SET_BW2_BW) = ucBW;
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_SET_BW2_SIZE + 1);
			break;
		default:
			break;
	}
}
#endif
//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetBW
 *  Set channel bandwidth.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBT_SetBW(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR	ii;
UCHAR	jj;

UCHAR  uc_iir_coef_8M[] = { 0x36, 0x0C, 0x2A, 0x1B, 0x6F, 0x1F, 
							0xF1, 0x0C, 0xB6, 0x19, 0x32, 0x14, 
							0x7E, 0x0D, 0xA4, 0x18, 0xB8, 0x12, 
							0xE5, 0x0D, 0x20, 0x19, 0x07, 0x11 };
UCHAR  uc_iir_coef_7M[] = { 0xB8, 0x0C, 0xCF, 0x1A, 0xBE, 0x1D, 
							0x6B, 0x0D, 0x8D, 0x19, 0x6E, 0x13, 
							0xF5, 0x0D, 0x99, 0x18, 0x2C, 0x12, 
							0x82, 0x0E, 0xD7, 0x18, 0xB0, 0x10 };
UCHAR  uc_iir_coef_6M[] = { 0x39, 0x0D, 0x6F, 0x1A, 0xEB, 0x1B, 
						    0xDD, 0x0D, 0x62, 0x19, 0xB9, 0x12, 
						    0x61, 0x0E, 0x8D, 0x18, 0xAE, 0x11, 
						    0xD8, 0x0E, 0xCB, 0x18, 0x6B, 0x10 };

UCHAR* uc_iir_coef = uc_iir_coef_8M;


	psDemodCtx->u1Bw = ucBW;

	mccSET_CMD2(aucData) = ccCMD_ID_FAST_SET_BW;
	mccSET_QUEUE2(aucData, ccOFST_FAST_SET_BW_BW) = ucBW;
	mccSET_QUEUE2(aucData, ccOFST_FAST_SET_BW_IIR) = psDemodCtx->fgSelectANIR;
	mcHOST_CMD(psDemodCtx, aucData, ccOFST_FAST_SET_BW_RSIZE, ccOFST_FAST_SET_BW_SIZE + 1);

	// CCI notch filter coeff.
	if(!psDemodCtx->fgSelectANIR)
	{
		switch (ucBW)
		{
			case MT5391_BW_8MHz:
				uc_iir_coef = uc_iir_coef_8M;
				break;
			case MT5391_BW_7MHz:
				uc_iir_coef = uc_iir_coef_7M;
				break;
			case MT5391_BW_6MHz:
				uc_iir_coef = uc_iir_coef_6M;
				break;
			default:
				uc_iir_coef = uc_iir_coef_8M;
				break;
		}
		for (jj = 0; jj < 4; jj++)
        {
			mccSET_CMD2(aucData) = ccCMD_ID_CCI_NOTCH;
			mccSET_QUEUE2(aucData, ccOFST_CCI_NOTCH_IDX) = jj;
			for (ii = 0; ii < 6; ii++)
				mccSET_QUEUE2(aucData, ccOFST_CCI_NOTCH_IDX+ii+1) = uc_iir_coef[jj*6+ii];
			mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_CCI_NOTCH_RSIZE + 1);
		}		
	}
	// add modified register settings here
	// ...
	
	DVBT_DemodReset(psDemodCtx);

#ifdef CC_SDAL
    // temporarily set target level = 7 for SS platform
    aucData[0] = 0x07;
    DVBT_SetReg(psDemodCtx, 0x1A1, &aucData, 1);
#endif

}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetTsIf
 *  Set TS interface parameters.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  tsif_format  TS interface format.
 *  TsIfFormat:
 *  b7  Reserve
 *  b6	VALID	Active polarity selection of tsif_val
 *  	0 -> tsif_val is high active
 *  	1 -> tsif_val is low active
 *  b5	ERR	Active polarity selection of tsif_err
 *  	0 -> tsif_err is high active
 *  	1 -> tsif_err is low active
 *  b4	SYNC	Active polarity selection of tsif_sync
 *  	0 -> tsif_sync is high active
 *  	1 -> tsif_sync is low active
 *  b3	TEI	Transport stream error indicator
 *  	0 -> TEI bit will not be changed by tsif
 *  	1 -> TEI bit will automatic set by tsif 
 *  b2	SP	Parrallel / Serial mode selection
 *  	0 -> Serial mode
 *  	1 -> Parralle mode
 *  b1	CLK_GATED	Output clock gated selection of tsif_clk
 *  	0 -> tsif_clk is gated
 *  	1 -> tsif_clk is non-gated
 *  b0	CLK_INV	Output clock edge selection of tsif_clk
 *  	0 -> tsif_data is valid at positive edge clock
 *  	1 -> tsif_data is valid at negative edge clock
 *  @param  tsif_ctrl  TS interface control.
 *  TsIfControl:
 *  b7  Reserve
 *  b6	FRACTIONAL_PKT	Allow MPEG fractional packet generated
 *  	0 -> Output packet is always 188 bytes
 *  	1 -> Output packet may not be 188 bytes when channel changed
 *  b5	SERIAL_SYNC  Control the width of TSIF_SYNC signal. Only valid in serial mode
 *  	0 -> SYNC duration = first 8 bit
 *  	1 -> SYNC duration = first 1 bit
 *  b4	DISABLE_HDR  Disable output header 'h47
 *  	0 -> Normal
 *  	1 -> Disable header 'h47 in TS
 *  b3	DATA_FLIP  Control data flip of TSIF_DATA[7:0]
 *  	0 -> Normal
 *  	1 -> Data flip of TSIF_DATA[7:0]
 *  b2	ERR_DROP_PKT  Drop packet if error packet occur
 *  	0 -> Normal
 *  	1 -> Drop packet if TSIF_ERR = 1'b1
 *  b1	ERR_SHORT  Define TSIF_ERR duration
 *  	0 -> Normal, TSIF_ERR duration is 188 bytes
 *  	1 -> TSIF_ERR duration is 1 byte
 *  b0	ERR_GATED  Define TSIF_ERR gated
 *  	0 -> TSIF_ERR is gated by data enable
 *  	1 -> TSIF_ERR is not gated between packet
 *  @param  tsif_rate  TS interface clock rate. Output data rate = (48MHz/(TSIF_RATE+1)). The value should be odd number.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBT_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate)
{
UCHAR ucPara[4];

	ucPara[0] = ccCMD_ID_SET_TSIF;
	ucPara[1] = tsif_format;
	ucPara[2] = tsif_ctrl;
	ucPara[3] = tsif_rate;
	
	psDemodCtx->ucTsIfFormat = ucPara[1];
	psDemodCtx->ucTsIfCtrl = ucPara[2];
	psDemodCtx->ucTsIfRate = ucPara[3];
	vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TSIF_SIZE+1);
}

//----------------------------------------------------------------------------- 
/** DVBT_SetTsOutputMode
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBT_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel)
{
#if 1
	UINT8 RegAddr,uValue;
    //set TS serial/paral
    
    RegAddr=0x30;
    ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );
    mcSHOW_DBG_MSG(("DTMB_SetTsOutputMode: fgParallel = %d\n", fgParallel));
    mcSHOW_DBG_MSG(("DTMB_SetTsOutputMode: read 0x30 = 0x%02X\n", uValue));


	if (fgParallel)
	uValue &= 0xFE;//0 is parallel and 1 is serial
	else
	uValue |= 0x01;	

    mcSHOW_DBG_MSG(("DTMB_SetTsOutputMode: write 0x30 = 0x%02X\n", uValue));	
    ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );

#endif

#if 0
UCHAR ucData;

    DVBT_GetDemodReg(psDemodCtx, MT5391_REG_TSIF_CTRL, &ucData, 1);
    mcSHOW_DBG_MSG(("DVBT_SetTsOutputMode: fgParallel = %d\n", fgParallel));
    mcSHOW_DBG_MSG(("DVBT_SetTsOutputMode: read 0x192 = 0x%02X\n", ucData));
    
    if (fgParallel)
    	ucData |= 0x04;
    else
        ucData &= 0xfB;
    
    mcSHOW_DBG_MSG(("DVBT_SetTsOutputMode: write 0x192 = 0x%02X\n", ucData));
    DVBT_SetDemodReg(psDemodCtx, MT5391_REG_TSIF_CTRL, &ucData, 1);
#endif
}

//----------------------------------------------------------------------------- 
/** DVBT_SetTsPathMode
 *  Set TS path mode
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable)
{
//UINT32 u4Temp;

#if 0

    if ((DVBT_GetSyncAfterAcq(psDemodCtx) == DVBT_DEMOD_STATE_UNLOCK) && (fgEnable == TRUE)) {
        mcSHOW_DBG_MSG(("DVBT_SetTsOutputMode : Can't enable TS due to demod unlock\n"));
        return DVBT_DEMOD_STATE_UNLOCK;
    }
#endif

UINT8 uValue, RegAddr;

    RegAddr=0x14;
    ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );
	
    mcSHOW_DBG_MSG(("DTMB_SetTsPathMode: fgEnable = %d\n", fgEnable));
    mcSHOW_DBG_MSG(("DTMB_SetTsPathMode: read 0x14 = 0x%08X\n", uValue));
    
    if (fgEnable)
    	uValue &= 0x7F;
    else
        uValue |= 0x80;
    
    mcSHOW_DBG_MSG(("DTMB_SetTsPathMode: write 0x14 = 0x%08X\n", uValue));
    ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );
//#endif

    return DTMB_DEMOD_STATE_LOCK;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DVBT_GetTsFmt(DEMOD_CTX_T *psDemodCtx)
{
//UCHAR     ucClk;
TS_FMT_T fgTsFmt;
UINT8 RegAddr, uValue;

    RegAddr=0x30;
    ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );

    if (uValue & 0x01)
        fgTsFmt.SerOrPar = SP_SERIAL;
    else
        fgTsFmt.SerOrPar = SP_PARALLEL;

    if (uValue & 0x04)
        fgTsFmt.fgGatedClock = BOOL_FALSE;
    else
        fgTsFmt.fgGatedClock = BOOL_TRUE;

    if (uValue & 0x02)
	    fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;
    else
        fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;

    RegAddr=0x14;
    ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );
    if (uValue & 0x80)
        fgTsFmt.fgTristate = BOOL_TRUE;
    else
        fgTsFmt.fgTristate = BOOL_FALSE;

	//TS clock unknow now
	fgTsFmt.u4TsClockInKHz = 0;
	#if 0
    DVBT_GetReg(psDemodCtx, 0x201, &ucClk, 1);
    if (ucData & 0x04)
    {
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 54000/(ucClk+1);
    }
    else
    {
        /* Serial */
        fgTsFmt.u4TsClockInKHz = 54000*8/(ucClk+1);
    }
	#endif

    fgTsFmt.fgExternalDemod = BOOL_TRUE;
    mcSHOW_DBG_MSG3(("SerOrPar=%d, GatedClock=%d, Tristate=%d\n",fgTsFmt.SerOrPar,fgTsFmt.fgGatedClock,fgTsFmt.fgTristate))

    return fgTsFmt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetSeachParam
 *  Set system parameters of channel scan or channel up/down mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  search_mode  search mode select. (CH_SEARCH_BLIND : search mode, CH_SEARCH_UPDOWN : channel up/down mode).
 *  @param  mdMode  MD mode select.
 *  b・7: Enable 2k mode
 *  b・6: Enable 4k mode
 *  b・5: Enable 8k mode
 *  [b・4:b・2] GI
 *   000: 1/32
 *   001: 1/16
 *   010: 1/8
 *   011: 1/4
 *  [b・1:b・0] Spectrum inversion mode
 *   01: inverted
 *   10: non-inverted
 *  @param  tpsCr  TPS Code Rate.
 *  b・7: 0
 *  b・6: 0
 *  [b・5:b・3] TPS LP code rate info
 *   000: 1/2
 *   001: 2/3
 *   010: 3/4
 *   011: 5/6
 *   100: 7/8
 *   101~111: Reserved
 *  [b・2:b・0] TPS HP code rate info
 *   000: 1/2
 *   001: 2/3
 *   010: 3/4
 *   011: 5/6
 *   100: 7/8
 *   101~111: Reserved
 *  @param  tpsCons  TPS constellation.
 *  0: QPSK
 *  1: 16QAM
 *  2: 64QAM
 *  Other values: Reserved
 *  @param  tpsHier  TPS hierarchy.
 *  0: native, non-hierarchy
 *  1: native, alpha = 1
 *  2: native, alpha = 2
 *  3: native, alpha = 4
 *  4: indepth, non-hierarchy
 *  5: indepth, alpha = 1
 *  6: indepth, alpha = 2
 *  7: indepth, alpha = 4
 *  Other values: Reserved
 *  @param  freqBias_40kHz  Frequency bias in 40kHz. (-18~+18).
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBT_SetSearchParam(DEMOD_CTX_T *psDemodCtx, UCHAR search_mode, UCHAR mdMode, 
						  UCHAR tpsCr, UCHAR tpsCons, UCHAR tpsHier, INT8 freqBias_40kHz)
{
	psDemodCtx->u1ChSearchMode = search_mode;
	psDemodCtx->u1MdMode  = mdMode;
	psDemodCtx->u1TpsCr   = tpsCr;
	psDemodCtx->u1TpsCons = tpsCons;
	psDemodCtx->u1TpsHier = tpsHier;
	psDemodCtx->s1FreqBias40khz = freqBias_40kHz;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetHpLp
 *  Set HP/LP.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucHpLpSel  HP/LP selection. (EN_HP_DECODE/EN_LP_DECODE).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DVBT_SetHpLp(DEMOD_CTX_T *psDemodCtx, UCHAR ucHpLpSel)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

	mccSET_CMD2(aucData) = ccCMD_ID_SET_HPLP;
	mccSET_QUEUE2(aucData, ccOFST_FAST_SET_HPLP) = ucHpLpSel;   
	mcHOST_CMD(psDemodCtx, aucData, ccOFST_FAST_SET_HPLP_RSIZE, ccOFST_FAST_SET_HPLP_TSIZE + 1);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetIndepth
 *  Get inner interleaver mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : native mode.
 *  @retval   1 : indepth mode.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetIndepth(DEMOD_CTX_T *psDemodCtx)
{
	if (!psDemodCtx)
		return 0;

	return psDemodCtx->u1Indepth;
}


//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetHier
 *  Get hierarchical mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : non-hierarchical.
 *  @retval   1 : hierarchical, alpha = 1.
 *  @retval   2 : hierarchical, alpha = 2.
 *  @retval   3 : hierarchical, alpha = 4.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetHierMode(DEMOD_CTX_T *psDemodCtx)
{
	if (!psDemodCtx)
		return 0;

	return psDemodCtx->u1Hier;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetHpLp
 *  Get the mode of current decoding stream.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : HP(high priority) stream or non-hierarchical mode.
 *  @retval   1 : LP(low  priority) stream.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetHpLp(DEMOD_CTX_T *psDemodCtx)
{
	if (!psDemodCtx)
		return 0;

	return psDemodCtx->u1HpLp;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetHier
 *  Get hierarchical mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   DVBT_HIR_PRIORITY_UNKNOWN : unknown type.
 *  @retval   DVBT_HIR_PRIORITY_NONE : non-hierarchical.
 *  @retval   DVBT_HIR_PRIORITY_HIGH : hierarchical, HP stream.
 *  @retval   DVBT_HIR_PRIORITY_LOW  : hierarchical, LP stream.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetHier(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1Hierarchy = DVBT_HIR_PRIORITY_UNKNOWN;

	if (!psDemodCtx)
		return 0;

    if (DVBT_GetHierMode(psDemodCtx) == 0){
        u1Hierarchy = DVBT_HIR_PRIORITY_NONE;
    }
    else{
        if (DVBT_GetHpLp(psDemodCtx) == 0) {
            u1Hierarchy = DVBT_HIR_PRIORITY_HIGH;
        }
        else{
            u1Hierarchy = DVBT_HIR_PRIORITY_LOW;
        }    
    }

	return u1Hierarchy;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetDvbtNotExist
 *  Check if really no DVB-T signal exists.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   TRUE  : It's confirmed that no DVB-T signal exists.   
 *  @retval   FALSE : DVB-T signal exists or it's not confirmed yet.
 */
//-----------------------------------------------------------------------------
BOOL DVBT_GetDvbtNotExist(DEMOD_CTX_T *psDemodCtx)
{
	if (!psDemodCtx)
		return FALSE;	
	
	if ((psDemodCtx->u1ControlStatus > 4) && (psDemodCtx->ucMdFftMode == 3))
		return TRUE;
	else
		return FALSE;
}



//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetPreVBer
 *  Get pre Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   Pre Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBT_GetPreVBer(DEMOD_CTX_T *psDemodCtx) // return value = PreVBer * 10^5
{
	INT32 s4PreVBer;

	if (!psDemodCtx)
		return -1;

    // PreVBer = (VdErrBit*4 / VdTotBit*16)
    // 25000 = 10^5 *4 /16
    if (psDemodCtx->u2VdTotBit <= 0) {
        s4PreVBer = -1;
    }
    else {
        s4PreVBer  = (S32)(psDemodCtx->u2VdErrBit) * 25000;
        s4PreVBer /= (S32)(psDemodCtx->u2VdTotBit);
    }

    return (s4PreVBer);
}


//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBT_GetPostVBer(DEMOD_CTX_T *psDemodCtx) // return value = PostVBer * 10^5
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
 *  DVBT_GetSearchParam
 *  Get and update system parameters of channel scan or channel up/down mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  mdMode  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  tpsCr  Pointer of code rate value.
 *  @param  tpsCons  Pointer of TPS value.
 *  @param  tpsHier  Pointer of hierarchical mode value.
 *  @param  freqBias_40kHz  Pointer of frequency offset in 40kHz .
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBT_GetSearchParam(DEMOD_CTX_T *psDemodCtx, UCHAR *mdMode, UCHAR *tpsCr, 
						   UCHAR *tpsCons, UCHAR *tpsHier, INT8 *freqBias_40kHz)
{
	vDTMBGetPara(psDemodCtx);
	vDTMBGetInfo(psDemodCtx);

	*mdMode = 0;
	// FFT
	switch (psDemodCtx->u1Fft)
	{
	case 0:   // 2k
		mcSET_BIT(*mdMode, 7);
		break;
	case 1:   // 8k
		mcSET_BIT(*mdMode, 5);
		break;
	case 2:   // 4k
		mcSET_BIT(*mdMode, 6);
		break;
	default:
		mcSET_BIT(*mdMode, 7);
		break;
	}
	// GI
	mcSET_FIELD(*mdMode, psDemodCtx->u1GI, 0x1C, 2);
	// SI
	mcSET_FIELD(*mdMode, psDemodCtx->u1Si, 0x03, 0);

	*tpsCr = psDemodCtx->u1HpCR;
	mcSET_FIELD(*tpsCr, psDemodCtx->u1LpCR, MT5391_MASK_TPS_LP_CR, 3);

	*tpsCons = psDemodCtx->u1Mod;
	*tpsHier = psDemodCtx->u1Hier;
	*freqBias_40kHz = DVBT_GetFreqOffset_kHz(psDemodCtx)/256/40;
}

/**********************************************************************/
// This function will get SNR immediately from MT5133, you don't need
// to call DVBT_GetInfo in advance
// return value ranges from 0~255 (mapping from 0~31.5dB)
//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetSnrInfo
 *  Get SNR value, need not to call DVBT_GetInfo in advance.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   SNR * 8(dB).   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_GetSnrInfo(DEMOD_CTX_T *psDemodCtx)
{
UINT8	uValue;

#if 0

INT16	s2_data;
UINT8   aucData[cMAX_READ_NUM + 1];
UINT8   uc_data[2];


	if (!psDemodCtx)
		return 0;

	mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_INFO10;
	if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DEMOD_INFO10_SIZE, 1))
	{
		// SNR, snr*512
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_SNR_CSI_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DEMOD_INFO10_SNR_CSI_H);
		psDemodCtx->s2Snr = (S16) mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
	}

	s2_data = (psDemodCtx->s2Snr) / 64;   // 0~255 (0~31.5dB)
	if (s2_data < 0)
		return 0;
	else
		return (UINT8) s2_data;
#endif

		if (!psDemodCtx)
		return 0;

	uValue=0x0D;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, 0xFF, &uValue, 1);
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS2, 0x5D, &uValue, 1);

	psDemodCtx->s2Snr=uValue;
	mcSHOW_DBG_MSG2(("Current SNR = %d dB", psDemodCtx->s2Snr));

	if (psDemodCtx->s2Snr < 0)
		return 0;
	else
		return (UINT8) psDemodCtx->s2Snr;
}

//----------------------------------------------------------------------------- 
/*
 *  can be removed!!!
 */
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetConnType
 *  Set connection type.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  type  Connection type. (Default is 0).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DVBT_SetConnType(DEMOD_CTX_T *psDemodCtx, UINT8 type)
{
	psDemodCtx->ucConnType = type;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetPowerMode
 *  Set power mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucMode  Unused.
 *  @retval   0. 
 */
//-----------------------------------------------------------------------------
UCHAR DVBT_SetPowerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode)
{
	return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetTunerMode
 *  Set tuner mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucMode  Unused for can tuner.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DVBT_SetTunerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode)
{
	psDemodCtx->m_TunerMode = ucMode;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetI2cMasterClock
 *  Set I2C master clock.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  clk_div  clk=48MHz/clk_div, clk_div total 12bits.
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
// clk = 48MHz/clk_div, clk_div total 12bits
void DVBT_SetI2cMasterClock(DEMOD_CTX_T *psDemodCtx, UINT16 clk_div)
{
UINT16 u2_data = 0;
UCHAR   aucData[cMAX_READ_NUM + 1];

	u2_data = clk_div & 0x0fff;
	mccSET_CMD2(aucData) = ccCMD_ID_I2C_CLK_DIV;
	mccSET_QUEUE2(aucData, ccOFST_I2C_CLK_DIV_H) = mcHIGH_BYTE(u2_data);
	mccSET_QUEUE2(aucData, ccOFST_I2C_CLK_DIV_L) = mcLOW_BYTE( u2_data);
	mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_I2C_CLK_DIV_SIZE+1);
}

#if 0
//----------------------------------------------------------------------------- 
/*
 *  DVBT_NormalMode
 *  Set Demod back to normal mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : Success.   
 *  @retval   1 : Fail.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_NormalMode(UINT8 i2cDeviceAddr)
{
UCHAR	ucData = 0;

	// switch to bank 1
	ucData = 1;
	if (ucI2cWrite(i2cDeviceAddr, 0xFF, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0xB4, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 3);
	if (ucI2cWrite(i2cDeviceAddr, 0xB4, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0xB5, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 7);
	if (ucI2cWrite(i2cDeviceAddr, 0xB5, &ucData, 1))
		return 1;

	mcDELAY_MS(1);

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0xBE, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 7);
	if (ucI2cWrite(i2cDeviceAddr, 0xBE, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x48, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 6);
	if (ucI2cWrite(i2cDeviceAddr, 0x48, &ucData, 1))
		return 1;
	
	// set TS continuos clock
	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x92, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 1);
	if (ucI2cWrite(i2cDeviceAddr, 0x92, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x43, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 7);
	mcSET_MASK(ucData, 0x07);
	if (ucI2cWrite(i2cDeviceAddr, 0x43, &ucData, 1))
		return 1;

	// switch to bank 0
	ucData = 0;
	if (ucI2cWrite(i2cDeviceAddr, 0xFF, &ucData, 1))
		return 1;
	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x00, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 0);
	if (ucI2cWrite(i2cDeviceAddr, 0x00, &ucData, 1))
		return 1;
		
	mcDELAY_MS(20);

	return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_StandbyMode
 *  Set Demod to standby mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   0 : Success.   
 *  @retval   1 : Fail.
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_StandbyMode(UINT8 i2cDeviceAddr)
{
UCHAR	ucPara[4];
UCHAR	ucData = 0;

	// set IF AGC curve
	// switch to bank 1
	ucData = 1;
	if (ucI2cWrite(i2cDeviceAddr, 0xFF, &ucData, 1))
		return 1;
	// write data
	ucPara[0] = 0x20;   // SLP
	ucPara[1] = 0x00;   // INT
	ucPara[2] = 0x80;   // MAX
	ucPara[3] = 0x80;   // MIN
	if (ucI2cWrite(i2cDeviceAddr, 0xAC, ucPara, 4))
		return 1;

	// set TS gated clock
	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x92, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 1);
	if (ucI2cWrite(i2cDeviceAddr, 0x92, &ucData, 1))
		return 1;

	// switch to bank 0
	ucData = 0;
	if (ucI2cWrite(i2cDeviceAddr, 0xFF, &ucData, 1))
		return 1;
	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x00, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 0);
	if (ucI2cWrite(i2cDeviceAddr, 0x00, &ucData, 1))
		return 1;

	// switch to bank 1
	ucData = 1;
	if (ucI2cWrite(i2cDeviceAddr, 0xFF, &ucData, 1))
		return 1;	
	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x43, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 7);
	mcCLR_MASK(ucData, 0x07);
	if (ucI2cWrite(i2cDeviceAddr, 0x43, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0x48, &ucData, 1))
		return 1;
	mcCLR_BIT(ucData, 6);
	if (ucI2cWrite(i2cDeviceAddr, 0x48, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0xBE, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 7);
	if (ucI2cWrite(i2cDeviceAddr, 0xBE, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0xB5, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 7);
	if (ucI2cWrite(i2cDeviceAddr, 0xB5, &ucData, 1))
		return 1;

	// write data
	if (ucI2cRead(i2cDeviceAddr, 0xB4, &ucData, 1))
		return 1;
	mcSET_BIT(ucData, 3);
	if (ucI2cWrite(i2cDeviceAddr, 0xB4, &ucData, 1))
		return 1;

	return 0;
}


#endif


//----------------------------------------------------------------------------- 
/*
 *  DVBT_DumpRegisters
 *  Dump Demod registers.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  bank_no  Register bank number.
 *  @param  max_i2c_len  Max I2C read byte at one time
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len)
{
UINT16  u2CurI2cLen, u2CurAddr, u2StartAddr = 0;
UINT16	u2CurLen = 256;

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

		if (DVBT_GetReg(psDemodCtx, u2StartAddr + u2CurAddr,
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
/*
 *  DVBT_SetNormalMode
 *  Set demod to normal mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_SetNormalMode(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 fgRet = 0;
    UINT8 ucData = 0x00;
    //U32   u4Temp;
#if 1    
    mcSHOW_DBG_MSG(("PI DVBT Set Normal Mode!!!\n"));  
    u1AfterAcq = 1;
    mcSHOW_DBG_MSG(("DVBT_SetNormalMode : u1AfterAcq = %d\n", u1AfterAcq));
   
    // power up AD clock
    /*u4Temp = mcRISC_Reg(CKGEN_BASE, 0x1d0);
    u4Temp &= 0xffffff1f;
    mcSET_SHOW_REG(CKGEN_BASE, 0x1d0, u4Temp);  */
    // will set @ DVBT_ChipInit()

    //Move to the beginning of TunerAcq(), Menghau, 090724
    //DVBT_ChipInit();
       
    // power up dvbt modules (paired with DVBT_Disconnect), 0x302, b'0 = 0
    DVBT_GetReg(psDemodCtx, MT5391_REG_DVBT_PDN, &ucData, 1);
    ucData &= 0xfe;
    DVBT_SetReg(psDemodCtx, MT5391_REG_DVBT_PDN, &ucData, 1);
 
    //No need in MT5363,  move to ChipInit
    // Enable DVBT and Disable ATD, LC, write 0x1b4 to 0x02
    //mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b0, 0x00000002);
    //mcSET_SHOW_REG(cRISC_DVBT_BASE, 0x4b4, 0x4001006d);
    //DVBT_GetReg(psDemodCtx, MT5391_REG_ANA_PD_MODE, &ucData, 1);
    //ucData &= 0xEF;   // b'4 = 0
    //DVBT_SetReg(psDemodCtx, MT5391_REG_ANA_PD_MODE, &ucData, 1);

    //set in ATD_Disconnect
    // disbale ATD, reset ATD (0x134 and 0x135)
    //UINT32 u4Temp;
    //u4Temp = mcRISC_Reg(cRISC_ATD_BASE, 0x134);
    //u4Temp &= 0xFFFF0000;
    //mcSET_SHOW_REG(cRISC_ATD_BASE, 0x134, u4Temp);

    /*
    // switch to IFAGC by DVBT demod
    DVBT_GetReg(psDemodCtx, 0x1B6, &ucData, 1);
    ucData |= 0x40;
    DVBT_SetReg(psDemodCtx, 0x1B6, &ucData, 1);
    */
            
    // switch to RFAGC by tuner (internal ATD use in MT5360A from LC)
    // Removed by Ken for customer use from LC. GPIO may need to be cusomized and use 
    // RVCUST_OptQuery(). Take vResetDemod() in pd_glue.c for one example, 081030
    //GPIO_SetOut(SERVO_GPIO5, 1);
    
    // switch to DVBT mode
    ucData = 0x02;
    DVBT_SetReg(psDemodCtx, 0x93E, &ucData, 1);

    // enable DVBT modules
    ucData = 0x77;
    DVBT_SetReg(psDemodCtx, MT5391_REG_CORE_CONTROL, &ucData, 1);

#endif
    return fgRet;
}

//retrun TURE is success
UINT8 DTMB_SetAutoDetect(DEMOD_CTX_T *psDemodCtx)
{
	UINT8 uValue, RegAddr; 


	//software reset
	RegAddr = 0x02;
    if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	uValue &= 0xFE;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	uValue |= 0x01;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

#if !SUPPORT_DISABLE_DEMOD_CLOCK
	if(fgBreak)
	{
		 mcSHOW_HW_MSG(("Ever break!!!!!!!!!\n"));
		 uValue=0x00;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
		 mcDELAY_MS(50);
		 
	     uValue=0x00;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
		 uValue=0x02;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset disable
		 uValue=0x03;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset enable
		 mcDELAY_MS(50);
		 
		 fgBreak = FALSE;
	}
#endif

	//mcDELAY_MS(200);

    //inital demod
	uValue = (UINT8)0x86;
	RegAddr = 0xC3;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

#if !fgIssharp2403
	uValue = (UINT8)0xB2;
	RegAddr = 0x30;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x30, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
#endif


	RegAddr = 0x07;
    if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	mcSHOW_HW_MSG(("writing 0x07=0x%02x++++++++++\n",uValue));
	uValue &= 0xFD;//need to verify?????????????????
	mcSHOW_HW_MSG(("writing 0x07=0x%02x+++++++++++\n",uValue));
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	RegAddr = 0xCF;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	uValue |= 0x80;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
#if SUPPORT_DISABLE_DEMOD_CLOCK
    //disable FW
    uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
    mcDELAY_MS(20);
    //enable FW
	uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
	uValue=0x02;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset disable
	uValue=0x03;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset enable
#endif
	
	mcDELAY_MS(20);

    //set auto detect	
	uValue = 0x0D;
	RegAddr = 0xFF;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	uValue = 0x01;
	RegAddr = 0x56;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;


	//enable PN auto and auto detect
	RegAddr = 0x0C;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	
	uValue |= 0x04;//enable PN auto
	uValue |= 0x80;//enable auto detect
	uValue &= 0xDF;//disable AFC CFE
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	//uValue |= 0x80;//enable auto detect
	//if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		//goto I2C_WRITE_FAIL;	


	RegAddr = 0x1E;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	uValue |= 0x40;//enable carrier  on
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;	

	return TRUE;
	
I2C_WRITE_FAIL:
		{
			//mcSHOW_USER_MSG(("SetAutoDetect Download I2C fail(0x%02x)!\n",RegAddr));
			mcSHOW_DBG_MSG1(("SetAutoDetect Download I2C fail(0x%02x)!\n", RegAddr));
			return FALSE;
        }

}

//retrun TURE is success
UINT8 DTMB_SetManualSwitch(DEMOD_CTX_T *psDemodCtx)
{
	UINT8 uValue, RegAddr; 

#if !SUPPORT_DISABLE_DEMOD_CLOCK
	if(fgBreak)
	{
		 mcSHOW_HW_MSG(("Ever break!!!!!!!!!\n"));
		 uValue=0x00;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
		 mcDELAY_MS(50);
		 
	     uValue=0x00;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
		 uValue=0x02;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset disable
		 uValue=0x03;
		 ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset enable
		 mcDELAY_MS(50);
		 
		 fgBreak = FALSE;
	}
#else
    //disable FW
    uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
    mcDELAY_MS(20);
    //enable FW
	uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
	uValue=0x02;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset disable
	uValue=0x03;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU on,MCU reset enable
	mcDELAY_MS(20);
#endif

	//Set GuardIntvl
	mcSHOW_DBG_MSG2(("GuardIntvl is 0x%02X\n",psDemodCtx->u1GI));
	RegAddr = 0x0C;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	uValue = (uValue&0xFC)|(psDemodCtx->u1GI);
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

    //Set SubCarrier
	mcSHOW_DBG_MSG2(("SubCarrier is 0x%02X\n",psDemodCtx->u1Mod));
	RegAddr = 0x19;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	
	uValue = (uValue&0xC7)|(psDemodCtx->u1Mod<<3);
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	
	//Set FecRate
	mcSHOW_DBG_MSG2(("FecRate is 0x%02X\n",psDemodCtx->u1FecRate));
	RegAddr = 0x19;
	 if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		 goto I2C_WRITE_FAIL;

	uValue = (uValue&0xF9)|(psDemodCtx->u1FecRate<<1);
	 if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		 goto I2C_WRITE_FAIL;

	//Set TimeDeintvl
	mcSHOW_DBG_MSG2(("TimeDeintvl is 0x%02X\n",psDemodCtx->u1TimDeInterl));
	RegAddr = 0x19;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	
	uValue = (uValue&0xBF)|(psDemodCtx->u1TimDeInterl<<6);
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;


	//Set CarrierMode
	mcSHOW_DBG_MSG2(("CarrierMode is 0x%02X\n",psDemodCtx->u1CarrierMode));
	RegAddr = 0x19;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	uValue = (uValue&0xFE)|(psDemodCtx->u1CarrierMode);
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	//Set PnNumber
	mcSHOW_DBG_MSG2(("PnNumber is 0x%02X\n",psDemodCtx->u1PnNumber));
	RegAddr = 0x18;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	
	uValue = (uValue&0xFE)|(psDemodCtx->u1PnNumber);
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	//Disable PN auto and auto detect
	RegAddr = 0x0C;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	
	//uValue &= 0xFB;//disable PN auto
	uValue |= 0x04;//enable PN auto
	//if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		//goto I2C_WRITE_FAIL;
	uValue &= 0x7F;//disable auto detect
	uValue |= 0x20;//enable  AFC CFE 
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;


	RegAddr = 0x1E;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	uValue &= 0xBF;//disable carrier  on
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	
	//software reset
	RegAddr = 0x02;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	uValue &= 0xFE;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;
	uValue |= 0x01;
	if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		goto I2C_WRITE_FAIL;

	
	return TRUE;
	
I2C_WRITE_FAIL:
		{
			//mcSHOW_USER_MSG(("SetAutoDetect Download I2C fail(0x%02x)!\n",RegAddr));
			mcSHOW_DBG_MSG1(("SetAutoDetect Download I2C fail(0x%02x)!\n", RegAddr));
			return FALSE;
        }

}
void DTMB_SetRegSaved(DEMOD_CTX_T *psDemodCtx)
{
	UINT8 RegAddr; 

	mcSHOW_DBG_MSG3(("Set saved registers first in connect!\n"));

	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x07, &Reg_3A_07, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x19, &Reg_3A_19, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x1C, &Reg_3A_1C, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x08, &Reg_3A_08, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x09, &Reg_3A_09, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x0A, &Reg_3A_0A, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x0B, &Reg_3A_0B, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x10, &Reg_3A_10, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x11, &Reg_3A_11, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x12, &Reg_3A_12, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x15, &Reg_3A_15, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x16, &Reg_3A_16, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x17, &Reg_3A_17, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x4B, &Reg_3A_4B, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x4C, &Reg_3A_4C, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x4D, &Reg_3A_4D, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x7E, &Reg_3A_7E, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS1, 0x7F, &Reg_3A_7F, 1 );

	RegAddr = 0x0A;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xff, &RegAddr, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB1, &Reg_3E_0A_B1, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB0, &Reg_3E_0A_B0, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB3, &Reg_3E_0A_B3, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB2, &Reg_3E_0A_B2, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB5, &Reg_3E_0A_B5, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB4, &Reg_3E_0A_B4, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB7, &Reg_3E_0A_B7, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB6, &Reg_3E_0A_B6, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB9, &Reg_3E_0A_B9, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xB8, &Reg_3E_0A_B8, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xBB, &Reg_3E_0A_BB, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xBA, &Reg_3E_0A_BA, 1 );

	RegAddr = 0x0C;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xff, &RegAddr, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x22, &Reg_3E_0C_22, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x42, &Reg_3E_0C_42, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x4C, &Reg_3E_0C_4C, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x24, &Reg_3E_0C_24, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x44, &Reg_3E_0C_44, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x4E, &Reg_3E_0C_4E, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x26, &Reg_3E_0C_26, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x46, &Reg_3E_0C_46, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x50, &Reg_3E_0C_50, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x48, &Reg_3E_0C_48, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x52, &Reg_3E_0C_52, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x20, &Reg_3E_0C_20, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x2A, &Reg_3E_0C_2A, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x2C, &Reg_3E_0C_2C, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x2E, &Reg_3E_0C_2E, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x30, &Reg_3E_0C_30, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x1A, &Reg_3E_0C_1A, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x1C, &Reg_3E_0C_1C, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x00, &Reg_3E_0C_00, 1 );
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0x02, &Reg_3E_0C_02, 1 );

	
}
//----------------------------------------------------------------------------- 
void DTMB_SaveReg(DEMOD_CTX_T *psDemodCtx)
{
	UINT8 RegAddr;
	mcSHOW_DBG_MSG3(("Save Regsters before disconnect!\n"));
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x07, &Reg_3A_07, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x19, &Reg_3A_19, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x1C, &Reg_3A_1C, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x08, &Reg_3A_08, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x09, &Reg_3A_09, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x0A, &Reg_3A_0A, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x0B, &Reg_3A_0B, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x10, &Reg_3A_10, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x11, &Reg_3A_11, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x12, &Reg_3A_12, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x15, &Reg_3A_15, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x16, &Reg_3A_16, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x17, &Reg_3A_17, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x4B, &Reg_3A_4B, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x4C, &Reg_3A_4C, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x4D, &Reg_3A_4D, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x7E, &Reg_3A_7E, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS1, 0x7F, &Reg_3A_7F, 1 );

	RegAddr = 0x0A;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xff, &RegAddr, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB0, &Reg_3E_0A_B0, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB1, &Reg_3E_0A_B1, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB2, &Reg_3E_0A_B2, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB3, &Reg_3E_0A_B3, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB4, &Reg_3E_0A_B4, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB5, &Reg_3E_0A_B5, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB6, &Reg_3E_0A_B6, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB7, &Reg_3E_0A_B7, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB8, &Reg_3E_0A_B8, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xB9, &Reg_3E_0A_B9, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xBA, &Reg_3E_0A_BA, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0xBB, &Reg_3E_0A_BB, 1 );

	RegAddr = 0x0C;
	ICtrlBus_I2cDemodWrite( 0x100, cLGS8G8X_ADDRESS2, 0xff, &RegAddr, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x22, &Reg_3E_0C_22, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x42, &Reg_3E_0C_42, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x4C, &Reg_3E_0C_4C, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x24, &Reg_3E_0C_24, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x44, &Reg_3E_0C_44, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x4E, &Reg_3E_0C_4E, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x26, &Reg_3E_0C_26, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x46, &Reg_3E_0C_46, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x50, &Reg_3E_0C_50, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x48, &Reg_3E_0C_48, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x52, &Reg_3E_0C_52, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x20, &Reg_3E_0C_20, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x2A, &Reg_3E_0C_2A, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x2C, &Reg_3E_0C_2C, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x2E, &Reg_3E_0C_2E, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x30, &Reg_3E_0C_30, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x1A, &Reg_3E_0C_1A, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x1C, &Reg_3E_0C_1C, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x00, &Reg_3E_0C_00, 1 );
	ICtrlBus_I2cDemodRead( 0x100, cLGS8G8X_ADDRESS2, 0x02, &Reg_3E_0C_02, 1 );
	
	
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_Connect
 *  Start demod module to acquire to a channel.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW)
{
	#if 0
    UINT8 fgRet = 0;
    mcSHOW_DBG_MSG(("PI DTMB Connect!!\n"));

    // Chip init
    DVBT_ChipInit();
    
    // stop driver
    DVBT_DriverStop(psDemodCtx);
    //mcDELAY_MS(10);
    
    fgRet = DVBT_SetNormalMode(psDemodCtx);

    // stop driver
    //DVBT_DriverStop(psDemodCtx);   
    // set BW
    DVBT_SetBW(psDemodCtx, ucBW);
    // driver start
    DVBT_DriverStart(psDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);

    //mcDELAY(5);
	
	 return fgRet;
	#else
	UINT8 RegAddr, uValue, SetAutoRet, SetManualSwitch ;

	#if SUPPORT_DISABLE_DEMOD_CLOCK
	//hw reset end
	UINT32 DEMOD_RESET = DRVCUST_OptGet(eDemodResetGpio);
	GPIO_SetOut(DEMOD_RESET, 1);
	
	//enable demod clock and if AGC switch
	//please selest a gpio and implement gpio code here
	
	mcDELAY_MS(100);
	#endif

	
#if (!defined(CC_MT5365)) && (!defined(CC_MT5395))
	UINT32 IF_AGC_SWITCH = DRVCUST_OptGet(eDTMBIFAGCGpio);
	
	mcSHOW_DBG_MSG3(("PI DTMB Connect!!\n"));
	
	mcSHOW_DBG_MSG3(("Connect digital IF AGC (GPIO%d)\n", IF_AGC_SWITCH));
	GPIO_SetOut(IF_AGC_SWITCH, GPIO_OUTPUT_OFF); // connect digital IF AGC(gpio switch)
#else
	mcSHOW_DBG_MSG3(("PI DTMB Connect!!\n"));

	// set dtmb some register saved before connect dvbc
	DTMB_SetRegSaved(psDemodCtx);

	RegAddr = 0x14;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );
	uValue &= 0xFB;//enable GPIO1
	uValue &= 0xF7;//GPIO1 output login 0
	mcSHOW_DBG_MSG3(("Connect digital IF AGC GPIO1(0x14) = 0x%02x\n", uValue));
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );

#endif
	
    //Enable IF AGC
	RegAddr = 0x07;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
	{
		mcSHOW_DBG_MSG3(("DTMB connect enable IFAGC read (0x07)fail!"));
		return 1;
	}
    else
    {
    	mcSHOW_DBG_MSG1(("DTMB connect enable IFAGC read (0x07==0x%02x)!\n",uValue));
    }

	uValue |= 0x10;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );

	if(psDemodCtx->fgAutoScan == TRUE)
	{
		//set Auto detect
		SetAutoRet=DTMB_SetAutoDetect(psDemodCtx);

		if(!SetAutoRet)
		{
			mcSHOW_USER_MSG(("AutoDetect set fail!\n"));
			return 1;
		}	
	}
	else
	{   //set manual switch
		SetManualSwitch=DTMB_SetManualSwitch(psDemodCtx);
		if(!SetManualSwitch)
		{
			mcSHOW_USER_MSG(("AutoDetect set fail!\n"));
			return 1;
		}

	}
	return 0;
	
	#endif

}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_DisConnect
 *  power down demod module.  
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 */
//-----------------------------------------------------------------------------
VOID DTMB_DisConnect(DEMOD_CTX_T *psDemodCtx)
{
#if 0
    UCHAR ucData = 0x00;
    U32   u4Temp;
    mcSHOW_DBG_MSG(("PI DVBT Disconnect!!\n"));
    u1AfterAcq = 0;
    mcSHOW_DBG_MSG(("DVBT_DisConnect : u1AfterAcq = %d\n", u1AfterAcq));
    
    // stop driver
    DVBT_DriverStop(psDemodCtx);
    
    // disable AGC => AGC = 0V
    DVBT_SetReg(psDemodCtx, MT5391_REG_CORE_CONTROL, &ucData, 1);

    // delay for SB reset, at least one complete TS packet
    mcDELAY_MS(10);
    
    // AD cannot be power down for uP (if no external DTD, such as DVBC)
    // power down AD clock
    //u4Temp = mcRISC_Reg(CKGEN_BASE, 0x1d0);
    //u4Temp |= 0x000000e0;
    //mcSET_SHOW_REG(CKGEN_BASE, 0x1d0, u4Temp);  

    // set demod54m_ck to TVDPLL/10 and demod60m_ck to TVDPLL/9
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x21c);
    u4Temp &= 0xFFFFFFFc;
    u4Temp |= 0x00000001;
    mcSET_SHOW_REG(CKGEN_BASE, 0x21c, u4Temp);

    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x218);
    u4Temp &= 0xFFFFFFFc;
    u4Temp |= 0x00000001;
    mcSET_SHOW_REG(CKGEN_BASE, 0x218, u4Temp);

    // power down DVBT most modules except CTRL path clock (uP_CLK)
    DVBT_GetReg(psDemodCtx, MT5391_REG_DVBT_PDN, &ucData, 1);
    ucData |= 0x01;
    DVBT_SetReg(psDemodCtx, MT5391_REG_DVBT_PDN, &ucData, 1);

    // power down AD clock (if with external DTD, such as DVBC)
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x1d0);
    u4Temp |= 0x000000e0;
    mcSET_SHOW_REG(CKGEN_BASE, 0x1d0, u4Temp); 
#else

#if 1
	UINT8 RegAddr, uValue;

#if (!defined(CC_MT5365)) && (!defined(CC_MT5395))

	UINT32 IF_AGC_SWITCH = DRVCUST_OptGet(eDTMBIFAGCGpio);
	
		 
	mcSHOW_DBG_MSG3(("PI DTMB Disconnect!!\n"));

	mcSHOW_DBG_MSG3(("Disconnect digital IF AGC (GPIO%d)\n", IF_AGC_SWITCH));
	GPIO_SetOut(IF_AGC_SWITCH, GPIO_OUTPUT_ON); // disconnect digital IF AGC(gpio switch)
#else

	mcSHOW_DBG_MSG3(("PI DTMB Disconnect!!\n"));

	// save some register before connect dvbc
	DTMB_SaveReg(psDemodCtx);

	RegAddr = 0x14;
	ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );
	uValue &= 0xFB;//enable GPIO1
	uValue |= 0x08;//GPIO1 output login 1
	mcSHOW_DBG_MSG3(("Disconnect digital IF AGC GPIO1(0x14) = 0x%02x\n", uValue));
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );

#endif	

    //disable IF AGC
	RegAddr = 0x07;
	if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 ))
		mcSHOW_DBG_MSG3(("Disconnect disable IFAGC read (0x07)fail!");	)
    else
    {
    	mcSHOW_DBG_MSG3(("Disconnect disable IFAGC read (0x07==0x%02x)!\n",uValue));
    }

	uValue &= 0xEF;
	mcSHOW_DBG_MSG3(("Disconnect disable IFAGC write (0x07==0x%02x)!\n",uValue));
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, RegAddr, &uValue, 1 );

	#if SUPPORT_DISABLE_DEMOD_CLOCK
	//disable FW
	uValue=0x00;
	ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS1, 0x39, &uValue, 1);//MCU off,MCU reset disable
	//hw reset 
	UINT32 DEMOD_RESET = DRVCUST_OptGet(eDemodResetGpio);
	GPIO_SetOut(DEMOD_RESET, 0);

	//disable demod clock and if AGC switch
	//please selest a gpio and implement gpio code here
	
	#endif

#endif
		
#endif

}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_ManualGetSync
 *  Get demod lock status.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   DTMB_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DTMB_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DTMB_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DTMB_ManualGetSync(DEMOD_CTX_T *psDemodCtx)
{
	UINT8 DetectResult=DTMB_DEMOD_STATE_UNLOCK;
	//UINT16 LoopCnt = 300;//need to fine tune
	//UINT16 LoopCntManSwitch = 50;


		//for( ; LoopCntManSwitch > 0; LoopCntManSwitch--)
		//{
		 //   mcDELAY_MS(200);
			vDTMBGetInfo(psDemodCtx);
			if( (DVBT_GetPER(psDemodCtx)<= DTMB_SYNC_PER_THRESHOLD) && ((INT32)(psDemodCtx->u2TotalPkt)>1500))
			{
				DetectResult = DTMB_DEMOD_STATE_LOCK;
				mcSHOW_DBG_MSG2(("PER is smaller than threshold\n"));
				//break;
			}
			else
			{
				mcSHOW_DBG_MSG2(("Current PER is heavy\n"));
			}				
		
		//}	

	
	return DetectResult;

}

/*
 *  DTMB_GetSync
 *  Get demod lock status.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   DTMB_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DTMB_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DTMB_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DTMB_GetSync(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 uValue;
	UINT8 RegAddr, reg_5A, reg_56, DetectResult=DTMB_DEMOD_STATE_UNLOCK;
	//UINT16 LoopCnt = 300;//need to fine tune
	//UINT16 LoopCntManSwitch = 50;

	if(psDemodCtx->fgAutoScan == TRUE)
	{
		if( psDemodCtx->u1MWIntf != cMTAL_IF )
		{
			uValue = 0x0D;
			RegAddr = 0xFF;
			if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, RegAddr, &uValue, 1 ))
				goto I2C_WRITE_FAIL;

			RegAddr = 0x56;
			if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS2, RegAddr, &reg_56, 1 ))
				goto I2C_WRITE_FAIL;
			
			if(reg_56 & 0x02)
			{
				mcSHOW_DBG_MSG(("Auto Detect Finished!\n"));
				uValue = 0x0D;
				RegAddr = 0xFF;
				if(ICtrlBus_I2cDemodWrite(0x100, cLGS8G8X_ADDRESS2, RegAddr, &uValue, 1 ))
					goto I2C_WRITE_FAIL;
			
				RegAddr = 0x5A;
				if(ICtrlBus_I2cDemodRead(0x100, cLGS8G8X_ADDRESS2, RegAddr, &reg_5A, 1 ))
					goto I2C_WRITE_FAIL;

				if(!(reg_5A & 0x02))
				{
					DetectResult = DTMB_DEMOD_STATE_UNKNOWN;
					mcSHOW_DBG_MSG(("Demod Unlock!!!!!!!\n"));
				
				}
				else
				{
					DetectResult = DTMB_DEMOD_STATE_LOCK;
					mcSHOW_DBG_MSG(("Demod lock!!!!!!!\n"));
				}
			}
		}
		else//for MTAL demo polling demod status
		{
			vDTMBGetInfo(psDemodCtx);
			if(DVBT_GetPER(psDemodCtx)<= DTMB_SYNC_PER_THRESHOLD)
			{
				DetectResult = DTMB_DEMOD_STATE_LOCK;
				mcSHOW_DBG_MSG2(("PER is smaller than threshold\n"));
				//break;
			}
			else
			{
				mcSHOW_DBG_MSG2(("Current PER is heavy\n"));
			}
		}

	}
	else
	{
		//for( ; LoopCntManSwitch > 0; LoopCntManSwitch--)
		//{
		 //   mcDELAY_MS(200);
			vDTMBGetInfo(psDemodCtx);
			if(DVBT_GetPER(psDemodCtx)<= DTMB_SYNC_PER_THRESHOLD)
			{
				DetectResult = DTMB_DEMOD_STATE_LOCK;
				mcSHOW_DBG_MSG2(("PER is smaller than threshold\n"));
				//break;
			}
			else
			{
				mcSHOW_DBG_MSG2(("Current PER is heavy\n"));
			}				
		
		//}	
	}
	
	return DetectResult;

I2C_WRITE_FAIL:
		{
			mcSHOW_USER_MSG(("GetSyncStatus write/read fail"));
			return FALSE;
        }	
#if 0
    else
    {
    	vDTMBGetStatus(psDemodCtx); // update SC state, FFT & TPS lock
    	if (DTMB_GetDvbtNotExist(psDemodCtx))
    	{
        	u1DvbtDemodState = DVBT_DEMOD_STATE_UNLOCK;
    	}
    	else if (DVBT_GetTpsLock(psDemodCtx) || DVBT_GetTsLock(psDemodCtx)) 
    	{
        	u1DvbtDemodState = DVBT_DEMOD_STATE_LOCK;
    	}
    }
	
    return u1DvbtDemodState;
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetSyncAfterAcq
 *  Get demod lock status after TunerAcq.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   DVBT_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DVBT_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DVBT_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DVBT_GetSyncAfterAcq(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1DvbtDemodState = DTMB_DEMOD_STATE_UNLOCK;

    if (u1AfterAcq == 1) {
        if (DTMB_GetSync(psDemodCtx)== DTMB_DEMOD_STATE_LOCK) {
            u1DvbtDemodState = DTMB_DEMOD_STATE_LOCK;
        }
    }

    return u1DvbtDemodState;
}


//----------------------------------------------------------------------------- 
/** DVBT_GetSwVer
 *  Get SW version.
 *  @retval   string of SW version.
 */
//-----------------------------------------------------------------------------
CHAR*  DVBT_GetSwVer(void)
{
	return ((CHAR*)MT5391_DVBT_SW_VER);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_SetArCoef
 *  Set demod AR coefficient.  
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @param  ucData Value of AR coefficient
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_SetArCoef(DEMOD_CTX_T *psDemodCtx, UINT8 ucData)
{
BOOL fgRetStatus = TRUE;

    fgRetStatus = fgSetFwVars(psDemodCtx, MT5391_REG_VARYING_AUTO_AR_ON, &ucData, 1);

    if (fgRetStatus)
    	return 0;
    else
    	return 1;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBT_GetArCoef
 *  Get demod AR coefficient.  
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   AR value
 */
//-----------------------------------------------------------------------------
UINT8 DVBT_GetArCoef(DEMOD_CTX_T *psDemodCtx, UINT8 *pucData)
{
BOOL fgRetStatus = TRUE;

    fgRetStatus = fgGetFwVars(psDemodCtx, MT5391_REG_VARYING_AUTO_AR_ON, pucData, 1);

    if (fgRetStatus)
    	return 0;
    else
    	return 1;
}

