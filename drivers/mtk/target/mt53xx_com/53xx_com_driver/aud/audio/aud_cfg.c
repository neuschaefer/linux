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
 * $Author: kai.wang $
 * $Date: 2013/01/18 $
 * $RCSfile: aud_cfg.c,v $
 * $Revision: #13 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_cfg.c
 *  Brief of file aud_cfg.c.
 *  Details of file aud_cfg.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL


LINT_EXT_HEADER_BEGIN

//#include "adc_if.h"
#include "x_util.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "x_os.h"
#include "x_hal_5381.h"
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
#include "x_pdwnc.h"
#endif

#include "sif_if.h"
#include "aud_debug.h"
#include "aud_cfg.h"
#include "adac_if.h"
#include "spdif_drvif.h"
#include "aud_if.h"
#include "aud_hw.h"
#include "aud_drv.h"
#include "aud_drvif.h"
#include "psr_drvif.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "dsp_intf.h"
#include "aud_pll.h"
#include "aud_dsp_cfg.h"
#include "dsp_common.h"
#include "../periph/codec_mt5365.h"
#include "x_aud_dec.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//Laihui: Remove it for APLL setting
//#define APLL_WORKAROUND
//#define SUPPORT_DUAL_UART1

#define USE_16BIT_ALIGNMENT

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define AUD_GPIO_SWITCH_0       (17)
#define AUD_GPIO_SWITCH_1       (15)
#define SW_NUMBER               (8)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef enum
{
    I2S_IN_LINEIN = 0,  // Line in 
    I2S_IN_SPDIF,       // SPDIF
    I2S_IN_HDMI         // HDMI
}   I2S_IN_SEL;

//---------------------------------------------------------------------------
// Macro 
//---------------------------------------------------------------------------

#define    REGISTER_ACCESS_TIME_BARRIER(u4Tick)      {  \
                                                        volatile UINT32 i, j; \
                                                        for (i=0; i<u4Tick; i++)  \
                                                        {                     \
                                                            j = i;            \
                                                            UNUSED(j);  \
                                                        }                     \
                                                     }   



#define SAMP_CHG_LOCK()      VERIFY(x_sema_lock(_hSmpChgSema, X_SEMA_OPTION_WAIT) == OSR_OK)
#define SAMP_CHG_UNLOCK()    VERIFY(x_sema_unlock(_hSmpChgSema) == OSR_OK)

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                           AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5, 
                           UINT8 PAD_MCLK0);
extern void vDspSetFifoReadPtr(UCHAR ucDecId, UINT32 u4ReadPtr);
extern BOOL fgDspReadPtrSet (UCHAR ucDecId, UINT32 u4Address);
extern BOOL fgHDMIAudioClkOk(void);

extern void AUD_DspDvdEnable(void);
extern void AUD_DspDvdDisable(void);
extern void vHDMIHandleAudFmtChange(void);
extern void bHDMIClockDetect(void);
extern void bHDMIAudioSigmaDeltaSetting(UINT32,UINT8,UINT8);
extern void _MT5365_DacChlSel(UINT8 u1DacId,AUD_CHL_DEF_T eChl);
extern void _MT5365_ADCDOWN(BOOL fgEnalbe);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
UINT32 AUD_DspGetPc(void);
UINT32 AUD_DspProbePause(void);
UINT32 AUD_DspProbeGo(void);
UINT32 AUD_DspProbeStep(UINT32 u4Count);
UINT32 fgAUD_DspProbeHalt(void);
UINT32 u4AUD_DspProbeRead(UINT32 u4Addr);
UINT32 AUD_DspProbeWrite(UINT32 u4Addr, UINT32 u4Data);
UINT32 u4AUD_DspProbePBCfg(BOOL fgSet,UINT32 u4Value);
UINT32 u4AUD_DspProbePB(BOOL fgSet,UINT32 u4BPNum, UINT32 u4Addr);
UINT32 u4AUD_DspProbeStopOnWrite(UINT32 u4Addr, UINT32 u4Data, UINT32 u4Mask);
UINT32 u4AUD_DspProbeChkStopOnWrite(BOOL *fgEnable, UINT32 *u4Data, UINT32 *u4Mask);
static void AUD_LineinLRDetect(BOOL fgEnable);
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
void AudAoutGainHwInit(void);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
UINT32 AUD_GetLineinWP(void);
void AUD_DrvSetFgHDMIParserthread(UINT8 u1DecId,BOOL fgHDMIParserThread);
extern UINT32 AUD_DrvGetCircularAddress(UINT32 u4StartAddress,UINT32 u4EndAddress,UINT32 u4Address);
#endif

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static AOUT_CFG_T    _arAudOutCfg[2];
static AIN_CFG_T     _rLineInCfg;
static AIN_CFG_T     _rHDMIInCfg;
static AIN_CFG_T     _rSPDIFInCfg;
static HANDLE_T     _hSmpChgSema;

//static AIN_CFG_T     _rHwI2SInCfg;
//static AUD_DEC_STREAM_FROM_T    _eHwI2SInSel = AUD_STREAM_FROM_LINE_IN;

static UINT8         _au1MckDrvCur[2] = {2, 2};
static UINT8         _au1BckDrvCur[2] = {2, 2};
static UINT8         _au1LRckDrvCur[2] = {2, 2};
static UINT8         _au1AOSDATADrvCur[2] = {2, 2};

static BOOL  _bSPDIFBypass = FALSE;
static BOOL _fgHwInit = FALSE;
static AUD_DEC_STREAM_FROM_T _aeStrSrcConf[AUD_DEC_MAX] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER,
                                                           AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER};
static BOOL _fgChangeSource[AUD_DEC_MAX]={TRUE,TRUE,TRUE,TRUE};
static SAMPLE_FREQ_T _aeSampleRate[AUD_DEC_MAX] = {FS_48K, FS_48K, FS_48K, FS_48K};
static AUD_INPUT_ID_T _eInputIdToDvdGpioConfig;
static BOOL _fgDualDecMode = FALSE;

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------
#ifdef CC_AUD_EU_INTERNALDAC_SAME_APLL
AUD_AOUT_PAD_CFG_T _grAOutPadCfg = {AUD_CH_AUX_FRONT_LEFT, AUD_CH_REAR_LEFT,
                                    AUD_CH_CENTER, AUD_CH_BYPASS_LEFT,
                                    AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT};

#else
AUD_AOUT_PAD_CFG_T _grAOutPadCfg = {AUD_CH_FRONT_LEFT, AUD_CH_REAR_LEFT,
                                    AUD_CH_CENTER, AUD_CH_BYPASS_LEFT,
                                    AUD_CH_FRONT_LEFT, AUD_CH_AUX_FRONT_LEFT};

#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _AudSetBI2SPinmux
 *  Set BI2S pinmux
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AudSetBI2SPinmux(void)
{
    BOOL fgBI2SEnable = FALSE;

    // Check if BI2S output from GPIO 10~13
    fgBI2SEnable = (BOOL)DRVCUST_OptGet(eAudioBI2SEnable);

    if (fgBI2SEnable)
    {
      #if defined(CC_MT5396) 
        #if 1 //Use A I2S as B I2S
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOMCLK); //function 1-->A I2S function 2-->B I2S
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOLRCK);        
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOBCK);        
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOSDATA0);
        #else //Use GPIO2~5 as B        
        vIO32WriteFldAlign(CKGEN_PMUX2, 3, FLD_PAD_GPIO2); //MCLK
        vIO32WriteFldAlign(CKGEN_PMUX2, 3, FLD_PAD_GPIO3); //LRCK
        vIO32WriteFldAlign(CKGEN_PMUX2, 3, FLD_PAD_GPIO4); //BCK
        vIO32WriteFldAlign(CKGEN_PMUX2, 3, FLD_PAD_GPIO5); //DATA
        #endif
      #elif defined(CC_MT5368)
        #if 1 //Use AOSDATA1~4 as BOMCLK/BOLRCK/BOBCK/BOSDATA0
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOSDATA1);//BOMCLK
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOSDATA2);//BOLRCK
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOSDATA3);//BOBCK
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOSDATA4);//BOSDATA0
        #else //USE GPIO0~3 as B
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_GPIO0);//BOMCLK
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_GPIO1);//BOLRCK
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_GPIO2);//BOBCK
        vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_GPIO3_1);//BOSDATA0 [31] = 0
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_GPIO3_2);//BOSDATA0 [1:0] = 01   => 010 = 2
        #endif
      #elif defined(CC_MT5389)
        //Use GPIO0/1/2/4 as BOMCLK/BOLRCK/BOBCK/BOSDATA0 (Function 1)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_GPIO0);//BOMCLK
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_GPIO1);//BOLRCK
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_GPIO2);//BOBCK
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_GPIO4);//BOSDATA0
      #elif defined(CC_MT5395)
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOMCLK); //function 1-->A I2S function 2-->B I2S
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOSDATA0);
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOLRCK);
        vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_AOBCK);
      #elif defined(CC_MT5365)
        vIO32WriteFldAlign(CKGEN_PMUX3, 6, FLD_PAD_BOMCLK);
        vIO32WriteFldAlign(CKGEN_PMUX3, 6, FLD_PAD_BOBCLK);
        vIO32WriteFldAlign(CKGEN_PMUX1, 6, FLD_PAD_BOLRCK);
        vIO32WriteFldAlign(CKGEN_PMUX3, 6, FLD_PAD_BOSDATA);
        vIO32WriteFldAlign(CKGEN_PMUX_MISC, 0, FLD_GPIO13_PSEL);
      #endif
    }
}

static void _AudPinmuxInit(void)
{
    CRIT_STATE_T rCritState;
    AUD_DIGITAL_DATA_T* prDigitalDataCfg;    

    // Enter critical section
    rCritState = x_crit_start();

    _AudSetBI2SPinmux();

  #if defined(CC_MT5396)
    // SPDIF select
    vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);
    vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF1);
    #ifdef SPDIF_MUTE_BY_PINMUX
    //Initially, set SPDIF0/SPDIF1 as SPDIF function, and unmute spdif
    AUD_CLR_BIT(REG_SPOCFG0, SPOUT_MUTE);
    #endif
  #elif defined(CC_MT5368)
    // SPDIF select
    vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);
    vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_ASPDIF1);
    #ifdef SPDIF_MUTE_BY_PINMUX
    //Initially, set SPDIF0/SPDIF1 as SPDIF function, and unmute spdif
    AUD_CLR_BIT(REG_SPOCFG0, SPOUT_MUTE);
    #endif
  #elif defined(CC_MT5389)
    // SPDIF select
    #ifdef CC_HDMI_ARC_SPDIF_CTRL_89
    vIO32WriteFldAlign(PDWNC_PINMUX0, 2, FLD_PAD1_ASPDIF0);
	#else
    vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);
	#endif
    #ifdef SPDIF_MUTE_BY_PINMUX
    //Initially, set SPDIF0/SPDIF1 as SPDIF function, and unmute spdif
    AUD_CLR_BIT(REG_SPOCFG0, SPOUT_MUTE);
    #endif  
  #elif defined(CC_MT5395)
    // SPDIF select
    vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF);    
  #elif defined(CC_MT5365)
    // SPDIF select
    vIO32WriteFldAlign(CKGEN_PMUX1, 1, FLD_PAD_ASPDIF);

    // PWM DAC output AL0/AR0/AL1/AR1/AL2/AR2/AL3/AR3
    vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AL0AR0);
    vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AL1AR1);
    vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AL2AR2);
    vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL3AR3);
  #endif

  
    _AUD_SpdifInBypass(PAD_RF_AGC);

    // Digital I2S output MCLK/LRCK/BCLK/DATA0/DATA1/DATA2/DATA3/DATA4/DATA5
    UNUSED(DRVCUST_OptQuery(eAudioDigitalDataConfig, (UINT32 *)(void *)&prDigitalDataCfg));

  
    if(rAudFlashAQ.u4AmpSel==0)
    {
      prDigitalDataCfg->fgCLK = FALSE;
    }
    
    if (prDigitalDataCfg->fgAOSDATA0)
    {    
        #if defined(CC_MT5396) 
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA0);
        #elif defined(CC_MT5368)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA0);
        #elif defined(CC_MT5389)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA0_B0);
        vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AOSDATA0_B12);
        #elif defined(CC_MT5395)        
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA0);
        #elif defined(CC_MT5365)
        vIO32WriteFldAlign(CKGEN_PMUX1, 1, FLD_PAD_AOSDATA0);
        #endif
    }
    
    if (prDigitalDataCfg->fgAOSDATA1)
    {
        #if defined(CC_MT5396)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA1);
        #elif defined(CC_MT5368)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA1);    
        #elif defined(CC_MT5389)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA1);
        #elif defined(CC_MT5395)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA1);
        #elif defined(CC_MT5365)
    	vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AOSDATA1);
        #endif
    }
    
    if (prDigitalDataCfg->fgAOSDATA2)
    {
        #if defined(CC_MT5396)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA2);
        #elif defined(CC_MT5368)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA2);        
        #elif defined(CC_MT5389)
        //No AOSDATA2
        #elif defined(CC_MT5395)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA2);
        #elif defined(CC_MT5365)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA2);
        #endif
    }
    
    if (prDigitalDataCfg->fgAOSDATA3)
    {
        #if defined(CC_MT5396)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA3);
        #elif defined(CC_MT5368)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA3);    
        #elif defined(CC_MT5389)
        //No AOSDATA3      
        #elif defined(CC_MT5395)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA3);
        #elif defined(CC_MT5365)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA3);
        #endif
    }
    
    if (prDigitalDataCfg->fgAOSDATA4)
    {
        #if defined(CC_MT5396)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA4);
        #elif defined(CC_MT5368)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA4);
        #elif defined(CC_MT5389)
        //No AOSDATA4
        #elif defined(CC_MT5395)
    	vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOSDATA4);
        #elif defined(CC_MT5365)
    	vIO32WriteFldAlign(CKGEN_PMUX1, 1, FLD_PAD_AOSDATA4);
        #endif
    }
    
    if (prDigitalDataCfg->fgCLK)
    {
        #if defined(CC_MT5396)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOMCLK); //MCLK/LRCK/BCK bind to the same control bits        
        #elif defined(CC_MT5368)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOMCLK);
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOLRCK);
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOBCK);
        #elif defined(CC_MT5389)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOMCLK);
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOLRCK_B0);
        vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AOLRCK_B12);
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOBCK_B0);
        vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_AOBCK_B12);
        #elif defined(CC_MT5395)
        vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_AOMCLK);
        #elif defined(CC_MT5365)
        vIO32WriteFldAlign(CKGEN_PMUX1, 1, FLD_PAD_AOMCLK);
        #endif
    }

    // End of critical section 
    x_crit_end(rCritState);
}

/*
static UINT8 _AudStrSrcToAudInId(AUD_DEC_STREAM_FROM_T eStrSrc)
{
    UINT8 u1Ret;
    
    switch (eStrSrc)
    {
    case AUD_STREAM_FROM_LINE_IN:
        u1Ret = 0;
        break;
    case AUD_STREAM_FROM_HDMI:
        u1Ret = 1;
        break;
    case AUD_STREAM_FROM_SPDIF:
        u1Ret = 2;
        break;
    default:
        LOG(3, "ERROR _AudStrSrcToAudInId\n");
        break;
    }   

    return u1Ret;
}
*/

static void _AudInOutCfgInit(void)
{
    UINT8 u1DecId;
    AOUT_CFG_T *prAudOutFmt;
    AOUT_CFG_T *prAudOutFmt2;
    AOUT_CFG_T *prAudInFmt;

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32 *)(void *) &prAudOutFmt));
    _arAudOutCfg[AUD_DEC_MAIN].eFormat = prAudOutFmt->eFormat;
    _arAudOutCfg[AUD_DEC_MAIN].eSampleFreq = prAudOutFmt->eSampleFreq;
    _arAudOutCfg[AUD_DEC_MAIN].eBits = prAudOutFmt->eBits;
    _arAudOutCfg[AUD_DEC_MAIN].eCycle = prAudOutFmt->eCycle;
    _arAudOutCfg[AUD_DEC_MAIN].fgDataInvert = prAudOutFmt->fgDataInvert;
    _arAudOutCfg[AUD_DEC_MAIN].fgLRInvert = prAudOutFmt->fgLRInvert;

    UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable2, (UINT32 *)(void *) &prAudOutFmt2));

    _arAudOutCfg[AUD_DEC_AUX].eFormat = prAudOutFmt2->eFormat;
    _arAudOutCfg[AUD_DEC_AUX].eSampleFreq = prAudOutFmt2->eSampleFreq;
    _arAudOutCfg[AUD_DEC_AUX].eBits = prAudOutFmt2->eBits;
    _arAudOutCfg[AUD_DEC_AUX].eCycle = prAudOutFmt2->eCycle;
    _arAudOutCfg[AUD_DEC_AUX].fgDataInvert = prAudOutFmt2->fgDataInvert;
    _arAudOutCfg[AUD_DEC_AUX].fgLRInvert = prAudOutFmt2->fgLRInvert;     
    
    UNUSED(DRVCUST_OptQuery(eAudioInFmtTable, (UINT32 *)(void *) &prAudInFmt));

    // Audio output format init
    for (u1DecId = AUD_DEC_MAIN; u1DecId < 2 ; u1DecId++)
    {       
        _au1MckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1BckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1LRckDrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
        _au1AOSDATADrvCur[u1DecId] = DRVCUST_OptGet(eAudioOutPadDrvCur);
    }

    // Line-in format init
    _rLineInCfg.eStrSrc       = AUD_STREAM_FROM_LINE_IN;
    _rLineInCfg.eFormat       = prAudInFmt->eFormat;
    _rLineInCfg.eBits         = prAudInFmt->eBits;
    _rLineInCfg.eCycle        = prAudInFmt->eCycle;
    _rLineInCfg.eSampleFreq   = prAudInFmt->eSampleFreq;
    _rLineInCfg.fgDataInvert  = prAudInFmt->fgDataInvert;
    _rLineInCfg.fgLRInvert    = prAudInFmt->fgLRInvert;

    // Spdif-in format init
    _rSPDIFInCfg.eStrSrc      = AUD_STREAM_FROM_SPDIF;
    _rSPDIFInCfg.eFormat      = FORMAT_RJ;
    _rSPDIFInCfg.eBits        = DAC_24_BIT;
    _rSPDIFInCfg.eCycle       = LRCK_CYC_32;
    _rSPDIFInCfg.eSampleFreq  = MCLK_256FS;
    _rSPDIFInCfg.fgDataInvert = FALSE;
    _rSPDIFInCfg.fgLRInvert   = FALSE;

    // HDMI-in format init
    _rHDMIInCfg.eStrSrc       = AUD_STREAM_FROM_HDMI;
    _rHDMIInCfg.eFormat       = FORMAT_I2S;
    _rHDMIInCfg.eBits         = DAC_24_BIT;
    _rHDMIInCfg.eCycle        = LRCK_CYC_32;
    _rHDMIInCfg.eSampleFreq   = MCLK_256FS;
    _rHDMIInCfg.fgDataInvert  = FALSE;
    _rHDMIInCfg.fgLRInvert    = FALSE;
}

// This function is used to select clock source
// a. apll  b. hdmi  c. spdif (external pin)
static void _AudClkSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt)
{
    static AUD_DEC_STREAM_FROM_T ePreviouStream = AUD_STREAM_FROM_OTHERS;

    AUD_DEC_ID_VALIDATE(u1DecId);

    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) || (ePreviouStream == AUD_STREAM_FROM_HDMI) ||
        (eStreamFrom == AUD_STREAM_FROM_SPDIF) || (ePreviouStream == AUD_STREAM_FROM_SPDIF))
    {
        bHDMIClockDetect();
    }
    
    // Main decoder set to apll0
    AUD_CLR_BIT(REG_ACK_CFG, CLK_SEL_APLL1);
    // Aux decoder set to apll0
    AUD_CLR_BIT(REG_ACK_CFG, DEC2_CLK_SEL_APLL1);

    // Line in from ACK_CFG
    AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC0);        

    AudPllSrcSel(u1DecId, eStreamFrom); 

    if (_AUD_IsIECRaw() == TRUE)
    {
        // Select IEC refer to apll1
        AUD_CLR_BIT(REG_ACK2_CFG, IEC_AUD3_CLK_SEL);
        AUD_WRITE32(REG_IEC_DIV, IEC_AUDCK_SEL | IEC_CLK_INDEPENDENT_SEL | IEC_LRCK_CYCLE  | IEC_UPDATE_SEL_2T); // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
    }
    else
    {
        // Select IEC refer to apll0
        AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) & (~IEC_AUDCK_SEL)) & (~IEC_CLK_INDEPENDENT_SEL));        
    }
    
    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_SPDIF:
        {
            // Disable AUD3
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
            AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            // Line in from HDMI/SPDIF
            AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LIN_CLK_SEL_MASK)) | AOUT_DEC2);
            //  TODO: Need to set tracking
            break;
        }        
        case AUD_STREAM_FROM_HDMI:
        {
            // Disable AUD3
           AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
           AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
           AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
            // Slave mode
            AUD_SET_BIT(REG_ACK_CFG, LIN_MS);
            //  TODO: Need to set tracking
            break;
        }
        case AUD_STREAM_FROM_LINE_IN:
        {
             // Select decoder refer to AUD3
             #ifdef CC_FPGA
             AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
             AUD_CLR_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
             AUD_CLR_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);
             #else
            AUD_SET_BIT(REG_ACK2_CFG, SDADC_AUD3_CLK_SEL);
            AUD_SET_BIT(REG_ACK2_CFG, SDADC_AUD3_LRCK_SEL);
            AUD_SET_BIT(REG_ACK2_CFG, LIN_CLK_AUD3_SEL);             
            #endif
			// Master mode
			AUD_CLR_BIT(REG_ACK_CFG, LIN_MS);  
            break;
        }
        default:
            break;
    }
    
    if ((eStreamFrom == AUD_STREAM_FROM_HDMI) || (ePreviouStream == AUD_STREAM_FROM_HDMI) ||
        (eStreamFrom == AUD_STREAM_FROM_SPDIF) || (ePreviouStream == AUD_STREAM_FROM_SPDIF))
    {
        bHDMIClockDetect();
    }
    
    ePreviouStream = eStreamFrom;
}

static void _AUD_SetCustSpecGpioEnable(BOOL fgEnable)
{
    UINT32 u4CustSpecGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustSpecGpio = DRVCUST_OptGet(eAudioCustSpecGpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSpecGpioPolarity);

    if (u4CustSpecGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
    	{
            i4RegGpioSwitch= fgEnable ? 1 : 0;
    	}
    	else
    	{
            i4RegGpioSwitch= fgEnable ? 0 : 1;
    	}
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4CustSpecGpio, i4RegGpioSwitch);
    }
    else
    {
        LOG(8, "Not support cust specGPIO\n");
    }
}

static void _AUD_SetCustSpec2GpioEnable(BOOL fgEnable)
{
    UINT32 u4CustSpecGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustSpecGpio = DRVCUST_OptGet(eAudioCustSpec2GpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioSpec2GpioPolarity);

    if (u4CustSpecGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
    	{
            i4RegGpioSwitch= fgEnable ? 1 : 0;
    	}
    	else
    	{
            i4RegGpioSwitch= fgEnable ? 0 : 1;
    	}
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4CustSpecGpio, i4RegGpioSwitch);
    }
    else
    {
        LOG(8, "Not support cust specGPIO\n");
    }
}


static void _AUD_SetCustHpGpioEnable(BOOL fgEnable)
{
    UINT32 u4CustHpGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4CustHpGpio = DRVCUST_OptGet(eAudioCustHpGpioCtrl);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioHpGpioPolarity);

    if (u4CustHpGpio != 0xffffffff)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
    	{
            i4RegGpioSwitch= fgEnable ? 1 : 0;
    	}
    	else
    	{
            i4RegGpioSwitch= fgEnable ? 0 : 1;
    	}
        LOG(8, "Aud Cust Spec Gpio Switch : %d\n", i4RegGpioSwitch);
        AUD_GPIOOutput(u4CustHpGpio, i4RegGpioSwitch);
    }
    else
    {
        LOG(8, "Not support cust HP GPIO\n");
    }
}

//-----------------------------------------------------------------------------
/** _AUD_ResetAmp
 *  Reset Amp
 *
 *  @param  fgEnable   TRUE (shut down), FALSE (not shut down)
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _AUD_ResetAmp(BOOL fgEnable)
{
    UINT32 u4AmpSdGpio;
    INT32 i4RegGpioSwitch;
    AUD_GPIO_POLARITY_T eGpioPolariy;

    u4AmpSdGpio = DRVCUST_OptGet(eAudioAmpResetGpioNum);
    eGpioPolariy =  (AUD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioAmpResetGpioPolarity);

    if (u4AmpSdGpio != 0xffffffffU)
    {
        if (eGpioPolariy == AUD_GPIO_HIGH_ENALBE)
    	{
            i4RegGpioSwitch= fgEnable ? 1 : 0;
    	}
    	else
    	{
            i4RegGpioSwitch= fgEnable ? 0 : 1;
    	}
        LOG(8, "Aud Amp Reset Gpio Switch : %d\n", i4RegGpioSwitch);
        //AUD_GPIOOutput(u4AmpSdGpio, i4RegGpioSwitch);
    }
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _AUD_DMX_SetAudFifo
 *
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  u4FifoStr   
 *  @param  u4FifoEnd   
 *
 *  @retval TRUE
 *  @retval FALSE
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DMX_SetAudFifo(UINT8 u1DecId, UINT32 u4FifoStr, UINT32 u4FifoEnd)
{

    AUD_DEC_ID_VALIDATE(u1DecId);
    
/*
    // Check if FIFO start/end addresses are valid aligned
    if (!IsAligned(u4FifoStr, DMX_FIFO_ALIGNMENT) ||
        !IsAligned(u4FifoEnd, DMX_FIFO_ALIGNMENT))
    {
        return FALSE;
    }
*/

    // In 3 decoder flow, dec1 use DMX buffer, dec0/3 use parser buffer for Getbits.
    // In 1 decoder flow, dec0 use DMX buffer, others use parser buffer
#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId==AUD_DEC_MAIN)
#else
    if (u1DecId==AUD_DEC_AUX)
#endif
    {
        // Fill in the registers in Unit of 4 bytes
        AUD_WRITE32(REG_DMX_STR, DSP_INTERNAL_ADDR(u4FifoStr >> 2));
        AUD_WRITE32(REG_DMX_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        #ifdef CC_AUD_DEC3_USE_FIFO1// USE_FIFO3 : dec3 use fifo3
        PARSER_WRITE32(REG_A0STA, DSP_INTERNAL_ADDR(u4FifoStr >> 8));
        PARSER_WRITE32(REG_A0END, DSP_INTERNAL_ADDR(u4FifoEnd >> 8));        
        #else
        AUD_WRITE32(REG_USER0_ST, DSP_INTERNAL_ADDR(u4FifoStr >> 2));
        AUD_WRITE32(REG_USER0_END, DSP_INTERNAL_ADDR(u4FifoEnd >> 2));
        #endif
    }
    else
    {   
        PARSER_WRITE32(REG_A0STA, DSP_INTERNAL_ADDR(u4FifoStr >> 8));
        PARSER_WRITE32(REG_A0END, DSP_INTERNAL_ADDR(u4FifoEnd >> 8));
    } 
    
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_GetAudFifo
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DMX_GetAudFifo(UINT8 u1DecId, UINT32 *pu4FifoStr, UINT32 *pu4FifoEnd)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    // Registers in Unit of 4 bytes
    if ((pu4FifoStr != NULL) && (pu4FifoEnd != NULL))
    {
        *pu4FifoStr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_STR) << 2);
        *pu4FifoEnd = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_END) << 2);
    }    
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_GetAudPtr
 *    Return the physical DRAM address
 *  @param  u1DecId     Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  pu4ReadPtr   
 *  @param  pu4WritePtr   
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_DMX_GetAudPtr(UINT8 u1DecId, UINT32 *pu4ReadPtr, UINT32 *pu4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
   
    // *pu4ReadPtr = AUD_READ32(REG_DMX_GTB_PNT);
    if ((pu4ReadPtr!= NULL) && (pu4WritePtr != NULL))
    {
        *pu4ReadPtr = DSP_GetDspReadPtrPhysicalAddr(u1DecId);

        // In 3 decoder flow, dec1 use DMX buffer, dec0/3 use parser buffer for Getbits.
        // In 1 decoder flow, dec0 use DMX buffer, others use parser buffer
#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_AUX) 
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());         
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT)); 
    	}
#else
#ifdef CC_AUD_DEC3_USE_FIFO1 //ndef CC_AUD_SKYPE_SUPPORT  
        if (u1DecId != AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());         
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT)); 
    	}        
#else
        if (u1DecId == AUD_DEC_MAIN) 
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());    
        }
        else if (u1DecId == AUD_DEC_AUX) 
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
        else if(u1DecId == AUD_DEC_THIRD)
        {        
            
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER0_WP));
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER1_WP));
        }
#endif
#endif        
    }    
}

void _AUD_DMX_GetAudWrtPtr(UINT8 u1DecId, UINT32 *pu4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);   
    
    // *pu4ReadPtr = AUD_READ32(REG_DMX_GTB_PNT);
    if ((pu4WritePtr != NULL))
    {
        // In 3 decoder flow, dec1 use DMX buffer, dec0/3 use parser buffer for Getbits.
        // In 1 decoder flow, dec0 use DMX buffer, others use parser buffer    
#ifndef CC_MT5391_AUD_3_DECODER
        if (u1DecId == AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());         
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT)); 
    	}
#else
#ifndef CC_AUD_SKYPE_SUPPORT  
        if (u1DecId != AUD_DEC_AUX)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());         
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT)); 
    	}        
#else
        if (u1DecId == AUD_DEC_MAIN) 
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(PSR_SoftGetAudioWp());
        }
        else if (u1DecId == AUD_DEC_AUX) 
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_DMX_WRITE_PNT));
        }
        else if(u1DecId == AUD_DEC_THIRD)
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER0_WP));
        }
        else
        {
            *pu4WritePtr = DSP_PHYSICAL_ADDR(AUD_READ32(REG_USER1_WP));
        }
#endif
#endif
    }    
}
//-----------------------------------------------------------------------------
/** _AUD_DMX_UpdateWritePtr
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DMX_UpdateWritePtr(UINT8 u1DecId, UINT32 u4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    // Check if write pointer address is within FIFO range
    if ((DSP_INTERNAL_ADDR(u4WritePtr) >= DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId))) || 
        (DSP_INTERNAL_ADDR(u4WritePtr) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId))))
    {
        Printf("FIFO (%x, %x), Wp = %x\n",DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)),  DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId)), \
           DSP_INTERNAL_ADDR(u4WritePtr) );

        return FALSE;
    }

#ifndef CC_MT5391_AUD_3_DECODER
    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));	//laihui:REG_DMX_WRITE_PNT
    }
    else //if (u1DecId == AUD_DEC_AUX)
    {
        _AUD_BitStrPtrSel(AUD_DEC_AUX, AUD_STREAM_FROM_MEMORY);  //Important!!
        PSR_RiscSetAudioWp(u1DecId, DSP_INTERNAL_ADDR(u4WritePtr)); 
    }
#else
    if (u1DecId == AUD_DEC_AUX)
    {
        AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));	//laihui:REG_DMX_WRITE_PNT
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        #ifdef CC_AUD_DEC3_USE_FIFO1 // USE_FIFO3 : dec3 use fifo3
        PSR_RiscSetAudioWp(u1DecId, DSP_INTERNAL_ADDR(u4WritePtr));
        #else
        AUD_WRITE32(REG_USER0_WP, DSP_INTERNAL_ADDR(u4WritePtr));
        #endif
    }        
    else
    {    
        //_AUD_BitStrPtrSel(u1DecId, AUD_STREAM_FROM_DIGITAL_TUNER);  //Important!!
        PSR_RiscSetAudioWp(u1DecId, DSP_INTERNAL_ADDR(u4WritePtr));
    }
#endif    
    
    return TRUE;
}

BOOL _AUD_DEC2_UpdateWritePtr(UINT8 u1DecId, UINT32 u4WritePtr)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    // Check if write pointer address is within FIFO range
    if ((DSP_INTERNAL_ADDR(u4WritePtr) >= DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId))) || 
        (DSP_INTERNAL_ADDR(u4WritePtr) < DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId))))
    {
        Printf("FIFO (%x, %x), Wp = %x\n",DSP_INTERNAL_ADDR(u4WritePtr),  DSP_INTERNAL_ADDR(u4GetAFIFOStart(u1DecId)), \
            DSP_INTERNAL_ADDR(u4GetAFIFOEnd(u1DecId)));

        return FALSE;
    }    
    AUD_WRITE32(REG_DMX_WRITE_PNT, DSP_INTERNAL_ADDR(u4WritePtr));
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _AUD_DMX_UpdateWPtr
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID _AUD_DMX_UpdateWPtr(UINT8 u1DecId, UINT32 u4Wp)
{
    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4Wp));
}

//-----------------------------------------------------------------------------
/** _AUD_BitStrPtrSel
 *
 *  @param  u1DecId       Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eStreamFrom   
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _AUD_BitStrPtrSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)    
{
    UINT32 u4Format = 0;
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
    AUD_DEC_STREAM_FROM_T eAudDecStreamFrom;
    AUD_FMT_T eAudDecFormat;
#endif
    
    AUD_DEC_ID_VALIDATE(u1DecId); 
    
    if (eStreamFrom != AUD_STREAM_FROM_ANALOG_TUNER) 
    {
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4Format = AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
        }
        else if (u1DecId == AUD_DEC_AUX) // AUX Decoder
        {
            u4Format = AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK1_MASK);
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            #ifdef CC_AUD_DEC3_USE_FIFO1 // USE_FIFO3 : dec3 use fifo3
            u4Format = AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
            #else
            u4Format = AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK2_MASK);
            #endif
        }
        else if (u1DecId == AUD_DEC_4TH)
        {
            u4Format = AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK3_MASK);            
        }
    }

    switch (eStreamFrom)
    {
    //Audio Description decoder1 - use soft parser pointer
    case AUD_STREAM_FROM_DIGITAL_TUNER:
    case AUD_STREAM_FROM_MULTI_MEDIA:
        //AUD_WRITE32(REG_BS_CFG, u4Format | 
	    //	         ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_DMX : PTR_SEL_BANK1_DMX));
#ifndef CC_MT5391_AUD_3_DECODER
        AUD_WRITE32(REG_BS_CFG, u4Format | 
            ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_DMX : PTR_SEL_BANK1_PSR));
#else
        //AUD_WRITE32(REG_BS_CFG, u4Format |
        //		         ((u1DecId == AUD_DEC_THIRD) ? PTR_SEL_BANK1_PSR : PTR_SEL_BANK0_DMX ));

  #ifdef CC_AUD_DEC3_USE_FIFO1 // USE_FIFO3 : dec3 use fifo3
        AUD_WRITE32(REG_BS_CFG, u4Format | 
          ((u1DecId == AUD_DEC_AUX) ? PTR_SEL_BANK1_DMX : PTR_SEL_BANK0_PSR ));
  #else
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4Format = u4Format | PTR_SEL_BANK0_PSR;
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4Format = u4Format | PTR_SEL_BANK1_DMX;            
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            u4Format = u4Format | PTR_SEL_BANK2_DEFINED0;
        }
        AUD_WRITE32(REG_BS_CFG, u4Format);
  #endif // CC_AUD_DEC3_USE_FIFO1        
#endif // CC_MT5391_AUD_3_DECODER
        break;
        	
    case AUD_STREAM_FROM_MEMORY:
    case AUD_STREAM_FROM_OTHERS:
    case AUD_STREAM_FROM_FEEDER:
        AUD_WRITE32(REG_BS_CFG, u4Format | 
            ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_PSR : PTR_SEL_BANK1_PSR));
        break;
        
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
    case AUD_STREAM_FROM_HDMI:
        AUD_DRVGetDecodeType(u1DecId, &eAudDecStreamFrom, &eAudDecFormat);
#ifdef CC_AUD_HDMI_PARSER_3_0_SUPPORT        
        if(1)//if (eAudDecFormat == AUD_FMT_AC3)
#else
        if (eAudDecFormat != AUD_FMT_PCM)
#endif 
        {
            AUD_WRITE32(REG_BS_CFG, u4Format | 
                         ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_PSR : PTR_SEL_BANK1_PSR));
        }
        else
        {
            AUD_WRITE32(REG_BS_CFG, u4Format | 
                         ((u1DecId == AUD_DEC_MAIN) ? PTR_SEL_BANK0_SPDIF_LINE_IN : PTR_SEL_BANK1_SPDIF_LINE_IN));
        }
        break;
#endif	        

    case AUD_STREAM_FROM_SPDIF:
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
    case AUD_STREAM_FROM_HDMI:
#endif
    case AUD_STREAM_FROM_LINE_IN:
#ifdef CC_AUD_SKYPE_SUPPORT
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4Format |= PTR_SEL_BANK0_SPDIF_LINE_IN;
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4Format |= PTR_SEL_BANK1_DEC2_SPDIF_LINE_IN;
        }
        else if (u1DecId == AUD_DEC_THIRD) // THIRD Decoder, on-line sound/phone-ring
        {
            u4Format |= PTR_SEL_BANK2_SPDIF_LINE_IN;
        }
        else if (u1DecId == AUD_DEC_4TH)
        {
            u4Format |= PTR_SEL_BANK3_SPDIF_LINE_IN;
        }
        AUD_WRITE32(REG_BS_CFG, u4Format);
#else
        #ifdef DSP_TEST_LINE2
        AUD_WRITE32(REG_BS_CFG, u4Format |
            ((u1DecId == AUD_DEC_MAIN) ?
            PTR_SEL_BANK0_DEC2_SPDIF_LINE_IN : PTR_SEL_BANK1_DEC2_SPDIF_LINE_IN));
        #else
        if (u1DecId == AUD_DEC_MAIN)
        {
            u4Format |= PTR_SEL_BANK0_SPDIF_LINE_IN;
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4Format |= PTR_SEL_BANK1_SPDIF_LINE_IN;
        }
        else if (u1DecId == AUD_DEC_THIRD) // THIRD Decoder, on-line sound/phone-ring
        {
            u4Format |= PTR_SEL_BANK2_SPDIF_LINE_IN;
        }
        else if (u1DecId == AUD_DEC_4TH)
        {
            u4Format |= PTR_SEL_BANK3_SPDIF_LINE_IN;
        }
        AUD_WRITE32(REG_BS_CFG, u4Format);
        #endif
#endif            
        break;
			
    default:
        break; 
    }
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** AUD_HwInit
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_HwInit(void)
{
    UINT8 u1DecId;
    AUD_AOUT_PAD_CFG_T* prAOutPadCfg;

    if (!_fgHwInit)
    {
        VERIFY(x_sema_create(&_hSmpChgSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgHwInit = TRUE;
    }    

    ADAC_Mute(TRUE); 
    ADAC_SpeakerPopEnable(FALSE);
    
    //20100504 [DTV00080261]
    #if 1 //(defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5395))
    ADAC_GpioMute(TRUE);
    #else
    ADAC_GpioCodecMute(TRUE);
    #endif
    //ADAC_GpioAmpEnable(FALSE);

	#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
	//AudAoutGainHwInit();
	#endif

    // Load audio in/out default setting
    // Must be called before _AudPllInit()
    _AudInOutCfgInit();    

    // Setup apll, default set sampling rate = 48 KHz
    if (!BSP_IsFPGA())
    {
        AudPllInit();
    }

    // Sync from [DTV00094645] (IEC_UPDATE_SEL fine tune)
    AUD_CLR_BIT(REG_IEC_DIV,(1<<6)); 
    AUD_WRITE32(REG_IEC_DIV, (AUD_READ32(REG_IEC_DIV) | IEC_UPDATE_SEL_2T));

    // Select audio output pinmux
    _AudPinmuxInit();

    UNUSED(DRVCUST_OptQuery(eAudioAoutPadConfig, (UINT32 *)(void *)&prAOutPadCfg));
    if (prAOutPadCfg != NULL)
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&_grAOutPadCfg), prAOutPadCfg, sizeof(AUD_AOUT_PAD_CFG_T));
    }
     
    AUD_AoutPADMux(_grAOutPadCfg.ePadSdata0, _grAOutPadCfg.ePadSdata1,
                   _grAOutPadCfg.ePadSdata2, _grAOutPadCfg.ePadSdata3,
                   _grAOutPadCfg.ePadSdata4, _grAOutPadCfg.ePadSdata5,
                   MCLK0);
      
    // Line in config
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    AUD_WRITE32(REG_LIN_CFG0, (DATA_PCM_24_BITS|INT_ADC_INTLIN|RAW_LONG_DETECT));
#else
    AUD_WRITE32(REG_LIN_CFG0, (DATA_PCM_24_BITS|INT_ADC_INTLIN));
#endif
    
    VERIFY(BIM_DisableIrq(VECTOR_SPDIF)); 

    // Setup audio output format
    for (u1DecId=AUD_DEC_MAIN; u1DecId<AUD_AOUT_NO; u1DecId++)
    {   
        // Audio output format config
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
       
        // Setup audio output(MCLK, BCLK , LRCK, AOSDATA) driving current
        AUD_AoutDrvCur(u1DecId, _au1MckDrvCur[u1DecId], _au1BckDrvCur[u1DecId] , _au1LRckDrvCur[u1DecId] , _au1AOSDATADrvCur[u1DecId]);
    }

    // Set AUD3 BCK_DIV=4
    AUD_WRITE32(REG_ACK2_CFG,
        ((AUD_READ32(REG_ACK2_CFG) & ~(AUD3_ACK_BCK_DIV_MASK)) | AUD3_ACK_BCK_DIV_4));

    // Turn off audio output 128 bit mode
    AUD_CLR_BIT(REG_AOUT_DRAM_CFG, DRAM_ACCESS_128_BITS);

    // Open MCLK ,
    AUD_OutPadEnable(AUD_DEC_MAIN, TRUE);
    AUD_OutPadEnable(AUD_DEC_AUX, TRUE);

    // Audio DAC init
    ADAC_Init();
    
    //SPDIF enable
    //UNUSED(AUD_DspIECConfig(AUD_DEC_MAIN,AUD_IEC_CFG_PCM,1));
    _AUD_SetCustSpecGpioEnable(TRUE);
    _AUD_SetCustSpec2GpioEnable(TRUE);
    _AUD_SetCustHpGpioEnable(TRUE);

    _AUD_ResetAmp(FALSE);

    // MT5365_Check
  	//AUD_WRITE32(REG_DSPROM_CFG, DSP_ROM_CFG);  
    //AUD_WRITE32(REG_AINROM_CFG, AIN_ROM_CFG);

    ///ADAC_GpioCodecMute(FALSE);  // For PWM first power on pop noise, 5388 let other module do GPIO unmute. MW: setoutport, DEMO : setSampFreq.

    // Laihui: Get bit hold issue
    AUD_SET_BIT(REG_BS_CFG, CLR_BS);

    _MT5365_ADCDOWN(TRUE); //sunman power down Line in AD
    
    
}

#ifdef CC_FAST_INIT
void AUD_Spdif_Power_Down(void)
{
    /// SPDIF Disable
    _AUD_SetCustSpecGpioEnable(FALSE);
    _AUD_SetCustSpec2GpioEnable(FALSE);
    _AUD_SetCustHpGpioEnable(FALSE);
}
#endif

void AUD_ClkCfgDualDecMode(BOOL fgEnable)
{
    _fgDualDecMode = fgEnable;
}

void AUD_ClkSetToInternal(UINT8 u1DecId, AUD_FMT_T eFmt)
{
    _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
}

void AUD_StrInputEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt,BOOL fgEnable, UINT32 u4ReceivePesCnt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    // Select audio getbs source
    _AUD_BitStrPtrSel(u1DecId, eStreamFrom);

    // Select clk source form apll ? spdif ? hdmi ?
#ifndef CC_MT5391_AUD_3_DECODER
    if (fgEnable)
#else    
    if ((u1DecId <  AUD_DEC_THIRD) && (fgEnable))
    // gallen, for dec3 and dec4, we should not change any clock setting
#endif    
    {
        _AudClkSrcSel(u1DecId, eStreamFrom, eFmt);
    }    

    //Workaround for 5365 demo program..The AMP wake up 500ms after AMP reset is released.
    //So this command can't be finished in AMP init.

    //Laihui remove this workaround. 0428
    //Add new initial amp flow of AD82581
	
    //ADAC_AmpRegWrite(2,0);
    

    if (fgEnable)
    {
        switch (eStreamFrom)
        {
            case AUD_STREAM_FROM_MEMORY:
            case AUD_STREAM_FROM_FEEDER:
                VERIFY(PSR_SoftInit());
                VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
    
                vDspSetFifoReadPtr(u1DecId, u4GetAFIFOStart(u1DecId));
                if (!fgDspReadPtrSet(u1DecId, u4GetAFIFOStart(u1DecId)))
                {
                    LOG(5, "DspReadPtrSet time out\n");  
                }
                break;
                    
            case AUD_STREAM_FROM_LINE_IN:
                // Setup audio fifo 
                vSetSPDIFInFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId));
#ifndef CC_MT5391_AUD_3_DECODER                
                VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
#else
  #ifndef CC_AUD_SKYPE_SUPPORT  
                VERIFY(PSR_SoftSetAudioFIFO(((u1DecId == AUD_DEC_AUX)? AUD_DEC_AUX : AUD_DEC_MAIN), u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                // The 1st and 3rd decoders both use 1st AFIFO
#endif
#endif
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DecId, eStreamFrom, eFmt);    
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);   
                
#ifndef AUD_HW_LR_INVERSE_DECT
                AUD_LineInCtrlEnable(u1DecId, FALSE);
                AUD_LineInCtrlEnable(u1DecId, TRUE);
#else
                AUD_LineinLRDetect(TRUE);                
#endif //AUD_HW_LR_INVERSE_DECT

                AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER);
                AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);

                _MT5365_ADCDOWN(FALSE); //sunman power down Line in AD
                break;

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT                
     		case AUD_STREAM_FROM_HDMI:
#ifdef CC_AUD_HDMI_PARSER_3_0_SUPPORT                
                if(1)//if(eFmt ==AUD_FMT_AC3 )
#else
                if(eFmt !=AUD_FMT_PCM)
#endif
		        {
                    VERIFY(PSR_SoftInit());
			        vSetSPDIFInFIFO(AUD_DEC_MAIN,u4GetHDMIParserFIFOStart(), u4GetHDMIParserFIFOEnd());
                    VERIFY(PSR_SoftSetAudioFIFO(AUD_DEC_MAIN, u4GetAFIFOStart(AUD_DEC_MAIN), u4GetAFIFOEnd(AUD_DEC_MAIN)));
                    // Turn off input when config format
        			AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
        			// Format config
        			AUD_AinSrcSel(u1DecId, eStreamFrom, eFmt);	  
        			// Turn on input
        			AUD_AinEnable(u1DecId, eStreamFrom, TRUE);	
        			AUD_DrvSetFgHDMIParserthread(u1DecId,fgEnable);
                    // Prevent L/R reverse
#ifndef AUD_HW_LR_INVERSE_DECT
                    x_thread_delay(10);
                    AUD_LineInCtrlEnable(u1DecId, FALSE);
                    AUD_LineInCtrlEnable(u1DecId, TRUE);
#else
                    AUD_LineinLRDetect(TRUE);
#endif //AUD_HW_LR_INVERSE_DECT            
                }
                else
                {
                    // Setup audio fifo 
                    vSetSPDIFInFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId));
#ifndef CC_MT5391_AUD_3_DECODER                
                    VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
#else
                    VERIFY(PSR_SoftSetAudioFIFO(((u1DecId == AUD_DEC_AUX)? AUD_DEC_AUX : AUD_DEC_MAIN), u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                    // The 1st and 3rd decoders both use 1st AFIFO
#endif
                    // Turn off input when config format
                    AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                    // Format config
                    AUD_AinSrcSel(u1DecId, eStreamFrom, eFmt);    
                    // Turn on input
                    AUD_AinEnable(u1DecId, eStreamFrom, TRUE);   
                   
                    // Prevent L/R reverse
#ifndef AUD_HW_LR_INVERSE_DECT
                    x_thread_delay(10);
                    AUD_LineInCtrlEnable(u1DecId, FALSE);
                    AUD_LineInCtrlEnable(u1DecId, TRUE);
#else
                    AUD_LineinLRDetect(TRUE);
#endif //AUD_HW_LR_INVERSE_DECT
                }
                break;
#endif
                
            case AUD_STREAM_FROM_SPDIF:
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
#endif
                // Setup audio fifo 
                vSetSPDIFInFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId));
#ifndef CC_MT5391_AUD_3_DECODER                
                VERIFY(PSR_SoftSetAudioFIFO(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
#else
                VERIFY(PSR_SoftSetAudioFIFO( ((u1DecId == AUD_DEC_AUX)? AUD_DEC_AUX : AUD_DEC_MAIN), u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                // The 1st and 3rd decoders both use 1st AFIFO
#endif
                // Turn off input when config format
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Format config
                AUD_AinSrcSel(u1DecId, eStreamFrom, eFmt);    
                // Turn on input
                AUD_AinEnable(u1DecId, eStreamFrom, TRUE);   
               
                // Prevent L/R reverse
#ifndef AUD_HW_LR_INVERSE_DECT
                x_thread_delay(10);
                AUD_LineInCtrlEnable(u1DecId, FALSE);
                AUD_LineInCtrlEnable(u1DecId, TRUE);
#else
                AUD_LineinLRDetect(TRUE);
#endif //AUD_HW_LR_INVERSE_DECT
                break;
                
            case AUD_STREAM_FROM_DIGITAL_TUNER:
	        case AUD_STREAM_FROM_MULTI_MEDIA:
#ifndef CC_AUD_SKYPE_SUPPORT	            
                PSR_SoftInit(); 
#else                
                if (u1DecId == AUD_DEC_MAIN) PSR_SoftInit();
#endif                 
                VERIFY(_AUD_DMX_SetAudFifo(u1DecId, u4GetAFIFOStart(u1DecId), u4GetAFIFOEnd(u1DecId)));
                if (u4ReceivePesCnt == 0)    //write pointer is not updated in SendAudPes
                {
                    VERIFY(_AUD_DMX_UpdateWritePtr(u1DecId, u4GetAFIFOStart(u1DecId)));
                }
                break;
                
            case AUD_STREAM_FROM_ANALOG_TUNER:
                DSP_SetAIFMux(SIF_PATH);
                break;
                
            case AUD_STREAM_FROM_OTHERS:
                break;  
        
            default:
                break;
        }
    }
    else // Turn off stream input
    {
        switch (eStreamFrom)
        {
            case AUD_STREAM_FROM_MEMORY:
                break;
            case AUD_STREAM_FROM_LINE_IN:
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
               _MT5365_ADCDOWN(TRUE); //sunman power down Line in AD
#ifdef AUD_HW_LR_INVERSE_DECT                
                AUD_LineinLRDetect(FALSE);
#endif                
                break;
            case AUD_STREAM_FROM_SPDIF:
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
#endif
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Switch clock source to apll since if there is no external clock,
                // dsp may fail to stop (flush done)
                _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);
#ifdef AUD_HW_LR_INVERSE_DECT                
                AUD_LineinLRDetect(FALSE);
#endif
                break;

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
            case AUD_STREAM_FROM_HDMI:
                AUD_DrvSetFgHDMIParserthread(u1DecId,fgEnable);
                AUD_AinEnable(u1DecId, eStreamFrom, FALSE);
                // Switch clock source to apll since if there is no external clock,
                // dsp may fail to stop (flush done)
                _AudClkSrcSel(u1DecId, AUD_STREAM_FROM_LINE_IN, eFmt);                
                #ifdef AUD_HW_LR_INVERSE_DECT                
                AUD_LineinLRDetect(FALSE);
                #endif                
                break;
#endif                

            case AUD_STREAM_FROM_DIGITAL_TUNER:
	        case AUD_STREAM_FROM_MULTI_MEDIA:
                // middleware will turn off dmx
                break;
                
            case AUD_STREAM_FROM_ANALOG_TUNER:
                // middleware will turn off tuner
                break;
                
            case AUD_STREAM_FROM_OTHERS:
                break;  
        
            default:
                break;
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_SetSampleFreq
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  eSmpFreq  Samping frequency
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void  AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    SAMP_CHG_LOCK();
    
    if ((_aeSampleRate[u1DecId] != eSmpFreq)||_fgChangeSource[u1DecId])
    {
        _fgChangeSource[u1DecId]=FALSE;// DTV00126551 kai wang for reset apll when change source 
        //Light: Mute external DAC or amplifier
        //Zhaoyuan: mark the codec mute/unmute. no needed with ASRC  
        //ADAC_Mute(TRUE);

        if (((u1DecId == AUD_DEC_MAIN) && (_aeStrSrcConf[AUD_DEC_MAIN] != AUD_STREAM_FROM_HDMI)) ||
			((u1DecId == AUD_DEC_AUX) && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER)))
        {       
            if(_AudGetStrFormat(u1DecId) == AUD_FMT_AAC)//for bypass DDCO 44.1K 
            {
                _aeSampleRate[u1DecId] = eSmpFreq;
                _AUD_DspSetIEC(u1DecId);
            }   
		        if((u1DecId == AUD_DEC_AUX)&&(_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_DIGITAL_TUNER)&&(_AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_AC3)&&(AUD_DRVGetAudioState(AUD_DEC_AUX)==AUD_ON_PLAY))
		        {
		            _AUD_DspSetAoutReinit(); //for samplingr rate change , do aout reinit to fix SPDIF raw output issue
		        }
             
            if(_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_AC3 || _AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_AC3
				
               || _AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_DTS || _AudGetStrFormat(AUD_DEC_AUX) == AUD_FMT_DTS)
            {
                //PLL setting
                AudPll2Setting(eSmpFreq);
            }
            else
            {
                AudPll2Setting(FS_48K);
            }
        }
        else if (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_HDMI)
        {
            bHDMIAudioSigmaDeltaSetting(0, 0, 0);
        }
		
        //_AudPllFsDivSetting(APLL_ID_AUX,eSmpFreq);
        //Light: Unmute external DAC or amplifier 
        //ADAC_Mute(FALSE);
    }
    
    _aeSampleRate[u1DecId] = eSmpFreq;    
    ADAC_GPIOPopMute(FALSE);
    SAMP_CHG_UNLOCK();   
}

//-----------------------------------------------------------------------------
/** AUD_GetSampleFreq
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval Samping frequency
 */
//-----------------------------------------------------------------------------
SAMPLE_FREQ_T AUD_GetSampleFreq(UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    return _aeSampleRate[u1DecId];
}

//-----------------------------------------------------------------------------
/** AUD_GetAinCfg
 *
 *  @param  eStreamFrom   
 *  @param  prAinCfg   
 *
 *  @retval TRUE
 *  @retval FALSE
 */
//-----------------------------------------------------------------------------
BOOL AUD_GetAinCfg(AUD_DEC_STREAM_FROM_T eStreamFrom, AIN_CFG_T* prAinCfg)
{   
    if (!_fgHwInit)
    {
        return FALSE;
    }

    if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)prAinCfg), (const VOID *)&_rLineInCfg, sizeof(AIN_CFG_T));
    }
    else if (eStreamFrom == AUD_STREAM_FROM_SPDIF)
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)prAinCfg), (const VOID *)&_rSPDIFInCfg, sizeof(AIN_CFG_T)); 
    }
    else if (eStreamFrom == AUD_STREAM_FROM_HDMI)
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)prAinCfg), (const VOID *)&_rHDMIInCfg, sizeof(AIN_CFG_T)); 
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** AUD_AinInit
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinInit(void)
{
  
}

//-----------------------------------------------------------------------------
/** AUD_AinGetMode
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinGetMode(BOOL *pfgDeteced, AUD_FMT_T *peMode)
{
#ifndef CC_AUD_HDMI_PARSER_3_0_SUPPORT           
    UINT32 u4Reg = 0;
    //UINT8 bHDMIReg;
    static AUD_FMT_T peOriMode = AUD_FMT_AC3;    
#endif

    if ((pfgDeteced == NULL) || (peMode == NULL))
    {
        return;
    }
    
#ifndef CC_AUD_HDMI_PARSER_3_0_SUPPORT           
    u4Reg = AUD_READ32(REG_SPDFTYP);
    //bHDMIReg = bHDMIReadDevH(0x2A); // HDMIH 0x2a [1] : SPDIF RAW(1) or PCM(0)
    *pfgDeteced = ((u4Reg & TYPE_DETECT) == 0) ? FALSE : TRUE;
#else
    AUD_DrvGetParserDetect(pfgDeteced);
#endif
    
    
 
    if (*pfgDeteced)
    {
#ifndef CC_AUD_HDMI_PARSER_3_0_SUPPORT       
        if ((u4Reg & TYPE_ROUGH) == TYPE_PCM)
        {
            *peMode = AUD_FMT_PCM;
            /*
            if ((bHDMIReg & 0x2) != 0)
            {
                // audio type from HDMI Rx is RAW
                // set as not detected
                *pfgDeteced = FALSE;
            }
            */
        }
        else if (((u4Reg & TYPE_DETAIL) == TYPE_AC3) || ((u4Reg & TYPE_DETAIL) == TYPE_EAC3))
        {
            if(AUD_DspIsFormatValid(AUD_FMT_AC3))
            {
                *peMode = AUD_FMT_AC3;
            }
            else
            {
                *peMode = AUD_FMT_UNKNOWN;
            }
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_PAUSE)        
        {            
            *peMode = peOriMode;        
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L1)
        {
            *peMode = AUD_FMT_MPEG;
        }
        // Note: MP2/MP3 has the same type in IEC61937!
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG1_L2L3)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L1LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else if ((u4Reg & TYPE_DETAIL) == TYPE_MPEG2_L2LSF)
        {
            *peMode = AUD_FMT_MPEG;
        }
        else
        {
            *peMode = AUD_FMT_UNKNOWN;
            LOG(3, "SPDIF format not support\n");
        }
        peOriMode = *peMode;   
#else
        AUD_DrvGetParserFormat(peMode);     
#endif
        
    }
}


void AUD_AinSetMode(UINT8 u1Mode)
{
    if (u1Mode == 0) // PCM
    {
#ifdef USE_16BIT_ALIGNMENT    
        AUD_CLR_BIT(REG_LIN_CFG0, DATA_PCM_24_BITS);   
#else  
        AUD_SET_BIT(REG_LIN_CFG0, DATA_PCM_24_BITS);
#endif

    }
    else if (u1Mode == 1) // RAW
    {
        AUD_CLR_BIT(REG_LIN_CFG0, DATA_PCM_24_BITS);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AinSrcSel
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eAudFmt)
{
   UINT32 u4Format1 = 0;	
   UINT32 u4Format2 = 0;	
   
   AUD_DEC_ID_VALIDATE(u1DecId);
    
   // Setup format
#ifndef CC_AUD_SKYPE_SUPPORT   
    if (u1DecId == AUD_DEC_MAIN)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK1_MASK);
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK2_MASK);
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK3_MASK);    
    }       
    u4Format2= AUD_READ32(REG_LIN_CFG0) & (~SPDIF_FMT_MASK);
#else
    if (u1DecId == AUD_DEC_MAIN)
    {    
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK0_MASK);
        u4Format2= AUD_READ32(REG_LIN_CFG0) & (~SPDIF_FMT_MASK);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK1_MASK);
        u4Format2= AUD_READ32(REG_DEC2_LIN_CFG0) & (~SPDIF_FMT_MASK);
    }
    else if (u1DecId == AUD_DEC_THIRD)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK2_MASK);
        u4Format2= AUD_READ32(REG_LIN_CFG0) & (~SPDIF_FMT_MASK);
    }
    else if (u1DecId == AUD_DEC_4TH)
    {
        u4Format1= AUD_READ32(REG_BS_CFG) & (~PTR_SEL_BANK3_MASK);
        u4Format2= AUD_READ32(REG_LIN_CFG0) & (~SPDIF_FMT_MASK);        
    }    
#endif
    if ((eStreamFrom == AUD_STREAM_FROM_SPDIF) ||
        (eStreamFrom == AUD_STREAM_FROM_HDMI))
    {
        if (eAudFmt == AUD_FMT_PCM)
        {
            u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
#ifdef  USE_16BIT_ALIGNMENT
            u4Format2 |= (DATA_RAW_16_BITS|
   	                      DATA_NO_SWAP|
        	              RISC_INT_PERIOD_256 |
   	                      INT_ADC_EXTLIN |
    	                  PCM_CH_NUM_2_CH);
#else
            u4Format2 |= (DATA_RAW_16_BITS |
   	                DATA_NO_SWAP |
   	                RISC_INT_PERIOD_256 |
   	                INT_ADC_EXTLIN |   	                
    	                PCM_CH_NUM_2_CH);	
#endif
        }
        else
        {
            if(eStreamFrom == AUD_STREAM_FROM_HDMI)
            {
#ifndef CC_AUD_HDMI_PARSER_2_0_SUPPORT                
                u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
#else
                u4Format1 |= ( PTR_SEL_BANK0_PSR);
#endif				
            }
            else
            {
                u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
            }        		
            u4Format2 |= (DATA_RAW_16_BITS |
   	                DATA_NO_SWAP |
   	                RISC_INT_PERIOD_256 |
   	                INT_ADC_EXTLIN |   	                
    	                PCM_CH_NUM_2_CH);				
        }  

#ifndef CC_MT5365    //No SPDIF in in MT5365
       // Init SPDIF-in receiver
       SPDIF_InEnable(TRUE);
#endif       
       
       AUD_WRITE32(REG_BS_CFG, u4Format1);		
       AUD_WRITE32(REG_LIN_CFG0, u4Format2);	
    }
    else if (eStreamFrom == AUD_STREAM_FROM_LINE_IN)
    {
#ifndef CC_AUD_SKYPE_SUPPORT         
        #ifdef DSP_TEST_LINE2
        u4Format1 |= ( PTR_SEL_BANK0_DEC2_SPDIF_LINE_IN);
        #else
        if (u1DecId == AUD_DEC_MAIN)
        {    
            u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4Format1 |= ( PTR_SEL_BANK1_SPDIF_LINE_IN);
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            u4Format1 |= ( PTR_SEL_BANK2_SPDIF_LINE_IN);
        }
        else if (u1DecId == AUD_DEC_4TH)
        {
            u4Format1 |= ( PTR_SEL_BANK3_SPDIF_LINE_IN);        
        }
        #endif
#else
        if (u1DecId == AUD_DEC_MAIN)
        {    
        u4Format1 |= ( PTR_SEL_BANK0_SPDIF_LINE_IN);
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            u4Format1 |= ( PTR_SEL_BANK1_DEC2_SPDIF_LINE_IN);
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            u4Format1 |= ( PTR_SEL_BANK2_SPDIF_LINE_IN);
        }
        else if (u1DecId == AUD_DEC_4TH)
        {
            u4Format1 |= ( PTR_SEL_BANK3_SPDIF_LINE_IN);        
        }        
#endif        

#ifdef USE_16BIT_ALIGNMENT        
        u4Format2 |= (DATA_RAW_16_BITS |
                    DATA_NO_SWAP |
                    RISC_INT_PERIOD_256 |
                    #ifdef CC_FPGA
                    INT_ADC_EXTLIN |
                    #else
                    INT_ADC_INTLIN |
                    #endif
                    PCM_CH_NUM_2_CH/*FIXME*/); 	
#else
        u4Format2 |= (DATA_PCM_24_BITS |
                    DATA_NO_SWAP |
                    RISC_INT_PERIOD_256 |
                    INT_ADC_INTLIN |
                    PCM_CH_NUM_2_CH/*FIXME*/); 	      
#endif
   	                
        AUD_WRITE32(REG_BS_CFG, u4Format1);		
#ifndef CC_AUD_SKYPE_SUPPORT           	
        AUD_WRITE32(REG_LIN_CFG0, u4Format2);
        #ifdef DSP_TEST_LINE2
        AUD_WRITE32(REG_DEC2_LIN_CFG0, u4Format2);
        #endif
#else
        if (u1DecId == AUD_DEC_MAIN)
        {    
            AUD_WRITE32(REG_LIN_CFG0, u4Format2);	
        }
        else if (u1DecId == AUD_DEC_AUX)
        {
            AUD_WRITE32(REG_DEC2_LIN_CFG0, u4Format2);
        }
        else if (u1DecId == AUD_DEC_THIRD)
        {
            AUD_WRITE32(REG_LIN_CFG0, u4Format2);
        }        	
#endif        	
    }
   
    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_SPDIF:
            AUD_AinCfg(AUD_STREAM_FROM_SPDIF, (const AIN_CFG_T * )&_rSPDIFInCfg);
            //AUD_WRITE32(REG_SPLIN_CTL, DATA_RAW_16_BITS);
            break;
        case AUD_STREAM_FROM_LINE_IN:
            AUD_AinCfg(AUD_STREAM_FROM_LINE_IN, (const AIN_CFG_T * )&_rLineInCfg);
            break;
        case AUD_STREAM_FROM_HDMI:
            AUD_AinCfg(AUD_STREAM_FROM_HDMI, (const AIN_CFG_T * )&_rHDMIInCfg);
            break;
        default:
            ASSERT(0);
    }
    
}

//-----------------------------------------------------------------------------
/** AUD_AinEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    // Enable input source
    switch (eStreamFrom)
    {
        case AUD_STREAM_FROM_DIGITAL_TUNER: 
	  case AUD_STREAM_FROM_MULTI_MEDIA:
        case AUD_STREAM_FROM_MEMORY:
        case AUD_STREAM_FROM_OTHERS:
            break;
            
        case AUD_STREAM_FROM_SPDIF:
        case AUD_STREAM_FROM_HDMI:
        case AUD_STREAM_FROM_LINE_IN:   
            if (fgEnable)
            {
                #ifdef DSP_TEST_LINE2
                AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                #else
                AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                #endif
            }
            else
            {
                #ifdef DSP_TEST_LINE2
                AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
                #else            
                AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);	
                #endif
            }
            break;
				
        default:
            break;
	}
}

//-----------------------------------------------------------------------------
/** AUD_AinCfg
 *  Set value for audio input related configuration
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AinCfg(AUD_DEC_STREAM_FROM_T eStrSrc, const AIN_CFG_T* prAinCfg)
{
    UINT32 u4AUDCFG = 0;

    // Update local variables
    switch (eStrSrc)
    {
        case AUD_STREAM_FROM_LINE_IN:
            x_memcpy((VOID *)VIRTUAL((UINT32)&_rLineInCfg), (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break;
        case AUD_STREAM_FROM_HDMI:
            x_memcpy((VOID *)VIRTUAL((UINT32)&_rHDMIInCfg), (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break;
        case AUD_STREAM_FROM_SPDIF:
            x_memcpy((VOID *)VIRTUAL((UINT32)&_rSPDIFInCfg), (const VOID *) prAinCfg, sizeof(AIN_CFG_T));
            break;
        default:
            LOG(1, "ERROR AUD_AinCfg\n");
            return;
    }

    // Save original audio output setting 	
    u4AUDCFG = AUD_READ32(REG_EXTADC_CFG) & (~AUD_IN_CFG_MASK);

    if (prAinCfg != NULL)
    {
        switch (prAinCfg->eFormat)
        {
            case FORMAT_I2S:
                u4AUDCFG |= SPLIN_I2S;
                break;
            case FORMAT_LJ:
                u4AUDCFG |= SPLIN_LJ;
                break;
            case FORMAT_RJ:
                u4AUDCFG |= SPLIN_RJ;
                break;
            default:
                break;
        }

        switch (prAinCfg->eBits)
        {
            case DAC_16_BIT:
            	u4AUDCFG |= ADC_16BIT;
            	break;
            case DAC_18_BIT:
            	u4AUDCFG |= ADC_18BIT;
            	break;
            case DAC_20_BIT:
            	u4AUDCFG |=ADC_20BIT;
            	break;
            case DAC_24_BIT:	
            	u4AUDCFG |= ADC_24BIT;
            	break;
            default:
            	break;
        }

        switch (prAinCfg->eCycle)
        {
            case LRCK_CYC_16:
                u4AUDCFG |= SPDIF_CYC16;
                break;
            case LRCK_CYC_24:
                u4AUDCFG |= SPDIF_CYC24;
                break;
            case LRCK_CYC_32:
                u4AUDCFG |= SPDIF_CYC32;
                break;
            default:
                break;
        }

        if (prAinCfg->fgLRInvert)
        {
            u4AUDCFG |= SPLIN_LR_INVT;
        }
        
        #ifdef DSP_TEST_LINE2
        AUD_WRITE32(REG_EXTADC_CFG, (u4AUDCFG&0xffff) | ((u4AUDCFG&0xffff)<<16)); //for line-in 2 control setting
        AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        AUD_SET_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        #else
        AUD_WRITE32(REG_EXTADC_CFG, u4AUDCFG);
        AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);
        AUD_SET_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);        
        #endif
    }
    else
    {
        ASSERT(0); 
    }
}

void AUD_AinFormat(AUD_DEC_STREAM_FROM_T eStrSrc, DATA_FORMAT_T eDataFormat)
{
    AIN_CFG_T * prAinCfg = NULL;

    switch (eStrSrc)
    {
        case AUD_STREAM_FROM_LINE_IN:
            prAinCfg = &_rLineInCfg;
            break;
        case AUD_STREAM_FROM_HDMI:
            prAinCfg = &_rHDMIInCfg;
            break;
        case AUD_STREAM_FROM_SPDIF:
            prAinCfg = &_rSPDIFInCfg;
            break;
        default:
            LOG(1, "ERROR AUD_AinCfg\n");
            prAinCfg = NULL;
    }

    if (prAinCfg != NULL)
    {   
        if (prAinCfg->eFormat != eDataFormat)
        {
            prAinCfg->eFormat = eDataFormat;
            AUD_AinCfg(eStrSrc, prAinCfg);
        }
    }   
}

//-----------------------------------------------------------------------------
/** AUD_AoutDrvCur
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur ,UINT8 u1LRckDrvCur, UINT8 u1AOSDATADrvCur)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

#if defined(CC_MT5396)

    switch (u1MckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_2MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_4MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_6MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
        	break;
        default:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
    }

    switch (u1BckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_2MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_4MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_6MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;	
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;
    }

    switch (u1LRckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_2MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_4MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_6MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
    }

    switch (u1AOSDATADrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_2MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_4MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_6MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;	
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG3, PADDRV_8MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;
    }
    
    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(CKGEN_PADDRVCFG4, PADDRV_8MA, FLD_PADDRV_ASPDIF0);
    vIO32WriteFldAlign(CKGEN_PADDRVCFG4, PADDRV_8MA, FLD_PADDRV_ASPDIF1);

#elif defined(CC_MT5368)

    switch (u1MckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOMCLK);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOMCLK);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOMCLK);            
        	_au1MckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOMCLK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOMCLK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
        	break;
        default:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOMCLK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOMCLK);            
        	_au1MckDrvCur[u1DecId] = 8;
    }

    switch (u1BckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOBCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOBCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOBCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOBCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;	
        	break;
        default:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOBCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;
    }

    switch (u1LRckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOLRCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOLRCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOLRCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOLRCK);            
        	_au1LRckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOLRCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
        	break;
        default:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOLRCK);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
    }

    switch (u1AOSDATADrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOSDATA0);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOSDATA0);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_AOSDATA0);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOSDATA0);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;	
        	break;
        default:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_AOSDATA0);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;
    }
    
    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_SPDIF0);
    vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_SPDIF0);

    vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_SPDIF1);
    vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADDRV_SPDIF1);

#elif defined(CC_MT5389)    

    switch (u1MckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_7);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_7);
            _au1MckDrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_7);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_7);            
            _au1MckDrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_7);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_7);
            _au1MckDrvCur[u1DecId] = 6;
            break;  
        case 7:
        case 8:
        default:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_7);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_7);
            _au1MckDrvCur[u1DecId] = 8;
            break;
    }
    //NOTE!!bck lrck aosdata0 is the same setting!
    switch (u1BckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_8);
            _au1BckDrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_8);
            _au1BckDrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_8);
            _au1BckDrvCur[u1DecId] = 6;
            break;  
        case 7:
        case 8:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_8);
            _au1BckDrvCur[u1DecId] = 8; 
            break;
    }

    switch (u1LRckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_8);
            _au1LRckDrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_8);
            _au1LRckDrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_8);            
            _au1LRckDrvCur[u1DecId] = 6;
            break;  
        case 7:
        case 8:
        default:            
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_8);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_8);
            _au1LRckDrvCur[u1DecId] = 8;
            break;
    }
    //NOTE!!only set the aosdata1
    switch (u1AOSDATADrvCur)
    {
        case 0:
        case 1:
        case 2:
            // 00
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_9);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_9);
            _au1AOSDATADrvCur[u1DecId] = 2;
            break;
        case 3:
        case 4:
            // 01
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_9);
            vIO32WriteFldAlign(CKGEN_PADE40, 0, FLD_PADE4_9);
            _au1AOSDATADrvCur[u1DecId] = 4;
            break;
        case 5:
        case 6:
            // 10
            vIO32WriteFldAlign(CKGEN_PADDRV, 0, FLD_PADDRV_9);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_9);
            _au1AOSDATADrvCur[u1DecId] = 6;
            break;  
        case 7:
        case 8:
        default:
            // 11
            vIO32WriteFldAlign(CKGEN_PADDRV, 1, FLD_PADDRV_9);
            vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_9);
            _au1AOSDATADrvCur[u1DecId] = 8; 
            break;
    }

    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(CKGEN_PADE40, 1, FLD_PADE4_6);
    vIO32WriteFldAlign(CKGEN_PADE80, 1, FLD_PADE8_6);

#elif defined(CC_MT5395)

    switch (u1MckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_2MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_4MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_6MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_8MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
        	break;
        default:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_8MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
    }

    switch (u1BckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_2MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_4MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_6MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_8MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;	
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_8MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;
    }

    switch (u1LRckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_2MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_4MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_6MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_8MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_8MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
    }

    switch (u1AOSDATADrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_2MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_4MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_6MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_8MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;	
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG1, PADDRV_8MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;
    }
    
    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(CKGEN_PADDRVCFG0, PADDRV_8MA, FLD_PADDRV_ASPDIF);

#elif defined(CC_MT5365)
    
    switch (u1MckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_2MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_4MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_6MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_8MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
        	break;
        default:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_8MA, FLD_PADDRV_AOMCLK);
        	_au1MckDrvCur[u1DecId] = 8;
    }

    switch (u1BckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_2MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_4MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_6MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_8MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;	
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_8MA, FLD_PADDRV_AOBCK);
        	_au1BckDrvCur[u1DecId] = 8;
    }

    switch (u1LRckDrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_2MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_4MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_6MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_8MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_8MA, FLD_PADDRV_AOLRCK);
        	_au1LRckDrvCur[u1DecId] = 8;
    }

    switch (u1AOSDATADrvCur)
    {
        case 0:
        case 1:
        case 2:
            vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_2MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 2;
        	break;
        case 3:
        case 4:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_4MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 4;
        	break;
        case 5:
        case 6:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_6MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 6;
        	break;	
        case 7:
        case 8:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_8MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;	
        	break;
        default:
        	vIO32WriteFldAlign(CKGEN_PADDRVCFG2, PADDRV_8MA, FLD_PADDRV_AOSDATA0);
        	_au1AOSDATADrvCur[u1DecId] = 8;
    }
    
    // Light: Set SPDIF output PAD driving current to 8mA
    vIO32WriteFldAlign(CKGEN_PADDRVCFG, PADDRV_8MA, FLD_PADDRV_ASPDIF);
#endif
}

//-----------------------------------------------------------------------------
/** AUD_AoutCfg
 *  Audio output configuration
 *  1. Setup Aout registers
 *  2. Setup ADAC
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutCfg(UINT8 u1DecId, const AOUT_CFG_T* prAoutCfg)
{
    UINT32 u4AUDCFG = 0;
    UINT32 u4AUDCFG2 = 0;
    UINT32 u4AUDCFG3 = 0;    

    AUD_DEC_ID_VALIDATE_2(u1DecId);

    // Update configuration to local variable
    if (prAoutCfg != NULL)
    {
        x_memcpy((VOID *)VIRTUAL((UINT32)&_arAudOutCfg[u1DecId]), (const VOID *) prAoutCfg, sizeof(AOUT_CFG_T));
    }

    if (prAoutCfg != NULL)
    {
        switch (prAoutCfg->eFormat)
        {
            case FORMAT_RJ:
                u4AUDCFG |= AOFMT_RJ;
                break;
            case FORMAT_LJ:
                u4AUDCFG |= AOFMT_LJ;
                break;
            case FORMAT_I2S:  
                u4AUDCFG |= AOFMT_I2S;
                break;    
            default:
                u4AUDCFG |= AOFMT_I2S;
        }
    
        switch (prAoutCfg->eSampleFreq)
        {
            case MCLK_128FS:
        	  u4AUDCFG2 |= (BCK_DIV_2 );
        	  u4AUDCFG3 |= (DEC2_BCK_DIV_2 );
        	  break;
        	case MCLK_256FS:
        	  u4AUDCFG2 |= (BCK_DIV_4 );
        	  u4AUDCFG3 |= (DEC2_BCK_DIV_4 );
        	  break;
        	case MCLK_384FS:  
        	  u4AUDCFG2 |= (BCK_DIV_6 );
        	  u4AUDCFG3 |= (DEC2_BCK_DIV_6);
        	  break;	
        	case MCLK_512FS:
        	  u4AUDCFG2 |= (BCK_DIV_8 );
        	  u4AUDCFG3 |= (DEC2_BCK_DIV_8);
        	  break;
        	default:
        	  u4AUDCFG2 |= (BCK_DIV_4);
        	  u4AUDCFG3 |= (DEC2_BCK_DIV_4 );
        	  LOG( 3,"Sampling Freq not support\n");
        }
    
        switch (prAoutCfg->eCycle)
        {
            case LRCK_CYC_16:
                u4AUDCFG |= LRCK_CYC16;
                break;
            case LRCK_CYC_24:
                u4AUDCFG |= LRCK_CYC24;
                break;
            case LRCK_CYC_32:  
                u4AUDCFG |= LRCK_CYC32;
                break;    
            default:
                u4AUDCFG |= LRCK_CYC24;
        }
    
        switch (prAoutCfg->eBits)
        {
            case DAC_16_BIT:
                u4AUDCFG |= DAC_16BIT;
                break;
            case DAC_18_BIT:
                u4AUDCFG |= DAC_18BIT;
                break;
            case DAC_20_BIT:  
                u4AUDCFG |= DAC_20BIT;
                break;
            case DAC_24_BIT:  
                u4AUDCFG |= DAC_24BIT;
                break;      
            default:
                u4AUDCFG |= DAC_24BIT;
        }
    
        // If invert the Sdata output
        if (prAoutCfg->fgDataInvert)
        {
            u4AUDCFG |= INV_SDATA;
        }

        // Light: For I2S format the INV_LRCK is not set in normal mode.
        // Light: For I2S format the INV_LRCK is set in swap mode.
        // Light: For LJ and RJ formats the INV_LRCK is set in normal mode.
        // Light: For LJ and RJ formats the INV_LRCK is not set in swap mode.
        if ((prAoutCfg->eFormat == FORMAT_RJ)||(prAoutCfg->eFormat == FORMAT_LJ))
        {
            if (!prAoutCfg->fgLRInvert)
            {
                u4AUDCFG |= INV_LRCK;
            }
        }
        else // For I2S format and default
        {
            // If invert L/R output
            if (prAoutCfg->fgLRInvert)
            {
                u4AUDCFG |= INV_LRCK;
            }
        }
        
        // setting driving current
        //u4AUDCFG |= IEC_CUR_4;  ???
        u4AUDCFG |= INV_BCK;
        AUD_AoutDrvCur(u1DecId, _au1MckDrvCur[u1DecId], _au1BckDrvCur[u1DecId], _au1LRckDrvCur[u1DecId] , _au1AOSDATADrvCur[u1DecId]); 

    }
    
    if (u1DecId == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_EXTDAC_CFG0, (AUD_READ32(REG_EXTDAC_CFG0) & (~AUD_OUT_CFG_MASK)) | u4AUDCFG);
        AUD_WRITE32(REG_ACK_CFG,(AUD_READ32(REG_ACK_CFG) & ~(BCK_DIV_MASK)) | u4AUDCFG2);
        AUD_WRITE32(REG_ACK_CFG,(AUD_READ32(REG_ACK_CFG) & ~(IEC_DIV_MASK)) | IEC_DIV_2);
    }    
    else // AUD_DEC_AUX
    {
        AUD_WRITE32(REG_EXTDAC_CFG1,(AUD_READ32(REG_EXTDAC_CFG1) & (~AUD_OUT_CFG_MASK)) | u4AUDCFG);
        AUD_WRITE32(REG_ACK_CFG,(AUD_READ32(REG_ACK_CFG) & ~(DEC2_BCK_DIV_MASK)) | u4AUDCFG3);
        AUD_WRITE32(REG_ACK_CFG,(AUD_READ32(REG_ACK_CFG) & ~(DEC2_IEC_DIV_MASK)) | DEC2_IEC_DIV_2);
    }

    // Update ADAC
    if (prAoutCfg != NULL)
    {
        ADAC_DacFormatCfg(u1DecId, prAoutCfg->eFormat, prAoutCfg->eSampleFreq);
    }    
}

//-----------------------------------------------------------------------------
/** AUD_AoutFormat
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutFormat(UINT8 u1DecId, DATA_FORMAT_T eDataFormat)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eFormat != eDataFormat)
    {
        _arAudOutCfg[u1DecId].eFormat = eDataFormat;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }   
}

//-----------------------------------------------------------------------------
/** AUD_AoutFormat
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutMclk(UINT8 u1DecId, MCLK_FREQUENCY_T eMclk)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eSampleFreq != eMclk)
    {
        _arAudOutCfg[u1DecId].eSampleFreq = eMclk;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }   
}

//-----------------------------------------------------------------------------
/** AUD_AoutDacFs
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs)
{
    UNUSED(u1DecId);
    UNUSED(eFs);
}

//-----------------------------------------------------------------------------
/** AUD_AoutInvertData
 *  Invert the Sdata output for OP phase
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutInvertData(UINT8 u1DecId, BOOL fgInvert)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].fgDataInvert != fgInvert)
    {
        _arAudOutCfg[u1DecId].fgDataInvert = fgInvert;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }   
}

//-----------------------------------------------------------------------------
/** AUD_AoutDataNum
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutDataNum(UINT8 u1DecId, DAC_DATA_NUMBER_T eDataNum)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eBits != eDataNum)
    {
        _arAudOutCfg[u1DecId].eBits = eDataNum;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }   
}

//-----------------------------------------------------------------------------
/** AUD_AoutLRCycle
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutLRCycle(UINT8 u1DecId, LRCK_CYC_T eCycle)
{
    AUD_DEC_ID_VALIDATE_2(u1DecId);

    if (_arAudOutCfg[u1DecId].eCycle != eCycle)
    {
        _arAudOutCfg[u1DecId].eCycle = eCycle;
        AUD_AoutCfg(u1DecId, &_arAudOutCfg[u1DecId]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_AoutPADMux
 *
 *  @param  PAD_SDATA0 (AOUTDATA0_MAIN ~ AOUTDATA4_MAIN, AOUTDATA0_AUX),
 *          PAD_SDATA1, ..., PAD_SDATA5,
 *          PAD_MCLK0 (MCLK0 / MCLK1)
 *          Notice that PAD_MCLK1 (MCLK0 /MCLK1) is decided by PAD_SDATA5
 *          
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5, 
                    UINT8 PAD_MCLK0)
{
    UINT32 u4ADSP_ENV_BAK;

    //Conflict check
    if (((((UINT32)ePAD_SDATA0)/2) != (((UINT32)ePAD_SDATA5)/2)) &&
        ((((UINT32)ePAD_SDATA0)/2) <= AOUTDATA4_MAIN) &&
       ((((UINT32)ePAD_SDATA5)/2) <= AOUTDATA4_MAIN))       
    {
        Printf("AUD_AoutPADMux DATA0/DATA5 conflict\n");
        return;
    }
    //End
            
    u4ADSP_ENV_BAK = AUD_READ32(REG_CH_CFG) & (~(CH_SEL|SDATA_MCLK_SYNC));
    
    //PAD_SDATA0
    switch (((UINT32)ePAD_SDATA0)/2)
    {
        default:    
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA0;
       	    break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA2;
       	    break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA3;
       	    break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA4;
       	    break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH0_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA0);
   	        break;
    }    

    //PAD_SDATA1
    switch (((UINT32)ePAD_SDATA1)/2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA0;
   	        break;
        default:
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA2;
       	    break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA3;
       	    break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH1_AOUTDATA4;
       	    break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH1_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA1);
   	        break;
    }

    //PAD_SDATA2
    switch (((UINT32)ePAD_SDATA2)/2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA0;
       	    break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA1;
            break;
        default:
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA2;
       	    break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA3;
   	        break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH2_AOUTDATA4;
       	    break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH2_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA2);
   	        break;
    }

    //PAD_SDATA3
    switch (((UINT32)ePAD_SDATA3)/2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA0;
   	        break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA2;
       	    break;
        default:
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA3;
   	        break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH3_AOUTDATA4;
       	    break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH0_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA3);
   	        break;
    }

    //PAD_SDATA4
    switch (((UINT32)ePAD_SDATA4)/2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA0;
       	    break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA2;
       	    break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA3;
       	    break;
        default:
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH4_AOUTDATA4;
       	    break;
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH1_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA4);
       	    break;
    }
    
    //PAD_SDATA5
    switch (((UINT32)ePAD_SDATA5)/2)
    {
        case AOUTDATA0_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA0;
       	    break;
        case AOUTDATA1_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA1;
            break;
        case AOUTDATA2_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA2;
       	    break;
        case AOUTDATA3_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA3;
       	    break;
        case AOUTDATA4_MAIN:
            u4ADSP_ENV_BAK |= CH0_AOUTDATA4;
       	    break;
        default:            
        case AOUTDATA0_AUX:
            u4ADSP_ENV_BAK |= (DEC2_CH0_AOUTDATA0 | AOUTDATA0_AUX_TO_SDATA5);
            break;
    }

#ifdef CC_AUD_EU_INTERNALDAC_SAME_APLL
    u4ADSP_ENV_BAK = u4ADSP_ENV_BAK | AO_BCK_LRCK_SEL;
#endif
    AUD_WRITE32(REG_CH_CFG, u4ADSP_ENV_BAK);
    
    if ((BOOL)DRVCUST_OptGet(eAudioAI2SuseforSpeaker))
    {
    	AUD_CLR_BIT(REG_CH_CFG,AO_BCK_LRCK_SEL);
    }
    //AUD_WRITE32(REG_CH_CFG, u4AENV_CFG);     // (Jessica)
    
    UNUSED (PAD_MCLK0);
}

static CHAR* _aszPadCh[] = {"LR","SLR","CLFE","BYPASS","DOWNMIX"};

void AUD_AoutPADMuxQuery(void)
{
    UINT8 u1Index;
    UINT32 u4Tmp = AUD_READ32(REG_CH_CFG);

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA0)
    {
        //DEC1
        u1Index = ((u4Tmp>>DEC2_CH0_SEL_POS)&0x7);
        if (u1Index < 5)
            Printf("AOSDATA0: DEC1 - %s\n", _aszPadCh[u1Index]);
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp>>CH0_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA0: DEC0 - %s\n", _aszPadCh[u1Index]);
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA1)
    {
        //DEC1
        u1Index = ((u4Tmp>>DEC2_CH1_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA1: DEC1 - %s\n", _aszPadCh[u1Index]);
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp>>CH1_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA1: DEC0 - %s\n", _aszPadCh[u1Index]);
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA2)
    {
        //DEC1
        u1Index = ((u4Tmp>>DEC2_CH2_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA2: DEC1 - %s\n", _aszPadCh[u1Index]);
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp>>CH2_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA2: DEC0 - %s\n", _aszPadCh[u1Index]);
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA3)
    {
        //DEC1
        u1Index = ((u4Tmp>>DEC2_CH0_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA3: DEC1 - %s\n", _aszPadCh[u1Index]);
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp>>CH3_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA3: DEC0 - %s\n", _aszPadCh[u1Index]);
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA4)
    {
        //DEC1
        u1Index = ((u4Tmp>>DEC2_CH1_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA4: DEC1 - %s\n", _aszPadCh[u1Index]);
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp>>CH4_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("AOSDATA4: DEC0 - %s\n", _aszPadCh[u1Index]);
    }

    if (u4Tmp & AOUTDATA0_AUX_TO_SDATA5)
    {
        //DEC1
        u1Index = ((u4Tmp>>DEC2_CH0_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("BOSDATA0: DEC1 - %s\n", _aszPadCh[u1Index]);
    }
    else
    {
        //DEC0
        u1Index = ((u4Tmp>>CH0_SEL_POS)&0x7);
        if (u1Index < 5)        
            Printf("BOSDATA0: DEC0 - %s\n", _aszPadCh[u1Index]);
    }
}

//-----------------------------------------------------------------------------
/** AUD_SpdifInPinSelect
 *  Set SPDIF (spdif-in 1 and spdif-in 2 use the same config)
 *
 *  @param  fgSet   TRUE: set as input, FALSE: set as GPIO
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_SpdifInPinSelect(BOOL fgSet)
{

    UNUSED(fgSet);
/* 
    UINT32 u4PadCfg0;

   
    u4PadCfg0 = CKGEN_READ32(REG_PAD_CFG_0) & (~((UINT32)(PAD_SPDIF_SEL)));
    if (fgSet)
    {
        u4PadCfg0 |= PAD_SPDIF;
    }
    CKGEN_WRITE32(REG_PAD_CFG_0, u4PadCfg0);
*/
    // TODO 
    // for line-in pad config
}

void _AUD_SpdifInBypass(SPDIF_IN_PIN_NUM u1PIN)
{
#ifdef CC_MT5389
    AUD_SET_BIT(REG_SPOCFG0, BYP_SPDIFIN);
    switch (u1PIN)
    {
        case PAD_AOSDATA1:
            vIO32WriteFldAlign(CKGEN_PMUX1, 6, FLD_PAD_AOSDATA1);
            AUD_WRITE32(REG_ACK2_CFG,  \
                (AUD_READ32(REG_ACK2_CFG)&(~(AUD_SPDFIN_BYPS_SEL))) \
                |(PIN_PAD_AOSDATA1));
            break;
        case PAD_IF_AGC:
            vIO32WriteFldAlign(CKGEN_PMUX1, 6, FLD_PAD_IF_AGC);
            AUD_WRITE32(REG_ACK2_CFG,  \
                (AUD_READ32(REG_ACK2_CFG)&(~(AUD_SPDFIN_BYPS_SEL))) \
                |(PIN_PAD_IF_AGC));
            break;
        case PAD_RF_AGC:
            vIO32WriteFldAlign(CKGEN_PMUX1, 6, FLD_PAD_RF_AGC);
            AUD_WRITE32(REG_ACK2_CFG,  \
                (AUD_READ32(REG_ACK2_CFG)&(~(AUD_SPDFIN_BYPS_SEL))) \
                |(PIN_PAD_RF_AGC));
            break;
        case PAD_DISTABLE:
        default:
            AUD_CLR_BIT(REG_SPOCFG0, BYP_SPDIFIN);
            break;
    }
#else
    LOG(0,"Please Check SPDIF-IN Bypass Function. \n");
#endif
}

//-----------------------------------------------------------------------------
/** AUD_LineInCtrlEnable
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
//#include "x_timer.h"
//static HAL_TIME_T _rTime;
void AUD_LineInCtrlEnable(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    if (fgEnable)
    {   // Turn on line-in buffer control	
        #ifdef DSP_TEST_LINE2
        AUD_SET_BIT(REG_DEC2_LIN_CFG0,SPDIF_LINE_IN_ENABLE);			//laihui
        #else
        AUD_SET_BIT(REG_LIN_CFG0,SPDIF_LINE_IN_ENABLE);			//laihui
        #endif
    }
    else
    {   // Turn off line-in buffer control	
        #ifdef DSP_TEST_LINE2
        AUD_CLR_BIT(REG_DEC2_LIN_CFG0, SPDIF_LINE_IN_ENABLE);		//laihui
        #else
        AUD_CLR_BIT(REG_LIN_CFG0, SPDIF_LINE_IN_ENABLE);		//laihui
        #endif
    }
   
    if (fgEnable)
    {
        LOG(5, "LineInCtrl start capturing\n");
        // HAL_GetTime(&_rTime);
        // Printf("LineInCtrl start capturing (%d.%06d)\n", _rTime.u4Seconds, _rTime.u4Micros);
    }
    else
    {
        LOG(5, "LineInCtrl stop capturing\n");
        // HAL_GetTime(&_rTime);
        // Printf("LineInCtrl stop capturing (%d.%06d)\n", _rTime.u4Seconds, _rTime.u4Micros);
    }
}

//-----------------------------------------------------------------------------
/** AUD_OutPadEnable
 *  Enable/disable audio output pad
 *
 *  @param  u1DecId   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
 *  @param  fgEnable  1: enable (Set MCLK/LRCK/BCLK/DATA pad as output)   \n
 *                    0: diable (Set MCLK/LRCK/BCLK/DATA pad as tiastate) 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable)
{
    if (fgEnable)
    {
        AUD_SET_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? DEC0_MTRI : DEC1_MTRI);      
    }
    else
    {
        AUD_CLR_BIT(REG_ACK_CFG, (u1DecId == AUD_DEC_MAIN) ? DEC0_MTRI : DEC1_MTRI);      
    }

    LOG(8, "AUD_OutPadEnable u1DecId = %d\n", u1DecId);
}

//-----------------------------------------------------------------------------
/** AUD_AdcEnable
 *
 *  @param  fgEnable  1: enable (Set ADC un-mute)
 *                    0: diable (Set ADC mute) 
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_AdcEnable(BOOL fgEnable)
{
}

void AUD_AVComponentSel(AV_COMPONENT_T eAVComp)
{
    AUD_INPUT_ID_T eInputId;

    // Get mux index
    switch (eAVComp)
    {
        case AVC_COMP_VIDEO_0:
            eInputId = AUD_INPUT_ID_COMP_VID_0;
            break;
        case AVC_COMP_VIDEO_1:
            eInputId = AUD_INPUT_ID_COMP_VID_1;
            break;
        case AVC_COMP_VIDEO_2:
            eInputId = AUD_INPUT_ID_COMP_VID_2;
            break;
        case AVC_S_VIDEO_0:
            eInputId = AUD_INPUT_ID_S_VID_0;
            break;
        case AVC_Y_PB_PR_0:
            eInputId = AUD_INPUT_ID_YPBPR_0;
            break;
        case AVC_Y_PB_PR_1:
            eInputId = AUD_INPUT_ID_YPBPR_1;
            break;
        case AVC_VGA:
            eInputId = AUD_INPUT_ID_VGA_0;
            break;
        case AVC_HDMI:
            eInputId = AUD_INPUT_ID_DVI_0;
            break;
        case AVC_S_VIDEO_1:
            eInputId = AUD_INPUT_ID_S_VID_1;
            break;
        case AVC_S_VIDEO_2:
            eInputId = AUD_INPUT_ID_S_VID_2;
            break;  
        default:
            eInputId = AUD_INPUT_ID_COMP_VID_0;
    }

    AUD_AvMuxSel(eInputId);
    
}

void AUD_AvMuxSel(AUD_INPUT_ID_T eInputId)
{
    UINT32 u4GpioSelNum1 = UNUSED_GPIO;
    UINT32 u4GpioSelNum2 = UNUSED_GPIO;
    UINT32 u4GpioSelNum3 = UNUSED_GPIO;
    UINT32 u4GpioSelNum4 = UNUSED_GPIO;
    INT32  i4GpioSel1Value = NO_USED;
    INT32  i4GpioSel2Value = NO_USED;
    INT32  i4GpioSel3Value = NO_USED;
    INT32  i4GpioSel4Value = NO_USED;
    UINT8  u1AacChlSel;
    INT32  i4Out = 1;
    static BOOL fgInit = FALSE;
    AUD_INPUT_SW_GPIO_T* prInputSwGpio;
    AUD_INPUT_MUX_SEL_TABLE_T* prInputMuxSelTable;
    
    AUD_AdcEnable(FALSE);               // Mute ADC to avoid AV mux pop noise

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioInputSwGpioSel, (UINT32 *)(void *)&prInputSwGpio));

    UNUSED(DRVCUST_OptQuery(eAudioInputMuxSelTable, (UINT32 *)(void *)&prInputMuxSelTable));

#ifdef CC_AUD_USE_NVM
    if (!Aud_NVM_GetCountry())
    {
        if (!fgInit)
        {
            prInputMuxSelTable->prAudInMuxSel += ((INT32)(prInputMuxSelTable->u4Size))/2;
        }
    }
#endif // CC_AUD_USE_NVM

    u1AacChlSel = prInputMuxSelTable->prAudInMuxSel[eInputId].u1AdcMuxSel;
    if (u1AacChlSel >= (UINT8)AUD_INPUT_ID_MAX)
    {
        LOG(0, "Invalid AdacChlSel %d\n", (UINT32)u1AacChlSel);
    }

    // Set adac channel
    ADAC_CodecAdChlSel(prInputMuxSelTable->prAudInMuxSel[eInputId].u1AdcMuxSel);

    // Load configuration
    u4GpioSelNum1 = prInputSwGpio->u4AudInSwGpioNum1;
    u4GpioSelNum2 = prInputSwGpio->u4AudInSwGpioNum2;
    u4GpioSelNum3 = prInputSwGpio->u4AudInSwGpioNum3;
    u4GpioSelNum4 = prInputSwGpio->u4AudInSwGpioNum4;
    i4GpioSel1Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum1Val;
    i4GpioSel2Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum2Val;
    i4GpioSel3Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum3Val;
    i4GpioSel4Value = (INT32)prInputMuxSelTable->prAudInMuxSel[eInputId].u1SwGpioNum4Val;

    // Setup gpio  => Init gpio
    if (!fgInit)
    {
        if (u4GpioSelNum1 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum1, i4Out);
        }
        if (u4GpioSelNum2 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum2, i4Out);
        }
        if (u4GpioSelNum3 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum3, i4Out);
        }
        if (u4GpioSelNum4 != UNUSED_GPIO)
        {
            AUD_GPIOEnable(u4GpioSelNum4, i4Out);
        }
        
        fgInit = TRUE;
    }

    // Setup gpio  => write gpio
    if ((u4GpioSelNum1 != UNUSED_GPIO) && (i4GpioSel1Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum1, i4GpioSel1Value);
    }
    if ((u4GpioSelNum2 != UNUSED_GPIO) && (i4GpioSel2Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum2, i4GpioSel2Value);
    }
    if ((u4GpioSelNum3 != UNUSED_GPIO) && (i4GpioSel3Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum3, i4GpioSel3Value);
    }
    if ((u4GpioSelNum4 != UNUSED_GPIO) && (i4GpioSel4Value != SW_NO_USED))
    {
        AUD_GPIOOutput(u4GpioSelNum4, i4GpioSel4Value);
    }

    //Setup DVD GPIO
    _eInputIdToDvdGpioConfig =eInputId;
    AUD_DspDvdEnable();    
    AUD_AdcEnable(TRUE);               // Un-mute ADC
}

void AUD_AvMuxSelByAD(UINT8 eADNum)
{
    AUD_INPUT_SW_GPIO_T* prInputSwGpio;
    AUD_INPUT_MUX_SEL_TABLE_T* prInputMuxSelTable;
    
    AUD_AdcEnable(FALSE);               // Mute ADC to avoid AV mux pop noise

    // Query ADAC config
    UNUSED(DRVCUST_OptQuery(eAudioInputSwGpioSel, (UINT32 *)(void *)&prInputSwGpio));

    UNUSED(DRVCUST_OptQuery(eAudioInputMuxSelTable, (UINT32 *)(void *)&prInputMuxSelTable));
    // Set adac channel
    ADAC_CodecAdChlSel(eADNum);
    AUD_DspDvdEnable();    
    AUD_AdcEnable(TRUE);               // Un-mute ADC
}


INT32 _AUD_GetAudFifo(UINT8 u1DecId, UINT32 * pu4FifoStart, UINT32 * pu4FifoEnd)
{
    if ((pu4FifoStart != NULL) && (pu4FifoEnd != NULL))
    {
#ifdef CC_MT53XX_SUPPORT_2G_DRAM
        *pu4FifoStart = (u4GetAFIFOStart(u1DecId));
        *pu4FifoEnd = (u4GetAFIFOEnd(u1DecId));
#else
        *pu4FifoStart = DSP_PHYSICAL_ADDR(u4GetAFIFOStart(u1DecId));
        *pu4FifoEnd = DSP_PHYSICAL_ADDR(u4GetAFIFOEnd(u1DecId));
#endif
    }
    else
    {
        return AUD_FAIL;
    }

    LOG(10,"Audio Fifo (%d) = (%x,%x)\n", u1DecId, *pu4FifoStart, *pu4FifoEnd);

    return AUD_OK;
}

void _AUD_SpdifInPinMuxEnable(void)
{
/*
    UINT32 u4Reg1 = 0;
    UINT32 u4Reg2 = 0;

    u4Reg1 = (CKGEN_READ32(REG_PINMUX_SEL_1) & ~(0xc000)) | 0x50000000;
    u4Reg2 = (CKGEN_READ32(REG_PINMUX_SEL_2) & ~(0x0ff003ff)) | 0x0f501155;

    CKGEN_WRITE32(REG_PINMUX_SEL_1, u4Reg1);
    CKGEN_WRITE32(REG_PINMUX_SEL_2, u4Reg2);
*/
}

void AUD_SetSrc(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    
    _aeStrSrcConf[u1DecId] = eStreamFrom;
    _fgChangeSource[u1DecId] = TRUE;
}

MCLK_FREQUENCY_T AUD_GetMclkFs(UINT8 u1ApllId)
{    
    return  _arAudOutCfg[u1ApllId].eSampleFreq;
}

// *********************************************************************
// Function : void AUD_DspDvdEnable(void)
// Description : turn on / off dvd gpio by input source and linein select
// Parameter :  none
// Return    :  None.
// *********************************************************************
void AUD_DspDvdEnable(void)
{
    UINT32 u4DvdGpio;
    AUD_INPUT_ID_T eDvdInputChl;
    AUD_DVD_GPIO_POLARITY_T eDvdPolariy;
    INT32 i4RegAoutSwitch;

    //Get DVD input source and DVD gpio Num
    u4DvdGpio = DRVCUST_OptGet(eAudioDvdGpio);
    eDvdInputChl = (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);

    eDvdPolariy = (AUD_DVD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDvdPolarity);

    LOG(10, "-------- AUD_DspDvdEnable Configuration -------\n");
    LOG(10, "           DVD Gpio : %d\n", u4DvdGpio);
    LOG(10, "           Dvd Input Channel : %d\n", eDvdInputChl);
    LOG(10, "           DVD Polarity : %d\n", eDvdPolariy);
    LOG(10, "           Decoder Stream Source : %d\n", _aeStrSrcConf[AUD_DEC_MAIN]);
    LOG(10, "           _eInputIdToDvdGpioConfig = %d\n", _eInputIdToDvdGpioConfig);
    LOG(10, "-------- AUD_DspDvdEnable Configuration -------\n");
    
    if ((_aeStrSrcConf[AUD_DEC_MAIN] != AUD_STREAM_FROM_LINE_IN)  ||
        (_eInputIdToDvdGpioConfig != eDvdInputChl))
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch=0;      
        }
        else
        {
            i4RegAoutSwitch=1;
        }
        LOG(10, "Aout Switch (1) : %d\n", i4RegAoutSwitch);
        AUD_GPIOOutput(u4DvdGpio, i4RegAoutSwitch);
    }   
    else if ((u4DvdGpio != 0xffffffff) && (_eInputIdToDvdGpioConfig == eDvdInputChl) 
        && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) )
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch=1;      
        }
        else
        {
            i4RegAoutSwitch=0;
        }
        LOG(10, "Aout Switch (2) : %d\n", i4RegAoutSwitch);
        AUD_GPIOOutput(u4DvdGpio, i4RegAoutSwitch);
    }   
    else
    {
        LOG(3, "Invalid eStreamFrom in AUD_DspDvdEnable\n");
    }
}

// *********************************************************************
// Function : void AUD_DspDvdDisable(void)
// Description : turn off dvd gpio
// Parameter :  none
// Return    :  None.
// *********************************************************************
void AUD_DspDvdDisable(void)
{
    UINT32 u4DvdGpio;
    AUD_INPUT_ID_T u4DvdInputChl;
    AUD_DVD_GPIO_POLARITY_T eDvdPolariy;
    INT32 i4RegAoutSwitch;

    //Get DVD input source and DVD gpio Num
    u4DvdGpio = DRVCUST_OptGet(eAudioDvdGpio);
    u4DvdInputChl = (AUD_INPUT_ID_T)DRVCUST_OptGet(eAudioDvdInputChl);

    eDvdPolariy = (AUD_DVD_GPIO_POLARITY_T)DRVCUST_OptGet(eAudioDvdPolarity);

    LOG(8, "AUD_DspDvdDisable Configuration -------\n");
    LOG(8, "DVD Gpio : %d\n", u4DvdGpio);
    LOG(8, "Dvd Input Channel : %d\n", u4DvdInputChl);
    LOG(8, "DVD Polarity : %d\n", eDvdPolariy);
    LOG(8, "Decoder Stream Source : %d\n", _aeStrSrcConf[AUD_DEC_MAIN]);
    LOG(8, "_eInputIdToDvdGpioConfig = %d\n", _eInputIdToDvdGpioConfig);
    if ((u4DvdGpio != 0xffffffff) && (_eInputIdToDvdGpioConfig == u4DvdInputChl) 
        && (_aeStrSrcConf[AUD_DEC_MAIN] == AUD_STREAM_FROM_LINE_IN) )
    {
        if (eDvdPolariy == AUD_DVD_GPIO_HIGH_ENALBE)
        {
            i4RegAoutSwitch=0;      
        }
        else
        {
            i4RegAoutSwitch=1;
        }
        LOG(8, "Aout Switch : %d\n", i4RegAoutSwitch);
        AUD_GPIOOutput(u4DvdGpio, i4RegAoutSwitch);
    }   
    else
    {
        LOG(3, "Input source is not DVD stream\n");
    }
}

AUD_INPUT_ID_T AUD_GetAvInputSrcId(void)
{
    return _eInputIdToDvdGpioConfig;
}

AUD_DEC_STREAM_FROM_T AUD_GetStrSrc(UINT8 u1DedId)
{
    return _aeStrSrcConf[u1DedId];
}   

BOOL AUD_GetDualDecMode(void)
{    
    return  _fgDualDecMode;
}

#ifdef SPDIF_MUTE_BY_PINMUX
UINT8 bSpdifFunc = 1; // 1: SPDIF 2: HDMI_BYPASS
#endif

void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable)
{
    if (_bSPDIFBypass == FALSE)
    {
        if (fgEnable)
        {
            AUD_SET_BIT(REG_SPOCFG0, HDMI_RX_SPDIF);
          #if defined(CC_MT5396)
            #ifdef SPDIF_MUTE_BY_PINMUX
            if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0) != 0)
            {
                vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF0);    
            }
            bSpdifFunc = 2;
            #else
            vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF0);
            vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF1);            
            #endif
          #elif defined(CC_MT5368)
            #ifdef SPDIF_MUTE_BY_PINMUX
            if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0) != 0)
            {
                vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF0);    
            }            
            bSpdifFunc = 2;
            #else
            vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF0);
            vIO32WriteFldAlign(CKGEN_PMUX4, 2, FLD_PAD_ASPDIF1);
            #endif
          #elif defined(CC_MT5389)
            #ifdef SPDIF_MUTE_BY_PINMUX
			    #ifdef CC_HDMI_ARC_SPDIF_CTRL_89
                if (IO32ReadFldAlign(PDWNC_PINMUX0, FLD_PAD1_ASPDIF0) != 0)
                {
                    vIO32WriteFldAlign(PDWNC_PINMUX0, 3, PDWNC_PINMUX0);    
                }            
                bSpdifFunc = 2;
			    #else
				if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0) != 0)
				{
					vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF0);	
				}			 
				bSpdifFunc = 2;
				#endif
            #else
                #ifdef CC_HDMI_ARC_SPDIF_CTRL_89
			    vIO32WriteFldAlign(PDWNC_PINMUX0, 3, FLD_PAD1_ASPDIF0);
                #else
			    vIO32WriteFldAlign(CKGEN_PMUX0, 2, FLD_PAD_ASPDIF0);
                #endif
            #endif          
          #elif defined(CC_MT5395)
            vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF);    
          #elif defined(CC_MT5365)
            vIO32WriteFldAlign(CKGEN_PMUX1, 0, FLD_PAD_ASPDIF);            
          #endif

            LOG(5, "SPDIF output source from HDMI Rx\n");
        }
        else
        {
            AUD_CLR_BIT(REG_SPOCFG0, HDMI_RX_SPDIF);
          #if defined(CC_MT5396)
            #ifdef SPDIF_MUTE_BY_PINMUX
            if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0) != 0)
            {
                vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);
            }            
            bSpdifFunc = 1;
            #else
            vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);            
            vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF1);
            #endif
          #elif defined(CC_MT5368)
            #ifdef SPDIF_MUTE_BY_PINMUX
            if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0) != 0)
            {
                vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);    
            }            
            bSpdifFunc = 1;
            #else
            vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);            
            vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_ASPDIF1);
            #endif
          #elif defined(CC_MT5389)
            #ifdef SPDIF_MUTE_BY_PINMUX
	            #ifdef CC_HDMI_ARC_SPDIF_CTRL_89
			    if (IO32ReadFldAlign(PDWNC_PINMUX0, FLD_PAD1_ASPDIF0) != 0)
			    {
				  vIO32WriteFldAlign(PDWNC_PINMUX0, 2, PDWNC_PINMUX0);	  
			    } 		   
			    bSpdifFunc = 1;
	           #else
			    if (IO32ReadFldAlign(CKGEN_PMUX0, FLD_PAD_ASPDIF0) != 0)
			    {
				  vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);	  
			    } 		   
			    bSpdifFunc = 1;
	           #endif
           #else
	           #ifdef CC_HDMI_ARC_SPDIF_CTRL_89
			   vIO32WriteFldAlign(PDWNC_PINMUX0, 2, FLD_PAD1_ASPDIF0);
	           #else
			   vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF0);
	           #endif
          #endif			

          #elif defined(CC_MT5395)            
            vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF);
          #elif defined(CC_MT5365)
            vIO32WriteFldAlign(CKGEN_PMUX0, 1, FLD_PAD_ASPDIF);
          #endif

            LOG(5, "SPDIF output source from audio module\n");
        }
    }
}

void AUD_SPDIF_Output_HDMI_Rx_Always(BOOL fgEnable)
{
    if (fgEnable == TRUE)
    {
        AUD_SET_BIT(REG_SPOCFG0, HDMI_RX_SPDIF);
        _bSPDIFBypass = TRUE;
    }
    else
    {
        _bSPDIFBypass = FALSE;
    }
}

#ifdef CC_AUD_HDMI_SPDIF_CFG
void AUD_SPDIF_Output_Mute(BOOL fgMute)
{
    if (_bSPDIFBypass == FALSE)
    {
        if (fgMute)
        {
            #ifdef SPDIF_MUTE_BY_PINMUX
            #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
			vIO32WriteFldAlign(PDWNC_PINMUX0, 0, FLD_PAD1_ASPDIF0);
#else
			vIO32WriteFldAlign(CKGEN_PMUX0, 0, FLD_PAD_ASPDIF0);
#endif
            #endif
            #else
            AUD_SET_BIT(REG_SPOCFG0, SPOUT_MUTE);
            #endif
            LOG(6, "SPDIF output Mute\n");
        }
        else
        {
            #ifdef SPDIF_MUTE_BY_PINMUX
            #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#ifdef CC_HDMI_ARC_SPDIF_CTRL_89
			vIO32WriteFldAlign(PDWNC_PINMUX0, 2, FLD_PAD1_ASPDIF0);
#else
			vIO32WriteFldAlign(CKGEN_PMUX0, bSpdifFunc, FLD_PAD_ASPDIF0);
#endif

            #endif
            #else        
            AUD_CLR_BIT(REG_SPOCFG0, SPOUT_MUTE);
            #endif
            LOG(6, "SPDIF output Un-Mute\n");
        }
    }
}
#endif

UINT32 AUD_DspGetPc(void)
{
    return AUD_READ32(REG_DSP_PC);
}

#if 1 // PROBE_RELATED_API
static void AUD_DspProbeEnable(void)
{
    if(!(AUD_READ32(REG_DSP_CTRL_R) & 0x80))
    {
        AUD_WRITE32(REG_DSP_CTRL_R,0x80|AUD_READ32(REG_DSP_CTRL_R));
    }
}

UINT32 AUD_DspProbePause(void)
{
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_CTRL,0x100);
    return 0;
}

UINT32 AUD_DspProbeGo(void)
{
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_CTRL,0x000);
    return 0;
}

UINT32 AUD_DspProbeStep(UINT32 u4Count)
{
    UINT32 u4Data;
    u4Data=u4Count<<8;     
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_CTRL,u4Data);
    return 0;
}

UINT32 fgAUD_DspProbeHalt(void)
{
    AUD_DspProbeEnable();
    return (AUD_READ32(REG_DSP_PB_CTRL)&0x1);
}

UINT32 u4AUD_DspProbeRead(UINT32 u4Addr)
{
    UINT16 u2Addr;
    u2Addr = u4Addr & 0xFFFF; 
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_INDEX,u2Addr);
    return (AUD_READ32(REG_DSP_PB_DATA));
}

UINT32 AUD_DspProbeWrite(UINT32 u4Addr, UINT32 u4Data)
{
    UINT16 u2Addr;
    u2Addr = u4Addr & 0xFFFF;
    AUD_DspProbeEnable();
    AUD_WRITE32(REG_DSP_PB_WRDX,u2Addr);
#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
    AUD_WRITE32(REG_DSP_PB_WR,u4Data);
#else
    AUD_WRITE32(REG_DSP_PB_WR,(u4Data & 0xFFFFFF));
#endif
    return 0;
}

UINT32 u4AUD_DspProbeStopOnWrite(UINT32 u4Addr, UINT32 u4Data, UINT32 u4Mask)
{
    UINT32 u4MaskD;
    UINT32 u4Temp;

    UNUSED(u4Addr);
    UNUSED(u4Temp);
        
    u4Addr = u4Addr & 0xFFFF;

    if (u4Addr != 0)
    {
        u4MaskD = 0x1FFFF;
        AUD_DspProbeEnable();
        AUD_WRITE32 (REG_DSP_PB_HIDX, 0x084);
        AUD_WRITE32 (REG_DSP_PB_CMSK, u4MaskD);
        AUD_WRITE32 (REG_DSP_PB_CTAR, (0x010000|u4Addr));
        AUD_WRITE32 (REG_DSP_PB_WMSK, u4Mask);
        AUD_WRITE32 (REG_DSP_PB_WTAR, u4Data);
        AUD_WRITE32 (REG_DSP_PB_TAR, 0x0100);
    }
    else
    {
        u4Temp = AUD_READ32(REG_DSP_PB_TAR);
        AUD_WRITE32 (REG_DSP_PB_TAR, (u4Temp & 0xfffffeff));
    }
        
    return 0;
}


UINT32 u4AUD_DspProbeChkStopOnWrite(BOOL *fgEnable, UINT32 *u4Data, UINT32 *u4Mask)
{
    UINT32 u4Addr;
    
    *fgEnable = (AUD_READ32(REG_DSP_PB_TAR)&(0x100)) >> 8;
    u4Addr = AUD_READ32(REG_DSP_PB_CTAR)& (0xFFFF);
    *u4Data = AUD_READ32(REG_DSP_PB_WTAR);
    *u4Mask = AUD_READ32(REG_DSP_PB_WMSK);
    return u4Addr;
    
}

UINT32 u4AUD_DspProbePBCfg(BOOL fgSet,UINT32 u4Value)
{
    AUD_DspProbeEnable();
    if(fgSet)
    {
        AUD_WRITE32(REG_DSP_PB_STOP,u4Value);
        return 0;
    }
    return (AUD_READ32(REG_DSP_PB_STOP));
}

UINT32 u4AUD_DspProbePB(BOOL fgSet,UINT32 u4BPNum, UINT32 u4Addr)
{
    AUD_DspProbeEnable();
    if(fgSet)
    {
        if(u4BPNum<4)
        {
            AUD_WRITE32((REG_DSP_PB_STP0+(u4BPNum<<2)),u4Addr);
            return 0;
        }
        return 0;
    }
    if(u4BPNum<4)
    {
        return(AUD_READ32((REG_DSP_PB_STP0+(u4BPNum<<2))));
    }
    return 0xFFFFFFFF;   
}
#endif

UINT32 AUD_AudioDescriptionChlSel(UINT8 u1DecId, AUD_CHL_DEF_T eChl)
{
    AUD_DEC_ID_VALIDATE(u1DecId);    
    _MT5365_DacChlSel(u1DecId, eChl);
    return 0;
}

static void AUD_LineinLRDetect(BOOL fgEnable)
{
    UNUSED(AUD_LineinLRDetect);
    
    if (fgEnable)
    {
        AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) | (LR_DETECT_ON)));
    }
    else
    {
        AUD_WRITE32(REG_LIN_CFG0, (AUD_READ32(REG_LIN_CFG0) & (~LR_DETECT_ON)));        
    }
}

//-----------------------------------------------------------------------------
/** AUD_GPIOOutput
 *
 *  @param  u4GPIONum       GPIO number
 *  @param  i4polarity      polarity
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_GPIOOutput(UINT32 u4GPIONum, INT32 i4Polarity)
{
    UNUSED(GPIO_Output((INT32)u4GPIONum, (INT32 *)&i4Polarity));
}

//-----------------------------------------------------------------------------
/** AUD_GPIOEnable
 *
 *  @param  u4GPIONum       GPIO number
 *  @param  i4polarity      polarity
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_GPIOEnable(UINT32 u4GPIONum, INT32 i4Polarity)
{
    UNUSED(GPIO_Enable((INT32)u4GPIONum, (INT32 *)&i4Polarity));
}

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT
UINT32 AUD_GetLineinWP(void)
{
    AUD_WRITE32(REG_ABUF_PNT, LINE_IN_SEL_WP);
    return DSP_PHYSICAL_ADDR(AUD_READ32(REG_ABUF_PNT));
}
#endif

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
/*  AOUT HW Gain control  since MT5368
*  @param.  u1Decid (MAIN or AUX)
*  @param.  CHID (All channel or L. to CH10)
*  @param.  Gain..( 0x1000 --0 db, 0x0 -- mute)
*/
void AudAoutGainCtrl(UINT8 u1DecId, UINT8 CHID, UINT16 Gain)
{
   if( u1DecId == AUD_DEC_MAIN) //AOUT
   {
     //enable risc control aout gain
     AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))&~AOUT_VOL_CTRL_BY_DSP); 
     //enable aout gain control  
     AUD_WRITE32(REG_AOUT_VOL_GAIN_SETUP, (AUD_READ32(REG_AOUT_VOL_GAIN_SETUP))&~AOUT_GAIN_BYPASS);
   
     //init gain step
     AUD_WRITE32(REG_AOUT_VOL_GAIN_SETUP, ((AUD_READ32(REG_AOUT_VOL_GAIN_SETUP))&~AOUT_VOL_STEP_MARK)|AOUT_VOL_SETP_4DB);

     //setup  target aout channel
     AUD_WRITE32(REG_AOUT_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT_VOL_GAIN_CTRL)&~CH_GAIN_SEL_MARK)|CHID<<16));

     //setup target gain

     Gain &= 0xFFFF;
   
     AUD_WRITE32(REG_AOUT_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT_VOL_GAIN_CTRL)&~AOUT_VOL_GAIN_MARK)| Gain));

     //swith  the hw control to DSP 
     //AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))|AOUT_VOL_CTRL_BY_DSP); 

   }
   else  //AOUT2
   {     
     //enable risc control aout2 gain
     AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))&~AOUT2_VOL_CTRL_BY_DSP); 
     //enable aout2 gain control  
     AUD_WRITE32(REG_AOUT2_VOL_GAIN_SETUP, (AUD_READ32(REG_AOUT2_VOL_GAIN_SETUP))&~AOUT2_GAIN_BYPASS);
   
     //init gain step
     AUD_WRITE32(REG_AOUT2_VOL_GAIN_SETUP, ((AUD_READ32(REG_AOUT2_VOL_GAIN_SETUP))&~AOUT2_VOL_STEP_MARK)|AOUT_VOL_SETP_4DB);

     //setup  target aout2 channel
     AUD_WRITE32(REG_AOUT2_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT2_VOL_GAIN_CTRL)&~CH_GAIN_SEL_MARK)|CHID<<16));

     //setup target gain

     Gain &= 0xFFFF;
   
     AUD_WRITE32(REG_AOUT2_VOL_GAIN_CTRL , ( (AUD_READ32(REG_AOUT2_VOL_GAIN_CTRL)&~AOUT_VOL_GAIN_MARK)| Gain));

	 //swith  the hw control to DSP 
     //AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))|AOUT2_VOL_CTRL_BY_DSP); 
    
   }

}

void AudAoutGainHwInit(void)
{
     UNUSED(AudAoutGainHwInit);  //UNUSED doesn't take care static function so we remove static.
     //enable aout gain control  
     AUD_WRITE32(REG_AOUT_VOL_GAIN_SETUP, (AUD_READ32(REG_AOUT_VOL_GAIN_SETUP))&~AOUT_GAIN_BYPASS);
     //init gain step
     AUD_WRITE32(REG_AOUT_VOL_GAIN_SETUP, ((AUD_READ32(REG_AOUT_VOL_GAIN_SETUP))&~AOUT_VOL_STEP_MARK)|AOUT_VOL_SETP_4DB);

	 //enable aout2 gain control  
     AUD_WRITE32(REG_AOUT2_VOL_GAIN_SETUP, (AUD_READ32(REG_AOUT2_VOL_GAIN_SETUP))&~AOUT2_GAIN_BYPASS);
     //init gain step
     AUD_WRITE32(REG_AOUT2_VOL_GAIN_SETUP, ((AUD_READ32(REG_AOUT2_VOL_GAIN_SETUP))&~AOUT2_VOL_STEP_MARK)|AOUT_VOL_SETP_4DB);

	 
     //enable risc control AOUT1 gain
     //AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))|AOUT_VOL_CTRL_BY_DSP); 
	 //AOUT2
	 //enable risc control aout2 gain
     //AUD_WRITE32(REG_AOUT_DRAM_CFG, (AUD_READ32(REG_AOUT_DRAM_CFG))|AOUT2_VOL_CTRL_BY_DSP); 

	 
}

void AudSPDIFInBypass(BOOL fgBypass)
{
    if(fgBypass)    
    {
        AUD_SET_BIT(REG_SPOCFG0,BYP_SPDIFIN);
    }
    else
    {
        AUD_CLR_BIT(REG_SPOCFG0,BYP_SPDIFIN);
    }
}


#endif
