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
 * $RCSfile: pi_tuner_SI2156.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_SI2156.c
 *  Tuner control for SS SI2156
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "demod_tuner_interface.h"
//#include "pd_i2capi.h"
#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "si2156_i2c_api.h"
#include "x_typedef.h"
//#include "tunerDebug.h"
#include "x_os.h"
#include "PD_Def.h"
#include "pi_def_dvbt.h"

// add your messaging command here for debugging.

#include "si2156_L1_API.h"
#include "1Eb6_patch.h"
#include "../re201/eq_script_re201.h"

 
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Frequency boundary .Get from tuner spec
#define C_SI2156_FREQ_DBOUND_UPPER   859*1000*1000
#define C_SI2156_FREQ_DBOUND_LOWER    48*1000*1000
#define C_SI2156_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_SI2156_FREQ_DBOUND_LOWER_Ana    45*1000*1000

//PLL lock check parameters
#define C_SI2156_POLL_INTERVAL      5 //ms
#define C_SI2156_PLL_POLL_TIMETOUT      100 //ms
#define C_SI2156_PLL_POLL_CNT           C_SI2156_PLL_POLL_TIMETOUT/C_SI2156_POLL_INTERVAL //counter

#define C_SI2156_PF_TO_CF_SHIFT_BG 2250 /*KHZ*/
#define C_SI2156_PF_TO_CF_SHIFT_DK 2750 /*KHZ*/
#define C_SI2156_PF_TO_CF_SHIFT_I  2750 /*KHZ*/
#define C_SI2156_PF_TO_CF_SHIFT_L  2250 /*KHZ*/
#define C_SI2156_PF_TO_CF_SHIFT_M  1750 /*KHZ*/
#define C_SI2156_PF_TO_CF_SHIFT_L1 2750 /*KHZ*/
#define C_SI2156_IF_CENTER_BG 5000 /*KHZ*/
#define C_SI2156_IF_CENTER_DK 5000 /*KHZ*/
#define C_SI2156_IF_CENTER_I  5000 /*KHZ*/
#define C_SI2156_IF_CENTER_L  5000 /*KHZ*/
#define C_SI2156_IF_CENTER_L1 5000 /*KHZ*/
#define C_SI2156_IF_CENTER_M  5000 /*KHZ*/

#define C_SI2156_IF_FREQUENCY          ((UINT16)  5000)  /* kHz */


#define C_SI2156_LO_ADDRESS           ((UINT8)  0xC0)
//#define C_SI2156_IF_FREQUENCY          ((UINT16)  36130)  /* kHz */
//#define C_SI2156_IF_FREQUENCY_ANA      ((UINT16)  38900)  /* kHz */
//#define C_SI2156_IF_FREQUENCY_L1       ((UINT16)  33900)  /* kHz */
#define C_SI2156_LO_DIVIDER_STEP       ((UINT32) 1000)  /* Hz */
#define C_SI2156_LO_DIVIDER_STEP_ANA   ((UINT16)  1000)  /* Hz */
#define C_SILABS_SI2156_STR                        "SILABS_SI2156"

#define C_SI2156_TOP_SET             ((U8)   0x02) 
#define C_SI2156_TOP_SET_DVBC        ((U8)   0x04) 
#define C_SI2156_TOP_SET_ANA         ((U8)   0x0A) 
#define C_SI2156_SAW_BW              SAW_BW_8M 

#define C_SI2156_AGC_IF_SLP           0.125	
#define C_SI2156_AGC_IF_INT           0.5   
#define C_SI2156_AGC_IF_MAX           0.998  // 0.5 will overflow  //0v
#define C_SI2156_AGC_IF_MIN          0.0  
#define C_SI2156_AGC_IF_SLP_SGN  (S8)((C_SI2156_AGC_IF_SLP>0)?1:-1) 
#define C_SI2156_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_SI2156_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256

#define C_SI2156_MAX_IF_GAIN_SET       -0.12   
#define C_SI2156_MIN_IF_GAIN_SET       -0.38   
#define C_SI2156_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_SI2156_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_SI2156_MAX_IF_GAIN_POW       -100    // dBm
#define C_SI2156_MIN_IF_GAIN_POW		-50     // dBm
#define C_SI2156_MAX_RF_GAIN_POW		 C_SI2156_MIN_IF_GAIN_POW
#define C_SI2156_MIN_RF_GAIN_POW		-20     // dBm
#define C_SI2156_POWER_CALIBRATE        26     // dBm


#define C_SI2156_AGC_COEF			((U8)   0xBD)
#define C_SI2156_DEMOD_INPUT_POWER	cIF_TARGET_LEVEL_0CCC_0D36

// ********************************************* //

#define C_U8_SI2156_AGC_IF_SLP         (S8)(C_SI2156_AGC_IF_SLP *64 )  // MT5135(S(1, -6)), -0.125*64 = -8 (0xF8), -8/64 = -1/8
#define C_U8_SI2156_AGC_IF_INT         (S8)(C_SI2156_AGC_IF_INT *16 ) // MT5135(S(3, -4))  0.5*16 = 8, 8/16 =0.5
#define C_U8_SI2156_AGC_IF_MAX         (S8)(C_SI2156_AGC_IF_MAX *128)    //0.499*256 = 0x7F
#define C_U8_SI2156_AGC_IF_MIN         (S8)(C_SI2156_AGC_IF_MIN *128)    //0
#define C_U8_SI2156_AGC_IF_LVL_MAX        (S8)(C_SI2156_AGC_IF_LVL_MAX *256)
#define C_U8_SI2156_AGC_IF_LVL_MIN         (S8)(C_SI2156_AGC_IF_LVL_MIN *256)

#define C_U8_SI2156_MAX_IF_GAIN_SET	(S8)(C_SI2156_MAX_IF_GAIN_SET *256)
#define C_U8_SI2156_MIN_IF_GAIN_SET	(S8)(C_SI2156_MIN_IF_GAIN_SET *256)
#define C_U8_SI2156_MAX_RF_GAIN_SET	(S8)((C_SI2156_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_SI2156_MIN_RF_GAIN_SET	(S8)((C_SI2156_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_SI2156_IF_GAIN_RANGE      (S8)(C_SI2156_MIN_IF_GAIN_POW - C_SI2156_MAX_IF_GAIN_POW)
#define C_U8_SI2156_RF_GAIN_RANGE      (S8)(C_SI2156_MIN_RF_GAIN_POW - C_SI2156_MAX_RF_GAIN_POW)

STATIC UINT8* pSI2156[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
STATIC UINT8* pSI2156EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};




STATIC ATD_TUNER_REL_REG_T arrAtdRelReg[]={
{0x2d4,0x04},
{0x2d5,0x00},
{0x2d6,0x00},
{0x2d7,0xEC},
{0x2d8,0xEF},
{0x2d9,0xEF},
{0x2da,0x7F},
{0x2db,0x7F},
{0x2dc,0x80},
{0x2dd,0x80},
{0x2de,0x80},
{0x2df,0x00},
{0x2b7,0x12},
{0x2b9,0x0e},
{0x13b,0x80},
{0x290,0x08},
{0x294,0x38},
{0x2a4,0x28},
{0x00,0x00}
};
STATIC ATD_TUNER_REL_REG_T arrAtdRelRegSecam[]={
{0x2d4,0x04},
{0x2d5,0x00},
{0x2d6,0x00},
{0x2d7,0xEC},
{0x2d8,0xEF},
{0x2d9,0xEF},
{0x2da,0x7F},
{0x2db,0x7F},
{0x2dc,0x80},
{0x2dd,0x80},
{0x2de,0x80},
{0x2df,0x00},
{0x2b7,0x07},
{0x2b9,0x03},
{0x13b,0x80},
{0x290,0x88},
{0x00,0x00}
};

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------
//Signal Power Sample Array Size
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------------------------------------------------------------------------------------------------------
L1_Si2156_Context api_context={0};
L1_Si2156_Context *api;
UINT8 VideoStandard;

typedef enum TV_MODES
{
    ANALOG_MODE,
    DIGITAL_MODE
} TV_MODES;

typedef enum TUNER_STATUSES
{
    CHL,
    PCL,
    DTV_CHL,
    RSSI,
} TUNER_STATUSES;

/************************************************************************************************************************
  NAME: PCFreq_To_CenterFreq
  DESCRIPTION: Convert Picture Carrier frequency to Center Frequency
  Parameter:  System
  Parameter:  PC Frequency
  Returns:    I2C transaction error code, 0 if successful
************************************************************************************************************************/
UINT32 PCFreq_To_CenterFreq(UINT8 u1SubSysIdx,UINT32 pc)
{
	UINT32 cf=0;
	switch (u1SubSysIdx)
	{
		case MOD_PAL_BG:
			cf = pc + C_SI2156_PF_TO_CF_SHIFT_BG*1000;
			break;
		case MOD_PAL_DK:
			cf = pc + C_SI2156_PF_TO_CF_SHIFT_DK*1000;
			break;
		case MOD_PAL_I:
			cf = pc + C_SI2156_PF_TO_CF_SHIFT_I*1000;
			break;
		case MOD_NTSC_M:
            cf = pc + C_SI2156_PF_TO_CF_SHIFT_M*1000;
            break;
        case MOD_SECAM_L:
            cf = pc + C_SI2156_PF_TO_CF_SHIFT_L*1000;
            break;
        case MOD_SECAM_L1:
            cf = pc - C_SI2156_PF_TO_CF_SHIFT_L1*1000;
            break;
	}
	return cf;
}
/************************************************************************************************************************
  NAME: Si2156_LoadFirmware
  DESCRIPTON: Load firmware from FIRMWARE_TABLE array in Si2156_Firmware_x_y_build_z.h file into Si2156
              Requires Si2156 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2156 Context (I2C address)
  Returns:    Si2156/I2C transaction error code, 0 if successful
************************************************************************************************************************/
int Si2156_LoadFirmware        (L1_Si2156_Context *api, unsigned char fw_table[], int nbLines)
{
    int return_code = 0;
    int line;

    SiTRACE("Si2156_LoadFirmware nbLines %d\n", nbLines);

    /* for each 8 bytes in fw_table */
    for (line = 0; line < nbLines; line++)
    {
        /* send that 8 byte I2C command to Si2156 */
        if ((return_code = Si2156_L1_API_Patch(api, 8, fw_table+8*line)) != 0)
        {
          SiTRACE("Si2156_LoadFirmware error 0x%02x patching line %d: %s\n", return_code, line, Si2156_L1_API_ERROR_TEXT(return_code) );
          if (line == 0) {
          SiTRACE("The firmware is incompatible with the part!\n");
          }
          return ERROR_Si2156_LOADING_FIRMWARE;
        }

    }
    SiTRACE ("Si2156_LoadFirmware complete...\n");
    return 0;
}
/************************************************************************************************************************
  NAME: configureSi2156
  DESCRIPTION: Setup Si2156 video filters, GPIOs/clocks, Common Properties startup, Tuner startup, ATV startup, and DTV startup.
  Parameter:  Pointer to Si2156 Context (I2C address)
  Returns:    I2C transaction error code, 0 if successful
************************************************************************************************************************/
int Si2156_Configure           (L1_Si2156_Context *api)
{
    int return_code;
    return_code=0;

    /* Set All Properties startup configuration */
    api->prop->tuner_ien.tcien = Si2156_TUNER_IEN_PROP_TCIEN_ENABLE;
    api->prop->tuner_ien.rssilien = Si2156_TUNER_IEN_PROP_RSSILIEN_DISABLE;
    api->prop->tuner_ien.rssihien = Si2156_TUNER_IEN_PROP_RSSIHIEN_DISABLE;
    if ((return_code=Si2156_L1_SetProperty2(api,Si2156_TUNER_IEN_PROP)) != 0)
    {
       SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
       return return_code;
    }
    api->prop->dtv_ien.chlien = Si2156_DTV_IEN_PROP_CHLIEN_ENABLE;
    if ((return_code=Si2156_L1_SetProperty2(api,Si2156_DTV_IEN_PROP)) != 0)
    {
       SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
       return return_code;
    }
     api->prop->atv_ien.chlien = Si2156_ATV_IEN_PROP_CHLIEN_ENABLE;
     api->prop->atv_ien.pclien = Si2156_ATV_IEN_PROP_PCLIEN_DISABLE;
    if ((return_code=Si2156_L1_SetProperty2(api,Si2156_ATV_IEN_PROP)) != 0)
    {
       SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
       return return_code;
    }
    api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_8MHZ;
    api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_DVBT;
    api->prop->dtv_mode.invert_spectrum = Si2156_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL;
    if ((return_code=Si2156_L1_SetProperty2(api,Si2156_DTV_MODE_PROP)) != 0)
    {
       SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
       return return_code;
    }

    api->prop->atv_video_mode.color= Si2156_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
    api->prop->atv_video_mode.invert_spectrum= Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_NORMAL;
    api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL;
    api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH;
   if ((return_code=Si2156_L1_SetProperty2(api,Si2156_ATV_VIDEO_MODE_PROP)) != 0)
    {
       SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
       return return_code;
    }

    return 0;
}

/** SI2156_TunerInit
 *  Tuner initialzation forSI2156.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */

//-----------------------------------------------------------------------------
VOID SI2156_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);
	SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);
    

	psTunerCtx->u2LO_Step=1000;  //*1Khz
	pUSCtx->u2LO_Step = 1000; //*1KHZ
    psTunerCtx->I2cAddress = C_SI2156_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_SI2156_IF_FREQUENCY;
	psTunerCtx->u2IF_Freq_A = 0; // C_SI2156_IF_FREQUENCY_ANA;//analog if frequency for ATD
	psTunerCtx->u4RF_Freq = 0;

	 // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x00;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x00; 
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B; 

    //ATD patch selection
    psTunerCtx->u1AtdPatchSwitch =0x09;
    psTunerCtx->fgRFAGC = FALSE;//indicate if tuner need extern RF_AGC
	psTunerCtx->fgRFTuner =FALSE;//Digital tuner

    // iven 20110726, add for polarity if setting for NA reference
    psTunerCtx->bSpecPolarity = 0;  // need confrim by tuner responser
    // iven 20110726, add for legacy tuner as default fixed PGA gain index. Don care for Sawless
    psTunerCtx->uSLD_DefaultPGAIndex = 0;	


	
	psTunerCtx->u1EqIndex=0;//EQ index select
	//Tuner frequency range
    pEUCtx->m_s4FreqBoundUpper = C_SI2156_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_SI2156_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_SI2156_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_SI2156_FREQ_DBOUND_LOWER_Ana;

	pEUCtx->m_SAW_BW  = C_SI2156_SAW_BW;
    pEUCtx->m_Ana_Top = C_SI2156_TOP_SET_ANA;	
	// for Signal Level display
		pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_SI2156_AGC_IF_LVL_MAX;
		pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_SI2156_AGC_IF_LVL_MIN;
		
		pEUCtx->m_SigLvTh = 0;
		pEUCtx->m_SigLvScan = 0;
		
		pEUCtx->m_aucPara[0] = C_SI2156_TOP_SET;
		pEUCtx->m_aucPara[1] = 0;
		
		pEUCtx->m_aucPara[2] = (U8)C_U8_SI2156_AGC_IF_SLP;
		pEUCtx->m_aucPara[3] = (U8)C_U8_SI2156_AGC_IF_INT;
		pEUCtx->m_aucPara[4] = (U8)C_U8_SI2156_AGC_IF_MAX;
		pEUCtx->m_aucPara[5] = (U8)C_U8_SI2156_AGC_IF_MIN;
		
		pEUCtx->m_aucPara[6] = C_SI2156_AGC_COEF;
		pEUCtx->m_aucPara[7] = C_SI2156_DEMOD_INPUT_POWER;
		
		pEUCtx->m_aucPara[ 8] = (U8)C_U8_SI2156_MAX_IF_GAIN_SET;
		pEUCtx->m_aucPara[ 9] = (U8)C_U8_SI2156_MIN_IF_GAIN_SET;
		pEUCtx->m_aucPara[10] = (U8)C_U8_SI2156_MAX_RF_GAIN_SET;
		pEUCtx->m_aucPara[11] = (U8)C_U8_SI2156_MIN_RF_GAIN_SET;
		pEUCtx->m_aucPara[12] = (U8)C_U8_SI2156_IF_GAIN_RANGE;
		pEUCtx->m_aucPara[13] = (U8)C_U8_SI2156_RF_GAIN_RANGE;
		pEUCtx->m_aucPara[14] = C_SI2156_POWER_CALIBRATE;
	//IF MAX setting!
		pEUCtx->m_aucPara[21] = 0x7F;
		pSI2156[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRe201BG;
		pSI2156[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRe201DK;
		pSI2156[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRe201I;
		pSI2156[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRe201L;	
		pSI2156[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRe201L1;
		pSI2156[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQRe201M;

		pSI2156EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQRe201BG;
		pSI2156EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQRe201DK;
		pSI2156EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQRe201I;
		pSI2156EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQRe201L;	
		pSI2156EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQRe201L1;
		pSI2156EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]	= EQRe201M;
			

	//ADD YOU TUNER DRIVER CODE IN HERE
	api = &api_context;
    int return_code =0;
    /* Software Init */
    Si2156_L1_API_Init(api,C_SI2156_LO_ADDRESS);

   if ((return_code=Si2156_pollForCTS(api)) != 0) {
        SiTRACE ("Si2156_pollForCTS error 0x%02x\n", return_code);

    }
    if ((return_code = Si2156_L1_POWER_UP (api,
                            Si2156_POWER_UP_CMD_SUBCODE_CODE,
                            Si2156_POWER_UP_CMD_RESERVED1_RESERVED,
                            Si2156_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2156_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2156_POWER_UP_CMD_CLOCK_MODE_XTAL,
                            Si2156_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2156_POWER_UP_CMD_ADDR_MODE_CURRENT,
                            Si2156_POWER_UP_CMD_FUNC_BOOTLOADER,        /* start in bootloader mode */
                            Si2156_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2156_POWER_UP_CMD_WAKE_UP_WAKE_UP)) != 0)
    {
        SiTRACE ("Si2156_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );

    }
   /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2156_L1_PART_INFO(api)) != 0) {
        SiTRACE ("Si2156_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );

    }
        SiTRACE ("Part Number = Si21%d Rev %c%c%c , ROMID= 0x%02x,  %s\n", api->rsp->part_info.part, api->rsp->part_info.chiprev-0x40,api->rsp->part_info.pmajor,api->rsp->part_info.pminor,api->rsp->part_info.romid );

    if ((return_code = Si2156_LoadFirmware(api, Si2156_FW_1_eb6, FIRMWARE_LINES_1_eb6)) != 0) {
         SiTRACE ("Si2156_LoadFirmware error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );

    }
     if ((return_code = Si2156_L1_EXIT_BOOTLOADER(api, Si2156_EXIT_BOOTLOADER_CMD_FUNC_TUNER, Si2156_EXIT_BOOTLOADER_CMD_CTSIEN_OFF)) != 0)
    {
        SiTRACE ("Si2156_StartFirmware error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );

    }
     Si2156_Configure (api);

     mcSHOW_DBG_MSG(("Init " C_SILABS_SI2156_STR "(Cust tuner)\n"));
}
/* Setup properties to switch standards. */
void SetIfDemod(ITUNER_CTX_T *pTCtx,UINT8 subSysIdx)
{
    // set a global here to save the video standard,  because it's not always available from the caller.
   	VideoStandard =subSysIdx;
    switch (subSysIdx)
    {
        case MOD_PAL_BG:
            api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B;
            api->prop->atv_video_mode.color = Si2156_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_SI2156_IF_CENTER_BG;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }   
              api->prop->atv_lif_out.offset = 148;
              api->prop->atv_lif_out.amp = 84;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_OUT_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
			
            break;
        case MOD_PAL_DK:
            api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK;
            api->prop->atv_video_mode.color = Si2156_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_SI2156_IF_CENTER_DK;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
			 api->prop->atv_lif_out.offset = 148;
              api->prop->atv_lif_out.amp = 84;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_OUT_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }

            break;
        case MOD_PAL_I:
            api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I;
            api->prop->atv_video_mode.color = Si2156_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_SI2156_IF_CENTER_I;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
			 api->prop->atv_lif_out.offset = 148;
              api->prop->atv_lif_out.amp = 84;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_OUT_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }

            break;
        case MOD_NTSC_M:
            api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M;
            api->prop->atv_video_mode.color = Si2156_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_SI2156_IF_CENTER_M;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
			 api->prop->atv_lif_out.offset = 148;
              api->prop->atv_lif_out.amp = 84;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_OUT_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
            break;
        case MOD_SECAM_L:
            api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L;
            api->prop->atv_video_mode.color = Si2156_ATV_VIDEO_MODE_PROP_COLOR_SECAM;
            api->prop->atv_video_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_SI2156_IF_CENTER_L;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
			 api->prop->atv_lif_out.offset = 148;
              api->prop->atv_lif_out.amp = 84;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_OUT_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
            break;
        case MOD_SECAM_L1:
            api->prop->atv_video_mode.video_sys = Si2156_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP;
            api->prop->atv_video_mode.color = Si2156_ATV_VIDEO_MODE_PROP_COLOR_SECAM;
            api->prop->atv_video_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            api->prop->atv_video_mode.trans = Si2156_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_SI2156_IF_CENTER_L1;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
			 api->prop->atv_lif_out.offset = 148;
              api->prop->atv_lif_out.amp = 84;
            if (Si2156_L1_SetProperty2(api, Si2156_ATV_LIF_OUT_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_ATV_LIF_FREQ_PROP\n"));
            }
            break;
       case MOD_DVBT:
            api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_DVBT;
            if(pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_6M)
				api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_6MHZ;
			else if (pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_7M)
				api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_7MHZ;
			else
                api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_8MHZ;
			
            api->prop->dtv_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2156_L1_SetProperty2(api, Si2156_DTV_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_SI2156_IF_FREQUENCY;
            if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
            }
			api->prop->dtv_lif_out.offset = Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT; 
			api->prop->dtv_lif_out.amp =Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
	        if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_OUT_PROP) != 0)
			{
				mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
			}




            break;
       case MOD_DVBT2:
            api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_DVBT;
            api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_8MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2156_L1_SetProperty2(api, Si2156_DTV_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_SI2156_IF_FREQUENCY;
            if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
            }
			api->prop->dtv_lif_out.offset = Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT; 
			api->prop->dtv_lif_out.amp =Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
	        if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_OUT_PROP) != 0)
			{
				mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
			}
            break;
       case MOD_DVBC:
            api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_DVBC;
            api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_8MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2156_L1_SetProperty2(api, Si2156_DTV_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_MODE_PROP\n"));
            }
           api->prop->dtv_lif_freq.offset=C_SI2156_IF_FREQUENCY;
            if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
            }
			api->prop->dtv_lif_out.offset = Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT; 
			api->prop->dtv_lif_out.amp =32;
	        if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_OUT_PROP) != 0)
			{
				mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
			}
            break;
      case MOD_ATSC_8VSB:
            api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_ATSC;
            api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_6MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2156_L1_SetProperty2(api, Si2156_DTV_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_SI2156_IF_FREQUENCY;
            if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
            }
			api->prop->dtv_lif_out.offset = Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT; 
			api->prop->dtv_lif_out.amp =Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
	        if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_OUT_PROP) != 0)
			{
				mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
			}
            break;
        case MOD_DTMB:
            api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_DTMB;
            api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_6MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2156_L1_SetProperty2(api, Si2156_DTV_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_SI2156_IF_FREQUENCY;
            if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
            }
			api->prop->dtv_lif_out.offset = Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT; 
			api->prop->dtv_lif_out.amp =Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
	        if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_OUT_PROP) != 0)
			{
				mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
			}
            break;
       case MOD_ISDBT:
            api->prop->dtv_mode.modulation = Si2156_DTV_MODE_PROP_MODULATION_ISDBT;
            api->prop->dtv_mode.bw = Si2156_DTV_MODE_PROP_BW_BW_6MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2156_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2156_L1_SetProperty2(api, Si2156_DTV_MODE_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_SI2156_IF_FREQUENCY;
            if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_FREQ_PROP) != 0)
            {
                mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
            }
			api->prop->dtv_lif_out.offset = Si2156_DTV_LIF_OUT_PROP_OFFSET_DEFAULT; 
			api->prop->dtv_lif_out.amp =Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
	        if (Si2156_L1_SetProperty2(api, Si2156_DTV_LIF_OUT_PROP) != 0)
			{
				mcSHOW_HW_MSG((" [Silabs]: ERROR_SETTING_PROPERTY Si2156_DTV_LIF_FREQ_PROP\n"));
			}
            break;
    }


}
//-----------------------------------------------------------------------------
/** SI2156_TunerSetFreq
 *  Set Tuner PLL forSI2156/SI2156 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range
                   -1 fail  I2C error
 */
//-----------------------------------------------------------------------------
INT16 SI2156_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
  UINT32 Freq = param->Freq; // transfer to video freq(KHz) digital center freq ,analog picture freq
  UINT8 Mode = param->Modulation;    //tv system
  int count=0;
  int tune_count=0;
  UINT32 freqHz =0;
  UINT32 u2TickStart_eu;
  TV_MODES tvMode;
  u2TickStart_eu = mcGET_SYS_TICK();

 // SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);
  SiTRACE ("SI2156_TunerSetFreq freq = %d\n", Freq );

  SetIfDemod(pTCtx,Mode);
  if(Mode == MOD_PAL_BG)
  pTCtx->u2IF_Freq_A = C_SI2156_IF_CENTER_BG+C_SI2156_PF_TO_CF_SHIFT_BG;
  if(Mode == MOD_PAL_DK)
  pTCtx->u2IF_Freq_A = C_SI2156_IF_CENTER_DK+C_SI2156_PF_TO_CF_SHIFT_DK;
  if(Mode == MOD_PAL_I)
  pTCtx->u2IF_Freq_A = C_SI2156_IF_CENTER_I+C_SI2156_PF_TO_CF_SHIFT_I;
  if(Mode == MOD_NTSC_M)
  pTCtx->u2IF_Freq_A = C_SI2156_IF_CENTER_M+C_SI2156_PF_TO_CF_SHIFT_M;
  if(Mode == MOD_SECAM_L)
  pTCtx->u2IF_Freq_A = C_SI2156_IF_CENTER_L+C_SI2156_PF_TO_CF_SHIFT_L;
  if(Mode == MOD_SECAM_L1)
  pTCtx->u2IF_Freq_A = C_SI2156_IF_CENTER_L1+C_SI2156_PF_TO_CF_SHIFT_L1;

  
  if (Mode < MOD_TYPE_END)
      tvMode = DIGITAL_MODE;
  else
      tvMode=ANALOG_MODE;

/* Use Picture Carrier Frequency if ATV and Center Frequency if DTV */
  if (tvMode==ANALOG_MODE)
    freqHz = PCFreq_To_CenterFreq( Mode,Freq * 1000);
  else
    freqHz = Freq * 1000;

  if (freqHz < Si2156_TUNER_TUNE_FREQ_CMD_FREQ_MIN || freqHz > Si2156_TUNER_TUNE_FREQ_CMD_FREQ_MAX)
	{
		mcSHOW_HW_MSG(("[Silabs]:<= Response out of Range\n"));
		return ERROR_Si2156_PARAMETER_OUT_OF_RANGE; // =1
	}
   if (Si2156_L1_TUNER_TUNE_FREQ (api,(tvMode==ANALOG_MODE) ? Si2156_TUNER_TUNE_FREQ_CMD_MODE_ATV : Si2156_TUNER_TUNE_FREQ_CMD_MODE_DTV,  freqHz) != 0)
	{
	    mcSHOW_DBG_MSG(("[Silabs]:Error Si2170_L1_TUNER_TUNE_FREQ\n"));
        return -1;
    }

     /* wait for TUNINT, timeout is 86ms */
   for (count=17; count ;count--)
	{
        if (( Si2156_L1_CheckStatus(api)) != 0)
        {
            mcSHOW_DBG_MSG(("[Silabs]:Error Si2156_L1_CheckStatus\n"));
            return ERROR_Si2156_SENDING_COMMAND;
        }
        if (api->status->tunint)
            break;
        //x_thread_delay(C_SI2156_POLL_INTERVAL);
        system_wait(5);
    }
   tune_count=count;

    /* wait for ATVINT, timeout is 150ms for ATVINT and 10 ms for DTVINT */
     if (tvMode==ANALOG_MODE)
        count =  30;
     else
        count = 2;

   for (;count ;count--)
	{
        if ((Si2156_L1_CheckStatus(api)) != 0)
        	{
        	 mcSHOW_DBG_MSG(("[Silabs]:Error reading STATUS\n"));
            return FALSE;
        	}
        	if (tvMode==ANALOG_MODE)
        	{
                if (api->status->atvint)
                    break;
        	}
        	else
        	{
                if (api->status->dtvint)
                    break;
        	}
        system_wait(5);
      //x_thread_delay(C_SI2156_POLL_INTERVAL);
    }
  mcSHOW_DBG_MSG(("[Silabs]:SI2156TunerSetFreq use time = (%3u ms) tun=%d atv=%d\n", (mcGET_SYS_TICK() - u2TickStart_eu) * mcGET_TICK_PERIOD(),tune_count,count));

    return (0);
}
/* Get Status information from part */
UCHAR GetStatus( L1_Si2156_Context *api,TUNER_STATUSES status)
{
    int return_code;
    if (status==CHL)
    {
        if ((return_code=Si2156_L1_ATV_STATUS(api,Si2156_ATV_STATUS_CMD_INTACK_CLEAR))!=0) {
             SiTRACE ("Si2156_L1_ATV_STATUS error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );
        }
        return api->rsp->atv_status.chl;
    }
    else if (status == DTV_CHL)
    {
        if ((return_code=Si2156_L1_DTV_STATUS(api,Si2156_DTV_STATUS_CMD_INTACK_CLEAR))!=0) {
         SiTRACE ("Si2156_L1_DTV_STATUS error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );
        }
        return api->rsp->dtv_status.chl;
    }
    else
        return 0;


}

/* Get RSSI  from part */
CHAR GetSignalLevel(L1_Si2156_Context *api)
{
    int return_code;
    if ((return_code=Si2156_L1_TUNER_STATUS(api,Si2156_TUNER_STATUS_CMD_INTACK_CLEAR))!=0) {
     SiTRACE ("Si2156_L1_TUNER_STATUS error 0x%02x: %s\n", return_code, Si2156_L1_API_ERROR_TEXT(return_code) );

    }

    return api->rsp->tuner_status.rssi;

}

//-----------------------------------------------------------------------------
/** SI2156_TunerGetVer
 *  Get Tuner type version
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
static CHAR *SI2156_TunerGetVer(VOID)
{
    return ((CHAR*)C_SILABS_SI2156_STR);
}
//-----------------------------------------------------------------------------
/** SI2156_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 SI2156_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    //L1_Si2156_Context *api;
    int return_code =0;
	
	//api->prop->dtv_mode.bw = 8;
switch(eOperation){

	case itGetEqScriptNormal: 
        *(UINT8**)pInOutVal = pSI2156[SetVal];;
        break;
    case itGetEqScriptWeak: 
        *(UINT8**)pInOutVal = pSI2156EqWeak[SetVal];
        break;
	case itGetTunerRelReg://Get ATD Tuner Rel register setting
	    {
		   	if((SetVal==MOD_SECAM_L)||(SetVal==MOD_SECAM_L1)){
				*(ATD_TUNER_REL_REG_T **)pInOutVal = arrAtdRelRegSecam;}
			else{
            *(ATD_TUNER_REL_REG_T **)pInOutVal = arrAtdRelReg;}
		}  
	break;
	/*case itTagetLevel:  //set if input target level
				{
					if((SetVal==MOD_SECAM_L)||(SetVal==MOD_SECAM_L1))
						pInOutVal = 0x05;
					else
						pInOutVal = 0x10;}
				break;*/
    case itGetVer:
        {CHAR ** RetStr=(CHAR **)pInOutVal;
        *RetStr=SI2156_TunerGetVer();
		//CHAR ** RetStr = (CHAR **)pInOutVal;
        //*RetStr = SI2156_GetSwVer();
    	}
        break;
	case itGetTop:
          if (SetVal >= MOD_ANA_TYPE_BEGIN)
             {
              if ((return_code=Si2156_L1_GetProperty2(api,Si2156_ATV_RF_TOP_PROP))!=0)
               {
                    SiTRACE ("Si2156_L1_GetProperty2 error 0x%02x\n", return_code);
                    return ITUNER_NOT_OK;
               }
                *(UINT8 *)pInOutVal= api->prop->atv_rf_top.atv_rf_top;
             }
             else
             {
              if ((return_code=Si2156_L1_GetProperty2(api,Si2156_DTV_RF_TOP_PROP))!=0)
               {
                    SiTRACE ("Si2156_L1_GetProperty2 error 0x%02x\n", return_code);
                    return ITUNER_NOT_OK;
               }
                *(UINT8 *)pInOutVal= api->prop->dtv_rf_top.dtv_rf_top;
             }

     break;
     case itSetIfDemod:
        //    SetIfDemod(SetVal);
     break;
    case itSetTop:
     {
          if(VideoStandard >= MOD_ANA_TYPE_BEGIN)
          {
            api->prop->atv_rf_top.atv_rf_top=SetVal;
            if ((return_code=Si2156_L1_SetProperty2(api,Si2156_ATV_RF_TOP_PROP)) != 0)
            {
               SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
               return ITUNER_NOT_OK;
            }
          }
          else
          {
            api->prop->dtv_rf_top.dtv_rf_top=SetVal;
            if ((return_code=Si2156_L1_SetProperty2(api,Si2156_DTV_RF_TOP_PROP)) != 0)
            {
               SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
               return ITUNER_NOT_OK;
            }

          }

     }
	 break;
	 case itSetSawBw://Set Channel Bandwidth
         pTCtx->specific_member.eu_ctx.m_SAW_BW = SetVal;
	 /*
		 if(SetVal== SAW_BW_6M)
			 api->prop->dtv_mode.bw = 6;
		 else if(SetVal== SAW_BW_7M)
			 api->prop->dtv_mode.bw = 7;
         else
	      api->prop->dtv_mode.bw = 8;

		 
          if ((return_code=Si2156_L1_SetProperty2(api,Si2156_DTV_MODE_PROP)) != 0)
            {
               SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
               return ITUNER_NOT_OK;
            }*/
	  break;
	  case itGetStatus: // return channel locked status.
          if (SetVal >= MOD_ANA_TYPE_BEGIN)
            *(UINT8 *)pInOutVal = GetStatus( api,CHL);
          else
            *(UINT8 *)pInOutVal = GetStatus( api,DTV_CHL);
	  break;
	  case itGetSignalLevel: // Get RSSI
            *(CHAR *)pInOutVal = GetSignalLevel(api);
	  break;
	  case itSetIF: // set ATV/DTV LIF FREQ
          if (VideoStandard >= MOD_ANA_TYPE_BEGIN)
             {
                 api->prop->atv_lif_freq.offset = SetVal;
                if ((return_code=Si2156_L1_SetProperty2(api,Si2156_ATV_LIF_FREQ_PROP)) != 0)
                {
                   SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
                   return ITUNER_NOT_OK;
                }

             }
             else
            {
                 api->prop->dtv_lif_freq.offset = SetVal;
                if ((return_code=Si2156_L1_SetProperty2(api,Si2156_DTV_LIF_FREQ_PROP)) != 0)
                {
                   SiTRACE ("Si2156_L1_SetProperty2 error 0x%02x\n", return_code);
                   return ITUNER_NOT_OK;
                }

            }

	  break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
