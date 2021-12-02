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
 * $RCSfile: pi_tuner_RA217.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_ra217.c
 *  SONY RA217 tuner driver.
 */

#include "tuner_interface_if.h"
#if (!defined(CC_DUMMY_RA217))
#if (!defined(DTD_INCLUDE))
#include "pi_def_dvbt.h"
#endif
#endif
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"

#include "sif_if.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "eq_script_ra217.h"


// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_RA217_VERSION				"RA217 v1.01 2011-11-07"

//#define C_RA217_PLL_POLL_INTERVAL   5
#define C_RA217_PLL_POLL_INTERVAL   10   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_RA217_PLL_TIMEOUT         100
#define C_RA217_PLL_TIMEOUT_CNT     C_RA217_PLL_TIMEOUT/C_RA217_PLL_POLL_INTERVAL
#define C_RA217_I2C_CLK_DIV         0x100
#define C_RA217_PLL_LOCK_BIT        6

#define C_RA217_TOP_SET             ((U8)   0x02) 
#define C_RA217_TOP_SET_DVBC        ((U8)   0x04) 
#define C_RA217_TOP_SET_ANA         ((U8)   0x0A) 



#define C_RA217_DELAY_MS           50//change to 50ms to align with RA217.
static UINT8 RA217DelayMs ;  

//#define C_RA217_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_RA217_FREQ_DBOUND_UPPER   862*1000*1000      //modified for DVB-C application, Menghau
#define C_RA217_FREQ_DBOUND_LOWER    47*1000*1000 
#define C_RA217_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_RA217_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_RA217_LO_ADDRESS         ((U16)  0xc0)
#define C_RA217_IF_FREQUENCY_DVBT_6M        ((U16)  3800)  /* kHz */  //JUST for 5.81M
#define C_RA217_IF_FREQUENCY_DVBT_7M        ((U16)  4350)  /* kHz */  //JUST for 6.8M
#define C_RA217_IF_FREQUENCY_DVBT_8M        ((U16)  4850)  /* kHz */  //JUST for 7.65M
#define C_RA217_IF_FREQUENCY_DVBT2_7M        ((U16)  4350)  /* kHz */  //JUST for 6.8M
#define C_RA217_IF_FREQUENCY_DVBT2_8M        ((U16)  4850)  /* kHz */  //JUST for 7.65M
#define C_RA217_IF_FREQUENCY_DVBC        ((U16)  5100)  /* kHz */  //JUST for 7.95M
#define C_RA217_IF_FREQUENCY_ATSC        ((U16)  3700)  /* kHz */  //JUST for 5.74M
#define C_RA217_IF_FREQUENCY_ISDBT        ((U16)  4000)  /* kHz */  //JUST for 5.95M


#define C_RA217_IF_FREQUENCY_PAL_DK   ((U16)  8200)  /* kHz */
//#define C_RA217_IF_FREQUENCY_PAL_I   ((U16)  8200)  /* kHz */
#define C_RA217_IF_FREQUENCY_PAL_BG   ((U16)  7300)  /* kHz */
#define C_RA217_IF_FREQUENCY_SECAM_L1   ((U16)  2200)  /* kHz */
//#define C_RA217_IF_FREQUENCY_SECAM_L   ((U16)  8200)  /* kHz */
#define C_RA217_IF_FREQUENCY_NTSC_M  ((U16)  5750)  /* kHz */

#define C_RA217_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_RA217_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_RA217_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)

#if (defined(DTD_INCLUDE))
#define C_RA217_AGC_IF_SLP           -0.125	
#define C_RA217_AGC_IF_INT           0.5   
#define C_RA217_AGC_IF_MAX           0.499  // 0.5 will overflow  //0v
#define C_RA217_AGC_IF_MIN          0.0  
#else
#define C_RA217_AGC_IF_SLP            -0.1407
#define C_RA217_AGC_IF_INT           0.4375    
#define C_RA217_AGC_IF_MAX           0.499  // 0.5 will overflow  //0v
#define C_RA217_AGC_IF_MIN          -0.0547  
#endif
#define C_RA217_AGC_IF_SLP_SGN  (S8)((C_RA217_AGC_IF_SLP>0)?1:-1) 
#define C_RA217_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_RA217_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256


#if (!defined(CC_DUMMY_RA217))
#define C_RA217_SAW_BW              SAW_BW_8M 

#define C_RA217_MAX_IF_GAIN_SET       -0.12   
#define C_RA217_MIN_IF_GAIN_SET       -0.38   
#define C_RA217_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_RA217_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_RA217_MAX_IF_GAIN_POW       -100    // dBm
#define C_RA217_MIN_IF_GAIN_POW		-50     // dBm
#define C_RA217_MAX_RF_GAIN_POW		 C_RA217_MIN_IF_GAIN_POW
#define C_RA217_MIN_RF_GAIN_POW		-20     // dBm
#define C_RA217_POWER_CALIBRATE        26     // dBm
//#define C_RA217_SIGNAL_LVL_TH          6  //liuqu,20080926,no use for nimtool

#define C_RA217_AGC_COEF			((U8)   0xBD)
#define C_RA217_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC_0D36
// ********************************************* //

#define C_U8_RA217_AGC_IF_SLP         (S8)(C_RA217_AGC_IF_SLP *64 )  // MT5135(S(1, -6)), -0.125*64 = -8 (0xF8), -8/64 = -1/8
#define C_U8_RA217_AGC_IF_INT         (S8)(C_RA217_AGC_IF_INT *16 ) // MT5135(S(3, -4))  0.5*16 = 8, 8/16 =0.5
#define C_U8_RA217_AGC_IF_MAX         (S8)(C_RA217_AGC_IF_MAX *256)    //0.499*256 = 0x7F
#define C_U8_RA217_AGC_IF_MIN         (S8)(C_RA217_AGC_IF_MIN *256)    //0
#define C_U8_RA217_AGC_IF_LVL_MAX        (S8)(C_RA217_AGC_IF_LVL_MAX *256)
#define C_U8_RA217_AGC_IF_LVL_MIN         (S8)(C_RA217_AGC_IF_LVL_MIN *256)

#define C_U8_RA217_MAX_IF_GAIN_SET	(S8)(C_RA217_MAX_IF_GAIN_SET *256)
#define C_U8_RA217_MIN_IF_GAIN_SET	(S8)(C_RA217_MIN_IF_GAIN_SET *256)
#define C_U8_RA217_MAX_RF_GAIN_SET	(S8)((C_RA217_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_RA217_MIN_RF_GAIN_SET	(S8)((C_RA217_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_RA217_IF_GAIN_RANGE      (S8)(C_RA217_MIN_IF_GAIN_POW - C_RA217_MAX_IF_GAIN_POW)
#define C_U8_RA217_RF_GAIN_RANGE      (S8)(C_RA217_MIN_RF_GAIN_POW - C_RA217_MAX_RF_GAIN_POW)
#endif

STATIC UINT8* pRA217Eq[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pRA217EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pRA217EqScan[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define SONY_RA217_SUBADDR_INIT1            ((UINT8) 0x01)
#define SONY_RA217_SUBADDR_INIT2            ((UINT8) 0x04)
#define SONY_RA217_SUBADDR_IFAGC            ((UINT8) 0x05)
#define SONY_RA217_SUBADDR_WAKSLEEP         ((UINT8) 0x14)
#define SONY_RA217_SUBADDR_TUNE1            ((UINT8) 0x06)
#define SONY_RA217_SUBADDR_TUNE2            ((UINT8) 0x0B)
#define SONY_RA217_SUBADDR_TUNE3            ((UINT8) 0x10)
#define REGNUM    9

#define MAX_LENGTH_TUNER_WRITE 10
#define DVBT_SLAVE_ADDRESS     0xd8
#define TUNER_SLAVE_ADDRESS     C_RA217_LO_ADDRESS

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

//static UINT8 DIF_SWITCH =1;
#if (!defined(CC_DUMMY_RA217))
#if (!defined(DTD_INCLUDE))
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
#endif

#if defined(DTD_INCLUDE)
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
#endif
#endif

//change for RA217,
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
//static UINT8* RA217_TunerSpec(ITUNER_CTX_T *pTCtx ,UINT8 Mode);
static void tuner_init(void);
static const UINT8* setRfTunerParam(unsigned char bandWidth, UINT8 system );
static void tuner_tune(TuningParam* pDigParam);
static void tuner_wakeup(void);
static void tuner_sleep(void);
static int iic_write(unsigned char subadr, unsigned char *data, unsigned char len);
static int iic_write_val( unsigned char subadr, unsigned char val);
static void Waitms(int ms); 

#ifndef CC_MT5135
STATIC BOOL fgTunerInit=FALSE;
#endif 

STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]=
#if ((defined CC_MT5396) || (defined CC_MT5368))
{
    {0x7B7,0x10}, //PRA target H
    {0x7B9,0x10}, //PRA target L
   // {0x790,0x00}, //DAGC setting
    {0x7D5,0x07}, 
    {0x7D6,0x07},  
    {0x7D8,0xF7}, 
    {0x7D9,0xF7}, 
    {0x7DB,0x7F}, 
    {0x7DD,0xF2}, 
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC	disable.IF_AGC polarity inverse
    //{0x245,0xe0},  //yu need sdjust cvbs swing
   // {0x794,0x38}, //DAGC Target level H
   // {0x7A4,0x28}, //DAGC Target level L
   // {0x7CA,0x88}, //PRA speed
    {0x00,0x00}    //end flag
};
#elif (defined CC_MT5389)
{
   {0x7B7,0x10}, //PRA target H
   {0x7B9,0x10}, //PRA target L
   // {0x790,0x00}, //DAGC setting
    {0x7D5,0x07}, 
    {0x7D6,0x07},  
    {0x7D8,0xF7}, 
    {0x7D9,0xF7}, 
    {0x7DB,0x7F}, 
    {0x7DD,0xF2}, 
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC	disable.IF_AGC polarity inverse
    //{0x245,0xe0},  //yu need adjust cvbs swing
    //{0x7e0,0x07},  //yu need just for sony 89 col
   // {0x794,0x38}, //DAGC Target level H
   // {0x7A4,0x28}, //DAGC Target level L
   // {0x7CA,0x88}, //PRA speed
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
// sony RA217 driver
///////////////////////////////////

CHAR* RA217_GetSwVer(void)
{
	return ((CHAR*)C_RA217_VERSION);
}

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID RA217_TunerInit(ITUNER_CTX_T *pTCtx)
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
#if (defined(DTD_INCLUDE))
    SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);
#else
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
#endif


#if ((defined CC_MT5396) || (defined CC_MT5368)|| (defined CC_MT5389)) 
  #ifndef CC_MT5135
    if(!fgTunerInit)
    {    
        UINT32 TunerReset = DRVCUST_OptGet(eTunerCtrlAna);
        // Tuner Reset
        GPIO_SetOut(TunerReset, 0); 
        mcDELAY_MS(20);
        GPIO_SetOut(TunerReset, 1);
        mcDELAY_MS(20);	
        fgTunerInit=TRUE;
    }	
  #endif			
#endif
	//eTunerCtrlAna

    //DemodHwReset(100);
    //GPIO_SetOut(35, 1);

    //REGION region=COUNTRY_OTHER;
    psTunerCtx->u2LO_Step=100;  //*10hz
    psTunerCtx->I2cAddress = C_RA217_LO_ADDRESS;
#if defined(DTD_INCLUDE)
    psTunerCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_ATSC;	//ATSC 6M
#else
    psTunerCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBC;  //DVBT 8M
#endif 
    psTunerCtx->u4RF_Freq = 0;
    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI); // Disable DRO patch for SLD ATD
    psTunerCtx->fgRFAGC = FALSE;

#if ((defined CC_MT5396) || (defined CC_MT5368)|| (defined CC_MT5389)) 
    psTunerCtx->fgRFTuner = FALSE;	 // Low IF for DVB-T/C
#endif

    psTunerCtx->u2RetryTVDTm = 100;  // Retry TVD delay 100ms
    // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x00;  
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

#if (defined(DTD_INCLUDE))
    pUSCtx->m_s4FreqBoundUpper = C_RA217_FREQ_DBOUND_UPPER;
    pUSCtx->m_s4FreqBoundLower = C_RA217_FREQ_DBOUND_LOWER;
    pUSCtx->m_s4AnaFreqBoundUpper = C_RA217_FREQ_DBOUND_UPPER_Ana;
    pUSCtx->m_s4AnaFreqBoundLower = C_RA217_FREQ_DBOUND_LOWER_Ana;

    pUSCtx->m_aucPara_SLD[2] = (U8)C_U8_RA217_AGC_IF_SLP;
    pUSCtx->m_aucPara_SLD[3] = (U8)C_U8_RA217_AGC_IF_INT;
    pUSCtx->m_aucPara_SLD[4] = (U8)C_U8_RA217_AGC_IF_MAX;
    pUSCtx->m_aucPara_SLD[5] = (U8)C_U8_RA217_AGC_IF_MIN;
/*		
    pUSCtx->m_aucPara[TUNER_FUNC_EN_TARGET_LEVEL] = FALSE;	  //for Hostcmd, sawless default AD target level in FW
#ifdef fcEXTERN_DEMOD
    pUSCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x23;	 // for 5365+5112ee
#else
    pUSCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_MSB] = 0x3D;
#endif  // fcEXTERN_DEMOD end
    pUSCtx->m_aucPara[TUNER_PARA_TARGET_LEVEL_LSB] = 0x00; 
*/
    pUSCtx->u2LO_Step = 100; //*10HZ

#else  // defined(DTD_INCLUDE)

    pEUCtx->m_Ana_Top = C_RA217_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_RA217_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_RA217_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_RA217_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_RA217_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_RA217_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_RA217_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_RA217_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
#if (!defined(CC_DUMMY_RA217))
    pEUCtx->m_SAW_BW  = C_RA217_SAW_BW;
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_RA217_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_RA217_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_RA217_DELAY_MS;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_RA217_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_RA217_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_RA217_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)0xf2;//C_U8_RA217_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_RA217_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_RA217_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_RA217_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_RA217_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_RA217_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_RA217_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_RA217_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_RA217_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_RA217_POWER_CALIBRATE;
//IF MAX setting!
    pEUCtx->m_aucPara[21] = 0x7F; //xiangfu 20100526
#endif   // (!defined(CC_DUMMY_RA217)) end
#endif

    pRA217Eq[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRA217BG;
    pRA217Eq[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRA217DK;
    pRA217Eq[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRA217I;
    pRA217Eq[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRA217L;	
    pRA217Eq[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRA217L1;
    pRA217Eq[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQRA217M;
    // Channel Scan EQ for NTSC-M
    pRA217EqScan[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQRA217M;

    pRA217EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRA217BGWeak;
    pRA217EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRA217DKWeak;
    pRA217EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRA217IWeak;
    pRA217EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRA217LWeak;	
    pRA217EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRA217L1Weak;
    pRA217EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]	= EQRA217MWeak;


// do tuner init procedue
    tuner_init();

    mcSHOW_USER_MSG(("Tuner RA217 init done!Version :%s\n",(CHAR *)C_RA217_VERSION));
}


 STATIC void  RA217_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("RA217_SetSawBw = %d\n", sawbw));
   
 }


S16 RA217_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    TuningParam TuningParam;
  	UINT8  Mode = param->Modulation;
	
    TuningParam.m_BandWidth = pTCtx->specific_member.eu_ctx.m_SAW_BW;
    TuningParam.m_CoexistL = 0;  
    TuningParam.m_frequency = param->Freq;
    TuningParam.m_mode = param->fgAutoSearch;
    TuningParam.system = param->Modulation;
	


#if (!(defined(CC_DUMMY_RA217) || defined(DTD_INCLUDE)))	
    SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
#endif

#if (!defined(CC_DUMMY_RA217))
#if defined(DTD_INCLUDE)
    if(Mode < MOD_DVBT)
    {  // ATSC 8VSB, J83B QAM64/256/Auto
        pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_ATSC;
    }
#else
    if(Mode == MOD_DVBT)
    {
        if(TuningParam.m_BandWidth == SAW_BW_6M)
            pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBT_6M;
        else if(TuningParam.m_BandWidth == SAW_BW_7M)
            pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBT_7M;
        else 
            pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBT_8M;
    }
    else if(Mode == MOD_DVBT2)
    {
        if(TuningParam.m_BandWidth == SAW_BW_7M)
            pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBT2_7M;
        else 
            pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBT2_8M;
    }
    else if(Mode == MOD_DVBC)
    {
        pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_DVBC;
    }
    else if(Mode == MOD_ISDBT)
    {
        pTCtx->u2IF_Freq = C_RA217_IF_FREQUENCY_ISDBT;
    }
#endif
#endif  // (!defined(CC_DUMMY_RA217)) end

    if (Mode == MOD_PAL_BG)
    {
        pTCtx->u2IF_Freq_A = C_RA217_IF_FREQUENCY_PAL_BG;
    }
    else if ((Mode >MOD_PAL_BG) &&(Mode <MOD_SECAM_L1))
    {
        pTCtx->u2IF_Freq_A = C_RA217_IF_FREQUENCY_PAL_DK;
    }
    else if (Mode == MOD_SECAM_L1)  
    {
        pTCtx->u2IF_Freq_A = C_RA217_IF_FREQUENCY_SECAM_L1;
    }
    else if (Mode == MOD_NTSC_M)    
    {
        pTCtx->u2IF_Freq_A = C_RA217_IF_FREQUENCY_NTSC_M;
    }
    mcSHOW_DBG_MSG(("RA217 ATV Only, Mode = %d, u2IF_Freq_A = %d KHz\n", Mode-MOD_ANA_TYPE_BEGIN, pTCtx->u2IF_Freq_A));

	
    pTCtx->u4RF_Freq = TuningParam.m_frequency;
	
    tuner_wakeup();
    //   Wakeup Data Setting
	
    //mcDELAY_MS(100);
    mcSHOW_DBG_MSG(("SONY_RA217_SetFreq\n"));
	
	
#if (defined(CC_DUMMY_RA217) || defined(DTD_INCLUDE))
     RA217DelayMs  =  C_RA217_DELAY_MS;	  // ATV channel, delay 50ms
#else
     RA217DelayMs  =  pTunerCtx->m_aucPara[0];
#endif

    tuner_tune(&TuningParam);
	
    //mcDELAY_MS(250);

    mcSHOW_DBG_MSG(("[%s]Freq = %d KHz, Mode =%d, setting PLL success\n" , (CHAR *)C_RA217_VERSION,TuningParam.m_frequency,Mode));	
    return (0);
}

INT16 RA217_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{   
    switch(eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr = (CHAR **)pInOutVal;
            *RetStr = RA217_GetSwVer();
        }
        break;
    case itSetTop:
        {
            SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
            pTunerCtx->m_aucPara[0] = SetVal;
        }
        break;
		
    case itSetReg:
        mcSHOW_DBG_MSG(("Not implement in SONY RA217 Tuner\n"));
        break;
		
    case itGetReg:
        {        
            mcSHOW_DBG_MSG(("Not implement in SONY RA217 Tuner\n"));	   
        }
        break;
    case itSetSawBw:
        RA217_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
        mcSHOW_DBG_MSG(("Not implement in SONY RA217 Tuner\n"));
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pRA217Eq[SetVal];;
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pRA217EqWeak[SetVal];
        break;
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
#if 0		
    case itGetEqScriptScan: 
        *(UINT8**)pInOutVal = pRA217EqScan[SetVal];
        break;  
#endif		
    case itSetIfMax :	
        {
            SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
            pTunerCtx->m_aucPara[21] = SetVal;
            break;
        } 
    case itSetIfMin:
        {
            UINT8 *ifMinPtr = (UINT8*)pInOutVal;			
            mcSHOW_DBG_MSG(("RA217 TOP Setting\n"));
            *ifMinPtr = 0xf2;
        }
        break;	
#if 0		
    case itGetAGCPol :
        {
            if(C_RA217_AGC_IF_SLP == 1)
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
            UINT8 subSysId = SetVal;
            UINT8 *ifThcPtr = (UINT8*)pInOutVal;
            mcSHOW_DBG_MSG(("RA217 Target Setting\n"));
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
	  UINT8 u1PRATar;
	  
	  u1PRATar =0x10;
	  
	    switch(subSysId)
		{
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                u1PRATar =0x05;
                break;
        }
	   arrAtdRelReg[PRA_TARGET_H_INDEX].u1RegVal=u1PRATar;
	   arrAtdRelReg[PRA_TARGET_L_INDEX].u1RegVal=u1PRATar;
	  
        *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg;
		}
        break;

    default:
        mcSHOW_DBG_MSG2(("NO case !Not implement in SONY RA217 Tuner.\n"));
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
    unsigned char senddata[ MAX_LENGTH_TUNER_WRITE+3];
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
       ICtrlBus_I2cTunerWrite(C_RA217_I2C_CLK_DIV,slv0,senddata,len+3);       
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
       ICtrlBus_I2cTunerWrite(C_RA217_I2C_CLK_DIV,slv1,senddata,len+1);
       
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

    iic_write_val( 0x05,0x0c ); //IF2 & AGC2, add for RA217 20101214
	  
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
#if 0 // merge RA217
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
    unsigned char t_addr;  //add for RA217 20101214
    unsigned char t_data[5]; //add for RA217 20101214
    unsigned char  data_length; //add for RA217 20101214

    mcSHOW_DBG_MSG(("Enter tuner_init\n"));
    iic_write_val( 0x01, 0x10 );//change for RA217 20101214
    iic_write_val( 0x04, 0x40 );
    iic_write_val( 0x14, 0xFB );
    iic_write_val( 0x21, 0x18 );

//add for RA217 20101214	
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

    iic_write_val(  0x07, 0x10 );    //change for RA217 20101214
//add for RA217 20101214    
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
#if (!defined(CC_DUMMY_RA217))  
#if defined(DTD_INCLUDE)
    case MOD_ATSC_8VSB:              
        buf = RfRegSetting_ATSC_8VSB; 
        break;
    case MOD_J83B_256QAM:
    case MOD_J83B_AUTO_QAM:
    case MOD_J83B_64QAM:             
        buf = RfRegSetting_US_QAM;    
        break;
#else
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
#endif
#endif	
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

    //set param
    param = setRfTunerParam( pDigParam->m_BandWidth,pDigParam->system);
    mcSHOW_DBG_MSG(("Param0: 0x%2x - 0x%2x - 0x%2x\n", param[0], param[1], param[2]));
    mcSHOW_DBG_MSG(("Param1: 0x%2x - 0x%2x - 0x%2x\n", param[3], param[4], param[5]));
    mcSHOW_DBG_MSG(("Param2: 0x%2x - 0x%2x - 0x%2x - 0x%2x - 0x%2x\n", param[6], param[7], param[8], param[9], param[10]));
    mcSHOW_DBG_MSG(("Param3: 0x%2x\n", param[11]));

#if 0   // merge RA217 Tuner	
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
    mcSHOW_DBG_MSG(("Freq(%dKHz): 0x%2x - 0x%2x - 0x%2x - 0x%2x - 0x%2x\n", frequency, t_data[0], t_data[1], t_data[2], t_data[3], t_data[4]));

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#endif
	
    if (pDigParam->system  < MOD_PAL_BG)
    {
        Waitms(100);	
        mcSHOW_DBG_MSG(("Tuner tune delay 100 MS.\n"));	 
    }
    else
    {
        Waitms(RA217DelayMs);
        mcSHOW_DBG_MSG(("Tuner tune delay %d MS.\n",RA217DelayMs));	
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


