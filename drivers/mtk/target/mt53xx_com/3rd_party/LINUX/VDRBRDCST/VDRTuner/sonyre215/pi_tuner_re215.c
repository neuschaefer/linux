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
 * $RCSfile: pi_tuner_re215.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_re215.c
 *  SONY RE215 tuner driver.
 */


#include "x_os.h"
#include "x_gpio.h"


#ifdef  INTERNAL_3RD_DEMOD
//#warning "************tuner INTERNAL_3RD_DEMOD"
#include "ctrl_bus.h"
#include "sif_if.h"
#else
#include "tunerDebug.h"
//#warning "************tuner VDR_3RD_DEMOD"
#include "vdr_PD_Def.h"

#endif
#include "drvcust_if.h"


#include "tuner_interface_if.h"
#include "vdr_pi_def_dvbt.h"
//#include "PI_Def.h"
//#include "pi_demod_atd.h"
#include "pi_dvbt_if.h"   
#include "eq_script_re215.h"




#if 0
#define mcDBG_MSG_L2(_x_)             mcDBG_MSG_L2(_x_)
#define mcDBG_MSG_L2(_x_)           mcDBG_MSG_L2(_x_)
#define mcSHOW_OSAI_MSG(_x_)             mcDBG_MSG_L2(_x_)
#define mcSHOW_OSAIOK_MSG(_x_)           mcDBG_MSG_L2(_x_)
#define mcDBG_MSG_L2(_x_)           mcDBG_MSG_L2(_x_)
#define mcSHOW_DRVAPIOK_MSG(_x_)         mcDBG_MSG_L2(_x_)
#define mcSHOW_DRVLOCK_MSG(_x_)          mcDBG_MSG_L2(_x_)
#define mcSHOW_API_MSG(_x_)              mcDBG_MSG_L2(_x_)            
#define mcDBG_MSG_L2(_x_)   mcDBG_MSG_L4(_x_)
#define mcDBG_MSG_L2(_x_)   mcDBG_MSG_L2(_x_)
#define mcDBG_MSG_L2(_x_)    mcDBG_MSG_L2(_x_)
#define mcDBG_MSG_L21(_x_)   mcDBG_MSG_L1(_x_)
#define mcDBG_MSG_L23(_x_)   mcDBG_MSG_L4(_x_)

#define mcDBG_MSG_ERR(_x_)  	mcSHOW_USER_MSG(_x_)  
#define mcDBG_MSG_INFO(_x_)  	mcSHOW_USER_MSG(_x_) 
#define mcDBG_MSG_L1(_x_)  	mcSHOW_USER_MSG(_x_) 
#define mcDBG_MSG_L2(_x_)  	mcSHOW_USER_MSG(_x_) 			
#define mcDBG_MSG_L4(_x_)  	mcDBG_MSG_L2(_x_) 


#endif


// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_RE215_VERSION				"RE215 v1.02"
#include "re215_i2c_MTK.h"
#define C_RE215_PLL_POLL_INTERVAL   10   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_RE215_PLL_TIMEOUT         100
#define C_RE215_PLL_TIMEOUT_CNT     C_RE215_PLL_TIMEOUT/C_RE215_PLL_POLL_INTERVAL
#define C_RE215_I2C_CLK_DIV         0x100
#define C_RE215_PLL_LOCK_BIT        6

#define C_RE215_TOP_SET             ((U8)   0x02) 
#define C_RE215_TOP_SET_DVBC        ((U8)   0x04) 
#define C_RE215_TOP_SET_ANA         ((U8)   0x0A) 
#define C_RE215_SAW_BW              SAW_BW_8M 


#define C_RE215_DELAY_MS           50//change to 50ms to align with re215.
//#define C_RE215_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_RE215_FREQ_DBOUND_UPPER   859*1000*1000      //modified for DVB-C application, Menghau
#define C_RE215_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_RE215_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_RE215_FREQ_DBOUND_LOWER_Ana    45*1000*1000

//#define C_RE215_LO_ADDRESS         ((U16)  0xc0)
#define C_RE215_IF_FREQUENCY_DVBT_6M        ((U16)  3800)  /* kHz */  //JUST for 5.81M
#define C_RE215_IF_FREQUENCY_DVBT_7M        ((U16)  4350)  /* kHz */  //JUST for 6.8M
#define C_RE215_IF_FREQUENCY_DVBT_8M        ((U16)  4850)  /* kHz */  //JUST for 7.65M
#define C_RE215_IF_FREQUENCY_DVBT2_7M        ((U16)  4350)  /* kHz */  //JUST for 6.8M
#define C_RE215_IF_FREQUENCY_DVBT2_8M        ((U16)  4850)  /* kHz */  //JUST for 7.65M
#define C_RE215_IF_FREQUENCY_DVBC        ((U16)  5100)  /* kHz */  //JUST for 7.95M
#define C_RE215_IF_FREQUENCY_ATSC        ((U16)  3700)  /* kHz */  //JUST for 5.74M
#define C_RE215_IF_FREQUENCY_ISDBT        ((U16)  4000)  /* kHz */  //JUST for 5.95M


#define C_RE215_IF_FREQUENCY_PAL_DK   ((U16)  8200)  /* kHz */
//#define C_RE215_IF_FREQUENCY_PAL_I   ((U16)  8200)  /* kHz */
#define C_RE215_IF_FREQUENCY_PAL_BG   ((U16)  7300)  /* kHz */
#define C_RE215_IF_FREQUENCY_SECAM_L1   ((U16)  2200)  /* kHz */
//#define C_RE215_IF_FREQUENCY_SECAM_L   ((U16)  8200)  /* kHz */
#define C_RE215_IF_FREQUENCY_NTSC_M  ((U16)  5750)  /* kHz */

#define C_RE215_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_RE215_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_RE215_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_RE215_AGC_IF_SLP            -0.1407
#define C_RE215_AGC_IF_INT           0.4375    
#define C_RE215_AGC_IF_MAX           0.499  // 0.5 will overflow  //0v
#define C_RE215_AGC_IF_MIN          -0.0547  
#define C_RE215_AGC_IF_SLP_SGN  (S8)((C_RE215_AGC_IF_SLP>0)?1:-1) 
#define C_RE215_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_RE215_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256


#define C_RE215_MAX_IF_GAIN_SET       -0.12   
#define C_RE215_MIN_IF_GAIN_SET       -0.38   
#define C_RE215_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_RE215_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_RE215_MAX_IF_GAIN_POW       -100    // dBm
#define C_RE215_MIN_IF_GAIN_POW		-50     // dBm
#define C_RE215_MAX_RF_GAIN_POW		 C_RE215_MIN_IF_GAIN_POW
#define C_RE215_MIN_RF_GAIN_POW		-20     // dBm
#define C_RE215_POWER_CALIBRATE        26     // dBm
//#define C_RE215_SIGNAL_LVL_TH          6  //liuqu,20080926,no use for nimtool

#define C_RE215_AGC_COEF			((U8)   0xBD)
#define C_RE215_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC_0D36

// ********************************************* //

#define C_U8_RE215_AGC_IF_SLP         (S8)(C_RE215_AGC_IF_SLP *64 )  // MT5135(S(1, -6)), -0.125*64 = -8 (0xF8), -8/64 = -1/8
#define C_U8_RE215_AGC_IF_INT         (S8)(C_RE215_AGC_IF_INT *16 ) // MT5135(S(3, -4))  0.5*16 = 8, 8/16 =0.5
#define C_U8_RE215_AGC_IF_MAX         (S8)(C_RE215_AGC_IF_MAX *256)    //0.499*256 = 0x7F
#define C_U8_RE215_AGC_IF_MIN         (S8)(C_RE215_AGC_IF_MIN *256)    //0
#define C_U8_RE215_AGC_IF_LVL_MAX        (S8)(C_RE215_AGC_IF_LVL_MAX *256)
#define C_U8_RE215_AGC_IF_LVL_MIN         (S8)(C_RE215_AGC_IF_LVL_MIN *256)

#define C_U8_RE215_MAX_IF_GAIN_SET	(S8)(C_RE215_MAX_IF_GAIN_SET *256)
#define C_U8_RE215_MIN_IF_GAIN_SET	(S8)(C_RE215_MIN_IF_GAIN_SET *256)
#define C_U8_RE215_MAX_RF_GAIN_SET	(S8)((C_RE215_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_RE215_MIN_RF_GAIN_SET	(S8)((C_RE215_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_RE215_IF_GAIN_RANGE      (S8)(C_RE215_MIN_IF_GAIN_POW - C_RE215_MAX_IF_GAIN_POW)
#define C_U8_RE215_RF_GAIN_RANGE      (S8)(C_RE215_MIN_RF_GAIN_POW - C_RE215_MAX_RF_GAIN_POW)

STATIC UINT8* pRe215[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pRe215EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
#include "re215_ascot2d.h"


/*************ATD PRA config****************/

STATIC ATD_TUNER_REL_REG_T arrAtdRelRegPAL[]=
#if ((defined CC_MT5396) || (defined CC_MT5368))
{
    {0x7B7,0x10}, //PRA target H
    {0x7B9,0x10}, //PRA target L
    {0x790,0x48}, //DAGC setting
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC	disable.IF_AGC polarity inverse
    {0x794,0x38}, //DAGC Target level H
    {0x7A4,0x28}, //DAGC Target level L
    {0x7CA,0x88}, //PRA speed
    {0x00,0x00}    //end flag
};
#else
{
    {0x2B7,0x10},//PRA target H
    {0x2B9,0x10},//PRA target L
    {0x290,0x48},//DAGC setting
    {0x2DF,0x51},//AGC polarity inverse.RF_AGC  disable.IF_AGC polarity inverse
    {0x294,0x38},//DAGC Target level H
    {0x2A4,0x28},//DAGC Target level L
    {0x2CA,0x88},//PRA speed
    {0x00,0x00}//end flag
};
#endif

STATIC ATD_TUNER_REL_REG_T arrAtdRelRegSECAM[]=
#if ((defined CC_MT5396) || (defined CC_MT5368))
{
    {0x7B7,0x05}, //PRA target H
    {0x7B9,0x05}, //PRA target L
    {0x790,0xC8}, //DAGC setting
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC	disable.IF_AGC polarity inverse
    {0x794,0x02}, //DAGC Target level H
    {0x7A4,0x01}, //DAGC Target level L
    {0x7CA,0xBB}, //PRA speed
    {0x00,0x00}    //end flag
};
#else
{
    {0x2B7,0x05},//PRA target H
    {0x2B9,0x05},//PRA target L
    {0x290,0xc8},//DAGC setting
    {0x2DF,0x51},//AGC polarity inverse.RF_AGC  disable.IF_AGC polarity inverse
    {0x294,0x02},//DAGC Target level H
    {0x2A4,0x01},//DAGC Target level L
    {0x2CA,0xBB},//PRA speed
    {0x00,0x00}//end flag
};
#endif

STATIC ATD_TUNER_REL_REG_T arrAtdRelRegMN[]=
#if ((defined CC_MT5396) || (defined CC_MT5368))
{
    {0x7B7,0x10}, //PRA target H
    {0x7B9,0x10}, //PRA target L
    {0x790,0x08}, //DAGC setting
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC	disable.IF_AGC polarity inverse
    {0x794,0x38}, //DAGC Target level H
    {0x7A4,0x28}, //DAGC Target level L
    {0x7CA,0x88}, //PRA speed
    {0x00,0x00}    //end flag
};
#else
{
    {0x2B7,0x10},//PRA target H
    {0x2B9,0x10},//PRA target L
    {0x290,0x08},//DAGC setting
    {0x2DF,0x51},//AGC polarity inverse.RF_AGC  disable.IF_AGC polarity inverse
    {0x294,0x38},//DAGC Target level H
    {0x2A4,0x28},//DAGC Target level L
    {0x2CA,0x88},//PRA speed
    {0x00,0x00}//end flag
};

#endif
/*************ATD PRA config****************/


/***********************************************************************/
#include "re215_i2c_MTK.h"
#include "re215_ascot2d.h"
/*              Defines                                                */
/***********************************************************************/
#define C_RE215_LO_ADDRESS          0xC0
#define C_RE215_I2C_GATEWAY_ADDRESS 0xD8
#define C_RE215_I2C_GATEWAY_SUB     0x09

/*************************************************
 DVB-T tuning parameter struct
**************************************************/
typedef struct ascot_tuner_ctx
{
    re215_ascot_t tuner;
    re215_i2c_t i2c;
} ascot_tuner_ctx;

static ascot_tuner_ctx g_ascot_tuner_ctx;

/***********************************************************************/
/*              Conditional Compiler                                   */
/***********************************************************************/

/***********************************************************************/
/*              Global variables                                       */
/***********************************************************************/

/***********************************************************************/
/*              External declarations                                  */
/***********************************************************************/

/***********************************************************************/
/*              Private (static) Function Prototypes                   */
/***********************************************************************/

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/

UINT8 DIF_SWITCH =1;


///////////////////////////////////
// sony RE215 driver
///////////////////////////////////

CHAR* RE215_GetSwVer(void)
{
	return ((CHAR*)C_RE215_VERSION);
}


VOID X_SONY_RE215_TunerInit(ITUNER_CTX_T *pTCtx)
{
    re215_result_t re215_result = RE215_RESULT_OK;
    re215_ascot_t *pTuner = &g_ascot_tuner_ctx.tuner;
    re215_i2c_t *pI2c = &g_ascot_tuner_ctx.i2c;

    // This silicon tuner is connected via demodulator. (I2C Gateway)
    re215_i2c_MTK_CreateI2cGw(pI2c, C_RE215_I2C_GATEWAY_ADDRESS, C_RE215_I2C_GATEWAY_SUB);

    /* Xtal = 16MHz, Sub Address = 0xC0, IF/AGC1 for Digital, IF/AGC2 for Analog, Low Phase noise module */
    re215_ascot2d_Create(pTuner, 16, C_RE215_LO_ADDRESS, pI2c, RE215_ASCOT_CONFIG_IFAGCSEL_D1A2 | RE215_ASCOT_CONFIG_LOW_PN);
    /* NOTE: If the set is used in System-L region (like France), Please do as follows. */
    //re215_ascot2d_Create(pTuner, 16, C_RE215_LO_ADDRESS, pI2c, RE215_TUNER_CONFIG_IFAGCSEL_D1A2 | RE215_ASCOT_CONFIG_LOW_PN | RE215_ASCOT_CONFIG_SYSTEML_REGION);

    re215_result = re215_ascot2d_Initialize(pTuner);
    if(re215_result != RE215_RESULT_OK){
        mcDBG_MSG_L2(("ERROR: ascot2d_Initialize is failed. (%d)\n", re215_result));
    }
    
    mcDBG_MSG_L2(("Tuner RE201 init done!\n"));
}


//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID RE215_TunerInit(ITUNER_CTX_T *pTCtx)
{/**********do ATD config tuner init procedue************/
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);
#if ((defined CC_MT5396) )//|| (defined CC_MT5368)) 
	UINT32 TunerReset = DRVCUST_OptGet(eTunerCtrlAna);
    // Tuner Reset
   // GPIO_SetOut(TunerReset, 0); 
    mcDELAY_MS(100);
    GPIO_SetOut(TunerReset, 1);
   mcDELAY_MS(100);				
#endif

    //REGION region=COUNTRY_OTHER;
    psTunerCtx->u2LO_Step=100;  //*10hz
    psTunerCtx->I2cAddress = C_RE215_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBC;  //DVBT 8M
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = FALSE;

#if ((defined CC_MT5396) || (defined CC_MT5368)) 
    psTunerCtx->fgRFTuner = FALSE;	 // Low IF for DVB-T/C
#endif

    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x00;  
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    pEUCtx->m_SAW_BW  = C_RE215_SAW_BW;
    pEUCtx->m_Ana_Top = C_RE215_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_RE215_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_RE215_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_RE215_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_RE215_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_RE215_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_RE215_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_RE215_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_RE215_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_RE215_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_RE215_DELAY_MS;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_RE215_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_RE215_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_RE215_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)0xf2;//C_U8_RE215_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_RE215_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_RE215_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_RE215_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_RE215_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_RE215_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_RE215_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_RE215_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_RE215_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_RE215_POWER_CALIBRATE;
//IF MAX setting!
    pEUCtx->m_aucPara[21] = 0x7F; //xiangfu 20100526
    pUSCtx->u2LO_Step = 100; //*10HZ

    pRe215[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRE215BG;
    pRe215[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRE215DK;
    pRe215[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRE215I;
    pRe215[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRE215L;	
    pRe215[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRE215L1;
    pRe215[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQRE215M;

    pRe215EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRE215BGWeak;
    pRe215EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRE215DKWeak;
    pRe215EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRE215IWeak;
    pRe215EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRE215LWeak;	
    pRe215EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRE215L1Weak;
    pRe215EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]	= EQRE215MWeak;


/**********do SONY tuner init procedue************/
	X_SONY_RE215_TunerInit(pTCtx);

    
    mcDBG_MSG_L2(("Tuner RE215 init done!Version :%s\n",(CHAR *)C_RE215_VERSION));
}

 STATIC void  RE215_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcDBG_MSG_L2(("RE215_SetSawBw = %d\n", sawbw));
   
 }


S16 RE215_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{

    UINT32 Freq = param->Freq; // transfer to video freq(KHz)
    UINT8 Mode = param->Modulation;
    UINT8 bandWidth = pTCtx->specific_member.eu_ctx.m_SAW_BW;
	re215_result_t re215_result = RE215_RESULT_OK;
    re215_ascot_t *pTuner = &g_ascot_tuner_ctx.tuner;
	uint8_t data;
	/*********************ATD config for ATV***************/
	pTCtx->u4RF_Freq =param->Freq;

	if (Mode == MOD_PAL_BG)
		{
			pTCtx->u2IF_Freq_A = C_RE215_IF_FREQUENCY_PAL_BG;
		}
		else if ((Mode >MOD_PAL_BG) &&(Mode <MOD_SECAM_L1))
		{
			pTCtx->u2IF_Freq_A = C_RE215_IF_FREQUENCY_PAL_DK;
		}
		else if (Mode == MOD_SECAM_L1)	
		{
			pTCtx->u2IF_Freq_A = C_RE215_IF_FREQUENCY_SECAM_L1;
		}
		else if (Mode == MOD_NTSC_M)	
		{
			pTCtx->u2IF_Freq_A = C_RE215_IF_FREQUENCY_NTSC_M;
		}			
		
/*********************ATD config for ATV***************/


    switch(Mode){
    /* Digital */
    case MOD_DVBT:
        switch(bandWidth){
        case (UINT8)SAW_BW_6M:
            re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBT_6);
            break;
        case (UINT8)SAW_BW_7M:
            re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBT_7);
            break;
        case (UINT8)SAW_BW_8M:
            re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBT_8);
            break;
        }
        break;
    case MOD_DVBT2:
        switch(bandWidth){
        case (UINT8)SAW_BW_6M:
            re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBT2_6);
            break;
        case (UINT8)SAW_BW_7M:
            re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBT2_7);
            break;
        case (UINT8)SAW_BW_8M:
            re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBT2_8);
            break;
        }
        break;
    case MOD_DVBC:
        re215_result = re215_ascot2d_DigitalTune(pTuner, Freq, RE215_ASCOT_DTV_SYSTEM_DVBC);
        break;
    
    /* Analog */
    /* NOTE: Audio ID is dummy because it's not used in tuner driver source now. */
    case MOD_PAL_BG:
        re215_result = re215_ascot2d_AnalogTune(pTuner, Freq, RE215_ASCOT_ATV_VIDEO_BG,
            RE215_ASCOT_ATV_AUDIO_BG_DUAL_FM_STEREO);
        break;
    case MOD_PAL_I:
        re215_result = re215_ascot2d_AnalogTune(pTuner, Freq, RE215_ASCOT_ATV_VIDEO_I,
            RE215_ASCOT_ATV_AUDIO_I_NICAM_FM);
        break;
    case MOD_PAL_DK:
        re215_result = re215_ascot2d_AnalogTune(pTuner, Freq, RE215_ASCOT_ATV_VIDEO_DK,
            RE215_ASCOT_ATV_AUDIO_I_NICAM_FM);
        break;
    case MOD_SECAM_L:
        re215_result = re215_ascot2d_AnalogTune(pTuner, Freq, RE215_ASCOT_ATV_VIDEO_L,
            RE215_ASCOT_ATV_AUDIO_L_NICAM_AM);
        break;
    case MOD_SECAM_L1:
        re215_result = re215_ascot2d_AnalogTune(pTuner, Freq, RE215_ASCOT_ATV_VIDEO_L_DASH,
            RE215_ASCOT_ATV_AUDIO_L_NICAM_AM);
        break;
		case MOD_NTSC_M:   //added 20111125
				re215_result = re215_ascot2d_AnalogTune(pTuner, Freq, RE215_ASCOT_ATV_VIDEO_M,
		 				RE215_ASCOT_ATV_AUDIO_N_BTSC);
				mcDBG_MSG_L2(("ATV tuner MN\n"));
				break;

    default:
        mcDBG_MSG_ERR(("Unknown MOD_XXX type! \n"));
        return 1;
    }

    if(re215_result == RE215_RESULT_OK){
       
		pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x05, &data, 1);

			mcDBG_MSG_L4(("%s-SetFreq Success 05=%02x! \n",(UINT8 *)C_RE215_VERSION,data));

		return 0;
    }else{
        mcDBG_MSG_ERR(("%s-SetFreq Fail!\n",(UINT8 *)C_RE215_VERSION));
        return 1;
    }
}

INT16 RE215_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
   
    switch(eOperation){
    case itGetVer:
        {
            CHAR ** RetStr = (CHAR **)pInOutVal;
            *RetStr = RE215_GetSwVer();
        }
        break;
    case itSetTop:
        {
            SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
            pTunerCtx->m_aucPara[0] = SetVal;
        }
        break;
		
    case itSetReg:
        mcDBG_MSG_L2(("Not implement in SONY RE215 Tuner\n"));
        break;
		
    case itGetReg:
        {        
            mcDBG_MSG_L2(("Not implement in SONY RE215 Tuner\n"));	   
        }
        break;
    case itSetSawBw:
        RE215_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
        mcDBG_MSG_L2(("Not implement in SONY RE215 Tuner\n"));
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pRe215[SetVal];;
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pRe215EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
        
    case itSetIfMax :	
        {
            SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
            pTunerCtx->m_aucPara[21] = SetVal;
            break;
        } 
    case itSetIfMin:
        {
            UINT8 *ifMinPtr = (UINT8*)pInOutVal;			
            mcDBG_MSG_L2(("RE215 TOP Setting\n"));
            *ifMinPtr = 0xf2;
        }
        break;	
#if 0		
    case itGetAGCPol :
        {
            if(C_RE215_AGC_IF_SLP == 1)
            { 
                *(UINT8*)pInOutVal=0;
            }
            else
            { 
                *(UINT8*)pInOutVal=1;	
            }
        }
        break;
#endif
    case itGetTHC:
        {
            UINT8 subSysId = SetVal;
            UINT8 *ifThcPtr = (UINT8*)pInOutVal;
            
            switch(subSysId)
            {
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
#if 0
    case itGetTagt:
        { 
            mcDBG_MSG_L2(("RE215 Target Setting\n"));
            UINT8 subSysId = SetVal;
            UINT8 *ifThcPtr = (UINT8*)pInOutVal;
            
            switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                *ifThcPtr = 0x05;
                break;
            default:
                *ifThcPtr = 0x10;
                break;
            }
        }
        break;
#endif
    case itSetIF:
        break;

    case itGetTunerRelReg://Get ATD Tuner Rel register setting
        {
            UINT8 subSysId = SetVal;
            
            switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
     *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelRegSECAM;
                break;
            case MOD_NTSC_M:
     *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelRegMN;
                break;
            default://PAL
	 *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelRegPAL;

                break;
            }
        }
        break;

    default:
        mcDBG_MSG_L2(("NO case !Not implement in SONY RE215 Tuner.\n"));
        return ITUNER_NOT_SUPPORTED;
    }
	
    return ITUNER_OK;
}

// RE215 Function trace (For Debug)
void re215_trace_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    mcDBG_MSG_L4(("RE215_ENTER : %s (%d)\n", funcname, linenum));
}

void re215_trace_log_return(re215_result_t result, const char* filename, unsigned int linenum)
{
    if(result != RE215_RESULT_OK){
        mcDBG_MSG_L4(("RE215_ERROR_RETURN(%d) : (line %d)\n", result, linenum));
    }else{
        mcDBG_MSG_L4(("RE215_RETURN : (line %d)\n", linenum));
    }
}

// Function trace for I2C functions (For Debug)
void re215_trace_i2c_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    re215_trace_log_enter(funcname, filename, linenum);
}
void re215_trace_i2c_log_return(re215_result_t result, const char* filename, unsigned int linenum)
{
    re215_trace_log_return(result, filename, linenum);
}
