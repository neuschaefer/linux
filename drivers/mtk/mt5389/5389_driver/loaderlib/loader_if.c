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
 * $Author: jiangbo.wang $
 * $Date: 2012/12/17 $
 * $RCSfile: loader_if.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_if.c
 *  loader_if.c provides loader high-level interface functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

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
#include "nor_if.h"

#ifdef CC_NAND_BOOT
#include "nand_if.h"
#endif

#ifdef CC_EMMC_BOOT
#include "msdc_if.h"
#endif

#include "drvcust_if.h"
#include "loader_if.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------
#define KEY_ESCAPE                      0x1b

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static LDR_DATA_T  _grLdrData;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** LDR_GetLdrData(): get the loader data structure pointer.
 *
 *  @param  no parameter
 *  @retval LDR_DATA_T pointer. the pointer to loader data structure.
 */
//-----------------------------------------------------------------------------
LDR_DATA_T *LDR_GetLdrData(VOID)
{
    return &_grLdrData;
}

//-----------------------------------------------------------------------------
/** LDR_IsEnterCli(): Loader check escape from execution to cli environment
 *  This function is to check whether uart has a ESC key in a period, then
 *  return the result.
 *
 *  @param  u4BootWaitTime  ms value to wait in this function.
 *  @retval 0               No ESC, continue to execute.
 *  @retval otherwise       break into cli mode.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_IsEnterCli(UINT32 u4BootWaitTime)
{
    HAL_TIME_T t0, t1, dt;
    UINT32 u4Diff;
    UINT32 fgRet = FALSE;

#if defined(NDEBUG) && defined(CC_LOADER_DISABLE_INPUT)
    LDR_DATA_T* prLdrData = LDR_GetLdrData();

    if ((prLdrData->rDtvCfg.u1Flags3 & DTVCFG_FLAG3_SUPPORT_CLI)==0)
    {
        return 0;
    }
#endif

    // UART change to transparent mode.
    SerTransparent();

    HAL_GetTime(&t0);
    while (1)
    {
        UCHAR uc;

        if (SerInByte(&uc))
        {
            if (uc == KEY_ESCAPE)
            {
                fgRet = TRUE;
                break;
            }
        }

        HAL_GetTime(&t1);
        HAL_GetDeltaTime(&dt, &t0, &t1);

        u4Diff = dt.u4Seconds * 1000 + dt.u4Micros / 1000;
        if (u4Diff >= u4BootWaitTime)
        {
            break;
        }
    }

    if (!fgRet)
    {
        // UART change to command mode.
        SerNormalMode();
    }
    else
    {
        HalFlushInvalidateDCache();
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** LDR_UsbVbusOn(): Turn on USB Vbus.
 *  This function is going to setup boot settings, such as GPIO_Init().
 *
 *  @param  void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void LDR_UsbVbusOn(void)
{
    UINT32 u4Gpio;    
    UINT32 u4GpioPolarity;

    // Set USB gpio init
    if ((0 == DRVCUST_InitQuery(eUSBVbus0Gpio, &u4Gpio)) &&
        (0 == DRVCUST_InitQuery(eUSBVbus0EnablePolarity, &u4GpioPolarity)))
    {
        Printf("USB0: Set GPIO%d = %d.\n", u4Gpio, u4GpioPolarity);
        (void)GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity);
    }
    if ((0 == DRVCUST_InitQuery(eUSBVbus1Gpio, &u4Gpio)) &&
        (0 == DRVCUST_InitQuery(eUSBVbus1EnablePolarity, &u4GpioPolarity)))
    {
        Printf("USB1: Set GPIO%d = %d.\n", u4Gpio, u4GpioPolarity);
        (void)GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity);
    }
    if ((0 == DRVCUST_InitQuery(eUSBVbus2Gpio, &u4Gpio)) &&
        (0 == DRVCUST_InitQuery(eUSBVbus2EnablePolarity, &u4GpioPolarity)))
    {
        Printf("USB2: Set GPIO%d = %d.\n", u4Gpio, u4GpioPolarity);
        (void)GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity);
    }
    if ((0 == DRVCUST_InitQuery(eUSBVbus3Gpio, &u4Gpio)) &&
        (0 == DRVCUST_InitQuery(eUSBVbus3EnablePolarity, &u4GpioPolarity)))
    {
        Printf("USB3: Set GPIO%d = %d.\n", u4Gpio, u4GpioPolarity);
        (void)GPIO_Output((INT32)u4Gpio, (INT32 *)&u4GpioPolarity);
    }    
}
//-----------------------------------------------------------------------------
/** LDR_UpSetup(): Do necessary setup settings for image boot up.
 *  This function is going to setup boot settings, such as GPIO_Init().
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_UpSetup(LDR_DATA_T *prLdrData)
{
    GPIO_Init();
    x_thread_delay(50); //delay 50ms
    GPIO_SetOut(GPIO(21),1); //set the gpio21 to high.
    LDR_UsbVbusOn();
    return 0;
}

//-----------------------------------------------------------------------------
/** LDR_ImageJump(): leave loader and jump to image binary in dram.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_ImageJump(LDR_DATA_T *prLdrData)
{
    PFN_IMAGE pfnImage;

    if (prLdrData->rLdrHdr.u4FirmwareExecAddr == 0)
    {
        Printf("\nERROR: no executive address\n");
        return 1;
    }

    //LDR_EEPWriteEnable(FALSE);

#ifndef CC_UBOOT
    // Sync instruction and data caches
    HalInstructionMemoryBarrier();

    // Disable MMU and caches
    HalDisableCaches();
    HalDisableMMU();
#endif

    // Execute the image
    Printf("Starting image...\n\n");

    // Wait until com data flush.
    SerWaitTxBufClear();
    // Set to normal mode back.
    SerEnd();

    pfnImage = (PFN_IMAGE)prLdrData->rLdrHdr.u4FirmwareExecAddr;
    pfnImage();

    return 0;
}

UINT32 LDR_FlashCopy(UINT8* pu1Buf, UINT32 u4FlashOffset, UINT32 u4Size)
{
#if defined(CC_EMMC_BOOT)
    Loader_ReadMsdc(0, u4FlashOffset, (void *)pu1Buf, u4Size);
#elif defined(CC_NAND_BOOT)
    Loader_ReadNandFlash(0, u4FlashOffset, (void *)pu1Buf, u4Size);
#else
    NOR_Read((UINT64)u4FlashOffset, (UINT32)pu1Buf, u4Size);
#endif

    return 0;
}
