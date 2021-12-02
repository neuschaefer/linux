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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: mtk_tve.h $ 
 *
 * Project:
 * --------
 *   MT5391
 *
 * Description:
 * ------------
 *   TVE customization
 *   
 * Author:
 * -------
 *   
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mtk_tve.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _TVE_CUSTOMIZATION_H_
#define _TVE_CUSTOMIZATION_H_


#include "x_typedef.h"
#include "u_common.h"
#include "source_select.h"

/* TVE OUT FORMAT */
#define TVE_OUT_FORMAT 1 // 0:480i 1: 576i

/* TVE OUT Config */
enum
{
	SV_TVE_NA,
	SV_TVE_DAC_BYPASS_CVBS,		
	SV_TVE_DAC_BYPASS_DEMOD,
	SV_TVE_DAC_PIP,
	SV_TVE_DAC_VIDEO_IN, // Video In
	SV_TVE_HW_BYPASS, // Only for TV SCART
	SV_TVE_D2S_BYPASS, // Only for Monitor SCART
	SV_TVE_MAX
};

/* Tuner Type */
enum
{
	SV_TVE_TUNER_ATV_DEFAULT,
	SV_TVE_TUNER_ATV,
	SV_TVE_TUNER_DTV,
	SV_TVE_TUNER_MAX,
        SV_TVE_TUNER_LAST_TYPE = SV_TVE_TUNER_MAX 
};

/* Scart Out ID */
enum
{
	SV_TVE_SCART_OUT_1,
	SV_TVE_SCART_OUT_2,
	SV_TVE_SCART_OUT_MAX
};

/* TVE Src Type */
enum
{
	SV_TVE_SRC_ORI,
	SV_TVE_SRC_CVBS0,
	SV_TVE_SRC_CVBS1,
	SV_TVE_SRC_PREV,	
	SV_TVE_SRC_MAX
};

typedef struct {
	UINT8  type;
	UINT8  src;
	UINT8  reset_src_change;	
}  RTVE_OUT_T;

/* SCART Src type */
#define TVE_SCART_DECODER 7

typedef struct {
	RTVE_OUT_T tve_out1;
	RTVE_OUT_T tve_out2;	
       UINT8 tve_out_fmt_576i;	
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
       UINT8 remap_msrc;
#endif	
}  RTVE_CONFIG_T;

#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
const UINT8 TVE_MAIN_SRC_REMAP_TABLE[] = 
{
    FALSE, /* SV_VD_NA */
    FALSE, /* SV_VD_TVD3D */
    FALSE, /* SV_VD_YPBPR */
    FALSE, /* SV_VD_VGA */
    SV_VS_CVBS1, /* SV_VD_DVI */    
    SV_VS_CVBS1, /* SV_VD_MPEGHD */
    SV_VS_CVBS1, /* SV_VD_MPEGSD */    	
    FALSE, /* TVE_SCART_DECODER */    
};
#endif


/**
 * @brief TVE ATV CONFIG TABLE
 */
// 20080616@gellmann:MT5362 support HW bypass for SCART1 and not support Demod bypass except RF tuner
#ifdef USE_ATD_IF_DEMOD
const RTVE_CONFIG_T TVE_ATV_CONFIG_TABLE[] = 
{
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}}, /* SV_VD_NA */
    {{SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_TVD3D */
    {{SV_TVE_NA,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_YPBPR */
    {{SV_TVE_NA,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_VGA */
    {{SV_TVE_NA,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_DVI */    
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_MPEGHD */
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_MPEGSD */    
    {{SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_ORI,TRUE}} /* TVE_SCART_DECODER */        
};
#else
const RTVE_CONFIG_T TVE_ATV_CONFIG_TABLE[] = 
{
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}}, /* SV_VD_NA */
    {{SV_TVE_HW_BYPASS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_TVD3D */
    {{SV_TVE_HW_BYPASS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_PIP,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_YPBPR */
    {{SV_TVE_HW_BYPASS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_PIP,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_VGA */
    {{SV_TVE_HW_BYPASS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_PIP,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_DVI */    
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_DAC_PIP,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_MPEGHD */
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_DAC_PIP,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_MPEGSD */    
    {{SV_TVE_HW_BYPASS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_ORI,TRUE}} /* TVE_SCART_DECODER */        
};
#endif

/**
 * @brief TVE DTV CONFIG TABLE
 */

const RTVE_CONFIG_T TVE_DTV_CONFIG_TABLE[] = 
{
     {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}}, /* SV_VD_NA */
    {{SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_TVD3D */
    {{SV_TVE_NA,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_YPBPR */
    {{SV_TVE_NA,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_VGA */
    {{SV_TVE_NA,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_DVI */    
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_MPEGHD */
    {{SV_TVE_NA,SV_TVE_SRC_ORI,FALSE}, {SV_TVE_NA,SV_TVE_SRC_ORI,TRUE}}, /* SV_VD_MPEGSD */    
    {{SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_CVBS1,FALSE}, {SV_TVE_DAC_BYPASS_CVBS,SV_TVE_SRC_ORI,TRUE}} /* TVE_SCART_DECODER */              
};

/**
 * @brief vApiTVEGetConfig(UINT8 u1TunerType, UINT8 u1DecType, rtve_config_t* ptTveConfig )
 * @param  u1TunerType: SV_TVE_TUNER_ATV/SV_TVE_TUNER_DTV
 * @param u1DecType: Decoder Type
 * @param ptTveConfig: TVE Out1 Type/Src/Reset
 * @retval void
 */

void vApiTVEGetConfig(UINT8 u1TunerType, UINT8 u1DecType, RTVE_CONFIG_T* ptTveConfig )
{
        static UINT8 u1LastTunerType = SV_TVE_TUNER_ATV;
        UINT8 u1OrigTunerType = u1TunerType;
        if(u1TunerType == SV_TVE_TUNER_LAST_TYPE)
        {
            u1TunerType = u1LastTunerType;
        }

        u1LastTunerType = u1TunerType;

	if (u1TunerType <= SV_TVE_TUNER_ATV)
	{
	    ptTveConfig->tve_out1.type	= TVE_ATV_CONFIG_TABLE[u1DecType].tve_out1.type;
	    ptTveConfig->tve_out1.src = TVE_ATV_CONFIG_TABLE[u1DecType].tve_out1.src;
	    ptTveConfig->tve_out1.reset_src_change = TVE_ATV_CONFIG_TABLE[u1DecType].tve_out1.reset_src_change;
	    ptTveConfig->tve_out2.type	= TVE_ATV_CONFIG_TABLE[u1DecType].tve_out2.type;
	    ptTveConfig->tve_out2.src = TVE_ATV_CONFIG_TABLE[u1DecType].tve_out2.src;
	    ptTveConfig->tve_out2.reset_src_change = TVE_ATV_CONFIG_TABLE[u1DecType].tve_out2.reset_src_change;	    
#ifdef USE_ATD_IF_DEMOD
            if(u1OrigTunerType == SV_TVE_TUNER_ATV)
            {
            	if(bGetSignalType(SV_VP_MAIN)==SV_ST_TV)
	        	ptTveConfig->tve_out1.type	= SV_TVE_DAC_BYPASS_DEMOD;
		else if((bGetSignalType(SV_VP_MAIN)==SV_ST_AV)||(bGetSignalType(SV_VP_MAIN)==SV_ST_SV))
			ptTveConfig->tve_out1.type	= SV_TVE_DAC_BYPASS_CVBS;
	        ptTveConfig->tve_out2.type	= SV_TVE_DAC_BYPASS_DEMOD;
            }
#endif
	}
	else
	{
	    ptTveConfig->tve_out1.type	= TVE_DTV_CONFIG_TABLE[u1DecType].tve_out1.type;
	    ptTveConfig->tve_out1.src = TVE_DTV_CONFIG_TABLE[u1DecType].tve_out1.src;
	    ptTveConfig->tve_out1.reset_src_change = TVE_DTV_CONFIG_TABLE[u1DecType].tve_out1.reset_src_change;
	    ptTveConfig->tve_out2.type	= TVE_DTV_CONFIG_TABLE[u1DecType].tve_out2.type;
	    ptTveConfig->tve_out2.src = TVE_DTV_CONFIG_TABLE[u1DecType].tve_out2.src;
	    ptTveConfig->tve_out2.reset_src_change = TVE_DTV_CONFIG_TABLE[u1DecType].tve_out2.reset_src_change;	    	
	}
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER_SLT
       ptTveConfig->remap_msrc = TVE_MAIN_SRC_REMAP_TABLE[u1DecType];
#endif	
       ptTveConfig->tve_out_fmt_576i = TVE_OUT_FORMAT;
	
}

/**
 * @brief vApiTVEOutPathSelect(UINT8 u1OutID, UINT8 u1OutType)
 * @param  u1OutID: SV_TVE_SCART_OUT_1/SV_TVE_SCART_OUT_2
 * @param u1OutType: TVE Out Type
 * @retval void
 */

void vApiTVEOutPathSelect(UINT8 u1OutID, UINT8 u1OutType)
{	
	if (u1OutID == SV_TVE_SCART_OUT_1)
	{
	    switch (u1OutType)
	    {
	      case SV_TVE_DAC_BYPASS_CVBS:
	      case SV_TVE_DAC_BYPASS_DEMOD:
	      case SV_TVE_DAC_PIP:
	      case SV_TVE_DAC_VIDEO_IN:
//	      		 GPIO_SetOut(16, 1);
	      	        break;
	      case SV_TVE_HW_BYPASS:
//	      		 GPIO_SetOut(16, 0);
	      	        break;
	      case SV_TVE_D2S_BYPASS:
	      case SV_TVE_NA:
             case SV_TVE_MAX:
             default:
             	        break;	      	        
	    }
	}
	else if (u1OutID == SV_TVE_SCART_OUT_2)
	{
	    switch (u1OutType)
	    {
	      case SV_TVE_DAC_BYPASS_CVBS:
	      case SV_TVE_DAC_BYPASS_DEMOD:
	      case SV_TVE_DAC_PIP:
	      case SV_TVE_DAC_VIDEO_IN:
	      case SV_TVE_D2S_BYPASS:
	      case SV_TVE_HW_BYPASS:
	      case SV_TVE_NA:
             case SV_TVE_MAX:
             default:
             	        break;	      	        
	    }	
	}	
}

#endif // _TVE_CUSTOMIZATION_H_

