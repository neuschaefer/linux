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


#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"

#include "sif_if.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "eq_script_re215.h"



// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_RE215_VERSION				"RE215 v1.02"

//#define C_RE215_PLL_POLL_INTERVAL   5
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
static UINT8 Re215DelayMs ;  

//#define C_RE215_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_RE215_FREQ_DBOUND_UPPER   859*1000*1000      //modified for DVB-C application, Menghau
#define C_RE215_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_RE215_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_RE215_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_RE215_LO_ADDRESS         ((U16)  0xc0)
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
#define C_RE215_AGC_IF_MAX           0.998  // 0.5 will overflow  //0v
#define C_RE215_AGC_IF_MIN          -0.1094 //-0.0547  
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
#define C_U8_RE215_AGC_IF_MAX         (S8)(C_RE215_AGC_IF_MAX *128)    //0.499*256 = 0x7F 0,998*128 = 0x7F
#define C_U8_RE215_AGC_IF_MIN         (S8)(C_RE215_AGC_IF_MIN *128)    //-0.0547 *2 = -0.1094
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


/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define SONY_RE215_SUBADDR_INIT1            ((UINT8) 0x01)
#define SONY_RE215_SUBADDR_INIT2            ((UINT8) 0x04)
#define SONY_RE215_SUBADDR_IFAGC            ((UINT8) 0x05)
#define SONY_RE215_SUBADDR_WAKSLEEP         ((UINT8) 0x14)
#define SONY_RE215_SUBADDR_TUNE1            ((UINT8) 0x06)
#define SONY_RE215_SUBADDR_TUNE2            ((UINT8) 0x0B)
#define SONY_RE215_SUBADDR_TUNE3            ((UINT8) 0x10)
#define REGNUM    9

#define MAX_LENGTH_TUNER_WRITE 10
#define DVBT_SLAVE_ADDRESS     0xd8
#define TUNER_SLAVE_ADDRESS     C_RE215_LO_ADDRESS

#define PRA_TARGET_H_INDEX 0
#define PRA_TARGET_L_INDEX 1
#define SLD_DAGC_00_INDEX 2
#define DAGC_TARGET_LEVEL_H_INDEX  4
#define DAGC_TARGET_LEVEL_L_INDEX  5
#define PRA_LOOP_CONF_INDEX  6


/*************************************************
 DVB-T tuning parameter struct
**************************************************/
typedef struct {
    unsigned char  m_mode;          /* mode 0:zapping 1:scannig */
    unsigned long  m_frequency;     /* center frequency */
    unsigned char  m_BandWidth;     /* 0:N/A 1:6MHzBandWidth 2:7MHzBandWidth 3:8MHzBandWidth */
    unsigned char  m_hierarchy;     /* 0:High Priority 1:Low Priority */
    unsigned char  m_CoexistL;      /* 0:not coexistence of System-L/L' , 1:coexistence of System-L/L' */
    UINT8 system;	  //add by SONY/AEC 20100406
} TuningParam;

/*************************************************
 Number of demod setting
**************************************************/
#define RF_REGSETTING_MAX_ENTRY          12

static UINT8 DVBT_IF_PORT=0x0C;
static UINT8 DVBC_IF_PORT=0x0C;
static UINT8 DVBT2_IF_PORT=0x00;
static UINT8 ANALOG_IF_PORT=0x0C; 

//static UINT8 DIF_SWITCH =1;






static const UINT8
RfRegSetting_ATSC_8VSB[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xD6, 0x1A, 
    0x00,
};

static const UINT8
RfRegSetting_US_QAM[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xD6, 0x1A, 
    0x00,
};

static const UINT8
RfRegSetting_ISDBT[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0x06, 0x00, 
    0x00,
};

static const UINT8
RfRegSetting_DVBT_6M[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xF6, 0x1C, 
    0x00,
};

static const UINT8
RfRegSetting_DVBT_7M[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xEE, 0x00, 
    0x10,
};

static const UINT8
RfRegSetting_DVBT_8M[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xEE, 0x00, 
    0x20,
};

static const UINT8  
RfRegSetting_DVBT2_7M[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xD6, 0x1E, 
    0x10,
};

static const UINT8
RfRegSetting_DVBT2_8M[RF_REGSETTING_MAX_ENTRY] = {
    0x05, 0x00, 0x00,
    0x04, 0x10, 0x0E,
    0x25, 0x13, 0x20, 0xD6, 0x1D, 
    0x20,
};

static const UINT8
RfRegSetting_DVBC[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x12, 0x20, 0xFB, 0x03, 
    0x20,
};

//change for re215,
static const UINT8
RfRegSetting_MN[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x12, 0x20, 0x03, 0x00, 
    0x80,
};

static const UINT8
RfRegSetting_BG[RF_REGSETTING_MAX_ENTRY] = {
    0x02, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x12, 0x20, 0x5B, 0x08, 
    0x90,
};

static const UINT8
RfRegSetting_I[RF_REGSETTING_MAX_ENTRY] = {
    0x02, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x12, 0x20, 0x4B, 0x07, 
    0xA0,
};

static const UINT8
RfRegSetting_DK[RF_REGSETTING_MAX_ENTRY] = {
    0x02, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x12, 0x20, 0x4B, 0x07, 
    0xA0,
};

static const UINT8
RfRegSetting_L[RF_REGSETTING_MAX_ENTRY] = {
    0x02, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x15, 0x20, 0x49, 0x07, 
    0xA0,
};

static const UINT8
RfRegSetting_L_DASH[RF_REGSETTING_MAX_ENTRY] = {
    0x02, 0x00, 0x00,
    0x10, 0x10, 0x02,
    0x0F, 0x15, 0x20, 0xF9, 0x07, 
    0xE0,
};


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
//static UINT8* RE215_TunerSpec(ITUNER_CTX_T *pTCtx ,UINT8 Mode);
static void tuner_init(void);
static const UINT8* setRfTunerParam(unsigned char bandWidth, UINT8 system );
static void tuner_tune(TuningParam* pDigParam);
static void tuner_wakeup(void);
static void tuner_sleep(void);
static int iic_write(unsigned char subadr, unsigned char *data, unsigned char len);
static int iic_write_val( unsigned char subadr, unsigned char val);
static void Waitms(int ms); 

STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]=
#if ((defined CC_MT5396) || (defined CC_MT5368)|| (defined CC_MT5389))
{
    {0x7B7,0x10}, //PRA target H
    {0x7B9,0x10}, //PRA target L
    {0x790,0x00}, //DAGC setting
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
    {0x290,0x00},//DAGC setting
    {0x2DF,0x51},//AGC polarity inverse.RF_AGC  disable.IF_AGC polarity inverse
    {0x294,0x38},//DAGC Target level H
    {0x2A4,0x28},//DAGC Target level L
    {0x2CA,0x88},//PRA speed
    {0x00,0x00}//end flag
};
#endif

/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/


///////////////////////////////////
// sony RE215 driver
///////////////////////////////////

CHAR* RE215_GetSwVer(void)
{
	return ((CHAR*)C_RE215_VERSION);
}

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID RE215_TunerInit(ITUNER_CTX_T *pTCtx)
{

	

    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);
	/*/////////////////////////
	GPIO_SetOut(729, 0); 
	mcDELAY_MS(100);
	GPIO_SetOut(729, 1);
	mcDELAY_MS(100);         
	//////////////////////////just for turnkey 5365 board*/


#if ((defined CC_MT5396) )//|| (defined CC_MT5368)) 
	UINT32 TunerReset = DRVCUST_OptGet(eTunerCtrlAna);
    // Tuner Reset
    GPIO_SetOut(TunerReset, 0); 
    mcDELAY_MS(100);
    GPIO_SetOut(TunerReset, 1);
   mcDELAY_MS(100);				
#endif
	//eTunerCtrlAna



    //DemodHwReset(100);
    //GPIO_SetOut(35, 1);

    //REGION region=COUNTRY_OTHER;
    psTunerCtx->u2LO_Step=100;  //*10hz
    psTunerCtx->I2cAddress = C_RE215_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBC;  //DVBT 8M
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = FALSE;

#if ((defined CC_MT5396) || (defined CC_MT5368)|| (defined CC_MT5389)) 
    psTunerCtx->fgRFTuner = FALSE;	 // Low IF for DVB-T/C
#endif

    // iven 20110706, add for polarity if setting for NA reference
    psTunerCtx->bSpecPolarity = 0;
    // iven 20110706, add for legacy tuner as default fixed PGA gain index. Don care for Sawless
    psTunerCtx->uSLD_DefaultPGAIndex = 0;		

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
    
    pRe215[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQRE215M;
    pRe215[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRE215BG;
    pRe215[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRE215DK;
    pRe215[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRE215I;
    pRe215[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRE215L;	
    pRe215[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRE215L1;
    
    pRe215EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]	= EQRE215MWeak;
    pRe215EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRE215BGWeak;
    pRe215EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRE215DKWeak;
    pRe215EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRE215IWeak;
    pRe215EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRE215LWeak;	
    pRe215EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRE215L1Weak;
    
   

// do tuner init procedue

#ifdef CC_MT5135
	 DVBT_IF_PORT=0x00;
	 DVBC_IF_PORT=0x00;
	// DVBT2_IF_PORT=0x00;
	 ANALOG_IF_PORT=0x0C; 
#endif


    tuner_init();

    mcSHOW_USER_MSG(("Tuner RE215 init done!Version :%s\n",(CHAR *)C_RE215_VERSION));
}


 STATIC void  RE215_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("RE215_SetSawBw = %d\n", sawbw));
   
 }


S16 RE215_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    TuningParam TuningParam;
  	
    TuningParam.m_BandWidth = pTCtx->specific_member.eu_ctx.m_SAW_BW;
    TuningParam.m_CoexistL = 0;  
    TuningParam.m_frequency = param->Freq;
    TuningParam.m_mode = param->fgAutoSearch;
    TuningParam.system = param->Modulation;
	
    UINT8  Mode = param->Modulation;
	
    SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);

    if(Mode == MOD_DVBT)
    {
        if(TuningParam.m_BandWidth == SAW_BW_6M)
            pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBT_6M;
        else if(TuningParam.m_BandWidth == SAW_BW_7M)
            pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBT_7M;
        else 
            pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBT_8M;
    }
    else if(Mode == MOD_DVBT2)
    {
        if(TuningParam.m_BandWidth == SAW_BW_7M)
            pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBT2_7M;
        else 
            pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBT2_8M;
    }
    else if(Mode == MOD_DVBC)
    {
        pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_DVBC;
    }
    else if(Mode < MOD_DVBT)
    {
        pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_ATSC;
    }
    else if(Mode == MOD_ISDBT)
    {
        pTCtx->u2IF_Freq = C_RE215_IF_FREQUENCY_ISDBT;
    }
    else if (Mode == MOD_PAL_BG)
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
	
    pTCtx->u4RF_Freq = TuningParam.m_frequency;
	
    tuner_wakeup();
    //   Wakeup Data Setting
	
    //mcDELAY_MS(100);
    mcSHOW_DBG_MSG(("SONY_RE215_SetFreq\n"));
	
    Re215DelayMs  =  pTunerCtx->m_aucPara[0];
    tuner_tune(&TuningParam);
	
    //mcDELAY_MS(250);

    mcSHOW_DBG_MSG(("[%s]Freq = %d KHz, Mode =%d, setting PLL success\n" , (CHAR *)C_RE215_VERSION,TuningParam.m_frequency,Mode));	
    return (0);
}

INT16 RE215_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{   
    switch(eOperation)
    {
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
        mcSHOW_DBG_MSG(("Not implement in SONY RE215 Tuner\n"));
        break;
		
    case itGetReg:
        {        
            mcSHOW_DBG_MSG(("Not implement in SONY RE215 Tuner\n"));	   
        }
        break;
    case itSetSawBw:
        RE215_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
        mcSHOW_DBG_MSG(("Not implement in SONY RE215 Tuner\n"));
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
            mcSHOW_DBG_MSG(("RE215 TOP Setting\n"));
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
            mcSHOW_DBG_MSG(("RE215 Target Setting\n"));
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
            UINT8 u1PRATar,u1Dagc,u1DagcTarH,u1DagcTarL,u1PRAConf;
	  
            u1PRATar =0x10;
            u1DagcTarH=0x38;
            u1DagcTarL=0x28;
            u1PRAConf=0x88;
	  
            switch(subSysId)
            {
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                u1PRATar =0x05;
                u1Dagc=0xC8;
                u1DagcTarH=0x02;
                u1DagcTarL=0x01;
                u1PRAConf=0xBB;
                break;
            case MOD_NTSC_M:
                u1Dagc=0x08;
                break;
            default://PAL
                u1Dagc=0X48;
                break;
            }
            //PRA target	
            arrAtdRelReg[PRA_TARGET_H_INDEX].u1RegVal=u1PRATar;
            arrAtdRelReg[PRA_TARGET_L_INDEX].u1RegVal=u1PRATar;
            arrAtdRelReg[SLD_DAGC_00_INDEX].u1RegVal=u1Dagc;
            arrAtdRelReg[DAGC_TARGET_LEVEL_H_INDEX].u1RegVal=u1DagcTarH;
            arrAtdRelReg[DAGC_TARGET_LEVEL_L_INDEX].u1RegVal=u1DagcTarL;
            arrAtdRelReg[PRA_LOOP_CONF_INDEX].u1RegVal=u1PRAConf;
	  
            *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg;
        }
        break;
    default:
        mcSHOW_DBG_MSG2(("NO case !Not implement in SONY RE215 Tuner.\n"));
        return ITUNER_NOT_SUPPORTED;
    }
	
    return ITUNER_OK;
}

//sony code

/******************************************************************************
 * write_tuner
 *-----------------------------------------------------------------------------
 * unsigned char    slv0    : Demod Slave Address (0xD8 / 0xDA)
 * unsigned char    slv1    : Tuner Slave Addressr (0xC0 / 0xC2)
 * unsigned char    subadr  : Sub Address
 * unsigned char*   data    : pointer to data buffer
 * unsigned char    len     : length
 * 
 * retrun                   : 
 *****************************************************************************/
static int write_tuner(
    unsigned char slv0,
    unsigned char slv1,
    unsigned char subadr,
    unsigned char *data,
    unsigned char len
){
    unsigned char senddata[ MAX_LENGTH_TUNER_WRITE+6];//chuanjin for Klocwork buffer overflow check.2012-12-14
    int i;
#ifdef SONY_DEMOD 
    senddata[0] = (unsigned short)(0x09);
    senddata[1] = slv1;
    senddata[2] = subadr;

    if( len <= MAX_LENGTH_TUNER_WRITE ) {
        for(i = 0; i < len; i++){
            senddata[i+3] = data[i];  
        }
	 
        /*------------------------------------------------
            Add code here for I2C communication
        ------------------------------------------------*/
       ICtrlBus_I2cTunerWrite(C_RE215_I2C_CLK_DIV,slv0,senddata,len+3);       
        return 0;
		
    } 
#else
senddata[0] = subadr;

    if( len <= MAX_LENGTH_TUNER_WRITE ) {
        for(i = 0; i < len; i++){
            senddata[i+1] = data[i];  
        }
	 
        /*------------------------------------------------
            Add code here for I2C communication
        ------------------------------------------------*/
       ICtrlBus_I2cTunerWrite(C_RE215_I2C_CLK_DIV,slv1,senddata,len+1);
       
        return 0;
		
    }
#endif
	else {
        printf("write_tuner error : length value is over MAX_LENGTH_TUNER_WRITE! \n");
        return 1;
    }
	
}


/******************************************************************************
 * iic_write
 *-----------------------------------------------------------------------------
 * int              id      : Block ID 
 * unsigned char    subadr  : sub address
 * unsigned char*   data    : pointer to data buffer to send
 * unsigned char    len     : data byte length to send
 * 
 * retrun                   : 
 *****************************************************************************/
static int iic_write(unsigned char subadr, unsigned char *data, unsigned char len)
{ 
    write_tuner( DVBT_SLAVE_ADDRESS, (unsigned char)TUNER_SLAVE_ADDRESS, subadr, data, len );
    return 0;
}

/******************************************************************************
 * iic_write_val
 *-----------------------------------------------------------------------------
 * int              id      : Block ID 
 * unsigned char    subadr  : sub address
 * unsigned char    val     : data value
 * 
 * retrun                   : 
 *****************************************************************************/
static int iic_write_val( unsigned char subadr, unsigned char val)
{
    iic_write(subadr, &val, 1);
    return 0;
}

/******************************************************************************
 * analog_tuner_wakeup
 *-----------------------------------------------------------------------------
 * Input        : none
 * Output       : none
 * Return       : none
 * Description  : Terrestrial Digital Awake
 *****************************************************************************/

static void tuner_wakeup(void)
{
    unsigned char taddr;
    unsigned char tdata[2];
    unsigned char data_length;

    mcSHOW_DBG_MSG(("Enter analog_tuner_wakeup\n"));

    taddr    = 0x14;
    tdata[0] = 0xFB;    /* 0x14 */
    tdata[1] = 0x0F;    /* 0x15 */
    data_length = 2;
    iic_write( taddr, tdata, data_length );

    iic_write_val( 0x05,0x0c ); //IF2 & AGC2, add for RE215 20101214
	  
    return;
}


/******************************************************************************
 * analog_tuner_sleep
 *-----------------------------------------------------------------------------
 * Input        : none
 * Output       : none
 * Return       : none
 * Description  : stop tuning
 *****************************************************************************/

static void tuner_sleep(void)
{
   unsigned char taddr;
    unsigned char tdata[2];
    unsigned char data_length;

    mcSHOW_DBG_MSG(("Enter analog_tuner_sleep\n"));

    taddr    = 0x14;
    tdata[0] = 0x00;    /* 0x14 */
    tdata[1] = 0x04;    /* 0x15 */
    data_length = 2;
    iic_write( taddr, tdata, data_length );

    return;
}

 
 /******************************************************************************
 * Waitms
 *-----------------------------------------------------------------------------
 * Input : int ms
 * Output: none
 * Return: void
 *****************************************************************************/
 void Waitms(int ms) 
 {
     mcDELAY_MS(ms);
        /*------------------------------------------------
            Add code for wait ( milliseconds unit )
        ------------------------------------------------*/

    return;
}

/******************************************************************************
 * tuner_init
 *-----------------------------------------------------------------------------
 * Input        : none
 * Output       : none
 * Return       : none
 * Description  : setting output ports HiZ, disable all functions.
 *****************************************************************************/
static void tuner_init(void)
{
#if 0 // merge re215
mcSHOW_DBG_MSG(("Enter tuner_init\n"));

    iic_write_val( 0x01, 0x04 );
    iic_write_val( 0x04, 0x40 );
    iic_write_val( 0x14, 0xFB );
    iic_write_val( 0x21, 0x18 );

    mcDELAY_MS(100);
    iic_write_val(0x07, 0x04 );    /* 2010-05-28 add */
    iic_write_val(0x04, 0x00 );    /* 2010-05-28 add */
    iic_write_val(0x03, 0xC0 );	   /* 2010-05-28 add */
    tuner_sleep();
#else
/***************************************/
    unsigned char t_addr;  //add for RE215 20101214
    unsigned char t_data[5]; //add for RE215 20101214
    unsigned char  data_length; //add for RE215 20101214

    mcSHOW_DBG_MSG(("Enter tuner_init\n"));
    iic_write_val( 0x01, 0x10 );//change for RE215 20101214
    iic_write_val( 0x04, 0x40 );
    iic_write_val( 0x14, 0xFB );
    iic_write_val( 0x21, 0x18 );

//add for RE215 20101214	
    t_addr    = 0x23;
    t_data[0] = 0x10;
    t_data[1] = 0x10;
    t_data[2] = 0x10;
    t_data[3] = 0x10;
    t_data[4] = 0x10;
    data_length = 5;
    iic_write( t_addr , t_data, data_length );
    iic_write_val(  0x4E, 0x01 );	
//end add	
    mcDELAY_MS(100);

    iic_write_val(  0x07, 0x10 );    //change for RE215 20101214
//add for RE215 20101214    
    t_addr    = 0x04;
    t_data[0] = 0x00;
    t_data[1] = 0x0C;
    data_length = 2;
    iic_write(  t_addr , t_data, data_length );

    iic_write_val(0x03, 0xC0);

//end add

    tuner_sleep();
#endif

/***************************************/
}

/******************************************************************************
 * terr_tuner_setparam
 *-----------------------------------------------------------------------------
 * Input        : RECEIVE_MODE m_mode
 * Output       : none
 * Return       : UINT8 *param
 * Description  : select RF Data
 *****************************************************************************/
static const UINT8* setRfTunerParam(unsigned char bandWidth,  UINT8 system )
{

    const UINT8* buf;

    switch(system)
    {
    case MOD_ATSC_8VSB:              
        buf = RfRegSetting_ATSC_8VSB; 
        break;
    case MOD_J83B_256QAM:
    case MOD_J83B_AUTO_QAM:
    case MOD_J83B_64QAM:             
        buf = RfRegSetting_US_QAM;    
        break;
    case MOD_ISDBT:	                 
        buf = RfRegSetting_ISDBT;	   
        break;
    case MOD_DVBT:
        {
            if(bandWidth==SAW_BW_6M)      
                buf = RfRegSetting_DVBT_6M;  
            else if(bandWidth==SAW_BW_7M) 
                buf = RfRegSetting_DVBT_7M;   
            else	                     
                buf = RfRegSetting_DVBT_8M;
        }   
	break;
				
    case MOD_DVBT2:  
        {
            if(bandWidth==SAW_BW_7M)      
                buf = RfRegSetting_DVBT2_7M;   
            else	                     
                buf = RfRegSetting_DVBT2_8M;   
            break;
        }
				
    case MOD_DVBC: 	                 
        buf = RfRegSetting_DVBC;	   
        break;  
    /*case MOD_DTMB:	                 buf = RfRegSetting_DTMB; 	 break; */
	
    case MOD_NTSC_M:		         
        buf = RfRegSetting_MN; 	   
        break;
    case MOD_PAL_BG:		         
        buf = RfRegSetting_BG; 	   
        break;
    case MOD_PAL_DK:		         
        buf = RfRegSetting_DK; 	   
        break;
    case MOD_PAL_I:		             
        buf = RfRegSetting_I;		   
        break;
    case MOD_SECAM_L:		         
        buf = RfRegSetting_L;	   
        break;
    case MOD_SECAM_L1:	             
        buf = RfRegSetting_L_DASH;    
        break;
		
    default:
        printf("m_mode is Illegal Params.");
        buf = RfRegSetting_BG; 
        break;
    }
    return (const UINT8*) buf;
}


/******************************************************************************
 * terr_tuner_tune
 *-----------------------------------------------------------------------------
 * Input        : TuningParam* pParam
 * Output       : none
 * Return       : none
 * Description  : Terrestrial Tuning
 *****************************************************************************/
static void tuner_tune(TuningParam* pDigParam)
{
    UINT8 t_addr;//,i;
    UINT8 t_data[5];
    UINT8 data_length;
    const UINT8* param;
    unsigned long frequency;

	frequency = pDigParam->m_frequency;

	//set if port
#if 0 //change by SONY AEC 20101228	
    t_addr    = 0x05;
	if(DIF_SWITCH ==1)
		{if(pDigParam->system < MOD_ANA_TYPE_BEGIN)
            {iic_write_val(0x05, 0x0C ); 
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x0C));}
	    else
	        {iic_write_val(0x05, 0x00 );
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x00));}}
	else
		{if(pDigParam->system < MOD_ANA_TYPE_BEGIN)
            {iic_write_val(0x05, 0x00 ); 
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x00));}
	    else
	        {iic_write_val(0x05, 0x0C );
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x0C));}}
#endif
	if(pDigParam->system == MOD_DVBT)
      {iic_write_val(0x05, DVBT_IF_PORT); 
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",DVBT_IF_PORT));}
	else if (pDigParam->system == MOD_DVBC)
      {iic_write_val(0x05, DVBC_IF_PORT); 
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",DVBC_IF_PORT));}
	else if (pDigParam->system == MOD_DVBT2)
		{iic_write_val(0x05, DVBT2_IF_PORT); 
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",DVBT2_IF_PORT));}
	else
        {iic_write_val(0x05, ANALOG_IF_PORT); 
		    mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",ANALOG_IF_PORT));}

			
    //set param
    param = setRfTunerParam( pDigParam->m_BandWidth,pDigParam->system);
    mcSHOW_DBG_MSG(("Param0: 0x%2x - 0x%2x - 0x%2x\n", param[0], param[1], param[2]));
    mcSHOW_DBG_MSG(("Param1: 0x%2x - 0x%2x - 0x%2x\n", param[3], param[4], param[5]));
    mcSHOW_DBG_MSG(("Param2: 0x%2x - 0x%2x - 0x%2x - 0x%2x - 0x%2x\n", param[6], param[7], param[8], param[9], param[10]));
    mcSHOW_DBG_MSG(("Param3: 0x%2x\n", param[11]));

#if 0   // merge RE215 Tuner	
    t_addr    = 0x06;
    t_data[0] = param[0];
    t_data[1] = param[1];
    t_data[2] = param[2];
    data_length = 3;
    iic_write( t_addr, t_data, data_length );
	

    t_addr    = 0x0B;
    t_data[0] = param[3];
    t_data[1] = param[4];
    t_data[2] = param[5];
    t_data[3] = param[6];
    t_data[4] = param[7];	
    data_length = 5;
    iic_write(  t_addr, t_data, data_length );

    t_addr    = 0x03;                                        
    t_data[0] = 0xC4;                                        
    t_data[1] = 0x40;                                       
    data_length = 2;                                        
    iic_write( t_addr, t_data, data_length );

    t_addr    = 0x10;
    t_data[0] = ((frequency & 0x0000ff));
    t_data[1] = ((frequency & 0x00ff00) >>  8);
    t_data[2] = ((frequency & 0x0f0000) >> 16) + param[8]; 
    t_data[3] = 0xff;
    t_data[4] = 0xff;
    data_length = 5;
    iic_write( t_addr, t_data, data_length);

#else
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

    t_addr    = 0x49;
    t_data[0] = param[0];
    t_data[1] = param[1];
    t_data[2] = param[2];
    data_length = 3;
    iic_write( t_addr, t_data, data_length );

    t_addr    = 0x06;
    t_data[0] = param[3];
    t_data[1] = param[4];
    t_data[2] = param[5];
    data_length = 3;
    iic_write( t_addr, t_data, data_length );

    t_addr    = 0x0B;
    t_data[0] = param[6];
    t_data[1] = param[7];
    t_data[2] = param[8];
    t_data[3] = param[9];
    t_data[4] = param[10];
    data_length = 5;
    iic_write(  t_addr, t_data, data_length );

    t_addr    = 0x03;                                        /* 2010-05-14 add */
    t_data[0] = 0xC4;                                        /* 2010-05-14 add */
    t_data[1] = 0x40;                                        /* 2010-05-14 add */
    data_length = 2;                                         /* 2010-05-14 add */
    iic_write(  t_addr, t_data, data_length );/* 2010-05-14 add */

    t_addr    = 0x10;
    t_data[0] = ((frequency & 0x0000ff));
    t_data[1] = ((frequency & 0x00ff00) >>  8);
    t_data[2] = ((frequency & 0x0f0000) >> 16) + param[11]; 
    t_data[3] = 0xff;
    t_data[4] = 0xff;
    data_length = 5;
    iic_write( t_addr, t_data, data_length);

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#endif
	
    if (pDigParam->system  < MOD_PAL_BG)
    {
        Waitms(100);	
        mcSHOW_DBG_MSG(("Tuner tune delay 100 MS.\n"));	 
    }
    else
    {
        Waitms(Re215DelayMs);
        mcSHOW_DBG_MSG(("Tuner tune delay %d MS.\n",Re215DelayMs));	
    }
  
   // if(pDigParam->m_CoexistL == 1) 	  //France
   // 	{
#if 0	
    	if(pDigParam->system ==MOD_DVBT)
	      iic_write_val( 0x0C, 0x03 );  
		else if(pDigParam->system ==MOD_DVBT2)
	      iic_write_val( 0x0C, 0x03 );
		else if(pDigParam->system ==MOD_DVBC)
	      iic_write_val( 0x0C, 0x02 );
		else if((pDigParam->system >= MOD_PAL_BG)&&(pDigParam->system <= MOD_PAL_I))
	      iic_write_val( 0x0C, 0x02 );
    	}
       if((pDigParam->system >= MOD_SECAM_L)&&(pDigParam->system <= MOD_SECAM_L1))
	      iic_write_val( 0x0C, 0x04 );
#else      //   change by SONY AEC 20101228
    iic_write_val( 0x0C, (param[7] & 0xCF) );
 // }	      
#endif	   
    iic_write_val( 0x04, 0x00 );        
    iic_write_val( 0x03, 0xC0 );       
    return;
}


