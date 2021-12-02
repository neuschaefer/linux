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
 * $RCSfile: pi_tuner_FA2307A.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_FA2307A.c
 *  Tuner control for SS FA2307A
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "pi_demod_atd.h"
#include "fe_tuner_common_if.h"
//#include "pd_tuner_type.h"
#include "eq_script_fa2307a_5387.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NUTUNE_FA2307A_BASE                    ((UINT16) 0xC2)
#define NUTUNE_FA2307A_IF_FREQUENCY    ((UINT16) 44000)           /* KHz */
#define NUTUNE_FA2307A_IF_FREQUENCY_ANA   ((UINT16) 45750)   /* KHz */
#define NUTUNE_FA2307A_LO_STEP              ((UINT16) 6250)             /* 10Hz */
#define NUTUNE_FA2307A_ANA_LO_STEP     ((UINT16) 6250)             /* 10Hz */
#define NUTUNE_FA2307A_STR             "NUTUNE_FA2307A 2010-01-01\n 2011-04-07\n v1.0"
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------
//Signal Power Sample Array Size
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------------------------------------------------------------------------------------------------------
//Signal Power Samlpe Array
STATIC INT16 IFCTRL_VHFL[] =  {4096, 4096, 563, 93, -287, -590, -880, -1195, -1513, -1546, -90};//From -90dBm
STATIC INT16 IFCTRL_VHFH[] =  {4096, 4096, 493, 45, -323, -622, -914,  -1233,  -1542, -90};//From -90dBm
STATIC INT16 IFCTRL_UHF[] =  {4096, 4096, 359, -62, -402, -703, -995, -1320, -1550, -90};//From -90dBm

STATIC UINT8* pFa2307aEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pFa2307aEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

//PLL lock check parameters
#define NUTUNE_FA2307A_PLL_POLL_INTERVAL      10 //ms
#define NUTUNE_FA2307A_PLL_POLL_TIMETOUT      100 //ms
#define NUTUNE_FA2307A_PLL_POLL_CNT           NUTUNE_FA2307A_PLL_POLL_TIMETOUT/NUTUNE_FA2307A_PLL_POLL_INTERVAL //counter

//----------------------------------------------------------------------------- 
/**  FA2307A_TunerGetStatus
 *  Set Tuner PLL staus for FA2307A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL FA2307A_TunerGetStatus(ITUNER_CTX_T *pTCtx);

//
/** FA2307A_TunerInit
 *  Tuner initialzation for FA2307A.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID FA2307A_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = NUTUNE_FA2307A_BASE;
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->fgRFAGC = FALSE;
    /* Specific part: Only for this tuner model */
    psTunerCtx->u2IF_Freq = NUTUNE_FA2307A_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A = NUTUNE_FA2307A_IF_FREQUENCY_ANA;

    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);

    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = TRUE;

#ifdef fcEXTERN_DEMOD
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;	 // for 5365+5112ee
#else
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x3D;
#endif
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00;  

    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x19;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // init EQ script
    pFa2307aEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQFA2307AMN;
    pFa2307aEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQFA2307AMNWeak;
    //pTdvwh810fEqNormal[MOD_NTSC_M_54M_44M]  = EQLGTDVWH810FMN;
    //pTdvwh810fEqNormal[MOD_NTSC_M_54M_36M]  = EQLGTDVWH810FMN;
   psTunerSpecificCtx->u2LO_Step=NUTUNE_FA2307A_ANA_LO_STEP;
   
    mcSHOW_USER_MSG(("Init " NUTUNE_FA2307A_STR "(TAR=0x3D)\n"));
}


//----------------------------------------------------------------------------- 
/** FA2307A_TunerSetFreq
 *  Set Tuner PLL for FA2307A to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 FA2307A_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32  Lo;
UINT8   CB1,CB2,BB,AB;
UINT16  Ndivider;
BOOL    fgAnalogSignal = FALSE;
UINT8   TunerS[6], TunerDataCnt,u1Mod;
UINT32 freq;
UINT8 ii;

u1Mod=param->Modulation;

   mcSHOW_DBG_MSG(("NuTune FA2307A_TunerSetFreq %dKHz\n",param->Freq));
  if ( u1Mod>= MOD_ANA_TYPE_BEGIN )
        fgAnalogSignal = TRUE;

    TunerDataCnt = 6;
    freq=param->Freq;

    if(fgAnalogSignal)
        freq+=1750;//analog IF 45.75MHz
    
    Lo = freq + psTunerCtx->u2IF_Freq;
    
    if(!fgAnalogSignal)
    {//digital  mode use digital LO step
        Ndivider = (UINT16) ((Lo * 100) / NUTUNE_FA2307A_LO_STEP);
    }
    else
    {
        Ndivider = (UINT16) ((Lo * 100) /NUTUNE_FA2307A_ANA_LO_STEP);
    }
    freq =freq /1000;//convert to unit MHZ

/* Assign Control word for specific tuner */
    if (freq < 53 || freq > 865)
    {
       mcSHOW_HW_MSG(("Out of range for LO!\n"));
       return (+1);
    }

   CB1 = 0xCE;
   CB2 = 0xDE;

    if (freq < 148)
        BB = 0x81;
    else if (freq < 418)
        BB = 0x82;
    else
        BB = 0x84;

    AB = 0x50;

    
/* LO settings */
    TunerS[0] = (UINT8) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UINT8) 0xFF & Ndivider;
    TunerS[2] = (UINT8) CB1;
    TunerS[3] = (UINT8) BB;
    TunerS[4] = (UINT8) CB2;
    TunerS[5] = (UINT8) AB;

    
 /* Write to tuner device */
    if (ICtrlBus_I2cTunerWrite(270,psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else//I2C bus  connect
    {
        //Polling PLL lock status
        for(ii=0;ii<NUTUNE_FA2307A_PLL_POLL_CNT;ii++)
        {
            mcDELAY_MS(NUTUNE_FA2307A_PLL_POLL_INTERVAL);
            if(FA2307A_TunerGetStatus(psTunerCtx))
            {
                break;
            }
        }
		
        if(ii<NUTUNE_FA2307A_PLL_POLL_CNT)
        {
            mcSHOW_DBG_MSG(("PLL lock at %d th time\n", ii));
        }
        else
        {
            mcSHOW_HW_MSG(("PLL unlock !(%d)", ii));
        }

        // Show the Tuner control word log
        if(fgAnalogSignal)
        {
            psTunerCtx->u4RF_Freq=(Ndivider*NUTUNE_FA2307A_ANA_LO_STEP)/100-psTunerCtx->u2IF_Freq-1750;
        }
        else
        {
            psTunerCtx->u4RF_Freq=(Ndivider*NUTUNE_FA2307A_LO_STEP)/100-psTunerCtx->u2IF_Freq;
        }
        mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X-%02X(%dKHz)\n",psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4],TunerS[5],psTunerCtx->u4RF_Freq));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/**  FA2307A_TunerGetStatus
 *  Set Tuner PLL staus for FA2307A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL FA2307A_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;

    UINT8 TunerS[4];

    if (ICtrlBus_I2cTunerRead(270,psTunerCtx->I2cAddress, TunerS, 1))
    {
        mcSHOW_HW_MSG(("Tuner read failed!\n"));
        return FALSE;
    }

    if (mcTEST_BIT(TunerS[0], 6))
        return TRUE;
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** FA2307A_TunerGetVer
 *  Get Tuner Driver Version for FA2307A
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *FA2307A_TunerGetVer(VOID)
{
    return (NUTUNE_FA2307A_STR);
}
/***********************************************************************/
//----------------------------------------------------------------------------- 
/** FA2307A_SignalLevel_Handler
 *  Get IFAGC Releated Value,ex:SignalLeveldBm,SignalLevel,Vol2Ctrl,Ctrl2Vol
 *  @param  rCmd transfer parameter in such as Freq and Modulation,inout parameter
 *  @retval TRUE Get successful
                  FALSE Found Error
 */
//-----------------------------------------------------------------------------
STATIC INT16 FA2307A_SignalLevel_Handler(ITUNER_CTX_T * pTCtx,CMD_SIGLVL_PARAM_T * rCmd){
     INT16 ret=ITUNER_OK;
     INT32 * Retval=&(rCmd->IOpara);//Input para
     
     switch(rCmd->Id){
            case IFAGCCTRL2VOL://Covert IfAGC Ctrl Word to Voltage(mv)
            {
             INT32 i2CtrlWord=*Retval;
             INT32 Freq=rCmd->Freq;
            
    	      if (Freq<= 148000 ) // VHF-Low band
             {
                 *Retval = 580 * (i2CtrlWord - 563) / 1443 + 1841;
             }
	      else if (Freq <=418000 ) // VHF-High band
             {
                 *Retval = 566 * (i2CtrlWord - 493) / 1407 + 1812;
             }
  	      else // UHF band
             {
                 *Retval= 542* (i2CtrlWord - 359) / 1354 + 1757;
             }
           }
           break;
			
            case IFAGCVOL2CTRL://Convert IfAGC Voltage to Ctrl Word
            {
               INT32 i2Vol=*Retval;
               INT32 Freq=rCmd->Freq;
               
               if(Freq<=148000)
               {
                   *Retval=1443*(i2Vol-1841)/580 + 563;
               }
               else if(Freq<=418000)
               {
                   *Retval=1407*(i2Vol-1812)/566 + 493;	
               }
               else
               {
                   *Retval=1354*(i2Vol-1757)/542 + 359;	
               }
            }
             
            break;
            
            case IFAGCCTRL2PWR://Conert IfAGC Ctrl Word to Signal Level dBm
            {
            	 INT32 Freq=rCmd->Freq;
            	 INT32 i2CtrlWord=*Retval;
            	 
               INT16 *pIFCtrl=NULL;
               
               INT32 i4RFPwr = 0;
               
               INT32 i4Low_Pwr_Level=0;
              
               UINT8 u1valid_len=0;//Array size not include base power
               UINT8 ii = 0;
               INT16 i2Value_0, i2Value_1;
               //Check if Ctrl Word is negative 14bits
               if(mcTEST_BIT(i2CtrlWord, 13)){//if ctrl word negative
                 i2CtrlWord=~i2CtrlWord;
		             i2CtrlWord++;
		             i2CtrlWord&=0x1FFF;
		             i2CtrlWord*=-1;
                }
                //Get Address of Sample Array                
               if(Freq<= 148000){
                    pIFCtrl=IFCTRL_VHFL;
                    u1valid_len=sizeof(IFCTRL_VHFL)/sizeof(IFCTRL_VHFL[0])-1;
               }
               else if(Freq<= 418000){
                    pIFCtrl=IFCTRL_VHFH;
                    u1valid_len=sizeof(IFCTRL_VHFH)/sizeof(IFCTRL_VHFH[0])-1;
               }
               else{
                   pIFCtrl=IFCTRL_UHF;
                   u1valid_len=sizeof(IFCTRL_UHF)/sizeof(IFCTRL_UHF[0])-1;
               }
               
               if(!pIFCtrl)//pIFCtrl is null!
               {
               	   ret=ITUNER_NOT_OK;
                   break;
               }
               
                i4Low_Pwr_Level=pIFCtrl[u1valid_len]*cPWR_CONST;//Base power
                
                 if (i2CtrlWord >= pIFCtrl[0]) // Larger than Max IFCtrl
                     i4RFPwr = i4Low_Pwr_Level;
                 else if (i2CtrlWord <= pIFCtrl[u1valid_len - 1]) // Smaller than Min IFCtrl
                     i4RFPwr = i4Low_Pwr_Level + cPWR_INTERVAL*(u1valid_len - 1);
                  else
                  {
          // Linear Inner-Interpolation
                      for (ii = 0; ii < (u1valid_len - 1); ii++)
                       {
                         if ((i2CtrlWord <= pIFCtrl[ii]) && (i2CtrlWord > pIFCtrl[ii + 1]))
                          {
                            i2Value_0 = pIFCtrl[ii];
                            i2Value_1 = pIFCtrl[ii + 1];
                            i4RFPwr = i4Low_Pwr_Level + cPWR_INTERVAL * ii;
                            i4RFPwr = i4RFPwr + cPWR_INTERVAL * (i2CtrlWord - i2Value_0) / (i2Value_1 - i2Value_0);
                            break;
                          }
                        }
                 }
                i4RFPwr = i4RFPwr/cPWR_CONST;//Signal Power
                *Retval=(INT16)i4RFPwr;
            }
            break;
            
            case IFAGCCTRL2LVL://Convert IfAGC Ctrl Word to SignalLevel
            {
              INT32 Freq=rCmd->Freq;
              INT32 i2RFPwr=*Retval;
              INT16 i2Low_Pwr_Level=0;
              UINT8 u1valid_len=0;
              INT16 *pIFCtrl=NULL;
              
            // RF Signal Strength (0-100)
              if(Freq<= 148000){
                    pIFCtrl=IFCTRL_VHFL;
                    u1valid_len=sizeof(IFCTRL_VHFL)/sizeof(IFCTRL_VHFL[0])-1;
               }
               else if(Freq<= 418000){
                    pIFCtrl=IFCTRL_VHFH;
                    u1valid_len=sizeof(IFCTRL_VHFH)/sizeof(IFCTRL_VHFH[0])-1;
               }
               else{
                   pIFCtrl=IFCTRL_UHF;
                   u1valid_len=sizeof(IFCTRL_UHF)/sizeof(IFCTRL_UHF[0])-1;
               }
               i2Low_Pwr_Level=pIFCtrl[u1valid_len];
              *Retval =(cLOW_SIG_STRENGTH +cSIG_INTERVAL * ( i2RFPwr - i2Low_Pwr_Level));
             }
            break;
            default:
            mcSHOW_USER_MSG(("Unknown operation acquisition id %d",rCmd->Id));
            ret=ITUNER_NOT_SUPPORTED;
            break;
     }
     return ret;
}
//----------------------------------------------------------------------------- 
/** FA2307A_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 FA2307A_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetStatus:
        {
        BOOL * Retval=(BOOL *)pInOutVal;
        *Retval=FA2307A_TunerGetStatus(pTCtx);
        }
        break;
    case itGetVer:
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=FA2307A_TunerGetVer();
        }
        break;
    case itGetSignalLevel:
        {
        CMD_SIGLVL_PARAM_T * SigLvlParam=(CMD_SIGLVL_PARAM_T *)pInOutVal;
        return FA2307A_SignalLevel_Handler(pTCtx,SigLvlParam);
        }

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pFa2307aEqNormal[SetVal];
        break;

    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pFa2307aEqWeak[SetVal];
        break;
    	
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
