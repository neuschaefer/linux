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
 * $RCSfile: drv_pwm.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "drv_pwm.h"
#include "x_hal_5381.h"
#include "x_pinmux.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "drv_tdtv_drvif.h"
#include "panel.h"

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 * This driver control the PWM0 & PWM1 output frequence and duty cycle
 * duty cycle = bDutyON/bDutyAll;
 * PWM freq.(in 8202) = 27MHz/(bDutyAll * bTimeBase * 8 clk)
 * PWM freq.(in 5371) = 125MHz/(bDutyAll+1 * bTimeBase+1)
 * PWM freq.(in 538x) = (CPUclk/2) / ((PWMP+1)x(PWMRSN+1))
 *
 * @param bSrc      PWM source (SrcPWM0/1/2)
 * @param bTimeBase PWMP, PWM basic unit = (0~255) * 8 clk
 * @param bDutyON   PWMH, ON duty width = (0~255) * bTimeBase
 * @param bDutyAll  PWMRSN, Period width = (0~255) * bTimeBase
 * @return void
 *
 * @example  vDrvSetPWM(SrcPWM0, 6, u4DutyON, 255);
 *           Use PWM0 with 2000Hz; Duty Cycle = u4DutyON/255
 */



void vDrvSetPWM(UINT32 u4Src, UINT32 u4TimeBase, UINT32 u4DutyON,
                UINT32 u4DutyAll)
{
    UINT32 u4Reg = REG_RW_PWM1, u4Value;

    switch (u4Src)
    {
    case SrcPWM0:
        u4Reg = REG_RW_PWM0;
        if (DRVCUST_InitGet(ePWM0Port) == PAD_PWM0_PWM0)
        {
            #ifdef CC_SCPOS_3DTV_SUPPORT
            #ifndef CC_MTK_LOADER
                if(u4DrvTDTVIsScanningBLEnabled())
                {
                    UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION3)); //Tuner 30V Boost-up PWM in BGA
                }
                else
                {
                    UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
                }
            #else
                UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
            #endif
            #else
                UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
            #endif
        }
        else if (DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_RF_AGC)
        {
            UNUSED(BSP_PinSet(PIN_RF_AGC, PINMUX_FUNCTION3)); 
        }
        else
        {
            //UNUSED(BSP_PinSet(MX0+8, PINMUX_FUNCTION3)); //DEMOD_RFAGC
        }
        break;
        
    case SrcPWM1:
        u4Reg = REG_RW_PWM1;
       if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_PWM1) 
        {
        #ifdef CC_SCPOS_3DTV_SUPPORT
        #ifndef CC_MTK_LOADER
            if (u4DrvTDTVIsScanningBLEnabled())
            {
                UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION3)); //Tuner 30V Boost-up PWM in BGA
            }
            else
            {
                UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
            }
        #else
            UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
        #endif
        #else
            UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
        #endif
        }
       else if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_IF_AGC) // QFP-ETH
        {
            UNUSED(BSP_PinSet(PIN_IF_AGC, PINMUX_FUNCTION2)); //OPWM1 in QFP-ETH
        }
	   else if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_PACEL) // QFP-ETH
        {
            UNUSED(BSP_PinSet(PIN_PACLE, PINMUX_FUNCTION3)); //OPWM1 in QFP-ETH
        }
        else // QFP-TCON
        {
            //UNUSED(BSP_PinSet(MX5+23, PINMUX_FUNCTION2)); //OPWM1 in QFP-TCON ,no MX5
        }
        break;
        
    case SrcPWM2:
        u4Reg = REG_RW_PWM2;
        if (DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_PWM2) 
        {
            UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION2)); //LED_CTL in BGA
        }
        else if(DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_GPIO13)
        {
			UNUSED(BSP_PinSet(PIN_GPIO13, PINMUX_FUNCTION8)); //LED_CTL in BGA
       	}
		else
		{
        //UNUSED(BSP_PinSet(MX3+24, PINMUX_FUNCTION8)); //ETMDIO in QFP-ETH, {pmisc[16], pmux3[26:24]}, no PINMUX_FUNCTION8
		}
        break;
    default:
        ASSERT(u4Src <= SrcPWM2);
    }

    // workaround for full duty drop issue
    if (u4DutyAll==0xff && u4DutyON==0xff)
    {
        u4DutyAll = 0xfe;
    }

    u4Value = (((UINT32)u4DutyAll << 24) | ((UINT32)u4DutyON << 16) |
               ((UINT32)(u4TimeBase&0xFFF) << 4) | 1);
    IO_WRITE32(BIM_BASE, u4Reg, u4Value);

    if (u4Src==SrcPWM1)
    {
        IO_WRITE32MSK(BIM_BASE, REG_RW_PWMLVDS, ((u4TimeBase&0xFF000)>>12), 0xFF);
    }
    else if  (u4Src==SrcPWM2)
    {
        IO_WRITE32MSK(BIM_BASE, REG_RW_PWMLVDS, ((u4TimeBase&0xFF000)<<4), 0xFF0000);
    }
}

/**
 * Set PWM sync to LVDS output v-sync signal.
 *
 * @param u4Src      PWM source (SrcPWM0/1/2)
 * @param u4LockON   Lock PWM on LVDS v-sync or not.
 */

void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON)
{
    switch (u4Src)
    {
    default:
    case SrcPWM0:
        // Don't support PWM lock.
        break;
    case SrcPWM1:
        IO_WRITE32MSK(
            BIM_BASE,
            REG_RW_PWMLVDS,
            u4LockON? 0x8000 : 0,
            0xC000);
        break;
    case SrcPWM2:
        IO_WRITE32MSK(
            BIM_BASE,
            REG_RW_PWMLVDS,
            u4LockON? 0x80000000 : 0,
            0xC0000000);
        break;
    }
}

