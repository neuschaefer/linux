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
 * $RCSfile: pi_tuner_rb211.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_rb211.c
 *  Tuner control for SS RB211
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "i2c_api.h"
#include "x_typedef.h"
#include "tunerDebug.h"
#include "x_os.h"
#include "eq_script_rb211.h"

#include "rb211_tuner.h" 
#include "rb211_tuner_i2c_MTK.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Frequency boundary .Get from tuner spec
#define C_RB211_FREQ_DBOUND_UPPER   880*1000*1000     
#define C_RB211_FREQ_DBOUND_LOWER    42*1000*1000 
#define C_RB211_FREQ_DBOUND_UPPER_Ana     880*1000*1000
#define C_RB211_FREQ_DBOUND_LOWER_Ana    42*1000*1000


#define RB211_ANA_LO_STEP             ((UINT16) 100)  /* 10Hz */

#define C_RB211_LO_ADDRESS           0xC0 // In 8bit form
#define C_RB211_I2C_GATEWAY_ADDRESS  0xD8  // Demod address connected to silicon tuner.
#define C_RB211_I2C_GATEWAY_SUB          0x09  // Gateway sub address 
#define C_RB211_IF_FREQUENCY        ((UINT16)  4000)  /* kHz */
#define C_RB211_IF_FREQUENCY_ANA      ((UINT16)  5750)  /* kHz */
#define C_RB211_LO_DIVIDER_STEP       ((UINT32) 1000)  /* Hz */
#define C_RB211_LO_DIVIDER_STEP_ANA   ((UINT16)  1000)  /* Hz */
#define C_RB211_STR                        "RB211_Silicon_tuner"



//-----------------------------------------------------------------------------
//static  variable
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------

static UINT8* pRB211EqNormal[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
static UINT8* pRB211EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};


//-----------------------------------------------------------------------------
//  Rb211 specific tuner information
//-----------------------------------------------------------------------------
typedef struct rb211_tuner_ctx
{
    rb211_tuner_t tuner;
    rb211_tuner_i2c_t i2c;
    BOOL fgCable;// Analog signal input, 0:Air input, 1:Cable input
} rb211_tuner_ctx;

static rb211_tuner_ctx g_rb211_tuner_ctx;


//Tuner releated ATD register
static ATD_TUNER_REL_REG_T arrAtdRelReg[]={
#if (defined CC_MT5396) || (defined CC_MT5368) || (defined CC_MT5389)
    {0x7D4,0x04},//PRA RF BIAS
    {0x7D5,0x04},//PRA IF BIAS 1
    {0x7D6,0x04},//PRA IF BIAS 2
    {0x7D7,0xEC},//PRA RF SCLOPE
    {0x7D8,0xF4},//PRA IF SLOPE 1
    {0x7D9,0xF4},//PRA IF SLOPE 2
    {0x7DA,0x7F},//PRA RF AGC MAXIMUM
    {0x7DB,0x7F},//PRA IF AGC MINIMUM
    {0x7DC,0x80},//PAR RF AGC MINIMUM
    {0x7DD,0x80},//PRA IF AGC MINIMUM 1
    {0x7DE,0x80},//PRA IF AGC MINIMUM 2
    {0x7DF,0X51},  //IFAGC PORIRITY
    {0x790,0x08},//DAGC DC REMOVER CONFIGURATION
    {0x794,0x38}, //DAGC Target H
    {0x7a4,0x28}, //DAGC Target L
    {0x7B7,0x10},//PRA STATIC TARGET LEVEL HIGH(MSB),degrade target level for lowif tuner(legacy)
    {0x7B9,0x10},//PRA STATIC TARGET LEVEL LOW(MSB)
    {0x00,0x00}//end flag
#else
    {0x2D4,0x04},//PRA RF BIAS
    {0x2D5,0x04},//PRA IF BIAS 1
    {0x2D6,0x04},//PRA IF BIAS 2
    {0x2D7,0xEC},//PRA RF SCLOPE
    {0x2D8,0xF4},//PRA IF SLOPE 1
    {0x2D9,0xF4},//PRA IF SLOPE 2
    {0x2DA,0x7F},//PRA RF AGC MAXIMUM
    {0x2DB,0x7F},//PRA IF AGC MINIMUM
    {0x2DC,0x80},//PAR RF AGC MINIMUM
    {0x2DD,0x80},//PRA IF AGC MINIMUM 1
    {0x2DE,0x80},//PRA IF AGC MINIMUM 2
    {0x290,0x08},//DAGC DC REMOVER CONFIGURATION
    {0x294,0x38}, //DAGC Target H
    {0x2a4,0x28}, //DAGC Target L
    {0x2B7,0x10},//PRA STATIC TARGET LEVEL HIGH(MSB),degrade target level for lowif tuner(legacy)
    {0x2B9,0x10},//PRA STATIC TARGET LEVEL LOW(MSB)
    {0x00,0x00}//end flag
#endif
};


//-----------------------------------------------------------------------------
/** RB211_TunerInit
 *  Tuner initialzation for RB211.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID RB211_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    //SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
    SPECIFIC_MEMBER_US_CTX *psTunerSpecificCtx=&(psTunerCtx->specific_member.us_ctx);
    // Rb211 specific
    rb211_result_t rb211_result = RB211_RESULT_OK;
    rb211_tuner_t *pTuner = &g_rb211_tuner_ctx.tuner;
    rb211_tuner_i2c_t *pI2c = &g_rb211_tuner_ctx.i2c;
	 
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    psTunerCtx->I2cAddress = C_RB211_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_RB211_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A = C_RB211_IF_FREQUENCY_ANA;//analog if frequency for ATD
    //ATD patch selection
    psTunerCtx->u1AtdPatchSwitch =0x09;
    
    psTunerCtx->fgRFAGC = FALSE;//indicate if tuner need extern RF_AGC
    psTunerCtx->fgRFTuner =FALSE;//not sawless tuner,it include BPF internal
    //psTunerCtx->u1EqIndex=0;//EQ index select
    //Tuner frequency range
    //pEUCtx->m_s4FreqBoundUpper = C_RB211_FREQ_DBOUND_UPPER;
    //pEUCtx->m_s4FreqBoundLower = C_RB211_FREQ_DBOUND_LOWER;
    //pEUCtx->m_s4AnaFreqBoundUpper = C_RB211_FREQ_DBOUND_UPPER_Ana;
    //pEUCtx->m_s4AnaFreqBoundLower = C_RB211_FREQ_DBOUND_LOWER_Ana;

    
    // AGC para
     psTunerCtx->sAtdAgcPara.AgcRfBias = 0x2F;     //CJ 091021//0x19;
     psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x80;
     psTunerCtx->sAtdAgcPara.AgcIfMin  = 0xD5;      //CJ 091021//0xE8;//Yueqin's test result 090925 0xEF;According to YueQin's advice 090921
     psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    // Rb211 driver initialization
    g_rb211_tuner_ctx.fgCable = 0;

    // This silicon tuner is connected via demodulator. (I2C Gateway)
    rb211_tuner_i2c_MTK_CreateI2cGw(pI2c, C_RB211_I2C_GATEWAY_ADDRESS, C_RB211_I2C_GATEWAY_SUB);

    /* Xtal = 16MHz, Sub Address = 0xC0, IF/AGC1 for Analog, IF/AGC2 for Digital */
    rb211_tuner_Create(pTuner, 16, C_RB211_LO_ADDRESS, pI2c, RB211_TUNER_CONFIG_NVMSEL_AUTO | RB211_TUNER_CONFIG_IFAGCSEL_D1A2);

    rb211_result = rb211_tuner_Initialize(pTuner);
    if(rb211_result != RB211_RESULT_OK){
        mcDBG_MSG_L2(("ERROR: rb211_tuner_Initialize is failed. (%d)\n", rb211_result));
    }

    // init EQ script
    pRB211EqNormal[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQRB211MN;
    pRB211EqWeak[MOD_NTSC_M - MOD_ANA_TYPE_BEGIN]  = EQRB211Weak;

    psTunerSpecificCtx->u2LO_Step=RB211_ANA_LO_STEP;

    mcDBG_MSG_L2(("Init " C_RB211_STR "(Cust tuner)\n"));
}


//----------------------------------------------------------------------------- 
/**  RB211_TunerGetStatus
 *  Set Tuner PLL staus for RB211/RB211
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval TRUE        PLL lock
 *  @retval FALSE       PLL unlock
 */
//-----------------------------------------------------------------------------
static BOOL RB211_TunerGetStatus(ITUNER_CTX_T *pTCtx)
{
    //ITUNER_CTX_T *psTunerCtx = pTCtx;

    rb211_result_t rb211_result = RB211_RESULT_OK;
    rb211_tuner_t *pTuner = &g_rb211_tuner_ctx.tuner;
    UINT8 data = 0;
    mcDBG_MSG_L2((" %s \n",__FUNCTION__));

    /* Get DLD */
    rb211_result = pTuner->pI2c->ReadRegister(pTuner->pI2c, pTuner->i2cAddress, 0x1D, &data, 1);
    if(rb211_result != RB211_RESULT_OK){
        return FALSE;
    }

    if(data & 0x02){
        return TRUE;
    }else{
        return FALSE;
    }
}

//----------------------------------------------------------------------------- 
/** RB211_TunerSetFreq
 *  Set Tuner PLL forRB211/RB211 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range 
                   -1 fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 RB211_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
    rb211_result_t rb211_result = RB211_RESULT_OK;
    rb211_tuner_t *pTuner = &g_rb211_tuner_ctx.tuner;

    UINT32 Freq = param->Freq; // transfer to video freq(KHz)
    UINT8 Mode = param->Modulation;

    mcDBG_MSG_L2((" %s \n",__FUNCTION__));
    pTCtx->u4RF_Freq = Freq;//Update current frequency to context

    mcDBG_MSG_L2((C_RB211_STR " Set Freq %d KHz\n",Freq));
    if (Freq < (C_RB211_FREQ_DBOUND_LOWER/1000) || 
        Freq > (C_RB211_FREQ_DBOUND_UPPER/1000))
    {
		mcDBG_MSG_ERR(("Out of range for LO!\n"));
		return 1;
    }
	
    if(Mode == MOD_ISDBT){
        mcDBG_MSG_L2(("Rb211Tuner ISDB-T SetFreq (%d kHz)\n", Freq));
        rb211_result = rb211_tuner_GPOCtrl(pTuner, 1, 1); /* GPO 1: Terr/Cable Ctrl  0: Cable, 1: Terr */
        if(rb211_result != RB211_RESULT_OK){
            return -1;
        }

	//for CXD2831 freq setting
	Freq = Freq - (UINT32)143;

        rb211_result = rb211_tuner_DigitalTune(pTuner, Freq, RB211_DTV_SYSTEM_ISDBT);
        if(rb211_result != RB211_RESULT_OK){
            return -1;
        }
    }else{
        mcDBG_MSG_L2(("Rb211Tuner Analog SetFreq (%d kHz, fgCable = %d)\n", Freq, g_rb211_tuner_ctx.fgCable));
        // Analog mode
        // Assume System-M

        // Switch input by g_rb211_tuner_ctx.fgCable flag
        rb211_result = rb211_tuner_GPOCtrl(pTuner, 1, g_rb211_tuner_ctx.fgCable ? 0 : 1); /* GPO 1: Terr/Cable Ctrl  0: Cable, 1: Terr */
        if(rb211_result != RB211_RESULT_OK){
            return -1;
        }
        // NOTE: The last argument of this function (Audio system) is not used except for RB211_ATV_AUDIO_M_DUAL_FM_STEREO (Korea).
        rb211_result = rb211_tuner_AnalogTune(pTuner, Freq, RB211_ATV_VIDEO_M, RB211_ATV_AUDIO_N_BTSC);
        if(rb211_result != RB211_RESULT_OK){
            return -1;
        }        
    }
	x_thread_delay(150);
    return 0;
}
//----------------------------------------------------------------------------- 
/** RB211_TunerGetVer
 *  Get Tuner type version
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
static CHAR *RB211_TunerGetVer(VOID)
{
    return (C_RB211_STR);
}
//----------------------------------------------------------------------------- 
/** RB211_SetInMode
 *  Set Tuner for Cable or Air input mode
 *  @param  pTunerCtx       Pointer of pi tuner driver context.
 *  @param  bMode       Pointer of pi tuner driver context.
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void  RB211_SetInMode(ITUNER_CTX_T * pTunerCtx, BOOL bMode)
{
    g_rb211_tuner_ctx.fgCable= bMode;
    mcDBG_MSG_L2(("%s, mode = %d (0: Air, 1:Cable)\n", __FUNCTION__, g_rb211_tuner_ctx.fgCable));  
}

//----------------------------------------------------------------------------- 
/** RB211_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 RB211_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){
         
    switch(eOperation){
    case itGetStatus:
        {
            BOOL * Retval=(BOOL *)pInOutVal;
            *Retval = RB211_TunerGetStatus(pTCtx);
        }
        break;
    case itGetVer:
        {
        CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=RB211_TunerGetVer();
        }
        break;
        
    case itSetInCableMode: 
        RB211_SetInMode(pTCtx,SetVal);
        break; 
        
    case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pRB211EqNormal[SetVal];
        break;
        
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pRB211EqWeak[SetVal];
        break;
                
    case itGetEqScriptStrongChroma: 
        *(UINT8**)pInOutVal = NULL;
        break;
        
    case itGetTunerRelReg:
        {
        *(ATD_TUNER_REL_REG_T **)pInOutVal=arrAtdRelReg;
        }
        break;


    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
void rb211_tuner_trace_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    mcDBG_MSG_L4(("RB211_ENTER : %s (%d)\n", funcname, linenum));
}
void rb211_tuner_trace_log_return(rb211_result_t result, const char* filename, unsigned int linenum)
{
    if(result != RB211_RESULT_OK){
        mcDBG_MSG_L4(("RB211_ERROR_RETURN(%d) : (line %d)\n", result, linenum));
    }else{
        mcDBG_MSG_L4(("RB211_RETURN : (line %d))\n", linenum));
    }
}

void rb211_tuner_trace_i2c_log_enter(const char* funcname, const char* filename, unsigned int linenum)
{
    rb211_tuner_trace_log_enter(funcname, filename, linenum);
}
void rb211_tuner_trace_i2c_log_return(rb211_result_t result, const char* filename, unsigned int linenum)
{
    rb211_tuner_trace_log_return(result, filename, linenum);
}
