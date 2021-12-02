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
 * $Author: xiongfeng.wen $
 * $Date: 2012/12/12 $
 * $RCSfile: pi_tuner_teqe3.c,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// **** REMOVE after common/specific part done!!!  *** //

#include "atd_feature.h"
#include "pi_demod_atd.h"
#include "pi_def_atd.h"
//#include "pd_common.h"
#include "drvcust_if.h"
#include "drv_tvd.h"
#include "x_ckgen.h"
#include "drv_pwm.h"
#include "PD_Def_dbg.h"  /*need include RISC IF macro, if re-ware RISC IF , remove this*/
//New RISC IF
#include "ctrl_bus.h"

//For sound carrier shift and OVM
#include "aud_if.h"
#include "pi_demod_setsystem_reg.c"

// include IC specific PI code here!
#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
#include "pi_demod_atd_sawless.c"
#else 
#include "pi_demod_atd_legacy.c"
#endif
/***********************************************************************/
/*                  Defines                                            */
/***********************************************************************/
STATIC PARAM_RISC_T sATDRiscCtx;
volatile UINT32 *puATD_Reg = (volatile UINT32 *) cRISC_ATD_BASE;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

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
//static UINT8 fgAudCFO=0;
static UINT8 SmallAGC=0;

UINT8 u1NullRegion = 0x37;
UINT8 Weak_Sinal_Indicator=0, weak_count = 0;	
UINT8 Strong_Chroma_Indicator=0;
//static UINT8 CCITest=0;
UINT8 CCI_Indicator=0;
UINT8 NoOVMCounter0 = cNO_OVM_THRESHOLD;
UINT8 PNCounter=5;	
UINT8 u1CRPatchLoopCnt = 0;
UINT8 u1CrAutoConfig = 0;
UINT8 u1CrBandwidth = 6;

UINT8 CCI_Tone_BW3[4][3] = {{0x0F,0xFE,0x00},{0x0F,0xFD,0xFD},{0x0F,0xFD,0xF9},{0x0F,0xFD,0xCF}};
UINT8 CCI_Tone_BW2[4][3] = {{0x0F,0xFE,0x00},{0x0F,0xFB,0xFD},{0x0F,0xFB,0xF9},{0x0F,0xFB,0xCF}};
extern UINT16 g_CR_BW_AutoConfig_Delay_cnt;


/***********************************************************************/
#define fgCRMOdify 1//d110510_Haibo:Enable new CR patch for MT5368 IC verification
#define cCR_FIFO_DELAY_LENGTH_Ext 410
#define cCR_FIFO_DELAY_LENGTH_Org 192

//#if (defined CC_MT5389)
#if (defined(CC_MT5389) || defined(CC_MT5396))  // Bypass the OVM Mode for MT5396
#define fgOVMBypass 1
#else
#define fgOVMBypass 0
#endif

#define cNO_OVM_CNT_THRHLD            5
#define cNO_PN_CNT_THRHLD             5
#define cCR_PATCH_NOISE_THRHLD_HIGH    0x19
#define cCR_PATCH_NOISE_THRHLD_LOW     0x15
#define cCR_PATCH_CMTRX_PN_THRHLD      0x20
#define cDRO_CVBS_UP_BUD_0_Hihg        0x3F
#define cDRO_CVBS_UP_BUD_0_Low        0x28
#if fgCRMOdify
static UINT8 u1CrPatchNoPnCntr = cNO_PN_CNT_THRHLD;
static UINT8 u1CrPatchNoOvmCntr = cNO_OVM_CNT_THRHLD;
UINT8 ucCrPatchState = cCR_PATCH_STATE_NORM_PN_FN;
#endif
ATD_CTX_T *ATD_DemodCtxCreate(void)
{
    ATD_CTX_T *p = NULL;

	CREATE_OBJECT(p, ATD_CTX_T);

    // error handling
    if (!p)
       return NULL;

   	// zero memory
	x_memset(p, 0, sizeof(ATD_CTX_T));

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
#if ATD_SUPPORT_BATCH_RW//d110623:ATD support batch write
    if (ICtrlBus_RiscDemWrite(&sATDRiscCtx, (u2RegAddr+ATD_REG_OFFSET), pu1Buffer, u2ByteCount))
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Write %d byte Error: Reg[%03X] = 0x%02X!\n",
                       u2ByteCount, u2RegAddr, *pu1Buffer));

		return 1;
    }
#else
    UINT16 u2ByteCntIdx=0;
		mcSHOW_DBG_MSG4(("Non-batch write\n"));
	for(;u2ByteCntIdx<u2ByteCount;u2ByteCntIdx++)
	{
	   if (ICtrlBus_RiscDemWrite(&sATDRiscCtx, (u2RegAddr+ATD_REG_OFFSET+u2ByteCntIdx), pu1Buffer+u2ByteCntIdx,1))
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Write  byte Error: Reg[%03X] = 0x%02X!\n",u2RegAddr+ATD_REG_OFFSET+u2ByteCntIdx, *(pu1Buffer+u2ByteCntIdx)));
		return 1;
    }
	}
#endif
	return 0;
}

/************************************************************************/
/* Read consecutive registers */
UINT8 ATD_GetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
#if ATD_SUPPORT_BATCH_RW//d110623:ATD support batch write
    if (ICtrlBus_RiscDemRead(&sATDRiscCtx, (u2RegAddr+ATD_REG_OFFSET), pu1Buffer, u2ByteCount))
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Read %d byte Error: Reg[%03X] = 0x%02X!\n",
                       u2ByteCount, u2RegAddr, *pu1Buffer));

        return 1;
    }
#else
    UINT16 u2ByteCntIdx=0;
	mcSHOW_DBG_MSG4(("Non-batch read\n"));
	
	for(;u2ByteCntIdx<u2ByteCount;u2ByteCntIdx++)
	{
    if (ICtrlBus_RiscDemRead(&sATDRiscCtx, (u2RegAddr+ATD_REG_OFFSET+u2ByteCntIdx), pu1Buffer+u2ByteCntIdx,1))
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Read  Error: Reg[%03X] = 0x%02X!\n",u2RegAddr+ATD_REG_OFFSET+u2ByteCntIdx, *(pu1Buffer+u2ByteCntIdx)));
        return 1;
    }
    }
#endif
    return 0;
}



/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/

UINT8 Demod_SetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    if (ICtrlBus_RiscDemWrite(&sATDRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Write %d byte Error: Reg[%03X] = 0x%02X!\n",
                       u2ByteCount, u2RegAddr, *pu1Buffer));
	    return 1;
    }
        return 0;
}

/************************************************************************/
/* Read consecutive registers */
UINT8 Demod_GetReg(ATD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    if (ICtrlBus_RiscDemRead(&sATDRiscCtx, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_HW_MSG((" _@ICtrlBus Read %d byte Error: Reg[%03X] = 0x%02X!\n",
                       u2ByteCount, u2RegAddr, *pu1Buffer));
            return 1;
    }
        return 0;
}




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


VOID ATD_SetAgc(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    ATD_AGC_CTX* psAtdAgcPara = &(psDemodCtx->pTCtx->sAtdAgcPara);
    UINT8 u1Ifmin=0;
    // AGC
    RegWrite8(REG_ATD_PRA_RF_BIAS, psAtdAgcPara->AgcRfBias);	//RF BIAS
    RegWrite8(REG_ATD_PRA_IF_BIAS, 0x00);	                    //IF BIAS
    RegWrite8(REG_ATD_PRA_RF_SLOPE, 0x80);	                //RF SLOPE
    RegWrite8(REG_ATD_PRA_IF_SLOPE, 0x80);	                //IF SLOPE
    RegWrite8(REG_ATD_PRA_RF_MAX, 0x7F);                      //RF MAX
    RegWrite8(REG_ATD_PRA_IF_MAX, 0x7F);                      //IF MAX
    RegWrite8(REG_ATD_PRA_RF_MIN, psAtdAgcPara->AgcRfMin);	//RF MIN

    //    depart  the tuner-dependent part to [ITUNER] layer , Jack Mei 2009/11/3    
    ucData = psDemodCtx->u1SubSysIdx;
    if (ITuner_OP(psDemodCtx->pTCtx, itSetIfMin,ucData, &u1Ifmin) == ITUNER_OK)
    {
        RegWrite8(REG_ATD_PRA_IF_MIN, u1Ifmin);
    }
    else
    {
        RegWrite8(REG_ATD_PRA_IF_MIN, psAtdAgcPara->AgcIfMin);	//for other tuner IF MIN
        u1Ifmin=psAtdAgcPara->AgcIfMin;
    }
   
/*
	#ifndef CC_MT5363//For MT5388
    if(psDemodCtx->u1TunerType==cPANASONIC_ENV57S){ //only for MT5388+Panasonic tuner
        mcSHOW_DBG_MSG(("ENV57S TOP Setting\n"));
    	switch(psDemodCtx->u1SubSysIdx){
    	    case MOD_PAL_BG:
    	    case MOD_PAL_I:
    	        RegWrite8(REG_ATD_PRA_IF_MIN, 0xE0);
    	        u1Ifmin=0xE0;
    	        break;
    	    case MOD_SECAM_L:
            case MOD_SECAM_L1:
            case MOD_PAL_DK:
                u1Ifmin=0xF0;
                RegWrite8(REG_ATD_PRA_IF_MIN, 0xF0);
    		break;
    	    default:
    	        mcSHOW_DBG_MSG(("TOP No change\n"));
    	        break;
        }
    }
    else  //for other tuner of MT5388
    {
        RegWrite8(REG_ATD_PRA_IF_MIN, psAtdAgcPara->AgcIfMin);	//IF MIN
        u1Ifmin=psAtdAgcPara->AgcIfMin;
    }
    #else
    if(psDemodCtx->u1TunerType==cNuTune_FT2607){ 
            mcSHOW_DBG_MSG(("NuTune_FT2607 TOP Setting\n"));
            switch(psDemodCtx->u1SubSysIdx){
                case MOD_PAL_BG:
                case MOD_PAL_I:                    
                case MOD_PAL_DK:
                    RegWrite8(REG_ATD_PRA_IF_MIN, 0xE3);
                    u1Ifmin=0xE3;
                    break;
                case MOD_SECAM_L:
                case MOD_SECAM_L1:
                    u1Ifmin=0xF3;
                    RegWrite8(REG_ATD_PRA_IF_MIN, 0xF3);
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
                        RegWrite8(REG_ATD_PRA_IF_MIN, 0xDC);
                        u1Ifmin=0xDC;
                        break;
                    case MOD_SECAM_L:
                    case MOD_SECAM_L1:
                        u1Ifmin=0xE4;
                        RegWrite8(REG_ATD_PRA_IF_MIN, 0xE4);
                    break;
                    default:
                        mcSHOW_DBG_MSG(("TOP No change\n"));
                        break;
                }
            }        
    else 
    {    	
        RegWrite8(REG_ATD_PRA_IF_MIN, psAtdAgcPara->AgcIfMin);	//IF MIN
        u1Ifmin=psAtdAgcPara->AgcIfMin;
     }
    #endif

*/
    RegWrite8(REG_ATD_PRA_DR_SEL, psAtdAgcPara->AgcDrSel);	//PRA Dynamic Range
    //RegWrite8(REG_ATD_PRA_LP1_CFG13, 0x4D); //REG_ATD_PRA_LP1_CFG13	    
    RegWrite8(REG_ATD_PRA_LP1_CFG12, 0x83); //REG_ATD_PRA_LP1_CFG12, disable PRA auto-target
   //To fix CR DTV00138738
    if((psDemodCtx->u1SubSysIdx==MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx==MOD_SECAM_L1))
        RegWrite8(REG_ATD_PRA_LP1_CFG12, 0x43); //REG_ATD_PRA_LP1_CFG12, disable PRA auto-target	
  
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
    RegWrite8(REG_ATD_PRA_LP1_CFG05, ucData); //REG_ATD_PRA_LP1_CFG05                         

    psDemodCtx->s1IFmin = u1Ifmin;
    psDemodCtx->s1RFmin = psAtdAgcPara->AgcRfMin;
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
    ATD_GetReg(psDemodCtx, REG_ATD_PS_CFG, &ucData, 1);

    ucData = (ucData & 0xFC) | u1PF;

    ATD_SetReg(psDemodCtx, REG_ATD_PS_CFG, &ucData, 1);
    mcSHOW_DBG_MSG(("Pilot filter is 0x%x \n",ucData));
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
    //RegWrite8(REG_DEMOD_ACR_CFG,0xF9);     //EC_code_d100305: Reg0x197[b7-b6] set to 11 Turn-on CR auto Re-acq config
    ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
    ucData = ucData | 0xC0;
    ATD_SetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
    mcSHOW_DBG_MSG(("Enable CR auto Re-acq config for ATD scan mode, 0x997 = 0x%X\n", ucData)); 

    switch (psDemodCtx->u1SubSysIdx)
    {
    case MOD_SECAM_L:  // to speed up PRA 
    case MOD_SECAM_L1:
        mcSHOW_DBG_MSG(("Set scan mode parameter for SECAM L/L'\n"));
        
        //ZY, 100126, SECAM L scan loss issue. PRA speed-up at channel scan
        ucData = 0x84; ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG02, &ucData, 1); //0x42->0x84, fast loop speed
        ATD_GetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG07, &ucData, 1);
        ucData = ucData & 0xEF;   //REG_ATD_PRA_LP1_CFG07[4]=0, middle-term
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG07, &ucData, 1); 
        
        
	#if  0
        ucData = 0x19;  // REG_ATD_PRA_LP1_CFG03 = 0x19
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG03, &ucData, 1);
        ATD_GetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG12, &ucData, 1);
        ucData &= 0xF8;  // REG_ATD_PRA_LP1_CFG12[2:0] = 1
        ucData |= 0x01;  
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG12, &ucData, 1);
        ucData = 0x8D;  // REG_ATD_PRA_LP1_CFG13 = 0x8D
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_LP1_CFG13, &ucData, 1);
        ucData = 0x1E;  // REG_ATD_PRA_CFG_1 = 0x1E
        ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
       #endif
       //ZY 100126, remove small DRO gain setting
       //liuqu,20100108,for secaml L scan issue. 
       //ucData = 0x04;
	   // ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData, 1);
	   break;

    case MOD_PAL_BG:  // to enlarge audio filter in scan mode for TV spec detection
    case MOD_PAL_DK:
    case MOD_PAL_I:
        ucData = 0x8C;
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData, 1);
        ucData = 0x20;
        ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData, 1);
        ucData = 0x81;
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData, 1);
        ucData = 0xD0;
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData, 1);
        break;

    default:
        break;
    }
	if((psDemodCtx->u1SubSysIdx>=MOD_ANA_TYPE_BEGIN) &&(psDemodCtx->u1SubSysIdx<MOD_NTSC_M))//Only need reset AVS when sys_sub_index is PAL and SECAM
   {
   mcSHOW_DBG_MSG(("Reset AVS\n"));
    ATD_GetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData &= 0xFB;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
    ucData |= 0x04;
    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
   }
}


/**********************************************************************/
INT32 ATD_DemodReset(ATD_CTX_T *psDemodCtx)  // need further implementation
{

	return (DRVAPI_TUNER_OK);
}



STATIC BOOL TVD_CCI_Indicator(VOID)
{		
    #if 0    //d110322,modified by LW, replaced by the CCI index
    mcSHOW_DBG_MSG2(("TVD CCI_Indicator 0x%X\n",(IO_READ16(VIDEO_IN0_BASE, 0x60) >> 1) & 0x3FF));

    if(((IO_READ16(VIDEO_IN0_BASE, 0x60) >> 1) & 0x3FF) > 0x0020) // CCI Present
    {
    	return TRUE;
    }
    else
    {
    	return FALSE;
    }
    #endif
	mcSHOW_DBG_MSG2(("TVD CCI_Index 0x%X\n",(IO_READ16(VIDEO_IN0_BASE, 0x60)) & 0x001));

    if(((IO_READ16(VIDEO_IN0_BASE, 0x60)) & 0x001) > 0) // CCI Present
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
    ATD_GetReg(psDemodCtx, REG_ATD_TD_CFG, &ucValue, 1);
    ucValue &= 0xF0;
    ucValue |= 0x0D;
    ATD_SetReg(psDemodCtx, REG_ATD_TD_CFG, &ucValue, 1);
    //2. Load Tone Script
    ATD_SetReg(psDemodCtx, REG_ATD_TD_BPF_MUL_COEF_0, &(CCI_Tone_BW3[CCIidx][0]), 1);
    ATD_SetReg(psDemodCtx, REG_ATD_TD_BPF_MUL_COEF_1, &(CCI_Tone_BW3[CCIidx][1]), 1);
    ATD_SetReg(psDemodCtx, REG_ATD_TD_BPF_MUL_COEF_2, &(CCI_Tone_BW3[CCIidx][2]), 1);
    //3. Get Tone Amp
    //mcDELAY_MS(100);
    mcDELAY_MS(10);
    ATD_GetReg(psDemodCtx, REG_ATD_TD_MAX_0, &ucValue, 1);
    ToneMag += ucValue << 8;
    ATD_GetReg(psDemodCtx, REG_ATD_TD_MAX_1, &ucValue, 1);
    ToneMag += ucValue;
    mcSHOW_DBG_MSG2(("TVD CCI Tone Mag 0x%X, %d\n",ToneMag, CCIidx));
    return (UINT16)ToneMag;
}



void ATD_SetAutoConfig(ATD_CTX_T *psDemodCtx, UINT8 u1CrAcfMsk)
{
    UINT8 ucValue = 0;

    ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucValue, 1);             
    ucValue &= 0xCF;
    ucValue |= u1CrAcfMsk;
    ATD_SetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucValue, 1);  //turn-off PN & FN auto-config            
    mcSHOW_DBG_MSG2(("CR Auto Config = 0x%02X \n", u1CrAcfMsk ));

}
void ATD_SetCrFIFO(ATD_CTX_T *psDemodCtx, UINT8 u1coef_a,UINT8 u1coef_b)
{
    UINT8 ucData;
	UINT16 u2CrFifoDelay=(UINT16)(13 + mcBIT(u1coef_a) + mcBIT(u1coef_b));
	
    #if ATD_SUPPORT_CR_ENLARGED_FIFO
	    ATD_GetReg(psDemodCtx, REG_ATD_CR_FIFO_SELECTION, &ucData, 1);    //using bit7 to identify the CR FIFO length
		if ((IC_VER_5396_AA!=BSP_GetIcVersion()) && (ucData &0x80))    // the MAX CR FIFO length is 410 symbols
		{
		    mcSHOW_DBG_MSG(("The CR FIFO length is in the Extension mode!!\n")); 
		    mcSHOW_DBG_MSG2(("The CR FIFO length is betseen 0 ~ 410 symbols!!\n"));
            if (u2CrFifoDelay > cCR_FIFO_DELAY_LENGTH_Ext) 
			{
                ucData = cCR_FIFO_DELAY_LENGTH_Ext/2;
            }
            else 
			{
                ucData = (UINT8)(u2CrFifoDelay/2);
            }
		}
		else  // the MAX CR FIFO length is 192 symbols
		{
		    mcSHOW_DBG_MSG(("The CR FIFO length is in the Origional mode!!\n"));
		    mcSHOW_DBG_MSG2(("The CR FIFO length is betseen 0 ~ 192 symbols!!\n"));
		    if (u2CrFifoDelay > cCR_FIFO_DELAY_LENGTH_Org) 
			{
                ucData = cCR_FIFO_DELAY_LENGTH_Org;
            }
            else 
			{
                ucData = (UINT8)(u2CrFifoDelay);
            }		
		}
	#else
	    mcSHOW_DBG_MSG(("The CR FIFO length is in the Origional mode!!\n"));
		mcSHOW_DBG_MSG2(("The CR FIFO length is betseen 0 ~ 192 symbols!!\n"));
		if (u2CrFifoDelay > cCR_FIFO_DELAY_LENGTH_Org) 
		{
            ucData = cCR_FIFO_DELAY_LENGTH_Org;
        }
        else 
		{
            ucData = (UINT8)(u2CrFifoDelay);
        }	
    #endif
        ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1);
}
void ATD_SetCrBw(ATD_CTX_T *psDemodCtx, UINT8 u1CrBw)
{
#if fgCRMOdify
        UINT8 ucData, ucData2, u1CrBwLpf, u1CrBwTmp;
        UINT8 coef_a, coef_b;
        ucData = 0;
        ucData2 = 0;
        ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1); //PN-FN auto config
        ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData2, 1); //PN-FN auto config
		ucData &=0x30;
        ucData2 &=0x0c;
		
		if (((ucData == 0x30)&&(ucData2 ==0x04)) || ((ucData == 0x10)&&(ucData2 >=0x04)))
		{
		    mcSHOW_DBG_MSG2(("FN only case, modify the FIFO delay!! \n"));
			ATD_GetReg(psDemodCtx, REG_ATD_ACR_FN_CFG1, &u1CrBw, 1);
			u1CrBw &= 0x0F;
		}
        mcSHOW_DBG_MSG2(("Set CR BW = 0x%02X \n", u1CrBw));
        if ((u1CrBw&0xF0)>0) //setting the CR BW when the u1CrBwLpf ~= u1CrBw
        {
            u1CrBwLpf = (u1CrBw&0x0f);
			u1CrBwTmp = (u1CrBw&0xf0);
			u1CrBw = (u1CrBwTmp >> 4);
        }
		else
		{
	        u1CrBwLpf = u1CrBw;
		}
        ucData = (0x30|u1CrBwLpf);  ATD_SetReg(psDemodCtx, REG_ATD_TRAN_STG, &ucData, 1); 
        ucData = (0x40|u1CrBw);     ATD_SetReg(psDemodCtx, REG_ATD_PRR_TRAN, &ucData, 1);
        ucData = (0x30|u1CrBw);     ATD_SetReg(psDemodCtx, REG_ATD_PRR_PRFK, &ucData, 1);
        ucData = (0x40|u1CrBwLpf);  ATD_SetReg(psDemodCtx, REG_ATD_PRR_DAMRK,&ucData, 1);//0x1b3
        if (ucCrPatchState == cCR_PATCH_STATE_FN_ONLY) {
            ATD_GetReg(psDemodCtx, REG_ATD_ACR_FN_CFG1, &ucData, 1); 
            coef_a = mcGET_FIELD(ucData, 0xF0, 4);
            coef_b = mcGET_FIELD(ucData, 0x0F, 0);
        }
        else {
            coef_a = u1CrBw;
            coef_b = u1CrBwLpf;
        }

		ATD_SetCrFIFO(psDemodCtx,coef_a,coef_b);
#else
    UINT8 ucData;

    ucData = (0x30|u1CrBw);  ATD_SetReg(psDemodCtx, REG_ATD_TRAN_STG, &ucData, 1); 
    ucData = (0x40|u1CrBw);  ATD_SetReg(psDemodCtx, REG_ATD_PRR_TRAN, &ucData, 1);
    ucData = (0x30|u1CrBw);  ATD_SetReg(psDemodCtx, REG_ATD_PRR_PRFK, &ucData, 1);
    ucData = (0x40|u1CrBw);  ATD_SetReg(psDemodCtx, REG_ATD_PRR_DAMRK,&ucData, 1);
    mcSHOW_DBG_MSG2(("Set CR BW = 0x%02X \n", u1CrBw));
#endif
}


UINT8 ATD_GetOVMIdx(ATD_CTX_T *psDemodCtx)
{
#if ATD_SUPPORT_DRO_SYNC_MODE//get ovm idx from DRO
    UINT8 ucData, ucGainJitter, ucCRstate0;
	UINT8 ucOVMIdx = 0;
	UINT8 ucOVMOffset = 1; //to consist with the previous methof that the OVM index gat frpm CR_State0
	UINT32 u4Fref2 = 0;
	UINT32 u4Fref2Step = 0;
	UINT32 u4SyncSwing = 0;
//	UINT32 u4SyncHigh = 0;
	UINT32 u4OVMTarget;//, u4SyncHighTmp;

    mcSHOW_DBG_MSG2(("OVM index: The New OVM index method!!\n"));
// Step1. Set the MUX
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1);
    ucData &= 0xD0;
	ucData |= 0x21;
    ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1);
// Step2. Get the Fref2 and Swing, High
    //Swing
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MAX1, &ucData, 1);
	mcSHOW_DBG_MSG2(("OVM index: The REG_ATD_DRO_LSYNC_MAX1 is 0x%02X!!\n", ucData));
    u4SyncSwing = ucData;
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MAX2, &ucData, 1);
	mcSHOW_DBG_MSG2(("OVM index: The REG_ATD_DRO_LSYNC_MAX2 is 0x%02X!!\n", ucData));
	u4SyncSwing = (u4SyncSwing<<8);
	u4SyncSwing &= 0xff00;
	u4SyncSwing |= ucData;
	u4SyncSwing = (u4SyncSwing<<4);//resolution = 1/16
	u4SyncSwing &= 0xfffffff0;
	//Fref2
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucData, 1);
	mcSHOW_DBG_MSG2(("OVM index: The REG_ATD_DRO_RPOBE_1 is 0x%02X!!\n", ucData));
    u4Fref2 = ucData;
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_2, &ucData, 1);
	mcSHOW_DBG_MSG2(("OVM index: The REG_ATD_DRO_PROBE_2 is 0x%02X!!\n", ucData));
	u4Fref2 = (u4Fref2<<8);
	u4Fref2 &= 0xff00;
	u4Fref2 |=ucData;
	u4Fref2Step = u4Fref2;    //resolution = 1/16
	u4Fref2 = (u4Fref2<<4);
	u4Fref2 &= 0xfffffff0;	

    u4OVMTarget = u4SyncSwing;

// Step4. compute the OVM index
    if(u4OVMTarget >= 0x80000000) //to avoid the negative value
    {
        mcSHOW_DBG_MSG2(("OVM index: the swing is negative!!\n"));
		ucOVMIdx +=ucOVMOffset;
		mcSHOW_DBG_MSG2(("OVM index: The index is 0x%02X!!\n", ucOVMIdx + ucOVMOffset));
	    return ucOVMIdx ;		
    }
//    if (u4SyncHigh <= u4SyncSwing)
//    {
        if (u4OVMTarget < u4Fref2)
        {
            mcSHOW_DBG_MSG2(("OVM index: The index is less than 100% !!\n"));
	    	ucOVMIdx +=ucOVMOffset;
    		mcSHOW_DBG_MSG2(("OVM index: The index is 0x%02X!!\n", ucOVMIdx + ucOVMOffset));
	        return ucOVMIdx;
        } 
//    }
//	else
//	{
//       ucOVMIdx +=ucOVMOffset;
//		mcSHOW_DBG_MSG2(("OVM index:sync High is larger than Swing!!\n"));
//		return ucOVMIdx;
//	}

	while (ucOVMIdx < 6)
	{	    	
		ucOVMIdx +=1;
		u4Fref2 += u4Fref2Step;
		if (u4OVMTarget < u4Fref2)
        {
			break;
        }
		mcSHOW_DBG_MSG2(("OVM index: Searching the OVM index !!\n"));
	}
// Step4. return the OVM index
	mcSHOW_DBG_MSG2(("OVM index: The index is 0x%02X!!\n", ucOVMIdx + ucOVMOffset));
	ucOVMIdx +=ucOVMOffset;

	ucData = 0x2E;
	ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1); // DRO probe sel: gain_jitter	 
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucGainJitter, 1);
	mcSHOW_DBG_MSG(("Gain jitter=0x%02X\n",ucGainJitter));
	if(ucGainJitter>0x0a)    //Avoid the AM-FV case
	{
	    ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucCRstate0, 1); 
		ucOVMIdx = ((ucCRstate0 & 0x70)>>4);
	    ucOVMIdx &=0x07;	
		mcSHOW_DBG_MSG(("Get the OVM index from 0x1A0\n"));
	}

	return ucOVMIdx;
//#elif CC_MT5396_MP
//	mcSHOW_DBG_MSG2(("OVM index: The OVM index form the New1 CR model!!\n"));
//    UINT8 u1OVMIdx, u1Data;
//    ATD_GetReg(psDemodCtx, REG_ATD_CR_OVMIND, &u1Data, 1);//Read OVM IND MaxHold
//    u1OVMIdx = (u1OVMIdx>>5);
//	u1OVMIdx &= 0x07;
//	mcSHOW_DBG_MSG(("OVMIDX=%d\n",u1OVMIdx));
//	return u1OVMIdx;
    
#elif ATD_SUPPORT_OVM_HW_MAX_HOLD//d110627_Haibo:MT5389 get the OVM MAX Hold directly(no need to set OVM_THD more)
    UINT8 u1Data;
	UINT8 u1OVMIdx=1;
	if(IC_VER_5396_AA==BSP_GetIcVersion())//MT5396 before ECO
	{
	  mcSHOW_DBG_MSG2(("IC Ver=53960000 OVM index: The OVM index form the CR model!!\n"));
	while(u1OVMIdx<8)
	{
	u1Data=(u1OVMIdx<<5);
    ATD_SetReg(psDemodCtx, REG_ATD_OVM_THD, &u1Data, 1);//Set OVM THD
	ATD_GetReg(psDemodCtx, REG_ATD_CR_OVMIND, &u1Data, 1);//Read OVM IND twice
	ATD_GetReg(psDemodCtx, REG_ATD_CR_OVMIND, &u1Data, 1);//Read OVM IND
	if(u1Data &0x80)
	{
	   mcSHOW_DBG_MSG(("OVMIDX=%d\n",u1OVMIdx));
	   u1OVMIdx++;
	}
	else
	{
	   break;
	}
	}
	return (u1OVMIdx-1);	
	}
	else
    {
	ATD_GetReg(psDemodCtx, REG_ATD_CR_OVMIND, &u1Data, 1);//Read OVM IND
	mcSHOW_DBG_MSG2(("OVM index: Get the Max hold %d!!\n",((u1Data>>5)&0x07)));
#if ATD_SUPPORT_OVM_ACCURATE_RANK	
    mcSHOW_DBG_MSG(("Normalized OVM index: %d\n",(u1Data>>1)&0x0F));
#endif
	return ((u1Data>>5)&0x07);
	}
#else  //for MT5396/68 ES
	UINT8 u1OVMIdx=1;
	UINT8 u1Data,u2Data;
	mcSHOW_DBG_MSG2(("OVM index: The OVM index form the CR model!!\n"));
    ATD_GetReg(psDemodCtx, REG_ATD_OVM_THD, &u2Data, 1);
    u2Data &= 0x1F;
	while(u1OVMIdx<8)
	{
	u1Data=(u1OVMIdx<<5);
    u1Data |= u2Data;
    ATD_SetReg(psDemodCtx, REG_ATD_OVM_THD, &u1Data, 1);//Set OVM THD
	ATD_GetReg(psDemodCtx, REG_ATD_CR_OVMIND, &u1Data, 1);//Read OVM IND twice
	ATD_GetReg(psDemodCtx, REG_ATD_CR_OVMIND, &u1Data, 1);//Read OVM IND
	if(u1Data &0x80)
	{
	   mcSHOW_DBG_MSG(("OVMIDX=%d\n",u1OVMIdx));
	   u1OVMIdx++;
	}
	else
	{
	   break;
	}
	}
	return (u1OVMIdx-1);	
#endif
}



STATIC void ATD_LoadCCIScript(ATD_CTX_T *psDemodCtx,UINT8 CCIidx)
{
    UINT8 ucValue;
    /*//Load Tone Detection Script
    //1. Set Tone Rejection BW3
    ATD_GetReg(psDemodCtx, REG_ATD_TD_CFG, &ucValue, 1);
    ucValue &= 0x0F;
    ucValue |= 0xE0; //0xF0 for BW3 , 0xE0 for BW2
    ATD_SetReg(psDemodCtx, REG_ATD_TD_CFG, &ucValue, 1);
    //2. Load Tone Script
    ATD_SetReg(psDemodCtx, REG_ATD_TD_NF_MUL_COEF_0, &(CCI_Tone_BW2[CCIidx][0]), 1);
    ATD_SetReg(psDemodCtx, REG_ATD_TD_NF_MUL_COEF_1, &(CCI_Tone_BW2[CCIidx][1]), 1);
    ATD_SetReg(psDemodCtx, REG_ATD_TD_NF_MUL_COEF_2, &(CCI_Tone_BW2[CCIidx][2]), 1);*/

    // for Q-lab CCI case, disable CVBS scaling, fine-tune CR loop BW    
    //ATD_SetReg(psDemodCtx, 0x00a, &ucValue, 1);
    //ATD_SetReg(psDemodCtx, 0x00f, &ucValue, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucValue, 1);
    ucValue = ucValue & 0x7F;
    ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucValue, 1);
    //ucValue = 0x2C;
    //ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucValue, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucValue, 1);
    ucValue = ucValue & 0x7F;
    ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucValue, 1);


    ucValue = 0x37;
    ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &ucValue, 1);
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
      /*ATD_GetReg(psDemodCtx, REG_ATD_TD_CFG, &ucValue, 1);
    ucValue &= 0x0F;						
    ATD_SetReg(psDemodCtx, REG_ATD_TD_CFG, &ucValue, 1);*/
      
            ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucValue, 1);
            ucValue = ucValue | 0x80;
            ATD_SetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucValue, 1);
            //ucValue = 0x38;
            //ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucValue, 1);
            ATD_GetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucValue, 1);
            ucValue = ucValue | 0x80;
            ATD_SetReg(psDemodCtx, REG_ATD_DRO_DCR_EXT1, &ucValue, 1);
            CCI_Indicator = 0;
            //CCITest = 0;
     //}		
        }	
    }	

    if (CCI_Indicator != 0) {
        u1CrAutoConfig = 0;
        u1CrBandwidth = 7;
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
    ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucValue, 1);
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
    	    //ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &ucData, 1);
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
    	    //ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &ucData, 1);
    	    ATD_SetEQ(psDemodCtx, itGetEqScriptNormal);
    	    Weak_Sinal_Indicator = 0;
    	    mcSHOW_DBG_MSG(("Escape weak signal mode!\n"));
        }
    }    
}

STATIC void ATD_Patch_CR(ATD_CTX_T *psDemodCtx)
{	
#if fgCRMOdify
	UINT8 ucData = 0;	 
	UINT8 ucCRstate, ucNoiselevel = 0, ucCmetrix = 0, ucOvmIdx, fgPN, fgFN;
	UINT8 ucNullRegion, ucCfoTraStep, fgAutoReconfig;
	#if fgEnable_CR_AVG_Mode
	UINT8 fgPrrAvgEn;
	#endif
	ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucData, 1);	   
	ucCRstate = mcGET_FIELD(ucData, MSK_ATD_CR_STATE0_CR_STATE, 0);
    ucOvmIdx=ATD_GetOVMIdx(psDemodCtx);		
	
	if (ucCRstate != cCR_STATE_TRA) {
		mcSHOW_DBG_MSG2(("CR not in TRA, state = 0x%02X\n", ucCRstate));
		return;
	}
	ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucNoiselevel, 1);
	ATD_GetReg(psDemodCtx, REG_ATD_CMETRIC0, &ucCmetrix, 1);	
	ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData, 1);	  
	fgPN = mcGET_FIELD(ucData, MSK_ATD_CR_STATE1_PNIND, 3);
	fgFN = mcGET_FIELD(ucData, MSK_ATD_CR_STATE1_FNIND, 2);
	mcSHOW_DBG_MSG2(("CMetrix	= 0x%02X\n", ucCmetrix));
	mcSHOW_DBG_MSG2(("NoiseLvl	= 0x%02X\n", ucNoiselevel));
	mcSHOW_DBG_MSG2(("PN/FN/OVM = %d/%d/%d\n", fgPN, fgFN, ucOvmIdx));
	
	if (fgOVMBypass)	
	{
		ucOvmIdx=0;    //Test for the OVM case
		mcSHOW_DBG_MSG2(("Bypass OVM mode\n"));
	}
	
	if(ucNoiselevel < cCR_PATCH_NOISE_THRHLD_LOW) 
	{   // normal power
	//#if ((defined CC_MT5396) || defined(CC_MT5368))
	if ((fgPN == 0) && (fgFN == 0)) 
	{
	//#else
    //        if (ucOvmIdx >= 4) 
	//		{
	//		    ucCrPatchState = cCR_PATCH_STATE_STRONG_OVM;
	//	    }
	//	    else if ((fgPN == 0) && (fgFN == 0) && (ucOvmIdx >= 2)) 
	//		{
	//#endif	
        if(ucOvmIdx >= 2)//d110702_Haibo:OVM mode
           {
		        u1CrPatchNoOvmCntr=0;
				ucCrPatchState = cCR_PATCH_STATE_OVM;
           }
		else
           {
             	if(u1CrPatchNoOvmCntr<cNO_OVM_CNT_THRHLD)//d110702_Haibo:OVM transient mode
                {
   				u1CrPatchNoOvmCntr++;
				ucCrPatchState = cCR_PATCH_STATE_OVM_WAIT;
                }
				else
				{
				  ucCrPatchState =cCR_PATCH_STATE_NORM; 
				}
           }
	}
	else 
	{
			if((fgPN == 0) && (fgFN != 0)) 
			{
				ucCrPatchState = cCR_PATCH_STATE_FN_ONLY;
			}
			else 
			{
				ucCrPatchState = cCR_PATCH_STATE_NORM_PN_FN;
			}
	}
	 }
	 else if (ucNoiselevel > cCR_PATCH_NOISE_THRHLD_HIGH) { // weak power
		if (ucCmetrix < cCR_PATCH_CMTRX_PN_THRHLD)
		{
			if (u1CrPatchNoPnCntr < cNO_PN_CNT_THRHLD) {
				u1CrPatchNoPnCntr++;
				ucCrPatchState = cCR_PATCH_STATE_WEAK_SIG_WAIT;
			}
			else {
				ucCrPatchState = cCR_PATCH_STATE_WEAK_SIG;
			}
		}
		else {
		    u1CrPatchNoPnCntr=0;
			ucCrPatchState = cCR_PATCH_STATE_WEAK_SIG_PN;
		}
	}
	mcSHOW_DBG_MSG(("CR State=%d\n",ucCrPatchState));
	switch (ucCrPatchState) {
	case cCR_PATCH_STATE_WEAK_SIG:
		#if fgEnable_CR_AVG_Mode
		u1CrBandwidth = 6;
		ucNullRegion = 1;
		ucCfoTraStep = 4;
		fgPrrAvgEn = 0;
		#else
        u1CrBandwidth = 7;//d110903_Haibo:weak signal CR BW is 7
		ucNullRegion = 1;
		ucCfoTraStep = 4;
		#endif
		fgAutoReconfig = 0;
		mcSHOW_DBG_MSG2(("CR state: cCR_PATCH_STATE_WEAK_SIG!\n"));
		break;
	case cCR_PATCH_STATE_WEAK_SIG_WAIT:
	    #if fgEnable_CR_AVG_Mode 
		u1CrBandwidth = 5;
		ucNullRegion = 1;
		ucCfoTraStep = 0;
		fgPrrAvgEn = 0;
		#else
		u1CrBandwidth = 5;
		ucNullRegion = 2;
		ucCfoTraStep = 0;
		#endif
		fgAutoReconfig = 0;
		mcSHOW_DBG_MSG2(("CR state: cCR_PATCH_STATE_WEAK_SIG_WAIT!\n"));
		break;
	case cCR_PATCH_STATE_WEAK_SIG_PN:
	    #if fgEnable_CR_AVG_Mode 
		u1CrBandwidth = 5;
		ucNullRegion = 1;
		ucCfoTraStep = 0;
		fgPrrAvgEn = 0;
		#else
        u1CrBandwidth = 5;
		ucNullRegion = 2;
		ucCfoTraStep = 0;
		#endif
		fgAutoReconfig = 0;
		mcSHOW_DBG_MSG2(("CR state: cCR_PATCH_STATE_WEAK_SIG_PN!\n"));
		break;
	case cCR_PATCH_STATE_STRONG_OVM:
	    #if fgEnable_CR_AVG_Mode 
		u1CrBandwidth = 6;
		ucNullRegion = 1;
		ucCfoTraStep = 4;
		if ((fgPN == 0) && (fgFN == 0)) {
			fgPrrAvgEn =1;
		}
		else {
			fgPrrAvgEn = 0;
		}
		#else
        u1CrBandwidth = 7;
		ucNullRegion = 2;
		ucCfoTraStep = 4;
		#endif
		fgAutoReconfig = 0;
	   mcSHOW_DBG_MSG2(("CR state: cCR_PATCH_STATE_STRONG_OVM!\n"));
		break;
	case cCR_PATCH_STATE_OVM:
	    #if fgEnable_CR_AVG_Mode 
			u1CrBandwidth = 6;
			ucNullRegion = 1;
			ucCfoTraStep = 4;
		    if ((fgPN == 0) && (fgFN == 0)) {
				fgPrrAvgEn = 1;
			}
			else {
				fgPrrAvgEn = 0;
			}
		#else
			u1CrBandwidth = 7;
			ucNullRegion = 2;
			ucCfoTraStep = 4;
		#endif
			fgAutoReconfig = 1;
		   mcSHOW_DBG_MSG2(("CR state: cCR_PATCH_STATE_OVM!\n"));
			break;
	case cCR_PATCH_STATE_OVM_WAIT:
	case cCR_PATCH_STATE_NORM_PN_FN:
	case cCR_PATCH_STATE_FN_ONLY:
	case cCR_PATCH_STATE_NORM:
	    #if fgEnable_CR_AVG_Mode
		u1CrBandwidth = 6;
		ucNullRegion = 1;
		ucCfoTraStep = 4;
		if ((fgPN == 0) && (fgFN == 0)) {
			fgPrrAvgEn = 1;
		}
		else {
			fgPrrAvgEn = 0;
		}
		#else
        u1CrBandwidth = 6;
		ucNullRegion = 2;
		ucCfoTraStep = 4;
		#endif
		fgAutoReconfig = 1;
		mcSHOW_DBG_MSG2(("CR state: cCR_PATCH_STATE_NORM_PN_FN!\n"));
		break;
	default:
	   #if fgEnable_CR_AVG_Mode 
		u1CrBandwidth = 6;
		ucNullRegion = 1;
		ucCfoTraStep = 4;
		if ((fgPN == 0) && (fgFN == 0)) {
			fgPrrAvgEn =fgEnable_CR_AVG_Mode;
		}
		else {
			fgPrrAvgEn = 0;
		}
		#else
        u1CrBandwidth = 6;
		ucNullRegion = 2;
		ucCfoTraStep = 4;
		#endif
		fgAutoReconfig = 1;
		mcSHOW_DBG_MSG2(("CR state: error state!\n"));
		break;
	}
    ATD_SetNullRegion(psDemodCtx,ucNullRegion);
	ATD_GetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);
	mcSET_FIELD(ucData, ucCfoTraStep, MSK_ATD_PE_CFOK_CFO_K_TRK, 0);
	ATD_SetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);
	ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
	if (fgAutoReconfig) {
		mcSET_MASK(u1CrAutoConfig, (MSK_ATD_ACR_CFG_PN_ACFG_EN|MSK_ATD_ACR_CFG_FN_ACFG_EN));
	}
	else {
		mcCLR_MASK(u1CrAutoConfig, (MSK_ATD_ACR_CFG_PN_ACFG_EN|MSK_ATD_ACR_CFG_FN_ACFG_EN));
	}
	#if fgEnable_CR_AVG_Mode 
	    ATD_GetReg(psDemodCtx, REG_ATD_CR_AVGPE1, &ucData, 1);
	    if (fgPrrAvgEn) {// no PN->enable hw auto->PN->HW AVG disable->maually disable
    		mcSET_MASK(ucData, MSK_ATD_CR_AVGPE1_AVG_PRR_EN);//0x08 b3:1
	    }
	    else {//manually disable
		     mcCLR_MASK(ucData, MSK_ATD_CR_AVGPE1_AVG_PRR_EN);//b3:0
	    }
	    ATD_SetReg(psDemodCtx, REG_ATD_CR_AVGPE1, &ucData, 1);	
	#endif
#else    //origion method;d110328
    UINT8 ucData, ucCRstate0, ucCRstate1, ucNoiselevel, ucCmetrix, ucOVMIdx;	
	UINT8 ucPRFDelay, ucLPFDelay,ucPRFShl, ucLPFShl;
	UINT8 ucPRFDelayIni=0x01;
	UINT8 ucLPFDelayIni=0x01;
    
    // new PN patch, when noise level is large (signal<60dBuV), turn off PN auto-config, wide CR BW
    // NTSC or PAL only
    // LC 080505
    // Modify for all mode
    // LC 080515
             
    if(u1CRPatchLoopCnt < cCR_PATCH_CHANGE_PN_THRESHOLD) {
        u1CRPatchLoopCnt++;
    }
    else {
        ucData = 0x1C;            
        ATD_SetReg(psDemodCtx, REG_ATD_CR_PNDEC_CFG, &ucData, 1);    
    }	
    
     ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucCRstate0, 1);         	
 	
    if( ((ucCRstate0 & 0x03) == 0x03))// && (u1CRPatchLoopCnt==5) )  //CR in tracking state
    {
        ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucNoiselevel, 1);    

        if(ucNoiselevel > 0x19)  //signal < 25dBuV, weak power
        {		                
            u1CrAutoConfig = 0;
            NoOVMCounter0 = cNO_OVM_THRESHOLD;
	    //if ((pucChipReg[REG_ATD_CR_STATE1] & 0x0C) == 0x00)  // no PN/FN
	    
            ATD_GetReg(psDemodCtx, REG_ATD_CMETRIC0, &ucCmetrix, 1);    
            mcSHOW_DBG_MSG2(("CMetrix = 0x%02X\n", ucCmetrix));
	    
            if (ucCmetrix < 0x20)  // CMetrix <= REG_ATD_DRO_OBS20, weak PN
            {
                if (PNCounter < cPN_CNTR_THRESHOLD) {
                    PNCounter++;
                    mcSHOW_DBG_MSG2(("CR entering weak signal mode...waiting\n"));
                }
                else {
                    u1CrBandwidth = 7;
                    
                    ucData = 0x17;  ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &ucData, 1);  //lower null-region for CR weak signal mode
                    ucData = 0x7C;  ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1);  //FIFO delay  
                                                                                          
                    //GA FT issue, DISABLE pilot quality mask of pilot stablity                                                                 
                    ATD_GetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);    
                    ucData |= 0x04; 
                    ATD_SetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);
                    mcSHOW_DBG_MSG2(("CR enter weak signal mode!\n"));
                }
            }
            else
            {
                PNCounter = 0;
                u1CrBandwidth = 5; //a little wider BW
                ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &u1NullRegion, 1);  //higher null-region for CR weak signal+PN mode
                ucData = 0x4D;	ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1);  //FIFO delay     
                
                //GA FT issue, ENABLE pilot quality mask of pilot stablity
                ATD_GetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);   
                if((ucData & 0x04) == 0x04)  
                {
                    ucData = ucData & 0x80; 
                    ATD_SetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);
                }     
                mcSHOW_DBG_MSG2(("CR enter weak signal+PN mode!\n"));                
            }				
        }
        else if(ucNoiselevel < 0x15)  // signal > 25dBuV, normal power
        {
            ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData, 1);
            if ((ucData & 0x20) >> 5 ) {  // if pilot stability == 1
                u1CrAutoConfig = cFN_ACFG_EN|cPN_ACFG_EN;               
            }
            else {
                u1CrAutoConfig = cFN_ACFG_EN|cPN_ACFG_EN;             
                mcSHOW_DBG_MSG2(("Pilot Stability = 0 !!\n"));
            }
                       
            PNCounter = cPN_CNTR_THRESHOLD;
    	    
            ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &u1NullRegion, 1);  //higher null-region for CR strong signal mode
    	    
            ATD_GetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1); //GA FT issue    
            if((ucData & 0x04) != 0x04)  
            {
                ucData = ucData | 0x04; 
                ATD_SetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);
            } 

            ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData, 1);    
            ucCRstate1 = ucData;
    	    
            mcSHOW_DBG_MSG2(("PN/FN = 0x%02X\n", (ucCRstate1&0x0C)>>2 ));

			ucOVMIdx=ATD_GetOVMIdx(psDemodCtx);
			mcSHOW_DBG_MSG(("OVM IDX=%d\n",ucOVMIdx));
            //if ((ucCRstate0 & 0x70) >= 0x40){  // OVM_IDX >= 4
            if(ucOVMIdx>=4)  
            {
                u1CrBandwidth = 7; //narrow BW
                NoOVMCounter0 = 0;
                mcSHOW_DBG_MSG2(("CR enter strong OVM mode!\n"));
            }
            else if ((ucCRstate1 & 0x0C) == 0x00)  // no PN/FN		
            {
                ucData = 0x7C;  ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1);  //FIFO delay       
                
                //if((ucCRstate0 & 0x70) >= 0x20)  // OVM_IDX >= 2
                if(ucOVMIdx>=2)
                {
                    u1CrBandwidth = 7; //narrow BW
                    NoOVMCounter0 = 0;
                    mcSHOW_DBG_MSG2(("CR enter OVM mode!\n"));
                }
                else
                {
                    if (NoOVMCounter0 < cNO_OVM_THRESHOLD) {
                        NoOVMCounter0++;
                        mcSHOW_DBG_MSG2(("CR entering normal mode, waiting...\n"));		
                    }                        
                    else {
                        u1CrBandwidth = 6; 
                        mcSHOW_DBG_MSG2(("CR enter normal mode!\n"));			
                    }
                }
            }
            else  // PN/FN exists
            {
                
                if((ucCRstate1 & 0x0C) == 0x04)  //only FN exists
                {
                    ATD_GetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1);             
                    if(ucData != 0x0C)
                    {            
                        ucData = 0x0C;
                        ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG1, &ucData, 1);  //FIFO delay       
    	            }    	
    	        } 
				else	//auto-compuate the fifo delay when PN exists;LiWei;d100818
				{
				ATD_GetReg(psDemodCtx, REG_ATD_ACR_PN_CFG1, &ucData, 1); 
                ucPRFShl = ((ucData >> 4) & 0x0f);
				ucLPFShl = (ucData & 0x0f);
				ucPRFDelay = (ucPRFDelayIni << ucPRFShl);
				ucLPFDelay = (ucLPFDelayIni << ucLPFShl);
				ucData = 0x0d + ucPRFDelay + ucLPFDelay;
				ATD_SetReg(psDemodCtx, REG_ATD_ACR_PN_CFG0, &ucData, 1);
				}                      	
                u1CrBandwidth = 6; 
                NoOVMCounter0 = cNO_OVM_THRESHOLD;
                mcSHOW_DBG_MSG2(("CR in PN/FN mode!\n"));
    	    }            
    	}
    }
#endif
}

STATIC void ATD_Patch_StrongChroma(ATD_CTX_T *psDemodCtx)
{       
    UINT32 u4Temp;
    UINT16 u2ChromaGain;
     
    u4Temp = mcRISC_Reg(cRISC_TVD_BASE, 0x084);
    u2ChromaGain =(UINT16)((u4Temp & 0xFFF00000) >> 20);
    mcSHOW_DBG_MSG2(("TVD chroma gain = 0x%04X\n", u2ChromaGain));
        //only patch PAL-BG
    if(psDemodCtx->u1SubSysIdx != MOD_PAL_BG)
        return;
		
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


#if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
void ATD_Patch_DRO_AGC(ATD_CTX_T *psDemodCtx)
{
	UINT8 ucNoiselevel,u1GainJitter,ucData;
	UINT8 u1IsAGCMode=2;
	ucData = 0;
	u1GainJitter = 0;
	//Get  CVBS SNR
		ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucData, 1);	  
		ucNoiselevel = ucData;
		
		mcSHOW_DBG_MSG(("SNR=0x%02X\n",ucNoiselevel));
		
		if(ucNoiselevel>0x06)//CVBS SNR >0x06
		{
			u1IsAGCMode=0;//normal-mode
		}
		else if(ucNoiselevel<=0x05)
		{
			//Get gain jitter
			 ucData = 0x2E;
			 ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1); // DRO probe sel: gain_jitter	 
			 ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &u1GainJitter, 1);
			 mcSHOW_DBG_MSG(("Gain jitter=0x%02X\n",u1GainJitter));
			 if(u1GainJitter>0x0a)
			  {
				 u1IsAGCMode=1;			  
			  }
			 if(u1GainJitter<0x08)
			  {
				 u1IsAGCMode=0;		  
			  }
		}
		if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx == MOD_SECAM_L1))
        {
            u1IsAGCMode=0;
			mcSHOW_DBG_MSG(("The TV system is SECAM.\n"));
		}
		mcSHOW_DBG_MSG(("Is AGC mode?%d\n",u1IsAGCMode));
		
		if(u1IsAGCMode==1)//AGC mode;disable LP3
		{ 
		    ATD_GetReg(psDemodCtx, REG_ATD_DRO_VSI_CFG4, &ucData, 1);			
			ucData &= 0xFE;    //Disable LP3
			ATD_SetReg(psDemodCtx, REG_ATD_DRO_VSI_CFG4, &ucData, 1);			
		}
		
		if(u1IsAGCMode==0)//normal mode; enable LP3
		{
			ATD_GetReg(psDemodCtx, REG_ATD_DRO_VSI_CFG4, &ucData, 1);			
			ucData |= 0x01;    //Enable LP3
			ATD_SetReg(psDemodCtx, REG_ATD_DRO_VSI_CFG4, &ucData, 1);
		}

}
#elif defined(CC_MT5365) || defined(CC_MT5395)
STATIC UINT8 fgAsyncMode;
STATIC UINT8 fgSyncmode;
void ATD_Patch_DRO_AGC(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucNoiselevel,u1GainJitter,ucData;
    UINT8 u1IssyncMode=2;
//Get  CVBS SNR
    ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucData, 1);    
    ucNoiselevel = ucData;
	
	mcSHOW_DBG_MSG(("SNR=0x%02X\n",ucNoiselevel));
	
	if(ucNoiselevel>0x06)//CVBS SNR >0x06
	{
	    u1IssyncMode=0;//Async-mode
	}
	else if(ucNoiselevel<=0x05)
	{
	    //Get gain jitter
         ucData = 0x2E;
         ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1); // DRO probe sel: gain_jitter    
         ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &u1GainJitter, 1);
		 mcSHOW_DBG_MSG(("Gain jitter=0x%02X\n",u1GainJitter));
         if(u1GainJitter>0x0a)
          {
             u1IssyncMode=1;      		  
          }
		 if(u1GainJitter<0x08)
          {
             u1IssyncMode=0;  		  
          }
	}
	mcSHOW_DBG_MSG(("Is Sync mode?%d\n",u1IssyncMode));
	
	if(u1IssyncMode==1)
	{
	    ucData=0x19;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1);
		ucData=0x20;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG05, &ucData, 1);//FIFO delay
		ucData=0xc0;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);	
		if(fgSyncmode == 0)
             {
                 ucData = 0xEF;
                 ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
                 ucData = 0xFF;
                  ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
                  fgSyncmode = 1;
             }
             fgAsyncMode=0;

	}
    if(u1IssyncMode==0)//async mode
    {
        ucData=0x14;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1);
		ucData=0x87;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG05, &ucData, 1);//FIFO delay 	
		if(fgAsyncMode == 0)
             {
                    ucData = 0xEF;
                    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
                    ucData = 0xFF;
                    ATD_SetReg(psDemodCtx, REG_ATD_CTL_RST2, &ucData, 1);
                    fgAsyncMode = 1;
             }
             fgSyncmode=0;

    }
}
#else
void ATD_Patch_DRO_AGC(ATD_CTX_T *psDemodCtx)
{	
}
#endif

//#if !ATD_SUPPORT_DRO_SYNC_MODE && !ATD_SUPPORT_H_BLANK_MODE
VOID ATD_DroTargetLevelModifySyncH(ATD_CTX_T *psDemodCtx, BOOL fgSel, UINT8 ucACC_MonCntSel, BOOL fgRegAuto, UINT32 u4TarSyncH, UINT16 u2DroTarLvlInitial)
{

if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx == MOD_SECAM_L1))
{
//only for the SECAM L
	UINT8 ucData1, ucData2, ucWGain, ucWGainTmp;
	UINT16 u2Fref2, u2SyncH_Wagc, u2DroTargetLevel, u2Counter, u2CounterMax, u2DroRatio;
	INT16 i2DroTargetLevel, i2DroWadc, i2TarBlk, i2ACC_MON, i2SyncH_Wagc; 
	INT32 i4WGainInver, i4BlkWagc, i4SyncWagc, i4SyncH_WagcTmp,i4ACC_MONTmp; 
	UINT32 u4CalHeigh, u4DroTargetLevelTmp, u4SyncH_Wagc, u4DroTarLevNxt, u4WGainTmp1, u4WGainTmp2;
	UINT16 u2DroTagetLvlMax, u2DroTagetLvlMin;
	UINT16 u2DRO_CVBS_UP_BUD_NEXT, u2DRO_CVBS_UP_BUD_NEXT_TMP, u2DroTargetLevelTMP , u2DroTargetLevelTMP2;
	u2DroTagetLvlMax = u2DroTarLvlInitial;
	u2DroTagetLvlMin = u2DroTarLvlInitial;
	ucData1 = 0;

	mcSHOW_DBG_MSG(("The System is  L/L1\n"));

	if (u2DroTarLvlInitial >= 0x4000)
	{
        mcSHOW_DBG_MSG(("The DRO target level is negative!!\n"));
        return ;
	}

	if (u2DroTagetLvlMax <= 0x03FF)
	{
        u2DroTagetLvlMax = (u2DroTagetLvlMax << 3);
	}
	else
	{
	    u2DroTagetLvlMax = 0x3FFF;
	}

    
	if (u2DroTagetLvlMin >= 0x0008)
	{
		u2DroTagetLvlMin = (u2DroTagetLvlMin >> 3);
	}
	else
	{
		u2DroTagetLvlMin = 0x0001;
	}
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucData1, 1);
	ucData1 &= 0x80;
    if (ucData1 == 0x00)
    {
        mcSHOW_DBG_MSG(("Scaling bypass==>no adjust\n"));
		ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
	    ucData1 &= 0x80;
		ucData2 = (UINT8)((u2DroTarLvlInitial >> 8) & 0x7F);
		ucData1 = ucData1 | ucData2;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
		ucData1 = (UINT8)(u2DroTarLvlInitial & 0xFF);		
	    ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData1, 1); 
        return ;
    }
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_ASYNC_CFG0, &ucData1, 1);
	ucData1 &= 0xE0;
	if (ucData1 != 0xC0)
	{
	    mcSHOW_DBG_MSG(("The Ferf2 is not sync Low==>no adjust\n"));
        return ;
	}
// Step 0: initialize the parameters	
    //u2CalHeigh (U(11,-3), according to the DRO target level)
//    u4CalHeigh = 3142; //Vpp = 1.9
//    u4CalHeigh = 3407; //Vpp = 1.97
//    u4CalHeigh = 1653;//Vpp = 1.09
    u4CalHeigh = u4TarSyncH;
// step 1 Get the parameters
    // DRO target Level
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData2, 1);
    ucData1 &= 0x7F;
    if (ucData1 >= 0x40) //	value < 0 (S(+10,_4))
    {
        i2DroTargetLevel = (ucData1*256 + ucData2) - 32768;
	}
	else
	{
	    i2DroTargetLevel = (ucData1*256 + ucData2);
	}
//	mcSHOW_DBG_MSG1(("i2DroTargetLevel	= 0x%04X\n", i2DroTargetLevel));

    // W_ADC
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_WADC_1, &ucData1, 1);	
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_WADC_2, &ucData2, 1);	
	if (ucData1 >=0x80)    //	value < 0 (S(+7,-8))
	{
        i2DroWadc = (ucData1*256 + ucData2) - 65536;
	}
	else
	{
        i2DroWadc = (ucData1*256 + ucData2);
	}
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData1, 1);
	ucData1 &=0xF0;
	ucData1 |=0x01;
	ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData1, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucData1, 1);
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_2, &ucData2, 1);
	u2Fref2 = ucData1*256 + ucData2;
	mcSHOW_DBG_MSG2(("Fref2	= 0x%04X\n", u2Fref2));

    //Target Blank
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_3, &ucData1, 1);
    if (ucData1 >0x80) //value < 0 (S(+11,+4))
	{
	    i2TarBlk = -(256 - ucData1);
	}
    else
    {
        i2TarBlk = ucData1;
	}
//	mcSHOW_DBG_MSG2(("Target Blank	= 0x%04X\n", i2TarBlk));

	//W_Gain (S(1,-4)) or U(1,-4)
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_0, &ucWGain, 1);
	ucWGain &=0x3F;
//	mcSHOW_DBG_MSG2(("I2C_GAIN	= 0x%04X\n", ucWGain));
#if ATD_SUPPORT_ENLARGED_SCALING_GAIN
    if ((ucWGain==0))
    {
        mcSHOW_DBG_MSG2(("Error I2CGain"));
        return;
	}
#else
    if ((ucWGain==0)||(ucWGain==0x37))
    {
        mcSHOW_DBG_MSG2(("Error I2CGain"));
        return;
	}
#endif
//Step 2. To find the Target_blank(S(11,4))/WGain(S,1,-4), output(S(11,-3))
#if ATD_SUPPORT_ENLARGED_SCALING_GAIN
    ucWGainTmp = ucWGain;
#else

	if (ucWGain >= 0x20) //find the ABS of ucWGain
	{
        ucWGainTmp = 64 - ucWGain;
	}
	else
	{
        ucWGainTmp = ucWGain;
	}
#endif
//    mcSHOW_DBG_MSG2(("ucWGainTmp	= 0x%02X\n", ucWGainTmp));

if (ucWGainTmp <= 1)
{
    i4WGainInver= 32767;
}
else
{
    i4WGainInver = 0;
	u4WGainTmp1 = 0x8000;
	u4WGainTmp2 = (UINT32)ucWGainTmp;
	u4WGainTmp2 = (u4WGainTmp2 << 16);
	for(u2Counter=0;u2Counter<16;u2Counter++) 
	{
	    if (u4WGainTmp1 >= u4WGainTmp2)
	    {
	        u4WGainTmp1 = u4WGainTmp1 - u4WGainTmp2;
			i4WGainInver = i4WGainInver + 1;
	    }
		i4WGainInver = (i4WGainInver << 1);
		u4WGainTmp2 = (u4WGainTmp2 >> 1);
	}
}
#if ATD_SUPPORT_ENLARGED_SCALING_GAIN
    i4WGainInver = i4WGainInver;
#else
	if (ucWGain >= 0x20) //
	{
        i4WGainInver = -i4WGainInver;
	}    
#endif
//    mcSHOW_DBG_MSG2(("WGainInver = = 0x%08X\n", i4WGainInver));

//Step 3. Obtain the Clamp_biasS(11,-3)    
    switch (ucACC_MonCntSel)
    {
        case 0:
            u2CounterMax = 1;
			break;
		case 1:
            u2CounterMax = 2;
			break;
		case 2:
            u2CounterMax = 4;
			break;
	    default:
			u2CounterMax = 1;
	        break;
	}
    i4ACC_MONTmp = 0;
	
    for (u2Counter=1;u2Counter <= u2CounterMax;u2Counter++)
    {
        ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_10, &ucData1, 1);
        ucData1 &= 0x7F;
        ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_11, &ucData2, 1);    
     	if (ucData1>=0x80)
     	{
            i2ACC_MON = 32768 - (ucData1*256 + ucData2);
    	}
        else
       	{
            i2ACC_MON = (ucData1*256 + ucData2);
        }

		i4ACC_MONTmp = (INT32)i2ACC_MON + i4ACC_MONTmp;
		mcDELAY_MS(30);
    }
	i4ACC_MONTmp = (i4ACC_MONTmp >> ucACC_MonCntSel);
	i2ACC_MON = (INT16)(i4ACC_MONTmp & 0xFFFF);
	
    mcSHOW_DBG_MSG2(("ACC_MON = 0x%04X\n", i2ACC_MON));
	
//Step 4. Target_blank/WGain  + Clamp_bias (output = S(11,-3))
    i4BlkWagc = (i2TarBlk*i4WGainInver);
	if (i4BlkWagc < 0)
	{
        i4BlkWagc = (i4BlkWagc >> 4);
		i4BlkWagc |=0xf0000000;
	}
	else
	{
        i4BlkWagc = (i4BlkWagc >> 4);
	}
    i4BlkWagc = i4BlkWagc + i2ACC_MON;
//	mcSHOW_DBG_MSG2(("i4BlkWagc = 0x%08X\n", i4BlkWagc));
	
//Step 5. Select the data for the SyncWagc (S(18,-3))
    if (fgSel)
    {
        i4SyncWagc = (INT32)i2DroTargetLevel;
	}
	else
	{
        i4SyncWagc = ((INT32)u2Fref2*(INT32)i2DroWadc) + 128;

		if (i4SyncWagc < 0)
		{
            i4SyncWagc = (i4SyncWagc >> 8);
			i4SyncWagc |= 0xFF000000;
		}
		else
		{
            i4SyncWagc = (i4SyncWagc >> 8);
		}
	}
//    mcSHOW_DBG_MSG2(("SyncWagc = = 0x%08X\n", i4SyncWagc));    

//Step 6. Estim the extra gain for the DRO target level 
        // i2SyncH_Wagc S(11,-3)
        //u2SyncH_Wagc U(11,-3)
        //u2DroTargetLevel U(16,-13)
        //u4DroTarLevNxt U(11,-18)
	i4SyncH_WagcTmp = i4BlkWagc - i4SyncWagc;
	i2SyncH_Wagc = i4SyncH_WagcTmp;
    if (i4SyncH_WagcTmp > 32767)
    {
        i2SyncH_Wagc = 32767;
	}
	else if (i4SyncH_WagcTmp < -32767)
	{
        i2SyncH_Wagc = -32767;
	}

	if (i2SyncH_Wagc < 0)
	{
        u2SyncH_Wagc = -i2SyncH_Wagc;
	}
	else
	{
        u2SyncH_Wagc = i2SyncH_Wagc;
	}
//	mcSHOW_DBG_MSG2(("SyncH_Wagc = = 0x%02X\n", u2SyncH_Wagc));	

	if (i2DroTargetLevel < 0)
	{
        u2DroTargetLevel = 256 + i2DroTargetLevel;
	}
	else
	{
        u2DroTargetLevel = i2DroTargetLevel;
	}
	
	u4DroTargetLevelTmp = (UINT32)u2DroTargetLevel;	
	u4DroTargetLevelTmp = (u4DroTargetLevelTmp <<15);
	u4SyncH_Wagc = (UINT32) u2SyncH_Wagc;
	u4SyncH_Wagc = (u4SyncH_Wagc <<15);
	u4CalHeigh = (u4CalHeigh << 10);
	u4DroTarLevNxt = 0;
	u2DroRatio = 0;
//	mcSHOW_DBG_MSG2(("u4CalHeigh = = 0x%08X\n", u4CalHeigh));
//	mcSHOW_DBG_MSG2(("u4SyncH_Wagc = = 0x%08X\n", u4SyncH_Wagc));

	for(u2Counter=1;u2Counter<16;u2Counter++) //(Ratio U(5,-10) )
	{
	    if (u4CalHeigh >=u4SyncH_Wagc)
	    {
	        u4CalHeigh = u4CalHeigh - u4SyncH_Wagc;
			u4DroTarLevNxt = u4DroTarLevNxt + u4DroTargetLevelTmp;
			u2DroRatio = u2DroRatio + 1;
		}		    
		u4SyncH_Wagc = (u4SyncH_Wagc >> 1);
		u4DroTargetLevelTmp = (u4DroTargetLevelTmp >> 1);
		u2DroRatio = (u2DroRatio << 1);
	 }
	mcSHOW_DBG_MSG2(("ucDroRatio = 0x%04X\n", u2DroRatio));	
	u4DroTarLevNxt = (u4DroTarLevNxt >> 10); //  U(16,-3)	
	if (u4DroTarLevNxt >= 0x3FFF)
	{
        mcSHOW_DBG_MSG2(("DRO_targetEstim larger than the 0x3FF\n"));
		u4DroTarLevNxt = 0x3FFF;
	}
	
//Step 7. Modify the DRO target level    
    if (i2DroTargetLevel < 0)
    {
        i2DroTargetLevel = -(INT16)u4DroTarLevNxt;
	}
	else
	{
        i2DroTargetLevel = (INT16)u4DroTarLevNxt;
		if (i2DroTargetLevel >= u2DroTagetLvlMax)
		{
            i2DroTargetLevel = (INT16)u2DroTagetLvlMax;
			mcSHOW_DBG_MSG(("Upper boundary limitted!!\n"));
		}
		else if (i2DroTargetLevel <= u2DroTagetLvlMin)
		{
            i2DroTargetLevel = (INT16)u2DroTagetLvlMin;
			mcSHOW_DBG_MSG(("Lower boundary limitted!!\n"));
		}
	}
    mcSHOW_DBG_MSG2(("The DroTarLevNxt = 0x%04X\n", i2DroTargetLevel));

	u2DRO_CVBS_UP_BUD_NEXT_TMP = cDRO_CVBS_UP_BUD_0_Low;
	u2DRO_CVBS_UP_BUD_NEXT_TMP = (u2DRO_CVBS_UP_BUD_NEXT_TMP<<8);
    u2DRO_CVBS_UP_BUD_NEXT_TMP = u2DRO_CVBS_UP_BUD_NEXT_TMP + 0xFF;
    if (i2DroTargetLevel > u2DroTarLvlInitial)
    {
        u4DroTargetLevelTmp = (UINT32)(u2DroTarLvlInitial + (u2DroTarLvlInitial>>1));
		if (i2DroTargetLevel > u4DroTargetLevelTmp)
		{
		    u2DRO_CVBS_UP_BUD_NEXT = 0x3FFF; 		
		}
		else
		{
		    u2DroTargetLevelTMP = u2DroTarLvlInitial;
			u2DroTargetLevelTMP2 = (UINT16)i2DroTargetLevel;
			u2DRO_CVBS_UP_BUD_NEXT = 0;
			for(u2Counter=1;u2Counter<16;u2Counter++)
			{
			    if (u2DroTargetLevelTMP2 >= u2DroTargetLevelTMP)
			    {
			        u2DRO_CVBS_UP_BUD_NEXT += u2DRO_CVBS_UP_BUD_NEXT_TMP;
					u2DroTargetLevelTMP2 = u2DroTargetLevelTMP2 - u2DroTargetLevelTMP;
			    }
                u2DroTargetLevelTMP = (u2DroTargetLevelTMP >> 1);	
				u2DRO_CVBS_UP_BUD_NEXT_TMP = (u2DRO_CVBS_UP_BUD_NEXT_TMP >>1);
			}
		}
    
    }
	else
	{
        u2DRO_CVBS_UP_BUD_NEXT = u2DRO_CVBS_UP_BUD_NEXT_TMP;
	}

     mcSHOW_DBG_MSG2(("The DRO_CVBS_UP_BUD_NEXT = 0x%04X\n", u2DRO_CVBS_UP_BUD_NEXT));
	if (fgRegAuto)
	{
	    ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
	    ucData1 &= 0x80;
		ucData2 = (UINT8)((i2DroTargetLevel >> 8) & 0x7F);
		ucData1 = ucData1 | ucData2;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
		ucData1 = (UINT8)(i2DroTargetLevel & 0xFF);
	    ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData1, 1); 
		ATD_GetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_0, &ucData1, 1);
	    ucData1 &= 0x80;
		ucData2 = (UINT8)((u2DRO_CVBS_UP_BUD_NEXT >> 8) & 0x7F);
		ucData1 = ucData1 | ucData2;
		ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_0, &ucData1, 1);
		ucData1 = (UINT8)(u2DRO_CVBS_UP_BUD_NEXT & 0xFF);
	    ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_1, &ucData1, 1); 		
	}
return;
}
else
	mcSHOW_DBG_MSG2(("The System is not L/L1\n"));
return;	
}

VOID ATD_DroTargetLevelAdjust(ATD_CTX_T *psDemodCtx, UINT16 u2DroTarLvlInitial)
{
    UINT8 ucData1,u1GainJitter;
	UINT8 ucData2;
    static UINT8 ucCntBN_DroTargetLevelAdjust = 4;
	static BOOL fgBNStatus = 1;    // 1 : BN status, 0:non-BN status
	UINT8 ucBNThreshold = 4;
	
	ucData1 = 0;
	u1GainJitter = 0;
	ucData2 = 0;

if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx == MOD_SECAM_L1))
{    
	if (ATD_GetVIFLock(psDemodCtx, 0) ==cATD_VIF_UNLOCK)
	{
	    ATD_GetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_0, &ucData2, 1);
		if (ucData2 != cDRO_CVBS_UP_BUD_0_Low)
        {
            ATD_GetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_1, &ucData2, 1);
            if (ucData2 != 0xFF)
            {
                ucData2 = cDRO_CVBS_UP_BUD_0_Low;
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_0, &ucData2, 1);
				ucData2 = 0xFF;
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_1, &ucData2, 1);
            }
        }
	}	
	        ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
			mcSHOW_DBG_MSG(("ATD_DroTargetLevelAdjust:0x166=0x%02X\n",ucData1));
			ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData1, 1);
			mcSHOW_DBG_MSG(("ATD_DroTargetLevelAdjust:0x167=0x%02X\n",ucData1));

	//Get gain jitter
			 ucData2 = 0x2E;
			 ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData2, 1); // DRO probe sel: gain_jitter	 
			 ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &u1GainJitter, 1);
			 mcSHOW_DBG_MSG(("ATD_DroTargetLevelAdjust:Gain jitter=0x%02X\n",u1GainJitter));
			 
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_SCAL_4, &ucData2, 1);
	ucData2 &= 0xC0;
	mcSHOW_DBG_MSG(("Scalling Clampping indicztor =0x%02X\n",ucData2));
	ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucData1, 1); 
	mcSHOW_DBG_MSG(("CVBS SNR =0x%02X\n",ucData1));
	if ((ucData2 >= 0x80) && (ucData1 <0x06))    //REG_ATD_DRO_SCAL_4 is valid when SNR >=45dB
	{
	    if (ucCntBN_DroTargetLevelAdjust < ucBNThreshold)
	    {
	        ucCntBN_DroTargetLevelAdjust = ucCntBN_DroTargetLevelAdjust + 2;
	    }
        else
        {
            fgBNStatus = 1;
			ucCntBN_DroTargetLevelAdjust = ucBNThreshold;
			
        }
		mcSHOW_DBG_MSG(("BN/AMFV condition=0x%02X\n",fgBNStatus));
		mcSHOW_DBG_MSG(("BN/AMFV condition counter=0x%02X\n",ucCntBN_DroTargetLevelAdjust));
	}
	else
	{
	    
		if (ucCntBN_DroTargetLevelAdjust > 0)
	    {
	        ucCntBN_DroTargetLevelAdjust = ucCntBN_DroTargetLevelAdjust - 1;
	    }
        else
        {
            fgBNStatus = 0;
			ucCntBN_DroTargetLevelAdjust = 0;
        }
		mcSHOW_DBG_MSG(("BN condition=0x%02X\n",fgBNStatus));
		mcSHOW_DBG_MSG(("BN condition counter=0x%02X\n",ucCntBN_DroTargetLevelAdjust));
	}	
	    //    mcDELAY_MS(30);
//	if ((u1GainJitter < 0x08))		
	if ((u1GainJitter < 0x02))	
	{	    

	    if (fgBNStatus == 0)
	    {
            ATD_DroTargetLevelModifySyncH(psDemodCtx, 1, 1, 1, 3200, u2DroTarLvlInitial);	
    		mcSHOW_DBG_MSG2(("Adjust the DRO Target Level adaptivly\n"));  
	    }
		else
		{
                ucData2 = (UINT8)(u2DroTarLvlInitial & 0xFF);
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData2, 1);
		    	ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
	            ucData1 &= 0x80;
    		    ucData2 = (UINT8)((u2DroTarLvlInitial >> 8) & 0x7F);
		        ucData1 = ucData1 | ucData2;
		        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);		
	    		mcSHOW_DBG_MSG(("Setting the DRO target level to default!!\n"));  
				ATD_GetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_0, &ucData1, 1);
        	    ucData1 &= 0x80;
		        ucData2 =cDRO_CVBS_UP_BUD_0_Low ;
		        ucData1 = ucData1 | ucData2;
		        ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_0, &ucData1, 1);
		        ucData1 = 0xFF;
	            ATD_SetReg(psDemodCtx, REG_ATD_DRO_CVBS_UP_BUD_1, &ucData1, 1); 

		}
	}
#if 0
	else
	{
            if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx == MOD_SECAM_L1))
            {
                ucData2 = (UINT8)(u2DroTarLvlInitial & 0xFF);
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG04, &ucData2, 1);
            	ATD_GetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);
    	        ucData1 &= 0x80;
                ucData2 = (UINT8)((u2DroTarLvlInitial >> 8) & 0x7F);
    	        ucData1 = ucData1 | ucData2;
    	        ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG03, &ucData1, 1);		
        		mcSHOW_DBG_MSG(("Setting the DRO target level to default!!\n"));  
    		}
	}
#endif
    return;
}
else
{
	mcSHOW_DBG_MSG(("DRO Adjust: not SECAM system\n"));
	return;
}
	
}
//#endif 

void ATD_Patch_AGC(ATD_CTX_T *psDemodCtx)
{
    UINT8 ucData;
    S8    s1RFgain, s1IFgain;
    
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_1, &ucData, 1);
    s1RFgain = (S8)ucData;
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_1, &ucData, 1);
    s1IFgain = (S8)ucData;

    if (SmallAGC) { // if now in SmallAGC mode
        if (s1IFgain > psDemodCtx->s1IFmin) {
            ucData = 0x00;
            ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_BIAS, &ucData, 1);

            ucData = psDemodCtx->s1IFmin;
            ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_MIN, &ucData, 1);        


            ATD_GetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
            ucData = ucData | 0x08;
            ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);

            SmallAGC = 0;
            mcSHOW_DBG_MSG(("Leave small AGC mode!\n"));
        }
    }
    else {// if now NOT in SmallAGC mode
        if ((s1RFgain <= psDemodCtx->s1RFmin) && (s1IFgain <= psDemodCtx->s1IFmin)) {
            ATD_GetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);
            ucData = ucData & 0xF7;
            ATD_SetReg(psDemodCtx, REG_ATD_PRA_CFG_1, &ucData, 1);

            ucData = 0xAB;
            ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_MIN, &ucData, 1);

            ucData = 0x26;
            ATD_SetReg(psDemodCtx, REG_ATD_PRA_IF_BIAS, &ucData, 1);

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
        
    ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA3, &ucData1, 1);                
    ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucData2, 1);
    ATD_SetReg(psDemodCtx, REG_ATD_VOP_FWCPOIL, &ucData3, 1);
    
    //mcSHOW_DBG_MSG(("REG_ATD_AVS_RFA3 = 0x%02X, REG_ATD_AVS_VOL = 0x%02X, REG_ATD_VOP_FWCPOIL = 0x%02X\n", ucData1, ucData2, ucData3));
      
}

#if ATD_SUPPORT_DRO_SYNC_MODE
STATIC void ATD_Patch_DRO(ATD_CTX_T *psDemodCtx)
{
    UINT8  ucData, ucData1, ucData2, ucIFgain, ucNoiselevel;
    UINT16 u2GainJitter;//, u2SigSwing, u2SyncHeight;
                
    //get IF gain
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_1, &ucData, 1);    
    ucIFgain = ucData;        	
        
    //get DRO gain jitter
    ucData = 0x2E;
    ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1); // DRO probe sel: gain_jitter    
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucData1, 1); 
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_2, &ucData2, 1); 
    u2GainJitter = (UINT16)(ucData1 << 8) | ucData2;        
        
    //get signal swing
    //ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MAX1, &ucData1, 1); 
    //ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MAX2, &ucData2, 1); 
    //u2SigSwing = (UINT16)(ucData1 << 8) | ucData2;        
                                                
    //get sync height                        
    //ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MIN1, &ucData1, 1); 
    //ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MIN2, &ucData2, 1); 
    //u2SyncHeight = (UINT16)(ucData1 << 8) | ucData2;     
    
    ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucData, 1);    
    ucNoiselevel = ucData;
    
    switch(psDemodCtx->u1SubSysIdx)
    {
        case MOD_NTSC_M:
        if ( u2GainJitter > 0x1500 )
        {
            ucData = 0x03;  ATD_SetReg(psDemodCtx, REG_ATD_PRA_TRA_CTL1, &ucData, 1);  //disable level-switch patch
        }  
        else if ( u2GainJitter < 0x1000 )
        {
            ucData = 0x83;  ATD_SetReg(psDemodCtx, REG_ATD_PRA_TRA_CTL1, &ucData, 1);  //enable level-switch patch
        }
                
        if (ucNoiselevel < 0x04 )  // signal > 45dBuV
        {	                    
            if ( u2GainJitter > 0x2500 )  // larger gain_jitter: AM-Fv case
            {
                //default 0xDC, GA FT issue 0xAC
                ucData = 0xAC;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO sync. mode, fast W_ADC                
                ucData = 0xC0;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1); //fast ref_0,ref_1 leaky              
                mcSHOW_DBG_MSG2(("DRO in AM-Fv mode!\n"));
            }
            else if ( u2GainJitter < 0x1400 ) // non AM-Fv case
            {
                //if ( u2SyncHeight < (u2SigSwing >> 3) )       // Low-sync. case
                /*{
                    //defualt 0xD4, GA FT issue
    	            ucData = 0xA4;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO async. mode, slow W_ADC
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);           
                    mcSHOW_DBG_MSG2(("DRO in low-sync mode!\n"));    	                	            
                }
                else if ( u2SyncHeight > (u2SigSwing >> 2) )*/   // non Low-sync. case
                {
                    //default 0xD4, GA FT issue 0xA4
    	            ucData = 0xA4;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO sync. mode
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
    	            mcSHOW_DBG_MSG2(("DRO in normal mode!\n"));       	            
                }	
            }       					
        }
        else if ( ucNoiselevel > 0x07 ) //signal < 39dBuV
        {
            ucData = 0xD6;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO async. mode, slow W_ADC
            ucData = 0xC9;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
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
            ucData = 0x03;  ATD_SetReg(psDemodCtx, REG_ATD_PRA_TRA_CTL1, &ucData, 1);  //disable level-switch patch
        }  
        else if ( u2GainJitter < 0x1000 )
        {
            ucData = 0x83;  ATD_SetReg(psDemodCtx, REG_ATD_PRA_TRA_CTL1, &ucData, 1);  //enable level-switch patch
        }
    	
    	if( (ucNoiselevel > 0x0D)) // weak signal priority is higher than CCI
        {
    	    ucData = 0xD6;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO weak signal mode    
            ucData = 0xED;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
    	    ucData = 0xA6;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG3, &ucData, 1); 
            ucData = 0x2D;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG4, &ucData, 1);
    	    mcSHOW_DBG_MSG2(("DRO in weak signal mode!\n"));      	    				
    	}  
    	else if (ucNoiselevel < 0x04 )  // signal > 45dBuV
        {	                                    				
            if ( u2GainJitter > 0x2500 )  // larger gain_jitter: AM-Fv case
            {
                //default 0xDC, GA FT issue 0xAC 
                ucData = 0xAC;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO sync. mode, fast W_ADC   
                ucData = 0xC0;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1); //fast ref_0,ref_1 leaky
                mcSHOW_DBG_MSG2(("DRO in AM-Fv mode!\n"));                                
            }
            else if ( u2GainJitter < 0x1400 ) // non AM-Fv case
            {
                /*if ( u2SyncHeight < (u2SigSwing >> 3) )       // Low-sync. case
                {
    	            ucData = 0xD4;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO async. mode, slow W_ADC  
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
    	            mcSHOW_DBG_MSG2(("DRO in low-sync mode!\n"));      	                	       
                }
                else if ( u2SyncHeight > (u2SigSwing >> 2) )*/   // non Low-sync. case
                {
                    //default 0xD4, GA FT issue 0xA4
    	            ucData = 0xA4;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_LP_CFG01, &ucData, 1); //DRO async. mode, slow W_ADC
                    ucData = 0xD2;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
    	            mcSHOW_DBG_MSG2(("DRO in normal mode!\n"));    	                	            
                }	
            }
            ucData = 0xA5;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG3, &ucData, 1); 
            ucData = 0x12;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG4, &ucData, 1);
        }                                                       		
    	break;
    	case MOD_SECAM_L:
    	case MOD_SECAM_L1:    		        
        if((ucIFgain >= 0x20) & (ucIFgain <= 0x7F)) // Weak Signal        
        {
    	    ucData = 0x2D;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
    	    ucData = 0x2D;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG4, &ucData, 1);
    	    mcSHOW_DBG_MSG2(("DRO in weak signal mode!\n"));       	    
        }
    	else if((ucIFgain <= 0x10) | (ucIFgain >= 0x80))
        {
    	    ucData = 0x1B;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_REF_COEF, &ucData, 1);
    	    ucData = 0x1B;  ATD_SetReg(psDemodCtx, REG_ATD_DRO_CCI_CFG4, &ucData, 1);
    	    mcSHOW_DBG_MSG2(("DRO in normal mode!\n"));    	        	    
    	}    	
    	break;
    }
}
#else//d110927_Haibo:If not support sync mode,can not apply the patch
STATIC void ATD_Patch_DRO(ATD_CTX_T *psDemodCtx)
{

}
#endif

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
   ATD_GetReg(psDemodCtx, (REG_ATD_PRA_IF_MIN + 0x800), &ucData1, 1);
   ATD_GetReg(psDemodCtx, (REG_ATD_PRA_RF_MAX + 0x800), &ucData2, 1);
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
       else if((ucDataIf == IFAgcMIN)&&((ucDataRf <= REG_ATD_DRO_WADC_1f)||(ucDataRf > RFAgcMAX))&&(Lna))  //make sure Signal level > 70dBv LNA off
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

    //ATD_SetNormalMode(psDemodCtx); //EC_code_d9922: [DTV00140179] turn-on CR pilot stability and quality auto re-acq  //EC_code: Disable it in monitor thread
    
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

            ATD_SetAutoConfig(psDemodCtx, u1CrAutoConfig);
            ATD_SetCrBw(psDemodCtx, u1CrBandwidth);
//Add LNA to  monitor process			
#ifdef   LNA_MONITOR
		    ATD_Patch_LNA(psDemodCtx);
#endif
            ATD_Patch_DRO_AGC(psDemodCtx);


			
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
    ATD_SetReg(psDemodCtx, REG_ATD_EQ_CFG, &ucData, 1);
    ucData= 0x00;  // Set tap index to 0
    ATD_SetReg(psDemodCtx, REG_ATD_EQ_WIDX, &ucData, 1);
    for (ii=0; ii<103; ii++)
    {
        ATD_SetReg(psDemodCtx, REG_ATD_EQ_WMSB, (pucAtdEQScript+ii*2), 1);
        ATD_SetReg(psDemodCtx, REG_ATD_EQ_WLSB, (pucAtdEQScript+ii*2+1), 1);
    }
    mcSHOW_DBG_MSG(("Finish EQ coefficient dump!!\n"));

    ucData= 0x81;  // Enable EQ
    ATD_SetReg(psDemodCtx, REG_ATD_EQ_CFG, &ucData, 1);
  
}
void ATD_GetEQ(ATD_CTX_T *psDemodCtx)
{
  UINT8 ucData,ucDataArr[2] = {0,0},ii;
 
  ATD_GetReg(psDemodCtx, REG_ATD_EQ_CFG, &ucData, 1);
  ucData|=0x40;//auto increase weight index for reading weight
  ATD_SetReg(psDemodCtx, REG_ATD_EQ_CFG, &ucData, 1);
  ucData=0x00;//initalize weight index 
  ATD_SetReg(psDemodCtx, REG_ATD_EQ_WIDX, &ucData, 1);
  
   for (ii=0; ii<103; ii++)
   {
       ATD_GetReg(psDemodCtx, REG_ATD_EQ_WMSB, ucDataArr, 2);
      mcSHOW_USER_MSG(("nim.atd w 0x%03X 0x%02X\n",REG_ATD_EQ_WMSB,ucDataArr[0]));
	  mcSHOW_USER_MSG(("nim.atd w 0x%03X 0x%02X\n",REG_ATD_EQ_WLSB,ucDataArr[1]));
   } 
 }
void ATD_TunerReset(ATD_CTX_T *psDemodCtx)
{
   UINT8   ucData1;
   ITuner_OP(psDemodCtx->pTCtx, resettuner, 0, &ucData1); 
}

void ATD_SetEQ(ATD_CTX_T *psDemodCtx, ITUNEROP_T EqScriptType)
{
    UINT8* pucAtdEQScript = NULL;
    UINT8 u1EqScriptSysIdx = psDemodCtx->u1SubSysIdx;
     // get EQ script
    ITuner_OP(psDemodCtx->pTCtx, EqScriptType, u1EqScriptSysIdx-MOD_ANA_TYPE_BEGIN, &pucAtdEQScript);

    if (pucAtdEQScript != NULL) {
        ATD_LoadEQCoef(psDemodCtx, (pucAtdEQScript));
    }
}
void ATD_SetTVDReChkCfg(ATD_CTX_T *psDemodCtx)
{
   UINT8 u1PnFnSts=0x00;
   UINT8 u1FnBw,coef_a,coef_b;
   UINT8 u1CrAutoCfg=0;
   UINT8 u1Data;
   
   u1PnFnSts=ATD_GetPnFn(psDemodCtx);
   ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &u1CrAutoCfg, 1); //PN-FN auto config
   ATD_GetReg(psDemodCtx, REG_ATD_PE_CFG0,&u1Data,1);
   
   mcSHOW_DBG_MSG(("Null region is 0x%02X\n ",u1Data));
  
   if((u1PnFnSts&MSK_ATD_CR_STATE1_PNIND) && (u1CrAutoCfg&MSK_ATD_ACR_CFG_PN_ACFG_EN))//d111107_Haibo:PN indicator ON but TVD unlock.Disable PN auto config
   {
     mcSHOW_DBG_MSG(("PN IND ON.Disable Auto Config and reset null region to 2\n"));
     ATD_SetAutoConfig(psDemodCtx,0x10);//disable PN auto config
	 ATD_SetNullRegion(psDemodCtx,2);//d111108_Haibo:reset null region to avoid tvd unlock caused by ovm 
   }
   else if((u1PnFnSts&MSK_ATD_CR_STATE1_FNIND) && (u1CrAutoCfg & MSK_ATD_ACR_CFG_FN_ACFG_EN))//d111107_Haibo:FN only case.Set normal FIFO as FN FIFO
   {
      mcSHOW_DBG_MSG(("FN only case.Correct the FIFO\n"));
      ATD_GetReg(psDemodCtx, REG_ATD_ACR_FN_CFG1, &u1FnBw, 1); 
      coef_a = mcGET_FIELD(u1FnBw, 0xF0, 4);
      coef_b = mcGET_FIELD(u1FnBw, 0x0F, 0);
	  ATD_SetCrFIFO(psDemodCtx,coef_a,coef_b);
   }
}

/**********************************************************************/
/*                  Query APIs                                        */
/**********************************************************************/
VOID ATD_GetInfo(ATD_CTX_T *psDemodCtx)
{
    S32     s2Data;
	UINT8 ucData1 = 0,ucData2 = 0;
	UINT16 u2Data;
	//==================================PRA========================================        
                mcSHOW_USER_MSG(("RF gain = 0x%04X\n", ATD_GetRFAGC(psDemodCtx)));
                mcSHOW_USER_MSG(("IF gain = 0x%04X\n", ATDPI_GetIFAGC(psDemodCtx)));
				ATD_GetReg(psDemodCtx, REG_ATD_PRA_ST, &ucData1, 1);
                mcSHOW_USER_MSG(("0x15E = 0x%02X\n", ucData1));
    //==================================CR=======================================
                s2Data = ATD_GetCFOKHz(psDemodCtx, FALSE);
                mcSHOW_USER_MSG(("CFO = %d KHz\n", s2Data));
                ATD_GetReg(psDemodCtx, REG_ATD_CR_CPO0, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_CR_CPO1, &ucData2, 1);
                s2Data = (S32)mcSIGN_2BYTE(ucData1, ucData2, 5, 8);
                s2Data = (S32)s2Data * 439 / 100000;
                mcSHOW_USER_MSG(("CPO = %d degree\n", s2Data));
                ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucData1, 1);                
                mcSHOW_USER_MSG(("OVM idx = %d\n", (ucData1 & 0x70) >> 4 ));
				mcSHOW_USER_MSG(("OVM IND=%d\n",ATD_GetOVMIdx(psDemodCtx)));
                mcSHOW_USER_MSG(("0x%03X = 0x%02X\n", REG_ATD_CR_STATE0,ucData1));
                ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData1, 1);
                mcSHOW_USER_MSG(("0x%03X = 0x%02X\n", REG_ATD_CR_STATE1,ucData1));
                mcSHOW_USER_MSG(("PN indicator = %d\n", (ucData1 & 0x08) >> 3 ));
                mcSHOW_USER_MSG(("FN indicator = %d\n", (ucData1 & 0x04) >> 2 ));
                ATD_GetReg(psDemodCtx, REG_ATD_CMETRIC0, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_CMETRIC1, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("CR CMetrix = 0x%04X\n", u2Data));
              
//==================================DRO=======================================
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_WADC_1, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_WADC_2, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO gain = 0x%04X\n", u2Data));
				ucData1 = 0x2E;
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_2, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO gain jitter = 0x%04X\n", u2Data));
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MAX1, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MAX2, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO signal swing = 0x%04X\n", u2Data));
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MIN1, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_LSYNC_MIN2, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO sync height = 0x%04X\n", u2Data));                                
                ucData1 = 0x2C;
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_2, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO sync height jitter = 0x%04X\n", u2Data));
                ucData1 = 0x2D;
                ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &ucData1, 1);
                ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_2, &ucData2, 1);
                u2Data = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
                mcSHOW_USER_MSG(("DRO blank jitter = 0x%04X\n", u2Data));
//==================================Noise level========================================       				
                ucData1 = IO_READ8(VIDEO_IN0_BASE, 0x80);
                mcSHOW_USER_MSG(("TVD noise level = %d\n", ucData1));
                mcSHOW_USER_MSG(("ATD noise level = 0x%08X\n", ATDPI_GetNoiseLevel(psDemodCtx)));
}
INT32 ATD_GetCFOKHz(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
{
    UINT8 ucData[2];
    S32  CFO_Tmp;

    ATD_GetReg(psDemodCtx, REG_ATD_CR_CFO0, &ucData[0], 1);
    ATD_GetReg(psDemodCtx, REG_ATD_CR_CFO1, &ucData[1], 1);

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

    /*ATD_GetReg(psDemodCtx, ATD_REG_ATD_VOP_HW_CPOH, &ucData_pre, 1);
    for(ii=0;ii<10;ii++)
    {
    	mcDELAY_MS(1);*/
        ATD_GetReg(psDemodCtx, REG_ATD_VOP_RESERVE0, &ucData, 1);
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

    ATD_GetReg(psDemodCtx, REG_ATD_PRA_ST, &ucData, 1);
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
#elif 0
    
    ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucData, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData2, 1);
    
    if(fgLogMessage)
    {
        mcSHOW_DBG_MSG(("Pilot Strength = 0x%02x\n", (ucData & 0x0C) >> 2  ));
        mcSHOW_DBG_MSG(("Pilot Stability = 0x%02x\n", (ucData2 & 0x20) >> 5 ));
        mcSHOW_DBG_MSG(("Pilot Quality = 0x%02x\n", (ucData2 & 0x10) >> 4 ));
    }

    if( ((ucData & 0x0C) != 0x00) || ((ucData2 & 0x30) == 0x30) ) //&& ((ucData2 & 0x33) == 0x03) )

#elif 0

    UINT8 ucData, ucData2, ucData3, ucData4, ucData5;
    UINT32 u4PilotPower;
    ATD_GetReg(psDemodCtx, 0x9A0, &ucData, 1);
    ATD_GetReg(psDemodCtx, 0x9A1, &ucData2, 1);
    ATD_GetReg(psDemodCtx, 0x9BC, &ucData3, 1);
    ATD_GetReg(psDemodCtx, 0x9BD, &ucData4, 1);
    ATD_GetReg(psDemodCtx, 0x9BE, &ucData5, 1);
    u4PilotPower = mcUNSIGN_2BYTE(ucData3, ucData4, 8, 8);
    u4PilotPower = (u4PilotPower << 8) + ucData5;        
    if(fgLogMessage)    
    {        
        mcSHOW_DBG_MSG(("Pilot Strength = 0x%02x\n", (ucData & 0x0C) >> 2  ));
        mcSHOW_DBG_MSG(("Pilot Stability = 0x%02x\n", (ucData2 & 0x20) >> 5 ));
        mcSHOW_DBG_MSG(("Pilot Quality = 0x%02x\n", (ucData2 & 0x10) >> 4 ));
        mcSHOW_DBG_MSG(("Pilot Power = 0x%08x\n", u4PilotPower));
    }    

    if( ((ucData & 0x0C) != 0x00) || ((ucData2 & 0x30) == 0x30) || (u4PilotPower >= 0x00000100) ) //&& ((ucData2 & 0x33) == 0x03) )
    {
    
#endif
    UINT8 ii,ucData, ucData1, ucData2, ucData3[3],OvmIdx;
    UINT16 u2ADCValue;
    UINT32 u4PilotPower, u4SignalPower, u2DroGain;
    UINT8 u1Fref2;
    UINT8 ucCdcSts, ucAudFilter;	// Used to check the SSIF CDC status
	BOOL fgACI;
    
    ATD_GetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1);
	ucData &= 0xf0;
    ucData |= 0x01;
	ATD_SetReg(psDemodCtx, REG_ATD_DRO_PROBE_CFG, &ucData, 1);
	ATD_GetReg(psDemodCtx, REG_ATD_DRO_RPOBE_1, &u1Fref2, 1);

	if (u1Fref2<=0x08) 
	{
	    fgACI=1;
        ATD_SetNullRegion(psDemodCtx,1);//d111108_Haibo:set null region
        OvmIdx=ATD_GetOVMIdx(psDemodCtx);
        if(OvmIdx==7)
        {
		    mcSHOW_DBG_MSG(("OvmIdx=%d\n",OvmIdx));
		    ATD_SetCrBwAutoCfg(psDemodCtx,TRUE);//d111108_Haibo:Enable Pilot filter BW auto config when strong ACI case
            if(psDemodCtx->pTCtx->fgRFTuner)
            {
                u2ADCValue = ATD_GetADCValue(psDemodCtx);
                if(u2ADCValue >= 0x0050)
                {
                    for(ii=0x00;ii<g_CR_BW_AutoConfig_Delay_cnt;ii++)
                    {
                        u4PilotPower=ATD_GetPilotPower(psDemodCtx);     
        				mcDELAY_MS(10);
                    } 
                }
            }
            psDemodCtx->pTCtx->u2RetryTVDTm=100;			
        }
        else
        {
            ATD_SetCrBwAutoCfg(psDemodCtx,FALSE);
		}
	}
	else
	{
	    fgACI=0;
        ATD_SetCrBwAutoCfg(psDemodCtx,FALSE);
		ATD_SetNullRegion(psDemodCtx,2);
	}
	if ((psDemodCtx->u1SubSysIdx == MOD_SECAM_L)||(psDemodCtx->u1SubSysIdx == MOD_SECAM_L1))
    {
         fgACI=1;    //SECAM L can not judgment due to the DAGC bypassing
	}

    ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE0, &ucData1, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_CR_STATE1, &ucData2, 1);


    u4PilotPower=ATD_GetPilotPower(psDemodCtx);  

    ucData = (0x07 << 3); // Select ADVPS_PROB as SignalPower by setting 0x9BB[b5:b3] 0x07
    ATD_SetReg(psDemodCtx, REG_ATD_ADVPS_CFG2 , &ucData, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_ADVPS_PRB0, ucData3, 3);            
    u4SignalPower = mcUNSIGN_2BYTE(ucData3[0], ucData3[1], 8, 8);
    u4SignalPower = (u4SignalPower << 8) + ucData3[2];

    ATD_GetReg(psDemodCtx, REG_ATD_DRO_WADC_1, ucData3, 2);    
    u2DroGain = mcUNSIGN_2BYTE(ucData3[0], ucData3[1], 8, 8);
               
    if(fgLogMessage)
    {
        mcSHOW_DBG_MSG(("Pilot Strength = 0x%02x\n", (ucData1 & 0x0C) >> 2  ));
        mcSHOW_DBG_MSG(("Pilot Stability = 0x%02x\n", (ucData2 & 0x20) >> 5 ));
        mcSHOW_DBG_MSG(("Pilot Quality = 0x%02x\n", (ucData2 & 0x10) >> 4 ));
        mcSHOW_DBG_MSG(("Pilot Power = %08d\n", u4PilotPower));
        mcSHOW_DBG_MSG(("Signal Power = %08d\n", u4SignalPower));
        mcSHOW_DBG_MSG(("DRO Gain = %08d\n", u2DroGain));
		mcSHOW_DBG_MSG(("Fref2=0x%02X\n",u1Fref2));
    }

#if ((defined CC_MT5365) || (defined CC_MT5395))
    if((ucData2 & 0x30) != 0) // pilot stability or pilot quality
#elif ((defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
    if( ((ucData1 & 0x0C) != 0x00) || ((ucData2 & 0x30) == 0x30) )
#else
    if(((ucData1 & 0x0C) != 0x00) || ((ucData2 & 0x30) == 0x30))// || (u4PilotPower >= 0x00000100) ) //&& ((ucData2 & 0x33) == 0x03) )        
#endif           
    {
        psDemodCtx->fgCRLock = TRUE;
    }
	else if((fgACI)&&((ucData2&0x20) == 0x20)) //for ACI case
	{
        psDemodCtx->fgCRLock = TRUE;
		mcSHOW_DBG_MSG2(("VIF lock modified by ACI case!\n"));
	}
    else
    {
        mcSHOW_DBG_MSG(("Bad pilot quality!\n"));
        psDemodCtx->fgCRLock = FALSE;
    }

    if(psDemodCtx->fgCRLock)
    { //CR Lock, check CDC status, if CDC status is unsync, toggle the audio filter bypass
        ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG3, &ucCdcSts, 1);
        if((ucCdcSts & 0x03) != 0x03)
        {  // Audio filter path filtering reset
            ATD_GetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucAudFilter, 1);
            ucAudFilter &= 0xF7;
            ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucAudFilter, 1);
            mcDELAY_MS(1);   // delay 1ms
            ucAudFilter |= 0x08;
            ATD_SetReg(psDemodCtx, REG_ATD_AVS_VOL, &ucAudFilter, 1);
            mcSHOW_DBG_MSG(("SSIF CDC unsync, reset the audio filter!\n"));
        }
    }

    return (psDemodCtx->fgCRLock);
}

UINT8 ATD_GetVIFLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
{
   // meirui,20100119,show RF and IF gain when channel  scan

    INT16 s2FullVIFLockRange;
	UINT16  u2LOStep;

    u2LOStep=psDemodCtx->pTCtx->u2LO_Step;	
	s2FullVIFLockRange=(INT16)(u2LOStep/100);
	if(s2FullVIFLockRange<cATD_VIF_FULL_LOCK_MIN_RANGE)
		s2FullVIFLockRange=cATD_VIF_FULL_LOCK_MIN_RANGE;
   
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
    	if( (psDemodCtx->s4Cfo > (-s2FullVIFLockRange)) && 
            (psDemodCtx->s4Cfo <   s2FullVIFLockRange)   )
    	{
    	    psDemodCtx->u1VIFLock = cATD_VIF_FULL_LOCK;
            #if !TVD_SUPPORT_SNOW_MODE
            mcSHOW_DBG_MSG3(("Snow Mode Off: VIF full lock\n"));
            ATD_SetSnowMode(psDemodCtx, FALSE);  // turn off snow mode when VIF lock
    	    #endif
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
    	
    UINT8 ucData1 = 0, ucData2 = 0;
    UINT16 u2RFAGC;
#if defined(CC_MT5365) || defined(CC_MT5395)  || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_20, &ucData1, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_21, &ucData2, 1);
#else
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_1, &ucData1, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_RF_CTL_2, &ucData2, 1);
 #endif
    
    u2RFAGC = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
    mcSHOW_DBG_MSG(("RF gain = 0x%04X\n", u2RFAGC));	
    return u2RFAGC;
}

UINT32 ATDPI_GetNoiseLevel(ATD_CTX_T *psDemodCtx)
{
    	
    UINT8 ucData1, ucData2, ucData3;
    UINT32 u4NoiseLevel;

    ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_0, &ucData1, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_1, &ucData2, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_CVBS_SNR_2, &ucData3, 1);

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
    #if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
	if(psDemodCtx->pTCtx->fgRFTuner)
	{
	  ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_30, &ucData1, 1);
      ucData2=0x00;	  
	}
	else
	{
	  ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_22, &ucData1, 1);
	  ATD_GetReg(psDemodCtx, REG_ATD_SLD_PRA_23, &ucData2, 1);
	}
	#else
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_1, &ucData1, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_PRA_IF_CTL_2, &ucData2, 1);
    #endif
    u2IFAGC = mcUNSIGN_2BYTE(ucData1, ucData2, 8, 8);
    	
    mcSHOW_DBG_MSG(("IF gain = 0x%04X\n", u2IFAGC));
    	
    return u2IFAGC;
}
UINT8 ATD_GetPnFn(ATD_CTX_T *psDemodCtx)
{
    UINT8 u1CRSts1;
	
    ATD_GetReg(psDemodCtx,REG_ATD_CR_STATE1, &u1CRSts1, 1);
	return (u1CRSts1 &(MSK_ATD_CR_STATE1_PNIND|MSK_ATD_CR_STATE1_FNIND));  
}
UINT32 ATD_GetPilotPower(ATD_CTX_T * psDemodCtx)
{
   UINT32 u4PilotPower;
   UINT8 ucData,ucData3[3];
   
    ucData = (0x06 << 3); // Select ADVPS_PROB as PilotPower by setting 0x9BB[b5:b3] to 0x06
    ATD_SetReg(psDemodCtx, REG_ATD_ADVPS_CFG2 , &ucData, 1);
    ATD_GetReg(psDemodCtx, REG_ATD_ADVPS_PRB0, ucData3, 3);    
    u4PilotPower = mcUNSIGN_2BYTE(ucData3[0], ucData3[1], 8, 8);
    u4PilotPower = (u4PilotPower << 8) + ucData3[2];
	mcSHOW_DBG_MSG(("Pilot Power=0x%06X\n",u4PilotPower));
	return u4PilotPower;
}    

UINT16 ATD_GetADCValue(ATD_CTX_T *psDemodCtx)
{
    UINT8  ucPRAProbe_Sel,ucADCValue[2];
    UINT16 u2ADCValue;
    ATD_GetReg(psDemodCtx,REG_ATD_SLD_PRA_35,&ucPRAProbe_Sel,1);
    ucPRAProbe_Sel &= 0xE0;
    ucPRAProbe_Sel |= 0x03;//d111230_hongji:Select PRA PROBE SELECT as ADC Value by setting 0xFE5[4:0] to 0x03
    ATD_SetReg(psDemodCtx,REG_ATD_SLD_PRA_35,&ucPRAProbe_Sel,1);
    ATD_GetReg(psDemodCtx,REG_ATD_SLD_PRA_36,ucADCValue,2);
    u2ADCValue = mcUNSIGN_2BYTE(ucADCValue[0],ucADCValue[1],8,8);
    mcSHOW_DBG_MSG(("Current ADC Value is 0x%x \n",u2ADCValue));
    return u2ADCValue;
}

void ATD_SetNullRegion(ATD_CTX_T *psDemodCtx, UINT8 u1NR)
{
     UINT8 ucData;
	 
     ATD_GetReg(psDemodCtx, REG_ATD_PE_CFG0, &ucData, 1);
	 ucData&=0x8F;
	 ucData|=(u1NR<<4);
	 ATD_SetReg(psDemodCtx, REG_ATD_PE_CFG0, &ucData, 1);	 
}
void ATD_SetCrBwAutoCfg(ATD_CTX_T *psDemodCtx,BOOL fgEnable)
{
    UINT8 ucData;
    	
     ATD_GetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
	 ucData&=0xF7;
	 ucData|=((FALSE==fgEnable)<<3);
	 ATD_SetReg(psDemodCtx, REG_ATD_ACR_CFG, &ucData, 1);
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

BOOL ATDPI_SetAntIn(ATD_CTX_T *psDemodCtx, UINT8 fgAntIn)
{

    BOOL fgReturn;
    UINT8 ucData1;
    
    mcSHOW_DBG_MSG(("Set LNA = %d\n", fgAntIn));
    
   fgReturn = ITuner_OP(psDemodCtx->pTCtx, itSetAntIn, fgAntIn, &ucData1);
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
    ATD_SetTVDReChkCfg(psDemodCtx);//d111107_Haibo:Apply TVD re-check setting
	
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

void SetFMRadio(ATD_CTX_T *psDemodCtx, UINT32  i4CurFreq)
    {
    UINT8 ucData1;
    //UINT32 u4Temp;

    //u4Temp = 0xffffffff;
    //IO_WRITE32(cRISC_TVD_BASE, 0x40C, u4Temp);
    //ATD_ChipInit(psAtdnaDemodCtx);
    //ATD_SetSystem(psAtdnaDemodCtx, MOD_PAL_BG);
    psDemodCtx->u1PF = 0x00;
    //ucData1 = 0x23;
    //ATD_SetReg(psAtdnaDemodCtx, REG_ATD_CTL_SYS1, &ucData1, 1);
    ucData1 = 0x6D;
    ATD_SetReg(psDemodCtx, REG_ATD_PJ_CFG, &ucData1, 1);
    ucData1 = 0xC9;
    ATD_SetReg(psDemodCtx, REG_ATD_PFLL_CFG, &ucData1, 1);
    ucData1 = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_CPR_CFO_CFG, &ucData1, 1);
    ucData1 = 0x00;
    ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOH, &ucData1, 1);
    ucData1 = 0x20;
    ATD_SetReg(psDemodCtx, REG_ATD_AVS_AOL, &ucData1, 1);
    ucData1 = 0x81;
    ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData1, 1);
    ucData1 = 0xA1;
    ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData1, 1);

	#if ((defined CC_MT8223) || (defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368)|| defined(CC_MT5389))
	    //LW_code_d100929; FM issuep;START
    	ATD_GetReg(psDemodCtx, REG_ATD_ATD_DC_CFG3, &ucData1, 1);
	    mcSHOW_DBG_MSG(("LW:beforeREG_ATD_ATD_DC_CFG3_ucData1      =  %d\n", ucData1));
	    ucData1 += cIfCtrlWordOffset;
	    mcSHOW_DBG_MSG(("LW:after REG_ATD_ATD_DC_CFG3_ucData1      =  %d\n", ucData1));
	    ATD_SetReg(psDemodCtx, REG_ATD_ATD_DC_CFG3, &ucData1, 1);
	
	    ucData1 = 0x81;
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG1, &ucData1, 1);
        ucData1 = 0x83;
        ATD_SetReg(psDemodCtx, REG_ATD_AUC_CFG2, &ucData1, 1);
	
	    ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG6, &ucData1, 1);	//NSF Enable
	    ucData1 &= 0x8F;
	    ucData1 |= 0x70;
	    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG6, &ucData1, 1);

	    ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG5, &ucData1, 1);	//ACF disable
	    ucData1 &= 0x80;
	    ucData1 |= 0x00;
	    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG5, &ucData1, 1);
	
	    ATD_GetReg(psDemodCtx, REG_ATD_TVDIF_CFG2, &ucData1, 1);	//NSF centeral frequency ~ 5.3MHz
	    ucData1 &= 0xE0;
	    ucData1 |= 0x09;
	    ATD_SetReg(psDemodCtx, REG_ATD_TVDIF_CFG2, &ucData1, 1);
	
	    ATD_GetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData1, 1);
	    ucData1 &= 0x0F;
	    ucData1 |= 0x40;
	    ATD_SetReg(psDemodCtx, REG_ATD_AVS_RFA0, &ucData1, 1);    //AVS BW
	//LW_code_d100929; FM issue; END
	#endif

    //SetBreakMonitor(psAtdPdCtx);
    //ATD_SetLNA(psAtdeuDemodCtx, TRUE);
    ITuner_OP(psDemodCtx->pTCtx, itSetLNA, 0, &ucData1);
    //ATD_SetFreq(psAtdeuDemodCtx, i4CurFreq, FALSE);
    //ATD_StartAcq(psAtdeuDemodCtx, FALSE);
    mcSHOW_DBG_MSG((" Enable ATD CR bypass mode for FM radio\n")); 

}


//move nim.c and nim.o to the function,xiongfeng120808 for ESD issue;
INT32 ATD_Recover(ATD_CTX_T *psDemodCtx)
{
   	UINT8   ucData1;
	ITuner_OP(psDemodCtx->pTCtx, resettuner, 0, &ucData1); 
    return (DRVAPI_TUNER_OK);
}
//for fix black screen issue,xiongfeng120808 for ESD issue;
INT32 ATD_Recover1(ATD_CTX_T *psDemodCtx,UINT32 *wFreqInKHz)
{
    ATD_SetFreq(psDemodCtx, wFreqInKHz, 0);   
	ATD_ChipInit(psDemodCtx);
    ATD_SetSystem(psDemodCtx, psDemodCtx->u1SubSysIdx);
    return (DRVAPI_TUNER_OK);
}

//for fix black screen issue,xiongfeng120808 for ESD issue;
UINT32 ATD_CheckReg(void)
{
    if(IO_READ32MSK(0xf0061000,0x400,0x00000001)==0x00000001)
		return TRUE;
	else
		return FALSE;
}

