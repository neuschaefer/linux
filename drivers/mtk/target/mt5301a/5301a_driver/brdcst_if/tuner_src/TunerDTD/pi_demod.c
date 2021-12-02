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
/***********************************************************************/
/*                                                                     */
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: pi_demod.c,v $                                       */
/*                                                                     */
/***********************************************************************/
/***********************************************************************/
/*              Includes                                               */
/***********************************************************************/
#include "PI_Def_dbg.h"
#include "pi_demod.h"
#include "pd_api.h"
//#include "rf_tuner_api.h"

//#include "demod_tuner_interface.h"
#include "tuner_interface_if.h"

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
/* For SmartAnt Scan */
#define fgALGO                  1           // Choose scan flow
#if (fgALGO == 0 )
#define fcCHK_LOCK              0
#define fcCHK_SNR               0
#else
#define fcCHK_LOCK              1
#define fcCHK_SNR               1
#endif

#define cALL_DIRECT_NUM         16
#define cALL_GAIN_NUM           4
// Setting
#define cSCAN_DIRECT_NUM        16          // Scan Direct Num Setting
#define cSCAN_GAIN_NUM          2           // Scan Gain Num Setting

/***********************************************************************/
#define fcADD_AD_CONTROL        0

/***********************************************************************/
#if fcADD_AD_CONTROL
#include "sv_const.h"
#include "source_table.h"
#include "drv_adcset.h"
#endif

/***********************************************************************/
/*              Global Variables                                       */
/***********************************************************************/
#if fcSHOW_HOST_CMD
//UCHAR   fgShowHostCmd = FALSE;
UCHAR   fgShowHostCmd = TRUE;
#endif

BOOL fgEnableI2C_Flag = FALSE;
/***********************************************************************/
/*              Tuner type mapping list                                */
/***********************************************************************/
/* Mapped to Tuner\PI_Def.h\LIST_TUNER_ATSC_T */
UINT8 au1TunerTypeMap[] =
{
    MT5112_TUNER_PHILIPS_TD1336O,
    MT5112_TUNER_SAMSUNG_DTVS203,
    MT5112_TUNER_PHILIPS_FQD1236,
    MT5112_TUNER_FORWARD_TAEH6,
    MT5112_TUNER_PHILIPS_FQD1236E,
    MT5112_TUNER_SAMSUNG_DTVS203EH,
    MT5112_TUNER_ALPS_TDQU2,
    MT5112_TUNER_ALPS_TDQU4X524A,
    MT5112_TUNER_ALPS_TDQU8XT01A,
    MT5112_TUNER_SAMSUNG_DTVS227CH
};

/***********************************************************************/
#define cCFG_EEP_SIZE           (sizeof(asNIM_CfgTunerEep) / sizeof(*asNIM_CfgTunerEep))
UINT8 asNIM_CfgTunerEep[] =
{
    MT5112_TUNER_ALPS_TDQU8XT01A,
    MT5112_TUNER_ALPS_TDQU2,
#if fcINTERNAL_TEST
    MT5112_TUNER_SAMSUNG_DTVS203,
#endif
    MT5112_TUNER_PHILIPS_FQD1236
};

static VOID vDTDGetPERInfo(DEMOD_CTX_T *psDemodCtx);
/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/
EXTERN UINT8 u1I2cMT8297_SetReg(UINT8 u1Offset, UINT8 u1Data);
EXTERN UINT8 u1I2cMT8297_SetRegMask(UINT8 u1Offset, UINT8 u1Data, UINT8 u1Msk);
EXTERN UINT8 u1I2cMT8297_InDirect_SetReg(UINT8 u1Offset, UINT8 u1Data);

EXTERN void CHIP_Delay_us(UINT32 u4Micros);

/***********************************************************************/
/*              Private (Static) Function Prototypes                   */
/***********************************************************************/
#if fcHOSTCMD_BUSY_WAIT
BOOL fgTunerBusyWait(UINT32 u2WaitTimeus, BOOL fgInit)
{
STATIC TIME_TICK_T rCurrent1;
TIME_TICK_T rCurrent2;
TIME_DIFF_T rDelta;
#if fcHOSTCMD_BUSY_WAIT == 1
UINT32      ii;
#endif
UINT32 u2CurTimeus;
#if (fcHOSTCMD_BUSY_WAIT != 1) && 1
STATIC UINT32   u2MaxCnt = 0;
#endif

#if fcHOSTCMD_BUSY_WAIT != 1
    if (fgInit)
#endif
        mcGET_SYS_TIME(rCurrent1);
#if fcHOSTCMD_BUSY_WAIT != 1
    else
#else
    for (ii = 0; ii < u2WaitTimeus; ii ++)
#endif
    {
        mcGET_SYS_TIME(rCurrent2);
        mcGET_DIFF_TIME(rDelta, rCurrent1, rCurrent2);
        u2CurTimeus = mcCONV_SYS_TIME_US(rDelta);
        if (u2CurTimeus >= u2WaitTimeus)
        {
#if fcHOSTCMD_BUSY_WAIT == 1
            break;
#else
            if (u2MaxCnt < u2CurTimeus)
            {
                u2MaxCnt = u2CurTimeus;
                mcSHOW_HW_MSG(("%s timeout: %"cFMT_UL"\n", __FUNCTION__, u2CurTimeus));
            }
            return TRUE;
#endif
        }
#if (fcHOSTCMD_BUSY_WAIT != 1) && 1
        if (u2MaxCnt < u2CurTimeus)
        {
            u2MaxCnt = u2CurTimeus;
            mcSHOW_DBG_MSG(("%s: %"cFMT_UL"\n", __FUNCTION__, u2CurTimeus));
        }
#endif
    }
#if fcHOSTCMD_BUSY_WAIT == 1
// u2CurTimeus = 1000 <--> ii = 270
// u2CurTimeus = 2000 <--> ii = 530
// u2CurTimeus = 5000 <--> ii = 1200
//    mcSHOW_DBG_MSG(("%s: %d (%d)\n", __FUNCTION__, u2CurTimeus, ii));
#endif
    return FALSE;
}
#endif

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
VOID vGetAsicVer(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    DTD_GetReg(psDemodCtx, DTD_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("DTD ASIC Ver %d.%d",
                    (UINT8) mcGET_FIELD_CMB(aucData[0], CHIP_ID_GEN) + 1,   // Chip Generation, 0 = MT5111
                    (UINT8) mcGET_FIELD_CMB(aucData[0], CHIP_ID_MASK) + 1));// Mask Rev., 1 = BO62350A

    mcSET_CMD2(aucData) = cCMD_ID_GET_LABEL;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_LABEL_SIZE, 1))
    {
        mcSHOW_USER_MSG((" %c%02X%02X-%02X%02X",
                        mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_TYPE),
                        mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_ID_1), mcGET_QUEUE(aucData, cOFST_LABEL_BRANCH_ID_2),
                        mcGET_QUEUE(aucData, cOFST_LABEL_ID_1), mcGET_QUEUE(aucData, cOFST_LABEL_ID_2)));
    }

    mcSET_CMD2(aucData) = cCMD_ID_GET_VER;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_VER_SIZE, 1))
    {
        mcSHOW_USER_MSG((" %02X%02X-%02X%02X\n",
                         mcGET_QUEUE(aucData, cOFST_VER_YEAR_MONTH), mcGET_QUEUE(aucData, cOFST_VER_DATE),
                         mcGET_QUEUE(aucData, cOFST_VER_HOUR), mcGET_QUEUE(aucData, cOFST_VER_MINUTE)));
    }

}

/***********************************************************************/
UINT8 u1GetRegs(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt)
{
    return u1GetRegOrVars(psDemodCtx, u2Addr, pu1Data, u1Cnt,
                          cCMD_ID_DEMOD_GET_REG);
}

UINT8 ucGetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
UINT8   u1Value = 0;

    u1GetRegs(psDemodCtx, u2Addr, &u1Value, 1);
    return u1Value;
}

UINT8 u1GetMultiRegs(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt)
{
UINT8    ii = 0;
UINT8    u1RegRdCount = u1Cnt;

    // Max read register number = 4 by HostCmd
    while(u1RegRdCount)
    {
           if (u1RegRdCount < 4)
           {
               u1GetRegs(psDemodCtx, u2Addr + ii, &pu1Data[ii], u1RegRdCount);
               break;
           }
           else
           {
            u1GetRegs(psDemodCtx, u2Addr + ii, &pu1Data[ii], 4);
            ii = ii + 4;
            u1RegRdCount = u1RegRdCount - 4;
         }
    }

    return u1Cnt;
}

/***********************************************************************/
UINT8 u1SetRegs(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt)
{
    return u1SetRegOrVars(psDemodCtx, u2Addr, pu1Data, u1Cnt,
                          cCMD_ID_DEMOD_SET_REG);
}

VOID vSetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 u1Value)
{
    u1SetRegs(psDemodCtx, u2Addr, &u1Value, 1);
}

/***********************************************************************/
VOID vSetFwVarBit(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 u1BitPos)
{
UINT8   u1Value;
      u1Value = ucGetFwVar(psDemodCtx, u2Addr);
      //mcSHOW_USER_MSG(("Get FW Var %03X = %02X\n", u2Addr, u1Value));
    mcSET_BIT(u1Value, u1BitPos);
      mcSHOW_USER_MSG(("Set Bit(%d): FW Var %03X = %02X\n", u1BitPos, u2Addr, u1Value));
    vSetFwVar(psDemodCtx, u2Addr, u1Value);
}

VOID vClrFwVarBit(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 u1BitPos)
{
UINT8   u1Value;
      u1Value = ucGetFwVar(psDemodCtx, u2Addr);
      //mcSHOW_USER_MSG(("Get FW Var %03X = %02X\n", u2Addr, u1Value));
    mcCLR_BIT(u1Value, u1BitPos);
      mcSHOW_USER_MSG(("CLR Bit(%d): FW Var %03X = %02X\n", u1BitPos, u2Addr, u1Value));
    vSetFwVar(psDemodCtx, u2Addr, u1Value);
}

/***********************************************************************/
/*              Set Analog-Register Functions                          */
/***********************************************************************/
#ifndef CC_WINDOWS_PLATFORM // [
    #if fcDEMOD_GEN == cMT5391_DTD
static INT32 _i4ClockInit_MT5391(VOID)
{
UINT32 u4RegValue;
#ifdef fcCKGENPLL_SET_BY_LOADER
CAL_SRC_T eSel;
UINT32 u4CalClk;
#endif

// Select Clock Source
//#if 1   // Do not set TVDPLL & DTDPLL in tuner code
#ifdef fcCKGENPLL_SET_BY_LOADER

   eSel = CAL_SRC_DTDPLL;
   u4CalClk = BSP_GetDomainClock(eSel);
   mcSHOW_DBG_MSG(("BSP_GetDomainClock=%08x\n",u4CalClk)); //fix warning: variable "u4CalClk" was set but never used
   ASSERT(((u4CalClk + 240000*4) > 600000000) && ((u4CalClk - 240000*4) < 600000000));
#else
    // DTDPLLCFG0 (0x2000d180)
    // bit[15:8] = d'8 => DIV = d'10
   // bit[1:0] = b'01 => CKSEL From TVDPLL
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTDPLLCFG0, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x08, PLL_CKDIV );
    mcSET_FIELD_CMB(u4RegValue, 0x01, PLL_CKSEL );
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTDPLLCFG0, u4RegValue);

    // bit[19] = b'0 => DTDPLL reference clock from TVDPLL control (540/9=60MHz)
    // bit[18] = b'1 => DTDPLL reference clock from TVDPLL 'enable'
    // bit[7]  = b'0 => power up
    // bit[0]  = b'0 -> b'1 => VCOCAL enable (DO it everytime you change frequency)
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTDPLLCFG1, u4RegValue);
    mcCLR_BIT(u4RegValue, cpPLL_REL_SEL);
    mcSET_BIT(u4RegValue, cpPLL_REL_EN);
    mcCLR_BIT(u4RegValue, cpPLL_PWR_UP);
    mcCLR_BIT(u4RegValue, cpPLL_CAL);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTDPLLCFG1, u4RegValue);
    CHIP_Delay_us(1);
    mcSET_BIT(u4RegValue, cpPLL_CAL);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTDPLLCFG1, u4RegValue);

    #if 0 // Remove TVDPLL since many modules shares it. The frequency is fixed at 432Mhz for US model.
   // TVDPLLCFG0 (0x2000d170)
   // bit[15:8] = d'18 = h'12=> DIV = d'20
   // bit[1:0] = b'00 => CLKSEL From XTAL
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TVDPLLCFG0, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x12, PLL_CKDIV );
    mcSET_FIELD_CMB(u4RegValue, 0x00, PLL_CKSEL );
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TVDPLLCFG0, u4RegValue);

    // TVDPLLCFG1 (0x2000d174)
    // bit[7]  = b'0 => power up
    // bit[0]  = b'1 => VCOCAL enable (DO it everytime you change frequency)
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TVDPLLCFG1, u4RegValue);
    mcCLR_BIT(u4RegValue, cpPLL_PWR_UP);
    mcSET_BIT(u4RegValue, cpPLL_CAL);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TVDPLLCFG1, u4RegValue);
    #endif    
    
#endif

    // Set DTD CLK	
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTD_CKCFG, 0x00010101);
    // Disable DVB-T CLK
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DVBT_CKCFG, 0x00008080);

   // MX1[3:2]= 01: IF_AGC [Normal, Testmode]
   // MX1[1:0] = 2'b01: Tuner i2c [Normal] 
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX1, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x01, PMUX1_IFAGC );
    mcSET_FIELD_CMB(u4RegValue, 0x01, PMUX1_TUNERI2C );
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX1, u4RegValue);

   // MX3[25]: = 0> Set 8297 input  [Normal, testmode]
   // MX3[26] = 1'b0 => DEMOD_ADSFSEL [Normal, testmode]
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX3, u4RegValue);
    mcCLR_BIT(u4RegValue, cpPMUX3_8297IN);
    mcCLR_BIT(u4RegValue, cpPMUX3_DEMOD_ADFSEL);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX3, u4RegValue);


    return OSR_OK;
}
    #endif

    #if fcDEMOD_GEN == cMT5387_DTD
static INT32 _i4ClockInit_MT5387(VOID)
{
UINT32 u4RegValue;

// DTD related clock
    // DEMOD200_CKCFG dtdpll_d3_ck
    mcSET_SHOW_REG(CKGEN_BASE, c5387_CKGEN_REG_DTD200_CKCFG, 0x00000001);

    // DEMOD54_CKCFG  tvdpll_d10_ck
    mcSET_SHOW_REG(CKGEN_BASE, c5387_CKGEN_REG_DTD54_CKCFG, 0x00000001);

    // DEMOD60_CKCFG  tvdpll_d9_ck
    mcSET_SHOW_REG(CKGEN_BASE, c5387_CKGEN_REG_DTD60_CKCFG, 0x00000001);

    // DEMOD50_CKCFG  dtdpll_d12_ck
    mcSET_SHOW_REG(CKGEN_BASE, c5387_CKGEN_REG_DTD50_CKCFG, 0x00000001);

   // MX2[22:20]= 3b'1: IF_AGC
   // MX2[18:16] = 3b'1: Tuner i2c
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, u4RegValue);
    mcSET_FIELD_CMB(u4RegValue, 0x01, PMUX2_IFAGC);
    mcSET_FIELD_CMB(u4RegValue, 0x01, PMUX2_TUNERI2C);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, u4RegValue);
    
    return OSR_OK;
}
    #endif
    
    
INT32 i4ClockInit(VOID)
{
INT32 i4Ret;
#if fcDEMOD_GEN == cMT5391_DTD
    i4Ret = _i4ClockInit_MT5391();
#elif fcDEMOD_GEN == cMT5387_DTD
    i4Ret = _i4ClockInit_MT5387();
#else
    i4Ret = OSR_OK;
#endif
    return i4Ret;
}

    #if fcDEMOD_GEN == cMT5391_DTD
static INT32 _i4ClockFinish_MT5391(VOID)
{
#ifdef CC_DYNAMIC_POWER_ONOFF
UINT32 u4RegValue;
    // Power down DTD_60(bit15) & DTD_200 (bit7) in DTD_CKCFG , DTD_50 is shared with other module, so do not power if off.
    mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTD_CKCFG, u4RegValue);
    mcSET_BIT(u4RegValue, cpDTD60_PD);
    mcSET_BIT(u4RegValue, cpDTD200_PD);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_DTD_CKCFG, u4RegValue);
#endif
    return OSR_OK;
}
    #endif
    
INT32 i4ClockFinish(DEMOD_CTX_T *psDemodCtx)
{
INT32 i4Ret;
#if fcDEMOD_GEN == cMT5391_DTD
    i4Ret = _i4ClockFinish_MT5391();
#else
    i4Ret = OSR_OK;
#endif
    return i4Ret;
}

#define cXTAL_27MHZ              0
#define cXTAL_60MHZ              1
#define cSYSPLL1_CLK             540000000
#define cSYSPLL2_CLK             600000000
#if fcDEMOD_GEN == cMT5391_DTD
static INT32 _i4ADInputInit_MT5391(DEMOD_CTX_T *psDemodCtx)
{
UINT8 u1XtalSel;
UINT32 u4Clock;
UINT8  u1Value;

    #if fcADD_AD_CONTROL
UINT16 InputSrc;

    AD_AcquireControl();
    InputSrc = AD_QueryInputSource();
    if (((InputSrc & 0xFF) == IPS_DTV) || (((InputSrc >> 8) & 0xFF) == IPS_DTV))
    #endif
    {
        // Set AD configuration for DTV
        if (IS_XTAL_27MHZ())
        {   
        	u1XtalSel = cXTAL_27MHZ;
            mcSHOW_DBG_MSG(("Detect XTAL is 27MHz!\n"))
        }
        else if (IS_XTAL_60MHZ())
        {
        	u1XtalSel = cXTAL_60MHZ;
            mcSHOW_DBG_MSG(("Detect XTAL is 60MHz!\n"))        	
        }
        else
        {
            mcSHOW_DBG_MSG(("Detect XTAL is neither 27 or 60MHz!\n"))
            return OSR_FAIL;        	
        }
        	
        if (u1XtalSel == cXTAL_27MHZ)
        {
            #ifndef fc8297PLL_SET_BY_LOADER
            u1I2cMT8297_SetReg(0x10, 0x33);
            u1I2cMT8297_SetReg(0x11, 0x24);
            u1I2cMT8297_SetReg(0x12, 0x12);
            #endif
            u1I2cMT8297_SetReg(0x13, 0x00);
            u1I2cMT8297_SetReg(0x14, 0x03);
            u1I2cMT8297_SetReg(0x15, 0x00);
            u1I2cMT8297_SetReg(0x16, 0x55);
            u1I2cMT8297_SetReg(0x17, 0x55);
            u1I2cMT8297_SetReg(0x18, 0x05);
            u1I2cMT8297_SetReg(0xb7, 0x18);
        }
        #ifndef fc8297PLL_SET_BY_LOADER        
        u1I2cMT8297_SetReg(0xa8, 0xc4);
        u1I2cMT8297_SetReg(0xa9, 0x34);        
        u1I2cMT8297_SetReg(0xaa, 0x21);
        #endif
        u1I2cMT8297_SetReg(0xab, 0x03);
        u1I2cMT8297_SetReg(0xac, 0x00);
        u1I2cMT8297_SetReg(0xad, 0x00);
        u1I2cMT8297_SetReg(0xae, 0x55);
        u1I2cMT8297_SetReg(0xaf, 0x55);
        
        u1I2cMT8297_SetReg(0xb0, 0x05);
        u1I2cMT8297_SetReg(0xb1, 0x00);
        u1I2cMT8297_SetReg(0xb2, 0x00);

        /* Notice. SYSPLL2 reset must be toggled. 0xb4 bit5=0->1, 2008.07.02 */
        if (u1XtalSel == cXTAL_27MHZ)
        {        
            #ifndef fc8297PLL_SET_BY_LOADER                
            u1I2cMT8297_SetReg(0xb4, 0x01);
            u1I2cMT8297_SetReg(0xb4, 0x21);
            #else
            u1I2cMT8297_SetRegMask(0xb4, 0x01, 0xF0);
            u1I2cMT8297_SetRegMask(0xb4, 0x21, 0xF0);
            #endif
        }
        else if (u1XtalSel == cXTAL_60MHZ)
        {
            #ifndef fc8297PLL_SET_BY_LOADER                
            u1I2cMT8297_SetReg(0xb4, 0x00);
            u1I2cMT8297_SetReg(0xb4, 0x20);
            #else
            u1I2cMT8297_SetRegMask(0xb4, 0x00, 0xF0);
            u1I2cMT8297_SetRegMask(0xb4, 0x20, 0xF0);
            #endif
        }        	
        u1I2cMT8297_SetReg(0xb5, 0x00);
        u1I2cMT8297_SetRegMask(0xb6, 0x03, 0x03); // Share with audio and CVBS
        u1I2cMT8297_SetReg(0xb8, 0x00) ;
        if (u1XtalSel == cXTAL_27MHZ)
        {     
        	// 2008.4.16 Remove temp
            //#ifndef fc8297PLL_SET_BY_LOADER
            u1I2cMT8297_SetReg(0xb9, 0x1c);
            //#else
            //u1I2cMT8297_SetRegMask(0xb9, 0x1c, 0x03) ;
            //#endif
        }
        if (u1XtalSel == cXTAL_27MHZ)
        {       
            u1I2cMT8297_SetReg(0x9c, 0xa2);
        }
        else if (u1XtalSel == cXTAL_60MHZ)
        {
            u1I2cMT8297_SetReg(0x9c, 0x82);
        }
        	
        u1I2cMT8297_SetReg(0x9e, 0x02) ;

        u1I2cMT8297_InDirect_SetReg(0x0d, 0x40); // 2008.10.22, power off 2x clk
        u1I2cMT8297_InDirect_SetReg(0x0e, 0x01); // 2008.10.22, power on demod adc clk
        u1I2cMT8297_InDirect_SetReg(0x63, 0x3f);       
        
        u1I2cMT8297_SetReg(0x81, 0x60) ;
        u1I2cMT8297_SetReg(0x8a, 0x00) ; // Add. Shared with audio and CVBS. 2008.4.7
        u1I2cMT8297_SetReg(0x8e, 0x21) ;
        u1I2cMT8297_SetReg(0x8f, 0x03) ;
        u1I2cMT8297_SetReg(0x90, 0x00);
        if (u1XtalSel == cXTAL_27MHZ)
        {
            u1I2cMT8297_SetReg(0x91, 0x25);
        }  
        else if (u1XtalSel == cXTAL_60MHZ)
        {
            u1I2cMT8297_SetReg(0x91, 0x04);
        }
        u1I2cMT8297_SetReg(0x92, 0x22);
        u1I2cMT8297_SetReg(0x93, 0x00);
        u1I2cMT8297_SetReg(0x94, 0x38);
        if (u1XtalSel == cXTAL_27MHZ)
        {        
            u1I2cMT8297_SetReg(0x95, 0x29);
        }
        else if (u1XtalSel == cXTAL_60MHZ)
        {
            u1I2cMT8297_SetReg(0x95, 0x28);
        }
        
        // Add @ 2008.05.02 throught FIFO to compensate phase diff
        //u1I2cMT8297_SetReg(0x81, 0x40);
        u1I2cMT8297_SetReg(0x81, psDemodCtx->u1TransPhase);        
        u1I2cMT8297_SetReg(0x83, 0x0A);

        // Fast switch enable
        if (psDemodCtx->u1ChipRev == DTD_CHIP_REV_5391)
        {
            u1I2cMT8297_SetReg(0x8d, 0x08);  // 2008.7.14. Change the sequece according to designer's suggestion.
        }
        else
        {
            u1I2cMT8297_SetReg(0x8d, 0x09);  // [8297ECO] Fast Switch HW Rst Enable
        }

        // 2008.7.14. PLL2 reset toggle again after all setting is applied.
        if (u1XtalSel == cXTAL_27MHZ)
        {        
            #ifndef fc8297PLL_SET_BY_LOADER                
            u1I2cMT8297_SetReg(0xb4, 0x01);
            u1I2cMT8297_SetReg(0xb4, 0x21);
            #else
            u1I2cMT8297_SetRegMask(0xb4, 0x01, 0xF0);
            u1I2cMT8297_SetRegMask(0xb4, 0x21, 0xF0);
            #endif
        }
        else if (u1XtalSel == cXTAL_60MHZ)
        {
            #ifndef fc8297PLL_SET_BY_LOADER                
            u1I2cMT8297_SetReg(0xb4, 0x00);
            u1I2cMT8297_SetReg(0xb4, 0x20);
            #else
            u1I2cMT8297_SetRegMask(0xb4, 0x00, 0xF0);
            u1I2cMT8297_SetRegMask(0xb4, 0x20, 0xF0);
            #endif
        }        	
        
        u1I2cMT8297_SetReg(0x80, 0x03);
        u1I2cMT8297_SetReg(0x80, 0x07);            
        		        
        if (u1XtalSel == cXTAL_27MHZ)
        {
            // Verify SYSPLL1 clock is correct
            u4Clock = BSP_GetDomainClock(CAL_8297_SYSPLL1);
            mcSHOW_DBG_MSG(("8297 SYSPLL1 clock = %d, \n", u4Clock));
            if (!IS_8297_PLL_INCLK(u4Clock, cSYSPLL1_CLK/5)) 
            { 
            	mcSHOW_DBG_MSG(("8297 SYSPLL1 clock is incorrect(Not 540MHz) %d \n", u4Clock));
            }
        }            

        // Verify SYSPLL2 clock is correct
        u4Clock = BSP_GetDomainClock(CAL_8297_SYSPLL2);
        u1Value = ucGetReg(psDemodCtx, DTD_REG_Chip_Fs_Cfg);        
        mcSHOW_DBG_MSG(("8297 SYSPLL2 clock = %d, Fs_Sel = %d\n", u4Clock, u1Value));
        if ( (!IS_8297_PLL_INCLK(u4Clock, cSYSPLL2_CLK/5) && (u1Value == 1))
       	  || (!IS_8297_PLL_INCLK(u4Clock, cSYSPLL2_CLK/6) && (u1Value == 0))
           )
        { 
        	mcSHOW_DBG_MSG(("8297 SYSPLL2 clock is incorrect %d\n", u4Clock));
        } 

    }
    #if fcADD_AD_CONTROL
    else
    {
        mcSHOW_HW_MSG(("Input source = %X, not DTV (%X)!\n", InputSrc, IPS_DTV));
        AD_ReleaseControl();
        return OSR_FAIL;
    }
    AD_ReleaseControl();
    #endif
    return OSR_OK;
}
#endif

#if fcDEMOD_GEN == cMT5387_DTD
static INT32 _i4ADInputInit_MT5387(DEMOD_CTX_T *psDemodCtx)
{
UINT16 u2RegAddr;
UINT8  u1RegValue[6];

// DEMOD ADC / PLL to ADC divider ratio  setting (Fast switch enable version)
//--- analog demod reg (Driver) ---
    u2RegAddr = 0x118;
    u1RegValue[0] = 0x00; // Write DTD 118H=00h	
    u1RegValue[1] = 0x41; // Write DTD 119H=41h
    u1RegValue[2] = 0xAA; // Write DTD 11AH=AAh  (same as default)
    u1RegValue[3] = 0x00; // Write DTD 11BH=00h	 (same as default)
    u1RegValue[4] = 0x54; // Write DTD 11CH=94h	 (same as default)
    DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue[0], 5);

//--- analog fast Switch setting (Driver) ---
    u2RegAddr = 0x112;
    u1RegValue[0] = 0x08; // Write DTD 112H=08h
    u1RegValue[1] = 0x14; // Write DTD 113H=14h
    u1RegValue[2] = 0x01; // Write DTD 114H=01h
    u1RegValue[3] = 0x08; // Write DTD 115H=08h
    u1RegValue[4] = 0x15; // Write DTD 116H=15h
    u1RegValue[5] = 0x01; // Write DTD 117H=01h
    DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue[0], 6);

//--- Fast switch enabled --- 
    u2RegAddr = 0x111;
    u1RegValue[0] = 0x03; // Write DTD 111H=03h
    DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue[0], 1);

//--- RESET PLL to AD divider (60MHz sampling) (Driver) ---
    u2RegAddr = 0x113;
    u1RegValue[0] = 0x04; //Write DTD 113H=04h
    DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue[0], 1);

    u2RegAddr = 0x113;
    u1RegValue[0] = 0x14; //Write DTD 113H=14h
    DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue[0], 1);    
 
    return OSR_OK;
}
#endif

INT32 i4ADInputInit(DEMOD_CTX_T *psDemodCtx)
{
INT32 i4Ret;
#if fcDEMOD_GEN == cMT5391_DTD
    i4Ret = _i4ADInputInit_MT5391(psDemodCtx);
#elif fcDEMOD_GEN == cMT5387_DTD
    i4Ret = _i4ADInputInit_MT5387(psDemodCtx);
#else
    i4Ret = OSR_OK;
#endif
    return i4Ret;
}

/***********************************************************************/

#if fcDEMOD_GEN == cMT5391_DTD
static INT32 _i4ADInputFinish_MT5391(DEMOD_CTX_T *psDemodCtx)
{
//UINT8 u1RegValue;
    #if fcADD_AD_CONTROL
UINT16 InputSrc;

    AD_AcquireControl();
    InputSrc = AD_QueryInputSource();
    if (((InputSrc & 0xFF) == IPS_DTV) || (((InputSrc >> 8) & 0xFF) == IPS_DTV))
    #endif
    {
       // Switch sampling rate control back to RISC
       //u1I2cReadMT8297Reg(0x8d, &u1RegValue);     // Fast switch disable
       u1I2cMT8297_SetReg(0x8d, 0x00);     // Fast switch disable
       
#ifdef  CC_DYNAMIC_POWER_ONOFF
       u1I2cMT8297_SetReg(0x9c, 0x04);  // Power off demod ADC, bit 2= 1
       u1I2cMT8297_InDirect_SetReg(0x0e, 0x40); // 2008.10.22, power off demod adc clk
#endif

    }
    #if fcADD_AD_CONTROL
    else
    {
        mcSHOW_HW_MSG(("Input source = %X, not DTV (%X)!\n", InputSrc, IPS_DTV));
        AD_ReleaseControl();
        return OSR_FAIL;
    }
    AD_ReleaseControl();
    #endif

    return OSR_OK;
}
#endif

#if fcDEMOD_GEN == cMT5387_DTD
static INT32 _i4ADInputFinish_MT5387(DEMOD_CTX_T *psDemodCtx)
{
UINT16 u2RegAddr;
UINT8  u1RegValue;

//--- Fast switch disabled --- 
    u2RegAddr = 0x111;
    u1RegValue = 0x02; // Write DTD 111H=02h
    DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue, 1);
    return OSR_OK;
}
#endif

INT32 i4ADInputFinish(DEMOD_CTX_T *psDemodCtx)
{
INT32 i4Ret;
#if fcDEMOD_GEN == cMT5391_DTD
    i4Ret = _i4ADInputFinish_MT5391(psDemodCtx);
#elif fcDEMOD_GEN == cMT5387_DTD
    i4Ret = _i4ADInputFinish_MT5387(psDemodCtx);
#else
    i4Ret = OSR_OK;
#endif
    return i4Ret;
}

#if fcDEMOD_GEN == cMT5391_DTD
static INT32 _i4ConnectInit_MT5391(DEMOD_CTX_T *psDemodCtx)
{
        i4ClockInit();
        // Switch AGC_Mux to DTD
        //    Ana_SwitchRfAgc(); // No interal ATD in MT539x
        
        return i4ADInputInit(psDemodCtx); // Follows AD Query rule
        
//    return OSR_OK;  //fix warning: statement is unreachable
}
#endif

/* Ana_SwitchRfAgc()
 *  @param  fgAtdDtd_Sel: 1:DTD control. 0: ATD control.
*/
#if fcDEMOD_GEN == cMT5387_DTD
INT32 Ana_SwitchRfAgc(UINT8 fgAtdDtd_Sel)
{
UINT16 u2RegAddr = 0x93e;
UINT8  u1RegValue = 0x03;  // atd
DEMOD_CTX_T *psDemodCtx = NULL;

    if (GetDigiDemodCtx == NULL)
    {
        return DRVAPI_TUNER_ERROR;
    }

    // Set atd_sel to 0 (Set demod adr 0x93e to be 0x02)
    if (fgAtdDtd_Sel == 1) {
        u1RegValue = 0x02;
    }

     psDemodCtx = (DEMOD_CTX_T*) GetDigiDemodCtx();
     if (psDemodCtx == NULL)
     {
         return DRVAPI_TUNER_ERROR;
     }
     DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue, 1);
    
    return OSR_OK;
}

static INT32 _i4ConnectInit_MT5387(DEMOD_CTX_T *psDemodCtx)
{
        i4ClockInit();
        // Switch AGC_Mux to DTD
            Ana_SwitchRfAgc(1); // No interal ATD in MT539x
        
        return i4ADInputInit(psDemodCtx); // Follows AD Query rule       
}
#endif

INT32 i4ConnectInit(DEMOD_CTX_T *psDemodCtx)
{
INT32 i4Ret;
#if fcDEMOD_GEN == cMT5391_DTD
    i4Ret = _i4ConnectInit_MT5391(psDemodCtx);
#elif fcDEMOD_GEN == cMT5387_DTD
    i4Ret = _i4ConnectInit_MT5387(psDemodCtx);
#else
    i4Ret = OSR_OK;
#endif
    return i4Ret;    
}

INT32 i4SetTSPadDriving(UINT8 u1Current)
{
#if (fcDEMOD_GEN == cMT5391_DTD)
UINT32 u4RegValue;
UINT32 u4SetValue;
UINT8  u1Value;

    if ((u1Current > 8) || (u1Current < 2))
    	return (-1);
    	
    u1Value = (u1Current - 2) / 2;

    mcGET_SHOW_REG(CKGEN_BASE, cTS_PAD_DRIVING, u4RegValue);
    mcCLR_MASK(u4RegValue, mcMASKS(2, 14));
    u4SetValue = mcFIELD(u1Value, mcMASKS(2, 14), 14);
    u4RegValue |= u4SetValue;
    mcSET_SHOW_REG(CKGEN_BASE, cTS_PAD_DRIVING, u4RegValue); 

#else
    mcSHOW_DBG_MSG(("Not support"));    

#endif
    
    return OSR_OK;
}

/***********************************************************************/
// Set register before any register R/W
VOID vCheckHostIf(DEMOD_CTX_T *psDemodCtx)
{
    psDemodCtx->psI2cCtx = psDemodCtx;
   // psDemodCtx->sTunerCtx.psI2cCtx = &(psDemodCtx->sTunerCtx);

}

/***********************************************************************/
// Set register after firmware start running
VOID vConfigHostIf(DEMOD_CTX_T *psDemodCtx)
{
UINT8   u1Value;
// Config HostCmd
    u1Value = mcBIT(cpGPIO_MODE_2);
    DTD_SetReg(psDemodCtx, DTD_REG_uP_Ctrl_E, &u1Value, 1);
    u1Value = 0;
    DTD_SetReg(psDemodCtx, DTD_REG_RISC_CFG, &u1Value, 1);
}
#endif  // ]


/***********************************************************************/
INT32 DTD_StartDemod(DEMOD_CTX_T *psDemodCtx, UCHAR eModType)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

     mcSHOW_API_MSG(("DTD_StartDemod\n"));

// check invalid input
    if (eModType >= MOD_TYPE_END)
    {
        mcSHOW_DBG_MSG(("Invalid Mod(%d)\n", eModType));
        return (-1);
    }
    
#ifdef fcEXTERN_DEMOD
    mcSET_CMD2(aucData) = cCMD_ID_SET_IFAGC_HIGH_Z;
    mcSET_QUEUE2(aucData, cOFST_SET_IFAGC_EN) = 0;  // 0: normal
    mcHOST_CMD(psDemodCtx, aucData, 0, cCMD_ID_SET_IFAGC_HIGH_Z_SIZE + 1);
#endif

    mcSET_CMD2(aucData) = cCMD_ID_DRIVER_START;

    mcSET_QUEUE2(aucData, cOFST_DRIVER_PWR_UP_PIN) = 1;
    mcSET_QUEUE2(aucData, cOFST_DRIVER_START_RESET) = 1;
    mcSET_QUEUE2(aucData, cOFST_DRIVER_DEMOD_MOD) = eModType;
    mcSET_QUEUE2(aucData, cOFST_DRIVER_QSPCINV) = psDemodCtx->u1QSpecInv;
    mcSET_QUEUE2(aucData, cOFST_DRIVER_AUTO_QAM) = 0;
#if fcADD_AUTO_QAM
    mcSET_QUEUE2(aucData, cOFST_DRIVER_AUTO_QAM) = psDemodCtx->fgAutoQam;
#endif
    mcSET_QUEUE2(aucData, cOFST_DRIVER_QADSAMP) = 1;

    #if 0  // Solution for 60MHz harmonics, default disable now
    if (psDemodCtx->Modulation != MOD_ATSC_8VSB)
    {
        if ((psDemodCtx->Frequency % 60000) <= 3000)
        {
            mcSET_QUEUE2(aucData, cOFST_DRIVER_QADSAMP) = 0;
        }

        if (mcSET_QUEUE2(aucData, cOFST_DRIVER_QADSAMP))
        {
            mcSHOW_DBG_MSG(("Freq = %d => QADSamp = 60MHz \n", psDemodCtx->Frequency));
        }
        else
        {
            mcSHOW_DBG_MSG(("Freq = %d => QADSamp = 50MHz \n", psDemodCtx->Frequency));
        }
    }
    #endif
    if (mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_DRIVER_START_SIZE + 1))
        return (-1);

    psDemodCtx->Modulation = eModType;

    return 0;
}

/***********************************************************************/
CHAR *szGetDrvVer(VOID)
{
//    return (TUNER_SOURCE_LABEL " (" __DATE__ ", " __TIME__ ")");
    return (TUNER_SOURCE_LABEL);
}

/***********************************************************************/
CHAR *DTD_GetVer(DEMOD_CTX_T *psDemodCtx)
{
   CHAR * cString1;
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = TRUE;
#endif
    vGetAsicVer(psDemodCtx);
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = FALSE;
#endif
    ITuner_OP(psDemodCtx->sTunerCtx,itGetVer,0,&cString1);
    mcSHOW_USER_MSG(("%s\n", cString1));
#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
#endif

    return szGetDrvVer();
}

/***********************************************************************/
/*              Initialization APIs                                    */
/***********************************************************************/
/***********************************************************************/
/*              Configuration APIs                                     */
/***********************************************************************/
/***********************************************************************/
/*              Query APIs                                             */
/***********************************************************************/
VOID vCalcErrClass(DEMOD_CTX_T *psDemodCtx)
{
    // Classify Error class
    if (psDemodCtx->u4AccErrorRate <= 20) // 2e-4
    {
        psDemodCtx->u1ErrClass = 0;
        mcSHOW_DBG_MSG1(("<PER Result> FinalAccPER: No error, "));
    }
    else if (psDemodCtx->u4AccErrorRate <= 900) // 9e-3
    {
        psDemodCtx->u1ErrClass = 1;
        mcSHOW_DBG_MSG1(("<PER Result> FinalAccPER: Minor error, "));
    }
    else if (psDemodCtx->u4AccErrorRate <= 50000) // 5e-1
    {
        psDemodCtx->u1ErrClass = 2;
        mcSHOW_DBG_MSG1(("<PER Result> FinalAccPER: Heavy error, "));
    }
    else
    {
        psDemodCtx->u1ErrClass = 3;
        mcSHOW_DBG_MSG1(("<PER Result> FinalAccPER: Unsync, "));
    }
}

VOID DTD_PERInit(DEMOD_CTX_T *psDemodCtx)
{
    psDemodCtx->u2PktErr = 0;
    psDemodCtx->u2PktCnt = 0;
    psDemodCtx->u2ErrorRate = 0;
    psDemodCtx->u4AccPktErr = 0;
    psDemodCtx->u4AccPktCnt = 0;
    psDemodCtx->u4AccErrorRate = 100000;
    psDemodCtx->u1PecErrSec = 0;
}

VOID DTD_AccumPER(DEMOD_CTX_T *psDemodCtx)
{
STATIC UINT32   u4Count;
STATIC TIME_TICK_T u4PecTickStart;
STATIC TIME_DIFF_T u4PecTickDiff;
TIME_TICK_T u4PecTickNow;

      vDTDGetPERInfo(psDemodCtx);

      if (psDemodCtx->u1PecFlag == 1) // Init state
      {
          mcSHOW_USER_MSG(("Start PEC... [Format]AccPER: <AccErrRate> (<AccErrPkt> <AccRcvPkt>), <Elapse time>\n"));

          mcGET_SYS_TIME(u4PecTickStart);
          vDTDGetPERInfo(psDemodCtx);// vCalcPER: Discard first data

          // Clear Err
          DTD_PERInit(psDemodCtx);
          psDemodCtx->fgPecReady = FALSE;

          u4Count = 1;
          psDemodCtx->u1PecFlag = 2;
      }
      else if (psDemodCtx->u1PecFlag == 2)  // Accum state
      {
          //vDTDGetPERInfo(psDemodCtx);// vCalcPER
          // Get elapsed time
          mcGET_SYS_TIME(u4PecTickNow);
          mcGET_DIFF_TIME(u4PecTickDiff, u4PecTickStart, u4PecTickNow);
          if ( mcCONV_SYS_TIME(u4PecTickDiff) >= (psDemodCtx->u2PecRefreshTime)*u4Count )
          {
              u4Count++;
              mcSHOW_DBG_MSG1((" AccPER: <%"cFMT_UL"e-5> (<%"cFMT_UL"> <%"cFMT_UL">), ", psDemodCtx->u4AccErrorRate, psDemodCtx->u4AccPktErr, psDemodCtx->u4AccPktCnt));
              mcSHOW_DBG_MSG1(("Elapsed time:%"cFMT_UL" sec\n", mcCONV_SYS_TIME(u4PecTickDiff)/1000  ));
          }
          // Check ObserveTime
          if ( mcCONV_SYS_TIME(u4PecTickDiff) >= psDemodCtx->u4PecObserveTime )
          {
              psDemodCtx->u1PecFlag = 3;
          }
      }
      else if (psDemodCtx->u1PecFlag == 3) // Stop state
      {
           // Error class classification
          vCalcErrClass(psDemodCtx);

          mcSHOW_DBG_MSG1(("%"cFMT_UL"e-5 (%"cFMT_UL" %"cFMT_UL"), ", psDemodCtx->u4AccErrorRate, psDemodCtx->u4AccPktErr, psDemodCtx->u4AccPktCnt));
          mcSHOW_DBG_MSG1(("Total time:%"cFMT_UL" sec\n", mcCONV_SYS_TIME(u4PecTickDiff)/1000 ));
          psDemodCtx->u1PecFlag = 0;
          psDemodCtx->fgPecReady = TRUE;
      }
}

/***********************************************************************/
#define cCHAN_BAND_NUM          4
#define cCHAN_BAND_WID          6           // 6 MHz

UINT16  u2ChanBand[cCHAN_BAND_NUM] = {51, 79, 177, 473};
UINT16  u2ChanNum[cCHAN_BAND_NUM] = {4, 2, 7, 67};


UINT16 u2ConvFreq2Chan(DEMOD_CTX_T *psDemodCtx)
{
UINT16 u2Chan = 1,u2Freq;
UINT16 ii, jj, u2ChanCnt = 0;

    u2Freq = psDemodCtx->Frequency / 1000;

    if ( psDemodCtx->Modulation == MOD_ATSC_8VSB  )
    {
        for (ii = 0; ii < cCHAN_BAND_NUM; ii ++)
        {
            if (u2Freq < u2ChanBand[ii])
                break;
        }
    
        if (ii >= 1) // Fix klocwork warning
        {
            u2ChanCnt = (u2Freq - u2ChanBand[ii - 1]) / cCHAN_BAND_WID;
        }
        for (jj = 0; jj < (ii - 1); jj ++)
        u2Chan += u2ChanNum[jj];
        u2Chan += u2ChanCnt;
        if (u2Chan > cMAX_CHAN_TABLE_NUM)
            u2Chan = cMAX_CHAN_TABLE_NUM;

    }
    else if (psDemodCtx->Modulation<MOD_TYPE_END)
    {
        if((u2Freq<76)&&(u2Freq>=72))
		u2Chan = 1;	
        else if (u2Freq<72)
        {
            u2Chan = 2+(u2Freq-54)/6;
        }
        else if ((u2Freq<88)&&(u2Freq>=76))
        {
            u2Chan = 5+(u2Freq-76)/6;
        }
	 else if ((u2Freq>=90)&&(u2Freq<120))
        {
            u2Chan = 95+(u2Freq-90)/6;
        }
	 else if ((u2Freq>=120)&&(u2Freq<174))
        {
            u2Chan = 14+(u2Freq-120)/6;
        }
	 else if ((u2Freq>=174)&&(u2Freq<216))
        {
            u2Chan = 7+(u2Freq-174)/6;
        }
	 else if ((u2Freq>=216)&&(u2Freq<648))
        {
            u2Chan = 23+(u2Freq-216)/6;
        }
        else
            u2Chan = 100+(u2Freq-648)/6;

    }
	return u2Chan;

}

/***********************************************************************/
#ifdef fcSMART_ANT_DRV
VOID vEnableDetectSmartAnt(DEMOD_CTX_T *psDemodCtx, BOOL fgDetectFlag)
{
UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_ANT_SET_DET_EN;
    mcSET_QUEUE2(aucData, cOFST_ANT_DET_EN) = fgDetectFlag;
    mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_ANT_DET_EN_SIZE + 1);
}

VOID vWriteSmartAnt(DEMOD_CTX_T *psDemodCtx, UINT8 u1Direct, UINT8 u1Gain, BOOL fgAntPol)
{
UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
UINT8   u1Code0, u1CurChan;

    vEnableDetectSmartAnt(psDemodCtx, FALSE);   // AVOID affect codeword data while polling

    u1CurChan = (UINT8) u2ConvFreq2Chan(psDemodCtx->Frequency / 1000);
    if (u1Direct >= cALL_DIRECT_NUM)
        u1Direct = 0;
    if (u1Gain >= cALL_GAIN_NUM)
        u1Gain = 0;
    u1Code0 = ((u1Direct << 3) + u1Gain);
    if (fgAntPol)
        u1Code0 |= mcBIT(2);
    mcSHOW_DBG_MSG(("AntIF Direct = %d, Gain = %d, Pol = %d\n", u1Direct, u1Gain, fgAntPol));
    mcSHOW_DBG_MSG(("AntIF Code = %02X, %02X\n", u1Code0, u1CurChan));
    mcSET_CMD2(aucData) = cCMD_ID_ANTIF_WRITE;
    mcSET_QUEUE2(aucData, cOFST_ANTIF_WRITE_CODE0) = u1Code0;
    mcSET_QUEUE2(aucData, cOFST_ANTIF_WRITE_CODE1) = u1CurChan;
    mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_ANTIF_WRITE_SIZE + 1);

    mcDELAY_MS(20);                         // Wait codeword data sent completely
    vEnableDetectSmartAnt(psDemodCtx, TRUE);
}

BOOL fgDetectSmartAnt(DEMOD_CTX_T *psDemodCtx)
{
UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
BOOL    fgDetectFlag = FALSE;

    mcSET_CMD2(aucData) = cCMD_ID_ANT_DETECT;

    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_ANT_DETECT_SIZE, 1))
    {
        if (!mcGET_QUEUE(aucData, cOFST_ANT_DETECT_PIN))
            fgDetectFlag = TRUE;            // LOW: detected
    }
    return fgDetectFlag;
}

/***********************************************************************/
#define cSLEEP_INTERVAL         1000        // ms
#define cWAIT_LOCK_TIME         2000
#define cWAIT_SNR_TIME          3000        // ms
#define cSCAN_PER_THR           (2e-4 * cPER_SCALE * cPER_PRE_SCALE)
#define cSCAN_SNR_THR           20

VOID SmartAntScan(DEMOD_CTX_T *psDemodCtx)
{
UINT8   nDirect = 0, nBestDirect = cALL_DIRECT_NUM, nGain = 0, nBestGain = cALL_GAIN_NUM;
UINT8   nDctIdx = 0;
UINT16  u2CurErrorRate = cPER_MAX, u2BestPER = cPER_MAX;
UINT8   u1curSNR = 0;
UINT8  nTime = 0;
//UINT8  DirectList[cALL_DIRECT_NUM] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
UINT8  nGainStep[cALL_GAIN_NUM] = {1, 3, 1, 1};
UINT8  nGainMax[cALL_GAIN_NUM] = {1, 4, 3, 4};

// Criteria

    nBestDirect = cALL_DIRECT_NUM;
    nBestGain = cALL_GAIN_NUM;

    for (nGain = 0; nGain < nGainMax[cSCAN_GAIN_NUM - 1]; nGain = (nGain + nGainStep[cSCAN_GAIN_NUM - 1]))
    {
        nDctIdx = 0;
        while (nDctIdx < cALL_DIRECT_NUM)
        {
            vDriverReset(psDemodCtx);
            vDriverInit(psDemodCtx);
            vWriteSmartAnt(psDemodCtx, nDctIdx, nGain, POL_NORMAL);
            DTD_Acquire(psDemodCtx, TRUE);

            mcDELAY_MS(cWAIT_LOCK_TIME);
            DTD_UpdateInfo(psDemodCtx);
        #if fcCHK_LOCK
            if (mcTEST_BIT(psDemodCtx->ucLockStatus, cpACQ_STATUS_CR) &&
                mcTEST_BIT(psDemodCtx->ucLockStatus, cpACQ_STATUS_TR))
        #endif
            {
                for (nTime = cWAIT_SNR_TIME / cSLEEP_INTERVAL; nTime > 0; nTime--)
                {
                    mcDELAY_MS(cSLEEP_INTERVAL);
                // get PER
                    mcSHOW_DBG_MSG(("D%d G%d %d %d %d %d\n", nDirect, nGain,
                                    psDemodCtx->u2PktErr, psDemodCtx->u2PktCnt,
                                    psDemodCtx->u4AccPktErr, psDemodCtx->u4AccPktCnt));
                }

            // Calculate PER
                u2CurErrorRate = (UINT16) ((psDemodCtx->u4AccPktErr / psDemodCtx->u4AccPktCnt)
                                           * cPER_SCALE * cPER_PRE_SCALE);

        #if fcCHK_SNR
                u1curSNR = psDemodCtx->u1SNR;

                if ((u1curSNR >= cSCAN_SNR_THR) && (u2CurErrorRate <= cSCAN_PER_THR))
                {
                // good SNR
                    nBestDirect = nDirect;
                    nBestGain   = nGain;

                    mcSHOW_DBG_MSG(("D%d G%d locked, %d dB, %d, good SNR ",
                                    nDirect, nGain, u1curSNR, u2CurErrorRate));
                    mcSHOW_DBG_MSG(("B%d,%d\n", nBestDirect, nBestGain));
                    break;
                }
                else
        #endif
                {
                // normal case
                    if (u2CurErrorRate < u2BestPER) // compare PER
                    {
                        nBestDirect = nDirect;
                        nBestGain   = nGain;
                        u2BestPER   = u2CurErrorRate;
                    }
                    mcSHOW_DBG_MSG(("D%d G%d locked, %d dB, %d, %d, %d ",
                                    nDirect, nGain, u1curSNR, u2CurErrorRate,
                                    psDemodCtx->u4AccPktErr, psDemodCtx->u4AccPktCnt));
                    mcSHOW_DBG_MSG(("B%d,%d\n", nBestDirect, nBestGain));

                    nDctIdx = nDctIdx + cALL_DIRECT_NUM / cSCAN_DIRECT_NUM;
                }
            }
        #if fcCHK_LOCK
            else
            {
            // unlocked
                mcSHOW_DBG_MSG(("D%d G%d unlocked ", nDirect, nGain));
                mcSHOW_DBG_MSG(("B%d,%d\n", nBestDirect, nBestGain));

            #if (cSCAN_DIRECT_NUM == cALL_DIRECT_NUM)
                nDctIdx = nDctIdx + 2;
            #else
                nDctIdx = nDctIdx + cALL_DIRECT_NUM / cSCAN_DIRECT_NUM;
            #endif
            }
        #endif
        }
    }

    if (nBestDirect == cALL_DIRECT_NUM)
        mcSHOW_DBG_MSG(("Not found!\n"));
}
#endif

/***********************************************************************/
/*              Acquisition APIs                                       */
/***********************************************************************/

/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
/***********************************************************************/
/*              Initialization APIs                                    */
/***********************************************************************/
/***********************************************************************/
/*              Configuration APIs                                     */
/***********************************************************************/
/***********************************************************************/
/*              Query APIs                                             */
/***********************************************************************/
BOOL DTD_GetTsOutFmt(DEMOD_CTX_T *psDemodCtx, UINT8 * u1Ret)
{
	UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
  UINT8 u1Value=0;


    mcSET_CMD2(aucData) = cCMD_ID_TS_GET_FMT;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_TS_FMT_SIZE, 1))
    {
        u1Value = mcGET_QUEUE(aucData, cOFST_DEMOD_TS_FMT_ALL);
        mcSHOW_DBG_MSG(("%s u1Value=0x%02X\n",__FUNCTION__,u1Value));
        *u1Ret=u1Value;
        return TRUE;
     }
     return FALSE;
}
VOID DTD_SetTsOutFmt(DEMOD_CTX_T *psDemodCtx, UINT8 u1Value)
{
	  UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
	 
	  mcSHOW_DBG_MSG(("%s u1Value=0x%02X\n",__FUNCTION__,u1Value));
	  
	  mcSET_CMD2(aucData) = cCMD_ID_TS_SET_FMT;
    mcSET_QUEUE2(aucData, cOFST_DEMOD_TS_FMT_ALL) = u1Value;
    mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_DEMOD_TS_FMT_SIZE + 1);
}
BOOL DTD_GetTsFmt(DEMOD_CTX_T *psDemodCtx, UINT8 *u1MpegFmt)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
UINT8   u1GetMode, u1Value;

    u1GetMode = 0;
    mcSET_CMD2(aucData) = cCMD_ID_TS_GET_FMT;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_TS_FMT_SIZE, 1))
    {
        u1Value = mcGET_QUEUE(aucData, cOFST_DEMOD_TS_FMT_ALL);
        

        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_VALID_LOW)) // Valid Low Active
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_VALID_LOW);
        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_SYNC_LOW))  // Sync Low Active
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_SYNC_LOW);
        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_ERR_LOW))
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_ERR_LOW);
        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_PARALLEL))  // Output Parallel
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_PARALLEL);
        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_CLK_CONT))  // Clock Continuous
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_CLK_CONT);
        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_DATA_FALL)) // Data Falling Edge
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_DATA_FALL);
        if (mcTEST_BIT(u1Value, DEMOD_TS_FORMAT_OUTPUT_EN))
            mcSET_BIT(u1GetMode, cBIT_TS_FMT_OUTPUT_EN);
        *u1MpegFmt = u1GetMode;
        return (TRUE);
    }
    return (FALSE);
}

/***********************************************************************/
//VOID DTD_SetTsFmt(DEMOD_CTX_T *psDemodCtx, MPEG_FMT_T *pt_mpeg_fmt)
VOID DTD_SetTsFmt(DEMOD_CTX_T *psDemodCtx, UINT8 u1MpegFmt)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
UCHAR   ucSetMode;

    ucSetMode = 0;
/*
    if (pt_mpeg_fmt->eValidPolarity == POL_LOW_ACTIVE)  // Valid Low Active
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_VALID_LOW);
    if (pt_mpeg_fmt->eSyncPolarity == POL_LOW_ACTIVE)   // Sync Low Active
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_SYNC_LOW);
    if (pt_mpeg_fmt->eClockPolarity == POL_LOW_ACTIVE)  // Data Falling Edge
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_DATA_FALL);
    if (pt_mpeg_fmt->fgParallelOutput)                  // Output Parallel
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_PARALLEL);
    if (!pt_mpeg_fmt->fgGatedClock)                     // Clock Continuous
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_CLK_CONT);
*/

    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_VALID_LOW))   // Valid Low Active
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_VALID_LOW);
    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_SYNC_LOW))    // Sync Low Active
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_SYNC_LOW);
    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_ERR_LOW))
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_ERR_LOW);
    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_PARALLEL))    // Output Parallel
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_PARALLEL);
    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_CLK_CONT))    // Clock Continuous
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_CLK_CONT);
    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_DATA_FALL))   // Data Falling Edge
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_DATA_FALL);
    if (mcTEST_BIT(u1MpegFmt, DEMOD_TS_FORMAT_OUTPUT_EN))
        mcSET_BIT(ucSetMode, cBIT_TS_FMT_OUTPUT_EN);

    mcSET_CMD2(aucData) = cCMD_ID_TS_SET_FMT;
    mcSET_QUEUE2(aucData, cOFST_DEMOD_TS_FMT_ALL) = ucSetMode;
    mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_DEMOD_TS_FMT_SIZE + 1);
}

/***********************************************************************/
VOID DTD_SetCtmrReq(DEMOD_CTX_T *psDemodCtx, UINT32 u4CtmrReq)
{
    psDemodCtx->u4DrvCustTunerCtrl = u4CtmrReq;
//    if (u4CtmrReq == 1)
    if (mcTEST_BIT(u4CtmrReq, cCUST_REQ_0))
        vSetFwVar(psDemodCtx, eVAR_FGDTD_QSYMBOLRATE, 1);

}


/***********************************************************************/
VOID DTD_GetQAMInterleave(DEMOD_CTX_T *psDemodCtx, UINT8 *u1InterleavingI, UINT8 *u1InterleavingJ)
{
UINT8 u1Value;

    u1Value = ucGetReg(psDemodCtx, DTD_REG_FTS_STATUS);
    u1Value &= 0x0F;
    switch (u1Value)
    {
        case 0:
        case 1:
    // Qam Interleave mode [I, J]
            *u1InterleavingI = 128;
            *u1InterleavingJ = 1;
            break;

        case 2:
            *u1InterleavingI = 128;
            *u1InterleavingJ = 2;
            break;

        case 3:
            *u1InterleavingI = 64;
            *u1InterleavingJ = 2;
            break;

        case 4:
            *u1InterleavingI = 128;
            *u1InterleavingJ = 3;
            break;

        case 5:
            *u1InterleavingI = 32;
            *u1InterleavingJ = 4;
            break;

        case 6:
            *u1InterleavingI = 128;
            *u1InterleavingJ = 4;
            break;

        case 7:
            *u1InterleavingI = 16;
            *u1InterleavingJ = 8;
            break;

        case 9:
            *u1InterleavingI = 8;
            *u1InterleavingJ = 16;
            break;

        default:
            *u1InterleavingI = 0;
            *u1InterleavingJ = 0;
        //    mcSHOW_HW_MSG(("QAM interleave Mode undefined\n"));
            break;
    }
}

INT32 i4ConvAgcCtrl2Vol(DEMOD_CTX_T *psDemodCtx, INT32 i4CtrlWord)
{
    // Tranform Control Word to Gain Voltage(mV)
    CMD_SIGLVL_PARAM_T param;
    param.Id=IFAGCCTRL2VOL;
    param.Freq=psDemodCtx->Frequency;
    param.Modulation=psDemodCtx->Modulation;
    param.IOpara=i4CtrlWord;
    ITuner_OP(psDemodCtx->sTunerCtx,itGetSignalLevel,0,&param);
    return param.IOpara;
}

INT32 i4ConvAgcVol2Ctrl(DEMOD_CTX_T *psDemodCtx, INT32 i4Vol)
{
    // Tranform to Gain Voltage(mV) to Control Word
    CMD_SIGLVL_PARAM_T param;
    param.Id=IFAGCVOL2CTRL;
    param.Freq=psDemodCtx->Frequency;
    param.Modulation=psDemodCtx->Modulation;
    param.IOpara=i4Vol;
    ITuner_OP(psDemodCtx->sTunerCtx,itGetSignalLevel,0,&param);
    return param.IOpara;
}


//INT16 i2ConvAgcCtrl2Pwr(DEMOD_CTX_T *psDemodCtx, INT16 i2CtrlWord, INT32 *i4Output)
VOID i2ConvAgcCtrl2Pwr(DEMOD_CTX_T *psDemodCtx, INT16 i2CtrlWord, INT32 *i4Output)
{
    CMD_SIGLVL_PARAM_T param;
    param.Id=IFAGCCTRL2PWR;
    param.Freq=psDemodCtx->Frequency;
    param.Modulation=psDemodCtx->Modulation;
    param.IOpara=i2CtrlWord;
    ITuner_OP(psDemodCtx->sTunerCtx,itGetSignalLevel,0,&param);
    i4Output[0]=param.IOpara;//Input is Signal power dBm
    param.Id=IFAGCCTRL2LVL;
    ITuner_OP(psDemodCtx->sTunerCtx,itGetSignalLevel,0,&param);
    i4Output[1] = param.IOpara;//Signal Level
}

VOID vCalcAGCGain(DEMOD_CTX_T *psDemodCtx, UINT8 *aucData, INT32 *ai4AGCGain)
{
INT32 i4Value;

    mcSHOW_API_MSG(("vCalcAGCGain\n"));

    // RF Gain
    i4Value = (mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_AGC_RF0),
                 mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_AGC_RF1),
                 6, 8) >> 6);
    ai4AGCGain[cAGC_RF_CTRL] = i4Value;

    // IF Gain
    i4Value = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_AGC_IF0),
                 mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_AGC_IF1),
                 6, 8);
    ai4AGCGain[cAGC_IF_CTRL] = i4Value >> 6;

    // IF Gain Voltage
    ai4AGCGain[cAGC_IF_VOL] = i4ConvAgcCtrl2Vol(psDemodCtx, i4Value);

    // RF Signal Power Level (dBm) & RF Signal Strength (0-100)
    i2ConvAgcCtrl2Pwr(psDemodCtx, i4Value, &(ai4AGCGain[cAGC_IF_PWR_DBM]));
}

BOOL fgCalcCFO(DEMOD_CTX_T *psDemodCtx, UINT8 *aucData, INT32 *i4CfoInHz)
{
BOOL fgIsCFO = TRUE;
INT32 i4Vlaue;

      mcSHOW_API_MSG(("fgCalcCFO\n"));
	
      // CFO in Hertz
      if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
      {
       i4Vlaue = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR0),
                                    mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR1),
                                    8, 8) * 48;  // 47.684 ~= 48
       (*i4CfoInHz) = i4Vlaue;                               
    }
    else
    {
        if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5391)
        {
             i4Vlaue = ((mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR0) << 16) +
                             (mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR1) << 8)  +
	                      mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR2));
             
  	        if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR0), 7))
               i4Vlaue = i4Vlaue - (1<<24);
  	        
   	        i4Vlaue = i4Vlaue * 25 * 1000000 / (1<<26);
	        *i4CfoInHz = -i4Vlaue;
         }
	  else
	  {
            if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR0), 1))
            {                                   // CPO
                  fgIsCFO = FALSE;
                 *i4CfoInHz = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR1),
                                        mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR2),
                                        4, 8) *1000/ 4;
            }
            else
            {                                   // CFO
                *i4CfoInHz = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR1),
                                         mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR2),
                                         7, 8) * 3 / 4;
               if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_CR1), 7))
                   *i4CfoInHz = -(*i4CfoInHz)*1000 / 16;
               else
                   *i4CfoInHz = -(*i4CfoInHz);

             }
         }
    }

    return fgIsCFO;
}

BOOL DTD_Get_AGCGain_CFO(DEMOD_CTX_T *psDemodCtx, INT32 *pi4Buffer, BOOL *fgIsCFO)
{
BOOL fgRetSts;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO1;
    fgRetSts = mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO1_SIZE, 1);
    if (fgRetSts)
    {
        vCalcAGCGain(psDemodCtx, aucData, &pi4Buffer[cAGC_RF_CTRL]);
        (*fgIsCFO) = fgCalcCFO(psDemodCtx, aucData, &pi4Buffer[cVCR_CFO]);
    }

    return fgRetSts;
}

/***********************************************************************/
INT32 i4CalcTFO(DEMOD_CTX_T *psDemodCtx, UINT8 *aucData)
{
INT32 i4Data, i4Data2;

    if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
    {
        i4Data = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_TR0),
                             mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_TR1), 8, 8);
        i4Data = i4Data * 745 / 1000; /* Orig Formula: dataf *= 0.745058(Hz) */
    }
    else
    {
       i4Data = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_TR1),
                             mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_TR0), 8, 8);
       /* Orig Formula: (ppm)
          dataf = dataf*pow(2.0, -20)*2/25.0*pow(10,6);
          dataf = dataf*5.057; for 64QAM, and dataf = dataf*5.361; for 256QAM */
       i4Data2 = (1000000 / 25) >> 10;
       if (psDemodCtx->Modulation == DTD_MOD_J83B_64QAM)
           i4Data2 = ((i4Data2 * 5057) >> 10);
       else
           i4Data2 = ((i4Data2 * 5361) >> 10);
       i4Data *= i4Data2 / 1000;
     }
     return i4Data;
}

BOOL DTD_GetTFO(DEMOD_CTX_T *psDemodCtx, INT32 *pi4Buffer)
{
BOOL fgRetSts;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

      mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO4;
      fgRetSts = mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO4_SIZE, 1);
    if (fgRetSts)
    {
        pi4Buffer[0] = i4CalcTFO(psDemodCtx, aucData);
        pi4Buffer[1] = (INT32) mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_EQ_STEP);
        pi4Buffer[2] = (INT32) mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_LEAKAGE);
    }
    return fgRetSts;
}

VOID DTD_ManualAGC(DEMOD_CTX_T *psDemodCtx, BOOL fgRFEn, BOOL fgIFEn, UINT16 *pu2GainVol)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
INT16 i2Value;

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_SET_MANUAL_AGC;

    mcSET_QUEUE2(aucData, cCMD_ID_DEMOD_MAGC_EN) = fgRFEn + (fgIFEn << cpMAGC_IF_EN);
    i2Value = (INT16)i4ConvAgcVol2Ctrl(psDemodCtx, pu2GainVol[0]);
    mcSET_QUEUE2(aucData, cCMD_ID_DEMOD_MAGC_RF_CTRL_0) = mcHIGH_BYTE(i2Value) & 0x3F;
    mcSET_QUEUE2(aucData, cCMD_ID_DEMOD_MAGC_RF_CTRL_1) = mcLOW_BYTE(i2Value);
    i2Value = (INT16)i4ConvAgcVol2Ctrl(psDemodCtx, pu2GainVol[1]);
    mcSET_QUEUE2(aucData, cCMD_ID_DEMOD_MAGC_IF_CTRL_0) = mcHIGH_BYTE(i2Value) & 0x3F;
    mcSET_QUEUE2(aucData, cCMD_ID_DEMOD_MAGC_IF_CTRL_1) = mcLOW_BYTE(i2Value);

    mcHOST_CMD(psDemodCtx, aucData, 0, cCMD_ID_DEMOD_MAGC_SIZE + 1);
}

VOID szGetCFOStr(INT32 i4Value, BOOL fgIsCFO, char *szCFO)
{
    if (!fgIsCFO)
        sprintf(szCFO, "%"cFMT_IL"rad", i4Value);
    else
    {
          if ((i4Value > 1000) || (i4Value < -1000)) /* Larger than 1KHz */
            sprintf(szCFO, "%"cFMT_IL"KHz", i4Value / 1000);
        else
            sprintf(szCFO, "%"cFMT_IL"Hz", i4Value);
    }
}

// Get uPIF Reg
#define uPIF_REG_CNT    0x10
VOID DTD_Get_UPIF_Reg(DEMOD_CTX_T *psDemodCtx)
{
UINT8 ii;
UINT8 au1Data[uPIF_REG_CNT];

    u1GetMultiRegs(psDemodCtx, DTD_REG_uP_Ctrl, au1Data, uPIF_REG_CNT);
    mcSHOW_USER_MSG(("DTD uPIF Module\n"));
    for (ii = 0; ii < uPIF_REG_CNT/4; ii ++)
    {
        mcSHOW_USER_MSG(("r 0x%08X: 0x%02X%02X%02X%02X\n", cRISC_DTD_BASE + DTD_REG_uP_Ctrl + 4*ii,
                          au1Data[4*ii + 3], au1Data[4*ii + 2], au1Data[4*ii + 1], au1Data[4*ii]));
    }
}

/***********************************************************************/
/*              Acquisition APIs                                       */
/***********************************************************************/
/***********************************************************************/
/*              Includes Extra Source                                  */
/***********************************************************************/
//#include "PI_Demod.c"

/***********************************************************************/
/*              Conditional Compiling                                  */
/***********************************************************************/
#define fcADD_REG_RESET         1
#define fcUP_50M_CLK            1
#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)
#define fcADD_ACQ_SEMA          1

/***************** Debug Only *********************/
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcRESET_UP_LOAD         0
#define fcHOSTCMD_NO_SLEEP      0
#define fcNEW_HOST_LOCK         0

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define cPLL_POLLING_TIME       10          //  in ms
#define cPER_MONI_DUR           500         //  in ms

/***********************************************************************/
#define cDEMOD_MAX_CHAN_CFG     135          // 1 Cfg for 1 channel with signal


/***********************************************************************/
/*              Global Variables                                       */
/***********************************************************************/
volatile UINT32 *pu4DTD_Reg = (volatile UINT32 *) cRISC_DTD_BASE;


/***********************************************************************/
/*              Global Variables                                       */
/***********************************************************************/
#if fcDEMOD_GEN == cMT5391_DTD
#if defined(CC_LINUX_KERNEL) && defined(CC_ARM_PLATFORM)
__align(4) UINT8 ucCodeImage_91[] =
#else
WORD_ALIGN UINT8 ucCodeImage_91[] =
#endif
{
#ifndef fcSLT_FW
#include "CodeImg_91.h"
#else
#include "CodeImg_91_SLT.h"
#endif
};
#endif

#if fcDEMOD_GEN == cMT5387_DTD
WORD_ALIGN UINT8 ucCodeImage_87[] =
{
#include "CodeImg_87.h"
};
#endif

#if fcDEMOD_GEN == cMT5112EE
WORD_ALIGN UINT8 ucCodeImage_EE[] =
{
#include "CodeImg_EE.h"
};
#endif

STATIC CHAR *cMsgUpErr[] =
{
    "",
    cMSG_UP_ERR_I2C,
    cMSG_UP_ERR_HW_RDY,
    cMSG_UP_ERR_CHKSUM_RDY,
//    cMSG_UP_ERR_CHKSUM_OK,
    cMSG_UP_ERR_CHKSUM_DL_OK,
    cMSG_UP_ERR_CHKSUM_DEM_OK
};

VOID vRestoreCfg(DEMOD_CTX_T *psDemodCtx);

DEMOD_CFG_T sDemodCfg[cDEMOD_MAX_CHAN_CFG], sCurCfg;
UINT8       ucCfgIdxMap[cMAX_CHAN_TABLE_NUM + 1], ucCfgNum = 0;

/***********************************************************************/
/*              Static Functions prototype                             */
/***********************************************************************/
STATIC VOID DTD_SetTunerPara(DEMOD_CTX_T *psDemodCtx);


/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
UINT16 DTD_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    if (u1I2cDemWrite(psDemodCtx->psI2cCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_HW_MSG((" _@IF_ Write Error: %02X-%03X!\n",
                       psDemodCtx->I2cAddress, u2RegAddr));

        return 0;
    }

    return u2ByteCount;
}

/***********************************************************************/
/* Read consecutive registers */
UINT16 DTD_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    if (u1I2cDemRead(psDemodCtx->psI2cCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG((" _@IF_ Read Error: %02X-%02X!\n",
                       psDemodCtx->I2cAddress, u2RegAddr));

        return 0;
    }

    return u2ByteCount;
}

/***********************************************************************/
BOOL fgHostCmd(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
{
UCHAR   ucWriteParacnt;
UINT16  ii;
UINT8   ucValue;
UCHAR   ucParaStart;
BOOL    fgRetStatus = TRUE;
#if fcMEAS_HOST_CMD_TIME
TIME_TICK_T u4TickStart, u4TickCnt;
#endif
UCHAR   ucReadChkCnt;
#if fcADD_HOSTCMD_CHKSUM
UINT8   u1ChkSum;
#endif
UINT16  jj;
#if fcHOSTCMD_BUSY_WAIT && 0
STATIC UINT16   u2MaxCnt = 0;
#endif
#if fcHOSTCMD_BUSY_WAIT == 2
BOOL    fgDelayTypeBusy;
#endif

#if fcMEAS_HOST_CMD_TIME
    mcGET_SYS_TIME(u4TickStart);
#endif

#if fcNEW_HOST_LOCK
    // wait semaphore
    mcSEMA_LOCK(psDemodCtx->hHostCmdLock);
#endif

//    mcSHOW_USER_MSG(("\nfgHostCmd: (%d, %d)\n", ucReadParacnt, ucWriteCmdcnt));
    if (DTD_GetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucValue, 1) == 0)
    {
    //    psDemodCtx->u1I2cNakCnt ++;
        mcSHOW_HW_MSG((" _I2_ "));
        #if fcNEW_HOST_LOCK
        // release semaphore
        mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
        #endif
        return FALSE;                       // I2C fail
    }

    if (!mcTEST_BIT(ucValue, cpUP_ENABLE))
    {
        psDemodCtx->u1UpChkCnt ++;
    //    mcSHOW_HW_MSG(("uP not enabled!\n"));
        mcSHOW_HW_MSG((" _UE_ "));
        #if fcNEW_HOST_LOCK
        // release semaphore
        mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
        #endif
        return FALSE;
    }

    if (DTD_GetReg(psDemodCtx, DTD_REG_uP_GP_REG06, &ucValue, 1) == 0)
    {
        #if fcNEW_HOST_LOCK
        // release semaphore
        mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
        #endif
        return FALSE;                       // I2C fail
    }

    if (ucValue != cFW_RUN_TAG)
    {
        psDemodCtx->u1UpChkCnt ++;
        if (ucValue == cFW_WDT_TAG)
        {
        //    mcSHOW_HW_MSG(("Watchdog reset!\n"));
            mcSHOW_HW_MSG((" _WR_ "));
        }
        else
        {
        //    mcSHOW_HW_MSG(("Fw not running!\n"));
            mcSHOW_HW_MSG((" _FR_ "));
        }
        #if fcNEW_HOST_LOCK
        // release semaphore
        mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
        #endif
        return FALSE;
    }

#if !fcNEW_HOST_LOCK
// wait semaphore
    mcSEMA_LOCK(psDemodCtx->hHostCmdLock);
#endif

#if fcHOSTCMD_BUSY_WAIT == 2
    fgDelayTypeBusy = psDemodCtx->fgUserGetInfo;
#endif
    if (ucWriteCmdcnt)
    {
        ucWriteParacnt = ucWriteCmdcnt - 1;
    #if !fcHOSTCMD_NO_SLEEP
        #if fcHOSTCMD_BUSY_WAIT == 2
        if (fgDelayTypeBusy)
            fgTunerBusyWait(cHOSTCMD_DELAY_US, TRUE);
        #endif
        for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
    #else
        for (ii = 0; ; ii ++)
    #endif
        {
        #if 1
            DTD_GetReg(psDemodCtx, DTD_REG_uP_CMDR_CFG, &ucValue, 1);
        #else
            if (DTD_GetReg(psDemodCtx, DTD_REG_uP_CMDR_CFG, &ucValue, 1) == 0)
            {                               // I2C fail
                if (ii < (cMAX_WAIT_LOOP - 1))
                {
                    mcDELAY_MS(cWAIT_TIME_INTERVAL);
                    continue;
                }
            //    fgRetStatus = FALSE;
            //    break;
            // release semaphore
                mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
                return FALSE;
            }
        #endif
        // This while loop will cause dead-lock for Combo-Tuner (when I2C_Block = TRUE)
        #if 1
            jj = 0;
            while (mcTEST_BIT(ucValue, cpUP_RSP_TX))
            {
            #if fcCHK_CHIP_REV
                if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5381)
                    DTD_GetReg(psDemodCtx, DTD_REG_RISC_RSP07, &ucValue, 1);
                else
            #endif
                    DTD_GetReg(psDemodCtx, DTD_REG_uP_RSPR07, &ucValue, 1);
                DTD_GetReg(psDemodCtx, DTD_REG_uP_CMDR_CFG, &ucValue, 1);
                jj ++;
                if (jj >= cMAX_WAIT_LOOP)
                {
                    mcSHOW_HW_MSG((" _RSP_CLR_ "));
                    break;
                }
            }
        #endif
            if (!mcTEST_BIT(ucValue, cpUP_CMD_RX))
                break;
        #if !fcHOSTCMD_NO_SLEEP
            mcTUNER_HOSTCMD_WAIT();
        #endif
        }
    #if fcMEAS_HOST_CMD_TIME
        mcGET_SYS_TIME(u4TickCnt);
        mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCnt);
        mcSHOW_TIME_MSG((" ** Cmd%02X %d (%3u)", mcSET_CMD2(aucCmdData),
                        ii, mcCONV_SYS_TIME(u4TickCnt)));
    #endif
        if (ucWriteParacnt > cCMD_REG_NUM - 1)
        {
            mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d > %d!",
                            mcSET_CMD2(aucCmdData), ucWriteParacnt, cCMD_REG_NUM - 1));
            ucWriteParacnt = cCMD_REG_NUM - 1;
        }
        #if fcADD_HOSTCMD_CHKSUM
        if (ucWriteParacnt == (cCMD_REG_NUM - 1))
        {
            mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d == %d!",
                            mcSET_CMD2(aucCmdData), ucWriteParacnt, cCMD_REG_NUM - 1));
        }
        else
        {
            u1ChkSum = 0;
            for (ii = 0; ii < cCMD_REG_NUM - 1; ii ++)
            {
                u1ChkSum ^= aucCmdData[cCMD_REG_NUM - 1 - ii + 1];
            }
            aucCmdData[1] = u1ChkSum;
            ucWriteParacnt = cCMD_REG_NUM - 1;
        }
        #endif
        if (ii == cMAX_WAIT_LOOP)
        {
            mcSHOW_HW_MSG((" _CMD_BUSY_ "));
            psDemodCtx->u1UpNakCnt ++;
        }
    #if fcHOSTCMD_BUSY_WAIT && 0
        else
        {
            if (u2MaxCnt < ii)
            {
                u2MaxCnt = ii;
                mcSHOW_DBG_MSG((" _CMD_%d ", ii));
            }
        }
    #endif
    //    Set(DTD_REG_uP_CMDR00 + cPARA_NUM_POS, &ucWriteParacnt, 1);
        ucParaStart = cCMD_REG_NUM - ucWriteParacnt - 1;

    #if fcCHK_CHIP_REV
        if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5381)
            u1HostWriteCmd(psDemodCtx, DTD_REG_RISC_CMD00,
                           aucCmdData, ucWriteParacnt, ucParaStart);
        else
    #endif
            u1HostWriteCmd(psDemodCtx, DTD_REG_uP_CMDR00,
                           aucCmdData, ucWriteParacnt, ucParaStart);
    #if fcMEAS_HOST_CMD_TIME
        mcGET_SYS_TIME(u4TickCnt);
        mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCnt);
        mcSHOW_TIME_MSG((" Para %d (%3u)", ucWriteParacnt, mcCONV_SYS_TIME(u4TickCnt)));
    #endif
    #if fcSHOW_HOST_CMD
        if (fgShowHostCmd)
        {
        //    mcSHOW_USER_MSG(("HostCmd: (%d) -->", ucParaStart));
            mcSHOW_USER_MSG(("HostCmd -->"));
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + 1 + ii]));
            mcSHOW_USER_MSG(("\n           "));
        }
    #endif
    }

    if (ucReadParacnt)
    {
    UCHAR   ucCmdId;

        ucCmdId = mcSET_CMD2(aucCmdData);
        fgRetStatus = FALSE;
        for (jj = 0; jj < cMAX_TRY_CNT; jj ++)
//        for (jj = 0; jj < 1; jj ++)
        {
        #if !fcHOSTCMD_NO_SLEEP
            #if fcHOSTCMD_BUSY_WAIT == 2
            if (fgDelayTypeBusy)
                fgTunerBusyWait(cHOSTCMD_DELAY_US, TRUE);
            #endif
            for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
        #else
            for (ii = 0; ; ii ++)
        #endif
            {
            #if 1
                DTD_GetReg(psDemodCtx, DTD_REG_uP_CMDR_CFG, &ucValue, 1);
            #else
                if (DTD_GetReg(psDemodCtx, DTD_REG_uP_CMDR_CFG, &ucValue, 1) == 0)
                {                           // I2C fail
                    if (ii < (cMAX_WAIT_LOOP - 1))
                    {
                        mcDELAY_MS(cSHORT_SLEEP_TIME);
                        continue;
                    }
                //    fgRetStatus = FALSE;
                //    break;
                // release semaphore
                    mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
                    return FALSE;
                }
            #endif
                if (mcTEST_BIT(ucValue, cpUP_RSP_TX))
                    break;
            #if !fcHOSTCMD_NO_SLEEP
                mcTUNER_HOSTCMD_WAIT();
            #endif
            }
        //    Get(DTD_REG_uP_RSPR00 + cPARA_NUM_POS, &ucReadParacnt, 1);
            if (ucReadParacnt > cCMD_REG_NUM - 1)
            {
                mcSHOW_HW_MSG(("Rsp %02X Para Cnt %d > %d!",
                                mcSET_CMD2(aucCmdData), ucReadParacnt, cCMD_REG_NUM - 1));
                ucReadParacnt = cCMD_REG_NUM - 1;
            }
            ucReadChkCnt = ucReadParacnt;
        #if fcADD_HOSTCMD_CHKSUM
            if (ucReadChkCnt < cCMD_REG_NUM - 1)
                ucReadChkCnt ++;
        #endif
            if (ii < cMAX_WAIT_LOOP)
            {
            UINT16  u2Addr = DTD_REG_uP_RSPR00;

            #if fcHOSTCMD_BUSY_WAIT && 0
                if (u2MaxCnt < ii)
                {
                    u2MaxCnt = ii;
                    mcSHOW_DBG_MSG((" _RSP_%d ", ii));
                }
            #endif
            #if fcMEAS_HOST_CMD_TIME
                mcGET_SYS_TIME(u4TickCnt);
                mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCnt);
                mcSHOW_TIME_MSG((" Rsp %d_%d (%3u)", jj, ii, mcCONV_SYS_TIME(u4TickCnt)));
            #endif
                ucParaStart = cCMD_REG_NUM - ucReadChkCnt - 1;

            #if fcCHK_CHIP_REV
                if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5381)
                    u2Addr = DTD_REG_RISC_RSP00;
            #endif
                if (u1HostReadRsp(psDemodCtx, u2Addr,
                                  aucCmdData, ucReadChkCnt, ucParaStart) == 0)
                {
                // release semaphore
                    mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);
                    return FALSE;
                }
            #if fcMEAS_HOST_CMD_TIME
                mcGET_SYS_TIME(u4TickCnt);
                mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCnt);
                mcSHOW_TIME_MSG((" Para %d (%3u)", ucReadChkCnt, mcCONV_SYS_TIME(u4TickCnt)));
            #endif
                if (mcGET_CMD(aucCmdData) == ucCmdId)
                {
                    fgRetStatus = TRUE;
            #if fcSHOW_HOST_CMD
                    if (fgShowHostCmd)
                    {
                        mcSHOW_USER_MSG(("HostRsp -->"));
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                            mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                        mcSHOW_USER_MSG(("\n"));
                    }
            #endif
            #if fcADD_HOSTCMD_CHKSUM
                    if (ucReadParacnt < cCMD_REG_NUM - 1)
                    {
                        u1ChkSum = 0;
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                        {
                            u1ChkSum ^= aucCmdData[cCMD_REG_NUM - 1 - ii];
                        }
                        if (u1ChkSum)
                        {
                            fgRetStatus = FALSE;
                            mcSHOW_HW_MSG((" _@CS_ "));
                            DTD_GetReg(psDemodCtx, DTD_REG_uP_RSPR00 + ucParaStart,
                                          aucCmdData + ucParaStart, ucReadChkCnt + 1);
                            mcSHOW_USER_MSG(("-->"));
                            for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                            mcSHOW_USER_MSG(("\n"));
                            psDemodCtx->u1UpMisCnt ++;
                        }
                    }
            #endif
                    break;
                }
                else
                {
                    mcSHOW_HW_MSG((" _CMD_MIS_ "));
                    psDemodCtx->u1UpMisCnt ++;
                }
            }
            else
            {
                mcSHOW_HW_MSG((" _RSP_BUSY_ "));
                psDemodCtx->u1UpNakCnt ++;
                break;
            }
        }
    //    mcSHOW_HW_MSG(("Error uP Rsp!"));
    }

// release semaphore
    mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);

#if fcMEAS_HOST_CMD_TIME
    mcGET_SYS_TIME(u4TickCnt);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCnt);
//    mcSHOW_DBG_MSG((" %3uH ", mcCONV_SYS_TIME(u4TickCnt)));
    mcSHOW_TIME_MSG((" %3uH ", mcCONV_SYS_TIME(u4TickCnt)));
    mcSHOW_TIME_MSG(("\n"));
#endif

    return fgRetStatus;
}

/***********************************************************************/
UINT8 u1GetRegOrVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt, UINT8 u1CmdId)
{
UINT8   ii;
UINT8   au1Data[cMAX_READ_NUM + 1];         // For Host Cmd
//UINT8   au1Data[cOFST_REG_ALL_SIZE + 1];      // For Host Cmd

    if (u1Cnt > (cOFST_REG_ALL_SIZE - cOFST_REG1_DATA))
        u1Cnt = (cOFST_REG_ALL_SIZE - cOFST_REG1_DATA);
    mcSET_CMD2(au1Data) = u1CmdId;
    mcSET_QUEUE2(au1Data, cOFST_REG_NUM) = u1Cnt;
    mcSET_QUEUE2(au1Data, cOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    mcSET_QUEUE2(au1Data, cOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    if (mcHOST_CMD(psDemodCtx, au1Data, cOFST_REG1_SIZE + u1Cnt - 1, cOFST_REG1_SIZE + 1))
    {
        for (ii = 0; ii < u1Cnt; ii ++)
            pu1Data[ii] = mcGET_QUEUE(au1Data, cOFST_REG1_DATA + ii);
    }
    else
        return 0;
    return u1Cnt;
}

UINT8 u1SetRegOrVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt, UINT8 u1CmdId)
{
UINT8   ii;
UINT8   au1Data[cMAX_READ_NUM + 1];         // For Host Cmd
//UINT8   au1Data[cOFST_REG_ALL_SIZE + 1];      // For Host Cmd

    if (u1Cnt > (cOFST_REG_ALL_SIZE - cOFST_REG1_DATA))
        u1Cnt = (cOFST_REG_ALL_SIZE - cOFST_REG1_DATA);
    mcSET_CMD2(au1Data) = u1CmdId;
    mcSET_QUEUE2(au1Data, cOFST_REG_NUM) = u1Cnt;
    mcSET_QUEUE2(au1Data, cOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    mcSET_QUEUE2(au1Data, cOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    for (ii = 0; ii < u1Cnt; ii ++)
        mcSET_QUEUE2(au1Data, cOFST_REG1_DATA + ii) = pu1Data[ii];
//    if (mcHOST_CMD(psDemodCtx, au1Data, 0, cOFST_REG1_SIZE + u1Cnt - 1))
    if (mcHOST_CMD(psDemodCtx, au1Data, 0, cOFST_REG1_SIZE + u1Cnt))
    {
        return u1Cnt;
    }
    return 0;
}

/***********************************************************************/
UINT8 u1GetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt)
{
    return u1GetRegOrVars(psDemodCtx, u2Addr, pu1Data, u1Cnt,
                          cCMD_ID_DEMOD_GET_VAR);
}

UINT8 ucGetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
UINT8   u1Value = 0;

    u1GetFwVars(psDemodCtx, u2Addr, &u1Value, 1);
    return u1Value;
}

/***********************************************************************/
UINT8 u1SetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 *pu1Data, UINT8 u1Cnt)
{
    return u1SetRegOrVars(psDemodCtx, u2Addr, pu1Data, u1Cnt,
                          cCMD_ID_DEMOD_SET_VAR);
}

VOID vSetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 u1Value)
{
    u1SetFwVars(psDemodCtx, u2Addr, &u1Value, 1);
}

/***********************************************************************/
VOID vSetTunerType(VOID *psCtx, UINT8 u1TunerType, UINT16 u2AgcTimeOut)
{
DEMOD_CTX_T *psDemodCtx = (DEMOD_CTX_T *) psCtx;

    vSetFwVar(psDemodCtx, eVAR_UCTUNERTYPE, u1TunerType);
    if (u2AgcTimeOut)
    {
    UCHAR   aucValue[2];

        aucValue[0] = mcLOW_BYTE(u2AgcTimeOut);
        aucValue[1] = mcHIGH_BYTE(u2AgcTimeOut);
        u1SetFwVars(psDemodCtx, eVAR_U2QAGCTIMEOUT_H, aucValue, 2);
    }
}
//-------------------------------------------------------------------------
/** vChipInit
 *  Set Fw/Reg when DTD initialize.
 *  @param  psDemodCtx    Pointer of demod device driver context.
 */
//-------------------------------------------------------------------------

VOID vChipInit(DEMOD_CTX_T *psDemodCtx)
{
#if fcDEMOD_GEN == cMT5112EE
UINT8 u1MpegFmt;
#endif
#ifdef CC_DYNAMIC_POWER_ONOFF
UINT8 u1Value;
#endif

/* Demod dependent */
#if fcDEMOD_GEN == cMT5112EE    
    // Set TS Format to be serial mode    
    #ifndef CC_DEMOD_TS_PARALLEL
    DTD_GetTsFmt(psDemodCtx, &u1MpegFmt);          
    mcCLR_BIT(u1MpegFmt, cBIT_TS_FMT_PARALLEL);    
    DTD_SetTsFmt(psDemodCtx, u1MpegFmt);
    #endif
#endif

#ifdef CC_DYNAMIC_POWER_ONOFF
   // Power off montor clk (New in 5392b)
   if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5391)
   {
       u1Value = 0x70; // bit6-4 = 3'b111
       DTD_SetReg(psDemodCtx, DTD_REG_Chip_Mon_Ctrl, &u1Value, 1);
   }
#endif
  
   // Set default transmission phase
   psDemodCtx->u1TransPhase = 0x40;

}

//-------------------------------------------------------------------------
/** vSetADSampling
 *  Set AD Sampling rate.
 *  @param  psDemodCtx    Pointer of demod device driver context.
 *  @param  u1Rate        Sampling Rate(Only for fixed case): eAD_SAMPLING_50MHZ or eAD_SAMPLING_60MHZ. 
 *  @param  u1Fix         flag to fix sampling rate: eFIX or eNOT_FIX
 */
//-------------------------------------------------------------------------
VOID vSetADSampling(DEMOD_CTX_T *psDemodCtx, UINT8 u1Rate, UINT8 u1Fix)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_SET_SAMPLING;

    mcSET_QUEUE2(aucData, cOFST_DEMOD_SAMPLING_RATE) = u1Rate;
    mcSET_QUEUE2(aucData, cOFST_DEMOD_SAMPLING_FIXED) = u1Fix;

    mcHOST_CMD(psDemodCtx, aucData, 0, cCMD_ID_DEMOD_SAMPLING_SIZE + 1);
}


/***********************************************************************/
VOID DTD_Acquire(DEMOD_CTX_T *psDemodCtx, BOOL fgStart)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

//    mcSHOW_HW_MSG(("DTD_Acquire: %d\n", psDemodCtx->Modulation));
    if (fgStart)                            // Start Auto-Acq
    {
        mcSET_CMD2(aucData) = cCMD_ID_DEMOD_START;
    //    mcSET_QUEUE2(aucData, cOFST_DEMOD_START_RESET) = 0;
//        if (m_fgDrvRunning)
            mcSET_QUEUE2(aucData, cOFST_DEMOD_START_RESET) = 1;
    }
    else                                    // Manual Set to Wait-AcqStart State
    {
        mcSET_CMD2(aucData) = cCMD_ID_DEMOD_STOP;
	 mcSHOW_DBG_MSG(("DigitalDemod is stop now!\n"));
    }
    mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_DEMOD_START_SIZE + 1);
}

/***********************************************************************/
/*              Initialization APIs                                    */
/***********************************************************************/
INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT32 u4I2cClkDiv, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{
UCHAR   ucValue, u1I2cAuto, ucUpCtrl, ii;
UINT16  u2CurI2cLen, u2CurRamAdr;
UCHAR   u1I2cAddr;
#if fcCHK_CHIP_REV
UINT16  u2Addr;
#endif
#if fcLOAD_CODE_TIME
TIME_TICK_T u4TickStart, u4TickCur;
TIME_DIFF_T u4TickCnt;
TIME_CONV_T u4TickCnt0;
#endif

 // Switch SRAM to DTD
 if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5391)
 {
      DTD_GetReg(psDemodCtx, DTD_REG_Chip_Misc_Sel, &ucValue, 1);
      mcCLR_BIT(ucValue, 4);
      DTD_SetReg(psDemodCtx, DTD_REG_Chip_Misc_Sel, &ucValue, 1);
 }

    if (DTD_GetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1) == 0)
        return cUP_LOAD_ERR_I2C;
    if (!mcTEST_BIT(ucUpCtrl, cpUP_MEM_INIT_DONE))
    {
        return cUP_LOAD_ERR_HW_RDY;
    }

#if fcSKIP_LOAD_CODE
    if (mcTEST_BIT(ucUpCtrl, cpUP_ENABLE))
    {
        DTD_GetReg(psDemodCtx, DTD_REG_uP_GP_REG06, &ucValue, 1);
        if (ucValue == cFW_RUN_TAG)
        {
    #if fcRESET_UP_LOAD
            ucUpCtrl = mcBIT(cpUP_DN_FREQ); // Reset uP
            DTD_SetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);
        #if fcUP_50M_CLK
            ucUpCtrl = mcBIT(cpUP_ENABLE);
        #else
            ucUpCtrl = mcBIT(cpUP_ENABLE) | mcBIT(cpUP_DN_FREQ);
        #endif
            DTD_SetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);
    #endif
            return cUP_LOAD_OK;
        }
    }  
#endif

    ucUpCtrl = mcBIT(cpUP_DN_FREQ) | mcBIT(cpUP_RESET_CHKSUM);  // Disable uP
    DTD_SetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);

    // After uP disable, uP MEM Reset for MT5112EE or MT5381
    if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5381)
    {
        ucValue = 0x00;
        DTD_SetReg(psDemodCtx, DTD_REG_Chip_Mem_Rst, &ucValue, 1);
        ucValue = 0x01;
        DTD_SetReg(psDemodCtx, DTD_REG_Chip_Mem_Rst, &ucValue, 1);
    }
    else if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_EE) // EE ES & EE MP
    {
        ucValue = 0x00;
        DTD_SetReg(psDemodCtx, MT5112EE_REG_Chip_Mem_Rst, &ucValue, 1);
        ucValue = 0x01;
        DTD_SetReg(psDemodCtx, MT5112EE_REG_Chip_Mem_Rst, &ucValue, 1);    
    }
    
    ucValue = 0;
    DTD_SetReg(psDemodCtx, DTD_REG_uP_GP_REG06, &ucValue, 1);
    for (ii = 0; ii < 0x10; ii++)
    {
        DTD_GetReg(psDemodCtx, (UINT16) (DTD_REG_uP_RESERVED + ii), &ucValue, 1);
        DTD_SetReg(psDemodCtx, (UINT16) (DTD_REG_uP_RESERVED + ii), &ucValue, 1);
    }


// Turn off I2C auto-increment
    DTD_GetReg(psDemodCtx, DTD_REG_I2C_Cfg, &u1I2cAuto, 1);
    mcCLR_BIT(u1I2cAuto, cpI2C_AUTO_INC);
    DTD_SetReg(psDemodCtx, DTD_REG_I2C_Cfg, &u1I2cAuto, 1);

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    u2CurRamAdr = 0;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
#if fcCHK_CHIP_REV
    if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5381)
    {
        DTD_SetReg(psDemodCtx, MT5381_REG_uP_IRA_H, &ucValue, 1);
        ucValue = mcLOW_BYTE(u2CurRamAdr);
        DTD_SetReg(psDemodCtx, MT5381_REG_uP_IRA_L, &ucValue, 1);
        u2Addr = MT5381_REG_uP_IRData;
    }
    else
#endif
    {
        DTD_SetReg(psDemodCtx, MT5112_REG_uP_IRA_H, &ucValue, 1);
        ucValue = mcLOW_BYTE(u2CurRamAdr);
        DTD_SetReg(psDemodCtx, MT5112_REG_uP_IRA_L, &ucValue, 1);
#if fcCHK_CHIP_REV
        u2Addr = MT5112_REG_uP_IRData;
#endif
    }
    u1I2cAddr = psDemodCtx->I2cAddress;
#if fcLOAD_CODE_TIME && 0
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt0, u4TickStart, u4TickCur);
    mcSHOW_DBG_MSG((" (Start %3u ms)", mcCONV_SYS_TIME(u4TickCnt0)));
#endif
    while (u2CurRomLen)
    {
        u2CurI2cLen = u2CurRomLen;
        if (u2CurI2cLen > u2MaxI2cLen)
            u2CurI2cLen = u2MaxI2cLen;
    //    DTD_SetReg(psDemodCtx, DTD_REG_uP_IRData, u1CodeImage + u2CurRamAdr, u2CurI2cLen);
#if fcCHK_CHIP_REV
        if (u1I2cWriteSpeed(psDemodCtx->psI2cCtx, u4I2cClkDiv, u1I2cAddr, u2Addr,
                      u1CodeImage + u2CurRamAdr, u2CurI2cLen))
#else
        if (u1I2cWriteSpeed(psDemodCtx->psI2cCtx, u4I2cClkDiv, u1I2cAddr, MT5112_REG_uP_IRData,
                      u1CodeImage + u2CurRamAdr, u2CurI2cLen))
#endif
        {
        //    mcSHOW_POP_MSG(("Download I2C fail! (%d)\n", u4I2cClkDiv));
            mcSHOW_HW_MSG(("Download I2C fail! (%"cFMT_UL")\n", u4I2cClkDiv));
            break;
        }
        u2CurRamAdr += u2CurI2cLen;
        u2CurRomLen -= u2CurI2cLen;
    #if fcLOAD_CODE_TIME && 0
        ii ++;
        if ((ii % 64) == 0)
        {
            mcGET_SYS_TIME(u4TickCur);
            mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
            mcSHOW_DBG_MSG((" (%3u --> %3d ms)", ii,
                            mcCONV_SYS_TIME(u4TickCnt - u4TickCnt0)));
            u4TickCnt0 = u4TickCnt;
        }
    #endif
    }
#if fcLOAD_CODE_TIME && 0
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    mcSHOW_DBG_MSG((" (%3u ms / Loop %3u) ", mcCONV_SYS_TIME(u4TickCnt),
                    mcCONV_SYS_TIME(u4TickCnt - u4TickCnt0)));
#endif


    // Turn on I2C auto-increment
    mcSET_BIT(u1I2cAuto, cpI2C_AUTO_INC);
    DTD_SetReg(psDemodCtx, DTD_REG_I2C_Cfg, &u1I2cAuto, 1);

    // Check uCode download process checksum
    DTD_GetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);
    if (!mcTEST_BIT(ucUpCtrl, cpUP_CHKSUM_OK))
    {
        return cUP_LOAD_ERR_CHKSUM_DL_OK;
    }

#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
#endif

    ucUpCtrl = 0;
    mcSET_BIT(ucUpCtrl, cpUP_RESTART_CHKSUM);
    DTD_SetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);
#if fcSHORT_LINUX_INIT && (fcHOSTCMD_BUSY_WAIT == 2)
    fgTunerBusyWait(cHOSTCMD_DELAY_US, TRUE);
    for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
#else
    for (ii = 0; ii < 8; ii ++)
#endif
    {
        DTD_GetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, cpUP_CHKSUM_DONE))
            break;
#if fcSHORT_LINUX_INIT && (fcHOSTCMD_BUSY_WAIT == 2)
        if (fgTunerBusyWait(cHOSTCMD_DELAY_US, FALSE))
            break;
#else
        mcDELAY_MS(cWAIT_TIME_INTERVAL);    // Unit: ms
#endif
    }
    if (!mcTEST_BIT(ucUpCtrl, cpUP_CHKSUM_DONE))
    {
        return cUP_LOAD_ERR_CHKSUM_RDY;
    }

    if (!mcTEST_BIT(ucUpCtrl, cpUP_CHKSUM_OK))
    {
        return cUP_LOAD_ERR_CHKSUM_MEM_OK;
    }

    ucUpCtrl = mcBIT(cpUP_ENABLE);          // 50 MHz Clock
    if (!fgClk50)
        ucUpCtrl = mcBIT(cpUP_ENABLE) | mcBIT(cpUP_DN_FREQ);    // 25 MHz Clock
    DTD_SetReg(psDemodCtx, DTD_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcSHOW_DBG_MSG2(("uP Startup!"));

#if fcLOAD_CODE_TIME
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    u4TickCnt0 = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" (%3"cFMT_UL" ms) ", u4TickCnt0));
    if (u4TickCnt0 > 0)
        return -((INT32) u4TickCnt0);
    else
#endif
        return cUP_LOAD_OK;
}

/***********************************************************************/
VOID DTD_Sleep(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_UP_PWR_DN;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);
 
#ifdef fcEXTERN_DEMOD
    mcSET_CMD2(aucData) = cCMD_ID_SET_IFAGC_HIGH_Z;
    mcSET_QUEUE2(aucData, cOFST_SET_IFAGC_EN) = 1;  // 1: High-Z
    mcHOST_CMD(psDemodCtx, aucData, 0, cCMD_ID_SET_IFAGC_HIGH_Z_SIZE + 1);
#endif

//    mcPRINTF("\n***** Sleep ****\n");
}

VOID DTD_Wakeup(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_UP_PWR_UP;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);
//    mcPRINTF("\n**** Wakeup ****\n");
}

/***********************************************************************/
#define cCHECK_TIME_LOOP        128

INT32 DTD_Init(DEMOD_CTX_T *psDemodCtx)
{
INT32   i4ErrIdx;
UCHAR   *pucCodeImage=NULL;
UINT16  u2CodeSize=0;
//UCHAR   *pucCodeImage = ucCodeImage_91;
//UINT16  u2CodeSize = sizeof(ucCodeImage_91);

#if fcDEMOD_GEN == cMT5391_DTD
    pucCodeImage = ucCodeImage_91;
    u2CodeSize = sizeof(ucCodeImage_91);
#elif fcDEMOD_GEN == cMT5387_DTD
    pucCodeImage = ucCodeImage_87;
    u2CodeSize = sizeof(ucCodeImage_87);
#endif

#if (fcMEASURE_DRIVER_TIME == 3) && fcINTERNAL_TEST && 0
UINT16  ii;
#endif

#if fcDEMOD_GEN == cMT5112EE
    if (psDemodCtx->u1ChipRev == DTD_CHIP_REV_EE || psDemodCtx->u1ChipRev == DTD_CHIP_REV_EE_MP)    	// For MT5112EE    
    {        
        pucCodeImage = ucCodeImage_EE;        
        u2CodeSize = sizeof(ucCodeImage_EE);    
    }
#endif
    
#if (fcMEASURE_DRIVER_TIME == 3) && fcINTERNAL_TEST && 0
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
    for (ii = 0; ii < cCHECK_TIME_LOOP; ii ++)
    {
        mcDELAY_MS(20);                     // --> 20 ms
                                            // --> 30 ms in Linux
    }
    mcESTIMATE_TIME((" [%s/%s-%d, %d] ", __FILE__, __FUNCTION__, __LINE__, mcCONV_SYS_TIME(u4TickDiffCnt1) / cCHECK_TIME_LOOP));
    for (ii = 0; ii < cCHECK_TIME_LOOP; ii ++)
    {
        mcDELAY_MS(1);                      // --> 5 ms
                                            // --> 20 ms in Linux
    }
    mcESTIMATE_TIME((" [%s/%s-%d, %d] ", __FILE__, __FUNCTION__, __LINE__, mcCONV_SYS_TIME(u4TickDiffCnt1) / cCHECK_TIME_LOOP));
#endif

    DTD_Wakeup(psDemodCtx);              // Note: Must not at power-down mode when load code
                                            //       or FW may read wrong register value

    // Use faster I2C clock: 27 MHz / 33 = 818 kHz (27 MHz / 0x100 = 105.5 kHz)
    /* u4ClkDiv = 15 (1.8 M) --> I2C Fail */
    /* u4ClkDiv = 16 (1.6875 M) --> Download 295 ms */
    /* u4ClkDiv = 20 (1.35 M) --> Download 325 ms */
    /* u4ClkDiv = 25 (1.08 M) --> Download 360 ms */
    /* u4ClkDiv = 33 (  818 k) --> Download 420 ms */

    if ((i4ErrIdx = s4LoadCode(psDemodCtx, 33, pucCodeImage, u2CodeSize, cMAX_I2C_LEN, fcUP_50M_CLK)) > 0)
    {
        if (i4ErrIdx < cUP_LOAD_ERR_CNT)
        {
            mcSHOW_HW_MSG(("%s\n", cMsgUpErr[i4ErrIdx]));
        }
    // Try again with slower I2C speed
        if ((i4ErrIdx = s4LoadCode(psDemodCtx, 0x100, pucCodeImage, u2CodeSize, cMAX_I2C_LEN, fcUP_50M_CLK)) > 0)
        {
            if (i4ErrIdx < cUP_LOAD_ERR_CNT)
            {
                mcSHOW_HW_MSG(("%s\n", cMsgUpErr[i4ErrIdx]));
            }
            return (DRVAPI_TUNER_ERROR);    // Load code fail
        }
    }

#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
#endif

    mcDELAY_MS(20);                         // Wait uP ready

    return 0;
}

/***********************************************************************/
VOID ResetRegister(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    DTD_Acquire(psDemodCtx, FALSE);
    mcSET_CMD2(aucData) = cCMD_ID_RESET_INBAND;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);
}


/***********************************************************************/
VOID vDriverInit(DEMOD_CTX_T *psDemodCtx)
{
    mcSHOW_API_MSG(("vDriverInit\n"));
    psDemodCtx->ucConnFlag = 0;
    psDemodCtx->fgInitFlag = TRUE;
    psDemodCtx->u4AccPktErr = 0;
    psDemodCtx->u4AccPktCnt = 0;
    psDemodCtx->u1PecFlag = 0;
    psDemodCtx->u2FwAcqTime = 0;
    psDemodCtx->u2AccReAcqCnt = 0;
    psDemodCtx->u1StatusInd = 0;
    psDemodCtx->i1MosaicCnt = 0;
    if (psDemodCtx->Frequency != psDemodCtx->PreFrequency)
    {
        psDemodCtx->fgMosaicUnLock = 0;
        mcSHOW_DBG_MSG(("Freq%d->%d: Reset fgMosaicUnLock = 0\n", psDemodCtx->PreFrequency, psDemodCtx->Frequency));
        psDemodCtx->PreFrequency = psDemodCtx->Frequency;
    }
}

/***********************************************************************/
VOID vDriverFlagReset(DEMOD_CTX_T *psDemodCtx)
{
    psDemodCtx->fgResetFlag = TRUE;
    psDemodCtx->ucLockStatus = 0;
    psDemodCtx->ucLockState = 0;
    psDemodCtx->ucSigCnt = 0;
}

/***********************************************************************/
VOID vDriverReset(DEMOD_CTX_T *psDemodCtx)
{
    vDriverFlagReset(psDemodCtx);
    vRestoreCfg(psDemodCtx);

}

/***********************************************************************/
/*              Configuration APIs                                     */
/***********************************************************************/
INT32 DTD_SetMod(DEMOD_CTX_T *psDemodCtx, UCHAR eModType)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

// check invalid input
    if (eModType >= MOD_TYPE_END)
    {
        return (-1);
    }

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_SET_MOD;
//    mcSET_QUEUE1(aucData, cOFST_DEMOD_MOD) = eModType;
    mcSET_QUEUE2(aucData, cOFST_DEMOD_MOD) = eModType;

    if (!mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_DEMOD_MOD_SIZE + 1))
    {
        return (-1);
    }

    psDemodCtx->Modulation = eModType;

    return 0;
}
                                                                                                
/***********************************************************************/
/*              Query APIs                                             */
/***********************************************************************/
INT32 DTD_GetMod(DEMOD_CTX_T *psDemodCtx, UCHAR *peModType)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_MOD;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_MOD_SIZE, 1))
    {
    UCHAR   ucDemodMode;

        ucDemodMode = mcGET_QUEUE(aucData, cOFST_DEMOD_MOD);
        if (ucDemodMode >= DTD_MOD_TYPE_END)
        {
            ucDemodMode = DTD_MOD_ATSC_8VSB;
            mcSHOW_HW_MSG(("Mod: %02X (%02X, %02X, %02X)",
                           mcGET_QUEUE(aucData, cOFST_DEMOD_MOD),
                           mcGET_QUEUE(aucData, cOFST_DEMOD_MOD_DBG1),
                           mcGET_QUEUE(aucData, cOFST_DEMOD_MOD_DBG2),
                           mcGET_QUEUE(aucData, cOFST_DEMOD_MOD_DBG3)));
        }
        *peModType = ucDemodMode;
        psDemodCtx->Modulation = ucDemodMode;
    }
    else
    {
        return -1;
    }

    return 0;
}

/***********************************************************************/
//#define cSNR_SCALE              2048
#define cSNR_SCALE              256
#define cSNR_SCALE_DB           9           // 10 * log10(2048 / 256)

UINT16 u2CalcSNR(DEMOD_CTX_T *psDemodCtx, UCHAR *pucTmp)
{
INT16   i2RawErrValue;
UINT16  u2RawErrValue;

    i2RawErrValue = mc2BYTE(pucTmp[0], pucTmp[1]);

    if (i2RawErrValue <= 0)
        return 0;

    u2RawErrValue = (UINT16) i2RawErrValue;
    if (psDemodCtx->Modulation == MOD_J83B_64QAM)
        u2RawErrValue = 42 * cSNR_SCALE / u2RawErrValue;
    else
        u2RawErrValue = 170 * cSNR_SCALE / u2RawErrValue;

    return u2RawErrValue;
}

VOID vCalcPER(DEMOD_CTX_T *psDemodCtx, UCHAR *pucTmp)
{
UINT16 u2Total, u2Error;
STATIC  UCHAR   ucPerMoniCnt = 0;
#if 0
UCHAR   ii;

    for (ii = 0; ii < 4; ii ++)
        mcSHOW_USER_MSG((" %02X", pucTmp[ii]));
    mcSHOW_USER_MSG(("\n"));
#endif
    u2Total = ((((UCHAR)(pucTmp[3])) & mcMASK(7)) * 256 + 255) * pucTmp[2];
    u2Error = mc2BYTE(pucTmp[0], pucTmp[1]);
//    u2Total = ((((UCHAR)(pucTmp[0])) & mcMASK(7)) * 256 + 255) * pucTmp[1];
//    u2Error = mc2BYTE(pucTmp[3], pucTmp[2]);

    //mcSHOW_DBG_MSG2((" PER: %04X, %04X", u2Total, u2Error));

    if (u2Error == 0xFFFF)
    {
        psDemodCtx->u2PktErr = 0xFFFF;
        psDemodCtx->u2PktCnt = 0x0000; // Add
        
        ucPerMoniCnt ++;
    //    mcSHOW_DBG_MSG2((" %d", ucPerMoniCnt));
        if (ucPerMoniCnt < (cPER_MONI_DUR / cUNCLOCK_POLLING_TIME))
            return;
        psDemodCtx->u2ErrorRate = cPER_MAX;
        //mcSHOW_DBG_MSG2(("PerNotReady"));
    }
    else
    {
        psDemodCtx->u2PktErr = u2Error;
        psDemodCtx->u2PktCnt = u2Total;
        if (u2Total)
        {
            psDemodCtx->u4AccPktErr += u2Error;
            psDemodCtx->u4AccPktCnt += u2Total;
        //    psDemodCtx->u2ErrorRate = (UINT16) (((u2Error / u2Total) * cPER_SCALE * cPER_PRE_SCALE));
            psDemodCtx->u2ErrorRate = (UINT16) (((UINT32) u2Error * cPER_SCALE * cPER_PRE_SCALE) / u2Total);

        // In order to prevent overflow, check the order of u4AccPktErr to decide the resolution
            if (psDemodCtx->u4AccPktCnt)
            {
                if (psDemodCtx->u4AccPktErr < cUINT32_MAX/100000) // Resolution: 1e-5
                    psDemodCtx->u4AccErrorRate = (UINT32) ( (psDemodCtx->u4AccPktErr * 100000) / (psDemodCtx->u4AccPktCnt) );
                else if (psDemodCtx->u4AccPktErr < cUINT32_MAX/10000) // Resolution: 1e-4
                    psDemodCtx->u4AccErrorRate = (UINT32) ( (psDemodCtx->u4AccPktErr * 10000) / (psDemodCtx->u4AccPktCnt) * 10);
                else if (psDemodCtx->u4AccPktErr < cUINT32_MAX/1000) // Resolution: 1e-3
                    psDemodCtx->u4AccErrorRate = (UINT32) ( (psDemodCtx->u4AccPktErr * 1000) / (psDemodCtx->u4AccPktCnt) * 100);
                else if (psDemodCtx->u4AccPktErr < cUINT32_MAX/100) // Resolution: 1e-2
                    psDemodCtx->u4AccErrorRate = (UINT32) ( (psDemodCtx->u4AccPktErr * 100) / (psDemodCtx->u4AccPktCnt) * 1000);
                else if (psDemodCtx->u4AccPktErr < cUINT32_MAX/10) // Resolution: 1e-1
                    psDemodCtx->u4AccErrorRate = (UINT32) ( (psDemodCtx->u4AccPktErr * 10) / (psDemodCtx->u4AccPktCnt) * 10000);
                else
                    psDemodCtx->u4AccErrorRate = 100000;
            }
            else
            {
                psDemodCtx->u4AccErrorRate = 100000;
            }
        }
        //mcSHOW_DBG_MSG2((" PER: %d, %d", u2Total, u2Error));
    }
    ucPerMoniCnt = 0;
}

/***********************************************************************/
VOID vDTDGetStatus(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   ucValue = 0;
//STATIC  UCHAR   ucPrevReAcqCnt = 0;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSHOW_API_MSG(("vDTDGetStatus\n"));

    if (psDemodCtx->Modulation < MOD_TYPE_END)  // FAT
    {
         
        mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_ACQ;
        if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_ACQ_SIZE, 1))
        {
            psDemodCtx->fgAcqStart = mcGET_QUEUE(aucData, cOFST_DEMOD_ACQ_START);
            psDemodCtx->u2SigLeveldBm = mcGET_QUEUE(aucData, cOFST_DEMOD_SIG_LEVEL);
            ucValue = mcGET_QUEUE(aucData, cOFST_DEMOD_ACQ_STATUS);
            psDemodCtx->ucLockStatus = ucValue;
            psDemodCtx->ucLockState = mcGET_QUEUE(aucData, cOFST_DEMOD_ACQ_STATE);
            psDemodCtx->ucSigCnt = mcGET_QUEUE(aucData, cOFST_DEMOD_SIG_CNT);
            psDemodCtx->u2AcqTime = (UINT16) (mcGET_QUEUE(aucData, cOFST_DEMOD_ACQ_TIME_H)) * 255 / 100;

            psDemodCtx->u1ReAcqCnt = mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_CNT);

            if (psDemodCtx->u1ReAcqCnt)
            {
                psDemodCtx->u2AccReAcqCnt = (UINT16) (psDemodCtx->u2AccReAcqCnt + psDemodCtx->u1ReAcqCnt);
                mcCTMR_DBG_MSG((" _FAT_ACQ%d_ ", psDemodCtx->u2AccReAcqCnt));
                mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_REACQ;
                if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_REACQ_SIZE, 1))
                {                           // Maggie, 061220
                    psDemodCtx->u1StatusInd = mcGET_QUEUE(aucData, cOFST_DEMOD_STATUS_IND);
                    if ((psDemodCtx->Modulation == MOD_ATSC_8VSB) && (psDemodCtx->u1ChipRev == DTD_CHIP_REV_5381MP))
                    {
                        mcCTMR_DBG_MSG(("(%02X) <Cfg_%02X ",
                            mcGET_QUEUE(aucData, cOFST_DEMOD_STATUS_IND), mcGET_QUEUE(aucData, cOFST_DEMOD_STATUS_IND_EXTEND)));
                    }
                    else
                    {
                        mcCTMR_DBG_MSG(("(%02X) ",                        
                            mcGET_QUEUE(aucData, cOFST_DEMOD_STATUS_IND)));
                    }
                }

                if ((psDemodCtx->Modulation == MOD_ATSC_8VSB) && (psDemodCtx->u1ChipRev == DTD_CHIP_REV_5381MP))
                {                	
                    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_REACQ_LOG;
                    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_REACQ_LOG_SIZE, 1))
                    {
                        mcCTMR_DBG_MSG(("Pilot_%d RCnt_%d RCnt2_%d RMetric_%d, CFODev_%d>\n",
                            mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_UCLOG_1), mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_UCLOG_2),
                            mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_UCLOG_3), 
                            mcUNSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_U2LOG_1_H), mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_U2LOG_1_L), 8, 8),
                            mcUNSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_U2LOG_2_H), mcGET_QUEUE(aucData, cOFST_DEMOD_REACQ_U2LOG_2_L), 8, 8)
                            ));
                    }  
                }                         
            }
            psDemodCtx->fgStatusReady = TRUE;
        }
        psDemodCtx->fgLockSignal = TRUE;
        if ((ucValue & 0x3F) != 0x3F)    // any one module is unsync
        {
            psDemodCtx->fgLockSignal = FALSE;
            #if fcIGNORE_VPRA_STAT
              if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
            {
                if ((ucValue & 0x3F) == 0x1F)    // Ignore PrA status
                    psDemodCtx->fgLockSignal = TRUE;
            }
            #endif
        }
    }
}

/***********************************************************************/
UINT16 u2LogTable[] =
{
     10,  13,  16,  20,  25,  32,  40,  50,  63,  79,
    100, 126, 158, 200, 251, 316, 398, 501, 631, 794
};

UINT8 u1Conv_dB(UINT16 u2InpValue)
{
UINT8 ii;

//    mcSHOW_DBG_MSG(("Linear = %d, ", u2InpValue));
    for (ii = 0; ii < sizeof(u2LogTable) / sizeof(*u2LogTable); ii ++)
        if (u2InpValue <= u2LogTable[ii])
            break;

//    mcSHOW_DBG_MSG(("Log = %d\n", ii + 10));
    return (ii + 10);
}

/***********************************************************************/
VOID vDTDGetInfo(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
//UCHAR   ii;
//UCHAR   aucPer[(cOFST_DEMOD_INFO_PER2 - cOFST_DEMOD_INFO_PER0 + 1)];
STATIC  UCHAR   ucPrevTsifCnt = 0;

    mcSHOW_API_MSG(("vDTDGetInfo\n"));

    if (psDemodCtx->Modulation < MOD_TYPE_END)  // FAT
    {
    //    if (psDemodCtx->fgLockSignal)
        {
            mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO2;
            if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO2_SIZE, 1))
            {
                if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
                {
                    psDemodCtx->u2SNRRaw = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR);
                    if (!mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR), 7))
                        psDemodCtx->u1SNR = psDemodCtx->u2SNRRaw / 2;
                    else
                        psDemodCtx->u1SNR = 0;
                #if fcINTERNAL_TEST
                    psDemodCtx->u2SNRRaw_FS = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_FS_SNR);
                    if (!mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_FS_SNR), 7))
                        psDemodCtx->u1SNR_FS = psDemodCtx->u2SNRRaw_FS / 2;
                    else
                        psDemodCtx->u1SNR_FS = 0;
                #endif
                }
                else
                {
                    if (psDemodCtx->u1ChipRev >= DTD_CHIP_REV_5391)
                    {
                        psDemodCtx->u2SNRRaw = mcSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR),
                                             mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_FS_SNR), 7, 8);
                        psDemodCtx->u1SNR = psDemodCtx->u2SNRRaw/256;
                    }
                    else
                    {
                        psDemodCtx->u2SNRRaw = u2CalcSNR(psDemodCtx,
                                                &(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR)) - 1);
                        if (psDemodCtx->u2SNRRaw == 0)
                            psDemodCtx->u1SNR = 0;
                        else
                            psDemodCtx->u1SNR = u1Conv_dB(psDemodCtx->u2SNRRaw) + cSNR_SCALE_DB;
                    	}
                }

#if 0
                vCalcPER(psDemodCtx, &(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_PER0)) - 3);
                psDemodCtx->u1PERRaw1 = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_PER0);
                psDemodCtx->u1PERRaw2 = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_PER1);
 #endif
                psDemodCtx->u1CurTSIFCnt = mcGET_QUEUE(aucData, cOFST_DEMOD_TSIF_RST);
                if (ucPrevTsifCnt != psDemodCtx->u1CurTSIFCnt)
                {
                    ucPrevTsifCnt = psDemodCtx->u1CurTSIFCnt;
                    mcCTMR_DBG_MSG((" _TSIF%d_ ", ucPrevTsifCnt));

                }
                psDemodCtx->fgInfoReady = TRUE;
            }

            // Get VSB SNR
            if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
            {
                mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO5;
                if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO5_SIZE, 1))
                {
                    if (!mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR_1), 7))
                    {
                        psDemodCtx->u2SNRRaw_HighRes = ((mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR_1) << 8) +
                                                         mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SNR_2));
                    }
                    else
                        psDemodCtx->u2SNRRaw_HighRes = 0;

                    //mcSHOW_DBG_MSG(("u2SNRRaw_HighRes=%d\n", psDemodCtx->u2SNRRaw_HighRes));
                }
            }
        }
    //    else
    //    {
    //        psDemodCtx->u1SNR = 0;
    //        psDemodCtx->u2ErrorRate = cPER_MAX;
    //    }
    //    mcSHOW_USER_MSG((" (%d)\n", psDemodCtx->u2ErrorRate));
    }
}

static VOID vDTDGetPERInfo(DEMOD_CTX_T *psDemodCtx)
{
 UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
 UINT32  u4NormTotalCnt;

    mcSHOW_API_MSG(("vDTDGetPERInfo\n"));

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO6;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO6_SIZE, 1))
    {
        vCalcPER(psDemodCtx, &(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO6_PER0)) - 3);
        //mcSHOW_DBG_MSG2(("pu4TotalPkt = %d \n", psDemodCtx->u2PktCnt));
        psDemodCtx->u1PERRaw1 = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO6_PER0);
        psDemodCtx->u1PERRaw2 = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO6_PER1);      
    }
    else
    {
        mcSHOW_DBG_MSG1(("vMT5112GetPERInfo Not Ready\n"));
    }
  	
    // Calculate ErrRate & LossRate
 	  if (psDemodCtx->Modulation == MOD_ATSC_8VSB)        	  	
        u4NormTotalCnt = cVSB_NORMAL_PKT_CNT;
    else if(psDemodCtx->Modulation == MOD_J83B_64QAM)
        u4NormTotalCnt = cQ64_NORMAL_PKT_CNT;
    else  // 256-QAM
        u4NormTotalCnt = cQ256_NORMAL_PKT_CNT;        
     
    if (psDemodCtx->u2PktCnt != 0)    
        psDemodCtx->u1ErrRateIdx = (UINT8) (psDemodCtx->u2PktErr * 100 / psDemodCtx->u2PktCnt);
    else
        psDemodCtx->u1ErrRateIdx = 100;
    if (u4NormTotalCnt > psDemodCtx->u2PktCnt)             	            	
        psDemodCtx->u1LossRateIdx = (UINT8) ((u4NormTotalCnt - psDemodCtx->u2PktCnt) * 100 / u4NormTotalCnt);
    else
    	  psDemodCtx->u1LossRateIdx = 0;

    mcSHOW_DBG_MSG2((" u2PktCnt=%d, u2PktErr=%d\n", psDemodCtx->u2PktCnt, psDemodCtx->u2PktErr));
        
     // Determine heavy mosaic or slight error to determine video mute or unmute
#ifdef CC_TUNER_CONN_DEFAULT_SURF
    mcSHOW_DBG_MSG2(("ConnectionType = %d\n", psDemodCtx->ConnectionType));
        
    if(psDemodCtx->ConnectionType == DEMOD_CONN_TYPE_SURF)//DEMOD_CONN_TYPE_SCAN)
    {                	            
        if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
        {
            if (psDemodCtx->u1ErrRateIdx >= cMOSAIC_VSB_ERR_THRD)
            	 psDemodCtx->i1MosaicCnt++;
             else if ((psDemodCtx->u1ErrRateIdx <= cUNMOSAIC_VSB_ERR_THRD) || (psDemodCtx->u1LossRateIdx <= cUNMOSAIC_VSB_LOSS_THRD))
            	 psDemodCtx->i1MosaicCnt--;
            else
            	 psDemodCtx->i1MosaicCnt = 0;
        }
        else if (psDemodCtx->Modulation == MOD_J83B_64QAM)
        {
            if ((psDemodCtx->u1ErrRateIdx >= cMOSAIC_Q64_ERR_THRD) || (psDemodCtx->u1LossRateIdx >= cMOSAIC_Q64_LOSS_THRD))
            	 psDemodCtx->i1MosaicCnt++;
            else if ((psDemodCtx->u1ErrRateIdx <= cUNMOSAIC_Q64_ERR_THRD) || (psDemodCtx->u1LossRateIdx <= cUNMOSAIC_Q64_LOSS_THRD)) 
            	 psDemodCtx->i1MosaicCnt--;
            else
            	 psDemodCtx->i1MosaicCnt = 0;                	                 	
        }
        else  /* 256-QAM */
        {
            if ((psDemodCtx->u1ErrRateIdx >= cMOSAIC_Q256_ERR_THRD) || (psDemodCtx->u1LossRateIdx >= cMOSAIC_Q256_LOSS_THRD))
            	 psDemodCtx->i1MosaicCnt++;
            else if ((psDemodCtx->u1ErrRateIdx <= cUNMOSAIC_Q256_ERR_THRD) || (psDemodCtx->u1LossRateIdx <= cUNMOSAIC_Q256_LOSS_THRD))
            	 psDemodCtx->i1MosaicCnt--;
            else
            	 psDemodCtx->i1MosaicCnt = 0;                	                 	                		 
        }
                
        if (psDemodCtx->i1MosaicCnt > cMOSAIC_CNT_THRD)
        	psDemodCtx->i1MosaicCnt = cMOSAIC_CNT_THRD;
        else if (psDemodCtx->i1MosaicCnt < cUNMOSAIC_CNT_THRD)	
        	psDemodCtx->i1MosaicCnt = cUNMOSAIC_CNT_THRD;

        mcSHOW_DBG_MSG2(("ER%d-LR%d-Cnt(%d)\n", psDemodCtx->u1ErrRateIdx, psDemodCtx->u1LossRateIdx, psDemodCtx->i1MosaicCnt));
        	        	
        if (psDemodCtx->i1MosaicCnt == cMOSAIC_CNT_THRD)            	
        {
            psDemodCtx->fgMosaicUnLock = 1; // SET TO UNLOCK!!
            mcSHOW_DBG_MSG2(("SET TO UNLOCK!(%d)\n", psDemodCtx->fgMosaicUnLock));
            psDemodCtx->i1MosaicCnt = 0;
        }
        else if (psDemodCtx->i1MosaicCnt == cUNMOSAIC_CNT_THRD)
        {                
            psDemodCtx->fgMosaicUnLock = 0; // SET TO LOCK!!
            mcSHOW_DBG_MSG2(("SET BACK TO LOCK!(%d)\n", psDemodCtx->fgMosaicUnLock));
            psDemodCtx->i1MosaicCnt = 0;
        }    
    }   
#endif    
  
}

VOID DTD_UpdateInfo(DEMOD_CTX_T *psDemodCtx)
{
    mcSHOW_API_MSG(("DTD_UpdateInfo\n"));

    vDTDGetStatus(psDemodCtx);
    vDTDGetInfo(psDemodCtx);
}

/***********************************************************************/
VOID DTD_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if (fgSwitchOn)                         // Access Tuner
    {
        mcSET_CMD2(aucData) = cCMD_ID_TUNER_I2C_ON;
    }
    else                                    // Bypass Tuner
    {
        mcSET_CMD2(aucData) = cCMD_ID_TUNER_I2C_OFF;
    }
    if (mcHOST_CMD(psDemodCtx, aucData, 0, 1))
    {
        if (fgSwitchOn)
        {
            mcSET_CMD2(aucData) = cCMD_ID_TUNER_I2C_STAT;
        #if !fcHOSTCMD_NO_SLEEP
            for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
        #else
            for (ii = 0; ; ii ++)
        #endif
            {
                if (mcHOST_CMD(psDemodCtx, aucData, 1, 1))
                {
                    if (mcGET_QUEUE(aucData, cOFST_TUNER_I2C_STAT))
                        break;
                }
            #if !fcHOSTCMD_NO_SLEEP
                mcDELAY_MS(0);
            #endif
            }
        }
    }

    mcSHOW_DBG_MSG(("%s(%d) is invoked.\n", __FUNCTION__, fgSwitchOn));

}

/***********************************************************************/
BOOL DTD_GetTunerStatus(DEMOD_CTX_T *psDemodCtx)
{
BOOL    fgRetSt;

    #ifdef fcEXTERN_DEMOD
    DTD_TunerI2c(psDemodCtx, TRUE);
    #endif
    ITuner_OP(psDemodCtx->sTunerCtx,itGetStatus,0,&fgRetSt);
    #ifdef fcEXTERN_DEMOD
    DTD_TunerI2c(psDemodCtx, FALSE);
    #endif
    return fgRetSt;
}

/***********************************************************************/
/*              Acquisition APIs                                       */
/***********************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FUNCTION DESCRIPTION                                               */
/*      Change RF frequency                                            */
/*                                                                     */
/*  INPUTS                                                             */
/*      u4Freq: RF frequency in kHz                                    */
/*                                                                     */
/*  OUTPUTS                                                            */
/*      0: Success                                                     */
/*      -1: I2C error                                                  */
/*      -2: Frequency out of range                                     */
/*      1: PLL can't lock                                              */
/*                                                                     */
/***********************************************************************/
INT8 DTD_SetFreq(DEMOD_CTX_T *psDemodCtx, PARAM_SETFREQ_T * param)
{
INT16   i2Err;

//UCHAR   ucValue;
UINT16  ii;

    if (psDemodCtx->Modulation < MOD_TYPE_END)  // FAT
    {
        // ++ Tuner Freq
        #ifdef fcEXTERN_DEMOD
        DTD_TunerI2c(psDemodCtx, TRUE);
        #endif
       i2Err = ITuner_SetFreq(psDemodCtx->sTunerCtx,param);        
        #ifdef fcEXTERN_DEMOD
        DTD_TunerI2c(psDemodCtx, FALSE);
        #endif

        if (i2Err < 0)
        {                                   // I2C bus error
            mcSHOW_HW_MSG(("Tuner bus error!\n"));
            return cRET_SETFREQ_I2C_ERROR;
        }
        else if (i2Err > 0)
        {                                   // frequency out of range
        //    mcSHOW_HW_MSG(("Frequency out of range!\n"));
            return cRET_SETFREQ_RANGE;
        }

        //++ Wait PLL lock
#if fcTUNE_NOT_START
        if (psDemodCtx->u1TunerAcqType >= ACQ_DEMOD_ONLY)
#endif
        {
            ii = 0;
            while (1)
            {
                mcDELAY_MS(cPLL_POLLING_TIME);  // cPLL_POLLING_TIME is defined as 10ms
                ii ++;

                if (DTD_GetTunerStatus(psDemodCtx))
                {
                    mcSHOW_DBG_MSG(("Tuner PLL locked in %d ms\n", ii*cPLL_POLLING_TIME));
                    break;
                }

                if ((ii * cPLL_POLLING_TIME) >= 100)
                {                               // Timeout 100ms
                    mcSHOW_HW_MSG(("### Tuner ERROR ### PLL can't lock in 100ms\n"));
                //    break;
                    return cRET_SETFREQ_PLL_LOCK;
                }
            }
            mcSHOW_DBG_MSG(("Tuner: Delay more 10ms for safety\n"));
            mcDELAY_MS(10);
        }
        //--
    }
    else
        mcSHOW_HW_MSG(("DTD_SetFreq Unknown Mod: %d.\n", psDemodCtx->Modulation));

    return cRET_SETFREQ_OK;
}


/***********************************************************************/
/*              Interface APIs                                         */
/***********************************************************************/
/***********************************************************************/
/*              Initialization APIs                                    */
/***********************************************************************/
//-------------------------------------------------------------------------
/** DTD_Initialize
 *  Initialize tuner device driver.
 *  @param  psDemodCtx  Pointer of tuner device driver context.
 *  @param  u1I2cAddress    Demod I2C address. Default is 0x92
 *  @param  u1RFTunerI2cAddr    Tuner I2C address.
 *  @param  fgHardwareInit  Init demod hardware
 *  @retval 0           Success
 *  @retval -1          Fail
 */
//-------------------------------------------------------------------------
INT32 DTD_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddress, BOOL fgHardwareInit)
{
//UINT8   TryErrCnt;
UINT8   u1Value;
INT32   i4RetSts = DRVAPI_TUNER_OK;
#if fcADD_CUST_IF
UINT32  TunerCtrl;
#endif
#if !fcADD_AD_SAMPLING
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
#endif

  //  psDemodCtx->sTunerCtx.psDemodCtx = psDemodCtx;
    psDemodCtx->I2cAddress = u1I2cAddress;

    psDemodCtx->u1ChipRev = DTD_CHIP_REV_5381;
    
#if !defined(CC_WINDOWS_PLATFORM)
    vCheckHostIf(psDemodCtx);
    /* Set MT539x MAIN CKGEN */
    i4ClockInit();
#endif

/* Check if MT5112 */
    if (DTD_GetReg(psDemodCtx, DTD_REG_Chip_ID, &u1Value, 1) == 0)
    {
        mcSHOW_HW_MSG(("I2C fail!\n"));
        i4RetSts = (-1);
        goto InitExit;
    }

    if (mcGET_FIELD(u1Value, mcMASKS(cwCHIP_ID_GEN, cpCHIP_ID_GEN), cpCHIP_ID_GEN) != 1)    // Chip Generation
    {
        mcSHOW_HW_MSG(("Not MT5112!\n"));
        i4RetSts = (DRVAPI_TUNER_ERROR);
        goto InitExit;
    }

// Get Chip Gen
    psDemodCtx->u1ChipGen = (UINT8) mcGET_FIELD(u1Value, mcMASKS(cwCHIP_ID_GEN, cpCHIP_ID_GEN), cpCHIP_ID_GEN); // Chip Gen.

// Get Chip Rev
    psDemodCtx->u1ChipRev = (UINT8) mcGET_FIELD(u1Value, mcMASKS(cwCHIP_ID_MASK, cpCHIP_ID_MASK), cpCHIP_ID_MASK); // Mask Rev.

#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
#endif

    if (fgHardwareInit)
    {
        if (DTD_Init(psDemodCtx))
        {
            i4RetSts = (-1);
            goto InitExit;
        }
    }

#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
#endif

#if !defined(CC_WINDOWS_PLATFORM) && fcCHK_CHIP_REV
    vConfigHostIf(psDemodCtx);
#endif

// create semaphore for HostCmd Interface
    if (mcSEMA_CREATE(&psDemodCtx->hHostCmdLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hHostCmdLock) Fail!\n"));
        i4RetSts = (DRVAPI_TUNER_ERROR);
        goto InitExit;
    }
    mcSEMA_UNLOCK(psDemodCtx->hHostCmdLock);

    if (mcSEMA_CREATE(&psDemodCtx->hRiscIfLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hRiscIfLock) Fail!\n"));
        i4RetSts = (DRVAPI_TUNER_ERROR);
        goto InitExit;
    }
    mcSEMA_UNLOCK(psDemodCtx->hRiscIfLock);

    if (mcSEMA_CREATE(&psDemodCtx->hRiscIfAtdLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hRiscIfAtdLock) Fail!\n"));
        i4RetSts = (DRVAPI_TUNER_ERROR);
        goto InitExit;
    }
    mcSEMA_UNLOCK(psDemodCtx->hRiscIfAtdLock);

    if (mcSEMA_CREATE(&psDemodCtx->hLogLock, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hLogLock) Fail!\n"));
        i4RetSts = (DRVAPI_TUNER_ERROR);
        goto InitExit;
    }
    mcSEMA_UNLOCK(psDemodCtx->hLogLock);

#if fcADD_ACQ_SEMA
    if (!psDemodCtx->fgDemodInitFlag)
    {
        psDemodCtx->fgDemodInitFlag = TRUE;
    //    if (mcSEMA_CREATE(&hDemodAcq, X_SEMA_STATE_UNLOCK) != OSR_OK)
        if (mcSEMA_CREATE(&psDemodCtx->hDemodAcq, X_SEMA_STATE_LOCK) != OSR_OK)
        {
            mcSHOW_HW_MSG(("TunerOpen: mcSEMA_CREATE(hDemodAcq) Fail!\n"));
            i4RetSts = (DRVAPI_TUNER_ERROR);
            goto InitExit;
        }
        mcSEMA_UNLOCK(psDemodCtx->hDemodAcq);
    }
#endif

    psDemodCtx->Modulation = MOD_ATSC_8VSB;
    psDemodCtx->Frequency  = cTUNER_DEFAULT_FREQ;

    psDemodCtx->u1UpChkCnt = 0;
    psDemodCtx->u1UpNakCnt = 0;
    psDemodCtx->u1UpMisCnt = 0;
    psDemodCtx->fgStatusReady = FALSE;
    psDemodCtx->fgInfoReady = FALSE;
    psDemodCtx->u4AccPktErr = 0;
    psDemodCtx->u4AccPktCnt = 0;
    psDemodCtx->u2SigLeveldBm = 0;
    psDemodCtx->fgMosaicUnLock = 0;    
    psDemodCtx->PreFrequency = psDemodCtx->Frequency;

#if fcTUNE_NOT_START
    psDemodCtx->u1TunerAcqType = ACQ_ACTION_DEFAULT;
#endif
#if !fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = FALSE;
#else
    psDemodCtx->fgUserGetInfo = TRUE;
#endif

//    DTD_SetFreq(psDemodCtx, cTUNER_DEFAULT_FREQ);
//    DTD_Acquire(psDemodCtx, TRUE);

#if fcADD_CUST_IF
// Note: DTD_GetCtmrReq must after DTD_Initialize()
    TunerCtrl = DTD_GetCtmrReq(psDemodCtx);
// DTD_SetCtmrReq should be called after download-code is complete.
    DTD_SetCtmrReq(psDemodCtx, TunerCtrl);
#endif

//#if fcTUNER_TYPE == cTUNER_ALL
#if 0 // Remove old tuner all by Maggie, 2008.08.27
    DTD_GetConfig(psDemodCtx);
//    if (DTD_GetConfig(psDemodCtx))
//        return (DRVAPI_TUNER_ERROR);
#endif

    // Tuner_Init with default u1RFTunerI2cAddr, and also set Fw var TunerType
//#if fcTUNER_TYPE == cTUNER_ALL
#if 0 // Remove old tuner all by Maggie, 2008.08.27
    if (!mcTEST_BIT(psDemodCtx->u4DrvCustTunerCtrl, cCUST_REQ_1))
#endif
        //Rf_Tuner_Init(&(psDemodCtx->sTunerCtx), u1RFTunerI2cAddr);
        ITuner_Init(psDemodCtx->sTunerCtx);
        DTD_SetTunerPara(psDemodCtx);

#if fcMEASURE_DRIVER_TIME == 3
    mcESTIMATE_TIME((" [%s/%s-%d] ", __FILE__, __FUNCTION__, __LINE__));
#endif

//    mcDELAY_MS(100);
#if !fcADD_AD_SAMPLING
     // When sampling rate is not controlled by FW, it should fix at 60MHz.
     mcSET_CMD2(aucData) = cCMD_ID_DEMOD_SET_SAMPLING;
     mcSET_QUEUE2(aucData, cOFST_DEMOD_SAMPLING_RATE) = 1;   // 60Mhz
     mcSET_QUEUE2(aucData, cOFST_DEMOD_SAMPLING_FIXED) = 0; // Fixed
     mcHOST_CMD(psDemodCtx, aucData, 0, cCMD_ID_DEMOD_SAMPLING_SIZE + 1);
#endif

    vChipInit(psDemodCtx);

#ifdef fcSMART_ANT_DRV
    vEnableDetectSmartAnt(psDemodCtx, TRUE);
#endif
    vInitDebug(psDemodCtx);
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = FALSE;
#endif

InitExit:
#if fcCHK_CHIP_REV
//    i4ConnectFinish(psDemodCtx);
#endif

// According to MW/Albert, Extinguish operation after opening.

    return i4RetSts;
}

//-------------------------------------------------------------------------
/** DTD_Close
 *  Cleanup system resource, and put demod into sleep mode
 *  @param  psDemodCtx  Pointer of tuner device driver context.
 *  @retval FALSE       System error
 */
//-------------------------------------------------------------------------
INT32 DTD_Close(DEMOD_CTX_T *psDemodCtx)
{

    vCloseDebug(psDemodCtx);

    DTD_Sleep(psDemodCtx);

    if (mcSEMA_DESTROY(psDemodCtx->hHostCmdLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerClose: mcSEMA_DESTROY(hHostCmdLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    if (mcSEMA_DESTROY(psDemodCtx->hRiscIfLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerClose: mcSEMA_DESTROY(hRiscIfLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    if (mcSEMA_DESTROY(psDemodCtx->hRiscIfAtdLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerClose: mcSEMA_DESTROY(hRiscIfAtdLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

    if (mcSEMA_DESTROY(psDemodCtx->hLogLock) != OSR_OK)
    {
        mcSHOW_HW_MSG(("TunerClose: mcSEMA_DESTROY(hLogLock) Fail!\n"));
        return (DRVAPI_TUNER_ERROR);
    }

#if fcADD_ACQ_SEMA
    if (psDemodCtx->fgDemodInitFlag)
    {
        if (mcSEMA_DESTROY(psDemodCtx->hDemodAcq) != OSR_OK)
        {
            mcSHOW_HW_MSG(("TunerClose: mcSEMA_DESTROY(hDemodAcq) Fail!\n"));
            return (DRVAPI_TUNER_ERROR);
        }
        psDemodCtx->fgDemodInitFlag = FALSE;
    }
#endif
    return (DRVAPI_TUNER_OK);
}

/***********************************************************************/
/*              Configuration APIs                                     */
/***********************************************************************/
/***********************************************************************/
/*              Query APIs                                             */
/***********************************************************************/
//-------------------------------------------------------------------------
/** DTD_GetSync
 *  Get demod and tuner lock status
 *  @param  psDemodCtx  Pointer of demod device driver context maintained by demod driver.
 *  @retval TRUE(1)         Locked
 *  @retval FALSE(0)        Unlocked
 */
//-------------------------------------------------------------------------
INT16 DTD_GetSync(DEMOD_CTX_T *psDemodCtx)
{
INT16   s2RetStatus = 0;
static  UINT8   ucMsgCnt = 0;

    mcSHOW_API_MSG(("DTD_GetSync\n"));

    psDemodCtx->fgUserGetInfo = TRUE;

    if (psDemodCtx->fgInitFlag)
    {
        psDemodCtx->fgInitFlag = FALSE;
    }

    if (psDemodCtx->ucConnFlag == 1)
    {
        psDemodCtx->ucConnFlag = 2;
    }
    DTD_UpdateInfo(psDemodCtx);
    if (psDemodCtx->fgLockSignal)
    {
        s2RetStatus = 1;

    //+++++++++++++++
    #ifdef CC_TUNER_CONN_DEFAULT_SURF
        // For Video mute function: Add a criteria for MOSAIC_UNLOCK
        if(psDemodCtx->ConnectionType == DEMOD_CONN_TYPE_SURF)
        {   
            if (psDemodCtx->fgMosaicUnLock)
            {
                s2RetStatus = 0;
            }
            mcSHOW_DBG_MSG(("GetSync: ConnectionType%d-fgLock%d-fgMosaic%d ", psDemodCtx->ConnectionType,psDemodCtx->fgLockSignal, psDemodCtx->fgMosaicUnLock));
        }
    #endif
    //-------------------
    }
    ucMsgCnt ++;

    //    if (ucMsgCnt >= 0x10)
    //    if (psDemodCtx->ucConnFlag == 0)
    {
        ucMsgCnt = 0;
        vShowDebug(psDemodCtx);
    }
    psDemodCtx->fgUserGetInfo = FALSE;

    return s2RetStatus;
}

/***********************************************************************/
UINT8 DTD_GetSignalLevel(DEMOD_CTX_T *psDemodCtx)
{

    if (psDemodCtx->fgLockSignal)
//    if ((psDemodCtx->Modulation < MOD_TYPE_END) // FAT
//        && DTD_GetSync(psDemodCtx))
    {
    UINT16  u2SigLevel;
    BOOL    fgIsCFO;
    INT32   ai4Data[cAGC_INFO_SIZE];
	
#if fcINTERNAL_TEST && 0
        mcSHOW_DBG_MSG(("SNR = %d dB\n", (UINT16) (psDemodCtx->u1SNR)));
        mcSHOW_DBG_MSG(("PER = %d\n", (UINT16) (psDemodCtx->u2ErrorRate)));
#endif

        DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO);
        mcSHOW_DBG_MSG2(("RF Signal = %d dBm, Ctrl=%d mv, %d mv\n", ai4Data[cAGC_IF_PWR_DBM], ai4Data[cAGC_IF_CTRL],  ai4Data[cAGC_IF_VOL]));
        u2SigLevel = (UINT16) ai4Data[cAGC_SIG_STRENGTH];
       
        if (u2SigLevel > 100)
            u2SigLevel = 100;
        return (UINT8) u2SigLevel;
    }
    return 0;
}

//-------------------------------------------------------------------------
/** DTD_GetSignalLeveldBm
 *  Get RF signal level (dBm).
 *  @param  psDemodCtx    Pointer of demod device driver context.
 *  @retval RF signal level (dBm).
 */
//-------------------------------------------------------------------------
INT16 DTD_GetSignalLeveldBm(DEMOD_CTX_T *psDemodCtx)
{
INT32   ai4Data[cAGC_INFO_SIZE];
BOOL    fgIsCFO;
  
    if (psDemodCtx->fgLockSignal)
    {
        DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO);
        mcSHOW_DBG_MSG2(("RF Signal = %d dBm\n", (ai4Data[cAGC_IF_PWR_DBM])));
	 return (ai4Data[cAGC_IF_PWR_DBM]);	
    }
    return -1000;
}

#define cVSB_HIGH_RES_SNR_SCALE     512
#define cQAM_HIGH_RES_SNR_SCALE     256
/***********************************************************************/
UINT16 DTD_GetSignalSNR(DEMOD_CTX_T *psDemodCtx)
{
    if (psDemodCtx->fgLockSignal)
    {
#ifndef CC_SDAL
        return psDemodCtx->u1SNR;
#else
        if (psDemodCtx->Modulation == MOD_ATSC_8VSB)
            return (10 * psDemodCtx->u2SNRRaw_HighRes / cVSB_HIGH_RES_SNR_SCALE);
        else
            return (10 * psDemodCtx->u2SNRRaw / cQAM_HIGH_RES_SNR_SCALE);
#endif
    }
    return 0;
}

/***********************************************************************/
UINT16 DTD_GetSignalPER(DEMOD_CTX_T *psDemodCtx)
{
	
//    if (psDemodCtx->fgLockSignal)
//    if ( (psDemodCtx->fgLockSignal) && (psDemodCtx->fgPecReady) )
//    {
//        if (psDemodCtx->u4AccErrorRate > 0xFFFF)
//              return (0xFFFF);
 //         else
           return (psDemodCtx->u4AccErrorRate);
 //   }
//    return (cPER_MAX);
}

/***********************************************************************/
UINT16 DTD_GetFWAcqTime(DEMOD_CTX_T *psDemodCtx)
{
UINT8 aucValue[2];
UINT16 u2AcqTime = 0;

    if (u1GetFwVars(psDemodCtx, eVAR_U2ACQTIME_H, aucValue, 2))
      u2AcqTime = ((aucValue[0] << 8) + aucValue[1]) ;

    return u2AcqTime;
}
#if 0
/***********************************************************************/
VOID vSetADTarget(RF_TUNER_CTX_T *psTunerCtx, BOOL fgManualEn, UINT8 *u1TarLevel)
{
    if (!fgManualEn)
        vSetFwVar((DEMOD_CTX_T *) psTunerCtx->psDemodCtx, eVAR_FGMANUAL_AD_TL, 0);
    else
    {
    UINT8 u1Value[3];

        u1Value[0] = u1TarLevel[0];         // AD_Target_Level_0
        u1Value[1] = u1TarLevel[1];         // AD_Target_Level_1
        u1Value[2] = 0x01;                  // Enable fgMaunal_AD_Target_Level
        u1SetFwVars((DEMOD_CTX_T *) psTunerCtx->psDemodCtx, eVAR_UCAD_TARGET_LEVEL_0, (UINT8 *) &u1Value, 3);
   }
}
#endif
STATIC VOID DTD_SetTunerPara(DEMOD_CTX_T *psDemodCtx)
{
//ITUNER_CTX_T *psTunerCtx = psDemodCtx->sTunerCtx;
SPECIFIC_MEMBER_US_CTX *psTunerCtx=&(psDemodCtx->sTunerCtx->specific_member.us_ctx);
UINT8 u1Value[3];
    
   mcSHOW_API_MSG(("DTD_SetTunerPara\n"));
     
  // Set AD target level
   if(psTunerCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] == TRUE) 
   {
   	   u1Value[0] = psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB]; // AD_Target_Level_0
   	   u1Value[1] = psTunerCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB]; // AD_Target_Level_1
   	   u1Value[2] = 0x01;                                               // Enable fgMaunal_AD_Target_Level
       u1SetFwVars(psDemodCtx, eVAR_UCAD_TARGET_LEVEL_0, (UINT8 *) &u1Value, 3);
   }        	
   else
   {
        vSetFwVar(psDemodCtx, eVAR_FGMANUAL_AD_TL, 0);
   }
      
}

/***********************************************************************/
/*              Acquisition APIs                                       */
/***********************************************************************/
//-------------------------------------------------------------------------
/** DTD_Connect
 *  Start acquisition to specified frequency and modulation type
 *  @param  psDemodCtx  Pointer of tuner device driver context.
 *  @param  u4Freq      RF center frequency in kHz
 *  @param  u1Modulation    Modulation type: MOD_ATSC_8VSB, MOD_J83B_64QAM & MOD_J83B_256QAM & MOD_J83B_AUTO_QAM
 *  @param  u1ConnType      Connection type: DEMOD_CONN_TYPE_SCAN & DEMOD_CONN_TYPE_SURF
 *  @retval TRUE        Success
 *  @retval FALSE       Hardware error
 */
//-------------------------------------------------------------------------
BOOL DTD_Connect(DEMOD_CTX_T *psDemodCtx, UINT32 u4Freq, UINT8 u1Modulation,UINT8 u1ConnType)
{
    //#if fcINTEGRATE_HOST_CMD   // [
//INT16   i2Err;
UINT32  u4SymbolRate;
PARAM_SETFREQ_T param;
#if fcADD_ACQ_SEMA
    mcSEMA_LOCK(psDemodCtx->hDemodAcq);                 // Avoid re-entry
#endif

#if fcADD_AUTO_QAM
    psDemodCtx->fgAutoQam = FALSE;
#endif

    if (u1Modulation == MOD_ATSC_8VSB)
    {
        mcSHOW_DBG_MSG(("ATSC_8VSB\n"));
        u4SymbolRate = 10762;
     }
    else if (u1Modulation == MOD_J83B_64QAM)
    {
        mcSHOW_DBG_MSG(("QAM 64\n"));
        u4SymbolRate = 5057;
    }
#if fcADD_AUTO_QAM
    else if (u1Modulation == MOD_J83B_AUTO_QAM)
    {
        mcSHOW_DBG_MSG(("Auto QAM\n"));
        psDemodCtx->fgAutoQam = TRUE;
        u4SymbolRate = 5361;  // Q256 Symbol rate
        u1Modulation = MOD_J83B_256QAM;
    }
#endif
    else
    {
        mcSHOW_DBG_MSG(("QAM 256\n"));
        u4SymbolRate = 5361;
        u1Modulation = MOD_J83B_256QAM;
    }

#if fcTUNE_NOT_START
    if (psDemodCtx->u1TunerAcqType != ACQ_DEMOD_ONLY)    
#endif
        psDemodCtx->Frequency  = u4Freq;
    psDemodCtx->SymbolRate = u4SymbolRate;
    psDemodCtx->Modulation = u1Modulation;
    psDemodCtx->ConnectionType = u1ConnType;

#if !defined(CC_WINDOWS_PLATFORM)
    if (psDemodCtx->u1TunerAcqType >= ACQ_DEMOD_ONLY)
    {
        // Set analog register
        if (i4ConnectInit(psDemodCtx) != OSR_OK)
            return FALSE;
    }
#endif

#ifdef fcSMART_ANT_DRV
        vWriteSmartAnt(psDemodCtx, DIR_000_0, GAIN_2, POL_NORMAL);
#endif

#if fcMEASURE_DRIVER_TIME == 2
     mcESTIMATE_TIME(("Parameter Setting"));
#endif

#if fcTUNE_NOT_START
    if (psDemodCtx->u1TunerAcqType != ACQ_DEMOD_ONLY)   // Do not SetFreq when do demod-only acq
#endif
    {
        param.Freq=u4Freq;
        param.Modulation=u1Modulation;
        param.fgAutoSearch=(DEMOD_CONN_TYPE_SCAN==u1ConnType)?1:0;
        DTD_Tune(psDemodCtx, &param);
    }

#if fcMEASURE_DRIVER_TIME == 2
     mcESTIMATE_TIME(("Set TunerFreq"));
#endif

     	
#if fcTUNE_NOT_START
    if (psDemodCtx->u1TunerAcqType >= ACQ_DEMOD_ONLY)
#endif
    {
        if (u1Modulation < MOD_TYPE_END)
        {
            vStartDebug(psDemodCtx);
            vDriverReset(psDemodCtx);           
            DTD_StartDemod(psDemodCtx, u1Modulation);
            vDriverInit(psDemodCtx);
        }
    }

#if fcMEASURE_DRIVER_TIME == 2
    mcESTIMATE_TIME(("Set Demod"));
#endif

#if fcADD_ACQ_SEMA
    mcSEMA_UNLOCK(psDemodCtx->hDemodAcq);
#endif
#if fcTUNE_NOT_START
    if (psDemodCtx->u1TunerAcqType == TUNE_FREQ_ONLY)
        return FALSE;
#endif
    return TRUE;

    //#endif  // ]
}

/***********************************************************************/
//-------------------------------------------------------------------------
/** DTD_Tune
 *  Tune to specified frequency
 *  @param  psDemodCtx  Pointer of tuner device driver context.
 *  @param  u4Freq      RF center frequency in kHz
 *  @retval TRUE        Success
 *  @retval FALSE       Hardware error
 */
//-------------------------------------------------------------------------
BOOL DTD_Tune(DEMOD_CTX_T *psDemodCtx, PARAM_SETFREQ_T * param)
{
INT16   i2Err;
    if ((i2Err = DTD_SetFreq(psDemodCtx, param)) != cRET_SETFREQ_OK)
    {
#if fcTRY_SET_FREQ_AGAIN
       if (i2Err == cRET_SETFREQ_PLL_LOCK)
       {                                   // Try again
            i2Err = DTD_SetFreq(psDemodCtx, param);
       }
#endif
       if (i2Err != cRET_SETFREQ_OK)
       {
        //    mcSHOW_HW_MSG(("DTD_SetFreq fail!\n"));
        #if fcADD_ACQ_SEMA
           mcSEMA_UNLOCK(psDemodCtx->hDemodAcq);
        #endif
            return FALSE;
       }
    }
    return TRUE;
}

/***********************************************************************/
UCHAR DTD_CheckTimeout(DEMOD_CTX_T *psDemodCtx)
{
UINT8 u1Count = 3;

    if (((psDemodCtx->ucSigCnt) >> 4) >= u1Count)
    {
        mcSHOW_DBG_MSG(("DTD_CheckTimeout(%d)", ((psDemodCtx->ucSigCnt) >> 4) ));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/***********************************************************************/
VOID DTD_Connected(DEMOD_CTX_T *psDemodCtx)
{
    psDemodCtx->ucConnFlag = 1;

    psDemodCtx->u2FwAcqTime = DTD_GetFWAcqTime(psDemodCtx);
    mcSHOW_DBG_MSG((" FwAcqTime = %d ms\n", psDemodCtx->u2FwAcqTime ));
  
    vStopDebug(psDemodCtx);
}

/***********************************************************************/
VOID DTD_AcqEnd(DEMOD_CTX_T *psDemodCtx, BOOL fgBreak)
{
    vEndDebug(psDemodCtx, fgBreak);
}

//-------------------------------------------------------------------------
/** DTD_DisConnect
 *  Stop acquisition and put Demod into sleep mode. This API should be invoked when the middleware disconnects the channel.
 *  @param  psDemodCtx  Pointer of demod device driver context maintained by demod driver.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_DisConnect(DEMOD_CTX_T *psDemodCtx)
{
    if (psDemodCtx->Modulation < MOD_TYPE_END)  // FAT
    {
        psDemodCtx->u1PecFlag = 0; // Stop PEC
        DTD_Acquire(psDemodCtx, FALSE);
        DTD_Sleep(psDemodCtx);
#if !defined(CC_WINDOWS_PLATFORM)
            i4ADInputFinish(psDemodCtx);
            i4ClockFinish(psDemodCtx);
#endif
        vDiscDebug(psDemodCtx);
        mcSHOW_DBG_MSG(("DTD_TunerDisc is invoked\n"));
    }
}

//-------------------------------------------------------------------------
/** DTD_SetSpcPolarity
 *  Set spectrum polarity.
 *  @param  psDemodCtx    Pointer of demod device driver context.
 *  @param  fgDebug       1 (Manual. For debug) / 0 (Auto. Normal flow use).
 *  @param  u1SpecInv    1 (Inverse) / 0 (Normal)
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_SetSpcPolarity(DEMOD_CTX_T *psDemodCtx, BOOL fgDebug, UINT8 u1SpecInv)
{
    mcSHOW_API_MSG(("DTD_SetSpcPolarity\n, fgDebug=%d, u1SpecInv=%d", fgDebug, u1SpecInv));

    
    if (fgDebug)
    {
        vSetFwVarBit(psDemodCtx, eVAR_FW_CFG_POOL, cpFW_CFG_POOL_fgSpecInvFix);
        if (u1SpecInv) //Inverse
        { 		   
            vSetFwVar(psDemodCtx, eVAR_FGDEFISYSSPECQAM, 0x01);
            vSetFwVar(psDemodCtx, eVAR_FGDEFISYSSPECVSB, 0x00);
        }
        else // Normal
        {
            vSetFwVar(psDemodCtx, eVAR_FGDEFISYSSPECQAM, 0x00);
            vSetFwVar(psDemodCtx, eVAR_FGDEFISYSSPECVSB, 0x01);
        }	
    }   
    else
    {
        vClrFwVarBit(psDemodCtx, eVAR_FW_CFG_POOL, cpFW_CFG_POOL_fgSpecInvFix);
        vSetFwVar(psDemodCtx, eVAR_FGDEFISYSSPECVSB, 0x01);
        vSetFwVar(psDemodCtx, eVAR_FGDEFISYSSPECQAM, 0x00);
    }
}

//-------------------------------------------------------------------------
/** DTD_GetTcmBER
 *  Set spectrum polarity.
 *  @param  psDemodCtx         Pointer of demod device driver context.
 *  @param  pu4TotalPkt        Received total packets.
 *  @param  pu4ErrSym          Received error symbols. 
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_GetTcmBER(DEMOD_CTX_T *psDemodCtx, UINT32 *pu4TotalPkt, UINT32 *pu4ErrSym)
{
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
UINT32  u4Total, u4Err;
UCHAR   u1Rs_Err_1;

    mcSHOW_API_MSG(("DTD_GetTcmBER\n"));
    
    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO3;
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO3_SIZE, 1))
    {
        mcSHOW_DBG_MSG2(("BER0_%02X BER1_%02X, BER2_%02X\n", 
            mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER0),
            mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER1), mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER2)));

    	// Calculate TCM total packets
    	u4Total = (UINT32) mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER0) & 0x07; // bit2-0
    	u4Total = u4Total * 256 + 255;
    	if (psDemodCtx->Modulation == MOD_ATSC_8VSB) // Transform packet number to symbol number
    	{
    	    u4Total = u4Total * 8* 207;	
    	}
    	else
    	{
    	    u4Total = u4Total * 7* 128;	
    	}
    				
    	// Calculate RS_Err symbols    	
        u4Err = mcUNSIGN_2BYTE(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER1), 
                                 mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER2), 5, 8); 	
        u1Rs_Err_1 = mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_BER1);
        
        // If Rs_Err is not ready
        if (mcTEST_BIT(u1Rs_Err_1, 7))
        {
       	    *pu4TotalPkt = u4Total;        
            *pu4ErrSym = u4Err;
        }
        else
        {
            *pu4TotalPkt = 0;
            *pu4ErrSym = 0;
        }
    }    
     
}

//-------------------------------------------------------------------------
/** DTD_GetIfAgcVol
 *  Get IF AGC Voltage in mV.
 *  @param  psDemodCtx    Pointer of demod device driver context.
 *  @param  pu2IfAgcVol   IF AGC voltage in mV.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx, UINT16 *pu2IfAgcVol)
{
BOOL    fgIsCFO;
INT32   ai4Data[cAGC_INFO_SIZE];	

    mcSHOW_API_MSG(("DTD_GetIfAgcVol\n"));
    DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO);
    *pu2IfAgcVol = ai4Data[cAGC_IF_VOL];  
    mcSHOW_DBG_MSG(("IfAgcVol = %d mV\n", *pu2IfAgcVol));	     
}


//-------------------------------------------------------------------------
/** DTD_SetTSDirectOut
 *  Set TS Direct on.
 *  @param  fgOut       TRUE: TS Out with Serial format / FALSE: Normal mode.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_SetTSDirectOut(DEMOD_CTX_T *psDemodCtx, BOOL fgOut)
{
#if fcDEMOD_GEN == cMT5391_DTD
UINT8   u1MpegFmt;
UINT32  u4RegValue;

    mcSHOW_API_MSG(("DTD_SetTSDirectOut\n"));

    i4ClockInit();
    DTD_Wakeup(psDemodCtx);
    
    if (fgOut == TRUE) 
    {        
        DTD_GetTsFmt(psDemodCtx, &u1MpegFmt);      
        mcCLR_BIT(u1MpegFmt, cBIT_TS_FMT_PARALLEL);  // Set TS format to be serial mode
        // 2008.07.18 changed by customer request. 
        mcCLR_BIT(u1MpegFmt, cBIT_TS_FMT_CLK_CONT);  // Set TS format to be gated-clk mode (clk suppressed when VAL is low)
        DTD_SetTsFmt(psDemodCtx, u1MpegFmt);
                 
        // Set PINMUX
        BSP_PinSet(PINMUX_GPIO_06, PINMUX_FUNCTION2);
        BSP_PinSet(PINMUX_GPIO_23, PINMUX_FUNCTION2);
        
        // Set ci or ts clk out to be Tsif_out
        mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TSOUT_CK_CFG, u4RegValue);
        mcSET_BIT(u4RegValue, cpCIPLUS_TSO_CK_SEL);
        mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TSOUT_CK_CFG, u4RegValue);        
        
        // Set Current = 6mA
        i4SetTSPadDriving(6);       
    }
    else 
    {
        // Set TS Format to be parallel & continuous mode (Default)
        DTD_GetTsFmt(psDemodCtx, &u1MpegFmt);
        mcSET_BIT(u1MpegFmt, cBIT_TS_FMT_PARALLEL);
        mcSET_BIT(u1MpegFmt, cBIT_TS_FMT_CLK_CONT);  
        DTD_SetTsFmt(psDemodCtx, u1MpegFmt);

        // Set PINMUX to GPIO function
        BSP_PinSet(PINMUX_GPIO_06, PINMUX_FUNCTION0);
        BSP_PinSet(PINMUX_GPIO_23, PINMUX_FUNCTION0);

        // Set ci or ts clk out to be Tsif_out
        mcGET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TSOUT_CK_CFG, u4RegValue);
        mcCLR_BIT(u4RegValue, cpCIPLUS_TSO_CK_SEL);
        mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_TSOUT_CK_CFG, u4RegValue);        

        // Set Current = 2mA
        i4SetTSPadDriving(2); 
    }
#endif
    
}

VOID DTD_SetTsMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel)
{
UINT8   u1MpegFmt;

    if (fgParallel == TRUE) 
    {
       // Set TS Format to be parallel mode
       DTD_GetTsFmt(psDemodCtx, &u1MpegFmt);          
       mcSET_BIT(u1MpegFmt, cBIT_TS_FMT_PARALLEL);    
       DTD_SetTsFmt(psDemodCtx, u1MpegFmt);
       mcSHOW_USER_MSG(("Set TS format to PARALLEL\n"));
    }
    else
    {     
       // Set TS Format to be serial mode    
       DTD_GetTsFmt(psDemodCtx, &u1MpegFmt);          
       mcCLR_BIT(u1MpegFmt, cBIT_TS_FMT_PARALLEL);    
       DTD_SetTsFmt(psDemodCtx, u1MpegFmt);           	
       mcSHOW_USER_MSG(("Set TS format to SERIEL mode\n"));
    }
}

//-------------------------------------------------------------------------
/**DTD_Get_RevPkt
 *  Get received total packet and error packets.
 *  @param   psDemodCtx       Pointer of demod device driver context.
 *  @param   pu4TotalPkt      Received total packets.
 *  @param   pu4ErrPkt        Received error packets.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_Get_RevPkt(DEMOD_CTX_T *psDemodCtx, UINT32 *pu4TotalPkt, UINT32 *pu4ErrPkt)
{
      //vDTDGetPERInfo(psDemodCtx);	 
      *pu4TotalPkt = psDemodCtx->u2PktCnt;
      if (psDemodCtx->u2PktErr != 0xFFFF)
      {
          // Update when PktCnt is ready
          psDemodCtx->u2PrePktErr = psDemodCtx->u2PktErr;
          *pu4ErrPkt = (UINT32)psDemodCtx->u2PktErr;
      }
      else
      {
          //*pu4ErrPkt = (UINT32)psDemodCtx->u2PrePktErr;
          *pu4ErrPkt = 0;
      }
	  mcSHOW_DBG_MSG2(("pu4TotalPkt = %d, pu4ErrPkt = %d\n", *pu4TotalPkt, *pu4ErrPkt));  
}

/************************************************************************/
VOID vGetDemodSetting(DEMOD_CTX_T *psDemodCtx, UINT8 u1RegAddr, UINT8 *ucValue)
{
UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_VAR;
    mcSET_QUEUE2(aucData, cOFST_REG_NUM) = 1;
    mcSET_QUEUE2(aucData, cOFST_REG1_ADDR_L) = mcLOW_BYTE(u1RegAddr);
    mcSET_QUEUE2(aucData, cOFST_REG1_ADDR_H) = 0;
#if fcSHOW_HOST_CMD
    fgShowHostCmd = TRUE;
#endif
    if (mcHOST_CMD(psDemodCtx, aucData, cOFST_REG1_SIZE, cOFST_REG1_SIZE + 1))
    {
        *ucValue = mcGET_QUEUE(aucData, cOFST_REG1_DATA);
    #if fcSHOW_HOST_CMD
        mcSHOW_DBG_MSG1((" VarR%X=%X ", u1RegAddr, *ucValue));
    #endif
    }
#if fcSHOW_HOST_CMD
    fgShowHostCmd = FALSE;
#endif
}

/***********************************************************************/
VOID vSetDemodSetting(DEMOD_CTX_T *psDemodCtx, UINT8 u1RegAddr, UINT8 ucValue)
{
#if fcSHOW_HOST_CMD
UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
UINT8   ucTmp;
#endif

    psDemodCtx->u1QSpecInv = ucValue;

#if fcSHOW_HOST_CMD
    mcSHOW_DBG_MSG1((" VarW%X=%X ", u1RegAddr, ucValue));
    fgShowHostCmd = TRUE;
#endif
//    mcHOST_CMD(psDemodCtx, aucData, 0, cOFST_REG1_SIZE + 1);
#if fcSHOW_HOST_CMD

    vGetDemodSetting(psDemodCtx, eVAR_FGDEFISYSSPECQAM, &ucTmp);
    vGetDemodSetting(psDemodCtx, eVAR_UCTUNERTYPE, &ucTmp);

    #if fcADD_HOSTCMD_CHKSUM
    vGetDemodSetting(psDemodCtx, eVAR_UCCMDCHKSUMERR, &ucTmp);
    if (ucTmp)
        vGetDemodSetting(psDemodCtx, eVAR_UCCMDCHKSUMID, &ucTmp);
    #endif
    fgShowHostCmd = TRUE;
#endif

#if fcSHOW_HOST_CMD
    fgShowHostCmd = FALSE;
#endif
}


/***********************************************************************/
/*              Public Functions (not STATIC)                          */
/***********************************************************************/
VOID vRestoreCfg(DEMOD_CTX_T *psDemodCtx)
{
UINT16 u2Chan;
UINT8   ucCfgIdx;

    if (psDemodCtx->Modulation != MOD_ATSC_8VSB)
    {
        u2Chan = u2ConvFreq2Chan(psDemodCtx);
        // Idx overflow check
        if (u2Chan > cMAX_CHAN_TABLE_NUM)
            u2Chan = cMAX_CHAN_TABLE_NUM;
    
        ucCfgIdx = ucCfgIdxMap[u2Chan];

        // Idx overflow check
        if (ucCfgIdx >= cDEMOD_MAX_CHAN_CFG)
        {
            ucCfgIdx = cDEMOD_MAX_CHAN_CFG - 1;
            mcSHOW_DBG_MSG1((" ===ucCfgIdx Overflow ===\n", ucCfgIdx));
        }
                
        if (ucCfgIdx != (UINT8) -1)
        {
            vSetDemodSetting(psDemodCtx, eVAR_FGDEFISYSSPECQAM, sDemodCfg[ucCfgIdx].fgSpecInv);
             mcSHOW_DBG_MSG((" ===Restore %d.Cfg=%d(%d/%d)===\n", u2Chan, sDemodCfg[ucCfgIdx].fgSpecInv, ucCfgIdx, ucCfgNum));
        }
    }
}

/***********************************************************************/
VOID vInitDebug(DEMOD_CTX_T *psDemodCtx)
{
UINT8   ii;

    ucCfgNum = 0;
    for (ii = 0; ii < cMAX_CHAN_TABLE_NUM; ii ++)
        ucCfgIdxMap[ii] = (UINT8) -1;
}

/***********************************************************************/
VOID vCloseDebug(DEMOD_CTX_T *psDemodCtx)
{
}

/***********************************************************************/
/*              Acquisition APIs                                       */
/***********************************************************************/
VOID vStartDebug(DEMOD_CTX_T *psDemodCtx)
{
#if fcMEASURE_DRIVER_TIME == 1
    mcGET_SYS_TIME(u4TickStartMeasure);
#endif
}

/***********************************************************************/
VOID vDiscDebug(DEMOD_CTX_T *psDemodCtx)
{
}

/***********************************************************************/
VOID vShowDebug(DEMOD_CTX_T *psDemodCtx)
{
UINT8   aucValue[5];
UINT8   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
CHAR    cSpec;
STATIC  CHAR    cPrevSpec = 0, cPrevSamp = 0;
CHAR    cSamp;
BOOL    fgIsCFO;
INT32   ai4Data[cAGC_INFO_SIZE];
char    szCFO[cCFO_STR_SIZE];

    mcSHOW_API_MSG(("vShowDebug\n"));

    if (mcCHECK_DBG_MSG)
    {
         mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO3;
        if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO3_SIZE, 1))
        {
    /***************** Spectrum Inverse *********************/
            cSpec = 'N';
            if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SPECTRUM), 1))
                cSpec = 'I';
            if (cPrevSpec != cSpec)
                mcSHOW_DBG_MSG((" _Spec%c_ ", cSpec));
            cPrevSpec = cSpec;
            cSamp = '2';
            if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SPECTRUM), 4))
                cSamp = '6';
            if (cPrevSamp != cSamp)
                mcSHOW_DBG_MSG((" _Samp%c_ ", cSamp));
            cPrevSamp = cSamp;
        }
    }

    mcCTMR_DBG_MSG2((" <S%02X / %02X / N%d> ",
            psDemodCtx->ucLockStatus, psDemodCtx->ucLockState,
            psDemodCtx->ucSigCnt >> 4 ));
    PFXMSG_OFF(); // Please keep this after the first message string


    if (mcCHECK_DBG_MSG2)
    {
        mcSET_CMD2(aucData) = cCMD_ID_DEMOD_GET_INFO3;
        if (mcHOST_CMD(psDemodCtx, aucData, cOFST_DEMOD_INFO3_SIZE, 1))
        {
    /***************** Spectrum Inverse *********************/
            cSpec = 'N';
            if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SPECTRUM), 1))
                cSpec = 'I';
        //    cSamp = '2';
        //    if (mcTEST_BIT(mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SPECTRUM), 4))
        //        cSamp = '6';
        //    mcSHOW_DBG_MSG2(("_Sp%c%c", cSpec, cSamp));
            mcSHOW_DBG_MSG2(("%c", cSpec));
            mcSHOW_DBG_MSG2((" SER_%02X", mcGET_QUEUE(aucData, cOFST_DEMOD_INFO_SER0)));
        }
         DTD_Get_AGCGain_CFO(psDemodCtx, ai4Data, &fgIsCFO);
    		/***************** AGC Gain *********************/
        mcSHOW_DBG_MSG2((" AGC_%2"cFMT_IL"R%2"cFMT_IL"I IF_%"cFMT_IL"mV SNR%ddB",
                ai4Data[cAGC_RF_CTRL], ai4Data[cAGC_IF_CTRL], ai4Data[cAGC_IF_VOL], psDemodCtx->u1SNR));
    		/***************** Carrier Offset *********************/
        mcMEMSET(szCFO, 0, cCFO_STR_SIZE);
        szGetCFOStr(ai4Data[cVCR_CFO], fgIsCFO, &szCFO[0]);
          mcSHOW_DBG_MSG2((" CFO_%s", szCFO));

        if (psDemodCtx->Modulation != MOD_ATSC_8VSB)
        {
            aucValue[0] = ucGetReg(psDemodCtx, DTD_REG_FTS_STATUS);
            aucValue[1] = ucGetReg(psDemodCtx, DTD_REG_QCR_TrkK);
            mcSHOW_DBG_MSG2((" FTS_%02X %02X", aucValue[0], aucValue[1]));
            if (u1GetRegs(psDemodCtx, DTD_REG_DEI_Cfg, aucValue, 2))
                mcSHOW_DBG_MSG2((" DEI_%02X %02X", aucValue[0], aucValue[1]));
            if (u1GetFwVars(psDemodCtx, eVAR_UCACCBK, aucValue, 2))
                mcSHOW_DBG_MSG2((" AccBk %2d %d", (UINT16) aucValue[0], (UINT16) aucValue[1]));
        }        
        else
        {
            // VCR
            if (u1GetRegs(psDemodCtx, DTD_REG_VCR_Pilot, aucValue, 5))
            {
                mcSHOW_DBG_MSG2((" Pilot_%02X CM_%5d RM_%d", 
                    aucValue[0], 
                    mcUNSIGN_2BYTE(aucValue[1], aucValue[2], 8, 8),                
                    mcUNSIGN_2BYTE(aucValue[3], aucValue[4], 8, 8)
                ));    
            }
        }
        
    }


#if fcMEASURE_DRIVER_TIME == 1
    mcGET_SYS_TIME(u4TickCurMeasure);
    mcGET_DIFF_TIME(u4TickDiffCnt0, u4TickStartMeasure, u4TickCurMeasure);
    mcGET_DIFF_TIME(u4TickDiffCnt1, u4PrevStartTime, u4TickCurMeasure);
    mcSHOW_DRVTIME_MSG((" (%4ums +%3u)", mcCONV_SYS_TIME(u4TickDiffCnt0),
                        mcCONV_SYS_TIME(u4TickDiffCnt1)));
    u4PrevStartTime = u4TickCurMeasure;
#endif
    mcCTMR_DBG_MSG2(("\n"));
    PFXMSG_DEF(); /* Please keep this after the final demod message */

}

/***********************************************************************/
VOID vStopDebug(DEMOD_CTX_T *psDemodCtx)
{
UINT16 u2Chan;
UINT8   ucValue, ucCfgIdx;
UINT8   eModType;

    eModType = psDemodCtx->Modulation;

    u2Chan = u2ConvFreq2Chan(psDemodCtx);
    // Idx overflow check
    if (u2Chan > cMAX_CHAN_TABLE_NUM)
        u2Chan = cMAX_CHAN_TABLE_NUM;
    
    if (psDemodCtx->Modulation != MOD_ATSC_8VSB)
    {
        if ((eModType < MOD_TYPE_END) && (ucCfgNum < cDEMOD_MAX_CHAN_CFG))
        {
            ucCfgIdx = ucCfgIdxMap[u2Chan];
                if (ucCfgIdx == (UINT8) -1)
                {
                    ucCfgIdx = ucCfgNum;
                    ucCfgIdxMap[u2Chan] = ucCfgNum;
                    ucCfgNum ++;
                }
                // Idx overflow check
                if (ucCfgIdx >= cDEMOD_MAX_CHAN_CFG)
                {
                    ucCfgIdx = cDEMOD_MAX_CHAN_CFG - 1;
                    mcSHOW_DBG_MSG1((" ===ucCfgIdx Overflow ===\n", ucCfgIdx));
                }
        #if fcINTERNAL_TEST
                mcSHOW_DBG_MSG1((" ===%d.Cfg:(%d/%d)===\n", u2Chan, ucCfgIdx, ucCfgNum));
        #endif
            vGetDemodSetting(psDemodCtx, eVAR_FGISYSSPECINV, &ucValue);
        #if fcSHOW_HOST_CMD
            if (ucValue > 1)                //JWEI 2006/04/13
            {
                mcSHOW_DBG_MSG1((" === _@Error Cfg: %d ===\n", ucValue));
                vGetDemodSetting(psDemodCtx, eVAR_FGISYSSPECINV, &ucValue);
            }
        #endif
            sDemodCfg[ucCfgIdx].fgSpecInv = ucValue;
        //    sCurCfg.fgSpecInv = ucValue;
            mcSHOW_DBG_MSG((" ===Save %d.Cfg:%d(%d/%d)===\n", u2Chan, ucValue, ucCfgIdx, ucCfgNum));
        }
    }

}

/***********************************************************************/
VOID vEndDebug(DEMOD_CTX_T *psDemodCtx, BOOL fgBreak)
{
}


/***********************************************************************/
VOID vEnableI2C_Mode(VOID)
{
	#if 0 // ####neglected 2008.3.18
// Config monitor pin
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX0, 0x00380000);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX1, 0xC004046C);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX2, 0x0333300A);
    mcSET_SHOW_REG(CKGEN_BASE, cCKGEN_REG_PMUX3, 0x0000F000);
// Switch Monitor Mux to DTD
//    mcRISC_Reg(PDWNC_BASE, cPDWNC_REG_PINMUX) = cPDWNC_CFG_MON_A;
    mcSET_SHOW_REG(BIM_BASE, cBIM_REG_MONSEL, 0x00070000);

    mcRISC_Reg(pu4DTD_Reg, DTD_REG_RISC_CTRL) = mcRISC_CTRL_MASK(cRISC_WORD_LEN, 0);
// Host_en to i2c
    mcSET_SHOW_REG(pu4DTD_Reg, DTD_REG_RISC_CFG, 0x05C87801);
// DTD test mode = 5
    mcSET_SHOW_REG(pu4DTD_Reg, DTD_REG_Chip_MPMD, 0xE5053001);

    mcSHOW_DBG_MSG(("Enable I2C Test Mode.\n"));
  #endif
}
/***********************************************************************/
#define EQBIST_SCRIPT_NUM   2
STATIC EQBIST_REG_SCRIPT_T EQScript[EQBIST_SCRIPT_NUM] = 
{
   {  /* begin of script 0 */
       20,
       {0x1E8, 0x1E4, 0x1E4, 0x1E5, 0x1E5, 0x1E6, 0x1E6, 0x1E7, 0x1E7,
        0x1E1, 0x1E1, 0x1E2, 0x1E2, 0x1E3, 0x1E3, 
        0x229, 0x229, 0x2EB, 0x2EC, 0x2E1
       },
       {0x000, 0x000, 0x0FF, 0x000, 0x0FF, 0x000, 0x0FF, 0x000, 0x0FF,
        0x000, 0x0FF, 0x000, 0x0FF, 0x000, 0x0FF,
        0x003, 0x004, 0x026, 0x0B6, 0x0CF
       },
       4,
       {0x2E0, 0x2ED, 0x2EE, 0x2EF},
       {0x003, 0x000, 0x026, 0x0B6}
   }, /* end of script 0 */

   {  /* begin of script 1 */
       21,
       {0x1E8, 0x1E4, 0x1E4, 0x1E5, 0x1E5, 0x1E6, 0x1E6, 0x1E7, 0x1E7,
        0x1E1, 0x1E1, 0x1E2, 0x1E2, 0x1E3, 0x1E3, 
        0x229, 0x229, 0x2E9, 0x2EB, 0x2EC, 0x2E1
       },
       {0x000, 0x000, 0x0FF, 0x000, 0x0FF, 0x000, 0x0FF, 0x000, 0x0FF,
        0x000, 0x0FF, 0x000, 0x0FF, 0x000, 0x0FF,
        0x003, 0x004, 0x011, 0x026, 0x07F, 0x0CF
       },
       4,
       {0x2E0, 0x2ED, 0x2EE, 0x2EF},
       {0x003, 0x000, 0x026, 0x07F}
   }, /* end of script 1 */
  
};


//-------------------------------------------------------------------------
/** DTD_SetEQBist_RegScript
 *  Set register according to EQ BIST register script
 *  @param  psDemodCtx    Pointer of tuner device driver context.
 *  @param  u1Opt         The script option      
 */
//-------------------------------------------------------------------------
VOID DTD_SetEQBist_RegScript(DEMOD_CTX_T *psDemodCtx, UINT8 u1Opt)
{    
UINT8 ii, u1RegValue, u1RegNum;
UINT16 u2RegAddr;
      
    u1RegNum = EQScript[u1Opt].u1SetRegNum;

    // Input check
    if (u1RegNum > EQBIST_MAX_SET_REG_NUM)
    	u1RegNum = EQBIST_MAX_SET_REG_NUM;

    if (u1Opt >= EQBIST_SCRIPT_NUM)
    {	
        mcSHOW_USER_MSG(("Invalide script option%d \n", u1Opt));
        return;
    }

    mcSHOW_DBG_MSG(("Set script%d, %d\n", u1Opt, u1RegNum));

    // Set EQBist Register
    for (ii = 0; ii < u1RegNum; ii++)
    {
        u2RegAddr = EQScript[u1Opt].u2SetRegAddr[ii];
        u1RegValue = EQScript[u1Opt].u1SetRegValue[ii];
        mcSHOW_DBG_MSG(("%04X = %02X\n",u2RegAddr, u1RegValue));
        DTD_SetReg(psDemodCtx, u2RegAddr, &u1RegValue, 1);
    }
}

//-------------------------------------------------------------------------
/** DTD_CheckEQBistResult
 *  Read register and check if the result is pass or fail
 *  @param  psDemodCtx   Pointer of tuner device driver context.
 *  @param  u1Opt        The script option
 *  @retval TRUE         PASS
 *  @retval FALSE        FAIL 
 */
//-------------------------------------------------------------------------
BOOL DTD_CheckEQBistResult(DEMOD_CTX_T *psDemodCtx, UINT8 u1Opt)
{
BOOL ret = TRUE;
UINT8 ii, u1RegValue, u1RegNum;
UINT16 u2RegAddr;

    u1RegNum = EQScript[u1Opt].u1ChkRegNum;
   
    // Input check
    if (u1RegNum > EQBIST_MAX_CHK_REG_NUM)
    	u1RegNum = EQBIST_MAX_CHK_REG_NUM;

    if (u1Opt >= EQBIST_SCRIPT_NUM)
    {	
        mcSHOW_USER_MSG(("Invalide script option%d \n", u1Opt));
        ret = FALSE;
        return ret;
    }

    mcSHOW_DBG_MSG(("Check script%d, %d\n", u1Opt, u1RegNum));
    
    // Check result
    for (ii = 0; ii < u1RegNum; ii++)
    {
        u2RegAddr = EQScript[u1Opt].u2ChkRegAddr[ii];
        DTD_GetReg(psDemodCtx, u2RegAddr, &u1RegValue, 1);
        mcSHOW_DBG_MSG(("%04X = %02X\n",u2RegAddr, u1RegValue));
        if ((ret == TRUE) && (u1RegValue != EQScript[u1Opt].u1ChkRegValue[ii]))
        {
            ret = FALSE;
        }	
        	
    }
    mcSHOW_USER_MSG(("DTD_EQBIST: %d", ret));   
    
    return ret;
}
//-------------------------------------------------------------------------
/**DTD_Get_GSCfg
 *  Get the information of GS target channel configuration
 *  @param   psDemodCtx       Pointer of demod device driver context.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_Get_GSCfg(DEMOD_CTX_T *psDemodCtx)
{
UINT8   au1Data; 

    vDTDGetStatus(psDemodCtx);
    au1Data = ucGetFwVar(psDemodCtx, eVAR_UCGSCFG);
    mcSHOW_DBG_MSG(("eVAR_UCGSCFG=0x%02X\n", au1Data)); 	
    mcSHOW_DBG_MSG(("<State: %02X>GSchannel: FixR1_%d_FastCRC_%d_SlowCRC_%d_R1_%d_R21CH4_%d_R21CH1&2&3_%d\n", psDemodCtx->ucLockState,\
		(au1Data&0x80)>>7,(au1Data&0x1F)>>4,(au1Data&0x08)>>3,(au1Data&0x04)>>2,(au1Data&0x02)>>1,(au1Data&0x01)));
}
//-------------------------------------------------------------------------
/**DTD_Get_BrazilCfg
 *  Get the information of Brazil target channel detection
 *  @param   psDemodCtx       Pointer of demod device driver context.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_Get_BrazilCfg(DEMOD_CTX_T *psDemodCtx)
{
UINT8   au1Data; 

    vDTDGetStatus(psDemodCtx);
    au1Data = ucGetFwVar(psDemodCtx, eVAR_UCBRZCCFG);
    mcSHOW_DBG_MSG(("eVAR_UCBRZCCFG=0x%02X\n", au1Data)); 	
    mcSHOW_DBG_MSG(("<State: %02X>BRZCCFG: R22SET_%d_R22CH3_%d_R22CH2_%d_BRZ_%d\n", psDemodCtx->ucLockState,\
		(au1Data&0x80)>>4,(au1Data&0x1F)>>2,(au1Data&0x08)>>1,(au1Data&0x01)));
}
//-------------------------------------------------------------------------
/**DTD_ControlGS
 *  Enable or disable the control of EQ GS operation
 *  @param   psDemodCtx       Pointer of demod device driver context.
 *  @param   fgGSOperation     True:enable, False:disable.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_ControlGS(DEMOD_CTX_T *psDemodCtx, UINT8 fgGSOperation)
{
    if(fgGSOperation)
    {
        vSetFwVarBit(psDemodCtx, eVAR_FW_CFG_POOL, cpFW_CFG_POOL_fgGS_Enable);
	 mcSHOW_DBG_MSG(("Enable GS\n")); 	

    }
    else
    {
        vClrFwVarBit(psDemodCtx, eVAR_FW_CFG_POOL, cpFW_CFG_POOL_fgGS_Enable);		
        mcSHOW_DBG_MSG(("Disable GS\n", fgGSOperation)); 	
    }
}
//-------------------------------------------------------------------------
/**DTD_HwEqGSIndex
 *  Check HW-selected EQ GS index
 *  @param   psDemodCtx       Pointer of demod device driver context.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_HwEqGSIndex(DEMOD_CTX_T *psDemodCtx)
{
UINT16 u2GSIndex;
UINT32 u4GSCoef;
UINT8   au1Data[4], ii; 

    vSetReg(psDemodCtx, 0x212, 0x80);
	
    for(ii=0; ii<12; ii++)
    {
        u1GetRegs(psDemodCtx, 0x212, au1Data, 1);
	 mcSHOW_DBG_MSG2(("0x212=0x%02X\n", au1Data[0])); 
	 
        u1GetRegs(psDemodCtx, 0x213, au1Data, 4);
	 mcSHOW_DBG_MSG2(("0x213=0x%02X,  0x214=0x%02X, 0x215=0x%02X, 0x216=0x%02X\n", au1Data[0], au1Data[1], au1Data[2], au1Data[3]));                       					  
	 u2GSIndex = mcUNSIGN_2BYTE((au1Data[0]&0x0F), au1Data[1], 8, 8);
	 u4GSCoef = mcUNSIGN_2BYTE(au1Data[2], au1Data[3], 8, 8);                      					  
				  
	 if(((au1Data[0]&0xC0)>>6) == 0x01)
        {
            mcSHOW_DBG_MSG(("GStype_EQR_TableEntry%d_GSindex_0x%02X_GScoef_0x%04X \n", ii, u2GSIndex, u4GSCoef ));                       					  
	 }
	 else if (((au1Data[0]&0xC0)>>6) == 0x02)
	 {
	     mcSHOW_DBG_MSG(("GStype_EQL_TableEntry%d_GSindex_0x%02X_GScoef_0x%04X \n", ii, u2GSIndex, u4GSCoef ));
	 }
	 else if (((au1Data[0]&0xC0)>>6) == 0x03)
	 {
	    mcSHOW_DBG_MSG(("GStype_Both_TableEntry%d_GSindex_0x%02X_GScoef_0x%04X \n", ii, u2GSIndex, u4GSCoef ));
	 }
	 else
	 {
	    mcSHOW_USER_MSG(("Wrong GStype\n")); 	
	 }					  						 
 }
}
//-------------------------------------------------------------------------
/**DTD_FwEqGSIndex
 *  Check FW-selected EQ GS index
 *  @param   psDemodCtx       Pointer of demod device driver context.
 *  @retval void
 */
//-------------------------------------------------------------------------
VOID DTD_FwEqGSIndex(DEMOD_CTX_T *psDemodCtx)
{
UINT8   au1Data[20], ii;

    mcSHOW_DBG_MSG(("Check EQL_GS_Index:\n")); 

    u1GetMultiRegs(psDemodCtx, 0x200, au1Data, 16);
    for(ii=0; ii<8; ii++)
    {
	 mcSHOW_DBG_MSG(("0x%03X:0x%02X,\t0x%03X:0x%02X\n",0x200+ii, au1Data[ii], 0x200+ii+8, au1Data[ii+8]));               						
    }

    mcSHOW_DBG_MSG(("Check EQR_GS_Index:\n"));

    u1GetMultiRegs(psDemodCtx, 0x2C6, au1Data, 20);
    for(ii=0; ii<10; ii++)
    {
	 mcSHOW_DBG_MSG(("0x%03X:0x%02X,\t0x%03X:0x%02X\n",0x2C6+ii, au1Data[ii], 0x2C6+ii+10, au1Data[ii+10]));               						
    }

}	


