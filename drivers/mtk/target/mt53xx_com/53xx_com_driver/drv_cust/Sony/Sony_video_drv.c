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
#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "pe_if.h"
#include "drv_nr.h"
#include "video_def.h"
#include "drv_video.h"
#include "api_backlt.h"
#include "drv_contrast.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_tvd.h"
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "x_debug.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "source_select.h"
#include "sony_drv_inc/a_bl_ctrl.h"
#include "sony_drv_inc/pq_gamma_if.h"
#include "sony_drv_inc/pq_ip_if.h"
#include "sony_drv_inc/rgb_if.h"
#include "sony_drv_inc/Sony_drv_cb.h"
#include "pd_atd_if.h"
#include "srm_drvif.h"
#include "drv_pq_cust.h"

static BOOL b_fginit = FALSE;    /* SONY, cheah 24/07/09, to ensure init before call ISR func */
static UINT8 gbSmartPicIndex = 0;
UINT8 SCEIndex = 0;
extern UINT8 bBackLightCtrl;




// Backlight PWM V-Sync.
///////////////////////////////////////////////////////
UINT16 ui2BacklightResult;
UINT32 DRVCUST_AutoBacklightISR(void);
UINT32 DRVCUST_AutoBacklightProc(void);
void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight);

/* BEGIN SONY, ramesh, 21/07/09 */

///////////////BEGIN SONY DRV CALLBACK/////////////////
extern UINT8 bSceneChange;
void cbSony_AutoBacklightGetDbcParam(void* pvArg)
{
    ST_PARAM_DBC_MAIN* param = (ST_PARAM_DBC_MAIN*)pvArg;

    param->aplm = bDrvGetAPL();
    param->scene_change_det = bSceneChange;
    bDrvGetNormHist32(param->wHist32);
    param->luma_max = bDrvGetLumaMax();
    vDrvGetChromaHist(param->cHist8);
}

void cbSony_AutoBacklightGetDbc4psEnable(void* pvArg)
{
    BOOL _fgNotFmRadio;
    BOOL _fgNotNoSignal;
    BOOL _fgNotPipPap;
    BOOL* dbc4psEnable = (BOOL*)pvArg;

    _fgNotFmRadio = 0;

    if ((SRM_GetTvMode() == SRM_TV_MODE_TYPE_PIP) ||
        (SRM_GetTvMode() == SRM_TV_MODE_TYPE_POP))
    {
      _fgNotPipPap = FALSE;
    }
    else
    {
      _fgNotPipPap = TRUE;
    }

    if ((bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8)SV_VDO_NOSIGNAL) &&
        (bDrvVideoSignalStatus(SV_VP_PIP) == (UINT8)SV_VDO_NOSIGNAL))
    {
      _fgNotNoSignal = FALSE;
    }
    else
    {
      _fgNotNoSignal = TRUE;
    }

    if(_fgNotFmRadio && _fgNotPipPap && _fgNotNoSignal)
    {
      *dbc4psEnable = TRUE;
    }
    else
    {
      *dbc4psEnable = FALSE;
    }
}

void cbSony_AutoBacklightGetDbcEnable(void* pvArg)
{
    BOOL* dbcEnable = (BOOL*)pvArg;

    *dbcEnable = TRUE;
}

void cbSony_AutoBacklightGetNoSigGain(void* pvArg)
{
    BOOL* noSigGain = (BOOL*)pvArg;

    *noSigGain = FALSE;
}

void sonyDrv_RegisterCallback(void)
{
  UINT8 i;
  SONY_DRV_CB_ST  callback_funcs;

  for(i=0; i < CB_DRV_FUNC_COUNT; i++)
  {
    callback_funcs.cbDrv[i] = NULL;
  }

  callback_funcs.cbDrv[CB_DRV_DBC_PARAM]         = cbSony_AutoBacklightGetDbcParam;
  callback_funcs.cbDrv[CB_DRV_DBC4PS_ENABLE]     = cbSony_AutoBacklightGetDbc4psEnable;
  callback_funcs.cbDrv[CB_DRV_DBC_ENABLE]        = cbSony_AutoBacklightGetDbcEnable;
  callback_funcs.cbDrv[CB_DRV_NO_SIG_GAIN]       = cbSony_AutoBacklightGetNoSigGain;

  sonyDrvRegisterCallback(&callback_funcs);
}

///////////////END SONY DRV CALLBACK///////////////////

/* BEGIN SONY, ramesh, 21/07/09 */
UINT32 DRVCUST_AutoBacklightInit(void)
{
    vAutoBacklightInit();

    return 0;
}

void vAutoBacklightPwmSwitch(Enum_PwmSwitch u1_switch)
{
    switch(u1_switch)
    {
      case MTK_PWM:
        /* Switch DEEP_DIMMER to MTK PWM */
        break;
      case EXT_PWM:
        /* Switch DEEP_DIMMER to External PWM */
        break;
      default:
        break;
    }
}
/* END SONY, ramesh, 21/07/09 */

UINT32 DRVCUST_AutoBacklightISR(void)
{
    // other ISR routine.
    vAutoBacklightISR();    /* SONY, zahrul 06/07/09 */

    return 0;
}

UINT32 DRVCUST_AutoBacklightProc(void)
{
    vAutoBacklightProc();   /* SONY, zahrul 06/07/09 */

    return 0;
}

void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight)
{
    /* BEGIN SONY, zahrul 06/07/09 */
    UINT8 ui1_Backlight;
    UINT8 ui1_ecoMode;
    UINT8 ui1_lightSensor;
    UINT8 ui1_advContEnhance;

    ui1_Backlight = aBLight->ui1BacklightLevel;
    ui1_ecoMode = aBLight->ui1PowerSavingMode;
    ui1_lightSensor = aBLight->ui1LightSensorOnOff;
    ui1_advContEnhance = aBLight->ui1AdvContrastEnhancerMode;

    vUserBacklightLvl(ui1_Backlight);
    vPwrSavingStatus(ui1_ecoMode);
    vLightSensorStatus(ui1_lightSensor);
    vAdvContrastEnhancerStatus(ui1_advContEnhance);
    /* END SONY, zahrul */
}
///////////////////////////////////////////////////////
INT8 Clarity_value=0;
void DRVCUST_ClarityUpdate(INT8 u1Value)
{
    Clarity_value = u1Value;  //Clarity UI range is -5 ~ +5
    // SONY implenment clarity.
//	DRVCUST_IP_Update(u1Value);	/* SONY, cheah 20/07/09 */
}


UINT8 bApiGetPanelIDFromEEPROM(void)
{
    //implement flash PQ ID for each panel
    return 0;
}

UINT8 vApiFlashPqGetGammaId(UINT8 bIndex)
{
	UNUSED(bIndex);

	return (0);
	//return (bApiGetPanelIDFromEEPROM());
}

// SCE.
///////////////////////////////////////////////////////
UINT8 vApiFlashPqGetSceId(UINT8 bIndex)
{
	UNUSED(bIndex);


	return (SCEIndex);
//	return (bApiGetPanelIDFromEEPROM()*12+SCEIndex);
}


UINT8 vApiFlashPqGetMaxSceId(UINT8 bIndex)
{
	UNUSED(bIndex);

    switch(SCEIndex)
    {
        case 1: //RF STD
            return 9;
        case 2: //RF CIN
            return 10;
        case 4: //SD STD
        case 7: //HD STD
            return 11;
        case 5: //SD CIN
        case 8: //HD CIN
            return 12;
    }

	return (SCEIndex);
//	return (bApiGetPanelIDFromEEPROM()*12+SCEIndex);
}

/* @ SCE set flash PQ Index
 *      1: off
 *      2: Low
 *      3: Medium
 *      4: High
 */
void vApiFlashPqSetSceId(UINT8 bIndex)
{
    UINT8 SrcIndex = 0;

	UNUSED(bIndex);

        if(bGetSignalType(SV_VP_MAIN)==SV_ST_TV)   // ATV
        {
           SrcIndex = 0;
        }
    else if(wDrvVideoInputHeight(SV_VP_MAIN) <= 720)   //SD
        {
            SrcIndex = 1;
        }
    else         //HD
        {
            SrcIndex = 2;
        }

    SCEIndex = SrcIndex*3 +
        MIN(vApiFlashPqGetSmartPicId(bIndex),2); // Currently,  Only support 3 Picture Mode

}
//Color Temp
///////////////////////////////////////////////////////

/* Sony customize function */
void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo)
{
	#ifdef CC_SONY_GAMMA
	DRVCUST_AutoWB_Update(aCTInfo);	/* SONY, cheah 14/07/09 */
	#endif /* CC_SONY_GAMMA */

	RGB_SetRgbUiData(aCTInfo->ui1AmbientSensorOnOff, aCTInfo->ui1CTIndex); /* BEGIN SONY, ramesh, 15/07/09 */
}
//Gamma.
///////////////////////////////////////////////////////

void DRVCUST_SET_GAMMA(void)
{
    EXTERN UINT8 GAMMA_256_MIDDLE[3][256];
    EXTERN void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff);

	#ifdef CC_SONY_GAMMA
	if (DRVCUST_VerifyPQNVM() == SV_TRUE)
	{
		DRVCUST_Gamma_Init();

		/* fetch gamma table from nvm */
		DRVCUST_GammaReadRGBNVM();
	}
	#endif /* CC_SONY_GAMMA */

	/* register event to load new gamma table */
	vApiRegisterVideoEvent(PE_ARG_GAMMA, SV_V_GAMMA_MIDDLE, SV_ON);
}

void vApiFlashPqSetGammaId(UINT8 bIndex)
{

}

///////////////////////////////////////////////////////

UINT8 vApiFlashPqGetQtyId(UINT8 bIndex)
{

    UNUSED(bIndex);

    return 0;
    //return (bApiGetPanelIDFromEEPROM());
}

void vApiFlashPqSetSmartPicId(UINT8 bIndex)
{
	gbSmartPicIndex = bIndex;
}

UINT8 vApiFlashPqGetSmartPicId(UINT8 bIndex)
{
	UNUSED(bIndex);

	return (gbSmartPicIndex);
}

// For Dynamic Back Light
/**
 * @brief Dynamic Back Light On/Off, call by UI
 * @param bOnOff SV_ON/SV_OFF
 * @warning related drivers are in drv_contrast.c
 */
void vApiVideoAdaptiveBackLightOnOff(BYTE bOnOff)
{
    bBackLightCtrl = bOnOff? SV_TRUE : SV_FALSE;
}

void DRVCUST_HwInit(void)
{
    return;
}

void DRVCUST_VideoInit(void)
{
    sonyDrv_RegisterCallback();  /* BEGIN SONY, ramesh, 24/07/09 */

    DRVCUST_AutoBacklightInit(); /* BEGIN SONY, ramesh, 07/07/09 */
	#ifdef CC_SONY_GAMMA
	DRVCUST_AutoWB_Init(); /* SONY, cheah 08/07/09 */
	#endif
	DRVCUST_IP_Init();		/* SONY, cheah 20/07/09 */
    b_fginit = TRUE;             /* SONY, cheah 24/07/09, to ensure init before call ISR func */
    return;
}

void DRVCUST_VdoModeChgDone(UINT8 bPath)
{
    return;
}

void DRVCUST_VideoMainloop()
{
    DRVCUST_AutoBacklightProc();
	#ifdef CC_SONY_GAMMA
	DRVCUST_AutoWB_Proc();	/* SONY, cheah 14/07/09 */
	#endif
}

void DRVCUST_OutVSyncISR()
{
    if (b_fginit == TRUE)        /* SONY, cheah 24/07/09, to ensure init before call ISR func */
    {
    DRVCUST_AutoBacklightISR();
	#ifdef CC_SONY_GAMMA
	DRVCUST_AutoWB_ISR();	/* SONY, cheah 14/07/09 */
        DRVCUST_Gamma_ISR();     /* SONY, cheah 28/07/09 */
	#endif
}
}

void DRVCUST_VdoInISR()
{
    return;
}

UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value)
{
    return u4Value;
}

/* @ Auto NR default Initial setting
 *      1: low
 *      2: Middle
 */
UINT8 DRVCUST_AutoNRDefSet(void)
{
    return 1;
}

void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
/* BEGIN, SONY cheah 28/07/09, remove temporary to avoid overwrite into sony nvm gamma curve */
#if 0
    EXTERN UINT8 GAMMA_256_BRIGHT[3][256];
    EXTERN UINT8 GAMMA_256_MIDDLE[3][256];

    EXTERN const  UINT16  GAMMA_950_TABLE[256];
    EXTERN const  UINT16  GAMMA_925_TABLE[256];
    EXTERN const  UINT16  GAMMA_900_TABLE[256];
    EXTERN const  UINT16  GAMMA_875_TABLE[256];
    EXTERN const  UINT16  GAMMA_850_TABLE[256];
    EXTERN const  UINT16  GAMMA_825_TABLE[256];



    EXTERN void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff);

    const UINT16 *pwRemapTable=NULL;
    UINT32 i;
    UINT32 u4Index,u4IndexNxt;
    UINT32 u4CurrentCh;
    UINT32 u4Msb;
    UINT16 u2RecontructArray[256];


    switch(u4GammaSel)
    {
        case 1:
            pwRemapTable=GAMMA_950_TABLE;
            break;
        case 2:
            pwRemapTable=GAMMA_925_TABLE;
            break;
        case 3:
            pwRemapTable=GAMMA_900_TABLE;
            break;
        case 4:
            pwRemapTable=GAMMA_875_TABLE;
            break;
        case 5:
            pwRemapTable=GAMMA_850_TABLE;
            break;
        case 6:
            pwRemapTable=GAMMA_825_TABLE;
            break;
        default:
            break;
    }

    if(pwRemapTable!=NULL)
    {
        for(u4CurrentCh=0; u4CurrentCh<3; ++u4CurrentCh)
        {
            //Recontruct the GAMMA Array
            for(i=1,u4Msb=0,u2RecontructArray[0]=0;i<256;++i)
            {
                if( GAMMA_256_BRIGHT[u4CurrentCh][i-1] >  GAMMA_256_BRIGHT[u4CurrentCh][i])
                {
                    u4Msb++;
                }
                u2RecontructArray[i]= (u4Msb<<8) + GAMMA_256_BRIGHT[u4CurrentCh][i];
            }

            for(i=0;i<256;++i)
            {
                u4Index = (pwRemapTable[i] >> 2)&0xFF;
                u4IndexNxt = u4Index + 1 < 0xFF ? u4Index + 1 : 0xFF;

                GAMMA_256_MIDDLE[u4CurrentCh][i]=
                    (UINT8)((u2RecontructArray[u4Index] * (4-(pwRemapTable[i]&3)) +
                        u2RecontructArray[u4IndexNxt]*(pwRemapTable[i]&3) + 2)>>2);

            }

        }
    }else{

        for(i=0;i<256;++i)
        {
            GAMMA_256_MIDDLE[0][i]=(UINT8)GAMMA_256_BRIGHT[0][i];
            GAMMA_256_MIDDLE[1][i]=(UINT8)GAMMA_256_BRIGHT[1][i];
            GAMMA_256_MIDDLE[2][i]=(UINT8)GAMMA_256_BRIGHT[2][i];
        }
    }
#endif
/* END, SONY cheah 28/07/09, remove temporary to avoid overwrite into sony nvm gamma curve */
}

/* @ Auto NR noise level implement
 * @ return noise level
 */
UINT8 DRVCUST_NoiseLevel(void)
{
    UINT8 bNoiseLevel = NM_INVALIDLEVEL;
  //  UINT8 bRFNMLevel;

    if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_TV))// TVD RF source
    {
		bNoiseLevel = DRVCUST_IP_Get8Level();   /* Ooi (21/7/09): get RF noise LVL */
    }
    else if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_AV))  // CVBS
    {
    	/* Ooi (21/7/09): get CVBS noise LVL - START */
        if (DRVCUST_IsSourceNoisy())
		{
			bNoiseLevel = DRVCUST_IP_Get8Level(); 	//CVBS NOISY
		}
		else
		{
        bNoiseLevel = vDrvNMLevel();   //content NR, Noise level 0 ~ 4
		}
		/* Ooi: END */
    }
    else
    {
        bNoiseLevel = NM_INVALIDLEVEL;
    }

    return bNoiseLevel;
}

/* @ get IP noise level for OSD - OSD USED ONLY!!
 * @ return noise level
 */
UINT8 DRVCUST_IPGetNoiseLevel(void)
{
	enum
	{
		LVL0,	//weak signal
		LVL1,
		LVL2,
		LVL3,
		LVL4,
		LVL5,
		LVL6,
		LVL7,
		LVL8	//good signal
	};
	UINT8 curLvl = LVL0;

	if(bDrvVideoSignalStatus_DBL(SV_VP_MAIN) == (UINT8)SV_VDO_STABLE)
	{
		curLvl = DRVCUST_NoiseLevel();

		if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_AV) && DRVCUST_IsSourceNoisy()==FALSE)
		{
			switch (curLvl)
			{
				case NM_LEVEL1_0:
				case NM_LEVEL1_1:
				case NM_LEVEL1_2:
				case NM_LEVEL1_3:
					curLvl = LVL8;
					break;

				case NM_LEVEL2:
					curLvl = LVL7;
					break;

				case NM_LEVEL3:
					curLvl = LVL6;
					break;

				case NM_LEVEL4:
					curLvl = LVL5;
					break;

				case NM_LEVEL5:
					curLvl = LVL4;
					break;

				case NM_LEVEL6:
					curLvl = LVL3;
					break;

				case NM_LEVEL7:
					curLvl = LVL2;
					break;

				case NM_LEVEL8:
					curLvl = LVL1;
					break;

				default:
					curLvl = LVL0;
					break;
			}
		}
		else
		{
			switch (curLvl)
			{
				case IP_LVL1:
					curLvl = LVL8;
					break;

				case IP_LVL2:
					curLvl = LVL7;
					break;

				case IP_LVL3:
					curLvl = LVL6;
					break;

				case IP_LVL4:
					curLvl = LVL5;
					break;

				case IP_LVL5:
					curLvl = LVL4;
					break;

				case IP_LVL6:
					curLvl = LVL3;
					break;

				case IP_LVL7:
					curLvl = LVL2;
					break;

				case IP_LVL8:
					curLvl = LVL1;
					break;

				default:
					curLvl = LVL0;
					break;
			}
		}
	}

	return curLvl;
}

void DRVCUST_SET_YGAMMA(UINT32 i1Mode)
{
    EXTERN void vDrvSetYGammaCoeff(UINT8 bmode);


//      LOG(0, "case 1 = GAMMA_950_TABLE\n");
//    	LOG(0, "case 2 = GAMMA_925_TABLE\n");
//    	LOG(0, "case 3 = GAMMA_900_TABLE\n");
//    	LOG(0, "case 4 = GAMMA_875_TABLE\n");
//    	LOG(0, "case 5 = GAMMA_850_TABLE\n");
//    	LOG(0, "case 6 = GAMMA_825_TABLE\n");
//    	LOG(0, "case 7 = GAMMA_105_TABLE\n");
//    	LOG(0, "case 8 = GAMMA_110_TABLE\n");
//    	LOG(0, "case 9 = GAMMA_115_TABLE\n");

    switch((INT8)i1Mode)
    {
        case 3:
            vDrvSetYGammaCoeff(9);
            break;
        case 2:
            vDrvSetYGammaCoeff(8);
            break;

        case 1:
            vDrvSetYGammaCoeff(7);

            break;
        case -1:
            vDrvSetYGammaCoeff(1);
            break;

        case -2:
            vDrvSetYGammaCoeff(3);
            break;
        case -3:
            vDrvSetYGammaCoeff(5);
            break;

        default:
            vDrvSetYGammaCoeff(0);
            break;
    }

    return;
}


/**
 * @brief Remap the middle point to 0x80
 * @param val: Value To Map
 */
static UINT8 _SignToOfst128(UINT8 val)
{
    return (val&0x80) ? (128 - (val&0x7f)) : (128 + val);
}

static UINT8 _Ofst128ToSign(UINT8 val)
{
    return (val&0x80) ? (val- 0x80) : (-val);
}

void DRVCUST_CalculateSCE(UINT8 bSceLumaGain, UINT8 bSceSatGain, UINT8 bSceHueGain)
{
	UINT8 i,j;
	INT32 i4Tmp;

    #define READ_BASE_SCE_TBL(index1,index2) BASE_SCE_TBL[index1*SCE_HUE_SLICE + index2]
    #define READ_MAX_SCE_TBL(index1,index2) MAX_SCE_TBL[index1*SCE_HUE_SLICE + index2]

	for (i = 0; i < 16; i++)
	{
		// y slope
		i4Tmp=((INT32)(_SignToOfst128(READ_MAX_SCE_TBL(ENUM_Y_SLOPE,i  )) - _SignToOfst128(READ_BASE_SCE_TBL(ENUM_Y_SLOPE,i  )))*((INT32)bSceLumaGain-0x80)/0x40)
		                +_SignToOfst128(READ_BASE_SCE_TBL(ENUM_Y_SLOPE,i  ));

        i4Tmp= i4Tmp<0 ? 0 :i4Tmp;
        i4Tmp= i4Tmp>0xFF ?	0xFF :i4Tmp;
        WRITE_SCE_TBL(ENUM_Y_SLOPE,i,_Ofst128ToSign((UINT8)i4Tmp));
	}

    for(j=ENUM_S_GAIN1;j<ENUM_S_GAIN3+1;++j)
    {
    	for (i = 0; i < 28; i++)
        {
            i4Tmp=((INT32)(READ_MAX_SCE_TBL(j, i) - READ_BASE_SCE_TBL(j, i))*((INT32)bSceSatGain-0x80)/0x40)
                                +READ_BASE_SCE_TBL(j, i);
            i4Tmp= i4Tmp<0 ? 0 :i4Tmp;
            i4Tmp= i4Tmp>0xFF ?	0xFF :i4Tmp;
            WRITE_SCE_TBL(j,i,(UINT8)i4Tmp);
        }
    }

    for(j=ENUM_H_FTN;j<ENUM_END;++j)
    {
    	for (i = 0; i < 28; i++)
        {

            i4Tmp=((INT32)(READ_MAX_SCE_TBL(j, i) - READ_BASE_SCE_TBL(j, i))*((INT32)bSceHueGain-0x80)/0x40)
            		    +READ_BASE_SCE_TBL(j, i);
            i4Tmp= i4Tmp<0 ? 0 :i4Tmp;
            i4Tmp= i4Tmp>0xFF ?	0xFF :i4Tmp;
            WRITE_SCE_TBL(j,i,(UINT8)i4Tmp);
        }
    }
}


void DRVCUST_UpdateBacklightState(UINT8 u1State, UINT16 u2Duty)
{
//    UNUSED(u1State);
    UINT16 u2Retval = 0;
    switch(u1State)
    {
      case BL_STATE_POWERON:
        vAutoBacklightState(BL_READY_STATE, &u2Retval);
        break;
      case BL_STATE_OUTPUT_FR_CHANGE:
        break;
      default:
        break;
    }
    UNUSED(u2Duty);
}

void DRVCUST_SetBacklightDuty(UINT16 u2Duty)
{
    UNUSED(u2Duty);
}

UINT32 DRVCUST_ATD_GetNoiseLevel(void)
{
    return ATD_GetNoiseLevel();
}

UINT16 DRVCUST_ATD_GetIFAGC(void)
{
    return ATD_GetIFAGC();
}

BOOL DRVCUST_ATD_SetLNA(BOOL fgLNA)
{
    return ATD_SetLNA(fgLNA);
}


//For SONY Read/Write GPIO status.
/////////////////////////////////////////////////////////////////////////
/*Please notice of below two point:
*1. Only using this two API to Read/Write GPIO that reserved for SONY.
*2. Only using DRVCUST_Gpio_Read  to read input mode gpio status. and using DRVCUST_Gpio_Write to write output mode gpio status.
*/

//-----------------------------------------------------------------------------
/** DRVCUST_Gpio_Read()  The GPIO input reading functions. It will check the
 *  i4GpioNum and read related register bit to return.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 DRVCUST_Gpio_Read(INT32 i4GpioNum)
{
	return GPIO_GetIn(i4GpioNum);
}

//-----------------------------------------------------------------------------
/** DRVCUST_Gpio_Write() to set gpio output value.
 *  @param i4GpioNum
 *  @param i4Val 0 or 1.
 *  @retval to return current gpio out register setting.
 */
//-----------------------------------------------------------------------------
INT32 DRVCUST_Gpio_Write(INT32 i4GpioNum,INT32 i4Val)
{
	return GPIO_SetOut(i4GpioNum,i4Val);
}

