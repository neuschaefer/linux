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
 * $Author: muran.xiang $
 * $Date: 2012/07/08 $
 * $RCSfile: loader_standby.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_standby.c
 *  This file provides functions for boot up decision and enter standby.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "c_model.h"

#include "x_os.h"
#include "u_irrc_btn_def.h"
#include "x_printf.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_gpio.h"
// #include "ir_if.h"
#include "nor_if.h"
// #include "eeprom_if.h"
#include "drvcust_if.h"

// #include "zip.h"
// #include "xmodem.h"
// #include "loader_imghdr.h"
#include "loader_if.h"
#include "pdwnc_if.h"
#include "ir_if.h"

#ifndef LOADER_MULTIPLE_LOGO_PARTITION
#define LOADER_MULTIPLE_LOGO_PARTITION (0)
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#if !defined(CC_LOADER_SHRINK_STANDBY)  //loader shrink

//---------------------------------------------------------------------
/** _LdrCheckEepromBoot()
 *  @param  fgPwrBtn 1 to toogle power button bit, and 0 do nothing.
 *  @param  fgBoot 1 to boot always.
 *  @retval -1:continue boot; 0:standby with button wake up only;
 *          WAK_IRRX: standby with both button & IR wakeup.
 */
//---------------------------------------------------------------------
static INT32 _LdrCheckEepromBoot(LDR_DATA_T *prLdrData, UINT32 fgPwrBtn, UINT32 fgBoot)
{

    UINT8 u1OrgFlags;

    u1OrgFlags = prLdrData->rDtvCfg.u1Flags;
    if (fgPwrBtn && !prLdrData->u4UseVGAStatus)
    {
        prLdrData->rDtvCfg.u1Flags ^= DTVCFG_FLAG_POWER_BTN_ON;
    }
    else
    {
        // if use VGA status for standby, !DTVCFG_FLAG_POWER_BTN_ON is VGA Signal off standby DPMS mode.
        prLdrData->rDtvCfg.u1Flags |= DTVCFG_FLAG_POWER_BTN_ON;
    }

    printf("Org:0x%02x Flags:0x%02x\n", (UINT32)u1OrgFlags, (UINT32)prLdrData->rDtvCfg.u1Flags);
    if (u1OrgFlags != prLdrData->rDtvCfg.u1Flags)
    {
        EEPDTV_SetCfg(&prLdrData->rDtvCfg);
    }

//    Printf("                             \n");

    if (fgBoot)
    {
        return -1;   // boot always, if fgBoot is on.
    }

    if ((prLdrData->rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) ||
        (prLdrData->rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
    {
        return -1;    // no initial EEPROM data, boot and let tv to re-config it.
    }

    if ((prLdrData->rDtvCfg.u1Flags & DTVCFG_FLAG_POWERON_BOOT) ||
        (prLdrData->rDtvCfg.u1Flags & DTVCFG_FLAG_FACTORY_MODE))
    {
        return -1;   // must continue boot.
    }

#ifdef ADVANCED_STANDBY_CTRL
    if (u1OrgFlags & DTVCFG_FLAG_POWERON_LAST)
    {
        if (u1OrgFlags & DTVCFG_FLAG_SYSTEM_STATE_ON)
        {
            return -1; // continue boot.
        }
        else
        {
            return WAK_IRRX;
        }
    }
#endif /* ADVANCED_STANDBY_CTRL */

    if (prLdrData->u4UsePrevPowerState)
    {
        if (prLdrData->u4UseVGAStatus)
        {
            // Here, !DTVCFG_FLAG_POWER_BTN_ON is VGA Signal off standby DPMS mode.
            if (!(u1OrgFlags & DTVCFG_FLAG_POWER_BTN_ON))
            {
                if (PDWNC_IsWakeupByPowerButton()) // only power button to standby.
                {
                    return WAK_IRRX;    // standby with button & IR wakeup
                }
                else // ir wakeup or VGA wakeup to boot
                {
                    return -1;  // continue boot.
                }
            }
            else       // not DPMS mode, use previous power state.
            {
                if (u1OrgFlags & DTVCFG_FLAG_SYSTEM_STATE_ON)
                {
                    return -1;  // continue boot.
                }
                else
                {
                    if (prLdrData->u4WakeupReason != PDWNC_WAKE_UP_REASON_AC_POWER)    // any kind of wakeup, always boot.
                    {
                        return -1;  // continue boot.
                    }
                    return WAK_IRRX;    // standby with button & IR wakeup
                }
            }
        }
        else
        {
            if (prLdrData->rDtvCfg.u1Flags & DTVCFG_FLAG_SYSTEM_STATE_ON)
            {
                return -1;  // continue boot.
            }
            else
            {
                //if (prLdrData->u4WakeupReason != PDWNC_WAKE_UP_REASON_AC_POWER)    // any kind of wakeup, always boot.
                if ((prLdrData->u4WakeupReason != PDWNC_WAKE_UP_REASON_AC_POWER) && (prLdrData->u4WakeupReason != PDWNC_WAKE_UP_REASON_WATCHDOG))   // any kind of wakeup, always boot.                
                {
                    return -1;  // continue boot.
                }                                            
                return WAK_IRRX;    // standby with button & IR wakeup
            }
        }
    }

    if (prLdrData->u4UsePowerBtnCount)
    {
        if (prLdrData->rDtvCfg.u1Flags & DTVCFG_FLAG_POWER_BTN_ON)
        {
            if (fgPwrBtn)   // boot is pressed by power button.
            {
                return -1;  // continue boot.
            }
            else
            {
                return WAK_IRRX;    // standby with button & IR wakeup
            }
        }
        else
        {
            return 0;   // standby with button wakeup only.
        }
    }

    if (prLdrData->u4UseVGAStatus)
    {
        // Here, DTVCFG_FLAG_POWER_BTN_ON is VGA Signal off standby DPMS mode.
        if (u1OrgFlags & DTVCFG_FLAG_POWER_BTN_ON)
        {
            if (prLdrData->u4WakeupReason==PDWNC_WAKE_UP_REASON_AC_POWER)
            {
                return WAK_IRRX;    // standby with button & IR wakeup
            }
            else
            {
                return -1;  // continue boot.
            }
        }
        else
        {
            if (IRRX_IsTheKey(BTN_POWER) || (PDWNC_IsWakeupByPowerButton()))
            {
                return WAK_IRRX;    // standby with button & IR wakeup
            }
            else
            {
                return -1;  // continue boot.
            }
        }
    }

    return WAK_IRRX;
}
#endif
//---------------------------------------------------------------------
/** _LdrCheckStandBy()
 */
//---------------------------------------------------------------------
static UINT32 _LdrCheckStandBy(LDR_DATA_T *prLdrData)
{
#if !defined(CC_LOADER_SHRINK_STANDBY)  //loader shrink

    if (prLdrData->u4UsePrevPowerState || (prLdrData->u4WakeupReason==PDWNC_WAKE_UP_REASON_AC_POWER))
    {
        // AC power on, enter standby mode.
        if (_LdrCheckEepromBoot(prLdrData, 0, 0) >= 0)
        {
            prLdrData->u4WakeupReason = PDWNC_POWER_DOWN_UNKNOWN;
            return 1;
        }
    }
    else if (prLdrData->u4WakeupReason==PDWNC_WAKE_UP_REASON_WATCHDOG) // watchdog reboot.
    {
#ifdef CC_WATCHDOG_BOOT_STANDBY //For Watchdog Reset entering Standby mode instead of booting directly
        if (_LdrCheckEepromBoot(prLdrData, 0, 0) >= 0)
        {
            prLdrData->u4WakeupReason = PDWNC_POWER_DOWN_UNKNOWN;
            return 1;
        }
#else /* CC_WATCHDOG_BOOT_STANDBY */
        return 0; // boot directly.
#endif /* CC_WATCHDOG_BOOT_STANDBY */
    }
    else    // wakeup by ir, button, uart, or VGA.
    {
        if (prLdrData->u4UsePowerBtnCount && PDWNC_IsWakeupByPowerButton())
        {
#ifdef CC_POWER_BTN_DIRECTLY_BOOT
            /* do nothing. Then it will continue booting. */
#else /* CC_POWER_BTN_DIRECTLY_BOOT */
            if (_LdrCheckEepromBoot(prLdrData, 1, 0) >= 0)
            {
                prLdrData->u4WakeupReason = PDWNC_POWER_DOWN_UNKNOWN;
                return 1;
            }
#endif /* CC_POWER_BTN_DIRECTLY_BOOT */
        }

        if (prLdrData->u4UseVGAStatus)
        {
            if (_LdrCheckEepromBoot(prLdrData, 0, 0) >= 0)
            {
                prLdrData->u4WakeupReason = PDWNC_POWER_DOWN_UNKNOWN;
                return 1;
            }
        }
    }
#endif
    // Wakeup, to continue booting.
    return 0;
}

//---------------------------------------------------------------------
/** _LdrDetermineEnterStandby()
 */
//---------------------------------------------------------------------
static UINT32 _LdrDetermineEnterStandby(LDR_DATA_T *prLdrData)
{
#if !defined(CC_LOADER_SHRINK_STANDBY)  //loader shrink

    PDWNC_FuncTbl *prPdwncFuncTbl;
    UINT32 u4State, u4NextState, fgEnterStandby;

    u4State = PDWNC_ReadWakeupReason();
    if(u4State == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Year = 0xff;
        // To indicate it is last cut off by A/C
        VERIFY(PDWNC_SetQueryRtcYear(&u1Year) == u1Year);        
    }
   
    if (!prLdrData->u4ExternalStatus)
    {
        return 0;
    }

#ifdef CC_CUST_USB_BREAK_HANDLE_SUPPORT
    // Check if the last USB upgrade function is break by AC off.
    UINT32 u4UpgradeGoAddr;
    if (DRVCUST_InitQuery(eLoaderUpgradeBreakStateOffset, &u4UpgradeGoAddr) == 0)
    {
        UINT8 u1UpgradeState = 0;

        UNUSED(EEPROM_Read(u4UpgradeGoAddr, (UINT32)(void *)&u1UpgradeState, sizeof(UINT8)));
        if (u1UpgradeState & DTVCFG_FLAG_USB_UPGRADE_ON)
        {
            PFN_UPGRADE_BREAK_FUNC pfnUpgradeFail;

            // Query the user defined function for the upgrade fail case
            pfnUpgradeFail = 
                (PFN_UPGRADE_BREAK_FUNC)DRVCUST_InitGet(eLoaderUpgradeBreakFunc);
            if (pfnUpgradeFail != NULL)
            {
                pfnUpgradeFail();   // go to the upgrade fail handle
        }
        else
        {
            if (DRVCUST_InitGet(eLoaderStandbyEnableIfUpgradeBreak))
            {
                Printf("Enter standby for upgrade abnormal break !\n");
                return 1;
            }
        }
    }
    }
#endif /* CC_CUST_USB_BREAK_HANDLE_SUPPORT */

    prPdwncFuncTbl = PDWNC_EXT_GetFunc();
    if (prPdwncFuncTbl->pfnGetCurrentState &&
        prPdwncFuncTbl->pfnDetermineNextState)
    {
        prPdwncFuncTbl->pfnGetCurrentState(&u4State);
        prPdwncFuncTbl->pfnDetermineNextState(u4State, prLdrData->u4WakeupReason, &u4NextState, &fgEnterStandby, &prLdrData->u4PowerDown);
        prPdwncFuncTbl->pfnEnterNextState(u4NextState);
        if (fgEnterStandby)
        {
            return 1;
        }
    }
    else
    {
        // check if enter standby
        if (_LdrCheckStandBy(prLdrData))
        {
            return 1;
        }
    }
#endif
    return 0;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** LDR_IsGoBootup(): Decide whether go to standby or continue boot up.
 *  This function is going to check wakeup reason and eeprom status (or some
 *  external uP) to decide it should enter standby or contineu boot.
 *  This function must set prLdrData->u4PowerDown flags for LDR_EnterStandby()
 *  whenever this function decide to go to Standby mode.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Enter standby.
 *  @retval otherwise       Continue boot.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_IsGoBootup(LDR_DATA_T *prLdrData)
{
#if !defined(CC_LOADER_SHRINK_STANDBY)  //loader shrink
    prLdrData->u4WakeupReason = PDWNC_ReadWakeupReason();
    prLdrData->u4PowerDown = WAK_IRRX;
    prLdrData->u4ExternalStatus =  DRVCUST_InitGet(eExternalStatus);
    prLdrData->u4UsePowerBtnCount =  DRVCUST_InitGet(eStdbyUsePowerBtnCount);
    prLdrData->u4UsePrevPowerState = DRVCUST_InitGet(eStdbyUsePrevPowerState);
    prLdrData->u4UseVGAStatus =  DRVCUST_InitGet(eStdbyUseVGAStatus);

    if (_LdrDetermineEnterStandby(prLdrData))
    {
        return 0;
    }
#endif
    // to continue boot.
    return 1;
}

//-----------------------------------------------------------------------------
/** LDR_EnableCEC(): Decide whether enable CEC or not
 *
 *  @param  fgCecEnable  Enable or disable CEC or not.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void LDR_EnableCEC(BOOL fgCecEnable)
{
#if !defined(CC_LOADER_SHRINK_STANDBY)  //loader shrink
    UINT32 u4Val;
    UINT32 u4Val2,u4Val3;
    INT32 i4Val;
    UINT8 au1Data[6] = {0};
    PDWNC_T8032_CMD_T rCmd;

    if (!fgCecEnable)
    {
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC; //Notify T8032 start/stop control CEC
        rCmd.u1SubCmd = 0;
        rCmd.au1Data[0] = 0;	//T8032 stop control CEC.
        PDWNC_T8032Cmd(&rCmd, NULL);        
        return;
    }
    
    // Write CEC configuration to uP
    if((DRVCUST_InitGet(eT8032uPOffset) != 0) && ((u4Val = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x0) &&
    ((DRVCUST_InitGet(eEepromCECEnableOffset)) == 0x00) && ((DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) == 0x00))
    {
        i4Val = EEPROM_Read(u4Val, (UINT32)au1Data, 6);        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE;
        rCmd.u1SubCmd = 0;
        x_memcpy(rCmd.au1Data, au1Data, 4);
        PDWNC_T8032Cmd(&rCmd, NULL);

        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
        x_memcpy(rCmd.au1Data, &au1Data[4], 1);
        PDWNC_T8032Cmd(&rCmd, NULL);

        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
        x_memcpy(rCmd.au1Data, &au1Data[5], 1);
        PDWNC_T8032Cmd(&rCmd, NULL);        
        UNUSED(i4Val);
    }
    else if((DRVCUST_InitGet(eT8032uPOffset) != 0) && ((u4Val = DRVCUST_InitGet(eEepromCECMenuLanguageOffset)) != 0x0) &&
    ((u4Val2 = DRVCUST_InitGet(eEepromCECEnableOffset)) != 0x00) && ((u4Val3 = DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset)) != 0x00))
    {
        i4Val = EEPROM_Read(u4Val, (UINT32)au1Data, 4);        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE;
        rCmd.u1SubCmd = 0;
        x_memcpy(rCmd.au1Data, au1Data, 4);
        PDWNC_T8032Cmd(&rCmd, NULL);

        i4Val = EEPROM_Read(u4Val2, (UINT32)au1Data, 1);        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC;
        x_memcpy(rCmd.au1Data, au1Data, 1);
        PDWNC_T8032Cmd(&rCmd, NULL);

        i4Val = EEPROM_Read(u4Val3, (UINT32)au1Data, 1);        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
        x_memcpy(rCmd.au1Data, au1Data, 1);
        PDWNC_T8032Cmd(&rCmd, NULL);        
        UNUSED(i4Val);
    }

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC; //Notify T8032 start/stop control CEC
    rCmd.u1SubCmd = 0;
    rCmd.au1Data[0] = 1;	//T8032 control CEC
    PDWNC_T8032Cmd(&rCmd, NULL);
#endif
}

//-----------------------------------------------------------------------------
/** LDR_EnterStandby(): Enter standby mode after calling.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_EnterStandby(LDR_DATA_T *prLdrData)
{
#if !defined(CC_LOADER_SHRINK_STANDBY)  //loader shrink

    BOOL fgFpWakeupEnable;

    // Check keypad lock
    if (prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_FP_LOCK)
    {
        fgFpWakeupEnable = 0;
    }
    else
    {
        fgFpWakeupEnable = 1; // not lock, to enable FP keypad wakeup.
    }
    PDWNC_SetupFpWakeup(&fgFpWakeupEnable);


#ifdef CC_CUST_CEC_CONFIG_SUPPORT
    // CEC configuration.
    PFN_CEC_CONFIG_QUERY_FUNC pfnQueryCECEnableFunc;
            
    pfnQueryCECEnableFunc = 
        (PFN_CEC_CONFIG_QUERY_FUNC)(DRVCUST_InitGet(eLoaderCECConfigQueryFunc)); 
    
    if (pfnQueryCECEnableFunc != NULL)
    {
        LDR_EnableCEC(pfnQueryCECEnableFunc());
    }
    else
#endif /* CC_CUST_CEC_CONFIG_SUPPORT */        
    {
        LDR_EnableCEC(TRUE);
    }
#endif

    PDWNC_EnterPowerDown(prLdrData->u4PowerDown, prLdrData->u4WakeupReason);

    return 0;
}

#if LOADER_MULTIPLE_LOGO_PARTITION

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

typedef enum _ENUM_JPEG_RET_CODE_T
{
    E_JPG_OK,
    E_JPG_ERR_NONFATAL,
    E_JPG_ERR_FATAL,
    E_JPG_ERR_PARAM,
    E_JPG_ERR_NOT_INITED,
    E_JPG_ERR_ALREADY, //5
    /* markers */
    E_JPG_ERR_NOT_JPEG_IMAGE,
    E_JPG_ERR_NOT_A_MARKER,
    E_JPG_ERR_PASS_END,
    E_JPG_ERR_MULTI_SOI,
    E_JPG_ERR_UNSUPPORT_SOF, //10
    E_JPG_ERR_MULTI_SOF,
    E_JPG_ERR_INVALID_SOF,
    E_JPG_ERR_SOS_BEFORE_SOF,
    E_JPG_ERR_INVALID_SOS,
    E_JPG_ERR_INVALID_DHT, //15
    E_JPG_ERR_INVALID_DRI,
    E_JPG_ERR_MULTI_APP1,
    E_JPG_ERR_INVALID_APP0,
    E_JPG_ERR_INVALID_APP1,
    E_JPG_ERR_PARSE_FAILED, //20
    E_JPG_ERR_NOT_PARSED,
    E_JPG_ERR_MULTI_EOI,
    /* instances */
    E_JPG_ERR_ALLOC_PARSER,
    E_JPG_ERR_ALLOC_DECODER,
    E_JPG_ERR_ALLOC_MEM, //25
    E_JPG_ERR_ALLOC_FLOW,
    /* general */
    E_JPG_ERR_UNSUPPORT,
    E_JPG_ERR_GENERAL,
    E_JPG_ERR_LAST,
    E_JPG_ERR_DISPLAY_ADDR, //30
    E_JPG_ERR_INVALID_COMPONENT //code sync
} ENUIM_JPEG_RET_CODE_T;

/* MISRA32 enum assign at first only or all assign */
LINT_SUPPRESS_NEXT_STATEMENT(960)
typedef enum _ENUM_HWJPG_RET_CODE
{
    E_HWJPG_OK = 0,
    E_HWJPG_BUSY,

    E_HWJPG_ERR_FETCH_TIMEOIT = E_JPG_ERR_LAST + 1,
    E_HWJPG_ERR_SET_BS,
    E_HWJPG_ERR_LOAD_Q,
    E_HWJPG_ERR_QTBL_INDEX,
    E_HWJPG_ERR_QTBL_NUM,

    E_HWJPG_ERR_PARAM,
    E_HWJPG_ERR_TIMEOUT,

    E_HWJPG_ERR_COMP_RANGE,
    E_HWJPG_ERR_TBL_RANGE,
    E_HWJPG_ERR_BLOCK,
    E_HWJPG_ERR_SIZE,
    E_HWJPG_ERR_OUTBUF,

    E_HWJPG_ERR_NULL_SCAN,

    E_HWJPG_ERR_GLOB_NOT_READY,
    E_HWJPG_ERR_SCAN_NOT_READY,
    E_HWJPG_ERR_ROW_NOT_READY,
    E_HWJPG_ERR_MCU_NOT_READY,
    E_HWJPG_ERR_MISC_NOT_READY,
    E_HWJPG_ERR_HUFF_ADDR,
    /* last */
    E_HWJPG_ERR_GENERAL
} ENUM_HWJPG_RET_CODE;

/* jfif types start */
#define D_MAX_JPEG_COMPONENT	4
#define D_MAX_NUM_APP0			2
#define D_MAX_NUM_SCAN			32

#define ADVANCE_BYTES(NBYTES) \
            /* this while 0 is safe */ \
            LINT_SUPPRESS_NEXT_STATEMENT(717) \
            do \
            { \
                i4Ret = _jfifAdvanceBytes(prInstance, NBYTES); \
                if ((INT32)E_JPG_OK != i4Ret) \
                { \
                    return i4Ret; \
                } \
            } while (0)

#define JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) (PINSTANCE->pu1CurrRead[NTH_BYTE])
#define JPEG_READ_2BYTES(PINSTANCE, NTH_BYTE) ((JPEG_READ_BYTE(PINSTANCE, NTH_BYTE) << 8) | JPEG_READ_BYTE(PINSTANCE, (NTH_BYTE) + 1))

#define JPEG_VLD_V4
#define JDEC_QTABLE_RISC_MODE
#define JPEG_VLD_V3

#define D_MAX_JPEG_HW_COMP		3
#define D_MAX_JPEG_HUFF_TBL		4
#define D_MAX_JPEG_QTBL			4
#define D_MAX_JPEG_HW_QTBL		2
#define D_MAX_JPEG_HW_BLOCK		10
#define D_HUFF_LOOKAHEAD_BITS	6	/* # of bits of lookahead */

#define D_DCTSIZE				8
#define D_JPEG_DRAM_ALIGN_MASK	0xf
#define D_JPEG_DRAM_ALIGN_SIZE	(D_JPEG_DRAM_ALIGN_MASK + 1)

#define D_JPEG_BASELINE			0
#define D_JPEG_DC_REFINE		1
#define D_JPEG_AC_FIRST			2
#define D_JPEG_AC_REFINE		3
#define D_JPEG_DC_FIRST			4

/* global data check flags */
#define D_FLAG_NONE				(0)
#define D_GLOB_PROGRESSIVE		(1 << 0)
#define D_GLOB_NCOMP			(1 << 1)
#define D_GLOB_BLK_W			(1 << 2)
#define D_GLOB_BLK_H			(1 << 3)
#define D_GLOB_PIC_W			(1 << 4)
#define D_GLOB_PIC_H			(1 << 5)
#define D_GLOB_DCHUFF			(1 << 6)	/* no need */
#define D_GLOB_ACHUFF			(1 << 7)	/* no need */
#define D_GLOB_NZBUFF			(1 << 8)	/* no need */
#define D_GLOB_QTBL				(1 << 9)	/* no need */
#define D_GLOB_RESTART			(1 << 10)
/* scan data check flags */
#define D_SCAN_DATASTART		(1 << 0)
#define D_SCAN_NCOMP			(1 << 1)
#define D_SCAN_COMPIN			(1 << 2)
#define D_SCAN_COMPQ			(1 << 3)
#define D_SCAN_BLKDC			(1 << 4)
#define D_SCAN_BLKAC			(1 << 5)
#define D_SCAN_ALAH				(1 << 6)
#define D_SCAN_SESS				(1 << 7)
#define D_SCAN_LAST				(1 << 8)
#define D_SCAN_ROWSTART			(1 << 9)
#define D_SCAN_ROWEND			(1 << 10)
#define D_SCAN_DCHUFF			(1 << 11)
#define D_SCAN_ACHUFF			(1 << 12)
#define D_SCAN_QTBL				(1 << 13)
#define D_SCAN_NZBUFF			(1 << 14)
/* mcu row check flags */
#define D_ROW_CURR				(1 << 0)
#define D_ROW_OUTBUF			(1 << 1)
#define D_ROW_OUTBUFSIZE		(1 << 2)
/* mcu check flags */
#define D_MCU_OUTADDR			(1 << 0)
#define D_MCU_COEFADDR			(1 << 1)
/* misc check flags */
#define D_MISC_ADDR_X			(1 << 0)
#define D_MISC_ADDR_Y			(1 << 1)
#define D_BIT(X)			(1 << (X))
#define ALIGN_MASK(BIT)		(((UINT32)(BIT) >> 3) - 1)

#define M_JPEG_INTERFACE_START() \
	INT32				i4Ret = (INT32)E_HWJPG_OK; \
	JPEG_VLDJPG_DATA_T	*prJpgData

#define M_JPEG_INTERFACE_END() \
	return i4Ret

#define M_JPEG_HANDLE_VALIDATE() \
	prJpgData = (JPEG_VLDJPG_DATA_T *)hInstance; \
	ASSERT(prJpgData != 0); \
	ASSERT(prJpgData->rParent.eJpegClass == E_JC_VLD_DECODER); \
	ASSERT(prJpgData->rParent.zObjectSize == sizeof(JPEG_VLDJPG_DATA_T))

#define INSTANCE prJpgData

#define D_QTBL_HW1	0
#define D_QTBL_HW2	1

#define D_VLD_POLL_DELAY			5
#define D_VLD_POLL_TIMEOUT			300
#define D_JPEG_HW_WAITPIC	(0x200000 * 10)

/* vld register clear value, by default, zero */
#define VLD_CLEAR					0

/* barrel shifter base, and bit shift address */
#define VLD_BARREL_SHIFTER_BITS(BITS) ((BITS) << 2)


#if 1	//CC_5368_JPG

/* vld register address/value definition*/
#define VLDREG_RISC_PROCESS		0x8C
#define VLD_INIT_FETCH_DRAM		(1 << 20)
#define VLD_INIT_FILL_BS		(1 << 23)

#define VLDREG_LOAD_INTRA_Q_MATRIX		0x0A0
#define VLDREG_LOAD_NONINTRA_Q_MATRIX	0x0A4

#define VLDREG_READ_POINTER		0x0B0
#define VLDREG_START_ADDRESS		0x0B4
#define VLDREG_END_ADDRESS		0x0B8

#define VLDREG_PWRSAVE		0xC4
#define VLD_PDN_SCLK    						1
#define VLD_PDN_DCLK       					(1 << 1)
#define VLD_PDN_IOMMU_DCLK    		(1 << 4)

#ifdef JPEG_VLD_V4
#define RW_VLD_PDNFINE          0xC8
#define RW_PDN_BASE_CLK_OFF         ((unsigned)0x1 << 0)
#define RW_PDN_PROG_CLK_OFF         ((unsigned)0x1 << 1)
#endif

#define VLDREG_LATCH2				0x0D0
#define LATCH2_SET					0x1
#define LATCH2_RCWP				((unsigned)0x1 << 18)

#define VLDREG_FETCH_FINISH		0xE8
#define VLD_FETCH_OK            ((unsigned)0x1 << 0)
#define VLD_DRAM_REQ_FIN        ((unsigned)0x1 << 2)
#define VLD_INIT_VALID_FLG       ((unsigned)0x1 << 3)

#define VLDREG_INPUT_WINDOW     (0xF0)

#define VLDREG_VID_BUF_CTL_SIGNAL   (0x0F4)
#ifdef JPEG_VLD_V4
#define SBITS_PROC_RDY    1
#define BBITS_PROC_RDY       (1 << 1)
#elif defined(JPEG_VLD_V3)
#define VLD_SRAM_AA_FIT_TAR_SCLK    1
#define VLD_SRAM_AA_FIT_TAR_D       (1 << 1)
#else
#define VLD_SRAM_AA_FIT_TAR_SCLK    (1 << 16)
#define VLD_SRAM_AA_FIT_TAR_D       (1 << 17)
#endif

#define VLDREG_WATCH_WP			0x0F8
#define VLDREG_WATCH_RP			0x0FC
#define VLDREG_VIDEO_RPTR       (0xFC)

#define VLDREG_WAIT_THRES       (0x100)

#define VLDREG_BS_PARSED_FLAG		0x104
#define VLD_USERDEF_QMATRIX	0x300

#define VLDREG_SOFT_RESET			0x0108
#define VLD_SW_RESET_CLEAR       0x0
#define VLD_SW_RESET_RESET       0x1
#ifdef JPEG_VLD_V4
#define VLD_LARB_RESET       ((unsigned)0x1 << 4)
#define VLD_ASYNC_FIFO_RST  ((unsigned)0x1 << 8)
#endif

#define VLDREG_RISC_CTRL_WP		0x0110
#define VLD_CTRL_PROG_EN    0x1
#define VLD_RISC_CTRL_WP_EN	0x2
#define VLD_WP_LATCH_EN		0x4


#define VLDREG_MAXCODE			0x0118

#define VLDREG_DRAM_HUFF			0x011C
#define VLD_AC_HUFF_FLAG		0x80000000

/* q table for each component */
#define VLDREG_Q_TBL_LIST		0x0120

/* huffman related registers */
#define VLDREG_LA_HUFF_INDEX	0x0124
#define VLDREG_LA_HUFF_DATA		0x0128

#define VLDREG_JPGMISC		0x012c

#define VLDREG_DC_TBL_LIST		0x0130
#define VLDREG_AC_TBL_LIST		0x0134

/* component info */
#define VLDREG_BLK_PARAM_LIST	0x0138
#define VLD_DCNEED_MASK			0x000003FF
#define VLD_DCNEED_SHIFT		0
#define VLD_ACNEED_MASK			0x000003FF
#define VLD_ACNEED_SHIFT		10
#define VLD_MCUBLK_MASK			0x0000000F
#define VLD_MCUBLK_SHIFT		24

/* block membership (blongs to which component) */
#define VLDREG_MCU_MEMBERSHIP	0x013C

#define VLDREG_START_DECODE			0x140
#define VLD_DECODE_MCU			1

#define VLDREG_JPEG_SWITCH		0x0144
#define VLD_MPEG_MODE		0
#define VLD_JPEG_MODE		1
#define VLD_INTMASK         4
#define VLD_ERRINT_OFF		6

#define VLDREG_FAST_START_DECODE	0x0148
#define VLD_CLEAN_INTERNAL_BUFFER		0
#define VLD_DONT_CLEAN_INTERNAL_BUFFER	1

#define VLDREG_RESTART				0x014C
#define VLD_RESTART				1
#define VLD_RESTART_ALL			0x101
#define VLD_RESTART_WORLD		0x10101
#define VLD_JSTOR               24
#define VLD_JSTOP_RDY           25

#define VLDREG_PROGRESSIVE_MODE		0x0150
#define VLD_PROG_NONE		    0
#define VLD_PROG_DC_REFINE		1
#define VLD_PROG_AC_FIRST		2
#define VLD_PROG_AC_REFINE		3
#define VLD_PROG_DC_FIRST		4
#define VLD_PROG_MASK			0x7
#define VLD_PROG_SHIFT			0
/* progressive mode write out (write to idct if last scan) */
#define VLD_PROG_COEF_WRITE		0
#define VLD_PROG_IDCT_WRITE		1
#define VLD_PROG_WRITE_MASK		1
#define VLD_PROG_WRITE_SHIFT	3
/* select baseline or progressive mode */
#define VLD_DECODE_BASELINE		0
#define VLD_DECODE_PROGRESSIVE	1
#define VLD_DECODE_MODE_MASK	1
#define VLD_DECODE_MODE_SHIFT	8

#define VLDREG_PROGRESSIVE_NONZERO_ONLY		0x0154
#define VLD_PROG_BUFF_ALL		0
#define VLD_PROG_BUFF_NZ_ONLY	1

#define VLDREG_PROGRESSIVE_ALAH		0x0158
#define VLD_PROG_AL_MASK		0xf
#define VLD_PROG_AL_SHIFT		0
#define VLD_PROG_AH_MASK		0xf
#define VLD_PROG_AH_SHIFT		8

#define VLDREG_PROGRESSIVE_SESS		0x015C
#define VLD_PROG_SE_MASK		0x3f
#define VLD_PROG_SE_SHIFT		0
#define VLD_PROG_SS_MASK		0x3f
#define VLD_PROG_SS_SHIFT		8

#define VLDREG_NONE_ZERO_HISTORY	0x0160

#define VLDREG_CURR_MCU				0x0164
#define VLDREG_EOB_RUN_WR			0x0168
#define VLDREG_EOB_RUN_RD			0x01A0

#define VLDREG_READ_START_COMP(X)	(0x016C + (4 * (X)))

#define VLDREG_WRITE_START_COMP(X)	(0x0178 + (4 * (X)))

#define VLDREG_X_ADDR_LIST			0x0184
#define VLDREG_Y_ADDR_LIST			0x0188

#define VLDREG_COMP_PITCH1			0x018C
#define VLDREG_COMP_PITCH2			0x0190
#define VLDREG_COMP_PITCH3			0x0194
#define VLD_PITCH_LO_MASK		0x3FFF
#define VLD_PITCH_LO_SHIFT		0
#define VLD_PITCH_HI_MASK		0x3FFF
#define VLD_PITCH_HI_SHIFT		14

#define VLDREG_MCU_STATUS			0x198
#define VLD_MCU_DECODE_FINISH	1

#ifdef JPEG_VLD_V4
#define VLDREG_JPG_MCU_COEF_SIZE 0x1C0
#define VLDREG_JPG_BST_OUT_MODE  0x1DC
#define JPG_BST_FORMAT_SEL_422           (0x1<<17)
#define JPG_ROW_BST_OUT_ON                  (0x1<<16)
#define JPG_SWAP_CBCR_SEL                     (0x1<<22)
#define JPG_VIDEO_FORMAT_PADDING      (0x1<<21)
#define JPG_VIDEO_FORMAT_OUTPUT        (0x1<<20)
#endif

#define VLDREG_JPEG_FSM				0x0208
#define VLD_JPEG_IDLE			0

#define VLD_JPEG_BS_CHKSUM 0x224

#define VLDREG_JPEG_WINTF_CHKSUM           0x238

#ifdef JPEG_VLD_V4
#define  VLDREG_IQ_TABLE_INDEX                           0x260
#define  VLDREG_IQ_TABLE_DATA_IN                       0x264
#define	 VLDREG_JPEG_WINTF_CRC           				0x278
#endif

#define VLDREG_DEC_MB_ROW           (0x2A8)
#define VLD_DEC_FIRST_ROW       0x1
#define VLD_DEC_NEXT_ROW        0x100

#define VLDREG_MB_ROW_DEC_SWITCH        (0x2AC)
#define VLD_ROW_DEC_WR_ADDR                 0x100
#define VLD_ROW_DEC_WR_BANK1_ADDR           0x200
#define VLD_ROW_DEC_MCU_LEVEL               0x0
#define VLD_ROW_DEC_MCU_ROW_LEVEL           0x1
#define VLD_ROW_DEC_PIC_LEVEL               0x3
#define VLDREG_ROW_DEC_COMP0_ADDR       (0x2B0)
#define VLDREG_ROW_DEC_COMP1_ADDR       (0x2B4)
#define VLDREG_ROW_DEC_COMP2_ADDR       (0x2B8)

#ifdef JPEG_VLD_V4
#define VLDREG_JPG_PARTIALMODE  (0x2C0)
#define JPG_PARTIAL_MODE					(0x1 << 16)
#define JPG_VERT_START_MB_MASK	0x1fff
#define JPG_REV_MCU_ROW_STA					(0x1 << 20)
#define JPG_REV_MCU_ROW_BUSY				(0x1 << 21)
#endif

//JPEG_VLD_V4 don't contains 0x2bc 0x2c0 0x2c4
#ifdef JPEG_VLD_V4
#define VLDREG_JPG_PIC_PIX_WIDTH  (0x2C4)
#else
#define VLDREG_ROW_DEC_COMP0_ADDR_OFST  (0x2BC)
#define VLDREG_ROW_DEC_COMP1_ADDR_OFST  (0x2C0)
#define VLDREG_ROW_DEC_COMP2_ADDR_OFST  (0x2C4)
#endif

#define VLDREG_JPEG_PIC_SIZE            (0x2C8)
#define VLD_ROW_DEC_WIDTH_SHIFT     16
#define VLD_ROW_DEC_HEIGHT_SHIFT    0

#define VLDREG_ROW_PIC_DEC_FINISH       (0x2CC)
#define VLD_ROW_DEC_PIC_FINISH      (0x1 << 8)
#define VLD_ROW_DEC_MCU_ROW_FINISH  (0x1 << 0)

#define VLDREG_RESTART_INTERVAL     (0x2D0)
#define VLD_RST_ENABLE          16


#ifdef JPEG_VLD_V4
#define VLDREG_JPEG_LOAD_INTER_DATA_TRIGGER  0x2D4
#define VLDREG_JPEG_LOAD_INTER_DATA1         0x2D8
#define VLDREG_JPEG_LOAD_INTER_DATA2         0x2DC
#define VLDREG_JPEG_LOAD_INTER_DATA3         0x2E0
#define VLDREG_JPEG_LOAD_INTER_DATA4         0x2E4
#define VLDREG_JPEG_LOAD_INTER_DATA5         0x2E8
#define VLDREG_JPEG_SRAM_CLEAR               0x2EC
#define VLDREG_JPEG_COEF_PITCH_0             0x2EC
#define VLDREG_JPEG_COEF_PITCH_1             0x2F0
#define VLDREG_JPEG_COEF_PITCH_2             0x2F4
#else
#define VLDREG_JPEG_SRAM_CLEAR      (0x2EC)
#endif

#define VLDREG_JNEWBASE		        (0x3C4)
#ifdef JPEG_VLD_V4
#define NEW_BASELINE_ENABLE             0x701
#define NEW_BASELINE_DISABLE            0x701
#define NEW_PROGRESSIVE_MULTI_COLLECT   0x701
#define NEW_PROGRESSIVE_ENHANCE         0x700
#else
#define VLD_NEW_JENGINE           0x7FF
#define VLD_OLD_JENGINE           0x707
#endif

#ifdef JPEG_VLD_V4
#define VLDREG_JPG_HSRAM_INIT               0x3C8
#define SRAM_CLOCK_DISABLE                   0x0
#define SRAM_CLOCK_ENABLE                    0x1
#define FILL_ENABLE                          (0x1 << 1)
#define FILL_DISABLE                         (0x0 << 1)
#endif

#define VLDREG_JED_EN    		    (0x3CC)
#define VLDREG_JEC_EN               (0x3D0)
#define VLD_EC_BEC_LEN_EN                        0 // baseline error code length
#define VLD_EC_GT64_EN                           1
#define VLD_EC_RSTMAK_EN                         4
#define VLDREG_JFILLH                   (0x3D4)
#define VLD_HUFF_DC_AC_SELECTION        12
#define VLD_HUFF_TABLE_IDENTIFIER       8
#define VLD_HUFF_TABLE_ADDR             0
#define VLDREG_JHVAL                    (0x3D8)
#define VLDREG_JERR_STATUS		    (0x3DC)
#ifdef JPEG_VLD_V4
#define VLD_ERR_PROGRSSIVE_SE  9
#define VLD_ERR_RSTMKR_EARLY  5
#define VLD_ERR_RSTMKR	            4
#define VLD_ERR_GT64	            1
#define VLD_ERR_ACLEN	            0
#else
#define VLD_ERR_RSTMKR	            4
#define VLD_ERR_DCRR	            3
#define VLD_ERR_ACSR	            2
#define VLD_ERR_GT64	            1
#define VLD_ERR_ACLEN	            0
#endif
#endif

#define BIG_LOOP 5000

#ifdef JPEG_VLD_V3
#ifdef VLD0_BASE
#undef VLD0_BASE
#define VLD0_BASE   JPG_VLD_BASE
#endif
#endif

#define VLD0_READ32(offset)				IO_READ32	(VLD0_BASE,	(offset))
#define VLD0_WRITE32(offset, value)		IO_WRITE32	(VLD0_BASE,	(offset), (value))

#define IMG_RESZ_START            0
#define IMG_RESZ_TYPE             1
#define IMG_RESZ_JPG_MODE         2
#define RZ_MEM_IF_MODE            3
#define RZ_SRC_BUF_LEN            4
#define RZ_JPEG_INT_SWITCH        5
#define RZ_TG_BUF_LEN             6
#define RZ_SRC_Y_OW_ADDR_BASE1    7
#define RZ_SRC_Y_OW_ADDR_BASE2    8
#define RZ_SRC_CB_OW_ADDR_BASE1   9
#define RZ_SRC_CB_OW_ADDR_BASE2   10
#define RZ_SRC_CR_OW_ADDR_BASE1   11
#define RZ_SRC_CR_OW_ADDR_BASE2   12
#define RZ_TG_Y_OW_ADDR_BASE      13
#define RZ_TG_C_OW_ADDR_BASE      14
#define RZ_SRC_SIZE_Y             16
#define RZ_SRC_SIZE_CB            17
#define RZ_SRC_SIZE_CR            18
#define RZ_TG_SIZE_Y              19
#define RZ_TG_SIZE_C              20
#define RZ_SRC_OFFSET_Y           21
#define RZ_SRC_OFFSET_C           22
#define RZ_SRC_OFFSET_CR          23
#define RZ_TG_OFFSET_Y            24
#define RZ_H8TAPS_SCL_Y           25
#define RZ_H8TAPS_SCL_CB          26
#define RZ_H8TAPS_SCL_CR          27
#define RZ_HEC_SCL_Y              28
#define RZ_HEC_SCL_CB             29
#define RZ_HEC_SCL_CR             30
#define RZ_V_SCL_Y                31
#define RZ_V_SCL_CB               32
#define RZ_V_SCL_CR               33
#define RZ_V4TAPS_SCL_Y           34
#define RZ_V4TAPS_SCL_CB          35
#define RZ_V4TAPS_SCL_CR          36
#define RZ_TMP_Y_OW_ADDR_BASE     37
#define RZ_PLD_Y_OW_ADDR_BASE     38
#define RZ_PLD_C_OW_ADDR_BASE     39
#define RZ_CPU_ASSIGN             40
#define RZ_SRC_CNT_Y              41
#define RZ_SRC_CNT_CB             42
#define RZ_SRC_CNT_CR             43
#define RZ_V_WR_OFFSET_Y          44
#define RZ_V_WR_OFFSET_CB         45
#define RZ_V_WR_OFFSET_CR         46
#define RZ_V_NEXT_C_Y             47
#define RZ_V_NEXT_C_CB            48
#define RZ_V_NEXT_C_CR            49
#define RZ_H8TAPS_OFT_Y           50
#define RZ_H8TAPS_OFT_CB          51
#define RZ_H8TAPS_OFT_CR          52
#define RZ_V4TAPS_OFT_Y           53
#define RZ_V4TAPS_OFT_CB          54
#define RZ_V4TAPS_OFT_CR          55
#define RZ_SPARE_REG1             56
#define RZ_SPARE_REG2             57
#define RZ_CHK_SUM                58 // read only
#define RZ_MONITOR_INT            59 // read only
#define RZ_MONITOR_FSM            60 // read only
#define RZ_MONITOR_STA            61 // read only
#define RZ_MONITOR_DATA           62 // read only
#define IMG_RESZ_DONE             63 // read only
#define IMG_RESZ_OSD_MODE         64
#define RZ_OSD_CTRL               65
#define RZ_OSD_A_TABLE            66
#define RZ_OSD_COLOR_TRANS        67
#define RZ_OSD_COLOR_TRANS1       68
#define RZ_OSD_COLOR_TRANS2       69
#define RZ_OSD_COLOR_TRANS3       70
#define RZ_OSD_CPT_ADDR           71
#define RZ_OSD_CPT_DATA           72
#define RZ_H_COEF0                73
#define RZ_H_COEF1                74
#define RZ_H_COEF2                75
#define RZ_H_COEF3                76
#define RZ_H_COEF4                77
#define RZ_H_COEF5                78
#define RZ_H_COEF6                79
#define RZ_H_COEF7                80
#define RZ_H_COEF8                81
#define RZ_H_COEF9                82
#define RZ_H_COEF10               83
#define RZ_H_COEF11               84
#define RZ_H_COEF12               85
#define RZ_H_COEF13               86
#define RZ_H_COEF14               87
#define RZ_H_COEF15               88
#define RZ_H_COEF16               89
#define RZ_H_COEF17               90
#define RZ_V_COEF0                91
#define RZ_V_COEF1                92
#define RZ_V_COEF2                93
#define RZ_V_COEF3                94
#define RZ_V_COEF4                95
#define RZ_V_COEF5                96
#define RZ_V_COEF6                97
#define RZ_V_COEF7                98
#define RZ_V_COEF8                99
#define RZ_DITHER                 100
#define RZ_CSC                    101
#define RZ_CSC_COEFF_11           102
#define RZ_CSC_COEFF_12           103
#define RZ_CSC_COEFF_13           104
#define RZ_CSC_COEFF_21           105
#define RZ_CSC_COEFF_22           106
#define RZ_CSC_COEFF_23           107
#define RZ_CSC_COEFF_31           108
#define RZ_CSC_COEFF_32           109
#define RZ_CSC_COEFF_33           110
#define RZ_LUMA_KEY               111
#define RZ_COLOR_SEL              112
#define RZ_JPG_V_Y                113
#define RZ_JPG_V_CB               114
#define RZ_JPG_V_CR               115

#define RISCWrite(addr, val)   (*(volatile UINT32 *)(addr)) = (val)
#define RISCRead(addr)         (*(volatile UINT32 *)(addr))
#define IMG_RESZ_BASE  (0xF0020000)


#define MJPEG_LIMT_WIDTH   (1920)
#define MJPEG_COLOR_BUF    (MJPEG_LIMT_WIDTH*16)
#define MJPEG_RZWORK_BUF   (MJPEG_LIMT_WIDTH*12)
#define MJPEG_HUFTABLE_BUF (4*1024)
#define MJPEG_WORKBUF_SIZE ((MJPEG_COLOR_BUF*6)+MJPEG_RZWORK_BUF+(MJPEG_HUFTABLE_BUF*2))
#define MJPEG_VALUE_ALIGN(value,align) (value= ((value&(align-1))!=0) ? ((value+align)&(~(align-1))) : value)

typedef struct _RZ_JPG_SCL_PARAM_SET_T
{
    UINT32 u4IsRsIn;        // block|raster based input
    UINT32 u4YSrcBase1;     // y1
    UINT32 u4YSrcBase2;     // y2
    UINT32 u4YSrcBufLen;
    UINT32 u4CSrcBufLen;
    UINT32 u4YSrcHOffset;
    UINT32 u4YSrcVOffset;
    UINT32 u4YSrcW;
    UINT32 u4YSrcH;
    UINT32 u4CbSrcBase1;    // cb1
    UINT32 u4CbSrcBase2;    // cb2
    UINT32 u4CbSrcHOffset;
    UINT32 u4CbSrcVOffset;
    UINT32 u4CbSrcW;
    UINT32 u4CbSrcH;
    UINT32 u4CrSrcBase1;    // cr1
    UINT32 u4CrSrcBase2;    // cr2
    UINT32 u4CrSrcHOffset;
    UINT32 u4CrSrcVOffset;
    UINT32 u4CrSrcW;
    UINT32 u4CrSrcH;

    UINT32 u4IsRsOut;       // block|raster based output
    UINT32 u4OutMode;       // 420|422|444 output
    UINT32 u4IsVdo2Osd;     // output in osd format ?
    UINT32 u4YTgBase;       // y
    UINT32 u4YTgCM;
    UINT32 u4YTgBufLen;
    UINT32 u4CTgBufLen;
    UINT32 u4YTgHOffset;
    UINT32 u4YTgVOffset;
    UINT32 u4YTgW;
    UINT32 u4YTgH;
    UINT32 u4CTgBase;       // cbcr
    UINT32 u4CTgW;
    UINT32 u4CTgH;

    UINT32 u4IsFstBl;       // first block line of jpg
    UINT32 u4IsLstBl;       // last  block line of jpg
    UINT32 u4JpgVfacY;
    UINT32 u4JpgVfacCb;
    UINT32 u4JpgVfacCr;
    UINT32 u4IsJpgRzOn;     // jpg and rz hand-off on ?
    UINT32 u4IsJpgSameaddr;
    UINT32 u4ColorComp;
    UINT32 u4JpgTempAddr;
    UINT32 u4OutSwap;

    BOOL   fgGray;
    BOOL   fgPreload;
    BOOL   fgBlassign;

    //paritial output
    BOOL fgJpgModePartialOutput;
} RZ_JPG_SCL_PARAM_SET_T;

/********************************************************************
	TYPE DEFINITION
********************************************************************/

typedef void* JPEGHANDLE;

typedef enum _ENUM_JPEG_CLASS_T
{
    E_JC_UNDEFINED_CLASS,
    E_JC_JFIF_HEADER,
    E_JC_VLD_DECODER,
    E_JC_IJG_DECODER,
    E_JC_HW_FLOW,
    E_JC_SW_FLOW,
    E_JC_MEM_YUV,
    E_JC_MEM_RGB,
    E_JC_MEM_ARGB,
    E_JC_MEM_ARGB_GFX,
    E_JC_MEM_MB,
    E_JC_MEM_MB_MTK,
    E_JC_MEM_MW,
    E_JC_LAST
} ENUM_JPEG_CLASS_T;

typedef struct _JPEG_BASE_CLASS_T
{
    ENUM_JPEG_CLASS_T	eJpegClass;
    SIZE_T				zObjectSize;
} JPEG_BASE_CLASS_T;

typedef enum _ENUM_JPEG_MARKER_CODE_T	/* JPEG marker codes */
{
    M_SOF0  = 0xffc0,
    M_SOF1  = 0xffc1,
    M_SOF2  = 0xffc2,
    M_SOF3  = 0xffc3,

    M_SOF5  = 0xffc5,
    M_SOF6  = 0xffc6,
    M_SOF7  = 0xffc7,

    M_JPG   = 0xffc8,
    M_SOF9  = 0xffc9,
    M_SOF10 = 0xffca,
    M_SOF11 = 0xffcb,

    M_SOF13 = 0xffcd,
    M_SOF14 = 0xffce,
    M_SOF15 = 0xffcf,

    M_DHT   = 0xffc4,

    M_DAC   = 0xffcc,

    M_RST0  = 0xffd0,
    M_RST1  = 0xffd1,
    M_RST2  = 0xffd2,
    M_RST3  = 0xffd3,
    M_RST4  = 0xffd4,
    M_RST5  = 0xffd5,
    M_RST6  = 0xffd6,
    M_RST7  = 0xffd7,

    M_SOI   = 0xffd8,
    M_EOI   = 0xffd9,
    M_SOS   = 0xffda,
    M_DQT   = 0xffdb,
    M_DNL   = 0xffdc,
    M_DRI   = 0xffdd,
    M_DHP   = 0xffde,
    M_EXP   = 0xffdf,

    M_APP0  = 0xffe0,
    M_APP1  = 0xffe1,
    M_APP2  = 0xffe2,
    M_APP3  = 0xffe3,
    M_APP4  = 0xffe4,
    M_APP5  = 0xffe5,
    M_APP6  = 0xffe6,
    M_APP7  = 0xffe7,
    M_APP8  = 0xffe8,
    M_APP9  = 0xffe9,
    M_APP10 = 0xffea,
    M_APP11 = 0xffeb,
    M_APP12 = 0xffec,
    M_APP13 = 0xffed,
    M_APP14 = 0xffee,
    M_APP15 = 0xffef,

    M_JPG0  = 0xfff0,
    M_JPG13 = 0xfffd,
    M_COM   = 0xfffe,

    M_TEM   = 0xff01,

    M_ERROR = 0x100
} ENUM_JPEG_MARKER_CODE_T;

/* jpeg format */
typedef enum _ENUM_JPEG_FORMAT_T
{
    E_JPG_UNKNOWN_FORMAT,
    E_JPG_BASELINE,
    E_JPG_EX_SEQ_HUFFMAN,
    E_JPG_PROGRESSIVE_HUFFMAN,
    E_JPG_EX_SEQ_ARITHMETIC,
    E_JPG_PROGRESSIVE_ARITHMETIC,
    E_JPG_LOSSLESS_HUFFMAN,
    E_JPG_DIFFERENTIAL_SEQ_HUFFMAN,
    E_JPG_DIFF_PROG_HUFFMAN,
    E_JPG_DIFF_LLESS_HUFFMAN,
    E_JPG_RESERVED_FOR_EXTENSIONS,
    E_JPG_LOSSLESS_ARITHMETIC,
    E_JPG_DIFF_SEQ_ARITHMETIC,
    E_JPG_DIFF_PROG_ARITHMETIC,
    E_JPG_UNSUPPORT_FORMAT
} ENUM_JPEG_FORMAT_T;

// jpeg decode mode
typedef enum _ENUM_JPEG_DEC_MODE_T
{
    E_JPG_DEC_MODE_NONE,
    E_JPG_DEC_MODE_BASELINE_PIC,                     ///< Jpeg baseline picture, decode whole picture
    E_JPG_DEC_MODE_BASELINE_MCU,                     ///< Jpeg baseline picture, decode MCU
    E_JPG_DEC_MODE_BASELINE_MCU_ROW,                 ///< Jpeg baseline picture, decode MCU row
    E_JPG_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT,    ///< Jpeg progressive picture, decode MCU milti-collect
    E_JPG_DEC_MODE_PROGRESSIVE_MCU_ENHANCE,           ///< Jpeg progressive picture, decode MCU enhance decoding
    E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ,             // jpeg MCU-row + resizer
    E_JPG_DEC_MODE_BASELINE_PIC_RZ,                  // jpeg PIC + resizer
    E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT		//jpg video format output(Y C two buffer)
} ENUM_JPEG_DEC_MODE_T;

/* component info in SOF marker */
typedef struct _JPEG_SOF_COMP_T
{
    UINT8		u1ComponentId;
    UINT8		u1HSampFactor;
    UINT8		u1VSampFactor;
    UINT8		u1QuantTblNo;
} JPEG_SOF_COMP_T;

/* SOF data */
typedef struct _JPEG_JFIF_SOF_T
{
    BOOL					fgSOF;			/* indicate that already have an SOF marker */
    ENUM_JPEG_FORMAT_T		eJpegFormat;
    UINT8					u1DataPrecision;
    UINT16					u2ImageHeight;
    UINT16					u2ImageWidth;
    UINT8					u1NumComponents;
    UINT8					au1MapId2Index[256];
    JPEG_SOF_COMP_T			arSofComp[D_MAX_JPEG_COMPONENT];
} JPEG_JFIF_SOF_T;

/* raw de-huffman table */
typedef struct _JPEG_DHT_HUFF_TBL_T
{
    UINT8 au1Bits[17];
    UINT8 au1HuffVal[256];
} JPEG_DHT_HUFF_TBL_T;

/* DHT data */
typedef struct _JPEG_JFIF_DHT_T
{
    UINT32					u4NumDcTbl;
    UINT32					u4NumAcTbl;
    UINT32					fgDcTblLoaded;	/* bit mask for loaded dc table */
    UINT32					fgAcTblLoaded;	/* bit mask for loaded ac table */
    JPEG_DHT_HUFF_TBL_T		arDcTbl[4];
    JPEG_DHT_HUFF_TBL_T		arAcTbl[4];
} JPEG_JFIF_DHT_T;

/* DQT data */
typedef struct _JPEG_JFIF_DQT_T
{
    /*
    although we leave 2bytes * 64 space here,
    if q table precision is 8bits, we use only
    first half (1x64) of this table
    */
    UINT8				aau1Qtbl[4][128];
    BOOL				afgPrec[4];
    UINT8				u1NumQ;
    BOOL				fgDQT;
} JPEG_JFIF_DQT_T;

typedef enum _ENUM_APP0_THUMBNAIL_MODE_T
{
    E_THUMB_NONE			= 0x00,
    E_THUMB_JPEG			= 0x10,
    E_THUMB_1BYTE_PIX		= 0x11,
    E_THUMB_3BYTE_PIX		= 0x13
} ENUM_APP0_THUMBNAIL_MODE;

typedef enum _ENUM_APP0_TYPE_T
{
    E_APP0_JFIF,
    E_APP0_JFXX,
    E_APP0_ADOBE,
    E_APP0_OTHER
} ENUM_APP0_TYPE_T;

/* APP0 - jfif, jfxx */
typedef struct _JPEG_JFIF_APP0_T
{
    BOOL				fgAPP0;
    ENUM_APP0_TYPE_T	eApp0Type;
    /* jfif */
    UINT16				u2Version;
    UINT8				u1Units;
    UINT16				u2DensityX;
    UINT16				u2DensityY;

    /* jfxx */
    ENUM_APP0_THUMBNAIL_MODE	eThumbnailMode;

    /* common part in all thumbnail mode */
    BOOL				fgHasThumbnail;
    /* thumbnail dimension */
    UINT8				u1ThumbW;
    UINT8				u1ThumbH;
    /* jpeg thumbnail mode */
    UINT8				*pu1JpegThumbnail;	/* point to JPEG data in APP0 stream */
    /* 1 byte pixel thumbnail mode */
    UINT8				*pu1ColorData;
    UINT32				au4Palette[256];	/* force A to 255, R G B read from APP0 */
    /* 3 byte pixel thumbnail mode */
    UINT8				*pu1RGBdata;		/* point to RGB data in APP0 stream */
} JPEG_JFIF_APP0_T;

/* APP1 - exif */
typedef struct _JPEG_JFIF_APP1_T
{
    BOOL				fgAPP1;		/* APP1 marker found */
    UINT8				*pu1ExifData;
    /* TODO: implement thumbnail picture in 1st IFD here */
    /* TODO: implement partial 0th IFD tag here */
} JPEG_JFIF_APP1_T;

typedef struct _JPEG_JFIF_SOS_T
{
    UINT32				u4ScanPass;			/* scan pass */
    UINT8				u1CompInScan;
    UINT8				au1CompId[D_MAX_JPEG_COMPONENT];
    UINT8				au1DcId[D_MAX_JPEG_COMPONENT];
    UINT8				au1AcId[D_MAX_JPEG_COMPONENT];
    UINT8				u1Ss, u1Se, u1AhAl;
    UINT8				*pu1ScanTableStart;
    UINT8				*pu1ScanDataStart;
} JPEG_JFIF_SOS_T;

typedef struct _JPEG_JFIF_HEADER_T
{
    JPEG_BASE_CLASS_T	rParent;
    /* memory address of jpeg image */
    UINT8				*pu1JpegImage;
    /* file size of this jpeg image */
    UINT32				u4ImageByteSize;
    /* ring buffer support */
    BOOL			fgRingBuffer;		/* in ring buffer mode or not */
    UINT32			u4RingBufSize;		/* ring buffer size */
    UINT8			*pu1RingBufStart;	/* ring buffer start address */
    UINT32			u4RingRP;			/* read pointer */
    UINT32			u4RingData;			/* data size in ring buffer */
    /* flags for data status */
    BOOL				fgInited;			/* jpeg image set */
    BOOL				fgParsed;			/* jpeg image parsed successfully */
    BOOL				fgFatalError;		/* fatal error found and can not decode */
    BOOL				fgThumbnail;		/* thumbnail image available (app0 or app1 marker) */
    /* current read pointer */
    UINT8				*pu1CurrRead;
    /* SOI - start of image */
    BOOL				fgSOI;				/* indicate that already have an SOI marker */
    /* SOF - start of frame */
    JPEG_JFIF_SOF_T		rSOF;
    /* DHT - de-huffman table */
    JPEG_JFIF_DHT_T		rDHT;
    /* DQT - de-quantization table */
    JPEG_JFIF_DQT_T		rDQT;
    /* DRI - restart interval */
    UINT16				u2RestartInterval;
    /* FMT - format flag */
    UINT32				fgFormat;			/* JFIF or Exif or ... */
    /* APP0 */
    UINT32				u4NumApp0;
    JPEG_JFIF_APP0_T	arAPP0[D_MAX_NUM_APP0];
    /* APP1 */
    JPEG_JFIF_APP1_T	rAPP1;
    /* SOS - start of scan */
    UINT32				u4NumScan;
    UINT32				u4CurrScan;
    JPEG_JFIF_SOS_T		arSOS[D_MAX_NUM_SCAN];
    JPEG_JFIF_SOS_T		rSOS;
    /* EOI */
    BOOL				fgEOI;					/* EOI reached */
    UINT8				*pu1EOI;				/* point to EOI marker */
    /* LAST SCAN */
    UINT32				au4CompLastScan[D_MAX_JPEG_COMPONENT];
    BOOL 				fgEOF;
} JPEG_JFIF_HEADER_T;

#if 1 //hw.h

typedef struct _DERIVED_HUFF_TABLE_T
{
    /* Basic tables: (element [0] of each array is unused) */
    INT32	ai4Maxcode[18];
    INT32	ai4Valoffset[17];
    INT32	ai4LookNbits[1 << D_HUFF_LOOKAHEAD_BITS];
    UINT8	au1LookSym[1 << D_HUFF_LOOKAHEAD_BITS];
} DERIVED_HUFF_TABLE_T;


typedef struct _JPEG_VLDJPG_DATA_T
{
    JPEG_BASE_CLASS_T	rParent;
    /* global */
    UINT32	fgGlobSet;
    BOOL	fgVldJpgDataReady;
    BOOL	fgProgressive;
    UINT8	u1NumComps;								// number of components in jpeg file
    UINT8	au1CompBlkWidth[D_MAX_JPEG_HW_COMP];	// component width in block of an mcu
    UINT8	au1CompBlkHeight[D_MAX_JPEG_HW_COMP];	// component height in block of an mcu
    UINT8	u1MaxBlkWidth, u1MaxBlkHeight;			// widest, highest
    UINT32	u4PicWidth, u4PicHeight;				// picture width and height
    UINT32	u4McuPerRow;
    UINT32	u4CompPitchRow[D_MAX_JPEG_HW_COMP];		// au1CompBlkWidth[n] * 8 * u4McuPerRow
    // byte pitch of a component
    DERIVED_HUFF_TABLE_T arDcHuffTable[4];
    DERIVED_HUFF_TABLE_T arAcHuffTable[4];

    UINT8	*pu1HwDcHuffTbl;						// aligned address, hw huff tables
    UINT8	*pu1HwAcHuffTbl;

    UINT32	fgAcHuff;								// bit pattern of loaded ac table
    // use only in look ahead table
    UINT8	u1NumBlocks;
    UINT8	au1MemberComp[D_MAX_JPEG_HW_BLOCK];		// indicates a block is a member of which component
    UINT8	*pu1NonZeroBuffer;
    UINT8	*apu1Qtbl[D_MAX_JPEG_QTBL];				// q tables in stream
    UINT32	u4RestartInterval;
    UINT32	u4CurrRestartInterval;
    UINT32	u4McuCount;
    /* current scan */
    UINT32	fgScanSet;
    BOOL	fgScanReady;
    UINT8	*pu1DataStart;
    BOOL	fgRingBuffer;
    UINT32	u4RingBufferSize;
    UINT8	*pu1RingBufStart;
    UINT8	u1NumCompsInCurrScan;					// number of components in current scan
    BOOL	afgCompInScan[D_MAX_JPEG_HW_COMP + 1];
    UINT8	au1CompQ[D_MAX_JPEG_HW_COMP];
    BOOL	afgQtblLoad[D_MAX_JPEG_HW_QTBL];		// vld q table loaded
    UINT8	au1QtblLoadIndex[D_MAX_JPEG_HW_QTBL];	// hw index to stream q tbl index
    UINT8	au1BlkDc[D_MAX_JPEG_HW_BLOCK];
    UINT8	au1BlkAc[D_MAX_JPEG_HW_BLOCK];
    UINT8	au1DcNeed[D_MAX_JPEG_HW_BLOCK];
    UINT8	au1AcNeed[D_MAX_JPEG_HW_BLOCK];
    UINT8	u1AlAh;
    UINT8	u1Se, u1Ss;
    BOOL	fgLastScan;
    UINT32	u4McuRowStart, u4McuRowEnd;				// mcu row range that can output
    UINT32	u4ScanMcuCount;
    /* current mcu row */
    UINT32	fgRowSet;
    BOOL	fgMcuRowReady;
    UINT32	u4CurrMcuRow;
    UINT8	*apu1OutputBuffer[D_MAX_JPEG_HW_COMP];
    UINT8	*apu1OutputBuffer1[D_MAX_JPEG_HW_COMP];
    UINT32	au4OutputBufferSize[D_MAX_JPEG_HW_COMP];
    UINT32	u4RowMcuCount;
    /* current mcu */
    UINT32	fgMcuSet;
    BOOL	fgMcuDataReady;
    UINT32	u4CurrMcuInRow;
    UINT8	*apu1OutputAddr[D_MAX_JPEG_HW_COMP];
    UINT8	*apu1CoefAddr[D_MAX_JPEG_HW_COMP];
    /* misc ... can be global or mcu data */
    UINT32	fgMiscSet;
    BOOL	fgMiscDataReady;
    UINT32	au4BlockAddrX[D_MAX_JPEG_HW_BLOCK];
    UINT32	au4BlockAddrY[D_MAX_JPEG_HW_BLOCK];
    /* eob run */
    INT32	i4SavedEOBRUN;
    UINT32	u4ProgMode;	/* dc_first dc_refine ac_first ac_refine */
    /* for single component in p scan */
    UINT8	u1FirstCompInScan;
    UINT32 u4SkipMcuRowNum;
} JPEG_VLDJPG_DATA_T;

#endif
#if 1// jpg_hw.c

/***********************************************************
* static variable
***********************************************************/
static INT32 _i4JpgCurDecMode5391 = (INT32)E_JPG_DEC_MODE_BASELINE_MCU;
static BOOL _fgJpgHwDetectError = FALSE;

/***********************************************************
* implementation
***********************************************************/
static INT32 VLDJPG_GetDecMode5391(void)
{
    return _i4JpgCurDecMode5391;
}
static INT32 _PollWaitFetchFinish(void)
{
#ifdef JPEG_VLD_V4
    while (! (VLD0_READ32(VLDREG_FETCH_FINISH) & VLD_FETCH_OK));
#else
    while (! (VLD0_READ32(VLDREG_FETCH_FINISH) & VLD_FETCH_OK));
    while ((! (VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & VLD_SRAM_AA_FIT_TAR_SCLK)) &&
            (! (VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & VLD_SRAM_AA_FIT_TAR_D)));
#endif

    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_BarrelShifterSkipBytes(UINT32 u4Bytes)
{
    UINT32 u4Dummy = 0;
    if (u4Bytes)
    {
        while (u4Bytes >= 4)
        {
            u4Dummy = VLD0_READ32(VLD_BARREL_SHIFTER_BITS(32));
            u4Bytes -= 4;
        }

        if (u4Bytes)
        {
            u4Dummy = VLD0_READ32(VLD_BARREL_SHIFTER_BITS(u4Bytes << 3));
        }
    }

    UNUSED(u4Dummy);
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_SetRingBuffer(void *pvStartAddr, UINT32 u4ByteSize)
{
    UINT32 u4StartAddr;
    u4StartAddr = PHYSICAL((UINT32)pvStartAddr);
    if ((u4StartAddr & 0x3f) != 0)
    {
        return -(INT32)E_HWJPG_ERR_PARAM;
    }
    if ((u4ByteSize & 0x3f) != 0)
    {
        return -(INT32)E_HWJPG_ERR_PARAM;
    }

    VLD0_WRITE32(VLDREG_START_ADDRESS, (UINT32)(u4StartAddr >> 6));
    VLD0_WRITE32(VLDREG_END_ADDRESS, (UINT32)((u4StartAddr + u4ByteSize) >> 6));

    return (INT32)E_JPG_OK;
}

static void VLDJPG_SetPowerOn(void)
{
#ifdef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_PWRSAVE, (UINT32)~(VLD_PDN_SCLK+VLD_PDN_DCLK+VLD_PDN_IOMMU_DCLK));
#else
    VLD0_WRITE32(VLDREG_PWRSAVE, (UINT32)~(VLD_PDN_SCLK+VLD_PDN_DCLK));
#endif
}

static void VLDJPG_SetPowerOff(void)
{
#ifdef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_PWRSAVE, (UINT32)(VLD_PDN_SCLK+VLD_PDN_DCLK+VLD_PDN_IOMMU_DCLK));
#else
    VLD0_WRITE32(VLDREG_PWRSAVE, (UINT32)(VLD_PDN_SCLK+VLD_PDN_DCLK));
#endif
}

static void VLDJPG_SetLinearBuffer(void)
{
    VLD0_WRITE32(VLDREG_START_ADDRESS, (UINT32)0);
    VLD0_WRITE32(VLDREG_END_ADDRESS, (UINT32)~0);
}

static INT32 VLDJPG_SetBarrelShifter(UINT8 *pu1MemAddr)
{
    UINT32 u4NonCacheAddr;
    UINT32 u4NonAlignByteCount;

    u4NonCacheAddr = PHYSICAL((UINT32)pu1MemAddr);

#ifdef JPEG_VLD_V4
    if ((VLD0_READ32(VLDREG_FETCH_FINISH) & VLD_INIT_VALID_FLG))
        while (!(VLD0_READ32(VLDREG_VID_BUF_CTL_SIGNAL) & 1));
#endif

    VLD0_WRITE32(VLDREG_READ_POINTER, u4NonCacheAddr);

//Reset async fifo
#ifdef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_SOFT_RESET,VLD_ASYNC_FIFO_RST);
    VLD0_WRITE32(VLDREG_SOFT_RESET,VLD_SW_RESET_CLEAR);
#endif

    VLD0_WRITE32(VLDREG_RISC_PROCESS, (UINT32)VLD_INIT_FETCH_DRAM);

    if ((INT32)E_HWJPG_OK != _PollWaitFetchFinish())
    {
        return -(INT32)E_HWJPG_ERR_SET_BS;
    }
    VLD0_WRITE32(VLDREG_RISC_PROCESS, (UINT32)VLD_INIT_FILL_BS);

    /* shift barrel shifter to non-128bit align address */
    u4NonAlignByteCount = (UINT32)pu1MemAddr & ALIGN_MASK(128);

    return VLDJPG_BarrelShifterSkipBytes(u4NonAlignByteCount);
}


static void VLDJPG_SetRiscCtrlWP(UINT32 u4VldWptr)
{
    UINT32 u4RegVal;
#ifdef JPEG_VLD_V2
    u4RegVal = (u4VldWptr << 3) | (VLD_CTRL_PROG_EN);
#else
    u4RegVal = (u4VldWptr << 4) | (VLD_RISC_CTRL_WP_EN | VLD_WP_LATCH_EN);
#endif

#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_RISC_CTRL_WP, u4VldWptr);
    UNUSED(u4RegVal);
#else
    VLD0_WRITE32(VLDREG_RISC_CTRL_WP, u4RegVal);
#endif
}

static BOOL VLDJPGFillQTBLRdy(void)
{
    if (VLD0_READ32(VLDREG_JPGMISC) & 0x1)
        return TRUE;

    return FALSE;
}

static INT32 VLDJPG_LoadTblQ(UINT8 u1Qn, UINT8 *pu1Qtbl)
{
#ifdef  JDEC_QTABLE_RISC_MODE
    INT32 i4Index;
    UINT32 *rQtable0Addr;
    UINT32 *rQtable1Addr;
#endif

    UINT32 u4Dummy;

    if (u1Qn > 1)
    {
        // vld hardware currently supports only 2 quantization table
        return -(INT32)E_HWJPG_ERR_QTBL_INDEX;
    }

    VLDJPG_SetLinearBuffer();

    if ((INT32)E_HWJPG_OK != VLDJPG_SetBarrelShifter(pu1Qtbl))
    {
        return -(INT32)E_HWJPG_ERR_LOAD_Q;
    }

    if (u1Qn== 0)
    {

#ifdef  JDEC_QTABLE_RISC_MODE
        rQtable0Addr = (UINT32 *)(pu1Qtbl);

        VLD0_WRITE32(VLDREG_IQ_TABLE_INDEX,0);
        for (i4Index = 0; i4Index < 16; i4Index++)
        {
            VLD0_WRITE32(VLDREG_IQ_TABLE_DATA_IN,(UINT32)*(rQtable0Addr + i4Index));
        }
#else
        u4Dummy = VLD0_READ32(VLDREG_LOAD_INTRA_Q_MATRIX);
#endif
    }
    else
    {
#ifdef JDEC_QTABLE_RISC_MODE
        rQtable1Addr = (UINT32 *)(pu1Qtbl);

        VLD0_WRITE32(VLDREG_IQ_TABLE_INDEX,0x40);
        for (i4Index = 0; i4Index < 16; i4Index++)
        {
            VLD0_WRITE32(VLDREG_IQ_TABLE_DATA_IN,(UINT32)*(rQtable1Addr + i4Index));
        }
#else
        u4Dummy = VLD0_READ32(VLDREG_LOAD_NONINTRA_Q_MATRIX);
#endif
    }

#ifdef JPEG_VLD_V4
    while (TRUE)
    {
        if (VLDJPGFillQTBLRdy())
            break;
    }
#endif
    UNUSED(u4Dummy);

    return (INT32)E_HWJPG_OK;
}
static void VLDJPG_SetUserDefQMatrix(void)
{
    VLD0_WRITE32(VLDREG_BS_PARSED_FLAG, VLD_USERDEF_QMATRIX);
}
static INT32 VLDJPG_CalcDerivedHuffTable(const UINT8 *au1Bits, const UINT8 *au1HuffVal, DERIVED_HUFF_TABLE_T *prDerivedHuffTbl)
{
    //int numsymbols;
    INT32	i4BitLen, i4Symbol, i4LookBits, i4Ctr, i4Size, i4Loop1, i4Loop2;
    INT16	i2Bits;

    INT8	ai1HuffSize[257];
    UINT16	au2HuffCode[257];
    UINT16	u2Code;

    ASSERT(au1Bits != NULL);
    ASSERT(au1HuffVal != NULL);
    ASSERT(prDerivedHuffTbl != NULL);

    /* Note that huffsize[] and huffcode[] are filled in code-length order,
     * paralleling the order of the symbols themselves in htbl->huffval[].
     */

    for (i4Loop1 = 0; i4Loop1 < 257; i4Loop1++)
    {
        ai1HuffSize[i4Loop1]=0;
        au2HuffCode[i4Loop1]=0;
    }
    /* Figure C.1: make table of Huffman code length for each symbol */

    i4Symbol = 0;
    for (i4BitLen = 1; i4BitLen <= 16; i4BitLen++)
    {
        i2Bits = (INT16)au1Bits[i4BitLen];

        if ((i4Symbol + i2Bits) > 256) /* protect against table overrun */
        {
            // Error handler:
            // set to nearest value
            i2Bits = 255 - i4Symbol;	/* p + i = 255 */
        }

        while (i2Bits--)  // safe while
        {
            ai1HuffSize[i4Symbol++] = (INT8) i4BitLen;
        }
    }

    ai1HuffSize[i4Symbol] = 0;
    //numsymbols = p;

    /* Figure C.2: generate the codes themselves */
    /* We also validate that the counts represent a legal Huffman code tree. */

    i4Symbol	= 0;
    u2Code		= 0;
    i4Size		= ai1HuffSize[0];


    for (i4Loop1 = 0; i4Loop1 < BIG_LOOP; i4Loop1++)
    {
        if (i4Symbol > 256)
        {
            i4Symbol = 256;
        }
        if (ai1HuffSize[i4Symbol])
        {
            for (i4Loop2 = 0; i4Loop2 < BIG_LOOP; i4Loop2++)
            {
                if (((INT32) ai1HuffSize[i4Symbol]) == i4Size)
                {
                    au2HuffCode[i4Symbol++] = u2Code;
                    u2Code++;
                }
                else
                {
                    break;
                }
            }
            /* code is now 1 more than the last code used for codelength si; but
            * it must still fit in si bits, since no code is allowed to be all ones.
            */

            if (((INT32) u2Code) >= (((INT32) 1) << i4Size))
            {
                // Error handler:
                // set to 0
                u2Code = 0;
            }

            u2Code <<= 1;
            i4Size++;
        }// huffsize[9]
        else
        {
            break;
        }
    }// BIG_LOOP

    /* Figure F.15: generate decoding tables for bit-sequential decoding */

    i4Symbol = 0;
    for (i4BitLen = 1; i4BitLen <= 16; i4BitLen++)
    {
        if (au1Bits[i4BitLen])
        {
            /* valoffset[l] = huffval[] index of 1st symbol of code length l,
             * minus the minimum code of length l
             */
            prDerivedHuffTbl->ai4Valoffset[i4BitLen] =
                (INT32)i4Symbol - (INT32)au2HuffCode[i4Symbol];
            i4Symbol += au1Bits[i4BitLen];
            if (!i4Symbol)
            {
                i4Symbol = 1;
            }
            prDerivedHuffTbl->ai4Maxcode[i4BitLen] =
                (INT32)au2HuffCode[i4Symbol - 1];		/* maximum code of length l */
        }
        else
        {
            prDerivedHuffTbl->ai4Valoffset[i4BitLen] = 0;
            /* clear to zero if no codes of this length */
            prDerivedHuffTbl->ai4Maxcode[i4BitLen] = -1;
            /* -1 if no codes of this length */
        }
    }
    prDerivedHuffTbl->ai4Maxcode[17] = (INT32)0xFFFFF;
    /* ensures jpeg_huff_decode terminates */

    /* Compute lookahead tables to speed up decoding.
     * First we set all the table entries to 0, indicating "too long";
     * then we iterate through the Huffman codes that are short enough and
     * fill in all the entries that correspond to bit sequences starting
     * with that code.
     */

    for (i4Loop1 = 0; i4Loop1 < (1 << D_HUFF_LOOKAHEAD_BITS); i4Loop1++)
    {
        prDerivedHuffTbl->ai4LookNbits[i4Loop1] = 0;
    }

    i4Symbol = 0;
    for (i4BitLen = 1; i4BitLen <= D_HUFF_LOOKAHEAD_BITS; i4BitLen++)
    {
        for (i2Bits = 1; i2Bits <= (INT16) au1Bits[i4BitLen]; i2Bits++, i4Symbol++)
        {
            /* i4BitLen = current code's length, i4Symbol = its index in huffcode[] & huffval[]. */
            /* Generate left-justified code followed by all possible bit sequences */
            i4LookBits = (INT32)au2HuffCode[i4Symbol] << (D_HUFF_LOOKAHEAD_BITS - i4BitLen);
            for (i4Ctr = 1 << (D_HUFF_LOOKAHEAD_BITS - i4BitLen); i4Ctr > 0; i4Ctr--)
            {
                prDerivedHuffTbl->ai4LookNbits[i4LookBits] = i4BitLen;
                prDerivedHuffTbl->au1LookSym[i4LookBits] = au1HuffVal[i4Symbol];
                i4LookBits++;
            }
        }
    }

    return (INT32)E_HWJPG_OK;
}

static void VLDJPG_SetMaxcodeSequence(INT32 u4Value)
{
    VLD0_WRITE32(VLDREG_LA_HUFF_INDEX, u4Value);
}

// maxcode and valoffset
static INT32 VLDJPG_LoadMaxcode(const INT32 *ai4Value)
{
    // decoder ap should load maxcode and valoffset follow this procedure

    INT32 i4Index;

    ASSERT(ai4Value != NULL);
    for (i4Index = 1; i4Index <= 16; i4Index++)
    {
        VLD0_WRITE32(VLDREG_MAXCODE, (UINT32)(ai4Value[i4Index]));
    }
    return (INT32)E_HWJPG_OK;
}

static void VLDJPG_TransHuffTblToHwFormat(UINT8 *pu1DstAddr, const UINT8 *au1Huffval)
{
    UINT8 *pu1Huff;
    INT32 i4Index;

    ASSERT(au1Huffval != NULL);
    ASSERT(pu1DstAddr != NULL);

    pu1Huff = pu1DstAddr;

    for (i4Index = 0; i4Index < 256; (i4Index += 4), (pu1Huff += 4))
    {
        // every 128 bits, write only 32 bits data, rest 96 bits fill zero
        //x_memcpy(pu1Huff, &au1Huffval[i4Index], 4);
        pu1Huff[0] = au1Huffval[i4Index + 0];
        pu1Huff[1] = au1Huffval[i4Index + 1];
        pu1Huff[2] = au1Huffval[i4Index + 2];
        pu1Huff[3] = au1Huffval[i4Index + 3];
        //x_memset(pu1Huff + 4, 0, 12);
    }
    //HalFlushDCache();
#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)
    HalFlushInvalidateDCacheMultipleLine((UINT32)VIRTUAL(pu1DstAddr), (UINT32)(256));
#else
    HalFlushInvalidateDCache();
#endif
}

// huff value address

static INT32 VLDJPG_SetHuffTblVal(UINT8 *au1Huffval, BOOL fgDCTbl)
{
    UINT32 u4Val, u4TblNum, u4I;
    UINT8 *pu1Huffval;

#ifdef __KERNEL__
    au1Huffval=(UINT8*)(VIRTUAL(au1Huffval));
#endif

    for (u4TblNum = 0; u4TblNum < D_MAX_JPEG_HUFF_TBL; u4TblNum++)
    {
        u4Val = u4TblNum << VLD_HUFF_TABLE_IDENTIFIER;
        if (!fgDCTbl)
        {
            u4Val |= (1 << VLD_HUFF_DC_AC_SELECTION);
        }

        VLD0_WRITE32(VLDREG_JFILLH, u4Val);

        pu1Huffval = au1Huffval + (u4TblNum << 8);

        for (u4I = 0; u4I < 256; u4I += 4)
        {
            u4Val = ((pu1Huffval[u4I + 3] << 24) | (pu1Huffval[u4I + 2] << 16) |
                     (pu1Huffval[u4I + 1] << 8) | pu1Huffval[u4I]);
            VLD0_WRITE32(VLDREG_JHVAL, u4Val);
        }
    }

    return (INT32)E_HWJPG_OK;
}

// set dc needed flag (1bit) for each block (maximum 10)
static INT32 VLDJPG_SetDcNeededList(UINT32 u4DcNeededList)
{
    UINT32 u4Reg;

    u4Reg = VLD0_READ32(VLDREG_BLK_PARAM_LIST);
    u4Reg = (u4Reg & ~(VLD_DCNEED_MASK << VLD_DCNEED_SHIFT)) |
            ((u4DcNeededList & VLD_DCNEED_MASK) << VLD_DCNEED_SHIFT);
    VLD0_WRITE32(VLDREG_BLK_PARAM_LIST, u4Reg);

    return (INT32)E_HWJPG_OK;
}

// set ac needed flag (1bit) for each block (maximum 10)
static INT32 VLDJPG_SetAcNeededList(UINT32 u4AcNeededList)
{
    UINT32 u4Reg;
    u4Reg = VLD0_READ32(VLDREG_BLK_PARAM_LIST);
    u4Reg = (u4Reg & ~(VLD_ACNEED_MASK << VLD_ACNEED_SHIFT)) | ((u4AcNeededList & VLD_ACNEED_MASK) << VLD_ACNEED_SHIFT);
    VLD0_WRITE32(VLDREG_BLK_PARAM_LIST, u4Reg);

    return (INT32)E_HWJPG_OK;
}

// set block count in an MCU
static INT32 VLDJPG_SetBlockCount(UINT32 u4BlockCount)
{
    UINT32 u4Reg;
    u4Reg = VLD0_READ32(VLDREG_BLK_PARAM_LIST);
    u4Reg = (u4Reg & ~(VLD_MCUBLK_MASK << VLD_MCUBLK_SHIFT)) | ((u4BlockCount & VLD_MCUBLK_MASK) << VLD_MCUBLK_SHIFT);
    VLD0_WRITE32(VLDREG_BLK_PARAM_LIST, u4Reg);

    return (INT32)E_HWJPG_OK;
}

// set membership register
static INT32 VLDJPG_SetMemberShip(UINT32 u4Membership)
{
    VLD0_WRITE32(VLDREG_MCU_MEMBERSHIP, u4Membership);
    return (INT32)E_HWJPG_OK;
}

// set dc & ac table for each block
// maximum 10 blocks (4,4,2) for mtk vld hardware
// each block can be one of table 0 1 2 3
static INT32 VLDJPG_SetDcTableList(UINT32 u4DcTableList)
{
    VLD0_WRITE32(VLDREG_DC_TBL_LIST, u4DcTableList);
    return (INT32)E_HWJPG_OK;
}
static INT32 VLDJPG_SetAcTableList(UINT32 u4AcTableList)
{
    VLD0_WRITE32(VLDREG_AC_TBL_LIST, u4AcTableList);
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_SetQuanTableList(UINT32 u4QuanTableList)
{
    VLD0_WRITE32(VLDREG_Q_TBL_LIST, u4QuanTableList);
    return (INT32)E_HWJPG_OK;
}

// switch VLD to jpeg decoding mode only when start jpeg decoding,
// after jpeg operation, switch back to mpeg
static INT32 VLDJPG_Switch2Jpeg(void)
{
    VLD0_WRITE32(VLDREG_JPEG_SWITCH, VLD_JPEG_MODE);
    return (INT32)E_HWJPG_OK;
}

// clean buffer only when decode 1st mcu
// after 1st mcu, we can keep buffer for fast decode
static INT32 VLDJPG_Switch2CleanBuffer(void)
{
#ifndef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_FAST_START_DECODE, VLD_CLEAN_INTERNAL_BUFFER);
#endif
    return (INT32)E_HWJPG_OK;
}

// set decoding mode
static INT32 VLDJPG_Switch2Baseline(void)
{
    VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, VLD_DECODE_BASELINE << VLD_DECODE_MODE_SHIFT);
    return (INT32)E_HWJPG_OK;
}


// set one of four progressive mode:
// ac_first, ac_refine, dc_first, dc_refine
static INT32 VLDJPG_SetProgressiveMode(UINT8 u1ProgMode)
{
    UINT32 u4Reg;
    u4Reg = VLD0_READ32(VLDREG_PROGRESSIVE_MODE);
    u4Reg = (u4Reg & (~(VLD_PROG_MASK << VLD_PROG_SHIFT))) | ((UINT32)u1ProgMode << VLD_PROG_SHIFT);
    VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, u4Reg);
    return (INT32)E_HWJPG_OK;
}


// indicate this is the last scan of a progressive image
static INT32 VLDJPG_SetProgressiveLastScan(BOOL fgLastScan)
{
    UINT32 u4Reg;
    u4Reg = VLD0_READ32(VLDREG_PROGRESSIVE_MODE);
    u4Reg = (u4Reg & (~(VLD_PROG_WRITE_MASK << VLD_PROG_WRITE_SHIFT))) |
            ((UINT32)fgLastScan << VLD_PROG_WRITE_SHIFT);
    VLD0_WRITE32(VLDREG_PROGRESSIVE_MODE, u4Reg);
    return (INT32)E_HWJPG_OK;
}

// specify Ah and Al in progressive decoding process
static INT32 VLDJPG_SetProgressiveAhAl(UINT8 u1AhAl)
{
    VLD0_WRITE32(VLDREG_PROGRESSIVE_ALAH, ((u1AhAl & 0xf) << VLD_PROG_AL_SHIFT) | ((u1AhAl >> 4) << VLD_PROG_AH_SHIFT));
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_SetProgressiveSeSs(UINT8 u1Se, UINT8 u1Ss)
{
    VLD0_WRITE32(VLDREG_PROGRESSIVE_SESS,
                 ((u1Se & VLD_PROG_SE_MASK) << VLD_PROG_SE_SHIFT) |
                 ((u1Ss & VLD_PROG_SS_MASK) << VLD_PROG_SS_SHIFT));
    return (INT32)E_HWJPG_OK;
}

// set none zero history buffer for progressive decoding process
static INT32 VLDJPG_SetProgressiveNoneZeroHistory(UINT8 *pu1NoneZeroHistory)
{
    UINT32 u4NzHist;
    u4NzHist = (UINT32)pu1NoneZeroHistory;
    VLD0_WRITE32(VLDREG_NONE_ZERO_HISTORY, u4NzHist);
    return (INT32)E_HWJPG_OK;
}


// write x address list and y address list for each block
// each block has 3 bits
static INT32 VLDJPG_SetBlockAddrList(const UINT32 *au4XAddrList, const UINT32 *au4YAddrList)
{
    INT32 i4Blk;
    UINT32 u4XAddrList = 0, u4YAddrList = 0;

    ASSERT(au4XAddrList != NULL);
    ASSERT(au4YAddrList != NULL);

    for (i4Blk = 0; i4Blk < 10; i4Blk++)
    {
        u4XAddrList |= (au4XAddrList[i4Blk] & 0x7) << (i4Blk * 3);
        u4YAddrList |= (au4YAddrList[i4Blk] & 0x7) << (i4Blk * 3);
    }
    VLD0_WRITE32(VLDREG_X_ADDR_LIST, u4XAddrList);
    VLD0_WRITE32(VLDREG_Y_ADDR_LIST, u4YAddrList);

    return (INT32)E_HWJPG_OK;
}

// write idct pitch of each component
static INT32 VLDJPG_SetCompIdctPitch(const UINT32 *au4CompPitch)
{
    UINT32 u4Reg;

    ASSERT(au4CompPitch != NULL);

    if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
            (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ)||
            (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT))
    {
        VLD0_WRITE32(VLDREG_COMP_PITCH1,
                     (((au4CompPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
                     (((au4CompPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));

        u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
        u4Reg = (u4Reg & ~(VLD_PITCH_LO_MASK << VLD_PITCH_LO_SHIFT)) |
                (((au4CompPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT);
        VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);
    }
    else
    {
        VLD0_WRITE32(VLDREG_COMP_PITCH1,
                     (((au4CompPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
                     (((au4CompPitch[1] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));

        u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
        u4Reg = (u4Reg & ~(VLD_PITCH_LO_MASK << VLD_PITCH_LO_SHIFT)) |
                (((au4CompPitch[2] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT);
        VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);
    }
    return (INT32)E_HWJPG_OK;
}

// write coef pitch of each component
static INT32 VLDJPG_SetCompCoefPitch(const UINT32 *au4CoefPitch)
{
    UINT32 u4Reg;

    ASSERT(au4CoefPitch != NULL);

    if ((VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ) ||
            (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_RZ)||
            (VLDJPG_GetDecMode5391() == E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT))
    {
        u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
        u4Reg = (u4Reg & (~(VLD_PITCH_HI_MASK << VLD_PITCH_HI_SHIFT))) |
                (((au4CoefPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT);
        VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);

        VLD0_WRITE32(VLDREG_COMP_PITCH3,
                     (((au4CoefPitch[0] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
                     (((au4CoefPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));
    }
    else
    {
        u4Reg = VLD0_READ32(VLDREG_COMP_PITCH2);
        u4Reg = (u4Reg & (~(VLD_PITCH_HI_MASK << VLD_PITCH_HI_SHIFT))) |
                (((au4CoefPitch[0] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT);
        VLD0_WRITE32(VLDREG_COMP_PITCH2, u4Reg);

        VLD0_WRITE32(VLDREG_COMP_PITCH3,
                     (((au4CoefPitch[1] >> 2) & VLD_PITCH_LO_MASK) << VLD_PITCH_LO_SHIFT) |
                     (((au4CoefPitch[2] >> 2) & VLD_PITCH_HI_MASK) << VLD_PITCH_HI_SHIFT));
    }
    return (INT32)E_HWJPG_OK;
}

// start decode process (pic mode, not polling in this api)
static void VLDJPG_DecodePic(void)
{
    VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_DEC_FIRST_ROW);
    VLD0_WRITE32(VLDREG_DEC_MB_ROW, VLD_CLEAR);
}

static INT32 VLDJPG_SoftwareReset(void)
{
#ifdef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_SOFT_RESET, (VLD_SW_RESET_RESET |VLD_LARB_RESET |VLD_ASYNC_FIFO_RST));
#else
    VLD0_WRITE32(VLDREG_SOFT_RESET, VLD_SW_RESET_RESET);
#endif

    VLD0_WRITE32(VLDREG_SOFT_RESET, VLD_SW_RESET_CLEAR);
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_SramClear(void)
{
    VLD0_WRITE32(VLDREG_JPEG_SRAM_CLEAR, 0x0);
    VLD0_WRITE32(VLDREG_JPEG_SRAM_CLEAR, 0x100);
    VLD0_WRITE32(VLDREG_JPEG_SRAM_CLEAR, 0x0);
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_Restart()
{
    VLD0_WRITE32(VLDREG_RESTART, VLD_RESTART);
    VLD0_WRITE32(VLDREG_RESTART, VLD_CLEAR);
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_AdvanceRestartMarker(UINT8 *pu1Marker)
{
    UINT8 u1RestartMarker;
    u1RestartMarker = (UINT8)((VLD0_READ32(VLDREG_MCU_STATUS) >> 8) & 0xff);
    if (pu1Marker)
    {
        *pu1Marker = u1RestartMarker;
    }
    return (INT32)E_HWJPG_OK;
}

static INT32 VLDJPG_InitScan(void)
{
    VLD0_WRITE32(VLDREG_RESTART, VLD_CLEAR);
    VLD0_WRITE32(VLDREG_RESTART, VLD_RESTART_WORLD);
    VLD0_WRITE32(VLDREG_RESTART, VLD_CLEAR);
    return (INT32)E_HWJPG_OK;
}

//==========================================================

// start hardware decoder, init data structure (allocate vld0)
static INT32 VLDJPG_New(const JPEGHANDLE hInstance)
{
    IO_WRITE32(CKGEN_BASE, 0x310, 2);

    VLDJPG_SetPowerOn();
    VERIFY(VLDJPG_SoftwareReset() == (INT32)E_HWJPG_OK);
    VLDJPG_SramClear();
#ifndef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_LATCH2, 0x010E0103);
#endif
    UNUSED(hInstance);
    return VLDJPG_Switch2Jpeg();
}




//////////////////////////////////////////////////////////////
//
// Picture Mode (MCU Row Mode) Related Function
//
//////////////////////////////////////////////////////////////
// read address of components
static INT32 VLDJPG_SetCompReadAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
    VLD0_WRITE32(VLDREG_READ_START_COMP(0), (u4Comp0Addr >> 2));
    VLD0_WRITE32(VLDREG_READ_START_COMP(1), (u4Comp1Addr >> 2));
    VLD0_WRITE32(VLDREG_READ_START_COMP(2), (u4Comp2Addr >> 2));
    return (INT32)E_HWJPG_OK;
}

// write address of components
static INT32 VLDJPG_SetCompWriteAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
    VLD0_WRITE32(VLDREG_WRITE_START_COMP(0), (u4Comp0Addr >> 2));
    VLD0_WRITE32(VLDREG_WRITE_START_COMP(1), (u4Comp1Addr >> 2));
    VLD0_WRITE32(VLDREG_WRITE_START_COMP(2), (u4Comp2Addr >> 2));
    return (INT32)E_HWJPG_OK;
}

static void VLDJPG_SetOutputBank0BufAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
    UINT32 u4Value;
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value |= VLD_ROW_DEC_WR_ADDR;
    u4Value &= ~(VLD_ROW_DEC_WR_BANK1_ADDR);

    VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP0_ADDR, (u4Comp0Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP1_ADDR, (u4Comp1Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP2_ADDR, (u4Comp2Addr >> 2));
}

static void VLDJPG_SetOutputBank1BufAddr5391(UINT32 u4Comp0Addr, UINT32 u4Comp1Addr, UINT32 u4Comp2Addr)
{
    UINT32 u4Value;
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value |= VLD_ROW_DEC_WR_ADDR;
    u4Value |= VLD_ROW_DEC_WR_BANK1_ADDR;

    VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP0_ADDR, (u4Comp0Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP1_ADDR, (u4Comp1Addr >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP2_ADDR, (u4Comp2Addr >> 2));
}

static void VLDJPG_SetMCUWidth5391(UINT8 compNum, UINT32 u4Comp0MCUWidth, UINT32 u4Comp1MCUWidth, UINT32 u4Comp2MCUWidth)
{
    UINT32 u4Value=0;
    //set each component MCU width
#ifdef JPEG_VLD_V4
    if (compNum == 1)
    {
        u4Value = (1 << 8) + (1 << 4) + 1;
    }
    else
    {
        u4Value += (u4Comp0MCUWidth>>3);
        u4Value += ((u4Comp1MCUWidth>>3) << 4);
        u4Value += ((u4Comp2MCUWidth>>3) << 8);
    }
    VLD0_WRITE32(VLDREG_JPG_MCU_COEF_SIZE,u4Value);

#else
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP0_ADDR_OFST, (u4Comp0MCUWidth >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP1_ADDR_OFST, (u4Comp1MCUWidth >> 2));
    VLD0_WRITE32(VLDREG_ROW_DEC_COMP2_ADDR_OFST, (u4Comp2MCUWidth >> 2));

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value &= ~(VLD_ROW_DEC_WR_ADDR | VLD_ROW_DEC_WR_BANK1_ADDR);
    VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
#endif


}

static void VLDJPG_SetPicSize5391(UINT32 u4MCUsPerRow, UINT32 u4TotalRows)
{
    UINT32 u4Value = 0;
    u4Value = ((u4MCUsPerRow << VLD_ROW_DEC_WIDTH_SHIFT) | (u4TotalRows));
    VLD0_WRITE32(VLDREG_JPEG_PIC_SIZE, u4Value);
}

static void VLDJPG_SetRestartInterval5391(UINT32 u4RestartInterval)
{
#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_RESTART_INTERVAL, ((u4RestartInterval-1) & 0xFFFF) | (1 << VLD_RST_ENABLE));
#else
    VLD0_WRITE32(VLDREG_RESTART_INTERVAL, ((u4RestartInterval-1) & 0xFFFF));
#endif
}

static void JPG_SetDecMode5391(INT32 i4DecMode)
{
    _i4JpgCurDecMode5391 = i4DecMode;
}

static void VLDJPG_SetDecMode5391(INT32 i4DecMode)
{
    UINT32 u4Value;

    u4Value = VLD0_READ32(VLDREG_MB_ROW_DEC_SWITCH);
    u4Value &= ~(VLD_ROW_DEC_MCU_LEVEL | VLD_ROW_DEC_MCU_ROW_LEVEL | VLD_ROW_DEC_PIC_LEVEL);

    switch ((ENUM_JPEG_DEC_MODE_T)i4DecMode)
    {
    case E_JPG_DEC_MODE_BASELINE_MCU_ROW:
    case E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ:
        u4Value |= VLD_ROW_DEC_MCU_ROW_LEVEL;
        VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
        break;
    case E_JPG_DEC_MODE_BASELINE_PIC:
    case E_JPG_DEC_MODE_BASELINE_PIC_RZ:
    case E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT:
        u4Value |= VLD_ROW_DEC_PIC_LEVEL;
        VLD0_WRITE32(VLDREG_MB_ROW_DEC_SWITCH, u4Value);
        break;
    default:
        ASSERT(0);
        return;
    }
}

static void VLDJPG_UseNewEngine(void)
{
#ifdef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_JNEWBASE, NEW_BASELINE_ENABLE);
#else
#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_JNEWBASE, VLD_NEW_JENGINE);
#endif
#endif
}
static void VLDJPG_HwIsr(UINT16 u2Vector)
{
#ifdef JPEG_VLD_V4
    UINT32 u4Value;

    ASSERT(u2Vector == (UINT16)VECTOR_JPGDEC);
    if (BIM_ClearIrq((UINT32)VECTOR_JPGDEC))
    {
        if (VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) == 0)
        {
            u4Value = VLD0_READ32(VLDREG_JERR_STATUS);
            if (u4Value & (UINT32)(1 << VLD_ERR_ACLEN))
            {
                //LOG(3, "*E* VLD_ERR_ACLEN\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_GT64))
            {
                //LOG(3, "*E* VLD_ERR_GT64\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_RSTMKR))
            {
                //LOG(3, "*E* VLD_ERR_ACSR\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_RSTMKR_EARLY))
            {
                //LOG(3, "*E* VLD_ERR_RSTMKR\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_PROGRSSIVE_SE))
            {
                //LOG(3, "*E* VLD_ERR_RSTMKR\n");
            }
            _fgJpgHwDetectError = TRUE;
        }
    }
#else
#ifdef JPEG_VLD_V3
    UINT32 u4Value;

    ASSERT(u2Vector == (UINT16)VECTOR_JPGDEC);
    if (BIM_ClearIrq((UINT32)VECTOR_JPGDEC))
    {
        //LOG(9, "JPG IRQ\n");
        if (VLD0_READ32(VLDREG_ROW_PIC_DEC_FINISH) == 0)
        {
            u4Value = VLD0_READ32(VLDREG_JERR_STATUS);
            if (u4Value & (UINT32)(1 << VLD_ERR_ACLEN))
            {
                // LOG(3, "*E* VLD_ERR_ACLEN\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_GT64))
            {
                //LOG(3, "*E* VLD_ERR_GT64\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_ACSR))
            {
                // LOG(3, "*E* VLD_ERR_ACSR\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_DCRR))
            {
                //LOG(3, "*E* VLD_ERR_RSTMKR\n");
            }
            if (u4Value & (UINT32)(1 << VLD_ERR_RSTMKR))
            {
                //LOG(3, "*E* VLD_ERR_RSTMKR\n");
            }
            _fgJpgHwDetectError = TRUE;
        }
    }
#endif
#endif

    UNUSED(u2Vector);
}

static void VLDJPG_SetErrorConcealment(void)
{
#ifdef JPEG_VLD_V4
    VLD0_WRITE32(VLDREG_JEC_EN, (1 << VLD_EC_GT64_EN) | (1 << VLD_EC_RSTMAK_EN)|(1<<VLD_EC_BEC_LEN_EN));
#else
#ifdef JPEG_VLD_V3
    VLD0_WRITE32(VLDREG_JEC_EN, (1 << VLD_EC_GT64_EN) | (1 << VLD_EC_RSTMAK_EN));
#endif
#endif
}

static void VLDJPG_SetErrorDetection(void)
{

#ifdef JPEG_VLD_V4
    x_os_isr_fct pfnOldIsr;
    UINT32 u4Value;
    static BOOL fgIRQ = FALSE;

    if (!fgIRQ)
    {
//        VERIFY(x_reg_isr((UINT16)VECTOR_JPGDEC, VLDJPG_HwIsr, &pfnOldIsr) == OSR_OK);
        fgIRQ = TRUE;
    }
    u4Value = VLD0_READ32(VLDREG_JPEG_SWITCH);
    VLD0_WRITE32(VLDREG_JPEG_SWITCH, u4Value | (1 << VLD_INTMASK));
#endif
}

static INT32 VLDJPG_DetectError(void)
{
    if (_fgJpgHwDetectError)
    {
        _fgJpgHwDetectError = FALSE;
        return -(INT32)E_JPG_ERR_FATAL;
    }

    return (INT32)E_HWJPG_OK;
}
#endif

#if 1//jpg_hw_if.c

/* picture hw parameters */
static INT32 VLDJPG_ValidateGlobData		(JPEGHANDLE hInstance)
{
    M_JPEG_INTERFACE_START();
    UINT32 fgFlagsCheck =
        (UINT32)(D_GLOB_PROGRESSIVE | D_GLOB_NCOMP | D_GLOB_BLK_W | D_GLOB_BLK_H |
                 D_GLOB_PIC_W | D_GLOB_PIC_H | D_GLOB_RESTART);
    //D_GLOB_QTBL is not a must according to KB Lee
    //UINT8 u1Comp;
    UINT8 u1Block;
    UINT32 u4Count;

    M_JPEG_HANDLE_VALIDATE();

    /* progressive, check nz-buffer */
    if (INSTANCE->fgProgressive)
    {
        fgFlagsCheck |= 0;
    }
    else
    {
        fgFlagsCheck |= D_GLOB_DCHUFF | D_GLOB_ACHUFF | D_GLOB_QTBL;
    }

    if ((fgFlagsCheck & INSTANCE->fgGlobSet) != fgFlagsCheck)
    {

        Printf("Glob validation failed : flags_set 0x%08x flags_check 0x%08x\n",
               INSTANCE->fgGlobSet, fgFlagsCheck);

        INSTANCE->fgVldJpgDataReady = FALSE;
        return -(INT32)E_HWJPG_ERR_GLOB_NOT_READY;
    }

    /*
    calculate max block width and height
    since there are only 3 hardware component
    the comparisons are hardcoded
    */
    // width
    INSTANCE->u1MaxBlkWidth		= MAX(INSTANCE->au1CompBlkWidth[0],
                                   INSTANCE->au1CompBlkWidth[1]);
    INSTANCE->u1MaxBlkWidth		= MAX(INSTANCE->u1MaxBlkWidth,
                                   INSTANCE->au1CompBlkWidth[2]);
    // height
    INSTANCE->u1MaxBlkHeight	= MAX(INSTANCE->au1CompBlkHeight[0],
                                   INSTANCE->au1CompBlkHeight[1]);
    INSTANCE->u1MaxBlkHeight	= MAX(INSTANCE->u1MaxBlkHeight,
                                   INSTANCE->au1CompBlkHeight[2]);

    /* calculate mcu per row */
    INSTANCE->u4McuPerRow		= (INSTANCE->u4PicWidth + ((INSTANCE->u1MaxBlkWidth * D_DCTSIZE) - 1)) /
                             (INSTANCE->u1MaxBlkWidth * D_DCTSIZE);

    /* calculate absolute "block in which component" */
    /* this will be used via a abs-component to hw-component map */
    u1Block = 0;
    for (u4Count = 0; u4Count <
            ((UINT32)INSTANCE->au1CompBlkWidth[0] *
             (UINT32)INSTANCE->au1CompBlkHeight[0]); u4Count++)
    {
        if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
        {
            u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
        }
        INSTANCE->au1MemberComp[u1Block++] = 0;
    }
    for (u4Count = 0; u4Count <
            ((UINT32)INSTANCE->au1CompBlkWidth[1] *
             (UINT32)INSTANCE->au1CompBlkHeight[1]); u4Count++)
    {
        if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
        {
            u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
        }
        INSTANCE->au1MemberComp[u1Block++] = 1;
    }
    for (u4Count = 0; u4Count <
            ((UINT32)INSTANCE->au1CompBlkWidth[2] *
             (UINT32)INSTANCE->au1CompBlkHeight[2]); u4Count++)
    {
        if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
        {
            u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
        }
        INSTANCE->au1MemberComp[u1Block++] = 2;
    }
    for (;u1Block < D_MAX_JPEG_HW_BLOCK; u1Block++)
    {
        /* set rest blocks to a member of component that does not exists */
        INSTANCE->au1MemberComp[u1Block] = 3;
    }


    /*
    validate all global data
    */
    if ((INSTANCE->u1MaxBlkWidth < 1) || (INSTANCE-> u1MaxBlkWidth > 2))
    {
        return -(INT32)E_HWJPG_ERR_BLOCK;
    }

    if ((INSTANCE->u4PicWidth > 65535) || (INSTANCE->u4PicHeight > 65535))
    {
        return -(INT32)E_HWJPG_ERR_SIZE;
    }

    /*
    commit all data to hardware
    1. progressive flag
    2. huffman table (dc/ac look ahead, maxcode)
    3. membership
    */

    // not support progressive
    VERIFY(VLDJPG_Switch2Baseline() == (INT32)E_HWJPG_OK);

    // done!
    INSTANCE->fgVldJpgDataReady = TRUE;

    M_JPEG_INTERFACE_END();
}
/* scan hw parameters */
static INT32 VLDJPG_SetScanCompInScan		(JPEGHANDLE hInstance, UINT8 u1CompInScan, const UINT8 *au1CompNoList)
{
    M_JPEG_INTERFACE_START();
    UINT8 u1Count;
    M_JPEG_HANDLE_VALIDATE();

    ASSERT(au1CompNoList != NULL);

    if (u1CompInScan <= INSTANCE->u1NumComps)
    {
        UINT8 u1FirstCompInScan = D_MAX_JPEG_HW_COMP;
        INSTANCE->u1NumCompsInCurrScan	= u1CompInScan;
        for (u1Count = 0; u1Count < (D_MAX_JPEG_HW_COMP + 1); u1Count++)
        {
            /* INSTANCE->afgCompInScan[D_MAX_JPEG_HW_COMP] == FALSE all the time */
            INSTANCE->afgCompInScan[u1Count] = FALSE;
        }
        for (u1Count = 0; u1Count < u1CompInScan; u1Count++)
        {
            INSTANCE->afgCompInScan[au1CompNoList[u1Count]] = TRUE;
            if (au1CompNoList[u1Count] < u1FirstCompInScan)
            {
                u1FirstCompInScan = au1CompNoList[u1Count];
            }
        }
        INSTANCE->u1FirstCompInScan		= u1FirstCompInScan;
        INSTANCE->fgScanSet				|= D_SCAN_NCOMP;
        INSTANCE->fgScanSet				|= D_SCAN_COMPIN;
        /*
        set member of component
        hardcoded due to only 3 components
        */
        INSTANCE->u1NumBlocks =
            ((INSTANCE->afgCompInScan[0]) ?
             (INSTANCE->au1CompBlkWidth[0] * INSTANCE->au1CompBlkHeight[0]) :
             0) +
            ((INSTANCE->afgCompInScan[1]) ?
             (INSTANCE->au1CompBlkWidth[1] * INSTANCE->au1CompBlkHeight[1]) :
             0) +
            ((INSTANCE->afgCompInScan[2]) ?
             (INSTANCE->au1CompBlkWidth[2] * INSTANCE->au1CompBlkHeight[2]) :
             0);

    }
    else
    {
        i4Ret = -(INT32)E_HWJPG_ERR_COMP_RANGE;
    }
    M_JPEG_INTERFACE_END();
}

static INT32 VLDJPG_ValidateScanData		(JPEGHANDLE hInstance)
{
    M_JPEG_INTERFACE_START();
    UINT32 fgFlagsCheck =
        (UINT32)(D_SCAN_DATASTART | D_SCAN_NCOMP | D_SCAN_COMPIN | D_SCAN_COMPQ |
                 D_SCAN_BLKDC | D_SCAN_BLKAC | D_SCAN_ALAH | D_SCAN_SESS | D_SCAN_LAST);
    UINT8 u1Comp;
    UINT8 u1Block;
    UINT8 u1Count;
    UINT32 u4MembershipList;
    UINT8 u1QtblAlloc = 0;
    const UINT8 _au1BitCount[] =
    {
        0,1,				// 0 1
        1,2,				// 10 11
        1,2,2,3,			// 100 101 110 111
        1,2,2,3,2,3,3,4		// 1000 1001 1010 1011 1100 1101 1110 1111
    };
    //UINT8 u1QtblChange = 0;
    UINT32 u4DcNeedList = 0;
    UINT32 u4AcNeedList = 0;
    UINT32 u4DcHuffList, u4AcHuffList;

    M_JPEG_HANDLE_VALIDATE();

    if (INSTANCE->fgProgressive)
    {
        fgFlagsCheck |= D_SCAN_NZBUFF | D_SCAN_ROWSTART | D_SCAN_ROWEND;
    }

    if ((fgFlagsCheck & INSTANCE->fgScanSet) != fgFlagsCheck)
    {
        INSTANCE->fgScanReady = FALSE;
        return -(INT32)E_HWJPG_ERR_SCAN_NOT_READY;
    }

    if (INSTANCE->pu1DataStart == NULL)
    {
        return -(INT32)E_HWJPG_ERR_NULL_SCAN;
    }


    /* huffman table */

#ifdef JPEG_VLD_V4
    VLDJPG_SetMaxcodeSequence(0x0);
#endif

    for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
    {
        VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arDcHuffTable[u1Count].ai4Maxcode) ==
               (INT32)E_HWJPG_OK);
    }
    for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
    {
        VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arDcHuffTable[u1Count].ai4Valoffset) ==
               (INT32)E_HWJPG_OK);
    }
    for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
    {
        VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arAcHuffTable[u1Count].ai4Maxcode) ==
               (INT32)E_HWJPG_OK);
    }
    for (u1Count = 0; u1Count < D_MAX_JPEG_HUFF_TBL; u1Count++)
    {
        VERIFY(VLDJPG_LoadMaxcode(INSTANCE->arAcHuffTable[u1Count].ai4Valoffset) ==
               (INT32)E_HWJPG_OK);
    }
#ifdef JPEG_VLD_V3
    VERIFY(VLDJPG_SetHuffTblVal(INSTANCE->pu1HwDcHuffTbl, TRUE) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_SetHuffTblVal(INSTANCE->pu1HwAcHuffTbl, FALSE) == (INT32)E_HWJPG_OK);
#endif
    // member ship
    // dc ac table number list
    u4MembershipList = 0;
    u4DcHuffList = 0;
    u4AcHuffList = 0;
    u1Block = 0;
    /*
    	following block use :
    		u1Count as absolute blocks in jpeg file
    		u1Block as hw blocks in mt5351
    */
    for (u1Count = 0; u1Count < D_MAX_JPEG_HW_BLOCK; u1Count++)
    {
        if (INSTANCE->afgCompInScan[INSTANCE->au1MemberComp[u1Count]])
        {
            u4MembershipList	|=
                (((UINT32)INSTANCE->au1MemberComp[u1Count] & 3) - (UINT32)INSTANCE->u1FirstCompInScan) <<
                (u1Block * 2);
            u4DcHuffList		|=
                ((UINT32)INSTANCE->au1BlkDc[u1Count] & 3) <<
                (u1Block * 2);
            u4AcHuffList		|=
                ((UINT32)INSTANCE->au1BlkAc[u1Count] & 3) <<
                (u1Block * 2);

            u1Block++;
        }
        else
        {
            ////
        }

    }

    VERIFY(VLDJPG_SetMemberShip(u4MembershipList) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_SetDcTableList(u4DcHuffList) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_SetAcTableList(u4AcHuffList) == (INT32)E_HWJPG_OK);

    /*
    commit hw registers
    1. q table - do smart load
    2. dc needed and ac needed and block count in scan
    */

    // calculate how many q table should load
    u1QtblAlloc = 0;
    for (u1Comp = 0; u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++)
    {
        if (INSTANCE->afgCompInScan[u1Comp])
        {
            u1QtblAlloc |= D_BIT(INSTANCE->au1CompQ[u1Comp]);
        }
    }
    // if table > D_MAX_JPEG_HW_QTBL ...
    // maybe some table should merge by application (fujifilm's case)
    // but in driver, just return error
    if (u1QtblAlloc > (UINT8)15)
    {
        u1QtblAlloc = (UINT8)15;
    }
    if (_au1BitCount[u1QtblAlloc] > D_MAX_JPEG_HW_QTBL)
    {
        //return -(INT32)E_HWJPG_ERR_QTBL_NUM;
        u1QtblAlloc = 3;
    }

    if (_au1BitCount[u1QtblAlloc] == 2)
    {
        //u1QtblChange = 3;
    }
    else if (_au1BitCount[u1QtblAlloc] == 1)
    {
        //u1QtblChange = 1;
        VLDJPG_SetUserDefQMatrix();
    }
    else
    {
        //u1QtblChange = 0;
        VLDJPG_SetUserDefQMatrix();
    }

    // load q table(s) u1QtblAlloc indicates
    for (u1Count = 0; u1Count < D_MAX_JPEG_QTBL; u1Count++)
    {
        if (u1QtblAlloc & D_BIT(u1Count))
        {
            // this table has to be changed
            if (u1QtblAlloc/*u1QtblChange*/ & D_BIT(D_QTBL_HW1))	// hardcoded due to only 2 hw entry
            {
                // load u1Count q table into hw entry 0
                u1QtblAlloc &= (~(UINT8)D_BIT(u1Count));		// bit remove
                VERIFY(VLDJPG_LoadTblQ(D_QTBL_HW1, INSTANCE->apu1Qtbl[u1Count]) == (INT32)E_HWJPG_OK);
                INSTANCE->afgQtblLoad[D_QTBL_HW1] = TRUE;
                INSTANCE->au1QtblLoadIndex[D_QTBL_HW1] = u1Count;
                //u1QtblChange &= (~(UINT8)D_BIT(D_QTBL_HW1));
            }
            else if (u1QtblAlloc/*u1QtblChange*/ & (UINT8)D_BIT(D_QTBL_HW2))	// second entry
            {
                // load u1Count q table into hw entry 0
                u1QtblAlloc &= (~(UINT8)D_BIT(u1Count));		// bit remove
                VERIFY(VLDJPG_LoadTblQ(D_QTBL_HW2, INSTANCE->apu1Qtbl[u1Count]) == (INT32)E_HWJPG_OK);
                INSTANCE->afgQtblLoad[D_QTBL_HW2] = TRUE;
                INSTANCE->au1QtblLoadIndex[D_QTBL_HW2] = u1Count;
                //u1QtblChange &= (~(UINT8)D_BIT(D_QTBL_HW2));
            }
            else
            {
                // impossible, or else above lines are wrong
                ASSERT(FALSE);
            }
        }
    }

    u1QtblAlloc = 0;
    u1Count = 0;
    for (u1Comp = 0; u1Comp < D_MAX_JPEG_HW_COMP; u1Comp++)
    {
        if (INSTANCE->afgCompInScan[u1Comp])
        {
            u1QtblAlloc |= ((INSTANCE->au1CompQ[u1Comp]) ? 1 : 0) << (u1Count * 2);
            u1Count++;
        }
    }

    VERIFY(VLDJPG_SetQuanTableList((UINT32)u1QtblAlloc) == (INT32)E_HWJPG_OK);

    /* pitch per row */
    if (INSTANCE->u1NumComps > (UINT8)3)
    {
        INSTANCE->u1NumComps = (UINT8)3;
    }
    for (u1Comp = 0; u1Comp < INSTANCE->u1NumComps; u1Comp++)
    {
        INSTANCE->u4CompPitchRow[u1Comp] = 0;
    }
    u1Count = 0;
    for (u1Comp = 0; u1Comp < INSTANCE->u1NumComps; u1Comp++)
    {
        if (INSTANCE->afgCompInScan[u1Comp])
        {
            INSTANCE->u4CompPitchRow[u1Count] =
                INSTANCE->u4McuPerRow *
                (UINT32)INSTANCE->au1CompBlkWidth[u1Comp] *
                D_DCTSIZE;
            if (INSTANCE->u4CompPitchRow[u1Count] & D_JPEG_DRAM_ALIGN_MASK)
            {
                // expand pitch to fit into dram aligned line
                INSTANCE->u4CompPitchRow[u1Count] &= (~D_JPEG_DRAM_ALIGN_MASK);
                INSTANCE->u4CompPitchRow[u1Count] += D_JPEG_DRAM_ALIGN_SIZE;
            }
            u1Count++;
        }
    }

    VERIFY(VLDJPG_SetCompIdctPitch(INSTANCE->u4CompPitchRow) == (INT32)E_HWJPG_OK);

    if (INSTANCE->fgProgressive)
    {
        INT32 i32Idx;
        UINT32 au4CompPitchRow[D_MAX_JPEG_HW_COMP];
        for (i32Idx = 0; i32Idx < D_MAX_JPEG_HW_COMP; i32Idx++)
        {
            au4CompPitchRow[i32Idx] = INSTANCE->u4CompPitchRow[i32Idx] * 2;
        }
        VERIFY(VLDJPG_SetCompCoefPitch(au4CompPitchRow) == (INT32)E_HWJPG_OK);
    }
    else
    {
        VERIFY(VLDJPG_SetCompCoefPitch(INSTANCE->u4CompPitchRow) == (INT32)E_HWJPG_OK);
    }

    // calculate dc/ac needed
    // set them to all false first,
    // them set the block back if the component it belong is in this scan
    for (u1Block = 0; u1Block < D_MAX_JPEG_HW_BLOCK; u1Block++)
    {
        INSTANCE->au1DcNeed[u1Block] = 0;
        INSTANCE->au1AcNeed[u1Block] = 0;
    }
    u1Block = 0;
    for (u1Comp = 0; u1Comp < INSTANCE->u1NumComps; u1Comp++)
    {
        if (INSTANCE->afgCompInScan[u1Comp])
        {
            for (u1Count = 0; u1Count < (INSTANCE->au1CompBlkWidth[u1Comp] *
                                         INSTANCE->au1CompBlkHeight[u1Comp]); u1Count++)
            {
                if (u1Block >= (UINT8)D_MAX_JPEG_HW_BLOCK)
                {
                    u1Block = (UINT8)(D_MAX_JPEG_HW_BLOCK - 1);
                }
                INSTANCE->au1DcNeed[u1Block] = 1;
                INSTANCE->au1AcNeed[u1Block] = 1;
                u1Block++;
            }
        }
        else
        {
            // skip this component
            /*
            u1Block += INSTANCE->au1CompBlkWidth[u1Comp] * INSTANCE->au1CompBlkWidth[u1Comp];
            */
        }
    }
    // calculate needed list
    for (u1Block = 0; u1Block < D_MAX_JPEG_HW_BLOCK; u1Block++)
    {
        if (INSTANCE->au1DcNeed[u1Block])
        {
            u4DcNeedList |= D_BIT(u1Block);
        }
        if (INSTANCE->au1AcNeed[u1Block])
        {
            u4AcNeedList |= D_BIT(u1Block);
        }
    }
    // commit to hw
    VERIFY(VLDJPG_SetAcNeededList(u4AcNeedList) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_SetDcNeededList(u4DcNeedList) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_SetBlockCount(INSTANCE->u1NumBlocks) == (INT32)E_HWJPG_OK);	// this should be place in glob section

    // set al ah
    // set se ss
    // set last scan
    VERIFY(VLDJPG_SetProgressiveAhAl(INSTANCE->u1AlAh) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_SetProgressiveSeSs(INSTANCE->u1Se, INSTANCE->u1Ss) == (INT32)E_HWJPG_OK);

    VERIFY(VLDJPG_SetProgressiveLastScan(TRUE) == (INT32)E_HWJPG_OK);

    // set none zero history buffer
    VERIFY(VLDJPG_SetProgressiveNoneZeroHistory(INSTANCE->pu1NonZeroBuffer) == (INT32)E_HWJPG_OK);

    if (INSTANCE->fgRingBuffer)
    {
        //VERIFY(VLDJPG_SetRingBuffer(INSTANCE->pu1DataStart, INSTANCE->u4RingBufferSize) == (INT32)E_HWJPG_OK);
        VERIFY(VLDJPG_SetRingBuffer(INSTANCE->pu1RingBufStart, INSTANCE->u4RingBufferSize) == (INT32)E_HWJPG_OK);
    }
    else
    {
        VLDJPG_SetLinearBuffer();
    }

    VERIFY(VLDJPG_SetBarrelShifter(INSTANCE->pu1DataStart) == (INT32)E_HWJPG_OK);
    VERIFY(VLDJPG_InitScan() == (INT32)E_HWJPG_OK);

    // done!
    INSTANCE->fgScanReady	= TRUE;

    M_JPEG_INTERFACE_END();
}


static INT32 VLDJPG_ValidateMcuRowData5391(JPEGHANDLE hInstance,
        UINT32 u4MCUsPerRow, UINT32 u4TotalRows)
{
    M_JPEG_INTERFACE_START();

    UINT32 fgCheckFlags = (UINT32)D_ROW_OUTBUF;
    M_JPEG_HANDLE_VALIDATE();
    if (INSTANCE->fgProgressive)
    {
        fgCheckFlags |= D_ROW_CURR;
    }

    if ((fgCheckFlags & INSTANCE->fgRowSet) != fgCheckFlags)
    {
        INSTANCE->fgMcuRowReady = FALSE;
        return -(INT32)E_HWJPG_ERR_ROW_NOT_READY;
    }

    /*
    1. if this is first mcu in an scan, set first-time flag else clear it
    2. set read write pointer (dram word align - 32 bit base)
    */

    VLDJPG_SetCompReadAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                               (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);
    VLDJPG_SetCompWriteAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                                (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);

#ifdef JPEG_VLD_V4
    if (INSTANCE->u4RowMcuCount==0)
    {
        // output bank 0
        VLDJPG_SetOutputBank0BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                                         (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);

        if (VLDJPG_GetDecMode5391() != E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT)
        {
            // output bank 1
            VLDJPG_SetOutputBank1BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer1[0],
                                             (UINT32)INSTANCE->apu1OutputBuffer1[1], (UINT32)INSTANCE->apu1OutputBuffer1[2]);
        }
    }
    else
    {
        if (INSTANCE->u4RowMcuCount%2==1)
        {
            // output bank 0
            VLDJPG_SetOutputBank0BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                                             (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);
        }
        else
        {
            // output bank 1
            VLDJPG_SetOutputBank1BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer1[0],
                                             (UINT32)INSTANCE->apu1OutputBuffer1[1], (UINT32)INSTANCE->apu1OutputBuffer1[2]);
        }
    }
#else
    // output bank 0
    VLDJPG_SetOutputBank0BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer[0],
                                     (UINT32)INSTANCE->apu1OutputBuffer[1], (UINT32)INSTANCE->apu1OutputBuffer[2]);
    // output bank 1
    VLDJPG_SetOutputBank1BufAddr5391((UINT32)INSTANCE->apu1OutputBuffer1[0],
                                     (UINT32)INSTANCE->apu1OutputBuffer1[1], (UINT32)INSTANCE->apu1OutputBuffer1[2]);
#endif
    VLDJPG_SetMCUWidth5391(
        (UINT8)	INSTANCE->u1NumComps,
        (UINT32)INSTANCE->au1CompBlkWidth[0] * D_DCTSIZE,
        (UINT32)INSTANCE->au1CompBlkWidth[1] * D_DCTSIZE,
        (UINT32)INSTANCE->au1CompBlkWidth[2] * D_DCTSIZE);


    VLDJPG_SetPicSize5391(u4MCUsPerRow, u4TotalRows);

    // Restart Interval Processing.
    if (INSTANCE->u4RestartInterval > 0)
    {
        VLDJPG_SetRestartInterval5391(INSTANCE->u4RestartInterval);
    }

    if ((INSTANCE->u4RestartInterval != 0) &&
            (INSTANCE->u4CurrRestartInterval == 0))
    {
        VERIFY(VLDJPG_Restart() == (INT32)E_HWJPG_OK);
        VERIFY(VLDJPG_AdvanceRestartMarker(NULL) == (INT32)E_HWJPG_OK);
        INSTANCE->u4CurrRestartInterval = INSTANCE->u4RestartInterval;
    }


    // done !
    INSTANCE->fgMcuRowReady = TRUE;
    M_JPEG_INTERFACE_END();
}

static INT32 VLDJPG_ValidateMiscData		(JPEGHANDLE hInstance)
{
    M_JPEG_INTERFACE_START();
    UINT32 fgCheckFlags = (UINT32)(D_MISC_ADDR_X | D_MISC_ADDR_Y);
    M_JPEG_HANDLE_VALIDATE();
    if ((fgCheckFlags & INSTANCE->fgMiscSet) != fgCheckFlags)
    {
        INSTANCE->fgMiscDataReady = FALSE;
        return -(INT32)E_HWJPG_ERR_MISC_NOT_READY;
    }

    VERIFY(VLDJPG_SetBlockAddrList(INSTANCE->au4BlockAddrX,
                                   INSTANCE->au4BlockAddrY) == (INT32)E_HWJPG_OK);

    // done !
    INSTANCE->fgMiscDataReady = TRUE;
    M_JPEG_INTERFACE_END();
}
static INT32 VLDJPG_DecodeOnePic(JPEGHANDLE hInstance)
{
    VLDJPG_DecodePic();
    UNUSED(hInstance);

    return (INT32)E_HWJPG_OK;
}
static INT32 VLDJPG_CheckDecodingError(JPEGHANDLE hInstance)
{
    UNUSED(hInstance);
    return VLDJPG_DetectError();
}
#endif

#if 1//jpg_jfif.c

/* private interfaces */
static INT32 _jfifAdvanceBytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Bytes);
/* advance read pointer n bytes */
static INT32 _jfifCheckInstance		(const JPEG_JFIF_HEADER_T *prInstance);
/* check instance */
static INT32 _jfifParse				(JPEG_JFIF_HEADER_T *prInstance);
/* parse all marker in jpeg file */
static INT32 _jfifIsMarker			(const JPEG_JFIF_HEADER_T *prInstance);
/* check if current read pointer points to an valid marker */
static INT32 _jfifPeekMarker		(JPEG_JFIF_HEADER_T *prInstance, UINT16 *pu2Marker);
/* get next marker (read pointer stops on this marker) */
static INT32 _jfifSkipMarker		(JPEG_JFIF_HEADER_T *prInstance);
/* skip current marker */
static INT32 _jfifDispatchMarker	(JPEG_JFIF_HEADER_T *prInstance, UINT16 u2Marker);
/* dispatch this marker to proper handler routine */
static INT32 _jfifProcSOI			(JPEG_JFIF_HEADER_T *prInstance);
/* process start of image */
static INT32 _jfifProcSOF			(JPEG_JFIF_HEADER_T *prInstance);
/* process start of frame */
static INT32 _jfifProcSOS			(JPEG_JFIF_HEADER_T *prInstance);
/* process start of scan */
static INT32 _jfifProcDHT			(JPEG_JFIF_HEADER_T *prInstance);
/* process de-huffman table */
static INT32 _jfifProcDQT			(JPEG_JFIF_HEADER_T *prInstance);
/* process de-quantization table */
static INT32 _jfifProcDRI			(JPEG_JFIF_HEADER_T *prInstance);
/* process restart interval marker */
static INT32 _jfifProcAPP0			(JPEG_JFIF_HEADER_T *prInstance);
/* process standard APP0 "JFIF" marker (and "JFXX" extension) */
static INT32 _jfifProcAPP1			(JPEG_JFIF_HEADER_T *prInstance);
/* process standard APP1 "Exif" marker */
static INT32 _jfifProcAPP14			(JPEG_JFIF_HEADER_T *prInstance);
/* process APP14 */
static INT32 _jfifProcAPPother		(JPEG_JFIF_HEADER_T *prInstance);
/* process all non-supported APPn format */
static INT32 _jfifProcEOI			(JPEG_JFIF_HEADER_T *prInstance);
/* process end of image marker */

//----------------------------------------------------------

/* private interfaces */

static INT32 _jfifAdvanceBytes		(JPEG_JFIF_HEADER_T *prInstance, UINT32 u4Bytes)
{

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (((UINT32)&prInstance->pu1CurrRead[u4Bytes]) >=
            ((UINT32)&prInstance->pu1JpegImage[prInstance->u4ImageByteSize]))
    {
        if (prInstance->fgEOF)
        {
            return -(INT32)E_JPG_ERR_PASS_END;
        }
        else                                    //not EOF, need more data for parsing
        {
            UINT32 u4Remind;
            UINT32 u4Used;

            u4Remind = (!prInstance->u4RingRP) ? (UINT32)0 : \
                       (UINT32)(prInstance->pu1RingBufStart + prInstance->u4RingBufSize - prInstance->pu1CurrRead);
            u4Used = (UINT32)prInstance->pu1CurrRead - (UINT32)prInstance->pu1RingBufStart + u4Remind;
            prInstance->u4RingRP = u4Used;
            prInstance->u4RingData -= u4Used;
            prInstance->pu1CurrRead = (prInstance->pu1CurrRead + u4Bytes) - prInstance->u4RingBufSize;
        }
    }
    else
    {
        prInstance->pu1CurrRead += u4Bytes;
    }

    return (INT32)E_JPG_OK;
}

/* instance check */
static INT32 _jfifCheckInstance		(const JPEG_JFIF_HEADER_T *prInstance)
{
    if (prInstance == NULL)
    {
        // null instance
        return -(INT32)E_JPG_ERR_PARAM;
    }

    if ((prInstance->rParent.eJpegClass		!= E_JC_JFIF_HEADER) ||
            (prInstance->rParent.zObjectSize	!= sizeof(JPEG_JFIF_HEADER_T)))
    {
        // not an jfif header instance
        return -(INT32)E_JPG_ERR_PARAM;
    }

    if (!prInstance->fgInited)
    {
        // didnt init
        return -(INT32)E_JPG_ERR_NOT_INITED;
    }

    if (prInstance->fgFatalError)
    {
        // an fatal error happened
        return -(INT32)E_JPG_ERR_FATAL;
    }

    return (INT32)E_JPG_OK;
}

/* parse header markers in jpeg file */
static INT32 _jfifParse				(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2JfifMarker;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (prInstance->fgParsed)
    {
        // already parsed
        return -(INT32)E_JPG_ERR_ALREADY;
    }

    // parse until EOI
    for (;;)
    {
        i4Ret = _jfifPeekMarker(prInstance, &u2JfifMarker);
        if ((INT32)E_JPG_OK == i4Ret)
        {
            // marker found
            i4Ret = _jfifDispatchMarker(prInstance, u2JfifMarker);

            if (((UINT16)M_EOI == u2JfifMarker) ||
                    ((UINT16)M_SOS == u2JfifMarker))
            {
                // stop condition reached - EOI
                if (//((prInstance->u4NumApp0 != 0) || (prInstance->rAPP1.fgAPP1)) &&
                    //(prInstance->rDQT.fgDQT) &&
                    /*(prInstance->rDHT.fgDcTblLoaded) &&
                    (prInstance->rDHT.fgAcTblLoaded || (prInstance->rSOF.eJpegFormat != E_JPG_BASELINE)) &&*/
                    (prInstance->rSOF.fgSOF) &&
                    (prInstance->fgSOI) &&
                    (prInstance->u4NumScan > 0) &&
                    (!prInstance->fgFatalError))
                {
                    prInstance->fgParsed = TRUE;
                }
                else
                {
                    prInstance->fgParsed = FALSE;
                }
                break;
            }

            if ((INT32)E_JPG_OK != i4Ret)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return i4Ret;
}


/* check if current read pointer points to an valid marker */
static INT32 _jfifIsMarker			(const JPEG_JFIF_HEADER_T *prInstance)
{
    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if ((UINT32)prInstance->pu1CurrRead >= ((UINT32)prInstance->pu1JpegImage + prInstance->u4ImageByteSize))
    {
        return -(INT32)E_JPG_ERR_PASS_END;
    }

    if (*prInstance->pu1CurrRead == 0xff)
    {
        return (INT32)E_JPG_OK;
    }

    return -(INT32)E_JPG_ERR_NOT_A_MARKER;
}

/* get next marker (read pointer stops on this marker) */
static INT32 _jfifPeekMarker		(JPEG_JFIF_HEADER_T *prInstance, UINT16 *pu2Marker)
{
    INT32 i4Ret;
    UINT16 u2Marker;

    ASSERT(pu2Marker);
    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    while ((i4Ret = _jfifIsMarker(prInstance)) == -(INT32)E_JPG_ERR_NOT_A_MARKER)
    {
        ADVANCE_BYTES(1);
        /*
        i4Ret = _jfifAdvanceBytes(prIntsance, 1);
        if ((INT32)E_JPG_OK != i4Ret)
        {
        	break;
        }
        */
    }

    if ((INT32)E_JPG_OK != i4Ret)
    {
        return i4Ret;
    }

    u2Marker = (JPEG_READ_BYTE(prInstance, 0) << 8) + JPEG_READ_BYTE(prInstance, 1);

    // exception handling
    switch (u2Marker)
    {
        // we can process some special case here
    default:
        break;
    }
    *pu2Marker = u2Marker;

    return (INT32)E_JPG_OK;
}

/* skip current marker */
static INT32 _jfifSkipMarker		(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2Length;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (_jfifIsMarker(prInstance))
    {
        // although this is not a marker
        // skip marker should return without error
        // due to its function : 'skip marker'
        return (INT32)E_JPG_OK;
    }

    // this is a marker, advance 2 bytes first
    ADVANCE_BYTES(2);

    u2Length = JPEG_READ_2BYTES(prInstance, 0);
    ADVANCE_BYTES(u2Length);

    return (INT32)E_JPG_OK;
}

/* dispatch this marker to proper handler routine */
static INT32 _jfifDispatchMarker	(JPEG_JFIF_HEADER_T *prInstance, UINT16 u2Marker)
{
    INT32 i4Ret;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    switch (u2Marker)
    {
    case M_SOI		:		// start of image
        i4Ret = _jfifProcSOI(prInstance);
        break;
    case M_SOF0		:		// baseline
    case M_SOF1		:		// extended sequential huffman
    case M_SOF2		:		// progressive huffman
    case M_SOF3		:		// lossless huffman
    case M_SOF5		:		// differential sequential huffman
    case M_SOF6		:		// differential progressive huffman
    case M_SOF7		:		// differential lossless huffman
    case M_JPG		:		// reserved jpeg extension
    case M_SOF9		:		// extended sequential arithmetic
    case M_SOF10	:		// progressive arithmetic
    case M_SOF11	:		// lossless arithmetic
    case M_SOF13	:		// differential sequential arithmetic
    case M_SOF14	:		// differential progressive arithmetic
    case M_SOF15	:		// differential lossless arithmetic
        i4Ret = _jfifProcSOF(prInstance);
        if ((INT32)E_JPG_OK != i4Ret)
        {
            //prInstance->rSOF.eJpegFormat	= E_JPG_UNSUPPORT_FORMAT;
            prInstance->fgFatalError		= TRUE;
        }
        break;
    case M_SOS		:		// start of scan
        i4Ret = _jfifProcSOS(prInstance);
        break;
    case M_EOI		:		// end of image
        i4Ret = _jfifProcEOI(prInstance);
        break;
    case M_DHT		:		// de-huffman table
        i4Ret = _jfifProcDHT(prInstance);
        break;
    case M_DQT		:		// de-quantization table
        i4Ret = _jfifProcDQT(prInstance);
        break;
    case M_DRI		:		// restart interval marker
        i4Ret = _jfifProcDRI(prInstance);
        break;
    case M_APP0		:
        i4Ret = _jfifProcAPP0(prInstance);
        break;
    case M_APP1		:
        i4Ret = _jfifProcAPP1(prInstance);
        break;
    case M_APP14	:
        i4Ret = _jfifProcAPP14(prInstance);
        break;
    case M_APP2		:		// NIKON ICC_PROFILE, EXIF FPXR (FlashPix)
    case M_APP3		:
    case M_APP4		:
    case M_APP5		:
    case M_APP6		:
    case M_APP7		:
    case M_APP8		:
    case M_APP9		:
    case M_APP10	:
    case M_APP11	:
    case M_APP12	:
    case M_APP13	:
    case M_APP15	:
        // unsupport marker, just skip it
        i4Ret = _jfifProcAPPother(prInstance);
        // already skiped in _jfifProcAPPother()
        // i4Ret = _jfifSkipMarker(prInstance);
        break;
    default			:
        // unknown marker, just skip it
        // due to unknown, we can not trust the length
        // hence advance through it
        ADVANCE_BYTES(1);
        i4Ret = _jfifSkipMarker(prInstance);
        break;
    }

    return i4Ret;
}

/* process start of image */
static INT32 _jfifProcSOI			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret; /* for ADVANCE_BYTES() error return */

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (prInstance->fgSOI)
    {
        return -(INT32)E_JPG_ERR_MULTI_SOI;
    }

    prInstance->fgSOI = TRUE;
    ADVANCE_BYTES(2);

    return (INT32)E_JPG_OK;
}

/* process start of frame */
static INT32 _jfifProcSOF			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2Marker;
    UINT16 u2Length;
    UINT8 u1CompIndex;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (prInstance->rSOF.fgSOF)
    {
        return -(INT32)E_JPG_ERR_MULTI_SOF;
    }


    u2Marker = JPEG_READ_2BYTES(prInstance, 0);	// 0,1
    u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
    ADVANCE_BYTES(2);	// skip marker length (does not count)

    /*
     * do baseline or progressive check here, but due to we implement baseline decoder only,
     * hence these code was marked out
     */

    switch (u2Marker)
    {
    case M_SOF0		:
        prInstance->rSOF.eJpegFormat = E_JPG_BASELINE;
        break;
    case M_SOF1		:
        prInstance->rSOF.eJpegFormat = E_JPG_EX_SEQ_HUFFMAN;
        break;
    case M_SOF2		:
        prInstance->rSOF.eJpegFormat = E_JPG_PROGRESSIVE_HUFFMAN;
        break;
        //return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF3		:
        prInstance->rSOF.eJpegFormat = E_JPG_EX_SEQ_ARITHMETIC;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF5		:
        prInstance->rSOF.eJpegFormat = E_JPG_PROGRESSIVE_ARITHMETIC;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF6		:
        prInstance->rSOF.eJpegFormat = E_JPG_LOSSLESS_HUFFMAN;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF7		:
        prInstance->rSOF.eJpegFormat = E_JPG_DIFFERENTIAL_SEQ_HUFFMAN;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF9		:
        prInstance->rSOF.eJpegFormat = E_JPG_DIFF_PROG_HUFFMAN;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF10	:
        prInstance->rSOF.eJpegFormat = E_JPG_DIFF_LLESS_HUFFMAN;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF11	:
        prInstance->rSOF.eJpegFormat = E_JPG_RESERVED_FOR_EXTENSIONS;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF13	:
        prInstance->rSOF.eJpegFormat = E_JPG_LOSSLESS_ARITHMETIC;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF14	:
        prInstance->rSOF.eJpegFormat = E_JPG_DIFF_SEQ_ARITHMETIC;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    case M_SOF15	:
        prInstance->rSOF.eJpegFormat = E_JPG_DIFF_PROG_ARITHMETIC;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    default			:
        prInstance->rSOF.eJpegFormat = E_JPG_UNSUPPORT_FORMAT;
        return -(INT32)E_JPG_ERR_UNSUPPORT_SOF;
    }

    // SOF0 & SOF1 & SOF2 parsing
    if (u2Length < 8)
    {
        return -(INT32)E_JPG_ERR_INVALID_SOF;
    }

    prInstance->rSOF.u1DataPrecision	= JPEG_READ_BYTE	(prInstance, 2);	// 2
    prInstance->rSOF.u2ImageHeight		= JPEG_READ_2BYTES	(prInstance, 3);	// 3,4
    prInstance->rSOF.u2ImageWidth		= JPEG_READ_2BYTES	(prInstance, 5);	// 5,6
    prInstance->rSOF.u1NumComponents	= JPEG_READ_BYTE	(prInstance, 7);	// 7

    ADVANCE_BYTES(8);
    u2Length -= 8;

    //code sync
    if ((prInstance->rSOF.u1NumComponents	<  1)||(prInstance->rSOF.u1NumComponents	>  3))
    {
        return (INT32)E_JPG_ERR_INVALID_COMPONENT;
    }

    // start of frame validation test
    if ((prInstance->rSOF.u2ImageHeight		== 0)	||
            (prInstance->rSOF.u2ImageWidth		== 0)	||
            (u2Length != ((UINT16)(prInstance->rSOF.u1NumComponents * 3))))
    {
        return -(INT32)E_JPG_ERR_INVALID_SOF;
    }

    for (u1CompIndex = 0; u1CompIndex < prInstance->rSOF.u1NumComponents; u1CompIndex++)
    {
        UINT8 u1CompId, u1Factor, u1QuantTblNo;
        u1CompId		= JPEG_READ_BYTE(prInstance, 0);
        u1Factor		= JPEG_READ_BYTE(prInstance, 1);
        u1QuantTblNo	= JPEG_READ_BYTE(prInstance, 2);

        prInstance->rSOF.arSofComp[u1CompIndex].u1ComponentId	= u1CompId;
        prInstance->rSOF.au1MapId2Index[u1CompId] = u1CompIndex;
        prInstance->rSOF.arSofComp[u1CompIndex].u1HSampFactor	= u1Factor >> 4;
        prInstance->rSOF.arSofComp[u1CompIndex].u1VSampFactor	= u1Factor & 0xf;
        prInstance->rSOF.arSofComp[u1CompIndex].u1QuantTblNo	= u1QuantTblNo;

        ADVANCE_BYTES(3);
    }

    prInstance->rSOF.fgSOF = TRUE;
    return (INT32)E_JPG_OK;
}

/* process start of scan */
static INT32 _jfifProcSOS			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2Length;
    UINT8 u1CompInScan;
    UINT8 u1CompIndex;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (!prInstance->rSOF.fgSOF)
    {
        return -(INT32)E_JPG_ERR_SOS_BEFORE_SOF;
    }

    u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
    u1CompInScan = JPEG_READ_BYTE(prInstance, 4);	// 4
    ADVANCE_BYTES(5);

    // sos validation
    if ((u2Length < (UINT16)((u1CompInScan * 2) + 6)) ||
            (u1CompInScan == 0) ||
            (u1CompInScan > D_MAX_JPEG_COMPONENT))
    {
        return -(INT32)E_JPG_ERR_INVALID_SOS;
    }

    // store components in scan
    prInstance->arSOS[prInstance->u4CurrScan].u1CompInScan = u1CompInScan;

    u2Length -= 3;	// skip length(2 bytes), comp in scan(1 byte)

    /*
    sos structure :
    	2 bytes - SOS marker
    	2 bytes - length
    	1 byte  - num comp in scan
    	n * 3 bytes - comp data
    	1 byte  - ss
    	1 byte  - se
    	1 byte  - ah al
    */
    if (prInstance->u4CurrScan != 0)
    {
        x_memcpy(
            &prInstance->arSOS[prInstance->u4CurrScan + 1],
            &prInstance->arSOS[prInstance->u4CurrScan],
            sizeof(JPEG_JFIF_SOS_T));
    }

    prInstance->arSOS[prInstance->u4CurrScan].u4ScanPass = prInstance->u4CurrScan;

    // read component id and table no
    for (u1CompIndex = 0; u1CompIndex < u1CompInScan; u1CompIndex++)
    {
        UINT8 u1CompId;
        UINT8 u1TableId;
        UINT8 u1MapedIndex;
        u1CompId = JPEG_READ_BYTE(prInstance, 0);
        u1TableId = JPEG_READ_BYTE(prInstance, 1);
        ADVANCE_BYTES(2);
        prInstance->arSOS[prInstance->u4CurrScan].au1CompId[u1CompIndex] =
            prInstance->rSOF.au1MapId2Index[u1CompId];

        u1MapedIndex = prInstance->rSOF.au1MapId2Index[u1CompId];
        if (u1MapedIndex >= (UINT8)4)
        {
            return -(INT32)E_JPG_ERR_INVALID_SOS;
        }
        /* following data use maped index */
        prInstance->arSOS[prInstance->u4CurrScan].au1DcId[u1MapedIndex] = u1TableId >> 4;
        prInstance->arSOS[prInstance->u4CurrScan].au1AcId[u1MapedIndex] = u1TableId & 0xf;
        prInstance->au4CompLastScan[u1MapedIndex] = prInstance->u4CurrScan;
    }

    prInstance->arSOS[prInstance->u4CurrScan].u1Ss		= JPEG_READ_BYTE(prInstance, 0);
    prInstance->arSOS[prInstance->u4CurrScan].u1Se		= JPEG_READ_BYTE(prInstance, 1);
    prInstance->arSOS[prInstance->u4CurrScan].u1AhAl	= JPEG_READ_BYTE(prInstance, 2);
    ADVANCE_BYTES(3);

    prInstance->arSOS[prInstance->u4CurrScan].pu1ScanDataStart = prInstance->pu1CurrRead;
    prInstance->u4CurrScan++;
    prInstance->u4NumScan++;
    return (INT32)E_JPG_OK;
}

/* process de-huffman table */
static INT32 _jfifProcDHT			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret, i;
    UINT16 u2Length;
    UINT8 au1Bits[17];
    UINT8 au1HuffVal[256];

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    for (i=0;i<sizeof(au1HuffVal); i++)
    {
        au1HuffVal[i]=0;
    }
    u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
    ADVANCE_BYTES(4);

    if (u2Length < (2 + 17))
    {
        return -(INT32)E_JPG_ERR_INVALID_DHT;
    }

    u2Length -= 2;

    while (u2Length > 16)
    {
        UINT32 u4Bits;
        UINT32 u4Count;
        UINT32 u4HuffIndex;
        BOOL fgIsAc;
        UINT8 u1TblIndex;

        x_memset(au1Bits, 0, 17);
        x_memset(au1HuffVal, 0, 256);

        u1TblIndex = JPEG_READ_BYTE(prInstance, 0);
        fgIsAc = (BOOL) (u1TblIndex >> 4);
        //u1TblIndex &= 0xf;
        u1TblIndex &= 0x3;	// at most 4 table each, for dc and ac
        u4Count = 0;

        for (u4Bits = 1; u4Bits < 17; u4Bits++)
        {
            au1Bits[u4Bits] = JPEG_READ_BYTE(prInstance, u4Bits);
            u4Count += au1Bits[u4Bits];
        }
        ADVANCE_BYTES(17);
        u2Length -= 17;

        /*
        if ((INT32)E_JPG_OK != i4Ret)
        {
        	return i4Ret;
        }
        */

        if (u4Count > 256)
        {
            //bogus dht
            u4Count = 256;
        }

        if ((UINT32)u2Length < u4Count)
        {
            //bogus again
            u4Count = u2Length;
        }

        // fill huff value
        for (u4HuffIndex = 0; u4HuffIndex < u4Count; u4HuffIndex++)
        {
            au1HuffVal[u4HuffIndex] = JPEG_READ_BYTE(prInstance, u4HuffIndex);
        }

        u2Length -= u4Count;
        ADVANCE_BYTES(u4Count);

        //Printf("DHT : %s table index %d\n", fgIsAc ? "AC" : "DC", u1TblIndex);

        if (fgIsAc)
        {
            if ((prInstance->rDHT.fgAcTblLoaded & (1 << u1TblIndex)) == 0)
            {
                prInstance->rDHT.u4NumAcTbl++;
                prInstance->rDHT.fgAcTblLoaded |= 1 << u1TblIndex;
            }
            x_memcpy(prInstance->rDHT.arAcTbl[u1TblIndex].au1HuffVal, au1HuffVal, u4Count);
            x_memcpy(prInstance->rDHT.arAcTbl[u1TblIndex].au1Bits, au1Bits, 17);
        }
        else
        {
            if ((prInstance->rDHT.fgDcTblLoaded & (1 << u1TblIndex)) == 0)
            {
                prInstance->rDHT.u4NumDcTbl++;
                prInstance->rDHT.fgDcTblLoaded |= 1 << u1TblIndex;
            }
            x_memcpy(prInstance->rDHT.arDcTbl[u1TblIndex].au1HuffVal, au1HuffVal, u4Count);
            x_memcpy(prInstance->rDHT.arDcTbl[u1TblIndex].au1Bits, au1Bits, 17);
        }
    }

    return (INT32)E_JPG_OK;
}

/* process de-quantization table */
static INT32 _jfifProcDQT			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2Length;
    INT32 i4Ntbl = 0;
    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
    ADVANCE_BYTES(4);

    if (u2Length < (2 + 1 + 64))
    {
        return -(INT32)E_JPG_ERR_INVALID_DHT;
    }

    u2Length -= 2;

    while (u2Length > 64)
    {
        UINT8 u1Prec;
        UINT8 u1Qtbl;

        u1Prec = JPEG_READ_BYTE(prInstance, 0);
        u1Qtbl = u1Prec & 0x3;	// table index 0 .. 3
        u1Prec >>= 4;

        if (u1Prec && (u2Length < (128 + 1)))
        {
            return -(INT32)E_JPG_ERR_INVALID_DHT;
        }
        x_memcpy(prInstance->rDQT.aau1Qtbl[u1Qtbl],
                 prInstance->pu1CurrRead + 1,
                 u1Prec ? (SIZE_T)128: (SIZE_T)64);
        prInstance->rDQT.afgPrec[u1Qtbl] = (BOOL)u1Prec;

        u2Length -= u1Prec ? (128 + 1) : (64 + 1);
        ADVANCE_BYTES(u1Prec ? (UINT32)(128 + 1) : (UINT32)(64 + 1));
        i4Ntbl++;
    }
    prInstance->rDQT.u1NumQ += i4Ntbl;
    prInstance->rDQT.fgDQT = TRUE;
    return (INT32)E_JPG_OK;
}

/* process restart interval marker */
static INT32 _jfifProcDRI			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2Length;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    u2Length = JPEG_READ_2BYTES(prInstance, 2); // 2,3
    ADVANCE_BYTES(4);

    if (u2Length < 4)
    {
        return -(INT32)E_JPG_ERR_INVALID_DRI;
    }

    prInstance->u2RestartInterval = JPEG_READ_2BYTES(prInstance, 0);

    ADVANCE_BYTES(2);

    return (INT32)E_JPG_OK;

}

/* process standard APP0 "JFIF" marker (and "JFXX" extension) */
static INT32 _jfifProcAPP0(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    INT32 i4Index;
    UINT16 u2Length;
    UINT8 au1Identifier[5];

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    u2Length = JPEG_READ_2BYTES(prInstance, 2);	//2, 3
    ADVANCE_BYTES(4);	// skip marker and length

    if (u2Length ==4)
    {
        //special handling
        ADVANCE_BYTES(2);	// skip marker and length
        goto APP0_ERROR_HANDLING;
    }

    if (u2Length < 8)
    {
        return -(INT32)E_JPG_ERR_INVALID_APP0;
    }
    u2Length -= 2;

    /* identifier */
    for (i4Index = 0; i4Index < 5; i4Index++)
    {
        au1Identifier[i4Index] = JPEG_READ_BYTE(prInstance, i4Index);
    }

    ADVANCE_BYTES(5);	// skip identifier bytes
    u2Length -= 5;

    if (0 == x_memcmp(au1Identifier, "JFIF\0", 5))
    {
        // jfif
        if (u2Length < 9)
        {
            return -(INT32)E_JPG_ERR_INVALID_APP0;
        }
        prInstance->arAPP0[E_APP0_JFIF].eApp0Type = E_APP0_JFIF;
        /* version */
        prInstance->arAPP0[E_APP0_JFIF].u2Version = JPEG_READ_2BYTES(prInstance, 0);
        ADVANCE_BYTES(2);
        u2Length -= 2;

        /* units */
        prInstance->arAPP0[E_APP0_JFIF].u1Units = JPEG_READ_BYTE(prInstance, 0);
        ADVANCE_BYTES(1);
        u2Length -= 1;

        /* x & y deisity */
        prInstance->arAPP0[E_APP0_JFIF].u2DensityX = JPEG_READ_2BYTES(prInstance, 0);
        prInstance->arAPP0[E_APP0_JFIF].u2DensityY = JPEG_READ_2BYTES(prInstance, 2);
        ADVANCE_BYTES(4);
        u2Length -= 4;

        /* thumbnail w & h */
        prInstance->arAPP0[E_APP0_JFIF].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
        prInstance->arAPP0[E_APP0_JFIF].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
        ADVANCE_BYTES(2);
        u2Length -= 2;

        /* basic thumbnail */
        if ((prInstance->arAPP0[E_APP0_JFIF].u1ThumbW != 0) &&
                (prInstance->arAPP0[E_APP0_JFIF].u1ThumbH != 0) &&
                (u2Length == (UINT16)((UINT16)prInstance->arAPP0[E_APP0_JFIF].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFIF].u1ThumbH * 3)))
        {
            prInstance->arAPP0[E_APP0_JFIF].fgHasThumbnail = TRUE;
            prInstance->arAPP0[E_APP0_JFIF].pu1RGBdata = prInstance->pu1CurrRead;
        }

        prInstance->arAPP0[E_APP0_JFIF].fgAPP0 = TRUE;
    }
    else if (0 == x_memcmp(au1Identifier, "JFXX\0", 5))
    {
        // jfxx
        if (u2Length < 1)
        {
            return -(INT32)E_JPG_ERR_INVALID_APP0;
        }
        prInstance->arAPP0[E_APP0_JFXX].eApp0Type = E_APP0_JFXX;
        /* extension code */
        prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode = (ENUM_APP0_THUMBNAIL_MODE) JPEG_READ_BYTE(prInstance, 0);
        ADVANCE_BYTES(1);
        u2Length -= 1;

        if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_JPEG)
        {
            prInstance->arAPP0[E_APP0_JFXX].pu1JpegThumbnail = prInstance->pu1CurrRead;
        }
        else if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_1BYTE_PIX)
        {
            if (u2Length < 2)
            {
                return -(INT32)E_JPG_ERR_INVALID_APP0;
            }
            /* thumbnail w & h */
            prInstance->arAPP0[E_APP0_JFXX].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
            prInstance->arAPP0[E_APP0_JFXX].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
            ADVANCE_BYTES(2);
            u2Length -= 2;
            /* palettd & pixel validation */
            if ((prInstance->arAPP0[E_APP0_JFXX].u1ThumbW != 0) &&
                    (prInstance->arAPP0[E_APP0_JFXX].u1ThumbH != 0) &&
                    (u2Length == (UINT16)(((UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbH) + 768)))
            {
                UINT8 u1R, u1G, u1B;
                for (i4Index = 0; i4Index < 256; i4Index++)
                {
                    u1R = JPEG_READ_BYTE(prInstance, 0);
                    u1G = JPEG_READ_BYTE(prInstance, 1);
                    u1B = JPEG_READ_BYTE(prInstance, 2);
                    ADVANCE_BYTES(3);
                    u2Length -= 3;
                    prInstance->arAPP0[E_APP0_JFXX].au4Palette[i4Index] =
                        (UINT32)(0xff000000 |
                                 (u1R << 16) |
                                 (u1G << 8) |
                                 u1B);
                }
                prInstance->arAPP0[E_APP0_JFXX].pu1ColorData = prInstance->pu1CurrRead;
                prInstance->arAPP0[E_APP0_JFXX].fgHasThumbnail = TRUE;
            }
        }
        else if (prInstance->arAPP0[E_APP0_JFXX].eThumbnailMode == E_THUMB_3BYTE_PIX)
        {
            if (u2Length < 2)
            {
                return -(INT32)E_JPG_ERR_INVALID_APP0;
            }
            /* thumbnail w & h */
            prInstance->arAPP0[E_APP0_JFXX].u1ThumbW = JPEG_READ_BYTE(prInstance, 0);
            prInstance->arAPP0[E_APP0_JFXX].u1ThumbH = JPEG_READ_BYTE(prInstance, 1);
            ADVANCE_BYTES(2);
            u2Length -= 2;
            /* palettd & pixel validation */
            if ((prInstance->arAPP0[E_APP0_JFXX].u1ThumbW != 0) &&
                    (prInstance->arAPP0[E_APP0_JFXX].u1ThumbH != 0) &&
                    (u2Length == (UINT16)(((UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbW * (UINT16)prInstance->arAPP0[E_APP0_JFXX].u1ThumbH) * 3)))
            {
                prInstance->arAPP0[E_APP0_JFXX].pu1RGBdata = prInstance->pu1CurrRead;
                prInstance->arAPP0[E_APP0_JFXX].fgHasThumbnail = TRUE;
            }
        }
        else
        {
            // no thumbnail in jfxx ...
            // TODO : try more cases
            return -(INT32)E_JPG_ERR_INVALID_APP0;
        }

        prInstance->arAPP0[E_APP0_JFXX].fgAPP0 = TRUE;
    }
    else
    {
        // unsuppported tag
        // just skip it
        //ADVANCE_BYTES(u2Length);
    }

    ADVANCE_BYTES(u2Length);

APP0_ERROR_HANDLING:
    prInstance->u4NumApp0++;
    return (INT32)E_JPG_OK;
}

/* process standard APP1 "Exif" marker */
static INT32 _jfifProcAPP1			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    UINT16 u2Length;

    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (prInstance->rAPP1.fgAPP1)
    {
        //return -(INT32)E_JPG_ERR_MULTI_APP1;
    }

    ADVANCE_BYTES(2);
    u2Length = JPEG_READ_2BYTES(prInstance, 0);
    if (u2Length < 2)
    {
        return -(INT32)E_JPG_ERR_INVALID_APP1;
    }

    prInstance->rAPP1.pu1ExifData = prInstance->pu1CurrRead;
    prInstance->rAPP1.fgAPP1 = TRUE;
    ADVANCE_BYTES(u2Length);
    return (INT32)E_JPG_OK;
}

/* process APP14 */
static INT32 _jfifProcAPP14			(JPEG_JFIF_HEADER_T *prInstance)
{
    return _jfifProcAPP0(prInstance);
}

/* process all non-supported APPn format */
static INT32 _jfifProcAPPother		(JPEG_JFIF_HEADER_T *prInstance)
{
    return _jfifSkipMarker(prInstance);
}

/* process end of image marker */
static INT32 _jfifProcEOI			(JPEG_JFIF_HEADER_T *prInstance)
{
    INT32 i4Ret;
    ASSERT(prInstance != NULL);
    VERIFY(_jfifCheckInstance(prInstance) == (INT32)E_JPG_OK);

    if (prInstance->fgEOI)
    {
        return -(INT32)E_JPG_ERR_MULTI_EOI;
    }

    ADVANCE_BYTES(2);

    prInstance->fgEOI = TRUE;
    prInstance->pu1EOI = prInstance->pu1CurrRead;

    return (INT32)E_JPG_OK;
}

// start and parse a jpeg file image store in memory
static INT32 JFIF_New(JPEGHANDLE hInstance, void *pvJpegImage, UINT32 u4JpegSize)
{
    INT32 i4Ret;
    JPEG_JFIF_HEADER_T *prInstance;

    prInstance = (JPEG_JFIF_HEADER_T*)hInstance;

    if (prInstance == NULL)
    {
        return -(INT32)E_JPG_ERR_PARAM;
    }

    prInstance->pu1JpegImage = (UINT8*)pvJpegImage;
    prInstance->pu1CurrRead = prInstance->pu1JpegImage;
    prInstance->u4ImageByteSize = u4JpegSize;
    prInstance->fgInited = TRUE;

    i4Ret = _jfifParse(prInstance);
    if ((INT32)E_JPG_OK != i4Ret)
    {
        return i4Ret;
    }

    if (prInstance->fgParsed)
    {
        return (INT32)E_JPG_OK;
    }
    else
    {
        return -(INT32)E_JPG_ERR_PARSE_FAILED;
    }
}


#endif
#if 1
void vSetHoribilinear(void)
{
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF0 * 4, 0x00000000);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF1 * 4, 0xfef40300);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF2 * 4, 0xf9ea0500);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF3 * 4, 0xf0e30700);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF4 * 4, 0xe5de0800);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF5 * 4, 0xd6db0800);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF6 * 4, 0xc5da0800);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF7 * 4, 0xb1db0700);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF8 * 4, 0x9ddd0600);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF9 * 4, 0x87e10500);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF10 * 4, 0x70e50400);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF11 * 4, 0x5aea0300);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF12 * 4, 0x44ef0200);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF13 * 4, 0x31f40100);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF14 * 4, 0x20f80000);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF15 * 4, 0x0ffc0000);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF16 * 4, 0x44444448);
    RISCWrite(IMG_RESZ_BASE + RZ_H_COEF17 * 4, 0x44444444);
}

void vSetV4TapFilter(void)
{
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF0 * 4, 0x01f40000);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF1 * 4, 0xf8e2ffea);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF2 * 4, 0xdfd9eddd);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF3 * 4, 0xbad9ced8);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF4 * 4, 0x8edfa5db);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF5 * 4, 0x5fe976e4);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF6 * 4, 0x33f348ee);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF7 * 4, 0x0ffc1ff8);
    RISCWrite(IMG_RESZ_BASE + RZ_V_COEF8 * 4, 0x00000005);
}
void vImgReszOsd(UINT32  u4TgBase,UINT32 u4BufLen,UINT32 u4Width, UINT32 u4Height,
                 UINT32 u4OffsetX,  UINT32 u4OffsetY)
{
    UINT32 qw;
    UINT32 u4Value;
    IO_WRITE32(CKGEN_BASE, 0x344, 7);
    u4Value = RISCRead(IMG_RESZ_BASE + IMG_RESZ_START*4);
    u4Value = (u4Value & (~(0x3<<2)));
    u4Value = ((u4Value) | (0x3 << 2));
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START*4,u4Value);
    u4Value = (u4Value & (~(0x3<<2)));
    u4Value = ((u4Value) | (0x0 << 2));
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START*4,u4Value);
    qw = 2; //osd mode
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_TYPE * 4, qw);

    //alpha setting
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE * 4,
              RISCRead(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE*4) | 0x80ff0100);
    //save to the same addr ?
    RISCWrite(IMG_RESZ_BASE+RZ_MEM_IF_MODE * 4 ,0x00000800);
    //set raster in and out
    RISCWrite(IMG_RESZ_BASE+IMG_RESZ_TYPE * 4,RISCRead(IMG_RESZ_BASE + IMG_RESZ_TYPE*4) |0x00010010);
    //clear preloader
    RISCWrite(IMG_RESZ_BASE + RZ_PLD_Y_OW_ADDR_BASE * 4, 0);
    RISCWrite(IMG_RESZ_BASE + RZ_PLD_C_OW_ADDR_BASE * 4, 0);
    //set source info
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_Y_OW_ADDR_BASE1 * 4, u4TgBase >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_OFFSET_Y * 4, 0);
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE * 4,
              RISCRead(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE*4) | 0x00000607);
    //set target info
    RISCWrite(IMG_RESZ_BASE + RZ_TG_Y_OW_ADDR_BASE * 4, u4TgBase >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_TG_OFFSET_Y * 4, 0);
    qw = u4BufLen >> 4;
    ASSERT(qw < ((UINT32)1 << 12));
    qw = (qw << 12) | qw;
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_BUF_LEN * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_TG_BUF_LEN * 4, qw);

    qw = (u4Width << 16) |u4Height;
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_SIZE_Y * 4, qw); //src_width_y, src_height_y
    RISCWrite(IMG_RESZ_BASE + RZ_TG_SIZE_Y * 4, qw); //tg_width_y, tg_height_y
    // _IMGRZ_SetHoriFilterOpt
    qw = 0x40000;
    // Y
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_SCL_Y * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_SCL_CB * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_SCL_CR * 4, qw);

    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_OFT_Y * 4,  0);
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_OFT_CB * 4,  0);
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_OFT_CR * 4,  0);
    qw = 0x800;
    RISCWrite(IMG_RESZ_BASE + RZ_HEC_SCL_Y * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_HEC_SCL_CB * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_HEC_SCL_CR * 4, qw);

    // horizontal bilinear
    vSetHoribilinear();
    //_IMGRZ_SetVertFilterOpt
    qw = 0x10 << 24 | 0xC0000000;
    RISCWrite(IMG_RESZ_BASE + RZ_TG_BUF_LEN * 4 ,RISCRead(IMG_RESZ_BASE + RZ_TG_BUF_LEN*4) | qw);
    RISCWrite(IMG_RESZ_BASE+IMG_RESZ_TYPE * 4,RISCRead(IMG_RESZ_BASE + IMG_RESZ_TYPE*4) & 0xfffffeff);
    RISCWrite(IMG_RESZ_BASE + RZ_V_SCL_Y * 4,0x00000800);
    RISCWrite(IMG_RESZ_BASE + RZ_V_SCL_CB * 4, 0x00000800);
    RISCWrite(IMG_RESZ_BASE + RZ_V_SCL_CR * 4,0x00000800);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_SCL_Y * 4,  0x00040000);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_SCL_CB* 4,  0x00040000);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_OFT_Y* 4,  0);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_OFT_CB* 4,  0);

    vSetV4TapFilter();
    RISCWrite(IMG_RESZ_BASE + RZ_CSC * 4, 0x5);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_11 * 4, 0x400);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_12 * 4, 0x0);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_13 * 4, 0x57c);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_21 * 4, 0x400);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_22 * 4, 0x1EA8);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_23 * 4, 0x1D35);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_31 * 4, 0x400);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_32 * 4, 0x6EE);
    RISCWrite(IMG_RESZ_BASE + RZ_CSC_COEFF_33 * 4, 0x0);

    RISCWrite(IMG_RESZ_BASE + RZ_JPEG_INT_SWITCH * 4, 0);

    // set misc param
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START * 4, 0xc2);
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START * 4, 0xc3); // activate

    while ((RISCRead(IMG_RESZ_BASE + IMG_RESZ_DONE * 4) & 1) == 0)
    {
    }
}

void vImgResz(void * pvSclParam)
{
    UINT32 qw;
    UINT32 u4Value;
    RZ_JPG_SCL_PARAM_SET_T* prJpgParam = (RZ_JPG_SCL_PARAM_SET_T*)pvSclParam ;
    IO_WRITE32(CKGEN_BASE, 0x344, 7);

    u4Value = RISCRead(IMG_RESZ_BASE + IMG_RESZ_START*4);
    u4Value = (u4Value & (~(0x3<<2)));
    u4Value = ((u4Value) | (0x3 << 2));
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START*4,u4Value);
    u4Value = (u4Value & (~(0x3<<2)));
    u4Value = ((u4Value) | (0x0 << 2));
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START*4,u4Value);


    qw = 1; //jpeg mode
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_TYPE * 4, qw);
    //alpha setting
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE * 4,
              RISCRead(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE*4) | 0x80ff0100);
    //tempbuffer setting
    RISCWrite(IMG_RESZ_BASE + RZ_TMP_Y_OW_ADDR_BASE * 4, (prJpgParam->u4JpgTempAddr >> 4));
    if (prJpgParam->fgGray)
    {
        RISCWrite(IMG_RESZ_BASE+IMG_RESZ_TYPE * 4,RISCRead(IMG_RESZ_BASE + IMG_RESZ_TYPE*4) |0x00180000);
    }
    RISCWrite(IMG_RESZ_BASE+RZ_MEM_IF_MODE * 4 ,0x00000800);
    //set raster in and out
    RISCWrite(IMG_RESZ_BASE+IMG_RESZ_TYPE * 4,RISCRead(IMG_RESZ_BASE + IMG_RESZ_TYPE*4) |0x00010010);

    //set source info
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_Y_OW_ADDR_BASE1 * 4, prJpgParam->u4YSrcBase1 >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_Y_OW_ADDR_BASE2 * 4, prJpgParam->u4YSrcBase2 >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_CB_OW_ADDR_BASE1 * 4, prJpgParam->u4CbSrcBase1 >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_CB_OW_ADDR_BASE2 * 4, prJpgParam->u4CbSrcBase2 >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_CR_OW_ADDR_BASE1 * 4, prJpgParam->u4CrSrcBase1 >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_CR_OW_ADDR_BASE2 * 4, prJpgParam->u4CrSrcBase2 >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_OFFSET_Y * 4, 0);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_OFFSET_C * 4,0);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_OFFSET_CR* 4, 0);
    qw = prJpgParam->u4YSrcBufLen >> 4;
    ASSERT(qw < ((UINT32)1 << 12));
    qw = (qw << 12) | qw;
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_BUF_LEN * 4, qw);

    //set target info

    RISCWrite(IMG_RESZ_BASE + RZ_TG_Y_OW_ADDR_BASE * 4, prJpgParam->u4YTgBase >> 4);
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE * 4,
              RISCRead(IMG_RESZ_BASE + IMG_RESZ_OSD_MODE*4) | 0x00000600);//prJpgParam.u4YTgCM

    RISCWrite(IMG_RESZ_BASE + RZ_PLD_Y_OW_ADDR_BASE * 4, prJpgParam->u4YTgBase >> 4);
    RISCWrite(IMG_RESZ_BASE + RZ_PLD_C_OW_ADDR_BASE * 4, prJpgParam->u4YTgBase >> 4);

    qw = prJpgParam->u4YTgBufLen >> 4;
    ASSERT(qw < ((UINT32)1 << 12));
    qw = (qw << 12) | qw;
    RISCWrite(IMG_RESZ_BASE + RZ_TG_BUF_LEN * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_TG_OFFSET_Y * 4, 0);
    //set osd mode and output 444
    RISCWrite(IMG_RESZ_BASE+IMG_RESZ_TYPE * 4,RISCRead(IMG_RESZ_BASE + IMG_RESZ_TYPE*4) |0x01040000);


    qw = (prJpgParam->u4YSrcW << 16) | prJpgParam->u4YSrcH;
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_SIZE_Y * 4, qw); //src_width_y, src_height_y
    qw = (prJpgParam->u4CbSrcW<< 16) | (prJpgParam->u4CbSrcH);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_SIZE_CB * 4, qw); //src_width_c, src_height_c

    qw = (prJpgParam->u4CrSrcW<< 16) | (prJpgParam->u4CrSrcH);
    RISCWrite(IMG_RESZ_BASE + RZ_SRC_SIZE_CR* 4, qw); //src_width_c, src_height_c

    //  qw = (u4CurrPicDecWidth) << 12 | u4CurrPicDecHeight;
    qw = (prJpgParam->u4YTgW) << 16 | prJpgParam->u4YTgH; // david
    RISCWrite(IMG_RESZ_BASE + RZ_TG_SIZE_Y * 4, qw); //tg_width_y, tg_height_y

    qw = (((0x40000* prJpgParam->u4YSrcW) + (prJpgParam->u4YTgW >> 1)) / (prJpgParam->u4YTgW)) ;
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_SCL_Y * 4, qw);
    qw = (((0x40000* prJpgParam->u4CbSrcW) + (prJpgParam->u4CTgW >> 1)) / (prJpgParam->u4CTgW)) ;
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_SCL_CB * 4, qw);
    qw = (((0x40000* prJpgParam->u4CrSrcW) + (prJpgParam->u4CTgW >> 1)) / (prJpgParam->u4CTgW)) ;
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_SCL_CR * 4, qw);

    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_OFT_Y * 4,  0);
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_OFT_CB * 4,  0);
    RISCWrite(IMG_RESZ_BASE + RZ_H8TAPS_OFT_CR * 4,  0);
    qw = 0x800;
    RISCWrite(IMG_RESZ_BASE + RZ_HEC_SCL_Y * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_HEC_SCL_CB * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_HEC_SCL_CR * 4, qw);
    vSetHoribilinear();

    UINT32 u4LBufL = 0x10;
    while ((prJpgParam->u4YTgW )% (u4LBufL << 5) < 8)
    {
        u4LBufL--;
    }
    qw = ((u4LBufL & 0x1F) << 24 | 0xC0000000);
    RISCWrite(IMG_RESZ_BASE + RZ_TG_BUF_LEN * 4 ,RISCRead(IMG_RESZ_BASE + RZ_TG_BUF_LEN*4) | qw);

    RISCWrite(IMG_RESZ_BASE+IMG_RESZ_TYPE * 4,RISCRead(IMG_RESZ_BASE + IMG_RESZ_TYPE*4) & 0xfffffeff);

    UINT32 u4YFactor,u4VupY,u4YOffset;
    if (prJpgParam->u4YTgH < prJpgParam->u4YSrcH)
    {
        u4YFactor = (((0x800 * prJpgParam->u4YTgH) + (prJpgParam->u4YSrcH >> 1)) / (prJpgParam->u4YSrcH));
        if (u4YFactor&0x1)
        {
            u4YFactor++;
        }
        u4YOffset = (0x800 - u4YFactor);
        u4VupY = 0;
    }
    else
    {
        u4YFactor = (((0x800 * prJpgParam->u4YSrcH) + (prJpgParam->u4YTgH >> 1)) / (prJpgParam->u4YTgH));
        u4YOffset = 0;
        u4VupY    = 1;
    }
    RISCWrite(IMG_RESZ_BASE + RZ_V_SCL_Y * 4, ( u4YFactor & 0x7ff ) | ((u4VupY << 11) & (0x00000800)) |( (u4YOffset  << 12) & (0x007ff000)));
    if (prJpgParam->u4CTgH < prJpgParam->u4CbSrcH)
    {
        u4YFactor = (((0x800 * prJpgParam->u4CTgH) + (prJpgParam->u4CbSrcH >> 1)) / (prJpgParam->u4CbSrcH));
        if (u4YFactor&0x1)
        {
            u4YFactor++;
        }
        u4YOffset = (0x800 - u4YFactor);
        u4VupY = 0;
    }
    else
    {
        u4YFactor = (((0x800 * prJpgParam->u4CbSrcH) + (prJpgParam->u4CTgH >> 1)) / (prJpgParam->u4CTgH));
        u4YOffset = 0;
        u4VupY    = 1;
    }
    RISCWrite(IMG_RESZ_BASE + RZ_V_SCL_CB * 4, ( u4YFactor & 0x7ff ) | ((u4VupY << 11) & (0x00000800)) |( (u4YOffset  << 12) & (0x007ff000)));
    if (prJpgParam->u4CTgH < prJpgParam->u4CrSrcH)
    {
        u4YFactor = (((0x800 * prJpgParam->u4CTgH) + (prJpgParam->u4CrSrcH >> 1)) / (prJpgParam->u4CrSrcH));
        if (u4YFactor&0x1)
        {
            u4YFactor++;
        }
        u4YOffset = (0x800 - u4YFactor);
        u4VupY = 0;
    }
    else
    {
        u4YFactor = (((0x800 * prJpgParam->u4CrSrcH) + (prJpgParam->u4CTgH >> 1)) / (prJpgParam->u4CTgH));
        u4YOffset = 0;
        u4VupY    = 1;
    }

    RISCWrite(IMG_RESZ_BASE + RZ_V_SCL_CR * 4, ( u4YFactor & 0x7ff ) | ((u4VupY << 11) & (0x00000800)) |( (u4YOffset  << 12) & (0x007ff000)));
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_SCL_Y * 4,  0x00040000);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_SCL_CB* 4,  0x00040000);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_OFT_Y* 4,  0);
    RISCWrite(IMG_RESZ_BASE + RZ_V4TAPS_OFT_CB* 4,  0);
    vSetV4TapFilter();
    // set misc param
    qw =  prJpgParam->u4ColorComp << 14 |prJpgParam->u4IsFstBl << 13 | prJpgParam->u4IsLstBl << 12 //0x7;        // y, cb, cr exist
          |  prJpgParam->u4JpgVfacY << 8 | prJpgParam->u4JpgVfacCb << 4 | prJpgParam->u4JpgVfacCr;


    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_JPG_MODE * 4, qw);
    RISCWrite(IMG_RESZ_BASE + RZ_JPEG_INT_SWITCH * 4, prJpgParam->u4IsJpgRzOn);
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START * 4, 0x82);
    RISCWrite(IMG_RESZ_BASE + IMG_RESZ_START * 4, 0x83); // activate
}

void vImageWait(void)
{
    while ((RISCRead(IMG_RESZ_BASE + IMG_RESZ_DONE * 4) & 1) == 0)
    {

    }
}
#endif

#if 1
typedef enum
{
    MJPEG_CB_FINISH,
    MJPEG_CB_ERROR,
    MJPEG_CB_STOPED,
    MJPEG_CB_REQ_DATA
}MJPEG_CB_TYPE;

typedef enum
{
    MJPEG_RET_OK,
    MJPEG_RET_ERR,
    MJPEG_RET_INVALID_PARAM,
    MJPEG_RET_BUSY
}MJPEG_RET_VAL;

typedef struct
{
    UINT8* pu1Addr;
    UINT32 u4CbCrOffset;	///< [IN] address offset for Y/CbCr seperate mode
    UINT32 u4Pitch;								///< [IN] pitch
    UINT32 u4Width;								///< [IN] region width
    UINT32 u4Height;								///< [IN] region height
}MJPG_OUTPARAM_T;

typedef struct
{
    UINT32 u4DstAddr;								///< [IN] start address
    UINT32 u4CbCrOffset;							///< [IN] address offset for Y/CbCr seperate mode
    UINT32 u4Pitch;								///< [IN] pitch
    UINT32 u4Width;								///< [IN] region width
    UINT32 u4Height;	   ///< [IN] region height
    UINT32 u4FrameSize;
    UINT32 u4DataAddr;
    UINT32 u4DataReady;
}MJPG_INPARAM_T;

typedef struct
{
    UINT32 u4DataBufStart;
    UINT32 u4DataBufSize;
    UINT32 u4WorkBufStart;
    UINT32 u4WorkBufSize;
}MJPG_OPEN_INFO_T;

static const UINT8 _JpgBitsDcLuminance[17] =
    { 0, 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
static const UINT8 _JpgValDcLuminance[] =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

static const UINT8 _JpgBitsDcChrominance[17] =
    { 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };
static const UINT8 _JpgValDcChrominance[] =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

static const UINT8 _JpgBitsAcLuminance[17] =
    { 0, 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 0x7d };
static const UINT8 _JpgValAcLuminance[] =
    { 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
      0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
      0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
      0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
      0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
      0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
      0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
      0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
      0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
      0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
      0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
      0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
      0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
      0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
      0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
      0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
      0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
      0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
      0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
      0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
      0xf9, 0xfa
    };

static const UINT8 _JpgBitsAcChrominance[17] =
    { 0, 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 0x77 };

static const UINT8 _JpgValAcChrominance[] =
    { 0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
      0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
      0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
      0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
      0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
      0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
      0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
      0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
      0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
      0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
      0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
      0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
      0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
      0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
      0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
      0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
      0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
      0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
      0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
      0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
      0xf9, 0xfa
    };


typedef struct
{
    UINT32 au4CompBlock[4];
    UINT32 u4McuWidth;
    UINT32 u4McuHeight;
    UINT32 u4MaxHFactor;
    UINT32 u4MaxVFactor;
    UINT32 u4ColorBuf[6];
    UINT32 u4HufTable[2];
    UINT32 u4RowSize[4];
    UINT32 u4RzWorkBuf;
    UINT32 u4DataAddr;
}MJPEG_DEC_PARAM;

typedef struct
{
    JPEG_JFIF_HEADER_T rJpgHeader;
    JPEG_VLDJPG_DATA_T rJpgHw;
    MJPG_OUTPARAM_T *prOutParam;
    MJPG_INPARAM_T *prInParam;
    MJPEG_DEC_PARAM rDecParam;
    UINT32 u4WorkBuffer;
    UINT32 u4WorkBufSize;
    UINT32 u4DataBuffer;
    UINT32 u4DataBufSize;
}MJPEG_DECODER;

static INT32 _MJPG_FetchRegValue(MJPEG_DECODER *prDecoder)
{
    JPEG_JFIF_HEADER_T *prJpgHeader=&prDecoder->rJpgHeader;
    JPEG_VLDJPG_DATA_T *prJpgData=&prDecoder->rJpgHw;
    MJPEG_DEC_PARAM *prDecParam=&prDecoder->rDecParam;
    UINT32 u4Num=0,u4Index=0;
    INT32 i4Ret=0;
    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
    prJpgData->pu1HwDcHuffTbl=(UINT8 *)prDecParam->u4HufTable[0];
    prJpgData->pu1HwAcHuffTbl=(UINT8 *)prDecParam->u4HufTable[1];

    //Glob
    {
        JPEG_JFIF_SOF_T *prSOF=&prJpgHeader->rSOF;
        if (prSOF->eJpegFormat ==E_JPG_BASELINE)
        {
            prJpgData->fgProgressive=FALSE;
        }
        else
        {
            return -1;
        }

        prJpgData->fgGlobSet|= D_GLOB_PROGRESSIVE;

        u4Num=prSOF->u1NumComponents;
        if (u4Num>3)
        {
            return -2;
        }
        prJpgData->u1NumComps= u4Num;
        prJpgData->fgGlobSet |= D_GLOB_NCOMP;

        prDecParam->u4MaxHFactor=0;
        prDecParam->u4MaxVFactor=0;
        for (u4Index = 0;u4Index < u4Num; u4Index++)
        {
            UINT32 u4FactorH=0;
            UINT32 u4FactorV=0;
            u4FactorH = prSOF->arSofComp[u4Index].u1HSampFactor;
            u4FactorV = prSOF->arSofComp[u4Index].u1VSampFactor;
            prJpgData->au1CompBlkWidth[u4Index] = u4FactorH;
            prJpgData->fgGlobSet|= D_GLOB_BLK_W;
            prJpgData->au1CompBlkHeight[u4Index] = u4FactorV;
            prJpgData->fgGlobSet |= D_GLOB_BLK_H;
            prDecParam->u4MaxHFactor=MAX(prDecParam->u4MaxHFactor,u4FactorH);
            prDecParam->u4MaxVFactor=MAX(prDecParam->u4MaxVFactor,u4FactorV);
            prDecParam->au4CompBlock[u4Index]=u4FactorV*u4FactorH;
        }

        prJpgData->u4PicWidth	= prSOF->u2ImageWidth;
        prJpgData->fgGlobSet	|= D_GLOB_PIC_W;
        prJpgData->u4PicHeight	= prSOF->u2ImageHeight;
        prJpgData->fgGlobSet	|= D_GLOB_PIC_H;

        prDecParam->u4McuWidth = (prDecParam->u4MaxHFactor==2) ?
                                 ((prJpgData->u4PicWidth+15)>>4):((prJpgData->u4PicWidth+7)>>3);
        prDecParam->u4McuHeight= (prDecParam->u4MaxVFactor==2) ?
                                 ((prJpgData->u4PicHeight+15)>>4) :((prJpgData->u4PicHeight+7)>>3);

        for (u4Index = 0;u4Index < u4Num; u4Index++)
        {
            UINT32 u4FactorH,u4FactorV,u4CompSize;
            u4FactorH = prSOF->arSofComp[u4Index].u1HSampFactor;
            u4FactorV = prSOF->arSofComp[u4Index].u1VSampFactor;
            u4CompSize=u4FactorH*u4FactorV*64;
            prDecParam->u4RowSize[u4Index]=u4CompSize*prDecParam->u4McuWidth;
            if (((prDecParam->u4McuWidth&1)!=0)&&(u4FactorH==1))
            {
                prDecParam->u4RowSize[u4Index]+=u4CompSize;
            }
        }

        /* set huff table */
        if (prJpgHeader->rDHT.u4NumDcTbl == 0)
        {
            x_memcpy((void *)prJpgHeader->rDHT.arDcTbl[0].au1Bits,
                     (void *)_JpgBitsDcLuminance, (UINT32)sizeof(_JpgBitsDcLuminance));
            x_memcpy((void *)prJpgHeader->rDHT.arDcTbl[0].au1HuffVal,
                     (void *)_JpgValDcLuminance, (UINT32)sizeof(_JpgValDcLuminance));
            x_memcpy((void *)prJpgHeader->rDHT.arDcTbl[1].au1Bits,
                     (void *)_JpgBitsDcChrominance, (UINT32)sizeof(_JpgBitsDcChrominance));
            x_memcpy((void *)prJpgHeader->rDHT.arDcTbl[1].au1HuffVal,
                     (void *)_JpgValDcChrominance, (UINT32)sizeof(_JpgValDcChrominance));
            x_memcpy((void *)prJpgHeader->rDHT.arAcTbl[0].au1Bits,
                     (void *)_JpgBitsAcLuminance, (UINT32)sizeof(_JpgBitsAcLuminance));
            x_memcpy((void *)prJpgHeader->rDHT.arAcTbl[0].au1HuffVal,
                     (void *)_JpgValAcLuminance, (UINT32)sizeof(_JpgValAcLuminance));
            x_memcpy((void *)prJpgHeader->rDHT.arAcTbl[1].au1Bits,
                     (void *)_JpgBitsAcChrominance, (UINT32)sizeof(_JpgBitsAcChrominance));
            x_memcpy((void *)prJpgHeader->rDHT.arAcTbl[1].au1HuffVal,
                     (void *)_JpgValAcChrominance, (UINT32)sizeof(_JpgValAcChrominance));
            prJpgHeader->rDHT.fgDcTblLoaded = (UINT32)0x3;
            prJpgHeader->rDHT.fgAcTblLoaded = (UINT32)0x3;
        }

        {
            JPEG_JFIF_DHT_T *prDHT=&prJpgHeader->rDHT;
            JPEG_DHT_HUFF_TBL_T *prDHTable;
            for (u4Index = 0; u4Index < D_MAX_JPEG_HUFF_TBL>>1; u4Index++)
            {
                if ((prDHT->fgDcTblLoaded & D_BIT(u4Index))!=0)
                {
                    prDHTable=&prDHT->arDcTbl[u4Index];
                    i4Ret = VLDJPG_CalcDerivedHuffTable(prDHTable->au1Bits,prDHTable->au1HuffVal,&prJpgData->arDcHuffTable[u4Index]);
                    if ((INT32)E_HWJPG_OK != i4Ret)
                    {
                        return -3;
                    }
                    VLDJPG_TransHuffTblToHwFormat((UINT8 *)VIRTUAL(prJpgData->pu1HwDcHuffTbl+(u4Index*256)),prDHTable->au1HuffVal);
                }
                if ((prDHT->fgAcTblLoaded & D_BIT(u4Index))!=0)
                {
                    prDHTable=&prDHT->arAcTbl[u4Index];
                    i4Ret = VLDJPG_CalcDerivedHuffTable(prDHTable->au1Bits,prDHTable->au1HuffVal,&prJpgData->arAcHuffTable[u4Index]);
                    if ((INT32)E_HWJPG_OK != i4Ret)
                    {
                        return -4;
                    }
                    prJpgData->fgAcHuff |= D_BIT(u4Index);
                    VLDJPG_TransHuffTblToHwFormat((UINT8 *)VIRTUAL(prJpgData->pu1HwAcHuffTbl + (u4Index * 256)),prDHTable->au1HuffVal);
                }
            }

            prJpgData->fgGlobSet |= D_GLOB_DCHUFF;
            prJpgData->fgScanSet |= D_SCAN_DCHUFF;
            prJpgData->fgGlobSet |= D_GLOB_ACHUFF;
            prJpgData->fgScanSet |= D_SCAN_ACHUFF;
        }

        /* set q table */
        for (u4Index = 0; u4Index < D_MAX_JPEG_QTBL; u4Index++)
        {
            prJpgData->apu1Qtbl[u4Index]= &prJpgHeader->rDQT.aau1Qtbl[u4Index][0];
        }
        prJpgData->fgGlobSet |= D_GLOB_QTBL;
        prJpgData->fgScanSet |= D_SCAN_QTBL;

        prJpgData->u4RestartInterval = prJpgHeader->u2RestartInterval;
        prJpgData->u4CurrRestartInterval = prJpgHeader->u2RestartInterval;
        prJpgData->fgGlobSet	 |= D_GLOB_RESTART;

        i4Ret=VLDJPG_ValidateGlobData((JPEGHANDLE)prJpgData);
        if (i4Ret!=(INT32)E_HWJPG_OK)
        {
            return -5;
        }
    }

    prJpgData->fgRingBuffer	= TRUE;
    prJpgData->u4RingBufferSize	= prJpgHeader->u4RingBufSize;
    prJpgData->pu1RingBufStart = (prJpgHeader->pu1RingBufStart);
    prJpgData->fgLastScan =	TRUE;
    VLDJPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);

    // scan
    /* set data start */
    {
        JPEG_JFIF_SOS_T *prSOS=&prJpgHeader->arSOS[0];
        UINT32 u4Block=0;
        prJpgData->u4McuCount = 0;
        prJpgData->pu1DataStart = prSOS->pu1ScanDataStart;
        prJpgData->fgScanSet   |= D_SCAN_DATASTART;
        /* set comp in scan */
        VLDJPG_SetScanCompInScan((JPEGHANDLE)prJpgData,prSOS->u1CompInScan,(UINT8*)prSOS->au1CompId);
        /* associate q tbl to comp, dc/ac id to block */
        u4Num=prJpgHeader->arSOS[0].u1CompInScan;
        if (u4Num>3)
        {
            return -6;
        }

        for (u4Index = 0;u4Index<u4Num;u4Index++)
        {
            UINT32 u4Count=0;
            prJpgData->au1CompQ[u4Index] = prJpgHeader->rSOF.arSofComp[u4Index].u1QuantTblNo;
            for (u4Count = 0; u4Count < prDecParam->au4CompBlock[u4Index];u4Count++,u4Block++)
            {
                prJpgData->au1BlkDc[u4Block]	= prSOS->au1DcId[u4Index];
                prJpgData->au1BlkAc[u4Block]	= prSOS->au1AcId[u4Index];
            }
        }

        prJpgData->fgScanSet |= D_SCAN_COMPQ;
        prJpgData->fgScanSet |= D_SCAN_BLKDC;
        prJpgData->fgScanSet |= D_SCAN_BLKAC;

        /* set al ah se ss*/
        prJpgData->u1AlAh=prSOS->u1AhAl;
        prJpgData->fgScanSet|= D_SCAN_ALAH;
        prJpgData->u1Se	  = prSOS->u1Se;
        prJpgData->u1Ss	  = prSOS->u1Ss;
        prJpgData->fgScanSet |= D_SCAN_SESS;
#ifdef JPEG_VLD_V3
        VLDJPG_UseNewEngine();
        VLDJPG_SetErrorConcealment();
        VLDJPG_SetErrorDetection();
#endif
        VLDJPG_SetProgressiveMode(D_JPEG_BASELINE);
        prJpgData->u4ProgMode=D_JPEG_BASELINE;


        /* set last scan flag */
        prJpgData->fgLastScan = (prJpgHeader->au4CompLastScan[0]==0)  \
                                ||(prJpgHeader->au4CompLastScan[1]==0) \
                                ||(prJpgHeader->au4CompLastScan[2]==0);
        prJpgData->fgScanSet  |= D_SCAN_LAST;

        /* mtk patent */
        prJpgData->u4McuRowStart= 0;
        prJpgData->fgScanSet|= D_SCAN_ROWSTART;
        prJpgData->u4McuRowEnd	= prDecParam->u4McuHeight-1;
        prJpgData->fgScanSet    |= D_SCAN_ROWEND;
        i4Ret=VLDJPG_ValidateScanData((JPEGHANDLE)prJpgData);
        if (i4Ret!=(INT32)E_HWJPG_OK)
        {
            return -7;
        }
        prJpgData->i4SavedEOBRUN = 0;
    }

    // Misc
    {
        UINT32 u4AddrX,u4AddrY;
        u4Num=0;
        for (u4Index=0;u4Index<D_MAX_JPEG_HW_COMP;u4Index++)
        {
            for (u4AddrY=0;u4AddrY<prJpgData->au1CompBlkHeight[u4Index];u4AddrY++)
            {
                for (u4AddrX=0;u4AddrX<prJpgData->au1CompBlkWidth[u4Index];u4AddrX++)
                {
                    if (u4Num>=D_MAX_JPEG_HW_BLOCK)
                    {
                        u4Num=D_MAX_JPEG_HW_BLOCK-1;
                    }
                    prJpgData->au4BlockAddrX[u4Num] = u4AddrX;
                    prJpgData->au4BlockAddrY[u4Num]	= u4AddrY;
                    u4Num++;
                }
            }
        }

        prJpgData->fgMiscSet |= D_MISC_ADDR_X;
        prJpgData->fgMiscSet |= D_MISC_ADDR_Y;

        i4Ret=VLDJPG_ValidateMiscData((JPEGHANDLE)prJpgData);
        if (i4Ret!=(INT32)E_HWJPG_OK)
        {
            return -8;
        }
        VLDJPG_Switch2CleanBuffer();
    }

    // buffer infor
    prJpgData->u4CurrMcuRow	= 0;
    prJpgData->fgRowSet	|= D_ROW_CURR;

    prJpgData->apu1OutputBuffer[0]	= (UINT8 *)prDecParam->u4ColorBuf[0];
    prJpgData->apu1OutputBuffer[1]	= (UINT8 *)prDecParam->u4ColorBuf[1];
    prJpgData->apu1OutputBuffer[2]	= (UINT8 *)prDecParam->u4ColorBuf[2];
    prJpgData->apu1OutputBuffer1[0] = (UINT8 *)prDecParam->u4ColorBuf[3];
    prJpgData->apu1OutputBuffer1[1] = (UINT8 *)prDecParam->u4ColorBuf[4];
    prJpgData->apu1OutputBuffer1[2] = (UINT8 *)prDecParam->u4ColorBuf[5];
    prJpgData->au4OutputBufferSize[0] = prDecParam->u4RowSize[0];
    prJpgData->au4OutputBufferSize[1] = prDecParam->u4RowSize[1];
    prJpgData->au4OutputBufferSize[2] = prDecParam->u4RowSize[2];
    prJpgData->fgRowSet	|= D_ROW_OUTBUF | D_ROW_OUTBUFSIZE;
    i4Ret=VLDJPG_ValidateMcuRowData5391((JPEGHANDLE)prJpgData,prDecParam->u4McuWidth,prDecParam->u4McuHeight);
    if (i4Ret!=(INT32)E_HWJPG_OK)
    {
        return -9;
    }
    return 0;
}

static VOID _MJPG_ResizeCfg(MJPEG_DECODER *prDecoder,RZ_JPG_SCL_PARAM_SET_T *prRzParam)
{
    MJPG_INPARAM_T *prInParam=prDecoder->prInParam;
    MJPG_OUTPARAM_T *prOutParam=prDecoder->prOutParam;
    JPEG_VLDJPG_DATA_T *prJpgData=&prDecoder->rJpgHw;
    MJPEG_DEC_PARAM *prDecParam=&prDecoder->rDecParam;

    prRzParam->u4JpgVfacY=prJpgData->au1CompBlkHeight[0]-1;
    prRzParam->u4JpgVfacCb=prJpgData->au1CompBlkHeight[1]-1;
    prRzParam->u4JpgVfacCr=prJpgData->au1CompBlkHeight[2]-1;

    if (prJpgData->u1NumComps==1)
    {
        prRzParam->fgGray=1;
        prRzParam->u4JpgVfacCb=prRzParam->u4JpgVfacY;
        prRzParam->u4JpgVfacCr=prRzParam->u4JpgVfacY;
    }
    else
    {
        prRzParam->fgGray=0;
    }

    prRzParam->u4IsJpgSameaddr = 0;
    prRzParam->u4YTgHOffset = 0;
    prRzParam->u4YTgVOffset = 0;
    prRzParam->u4YTgBase = PHYSICAL(prInParam->u4DstAddr);
    prRzParam->u4IsVdo2Osd=1;
    prRzParam->u4YTgCM = 3;// E_RZ_OSD_DIR_CM_AYCbCr8888;

    prRzParam->u4IsRsOut=1;  //static jpeg is raster out
    prRzParam->u4OutSwap=0;
    //prRzParam->u4YTgBufLen=prJpgData->au1CompBlkWidth[0]*prDecParam->u4McuWidth*8*4;
    // prRzParam->u4YTgBufLen=((prRzParam->u4YTgBufLen+15)/16)*16;

    prRzParam->u4YTgBufLen = prOutParam->u4Pitch;
    prRzParam->u4CTgBase=  prRzParam->u4YTgBase;
    prRzParam->u4CTgBufLen = prRzParam->u4YTgBufLen;

    prRzParam->u4IsRsIn = 1;        // block|raster based input
    prRzParam->u4YSrcBase1 =  prDecParam->u4ColorBuf[0]; // y1
    prRzParam->u4YSrcBase2 =  prDecParam->u4ColorBuf[3]; // y2
    prRzParam->u4YSrcBufLen = prJpgData->u4CompPitchRow[0];
    prRzParam->u4CSrcBufLen = prRzParam->u4YSrcBufLen;
    prRzParam->u4YSrcHOffset = 0;
    prRzParam->u4YSrcVOffset = 0;
    prRzParam->u4YSrcW=prJpgData->u4PicWidth;
    prRzParam->u4YSrcH = (prRzParam->u4JpgVfacY == prRzParam->u4JpgVfacCb) ? prJpgData->u4PicHeight :(prJpgData->u4PicHeight&0xfffffffe);

    prRzParam->u4CbSrcBase1 = prDecParam->u4ColorBuf[1];    // cb1
    prRzParam->u4CbSrcBase2 = prDecParam->u4ColorBuf[4];    // cb2
    prRzParam->u4CbSrcHOffset = 0;
    prRzParam->u4CbSrcVOffset = 0;
    prRzParam->u4CbSrcW = (prJpgData->au1CompBlkWidth[0] == prJpgData->au1CompBlkWidth[1])? prRzParam->u4YSrcW:(prRzParam->u4YSrcW / 2);
    prRzParam->u4CbSrcH = (prJpgData->au1CompBlkHeight[0]== prJpgData->au1CompBlkHeight[1])? prRzParam->u4YSrcH :(prRzParam->u4YSrcH / 2);

    prRzParam->u4CrSrcBase1 = prDecParam->u4ColorBuf[2];    // cr1
    prRzParam->u4CrSrcBase2 = prDecParam->u4ColorBuf[5];    // cr2
    prRzParam->u4CrSrcHOffset = 0;
    prRzParam->u4CrSrcVOffset = 0;
    prRzParam->u4CrSrcW = (prJpgData->au1CompBlkWidth[0] == prJpgData->au1CompBlkWidth[2])?  prRzParam->u4YSrcW :(prRzParam->u4YSrcW / 2);
    prRzParam->u4CrSrcH = (prJpgData->au1CompBlkHeight[0]== prJpgData->au1CompBlkHeight[2])? prRzParam->u4YSrcH: (prRzParam->u4YSrcH / 2);
    prRzParam->fgPreload = 0;

    if (prRzParam->fgGray)
    {
        prRzParam->u4CbSrcW = prRzParam->u4CrSrcW = prRzParam->u4YSrcW;
        prRzParam->u4CrSrcH = prRzParam->u4CbSrcH = prRzParam->u4YSrcH;
    }

    prRzParam->u4ColorComp = 0x7;
    //prRzParam->u4YTgW=prRzParam->u4YSrcW;
    //prRzParam->u4YTgH = prRzParam->u4YSrcH;
    prRzParam->u4YTgW=prOutParam->u4Width;
    prRzParam->u4YTgH = prOutParam->u4Height;
    prRzParam->u4OutMode = 2;//E_RZ_VDO_OUTMD_444;

    prRzParam->u4CTgH = prRzParam->u4YTgH;
    prRzParam->u4CTgW = prRzParam->u4YTgW;
    prRzParam->u4IsFstBl = 1;
    prRzParam->u4IsLstBl =prDecParam->u4McuHeight==1 ? 1 : 0;
    prRzParam->u4IsJpgRzOn = 1;
    prRzParam->u4JpgTempAddr = prDecParam->u4RzWorkBuf;
    prOutParam->pu1Addr= (UINT8*)prRzParam->u4YTgBase;
    prOutParam->u4CbCrOffset=prRzParam->u4CTgBase-prRzParam->u4YTgBase;
    prOutParam->u4Height=prRzParam->u4YTgH;
    prOutParam->u4Width=prRzParam->u4YTgW;
    prOutParam->u4Pitch=prRzParam->u4YTgBufLen;
    return;
}

static INT32 _MJPG_DecodeFrame(MJPEG_DECODER *prDecoder)
{
    JPEG_JFIF_HEADER_T *prJpegHdr=&prDecoder->rJpgHeader;
    RZ_JPG_SCL_PARAM_SET_T rJpgParam;
    INT32 i4Ret=0;
    prDecoder->rJpgHw.rParent.eJpegClass=E_JC_VLD_DECODER;
    prDecoder->rJpgHw.rParent.zObjectSize=sizeof(JPEG_VLDJPG_DATA_T);
    VLDJPG_New((JPEGHANDLE)(&prDecoder->rJpgHw));

    VLDJPG_SetRiscCtrlWP(0xFFFFFFFF);
    if (JFIF_New((JPEGHANDLE)(&prDecoder->rJpgHeader),(void *)prDecoder->rDecParam.u4DataAddr,
                 prDecoder->prInParam->u4FrameSize)!=0)
    {
        return -1;
    }

    i4Ret=_MJPG_FetchRegValue(prDecoder);
    if (i4Ret!=0)
    {
        return -2;
    }

    _MJPG_ResizeCfg(prDecoder,&rJpgParam);
    vImgResz((void *)(&rJpgParam));
    VLDJPG_DecodeOnePic((JPEGHANDLE)&prDecoder->rJpgHw);
    vImageWait();
#if 0
    Printf("Decode Finish error status 0x%x checksum 0x%x CRC 0x%x Input Checksum 0x%x\n",VLD0_READ32(VLDREG_JERR_STATUS),
		VLD0_READ32(VLDREG_JPEG_WINTF_CHKSUM),VLD0_READ32(VLDREG_JPEG_WINTF_CRC) ,VLD0_READ32(VLD_JPEG_BS_CHKSUM));

    Printf("input window checksum 0x%x hf decode ck 0x%0x vld outck 0x%x  idct outck 0x%x \n ",VLD0_READ32(0x228),VLD0_READ32(0x22c),VLD0_READ32(0x230),VLD0_READ32(0x234));
    Printf("coffi ck 0x%x nonzero ck 0x%0x nonzero outck 0x%x \n ",VLD0_READ32(0x23c),VLD0_READ32(0x240),VLD0_READ32(0x244));
#endif
	if (VLDJPG_CheckDecodingError((JPEGHANDLE)&prDecoder->rJpgHw) != (INT32)E_HWJPG_OK)
    {
        i4Ret = -3;
    }
    vImgReszOsd(rJpgParam.u4YTgBase,rJpgParam.u4YTgBufLen,rJpgParam.u4YTgW,rJpgParam.u4YTgH,0,0);
    return i4Ret;
}

INT32 MJPG_DecoderOpen(HANDLE_T *hDecoder,
                       MJPG_OPEN_INFO_T *prOpenInfo)
{
    MJPEG_DECODER *prDecoder=NULL;
    MJPEG_DEC_PARAM *prDecParam;
    UINT32 u4WorkBuf;

    prDecoder = x_mem_alloc(sizeof(MJPEG_DECODER));
    if (prDecoder==NULL)
    {
        return MJPEG_RET_ERR;
    }
    x_memset(prDecoder,0,sizeof(MJPEG_DECODER));
    prDecParam=&prDecoder->rDecParam;
    u4WorkBuf=prOpenInfo->u4WorkBufStart;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4ColorBuf[0]=u4WorkBuf;
    u4WorkBuf+=MJPEG_COLOR_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4ColorBuf[3]=u4WorkBuf;

    u4WorkBuf+=MJPEG_COLOR_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4ColorBuf[1]=u4WorkBuf;
    u4WorkBuf+=MJPEG_COLOR_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4ColorBuf[4]=u4WorkBuf;

    u4WorkBuf+=MJPEG_COLOR_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4ColorBuf[2]=u4WorkBuf;
    u4WorkBuf+=MJPEG_COLOR_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4ColorBuf[5]=u4WorkBuf;

    u4WorkBuf+=MJPEG_COLOR_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4RzWorkBuf=u4WorkBuf;

    u4WorkBuf+=MJPEG_RZWORK_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4HufTable[0]=u4WorkBuf;
    u4WorkBuf+=MJPEG_HUFTABLE_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);
    prDecParam->u4HufTable[1]=u4WorkBuf;
    u4WorkBuf+=MJPEG_HUFTABLE_BUF;
    MJPEG_VALUE_ALIGN(u4WorkBuf,64);

    if (u4WorkBuf>prOpenInfo->u4WorkBufStart+prOpenInfo->u4WorkBufSize)
    {
        return -1;
    }
    prDecoder->u4WorkBuffer=u4WorkBuf;
    prDecoder->u4WorkBufSize=prOpenInfo->u4WorkBufSize-(u4WorkBuf-prOpenInfo->u4WorkBufStart);
    prDecoder->u4DataBuffer=prOpenInfo->u4DataBufStart;
    prDecoder->u4DataBufSize=prOpenInfo->u4DataBufSize;

    *hDecoder=(HANDLE_T)prDecoder;
    return MJPEG_RET_OK;
}

INT32 MJPG_DecoderDecode(HANDLE_T hDecoder,MJPG_INPARAM_T *prInParam,MJPG_OUTPARAM_T*prOutParam)
{
    MJPEG_DECODER *prDecoder=(MJPEG_DECODER*)hDecoder;
    JPEG_JFIF_HEADER_T *prJpegHdr=&prDecoder->rJpgHeader;
    INT32 i4Ret=MJPEG_RET_OK;
    if (hDecoder==NULL || prInParam==NULL || prOutParam==NULL)
    {
        return MJPEG_RET_INVALID_PARAM;
    }
    x_memset(&prDecoder->rJpgHeader,0,sizeof(JPEG_JFIF_HEADER_T));
    x_memset(&prDecoder->rJpgHw,0,sizeof(JPEG_VLDJPG_DATA_T));

    prDecoder->prInParam=prInParam;
    prDecoder->prOutParam=prOutParam;
    prJpegHdr->rParent.eJpegClass=E_JC_JFIF_HEADER;
    prJpegHdr->rParent.zObjectSize=sizeof(JPEG_JFIF_HEADER_T);
    prJpegHdr->fgRingBuffer=TRUE;
    prJpegHdr->u4RingRP = 0;
    prJpegHdr->u4RingData=prInParam->u4FrameSize;
    prJpegHdr->fgEOF=TRUE;

    prJpegHdr->pu1RingBufStart=(UINT8 *)VIRTUAL(prDecoder->u4DataBuffer);
    prJpegHdr->u4RingBufSize=prDecoder->u4DataBufSize;

    prDecoder->rDecParam.u4DataAddr=(UINT32)VIRTUAL(prInParam->u4DataAddr);
    HalFlushInvalidateDCacheMultipleLine((UINT32)prJpegHdr->pu1RingBufStart, prJpegHdr->u4RingBufSize);

    i4Ret=_MJPG_DecodeFrame(prDecoder);
    if (i4Ret!=0)
    {
        return MJPEG_RET_ERR;
    }

    return MJPEG_RET_OK;
}

INT32 MJPG_DecoderClose(HANDLE_T hDecoder)
{
    MJPEG_DECODER *prDecoder=(MJPEG_DECODER*)hDecoder;
    if (prDecoder==NULL)
    {
        return MJPEG_RET_INVALID_PARAM;
    }
    x_mem_free(prDecoder);
    VLDJPG_SetPowerOff();
    return MJPEG_RET_OK;
}
VOID  jpeg_decode(UINT32 u4Buffer,UINT32 u4size,UINT32 u4WorkBuffer,UINT32 u4Worksize,UINT32 u4DstAddr,UINT32 u4Width,UINT32 u4Height)
{
    HANDLE_T hDecoder;
    MJPG_OPEN_INFO_T rOpenInfo;
    MJPG_INPARAM_T rInputParam;
    MJPG_OUTPARAM_T rOutputParam;
    rOpenInfo.u4DataBufStart = u4Buffer;
    rOpenInfo.u4DataBufSize= u4size;
    rOpenInfo.u4WorkBufStart= u4WorkBuffer;
    rOpenInfo.u4WorkBufSize= u4Worksize;
    rInputParam.u4DataReady = 1;
    rInputParam.u4DataAddr = u4Buffer;
    rInputParam.u4FrameSize = u4size;
    rInputParam.u4DstAddr = u4DstAddr;
    rOutputParam.u4Width = u4Width;
    rOutputParam.u4Height = u4Height;
    rOutputParam.u4Pitch = ((u4Width +  0xF)&(~0xF))*4;
    MJPG_DecoderOpen(&hDecoder,&rOpenInfo);
    MJPG_DecoderDecode(hDecoder,&rInputParam,&rOutputParam);
    MJPG_DecoderClose(hDecoder);
}

void jpeg_test()
{
    UINT32 u4Addr;
    UINT32 u4Size = 200*1024;

    UINT32 u4TgtY;
    UINT32 u4WorkBuf;
    UINT32 u4WorkSize = 400*1024;
    u4Addr = (UINT32)x_mem_alloc(200*1024+64);
    if (u4Addr == 0)
    {
        return ;
    }
    u4Addr = ( (u4Addr + 63) / 64) *64;

    u4WorkBuf = (UINT32)x_mem_alloc(400*1024+64);
    if (u4WorkBuf == 0)
    {
        return ;
    }
    u4WorkBuf = ( (u4WorkBuf + 63) / 64) *64;

    u4TgtY = (UINT32)x_mem_alloc(3072*1024+64);
    if (u4TgtY == 0)
    {
        return ;
    }
    u4TgtY = ( (u4TgtY + 63) / 64) *64;

    jpeg_decode(u4Addr,u4Size,u4WorkBuf,u4WorkSize,u4TgtY,1024,768);
}
#endif

#endif


