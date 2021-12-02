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
#include "drv_pq_cust.h"

static UINT8 gbSmartPicIndex = 0;
UINT8 SCEIndex = 0;
extern UINT8 bBackLightCtrl;

// Backlight PWM V-Sync.
///////////////////////////////////////////////////////
UINT16 ui2BacklightResult;
UINT32 DRVCUST_AutoBacklightISR(void);
UINT32 DRVCUST_AutoBacklightProc(void);
void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight);

UINT32 DRVCUST_AutoBacklightISR(void)
{
    // other ISR routine.
    //vApiSetPanelBright(ui2BacklightResult);

    return 0;
}

UINT32 DRVCUST_AutoBacklightProc(void)
{

    // SONY backlight algorithm
    // ui2BacklightResult = 128;

    return 0;
}

void DRVCUST_AutoBacklightUpdate(AUTO_BACKLIGHT_SETS_INFO_T *aBLight)
{
    //SONY udpate auto backlight algo info.
}
///////////////////////////////////////////////////////

INT8 Clarity_value=0;
void DRVCUST_ClarityUpdate(INT8 u1Value)
{
    // SONY implenment clarity.
}

UINT8 bApiGetPanelIDFromEEPROM(void)
{
    //implement flash PQ ID for each panel
    return 0;
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

void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo)
{
#if 1
    UINT8 bCTIdx;//, bAmient;
    UINT16 wRBiasGain, wGBiasGain, wBBiasGain;
    UINT16 wRBiasOffset, wGBiasOffset, wBBiasOffset;
    INT16 aai2_clr_gain[4][3] =
    {
        /* R,  G,  B*/
        {624,616,680},      /* Cool */
        {644,616,616},      /* Neutral */
        {660,592,436},      /* Warm */
        {680,680,420}       /* Warm2 */
    };

    INT16 aai2_clr_offset[4][3] =
    {
        /* R,  G,  B*/
        {436,448,448},      /* Cool */
        {444,448,448},      /* Neutral*/
        {439,448,444},      /* Warm */
        {439,448,444}       /* Warm2 */
    };

    bCTIdx = (aCTInfo->ui1CTIndex>3)?3:aCTInfo->ui1CTIndex;
    //bAmient = aCTInfo->ui1AmbientSensorOnOff;
    wRBiasGain = aCTInfo->ui1Bias_RGain+aai2_clr_gain[bCTIdx][0];
    wGBiasGain = aCTInfo->ui1Bias_GGain+aai2_clr_gain[bCTIdx][1];
    wBBiasGain = aCTInfo->ui1Bias_BGain+aai2_clr_gain[bCTIdx][2];
    wRBiasOffset= aCTInfo->ui1Bias_ROffset+aai2_clr_offset[bCTIdx][0]-0x200;
    wGBiasOffset = aCTInfo->ui1Bias_GOffset+aai2_clr_offset[bCTIdx][1]-0x200;
    wBBiasOffset = aCTInfo->ui1Bias_BOffset+aai2_clr_offset[bCTIdx][2]-0x200;

    //Read Base RGB Gain/Offset Value from NVM

    //Calculation

    //Write final result to Reg
    vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, wRBiasGain);
    vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, wGBiasGain);
    vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, wBBiasGain);

    vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, wRBiasOffset);
    vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, wGBiasOffset);
    vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, wBBiasOffset);
#endif
}
//Gamma.
///////////////////////////////////////////////////////
UINT8 vApiFlashPqGetGammaId(UINT8 bIndex)
{
	UNUSED(bIndex);

    return 0;
	//return (bApiGetPanelIDFromEEPROM());
}

void DRVCUST_SET_GAMMA(void)
{
    EXTERN UINT8 GAMMA_256_MIDDLE[3][256];
    EXTERN void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff);

    //Check about NVM id

    //Read Data from NVM

    //Interpolating data to 256 points

    //Write the data to GAMMA_256_MIDDLE

    vApiRegisterVideoEvent(PE_ARG_GAMMA, SV_V_GAMMA_MIDDLE, SV_ON);
}
void vApiFlashPqSetGammaId(UINT8 bIndex)
{

}
///////////////////////////////////////////////////////

UINT8 vApiFlashPqGetQtyId(UINT8 bIndex)
{
	UNUSED(bIndex);

	return (0);
//    return (bApiGetPanelIDFromEEPROM());
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
    return;
}

void DRVCUST_VdoModeChgDone(UINT8 bPath)
{
    return;
}

void DRVCUST_VideoMainloop()
{
    DRVCUST_AutoBacklightProc();
}
void DRVCUST_OutVSyncISR()
{
    DRVCUST_AutoBacklightISR();
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
    return 2;
}

/* @ Auto NR noise level implement
 * @ return noise level
 */
UINT8 DRVCUST_NoiseLevel(void)
{
    UINT8 bNoiseLevel = NM_INVALIDLEVEL;
    UINT8 bRFNMLevel;

    if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_TV))// TVD RF source
    {

        bRFNMLevel = bTvdSignalStrengthLevel(1);   //RF level 0 ~ 80

        if(bRFNMLevel > 75)
        {
            bNoiseLevel = NM_LEVEL1_3;
        }
        else if(bRFNMLevel > 69)
        {
            bNoiseLevel = NM_LEVEL2;
        }
        else if(bRFNMLevel > 59)
        {
            bNoiseLevel = NM_LEVEL3;
        }
        else if(bRFNMLevel > 55)
        {
            bNoiseLevel = NM_LEVEL4;
        }
        else
        {
            bNoiseLevel = NM_LEVEL5;
        }
    }
    else
    {
        bNoiseLevel = vDrvNMLevel();   //content NR, Noise level 0 ~ 4
    }

    return bNoiseLevel;
}

static const  UINT16  PANEL_GAMMA_950_TABLE[256]=
{
    0 ,5 ,10 ,15 ,20 ,24 ,29 ,34 ,38 ,43 ,47 ,51 ,56 ,60 ,65 ,69 ,
    74 ,78 ,82 ,87 ,91 ,95 ,99 ,104 ,108 ,112 ,117 ,121 ,125 ,129 ,134 ,138 ,
    142 ,146 ,150 ,155 ,159 ,163 ,167 ,171 ,176 ,180 ,184 ,188 ,192 ,196 ,200 ,205 ,
    209 ,213 ,217 ,221 ,225 ,229 ,233 ,238 ,242 ,246 ,250 ,254 ,258 ,262 ,266 ,270 ,
    274 ,278 ,282 ,287 ,291 ,295 ,299 ,303 ,307 ,311 ,315 ,319 ,323 ,327 ,331 ,335 ,
    339 ,343 ,347 ,351 ,355 ,359 ,363 ,367 ,371 ,375 ,379 ,383 ,387 ,391 ,395 ,399 ,
    403 ,407 ,411 ,415 ,419 ,423 ,427 ,431 ,435 ,439 ,443 ,447 ,451 ,455 ,459 ,463 ,
    467 ,471 ,475 ,479 ,483 ,487 ,491 ,494 ,498 ,502 ,506 ,510 ,514 ,518 ,522 ,526 ,
    530 ,534 ,538 ,542 ,546 ,550 ,554 ,557 ,561 ,565 ,569 ,573 ,577 ,581 ,585 ,589 ,
    593 ,597 ,601 ,604 ,608 ,612 ,616 ,620 ,624 ,628 ,632 ,636 ,640 ,643 ,647 ,651 ,
    655 ,659 ,663 ,667 ,671 ,675 ,678 ,682 ,686 ,690 ,694 ,698 ,702 ,706 ,709 ,713 ,
    717 ,721 ,725 ,729 ,733 ,737 ,740 ,744 ,748 ,752 ,756 ,760 ,764 ,767 ,771 ,775 ,
    779 ,783 ,787 ,791 ,794 ,798 ,802 ,806 ,810 ,814 ,817 ,821 ,825 ,829 ,833 ,837 ,
    841 ,844 ,848 ,852 ,856 ,860 ,864 ,867 ,871 ,875 ,879 ,883 ,887 ,890 ,894 ,898 ,
    902 ,906 ,909 ,913 ,917 ,921 ,925 ,929 ,932 ,936 ,940 ,944 ,948 ,951 ,955 ,959 ,
    963 ,967 ,971 ,974 ,978 ,982 ,986 ,990 ,993 ,997 ,1001 ,1005 ,1009 ,1012 ,1016 ,1020
};

static const  UINT16  PANEL_GAMMA_925_TABLE[256]=
{

0 ,6 ,12 ,17 ,22 ,27 ,32 ,37 ,41 ,46 ,51 ,56 ,60 ,65 ,70 ,74 ,
79 ,83 ,88 ,92 ,97 ,101 ,106 ,110 ,115 ,119 ,123 ,128 ,132 ,137 ,141 ,145 ,
150 ,154 ,158 ,162 ,167 ,171 ,175 ,180 ,184 ,188 ,192 ,197 ,201 ,205 ,209 ,213 ,
218 ,222 ,226 ,230 ,234 ,239 ,243 ,247 ,251 ,255 ,259 ,263 ,268 ,272 ,276 ,280 ,
284 ,288 ,292 ,296 ,300 ,304 ,309 ,313 ,317 ,321 ,325 ,329 ,333 ,337 ,341 ,345 ,
349 ,353 ,357 ,361 ,365 ,369 ,373 ,377 ,381 ,385 ,389 ,393 ,397 ,401 ,405 ,409 ,
413 ,417 ,421 ,425 ,429 ,433 ,437 ,441 ,445 ,449 ,453 ,457 ,461 ,465 ,469 ,473 ,
477 ,480 ,484 ,488 ,492 ,496 ,500 ,504 ,508 ,512 ,516 ,520 ,524 ,527 ,531 ,535 ,
539 ,543 ,547 ,551 ,555 ,559 ,562 ,566 ,570 ,574 ,578 ,582 ,586 ,590 ,593 ,597 ,
601 ,605 ,609 ,613 ,617 ,621 ,624 ,628 ,632 ,636 ,640 ,644 ,647 ,651 ,655 ,659 ,
663 ,667 ,670 ,674 ,678 ,682 ,686 ,690 ,693 ,697 ,701 ,705 ,709 ,712 ,716 ,720 ,
724 ,728 ,731 ,735 ,739 ,743 ,747 ,750 ,754 ,758 ,762 ,766 ,769 ,773 ,777 ,781 ,
785 ,788 ,792 ,796 ,800 ,803 ,807 ,811 ,815 ,818 ,822 ,826 ,830 ,834 ,837 ,841 ,
845 ,849 ,852 ,856 ,860 ,864 ,867 ,871 ,875 ,879 ,882 ,886 ,890 ,894 ,897 ,901 ,
905 ,908 ,912 ,916 ,920 ,923 ,927 ,931 ,935 ,938 ,942 ,946 ,949 ,953 ,957 ,961 ,
964 ,968 ,972 ,976 ,979 ,983 ,987 ,990 ,994 ,998 ,1001 ,1005 ,1009 ,1013 ,1016 ,1020

};



static const  UINT16  PANEL_GAMMA_900_TABLE[256]=
{
    0 ,7 ,13 ,19 ,24 ,30 ,35 ,40 ,45 ,50 ,55 ,60 ,65 ,70 ,75 ,80 ,
    84 ,89 ,94 ,99 ,103 ,108 ,112 ,117 ,122 ,126 ,131 ,135 ,140 ,144 ,149 ,153 ,
    158 ,162 ,166 ,171 ,175 ,180 ,184 ,188 ,193 ,197 ,201 ,206 ,210 ,214 ,218 ,223 ,
    227 ,231 ,235 ,240 ,244 ,248 ,252 ,256 ,261 ,265 ,269 ,273 ,277 ,282 ,286 ,290 ,
    294 ,298 ,302 ,306 ,310 ,315 ,319 ,323 ,327 ,331 ,335 ,339 ,343 ,347 ,351 ,355 ,
    359 ,363 ,367 ,371 ,375 ,379 ,383 ,388 ,392 ,396 ,400 ,404 ,407 ,411 ,415 ,419 ,
    423 ,427 ,431 ,435 ,439 ,443 ,447 ,451 ,455 ,459 ,463 ,467 ,471 ,475 ,479 ,483 ,
    486 ,490 ,494 ,498 ,502 ,506 ,510 ,514 ,518 ,521 ,525 ,529 ,533 ,537 ,541 ,545 ,
    549 ,552 ,556 ,560 ,564 ,568 ,572 ,575 ,579 ,583 ,587 ,591 ,595 ,598 ,602 ,606 ,
    610 ,614 ,617 ,621 ,625 ,629 ,633 ,636 ,640 ,644 ,648 ,652 ,655 ,659 ,663 ,667 ,
    671 ,674 ,678 ,682 ,686 ,689 ,693 ,697 ,701 ,704 ,708 ,712 ,716 ,719 ,723 ,727 ,
    731 ,734 ,738 ,742 ,746 ,749 ,753 ,757 ,760 ,764 ,768 ,772 ,775 ,779 ,783 ,786 ,
    790 ,794 ,798 ,801 ,805 ,809 ,812 ,816 ,820 ,823 ,827 ,831 ,834 ,838 ,842 ,845 ,
    849 ,853 ,856 ,860 ,864 ,867 ,871 ,875 ,878 ,882 ,886 ,889 ,893 ,897 ,900 ,904 ,
    908 ,911 ,915 ,919 ,922 ,926 ,930 ,933 ,937 ,940 ,944 ,948 ,951 ,955 ,959 ,962 ,
    966 ,969 ,973 ,977 ,980 ,984 ,988 ,991 ,995 ,998 ,1002 ,1006 ,1009 ,1013 ,1016 ,1020
};


static const  UINT16  PANEL_GAMMA_875_TABLE[256]=
{

0 ,8 ,15 ,21 ,27 ,33 ,38 ,44 ,49 ,55 ,60 ,65 ,70 ,75 ,80 ,85 ,
90 ,95 ,100 ,105 ,110 ,115 ,120 ,124 ,129 ,134 ,138 ,143 ,148 ,152 ,157 ,161 ,
166 ,170 ,175 ,179 ,184 ,188 ,193 ,197 ,202 ,206 ,210 ,215 ,219 ,224 ,228 ,232 ,
237 ,241 ,245 ,249 ,254 ,258 ,262 ,266 ,271 ,275 ,279 ,283 ,288 ,292 ,296 ,300 ,
304 ,308 ,313 ,317 ,321 ,325 ,329 ,333 ,337 ,341 ,346 ,350 ,354 ,358 ,362 ,366 ,
370 ,374 ,378 ,382 ,386 ,390 ,394 ,398 ,402 ,406 ,410 ,414 ,418 ,422 ,426 ,430 ,
434 ,438 ,442 ,446 ,450 ,454 ,458 ,461 ,465 ,469 ,473 ,477 ,481 ,485 ,489 ,493 ,
497 ,500 ,504 ,508 ,512 ,516 ,520 ,524 ,527 ,531 ,535 ,539 ,543 ,547 ,550 ,554 ,
558 ,562 ,566 ,569 ,573 ,577 ,581 ,585 ,588 ,592 ,596 ,600 ,604 ,607 ,611 ,615 ,
619 ,622 ,626 ,630 ,634 ,637 ,641 ,645 ,649 ,652 ,656 ,660 ,664 ,667 ,671 ,675 ,
678 ,682 ,686 ,690 ,693 ,697 ,701 ,704 ,708 ,712 ,715 ,719 ,723 ,726 ,730 ,734 ,
737 ,741 ,745 ,748 ,752 ,756 ,759 ,763 ,767 ,770 ,774 ,778 ,781 ,785 ,788 ,792 ,
796 ,799 ,803 ,807 ,810 ,814 ,817 ,821 ,825 ,828 ,832 ,835 ,839 ,843 ,846 ,850 ,
853 ,857 ,861 ,864 ,868 ,871 ,875 ,879 ,882 ,886 ,889 ,893 ,896 ,900 ,904 ,907 ,
911 ,914 ,918 ,921 ,925 ,928 ,932 ,935 ,939 ,943 ,946 ,950 ,953 ,957 ,960 ,964 ,
967 ,971 ,974 ,978 ,981 ,985 ,988 ,992 ,995 ,999 ,1002 ,1006 ,1009 ,1013 ,1016 ,1020

};



static const  UINT16  PANEL_GAMMA_850_TABLE[256]=
{
    0 ,9 ,17 ,23 ,30 ,36 ,42 ,48 ,54 ,59 ,65 ,71 ,76 ,81 ,87 ,92 ,
    97 ,102 ,107 ,112 ,117 ,122 ,127 ,132 ,137 ,142 ,146 ,151 ,156 ,161 ,165 ,170 ,
    175 ,179 ,184 ,189 ,193 ,198 ,202 ,207 ,211 ,216 ,220 ,225 ,229 ,233 ,238 ,242 ,
    247 ,251 ,255 ,260 ,264 ,268 ,273 ,277 ,281 ,285 ,290 ,294 ,298 ,302 ,307 ,311 ,
    315 ,319 ,323 ,327 ,332 ,336 ,340 ,344 ,348 ,352 ,356 ,360 ,365 ,369 ,373 ,377 ,
    381 ,385 ,389 ,393 ,397 ,401 ,405 ,409 ,413 ,417 ,421 ,425 ,429 ,433 ,437 ,441 ,
    445 ,449 ,452 ,456 ,460 ,464 ,468 ,472 ,476 ,480 ,484 ,488 ,491 ,495 ,499 ,503 ,
    507 ,511 ,515 ,518 ,522 ,526 ,530 ,534 ,537 ,541 ,545 ,549 ,553 ,556 ,560 ,564 ,
    568 ,572 ,575 ,579 ,583 ,587 ,590 ,594 ,598 ,602 ,605 ,609 ,613 ,616 ,620 ,624 ,
    628 ,631 ,635 ,639 ,642 ,646 ,650 ,653 ,657 ,661 ,664 ,668 ,672 ,675 ,679 ,683 ,
    686 ,690 ,694 ,697 ,701 ,705 ,708 ,712 ,715 ,719 ,723 ,726 ,730 ,733 ,737 ,741 ,
    744 ,748 ,751 ,755 ,759 ,762 ,766 ,769 ,773 ,776 ,780 ,784 ,787 ,791 ,794 ,798 ,
    801 ,805 ,808 ,812 ,816 ,819 ,823 ,826 ,830 ,833 ,837 ,840 ,844 ,847 ,851 ,854 ,
    858 ,861 ,865 ,868 ,872 ,875 ,879 ,882 ,886 ,889 ,893 ,896 ,900 ,903 ,907 ,910 ,
    914 ,917 ,921 ,924 ,927 ,931 ,934 ,938 ,941 ,945 ,948 ,952 ,955 ,958 ,962 ,965 ,
    969 ,972 ,976 ,979 ,982 ,986 ,989 ,993 ,996 ,1000 ,1003 ,1006 ,1010 ,1013 ,1017 ,1020
};

static const  UINT16  PANEL_GAMMA_825_TABLE[256]=
{
    0 ,11 ,19 ,26 ,33 ,40 ,46 ,53 ,59 ,65 ,71 ,76 ,82 ,88 ,93 ,99 ,
    104 ,109 ,114 ,120 ,125 ,130 ,135 ,140 ,145 ,150 ,155 ,160 ,165 ,170 ,175 ,179 ,
    184 ,189 ,193 ,198 ,203 ,207 ,212 ,217 ,221 ,226 ,230 ,235 ,239 ,244 ,248 ,253 ,
    257 ,262 ,266 ,270 ,275 ,279 ,283 ,288 ,292 ,296 ,301 ,305 ,309 ,313 ,318 ,322 ,
    326 ,330 ,334 ,339 ,343 ,347 ,351 ,355 ,359 ,363 ,368 ,372 ,376 ,380 ,384 ,388 ,
    392 ,396 ,400 ,404 ,408 ,412 ,416 ,420 ,424 ,428 ,432 ,436 ,440 ,444 ,448 ,452 ,
    456 ,460 ,463 ,467 ,471 ,475 ,479 ,483 ,487 ,491 ,494 ,498 ,502 ,506 ,510 ,514 ,
    517 ,521 ,525 ,529 ,533 ,536 ,540 ,544 ,548 ,551 ,555 ,559 ,563 ,566 ,570 ,574 ,
    578 ,581 ,585 ,589 ,592 ,596 ,600 ,604 ,607 ,611 ,615 ,618 ,622 ,626 ,629 ,633 ,
    637 ,640 ,644 ,648 ,651 ,655 ,658 ,662 ,666 ,669 ,673 ,676 ,680 ,684 ,687 ,691 ,
    694 ,698 ,702 ,705 ,709 ,712 ,716 ,719 ,723 ,726 ,730 ,734 ,737 ,741 ,744 ,748 ,
    751 ,755 ,758 ,762 ,765 ,769 ,772 ,776 ,779 ,783 ,786 ,790 ,793 ,797 ,800 ,804 ,
    807 ,811 ,814 ,817 ,821 ,824 ,828 ,831 ,835 ,838 ,842 ,845 ,848 ,852 ,855 ,859 ,
    862 ,866 ,869 ,872 ,876 ,879 ,883 ,886 ,889 ,893 ,896 ,900 ,903 ,906 ,910 ,913 ,
    917 ,920 ,923 ,927 ,930 ,933 ,937 ,940 ,943 ,947 ,950 ,954 ,957 ,960 ,964 ,967 ,
    970 ,974 ,977 ,980 ,984 ,987 ,990 ,994 ,997 ,1000 ,1003 ,1007 ,1010 ,1013 ,1017 ,1020
};



void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
    EXTERN UINT8 GAMMA_256_BRIGHT[3][256];
    EXTERN UINT8 GAMMA_256_MIDDLE[3][256];





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
            pwRemapTable=PANEL_GAMMA_950_TABLE;
            break;
        case 2:
            pwRemapTable=PANEL_GAMMA_925_TABLE;
            break;
        case 3:
            pwRemapTable=PANEL_GAMMA_900_TABLE;
            break;
        case 4:
            pwRemapTable=PANEL_GAMMA_875_TABLE;
            break;
        case 5:
            pwRemapTable=PANEL_GAMMA_850_TABLE;
            break;
        case 6:
            pwRemapTable=PANEL_GAMMA_825_TABLE;
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
    UNUSED(u1State);
    UNUSED(u2Duty);
}

void DRVCUST_SetBacklightDuty(UINT16 u2Duty)
{
    vApiSetPanelBright(u2Duty);
}

