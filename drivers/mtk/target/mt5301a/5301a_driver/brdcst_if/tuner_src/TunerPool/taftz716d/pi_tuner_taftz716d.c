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
 * $RCSfile: pi_tuner_taftz716d.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_taftz716d.c
 *  Tuner control for SS taftz716d
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "tuner_interface_if.h"
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"
#include "pi_def_dvbt.h" 
#include "pi_dvbt_if.h"  
//#include "pd_tuner_type.h"

#define C_TAFTZ716D_VERSION				"LG TAFTZ716D v1.00"

#define C_TAFTZ716D_I2C_DIVIDER       ((U16) 270)

#define C_TAFTZ716D_TOP_SET               ((U8)   0x0a)   
#define C_TAFTZ716D_SAW_BW                SAW_BW_8M 

#define C_TAFTZ716D_FREQ_DBOUND_UPPER   858*1000*1000
#define C_TAFTZ716D_FREQ_DBOUND_LOWER    48*1000*1000
#define C_TAFTZ716D_FREQ_DBOUND_UPPER_Ana   867*1000*1000
#define C_TAFTZ716D_FREQ_DBOUND_LOWER_Ana    45*1000*1000

#define C_TAFTZ716D_LO_ADDRRESS           ((U16)  0xC0)    // According to the customer board change
#define C_TAFTZ716D_IF_ADDRRESS            ((U16)  0x86)
#define C_TAFTZ716D_IF_FREQUENCY          ((U16)  36167)  /* kHz */
#define C_TAFTZ716D_IF_FREQUENCY_ANA      ((U16)  38900)  /* kHz */

#define C_TAFTZ716D_LO_DIVIDER_STEP       ((U32)  50000) /* Hz */
#define C_TAFTZ716D_LO_DIVIDER_STEP_ANA   ((U16)  50000) /* Hz */

#define C_TAFTZ716D_CHANNEL_SCAN_JUMP_SMALL_STEP ((U16) 250)
#define C_TAFTZ716D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP ((U16) 800)
#define C_TAFTZ716D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN ((U16) 250)

#define C_TAFTZ716D_AGC_IF_SLP             1
#define C_TAFTZ716D_AGC_IF_INT             0
#define C_TAFTZ716D_AGC_IF_MAX             0.499
#define C_TAFTZ716D_AGC_IF_MIN            -0.5
#define C_TAFTZ716D_AGC_IF_SLP_SGN  (S8)((C_TAFTZ716D_AGC_IF_SLP>0)?1:-1) 
#define C_TAFTZ716D_AGC_IF_LVL_MAX      0.499
#define C_TAFTZ716D_AGC_IF_LVL_MIN     -0.5


#define C_TAFTZ716D_MAX_IF_GAIN_SET       -0.12   
#define C_TAFTZ716D_MIN_IF_GAIN_SET       -0.38   
#define C_TAFTZ716D_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TAFTZ716D_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_TAFTZ716D_MAX_IF_GAIN_POW       -100    // dBm
#define C_TAFTZ716D_MIN_IF_GAIN_POW		-50     // dBm
#define C_TAFTZ716D_MAX_RF_GAIN_POW		 C_TAFTZ716D_MIN_IF_GAIN_POW
#define C_TAFTZ716D_MIN_RF_GAIN_POW		-20     // dBm
#define C_TAFTZ716D_POWER_CALIBRATE        26     // dBm
#define C_TAFTZ716D_SIGNAL_LVL_TH             4

#define C_TAFTZ716D_AGC_COEF				((U8)   0xBD)

#define C_TAFTZ716D_DEMOD_INPUT_POWER		0

// ********************************************* //

#define C_U8_TAFTZ716D_AGC_IF_SLP         (S8)(C_TAFTZ716D_AGC_IF_SLP *32 )
#define C_U8_TAFTZ716D_AGC_IF_INT         (S8)(C_TAFTZ716D_AGC_IF_INT *64 )
#define C_U8_TAFTZ716D_AGC_IF_MAX         (S8)(C_TAFTZ716D_AGC_IF_MAX *256)
#define C_U8_TAFTZ716D_AGC_IF_MIN         (S8)(C_TAFTZ716D_AGC_IF_MIN *256)
#define C_U8_TAFTZ716D_AGC_IF_LVL_MAX (S8)(C_TAFTZ716D_AGC_IF_LVL_MAX *256)
#define C_U8_TAFTZ716D_AGC_IF_LVL_MIN  (S8)(C_TAFTZ716D_AGC_IF_LVL_MIN *256)

#define C_U8_TAFTZ716D_MAX_IF_GAIN_SET	(S8)(C_TAFTZ716D_MAX_IF_GAIN_SET *256)
#define C_U8_TAFTZ716D_MIN_IF_GAIN_SET	(S8)(C_TAFTZ716D_MIN_IF_GAIN_SET *256)
#define C_U8_TAFTZ716D_MAX_RF_GAIN_SET	(S8)((C_TAFTZ716D_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TAFTZ716D_MIN_RF_GAIN_SET	(S8)((C_TAFTZ716D_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_TAFTZ716D_IF_GAIN_RANGE      (S8)(C_TAFTZ716D_MIN_IF_GAIN_POW - C_TAFTZ716D_MAX_IF_GAIN_POW)
#define C_U8_TAFTZ716D_RF_GAIN_RANGE      (S8)(C_TAFTZ716D_MIN_RF_GAIN_POW - C_TAFTZ716D_MAX_RF_GAIN_POW)

/*************************************
variable
*************************************/

/*************************************
functions
*************************************/

VOID TAFTZ716D_TunerInit( ITUNER_CTX_T *pTunerCtx );
INT16 TAFTZ716D_TunerSetFreq(    ITUNER_CTX_T *pTunerCtx,  PARAM_SETFREQ_T* param );
STATIC CHAR *TAFTZ716D_TunerGetVer(VOID);
STATIC INT8  TAFTZ716D_SetIFDemod( ITUNER_CTX_T * pTunerCtx,U8 SubSysIdx);


VOID TAFTZ716D_TunerInit( ITUNER_CTX_T *pTunerCtx )
{
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTunerCtx->specific_member.eu_ctx);
	   
    pTunerCtx->I2cAddress = C_TAFTZ716D_LO_ADDRRESS;
    pTunerCtx->u2IF_Freq= C_TAFTZ716D_IF_FREQUENCY;
    pEUCtx->m_SAW_BW  = C_TAFTZ716D_SAW_BW;

    pEUCtx->m_s4FreqBoundUpper = C_TAFTZ716D_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_TAFTZ716D_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_TAFTZ716D_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_TAFTZ716D_FREQ_DBOUND_LOWER_Ana;

    pEUCtx->m_Small_Step = C_TAFTZ716D_CHANNEL_SCAN_JUMP_SMALL_STEP;//set small step as 250k
    pEUCtx->m_Middle_Step_Up = C_TAFTZ716D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_UP;
    pEUCtx->m_Middle_Step_Down = C_TAFTZ716D_CHANNEL_SCAN_JUMP_MIDDLE_STEP_DOWN;
    
    // for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (U8)C_U8_TAFTZ716D_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (U8)C_U8_TAFTZ716D_AGC_IF_LVL_MIN;
	
    pEUCtx->m_aucPara[0] = C_TAFTZ716D_TOP_SET;
    pEUCtx->m_aucPara[1] = 0;
    pEUCtx->m_aucPara[2] = (U8)C_U8_TAFTZ716D_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (U8)C_U8_TAFTZ716D_AGC_IF_INT;
    pEUCtx->m_aucPara[4] = (U8)C_U8_TAFTZ716D_AGC_IF_MAX;
    pEUCtx->m_aucPara[5] = (U8)C_U8_TAFTZ716D_AGC_IF_MIN;
    pEUCtx->m_aucPara[6] = C_TAFTZ716D_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_TAFTZ716D_DEMOD_INPUT_POWER;
    pEUCtx->m_aucPara[8] = (U8)C_U8_TAFTZ716D_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[9] = (U8)C_U8_TAFTZ716D_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (U8)C_U8_TAFTZ716D_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (U8)C_U8_TAFTZ716D_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (U8)C_U8_TAFTZ716D_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (U8)C_U8_TAFTZ716D_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_TAFTZ716D_POWER_CALIBRATE;
    pEUCtx->m_aucPara[21] = 0x07;

    pEUCtx->m_SigLvTh = C_TAFTZ716D_SIGNAL_LVL_TH;
    pEUCtx->m_SigLvScan = 0;
    
    mcSHOW_USER_MSG(("Init " C_TAFTZ716D_VERSION "\n"));
}


//----------------------------------------------------------------------------- 
/** TAFTZ716D_TunerSetFreq
 *  Set Tuner PLL for TAFTZ716D to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param 	param    Pointer of structure of parameters
 *  @retval      0  success
                     1   fail   out of range 
                   -1   fail  I2C error 
 */
//-----------------------------------------------------------------------------
INT16 TAFTZ716D_TunerSetFreq(
    ITUNER_CTX_T *pTunerCtx,
    PARAM_SETFREQ_T* param
    )
{
    UCHAR uc_data;
    S16 ucTunerLockConfCnt;
    UCHAR TunerS[5];
    U32 Lo,Freq;
    U16 Ndivider, CB1, CB2, BB; 
    S16 ii, jj;

    SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);
       
    pTunerCtx->u4RF_Freq = param->Freq;
    Freq = param->Freq;  
    pTunerCtx->I2cAddress = C_TAFTZ716D_LO_ADDRRESS;

    Lo = Freq + C_TAFTZ716D_IF_FREQUENCY_ANA; // kHz
    Ndivider = (U16) ( (Lo*1000+C_TAFTZ716D_LO_DIVIDER_STEP_ANA/2) / C_TAFTZ716D_LO_DIVIDER_STEP_ANA);
    mcSHOW_DBG_MSG(("FreqSet: Freq = %d, Divider ratio = %04X, PLL set freq=%d kHz\n", pTunerCtx->u4RF_Freq, Ndivider,(Ndivider*C_TAFTZ716D_LO_DIVIDER_STEP_ANA/1000-C_TAFTZ716D_IF_FREQUENCY_ANA)));
     	    
    // Conver Freq to unit of MHz
    Freq = Freq/1000;
	    
    if (Freq < (C_TAFTZ716D_FREQ_DBOUND_LOWER_Ana/1000/1000) || 
            Freq > (C_TAFTZ716D_FREQ_DBOUND_UPPER_Ana/1000/1000)  )
    {
        mcSHOW_DRVAPI_MSG(("Out of range for LO!\n"));
        return (+1);
    }

    CB1 = 0xBD;   
    CB2 = 0xC2;
	    
    if (Freq <= 165)       
    {
        BB = 0x01;
    }
    else if (Freq <= 449)  
    {        
        BB = 0x02;
    }
    else                 
    {
        BB = 0x08;
    }  
	             
    // LO settings
    TunerS[0] = (UCHAR) 0x7F & (Ndivider >> 8);
    TunerS[1] = (UCHAR) 0xFF & Ndivider;
    TunerS[2] = (UCHAR) CB1;  
    TunerS[3] = (UCHAR) BB;
    TunerS[4] = (UCHAR) CB2; 

    for (jj=0; jj<2; jj++)
    {
        //  send this data 
        if(ICtrlBus_I2cTunerWrite(C_TAFTZ716D_I2C_DIVIDER, pTunerCtx->I2cAddress, TunerS, 5)>0)  
        {
            mcSHOW_DBG_MSG(("TunerWrite PLL failed!\n"));
            return (-1);
        }
	
        ucTunerLockConfCnt = 0;
        for (ii=0; ii<20; ii++)
        {
            mcDELAY(5);	
            ICtrlBus_I2cTunerRead(C_TAFTZ716D_I2C_DIVIDER, pTunerCtx->I2cAddress, &uc_data, 1);
            if (mcGET_FIELD(uc_data, 0x40, 6))  // if PLL is locked
                ucTunerLockConfCnt++;
            else
                ucTunerLockConfCnt = 0;	                
	                
            if (ucTunerLockConfCnt>ccTUNER_LOCK_CONF_THRESHOLD)
                break;							
        }
        ucTunerLockConfCnt = ii;

        if(ii < 20)
        {
            mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL success, cnt = %d\n" , Freq, jj+1, ucTunerLockConfCnt));	
            break;
        }
        else
        {
            mcSHOW_DRVAPI_MSG(("Freq = %d MHz, the %d-th time setting PLL fail\n" , Freq, jj+1));				
        }
    }
               
    mcDELAY_MS(10);   
    //Record the latest TunerS code.
    pEUCtx->m_aucPara[15] = TunerS[0];
    pEUCtx->m_aucPara[16] = TunerS[1];
    pEUCtx->m_aucPara[17] = TunerS[2];
    pEUCtx->m_aucPara[18] = TunerS[3];
    pEUCtx->m_aucPara[19] = TunerS[4];
	    
    return (0);
}

//----------------------------------------------------------------------------- 
/** TAFTZ716D_TunerGetVer
 *  Set Tuner PLL staus for TAFTZ716D
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
STATIC CHAR *TAFTZ716D_TunerGetVer(VOID)
{
    return (C_TAFTZ716D_VERSION);
}

//----------------------------------------------------------------------------- 
/** TAFTZ716D_SetIFDemod
 *  TAFTZ716D_SetIFDemod.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  param    Pointer of structure of parameters
 *  @retval   0   success
 *                1   fail   out of range 
 *              -1  fail  I2C error 
 */
//-----------------------------------------------------------------------------
STATIC INT8   TAFTZ716D_SetIFDemod( ITUNER_CTX_T * pTunerCtx,U8 SubSysIdx)
{
       UCHAR TunerS[7],B,C,E,F,G,H,J,K,L,Data[9];
	U8 SubSysMod;
	
	TunerS[0] = 0x86;	// IF IC slave address
	TunerS[1] = 0x00;	// IF IC sub address
	SubSysMod = SubSysIdx;

	mcSHOW_DBG_MSG(("%s: SubSysIdx =%d\n", __FUNCTION__, SubSysMod));
	SubSysMod += MOD_ANA_TYPE_BEGIN;
		
	switch (SubSysMod)
	{
	case MOD_PAL_BG:
              B = 0x21; 
		C = 0x02;
		E = 0x44; 
		F = 0x21;
		G = 0x84;
		H = 0x00;
		J = 0x20;	
		K = 0x30; 
		L = 0x02;
		mcSHOW_DBG_MSG(("Set System PAL BG\n"));
		break;
	case MOD_PAL_I:
		B = 0x21; 
		C = 0x02;
		E = 0x44; 
		F = 0x21;
		G = 0x8C;
		H = 0x00;
		J = 0x20;
		K = 0x30;  
		L = 0x02; 
		mcSHOW_DBG_MSG(("Set System PAL I\n"));
		break;
	case MOD_PAL_DK:
		B = 0x21; 
		C = 0x02;
		E = 0x44; 
		F = 0x21;
		G = 0x94;
		H = 0x00;
		J = 0x20;
		K = 0x30; 
		L = 0x02; 
		mcSHOW_DBG_MSG(("Set System PAL DK\n"));
		break;	
        case MOD_NTSC_M:
              B = 0x21; 
              C = 0x1A;
              E = 0x44; 
              F = 0x21;
              G = 0x9C;
              H = 0x00;
              J = 0x20;
              K = 0x30; 
              L = 0x02; 
              mcSHOW_DBG_MSG(("Set System NTSC M\n"));
              break;
	default:
		B = 0x21; 
		C = 0x02;
		E = 0x44; 
		F = 0x21;
		G = 0x94;
		H = 0x00;
		J = 0x20;
		K = 0x30; 
		L = 0x02;  
		mcSHOW_DBG_MSG(("Set System Default : PAL DK\n"));
		break;
	}

	Data[0] = B;
	Data[1] = C;
	Data[2] = E;
	Data[3] = F;
	Data[4] = G;
	Data[5] = H;
	Data[6] = J;
	Data[7] = K;
	Data[8] = L;

	
	//apply new I2C mode:increastial mode.
      TunerS[1] =0x80;
      TunerS[2] = Data[0];
      TunerS[3] = Data[1];
      TunerS[4] = Data[2];
      TunerS[5] = Data[3];
      TunerS[6] = Data[4];

       mcSHOW_DRVAPI_MSG(("the first time  IFDEMOD RegSetRegistersTuner.\n"));
	   
       if(ICtrlBus_I2cTunerWrite(C_TAFTZ716D_I2C_DIVIDER, TunerS[0], &TunerS[1] , 6)>0)
       {
           mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
           return (-1);
       }
       mcDELAY(2);	
			
      TunerS[1] =0x85;
      TunerS[2] = Data[5];
      TunerS[3] = Data[6];
      TunerS[4] = Data[7];
      TunerS[5] = Data[8];

      mcSHOW_DRVAPI_MSG(("the second time IFDEMOD RegSetRegistersTuner.\n"));

      if(ICtrlBus_I2cTunerWrite(C_TAFTZ716D_I2C_DIVIDER, TunerS[0], &TunerS[1] , 5)>0)
      {
          mcSHOW_USER_MSG(("\nTunerSet() failed!\n"));
          return (-1);
      }	
      mcDELAY(10);

	return (0);
}

//----------------------------------------------------------------------------- 
/** TAFTZ716D_SetSawBw
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
STATIC VOID   TAFTZ716D_SetSawBw( ITUNER_CTX_T * pTunerCtx,UINT8 sawbw)
{
   pTunerCtx->specific_member.eu_ctx.m_SAW_BW = sawbw ;
}

//----------------------------------------------------------------------------- 
/** TAFTZ716D_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 TAFTZ716D_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal)
{         
    switch (eOperation)
    {
    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
            *RetStr=TAFTZ716D_TunerGetVer();
        }
        break;
    case itSetIfDemod:
        {
            INT8 * Retval = (INT8 *)pInOutVal;	
            * Retval = TAFTZ716D_SetIFDemod(pTCtx, SetVal); 
        }
        break;	
    case itSetSawBw:
        TAFTZ716D_SetSawBw(pTCtx, SetVal);
        break;
    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
