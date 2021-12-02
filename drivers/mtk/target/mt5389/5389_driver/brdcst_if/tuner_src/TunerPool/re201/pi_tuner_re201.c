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
 * $RCSfile: pi_tuner_re201.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_re201.c
 *  SONY RE201 tuner driver.
 */


#include "tuner_interface_if.h"
#include "pi_def_dvbt.h"
#include "PI_Def.h"
#include "ctrl_bus.h"
#include "pi_dvbt_if.h"  //add by liuqu,20090424
#include "pi_demod_atd.h"
#include "eq_script_re201.h"
#include "sif_if.h"
#include "x_gpio.h"
#include "drvcust_if.h"



// *** Release Note ***
// Ver     Date         Description
// 1.00    2009.03.25   file established
#define C_RE201_VERSION				"RE201 2010-01-01\n 2011-04-14\n v1.1"

//#define C_RE201_PLL_POLL_INTERVAL   5
#define C_RE201_PLL_POLL_INTERVAL   10   // to solve SLT thread UART overflow issue, 090601, Ken
#define C_RE201_PLL_TIMEOUT         100
#define C_RE201_PLL_TIMEOUT_CNT     C_RE201_PLL_TIMEOUT/C_RE201_PLL_POLL_INTERVAL
#define C_RE201_I2C_CLK_DIV         0x100
#define C_RE201_PLL_LOCK_BIT        6

#define C_RE201_TOP_SET             ((U8)   0x02) 
#define C_RE201_TOP_SET_DVBC        ((U8)   0x04) 
#define C_RE201_TOP_SET_ANA         ((U8)   0x0A) 
#define C_RE201_SAW_BW              SAW_BW_8M 

//#define C_RE201_FREQ_DBOUND_UPPER   858*1000*1000 
#define C_RE201_FREQ_DBOUND_UPPER   859*1000*1000      //modified for DVB-C application, Menghau
#define C_RE201_FREQ_DBOUND_LOWER    48*1000*1000 
#define C_RE201_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_RE201_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_RE201_LO_ADDRESS         ((U16)  0xc0)
#define C_RE201_IF_FREQUENCY_DVBT_6M        ((U16)  3800)  /* kHz */  //JUST for 5.81M
#define C_RE201_IF_FREQUENCY_DVBT_7M        ((U16)  4350)  /* kHz */  //JUST for 6.8M
#define C_RE201_IF_FREQUENCY_DVBT_8M        ((U16)  4850)  /* kHz */  //JUST for 7.65M
#define C_RE201_IF_FREQUENCY_DVBT2_7M        ((U16)  4350)  /* kHz */  //JUST for 6.8M
#define C_RE201_IF_FREQUENCY_DVBT2_8M        ((U16)  4850)  /* kHz */  //JUST for 7.65M
#define C_RE201_IF_FREQUENCY_DVBC        ((U16)  5100)  /* kHz */  //JUST for 7.95M
#define C_RE201_IF_FREQUENCY_ATSC        ((U16)  3700)  /* kHz */  //JUST for 5.74M
#define C_RE201_IF_FREQUENCY_ISDBT        ((U16)  4000)  /* kHz */  //JUST for 5.95M


#define C_RE201_IF_FREQUENCY_PAL_DK   ((U16)  8200)  /* kHz */
//#define C_RE201_IF_FREQUENCY_PAL_I   ((U16)  8200)  /* kHz */
#define C_RE201_IF_FREQUENCY_PAL_BG   ((U16)  7300)  /* kHz */
#define C_RE201_IF_FREQUENCY_SECAM_L1   ((U16)  2200)  /* kHz */
//#define C_RE201_IF_FREQUENCY_SECAM_L   ((U16)  8200)  /* kHz */
#define C_RE201_IF_FREQUENCY_NTSC_M  ((U16)  5750)  /* kHz */

#define C_RE201_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_RE201_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 1500)
#define C_RE201_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 500)


#define C_RE201_AGC_IF_SLP           -0.125	
#define C_RE201_AGC_IF_INT           0.5   
#define C_RE201_AGC_IF_MAX           0.998  // 0.999 will overflow  //0v
#define C_RE201_AGC_IF_MIN          0.0  
#define C_RE201_AGC_IF_SLP_SGN  (S8)((C_RE201_AGC_IF_SLP>0)?1:-1) 
#define C_RE201_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_RE201_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256


#define C_RE201_MAX_IF_GAIN_SET       -0.12   
#define C_RE201_MIN_IF_GAIN_SET       -0.38   
#define C_RE201_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_RE201_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_RE201_MAX_IF_GAIN_POW       -100    // dBm
#define C_RE201_MIN_IF_GAIN_POW		-50     // dBm
#define C_RE201_MAX_RF_GAIN_POW		 C_RE201_MIN_IF_GAIN_POW
#define C_RE201_MIN_RF_GAIN_POW		-20     // dBm
#define C_RE201_POWER_CALIBRATE        26     // dBm
//#define C_RE201_SIGNAL_LVL_TH          6  //liuqu,20080926,no use for nimtool

#define C_RE201_AGC_COEF			((U8)   0xBD)
#define C_RE201_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC_0D36

// ********************************************* //

#define C_U8_RE201_AGC_IF_SLP         (S8)(C_RE201_AGC_IF_SLP *64 )  // MT5135(S(1, -6)), -0.125*64 = -8 (0xF8), -8/64 = -1/8
#define C_U8_RE201_AGC_IF_INT         (S8)(C_RE201_AGC_IF_INT *16 ) // MT5135(S(3, -4))  0.5*16 = 8, 8/16 =0.5
#define C_U8_RE201_AGC_IF_MAX         (S8)(C_RE201_AGC_IF_MAX *128)    //0.499*256 = 0x7F, 0.998*128
#define C_U8_RE201_AGC_IF_MIN         (S8)(C_RE201_AGC_IF_MIN *128)    //0
#define C_U8_RE201_AGC_IF_LVL_MAX        (S8)(C_RE201_AGC_IF_LVL_MAX *256)
#define C_U8_RE201_AGC_IF_LVL_MIN         (S8)(C_RE201_AGC_IF_LVL_MIN *256)

#define C_U8_RE201_MAX_IF_GAIN_SET	(S8)(C_RE201_MAX_IF_GAIN_SET *256)
#define C_U8_RE201_MIN_IF_GAIN_SET	(S8)(C_RE201_MIN_IF_GAIN_SET *256)
#define C_U8_RE201_MAX_RF_GAIN_SET	(S8)((C_RE201_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_RE201_MIN_RF_GAIN_SET	(S8)((C_RE201_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_RE201_IF_GAIN_RANGE      (S8)(C_RE201_MIN_IF_GAIN_POW - C_RE201_MAX_IF_GAIN_POW)
#define C_U8_RE201_RF_GAIN_RANGE      (S8)(C_RE201_MIN_RF_GAIN_POW - C_RE201_MAX_RF_GAIN_POW)

STATIC UINT8* pRe201[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pRe201EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
#define SONY_RE201_SUBADDR_INIT1            ((UINT8) 0x01)
#define SONY_RE201_SUBADDR_INIT2            ((UINT8) 0x04)
#define SONY_RE201_SUBADDR_IFAGC            ((UINT8) 0x05)
#define SONY_RE201_SUBADDR_WAKSLEEP         ((UINT8) 0x14)
#define SONY_RE201_SUBADDR_TUNE1            ((UINT8) 0x06)
#define SONY_RE201_SUBADDR_TUNE2            ((UINT8) 0x0B)
#define SONY_RE201_SUBADDR_TUNE3            ((UINT8) 0x10)
#define REGNUM    9

#define MAX_LENGTH_TUNER_WRITE 10
#define DVBT_SLAVE_ADDRESS     0xd8
#define TUNER_SLAVE_ADDRESS     C_RE201_LO_ADDRESS

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
#define RF_REGSETTING_MAX_ENTRY          9

UINT8 DIF_SWITCH =1;
#ifndef CC_SONY_DEMOD
static UINT8 SIF_CONTROL = 1;
#endif
static const UINT8
RfRegSetting_ATSC_8VSB[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xD5, 0x1A, 
    0x00,
};

static const UINT8
RfRegSetting_US_QAM[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xD5, 0x1A, 
    0x00,
};

static const UINT8
RfRegSetting_ISDBT[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0x05, 0x00, 
    0x00,
};

static const UINT8
RfRegSetting_DVBT_6M[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xE5, 0x1C, 
    0x00,
};

static const UINT8
RfRegSetting_DVBT_7M[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xED, 0x00, 
    0x10,
};

static const UINT8
RfRegSetting_DVBT_8M[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xED, 0x00, 
    0x20,
};

static const UINT8
RfRegSetting_DVBT2_7M[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xED, 0x00, 
    0x10,
};

static const UINT8
RfRegSetting_DVBT2_8M[RF_REGSETTING_MAX_ENTRY] = {
    0x01, 0x04, 0x0E,
    0x25, 0x13, 0x20, 0xED, 0x00, 
    0x20,
};

static const UINT8
RfRegSetting_MN[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x12, 0x20, 0x02, 0x00, 
    0x80,
};

static const UINT8
RfRegSetting_BG[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x12, 0x20, 0x5A, 0x08, 
    0x90,
};

static const UINT8
RfRegSetting_I[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x12, 0x20, 0x4A, 0x07, 
    0xA0,
};

static const UINT8
RfRegSetting_DK[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x12, 0x20, 0x4A, 0x07, 
    0xA0,
};

static const UINT8
RfRegSetting_L[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x15, 0x20, 0x48, 0x07, 
    0xA0,
};

static const UINT8
RfRegSetting_L_DASH[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x15, 0x20, 0xF8, 0x07, 
    0xE0,
};

static const UINT8
RfRegSetting_DVBC[RF_REGSETTING_MAX_ENTRY] = {
    0x04, 0x02, 0x02,
    0x0F, 0x12, 0x20, 0x12, 0x06, 
    0x20,
};

#define PRA_TARGET_H_INDEX 0
#define PRA_TARGET_L_INDEX 1
#define SLD_DAGC_00_INDEX 2
#define DAGC_TARGET_LEVEL_H_INDEX  4
#define DAGC_TARGET_LEVEL_L_INDEX  5
#define PRA_LOOP_CONF_INDEX  6
//Tuner releated ATD register
STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]=
#if ((defined CC_MT5396) || (defined CC_MT5368) || (defined CC_MT5389)) 
{
    #if(defined CC_MT5389)
    {0x7B7,0x10}, //PRA target H
    {0x7B9,0x10}, //PRA target L
    {0x790,0x00}, //DAGC setting
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC disable.IF_AGC polarity inverse
    {0x794,0x38}, //DAGC Target level H
    {0x7A4,0x28}, //DAGC Target level L
    {0x7CA,0x88}, //PRA speed
    {0x7D4,0x04},
    {0x7D5,0x00},
    {0x7D6,0x00},
    {0x7D7,0xEC},
    {0x7D8,0xEF},
    {0x7D9,0xEF},
    {0x7DA,0x7F},
    {0x7DB,0x7F},
    {0x7DC,0x80},
    {0x7DD,0x80},
    {0x7DE,0x80},
    {0x00,0x00}    //end flag
    #else
    {0x7B7,0x10}, //PRA target H
    {0x7B9,0x10}, //PRA target L
    {0x790,0x00}, //DAGC setting
    {0x7DF,0x51}, //AGC polarity inverse.RF_AGC	disable.IF_AGC polarity inverse
    {0x794,0x38}, //DAGC Target level H
    {0x7A4,0x28}, //DAGC Target level L
    {0x7CA,0x88}, //PRA speed
    {0x00,0x00}    //end flag
    #endif
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
//static UINT8* RE201_TunerSpec(ITUNER_CTX_T *pTCtx ,UINT8 Mode);
static void tuner_init(void);
static const UINT8* setRfTunerParam(unsigned char bandWidth, UINT8 system );
static void tuner_tune(TuningParam* pDigParam);
static void tuner_wakeup(void);
static void tuner_sleep(void);
static int iic_write(unsigned char subadr, unsigned char *data, unsigned char len);
static int iic_write_val( unsigned char subadr, unsigned char val);
static void Waitms(int ms); 



/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/


///////////////////////////////////
// sony RE201 driver
///////////////////////////////////

CHAR* RE201_GetSwVer(void)
{
	return ((CHAR*)C_RE201_VERSION);
}


#ifdef CC_SMART_RESET
extern INT32 SmartResetDemod(UINT32 gpio);
#endif

//Justin TODO, pEUCtx->m_aucPara all necessary????
VOID RE201_TunerInit(ITUNER_CTX_T *pTCtx)
{
#ifdef CC_SMART_RESET
	UINT32 u4RstPinNum;
	static UINT8 InitFg=0;
#endif
	ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);

	/*/////////////////////////
	GPIO_SetOut(729, 0); 
	mcDELAY_MS(100);
	GPIO_SetOut(729, 1);
	mcDELAY_MS(100);         
	//////////////////////////just for turnkey 5365 board*/


    //REGION region=COUNTRY_OTHER;
    psTunerCtx->u2LO_Step=100;  //*10hz
    psTunerCtx->I2cAddress = C_RE201_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBC;  //DVBT 8M
    psTunerCtx->u4RF_Freq = 0;	

    //Justin TODO
    psTunerCtx->u1AtdPatchSwitch = (MASK_PATCH_CR|MASK_PATCH_DRO|MASK_PATCH_SENS);
    psTunerCtx->fgRFAGC = FALSE;

#if ((defined CC_MT5396) || (defined CC_MT5368)) 
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

    pEUCtx->m_SAW_BW  = C_RE201_SAW_BW;
    pEUCtx->m_Ana_Top = C_RE201_TOP_SET_ANA;
    pEUCtx->m_s4FreqBoundUpper = C_RE201_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_RE201_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_RE201_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_RE201_FREQ_DBOUND_LOWER_Ana;
    
    pEUCtx->m_Small_Step = C_RE201_CHANNEL_SCAN_JUMP_SMALL_STEP;
    pEUCtx->m_Middle_Step_Up = C_RE201_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_RE201_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // add by JC, 081215, for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_RE201_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_RE201_AGC_IF_LVL_MIN;
    
    pEUCtx->m_SigLvTh = 0;
    pEUCtx->m_SigLvScan = 0;
    
    pEUCtx->m_aucPara[0] = C_RE201_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    
    pEUCtx->m_aucPara[2] = (U8)C_U8_RE201_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_RE201_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_RE201_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_RE201_AGC_IF_MIN;
    
    pEUCtx->m_aucPara[6] = C_RE201_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_RE201_DEMOD_INPUT_POWER;
    
    pEUCtx->m_aucPara[ 8] = (U8)C_U8_RE201_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (U8)C_U8_RE201_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_RE201_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_RE201_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_RE201_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_RE201_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_RE201_POWER_CALIBRATE;
//IF MAX setting!
    pEUCtx->m_aucPara[21] = 0x7F; //xiangfu 20100526

// Add for US demod in SLD PRA setting, iven 20110602
    pUSCtx->m_aucPara_SLD[2] = (U8)C_U8_RE201_AGC_IF_SLP;
    pUSCtx->m_aucPara_SLD[3] = (U8)C_U8_RE201_AGC_IF_INT;    
    pUSCtx->m_aucPara_SLD[4] = (U8)C_U8_RE201_AGC_IF_MAX;    
    pUSCtx->m_aucPara_SLD[5] = (U8)C_U8_RE201_AGC_IF_MIN;        

    pUSCtx->u2LO_Step = 100; //*10HZ

    pRe201[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRe201BG;
    pRe201[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRe201DK;
    pRe201[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRe201I;
    pRe201[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRe201L;	
    pRe201[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRe201L1;
    pRe201[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQRe201M;

    pRe201EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRe201BG;
    pRe201EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRe201DK;
    pRe201EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRe201I;
    pRe201EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRe201L;	
    pRe201EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRe201L1;
    pRe201EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]	= EQRe201M;


#ifdef CC_SMART_RESET

if(InitFg==0) {InitFg=1;}//only need init one time 
else return;


	if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
    {
    SmartResetDemod(u4RstPinNum);
    printf("DTD Reset Demod GPIO=0x%x, 0->1\r\n",u4RstPinNum);
    }
	   else {printf("pls check eDemodResetGpio config\n");}

#endif

#if ((defined CC_MT5396) || (defined CC_MT5368)) 
    // Tuner Reset
    GPIO_SetOut(201, 0);  // Reset Pin(OPCTRL1)
    mcDELAY_MS(100);
    GPIO_SetOut(201, 1);
    mcDELAY_MS(100);				
#endif

// do tuner init procedue
    tuner_init();

    mcSHOW_USER_MSG(("Tuner RE201 init done!\n"));
}


 STATIC void  RE201_SetSawBw(ITUNER_CTX_T * pTunerCtx, UINT8 sawbw)
 {
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw;

   mcSHOW_DRVAPI_MSG(("RE201_SetSawBw = %d\n", sawbw));
   
 }


S16 RE201_TunerSetFreq(ITUNER_CTX_T *pTCtx, PARAM_SETFREQ_T* param)
{
    U8  Mode = 0;
    TuningParam TuningParam;
	
    TuningParam.m_BandWidth = pTCtx->specific_member.eu_ctx.m_SAW_BW;
    TuningParam.m_CoexistL = 0;  
    TuningParam.m_frequency = param->Freq;
    TuningParam.m_mode = param->fgAutoSearch;
    TuningParam.system = param->Modulation;
	
    Mode = param->Modulation;

    if(Mode == MOD_DVBT)
    	{
		if(TuningParam.m_BandWidth == SAW_BW_6M)
			pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBT_6M;
		else if(TuningParam.m_BandWidth == SAW_BW_7M)
			pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBT_7M;
		else 
			pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBT_8M;
    	}
	else if(Mode == MOD_DVBT2)
		{
		if(TuningParam.m_BandWidth == SAW_BW_7M)
			pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBT2_7M;
		else 
			pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBT2_8M;
		}
	else if(Mode == MOD_DVBC)
		{
		    pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_DVBC;
		}
	else if(Mode < MOD_DVBT)
		{
		    pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_ATSC;
		}
	else if(Mode == MOD_ISDBT)
		{
		    pTCtx->u2IF_Freq = C_RE201_IF_FREQUENCY_ISDBT;
		}
    else if (Mode == MOD_PAL_BG)
    	{
            pTCtx->u2IF_Freq_A = C_RE201_IF_FREQUENCY_PAL_BG;
    	}
    else if ((Mode >MOD_PAL_BG) &&(Mode <MOD_SECAM_L1))
    	{
            pTCtx->u2IF_Freq_A = C_RE201_IF_FREQUENCY_PAL_DK;
    	}
    else if (Mode == MOD_SECAM_L1)	
        {
            pTCtx->u2IF_Freq_A = C_RE201_IF_FREQUENCY_SECAM_L1;
        }
	else if (Mode == MOD_NTSC_M)	
        {
            pTCtx->u2IF_Freq_A = C_RE201_IF_FREQUENCY_NTSC_M;
        }
	
	
    pTCtx->u4RF_Freq = TuningParam.m_frequency;
	
       tuner_wakeup();
	//   Wakeup Data Setting
	
	//mcDELAY_MS(100);

    mcSHOW_DBG_MSG(("SONY_RE201_SetFreq\n"));

    tuner_tune(&TuningParam);
	
	//mcDELAY_MS(250);


    mcSHOW_DBG_MSG(("Freq = %d KHz, Mode =%d, setting PLL success\n" , TuningParam.m_frequency,Mode));	
    return (0);
}
typedef struct
{
    INT16 u2power;
	INT16 u2val;
} SSI_MAP_T;

//LNA off table for SSI
static SSI_MAP_T SSItbOffRF[]=
{
	{-5,127},
	{-10,174},
	{-17,300},
	{0,0},//end flag by NULL 
};

static SSI_MAP_T SSItbOffIF[]=
{

 {-17,838},
 {-18,912},
 {-19,979},
 {-20,1033},
 {-21,1083},
 {-22,1125},
 {-23,1202},
 {-24,1291},
 {-25,1364},
 {-26,1403},
 {-27,1431},
 {-28,1504},
 {-29,1548},
 {-30,1579},
 {-31,1608},
 {-32,1635},
 {-33,1663},
 {-34,1696},
 {-35,1743},
 {-36,1776},
 {-37,1813},
 {-38,1855},
 {-39,1900},
 {-40,1958},
 {-41,1990},
 {-42,2017},
 {-43,2042},
 {-44,2072},
 {-45,2104},
 {-46,2136},
 {-47,2174},
 {-48,2210},
 {-49,2258},
 {-50,2312},
 {-51,2342},
 {-52,2380},
 {-53,2420},
 {-54,2471},
 {-55,2512},
 {-56,2553},
 {-57,2589},
 {-58,2628},
 {-59,2655},
 {-60,2690},
 {-61,2729},
 {-62,2765},
 {-63,2797},
 {-64,2830},
 {-65,2868},
 {-66,2899},
 {-67,2941},
 {-68,2975},
 {-69,3005},
 {-70,3037},
 {-71,3072},
 {-72,3103},
 {-73,3138},
 {-74,3167},
 {-75,3203},
 {-76,3231},
	{0,0},//end flag by NULL 
	
};

static SSI_MAP_T SSItbOffBB[]=
{
	{-100,28},
	{-97,31},
	{-95,34},
	{-93,38},
	{-90,48},
	{-87,64},
	{-85,76},
	{-83,92},
	{-81,116},
	{-79,145},
	{-78,164},
	{-76,202},
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

static INT16 MapRecPowerByTB(INT16 dataRF,INT16 dataIF,INT16 dataBB,SSI_MAP_T * SSItbRF,SSI_MAP_T *SSItbIF,SSI_MAP_T * SSItbBB)
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


static void RE201_SetSSICondition(ITUNER_CTX_T* pTunerCtx, VOID * pInOutVal);
static UINT32 RE201_GetSSI(ITUNER_CTX_T* pTunerCtx) ;

//----------------------------------------------------------------------------- 
/** RE201_SetSSICondition
 *  Set SSI Condition for TH2603
 *  @param  
 *  @retval void
 */
//-----------------------------------------------------------------------------
void RE201_SetSSICondition(ITUNER_CTX_T* pTunerCtx, VOID * pInOutVal)
{
    pTunerCtx->sSSIPara.i2Mod = *((INT16*) pInOutVal);
    pTunerCtx->sSSIPara.i2CR = *((INT16*) pInOutVal+1);
    pTunerCtx->sSSIPara.i2Ssi_RF= *((INT16 *) pInOutVal+2);
    pTunerCtx->sSSIPara.i2Ssi_IF= *((INT16 *) pInOutVal+3);
    pTunerCtx->sSSIPara.i2Ssi_BB= *((INT16 *) pInOutVal+4);
}


//----------------------------------------------------------------------------- 
/*
 *  RE201_GetSSI
 *  calculate Signal Strength Indicator.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
UINT32 RE201_GetSSI(ITUNER_CTX_T* pTunerCtx) 
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

   // INT16	IF_AGC_Max = IFAGC[sizeof(IFAGC)/sizeof(IFAGC[0])-1];
   // INT16  IF_AGC_Min = IFAGC[0];
	//    UINT8 i =0;	

    UINT32 u4SSI_Indicator = 0;
    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);

    // initial level for best mux, Ken, 100607
    pCtx->m_SigLvScan = 0;
    
    /* Get Power Ref Value */  
    mcSHOW_DBG_MSG(("DVBT: Mod=%d, Code Rate=%d, RFAGC=%d, IFAGC=%d, BBAGC=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR,
	                                                              pTunerCtx->sSSIPara.i2Ssi_RF,pTunerCtx->sSSIPara.i2Ssi_IF,pTunerCtx->sSSIPara.i2Ssi_BB));
    i2Ssi_RF = pTunerCtx->sSSIPara.i2Ssi_RF;    
    i2Ssi_IF = pTunerCtx->sSSIPara.i2Ssi_IF;    
    i2Ssi_BB = pTunerCtx->sSSIPara.i2Ssi_BB;
	 /****************************/
       
	i4Power_rec=MapRecPowerByTB(i2Ssi_RF,i2Ssi_IF,i2Ssi_BB,SSItbOffRF,SSItbOffIF,SSItbOffBB);
	    //mcSHOW_DBG_MSG(("IF AGC Range,i = %d\n,input power = %d\n",i,i4Power_rec));
/****************************/

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


UINT32 RE201_GetSSI_DVBC(ITUNER_CTX_T* pTunerCtx) 
{
    INT16 Power_Ref[] =
    {
        -84,//16QAM
        -81,//32QAM
        -78,//64
        -75,//128QAM
        -73,//256QAM
    };
    INT32 i4Power_ref = 0;
    INT32 i4Power_rec = 0;
    INT32 i4Power_rel = 0;
    INT16 i2Ssi_RF = 0;
    INT16 i2Ssi_IF = 0;
    INT16 i2Ssi_BB = 0;
	INT16 refIndex=0;

   // INT16	IF_AGC_Max = IFAGC[sizeof(IFAGC)/sizeof(IFAGC[0])-1];
   // INT16  IF_AGC_Min = IFAGC[0];
	//    UINT8 i =0;	

    UINT32 u4SSI_Indicator = 0;
    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);

    // initial level for best mux, Ken, 100607
    pCtx->m_SigLvScan = 0;
    
    /* Get Power Ref Value */  
    mcSHOW_DBG_MSG(("DVBC: Mod=%d, Code Rate=%d, RFAGC=%d, IFAGC=%d, BBAGC=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR,
	                                                              pTunerCtx->sSSIPara.i2Ssi_RF,pTunerCtx->sSSIPara.i2Ssi_IF,pTunerCtx->sSSIPara.i2Ssi_BB));
    i2Ssi_RF = pTunerCtx->sSSIPara.i2Ssi_RF;    
    i2Ssi_IF = pTunerCtx->sSSIPara.i2Ssi_IF;    
    i2Ssi_BB = pTunerCtx->sSSIPara.i2Ssi_BB;
	 /****************************/
       
	i4Power_rec=MapRecPowerByTB(i2Ssi_RF,i2Ssi_IF,i2Ssi_BB,SSItbOffRF,SSItbOffIF,SSItbOffBB);
	 i4Power_rec+=6;//compensate 
	    //mcSHOW_DBG_MSG(("IF AGC Range,i = %d\n,input power = %d\n",i,i4Power_rec));
/****************************/
		refIndex=pTunerCtx->sSSIPara.i2Mod;
		mcSHOW_DBG_MSG1(("refIndex=%d",refIndex));

	/*Get Ref Value-Start*/
	if(refIndex>5)
	{
		mcSHOW_DBG_MSG1(("DVBC: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
		return u4SSI_Indicator;
	}
	else
	{
		i4Power_ref = Power_Ref[refIndex];
		mcSHOW_DBG_MSG1(("i4Power_ref=%d\n",i4Power_ref));

				i4Power_ref = *(Power_Ref+refIndex);
		mcSHOW_DBG_MSG1(("-i4Power_ref=%d\n",i4Power_ref));
		
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


INT16 RE201_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
   
    switch(eOperation){
    case itGetVer:
        {
        CHAR ** RetStr = (CHAR **)pInOutVal;
        *RetStr = RE201_GetSwVer();
        }
        break;
    case itSetTop:
	    {
          SPECIFIC_MEMBER_EU_CTX * pTunerCtx= &(pTCtx->specific_member.eu_ctx);
          pTunerCtx->m_aucPara[0] = SetVal;
          DIF_SWITCH = SetVal;
	    }
		break;
		
	case itSetReg:
           mcSHOW_DBG_MSG(("Not implement in SONY RE201 Tuner\n"));
	    break;
		
	case itGetReg:
	    {        
	   mcSHOW_DBG_MSG(("Not implement in SONY RE201 Tuner\n"));	   
           }
		break;
    case itSetSawBw:
		RE201_SetSawBw(pTCtx,SetVal);
        break;
    case itSetLNA:
    case itGetLNA:
		mcSHOW_DBG_MSG(("Not implement in SONY RE201 Tuner\n"));
        break;

    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pRe201[SetVal];;
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pRe201EqWeak[SetVal];
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

     //     UINT8 subSysId = SetVal;
	   UINT8 *ifMinPtr = (UINT8*)pInOutVal;
	//   UINT8 data;
			
         mcSHOW_DBG_MSG(("RE201 TOP Setting\n"));
        *ifMinPtr = 0x80;
	    }
	    break;	
#if 0		
	case itGetAGCPol :
		{if(C_RE201_AGC_IF_SLP == 1)
          *(UINT8*)pInOutVal=0;
		else
		  *(UINT8*)pInOutVal=1;	}
	   break;
#endif
	case itGetTHC:
        {

        UINT8 subSysId = SetVal;
        UINT8 *ifThcPtr = (UINT8*)pInOutVal;
            
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
#if 0
	case itGetTagt:
        { mcSHOW_DBG_MSG(("RE201 Target Setting\n"));
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
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
	{
	  UINT8 subSysId = SetVal;
	  UINT8 u1PRATar,u1Dagc,u1DagcTarH,u1DagcTarL,u1PRAConf;
	  
	  u1PRATar =0x10;
      u1DagcTarH=0x2C;
	  u1DagcTarL=0x2C;
	  u1PRAConf=0x88;
      u1Dagc=0x08;
	  
	    switch(subSysId)
		{
            case MOD_SECAM_L:
            case MOD_SECAM_L1:
                u1PRATar =0x05;
				u1Dagc=0x88;
				u1DagcTarH=0x02;
				u1DagcTarL=0x01;
				u1PRAConf=0xBB;
                break;
			case MOD_NTSC_M:
				//u1Dagc=0x08;
                u1DagcTarH=0x30;
                u1DagcTarL=0x30;
				break;
            case MOD_PAL_I:
                u1DagcTarH=0x2a;
                break;
            default://PAL
				//u1Dagc=0x08;
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
	case itSetIF:
        break;
 	case itSetSSICond:
        RE201_SetSSICondition(pTCtx, pInOutVal);		
        break;
	case itGetSSIIndex:
			{ UINT8 DTVid = SetVal;
			if(DTVid==0)
	 *((UINT32*)pInOutVal) =RE201_GetSSI(pTCtx);
			else
			{*((UINT32*)pInOutVal) =RE201_GetSSI_DVBC(pTCtx);
			mcSHOW_DBG_MSG(("DVBC SSI\n"));}
		}
	 break;  
    default:
	mcSHOW_DBG_MSG(("NO case !Not implement in SONY RE201 Tuner.\n"));
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
    unsigned char senddata[ MAX_LENGTH_TUNER_WRITE];
    int i;
#ifdef CC_SONY_DEMOD
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
       ICtrlBus_I2cTunerWrite(C_RE201_I2C_CLK_DIV,slv0,senddata,len+3);
       
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
		if(SIF_CONTROL == 0)
			ICtrlBus_I2cTunerWrite(C_RE201_I2C_CLK_DIV,slv1,senddata,len+1);
		else
			{
		  if((SIF_X_Write(1,540, slv1,0,0, senddata, len+1)) == 0)
	     {mcSHOW_HW_MSG(("SIF_X_TunerWrite failed\n"));	
		  SIF_CONTROL = 0;}
	   	  }
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
			case MOD_ATSC_8VSB:              buf = RfRegSetting_ATSC_8VSB; break;
			case MOD_J83B_256QAM:
			case MOD_J83B_AUTO_QAM:
			case MOD_J83B_64QAM:             buf = RfRegSetting_US_QAM;    break;
			case MOD_ISDBT:	                 buf = RfRegSetting_ISDBT;	   break;
			case MOD_DVBT:
				{
				if(bandWidth==SAW_BW_6M)      
					                         buf = RfRegSetting_DVBT_6M;  
				else if(bandWidth==SAW_BW_7M) 
					                         buf = RfRegSetting_DVBT_7M;   
			    else	                     
					                         buf = RfRegSetting_DVBT_8M;}   break;
				
			case MOD_DVBT2:  
				{
			    if(bandWidth==SAW_BW_7M)      
					                         buf = RfRegSetting_DVBT2_7M;   
			    else	                     
					                         buf = RfRegSetting_DVBT2_8M;   break;
				
			case MOD_DVBC: 	                 buf = RfRegSetting_DVBC;}	   break;
    		/*case MOD_DTMB:	                 buf = RfRegSetting_DTMB; 	 break; */
	
			case MOD_NTSC_M:		         buf = RfRegSetting_MN; 	   break;
			case MOD_PAL_BG:		         buf = RfRegSetting_BG; 	   break;
			case MOD_PAL_DK:		         buf = RfRegSetting_DK; 	   break;
			case MOD_PAL_I:		             buf = RfRegSetting_I;		   break;
			case MOD_SECAM_L:		         buf = RfRegSetting_L;		   break;
			case MOD_SECAM_L1:	             buf = RfRegSetting_L_DASH;    break;
		
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
    UINT8 t_addr,i;
    UINT8 t_data[5];
    UINT8 data_length;
    const UINT8* param;
    unsigned long frequency;

    frequency = pDigParam->m_frequency;

    //set if port
    t_addr    = 0x05;
#if ((defined CC_MT5396) || (defined CC_MT5368)) 
    iic_write_val(0x05, 0x0C ); 
    mcSHOW_DBG_MSG(("IF PORT is IF2 & AGC2\n"));
#else
    if(DIF_SWITCH ==1)
    {
        if(pDigParam->system < MOD_ANA_TYPE_BEGIN)
        {
            iic_write_val(0x05, 0x0C ); 
            mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x0C));
        }
        else
        {
            iic_write_val(0x05, 0x00 );
            mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x00));
        }
    }
    else
    {
        if(pDigParam->system < MOD_ANA_TYPE_BEGIN)
        {
            iic_write_val(0x05, 0x00 ); 
            mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x00));
        }
        else
        {
            iic_write_val(0x05, 0x0C );
            mcSHOW_DBG_MSG(("IF PORT 0x05 = 0x%2x\n",0x0C));
        }
    }
#endif		 

	//set param
	param = setRfTunerParam( pDigParam->m_BandWidth,pDigParam->system);
    for(i=0;i<9;i++)
		{mcSHOW_DBG_MSG((" 0x%2x\t\n",param[i]));}
	
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

    Waitms(100);


    if(pDigParam->m_CoexistL == 1) 	  //France
    	{
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
	   
    iic_write_val( 0x04, 0x00 );        
    iic_write_val( 0x03, 0xC0 );       
    return;
}

