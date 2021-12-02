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
 * $Date  $
 * $RCSfile: source_table.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#include "hw_vdoin.h"
#include "sv_const.h"
#include "source_table.h"
#include "drv_adcset.h"
#include "drv_video.h"
#include "x_gpio.h"
#include "drv_hdmi.h"
#include "x_printf.h"


/**
 * This table customize NPTV driver layer PIP/POP decoder dependencies
 */
struct VSS_Major_Rules	VSS_major_rule[VSS_MAJORNR]=
{
/*VSS_ATV*/		VSS_TVDDEC1,
/*VSS_ATD*/		VSS_TVDDEC1,
/*VSS_CVBS*/		VSS_TVDDEC1,
/*VSS_SV*/		VSS_TVDDEC1,
/*VSS_YPBPR*/		VSS_HDTVDEC1,
/*VSS_VGACOMP*/		VSS_HDTVDEC1,
/*VSS_VGA*/		VSS_HDTVDEC1,
/*VSS_HDMI*/		VSS_HDMIDEC1,
/*VSS_DVI*/		VSS_HDMIDEC1,
/*VSS_DTV*/		VSS_DTVDEC1,
/*VSS_DTD*/		VSS_DTVDEC1,
/*VSS_SCART*/		VSS_TVDDEC1|VSS_HDTVDEC1,
/*VSS_RESERVE*/		0
};


/**
 * This table customize NPTV driver layer input port maping & avmux
 */
const rPortmap_t portmap_table[] =
{
	{SV_VS_ATV1	,P_C0	,AVMUX(CVBS0,0), 0},
	{SV_VS_ATD1	,P_C0	,AVMUX(CVBS0,0), 0},
	{SV_VS_CVBS1	,P_C1	,AVMUX(CVBS1,0), 0},
	{SV_VS_CVBS2	,P_C2	,AVMUX(CVBS2,0), 0},
	{SV_VS_CVBS3	,P_C3	,AVMUX(CVBS3,0), 0},
	{SV_VS_CVBS4	,P_C4	,AVMUX(CVBS4,0), 0}, 
	{SV_VS_S1	,P_SV0	,AVMUX(SY1,SC1), 0},
	{SV_VS_S2	,P_SV1	,AVMUX(SY0,SC0), 0},
	{SV_VS_S3	,P_SV2	,AVMUX(SY2,SC2), 0},
	{SV_VS_YPbPr1	,P_YP1	,	0, 0},
	{SV_VS_YPbPr2	,P_YP0	,	0, 0},
	{SV_VS_VGACOMP1,P_VGACOMP,	0, 0},
	{SV_VS_VGA	,P_VGA	,	0, 0},
	{SV_VS_HDMI1	,P_DVI	,	0, 0},
	{SV_VS_HDMI2	,P_DVI	,	0, 0},
	{SV_VS_HDMI3	,P_DVI	,	0, 0},
	{SV_VS_DVI1	,P_DVI	,	0, 0},
	{SV_VS_DVI2	,P_DVI	,	0, 0},
	{SV_VS_DVI3	,P_DVI	,	0, 0},
	{SV_VS_DTV1	,P_DT1	,	0, 0},
	{SV_VS_DTD1	,P_DT1	,	0, 0},
	{SV_VS_DTV2	,P_DT2	,	0, 0},
	{SV_VS_DTD2	,P_DT2	,	0, 0},
	{SV_VS_SCART1 	,P_FB0	,AVMUX(CVBS3,SC2), P_YP0},   //SC2 is for internal path
	{SV_VS_SCART2	,P_FB1	,AVMUX(SY1,SC1), P_YP1},
	{SV_VS_MAX	,P_MA	,	0, 0},	/* must have this line*/
};

const UINT8 portmap_table_num=sizeof(portmap_table)/sizeof(rPortmap_t);


static UINT8 vss_portmap(UINT8 src)
{
	UINT8 i;
	for(i=0;i<portmap_table_num; i++)
	{
		if(portmap_table[i].vs_src==src)
			return portmap_table[i].ic_port;
	}
	return P_FA;
}


/**
 * Set external gpio mux, according to Main/Sub path's input video source.
 *
 * @param bMainSrc 
 * @param bSubSrc 
 * @return Main/Sub's input port
 */
UINT16 bDrvGetMapIntMode(UINT8 bMainSrc, UINT8 bSubSrc) 
{
	UINT8 m_major,s_major;
	UINT8 m_port,s_port;

	m_major=VSS_MAJOR(bMainSrc);
	s_major=VSS_MAJOR(bSubSrc);

	if(m_major>VSS_RESERVE)
		m_major=VSS_RESERVE;
	if(s_major>VSS_RESERVE)
		s_major=VSS_RESERVE;
	
	/* 1st support minor mask check */
	m_port=vss_portmap(bMainSrc);
	s_port=vss_portmap(bSubSrc);

	if((m_port==P_FA)||(s_port==P_FA))
	{
		return MS(P_FA, P_FA);
	}

	/* special rule,eg. to support special VGA, now empty*/

	/* resource conflict check */
	if(VSS_major_rule[m_major].hwdec_mask&VSS_major_rule[s_major].hwdec_mask)
	{
        #ifndef CC_SUPPORT_TVE	
		return MS(P_FA, P_FA);
        #endif
	}
	return MS(m_port, s_port);

}


/**
 * Set external gpio mux, according to Main/Sub path's input video source.
 *
 * @param bMainSrc 
 * @param bSubSrc 
 * @return NONE
 */
void vDrvSetExternalMux(UINT8 bMainSrc, UINT8 bSubSrc) 
{
	UINT8 i,bSrc;
	/* INT32 low=0,high=1; */

	for(i=0;i<2;i++)
	{
		bSrc=i?bSubSrc:bMainSrc;
		switch(VSS_MAJOR(bSrc))
		{
			case VSS_HDMI:
			case VSS_DVI:
				_bDviExtSrc = (UINT8)SV_DV_HDMI;
				vHDMISetSwitch(HDMI_SWITCH_1+VSS_MINOR(bSrc));
				break;
			/*
			case VSS_SCART:
				switch(VSS_MINOR(bSrc))
				{
					case 0:	UNUSED(GPIO_Output(39,&low)); 
						break;
					case 1:
					default:	UNUSED(GPIO_Output(39,&high)); 
						break;
				}
				break; 
			*/
			default:
				break;				
		}
	}
}

#if 0

/**
 * @brief SCART input port mapping to CVBS/SV input port
 * @param bInPort P_FB0/P_FB1
 * @return CVBS/SV input port
 */
UINT8 bDrvGetFBCVBS(UINT8 bInPort) 
{
	UNUSED(bInPort);
	return (UINT8)P_SV1;
}

/**
 * @brief SCART input port mapping to RGB input port
 * @param bInPort P_FB0/P_FB1
 * @return RGB input port
 */
UINT8 bDrvGetFBRGB(UINT8 bInPort) 
{
	UNUSED(bInPort);
	return (UINT8)P_YP0;
}
#endif

