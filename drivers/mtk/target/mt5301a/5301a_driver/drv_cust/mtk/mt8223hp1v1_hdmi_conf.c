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

/**
 * @file HDMI switch customization file
 *
 * @author josh.pan@mediatek.com
 */
#include "x_gpio.h"
#include "x_printf.h"
#include "x_pinmux.h"

#include "hw_vdoin.h"
#include "hw_hdmi.h"

#include "hw_pdwnc.h"

#include "drv_hdmi.h"
#include "drv_video.h"
#include "drv_adcset.h"

#include "sv_const.h"
#include "source_table.h"

//Change the following definition for gpio customization
#define HDMI_GPIO_HPD1 26
#define HDMI_GPIO_HPD2 26
#define HDMI_GPIO_HPD3 214

#define HDMI_SW_CTRL1 27
// switch ctrl 2 is tied high always.

//====================//
void vHDMIHPDHigh(UINT8 bSwitch)
{
    INT32 dval = 0;

    switch (bSwitch)
    {
    case HDMI_SWITCH_1:
        GPIO_Output(HDMI_GPIO_HPD1, &dval);
        break;

    case HDMI_SWITCH_2:
        GPIO_Output(HDMI_GPIO_HPD2, &dval);
        break;

    case HDMI_SWITCH_3:
        GPIO_Output(HDMI_GPIO_HPD3, &dval);
        break;

    default: /* disable all */
        break;
    }
}

//====================//
void vHDMIHPDLow(UINT8 bSwitch)
{
    INT32 dval = 1;

    switch (bSwitch)
    {
	    case HDMI_SWITCH_1:
	        GPIO_Output(HDMI_GPIO_HPD1, &dval);
	        break;

	    case HDMI_SWITCH_2:
	        GPIO_Output(HDMI_GPIO_HPD2, &dval);
	        break;

	    case HDMI_SWITCH_3:
	        GPIO_Output(HDMI_GPIO_HPD3, &dval);
	        break;

	    default: /* disable all */
	        break;
    }
}

//====================//
UINT8 bHDMIPort5VStatus(UINT8 port)
{
	switch (port)
	{
		case HDMI_SWITCH_1:
			return RegReadFldAlign(SRST, PWR5V_RX1);
		case HDMI_SWITCH_2:
			return GPIO_Input(218);  // mtk70117 modify "PINMUX_HDMI_HPD1-->218"
		case HDMI_SWITCH_3:
			return RegReadFldAlign(SRST, PWR5V_RX2);
		default:
			return 0;
	}
}

//====================//
void vHDMISwitchInit(void)
{
	INT32  GpioMode;
	GpioMode = 0;
	
	vRegWriteFldAlign(PDWNC_PADCFG3,1,FLD_PWR5V_1_PD);
	vRegWriteFldAlign(PDWNC_PADCFG3,0,FLD_PWR5V_1_SMT);
	GPIO_Enable(218, &GpioMode);  // config GPIO218 to input-mode, jiewen
}

//====================//
void vHDMISetSwitch(UINT8 bSwitch)
{
    UINT8 i = 0;
    INT32 dval;
	
    if (_bHDMICurrSwitch != bSwitch)
    {
        _bHDMICurrSwitch = bSwitch;

#if MUTE_TEMP
        vHDMIVideoOutOff();
#endif

        switch (bSwitch)
        {
        case HDMI_SWITCH_1:
			// external port switching
            dval = 0;
            GPIO_Output(HDMI_SW_CTRL1,&dval);

			// internal port switching
            vRegWriteFldAlign(ANA_INTF_2, 0x2, RG_HDMI_SW_EN);
            vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_HDMI_SEL);
            break;

        case HDMI_SWITCH_2:
			// external port switching
            dval = 1;
            GPIO_Output(HDMI_SW_CTRL1,&dval);

			// internal port switching
            vRegWriteFldAlign(ANA_INTF_2, 0x2, RG_HDMI_SW_EN);
            vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_HDMI_SEL);
            break;

        case HDMI_SWITCH_3:
			// external port switching
			// direct in
            			
			// internal port switching
            vRegWriteFldAlign(ANA_INTF_2, 0x4, RG_HDMI_SW_EN);
            vIO32WriteFldAlign(PDWNC_PDMISC, 0x2, FLD_HDMI_SEL);
            break;

        default: /* disable all */
            vRegWriteFldAlign(ANA_INTF_2, 0x0, RG_HDMI_SW_EN);
            break;
        }

		// HPD control
		vHDMIHPDLow(bSwitch);

		// for supporting CEC feature, set HPD high for unselected ports.
		for ( i = HDMI_SWITCH_1 ; i <=  HDMI_SWITCH_6; i++)
		{
			if (i != bSwitch)
			{
				vHDMIHPDHigh(i);
			}
		}

        _bHDMIState = HDMI_STATE_INIT;
    }
}

