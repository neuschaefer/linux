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

#ifndef MTK_GPIO_H
#define MTK_GPIO_H

#include "x_gpio.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
//#include "eeprom_if.h"

#define GPIO_POWER_ON_FUNC      (GPIO_MtkPowerOnFunc)
#define GPIO_POWER_OFF_FUNC     (GPIO_MtkPowerOffFunc)
#define GPIO_LED_CTRL_FUNC      (GPIO_MtkLedCtrlFunc)

static void GPIO_MtkPowerOnFunc(void);
static void GPIO_MtkPowerOffFunc(UINT32 u4PowerDownEn);
static void GPIO_MtkLedCtrlFunc(LED_STATUS_T eLedStatus);

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

#define INIT_SET_GPIO(gpio, onval)                  \
    if (DRVCUST_InitQuery(gpio, &i)==0)              \
    {                                               \
        if (DRVCUST_InitQuery(onval, (UINT32 *)&i4Val)==0)     \
        {                                           \
            VERIFY(GPIO_Output((INT32)i, &i4Val) == i4Val);  \
        }                                           \
    }

#define DOWN_SET_GPIO(gpio, onval)                  \
    if (DRVCUST_InitQuery(gpio, &i)==0)              \
    {                                               \
        if (DRVCUST_InitQuery(onval, (UINT32 *)&i4Val)==0)     \
        {                                           \
            i4Val = !i4Val;                         \
            VERIFY(GPIO_Output((INT32)i, &i4Val)== i4Val);  \
        }                                           \
    }


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

static void GPIO_MtkLedCtrlFunc(LED_STATUS_T eLedStatus)
{
    UINT32 i;
    INT32 i4Val;

    switch(eLedStatus)
    {
    case eLedLoaderPowerOn:
        INIT_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        INIT_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        break;
    case eLedLoaderPowerOff:
        DOWN_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        DOWN_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        break;
    case eLedSystemPowerOn:
        INIT_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        INIT_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        break;
    case eLedSystemPowerOff:
        DOWN_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        DOWN_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        break;
    case eLedDarkLed:
        INIT_SET_GPIO(eBackupLedGpio, eBackupLedDarkVal);
        INIT_SET_GPIO(ePowerLedGpio, ePowerLedDarkVal);
        break;
    case eLedIrOn:
        INIT_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        INIT_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        break;
    case eLedIrOff:
        DOWN_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        DOWN_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        break;
    case eLedVgaStandby:
        break;
    default:
        break;
    }
}

static void GPIO_MtkPowerOnFunc()
{
    UINT32 i;
    INT32 i4Val;

#ifdef CC_5391_LOADER
    GPIO_MtkLedCtrlFunc(eLedLoaderPowerOn);
#else /* CC_5391_LOADER */
    GPIO_MtkLedCtrlFunc(eLedSystemPowerOn);

#ifdef CC_MT8295
    /* TPV demo workaround */
    Printf("PINMUX0:0x%08X\n", PINMUX_REG(0) );
    Printf("PINMUX1:0x%08X\n", PINMUX_REG(1) );
    Printf("PINMUX2:0x%08X\n", PINMUX_REG(2) );
    Printf("PINMUX3:0x%08X\n", PINMUX_REG(3) );
    PINMUX_WRITE(1, (PINMUX_REG(1)& ~(0x00000700)) | 0x00000500 );		//MX1[10: 8]=5=b'101
    PINMUX_WRITE(1, (PINMUX_REG(1)& ~(0x00007000)) | 0x00004000 );		//MX1[14:12]=4=b'100
    PINMUX_WRITE(1, (PINMUX_REG(1)& ~(0x00070000)) | 0x00070000 );		//MX1[18:16]=7=b'111
    PINMUX_WRITE(1, (PINMUX_REG(1)& ~(0xc0000000)) | 0x40000000 );		//MX1[31:30]=1=b'01
    PINMUX_WRITE(2, (PINMUX_REG(2)& ~(0x70000000)) | 0x60000000 );		//MX2[30:28]=6=b'110
    PINMUX_WRITE(3, (PINMUX_REG(3)& ~(0x00400000)) | 0x00400000 );		//MX3[22]   =1=b'1
    PINMUX_WRITE(3, (PINMUX_REG(3)& ~(0x00800000)) | 0x00800000 );		//MX3[23]   =1=b'1
    Printf("---------------8295 Workaround-------------------\n");
    Printf("PINMUX0:0x%08X\n", PINMUX_REG(0) );
    Printf("PINMUX1:0x%08X\n", PINMUX_REG(1) );
    Printf("PINMUX2:0x%08X\n", PINMUX_REG(2) );
    Printf("PINMUX3:0x%08X\n", PINMUX_REG(3) );

    {
        UINT32 u4GpioNum;
        if (DRVCUST_OptQuery(eMT8295ResetGpio, &u4GpioNum) == 0)
        {
            Printf("8295 Reset Gpio : %d \n", u4GpioNum);
            GPIO_SetOut(u4GpioNum, 1);
            GPIO_SetOut(u4GpioNum, 0);
            GPIO_SetOut(u4GpioNum, 0);
            GPIO_SetOut(u4GpioNum, 1);
        }
    }
/*
    Printf("[0x20017020]=0x%08X\n", IO_READ32(0x20017020,0));
    IO_WRITE32MSK(0x20017020, 0, 0x0, 0x00000002 );
    Printf("---------------Change 5355 to Serial-TS in-------------------\n");
    Printf("[0x20017020]=0x%08X\n", IO_READ32(0x20017020,0));    
*/
#endif /* CC_MT8295 */

#endif /* CC_5391_LOADER */
    INIT_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
    INIT_SET_GPIO(eDvdCtrlGpio, eDvdCtrlOnVal);
    DOWN_SET_GPIO(eBackLightGpio, eBackLightOnVal);
    DOWN_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);
}

static void GPIO_MtkPowerOffFunc(UINT32 u4PowerDownEn)
{
    UINT32 i, fgLedOff;
    INT32 i4Val, i4Ret;
    DTVCFG_T rDtvCfg;

    if (DRVCUST_InitGet(eFlagSupportPowerLedOff))
    {
        i4Ret = EEPROM_Read(EEPROM_DTV_DRIVER_OFFSET, (UINT32)(void *)&(rDtvCfg), sizeof(rDtvCfg));
        if (i4Ret == 0)
        {
            fgLedOff = (rDtvCfg.u1Flags & DTVCFG_FLAG_POWER_LEDOFF) ? 1 : 0;
        }
        else
        {
            fgLedOff = 0;
        }
    }
    else
    {
        fgLedOff = 0;
    }

    if (!DRVCUST_InitGet(eSupportDarkLed))
    {
        if (!fgLedOff)
        {
#ifdef CC_5391_LOADER
            GPIO_MtkLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_5391_LOADER */
            GPIO_MtkLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_5391_LOADER */
        }
    }

    DOWN_SET_GPIO(eBackLightGpio, eBackLightOnVal);
    DOWN_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);
    DOWN_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
    DOWN_SET_GPIO(eDvdCtrlGpio, eDvdCtrlOnVal);

    if (DRVCUST_InitGet(eSupportDarkLed))
    {
        if (!(u4PowerDownEn & WAK_IRRX))
        {
            GPIO_MtkLedCtrlFunc(eLedDarkLed);
        }
        else
        {
#ifdef CC_5391_LOADER
            GPIO_MtkLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_5391_LOADER */
            GPIO_MtkLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_5391_LOADER */
        }
    }
}

#else
#error "reinclude mtk_gpio.h..."
#endif

