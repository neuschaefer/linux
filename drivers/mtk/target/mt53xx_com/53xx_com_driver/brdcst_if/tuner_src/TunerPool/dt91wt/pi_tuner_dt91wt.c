/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2008, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pi_tuner_dt91wt.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_dt91wt.c
 *  Tuner control for NuTune DT91WT.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "x_gpio.h"
#include "pi_dvbt_if.h" 
#include "fe_tuner_common_if.h"
#include "pi_demod_atd.h"
#include "drvcust_if.h"
#include "eq_script_dt91wt.h"
#include "eeprom_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define C_DT91WT_VERSION                "TCL DT91WT 2010-01-01\n 2011-12-23\n v1.1"

#define C_DT91WT_BASE                  ((UINT16) 0xC0)  
#define C_DT91WT_IF_FREQUENCY          ((UINT16) 36000)  /* kHz */
#define C_DT91WT_IF_FREQUENCY_LO_STEP  ((UINT16) 900)    /* 100Hz */
#define C_DT91WT_IF_FREQUENCY_ANA      ((UINT16) 38875)  /* kHz */
#define C_DT91WT_IF_FREQUENCY_L1       ((UINT16) 33400)  /* kHz */
#define C_DT91WT_IF_FREQUENCY_I        ((UINT16) 39500)  /* kHz */
#define C_DT91WT_LO_DIVIDER_STEP       ((UINT32) 166670) /* Hz */
#define C_DT91WT_LO_DIVIDER_STEP_ANA   ((UINT16) 50000)  /* Hz */
#define C_DT91WT_LO_DIVIDER_STEP_C     ((UINT16) 62500)  /* Hz */

// DVB-T+DVB-C D-only Tuner
#define ccTUNER_LOCK_CONF_THRESHOLD     0
#define cALC_ADC_COMM_VOLT              1.2
#define cALC_ADC_INPUT_SWING            2  //Volt
#define cALC_ADC_BIAS                   (cALC_ADC_COMM_VOLT - cALC_ADC_INPUT_SWING/2)

#define C_DT91WT_TOP_SET_T              ((U8)   0x04)  //103dBuV
#define C_DT91WT_TOP_SET_C              ((U8)   0x05)  //100dBuV
//#define C_DT91WT_TOP_SET                ((U8)   0x06)  //external AGC
//#define C_DT91WT_TOP_SET_ANA           ((U8)   0x03)  //106dBuV
//#define C_DT91WT_TOP_SET               ((U8)   0x06)  //EXTERNAL
#define C_DT91WT_TOP_SET_ANA            ((U8)   0x06)  //EXTERNAL
#define C_DT91WT_SAW_BW                 SAW_BW_8M 

#define C_DT91WT_AGC_IF_SLP             1
#define C_DT91WT_AGC_IF_INT             0
#define C_DT91WT_AGC_IF_MAX             0.499
#define C_DT91WT_AGC_IF_MIN             -0.5

#define C_DT91WT_MAX_IF_GAIN_SET        -0.12   
#define C_DT91WT_MIN_IF_GAIN_SET        -0.38   
#define C_DT91WT_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_DT91WT_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_DT91WT_MAX_IF_GAIN_POW        -100    // dBm
#define C_DT91WT_MIN_IF_GAIN_POW        -50     // dBm
#define C_DT91WT_MAX_RF_GAIN_POW        C_DT91WT_MIN_IF_GAIN_POW
#define C_DT91WT_MIN_RF_GAIN_POW        -20     // dBm
#define C_DT91WT_POWER_CALIBRATE        26     // dBm

#define C_DT91WT_AGC_COEF               ((U8)   0xBD)
#define C_DT91WT_DEMOD_INPUT_POWER      cIF_TARGET_LEVEL_0CCC

#define C_DT91WT_I2C_DIVIDER       ((U16) 0x100)
// ********************************************* //

#define C_U8_DT91WT_AGC_IF_SLP          (S8)(C_DT91WT_AGC_IF_SLP *32 )
#define C_U8_DT91WT_AGC_IF_INT          (S8)(C_DT91WT_AGC_IF_INT *64 )
#define C_U8_DT91WT_AGC_IF_MAX          (S8)(C_DT91WT_AGC_IF_MAX *256)
#define C_U8_DT91WT_AGC_IF_MIN          (S8)(C_DT91WT_AGC_IF_MIN *256)

#define C_U8_DT91WT_MAX_IF_GAIN_SET     (S8)(C_DT91WT_MAX_IF_GAIN_SET *256)
#define C_U8_DT91WT_MIN_IF_GAIN_SET     (S8)(C_DT91WT_MIN_IF_GAIN_SET *256)
#define C_U8_DT91WT_MAX_RF_GAIN_SET     (S8)((C_DT91WT_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DT91WT_MIN_RF_GAIN_SET     (S8)((C_DT91WT_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_DT91WT_IF_GAIN_RANGE       (S8)(C_DT91WT_MIN_IF_GAIN_POW - C_DT91WT_MAX_IF_GAIN_POW)
#define C_U8_DT91WT_RF_GAIN_RANGE       (S8)(C_DT91WT_MIN_RF_GAIN_POW - C_DT91WT_MAX_RF_GAIN_POW)

#define C_DT91WT_FREQ_DBOUND_UPPER   858*1000*1000     //modified for DVB-C application, Menghau, 091020
#define C_DT91WT_FREQ_DBOUND_LOWER    51*1000*1000
#define C_DT91WT_FREQ_DBOUND_UPPER_Ana   865*1000*1000
#define C_DT91WT_FREQ_DBOUND_LOWER_Ana    44*1000*1000
#define C_DT91WT_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_DT91WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_DT91WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)
#define C_U8_DT91WT_AGC_IF_LVL_MAX     0x12//18  //no signal
#define C_U8_DT91WT_AGC_IF_LVL_MIN     0x08 //8  //-70dbm input


/*
STATIC UINT8* pDt91wtEqWeakVHFL[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDt91wtEqWeakVHFH[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pDt91wtEqWeakUHF[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
*/
	STATIC UINT8* pDt91wtEqNormalVHFL[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
	STATIC UINT8* pDt91wtEqNormalVHFH[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
	STATIC UINT8* pDt91wtEqNormalUHF[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
	
	
	//STATIC UINT8* pDt91wtEqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
	
	
	STATIC UINT8* pDt91wtEqStrongChromaVHFL[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
	STATIC UINT8* pDt91wtEqStrongChromaVHFH[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
	STATIC UINT8* pDt91wtEqStrongChromaUHF[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


//STATIC UINT8* pDt91wtEqBG65M=EQDt91wtBG65M;
UINT32 u4EQaddr_dt91;
UINT8  u1EQid_dt91;


extern S16 DT91WT_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param);
static  void ATD_CalAGC(ATD_TUNER_REL_REG_T * pTunerRelReg,INT8 i1If_min1,INT8 i1If_min2);

static  S8 u8TOPval=0xb0;
static  S8 u8TOPvalSECAM=0x00;
//Tuner releated ATD register
#if defined(CC_MT5396)  || defined(CC_MT5368) || defined(CC_MT5389)
static ATD_TUNER_REL_REG_T TunerRel[14]=
	{
  {0x7d4,0x02},//
  {0x7d5,0xa5},
  {0x7d6,0xa5},
  {0x7d7,0xee},
  {0x7d8,0x95},
  {0x7d9,0x95},
  {0x7da,0x7f},
  {0x7db,0x7f},
  {0x7dc,0x80},
  {0x7dd,0xb0}, // IF_min1
  {0x7de,0xb0},// IF_min2
  {0x7ca,0x99},//RF_AGC loop BW
  {0x7cb,0x01},
  {0x0,0x0}//end flag

	};
static ATD_TUNER_REL_REG_T arrAtdRelRegSECAM[]={
	{0x7d4,0x01},//
	{0x7d5,0xa1},
	{0x7d6,0xa1},
	{0x7d7,0xef},
	{0x7d8,0x91},
	{0x7d9,0x91},
	{0x7da,0x7f},
	{0x7db,0x7f},
	{0x7dc,0x80},
	{0x7dd,0x00}, // IF_min1
	{0x7de,0x00},// IF_min2
	{0x7ca,0x66},//RF_AGC loop BW
	{0x7cb,0x10},
	{0x7cc,0x3f},
	/*{0x164,0xdc},
	{0x165,0x8d},
	{0x237,0x00},
	{0x238,0x00},
	{0x174,0x55},
	{0x166,0x0e},
	{0x167,0x80},
	{0x248,0xba},
	//{0x245,0xd1},*/
	{0x0,0x0}//end flag//01 30 d1
 };

#else

static ATD_TUNER_REL_REG_T TunerRel[14]=
	{
  {0x2d4,0x02},//
  {0x2d5,0xa5},
  {0x2d6,0xa5},
  {0x2d7,0xee},
  {0x2d8,0x95},
  {0x2d9,0x95},
  {0x2da,0x7f},
  {0x2db,0x7f},
  {0x2dc,0x80},
  {0x2dd,0xb0}, // IF_min1
  {0x2de,0xb0},// IF_min2
  {0x2ca,0x99},//RF_AGC loop BW
  {0x2cb,0x01},
  {0x0,0x0}//end flag

	};
static ATD_TUNER_REL_REG_T arrAtdRelRegSECAM[]={
	{0x2d4,0x01},//
	{0x2d5,0xa1},
	{0x2d6,0xa1},
	{0x2d7,0xef},
	{0x2d8,0x91},
	{0x2d9,0x91},
	{0x2da,0x7f},
	{0x2db,0x7f},
	{0x2dc,0x80},
	{0x2dd,0x00}, // IF_min1
	{0x2de,0x00},// IF_min2
	{0x2ca,0x66},//RF_AGC loop BW
	{0x2cb,0x10},
	{0x2cc,0x3f},
	/*{0x164,0xdc},
	{0x165,0x8d},
	{0x237,0x00},
	{0x238,0x00},
	{0x174,0x55},
	{0x166,0x0e},
	{0x167,0x80},
	{0x248,0xba},
	//{0x245,0xd1},*/
	{0x0,0x0}//end flag//01 30 d1
 };
#endif
static  S8 fgLNAADC=0;

//----------------------------------------------------------------------------- 
/** DT91WT_TunerInit
 *  Tuner initialzation for DT91WT.
 *  @param  pTunerCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DT91WT_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);

    psTunerCtx->I2cAddress = C_DT91WT_BASE;
    psTunerCtx->u2IF_Freq = C_DT91WT_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A = C_DT91WT_IF_FREQUENCY_ANA; //EC_code_dA402: Add for MT5365 SAWLess ATD IF Freq. Info. 
    psTunerCtx->u4RF_Freq = 0;
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_STCHR);
    psTunerCtx->fgRFTuner = TRUE;

    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2f; //EC_code_d9930: Change 0x151 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xe3; //EC_code_d9930: Change 0x158 value for EU new BPF
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; //EC_code_d9930: Change 0x15F value for EU new BPF

    pEUCtx->m_SAW_BW  = C_DT91WT_SAW_BW;
    //pEUCtx->m_Ana_Top = C_DT91WT_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_DT91WT_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_DT91WT_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_DT91WT_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_DT91WT_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_DT91WT_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_DT91WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_DT91WT_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_DT91WT_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_DT91WT_AGC_IF_LVL_MIN;

    pEUCtx->m_SigLvTh = 0;//C_TDTWS710D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;

    pEUCtx->m_aucPara[0] = C_DT91WT_TOP_SET_T;
    pEUCtx->m_aucPara[1] = 0;

    pEUCtx->m_aucPara[2] = (U8)C_U8_DT91WT_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_DT91WT_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_DT91WT_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_DT91WT_AGC_IF_MIN;

    pEUCtx->m_aucPara[6] = C_DT91WT_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_DT91WT_DEMOD_INPUT_POWER;

    pEUCtx->m_aucPara[ 8] = (U8)C_U8_DT91WT_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_DT91WT_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_DT91WT_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_DT91WT_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_DT91WT_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_DT91WT_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_DT91WT_POWER_CALIBRATE;
  /*
    pDt91wtEqNormal[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDt91wtBG;
    pDt91wtEqNormal[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDt91wtDK;
    pDt91wtEqNormal[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDt91wtI;
    pDt91wtEqNormal[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtL;
    pDt91wtEqNormal[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDt91wtL1; 
    pDt91wtEqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN] = EQDt91wtM;
    
    pDt91wtEqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQDt91wtBG;//EQDT91WTBG;
    pDt91wtEqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQDt91wtDK;//EQDT91WTDK;
    pDt91wtEqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQDt91wtI;//EQDT91WTI;
    pDt91wtEqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtL;//EQDT91WTL;
    pDt91wtEqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQDt91wtL1;//EQDT91WTL1;
    pDt91wtEqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN] = EQDt91wtM;//EQDT91WTM;
*/
	u4EQaddr_dt91 = 1;//DRVCUST_OptGet(etunerEQaddr);
	//VERIFY(0 == EEPROM_Read((UINT64)u4EQaddr_dt91, (UINT32)&u1EQid_dt91, 1));
	
	//±³³ö	
	if(u1EQid_dt91 ==2)
		{
		printf("u1EQid ==2, 2222222222");
	pDt91wtEqNormalVHFL[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt71wtBGVHFL;
	pDt91wtEqNormalVHFL[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt71wtDKVHFL;
	pDt91wtEqNormalVHFL[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]	= EQDt71wtIVHFL;
	pDt91wtEqNormalVHFL[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt71wtLVHFL;
	pDt91wtEqNormalVHFL[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt71wtL1VHFL;
	
	pDt91wtEqNormalVHFH[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt71wtBGVHFL;
	pDt91wtEqNormalVHFH[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt71wtDKVHFL;
	pDt91wtEqNormalVHFH[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]	= EQDt71wtIVHFL;
	pDt91wtEqNormalVHFH[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt71wtLVHFL;
	pDt91wtEqNormalVHFH[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt71wtL1VHFL;
	
	
	pDt91wtEqNormalUHF[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]	= EQDt71wtBGVHFL;
	pDt91wtEqNormalUHF[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]	= EQDt71wtDKVHFL;
	pDt91wtEqNormalUHF[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt71wtIVHFL;
	pDt91wtEqNormalUHF[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt71wtLVHFL;
	pDt91wtEqNormalUHF[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt71wtL1VHFL;
	   // pDt91wtEqBGh[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN] = EQDt71wtBGh;
		
	
	pDt91wtEqStrongChromaVHFL[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGStrongChromaVHFL;
	pDt91wtEqStrongChromaVHFL[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKStrongChromaVHFL;
	pDt91wtEqStrongChromaVHFL[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIStrongChromaVHFL;
	pDt91wtEqStrongChromaVHFL[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]	= EQDt91wtLStrongChromaVHFL;
	pDt91wtEqStrongChromaVHFL[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]	= EQDt91wtL1StrongChromaVHFL;
	
	pDt91wtEqStrongChromaVHFH[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGStrongChromaVHFH;
	pDt91wtEqStrongChromaVHFH[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKStrongChromaVHFH;
	pDt91wtEqStrongChromaVHFH[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIStrongChromaVHFH;
	pDt91wtEqStrongChromaVHFH[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]	= EQDt91wtLStrongChromaVHFH;
	pDt91wtEqStrongChromaVHFH[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]	= EQDt91wtL1StrongChromaVHFH;
	
	
	pDt91wtEqStrongChromaUHF[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGStrongChromaUHF;
	pDt91wtEqStrongChromaUHF[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKStrongChromaUHF;
	pDt91wtEqStrongChromaUHF[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIStrongChromaUHF;
	pDt91wtEqStrongChromaUHF[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLStrongChromaUHF;
	pDt91wtEqStrongChromaUHF[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1StrongChromaUHF;

}
else
{  printf("u1EQid ==1, 11111111111111");


pDt91wtEqNormalVHFL[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGVHFL;
pDt91wtEqNormalVHFL[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKVHFL;
pDt91wtEqNormalVHFL[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIVHFL;
pDt91wtEqNormalVHFL[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLVHFL;
pDt91wtEqNormalVHFL[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1VHFL;

pDt91wtEqNormalVHFH[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGVHFL;
pDt91wtEqNormalVHFH[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKVHFL;
pDt91wtEqNormalVHFH[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIVHFL;
pDt91wtEqNormalVHFH[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLVHFL;
pDt91wtEqNormalVHFH[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1VHFL;


pDt91wtEqNormalUHF[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGVHFL;
pDt91wtEqNormalUHF[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKVHFL;
pDt91wtEqNormalUHF[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIVHFL;
pDt91wtEqNormalUHF[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLVHFL;
pDt91wtEqNormalUHF[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1VHFL;
   // pDt91wtEqBGh[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN] = EQDt91wtBGh;
	

pDt91wtEqStrongChromaVHFL[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGStrongChromaVHFL;
pDt91wtEqStrongChromaVHFL[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKStrongChromaVHFL;
pDt91wtEqStrongChromaVHFL[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIStrongChromaVHFL;
pDt91wtEqStrongChromaVHFL[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLStrongChromaVHFL;
pDt91wtEqStrongChromaVHFL[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1StrongChromaVHFL;

pDt91wtEqStrongChromaVHFH[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGStrongChromaVHFH;
pDt91wtEqStrongChromaVHFH[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKStrongChromaVHFH;
pDt91wtEqStrongChromaVHFH[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIStrongChromaVHFH;
pDt91wtEqStrongChromaVHFH[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLStrongChromaVHFH;
pDt91wtEqStrongChromaVHFH[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1StrongChromaVHFH;


pDt91wtEqStrongChromaUHF[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]  = EQDt91wtBGStrongChromaUHF;
pDt91wtEqStrongChromaUHF[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]  = EQDt91wtDKStrongChromaUHF;
pDt91wtEqStrongChromaUHF[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]  = EQDt91wtIStrongChromaUHF;
pDt91wtEqStrongChromaUHF[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQDt91wtLStrongChromaUHF;
pDt91wtEqStrongChromaUHF[MOD_SECAM_L1- MOD_ANA_TYPE_BEGIN]  = EQDt91wtL1StrongChromaUHF;

}
	ATD_CalAGC(TunerRel,u8TOPval,u8TOPval);

    mcSHOW_USER_MSG(("Tuner DT91WT init done!\n"));     
}


//----------------------------------------------------------------------------- 
/** DT91WT_TunerSetFreq
 *  Set Tuner PLL for DT91WT to tune RF frequency.
 *  @param  pTCtx     Pointer of pi tuner driver context.
 *  @param  freq        RF center frequency in KHz.
 *  @param  mode                    
 *  @param  autoSearch  
 *  @param  step            
 *  @retval 
 */
//-----------------------------------------------------------------------------
INT16 DT91WT_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
  )
{
    UCHAR uc_data;
    UCHAR TunerS[5];
    S16 ucTunerLockConfCnt;
    UINT32 Lo;
    UINT16  Ndivider, CB1, CB2, BB; 
    S16 ii, jj;
	STATIC UINT8  u1PreMode = 0xFF;
//    UINT8 autoSearch=param->fgAutoSearch;;
    UINT8 Mode = param->Modulation;
    UINT32 Freq = param->Freq;
	UINT32 dvbclnacontrol=1;

    mcSHOW_DBG_MSG(("DT91WT_TunerSetFreq\n"));
    
    SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);
    //GPIO_AGC_SWITCH = DRVCUST_OptGet(eTunerCtrlAna);

    pTunerCtx->u4RF_Freq = Freq;
    pTunerCtx->I2cAddress = C_DT91WT_BASE;

      if( Mode == MOD_DVBT)//digital reception
    {

	    Lo = Freq + C_DT91WT_IF_FREQUENCY; // kHz
	    //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
	  
	    Ndivider = (U16) ( (Lo*1000+C_DT91WT_LO_DIVIDER_STEP/2) / C_DT91WT_LO_DIVIDER_STEP);
		
		// Conver Freq to unit of MHz
			   Freq = Freq/1000;
	
	    if (Freq < 48 || Freq > 866)
	    {
	        mcSHOW_DBG_MSG(("Out of range for LO!\n"));
	        //return (+1);
	    }
	    
	     if (Freq < 158)	   
		{
			BB = 0x81;
		}
		else if (Freq < 450)  
		{		 
			BB = 0x82;
		}
	
		else	 if (Freq < 670)			 
		{
			BB = 0x88;
		}
		else
		{
			BB = 0xc8;
		}
		if (param->fgAutoSearch)
      	{
      	BB|= 0x04;
      	mcSHOW_DBG_MSG(("LNA ALWAYS ON WHEN DTV SCAN!\n"));
      	} 
	 	CB1 = 0x98;
		CB2 = 0xd1;
			                                                                                	    	   
    }
	else if ( Mode == MOD_DVBC)
		  {
		    Lo = Freq + C_DT91WT_IF_FREQUENCY; // kHz
		   //Ndivider = (U16) ( ((Lo * 1000) / DTOS403_LO_DIVIDER_STEP) + 0.5 );
		 
		    Ndivider = (U16) ( (Lo*1000+C_DT91WT_LO_DIVIDER_STEP_C/2) / C_DT91WT_LO_DIVIDER_STEP_C);

		    // Conver Freq to unit of MHz
	          Freq = Freq/1000;

		    if (Freq < 48|| Freq > 866)
		  {
			mcSHOW_USER_MSG(("Out of range for LO!\n"));
		   // return (+1);
		  }
			
		   if (Freq <= 158)	   
		  {
			BB = 0x01;
		  }
		  else if (Freq <= 450)  
		  {		 
			BB = 0x02;
		  }
		  else				 
		  {
			BB = 0x08;
		  }
		if (param->fgAutoSearch)
      	{
      	BB|= 0x04;
      	mcSHOW_DBG_MSG(("LNA ALWAYS ON WHEN DTV SCAN!\n"));
      	} 
		
		VERIFY(0 ==EEPROM_Read(0x50f, (UINT32)&dvbclnacontrol, 1));

		if(dvbclnacontrol==0)
		{	BB&= 0xfb;
		mcSHOW_DBG_MSG(("CTMR setting LNA OFF DVBC!\n"));
		}

		
	 	   CB1 = 0x9b;
		   CB2 = 0xd1;			
	
	    }		
    else //analog reception
        {
    
	    if (Mode == MOD_SECAM_L1 )
	    {
	    	Lo = Freq + C_DT91WT_IF_FREQUENCY_L1; // kHz
	    }
	    else
	    {
	    	Lo = Freq + C_DT91WT_IF_FREQUENCY_ANA; // kHz
	    }	
	    Ndivider = (U16) ( (Lo*1000+C_DT91WT_LO_DIVIDER_STEP_ANA/2) / C_DT91WT_LO_DIVIDER_STEP_ANA);
	    
	    mcSHOW_DBG_MSG(("Freq = %d, Divider ratio = %04X \n",param->Freq, Ndivider));


		pTunerCtx->u2IF_Freq_A = Lo - Freq;
	    // Conver Freq to unit of MHz
	    Freq = Freq/1000;
	    
	    if (Freq < 45 || Freq > 865)
	    {
	       mcSHOW_DBG_MSG(("Out of range for LO!\n"));
        //  return (+1);    // liuqu,20090910, can not returen 1 for 47.25MHZ channel loss!
	    }
	    
	    if (Freq <= 158)       
		{
			BB = 0x01;
		}
		else if (Freq < 450)  
		{        
			BB = 0x02;
		}
		else                 
	    {
			BB = 0x08;
		}

     if (param->fgAutoSearch)
      	{
      	BB|= 0x04;
      	mcSHOW_DBG_MSG(("LNA ALWAYS ON WHEN ANALOG SCAN!\n"));
      	}            
	       CB1 = 0xbd;
           CB2 =0xd1;

    }
//for TCL tuner vendor request
	if (!param->fgAutoSearch )
      	{
      	BB|= 0x04;
      	mcSHOW_DBG_MSG(("LNA ALWAYS ON WHEN channel change first!\n"));
      	} 


	
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) BB;
    TunerS[4] = (UCHAR) CB2; 

    //pTunerCtx->u4LO_Freq = Lo;
	if( TunerS[3] & 0x04 ) fgLNAADC=1;
	else fgLNAADC=0;//update LNA status 

    for (jj=0; jj<2; jj++)
    {        
        if (ICtrlBus_I2cTunerWrite(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, TunerS,5) > 0)
        {
            return (-1);
        }

	mcDELAY(10); 
	
        ucTunerLockConfCnt = 0;
        for (ii=0; ii<20; ii++)
        {
            mcDELAY_MS(5);
            ICtrlBus_I2cTunerRead(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
			mcSHOW_DBG_MSG(("data=%x\n",uc_data));

            if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
                ucTunerLockConfCnt++;
            else
                ucTunerLockConfCnt = 0;

            if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
                break;
        }
        ucTunerLockConfCnt = ii;

        if(ii < 20)
        {
			mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, ucTunerLockConfCnt)); 
            break;
        }
        else
        {
            mcSHOW_DBG_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));
        }
    }

    mcSHOW_DBG_MSG(("[%s]-LO_%02X: %02X-%02X-%02X-%02X-%02X\n",(CHAR *)C_DT91WT_VERSION,pTunerCtx->I2cAddress, TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4]));

/*** LNA PART ************************************/
  if (!param->fgAutoSearch && (Mode == MOD_DVBC || Mode == MOD_DVBT))
  	{
	  if ((u1PreMode != MOD_DVBC) && (u1PreMode != MOD_DVBT) ) 
	  	{
		for(jj=0;jj<3;jj++)
			{
		mcDELAY_MS(160);  //Delay 160ms after switching to internal RFAGC to wait RFAGC stable
		ICtrlBus_I2cTunerRead(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
	       uc_data&=0x07;
		mcSHOW_DBG_MSG(("cnt=%d,uc_ADC=0x%2x\n",jj,uc_data));
			}
	  	}
	  else 
	  	{
	  	mcDELAY_MS(160);  //Delay 160ms after switching to internal RFAGC to wait RFAGC stable
		ICtrlBus_I2cTunerRead(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
		uc_data&=0x07;
	       mcSHOW_DBG_MSG(("uc_ADC=0x%2x\n",uc_data));
		}
	//---------set LNA
		if(0==uc_data)	 
			{
			BB&=0xFB;
			TunerS[3] = (UCHAR) BB;
			mcSHOW_DBG_MSG(("LNA off,change LNA\n"));
			ICtrlBus_I2cTunerWrite(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, TunerS,5);

			}
		else 
			{
				BB|=0x04;
				TunerS[3] = (UCHAR) BB;
			mcSHOW_DBG_MSG(("LNA on\n"));
			}
		
		if( TunerS[3] & 0x04 ) fgLNAADC=1;
		else fgLNAADC=0;
	

  }

	 if (param->fgAutoSearch && (Mode == MOD_DVBC || Mode == MOD_DVBT))
	   {
			 if ((u1PreMode != MOD_DVBC) && (u1PreMode != MOD_DVBT) ) 
		   {
				 for(jj=0;jj<7;jj++)
					 {
				 mcDELAY_MS(30);   //Delay 160ms after switching to internal RFAGC to wait RFAGC stable form ATV to DTV,AGC extenal to internal while form 0 to1 23
				 ICtrlBus_I2cTunerRead(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
					uc_data&=0x07;
				 mcSHOW_DBG_MSG(("cnt=%d,uc_ADC=0x%2x\n",jj,uc_data));
				 if(uc_data==2|| uc_data==3)
				   break;
				 }
	 
			 } 
			 else
			   {
				mcDELAY_MS(30);    //
				mcSHOW_DBG_MSG(("dealay for AGC\n"));
				for(jj=0;jj<3;jj++)
			   {
				 //Delay 160ms after switching to internal RFAGC to wait RFAGC stable form ATV to DTV,AGC extenal to internal while form 0 to1 23
				ICtrlBus_I2cTunerRead(C_DT91WT_I2C_DIVIDER,pTunerCtx->I2cAddress, &uc_data,1);
				   uc_data&=0x07;
				mcSHOW_DBG_MSG(("cnt=%d,uc_ADC=0x%2x\n",jj,uc_data));
				 if(uc_data<2)
				   {mcDELAY_MS(30); }
				  else {break;}
				 mcDELAY_MS(30); 
				}
	 
				
			   }
			   
	   }

	u1PreMode = Mode;
    pEUCtx->m_aucPara[1]  = TunerS[0];   //send register value to UI, LC 070102
    pEUCtx->m_aucPara[15] = TunerS[1];
    pEUCtx->m_aucPara[16] = TunerS[2];
    pEUCtx->m_aucPara[17] = TunerS[3];
    pEUCtx->m_aucPara[18] = TunerS[4];
	mcDELAY_MS(20); 

    return (0);
}




//----------------------------------------------------------------------------- 
/** DT91WT_TunerGetStatus
 *  Set Tuner PLL staus for DT91WT
 *  @param  
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
char *DT91WT_TunerGetVer(void)
{
    return ((CHAR*)C_DT91WT_VERSION);
}
//----------------------------------------------------------------------------- 
/** DT91WT_SetSawBw
 *  Set Tuner PLL staus for DT91WT
 *  @param  
 *  @retval tuner Saw BandWidth
 */
//-----------------------------------------------------------------------------
void DT91WT_SetSawBw(ITUNER_CTX_T* pTunerCtx, UINT8 sawbw)
{
    pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;
}
//----------------------------------------------------------------------------- 
/** DT91WT_SetSSICondition
 *  Set SSI Condition for DT91WT
 *  @param  
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DT91WT_SetSSICondition(ITUNER_CTX_T* pTunerCtx, VOID * pInOutVal)
{
    pTunerCtx->sSSIPara.i2Mod = *((INT16*) pInOutVal);
    pTunerCtx->sSSIPara.i2CR = *((INT16*) pInOutVal+1);
    pTunerCtx->sSSIPara.i2Ssi_RF= *((INT16 *) pInOutVal+2);
    pTunerCtx->sSSIPara.i2Ssi_IF= *((INT16 *) pInOutVal+3);
    pTunerCtx->sSSIPara.i2Ssi_BB= *((INT16 *) pInOutVal+4);
}

//RF mapping table

typedef struct
{
    INT16 u2power;
	INT16 u2val;
} SSI_MAP_T;

//LNA off table for SSI
static SSI_MAP_T SSItbOffRF[]=
{
	{-16,163},
	{-20,198},
	{-23,248},
	{-26,275},
	{-30,319},
	{-32,343},
	{-34,355},
	{-36,369},
	{-38,384},
	{-39,385},
	{-40,392},
	{-42,399},
	{-44,408},
	{-48,432},
	{-50,543},
	{-51,736},
	{-52,739},
	{0,0},//end flag by NULL 
};

static SSI_MAP_T SSItbOffIF[]=
{
	{-52,3},
	{-53,4},
	{-54,6},
	{-55,7},
	{-56,9},
	{-57,11},
	{-58,13},
	{-59,14},
	{-60,16},
	{-61,17},
	{-62,19},
	{-63,20},
	{0,0},//end flag by NULL 
	
};

static SSI_MAP_T SSItbOffBB[]=
{
	{-100,12},
	{-97,13},
	{-94,14},
	{-90,15},
	{-86,20},
	{-84,24},
	{-82,30},
	{-80,32},
	{-78,39},
	{-76,53},
	{-74,61},
	{-72,80},
	{-70,97},
	{-69,107},
	{-68,128},
	{-64,200},
	{-63,235},
	{0,0},//end flag by NULL 
};

//LNA on table for SSI
static SSI_MAP_T SSItbOnRF[]=
{
	{-16,127},
	{-20,135},
	{-24,156},
	{-26,159},
	{-30,192},
	{-32,225},
	{-34,252},
	{-36,278},
	{-38,304},
	{-40,318},
	{-44,352},
	{-46,360},
	{-48,380},
	{-50,385},
	{-54,403},
	{-56,414},
	{-58,423},
	{-60,449},
	{-61,679},
	{-62,737},
	{0,0},//end flag by NULL 
};

static SSI_MAP_T SSItbOnIF[]=
{
	{-62,2},
	{-63,4},
	{-64,6},
	{-65,7},
	{-66,9},
	{-67,10},
	{-68,12},
	{-69,14},
	{-70,15},
	{-71,17},
	{-72,19},
	{-73,20},
	{0,0},//end flag by NULL 
};

static SSI_MAP_T SSItbOnBB[]=
{
	{-100,20},
	{-96,23},
	{-94,25},
	{-90,40},
	{-86,59},
	{-84,73},
	{-82,91},
	{-80,108},
	{-78,135},
	{-76,163},
	{-74,206},
	{-73,232},
	{0,0},//end flag by NULL 
};

//=============
static INT16 power_mapping(INT16 i2SLD_RFAGC, SSI_MAP_T *pSSItabel,INT16 TbLen)
{
     UINT16 i =0;
	 INT16 i4Power_rec = -100;
   
INT16  RF_AGC_Max = pSSItabel[TbLen-1].u2val;
INT16  RF_AGC_Min = pSSItabel[0].u2val;
	   
//=========lim
			   if (i2SLD_RFAGC < RF_AGC_Min )
			     i2SLD_RFAGC = RF_AGC_Min;
			    if(i2SLD_RFAGC > RF_AGC_Max)
				i2SLD_RFAGC = RF_AGC_Max;
//==========cal
		
				{
			       for (i=0; i<(TbLen-1); i++)//must use ssiLEn-1 to ssiLen
				    {
					  if (i2SLD_RFAGC == pSSItabel[i].u2val)
					  {
					    i4Power_rec =pSSItabel[i].u2power;
					    break;
					  } 			
				       else if ((i2SLD_RFAGC<=pSSItabel[i+1].u2val )&&(i2SLD_RFAGC>pSSItabel[i].u2val))
					  {
					   i4Power_rec =((pSSItabel[i+1].u2power-pSSItabel[i].u2power )*(i2SLD_RFAGC-pSSItabel[i+1].u2val)/(pSSItabel[i+1].u2val-pSSItabel[i].u2val))+pSSItabel[i+1].u2power;
					   break;
					  } 				
				  }
			    }
			return i4Power_rec;
	
}

static INT16 MaxValue(SSI_MAP_T *pSSItabel,INT16 TbLen)
{
return pSSItabel[TbLen-1].u2val;
}

static INT16 MinValue(SSI_MAP_T *pSSItabel,INT16 TbLen)
{
return pSSItabel[0].u2val;
}


static INT16 MaxPower(SSI_MAP_T *pSSItabel,INT16 TbLen)
{
return pSSItabel[0].u2power;
}

static INT16 MinPower(SSI_MAP_T *pSSItabel,INT16 TbLen)
{
return pSSItabel[TbLen-1].u2power;
}


static INT16 SSItbLen(SSI_MAP_T *pSSItabel)
{
INT16 i;
for(i=0;i<100;i++)//MAX is 100 for one tuner
	{
	if(0==pSSItabel[i].u2power && i>0) break;
	}
if(i>98)
	mcSHOW_DBG_MSG1(("!!!SSI table may outflow!!!\n"));
return i;//len of table for 1,2,3...not include {0,0} so only length of valid data
}

INT16 MapRecPowerByTB(INT16 dataRF,INT16 dataIF,INT16 dataBB,SSI_MAP_T * SSItbRF,SSI_MAP_T *SSItbIF,SSI_MAP_T * SSItbBB)
{
	INT16 retPow;
    INT16 SSItbRFLen,MinValueTabRFAGC=0,MaxValueTabRFAGC=0, MaxValueTabRFAGCValid,	MaxPowerTabRFAGC;  //for table RF 
	INT16 SSItbIFLen,MinValueTabIFAGC=0,MaxValueTabIFAGC=0,							MaxPowerTabIFAGC;  //for table IF 
    INT16 SSItbBBLen,MinValueTabBBAGC=0,MaxValueTabBBAGC=0, MaxValueTabBBAGCValid,	MinPowerTabBBAGC,MaxPowerTabBBAGC;  //for table BB 

SSItbRFLen=SSItbLen(SSItbRF);
SSItbIFLen=SSItbLen(SSItbIF);
SSItbBBLen=SSItbLen(SSItbBB);

MaxPowerTabRFAGC=MaxPower(SSItbRF,SSItbRFLen);
MinValueTabRFAGC=MinValue(SSItbRF,SSItbRFLen);
MaxValueTabRFAGC=MaxValue(SSItbRF,SSItbRFLen);
MaxValueTabRFAGCValid=(MaxValueTabRFAGC*9)/10;//avoid RF jitter range .90% value is ok,90%~100% maybe unstable 

//==IF parameter

MaxPowerTabIFAGC=MaxPower(SSItbIF,SSItbIFLen);

MinValueTabIFAGC=MinValue(SSItbIF,SSItbIFLen);
MaxValueTabIFAGC=MaxValue(SSItbIF,SSItbIFLen);
//==BB parameter
MinPowerTabBBAGC=MaxPower(SSItbBB,SSItbBBLen);
MaxPowerTabBBAGC=MinPower(SSItbBB,SSItbBBLen);

MinValueTabBBAGC=MinValue(SSItbBB,SSItbBBLen);
MaxValueTabBBAGC=MaxValue(SSItbBB,SSItbBBLen);

MaxValueTabBBAGCValid=(MaxValueTabBBAGC*9)/10;//avoid BB jitter range .90% value is ok,90%~100% maybe unstable 
//======================

if(dataRF<MinValueTabRFAGC && dataBB> MaxValueTabBBAGCValid )//signal is strong!
return retPow=MaxPowerTabRFAGC;
//============range 1
if(dataRF<MaxValueTabRFAGCValid && dataIF< MinValueTabIFAGC)
	{ 
	retPow=power_mapping( dataRF,SSItbRF ,SSItbRFLen);
	return 	retPow;
	}

//====corros range
if(dataIF<MinValueTabIFAGC)// data>RF valid range && dataIF<minIF range
return retPow=MaxPowerTabIFAGC;//power of range
//====range 2
if(dataIF<MaxValueTabIFAGC && dataBB>MaxValueTabBBAGCValid)
	{
	retPow=power_mapping( dataIF,SSItbIF ,SSItbIFLen);
	return 	retPow;
	}

//====corros range
if(dataBB>MaxValueTabBBAGCValid )// max of 3   dataIF=18 && data>600 ;dtatBB>222
{
return retPow=MaxPowerTabBBAGC;
}
//=======range 3
if(dataBB>MinValueTabBBAGC)//min of 3 and signal loss
{
	retPow=power_mapping( dataBB,SSItbBB ,SSItbBBLen);
	return 	retPow;
}
else
	return 	retPow=MinPowerTabBBAGC;

}
//----------------------------------------------------------------------------- 
/*
 *  DT91WT_GetSSI
 *  calculate Signal Strength Indicator.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
UINT32 DT91WT_GetSSI(ITUNER_CTX_T* pTunerCtx) 
{
    INT16 Power_Ref[] =
    {
        -93,
        -91,
        -90,
        -89,
        -88,
        -87,
        -85,
        -84,
        -83,
        -82,
        -82,
        -80,
        -78,
        -77,
        -76,
    };
    INT32 i4Power_ref = 0;
    INT32 i4Power_rec = 0;
    INT32 i4Power_rel = 0;
    INT16 i2Ssi_RF = 0;
    INT16 i2Ssi_IF = 0;
    INT16 i2Ssi_BB = 0;
    UINT32 u4SSI_Indicator = 0;
    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);

    // initial level for best mux, Ken, 100607
    pCtx->m_SigLvScan = 0;
    
    /* Get Power Ref Value */  
    mcSHOW_DBG_MSG(("DVBT: Mod=%d, Code Rate=%d, RFAGC=%d, IFAGC=%d, BBAGC=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR,
	                                                              pTunerCtx->sSSIPara.i2Ssi_RF,pTunerCtx->sSSIPara.i2Ssi_IF,pTunerCtx->sSSIPara.i2Ssi_BB));

    /* Get Pre Value */    
    i2Ssi_RF = pTunerCtx->sSSIPara.i2Ssi_RF;    
    i2Ssi_IF = pTunerCtx->sSSIPara.i2Ssi_IF;    
    i2Ssi_BB = pTunerCtx->sSSIPara.i2Ssi_BB;	
    /* Get Power_Rec Value */
 if(pCtx->m_aucPara[17] & 0x04)//get LNA status
	   {i4Power_rec=MapRecPowerByTB(i2Ssi_RF,i2Ssi_IF,i2Ssi_BB,SSItbOnRF,SSItbOnIF,SSItbOnBB);
 	mcSHOW_DBG_MSG3(("SSI LNA on\n"));
 	}
 else
	   {i4Power_rec=MapRecPowerByTB(i2Ssi_RF,i2Ssi_IF,i2Ssi_BB,SSItbOffRF,SSItbOffIF,SSItbOffBB);
 	mcSHOW_DBG_MSG3(("SSI LNA off\n"));
 	}

	/*Get Ref Value-Start*/
	if(pTunerCtx->sSSIPara.i2Mod==DVBT_Qam_Mode_Unknow)
	{
		mcSHOW_DBG_MSG1(("DVBT: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
		return u4SSI_Indicator;
	}
	else
	{
		i4Power_ref = Power_Ref[(pTunerCtx->sSSIPara.i2Mod)*DVBT_CR_Mode_Size + pTunerCtx->sSSIPara.i2CR];
	}


    
    /* Get Power Rel Value */
    i4Power_rel = i4Power_rec - i4Power_ref;
	mcSHOW_DBG_MSG(("DVBT: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
    /* Cal SSI_Indicator Value */
    if(i4Power_rel < -15)
    {
        u4SSI_Indicator = 0;
    }
    else if((i4Power_rel >= -15) && (i4Power_rel < 0))
    {
        u4SSI_Indicator = 2*(i4Power_rel+15)/3;
    }
    else if((i4Power_rel >= 0) && (i4Power_rel < 20))
    {
        u4SSI_Indicator = 4*i4Power_rel+10;
    }
    else if((i4Power_rel >= 20) && (i4Power_rel < 35))
    {
        u4SSI_Indicator = 2*(i4Power_rel-20)/3+90;
    }
    else if(i4Power_rel >= 35)
    {
        u4SSI_Indicator = 100;
    }

    // update signal level for best mux, Ken, 100607
    pCtx->m_SigLvScan = (S8) u4SSI_Indicator;
    
    return u4SSI_Indicator;
}

/******************/
#define     C_AGC_RF_GAIN_RANGE      450
#define     C_AGC_IF_MAX                        ((S8) 0x7F)
#define     C_AGC_RF_MAX                      ((S8)0x7F)
#define     C_AGC_RF_MIN                        ((S8)0x80)   


STATIC UINT16 ATD_GetTunerIfGainRange(INT8 i1IfMin)
{

INT16 IfGainRange,IfGainValue;
IfGainValue=120+(INT16)(i1IfMin*60)/128;//range db*10

if(IfGainValue >180) IfGainValue=180;//mean 18db gain
if(IfGainValue <60) IfGainValue=60;//mean 18db gain

IfGainRange=180-IfGainValue;//get IF range 

 mcSHOW_DBG_MSG(("---IfGainRange=%d\n",IfGainRange));
 return IfGainRange;
}

static  void ATD_CalAGC(ATD_TUNER_REL_REG_T * pTunerRelReg,INT8 i1If_min1,INT8 i1If_min2)
{
  INT16 i2IfGainRange=ATD_GetTunerIfGainRange(i1If_min1);//Get IF_gain range
  INT16 i2Top1=-4*1000+8*(INT32)(i2IfGainRange*1000)/(C_AGC_RF_GAIN_RANGE+i2IfGainRange);
  INT16 i2Rfmax=(C_AGC_RF_MAX*1000)/128;
  INT16 i2RfSlope=(INT32)((i2Rfmax+1000)*1000)/(i2Top1-4000);
  INT16 i2RfBias=(INT32)(i2Rfmax*1000-i2RfSlope*i2Top1+500)/1000;
  INT16 i2Ifmin1=(i1If_min1*1000)/128;
  INT16 i2IfSlope1=(INT32)((1000-i2Ifmin1)*1000)/(-4000-i2Top1);
  INT16 i2IfBias1=(INT32)(i2Ifmin1*1000-i2IfSlope1*i2Top1+500)/1000;
  INT16 i2Top2,i2IfSlope2,i2IfBias2,i2Ifmin2;
  UINT8 u1AGCArr[11];
  UINT8 u1Len=sizeof(u1AGCArr)/sizeof(u1AGCArr[0]);
  UINT8 ii=0;
  
  if(i1If_min1!=i1If_min2)//Exist TOP2
  {
    i2Top2=(INT32)(((C_AGC_RF_MIN*1000/128)-i2RfBias)*1000)/i2RfSlope;
	i2Ifmin2=(i1If_min2*1000)/128;
	i2IfSlope2=(INT32)((i2Ifmin1-i2Ifmin2)*1000)/(i2Top2-4000);
	i2IfBias2=(INT32)(i2Ifmin1*1000-i2IfSlope2*i2Top2+500)/1000;
  }
  else
  {
   i2Ifmin2=i2Ifmin1;
   i2IfSlope2=i2IfSlope1;
   i2IfBias2=i2IfBias1;
  }
  if(!pTunerRelReg)
  {
   mcSHOW_DRVERR_MSG(("pTunerRelReg is NULL!\n"));
   return;   
  }
  u1AGCArr[0]=(UINT8)((INT32)(i2RfBias*16+500)/1000);
  u1AGCArr[1]=(UINT8)((INT32)(i2IfBias1*16+500)/1000);
  u1AGCArr[2]=(UINT8)((INT32)(i2IfBias2*16+500)/1000);
  u1AGCArr[3]=(UINT8)((INT32)(i2RfSlope*64+500)/1000);
  u1AGCArr[4]=(UINT8)((INT32)(i2IfSlope1*64+500)/1000);
  u1AGCArr[5]=(UINT8)((INT32)(i2IfSlope2*64+500)/1000);
  u1AGCArr[6]=(UINT8)C_AGC_RF_MAX;
  u1AGCArr[7]=(UINT8)C_AGC_IF_MAX;
  u1AGCArr[8]=(UINT8)C_AGC_RF_MIN;
  u1AGCArr[9]=(UINT8)i1If_min1;
  u1AGCArr[10]=(UINT8)i1If_min2;
  //Set AGC parameter 
  while(1)
  {
   if(ii>=u1Len)break;
   #if defined(CC_MT5396)  || defined(CC_MT5368) || defined(CC_MT5389)
   pTunerRelReg->u2RegAddr=0x7D4+ii;
   #else   
   pTunerRelReg->u2RegAddr=0x2D4+ii;
   #endif
   pTunerRelReg->u1RegVal=u1AGCArr[ii];
   
   mcSHOW_DBG_MSG(("u1AGCArr[%d]=0x%02X\n",ii,u1AGCArr[ii]));
   ii++;
   pTunerRelReg++;
  }
}

//----------------------------------------------------------------------------- 
/** DT91WT_TunerOP
 *  Set Tuner PLL staus for DT91WT
 *  @param  
 *  @retval tuner OP
 */
//-----------------------------------------------------------------------------
INT16 DT91WT_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{
//	 SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

    switch (eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
          *RetStr=DT91WT_TunerGetVer();
        }
        break;    

    case itSetTop:
    {
          INT8 i1Data;
		  UINT8 subSysId = SetVal;
          u8TOPval = *(INT8 *)pInOutVal;//SetVal;
          u8TOPvalSECAM= *(INT8 *)pInOutVal;//SetVal;
          mcSHOW_USER_MSG(("u8TOPval set TOP=%x done!\n",u8TOPval));
		  switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
		  i1Data=u8TOPvalSECAM;
	  	ATD_CalAGC(arrAtdRelRegSECAM,i1Data,i1Data);
                break;
            default:
                i1Data=u8TOPval;
		ATD_CalAGC(TunerRel,i1Data,i1Data);
                break;
            }  
		 
    }
        break;
	case itGetTop:
		{  

			UINT8 subSysId = SetVal;
			switch(subSysId)
					   {
					   case MOD_SECAM_L:
					   case MOD_SECAM_L1:
				   *((INT8 *) pInOutVal)   = u8TOPvalSECAM;
						 break;
					   default:
						 *((INT8 *) pInOutVal)   = u8TOPval;
					   break;
					   }  		
			
			mcSHOW_USER_MSG(("u8TOPval pInOutVal-set TOP=%x done!\n",*((INT8 *) pInOutVal)));    
		}
		break;

    case itSetReg:
         {
        
         UCHAR TunerS[5];
         UCHAR uc_data;
         UINT8 *ptrWord = (UINT8 *)pInOutVal;
         int ii,jj;
         int ucTunerLockConfCnt;
    
         SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
         
         for(ii = 0; ii < 5; ii++)
            {
               TunerS[ii]=(UCHAR)(*ptrWord);
               ptrWord++;
               //mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",TunerS[ii]));
               // mcDELAY(C_ENV57S_PLL_POLL_INTERVAL);
            }
           mcSHOW_DRVERR_MSG(("The register words are: 0x%x-0x%x-0x%x-0x%x-0x%x\n",TunerS[0],TunerS[1],TunerS[2],TunerS[3],TunerS[4]));
        
          
          for (jj=0; jj<2; jj++)
              {
                if (ICtrlBus_I2cTunerWrite(C_DT91WT_BASE,pTCtx->I2cAddress, TunerS,5) > 0)
                {
                    return (-1);
                }

                  pTunerCtx->m_aucPara[1] = (UCHAR)TunerS[0]; //send register value to UI, LC 070102
          pTunerCtx->m_aucPara[15] = (UCHAR)TunerS[1];
          pTunerCtx->m_aucPara[16] = (UCHAR)TunerS[2];
          pTunerCtx->m_aucPara[17] = (UCHAR)TunerS[3];
          pTunerCtx->m_aucPara[18] = (UCHAR)TunerS[4]; 
                ucTunerLockConfCnt = 0;
              for (ii=0; ii<20; ii++)
              {
              mcDELAY(5);// wait PLL lock
                ICtrlBus_I2cTunerRead(C_DT91WT_BASE,pTCtx->I2cAddress, &uc_data,1);
                if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL lock
                  ucTunerLockConfCnt++;
                else
                  ucTunerLockConfCnt = 0;
                if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
                  break;
              }
              ucTunerLockConfCnt = ii;
              if(ii < 20)
              {
                mcSHOW_DRVAPI_MSG(("the %d-th time setting PLL success, cnt = %d\n" , jj+1, ucTunerLockConfCnt));
                break;
              }
              else
              {
                mcSHOW_DRVAPI_MSG(("the %d-th time setting PLL fail\n" , jj+1));
              }
          
            }
          
        }
        break;
        
    case itGetReg:
        {            
                int i = 0;
            SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
                *((INT8 *) pInOutVal)   = pTunerCtx->m_aucPara[1]; 
          *((INT8 *) pInOutVal+1) = pTunerCtx->m_aucPara[15];
          *((INT8 *) pInOutVal+2) = pTunerCtx->m_aucPara[16];
                *((INT8 *) pInOutVal+3) = pTunerCtx->m_aucPara[17];
          *((INT8 *) pInOutVal+4) = pTunerCtx->m_aucPara[18];
          for (i = 0; i < 5; i++)
                  mcSHOW_DRVERR_MSG(("The register word is: 0x%x\n",*((INT8 *)pInOutVal+i)));              
        }
        break;

    case itSetIfDemod:
        break;
    case itSetSawBw:
        DT91WT_SetSawBw(pTCtx, SetVal);
        break;
    case itSetLNA:
        break;
	case itSetLNABYADC:
		{            
             UCHAR uc_data;
			 UCHAR TunerS[5];
			 UINT8 *pLNAstatus = (UINT8*)pInOutVal;
			SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
			TunerS[0]= pTunerCtx->m_aucPara[1]  ;   
			TunerS[1]= pTunerCtx->m_aucPara[15] ;
			TunerS[2]= pTunerCtx->m_aucPara[16] ;
			TunerS[3]= pTunerCtx->m_aucPara[17] ;
			TunerS[4]= pTunerCtx->m_aucPara[18] ;
			if(SetVal) //only for get LNA status value ,so dont setLNA by ADC
				{
				if(pLNAstatus)
				*pLNAstatus=fgLNAADC;
				else
				{mcSHOW_DBG_MSG(("ptr is NULL\n"));}
				
				break;
				}
			

			 //set LNA
			 mcDELAY_MS(10);
			 mcDELAY_MS(10);
		ICtrlBus_I2cTunerRead(C_DT91WT_I2C_DIVIDER,C_DT91WT_BASE, &uc_data,1);
	    mcSHOW_DBG_MSG(("uc_data=0x%2x\n",uc_data));
				uc_data&=0x07;
		mcSHOW_DBG_MSG(("ADC_data=0x%2x\n",uc_data));
	
		if(uc_data==0)	 
			{
				TunerS[3]&=0xFB;
				ICtrlBus_I2cTunerWrite(C_DT91WT_I2C_DIVIDER,C_DT91WT_BASE, TunerS,5);
			mcSHOW_DBG_MSG(("LNA off,change LNA setting\n"));

			}
		else 
			{
				TunerS[3]|=0x04;
			mcSHOW_DBG_MSG(("LNA on\n"));
			}
		if( TunerS[3] & 0x04 ) fgLNAADC=1;
		else fgLNAADC=0;
		
		pTunerCtx->m_aucPara[17] =TunerS[3];//update to UI
     mcSHOW_DBG_MSG(("DT91WT v1.02 20101026-LO_%02X: %02X-%02X-%02X-%02X-%02X\n",C_DT91WT_BASE, TunerS[0], TunerS[1], TunerS[2], TunerS[3],TunerS[4]));              
        }
        break;
    case itSetIfMin:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifMinPtr = (UINT8*)pInOutVal;
            
        mcSHOW_DBG_MSG(("DT91WT TOP Setting\n"));
        switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifMinPtr = 0x00;
                break;
            default:
                *ifMinPtr = 0xC0;
                break;
            }
        }
        break;

    case itGetTHC:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifThcPtr = (UINT8*)pInOutVal;
            
        mcSHOW_DBG_MSG(("DT91WT TOP Setting\n"));
        switch(subSysId){
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifThcPtr = 0x20;
                break;
            default:
                *ifThcPtr = 0x1A;
                break;
        }
        }
        break;

    case itGetEqScriptNormal: 
        	{
		#if defined(CC_OCEANIA_EQ)
		if(pTCtx->u4RF_Freq <165000)
        {*(UINT8**)pInOutVal = pDt91wtEqNormalVHFL[SetVal];
		mcSHOW_DBG_MSG(("VHFL %d\n",SetVal));}
		else if(pTCtx->u4RF_Freq <250000)
		{*(UINT8**)pInOutVal = pDt91wtEqNormalVHFH[SetVal];
		mcSHOW_DBG_MSG(("VHFH%d\n",SetVal));}
		else
		{*(UINT8**)pInOutVal = pDt91wtEqNormalUHF[SetVal];
		mcSHOW_DBG_MSG(("UHF%d\n",SetVal));}
		#else
		if(pTCtx->u4RF_Freq <165000)
        {*(UINT8**)pInOutVal = pDt91wtEqNormalVHFL[SetVal];
		mcSHOW_DBG_MSG(("VHFL %d\n",SetVal));}
		else if(pTCtx->u4RF_Freq <450000)
		{*(UINT8**)pInOutVal = pDt91wtEqNormalVHFH[SetVal];
		mcSHOW_DBG_MSG(("VHFH%d\n",SetVal));}
		else
		{*(UINT8**)pInOutVal = pDt91wtEqNormalUHF[SetVal];
		mcSHOW_DBG_MSG(("UHF%d\n",SetVal));}
		#endif
    		}
 
         break;
    case itGetEqScriptWeak: 
        	{
		#if defined(CC_OCEANIA_EQ)
		if(pTCtx->u4RF_Freq <165000)
        {*(UINT8**)pInOutVal = pDt91wtEqNormalVHFL[SetVal];
		mcSHOW_DBG_MSG(("VHFL %d\n",SetVal));}
		else if(pTCtx->u4RF_Freq <250000)
		{*(UINT8**)pInOutVal = pDt91wtEqNormalVHFH[SetVal];
		mcSHOW_DBG_MSG(("VHFH%d\n",SetVal));}
		else
		{*(UINT8**)pInOutVal = pDt91wtEqNormalUHF[SetVal];
		mcSHOW_DBG_MSG(("UHF%d\n",SetVal));}

		#else
		if(pTCtx->u4RF_Freq <165000)
        {*(UINT8**)pInOutVal = pDt91wtEqNormalVHFL[SetVal];
		mcSHOW_DBG_MSG(("VHFL %d\n",SetVal));}
		else if(pTCtx->u4RF_Freq <450000)
		{*(UINT8**)pInOutVal = pDt91wtEqNormalVHFH[SetVal];
		mcSHOW_DBG_MSG(("VHFH%d\n",SetVal));}
		else
		{*(UINT8**)pInOutVal = pDt91wtEqNormalUHF[SetVal];
		mcSHOW_DBG_MSG(("UHF%d\n",SetVal));}
		#endif
    		}
		
        break;
    case itGetEqScriptStrongChroma: 
			{
		#if defined(CC_OCEANIA_EQ)
		if(pTCtx->u4RF_Freq <165000)
        {*(UINT8**)pInOutVal = pDt91wtEqStrongChromaVHFL[SetVal];
		mcSHOW_DBG_MSG(("SVHFL %d\n",SetVal));}
		else if(pTCtx->u4RF_Freq <250000)
		{*(UINT8**)pInOutVal = pDt91wtEqStrongChromaVHFH[SetVal];
		mcSHOW_DBG_MSG(("SVHFH%d\n",SetVal));}
		else
		{*(UINT8**)pInOutVal = pDt91wtEqStrongChromaUHF[SetVal];
		mcSHOW_DBG_MSG(("SUHF%d\n",SetVal));}

		#else
		if(pTCtx->u4RF_Freq <165000)
        {*(UINT8**)pInOutVal = pDt91wtEqStrongChromaVHFL[SetVal];
		mcSHOW_DBG_MSG(("SVHFL %d\n",SetVal));}
		else if(pTCtx->u4RF_Freq <450000)
		{*(UINT8**)pInOutVal = pDt91wtEqStrongChromaVHFH[SetVal];
		mcSHOW_DBG_MSG(("SVHFH%d\n",SetVal));}
		else
		{*(UINT8**)pInOutVal = pDt91wtEqStrongChromaUHF[SetVal];
		mcSHOW_DBG_MSG(("SUHF%d\n",SetVal));}
		#endif
    		}
        break; 

    case itSetSSICond:
        DT91WT_SetSSICondition(pTCtx, pInOutVal);
        break;
    case itGetSSIIndex:
        *((UINT32*)pInOutVal) = DT91WT_GetSSI(pTCtx);
        break;  

    case itGetAnaFreqBound:
        {
        TUNER_ANA_ATTRIBUTE_T   *psTunerAnaAttribute;

        psTunerAnaAttribute = (TUNER_ANA_ATTRIBUTE_T *) pInOutVal;
        psTunerAnaAttribute->ui4_lower_bound_freq = C_DT91WT_FREQ_DBOUND_LOWER_Ana;
        psTunerAnaAttribute->ui4_upper_bound_freq = C_DT91WT_FREQ_DBOUND_UPPER_Ana;
        }
        break;
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
		{
				UINT8 subSysId = SetVal;
				switch(subSysId)
					{
					case MOD_SECAM_L:
					case MOD_SECAM_L1:
						*(ATD_TUNER_REL_REG_T **)pInOutVal = arrAtdRelRegSECAM;
						break;
					default:
						*(ATD_TUNER_REL_REG_T **)pInOutVal= TunerRel;
						break;
					}
		}
		break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}

