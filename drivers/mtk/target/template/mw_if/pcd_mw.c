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
 * $IRRCSfile$ pcd_mw.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *         This header file contains implementation Middleware interface for
 *         Power Control driver. 
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_typedef.h"
#include "x_rm_dev_types.h"
#include "x_rm.h"
#include "x_pwr_ctrl.h"
#include "x_dram.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "x_assert.h"
#include "eeprom_if.h"
#include "ir_if.h"
#include "drvcust_if.h"
#include "rtc_if.h"
#include "x_gpio.h"
#include "x_debug.h"

/*-----------------------------------------------------------------------------
                    configurations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PCD_DRV_ID          0
#define PCD_COMP_NAME       DRVN_PWR_CTRL
#define DTVCFG_MW_FLAG_BITS (DTVCFG_FLAG_POWERON_BOOT | DTVCFG_FLAG_FACTORY_MODE | \
                                DTVCFG_FLAG_POWERON_LAST | DTVCFG_FLAG_USB_UPGRADE_ON | DTVCFG_FLAG_POWER_LEDOFF)

#ifndef PC_MISC_CTRL_COMPANION_CHIP_UPGRADE
#define PC_MISC_CTRL_COMPANION_CHIP_UPGRADE             (1U << 5)
#endif /* PC_MISC_CTRL_COMPANION_CHIP_UPGRADE */

/*-----------------------------------------------------------------------------
        External function.
 ----------------------------------------------------------------------------*/
extern void IRRC_DisconnectMiddleware(void);
extern void IRRC_WaitPowerKeyReboot(void);
extern UINT32 IRRC_IsPowerButtonOff(void);

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL     b_pcd_init = FALSE;
static UINT64   _u8UtcWakeup = 0;
// static UINT64   _u8UtcPowerDown = 0;
static BOOL     _fgVGAWakeup = FALSE;
static BOOL     _fgUartNormalWakeup = FALSE;
static PC_NFY_INFO_T _rPCNfyInfo[PC_WAKE_UP_REASON_UART + 1];
static UINT64  _u8GpioModeCtrlStatus    =0;
static PC_NFY_INFO_T    _rGpioMonitorNfyInfo ={NULL, NULL};
static UINT32   _u4TheInitKey = 0;
static PC_WAKE_UP_REASON_T  _rPCWakeupReason = (PC_WAKE_UP_REASON_T)0;
static UINT32 g_u4AdvSifClkDiv = 0x100;

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static void _GpioPolarityChangeNotify_Isr(INT32 i4Gpio, BOOL fgStatus)
{
    
    UINT32 u4Data;
    ASSERT ( i4Gpio >= 0 );
    
    u4Data = i4Gpio & 0x0000FFFF;
    u4Data |=  (((UINT32)(fgStatus?1:0))<<16);

/*  ASSERT( _rGpioMonitorNfyInfo.pv_tag !=NULL );   */
    ASSERT( _rGpioMonitorNfyInfo.pf_nfy != NULL );

//_rGpioMonitorNfyInfo.pf_nfy( _rGpioMonitorNfyInfo.pv_tag , PC_COND_WAKE_UP_EXPIRED, u4Data );
_rGpioMonitorNfyInfo.pf_nfy( _rGpioMonitorNfyInfo.pv_tag , PC_COND_GPIO_UPDATED, u4Data );

    Printf( "[_GpioPolarityChangeNotify_Isr] GPIO %d's polarity =%d\n", i4Gpio, fgStatus );
}

static BOOL _PCDInit(void);
// static void _PCDPowerDownHandler(UINT16 u2Dummy);
static INT32 _PCDEepromStandby(void);
static void _PCDSetEepromFlag(UINT32 u4Flags);
 
/*
static void _PCDPowerDownHandler(UINT16 u2Dummy)
{
    UNUSED(u2Dummy);
    Printf("Power down time up. send the power down key.\n");
    VERIFY(IRRX_SendMtkIr(BTN_POWER | KEY_SRC_SW)==0);
}
*/

static INT32 _PCDEepromStandby(void)
{
    UINT32 u4Size;
    INT32 i4Ret;
    DTVCFG_T rDtvCfg;

    u4Size = EEPDTV_GetSize();
    ASSERT(u4Size >= sizeof(DTVCFG_T));
    if (u4Size < sizeof(DTVCFG_T))
    {
        Printf("DTV EEPROM size is %d\n", u4Size);
    }

    i4Ret = EEPDTV_GetCfg(&rDtvCfg);
    ASSERT(i4Ret==0);
    if (i4Ret!=0)
    {
        Printf("DTV EEPROM read failure.\n");
    }

    if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) ||
        (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
    {
        // No data, Enter standby
        return 0;
    }

    return (INT32)rDtvCfg.u1Flags;
}

static UINT32 _PCDWriteDtvCfg(DTVCFG_T *prDtvCfg, UINT32 u4Flags)
{
    if ((prDtvCfg->u1Magic1 != DTVCFG_MAGIC_CHAR1) || 
        (prDtvCfg->u1Magic2 != DTVCFG_MAGIC_CHAR2))
    {
        prDtvCfg->u1Magic1 = DTVCFG_MAGIC_CHAR1;
        prDtvCfg->u1Magic2 = DTVCFG_MAGIC_CHAR2;
        prDtvCfg->u1PanelIdx = 0;
        prDtvCfg->u1Flags = (0xffU & (~DTVCFG_FLAG_POWERON_LAST));
    }

    if ((prDtvCfg->u1Flags & DTVCFG_MW_FLAG_BITS) != (u4Flags & DTVCFG_MW_FLAG_BITS))
    {
        if (u4Flags & DTVCFG_FLAG_POWERON_BOOT)
        {
            prDtvCfg->u1Flags |= DTVCFG_FLAG_POWERON_BOOT;
        }
        else
        {
            prDtvCfg->u1Flags &= ~(DTVCFG_FLAG_POWERON_BOOT);
        }

        if (u4Flags & DTVCFG_FLAG_FACTORY_MODE)
        {
            prDtvCfg->u1Flags |= DTVCFG_FLAG_FACTORY_MODE;
        }
        else
        {
            prDtvCfg->u1Flags &= ~(DTVCFG_FLAG_FACTORY_MODE);
        }

        if (u4Flags & DTVCFG_FLAG_POWER_LEDOFF)
        {
            prDtvCfg->u1Flags |= DTVCFG_FLAG_POWER_LEDOFF;
        }
        else
        {
            prDtvCfg->u1Flags &= ~(DTVCFG_FLAG_POWER_LEDOFF);
        }

        if (u4Flags & DTVCFG_FLAG_POWERON_LAST)
        {
            prDtvCfg->u1Flags |= DTVCFG_FLAG_POWERON_LAST;
        }
        else
        {
            prDtvCfg->u1Flags &= ~(DTVCFG_FLAG_POWERON_LAST);
        }

#if 0
        if (u4Flags & DTVCFG_FLAG_USB_UPGRADE_ON)
        {
            prDtvCfg->u1Flags |= DTVCFG_FLAG_USB_UPGRADE_ON;
            prDtvCfg->u4Flags |= DTVCFG_FLAG_USB_UPGRADE_ON;
        }
        else
        {
            prDtvCfg->u1Flags &= ~(DTVCFG_FLAG_USB_UPGRADE_ON);
            prDtvCfg->u4Flags &= ~(DTVCFG_FLAG_USB_UPGRADE_ON);
        }
#endif

        return 1;
    }
    return 0;
}

static void _PCDSetEepromFlag(UINT32 u4Flags)
{
    UINT32 u4Size;
    INT32 i4Ret;
    DTVCFG_T rDtvCfg;

    u4Size = EEPDTV_GetSize();
    ASSERT(u4Size >= sizeof(DTVCFG_T));
    UNUSED(u4Size);

    i4Ret = EEPDTV_GetCfg(&rDtvCfg);
    ASSERT(i4Ret==0);
    UNUSED(i4Ret);
    if (_PCDWriteDtvCfg(&rDtvCfg, u4Flags))
    {
        i4Ret = EEPDTV_SetCfg(&rDtvCfg);
        ASSERT(i4Ret==0);
    }
}

static INT32 _PCD_XferGpio(UINT32 ui4_id)
{
    UINT32 u4Val;

    if (ui4_id & PC_GPIO_TYPE_OP_CTRL)
    {
        u4Val = (ui4_id & ~(PC_GPIO_TYPE_OP_CTRL));
        if (u4Val >= TOTAL_OPCTRL_NUMBER)
        {
            ASSERT(0);
            return -1;
        }
        return (INT32)(OPCTRL(u4Val));
    }
    return (INT32)(GPIO(ui4_id));
}


/*-----------------------------------------------------------------------------
 * Name: _PCDSetAlertHandler
 *
 * Description: This API implements Middleware set functionality for Power 
 *              Control driver.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
void _PCDSetPCNfyFct(PC_NFY_INFO_T* prInfo, PC_WAKE_UP_REASON_T eReason)
{   
    ASSERT(prInfo->pf_nfy != NULL);
    _rPCNfyInfo[(UINT32)eReason].pf_nfy = prInfo->pf_nfy;    
    _rPCNfyInfo[(UINT32)eReason].pv_tag = prInfo->pv_tag;
}


/*-----------------------------------------------------------------------------
 * Name: _PCDAlertHandler
 *
 * Description: This API implements Middleware set functionality for Power 
 *              Control driver.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
void _PCDRTCNfyFct(void)
{
    UINT32 u4Data = 0;
    if(_u8UtcWakeup != 0 && _rPCNfyInfo[PC_WAKE_UP_REASON_RTC].pf_nfy != NULL)
    {
        _rPCNfyInfo[PC_WAKE_UP_REASON_RTC].pf_nfy(_rPCNfyInfo[PC_WAKE_UP_REASON_RTC].pv_tag, PC_COND_WAKE_UP_EXPIRED, u4Data);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _CheckWakeupReason
 *
 * Description: This API implements to check the key on/off at initial time.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static void _CheckWakeupReason()
{
    INT32 i, i4Val;
    UINT32 u4TheKey, u4Val;
    SGL_GPIO_T  *prSingleGpioKeyList = NULL;
    SGL_SERVO_T *prSingleServoKeyList = NULL;
    GREED_KEY_T *prGreedGpioKeyList = NULL;

    if (DRVCUST_OptQuery(ePcdWakeupTheKey, &u4TheKey) < 0)
    {
        return;
    }
    if (DRVCUST_OptQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList) < 0)
    {
        prSingleGpioKeyList = NULL;
    }
    if (DRVCUST_OptQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList) < 0)
    {
        prSingleServoKeyList = NULL;
    }
    if (DRVCUST_OptQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList) < 0)
    {
        prGreedGpioKeyList = NULL;
    }

    i = 0;
    while ((prSingleGpioKeyList) &&
           (prSingleGpioKeyList[i].u4Key != 0) &&
           (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if ((UINT32)GPIO_Input((INT32)prSingleGpioKeyList[i].u4Gpio) == prSingleGpioKeyList[i].u4Val)
        {
            _u4TheInitKey = prSingleGpioKeyList[i].u4Key;
            if (prSingleGpioKeyList[i].u4Key == u4TheKey)
            {
                _rPCWakeupReason = PC_WAKE_UP_REASON_CUSTOM_1;
                return;
            }
        }
        i++;
    }

    i = 0;
    while ((prSingleServoKeyList) &&
           (prSingleServoKeyList[i].u4Key != 0) &&
           (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
    {
        u4Val = IRRX_ReadSvadc(prSingleServoKeyList[i].u4Servo);
        if ((prSingleServoKeyList[i].u4Top > u4Val) &&
            (u4Val >= prSingleServoKeyList[i].u4Btm))
        {
            _u4TheInitKey = prSingleServoKeyList[i].u4Key;
            if (prSingleServoKeyList[i].u4Key == u4TheKey)
            {
                _rPCWakeupReason = PC_WAKE_UP_REASON_CUSTOM_1;
                return;
            }
        }
        i++;
    }

    i = 0;
    while ((prGreedGpioKeyList) &&
           (prGreedGpioKeyList[i].u4Key != 0) &&
           (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        i4Val = (INT32)prGreedGpioKeyList[i].u4Val;
        VERIFY(i4Val==GPIO_Output((INT32)prGreedGpioKeyList[i].u4GpioOut, &i4Val));
        if (i4Val == GPIO_Input((INT32)prGreedGpioKeyList[i].u4GpioIn))
        {
            _u4TheInitKey = prGreedGpioKeyList[i].u4Key;
            if (prGreedGpioKeyList[i].u4Key == u4TheKey)
            {
                _rPCWakeupReason = PC_WAKE_UP_REASON_CUSTOM_1;
                return;
            }
        }
        i++;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _PCDInit
 *
 * Description: This API initializes PCD driver and hardwre.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: TRUE    Succeed
 *          FALSE   Fail
 ----------------------------------------------------------------------------*/
static BOOL _PCDInit(void)
{
    INT32 i4Val, fgWrite;
    DTVCFG_T rDtvCfg;

    // Set System state on.
    i4Val = EEPDTV_GetCfg(&rDtvCfg);
    UNUSED(i4Val);
    ASSERT(i4Val==0);
    fgWrite = 0;

    if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) ||
        (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
    {
        // default flag value is 0.
        fgWrite = _PCDWriteDtvCfg(&rDtvCfg, 0);
    }

#ifndef ADVANCED_STANDBY_CTRL
    if (DRVCUST_OptGet(eStdbyUsePrevPowerState) &&
        (!(rDtvCfg.u1Flags & DTVCFG_FLAG_SYSTEM_STATE_ON)))
#else /* ADVANCED_STANDBY_CTRL */
    if (!(rDtvCfg.u1Flags & DTVCFG_FLAG_SYSTEM_STATE_ON))
#endif /* ADVANCED_STANDBY_CTRL */
    {
        rDtvCfg.u1Flags |= (DTVCFG_FLAG_SYSTEM_STATE_ON);
        fgWrite = 1;
    }

    if (DRVCUST_OptGet(eStdbyUsePowerBtnCount) && 
        (!(rDtvCfg.u1Flags & DTVCFG_FLAG_POWER_BTN_ON)))
    {
        rDtvCfg.u1Flags |= (DTVCFG_FLAG_POWER_BTN_ON);
        fgWrite = 1;
    }

    if (fgWrite)
    {
        i4Val = EEPDTV_SetCfg(&rDtvCfg);
        ASSERT(i4Val==0);
    }   
    
    return TRUE;    
}    

UINT32 PCDSetAdvSifClkDiv(UINT32 u4Clk)
{
    UINT32 u4OldClk;

    u4OldClk = g_u4AdvSifClkDiv;
    g_u4AdvSifClkDiv = u4Clk;
    return u4OldClk;
}

/*-----------------------------------------------------------------------------
 * Name: PCDGet
 *
 * Description: This API implements Middleware get functionality for Power 
 *              Control driver.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 PCDGet(DRV_COMP_ID_T* pt_comp_id,
             DRV_GET_TYPE_T e_get_type,
             VOID*          pv_get_info,
             SIZE_T*        pz_get_info_len)
{
    SIZE_T          t_get_info_len;
    UINT64          u8Utc;
    BOOL            fgRet;
    INT32           i4DtvFlags, i4Gpio, i4Val, u4Clk;
    PC_SIF_CH_ID_T  e_channel_id;
    DTVCFG_T rDtvCfg;

    /* Check aguments */
    if (!b_pcd_init)
    {
        return RMR_DRV_INV_GET_INFO;
    }
        
    if ((pt_comp_id == NULL) ||
        (pt_comp_id->e_type != DRVT_PWR_CTRL) ||
        (pt_comp_id->ui2_id != PCD_DRV_ID))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    /* Perform get opertion */
    switch (e_get_type)
    {
        case PC_GET_TYPE_WAKE_UP_CAP:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            *(UINT32 *)pv_get_info = (PC_WAKE_UP_CAP_IRRC | PC_WAKE_UP_CAP_FP
                                        | PC_WAKE_UP_CAP_VGA | PC_WAKE_UP_CAP_UART);
            fgRet = RTC_GetTimeDate(&u8Utc);
            if (fgRet)
            {
                *(UINT32 *)pv_get_info |= (PC_WAKE_UP_CAP_RTC);
            }
            break;

        case PC_GET_TYPE_WAKE_UP_CTRL_CAP:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }
            *(UINT32 *)pv_get_info = (PC_WAKE_UP_CAP_VGA | PC_WAKE_UP_CAP_RTC);
            break;

        case PC_GET_TYPE_WAKE_UP_SETUP:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }
            *(UINT32 *)pv_get_info = (PC_WAKE_UP_CAP_IRRC | PC_WAKE_UP_CAP_FP);
            *(UINT32 *)pv_get_info |= (_fgUartNormalWakeup) ? PC_WAKE_UP_CAP_UART : 0;
            *(UINT32 *)pv_get_info |= (_u8UtcWakeup) ? (PC_WAKE_UP_CAP_RTC) : 0;
            *(UINT32 *)pv_get_info |= (_fgVGAWakeup) ? (PC_WAKE_UP_CAP_VGA) : 0;
            break;
            
        case PC_GET_TYPE_WAKE_UP_REASON:
            t_get_info_len = sizeof(PC_WAKE_UP_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

#if 0            
            if (_rPCWakeupReason)
            {
                ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = _rPCWakeupReason;
            }
            else if (BSP_IsSystemAcOn())
            {
                ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_AC_POWER);
            }
            // XXX uP wakeup reason query.
            else if (PDWNC_READ32(REG_RO_PDWAKE_STAT)==0)
            {
                ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_IRRC);
            }
            else
            {
                UINT32 u4Val;

                // Wakeup REASON...
                u4Val = PDWNC_READ32(REG_RO_PDWAKE_STAT);
                if (u4Val & WAK_RTC)
                {
                    ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_RTC);
                }
                else if (u4Val & WAK_VGA)
                {
                    ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_VGA);
                }
                else if (u4Val & WAK_RS232)
                {
                    ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_UART);
                }
                else if (PDWNC_IsWakeupByPowerButton())
                {
                    ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_FP);
                }
                else // if (u4Val & WAK_IRRX)
                {
                    ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = (PC_WAKE_UP_REASON_IRRC);
                }
            }
#endif    

            if (_rPCWakeupReason)
            {
                ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = _rPCWakeupReason;
            }
            else
            {
                UINT32 u4WakeupReason;

                u4WakeupReason = PDWNC_ReadWakeupReason();
                if (u4WakeupReason == PDWNC_WAKE_UP_REASON_WATCHDOG)
                {
                    u4WakeupReason = PDWNC_WAKE_UP_REASON_IRRC;
                }
                ((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason = 
                    (PC_WAKE_UP_REASON_T)u4WakeupReason;
            }

            Printf("reason: 0x%08x\n", (UINT32)((PC_WAKE_UP_INFO_T *)pv_get_info)->e_reason);
            ((PC_WAKE_UP_INFO_T *)pv_get_info)->pv_data = NULL;
            break;
            
        case PC_GET_TYPE_WAKE_UP_TIME:
            t_get_info_len = sizeof(TIME_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }
            *(TIME_T *)pv_get_info = (TIME_T)_u8UtcWakeup;
            break;

        case PC_GET_TYPE_MISC_CTRL:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }
            *(UINT32 *)pv_get_info = 0;
            i4DtvFlags = _PCDEepromStandby();
            if (i4DtvFlags & DTVCFG_FLAG_POWERON_BOOT)
            {
                *(UINT32 *)pv_get_info |= PC_MISC_CTRL_NO_STANDBY;
            }
            if (i4DtvFlags & DTVCFG_FLAG_FACTORY_MODE)
            {
                *(UINT32 *)pv_get_info |= PC_MISC_CTRL_FACTORY_MODE;
            }
            if (i4DtvFlags & DTVCFG_FLAG_POWER_LEDOFF)
            {
                *(UINT32 *)pv_get_info |= PC_MISC_CTRL_POWER_LED_OFF;
            }
            if (i4DtvFlags & DTVCFG_FLAG_POWERON_LAST)
            {
                *(UINT32 *)pv_get_info |= PC_MISC_CTRL_NO_STANDBY_2;
            }
            if (i4DtvFlags & DTVCFG_FLAG_USB_UPGRADE_ON)
            {
                *(UINT32 *)pv_get_info |= PC_MISC_CTRL_FIRMWARE_UPGRADE;
            }
            break;

        case PC_GET_TYPE_MISC_STATUS:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (PDWNC_PowerDownByAcCut())
            {
                *(UINT32 *)pv_get_info = PC_MISC_STS_POWER_DOWN_BY_AC_CUT;
            }
            else
            {
                *(UINT32 *)pv_get_info = 0;
            }
            break;

        case PC_GET_TYPE_GPIO_MODE:
            t_get_info_len = sizeof(PC_GPIO_MODE_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            i4Gpio = _PCD_XferGpio(((PC_GPIO_MODE_INFO_T *)pv_get_info)->ui4_id);
            if (i4Gpio < 0)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if (GPIO_Enable(i4Gpio, NULL))
            {   // output mode.
                ((PC_GPIO_MODE_INFO_T *)pv_get_info)->e_mode = PC_GPIO_MODE_OUTPUT;
            }
            else
            {   // input mode.
                ((PC_GPIO_MODE_INFO_T *)pv_get_info)->e_mode = PC_GPIO_MODE_INPUT;
            }
            break;

        case PC_GET_TYPE_GPIO_STATUS:
            t_get_info_len = sizeof(PC_GPIO_STATUS_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            i4Gpio = _PCD_XferGpio(((PC_GPIO_STATUS_INFO_T *)pv_get_info)->ui4_id);
            if (i4Gpio < 0)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if (GPIO_Enable(i4Gpio, NULL))
            {   // output mode.
                ((PC_GPIO_STATUS_INFO_T *)pv_get_info)->e_status =
                    (GPIO_Output(i4Gpio, NULL) ? PC_GPIO_STATUS_HIGH : PC_GPIO_STATUS_LOW);
            }
            else
            {   // input mode.
                ((PC_GPIO_STATUS_INFO_T *)pv_get_info)->e_status =
                    (GPIO_Input(i4Gpio) ? PC_GPIO_STATUS_HIGH : PC_GPIO_STATUS_LOW);
            }
            break;

        case PC_GET_TYPE_SIF:
            t_get_info_len = sizeof(PC_SIF_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            i4Val = SIF_Read(0x100,
                        (UINT8)(((PC_SIF_INFO_T *)pv_get_info)->ui4_addr),
                        (UINT8)(((PC_SIF_INFO_T *)pv_get_info)->ui4_offset),
                        (UINT8 *)&(((PC_SIF_INFO_T *)pv_get_info)->ui1_data),
                        1);
            if (i4Val == 0)
            { // failed
                //return RMR_DRV_ERROR;		//this could cause RM abort...
                return RMR_DRV_INV_GET_INFO;
            }

            break;

        case PC_GET_TYPE_SIF_ADVANCED:
            t_get_info_len = sizeof(PC_SIF_INFO_ADVANCED_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            i4Val = 0;	u4Clk = g_u4AdvSifClkDiv;
            e_channel_id = ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->e_channel_id ;
            
            switch (e_channel_id)
            {
                case PC_SIF_CH_ID_EDID_BUS:
                    /* Do nothing */
                    break;
                case PC_SIF_CH_ID_SYS_BUS:
                    if( ((((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_offset_len)<=3) )  /* check sub-address length*/
                    {
                        LOG(7,"SIF_X_Read: SIF_BUS_SYSTEM_I2C ClkDiv:0x%x  ui4_addr:0x%02X  \n", u4Clk, ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_addr);
                        i4Val = SIF_X_Read(		SIF_BUS_SYSTEM_I2C,
                        				u4Clk,
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_addr),
                                                        ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_offset_len, 
                                                        ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_offset,
                                                        (UINT8 *)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->pui1_data),
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_data_len));
                        LOG(7, "Return i4Val=%d\n", i4Val);
		    }
                    break;
                case PC_SIF_CH_ID_TUNER_BUS:
                    if((((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_offset_len) == 0x00 )  /* No sub-address */
                    {
/*                        i4Val = SIF_TunerReadNoSubAddr(0x100,
                                                       (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_addr),
                                                       (UINT8 *)&(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_data),
                                                       1);  */
                        i4Val = SIF_TunerReadNoSubAddr(u4Clk,
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_addr),
                                                        (UINT8 *)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->pui1_data),
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_data_len));
//                        Printf("SIF_TunerReadNoSubAddr: ClkDiv:0x100  ui4_addr:0x%02X  ui1_data:0x%02X  ByteCnt:1\n", ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_addr, ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui1_data);
                        Printf("SIF_TunerReadNoSubAddr: ClkDiv:0x%x  ui4_addr:0x%02X  \n", u4Clk, ((PC_SIF_INFO_ADVANCED_T *)pv_get_info)->ui4_addr);
                        Printf("Return i4Val=%d\n", i4Val);
                    }                    
                    break;
            }
            
            if (i4Val == 0)
            { 
                /* Failed */
                //return RMR_DRV_ERROR;		//this could make RM abort...
                return RMR_DRV_INV_GET_INFO;
            }

            break;

    case PC_GET_TYPE_GPIO_MONITOR_CTRL:
        VERIFY( *pz_get_info_len == 8 );
        /* check validatity of parameter length */
        t_get_info_len = sizeof(UINT64);
        if (*pz_get_info_len < sizeof(UINT64) )
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        else
        {
            *pz_get_info_len = t_get_info_len;
        }
        *((UINT64*)pv_get_info) = _u8GpioModeCtrlStatus;
        break;

        case PC_GET_TYPE_UPGRADE_CTRL:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }
            i4Val = EEPDTV_GetCfg(&rDtvCfg);
            if (i4Val)
            {
                return RMR_DRV_ERROR;
            }
            *(UINT32 *)pv_get_info = (UINT32) (rDtvCfg.u1Upgrade | ((UINT32)rDtvCfg.u1Upgrade_Ext<<8) );
            break;

        case  PC_GET_TYPE_SERVO_AD:
            {
            	  UINT32 u4Val=0, u4Id, i = 100;
                t_get_info_len = sizeof(PC_SERVO_AD_INFO_T);
                if (*pz_get_info_len < t_get_info_len)
                {
                    return RMR_DRV_NOT_ENOUGH_SPACE;
                }
                else
                {
                    *pz_get_info_len = t_get_info_len;
                }
                u4Id = ((PC_SERVO_AD_INFO_T *)pv_get_info)->ui4_id;
                if (u4Id >= TOTAL_SERVOADC_NUMBER)
                {
                    ASSERT(0);
                    return RMR_DRV_ERROR;
                }                                
                do {
                    if(i-- == 0)
                    {
                        break;
                    }
                    u4Val = PDWNC_ReadServoADCChannelValue(u4Id);                    
                } while (u4Val != PDWNC_ReadServoADCChannelValue(u4Id));
                ((PC_SERVO_AD_INFO_T *)pv_get_info)->ui4_data = u4Val;            
            }
            break;
            
        default:
            return RMR_DRV_INV_GET_INFO;
    }

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: PCDSet
 *
 * Description: This API implements Middleware set functionality for Power 
 *              Control driver.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 PCDSet(DRV_COMP_ID_T* pt_comp_id,
             DRV_SET_TYPE_T e_set_type,
             const VOID*    pv_set_info,
             SIZE_T         z_set_info_len)
{
    UINT32 u4Val, fgVgaWorkAround = 0;
    INT32 i4Val, i4EdidWPGpio, i4EdidWPEnable, i4Gpio;
    TIME_T timeVal;
    DTVCFG_T rDtvCfg;
    UINT64 _u8OldGpioModeCtrlStatus;
    UINT32 u4GpioNum, u4CtrlWordBitNum, u4Clk;
    UINT8 e_channel_id;
    UINT64 _u8BitMask;
    UINT32 u4PDWNEvent;
   
    i4Val = 0; // DRVCUST_OptQuery(eFlagVgaWakeWorkAround, &fgVgaWorkAround);
    fgVgaWorkAround = (i4Val < 0) ? 0 : fgVgaWorkAround;

    /* Check aguments */
    if (!b_pcd_init)
    {
        return RMR_DRV_INV_SET_INFO;
    }
        
    i4EdidWPEnable = (INT32)DRVCUST_OptGet(eEdidEepromWPEnablePolarity);
        
    if (0 > DRVCUST_OptQuery(eEdidEepromWPGpio, &u4Val))
    {
        i4EdidWPGpio = -1;
    }
    else
    {
        i4EdidWPGpio = (INT32)u4Val;
    }

    if ((pt_comp_id == NULL) ||
        (pt_comp_id->e_type != DRVT_PWR_CTRL) ||
        (pt_comp_id->ui2_id != PCD_DRV_ID))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    /* Perform set opertion */
    switch (e_set_type)
    {
        case PC_SET_TYPE_WAKE_UP_SETUP:
            u4Val = (UINT32)pv_set_info;
            _fgVGAWakeup = (u4Val & PC_WAKE_UP_CAP_VGA) ? TRUE : FALSE;
            _fgUartNormalWakeup = (u4Val & PC_WAKE_UP_CAP_UART) ? TRUE : FALSE;            
            if (!(u4Val & PC_WAKE_UP_CAP_RTC))
            {
                _u8UtcWakeup = 0;
            }
                                    
            if ((fgVgaWorkAround) &&
                (BSP_GetIcVersion() <= IC_VER_AB) &&
                (u4Val & PC_WAKE_UP_CAP_VGA))
            {
                // Disconnect IR path to middleware but for waiting Power to reboot.
                IRRC_DisconnectMiddleware();
            }
            break;

        case PC_SET_TYPE_WAKE_UP_TIME:
            timeVal = *((TIME_T *)pv_set_info);
            _u8UtcWakeup = (UINT64)timeVal;
            if(!RTC_SetAlertTime(_u8UtcWakeup))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        case PC_SET_TYPE_NFY_FCT:
            if(pv_set_info == NULL)
            {
                return RMR_DRV_INV_SET_INFO;
            }
            _PCDSetPCNfyFct( (PC_NFY_INFO_T*)pv_set_info, PC_WAKE_UP_REASON_RTC);
            _rGpioMonitorNfyInfo = *(PC_NFY_INFO_T*)pv_set_info;        //for gpio monitor use (PC_SET_TYPE_GPIO_MONITOR_CTRL)
            break;

        case PC_SET_TYPE_MISC_CTRL:
            u4Val = (UINT32)pv_set_info;
            _PCDSetEepromFlag(((u4Val & PC_MISC_CTRL_NO_STANDBY) ? DTVCFG_FLAG_POWERON_BOOT : 0) |
                              ((u4Val & PC_MISC_CTRL_FACTORY_MODE) ? DTVCFG_FLAG_FACTORY_MODE : 0) |
                              ((u4Val & PC_MISC_CTRL_POWER_LED_OFF) ? DTVCFG_FLAG_POWER_LEDOFF : 0) |
                              ((u4Val & PC_MISC_CTRL_NO_STANDBY_2) ? DTVCFG_FLAG_POWERON_LAST : 0) |
                              ((u4Val & PC_MISC_CTRL_FIRMWARE_UPGRADE) ? DTVCFG_FLAG_USB_UPGRADE_ON : 0) |
                              ((u4Val & PC_MISC_CTRL_COMPANION_CHIP_UPGRADE) ? DTVCFG_FLAG_8280_UPGRADE_ON : 0)
                              );
            if (u4Val & PC_MISC_CTRL_FACTORY_MODE)
            {
                if (i4EdidWPGpio >= 0)
                {
                    // disable edid write protect.
                    i4Val = !i4EdidWPEnable;
                    GPIO_Output(i4EdidWPGpio, &i4Val);
                }
            }
            else
            {
                if (i4EdidWPGpio >= 0)
                {
                    // enable edid write protect.
                    i4Val = i4EdidWPEnable;
                    GPIO_Output(i4EdidWPGpio, &i4Val);
                }
            }
            break;

        case PC_SET_TYPE_POWER_STATE:
            if(pv_set_info == NULL)
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4Val = EEPDTV_GetCfg(&rDtvCfg);
            ASSERT(i4Val==0);
            if ((*((PC_PWR_STATE_CTRL_T *)pv_set_info)) == PC_PWR_STATE_CTRL_STANBY)
            {
                rDtvCfg.u1Flags &= ~(DTVCFG_FLAG_SYSTEM_STATE_ON);
            }
            else
            {
                rDtvCfg.u1Flags |= (DTVCFG_FLAG_SYSTEM_STATE_ON);
            }
            i4Val = EEPDTV_SetCfg(&rDtvCfg);
            ASSERT(i4Val==0);
            break;

        case PC_SET_TYPE_POWER_DOWN:
            u4Val = WAK_IRRX;
            u4PDWNEvent = (UINT32)PDWNC_POWER_DOWN_IRRC;
            if (_u8UtcWakeup)
            {
                u4Val |= WAK_RTC;
                // Set wakeup timer at RTC.
                RTC_SetAlertTime(_u8UtcWakeup);
            }
            if (_fgVGAWakeup)
            {
                u4Val |= WAK_VGA;
                u4PDWNEvent = (UINT32)PDWNC_POWER_DOWN_VGA;
            }
            if (_fgUartNormalWakeup)
            {
                u4Val |= WAK_UNOR;
            }
#ifndef ADVANCED_STANDBY_CTRL
            if (DRVCUST_OptGet(eStdbyUsePrevPowerState))
#endif /* ADVANCED_STANDBY_CTRL */
            {
                // Set System state off.
                i4Val = EEPDTV_GetCfg(&rDtvCfg);
                ASSERT(i4Val==0);
                rDtvCfg.u1Flags &= ~(DTVCFG_FLAG_SYSTEM_STATE_ON);
                i4Val = EEPDTV_SetCfg(&rDtvCfg);
                ASSERT(i4Val==0);
            }

            if (DRVCUST_OptGet(eStdbyUsePowerBtnCount))
            {
                UINT8 u1OrgFlags;

                i4Val = EEPDTV_GetCfg(&rDtvCfg);
                ASSERT(i4Val==0);
                u1OrgFlags = rDtvCfg.u1Flags;
                if (IRRC_IsPowerButtonOff())
                {
                    // Set POWER_BTN_ON from X to 0.
                    rDtvCfg.u1Flags &= ~(DTVCFG_FLAG_POWER_BTN_ON);
                    // Power off mode.
                    u4Val = 0;
                    
                    u4PDWNEvent = (UINT32)PDWNC_POWER_DOWN_KEYPAD;
                }
                else
                {
                    // Set POWER_BTN_ON from X to 1.
                    rDtvCfg.u1Flags |= (DTVCFG_FLAG_POWER_BTN_ON);
                }
                if (u1OrgFlags != rDtvCfg.u1Flags)
                {
                    i4Val = EEPDTV_SetCfg(&rDtvCfg);
                    ASSERT(i4Val==0);
                }
            }

            if ((u4Val & WAK_VGA) && DRVCUST_OptGet(eStdbyUseVGAStatus))
            {
                i4Val = EEPDTV_GetCfg(&rDtvCfg);
                ASSERT(i4Val==0);
                rDtvCfg.u1Flags &= ~(DTVCFG_FLAG_POWER_BTN_ON);
                i4Val = EEPDTV_SetCfg(&rDtvCfg);
                ASSERT(i4Val==0);
            }

            PDWNC_EnterPowerDown(u4Val, u4PDWNEvent);
            break;

        case PC_SET_TYPE_REBOOT:
            PDWNC_Reboot();
            break;

        case PC_SET_TYPE_GPIO_MODE:
            i4Gpio = _PCD_XferGpio(((PC_GPIO_MODE_INFO_T *)pv_set_info)->ui4_id);
            if (i4Gpio < 0)
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4Val = (((PC_GPIO_MODE_INFO_T *)pv_set_info)->e_mode == PC_GPIO_MODE_OUTPUT) ? 1 : 0;
            GPIO_Enable(i4Gpio, &i4Val);
            break;

        case PC_SET_TYPE_GPIO_STATUS:
            i4Gpio = _PCD_XferGpio(((PC_GPIO_STATUS_INFO_T *)pv_set_info)->ui4_id);
            if (i4Gpio < 0)
            {
                return RMR_DRV_INV_SET_INFO;
            }

            i4Val = (((PC_GPIO_STATUS_INFO_T *)pv_set_info)->e_status == PC_GPIO_STATUS_HIGH) ? 1 : 0;
            GPIO_Output(i4Gpio, &i4Val);
            break;

        case PC_SET_TYPE_SIF:
            i4Val = SIF_Write(0x100,
                        (UINT8)(((PC_SIF_INFO_T *)pv_set_info)->ui4_addr),
                        (UINT8)(((PC_SIF_INFO_T *)pv_set_info)->ui4_offset),
                        (UINT8 *)&(((PC_SIF_INFO_T *)pv_set_info)->ui1_data),
                        1);
            if (i4Val == 0)
            { // failed
                //return RMR_DRV_ERROR;		//this could cause RM abort...
                return RMR_DRV_INV_SET_INFO;
            }
            break;

        case PC_SET_TYPE_SIF_ADVANCED:
            i4Val = 0;	u4Clk = g_u4AdvSifClkDiv;
            e_channel_id = ((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->e_channel_id ;
            
            switch (e_channel_id)
            {
                case PC_SIF_CH_ID_EDID_BUS:
                    /* Do nothing */
                    break;
                case PC_SIF_CH_ID_SYS_BUS:
                    if( ((((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_offset_len)<=3) )  /* check sub-address length*/
                    {
                        LOG(7,"SIF_X_Write: SIF_BUS_SYSTEM_I2C ClkDiv:0x%x  ui4_addr:0x%02X  \n", u4Clk, ((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr);
                        i4Val = SIF_X_Write(		SIF_BUS_SYSTEM_I2C,
                                                    u4Clk,
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr),
                                                    (UINT8)((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_offset_len,                                                         
                                                    (UINT32)((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_offset,
                                                        (UINT8 *)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->pui1_data),
                                                    (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_data_len)
                                            );
                        LOG(7,"Return i4Val=%d\n", i4Val);
		    }
                    break;
                case PC_SIF_CH_ID_TUNER_BUS:
#if 0
                    if((((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_offset_len) == 0x00 )  /* No sub-address */
                    {
//                        Printf("SIF_TunerWriteNoSubAddr: ClkDiv:0x100  ui4_addr:0x%02X  ui1_data:0x%02X  ByteCnt:1\n", ((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr, ((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_data);
                        Printf("SIF_TunerWriteNoSubAddr: ClkDiv:0x100  ui4_addr:0x%02X  \n", ((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr);
/*                        i4Val = SIF_TunerWriteNoSubAddr(0x100,
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr),
                                                        (UINT8 *)&(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_data),
                                                        1);	*/
                        i4Val = SIF_TunerWriteNoSubAddr(0x100,
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr),
                                                        (UINT8 *)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->pui1_data),
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_data_len));
                        Printf("Return i4Val=%d\n", i4Val);
                    }                    
#else	//more general
                    if( ((((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_offset_len)<=3) )  /* check sub-address length*/
                    {
                        Printf("SIF_X_Write: SIF_BUS_TUNER_I2C ClkDiv:0x%x  ui4_addr:0x%02X  \n", u4Clk, ((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr);
                        i4Val = SIF_X_Write(		SIF_BUS_TUNER_I2C,
                        				u4Clk,
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui4_addr),
                                                        0, 
                                                        0,
                                                        (UINT8 *)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->pui1_data),
                                                        (UINT8)(((PC_SIF_INFO_ADVANCED_T *)pv_set_info)->ui1_data_len));
                        Printf("Return i4Val=%d\n", i4Val);
                    }                    
#endif
		    break;
            }
            
            if (i4Val == 0)
            { 
                /* Failed */
                //return RMR_DRV_ERROR;		//this could cause RM abort...
                return RMR_DRV_INV_SET_INFO;
            }
            break;

    case PC_SET_TYPE_GPIO_MONITOR_CTRL:
        /* check validatity of parameter length */
        VERIFY( z_set_info_len == 8 );
        if ( z_set_info_len < sizeof(UINT64) )
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        else
        {
            z_set_info_len = sizeof(UINT64);
        }

        /* register interrupt */
        //check GPIO validatity first 
        VERIFY( 0 == GPIO_Init() ); 

        _u8OldGpioModeCtrlStatus = _u8GpioModeCtrlStatus;
        _u8GpioModeCtrlStatus =   *((UINT64*)pv_set_info) ;
        u4GpioNum= 0;
        
        for( u4CtrlWordBitNum = 0 ; u4CtrlWordBitNum <=63 ; u4CtrlWordBitNum++)
        {
            if( u4CtrlWordBitNum<=55 )      //GPIO                      
            {
                u4GpioNum = u4CtrlWordBitNum;           }
            else
            {
                u4GpioNum = u4CtrlWordBitNum -56 + OPCTRL0 ;            
            }

            LOG( 9 , "[PCDSET]: GPIO %d\n", u4GpioNum );
            //Printf("[PCDSET]: GPIO %d\n", u4GpioNum);

            _u8BitMask = ((UINT64)1<< u4CtrlWordBitNum);    
            if( _u8GpioModeCtrlStatus & _u8BitMask )    //do set
            {
                VERIFY( GPIO_Reg( u4GpioNum , GPIO_TYPE_INTR_BOTH, _GpioPolarityChangeNotify_Isr ) >=0 );
                Printf("\r[PCDSET]: GPIO %d hook to notify isr\n", u4GpioNum);
            }
            else        //do un-set
            {
                //set already before?
                if( (_u8OldGpioModeCtrlStatus&(1<< u4CtrlWordBitNum)) == TRUE  )        //yes: unset it
                {
                    VERIFY( GPIO_Reg( u4GpioNum , GPIO_TYPE_INTR_BOTH, NULL ) >=0 );
                    Printf("\r[PCDSET]: GPIO %d was hooked before-> hook to NULL \n", u4GpioNum);
                }
                else        //no: dont care
                {
                    //do nothing
                }
            }
        }

        break;

        case PC_SET_TYPE_UPGRADE_CTRL:
            i4Val = EEPDTV_GetCfg(&rDtvCfg);
            if (i4Val)
            {
                return RMR_DRV_ERROR;
            }
            rDtvCfg.u1Upgrade = (UINT8)(((UINT32)pv_set_info) & 0xffU);
            rDtvCfg.u1Upgrade_Ext = (UINT8) ((((UINT32)pv_set_info) & 0xff00U)>>8);
            i4Val = EEPDTV_SetCfg(&rDtvCfg);
            if (i4Val)
            {
                return RMR_DRV_INV_SET_INFO;
            }
            break;
        default:
            return RMR_DRV_INV_SET_INFO;
    }

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: MW_PCDInit
 *
 * Description: This API initializes Power Control driver Middleware 
 *              adaptation layer.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 MW_PCDInit(void)
{
    INT32               i4_return;
    DRV_COMP_REG_T      t_pcd_comp_reg;
    DRV_COMP_FCT_TBL_T  t_pcd_comp_fct_tbl;

    if (b_pcd_init)
    {
        return RMR_OK;
    }

    _CheckWakeupReason();

    UNUSED(_u4TheInitKey);
    if ((_rPCWakeupReason) && (DRVCUST_OptGet(eWakeupReasonGpio)))
    {
        if (GPIO_Input((INT32)DRVCUST_OptGet(eWakeupReasonGpio))!=(INT32)DRVCUST_OptGet(eWakeupReasonGpioPolarity))
        {
            _rPCWakeupReason = (PC_WAKE_UP_REASON_T)0;
        }
    }

    if (!RTC_Init())   // Init failed.
    {
        return RMR_DRV_ERROR;
    }
            
    /* Init underneath driver */
    if (!_PCDInit())
    {
        return RMR_DRV_ERROR;
    }

    RTC_HookAlertHandler(_PCDRTCNfyFct);
                
    /* Setup PCD component ID */
    t_pcd_comp_reg.e_type = DRVT_PWR_CTRL;
    t_pcd_comp_reg.e_id_type = ID_TYPE_IND;
    t_pcd_comp_reg.u.t_ind.ui2_id = PCD_DRV_ID;
    t_pcd_comp_reg.u.t_ind.pv_tag = NULL;
    t_pcd_comp_reg.u.t_ind.ui1_port = 0;

    /* Setup PCD component function table */
    t_pcd_comp_fct_tbl.pf_rm_connect = NULL;
    t_pcd_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_pcd_comp_fct_tbl.pf_rm_get = PCDGet;
    t_pcd_comp_fct_tbl.pf_rm_set = PCDSet;

    /* Register PCD component with Resource Manager database */
    i4_return = x_rm_reg_comp(&t_pcd_comp_reg,
                              1,
                              1,
                              NULL,
                              0,
                              &t_pcd_comp_fct_tbl,
                              NULL,
                              0);
                              
    b_pcd_init = TRUE;                              

    return i4_return;
}

INT32 d_custom_misc_get_loader_info(UINT8 *szVersion, UINT32 *pu4VersionSize, UINT32 *pu4CheckSum)
{
    UINT8 *szFlashVer;
    UINT32 u4FlashAddr;
    UINT8 szBuf[32];

    u4FlashAddr = 0x28000000 + DRVCUST_InitGet(eNorFlashLoaderSize);
    szFlashVer = (UINT8 *)(u4FlashAddr - 64);

    if (pu4VersionSize)
    {
        if ((*pu4VersionSize) > x_strlen((CHAR *)szFlashVer))
        {
            x_strncpy((CHAR *)szVersion, (CHAR *)szFlashVer, 32);
        }
        x_memcpy(szBuf, szFlashVer, 32);
        szBuf[31] = 0;
        *pu4VersionSize = x_strlen((CHAR *)szBuf) + 1;
    }

    if (pu4CheckSum)
    {
        *pu4CheckSum = *((UINT32 *)(u4FlashAddr - 4));
    }
    return 0;
}

#if 0
INT32 d_custom_misc_set_loader_upgrade(BOOL fgEnable)
{
    DTVCFG_T rDtvCfg;
    INT32 i4Ret;

    i4Ret = EEPDTV_GetCfg(&rDtvCfg);
    ASSERT(i4Ret==0);
    if (i4Ret!=0)
    {
        Printf("DTV EEPROM read failure.\n");
        return -1;
    }

    if (fgEnable)
    {
        rDtvCfg.u1Flags |= DTVCFG_FLAG_USB_UPGRADE_ON;
        rDtvCfg.u4Flags |= DTVCFG_FLAG_USB_UPGRADE_ON;
    }
    else
    {
        rDtvCfg.u1Flags &= ~(DTVCFG_FLAG_USB_UPGRADE_ON);
        rDtvCfg.u4Flags &= ~(DTVCFG_FLAG_USB_UPGRADE_ON);
    }

    i4Ret = EEPDTV_SetCfg(&rDtvCfg);

    return 0;
}
#endif

INT32 d_custom_get_custom_mode()
{
    return (GET_INIT_CUSTOM_MODE());
}

