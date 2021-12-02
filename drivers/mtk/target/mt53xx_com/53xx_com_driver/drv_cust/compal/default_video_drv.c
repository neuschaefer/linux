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

static UINT8 gbSmartPicIndex = 0;
extern UINT8 bBackLightCtrl;

UINT8 vApiFlashPqGetGammaId(UINT8 bIndex)
{
	UNUSED(bIndex);

	return (0);
}

UINT8 vApiFlashPqGetSceId(UINT8 bIndex)
{
	UNUSED(bIndex);

	return (0);
}

UINT8 vApiFlashPqGetQtyId(UINT8 bIndex)
{
	UNUSED(bIndex);

	return (0);
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
    vDrvBacklightCtrl();
}
void DRVCUST_OutVSyncISR()
{
//    vApiSyncDimming();
}

void DRVCUST_VdoInISR()
{
    return;
}

UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value)
{
    return u4Value<<2;
}

void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
/*
    EXTERN UINT8 GAMMA_256_BRIGHT[3][256];
    EXTERN UINT8 GAMMA_256_MIDDLE[3][256];

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
            //Gamma 0.9
            pwRemapTable=GAMMA_900_TABLE;
            break;
        case 2:
            //Gamma 0.875
            pwRemapTable=GAMMA_875_TABLE;
            break;
        case 3:
            //Gamma 0.850
            pwRemapTable=GAMMA_850_TABLE;
            break;
        case 4:
            //Gamma 0.825
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
                u4Index = pwRemapTable[i] >> 2;
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

    vApiRegisterVideoEvent(PE_ARG_GAMMA, SV_V_GAMMA_MIDDLE, SV_ON);
*/
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
