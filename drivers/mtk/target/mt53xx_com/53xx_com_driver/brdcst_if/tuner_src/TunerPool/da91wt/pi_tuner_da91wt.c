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
 * $RCSfile: pi_tuner_DA91WT.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 /*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pi_tuner_DA91WT.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_da91wt.c
 *  Tuner control for TCL da91wt
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
#include "eq_script_da91wt.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define TCL_DA91WT_BASE            ((UINT16) 0xC0)
#define TCL_DA91WT_IF_FREQUENCY_ANA    ((UINT16) 45750)   /* KHz */
#define TCL_DA91WT_IF_FREQUENCY    ((UINT16) 44000)   /* KHz */
#define TCL_DA91WT_LO_STEP         ((UINT16) 5000)  /* 10Hz */
#define TCL_DA91WT_ANA_LO_STEP         ((UINT16) 5000)  /* 10Hz */
#define TCL_DA91WT_STR             "TCL_DA91WT 2012-01-04\n2012-01-04\n v1.0"
#define TCL_DA91WT_Div_Ratio     0x07    //0x07 --> 50khz
//#define TCL_DA91WT_TOP_SET                ((U8)   0x05)  //104 dBuV


#define TCL_DA91WT_FREQ_DBOUND_UPPER   865*1000*1000     
#define TCL_DA91WT_FREQ_DBOUND_LOWER    53*1000*1000
#define TCL_DA91WT_FREQ_DBOUND_UPPER_Ana   865*1000*1000
#define TCL_DA91WT_FREQ_DBOUND_LOWER_Ana    53*1000*1000
// for SLD
#define TCL_DA91WT_AGC_IF_SLP             0.75
#define TCL_DA91WT_AGC_IF_INT             -2
#define TCL_DA91WT_AGC_IF_MAX             0.998
#define TCL_DA91WT_AGC_IF_MIN             -1

#define TCL_U8_DA91WT_AGC_IF_SLP          (S8)(TCL_DA91WT_AGC_IF_SLP *64 )
#define TCL_U8_DA91WT_AGC_IF_INT          (S8)(TCL_DA91WT_AGC_IF_INT *16 )
#define TCL_U8_DA91WT_AGC_IF_MAX          (S8)(TCL_DA91WT_AGC_IF_MAX *128)
#define TCL_U8_DA91WT_AGC_IF_MIN          (S8)(TCL_DA91WT_AGC_IF_MIN *128)

#define C_U8_TH2603_MAX_IF_GAIN_SET     (S8)(C_TH2603_MAX_IF_GAIN_SET *256)
#define C_U8_TH2603_MIN_IF_GAIN_SET     (S8)(C_TH2603_MIN_IF_GAIN_SET *256)
#define C_U8_TH2603_MAX_RF_GAIN_SET     (S8)((C_TH2603_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TH2603_MIN_RF_GAIN_SET     (S8)((C_TH2603_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TH2603_IF_GAIN_RANGE       (S8)(C_TH2603_MIN_IF_GAIN_POW - C_TH2603_MAX_IF_GAIN_POW)
#define C_U8_TH2603_RF_GAIN_RANGE       (S8)(C_TH2603_MIN_RF_GAIN_POW - C_TH2603_MAX_RF_GAIN_POW)


//PLL lock check parameters
#define TCL_DA91WT_PLL_POLL_INTERVAL      10 //ms
#define TCL_DA91WT_PLL_POLL_TIMETOUT      100 //ms
#define TCL_DA91WT_PLL_POLL_CNT           TCL_DA91WT_PLL_POLL_TIMETOUT/TCL_DA91WT_PLL_POLL_INTERVAL //counter
   
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
STATIC INT16 IFCTRL_VHFL[] =  {-720,  -986,  -1231 , -1455 , -1688,  -1954,  -2162,-90};//From -90dBm
STATIC INT16 IFCTRL_VHFH[] =  {-161,  -574,  -887,   -1143 , -1384,  -1607,  -1852, -2164,-90};//From -90dBm
STATIC INT16 IFCTRL_UHF [] =  {490,    88,  -355,     -719 ,  -1004, -1254,  -1481, -1711 ,-1979,-2163,-90};//From -90dBm

STATIC UINT8* pDA91WTEqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDA91WTEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

STATIC UINT8 u1AnaTop = 4;   //TOP is 106

//Tuner releated ATD register
#if defined(CC_MT5396)  || defined(CC_MT5368) || defined(CC_MT5389)
STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]=
{
    {0x7d4, 0x04},
    {0x7d5, 0x00},
    {0x7d6, 0x00},
    {0x7d7, 0xec},
    {0x7d8, 0xEF},
    {0x7d9, 0xEF},
    {0x7da, 0x7f},
    {0x7db, 0x7f},
    {0x7dc, 0x80},
    {0x7dd, 0x80},
    {0x7de, 0x80},
    {0x0,0x0}//end flag
 };
#else
STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]=
{
    {0x0,0x0}//end flag
};
#endif
//
/** DA91WT_TunerInit
 *  Tuner initialzation forDA91WT.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID DA91WT_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);

    /* Common part: All tuners need to do the following procedures */
    //Tuner_ParaInit(psTunerCtx);   // move to InitializeTunerCtx() (tuner_interface.c)
    psTunerCtx->I2cAddress = TCL_DA91WT_BASE;
    psTunerCtx->u2IF_Freq = TCL_DA91WT_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A = TCL_DA91WT_IF_FREQUENCY_ANA;    
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI); // iven    
    psTunerCtx->fgRFTuner=TRUE;//Sawless tuner

    // iven 20110706, add for polarity if setting for NA reference
    psTunerCtx->bSpecPolarity = 0;
    // iven 20110706, add for legacy tuner as default fixed PGA gain index. Don care for Sawless
    psTunerCtx->uSLD_DefaultPGAIndex = 0;
//    psTunerCtx->fgRFAGC = TRUE;  // Sawless Tuner
    /* Specific part: Only for this tuner model */

    psTunerSpecificCtx->m_s4FreqBoundUpper = TCL_DA91WT_FREQ_DBOUND_UPPER;
    psTunerSpecificCtx->m_s4FreqBoundLower = TCL_DA91WT_FREQ_DBOUND_LOWER;
    psTunerSpecificCtx->m_s4AnaFreqBoundUpper = TCL_DA91WT_FREQ_DBOUND_UPPER_Ana;
    psTunerSpecificCtx->m_s4AnaFreqBoundLower = TCL_DA91WT_FREQ_DBOUND_LOWER_Ana;
    
    psTunerSpecificCtx->m_aucPara_SLD[0] = u1AnaTop;
    psTunerSpecificCtx->m_aucPara_SLD[1] = 0;

    psTunerSpecificCtx->m_aucPara_SLD[2] = (U8)TCL_U8_DA91WT_AGC_IF_SLP;
    psTunerSpecificCtx->m_aucPara_SLD[3] = (U8)TCL_U8_DA91WT_AGC_IF_INT;
    psTunerSpecificCtx->m_aucPara_SLD[4] = (U8)TCL_U8_DA91WT_AGC_IF_MAX;
    psTunerSpecificCtx->m_aucPara_SLD[5] = (U8)TCL_U8_DA91WT_AGC_IF_MIN;



    
    psTunerSpecificCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = FALSE;    //for Hostcmd, sawless default AD target level in FW
    #ifdef fcEXTERN_DEMOD
    psTunerSpecificCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;   // for 5365+5112ee
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
    pDA91WTEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQDA91WTMN;    
    pDA91WTEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQDA91WTMNWeak;

    //pDA91WTEqNormal[MOD_NTSC_M_54M_44M]  = EQTCLDA91WTMN;
    //pDA91WTEqNormal[MOD_NTSC_M_54M_36M]  = EQTCLDA91WTMN;
   psTunerSpecificCtx->u2LO_Step=TCL_DA91WT_ANA_LO_STEP;
   
    mcSHOW_USER_MSG(("Init " TCL_DA91WT_STR "\n"));
}

//----------------------------------------------------------------------------- 
/**  DA91WT_TunerGetStatus
 *  Set Tuner PLL staus forDA91WT/DA91WT
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
STATIC BOOL DA91WT_TunerGetStatus(ITUNER_CTX_T *pTCtx)
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
/** DA91WT_TunerSetFreq
 *  Set Tuner PLL forDA91WT/DA91WT to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 DA91WT_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
ITUNER_CTX_T *psTunerCtx = pTCtx; 
UINT32  Lo;
UINT8   CB1, CB2, BB;
UINT16  Ndivider;
BOOL 	 fgAnalogSignal = FALSE;
UINT8   TunerS[5], TunerDataCnt,u1Mod;
UINT32 freq;
UINT8 ii;

    u1Mod=param->Modulation;

    mcSHOW_DBG_MSG(("TCL DA91WT_TunerSetFreq %dKHz\n",param->Freq));
    if( u1Mod>= MOD_ANA_TYPE_BEGIN )
    { 
        fgAnalogSignal = TRUE;
    }

    TunerDataCnt = 5;
    freq=param->Freq;

    if (fgAnalogSignal)
    { //analog IF 45.75MHz    
        Lo = freq + psTunerCtx->u2IF_Freq_A;
    }
    else
    {  //digital IF 44MHz
        Lo = freq + psTunerCtx->u2IF_Freq;
    }


    if(!fgAnalogSignal)
    {//digital  mode use digital LO step
        Ndivider = (UINT16) ((Lo * 100) / TCL_DA91WT_LO_STEP);
    }
    else
    {
        Ndivider = (UINT16) ((Lo * 100) /TCL_DA91WT_ANA_LO_STEP);
    }
    freq =freq /1000;//convert to unit MHZ

    /* Assign Control word for specific tuner */
    if (freq < 53 || freq > 865)
    {
       mcSHOW_HW_MSG(("Out of range for LO!\n"));
       return (+1);
    }
    CB1 = 0x80;
    CB1 = (CB1 & 0xC7) | ((u1AnaTop)<<3) ; //  TOP setting 00xxx000
    CB1 = (CB1 & 0xF8) | (TCL_DA91WT_Div_Ratio) ; //  Divid ratio setting 00000xxx        
    CB2 = 0xD1;
    BB = 0x00;

    if (freq < 120)
    {
        BB = 0x01;
    }
    else if (freq < 165)
    {
        BB = 0x41;
    }
	else if (freq < 250)
    {
        BB =0x02;
    }
	else if (freq < 448)
    {
        BB = 0x42;
    }
	else if (freq < 720)
    {
        BB = 0x08;
    }
    else
    {
        BB =0x48;
    }
    if(fgAnalogSignal)
	{
	BB &=0xBF;
	}
/* LO settings */

    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;
    TunerS[3] = (UCHAR) BB;
    TunerS[4] = (UCHAR) CB2;
    
 /* Write to tuner device */
    if (ICtrlBus_I2cTunerWrite(270,psTunerCtx->I2cAddress, TunerS, TunerDataCnt))
    {
        mcSHOW_HW_MSG(("TunerWrite PLL failed!\n"));
        return (-1);
    }
    else//I2C bus  connect
    {
	//polling PLL lock status
	
	for(ii=0;ii<TCL_DA91WT_PLL_POLL_CNT;ii++)
	{
	  mcDELAY_MS(TCL_DA91WT_PLL_POLL_INTERVAL);
	  if(DA91WT_TunerGetStatus(psTunerCtx))break;
	}
	
	if(ii<TCL_DA91WT_PLL_POLL_CNT)
	{
	 mcSHOW_DBG_MSG(("PLL lock at %d th time\n",ii));
	}
	else
	{
	 mcSHOW_HW_MSG(("PLL unlock !(%d)",ii));
	}
	
	
        if(fgAnalogSignal){
            psTunerCtx->u4RF_Freq=(Ndivider*TCL_DA91WT_ANA_LO_STEP)/100-psTunerCtx->u2IF_Freq_A;
        }
        else{
            psTunerCtx->u4RF_Freq=(Ndivider*TCL_DA91WT_LO_STEP)/100-psTunerCtx->u2IF_Freq;
        }

        mcSHOW_DBG_MSG(("LO_%02X: (%d) %02X-%02X-%02X-%02X-%02X(%dKHz)\n",psTunerCtx->I2cAddress, TunerDataCnt, TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4],psTunerCtx->u4RF_Freq));
    }

    mcDELAY_MS(10);

    return (0);
}

//----------------------------------------------------------------------------- 
/** DA91WT_TunerGetStatus
 *  Set Tuner PLL staus forDA91WT/DA91WT
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *DA91WT_TunerGetVer(VOID)
{
    return (TCL_DA91WT_STR);
}
/***********************************************************************/
//----------------------------------------------------------------------------- 
/** TDAU4XB02A_SignalLevel_Handler
 *  Get IFAGC Releated Value,ex:SignalLeveldBm,SignalLevel,Vol2Ctrl,Ctrl2Vol
 *  @param  rCmd transfer parameter in such as Freq and Modulation,inout parameter
 *  @retval TRUE Get successful
                  FALSE Found Error
 */
//-----------------------------------------------------------------------------
STATIC INT16 DA91WT_SignalLevel_Handler(ITUNER_CTX_T * pTCtx,CMD_SIGLVL_PARAM_T * rCmd){
     INT16 ret=ITUNER_OK;
     INT32 * Retval=&(rCmd->IOpara);//Input para
     
     switch(rCmd->Id){
            case IFAGCCTRL2VOL://Covert IfAGC Ctrl Word to Voltage(mv)
            {
             INT32 i2CtrlWord=*Retval;
             INT32 Freq=rCmd->Freq;
            
    	    if (Freq<= 165000 ) // VHF-Low band
            {
            *Retval = 549 * (i2CtrlWord+720) / 1442 + 1298;
            }
	        else if (Freq <=465000 ) // VHF-High band
            {
            *Retval = 759 * (i2CtrlWord +161) /2003 + 1507;
            }
  	        else // UHF band
            {
            *Retval= 1022* (i2CtrlWord - 490) / 2653 + 1769;
            }
            }
            
            break;
            case IFAGCVOL2CTRL://Convert IfAGC Voltage to Ctrl Word
            {
               INT32 i2Vol=*Retval;
               INT32 Freq=rCmd->Freq;
               
               if(Freq<=165000)
               	{
               		*Retval=1442*(i2Vol-749)/549-2162;
               	}
               	else if(Freq<=465000)
               	{
               	  *Retval=2003*(i2Vol-748)/759-2164;	
               	}
               	else
               	{
               	  *Retval=2653*(i2Vol-747)/1022-2163;	
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
               if(Freq<= 168000){
                    pIFCtrl=IFCTRL_VHFL;
                    u1valid_len=sizeof(IFCTRL_VHFL)/sizeof(IFCTRL_VHFL[0])-1;
               }
               else if(Freq<= 465000){
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
              if(Freq<= 168000){
                    pIFCtrl=IFCTRL_VHFL;
                    u1valid_len=sizeof(IFCTRL_VHFL)/sizeof(IFCTRL_VHFL[0])-1;
               }
               else if(Freq<= 465000){
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
/** DA91WT_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 DA91WT_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetStatus:
        {
        BOOL * Retval=(BOOL *)pInOutVal;
        *Retval=DA91WT_TunerGetStatus(pTCtx);
        }
        break;
    case itGetVer:
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=DA91WT_TunerGetVer();
        }
        break;
    case itGetSignalLevel:
        {
        CMD_SIGLVL_PARAM_T * SigLvlParam=(CMD_SIGLVL_PARAM_T *)pInOutVal;
        return DA91WT_SignalLevel_Handler(pTCtx,SigLvlParam);
        }

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pDA91WTEqNormal[SetVal];
        break;

    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pDA91WTEqWeak[SetVal];
        break;
    	
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
    case itSetTop:
	    u1AnaTop=SetVal;
		break;
	case itGetTop:
        *(UINT8*)pInOutVal=u1AnaTop;
         break;		 
      case itGetTunerRelReg://Get ATD Tuner Rel register setting
       {
            *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg;
        }
        break;    
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
