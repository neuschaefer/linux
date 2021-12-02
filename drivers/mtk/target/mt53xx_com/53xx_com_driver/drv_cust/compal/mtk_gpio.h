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
#include "x_timer.h"
#include "x_pinmux.h"
#include "drv_pwm.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "pmx_if.h"

#define GPIO_POWER_ON_FUNC      (GPIO_MtkPowerOnFunc)
#define GPIO_POWER_OFF_FUNC     (GPIO_MtkPowerOffFunc)
#define GPIO_LED_CTRL_FUNC      (GPIO_MtkLedCtrlFunc)
#ifdef CC_MTK_LOADER
// #define LOADER_UPGRADE_VER_CMP_FUNC (LOADER_MtkUpgradeVerCmpFunc)
#define LOADER_UPGRADE_VER_CMP_FUNC (NULL)
#define LOADER_ENV_INIT_FUNC        (LOADER_MtkEnvInitFunc)
#ifdef LOADER_USB_UPGRADE
// #define LOADER_UPGRADE_DLM_CB_FUNC (LOADER_MtkUpgradeDlmCbFunc)
#define LOADER_UPGRADE_DLM_CB_FUNC (NULL)
#endif
#endif /* CC_MTK_LOADER */

static void GPIO_MtkPowerOnFunc(void);
static void GPIO_MtkPowerOffFunc(UINT32 u4PowerDownEn);
static void GPIO_MtkLedCtrlFunc(LED_STATUS_T eLedStatus);
#ifdef CC_MTK_LOADER
static INT32 LOADER_MtkUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer);
static void LOADER_MtkEnvInitFunc(void);
#ifdef LOADER_USB_UPGRADE
static void LOADER_MtkUpgradeDlmCbFunc(UINT32 u4BufSize);
#endif
#endif /* CC_MTK_LOADER */

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

#ifdef CC_MTK_LOADER
static INT32 LOADER_MtkUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer)
{
    INT32 i4Ret;
    i4Ret = strcmp(szCurVer, szUpgVer); // i4Ret = szCurVer - szUpgVer.
    Printf("CurVer:%s UpgVer:%s\n", szCurVer, szUpgVer);
    return (0 > i4Ret) ? 0 : 1;   // 0 is ok to upgrade, upgrade when szCurVer < szUpgVer.
}

static void LOADER_MtkEnvInitFunc()
{
    UNUSED(LOADER_MtkUpgradeVerCmpFunc);
    // AUD_CLR_BIT(REG_INT_SDADC2, ADC_R_PD);
    IO_WRITE32(AUDIO_BASE,0x0248, (IO_READ32(AUDIO_BASE,0x0248)) & (~((0x1 << 27))));
    // AUD_CLR_BIT(REG_INT_SDADC2, ADC_L_PD);
    IO_WRITE32(AUDIO_BASE,0x0248, (IO_READ32(AUDIO_BASE,0x0248)) & (~((0x1 << 26))));
    // AUD_WRITE32(REG_INT_DACCTL4,(AUD_READ32(REG_INT_DACCTL4) & ~PD_CTL_MASK)|PD_ON);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0))& ~(0xf << 4)|(0x0 << 4));
    // AUD_CLR_BIT(REG_INT_DACCTL4,PD_ISOURCE);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0)) & (~((0x1 << 3))));
    // AUD_CLR_BIT(REG_INT_DACCTL4,PD_REF2);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0)) & (~((0x1 << 1))));
    // AUD_CLR_BIT(REG_INT_DACCTL4,PD_REF1);
    IO_WRITE32(AUDIO_BASE,0x01d0, (IO_READ32(AUDIO_BASE,0x01d0)) & (~((0x1 << 0))));
}

#ifdef LOADER_USB_UPGRADE
#include "x_dlm.h"
#include "loader_if.h"
static INT32 LOADER_MtkAllUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;
    UINT32 u4ImageWriteAddr = 0;

    if (!u4Flag)
    {
        u4Flag = 1;
        return LDR_FlashWrite(e_flag, pauc_buffer, ui4_len, 1, u4ImageWriteAddr);
    }
    else
    {
        return LDR_FlashWrite(e_flag, pauc_buffer, ui4_len, 0, 0);
    }
}

static INT32 LOADER_MtkFirmUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    static UINT32 u4Flag = 0;
    UINT32 u4ImageWriteAddr = LOADER_MAX_SIZE;

    if (!u4Flag)
    {
        u4Flag = 1;
        return LDR_FlashWrite(e_flag, pauc_buffer, ui4_len, 1, u4ImageWriteAddr);
    }
    else
    {
        return LDR_FlashWrite(e_flag, pauc_buffer, ui4_len, 0, 0);
    }
}

static void LOADER_MtkUpgradeDlmCbFunc(UINT32 u4BufSize)
{
    UNUSED(LOADER_MtkUpgradeDlmCbFunc);
    x_dlm_reg_append_callback("all", u4BufSize, LOADER_MtkAllUpgrade);
    x_dlm_reg_append_callback("firm", u4BufSize, LOADER_MtkFirmUpgrade);
    x_dlm_reg_append_callback("nor1", u4BufSize, LOADER_MtkFirmUpgrade);
}
#endif  /* LOADER_USB_UPGRADE */
#endif /* CC_MTK_LOADER */

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
    UINT32 i,polarity;
    INT32 i4Val;

#ifdef CC_MTK_LOADER
    //Audio mute circuit mute in loader
    if (DRVCUST_InitQuery(eAudioMuteGpioNum, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(eAudioMuteGpioPolarity, &polarity));
        GPIO_SetOut(i, polarity);
      	//GPIO_SetOut(204, 0);
    }

    GPIO_MtkLedCtrlFunc(eLedLoaderPowerOn);

    // release reset of Demod chip
    if (DRVCUST_InitQuery(eDemodResetGpio, &i) == 0)
    {
        GPIO_SetOut(i, 0);
        HAL_Delay_us(20000); //delay 20ms
        GPIO_SetOut(i, 1);
        Printf("Reset Demod GPIO=%x, 0->1\r\n",i);
    }
#else /* CC_MTK_LOADER */
    GPIO_MtkLedCtrlFunc(eLedSystemPowerOn);

    // Mute OSD before driver init.
    PMX_SetBg(0x00000000, FALSE);
    IO_WRITE32(OSD_BASE, 0x108, IO_READ32(OSD_BASE, 0x108) & 0xffff00ffU);
    IO_WRITE32(OSD_BASE, 0x208, IO_READ32(OSD_BASE, 0x208) & 0xffff00ffU);

    vDrvSetPWM(SrcPWM2, (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / 48000, 0xFF, 0x80);
    UNUSED(polarity);
#ifdef CC_MT8295

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
#endif /* CC_MT8295 */

#endif /* CC_MTK_LOADER */

    INIT_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
    INIT_SET_GPIO(eDvdCtrlGpio, eDvdCtrlOnVal);
}

static void GPIO_MtkPowerOffFunc(UINT32 u4PowerDownEn)
{
    UINT32 i, fgLedOff;
    INT32 i4Val, i4Ret;
    DTVCFG_T rDtvCfg;

    if (DRVCUST_InitGet(eFlagSupportPowerLedOff))
    {
        i4Ret = EEPDTV_GetCfg(&rDtvCfg);
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
#ifdef CC_MTK_LOADER
            GPIO_MtkLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_MTK_LOADER */
            GPIO_MtkLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_MTK_LOADER */
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
#ifdef CC_MTK_LOADER
            GPIO_MtkLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_MTK_LOADER */
            GPIO_MtkLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_MTK_LOADER */
        }
    }
}

#else
#error "reinclude mtk_gpio.h..."
#endif

