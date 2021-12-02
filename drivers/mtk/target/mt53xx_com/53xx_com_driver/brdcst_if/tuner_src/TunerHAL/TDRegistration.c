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

#include "TDIS.h"
#include "x_drvapi_tuner.h"
#include "x_rm_dev_types.h"

#include "TDMgr.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "pd_tuner_type.h"
#include "ctrl_bus.h"

#if defined(MT5391ROW_P_ANALOG)
EXTERN INT32 ITunerRegistration(UINT16 id);
#endif

#if defined(CHECK_TUNER_TYPE_FROM_EEPROM)
STATIC UINT8 CheckTunerTypeFromEeprom(VOID);
#endif//#if defined(CHECK_TUNER_TYPE_FROM_EEPROM)

U8 DynamicSwitch_DTD_DVBT_Mode=1;/*DTD: DynamicSwitchMode=0,DVBT: DynamicSwitchMode=1(default)*/

S32 Undefined_TunerOpen (PTD_CTX ptTDCtx,
                                    TUNER_CONFIG_T * ptTunerCfgInfo,
                                    PTD_SPECIFIC_CTX* pptTDSpecificCtx)
{
	return DRVAPI_TUNER_ERROR;
}

#define DECLARE_OPENFCT(fct) extern S32 fct (PTD_CTX ptTDCtx, \
                                    TUNER_CONFIG_T * ptTunerCfgInfo, \
                                    PTD_SPECIFIC_CTX* pptTDSpecificCtx)

#define DECLARE_OPENPAIR(Tuner0_ID,Tuner0_OpenFct,Tuner1_ID,Tuner1_OpenFct)  \
	DECLARE_OPENFCT(Tuner0_OpenFct); \
	DECLARE_OPENFCT(Tuner1_OpenFct);\
	LTDIS_TUNERID_OPENFCT_PAIR_T   tTunerID_OpenFct_Pair[MAX_TUNER_NUM]= \
	{ \
		{Tuner0_ID,Tuner0_OpenFct}, \
		{Tuner1_ID,Tuner1_OpenFct} \
	};
/************************ DUMMY  PART*************************/
#if defined(DUMMY)
	DECLARE_OPENPAIR( DUMMY_0,Dummy_TunerOpen, DUMMY_1, Dummy_TunerOpen)
#elif defined(DUMMY_P_ANANA)
	DECLARE_OPENPAIR( DUMMY_0, Dummy_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen)
#elif defined(DUMMY_P_ATDNA)
	DECLARE_OPENPAIR( DUMMY_0, Dummy_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DUMMY_P_ANAEU)
	DECLARE_OPENPAIR( DUMMY_0, Dummy_TunerOpen, MTK_ANALOG_0, AnalogEU_TunerOpen)
#elif defined(DUMMY_P_ATDEU)
	DECLARE_OPENPAIR( DUMMY_0, Dummy_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
/************************ DTD  PART*************************/
#elif defined(DTD_P_ANANA)
	DECLARE_OPENPAIR( MTK_DTD_0, DTD_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen)
#elif defined(DTD_P_ATDNA)
	DECLARE_OPENPAIR( MTK_DTD_0, DTD_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)//AtdNA_TunerOpen
#elif defined(MT5112EE_P_ATDNA)
       DECLARE_OPENPAIR( MTK_DTD_0, DTD_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)//AtdNA_TunerOpen
#elif defined(DTD_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DTD_0, DTD_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DTD_P_DUMMY)
	DECLARE_OPENPAIR( MTK_DTD_0, DTD_TunerOpen, DUMMY_1, Dummy_TunerOpen)
/************************ DVBT  PART*************************/
#elif defined(DVBT_P_ANAEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, DVBT_TunerOpen, MTK_ANALOG_0, AnalogEU_TunerOpen)
#elif defined(DVBT_P_ANANA)
	DECLARE_OPENPAIR( MTK_DVBT_0, DVBT_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen)	
#elif defined(DVBT_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, DVBT_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DVBT_P_ATDNA)
    DECLARE_OPENPAIR( MTK_DVBT_0, DVBT_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DVBT_P_DUMMY)
	DECLARE_OPENPAIR( MTK_DVBT_0, DVBT_TunerOpen, DUMMY_1, Dummy_TunerOpen)	
************************ MT5135  PART*************************/
#elif defined(MT5135_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(MT5135_P_ATDNA)
		DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
	//DECLARE_OPENPAIR( MTK_DVBC_0, DVBC_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
/************************ DVBS  PART*************************/
#elif defined(DVBS_P_ANAEU)
	DECLARE_OPENPAIR( MTK_DVBS_0, DVBS_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DVBS_P_ANANA)
	DECLARE_OPENPAIR( MTK_DVBS_0, DVBS_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen) 
#elif defined(DVBS_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBS_0, DVBS_TunerOpen, MTK_ANALOG_0, Undefined_TunerOpen)	
#elif defined(MT5135DVBS_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DVBS_P_ATDNA)
	DECLARE_OPENPAIR( MTK_DVBS_0, DVBS_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DVBS_P_DUMMY)
	DECLARE_OPENPAIR( MTK_DVBS_0, DVBS_TunerOpen, DUMMY_1, Dummy_TunerOpen) 



/************************DVB2TS_P_ATDEU*************************/
#elif defined(DVB2TS_P_ANAEU)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AnalogEU_TunerOpen)
#elif defined(DVB2TS_P_ANANA)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen) 
#elif defined(DVB2TS_P_ATDEU)
		DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DVB2TS_P_ATDNA)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DVB2TS_P_DUMMY)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, DUMMY_1, Dummy_TunerOpen)


/************************ VDRDVBC  PART*************************/
#elif defined(VDRDVBC_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
/************************ DVBT2  PART*************************/
#elif defined(DVB2T_P_ANAEU)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AnalogEU_TunerOpen)
#elif defined(DVB2T_P_ANANA)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen) 
#elif defined(DVB2T_P_ATDEU)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DVB2T_P_ATDNA)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DVB2T_P_DUMMY)
		DECLARE_OPENPAIR( MTK_DVBT_0, DVBT2_TunerOpen, DUMMY_1, Dummy_TunerOpen)	
	
	
/************************ DVBCT  PART*************************/
#elif defined(DVBCT_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
/************************ DVBTC PART*************************//*Lei_code_dB0413,For MT5396 Demo Program*/ 
#elif defined(DVBTC_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DVBTC_P_ATDNA)
	DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DVBTC_P_DUMMY)
    DECLARE_OPENPAIR( MTK_DVBT_0, REMAP_TunerOpen, DUMMY_1, Dummy_TunerOpen)

/************************ CUSTOM  PART*************************/
#elif defined(MT5391ISDBT_P_ANALOG)
	DECLARE_OPENPAIR( MTK_DTD_0, ISDBT_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen)
#elif defined(MT5391DVBT_P_TW_ANALOG)
	DECLARE_OPENPAIR( MTK_ANALOG_0, AnalogNA_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen)
#elif defined(MT5391DVBC_P_ANALOG)
	DECLARE_OPENPAIR( MTK_DVBC_0, DVBC_TunerOpen, UNDEFINED_ID, Undefined_TunerOpen)
#elif defined(MT5391DVBCDVBT_P_ANALOG)
	DECLARE_OPENPAIR( MTK_DVBCDVBT_0, DVBCDVBT_TunerOpen, MTK_ANALOG_0, AnalogEU_TunerOpen)
#elif defined(TUNER_VENDOR_P_ANALOG)
	DECLARE_OPENPAIR( VENDOR_0, Vendor_TunerOpen, DUMMY_1, Dummy_TunerOpen)
#elif defined(VDRISDBT_P_ATDNA)
	DECLARE_OPENPAIR( VENDOR_0, Vendor_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(DTDDVBT_P_ANALOG) || defined(MT5391DUAL_TDQU6_DTOS40FP_P_ANALOG) \
		|| defined(MT5391ROW_P_ANALOG) || defined(DTDDUMMY_P_UATD) || defined(MT5135MT5112EE_P_UATD)
	DECLARE_OPENPAIR( MTK_DIGITAL_0, Digital_TunerOpen, MTK_ANALOG_0, Analog_TunerOpen)
#elif  defined(DTDDVBTC_P_ANALOG)	
	DECLARE_OPENPAIR( MTK_DIGITAL_0, Digital_TunerOpen, MTK_ANALOG_0, Analog_TunerOpen)
#elif defined(ISDBT_P_ANANA)
	DECLARE_OPENPAIR( MTK_ISDBT_0, ISDBT_TunerOpen, MTK_ANALOG_0, AnalogNA_TunerOpen)
#elif defined(ISDBT_P_ATDNA)
	DECLARE_OPENPAIR( MTK_ISDBT_0, ISDBT_TunerOpen, MTK_ANALOG_0, AtdNA_TunerOpen)
#elif defined(ISDBT_P_DUMMY)
	DECLARE_OPENPAIR( MTK_ISDBT_0, ISDBT_TunerOpen, DUMMY_1, Dummy_TunerOpen)	
#elif defined(DTMB_P_ATDEU)
	DECLARE_OPENPAIR( MTK_DTMB_0, DTMB_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)
#elif defined(DTMBREMAP_P_ATDEU)
   	DECLARE_OPENPAIR( MTK_DTMB_0, REMAP_TunerOpen, MTK_ANALOG_0, AtdEU_TunerOpen)	
#endif

#if defined(DTDDVBTC_P_ANALOG)
DECLARE_OPENFCT(DTD_TunerOpen);
DECLARE_OPENFCT(AtdNA_TunerOpen);
DECLARE_OPENFCT(REMAP_TunerOpen);
DECLARE_OPENFCT(AtdEU_TunerOpen); 
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){   
    if(DynamicSwitch_DTD_DVBT_Mode==0){ /*get DTD TunerOpen API*/
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
    else{/*get DVBT TunerOpen API*/
        return REMAP_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
}
S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    if(DynamicSwitch_DTD_DVBT_Mode==0){ /*get DTD TunerOpen API*/
        return AtdNA_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
    else{/*get DVBT TunerOpen API*/
        return AtdEU_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
}
#elif defined(DTDDVBT_P_ANALOG)
DECLARE_OPENFCT(DTD_TunerOpen);
DECLARE_OPENFCT(AnalogNA_TunerOpen);
DECLARE_OPENFCT(DVBT_TunerOpen);
DECLARE_OPENFCT(AnalogEU_TunerOpen); 
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){   
    if(DynamicSwitch_DTD_DVBT_Mode==0){ /*get DTD TunerOpen API*/
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
    else{/*get DVBT TunerOpen API*/
        return DVBT_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
}
S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    if(DynamicSwitch_DTD_DVBT_Mode==0){ /*get DTD TunerOpen API*/
        return AnalogNA_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
    else{/*get DVBT TunerOpen API*/
        return AnalogEU_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
}
#elif defined(MT5135MT5112EE_P_UATD)
#include "x_util.h"

DECLARE_OPENFCT(DTD_TunerOpen);
DECLARE_OPENFCT(AtdNA_TunerOpen);
DECLARE_OPENFCT(REMAP_TunerOpen);
DECLARE_OPENFCT(AtdEU_TunerOpen); 
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
UINT32 u4DtvCountry;

#ifdef NEW_COUNTRY_TYPE
    u4DtvCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    
    mcSHOW_USER_MSG(("%s:DTV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4DtvCountry, GetTargetCountry()));
     
    switch (u4DtvCountry)
    {
    case COUNTRY_DTV_ATSC:
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_DTV_DVBT:
        return REMAP_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: DTV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);        
    }
#else
    u4DtvCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    

    mcSHOW_USER_MSG(("%s:DTV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4DtvCountry, GetTargetCountry()));
     
    switch (u4DtvCountry)
    {
    case COUNTRY_US:
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_EU:
        return REMAP_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: DTV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);        
    }
#endif    
}
S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    TUNER_DEVICE_CTX_T* pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    UINT32 u4AtvCountry;
    
#ifdef NEW_COUNTRY_TYPE
    u4AtvCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);

    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_ATV_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_ATV_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#else
    u4AtvCountry = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;    

    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif
}

#elif defined(MT5391DUAL_TDQU6_DTOS40FP_P_ANALOG)
#include "nptv_drvif.h"
DECLARE_OPENFCT(DTD_TunerOpen);
DECLARE_OPENFCT(AnalogNA_TunerOpen);
DECLARE_OPENFCT(DVBT_TunerOpen);
DECLARE_OPENFCT(AnalogEU_TunerOpen); 
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
UINT32 u4DtvCountry;
 
#ifdef NEW_COUNTRY_TYPE
    u4DtvCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    if (u4DtvCountry == COUNTRY_DTV_ATSC)
#else
    u4DtvCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    
    if(u4DtvCountry == COUNTRY_US)
#endif
    { /*get DTD TunerOpen API*/
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
    else
    {/*get DVBT TunerOpen API*/
        return DVBT_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
}
S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
UINT32 u4AtvCountry;
#ifdef NEW_COUNTRY_TYPE
    u4AtvCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);
    if (u4AtvCountry == COUNTRY_ATV_US)
#else
    u4AtvCountry = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;    
    if(u4AtvCountry == COUNTRY_US)
#endif
    { /*get DTD TunerOpen API*/
        return AnalogNA_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
    else{/*get DVBT TunerOpen API*/
        return AnalogEU_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    }
}
#elif defined(MT5391ROW_P_ANALOG)

DECLARE_OPENFCT(Dummy_TunerOpen);
DECLARE_OPENFCT(AnalogNA_TunerOpen);
DECLARE_OPENFCT(AnalogEU_TunerOpen);	
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    return Dummy_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
}
S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    TUNER_DEVICE_CTX_T* pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    UINT8 tunerType = CheckTunerTypeFromEeprom();
    
    pTunerDeviceCtx->tunerType = tunerType;
    switch (tunerType)
    {
    case cNXP_UV1336:
        DynamicSwitch_DTD_DVBT_Mode = 0;
        ITunerRegistration(cNXP_UV1336);
        return AnalogNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case cNXP_UV1316E:
        DynamicSwitch_DTD_DVBT_Mode = 1;
        ITunerRegistration(cNXP_UV1316E);
        return AnalogEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case cNXP_UV1356E:
        DynamicSwitch_DTD_DVBT_Mode = 1;
        ITunerRegistration(cNXP_UV1356E);
        return AnalogEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: wrong tuner type\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
}
#elif defined(DTDDUMMY_P_UATD)
#include "x_util.h"

DECLARE_OPENFCT(Dummy_TunerOpen);
DECLARE_OPENFCT(DTD_TunerOpen);
DECLARE_OPENFCT(AtdNA_TunerOpen);
DECLARE_OPENFCT(AtdEU_TunerOpen);	
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
UINT32 u4DtvCountry;

#ifdef NEW_COUNTRY_TYPE
    u4DtvCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    
    mcSHOW_USER_MSG(("%s:DTV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4DtvCountry, GetTargetCountry()));
     
    switch (u4DtvCountry)
    {
    case COUNTRY_DTV_ATSC:
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_DTV_DUMMY:
        return Dummy_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: DTV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);        
    }
#else
    u4DtvCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    

    mcSHOW_USER_MSG(("%s:DTV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4DtvCountry, GetTargetCountry()));
     
    switch (u4DtvCountry)
    {
    case COUNTRY_US:
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_DUMMY:
        return Dummy_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: DTV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);        
    }
#endif    
}

S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    TUNER_DEVICE_CTX_T* pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    UINT32 u4AtvCountry;
    
#ifdef NEW_COUNTRY_TYPE
    u4AtvCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);

    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_ATV_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_ATV_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#else
    u4AtvCountry = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;    

    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif

#if 0
    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif
}
#elif defined(MT5135DUMMY_P_ATDUN)
#include "x_util.h"   //COUNTRY define in this file

DECLARE_OPENFCT(Dummy_TunerOpen);
DECLARE_OPENFCT(REMAP_TunerOpen);
DECLARE_OPENFCT(AtdNA_TunerOpen);
DECLARE_OPENFCT(AtdEU_TunerOpen);	
S32 Digital_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
UINT32 u4DtvCountry;

#ifdef NEW_COUNTRY_TYPE   //for 65 95
    u4DtvCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    
    mcSHOW_USER_MSG(("%s:DTV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4DtvCountry, GetTargetCountry()));
     
    switch (u4DtvCountry)
    {
    case COUNTRY_DTV_DVBT:
        return DTD_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_DTV_DUMMY:
        return Dummy_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: DTV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);        
    }
#else      //for 63 87...
    u4DtvCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    

    mcSHOW_USER_MSG(("%s:DTV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4DtvCountry, GetTargetCountry()));
     
    switch (u4DtvCountry)
    {
    case COUNTRY_EU:
        return REMAP_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_DUMMY:
        return Dummy_TunerOpen(ptTDCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: DTV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);        
    }
#endif    
}

S32 Analog_TunerOpen(PTD_CTX ptTDCtx, TUNER_CONFIG_T * ptTunerCfgInfo, PTD_SPECIFIC_CTX* pptTDSpecificCtx){
    TUNER_DEVICE_CTX_T* pTunerDeviceCtx = (TUNER_DEVICE_CTX_T*)ptTDCtx;
    UINT32 u4AtvCountry;
    
#ifdef NEW_COUNTRY_TYPE
    u4AtvCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);

    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_ATV_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_ATV_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#else
    u4AtvCountry = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;    

    mcSHOW_USER_MSG(("%s:ATV Scan=%d(TargetCountry=0x%02X)\n", __FUNCTION__, u4AtvCountry, GetTargetCountry()));
    
    switch (u4AtvCountry)
    {
    case COUNTRY_US:
         return AtdNA_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    case COUNTRY_EU:
         return AtdEU_TunerOpen((VOID*)pTunerDeviceCtx, ptTunerCfgInfo,pptTDSpecificCtx);
    default:
        mcSHOW_USER_MSG(("%s: ATV scan configuration not supported\n", __FUNCTION__));
        return (DRVAPI_TUNER_ERROR);
    }
#endif
}

#endif


INT32 TDIS_RetrieveTunerCfgTbl(TUNER_CONFIG_TBL_T* ptTunerCfgTbl)
{
    x_memset(ptTunerCfgTbl, 0, sizeof(TUNER_CONFIG_TBL_T));
/************************ DUMMY  PART*************************/
#if defined(DUMMY)
    ptTunerCfgTbl->TunerNum = 2;
//    ptTunerCfgTbl->TunerNum = 1;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cCONN_EXCLUSIVE_EN;
//			    cWITH_DRVT_TUNER_CAB_DIG_OOB_RX;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = DUMMY_1;

    ptTunerCfgTbl->tTunerCfg[1].TunerID = DUMMY_1;
//    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
//                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;
#elif defined(DUMMY_P_ANANA) || defined(DUMMY_P_ATDNA) 
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;
#elif defined(DUMMY_P_ANAEU) || defined(DUMMY_P_ATDEU) 
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;
/************************ DTD  PART*************************/
#elif defined(DTD_P_ANANA) || defined(DTD_P_ATDNA) || defined(MT5112EE_P_ATDNA)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
#elif defined(DTD_P_ANAEU) || defined(DTD_P_ATDEU)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN; /* SCART*/
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTD_0;
#elif defined(DTD_P_DUMMY)  
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = DUMMY_1;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID =DUMMY_1;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
	
/********************** DVBT2 PART************************/
#elif defined(DVB2T_P_ANAEU) || defined(DVB2T_P_ATDEU) || defined(DVB2T_P_ATDNA) ||defined(DVB2T_P_DUMMY)//||defined(DVB2TS_P_ATDEU)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0xD8;//according to sy driver code
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;	
/************************ DVBT  PART*************************/
/********************** VDRDVBC  PART************************/
#elif defined(DVBT_P_ANAEU) || defined(DVBT_P_ATDEU) || defined(VDRDVBC_P_ATDEU)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
/************************ MT5135  PART*************************/
#elif defined(MT5135_P_ATDEU)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
#elif defined(MT5135_P_ATDNA)
		ptTunerCfgTbl->TunerNum = 2;
		ptTunerCfgTbl->MutualExcl = 1;
		ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
		ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;    
		ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
								cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
/************************ DVBS  PART*************************/
#elif defined(DVBS_P_ATDEU)
	ptTunerCfgTbl->TunerNum = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBS_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_SAT_DIG;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;
#elif defined(MT5135DVBS_P_ATDEU)||defined(DVB2TS_P_ATDEU)
	ptTunerCfgTbl->TunerNum = 2;
		ptTunerCfgTbl->MutualExcl = 1;
		ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
		ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+cWITH_DRVT_TUNER_SAT_DIG+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;	 //no use 
		ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;	 //no use 
		ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;	 //no use
		ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;    
		ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+cWITH_DRVT_TUNER_SAT_ANA+
								cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+cWITH_DRVT_TUNER_SAT_ANA_SCART_OUT+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;	
		ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;	 
		ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;	 
		ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
/********************** ANANA & DVBT  PART************************/

#elif defined(DVBT_P_ANANA)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
						  cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
						  cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
						  cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
	
#elif defined(DVBT_P_ATDNA)
		ptTunerCfgTbl->TunerNum = 2;
		ptTunerCfgTbl->MutualExcl = 1;
		ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
		ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
							  cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
							  cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
							  cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
		ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
		ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
	
		ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;

/********************** DVBC & DVBT  PART************************/
#elif defined(DVBCT_P_ATDEU)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG +
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT +
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;

#elif defined(DVBTC_P_ATDEU)//Lei_code_dB0413,For MT5396 Demo Program
		ptTunerCfgTbl->TunerNum = 2;
		ptTunerCfgTbl->MutualExcl = 1;
		ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
		ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG +
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
								cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT +
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
#elif defined(DVBTC_P_ATDNA)
		ptTunerCfgTbl->TunerNum = 2;
		ptTunerCfgTbl->MutualExcl = 1;
		ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
		ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;    
		ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
								cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
#elif defined(DVBTC_P_DUMMY)
        ptTunerCfgTbl->TunerNum = 2;
        ptTunerCfgTbl->MutualExcl = 1;
        ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
        ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG +
                                cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;     //no use in DVBT and DVBC, Menghau
        ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;     //no use in DVBT and DVBC, Menghau
        ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;     //no use in DVBT and DVBC, Menghau
        ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = DUMMY_1;
        ptTunerCfgTbl->tTunerCfg[1].TunerID = DUMMY_1;
        ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                                cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT +
                                cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;     //no use in DVBT and DVBC, Menghau
        ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;     //no use in DVBT and DVBC, Menghau
        ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;     //no use in DVBT and DVBC, Menghau
        ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;

#elif defined(DVBT_P_DUMMY)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;

    ptTunerCfgTbl->tTunerCfg[1].TunerID = DUMMY_1;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA;
/************************ CUSTOM  PART*************************/		    
#elif defined(ISDBT_P_ANANA) || defined(ISDBT_P_ATDNA)//5363 ISDBT
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_ISDBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;		    
#elif defined(MT5391ISDBT_P_ANALOG)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;;
#elif defined(VDRISDBT_P_ATDNA)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = VENDOR_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xff;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
#elif defined(MT5391DVBT_P_TW_ANALOG)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
	ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
#elif defined(MT5391DVBC_P_ANALOG)
    ptTunerCfgTbl->TunerNum = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBC_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
#elif defined(MT5391DVBCDVBT_P_ANALOG)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBCDVBT_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBCDVBT_0;
#elif defined(TUNER_VENDOR_P_ANALOG)
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = VENDOR_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = DUMMY_1;

    ptTunerCfgTbl->tTunerCfg[1].TunerID = DUMMY_1;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = VENDOR_0;

#elif defined(DTDDVBTC_P_ANALOG)
    mcSHOW_USER_MSG(("%s: DTDDVBTC_P_ATDLOG\n", __FUNCTION__));
    if(DynamicSwitch_DTD_DVBT_Mode == 0){/*get DTD configuration*/
    	    mcSHOW_USER_MSG(("%s: DynamicSwitch_DTD_DVBT_Mode = %d\n", __FUNCTION__, DynamicSwitch_DTD_DVBT_Mode));
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN; /* SCART*/
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTD_0;
    }
    else{/*Get DVBT configuration*/
    	    mcSHOW_USER_MSG(("%s: DynamicSwitch_DTD_DVBT_Mode = %d\n", __FUNCTION__, DynamicSwitch_DTD_DVBT_Mode));    	
		ptTunerCfgTbl->TunerNum = 2;
		ptTunerCfgTbl->MutualExcl = 1;
		ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
		ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG +
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
		ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
								cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
								cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT +
								cCONN_EXCLUSIVE_EN;
		ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;	 //no use in DVBT and DVBC, Menghau
		ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
    }

    
#elif defined(DTDDVBT_P_ANALOG)
    mcSHOW_USER_MSG(("%s: DTDDVBT_P_ATDLOG\n", __FUNCTION__));
    if(DynamicSwitch_DTD_DVBT_Mode == 0){/*get DTD configuration*/
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN; /* SCART*/
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTD_0;
    }
    else{/*Get DVBT configuration*/
      ptTunerCfgTbl->TunerNum = 2;
      ptTunerCfgTbl->MutualExcl = 1;
      ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
      ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cCONN_EXCLUSIVE_EN;
      ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;    
      ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
      ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
      ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
    }

#elif defined(MT5135MT5112EE_P_UATD)
    UINT32 u4DtvCountry;

#ifdef NEW_COUNTRY_TYPE
    u4DtvCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    if (u4DtvCountry == COUNTRY_DTV_ATSC)
#else
    u4DtvCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    
    if(u4DtvCountry == COUNTRY_US)
#endif
    {/*get DTD configuration*/
        ptTunerCfgTbl->TunerNum = 2;
        ptTunerCfgTbl->MutualExcl = 1;
        ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_MT5112_0;
        ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    	ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    	ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    	ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    	ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    	
    	ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    	ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    	ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
    }
    else{/*Get DVBT configuration*/
      ptTunerCfgTbl->TunerNum = 2;
      ptTunerCfgTbl->MutualExcl = 1;
      ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
      ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cCONN_EXCLUSIVE_EN;
      ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;    
      ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
      ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
      ptTunerCfgTbl->tTunerCfg[1].ucDemodInbAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[1].ucRFTunerAddr = 0x00;    //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[1].ucDemodOobAddr = 0x00;   //no use in DVBT and DVBC, Menghau
      ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
    }


#elif defined(MT5391DUAL_TDQU6_DTOS40FP_P_ANALOG)
    UINT32 u4DtvCountry;

#ifdef NEW_COUNTRY_TYPE
    u4DtvCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    if (u4DtvCountry == COUNTRY_DTV_ATSC)
#else
    u4DtvCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    
    if(u4DtvCountry == COUNTRY_US)
#endif
    {/*get DTD configuration*/
        ptTunerCfgTbl->TunerNum = 2;
        ptTunerCfgTbl->MutualExcl = 1;
        ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_MT5112_0;
        ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    	ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    	ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    	ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    	ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    	
    	ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    	ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
    	ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_MT5112_0;
    }
    else{/*Get DVBT configuration*/
        ptTunerCfgTbl->TunerNum = 2;
        ptTunerCfgTbl->MutualExcl = 1;
        ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DVBT_0;
        ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
        ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
        ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
        ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
        
        ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
        ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DVBT_0;
    }
#elif defined(MT5391ROW_P_ANALOG)
    if(0 == DynamicSwitch_DTD_DVBT_Mode) {/*US*/
        ptTunerCfgTbl->TunerNum = 2;
        ptTunerCfgTbl->MutualExcl = 1;
        ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;
        ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

        ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
        ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;
    }
    else {/*EU*/
        ptTunerCfgTbl->TunerNum = 2;
        ptTunerCfgTbl->MutualExcl = 1;
        ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;
        ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

        ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
        ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
        ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;
    }
#elif defined(DTDDUMMY_P_UATD)
    UINT32 u4TarCountry = 0;

    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;

    // DTV configuration
#ifdef NEW_COUNTRY_TYPE
    u4TarCountry = GetTargetCountryMask(COUNTRY_DTV_MASK);
    
    switch (u4TarCountry)
    {
        case COUNTRY_DTV_ATSC: /*DTD*/
            ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
            ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                                cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                                cCONN_EXCLUSIVE_EN;
            
            ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTD_0;      	
        	break;
        case COUNTRY_DTV_DUMMY:
            ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;;
            ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                                cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                                cCONN_EXCLUSIVE_EN;
            
            ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;     	
        	break;
       	default:
            mcSHOW_USER_MSG(("%s: DTV scan configuration not supported \n", __FUNCTION__));
       		break;
    }

#else
    u4TarCountry = (GetTargetCountry() & COUNTRY_SCAN_DTV_MASK) >> SCAN_DTV_POS;    

    switch (u4TarCountry)
    {
        case COUNTRY_US: /*DTD*/
            ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTD_0;
            ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                                cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                                cCONN_EXCLUSIVE_EN;
            
            ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTD_0;      	
        	break;
        case COUNTRY_DUMMY:
            ptTunerCfgTbl->tTunerCfg[0].TunerID = DUMMY_0;;
            ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                                cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                                cCONN_EXCLUSIVE_EN;
            
            ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = DUMMY_0;     	
        	break;
       	default:
            mcSHOW_USER_MSG(("%s: DTV scan configuration not supported \n", __FUNCTION__));
       		break;
    }

#endif     
    
    // ATV configuration
#ifdef NEW_COUNTRY_TYPE
    u4TarCountry = GetTargetCountryMask(COUNTRY_ATV_MASK);

    switch (u4TarCountry)
    {
        case COUNTRY_ATV_US:
            ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

            ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
            ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                                cCONN_EXCLUSIVE_EN;
        	break;
        case COUNTRY_ATV_EU:
            ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

            ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
            ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                                cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                                cCONN_EXCLUSIVE_EN;
        	break;
       	default:
            mcSHOW_USER_MSG(("%s: ATV scan configuration not supported \n", __FUNCTION__));
       		break;
    }    
#else
    u4TarCountry = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;    

    switch (u4TarCountry)
    {
        case COUNTRY_US:
            ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

            ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
            ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                                cCONN_EXCLUSIVE_EN;
        	break;
        case COUNTRY_EU:
            ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;

            ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
            ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                                cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                                cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                                cCONN_EXCLUSIVE_EN;
        	break;
       	default:
            mcSHOW_USER_MSG(("%s: ATV scan configuration not supported \n", __FUNCTION__));
       		break;
    }
#endif
   #elif defined(DTMB_P_ATDEU) 
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTMB_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTMB_0; 
   #elif defined(DTMBREMAP_P_ATDEU) 
    ptTunerCfgTbl->TunerNum = 2;
    ptTunerCfgTbl->MutualExcl = 1;
    ptTunerCfgTbl->tTunerCfg[0].TunerID = MTK_DTMB_0;
    ptTunerCfgTbl->tTunerCfg[0].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_DIG+cWITH_DRVT_TUNER_CAB_DIG+
                            cWITH_DRVT_TUNER_CAB_DIG_OOB_RX+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodInbAddr = 0x92;
    ptTunerCfgTbl->tTunerCfg[0].ucRFTunerAddr = 0xc2;
    ptTunerCfgTbl->tTunerCfg[0].ucDemodOobAddr = 0xd2;
    ptTunerCfgTbl->tTunerCfg[0].ConnExclusiveID = MTK_ANALOG_0;
    
    ptTunerCfgTbl->tTunerCfg[1].TunerID = MTK_ANALOG_0;
    ptTunerCfgTbl->tTunerCfg[1].i2TunerCapability =
                            cWITH_DRVT_TUNER_TER_ANA+cWITH_DRVT_TUNER_CAB_ANA+
                            cWITH_DRVT_TUNER_TER_ANA_SCART_OUT+cWITH_DRVT_TUNER_CAB_ANA_SCART_OUT+
                            cCONN_EXCLUSIVE_EN;
    ptTunerCfgTbl->tTunerCfg[1].ConnExclusiveID = MTK_DTMB_0; 

#else
    return (DRVAPI_TUNER_ERROR);
#endif

    return (DRVAPI_TUNER_OK);
}

INT32 LTDIS_GetOpenFct(UINT16 TunerID, LTDIS_TunerOpen_fct* ppf_TunerOpen)
{
    int     i;

    for (i=0;i<MAX_TUNER_NUM;i++){
        if (tTunerID_OpenFct_Pair[i].TunerID == TunerID){
            *ppf_TunerOpen = tTunerID_OpenFct_Pair[i].pf_TunerOpen;
		   mcSHOW_USER_MSG(("Tuner %d LTDIS_GetOpenFct Pair %d done!\n",TunerID,i));
            return (DRVAPI_TUNER_OK);
        }
    }

    *ppf_TunerOpen = NULL;
    return (DRVAPI_TUNER_ERROR);
}

char* LTDIS_GetCompNameForMW(UINT16 TunerID, DRV_TYPE_T DrvType)
{
    int     i;
    for (i=0;i<MAX_TUNER_NUM;i++){
        if (tTunerID_OpenFct_Pair[i].TunerID == TunerID){
            if(TunerID==MTK_DIGITAL_0)
            {
	            switch (DrvType){
	                case DRVT_TUNER_CAB_DIG:
	                	return SN_MAIN_TUNER_CAB_DIG;
	                case DRVT_TUNER_TER_DIG:
	                    return SN_MAIN_TUNER_TER_DIG;
	                case DRVT_TUNER_CAB_DIG_OOB_RX:
	                    return SN_MAIN_TUNER_OOB_CAB_RX;
					case DRVT_TUNER_SAT_DIG:
						return SN_MAIN_TUNER_SAT_DIG;
	            }
            }
            else if(TunerID==MTK_ANALOG_0)
            {
	            switch (DrvType){
	                case DRVT_TUNER_TER_ANA:
	                    return  "tuner_ter_ana_0";
	                case DRVT_TUNER_CAB_ANA:
	                    return "tuner_cab_ana_0";
	            }
            }
        }
    }
    return NULL;
}

#if 0
#if defined(CHECK_TUNER_TYPE_FROM_EEPROM)
//------------------------------------------------------------------------- 
/*
 *  CheckValidTunerType
 *  check whether tuner Type is valid
 *  @param  u1TestTunerType   TunerType for test
 *  @retval TRUE    valid tuner type   
 *  @retval FALSE    invalid tuner type
 */
//-------------------------------------------------------------------------
STATIC BOOL CheckValidTunerType(UINT8 data)
{
    BOOL fgRet = FALSE;

#if defined(MT5391ROW_P_ANALOG)
    if ((data >= cITUNER_VALID_START) && (data <= cITUNER_VALID_END))
    {  
        fgRet = TRUE;
    }
#endif

    return fgRet;
}

//------------------------------------------------------------------------- 
/*
 *  CheckTunerTypeFromEeprom
 *  1. Tuner id exist in three additional EEPROM address for check
 *  2. Judgement:
 *  Tuner ID invalid, ex. 0xff -> Use the default Tuner, 
 *  Three address are with the same valid Tuner ID -> Trust the tuner ID
 *  Two address are with the same valid Tuner ID -> Trust the tuner ID
 *  All address are with different Tuner ID -> Use the default Tuner
 *  @param  void
 *  @retval tunerid    return tuner id depend on the matched judgement rule
 */
//-------------------------------------------------------------------------
STATIC UINT8 CheckTunerTypeFromEeprom(VOID)
{
    UINT8  u1Val[3];
    UINT8  u1DeftType,u1ResultType;
    UINT32 u4Ofst[3];
    UINT32 u4Default;
#if defined(MT5391ROW_P_ANALOG)
    UINT8  i = 0;
#endif

    I2cPinmuxInit();

    /* Read three offset value and default tuner type from drv_cust */
    u4Ofst[0] = DRVCUST_OptGet(eTunerTypeOfst0);
    u4Ofst[1] = DRVCUST_OptGet(eTunerTypeOfst1);
    u4Ofst[2] = DRVCUST_OptGet(eTunerTypeOfst2);
    u4Default = DRVCUST_OptGet(eTunerTypeDefault);
    mcSHOW_USER_MSG(("EEPROM(tuner): Ofst0=0x%x,Ofst1=0x%x,Ofst2=0x%x,def=0x%x\r\n",u4Ofst[0], u4Ofst[1], u4Ofst[2], u4Default));

    /* Read three tuner IDs from EEPROM offset */
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[0], (UINT32)(void *)&u1Val[0], 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[1], (UINT32)(void *)&u1Val[1], 1));
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst[2], (UINT32)(void *)&u1Val[2], 1));
    mcSHOW_USER_MSG(("Read TunerType from EEPROM: addr(0x%x)=0x%x,addr(0x%x)=0x%x,addr(0x%x)=0x%x\r\n",u4Ofst[0],u1Val[0],u4Ofst[1],u1Val[1],u4Ofst[2],u1Val[2]));

#if defined(MT5391ROW_P_ANALOG)  
    /* translate from EEPROM value to tuner type */  
    for (i = 0; i < 3; i++)
    {
        u1Val[i] = ((u1Val[i]&0xE0)>>5) + cITUNER_VALID_START;
    }
    /* Default tuner */
   	u1DeftType = cNXP_UV1336;
#endif

    /* Or you can change it from drv_cust*/
    if (CheckValidTunerType(u4Default) == TRUE)
        u1DeftType = u4Default;
    
    /* If two of tuner types in eeprom are the same and are valid tuner type, then trust this value, other condition return default type */
    if ((CheckValidTunerType(u1Val[0]) == TRUE) && (u1Val[0]==u1Val[1])){
        u1ResultType = u1Val[0];
    }
    else if((CheckValidTunerType(u1Val[0]) == TRUE) && (u1Val[0]==u1Val[2])){
        u1ResultType = u1Val[0];
    }
    else if((CheckValidTunerType(u1Val[1]) == TRUE) && (u1Val[1]==u1Val[2])){
        u1ResultType = u1Val[1];
    }
    else{
        //Return default tuner type
        u1ResultType = u1DeftType;
    }
    mcSHOW_DBG_MSG(("u1ResultType=0x%x\r\n", u1ResultType));

    return u1ResultType;
}

#endif//#if defined(CHECK_TUNER_TYPE_FROM_EEPROM)


#endif



