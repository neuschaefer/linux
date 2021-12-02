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
//#include "pi_atdeu_if.h"


#include "pi_demod_atd.h"
//#include "com_defs.h"
#include "pi_def_atd.h"
//#include "pd_common.h"
#include "PD_Def_dbg.h"  /*need include RISC IF macro, if re-ware RISC IF , remove this*/
#include "drvcust_if.h"
#include "drv_tvd.h"
#include "pi_demod_atdna.h"

//ITuner
#include "tuner_interface_if.h"

//New RISC IF
//#include "ctrl_bus.h"

/***********************************************************************/
/*                  Defines                                            */
/***********************************************************************/

//#define fcADD_REG_RESET         1
//#define fcCHK_TUNER_PLL         0
#define fcUP_50M_CLK            1
//#define fcPLL_POLLING_TIME      10          //  in ms
#define fcMEASURE_DRIVER_TIME   0
#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)

/***************** Debug Only *********************/
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcADD_HOSTCMD_CHKSUM    0

//-----------------------------------------------------------------------------
// TVD lock parameter definition
//-----------------------------------------------------------------------------
#define CHN_LOCK_VLOCK          0
#define CHN_LOCK_HSYNC_COUNTER  0
#define CHN_LOCK_VPRES          1

#if (CHN_LOCK_HSYNC_COUNTER || CHN_LOCK_VPRES)
//    #if CONFIG_MT8202B
     #ifdef CONFIG_MT8202B //fix warning zero used for undefined preprocessing identifier
#define CHN_LOCK_CHECK_TIME     0x50        // Good for EU searching.  Modified by Benson 05.10.17
     #else
#define CHN_LOCK_CHECK_TIME     0x30        // 8202B vpres needs more time.  Modified by Benson 06.01.04
     #endif
#elif CHN_LOCK_VLOCK
#define CHN_LOCK_CHECK_TIME     100
#else
#define CHN_LOCK_CHECK_TIME     0
#endif


//-----------------------------------------------------------------------------
// RISC IF Related Implementation
//-----------------------------------------------------------------------------
//For New RISC IF 
//STATIC PARAM_RISC_T sATDRiscCtx;

#define cRISC_ATD_BASE           (IO_VIRT + 0x2A000)
#define cRISC_DVBT_BASE          (IO_VIRT + 0x2B000)
#define cRISC_AUD_BASE           (IO_VIRT + 0x05000)
#define cRISC_TVD_BASE           (IO_VIRT + 0x22000)
#define cRISC_DAC_BASE           (IO_VIRT + 0x2D000)

//Pluto
//volatile UINT32 *pu4ATD_Reg = (volatile UINT32 *) cRISC_ATD_BASE;
//volatile UINT32 *puATD_Reg = (volatile UINT32 *) cRISC_ATD_BASE;
//volatile UINT32 *puATD_Reg = (volatile UINT32 *) 0x2002B000;

#define ATD_RISC_IF            1
#define fcRISCIF_ATD_DIRECT_RW 1


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define mcSET_RISCIF_ACT(enb)                                         \
{                                                                   \
    mcSET_BIT(u4RegSetting, cpRISCIF_CTRL_##enb##_EN);                \
    mcRISC_Reg(puATD_Reg, ATD_REG_RISCIF_CTRL) = u4RegSetting;    \
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
        u4RegSetting = mcRISC_Reg(puATD_Reg, ATD_REG_RISCIF_STAT); \
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
    u4RegValue = mcRISC_Reg(puATD_Reg, ATD_REG_RISCIF_RDATA);     \
    mcRISC_ENDIAN_READ(pu1Buffer, u1ByteOfst, u2CurByteCount);      \
}\


#define mcRISCIF_DATA_WRITE(pu1Buffer, u1ByteOfst)                    \
{                                                                   \
    mcRISC_ENDIAN_WRITE(pu1Buffer, u1ByteOfst, u2CurByteCount);     \
    mcRISC_Reg(puATD_Reg, ATD_REG_RISCIF_WDATA) = u4RegValue;     \
}\


#define mcRISCIF_DATA_WRITE_ALGN(pu1Buffer)   \
    (mcRISC_Reg(puATD_Reg, ATD_REG_RISCIF_WDATA) = *((UINT32 *) (pu1Buffer)))


#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_USER_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\

#define RegWrite8(Addr, Value)    \
{                                 \
    ucData = Value; ATD_SetReg(psDemodCtx, (Addr+0x800), &ucData, 1);  \
}


/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/
#if 0
static UINT8 u1I2cDemWriteATD(ATD_CTX_T *psDemodCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
static UINT8 u1I2cDemReadATD(ATD_CTX_T *psDemodCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
#endif

UINT8 GetATDReg(UINT16 Addr);
void  SetATDReg(UINT16 Addr,UINT8 Value);


/***********************************************************************/
/*                  Private Types                                      */
/***********************************************************************/

/***********************************************************************/
/*                  Global variables                                   */
EXTERN BOOL fgEnableATP;
/***********************************************************************/
UINT32  u2TickStart_ATDLayer;
#if defined(CC_SONY_GA)
STATIC UINT32 wPreLockFreq = 900000;
STATIC UINT32 u4FreqTemp = 900000;
#endif

UINT8 ATDNA_SearchNearbyFreq(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct,
                                 void *pvArg, UINT32 u4FreqBDKHz)
{

    UINT32  wFRTmp, s4NextFreqStep;
    UINT8   bScanState = 0;
    UINT8   ii,iii;
    //UINT8   fgScanDir;
    //INT32   s4NextFreqStep;
    UINT8   bATVStatus = 0;
    BOOL fgFirstCheck = FALSE;  
    long TimingOffset;
    //UINT8 SkipTmp;
    UINT16  u2LOStep;

    u2LOStep=psDemodCtx->pTCtx->specific_member.us_ctx.u2LO_Step;
    psDemodCtx->isBreak = _BreakFct;
    
    wFRTmp = *wFreqInKHz;
    
    mcSHOW_DBG_MSG(("[ATuner] SearchNearBy: Freq = %d kHz \n", wFRTmp));

    u2TickStart_ATDLayer = mcGET_SYS_TICK();
    TimingOffset = (mcGET_SYS_TICK() - u2TickStart_ATDLayer) * mcGET_TICK_PERIOD();
   
    mcDELAY_MS(10);
    
    ATDNA_GetVIFLock(psDemodCtx,TRUE);   

    mcSHOW_HW_MSG(("[ATuner] VIF Lock : %d\n",psDemodCtx->u1VIFLock));
    
  // Initial check to see if curent freq need full range AFT or not
    if ((psDemodCtx->u1VIFLock >= 2))
    {
        //if (fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME)) //5 : NTSC
        if (fgGetTVDSts(psDemodCtx,CHN_LOCK_CHECK_TIME)) //For MT5387 TVD export fucntion        
        {
            TimingOffset = (mcGET_SYS_TICK() - u2TickStart_ATDLayer) * mcGET_TICK_PERIOD() - TimingOffset;
            mcSHOW_DBG_MSG(("[ATuner] End of SearchNearby = %3u ms\n",TimingOffset));
            mcMSG_IN_SEARCHNEARBY_AUTO_TEST(u2LOStep)
            return 3;
        }       
    else
    {
            mcSHOW_DBG_MSG(("VIF lock, but TVD unlock!\n"));
            fgFirstCheck = FALSE;            
        }       
    }    	
    else if ((psDemodCtx->u1VIFLock == 1))
    {
        fgFirstCheck = TRUE;
        
        psDemodCtx->u1PF = 0x00;
        psDemodCtx->fgCheckCR = FALSE;
        s4NextFreqStep = (psDemodCtx->s4Cfo*1000)/50000;
        s4NextFreqStep *= (50000/1000);
                
        wFRTmp += s4NextFreqStep;
        bScanState = TUNER_PLL_LOCK_STATE;
    }
    else
    {        
        fgFirstCheck = FALSE;        
    }

  for(ii=0;ii<3;ii++)  //max 3 trial per searchnearby
  {

     if (psDemodCtx->isBreak(pvArg))
     {
        	mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
        	mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
        	return bATVStatus;
     }
      
     if((ii==0) && (!fgFirstCheck))
     {
     	ii++;
     }
     	 
     if (ii == 1)
     {
    		psDemodCtx->u1PF = 0x01;
                psDemodCtx->fgCheckCR = TRUE;
    		wFRTmp = *wFreqInKHz - 1800;
    		bScanState = TUNER_PLL_LOCK_STATE;
     }
     else if (ii == 2)
     {
      		psDemodCtx->u1PF = 0x01;
                psDemodCtx->fgCheckCR = TRUE;
      		wFRTmp = *wFreqInKHz + 1800;
     		bScanState = TUNER_PLL_LOCK_STATE;
     }     

    mcDELAY_MS(1);

    for(iii=0;iii<8;iii++)  //max 4 iteration per trial
    {

        if(ii == 0 & iii == 0)
        {
            iii = 2;
        }
    
        switch(bScanState)
        {
        case TUNER_PLL_LOCK_STATE:
        // Check if current freq is outside TV_FINE_TUNE_THRESHOLD window
            mcSHOW_DBG_MSG(("[ATuner] Set Freq %d Bnad %d Itra %d \n",wFRTmp,ii,iii));
            if ((*wFreqInKHz == u4FreqBDKHz) && ((wFRTmp > (*wFreqInKHz + TV_FINE_TUNE_THRESHOLD_KHZ/2 + 200) ) || (wFRTmp < (*wFreqInKHz - TV_FINE_TUNE_THRESHOLD_KHZ/2 - 200))))
            {
                //Hit upper bound or lower bound
                //mcSHOW_DBG_MSG(("[ATuner] Hit upper bound or lower bound\n"));
                if(ii<=1)
                {
                    mcSHOW_DBG_MSG(("[ATuner] Hit upper bound or lower bound at 1st or 2nd trial\n"));
                    iii=7;
                    break;
                }
                else
                {                
                    mcSHOW_DBG_MSG(("[ATuner] Hit upper bound or lower bound at 3rd trial\n"));
                    return (FALSE);
                }       
            }       
            ATD_SetFreq(psDemodCtx, wFRTmp, TRUE);
            ATD_StartAcq(psDemodCtx,TRUE);
            
            bScanState = TUNER_VIF_LOCK_STATE;

            break;

        case TUNER_VIF_LOCK_STATE:
        
            mcDELAY_MS(10);
            ATDNA_GetVIFLock(psDemodCtx,TRUE);
        
              switch(psDemodCtx->u1VIFLock)
              {
              case 0:            
                  iii = 7;
                  break;
                              
              case 1:
                  psDemodCtx->u1PF = 0x00;
                  psDemodCtx->fgCheckCR = FALSE;
                  s4NextFreqStep = (psDemodCtx->s4Cfo*1000)/50000;
                  s4NextFreqStep *= (50000/1000);
                
                  wFRTmp += s4NextFreqStep;
                  
                  //wFRTmp += psDemodCtx->s4Cfo;                
                  bScanState = TUNER_PLL_LOCK_STATE;
                  break;
                                
              case 2:  //VIF lock
                  //if (fgDrvTunerCheckTVDLock(CHN_LOCK_CHECK_TIME))
                  
                  if((*wFreqInKHz == 73250) && (wFRTmp > 75000))
                  {
                      mcSHOW_DBG_MSG(("Got false CH1!\n"));
                      return (FALSE);
                  }
                  
                  if((*wFreqInKHz == 79250) && (wFRTmp > 81000))
                  {
                      mcSHOW_DBG_MSG(("Got false CH5!\n"));
                      return (FALSE);
                  }
                  
                  if (fgGetTVDSts(psDemodCtx,CHN_LOCK_CHECK_TIME)) //For MT5387 TVD export fucntion
                  {
                      TimingOffset = (mcGET_SYS_TICK() - u2TickStart_ATDLayer) * mcGET_TICK_PERIOD() - TimingOffset;
                      mcSHOW_DBG_MSG(("[ATuner] End SearchNearby with Frequency Offset = %3u ms\n",TimingOffset));    
                      mcMSG_IN_SEARCHNEARBY_AUTO_TEST(u2LOStep)
                      return 3;
                  }
                  else
                  {
                      mcSHOW_DBG_MSG(("VIF lock, but TVD unlock!\n"));
                      iii = 7;
                  }     
                  break;         
              }
              break;
            
            default:
                break;
            } //end of state machine
        } //end of for loop iii
    } //end of for loop ii
    return (FALSE);
}


UINT8 ATDNA_Monitor(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, void *pvArg)
{
    UINT16  u2LOStep;

     u2LOStep=psDemodCtx->pTCtx->specific_member.us_ctx.u2LO_Step;

UINT32 Freq=psDemodCtx->u4Frequency;
//BOOL fgIFStatus;
//UINT8 bIFStatus, bAFCStatus;
//UINT8 bATVStatus;
//UINT32 wFreqDR;
UINT32 wFreqDR = *wFreqInKHz;
    UINT8  ucVpress;
    #if defined(CC_SONY_GA)    
    UINT8 ucData;
    if(wPreLockFreq != *wFreqInKHz)
    {
    	mcSHOW_DBG_MSG(("[ATuner] 1st time Monitor\n"));
        u4FreqTemp = psDemodCtx -> u4Frequency;
    }

    wPreLockFreq = *wFreqInKHz;
    #endif
//For Driver ATP Auto Test
   mcMSG_IN_MONITOR_AUTO_TEST(u2LOStep)
    psDemodCtx->isBreak = _BreakFct;

    if (psDemodCtx->isBreak(pvArg))
    {
        mcSHOW_DBG_MSG(("[ATuner] Monitor.BreakFct\n"));
        return (FALSE);
    }
    else if(!psDemodCtx->fgAFT)
    {
    	mcSHOW_DBG_MSG2(("Do not do AFT in monitor!\n"));
    	return TRUE;
    }
    else
    {
        psDemodCtx->fgCheckCR = TRUE;
        ucVpress = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4;
         
        if(ATDNA_GetVIFLock(psDemodCtx, FALSE)<2)
        {
            if( (psDemodCtx->u1VIFLock == 1) && (psDemodCtx->s4Cfo > -100) && (psDemodCtx->s4Cfo < 100) )
            {
            	mcSHOW_DBG_MSG(("[ATuner] CFO > 50Khz but < 100KHz!\n"));
                return TRUE;
            	}
            else if((psDemodCtx->s4Cfo > -50) && (psDemodCtx->s4Cfo < 50) && (ucVpress == 0x01))
            {
            	mcSHOW_DBG_MSG(("[ATuner] CFO <50KHz, Vpress lock, poor pilot quality!\n"));
                return TRUE;
            }            
            else
            {   
              mcSHOW_DBG_MSG(("[ATuner] VIF unlock!\n"));
            	ATD_SetSystem(psDemodCtx, psDemodCtx->u1SubSysIdx);            	            
            	ATD_SetFreq(psDemodCtx, *wFreqInKHz, TRUE);
            	psDemodCtx->u1PF = 0x01;
            	ATD_StartAcq(psDemodCtx, TRUE);
            psDemodCtx->fgCheckCR = TRUE;
            if(ATDNA_SearchNearbyFreq(psDemodCtx, &wFreqDR,_BreakFct, pvArg, *wFreqInKHz))
            {
                #if defined(CC_SONY_GA) 
                mcSHOW_DBG_MSG(("[ATuner] New Freq = %dKhz\n", psDemodCtx -> u4Frequency));  
                #endif
                return TRUE;
            }
            else
            {
                #if defined(CC_SONY_GA) 
                if(u4FreqTemp != *wFreqInKHz)
                    ATD_SetFreq(psDemodCtx, u4FreqTemp, TRUE);                    	    
                else
                 ATD_SetFreq(psDemodCtx, *wFreqInKHz, TRUE);                    	    

                psDemodCtx->u1PF = 0x00;
                ucData = 0x24;                    
                ATD_SetReg(psDemodCtx, 0x9B6, &ucData, 1);
                ATD_StartAcq(psDemodCtx, FALSE);
 
                 mcSHOW_DBG_MSG(("[ATuner] AFT fail!\n"));
                return FALSE;
                #else
                 ATD_SetFreq(psDemodCtx, *wFreqInKHz, TRUE);                    	    
                 mcSHOW_DBG_MSG(("[ATuner] AFT fail!\n"));
                return FALSE;
                #endif
            }
        }
        }
        else
        {            
            #if defined(CC_SONY_GA) 
            u4FreqTemp = psDemodCtx -> u4Frequency;
            #endif
            mcSHOW_DBG_MSG2(("[ATuner] AFC in range!\n"));
            //ATD_SetMonitor(psDemodCtx, TRUE);
            //ATD_SetNormalMode(psDemodCtx);
            return TRUE;
        }
    }
}

static UINT8 ATDNA_GetCRLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
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
    UINT8 ucData, ucData2, ucPOAGain;
    
    ATD_GetReg(psDemodCtx, 0x988, &ucData, 1);
    ucPOAGain = ucData;

        ATD_GetReg(psDemodCtx, 0x9A0, &ucData, 1);
        ATD_GetReg(psDemodCtx, 0x9A1, &ucData2, 1);

        if(fgLogMessage)
        {
            mcSHOW_DBG_MSG(("Pilot Strength = 0x%02x\n", (ucData & 0x0C) >> 2  ));
            mcSHOW_DBG_MSG(("Pilot Stability = 0x%02x\n", (ucData2 & 0x20) >> 5 ));
            mcSHOW_DBG_MSG(("Pilot Quality = 0x%02x\n", (ucData2 & 0x10) >> 4 ));
            mcSHOW_DBG_MSG(("POA Gain = 0x%02x\n", ucPOAGain ));
        }

        if( ((ucData & 0x0C) != 0x00) || ((ucData2 & 0x30) == 0x30) ) //&& ((ucData2 & 0x33) == 0x03) )
        //if((ucData & 0x0C) != 0x00)
        {
            psDemodCtx->fgCRLock = TRUE;
        }
        else if(ucPOAGain <= 0x02) //for audio carrier > video carrier case
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

UINT8 ATDNA_GetVIFLock(ATD_CTX_T *psDemodCtx, BOOL fgLogMessage)
{
    ATDNA_GetCRLock(psDemodCtx, fgLogMessage);
    ATD_GetCFOKHz(psDemodCtx, fgLogMessage);
    //GetPilotPower(psDemodCtx);

    if( (psDemodCtx->fgCRLock == FALSE) && (psDemodCtx->fgCheckCR == TRUE) )
    {
        psDemodCtx->u1VIFLock = cATD_VIF_UNLOCK;
    }
    else
    {
    	if( (psDemodCtx->s4Cfo > (-cATDNA_VIF_FULL_LOCK_RANGE)) && 
            (psDemodCtx->s4Cfo <   cATDNA_VIF_FULL_LOCK_RANGE)   )
    	{
    	    psDemodCtx->u1VIFLock = cATD_VIF_FULL_LOCK;
    	}
    	else if( (psDemodCtx->s4Cfo >= (-cATDNA_VIF_PARTIAL_LOCK_RANGE)) && 
                 (psDemodCtx->s4Cfo <=   cATDNA_VIF_PARTIAL_LOCK_RANGE)   )
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

/**********************************************************************/
