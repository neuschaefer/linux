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

/** @file fcihw_reg.c
 *  This C file implements the FCI module functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "sdc_const.h"
#include "msp_const.h"

#include "fci_type.h"
#include "fci_debug.h"
#include "fcihw_reg.h"
#include "fcihw_io.h"
#include "fcihw_func.h"
#include "fci_if.h"
#include "sdc_func.h"
#include "msp_func.h"
#ifdef CC_BD_FCI
#include "x_debug.h"
#endif

#ifdef CC_FCI_SDIO_SUPPORT
#include "sdio_func.h"
#endif

LINT_EXT_HEADER_BEGIN

#ifdef CC_BD_FCI
#include "x_hal_ic.h"
#include "x_hal_1176.h"
#elif defined(CC_DTV_FCI)
#include "x_hal_5381.h"
#include "x_hal_926.h"
#endif

#include "x_typedef.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_pinmux.h"
#ifdef CC_BD_FCI
#include "x_pdwnc.h"  //mhzhang
#include "x_ckgen.h"
#elif defined(CC_DTV_FCI)
#include "x_clksrc.h"
#endif
#include "drvcust_if.h"

LINT_EXT_HEADER_END

#ifndef FCIHW_INTERRUPT_WRITE_CLEAR
UINT32 _u4ProceedIntrs = 0;
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
//#define FCIHW_POLLING
//#define FLASH_INTERFACE // For MT5362A/B

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// Time out of FCI polling interrupt status. Unit = second.

#define FCI_ISR_TIMEOUT_PERIOD       (5)

/// FCI debounce time for card detection. Unit = milli-second.
#define FCI_CARD_DEBOUNCE_ENABLE
#define FCI_CARD_DEBOUNCE_TIME      ((UINT32)500)

/// FCI wait interrupt time. Unit = milli-second.
#define FCI_WAIT_INT_TIME             ((UINT32)300)
#define FCI_WAIT_SIT_TIME             ((UINT32)1000)    // For MSP card, INT timeout is 1000 ms

#ifndef FCI_SDMS_S_RST_INT
static UINT32 _u4FciHwRstWtTimes = 1; // repeat write opeartion to wait sd/ms host sync with RISC domain
#endif
static UINT32 _u4FciWaitIntTime = FCI_WAIT_INT_TIME; // Wait for Int delay time (unit : ms)
static UINT32 _u4FciWaitSitTime = FCI_WAIT_SIT_TIME; // Wait for Int delay time (unit : ms)

#if 0
#ifdef CC_CARD_DETECT_PIN
#define FCI_CARD_DETECT_PIN     ((INT32)11)
#endif
#endif

#define CKGEN_SETBIT(offset, dBit)        CKGEN_WRITE32(offset, CKGEN_READ32(offset) | (dBit))
#define CKGEN_CLRBIT(offset, dBit)        CKGEN_WRITE32(offset, CKGEN_READ32(offset) & (~(dBit)))

#ifdef CC_BD_FCI
//mhzhang
#define PDWNC_SETBIT(offset, dBit)       PDWNC_WRITE32(offset, PDWNC_REG32(offset) | (dBit));
#define PDWNC_CLRBIT(offset, dBit)       PDWNC_WRITE32(offset, PDWNC_REG32(offset) | (~dBit));
#endif

UINT32 _u4SDClockSetting = 0;
UINT32 _u4MSClockSetting = 0;
extern BOOL  _fgFCI4BitBusEnable;
BOOL _fgFCIContinueClock = FALSE;


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static volatile INT32 _i4FCIPrevSlot = NONE_SLOT;

/// Timer handle for card detection debounce.
#ifdef FCI_CARD_DEBOUNCE_ENABLE
static HANDLE_T _hFCItimerHdl;
#endif

#ifdef CC_DTV_FCI
/// FCI semaphore
static HANDLE_T _hFCIRDYSema;
#endif

/// MS RDY Binary semaphore.
static HANDLE_T _hFCIMSRDYSema;

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
static HANDLE_T _hFCIMSSITSema;
#endif

/// SD DR Binary semaphore.
static HANDLE_T _hFCISDDRSema;

#ifdef FCI_SDMS_S_RST_INT
/// SD SoftReset Binary semaphore.
static HANDLE_T _hFCISRSTSema;
#endif

/// SD CR Binary semaphore.
static HANDLE_T _hFCISDCRSema;

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 use INTR_SD_DTRDYI instead
/// SD WBF Binary semaphore.
static HANDLE_T _hFCISDWBFSema;
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
/// SD RBI Binary semaphore.
static HANDLE_T _hFCISDRBISema;
#endif

/// SD, MS Int Status
static UINT32 _u4FCISStatus = 0x0;

#endif  // #ifdef FCIHW_INTERRUPT_WRITE_CLEAR

#ifndef SMXD_USING_NAND_IF
/// SM RDY Binary semaphore.
static HANDLE_T _hFCISMRDYSema;
#endif

/// ISR is registered or using polling mode.
static INT32 _fgFCIIsrEn = FALSE;

/// Card remove status: FALSE = card inserted, TRUE = card removed.
#ifdef CC_CARD_DETECT_PIN

#if 0
/// Previous card detection interrupt status.
static UINT32 _u4FCICDStatus = INTR_FCI_CDI;
#endif

/// Card remove status: FALSE = card inserted, TRUE = card removed.
static INT32 _fgFCICardRemove = TRUE;

#if 0
static UINT32 _u4FCICardDetectPin = FCI_CARD_DETECT_PIN;
#endif

#endif

// SDIO interrupt handler.
#ifdef CC_FCI_SDIO_SUPPORT
static FCI_CALLBACK _pfnSDIOINT = NULL;
#endif

extern INT32 _fgSdCardRemove;
extern INT32 _fgMsCardRemove;

//static INT16 _u2FciIsrCnt = 0;

// SRAM FIFO Setting
static UINT32 _u4SRAMSel = 1;   // 01: SRAM size is 32 x 32 bits (128 bytes).

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)
UINT32 _u4SDMultiFuncCfg = SD1_MLT_FNC_CFG;
UINT32 _u4MSMultiFuncCfg = MS_MLT_FNC_CFG;
#else
UINT32 _u4SDMultiFuncCfg = SD_MLT_FNC_CFG;
UINT32 _u4MSMultiFuncCfg = MS_MLT_FNC_CFG;
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#ifdef CC_CARD_DETECT_PIN
//-------------------------------------------------------------------------
/** _Fcihw_GetCardStatus
 *  Get SD, MS, xD detection status.
 *  @param  VOID
 *  @return mask all cards detection status, 1 = card remove, 0 = card insert.
 */
//-------------------------------------------------------------------------
static INT32 _Fcihw_GetCardStatus(VOID)
{
#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520) // MT8520

    CKGEN_CLRBIT(0xC0, ((UINT32)0x03 << 10));     //  D0C0h[11:10] = 00  (PAD_CFG_0[11:10])
    CKGEN_CLRBIT(0xCC, ((UINT32)0x01 << 0));     //  D0CCh[0] = 0  (PAD_CFG_3[0])
    CKGEN_CLRBIT(0xCC, ((UINT32)0x01 << 28));     //  D0CCh[28] = 0  (PAD_CFG_3[23])
    CKGEN_CLRBIT(0xD4, ((UINT32)0x01 << 0));    //  D0D4h[0] = 0  (PAD_CFG_5[0])：
    
    // 設定I/O方向（0：input、1：output）
    CKGEN_CLRBIT(0x18C, ((UINT32)0x01 << 10));    // enable：D18Ch[10]  (controll I/O direction, GPIO_EN3[10])

    _fgFCICardRemove = (CKGEN_READ32(0x1CC) & ((UINT32)0x01 << 10)) ? TRUE : FALSE; // input： read D1CCh[10]  (GPIO_IN3[10])

#ifdef CC_FCI_3_IN_1_SOCKET
    if (_fgFCICardRemove)   // if no card inserted => SD & MS be all not inserted   
    {
        _fgSdCardRemove = _fgMsCardRemove = _fgFCICardRemove;
    }
#endif

    if (_fgFCICardRemove)
    {
        LOG(1, "Card Not Insert !!\n");
    }
    
    return _fgFCICardRemove;

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)
    // To be Add : Jethro
    return _fgFCICardRemove;

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)
    // To be Add : Jethro
    return _fgFCICardRemove;

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)


    //以下CKGEN 的 register 都 program成 0，VIND7才可做 GPIO 用：
    CKGEN_CLRBIT(0xC0, ((UINT32)0x01 << 26));    // 00C0[26]
    CKGEN_CLRBIT(0xC8, ((UINT32)0xFF << 24) | ((UINT32)0x3F << 16));    // 00C8[31:24], 00C8[22:16]
    CKGEN_CLRBIT(0xCC, ((UINT32)0x0F << 0));    // 00CC[3:0]
    CKGEN_CLRBIT(0xD0, ((UINT32)0x3F << 22));    // 00D0[28:22]
    CKGEN_CLRBIT(0xD4, ((UINT32)0x01 << 3));    // 00D4[3]
    
    // 設定I/O方向（0：input、1：output）
    CKGEN_CLRBIT(0x188, ((UINT32)0x01 << 4));    // 0188[4]

    _fgFCICardRemove = (CKGEN_READ32(0x1C8) & ((UINT32)0x01 << 4)) ? TRUE : FALSE; // 01C8[4]：input value

#ifdef CC_FCI_3_IN_1_SOCKET
    if (_fgFCICardRemove)   // if no card inserted => SD & MS be all not inserted
    {
        _fgSdCardRemove = _fgMsCardRemove = _fgFCICardRemove;
    }
#endif
    
    if (_fgFCICardRemove)
    {
        LOG(1, "Card Not Insert !!\n");
    }
    
    return _fgFCICardRemove;
#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)

    if (_u4SDMultiFuncCfg == 2)   // Multi-function use GPIO43 as fixed card detection PIN, and it can be read by FCI register
    {        
        _fgFCICardRemove = FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_FCI_SDCD;
    }
    else                                     // Multi-function 1, 3 must use other GPIO as card detection pin
    {
    }

    return _fgFCICardRemove;
#endif
}
#endif    // #ifdef CC_CARD_DETECT_PIN

//-------------------------------------------------------------------------
/** _Fcihw_GetSec
 *  Get system time. Unit is second.
 *  @param  VOID
 *  @return system time. Unit is second.
 */
//-------------------------------------------------------------------------
#ifdef FCIHW_POLLING
static UINT32 _Fcihw_GetSec(VOID)
{
    HAL_TIME_T rTime;

    HAL_GetTime(&rTime);

    return rTime.u4Seconds;
}
#endif    // #ifdef FCIHW_POLLING

#if 0
#ifdef CC_CARD_DETECT_PIN
//-------------------------------------------------------------------------
/** _Fcihw_CheckCard
 *  Debounce timer to check card status.
 *  @param  pt_tm_handle   timer's handle.
 *  @param  pv_tag   user's tag
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_CheckCard(HANDLE_T pt_tm_handle, VOID *pv_tag)
{
    UINT32 u4Status;

    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    u4Status = _Fcihw_GetCardStatus();

    if ((_u4FCICDStatus & INTR_FCI_CDI) != (u4Status & INTR_FCI_CDI))
    {
        // Card remove status: 0 = card inserted, 1 = card removed.
        _fgFCICardRemove = (u4Status & INTR_FCI_CDI) ? TRUE : FALSE;

        LOG(1, "Card remove = %d.\n", _fgFCICardRemove);
        if (_pfnCardDetect)
        {
            _pfnCardDetect(_fgFCICardRemove);
        }
    }

    _u4FCICDStatus = u4Status;

    return;
}
#endif  // #ifdef CC_CARD_DETECT_PIN
#endif

#ifdef CC_DTV_FCI
//-------------------------------------------------------------------------
/** FCIHW_LockSemaphore
 *  lock fci semaphore
 *  @return  OSR_OK : lock OK
 */
//-------------------------------------------------------------------------
INT32 FCIHW_LockSemaphore(VOID)
{
    INT32 i4Ret;
    
    i4Ret = x_sema_lock(_hFCIRDYSema, X_SEMA_OPTION_WAIT);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** FCIHW_ReleaseSemaphore
 *  release fci semaphore
 *  @return  OSR_OK : lock OK
 */
//-------------------------------------------------------------------------
INT32 FCIHW_ReleaseSemaphore(VOID)
{
    // unlock the specific type semaphore.
    VERIFY(OSR_OK == x_sema_unlock(_hFCIRDYSema));

    return OSR_OK;
}
#endif

#ifdef CC_CARD_DETECT_PIN
//-------------------------------------------------------------------------
/** FCIHW_DetectCard
 *  Handle card inset/remove in FCI HISR by trigger debounce timer.
 *  @param  u4IntrStatus   interrupt status.
 *  @return  TRUE : Card remove
 *  @return  FALSE : Card Insert
 */
//-------------------------------------------------------------------------
INT32 FCIHW_DetectCard()
{
    return _Fcihw_GetCardStatus();
#if 0
    if ((_u4FCICDStatus & INTR_FCI_CDI) != (u4IntrStatus & INTR_FCI_CDI))

    {
#ifdef FCI_CARD_DEBOUNCE_ENABLE
        //restart timer by delay at card insert.
        if (_fgFCICardRemove)
        {
            // stop previous timer. It's ok to stop a non-start timer handle.
            VERIFY(OSR_OK == x_timer_stop(_hFCItimerHdl));
            VERIFY(OSR_OK == x_timer_start(_hFCItimerHdl, FCI_CARD_DEBOUNCE_TIME,
            X_TIMER_FLAG_ONCE, _Fcihw_CheckCard,
            NULL));
        }
        else
        {
            //do not restart timer at card remove.
            _Fcihw_CheckCard((HANDLE_T)NULL, (VOID *)NULL);
        }
#else
        _Fcihw_CheckCard((HANDLE_T)NULL, (VOID *)NULL);
#endif
    }
#endif    
}
#endif    // #ifdef CC_CARD_DETECT_PIN

//-------------------------------------------------------------------------
/** _Fcihw_HandleIntr
 *  Handle interrupt status change for each card type.
 *  @param  u4IntrStatus   interrupt status.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_HandleIntr(UINT32 u4IntrStatus)
{
    switch (_i4FCIPrevSlot)
    {
    case SDC_SLOT:

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // Error bit in MT8550/8555 is not interrupt source
        // error interrupt.
        if (INTR_SD_ERRI & u4IntrStatus)
        {
            // unlock the all semaphores.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDDRSema));
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDCRSema));
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDWBFSema));

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDRBISema));
#endif

            LOG(7, "SD ERRINT.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= (FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_SD_ERRI);

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SD_ERRI);
#endif
            return;
        }
#endif    // #if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)


#ifdef FCI_SDMS_S_RST_INT
        if (INTR_SDMS_SRST_INT & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISRSTSema));
            LOG(7, "SDMS_SRST unlock.\n");

            // Save FCI Status for further usage
            _u4FCISStatus |= (FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_SDMS_SRST_INT);

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SDMS_SRST_INT);            
        }
#endif

        if (INTR_SD_DTRDYI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDDRSema));
            LOG(7, "SDDR unlock.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= INTR_SD_DTRDYI;

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SD_DTRDYI);
#endif
        }
        if (INTR_SD_CMDRDYI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDCRSema));
            LOG(7, "SDCR unlock.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= INTR_SD_CMDRDYI;

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SD_CMDRDYI);
#endif
        }

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // WBE bit is not used in MT8550/8555 (use DTRDYI instead)
        if (INTR_SD_NWRITEBUSY & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDWBFSema));
            LOG(7, "SDWBF unlock.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= INTR_SD_NWRITEBUSY;

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SD_NWRITEBUSY);
#endif
        }
#endif    // #if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // RBI bit is not used as interrupt in MT8550/8555 (cab use DTRDYI instead)
        if (INTR_SD_RBI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISDRBISema));
            LOG(7, "SDRBI unlock.\n");

            // Save FCI Status for further usage
            _u4FCISStatus |= INTR_SD_RBI;

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SD_RBI);
        }
#endif  // #if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
#endif

#ifdef CC_FCI_SDIO_SUPPORT
#ifdef SDIO_1BIT_MODE
        u4IntrStatus = FCI_READ32(RW_FCI_INTR_STAT_REG);

// 538x 1 bit mode h/w bug:
// 538x will generate data in data1. This signal is the same as data0.
        if ((INTR_SD_DATAIDLE & u4IntrStatus) && (INTR_SDIO_INT & u4IntrStatus))
#else
        if (INTR_SDIO_INT & u4IntrStatus)
#endif  // #ifdef SDIO_1BIT_MODE
        {
            LOG(7, "SDIO INT.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= INTR_SDIO_INT;

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SDIO_INT);
#endif

            if (_pfnSDIOINT)
            {
                _pfnSDIOINT((INT32)u4IntrStatus);
            }
        }
#endif  // #ifdef CC_FCI_SDIO_SUPPORT
        break;

    case MSP_SLOT:
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
        if (INTR_MS_RDYI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCIMSRDYSema));
            LOG(7, "MSRI unlock.\n");

            // Save FCI Status for further usage
            _u4FCISStatus |= (FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_MS_RDYI);

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_MS_RDYI);
        }

        if (INTR_MS_SITI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCIMSSITSema));
            LOG(7, "MSSIT unlock.\n");

            // Save FCI Status for further usage
            _u4FCISStatus |= (FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_MS_SITI);

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_MS_SITI);
        }
#else
        if (INTR_MS_ALLI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCIMSRDYSema));
            LOG(7, "MSRI unlock.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= (FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_MS_ALLI);

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, _u4FCISStatus);
#endif
        }
#endif // #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
        break;

#ifdef CC_FCI_SMXD_SUPPORT
#ifndef SMXD_USING_NAND_IF
    case SMM_SLOT:
        if (INTR_SM_ALLI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCISMRDYSema));
            LOG(7, "SMRI unlock.\n");
        }
        break;
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

    default:
        break;
    }
}

#if 0
#ifdef CC_CARD_DETECT_PIN
//-------------------------------------------------------------------------
/** _Fcihw_xDCardDetect
 *  xD card detect GPIO call back function.
 *  @param  i4Gpio	card detect GPIO function.
 *  @param  fgStatus  1 = GPIO high, 0 = GPIO low.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_CardDetect(INT32 i4Gpio, BOOL fgStatus)
{
    UINT32 u4Status;

    UNUSED(fgStatus);
    ASSERT(_u4FCICardDetectPin == i4Gpio);

    u4Status = _Fcihw_GetCardStatus();

    // handle card detection.
    _Fcihw_DetectCard(u4Status);
}
#endif
#endif

//-------------------------------------------------------------------------
/** _Fcihw_HandleIsr
 *  FCI handle ISR function. This function will be call from DriverCommonHISR().
 *  @param  u2VctId  interrupt vector number = VECTOR_FLASHCARD.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_HandleIsr(UINT16 u2VctId)
{
    UINT32 u4Status;

    // check interrrupt vector number.
    ASSERT(u2VctId == FCI_INT_VECTOR_ID);

    // clear interrupt.
    VERIFY(BIM_ClearIrq(FCI_INT_VECTOR_ID));

    // 7 segment display for debug.
    //BIM_WRITE32(REG_SEG7, (UINT16)_u2FciIsrCnt++);

    u4Status = FCI_READ32(RW_FCI_INTR_STAT_REG);

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    u4Status &= FCI_READ32(RW_FCI_INTR_CTRL_REG);
#else
    u4Status &= (FCI_READ32(RW_FCI_INTR_CTRL_REG) & (~_u4ProceedIntrs));
    _u4ProceedIntrs = u4Status;
#endif

    LOG(7, "***** FCI HW Isr status is 0x%08x state 0x%08x*****\n", u4Status);

#if 0
#ifdef CC_CARD_DETECT_PIN
    // handle card detection.
    _Fcihw_DetectCard(u4Status);
#endif
#endif

#ifdef CC_CARD_DETECT_PIN
    // handle interrupt status only in card inserted state.
    if (!_fgFCICardRemove)
#endif
    {
        _Fcihw_HandleIntr(u4Status);
    }
}

//-------------------------------------------------------------------------
/** _Fcihw_ResetInt
 *  Reset FCI interrupt.
 *  @param  VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_ResetInt(VOID)
{
    UINT32 u4Val;

    CRIT_STATE_T crit;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    ASSERT((u4Val & CTRL_FCI_INTRST) == 0);

    /* It's a bug that interrupt might come at this time.
     * Why does it happen? fix me! XXX
     * Let interrupt cannot come at here tentatively. */
    crit = x_crit_start();
    
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_FCI_INTRST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
    
#ifndef FCIHW_INTERRUPT_WRITE_CLEAR
    // Clear Processed Interrupt status
    _u4ProceedIntrs = 0;
#endif

    x_crit_end(crit);
}

//-------------------------------------------------------------------------
/** _Fcihw_CheckFifo
 *  Check and wait FIFO back to default status.
 *  @param  VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_CheckFifo(VOID)
{
    UINT32 u4Val;

    UINT32 u4Count = 0;

    do
    {
        u4Count++;

        if (u4Count > 1000)
        {
            LOG(1, "_Fcihw_CheckFifo timeout.\n");
            return;
        }

        u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
        u4Val &= INTR_FIFO_MASK;
    } while (u4Val != (INTR_FCI_WRFIFO_EMPTY | INTR_FCI_RDFIFO_EMPTY));
}

//-------------------------------------------------------------------------
/** _Fcihw_ResetFifo
 *  Reset FCI SRAM and FIFO status.
 *  @param  VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_ResetFifo(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    ASSERT((u4Val & CTRL_FCI_FIFORST) == 0);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_FCI_FIFORST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** _Fcihw_ResetDramClk
 *  Reset FCI Dram Clock Domain
 *  @param  VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_ResetDramClk(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    ASSERT((u4Val & CTRL_DRAM_CLKRST) == 0);

    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_DRAM_CLKRST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** _Fcihw_ResetSlot
 *  Reset and clear interrupt FCI specific interface.
 *  @param  u4SlotIntr   specific card interrupt source.
 *  @param  u4SlotRest   specific card reset bit.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_ResetSlot(UINT32 u4SlotIntr, UINT32 u4SlotRest)
{
    UINT32 u4Val;

#ifdef FCI_SDMS_S_RST_INT
    UINT32 i4Ret;
#else
    UINT32 u4Loop;
#endif

    // Disable Slot interrupt.
    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    u4Val &= ~(u4SlotIntr);
    u4Val &= ~(INTR_SRAMSELI);
    u4Val |= FCI_SRAMSEL(_u4SRAMSel);
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);

    // Reset slot.
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    ASSERT((u4Val&(u4SlotRest)) == 0);

#ifdef FCI_SDMS_S_RST_INT
    // Clear the interrupt before enable it
    FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SDMS_SRST_INT);
    // Make sure S_RST interrupt enabled
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, FCI_READ32(RW_FCI_INTR_CTRL_REG) | INTR_SDMS_SRST_INT);
#endif

#ifndef FCI_SDMS_S_RST_INT
    for(u4Loop = 0; u4Loop < _u4FciHwRstWtTimes; u4Loop++)
#endif      
    {
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | u4SlotRest | CTRL_FCI_FIFORST);    
    }
    
#ifdef FCI_SDMS_S_RST_INT
    // Wait FCI hw receive reset request
    if (BIM_IsIrqEnabled(FCI_INT_VECTOR_ID))
    {
        i4Ret = x_sema_lock(_hFCISRSTSema, X_SEMA_OPTION_WAIT);
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
    }
    else
    {
        while(!(FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_SDMS_SRST_INT));
        // Clear the interrupt
        FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SDMS_SRST_INT);
    }
#endif
    
#ifndef FCI_SDMS_S_RST_INT
    for(u4Loop = 0; u4Loop < (4*_u4FciHwRstWtTimes); u4Loop++)
#endif      
    {
      FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_FCI_FIFORST);
    }

#ifdef FCI_SDMS_S_RST_INT
    // Wait FCI hw perform reset operation
    if (BIM_IsIrqEnabled(FCI_INT_VECTOR_ID))
    {
        i4Ret = x_sema_lock(_hFCISRSTSema, X_SEMA_OPTION_WAIT);
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
    }
    else
    {
        while(!(FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_SDMS_SRST_INT));
        // Clear the interrupt
        FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_SDMS_SRST_INT);
    }    
#endif
    
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
    
    // Reset FCI FIFO.
    //_Fcihw_ResetFifo();
    
    // Reset FCI Dram domain.
    _Fcihw_ResetDramClk();
    
    // Reset FCI interrupt.
    _Fcihw_ResetInt();

    // Write Clear all interrupt
    FCI_WRITE32(RW_FCI_INTR_STAT_REG, FCI_READ32(RW_FCI_INTR_STAT_REG));
}

//-------------------------------------------------------------------------
/** _Fcihw_ChangeSettings
 *  Modify settings according to current FCI clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_ChangeSettings(INT32 i4ClkMHz)
{
    UNUSED(_u4FciWaitIntTime);
    UNUSED(_u4FciWaitSitTime);
    
    if (i4ClkMHz > 25)
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 1;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME;
    }
    else if (i4ClkMHz >= 18)
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 3;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME * 2;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME * 2;
    }
    else if (i4ClkMHz >= 13)
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 4;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME * 4;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME * 4;
    }
    else if (i4ClkMHz >= 6)
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 8;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME * 5;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME * 5;
    }
    else if (i4ClkMHz >= 3)
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 16;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME * 7;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME * 7;
    }
    else if (i4ClkMHz >= 1)
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 32;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME * 8;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME * 8;
    }
    else
    {
#ifndef FCI_SDMS_S_RST_INT
        _u4FciHwRstWtTimes = 64;
#endif
        _u4FciWaitIntTime = FCI_WAIT_INT_TIME * 10;
        _u4FciWaitSitTime = FCI_WAIT_SIT_TIME * 10;
    }
}

//-------------------------------------------------------------------------
/** _Fcihw_SetupSlot
 *  Setup FCI specific interface and enable interrupt.
 *  @param  u4SlotIntr   specific card interrupt source.
 *  @param  u4SelSlot   FCI controller selection.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Fcihw_SetupSlot(UINT32 u4SlotIntr, UINT32 u4SelSlot)
{
    UINT32 u4Val;
    SDC_T *prSDC = FCI_GetSDCSlot();
    
    // write Slot interrupt enable bit.
    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    u4Val &= ~(INTR_FCISELI | INTR_FCI_ALLI | u4SlotIntr);

    if (_fgFCIIsrEn)
    {
        u4Val |= u4SlotIntr | u4SelSlot | INTR_FCI_ALLI;
    }
    else
    {
        u4Val |= u4SelSlot;
    }

    if (prSDC->fgHighSpeedMode)
    {
        if ((prSDC->fgFifoFullModeTest))    // Only for test
        {
            if (prSDC->fgHighSpeedTestBit)
            {
                u4Val |= INTR_FCI_HIGH_SPEED_MODE;
            }
            else
            {
                u4Val &= ~(INTR_FCI_HIGH_SPEED_MODE);
            }
        }
        else
        {
            // SD High speed mode setting
            u4Val |= INTR_FCI_HIGH_SPEED_MODE;
        }
    }
    else
    {
        // SD Default speed mode setting
        u4Val &= ~(INTR_FCI_HIGH_SPEED_MODE);
    }

  
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** _Fcihw_WaitRxBufNotEmpty
 *  Check Read Buffer Empty flag (bit 19 of 0x10 register)
 *  @param  VOID
 *  @retval   S_OK       Buffer is empty.
 *  @retval   E_FAIL     Buffer is not empty.
 */
//-------------------------------------------------------------------------
static INT32 _Fcihw_WaitRxBufNotEmpty(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);

    if (u4Val & INTR_FCI_RDFIFO_EMPTY)
    {
        return S_OK;
    }

    return E_FAIL;
}

//-------------------------------------------------------------------------
/** _Sdchw_WaitWBFBit
 *  Check if the end of write busy status on data line.
 *  @param  VOID.
 *  @retval   S_OK       Success.
 *  @retval   Others      Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdchw_WaitWBFBit(VOID)
{
    UINT32 u4Val;

#ifdef CC_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
  #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 can read the status
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
  #else
    u4Val = _u4FCISStatus;
  #endif
#else
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
#endif

    // error: time out.
    if (u4Val&(INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI))
    {
        LOG(1, "%s(%d) %s Wait TO IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_TIMEOUT;
    }
    // error: CRC error.
    else if (u4Val&(INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_CMDCRCI))
    {
        LOG(1, "%s(%d) %s Wait CRC IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_RSP_CRC_ERR;
    }
    // error: FLASH programming error.
    else if (u4Val&(INTR_SD_FPEI))
    {
        LOG(1, "%s(%d) %s Wait FPE IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_RSP_CRC_ERR;
    }
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 INTR_SD_DTRDYI will be saved in Interrupt routine
    else if (_u4FCISStatus & INTR_SD_DTRDYI)
#else
    else if (u4Val & INTR_SD_NWRITEBUSY)
#endif
    {
        LOG(7, "%s(%d) %s IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__,
        FCI_READ32(RW_FCI_INTR_STAT_REG));
        return S_OK;
    }

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    LOG(1, "Wait Idle is timeout. IntrStat:0x%08x, 0x%08x\n", u4Val, _u4FCISStatus);
#else
    LOG(1, "Wait Idle is timeout. IntrStat:0x%08x\n", u4Val);
#endif

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** _Sdchw_WaitDataRdyBit
 *  Check if data line ready.
 *  @param  VOID.
 *  @retval   S_OK       Success.
 *  @retval   Others      Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdchw_WaitDataRdyBit(VOID)
{
    UINT32 u4Val;

#ifdef CC_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
  #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 can read the status
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
  #else
    u4Val = _u4FCISStatus;
  #endif
#else
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
#endif

    // error: time out.
    if (u4Val&(INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI))
    {
        LOG(1, "%s(%d) %s Wait TO IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_TIMEOUT;
    }
    // error: CRC error.
    else if (u4Val&(INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_CMDCRCI))
    {
        LOG(1, "%s(%d) %s Wait CRC IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_RSP_CRC_ERR;
    }
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 INTR_SD_DTRDYI will be saved in Interrupt routine
    else if (_u4FCISStatus & INTR_SD_DTRDYI)
#else
    else if (u4Val & INTR_SD_DTRDYI)
#endif
    {
        LOG(7, "%s(%d) %s ok IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return S_OK;
    }

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    LOG(1, "Wait DataRdy is timeout. IntrStat:0x%08x, 0x%08x\n", u4Val, _u4FCISStatus);
#else
    LOG(1, "Wait DataRdy is timeout. IntrStat:0x%08x\n", u4Val);
#endif

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** _Sdchw_WaitCmdRdyBit
 *  Check if command line ready.
 *  @param  VOID.
 *  @retval   S_OK       Success.
 *  @retval   Others      Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdchw_WaitCmdRdyBit(VOID)
{
    UINT32 u4Val;

#ifdef CC_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
  #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 can read the status
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
  #else
    u4Val = _u4FCISStatus;
  #endif
#else
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
#endif

    // error: time out.
    if (u4Val&(INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI))
    {
        LOG(1, "%s(%d) %s Wait TO IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_TIMEOUT;
    }
    // error: CRC error.
    else if (u4Val&(INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_CMDCRCI))
    {
        LOG(1, "%s(%d) %s Wait CRC IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_RSP_CRC_ERR;
    }
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 INTR_SD_DTRDYI will be saved in Interrupt routine
    else if (_u4FCISStatus & INTR_SD_CMDRDYI)
#else
    else if (u4Val & INTR_SD_CMDRDYI)
#endif
    {
        LOG(7, "%s(%d) %s ok IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return S_OK;
    }

    LOG(1, "Wait CmdRdy is timeout, u4Val = 0x%x\n", u4Val);

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** _Sdchw_WaitCmdRdyOrTo
 *  Wait command line ready. Use in commad write operation.
 *  @param VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Sdchw_WaitCmdRdyOrTo(VOID)
{
#ifdef FCIHW_POLLING
    UINT32 u4Time;
#endif

    INT32 i4Ret;

    if (_fgFCIIsrEn)
    {
         // handle interrupt status only in card inserted state.
#ifdef CC_CARD_DETECT_PIN
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        i4Ret = x_sema_lock_timeout(_hFCISDCRSema, _u4FciWaitIntTime);

        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "_hFCISDCRSema lock timeout <= Failed @@\n");
        }

        i4Ret = _Sdchw_WaitCmdRdyBit();

        if (i4Ret == E_NO_RESPONSE)
        {
            LOG(1, "Wait CmdRdy Err of no response IntrStat:0x%08x <= Failed @@\n",
            FCI_READ32(RW_FCI_INTR_STAT_REG));
        }
        return i4Ret;
    }

#ifdef FCIHW_POLLING
    u4Time = _Fcihw_GetSec();

    while ((_Fcihw_GetSec() - u4Time) < FCI_ISR_TIMEOUT_PERIOD)
    {
        i4Ret = _Sdchw_WaitCmdRdyBit();
        if (i4Ret != E_NO_RESPONSE)
        {
            return i4Ret;
        }
    }
#endif

    return E_NO_RESPONSE;
}
/*
 * MSPHW static functions.
 */

//-------------------------------------------------------------------------
/** _Msphw_CheckBit
 *  Check MS card interrupt status bit.
 *  @param u4StatusBit      interrupt status mask.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Msphw_CheckBit(UINT32 u4StatusBit)
{
    UINT32 u4Val;

#ifdef CC_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
  #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 can read the status
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
  #else
    u4Val = _u4FCISStatus;
  #endif
#else
    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
#endif

    if (u4Val & INTR_MS_TOEI)
    {
        LOG(1, "WaitBit:0x%08x Err of Timeout IntrStat:0x%08x\n", u4StatusBit, u4Val);
        return E_CMD_TIMEOUT;
    }
    else if (u4Val & INTR_MS_CRCEI)
    {
        LOG(1, "WaitBit:0x%08x Err of CRC IntrStat:0x%08x\n", u4StatusBit, u4Val);
        return E_CMD_RSP_CRC_ERR;
    }
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 read status bit in interrupt routine
    else if (_u4FCISStatus & u4StatusBit)
#else
    else if (u4Val & u4StatusBit)
#endif
    {
        LOG(9, "WaitBit:0x%08x OK IntrStat:0x%08x\n", u4StatusBit, u4Val);
        return S_OK;
    }

    LOG(10, "WaitBit:0x%08x Err of no response IntrStat:0x%08x\n", u4StatusBit, u4Val);

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** _Msphw_OpenSitInt
 *  open MS SIT Interrupt.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Msphw_OpenSitInt(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
    u4Val |= (INTR_MS_SITI | INTR_MS_RDYI);
#else
    u4Val |= INTR_MS_SITI;
#endif

    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** _Msphw_OpenRdyInt
 *  open MS RDY Interrupt.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Msphw_OpenRdyInt(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    u4Val |= INTR_MS_RDYI;
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** _Msphw_CloseInt
 *  close MS command and data read/write related interrupt.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static VOID _Msphw_CloseInt(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    // clear all MS interrupt.
    u4Val &= (~(INTR_MS_RDYI | INTR_MS_SITI));
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
    
#ifndef FCIHW_INTERRUPT_WRITE_CLEAR
    // Ensure all interrup is cleared
    FCI_WRITE32(RW_FCI_INTR_STAT_REG, FCI_READ32(RW_FCI_INTR_STAT_REG));
#endif
}

//-------------------------------------------------------------------------
/** _Msphw_InitSemaphore
 *  Make sure all MSsemaphore is in lock state.
 *  @param  VOID
 *  @retval   VOID.
 */
//-------------------------------------------------------------------------
static VOID _Msphw_InitSemaphore(VOID)
{
    INT32 i4Ret = 0;

    UNUSED(i4Ret);

    i4Ret = x_sema_lock(_hFCIMSRDYSema, X_SEMA_OPTION_NOWAIT);
    //ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    if (!((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK)))
    {
        ASSERT(0);
    }

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
    i4Ret = x_sema_lock(_hFCIMSSITSema, X_SEMA_OPTION_NOWAIT);
    //ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    if (!((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK)))
    {
        ASSERT(0);
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    // Disable the interrupt
    VERIFY(BIM_DisableIrq(FCI_INT_VECTOR_ID));

    // Clear FCI Status variable
    _u4FCISStatus = 0x0;

    // Enable the interrupt
    VERIFY(BIM_EnableIrq(FCI_INT_VECTOR_ID));
#endif
}

#ifdef CC_FCI_SMXD_SUPPORT

//-------------------------------------------------------------------------
/** _Smxdhw_WaitDataRdyBit
 *  Check if SM/xD data ready.
 *  @param  VOID.
 *  @retval   S_OK       Success.
 *  @retval   Others      Fail.
 */
//-------------------------------------------------------------------------
#ifndef SMXD_USING_NAND_IF
static INT32 _Smxdhw_WaitDataRdyBit(VOID)
{
    UINT32 u4Val;

     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        return E_NO_CARD;
    }

    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);

    if (u4Val & INTR_SM_TOERR)
    {
        LOG(1, "%s(%d) %s TOERR IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_CMD_TIMEOUT;
    }
    else if (u4Val & INTR_SM_NONBUSY)
    {
        LOG(7, "%s(%d) %s card BUSY IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return E_NO_RESPONSE;
    }
    else if (u4Val & INTR_SM_TRRDY)
    {
        LOG(7, "%s(%d) %s TRRDY IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return S_OK;
    }

    LOG(7, "%s(%d) %s _Smxdhw_WaitDataRdyBit ERROR IntrStat:0x%08x\n", __FILE__, __LINE__,
    __FUNCTION__, u4Val);

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** _Smxdhw_WaitDataRdyOrTo
 *  Wait SM/xD data ready.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static INT32 _Smxdhw_WaitDataRdyOrTo(VOID)
{
#ifdef FCIHW_POLLING
    UINT32 u4New, u4Time;
#endif

    INT32 i4Ret;

    if (_fgFCIIsrEn)
    {
         // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            return E_NO_CARD;
        }

        i4Ret = x_sema_lock_timeout(_hFCISMRDYSema, _u4FciWaitIntTime);

        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "_hFCISMRDYSema lock timeout\n");
        }

        i4Ret = _Smxdhw_WaitDataRdyBit();

        if (i4Ret == E_NO_RESPONSE)
        {
            LOG(9, "Wait DataRdy Err of no response IntrStat:0x%08x.\n",
            FCI_READ32(RW_FCI_INTR_STAT_REG));
        }
        return i4Ret;
    }

#ifdef FCIHW_POLLING
    u4New = (u4Time = _Fcihw_GetSec());

    while ((u4New - u4Time) < FCI_ISR_TIMEOUT_PERIOD)
    {
        i4Ret = _Smxdhw_WaitDataRdyBit();

        if (i4Ret != E_NO_RESPONSE)
        {
            return i4Ret;
        }
        u4New = _Fcihw_GetSec();
    }
#endif

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** _Smxdhw_InitSemaphore
 *  Make sure all SM semaphore is in lock state.
 *  @param  VOID
 *  @retval   VOID.
 */
//-------------------------------------------------------------------------
static VOID _Smxdhw_InitSemaphore(VOID)
{
    INT32 i4Ret;

    i4Ret = x_sema_lock(_hFCISMRDYSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    UNUSED(i4Ret);
}
#endif  // #ifndef SMXD_USING_NAND_IF

#endif  // #ifndef CC_FCI_SMXD_SUPPORT

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#ifndef NDEBUG

//-------------------------------------------------------------------------
/** FCIHW_IoWr
 *  FCI register IO write function including debug information logging.
 *  @param  u4Offset   offset from FCI base address.
 *  @param  u4Value   register value.
 *  @param  szFile   file name string.
 *  @param  i4Line     file line number.
 *  @return  VOID
 */
//-------------------------------------------------------------------------

VOID FCIHW_IoWr(UINT32 u4Offset, UINT32 u4Value, INT8 *szFile, INT32 i4Line)
{
    LOG(_W, "%s(%d) write 0x%08x with val 0x%08x\n", szFile, i4Line, u4Offset, u4Value);

    IO_WRITE32(FCI_IO_BASE_ADDR, u4Offset, u4Value);
}

//-------------------------------------------------------------------------
/** FCIHW_IoRd
 *  FCI register IO read function including debug information logging.
 *  @param  u4Offset   offset from FCI base address.
 *  @param  szFile       file name string.
 *  @param  i4Line         file line number.
 *  @return  u4Value    register value
 */
//-------------------------------------------------------------------------
UINT32 FCIHW_IoRd(UINT32 u4Offset, INT8 *szFile, INT32 i4Line)
{
    UINT32 u4Value;

    u4Value = IO_READ32(FCI_IO_BASE_ADDR, u4Offset);
    LOG(_R, "%s(%d) read 0x%08x getting val 0x%08x\n", szFile, i4Line, u4Offset, u4Value);
    return u4Value;
}

#if 0 // unused
//-------------------------------------------------------------------------
/** FCIHW_DumpReg
 *  FCI all register dump function including debug information logging.
 *  @param  szFile   file name string.
 *  @param  i4Line     file line number.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_DumpReg(INT8 *szFile, INT32 i4Line)
{
    INT32 i, i4OrgLog, i4Log;

    UINT32 *pu4Ptr;

    pu4Ptr = (UINT32 *)FCI_IO_BASE_ADDR;
    //pu4Ptr = (UINT32 *)FLASH_CARD_BASE;
    i4OrgLog = (i4Log = FCI_i4LogLvlQuerySet(NULL));
    i4Log = (INT32)((UINT32)i4Log & ~(LOG_HEADER_MASK));
    VERIFY(i4Log == FCI_i4LogLvlQuerySet(&i4Log));

    LOG(10, "%s(%d)------------------------------------", szFile, i4Line);

    for (i = 0; i < 24; i++)
    {
        if ((i %4) == 0)
        {
            LOG(10, "\n0x%08x | 0x%08x", (UINT32)pu4Ptr + (UINT32)(4 *i), pu4Ptr[i]);
        }
        else
        {
            LOG(10, " 0x%08x", pu4Ptr[i]);
        }
    }

    LOG(10, "\n-------------------------------------------\n");
    VERIFY(i4OrgLog == FCI_i4LogLvlQuerySet(&i4OrgLog));
}
#endif // #if 0 // unused

#endif /* NDEBUG */

//-------------------------------------------------------------------------
/** FCIHW_PowerOn
 *  Power on FCI module (DRAM interface)
 *  @param  void
 *  @return TRUE.
 */
//-------------------------------------------------------------------------

BOOL FCIHW_PowerOn(void)
{
    UINT32 u4Val;

    // turn on MS & SD control clock
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val & ~(CTRL_MS_PWS|CTRL_SD_PWS) );

    // turn on FCI DRAM clock
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_DRAM_CLKEN );

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)
    CKGEN_CLRBIT(0x0260, ((UINT32)1) << 7);   // MS clock => normal clock
    CKGEN_CLRBIT(0x0264, ((UINT32)1) << 7);   // SD clock => normal clock    
#endif

    return TRUE;
}

//-------------------------------------------------------------------------
/** FCIHW_PowerOff
 *  Power off FCI module (DRAM interface)
 *  @param  void
 *  @return TRUE.
 */
//-------------------------------------------------------------------------

BOOL FCIHW_PowerOff(void)
{
    UINT32 u4Val;

    // turn off MS & SD control clock
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_MS_PWS | CTRL_SD_PWS );

    // turn off FCI DRAM clock
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val & ~CTRL_DRAM_CLKEN );

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)
    CKGEN_SETBIT(0x0260, ((UINT32)1) << 7);   // MS clock => power down
    CKGEN_SETBIT(0x0264, ((UINT32)1) << 7);   // SD clock => power downs    
#endif

    return TRUE;
}

//-------------------------------------------------------------------------
/** FCIHW_SetSRAMSEL
 *  Setup SRAM Fifo Selection.
 *  NOTICE: This function will be called in FCI CLI
 *  @u4SRAMSel  0,1,2,3 : fifo selection
 *  @return  VOID
 */
//-------------------------------------------------------------------------

VOID FCIHW_SetSRAMSEL(UINT32 u4SRAMSel)
{
    _u4SRAMSel = u4SRAMSel;
}

//-------------------------------------------------------------------------
/** FCIHW_SetSDIOINTFct
 *  Register SDIO interrupt HISR function.
 *  NOTICE: This function will be called in FCI HISR.
 *  @param  pfnCallback   call back funtion pointer.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
#ifdef CC_FCI_SDIO_SUPPORT
VOID FCIHW_SetSDIOINTFct(FCI_CALLBACK pfnCallback)
{
    _pfnSDIOINT = pfnCallback;
}
#endif

//-------------------------------------------------------------------------
/** FCIHW_SetIsr
 *  Enable/disable FCI ISR handler.
 *  @param  fgSet   enable or disable FCI ISR handler.
 *  @retval S_OK        Success
 *  @retval E_FAIL      Fail.
 */
//-------------------------------------------------------------------------
INT32 FCIHW_SetIsr(BOOL fgSet)
{
    VOID (*pfnOldIsr)(UINT16);
    UINT32 u4Val;
#if 0    
#ifdef CC_CARD_DETECT_PIN
    UINT32 u4Status;
#endif
#endif

    // initialize ISR.
    if (fgSet)
    {
        // Clear the pending interrupt of a given vector.
        VERIFY(BIM_ClearIrq(FCI_INT_VECTOR_ID));

        if (!_fgFCIIsrEn)
        {
#ifdef FCI_CARD_DEBOUNCE_ENABLE
            VERIFY(OSR_OK == x_timer_create(&_hFCItimerHdl));
#endif

#ifdef CC_DTV_FCI
            /// FCI semaphore
            VERIFY(OSR_OK == x_sema_create(&_hFCIRDYSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_UNLOCK));
#endif

#ifdef FCI_SDMS_S_RST_INT
            // SD Software Reset semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISRSTSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif

            // SD data ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDDRSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

            // SD cmd ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDCRSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550 use INTR_SD_DTRDYI instead
            // SD write buffer finish semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDWBFSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
            // SD Response Busy interrupt semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDRBISema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif
#endif

            // MS ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCIMSRDYSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
            // MS SIT semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCIMSSITSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif

#ifndef SMXD_USING_NAND_IF
            // SM ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISMRDYSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif

#if 0
// MT5391 default insert SDIO card, don't need card detect pin
#ifdef CC_CARD_DETECT_PIN
            UNUSED(DRVCUST_OptQuery(eFCICardDetectGpio, &_u4FCICardDetectPin));

            VERIFY(0 == GPIO_Init());
            VERIFY(0 <= GPIO_Reg(_u4FCICardDetectPin, GPIO_TYPE_INTR_BOTH, _Fcihw_CardDetect));
#endif
#endif

            //enable ISR.
            VERIFY(OSR_OK == x_reg_isr(FCI_INT_VECTOR_ID, _Fcihw_HandleIsr, &pfnOldIsr));

            _fgFCIIsrEn = TRUE;
        }

        // set pin mux before query card detect pin.
        FCIHW_ResetSlot(NONE_SLOT);

        // turn on card detect interrupt.
        u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val |INTR_FCI_ALLI);

#ifdef CC_CARD_DETECT_PIN
#if 0
        // query card detect status.
        u4Status = _Fcihw_GetCardStatus();
#endif

        // card detection.
        _fgFCICardRemove = FCIHW_DetectCard();
#endif

        LOG(9, "%s(%d) Set Isr On(Enable)!!!\n", __FILE__, __LINE__);
    }
    else
    {
#ifdef FCIHW_POLLING
        if (_fgFCIIsrEn)
        {
#ifdef FCI_CARD_DEBOUNCE_ENABLE
            // delete timer.
            VERIFY(OSR_OK == x_timer_delete(_hFCItimerHdl));
#endif
            //disable ISR.
            VERIFY(OSR_OK == x_reg_isr(FCI_INT_VECTOR_ID, NULL, &pfnOldIsr));

#ifdef CC_DTV_FCI
            /// FCI semaphore
            VERIFY(OSR_OK == x_sema_delete(_hFCIRDYSema));
#endif
  
#ifdef FCI_SDMS_S_RST_INT
            VERIFY(OSR_OK == x_sema_delete(_hFCISRSTSema));
#endif
            // SD card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCISDDRSema));
            VERIFY(OSR_OK == x_sema_delete(_hFCISDCRSema));
#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550 use INTR_SD_DTRDYI instead
            VERIFY(OSR_OK == x_sema_delete(_hFCISDWBFSema));
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
            VERIFY(OSR_OK == x_sema_delete(_hFCISDRBISema));
#endif
#endif

            // MS card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCIMSRDYSema));

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
            // MS card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCIMSSITSema));
#endif

#ifdef CC_FCI_SMXD_SUPPORT
#ifndef SMXD_USING_NAND_IF
            // SM card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCISMRDYSema));
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

#ifdef CC_CARD_DETECT_PIN
            // clear interrupt status.
            _u4FCICDStatus = INTR_FCI_CDI;
#endif

            _fgFCIIsrEn = FALSE;
        }
#endif
        LOG(9, "%s(%d) Set Isr Off(Disable)!!!\n", __FILE__, __LINE__);
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** FCIHW_SetFifoRead
 *  Data transfer from memory card to FCI controller.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SetFifoRead(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val & ~(CTRL_FCI_FIFO_W_DIR));
}

//-------------------------------------------------------------------------
/** FCIHW_SetFifoWrite
 *  Data transfer from FCI controller to memory card.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SetFifoWrite(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_FCI_FIFO_W_DIR);
}

//-------------------------------------------------------------------------
/** FCIHW_ResetSlot
 *  reset specific card slot. call this function when card swap.
 *  @param  i4Slot  slot number.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_ResetSlot(INT32 i4Slot)
{
    UINT32 u4Val;

    SDC_T *prSDC = FCI_GetSDCSlot();
    MSP_T *prMSP = FCI_GetMSPSlot();

    _i4FCIPrevSlot = i4Slot;

    // It must to disable interrupt before register setting.
    switch (i4Slot)
    {
    case NONE_SLOT:
        break;

    case SDC_SLOT:        
        // disable interrupt.
        if (prSDC->u4CardType == FCI_SDIO_CARD)
        {
            _Fcihw_ResetSlot(INTR_SD_ALLI |INTR_SDIO_INT, CTRL_SD_RST);
        }
        else
        {
            _Fcihw_ResetSlot(INTR_SD_ALLI, CTRL_SD_RST);
        }
        
        // Set SD_NWP_BF 14[23:20] = 0 => check if SD DWT time out can be detected 
        // Set SD_NACBCNT 14[19:0] = 1 => check if SD DRT time out can be detected
        if (prSDC->fgDataTimeOutTest)
        {
            FCI_WRITE32(RW_SD_BUSYCNT_REG,
                ((UINT32) 0x000001 << 0) | ((UINT32) 0x00 << 20) | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);
        }
        // Set SD_NCRBCNT 14[29:24] = 2 => check if SD CT time out can be detected 
        else if (prSDC->fgCmdTimeOutTest)
        {
            FCI_WRITE32(RW_SD_BUSYCNT_REG,
                MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | ((UINT32) 0x02 << 24) |CTRL_FCI_SDWC);
        }
        else
        {
            FCI_WRITE32(RW_SD_BUSYCNT_REG,
                MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);
        }        
        
        u4Val = FCI_READ32(RW_FCI_CTRL_REG);
        // Clear MS, SM part.
        u4Val &= 0xFF0000FF;
        
        // SD host must change to latch data at rising edge 0x08[25] = 0
        if (prSDC->fgHighSpeedMode)
        {
            if ((prSDC->fgFifoFullModeTest))    // Only for test
            {
                if (prSDC->fgHighSpeedTestBit)
                {
                    u4Val |= CTRL_SD_MLBLCNEN | CTRL_SD_NEGDRIVE | CTRL_SD_DATASWAP | CTRL_SD_SIEN | CTRL_SD_HS_ADJ;
                    u4Val &= ~(CTRL_SD_RECEDGE | CTRL_SD_WIDEBUSEN);
                }
                else
                {
                    u4Val |= CTRL_SD_MLBLCNEN | CTRL_SD_NEGDRIVE | CTRL_SD_DATASWAP | CTRL_SD_SIEN;
                    u4Val &= ~(CTRL_SD_RECEDGE | CTRL_SD_WIDEBUSEN | CTRL_SD_HS_ADJ);
                }
            }
            else
            {
                // SD High speed mode setting
                u4Val |= CTRL_SD_MLBLCNEN | CTRL_SD_NEGDRIVE | CTRL_SD_DATASWAP | CTRL_SD_SIEN | CTRL_SD_HS_ADJ;
                u4Val &= ~(CTRL_SD_RECEDGE | CTRL_SD_WIDEBUSEN);
            }
        }
        else
        {
            // SD Default speed mode setting
            u4Val |= CTRL_SD_MLBLCNEN | CTRL_SD_NEGDRIVE | CTRL_SD_DATASWAP | CTRL_SD_SIEN |CTRL_SD_RECEDGE;
            u4Val &= ~(CTRL_SD_WIDEBUSEN | CTRL_SD_HS_ADJ);
        }

        prSDC = FCI_GetSDCSlot();
        if (prSDC->fg4BitEn)
        {
            u4Val |= CTRL_SD_WIDEBUSEN;
        }
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);

        if (prSDC->u4CardType == FCI_SDIO_CARD)
        {
            // turn on interrupt.
            _Fcihw_SetupSlot(INTR_SD_ALLI |INTR_SDIO_INT, FCI_SEL_SD);
        }
        else
        {
            // turn on interrupt.
            _Fcihw_SetupSlot(INTR_SD_ALLI, FCI_SEL_SD);
        }
        break;

    case MSP_SLOT:
        // disable interrupt.
        _Fcihw_ResetSlot(INTR_MS_ALLI, CTRL_MS_RST);

        u4Val = FCI_READ32(RW_FCI_CTRL_REG);
        // Clear SD, SM part.
        u4Val &= 0x00FF00FF;
        u4Val &= ~CTRL_MS_BSYCNT;
        u4Val |= CTRL_MS_SIEN | FCI_MS_BSYCNT(0);
        u4Val &= ~CTRL_MS_WIDEBUSEN;

        prMSP = FCI_GetMSPSlot();
        if (prMSP->fg4BitEn)
        {
            u4Val |= CTRL_MS_WIDEBUSEN;

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)            
            if (_u4MSMultiFuncCfg == 3)
            {
                FCI_WRITE32(RW_MS_AUTO_CTRL_REG, CTRL_MS_RED | CTRL_MS_D5 | CTRL_MS_ND);
            }
            else
#endif            
            {            
                FCI_WRITE32(RW_MS_AUTO_CTRL_REG, CTRL_MS_RED | CTRL_MS_D5/*| CTRL_MS_ND*/);
            }

            // MT5360 : FCI_WRITE32(RW_MS_AUTO_CTRL_REG, MS_APID(7) | MS_ADS(1) | CTRL_MS_RED | CTRL_MS_D5 | CTRL_MS_ND);

            /* // For 5362A/B
            // MT5360 Add : CKGEN 0x2000d410 [0]
            u4Tmp = CKGEN_READ32(REG_PINMUX_MISC); // MISC regiter
            u4Tmp &= ~(1U<<0);
            u4Tmp |= (0U<<0);   // 4 bit mode
            CKGEN_WRITE32(REG_PINMUX_MISC, u4Tmp);
            */
        }
        else
        {
            FCI_WRITE32(RW_MS_AUTO_CTRL_REG, CTRL_MS_RED);

            /* // For 5362A/B
            // MT5360 Add : CKGEN 0x2000d410 [0]
            u4Tmp = CKGEN_READ32(REG_PINMUX_MISC); // MISC regiter
            u4Tmp &= ~(1U<<0);
            u4Tmp |= (1U<<0);   // 1 bit mode
            CKGEN_WRITE32(REG_PINMUX_MISC, u4Tmp);
            */
        }
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);

        // turn on interrupt.
        _Fcihw_SetupSlot(INTR_MS_ALLI, FCI_SEL_MS);
        break;

#ifdef CC_FCI_SMXD_SUPPORT
#ifndef SMXD_USING_NAND_IF
    case SMM_SLOT:
        // disable interrupt.
        _Fcihw_ResetSlot(INTR_SM_ALLI, CTRL_SM_RST);

        FCI_WRITE32(RW_SD_BUSYCNT_REG,
            MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);

        // Clear and reset ctrl register.
        u4Val = CTRL_SM_SIEN | CTRL_SM_DATASWAP | CTRL_DRAM_CLKEN;
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);

        // turn on interrupt.
        _Fcihw_SetupSlot(INTR_SM_ALLI, FCI_SEL_SM);
        break;
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

    default:
        break;
    }
}

#ifdef CC_FCI_POWER_CONTROL
//-------------------------------------------------------------------------
/** FCIHW_PowerSwitch
 *  Initailize all FCI card.
 *  @param  fgPowerOn TRUE : power on, FALSE : power off
 *  @retval   none
 */
//-------------------------------------------------------------------------
VOID FCIHW_PowerSwitch(BOOL fgPowerOn, UINT32 u4Delayms)
{
    UNUSED(fgPowerOn);

    LOG(5, "Switch FCI Power : %s\n", fgPowerOn ? "On" : "Off");

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)    // MT8530 use VIND7 as Power controller Pin
    
    CKGEN_CLRBIT(0xC0, ((UINT32)0x03 << 10));     //  D0C0h[11:10] = 00  (PAD_CFG_0[11:10])
    CKGEN_CLRBIT(0xCC, ((UINT32)0x01 << 0));     //  D0CCh[0] = 0  (PAD_CFG_3[0])
    CKGEN_CLRBIT(0xCC, ((UINT32)0x01 << 28));     //  D0CCh[28] = 0  (PAD_CFG_3[23])
    CKGEN_CLRBIT(0xD4, ((UINT32)0x01 << 0));    //  D0D4h[0] = 0  (PAD_CFG_5[0])：
    
    // 設定I/O方向（0：input、1：output）
    CKGEN_SETBIT(0x18C, ((UINT32)0x01 << 10));    // enable：D18Ch[10]  (controll I/O direction, GPIO_EN3[10])

    if(fgPowerOn)    // configure GPIO low to turn on FCI power
    {    
        CKGEN_CLRBIT(0x01AC, ((UINT32)0x01 << 10));    // output：set D1ACh[10]  (GPIO_OUT3[10])
        
        // 設定I/O方向（0：input、1：output）
        CKGEN_CLRBIT(0x18C, ((UINT32)0x01 << 10));    // enable：D18Ch[10]  (controll I/O direction, GPIO_EN3[10])
    }
    else                   // configure GPIO high to turn off FCI power
    {
        CKGEN_SETBIT(0x01AC, ((UINT32)0x01 << 10));    // output：set D1ACh[10]  (GPIO_OUT3[10])
    }

    // Apply delay time
    x_thread_delay(u4Delayms);
    
#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)

    // To be Add
    // Apply delay time
    x_thread_delay(u4Delayms);

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)

    // To be Add
    // Apply delay time
    x_thread_delay(u4Delayms);

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)

    // MT8530 use VIND7 as Power controller Pin
    //以下CKGEN 的 register 都 program成 0，VIND7才可做 GPIO 用：
    CKGEN_CLRBIT(0xC0, ((UINT32)0x01 << 26));    // 00C0[26]
    CKGEN_CLRBIT(0xC8, ((UINT32)0xFF << 24) | ((UINT32)0x3F << 16));    // 00C8[31:24], 00C8[22:16]
    CKGEN_CLRBIT(0xCC, ((UINT32)0x0F << 0));    // 00CC[3:0]
    CKGEN_CLRBIT(0xD0, ((UINT32)0x3F << 22));    // 00D0[28:22]
    CKGEN_CLRBIT(0xD4, ((UINT32)0x01 << 3));    // 00D4[3]
    
    // 設定I/O方向（0：input、1：output）
    CKGEN_SETBIT(0x188, ((UINT32)0x01 << 4));    // 0188[4]

    if(fgPowerOn)    // configure GPIO low to turn on FCI power
    {    
        CKGEN_CLRBIT(0x01A8, ((UINT32)0x01 << 4));    // 01A8[4] = Low
        
        // 設定I/O方向（0：input、1：output）: set GPIO as GPI for card detection
        CKGEN_CLRBIT(0x188, ((UINT32)0x01 << 4));    // 0188[4]
    }
    else                   // configure GPIO high to turn off FCI power
    {
        CKGEN_SETBIT(0x01A8, ((UINT32)0x01 << 4));    // 01A8[4] = High    
    }

    // Apply delay time
    x_thread_delay(u4Delayms);
        
#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)

    if (_u4SDMultiFuncCfg == 2)   // Multi-function use GPIO43 as fixed card detection PIN, and it can be read by FCI register
    {    
        UINT32 u4MultiFunc;

        // Setup GPIO high or log
        if (fgPowerOn)    // Set low to power on
        {
            CKGEN_CLRBIT(REG_GPIO_OUT2, (((UINT32)0x01) << 30));
        }
        else                    // Set high to power on
        {
            CKGEN_SETBIT(REG_GPIO_OUT2, (((UINT32)0x01) << 30));
        }

        // Setup GPIO output mode
        CKGEN_SETBIT(REG_GPIO_EN2, (((UINT32)0x01) << 30));
            
        // Backup Multi-function setting
        u4MultiFunc = CKGEN_READ32(REG_PINMUX_SEL1);
        
        // Enable GPIO 43 as GPIO function    
        CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22));
         
        // Apply delay time
        x_thread_delay(u4Delayms);
    
        // Restore GPIO 43 back to FCI / SD or MS
        CKGEN_SETBIT(REG_PINMUX_SEL1, (u4MultiFunc & (((UINT32)0x03) << 22)));
    }
    else                                     // Multi-function 1, 3 must use other GPIO as card detection pin
    {
        // Apply delay time
        x_thread_delay(u4Delayms);
    }
#endif    
}
#endif

//-------------------------------------------------------------------------
/** FCIHW_HWConfigSetup
 *  Setup multi-func, driving current, pull up resistor ... HW setting.
 *  @param  i4Slot  slot number.
 *  @return  VOID.
 */
//-------------------------------------------------------------------------
VOID FCIHW_HWConfigSetup(INT32 i4Slot)
{
    UINT32 u4Reg = 0;
    
    UNUSED(i4Slot);
    UNUSED(u4Reg);
    
#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)

    if (i4Slot == SDC_SLOT)
    {
#ifdef MT8520_USE_SD2    
        UINT32 u4MsPalSel, u4MsSerSel;
        
        // (1) Setup PAD Multi-function
        /*
        u4Reg = CKGEN_READ32(REG_RW_PAD_CFG_5);
        u4Reg &= ~(0x1 << 11);  // Disable NAND_SEL
        CKGEN_WRITE32(REG_RW_PAD_CFG_5, u4Reg);
        */

        // (2) Setup SD2 Multi-function
        u4Reg = CKGEN_READ32(REG_RW_PAD_CFG_1);
        u4Reg |= (1 << 13);  // Set SD2_SEL

        // (3) Setup MS multi-func : MS default use SD2 PAD as output pad, so we need
        // change MS output pad here. (don't select CFG 0, 1)
        u4MsPalSel = (u4Reg >> 18) & 0x0F;

        if (u4MsPalSel == 0 || u4MsPalSel == 1)
        {
            u4Reg &= ~(((UINT32)0x0F) << 18);
            u4Reg |= (4 << 18);  // Set MSSER_PAL = 4
        }

        u4MsSerSel = (u4Reg >> 14) & 0x0F;

        if (u4MsSerSel == 0 || u4MsSerSel == 1)
        {
            u4Reg &= ~(((UINT32)0x0F) << 14);
            u4Reg |= (4 << 14);  // Set MSSER_SEL = 4
        }

        CKGEN_WRITE32(REG_RW_PAD_CFG_1, u4Reg);

        // (4) Setup Driving current for SD 2 PADs
        u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_2);
        // u4Reg |= (0x0FFF);  // Set 8 mA for SD2CLK, CMD, D0, D1, D2, D3
        u4Reg |= (0x0FFE);  // Set 8 mA for CMD, D0, D1, D2, D3. 6mA for SD2CLK

        CKGEN_WRITE32(REG_RW_PAD_CTRL_2, u4Reg);
#else // SD1
        //================= SD configuration =================//

        // (1) Setup PAD Multi-function
        /*
        u4Reg = CKGEN_READ32(REG_RW_PAD_CFG_5);
        u4Reg &= ~(0x1 << 11);  // Disable NAND_SEL
        CKGEN_WRITE32(REG_RW_PAD_CFG_5, u4Reg);
        */
 
        // (2) Setup SD1 Multi-function
        u4Reg = CKGEN_READ32(REG_RW_PAD_CFG_1);
        u4Reg &= ~(0x3FC00000 | 0x003FC000);   // clear SD & MS multi-function setting
        u4Reg |= (_u4SDMultiFuncCfg << 22);  // Set SDPAL_SEL for SD1
        u4Reg |= (_u4SDMultiFuncCfg << 26);  // Set SDSER_SEL for SD1
        CKGEN_WRITE32(REG_RW_PAD_CFG_1, u4Reg);

        // (3) Setup Driving current & pull up resistor for SD 1 PADs
        switch(_u4SDMultiFuncCfg)
        {
            case 1: // multi-func 1
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_9);
                u4Reg |= (0x000FFF00);  // Set 8 mA for GPIO 5, 6, 7, 4, 8, 9
                CKGEN_WRITE32(REG_RW_PAD_CTRL_9, u4Reg);         
                break;
            }
            case 2: // multi-func 2
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_9);
                u4Reg |= (0xC0000000);  // Set 8 mA for GPIO 15
                CKGEN_WRITE32(REG_RW_PAD_CTRL_9, u4Reg);

                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_3);
                // u4Reg |= (0x00DB6C00);  // Set 8 mA for PMD3, PMA3, PMD4, PMD5, PMA5
                u4Reg |= (0x00006000);  // Set 8 mA for PMD3, PMA3, PMD4, PMD5, PMA5
                CKGEN_WRITE32(REG_RW_PAD_CTRL_3, u4Reg);
                break;
            }
            case 3: // multi-func 3
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_5);
                u4Reg |= (0x01BDB600);  // Set 8 mA for PMCE2N, PMRDN, PMRST, PMA6, PMBVD2, PMBVD1
                CKGEN_WRITE32(REG_RW_PAD_CTRL_5, u4Reg);
                break;
            }
            case 4: // multi-func 4
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_5);
                // Need stronger current to drive, or it will CRC error
                u4Reg |= (0x6C000000);  // Set 8 mA for PMA1, PMD0
                // u4Reg |= (0x00000000);  // Set 1 mA for PMA1, PMD0
                CKGEN_WRITE32(REG_RW_PAD_CTRL_5, u4Reg);

                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_6);
                // Need stronger current to drive, or it will CRC error
                u4Reg |= (0x000006DB);  // Set 8 mA for PMD1, PMA0, PMIPKN, PMALEL
                // u4Reg |= (0x00000000);  // Set 2 mA for PMD1, PMA0, PMIPKN, PMALEL
                CKGEN_WRITE32(REG_RW_PAD_CTRL_6, u4Reg);
                break;
            }
            case 5: // multi-func 5
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_6);
                // u4Reg |= (0x1B6DB000);  // Set 8 mA for PMCD2N, PMWP, PMALEH, PMD2, PMEN, PMA2
                u4Reg |= (0x00000000);  // Set 2 mA for PMCD2N, PMWP, PMALEH, PMD2, PMEN, PMA2
                CKGEN_WRITE32(REG_RW_PAD_CTRL_6, u4Reg);
                break;
            }
            case 6: // multi-func 6
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_9);
                // Jethro, 8 mA for MT8520 DEMO board is good
                u4Reg |= (0x00F000FF);  // Set 8 mA for GPIO11, GPIO1, GPIO0, GPIO10, GPIO2, GPIO3
                // Can't set 8ma => [FCI]project_x/target/mt8520/8520_driver/fci/fci_sdc.c(859) stop trans failed 11
                //                         [FCI]SDC Check status failed. u4Val:0x00800b00
                // u4Reg |= (0x00000000);  // Set 2 mA for GPIO11, GPIO1, GPIO0, GPIO10, GPIO2, GPIO3
                CKGEN_WRITE32(REG_RW_PAD_CTRL_9, u4Reg);
                break;
            }
            case 7: // multi-func 7
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_2);
                u4Reg |= (0xFFF00000);  // Set 8 mA for PAD_PHYD0, PAD_PHYC1, PAD_PHYC0, PAD_PHYD1, PHYCLK, LREQ
                CKGEN_WRITE32(REG_RW_PAD_CTRL_2, u4Reg);
                break;
            }
            case 8: // multi-func 8
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_7);
                // u4Reg |= (0x03FF000C);  // Set 8 mA for NFRBN, NFREN, NFCEN, NFD1, NFCLE, NFALE
                // 8mA will overshoot & undershoot too seriously
                u4Reg |= (0x00000000);  // Set 2 mA for NFRBN, NFREN, NFCEN, NFD1, NFCLE, NFALE
                CKGEN_WRITE32(REG_RW_PAD_CTRL_7, u4Reg);
                break;
            }
        }

        // (4) pull up resistor (remember to remove PCB R690 pull low resistor)
        u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_12);
        u4Reg |= (0x00000002);  // Pull up PMCE2N, PMRST, PMA6, PMBVD2, PMBVD1
        CKGEN_WRITE32(REG_RW_PAD_CTRL_12, u4Reg);
#endif  // #ifdef MT8520_USE_SD2    
    }
    else if (i4Slot == MSP_SLOT)
    {
       //================= MS configuration =================//

        // (1) Setup PAD Multi-function
        /*
        u4Reg = CKGEN_READ32(REG_RW_PAD_CFG_5);
        u4Reg &= ~(0x1 << 11);  // Disable NAND_SEL
        CKGEN_WRITE32(REG_RW_PAD_CFG_5, u4Reg);
        */

        // (2) Setup MS Multi-function
        u4Reg = CKGEN_READ32(REG_RW_PAD_CFG_1);
        u4Reg &= ~(0x3FC00000 | 0x003FC000);   // clear SD & MS multi-function setting

        if (_u4MSMultiFuncCfg == 9)    // Configure 9 => 8
        {
            u4Reg |= (8 << 14);  // Set MSSER_SEL for MS
        }
        else if (_u4MSMultiFuncCfg == 8)    // Configure 8 => 7
        {
            u4Reg |= (7 << 14);  // Set MSSER_SEL for MS
        }
        else
        {
            u4Reg |= (_u4MSMultiFuncCfg << 14);  // Set MSSER_SEL for MS
        }

        u4Reg |= (_u4MSMultiFuncCfg << 18);  // Set MSPAL_SEL for MS
        CKGEN_WRITE32(REG_RW_PAD_CFG_1, u4Reg);

        // (3) Setup Driving current & pull up resistor for MS PADs
        switch(_u4MSMultiFuncCfg)
        {
            case 1: // multi-func 1
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_2);
                u4Reg |= (0x00000FFF);  // Set 8 mA for SD2CLK, SD2CMD, SD2D0, SD2D1, SD2D2, SD2D3
                // The driving current can't set too large for this multi-func pad, or it the shape will too bad => CRC error
                // u4Reg |= (0x00000000);  // Set 2 mA for SD2CLK, SD2CMD, SD2D0, SD2D1, SD2D2, SD2D3
                // u4Reg |= (0x0000057F);  // Fine tuned value
                CKGEN_WRITE32(REG_RW_PAD_CTRL_2, u4Reg);
                break;
            }
            case 2: // multi-func 2
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_9);
                u4Reg |= (0x000FFF00);  // Set 8 mA for GPIO4, GPIO5, GPIO6, GPIO7, GPIO8, GPIO9
                CKGEN_WRITE32(REG_RW_PAD_CTRL_9, u4Reg);
                break;
            }
            case 3: // multi-func 3
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_9);
                // u4Reg |= (0xC0000000);  // Set 8 mA for GPIO15
                u4Reg |= (0x00000000);  // Set 2 mA for GPIO15
                CKGEN_WRITE32(REG_RW_PAD_CTRL_9, u4Reg);

                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_3);
                // Parallel Mode 36 Mhz use 8mA for D1,2,3 or it will CRC error
                // u4Reg |= (0x00DB6C00);  // Set 8 mA for PMD3, PMA3, PMD4, PMD5, PMA5
                // Serial Mode 20 Mhz use 2mA is better for D0,BS,CLK
                u4Reg |= (0x00000000);  // Set 2 mA for PMD3, PMA3, PMD4, PMD5, PMA5
                CKGEN_WRITE32(REG_RW_PAD_CTRL_3, u4Reg);
                break;
            }
            case 4: // multi-func 4
            {
                //* 8 mA
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_4);
                u4Reg |= (0x1B600000);  // Set 8 mA for PMWRN, PMWEN, PMRDY
                CKGEN_WRITE32(REG_RW_PAD_CTRL_4, u4Reg);

                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_5);
                u4Reg |= (0x000000DB);  // Set 8 mA for PMVS2N, PMWAITN, PMREGN
                CKGEN_WRITE32(REG_RW_PAD_CTRL_5, u4Reg);
                //*/
                /*
                // 4 mA
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_4);
                u4Reg |= (0x12400000);  // Set 4 mA for PMWRN, PMWEN, PMRDY
                CKGEN_WRITE32(REG_RW_PAD_CTRL_4, u4Reg);

                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_5);
                u4Reg |= (0x000000D2);  // Set 4 mA for PMVS2N, PMWAITN. 8mA for PMREGN
                CKGEN_WRITE32(REG_RW_PAD_CTRL_5, u4Reg);
                break;
                */
            }
            case 5: // multi-func 5
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_5);
                u4Reg |= (0x6C000000);  // Set 8 mA for PMD0, PMA1
                //u4Reg |= (0x00000000);  // Set 2 mA for PMD0, PMA1
                CKGEN_WRITE32(REG_RW_PAD_CTRL_5, u4Reg);

                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_6);
                u4Reg |= (0x000006DB);  // Set 8 mA for PMD1, PMA0, PMIPKN, PMALEL
                //u4Reg |= (0x00000000);  // Set 2 mA for PMD1, PMA0, PMIPKN, PMALEL
                CKGEN_WRITE32(REG_RW_PAD_CTRL_6, u4Reg);
                break;
            }
            case 6: // multi-func 6
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_9);
                // u4Reg |= (0x00F000FF);  // Set 8 mA for GPIO10, GPIO11, GPIO1, GPIO0, GPIO2, GPIO3
                // 4 mA for better wave form
                u4Reg |= (0x00500055);  // Set 4 mA for GPIO10, GPIO11, GPIO1, GPIO0, GPIO2, GPIO3
                CKGEN_WRITE32(REG_RW_PAD_CTRL_9, u4Reg);
                break;
            }
            case 8: // multi-func 7,8
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_7);
                // u4Reg |= (0x00003FF3);  // Set 8 mA for NFD6, NFD5, NFD4, NFD3, NFD2, NFD0
                u4Reg |= (0x00000000);  // Set 2 mA for NFD6, NFD5, NFD4, NFD3, NFD2, NFD0
                CKGEN_WRITE32(REG_RW_PAD_CTRL_7, u4Reg);
                break;
            }
            case 9: // multi-func 8,9
            {
                // Driving current
                u4Reg = CKGEN_READ32(REG_RW_PAD_CTRL_7);
                //u4Reg |= (0x03FF000C);  // Set 8 mA for NFRBN, NFREN, NFCEN, NFD1, NFCLE, NFALE
                // 8mA will overshoot & undershoot too seriously => 2mA is very well
                u4Reg |= (0x00000000);  // Set 2 mA for NFRBN, NFREN, NFCEN, NFD1, NFCLE, NFALE
                CKGEN_WRITE32(REG_RW_PAD_CTRL_7, u4Reg);
                break;
            }
        }
    }

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550) //mhzhang
    if (i4Slot == SDC_SLOT)
    {
        switch(_u4SDMultiFuncCfg)
        {
            case 1: // multi-func 1
            { 
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[18:16] = 1
                // 1. 0x7000_00E0 bit[22:20] = 1
                // 1. 0x7000_00E0 bit[26:24] = 1                
                // 1. 0x7000_00E0 bit[30:28] = 1                                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
                u4Reg |=   (((UINT32)0x01 << 16) | ((UINT32)0x01 << 20) | ((UINT32)0x01 << 24) | ((UINT32)0x01 << 28));
                CKGEN_WRITE32(0xE0, u4Reg);

               // common setting // using the same GPIO , high porioty than FCI, disable them
               // 7000_00C4[5] = 0
               // 7000_00CC[7] = 0
               // 7000_00C4[5] = 0
               // 7000_00C4[6] = 0
               // 7000_00CC[7] = 0               
               CKGEN_CLRBIT(0xC4, ((UINT32)0x3)<<5);
               CKGEN_CLRBIT(0xCC, ((UINT32)0x1)<<7);
			   
                // Setup Pull up register
                // clock don't need to Pull-up or Pull-down
                //PDWNC_SETBIT(0x0EC, (((UINT32)1) <<13)); // EXRXD3, SD-CLK, Set PU 
                //PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<13)); // EXRXD3, SD-CLK, Clear PD
                PDWNC_SETBIT(0x0EC, (((UINT32)1) <<20)); // ETTXD0, SD-CMD, Set PU
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<20)); // ETTXD0, SD-CMD, Clear PD                
                PDWNC_SETBIT(0x0EC, (((UINT32)1) <<22)); // ETTXCLK, SD-D0, Set PU
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<22)); // ETTXCLK, SD-D0, Clear PD               
                PDWNC_SETBIT(0x0EC, (((UINT32)1) <<17)); // ETTXD3, SD-D1, Set PU
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<17)); // ETTXD3, SD-D1, Clear PD
                PDWNC_SETBIT(0x0EC, (((UINT32)1) <<18)); // ETTXD2, SD-D2, Set PU
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<18)); // ETTXD2, SD-D2, Clear PD
                PDWNC_SETBIT(0x0EC, (((UINT32)1) <<19)); // ETTXD1, SD-D3, Set PU
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<19)); // ETTXD1, SD-D3, Clear PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 2: // multi-func 2
            {
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[18:16] = 2
                // 1. 0x7000_00E0 bit[22:20] = 2
                // 1. 0x7000_00E0 bit[26:24] = 2
                // 1. 0x7000_00E0 bit[30:28] = 2                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
                u4Reg |=   (((UINT32)0x02 << 16) | ((UINT32)0x02 << 20) | ((UINT32)0x02 << 24) | ((UINT32)0x02 << 28));
                CKGEN_WRITE32(0xE0, u4Reg);
				
                //common setting // disable pirioty higher than FCI
               // 7000_00C0[9] = 0
               // 7000_00C0[20] = 0
               // 7000_00C0[10] = 0
               // 7000_00C0[17:15] = 0
               // 7000_00C0[18] = 0
               // 7000_00C0[19] = 0
               // 7000_00C0[1:0] = 0
              CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<9);
              CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<20);
              CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<10);		  
              CKGEN_CLRBIT(0xC0, ((UINT32)0x7)<<15);
              CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<18);
              CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<19);
              CKGEN_CLRBIT(0xC0, ((UINT32)0x3)<<0);
		  
                // Setup Pull up register
                // clock don't need to Pull-up or Pull -down
                //CKGEN_SETBIT(0x0104, (((UINT32)1) << 6));  // AOSDATA3, SD-CLK, Set PU
                //CKGEN_CLRBIT(0x0104, (((UINT32)1) << 7));  // AOSDATA3, MS-CLK, Clear PD
                CKGEN_SETBIT(0x0108, (((UINT32)1) << 4));  // SPMCLK, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0108, (((UINT32)1) << 5));  // SPMCLK, MS-CMD, Clear PD                
                CKGEN_SETBIT(0x0104, (((UINT32)1) << 12)); // AOSDATA4, SD-D0, Set PU
                CKGEN_CLRBIT(0x0104, (((UINT32)1) << 13)); // AOSDATA4, MS-D0, Clear PD                
                CKGEN_SETBIT(0x0104, (((UINT32)1) << 30)); // SPDATA, SD-D1, Set PU
                CKGEN_CLRBIT(0x0104, (((UINT32)1) << 31)); // SPDATA, MS-D1, Clear PD                
                CKGEN_SETBIT(0x0108, (((UINT32)1) << 16)); // SPLRCK, SD-D2, Set PU
                CKGEN_CLRBIT(0x0108, (((UINT32)1) << 17)); // SPLRCK, MS-D2, Clear PD                
                CKGEN_SETBIT(0x0108, (((UINT32)1) << 10)); // SPBCK, SD-D3, Set PU
                CKGEN_CLRBIT(0x0108, (((UINT32)1) << 11)); // SPBCK, MS-D3, Clear PD                

                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 3: // multi-func 3
            {
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[18:16] = 3
                // 1. 0x7000_00E0 bit[22:20] = 3
                // 1. 0x7000_00E0 bit[26:24] = 3
                // 1. 0x7000_00E0 bit[30:28] = 3                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
                u4Reg |=   (((UINT32)0x03 << 16) | ((UINT32)0x03 << 20) | ((UINT32)0x03 << 24) | ((UINT32)0x03 << 28));
                CKGEN_WRITE32(0xE0, u4Reg);

                // Setup Pull up register
                //CKGEN_SETBIT(0x0128, (((UINT32)1) << 24)); // GPIO4, SD-CMD, Set PU
                //CKGEN_CLRBIT(0x0128, (((UINT32)1) << 25)); // GPIO4, MS-CMD, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 18)); // GPIO3, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 19)); // GPIO3, MS-CMD, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 30)); // GPIO5, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 31)); // GPIO5, MS-CMD, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 12)); // GPIO2, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 13)); // GPIO2, MS-CMD, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 0));  // GPIO0, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 1));  // GPIO0, MS-CMD, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 6));  // GPIO1, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 7));  // GPIO1, MS-CMD, Clear PD

                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 4: // multi-func 4
            {
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[18:16] = 4
                // 1. 0x7000_00E0 bit[22:20] = 4
                // 1. 0x7000_00E0 bit[26:24] = 4
                // 1. 0x7000_00E0 bit[30:28] = 4                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
                u4Reg |=   (((UINT32)0x04 << 16) | ((UINT32)0x04 << 20) | ((UINT32)0x04 << 24) | ((UINT32)0x04 << 28));
                CKGEN_WRITE32(0xE0, u4Reg);

                // common setting  // disable pirioty higher than FCI
                // 7000_00C0[31]=0
                // 7000_00D4[2]=1
                CKGEN_CLRBIT(0xC0, ((UINT32)1)<<31);
                CKGEN_SETBIT(0xD4, ((UINT32)1)<<2);
				
                // Setup Pull up register
                //CKGEN_SETBIT(0x0114, (((UINT32)1) << 22)); // NFCEN2, SD-CLK, Set PU
                //CKGEN_CLRBIT(0x0114, (((UINT32)1) << 23)); // NFCEN2, MS-CLK, Clear PD
                CKGEN_SETBIT(0x0114, (((UINT32)1) << 28)); // NFRBN2, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0114, (((UINT32)1) << 29)); // NFRBN2, MS-CMD, Clear PD
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 20)); // SFDO, SD-D0, Set PU
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 21)); // SFDO, MS-D0, Clear PD
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 14)); // SFDI, SD-D1, Set PU
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 15)); // SFDI, MS-D1, Clear PD
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 2));  // SFCS, SD-D2, Set PU
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 3));  // SFCS, MS-D2, Clear PD
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 8));  // SFCK, SD-D3, Set PU
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 9));  // SFCK, MS-D3, Clear PD

                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 5: // multi-func 5
            {
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[18:16] = 5
                // 1. 0x7000_00E0 bit[22:20] = 5                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20));
                u4Reg |=   (((UINT32)0x05 << 16) |((UINT32)0x05 << 20));
                CKGEN_WRITE32(0xE0, u4Reg);      
                
                // Setup Pull up register
                //CKGEN_SETBIT(0x0128, (((UINT32)1) << 0));   // GPIO0, SD-CLK, Set PU
                //CKGEN_CLRBIT(0x0128, (((UINT32)1) << 1));   // GPIO0, SD-CLK, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 12)); // GPIO2, SD-CMD, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 13)); // GPIO2, SD-CMD, Clear PD
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 6));  // GPIO1, SD-D0, Set PU
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 7));  // GPIO1, SD-D0, Clear PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = FALSE;
                break;
            }
            default:
            {
                break;
            }
        }
    }
   else if(i4Slot == MSP_SLOT)
  {
        // Set SD to multi-function 0 : avoid conflict with MS
        u4Reg = CKGEN_READ32(0xE0);
        u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
        u4Reg |=   (((UINT32)0x00 << 16) | ((UINT32)0x00 << 20) | ((UINT32)0x00 << 24) | ((UINT32)0x00 << 28));
        CKGEN_WRITE32(0xE0, u4Reg);
                
        switch(_u4MSMultiFuncCfg)
        {
            case 1: // multi-func 1
            {
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[6:4] = 1
                // 1. 0x7000_00E0 bit[10:8] = 1                
                // 1. 0x7000_00E0 bit[14:12] = 1                                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 4) | ((UINT32)0x07 << 8) | ((UINT32)0x07 << 12));
                u4Reg |=   (((UINT32)0x01 << 4) | ((UINT32)0x01 << 8) | ((UINT32)0x01 << 12));
                CKGEN_WRITE32(0xE0, u4Reg);                
          				
               // common setting // using the same GPIO , high porioty than FCI, disable them
               // 7000_00C4[5] = 0
               // 7000_00CC[7] = 0
               // 7000_00C4[5] = 0
               // 7000_00C4[6] = 0
               // 7000_00CC[7] = 0               
               CKGEN_CLRBIT(0xC4, ((UINT32)0x3)<<5);
               CKGEN_CLRBIT(0xCC, ((UINT32)0x1)<<7);

                // Setup Pull up register to SD-CMD, if not set, system will power reset !!
                PDWNC_SETBIT(0x0EC, (((UINT32)1) <<20)); // ETTXD0, SD-CMD, Set PU
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) <<20)); // ETTXD0, SD-CMD, Clear PD                    
				
                // Setup Pull down register
                //PDWNC_CLRBIT(0x0EC, (((UINT32)1) << 13)); // ETRXD3, MS-CLK, Clear PU 
                //PDWNC_SETBIT(0x0F0, (((UINT32)1) << 13)); // ETRXD3, MS-CLK, Set PD   
                PDWNC_CLRBIT(0x0EC, (((UINT32)1) << 20)); // ETTXD0, MS-BS, Clear PU 
                PDWNC_CLRBIT(0x0F0, (((UINT32)1) << 20)); // ETTXD0, MS-BS, Clear PD   
                PDWNC_CLRBIT(0x0EC, (((UINT32)1) << 22)); // ETTXCLK, MS-D0, Clear PU 
                PDWNC_SETBIT(0x0F0, (((UINT32)1) << 22)); // ETTXCLK, MS-D0, Set PD   
                PDWNC_CLRBIT(0x0EC, (((UINT32)1) << 17)); // ETTXD3, MS-D1, Clear PU 
                PDWNC_SETBIT(0x0F0, (((UINT32)1) << 17)); // ETTXD3, MS-D1, Set PD   
                PDWNC_CLRBIT(0x0EC, (((UINT32)1) << 18)); // ETTXD2, MS-D2, Clear PU 
                PDWNC_SETBIT(0x0F0, (((UINT32)1) << 18)); // ETTXD2, MS-D2, Set PD   
                PDWNC_CLRBIT(0x0EC, (((UINT32)1) << 19)); // ETTXD1, MS-D3, Clear PU 
                PDWNC_SETBIT(0x0F0, (((UINT32)1) << 19)); // ETTXD1, MS-D3, Set PD   
                         
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 2: // multi-func 2
            {
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[6:4] = 2
                // 1. 0x7000_00E0 bit[10:8] = 2            
                // 1. 0x7000_00E0 bit[14:12] = 2                            
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 4) | ((UINT32)0x07 << 8) | ((UINT32)0x07 << 12));
                u4Reg |=   (((UINT32)0x02 << 4) | ((UINT32)0x02 << 8) | ((UINT32)0x02 << 12));
                CKGEN_WRITE32(0xE0, u4Reg);
				
                // common setting
               // 7000_00C0[9] = 0
               // 7000_00C0[20] = 0
               // 7000_00C0[10] = 0
               // 7000_00C0[17:15] = 0
               // 7000_00C0[18] = 0
               // 7000_00C0[19] = 0
               // 7000_00C0[1:0] = 0               
                CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<9);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<20);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<10);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x7)<<15);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<18);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<19);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x3)<<0);
				
                // Setup Pull down register
                //CKGEN_CLRBIT(0x0104, (((UINT32)1) << 6));   // AOSDATA3, MS-CLK, Clear PU
                //CKGEN_SETBIT(0x0104, (((UINT32)1) << 7));   // AOSDATA3, MS-CLK, Set PD
                //CKGEN_CLRBIT(0x0108, (((UINT32)1) << 4));   // SPMCLK, MS-BS, Clear PU
                //CKGEN_SETBIT(0x0108, (((UINT32)1) << 5));   // SPMCLK, MS-BS, Set PD
                CKGEN_CLRBIT(0x0104, (((UINT32)1) << 12)); // AOSDATA4, MS-D0, Clear PU
                CKGEN_SETBIT(0x0104, (((UINT32)1) << 13)); // AOSDATA4, MS-D0, Set PD
                CKGEN_CLRBIT(0x0104, (((UINT32)1) << 30)); // SPDATA, MS-D1, Clear PU
                CKGEN_SETBIT(0x0104, (((UINT32)1) << 31)); // SPDATA, MS-D1, Set PD
                CKGEN_CLRBIT(0x0108, (((UINT32)1) << 16)); // SPLRCK, MS-D2, Clear PU
                CKGEN_SETBIT(0x0108, (((UINT32)1) << 17)); // SPLRCK, MS-D2, Set PD
                CKGEN_CLRBIT(0x0108, (((UINT32)1) << 10)); // SPBCK, MS-D3, Clear PU
                CKGEN_SETBIT(0x0108, (((UINT32)1) << 11)); // SPBCK, MS-D3, Set PD
                               
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 3: // multi-func 3
            {                
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[6:4] = 3                
                // 1. 0x7000_00E0 bit[10:8] = 3          
                // 1. 0x7000_00E0 bit[14:12] = 3                          
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 4) | ((UINT32)0x07 << 8) | ((UINT32)0x07 << 12));
                u4Reg |=   (((UINT32)0x03 << 4) | ((UINT32)0x03 << 8) | ((UINT32)0x03 << 12));
                CKGEN_WRITE32(0xE0, u4Reg);

                // Setup Pull down register
                //CKGEN_CLRBIT(0x0128, (((UINT32)1) << 24)); // GPIO4, MS-CLK, Clear PU
                //CKGEN_SETBIT(0x0128, (((UINT32)1) << 25)); // GPIO4, MS-CLK, Set PD
                //CKGEN_CLRBIT(0x0128, (((UINT32)1) << 18)); // GPIO3, MS-BS, Clear PU
                //CKGEN_SETBIT(0x0128, (((UINT32)1) << 19)); // GPIO3, MS-BS, Set PD               
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 30)); // GPIO5, MS-D0, Clear PU
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 31)); // GPIO5, MS-D0, Set PD
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 12)); // GPIO2, MS-D1, Clear PU
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 13)); // GPIO2, MS-D1, Set PD
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 0));  // GPIO0, MS-D2, Clear PU
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 1));  // GPIO0, MS-D2, Set PD
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 6));  // GPIO1, MS-D3, Clear PU
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 7));  // GPIO1, MS-D3, Set PD
			
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 4: // multi-func 4
            {            
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[6:4] = 4
                // 1. 0x7000_00E0 bit[10:8] = 4
                // 1. 0x7000_00E0 bit[14:12] = 4                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 4) | ((UINT32)0x07 << 8) | ((UINT32)0x07 << 12));
                u4Reg |=   (((UINT32)0x04 << 4) | ((UINT32)0x04 << 8) | ((UINT32)0x04 << 12));
                CKGEN_WRITE32(0xE0, u4Reg);
				
                // common setting  // disable pirioty higher than FCI
                // 7000_00C0[31]=0
                // 7000_00D4[2]=1
                CKGEN_CLRBIT(0xC0, ((UINT32)1)<<31);
                CKGEN_SETBIT(0xD4, ((UINT32)1)<<2);
				
                // Setup Pull down register
                //CKGEN_CLRBIT(0x0114, (((UINT32)1) << 22)); // NFCEN2, MS-CLK, Clearcc PU
                //CKGEN_SETBIT(0x0114, (((UINT32)1) << 23)); // NFCEN2, MS-CLK, Set PD
                //CKGEN_CLRBIT(0x0114, (((UINT32)1) << 28)); // NFRBN2, MS-BS, Clearcc PU
                //CKGEN_SETBIT(0x0114, (((UINT32)1) << 29)); // NFRBN2, MS-BS, Set PD
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 20)); // SFDO, MS-D0, Clearcc PU
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 21)); // SFDO, MS-D0, Set PD
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 14)); // SFDI, MS-D1, Clearcc PU
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 15)); // SFDI, MS-D1, Set PD
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 2));  // SFCS, MS-D2, Clearcc PU
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 3));  // SFCS, MS-D2, Set PD
                CKGEN_CLRBIT(0x0118, (((UINT32)1) << 8));  // SFCK, MS-D3, Clearcc PU
                CKGEN_SETBIT(0x0118, (((UINT32)1) << 9));  // SFCK, MS-D3, Set PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 5: // multi-func 5
            {                 
                // (1) Multiple function
                // 1. 0x7000_00E0 bit[6:4] = 5
                // 1. 0x7000_00E0 bit[10:8] = 5                
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 4) | ((UINT32)0x07 << 8));
                u4Reg |=   (((UINT32)0x05 << 4) | ((UINT32)0x05 << 8));
                CKGEN_WRITE32(0xE0, u4Reg);            

                // Setup Pull down register
                //CKGEN_CLRBIT(0x0128, (((UINT32)1) << 0)); // GPIO0, MS-CLK, Clear PU
                //CKGEN_SETBIT(0x0128, (((UINT32)1) << 1)); // GPIO0, MS-CLK, Set PD
                //CKGEN_CLRBIT(0x0128, (((UINT32)1) << 12)); // GPIO2, MS-BS, Clear PU
                //CKGEN_SETBIT(0x0128, (((UINT32)1) << 13)); // GPIO2, MS-BS, Set PD
                CKGEN_CLRBIT(0x0128, (((UINT32)1) << 6)); // GPIO1, MS-D0, Clear PU
                CKGEN_SETBIT(0x0128, (((UINT32)1) << 7)); // GPIO1, MS-D0, Set PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = FALSE;
                break;
            }
            default:
            {
                break;
            }
        }
    }	


#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)  // mhzhang
    if(i4Slot == SDC_SLOT)
    {
           switch(_u4SDMultiFuncCfg)
           {
               case 1:
        	{
		    //multifunction 1
		    // 0x7000_00E0[18:16]=1
		    // 0x7000_00E0[22:20]=1
		    // 0x7000_00E0[26:24]=1
		    // 0x7000_00E0[30:28]=1
		    CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<9);
		    CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<20);
		    CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<24);
		    CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<28);
			
		    //common setting
		    CKGEN_CLRBIT(0xE4, ((UINT32)0x7)<<9);
		    CKGEN_CLRBIT(0xE4, ((UINT32)0x3)<<12);
		    CKGEN_CLRBIT(0xE4, ((UINT32)0x7)<<14);
		    CKGEN_CLRBIT(0xE4, ((UINT32)0x3)<<4);
			
		    // set up Pull Up
		    // enable Pull Up for SD
		    // clock don't need Pull-up or Pull-down
		    //CKGEN_SETBIT(0x150, ((UINT32)0x1)<<10);
		    CKGEN_SETBIT(0x150, ((UINT32)0x1)<<4);
		    CKGEN_SETBIT(0x150, ((UINT32)0x1)<<16);
		    CKGEN_SETBIT(0x150, ((UINT32)0x1)<<23);
		    CKGEN_SETBIT(0x150, ((UINT32)0x1)<<28);
		    CKGEN_SETBIT(0x154, ((UINT32)0x1)<<2);
		   // disable Pull Down of MS
		    //CKGEN_CLRBIT(0x150, ((UINT32)0x1)<<11);
		    CKGEN_CLRBIT(0x150, ((UINT32)0x1)<<5);
		    CKGEN_CLRBIT(0x150, ((UINT32)0x1)<<17);
		    CKGEN_CLRBIT(0x150, ((UINT32)0x1)<<24);
		    CKGEN_CLRBIT(0x150, ((UINT32)0x1)<<29);
		    CKGEN_CLRBIT(0x154, ((UINT32)0x1)<<3);				

                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;			
             	}
	       case 2:
   		{
			// multifunction 2:
			// 0x7000_00E0[18:16] = 2
			// 0x7000_00E0[22:20] = 2
			// 0x7000_00E0[26:24] = 2
			// 0x7000_00E0[30:28] = 2
			CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<16);
			CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<20);
			CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<24);
			CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<28);
			
			//common setting
		    	CKGEN_CLRBIT(0xC0, ((UINT32)0x3f)<<15);
		    	CKGEN_CLRBIT(0xC0, ((UINT32)0x3)<<9);
		    	CKGEN_CLRBIT(0xC0, ((UINT32)0x3)<<0);
                     CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<21);
		    	CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<18);
		    	CKGEN_CLRBIT(0xE4, ((UINT32)0x1)<<29);

			// set pull up
			// enable Pull Up for SD
			//CKGEN_SETBIT(0x104, ((UINT32)0x1)<<6);
			CKGEN_SETBIT(0x108, ((UINT32)0x1)<<4);
			CKGEN_SETBIT(0x104, ((UINT32)0x1)<<12);
			CKGEN_SETBIT(0x104, ((UINT32)0x1)<<30);
			CKGEN_SETBIT(0x108, ((UINT32)0x1)<<16);
			CKGEN_SETBIT(0x108, ((UINT32)0x1)<<10);
			// dsiable Pull Down of MS
			//CKGEN_CLRBIT(0x104, ((UINT32)0x1)<<7);
			CKGEN_CLRBIT(0x108, ((UINT32)0x1)<<5);
			CKGEN_CLRBIT(0x104, ((UINT32)0x1)<<13);
			CKGEN_CLRBIT(0x104, ((UINT32)0x1)<<31);
			CKGEN_CLRBIT(0x108, ((UINT32)0x1)<<17);
			CKGEN_CLRBIT(0x108, ((UINT32)0x1)<<11);	
			
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;			
       	}
		 case 3:
		 {
		 	// multifunciton 3
		 	// 0x7000_00E0[18:16] = 3
		 	// 0x7000_00E0[22:20] = 3
		 	// 0x7000_00E0[26:24] = 3
		 	// 0x7000_00E0[30:28] = 3
		 	CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<16);
		 	CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<20);
		 	CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<24);
		 	CKGEN_SETBIT(0xE0, ((UINT32)0x7)<<28);
			
			//common setting
			CKGEN_CLRBIT(0xD4, ((UINT32)0x1ff)<<15);
			CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<31);
			CKGEN_CLRBIT(0xD4, ((UINT32)0x1)<<2);

			// set Pull up
			// enalbe Pull Up for SD
			//CKGEN_SETBIT(0x114, ((UINT32)0x1)<<22);
			CKGEN_SETBIT(0x114, ((UINT32)0x1)<<28);
			CKGEN_SETBIT(0x118, ((UINT32)0x1)<<20);
			CKGEN_SETBIT(0x118, ((UINT32)0x1)<<14);
			CKGEN_SETBIT(0x118, ((UINT32)0x1)<<2);
			CKGEN_SETBIT(0x118, ((UINT32)0x1)<<8);
			// disable Pull Down of MS
			//CKGEN_CLRBIT(0x114, ((UINT32)0x1)<<23);
			CKGEN_CLRBIT(0x114, ((UINT32)0x1)<<29);
			CKGEN_CLRBIT(0x118, ((UINT32)0x1)<<21);
			CKGEN_CLRBIT(0x118, ((UINT32)0x1)<<15);
			CKGEN_CLRBIT(0x118, ((UINT32)0x1)<<3);
			CKGEN_CLRBIT(0x118, ((UINT32)0x1)<<9);
			
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;			
		 }
		default:
              {
                break;
              }
           }
     }
     else if(i4Slot == MSP_SLOT)
       {
        switch(_u4MSMultiFuncCfg)
        {
            case 1: // multi-func 1
            {
                // (1) Multiple function
                //  0x7000_00E0 bit[6:4] = 1
                //  0x7000_00E0 bit[10:8] = 1 
                //  0x7000_00E0 bit[14:12] = 1                 
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
                u4Reg |=   (((UINT32)0x07 << 16) | ((UINT32)0x07 << 20) | ((UINT32)0x07 << 24) | ((UINT32)0x07 << 28));
                CKGEN_WRITE32(0xE0, u4Reg);

		  // (2) common setting
                // 0x7000_00E4 bit[11:9] = 0            
                // 0x7000_00E4 bit[13:12] = 0
                // 0x7000_00E4 bit[16:14] = 0            
                // 0x7000_00E4 bit[5:4] = 0                
               CKGEN_CLRBIT(0xE4, ((UINT32)0x7)<<9);
               CKGEN_CLRBIT(0xE4, ((UINT32)0x3)<<2);
               CKGEN_CLRBIT(0xE4, ((UINT32)0x7)<<14);
               CKGEN_CLRBIT(0xEC, ((UINT32)0x3)<<4);
			   
                // Setup Pull down register
                // clear Pull Up of SD
                //CKGEN_CLRBIT(0x150, (((UINT32)1) << 10)); // VIND10, MS-BS, Set PD   
                CKGEN_CLRBIT(0x150, (((UINT32)1) << 4)); // VIND9, MS-D0, Set PD   
                CKGEN_CLRBIT(0x150, (((UINT32)1) << 16)); // VIND13, MS-D1, Set PD   
                CKGEN_CLRBIT(0x150, (((UINT32)1) << 23)); // VIND10, MS-BS, Set PD   
                CKGEN_CLRBIT(0x150, (((UINT32)1) << 28)); // VIND9, MS-D0, Set PD   
                CKGEN_CLRBIT(0x154, (((UINT32)1) << 2)); // VIND13, MS-D1, Set PD  
                // enalbe Pull Down for MS
                //CKGEN_SETBIT(0x150, (((UINT32)1) << 11)); // VIND10, MS-BS, Set PD   
                //CKGEN_SETBIT(0x150, (((UINT32)1) << 5)); // VIND9, MS-D0, Set PD   
                CKGEN_SETBIT(0x150, (((UINT32)1) << 17)); // VIND13, MS-D1, Set PD   
                CKGEN_SETBIT(0x150, (((UINT32)1) << 24)); // VIND10, MS-BS, Set PD   
                CKGEN_SETBIT(0x150, (((UINT32)1) << 29)); // VIND9, MS-D0, Set PD   
                CKGEN_SETBIT(0x154, (((UINT32)1) << 3)); // VIND13, MS-D1, Set PD  
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 2: // multi-func 2
            {
                // (1) Multiple function
                // 0x7000_00E0 bit[6:4] = 2
                // 0x7000_00E0 bit[10:8] = 2            
                // 0x7000_00E0 bit[14:12] = 2
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 4) | ((UINT32)0x07 << 8) | ((UINT32)0x07 << 12));
                u4Reg |=   (((UINT32)0x07 << 4) | ((UINT32)0x07 << 8) | ((UINT32)0x07 << 12));
                CKGEN_WRITE32(0xE0, u4Reg);
                                
                // common setting
                // 0x7000_00C0[20] = 0
                // 0x7000_00C0[19] = 0
                // 0x7000_00C0[18] = 0
                // 0x7000_00C0[17:15] = 0
                // 0x7000_00C0[9] = 0
                // 0x7000_00C0[10] = 0
                // 0x7000_00C0[1:0] = 0
                // 0x7000_00C0[23:21] = 0
                // 0x7000_00C0[20:18] = 0
                // 0x7000_00C0[29] = 0
                CKGEN_CLRBIT(0xC0, ((UINT32)0x3f)<<15);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x3)<<9);
                CKGEN_CLRBIT(0xC0, ((UINT32)0x3)<<0);
                CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<21);
                CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<18);
                CKGEN_CLRBIT(0xE4, ((UINT32)0x1)<<29);

                // Setup Pull down register
		  // disalbe Pull Down for SD
                //CKGEN_CLRBIT(0x104, (((UINT32)1) << 6)); 
                //CKGEN_CLRBIT(0x108, (((UINT32)1) << 4)); 
                CKGEN_CLRBIT(0x104, (((UINT32)1) << 12)); 
                CKGEN_CLRBIT(0x104, (((UINT32)1) << 30)); 
                CKGEN_CLRBIT(0x108, (((UINT32)1) << 16)); 
                CKGEN_CLRBIT(0x108, (((UINT32)1) << 10)); 
		  // enalbe Pull Down for MS
                //CKGEN_SETBIT(0x104, (((UINT32)1) << 7)); 
                //CKGEN_SETBIT(0x108, (((UINT32)1) << 5)); 
                CKGEN_SETBIT(0x104, (((UINT32)1) << 13)); 
                CKGEN_SETBIT(0x104, (((UINT32)1) << 31)); 
                CKGEN_SETBIT(0x108, (((UINT32)1) << 17)); 
                CKGEN_SETBIT(0x108, (((UINT32)1) << 11)); 
				
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 3: // multi-func 3
            {                
                // (1) Multiple function
                //  0x7000_00C4 bit[6:4] = 3                
                //  0x7000_00C4 bit[10:8] = 3          
                //  0x7000_00C4 bit[14:12] = 3                          
                u4Reg = CKGEN_READ32(0xE0);
                u4Reg &= ~(((UINT32)0x07 << 14) | ((UINT32)0x07 << 17));
                u4Reg |=   (((UINT32)0x04 << 14) | ((UINT32)0x03 << 17));
                CKGEN_WRITE32(0xE0, u4Reg);

                // (2) common setting
               CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<15);
               CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<18);
               CKGEN_CLRBIT(0xD4, ((UINT32)0x7)<<21);
               CKGEN_CLRBIT(0xC0, ((UINT32)0x1)<<31);
               CKGEN_CLRBIT(0xD4, ((UINT32)0x1)<<2);
			   
                // (3) Setup Pull down register
                // disalbe Pull Up of SD
                //CKGEN_CLRBIT(0x114, (((UINT32)1) << 22));
                //CKGEN_CLRBIT(0x114, (((UINT32)1) << 28));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 20));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 14));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 2));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 8));
                // enalbe Pull Down for MS
                //CKGEN_CLRBIT(0x114, (((UINT32)1) << 23));
                //CKGEN_CLRBIT(0x114, (((UINT32)1) << 29));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 21));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 15));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 3));
                CKGEN_CLRBIT(0x118, (((UINT32)1) << 9));                
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            default:
            {
                break;
            }
        }
    }      		


#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)

    if (i4Slot == SDC_SLOT)
    {
        switch(_u4SDMultiFuncCfg)
        {
            case 1: // multi-func 1
            { 
                // (1) Multiple function
                // 1. 0x7000_00D4 bit[17:15] = 3
                // 1. 0x7000_00D4 bit[20:18] = 3
                // 1. 0x7000_00D4 bit[23:21] = 3                
                u4Reg = CKGEN_READ32(0xD4);
                u4Reg &= ~(((UINT32)0x07 << 15) | ((UINT32)0x07 << 18) | ((UINT32)0x07 << 21));
                u4Reg |=   (((UINT32)0x03 << 15) | ((UINT32)0x03 << 18) | ((UINT32)0x03 << 21));
                CKGEN_WRITE32(0xD4, u4Reg);
                
                // 2. 0x7000_00C0 bit[27] = 0                
                // 2. 0x7000_00C0 bit[28] = 0 
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~(((UINT32)0x01 << 27) | ((UINT32)0x01 << 28));
                u4Reg |=   (((UINT32)0x00 << 27) | ((UINT32)0x00 << 28)); 
                CKGEN_WRITE32(0xC0, u4Reg);
                
                // 3. 0x7000_00CC bit[5] = 0 
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~((UINT32)0x01 << 5);
                u4Reg |=   ((UINT32)0x00 << 5); 
                CKGEN_WRITE32(0xC0, u4Reg);  

                // Setup Pull up register
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 26)); // VIND10, SD-CMD, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 27)); // VIND10, SD-CMD, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 22)); // VIND9, SD-D0, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 23)); // VIND9, SD-D0, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 6)); // VIND13, SD-D1, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 7)); // VIND13, SD-D1, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 2)); // VIND12, SD-D2, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 3)); // VIND12, SD-D2, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 30)); // VIND11, SD-D3, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 31)); // VIND11, SD-D3, Clear PD                

                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 2: // multi-func 2
            {
                // (1) Multiple function
                // 1. 0x7000_00D4 bit[17:15] = 4
                // 1. 0x7000_00D4 bit[20:18] = 4
                // 1. 0x7000_00D4 bit[23:21] = 4
                u4Reg = CKGEN_READ32(0xD4);
                u4Reg &= ~(((UINT32)0x07 << 15) | ((UINT32)0x07 << 18) | ((UINT32)0x07 << 21));
                u4Reg |=   (((UINT32)0x04 << 15) | ((UINT32)0x04 << 18) | ((UINT32)0x04 << 21));
                CKGEN_WRITE32(0xD4, u4Reg);
                
                // 2. 0x7000_00C0 bit[20:0] = 0
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~((UINT32)0x1FFFFF << 0);
                u4Reg |=   ((UINT32)0x00 << 0);
                CKGEN_WRITE32(0xC0, u4Reg);

                // Setup Pull up register
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 6)); // SPMCLK, SD-CMD, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 7)); // SPMCLK, SD-CMD, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, (((UINT32)1) << 22)); // AOSDATA4, SD-D0, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, (((UINT32)1) << 23)); // AOSDATA4, SD-D0, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 2)); // SPDATA, SD-D1, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 3)); // SPDATA, SD-D1, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 16)); // SPLRCK, SD-D2, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 17)); // SPLRCK, SD-D2, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 11)); // SPBCK, SD-D3, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 12)); // SPBCK, SD-D3, Clear PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 3: // multi-func 3
            {
                // (1) Multiple function
                // 1. 0x7000_00D4 bit[17:15] = 1
                // 1. 0x7000_00D4 bit[20:18] = 1
                // 1. 0x7000_00D4 bit[23:21] = 1
                u4Reg = CKGEN_READ32(0xD4);
                u4Reg &= ~(((UINT32)0x07 << 15) | ((UINT32)0x07 << 18) | ((UINT32)0x07 << 21));
                u4Reg |=   (((UINT32)0x01 << 15) | ((UINT32)0x01 << 18) | ((UINT32)0x01 << 21));
                CKGEN_WRITE32(0xD4, u4Reg);

                // Setup Pull up register
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 14)); // GPIO3, SD-CMD, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 15)); // GPIO3, SD-CMD, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 22)); // GPIO5, SD-D0, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 23)); // GPIO5, SD-D0, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 10)); // GPIO2, SD-D1, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 11)); // GPIO2, SD-D1, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 2)); // GPIO0, SD-D2, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 3)); // GPIO0, SD-D2, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 6)); // GPIO1, SD-D3, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 7)); // GPIO1, SD-D3, Clear PD                
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 4: // multi-func 4
            {
                // (1) Multiple function
                // 1. 0x7000_00D4 bit[17:15] = 2
                // 1. 0x7000_00D4 bit[20:18] = 2
                // 1. 0x7000_00D4 bit[23:21] = 2
                u4Reg = CKGEN_READ32(0xD4);
                u4Reg &= ~(((UINT32)0x07 << 15) | ((UINT32)0x07 << 18) | ((UINT32)0x07 << 21));
                u4Reg |=   (((UINT32)0x02 << 15) | ((UINT32)0x02 << 18) | ((UINT32)0x02 << 21));
                CKGEN_WRITE32(0xD4, u4Reg);

                // 3. 0x7000_00C0 bit[29] = 0
                // 2. 0x7000_00C0 bit[28] = 0                
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~(((UINT32)0x01 << 28) | ((UINT32)0x01 << 29));
                u4Reg |=   (((UINT32)0x00 << 28) | ((UINT32)0x00 << 29));                 
                CKGEN_WRITE32(0xC0, u4Reg);
                
                // Setup Pull up register
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 22)); // VIND17, SD-CMD, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 23)); // VIND17, SD-CMD, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 30)); // VIND19, SD-D0, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 31)); // VIND19, SD-D0, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 18)); // VIND16, SD-D1, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 19)); // VIND16, SD-D1, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 10)); // VIND14, SD-D2, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 11)); // VIND14, SD-D2, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 14)); // VIND15, SD-D3, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 15)); // VIND15, SD-D3, Clear PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 5: // multi-func 5
            {
                // (1) Multiple function
                // 1. 0x7000_00D4 bit[17:15] = 5
                u4Reg = CKGEN_READ32(0xD4);
                u4Reg &= ~((UINT32)0x07 << 15);
                u4Reg |=   ((UINT32)0x05 << 15);
                CKGEN_WRITE32(0xD4, u4Reg);      
                
                // Setup Pull up register
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 10)); // GPIO2, SD-CMD, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 11)); // GPIO2, SD-CMD, Clear PD
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 6)); // GPIO1, SD-D0, Set PU
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 7)); // GPIO1, SD-D0, Clear PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = FALSE;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else if (i4Slot == MSP_SLOT)
    {
        switch(_u4MSMultiFuncCfg)
        {
            case 1: // multi-func 1
            {
                // (1) Multiple function
                // 1. 0x7000_00C4 bit[16:14] = 1
                // 1. 0x7000_00C4 bit[19:17] = 1                
                u4Reg = CKGEN_READ32(0xC4);
                u4Reg &= ~(((UINT32)0x07 << 14) | ((UINT32)0x07 << 17));
                u4Reg |=   (((UINT32)0x01 << 14) | ((UINT32)0x01 << 17));
                CKGEN_WRITE32(0xC4, u4Reg);
                
                // 2. 0x7000_00C0 bit[27] = 0            
                // 3. 0x7000_00C0 bit[28] = 0
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~(((UINT32)0x01 << 27) | ((UINT32)0x01 << 28));
                u4Reg |=   (((UINT32)0x00 << 27) | ((UINT32)0x00 << 28)); 
                CKGEN_WRITE32(0xC0, u4Reg);
                
                // 3. 0x7000_00CC bit[5] = 0
                u4Reg = CKGEN_READ32(0xCC);
                u4Reg &= ~((UINT32)0x01 << 5);
                u4Reg |=   ((UINT32)0x00 << 5); 
                CKGEN_WRITE32(0xCC, u4Reg);  

                // Setup Pull down register
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 26)); // VIND10, MS-BS, Clear PU 
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 27)); // VIND10, MS-BS, Set PD   
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 22)); // VIND9, MS-D0, Clear PU 
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 23)); // VIND9, MS-D0, Set PD   
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 6)); // VIND13, MS-D1, Clear PU 
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 7)); // VIND13, MS-D1, Set PD   
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 2)); // VIND12, MS-D2, Clear PU 
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 3)); // VIND12, MS-D2, Set PD   
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 30)); // VIND11, MS-D3, Clear PU 
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, (((UINT32)1) << 31)); // VIND11, MS-D3, Set PD            
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 2: // multi-func 2
            {
                // (1) Multiple function
                // 1. 0x7000_00C4 bit[16:14] = 2
                // 1. 0x7000_00C4 bit[19:17] = 2            
                u4Reg = CKGEN_READ32(0xC4);
                u4Reg &= ~(((UINT32)0x07 << 14) | ((UINT32)0x07 << 17));
                u4Reg |=   (((UINT32)0x02 << 14) | ((UINT32)0x02 << 17));
                CKGEN_WRITE32(0xC4, u4Reg);
                                
                // 2. 0x7000_00C0 bit[20:0] = 0
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~((UINT32)0x1FFFFF << 0);
                u4Reg |=   ((UINT32)0x00 << 0);
                CKGEN_WRITE32(0xC0, u4Reg);
                
                // Setup Pull down register
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 6)); // SPMCLK, MS-BS, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 7)); // SPMCLK, MS-BS, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, (((UINT32)1) << 22)); // AOSDATA4, MS-D0, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, (((UINT32)1) << 23)); // AOSDATA4, MS-D0, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 2)); // SPDATA, MS-D1, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 3)); // SPDATA, MS-D1, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 16)); // SPLRCK, MS-D2, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 17)); // SPLRCK, MS-D2, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 11)); // SPBCK, MS-D3, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, (((UINT32)1) << 12)); // SPBCK, MS-D3, Set PD                
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 3: // multi-func 3
            {                
                // (1) Multiple function
                // 1. 0x7000_00C4 bit[16:14] = 4                
                // 1. 0x7000_00C4 bit[19:17] = 3          
                u4Reg = CKGEN_READ32(0xC4);
                u4Reg &= ~(((UINT32)0x07 << 14) | ((UINT32)0x07 << 17));
                u4Reg |=   (((UINT32)0x04 << 14) | ((UINT32)0x03 << 17));
                CKGEN_WRITE32(0xC4, u4Reg);

                // Setup Pull down register
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 14)); // GPIO3, MS-BS, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 15)); // GPIO3, MS-BS, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 22)); // GPIO5, MS-D0, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 23)); // GPIO5, MS-D0, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 10)); // GPIO2, MS-D1, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 11)); // GPIO2, MS-D1, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 2)); // GPIO0, MS-D2, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 3)); // GPIO0, MS-D2, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 6)); // GPIO1, MS-D3, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 7)); // GPIO1, MS-D3, Set PD                 
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 4: // multi-func 4
            {            
                // (1) Multiple function
                // 1. 0x7000_00C4 bit[16:14] = 5
                // 1. 0x7000_00C4 bit[19:17] = 4
                u4Reg = CKGEN_READ32(0xC4);
                u4Reg &= ~(((UINT32)0x07 << 14) | ((UINT32)0x07 << 17));
                u4Reg |=   (((UINT32)0x05 << 14) | ((UINT32)0x04 << 17));
                CKGEN_WRITE32(0xC4, u4Reg);
                
                // 2. 0x7000_00C0 bit[28] = 0
                // 3. 0x7000_00C0 bit[29] = 0
                u4Reg = CKGEN_READ32(0xC0);
                u4Reg &= ~(((UINT32)0x01 << 28) | ((UINT32)0x01 << 29));
                u4Reg |=   (((UINT32)0x00 << 28) | ((UINT32)0x00 << 29));
                CKGEN_WRITE32(0xC0, u4Reg);
                
                // Setup Pull down register
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 22)); // VIND17, MS-BS, Clearcc PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 23)); // VIND17, MS-BS, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 30)); // VIND19, MS-D0, Clearcc PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 31)); // VIND19, MS-D0, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 18)); // VIND16, MS-D1, Clearcc PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 19)); // VIND16, MS-D1, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 10)); // VIND14, MS-D2, Clearcc PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 11)); // VIND14, MS-D2, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 14)); // VIND15, MS-D3, Clearcc PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, (((UINT32)1) << 15)); // VIND15, MS-D3, Set PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = TRUE;
                break;
            }
            case 5: // multi-func 5
            {                 
                // (1) Multiple function
                // 1. 0x7000_00C4 bit[16:14] = 3
                u4Reg = CKGEN_READ32(0xC4);
                u4Reg &= ~((UINT32)0x07 << 14);
                u4Reg |=   ((UINT32)0x03 << 14);
                CKGEN_WRITE32(0xC4, u4Reg);            

                // Setup Pull down register
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 10)); // GPIO2, MS-BS, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 11)); // GPIO2, MS-BS, Set PD
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 6)); // GPIO1, MS-D0, Clear PU
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, (((UINT32)1) << 7)); // GPIO1, MS-D0, Set PD
                
                // 4 bit configuration
                _fgFCI4BitBusEnable = FALSE;
                break;
            }
            default:
            {
                break;
            }
        }
    }

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)

    if (i4Slot == SDC_SLOT)
    {
        if (_u4SDMultiFuncCfg == 1)
        {
            // Set pinmux select to FCI
            // On board connector for FCI pin set 1
            //CKGEN_WRITE32(REG_PINMUX_SEL1, 0x180003c0);

            // Disable SD Multi-function 1
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x07) << 6));            
            // Disable SD Multi-function 2
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));
            // Disable SD Multi-function 3
            CKGEN_CLRBIT(REG_PINMUX_SEL2, (((UINT32)0x07) << 11) | (((UINT32)0x07) << 8));
            
            // Enable SD Multi-function 1
            CKGEN_SETBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 6));  
            
            // pull high SD card pin.
            // PADPUCFG3	PAD PULL CONFIGURATION 3
            // PAD_GPIO5  sd_d3_0 
            // PAD_GPIO6  sd_d2_0 
            // PAD_GPIO7  sd_d1_0 
            // PAD_GPIO8  sd_d0_0 
            // PAD_GPIO9  sd_cmd_0        
            u4Reg = CKGEN_READ32(0x30C);      // For 5363
            u4Reg &= ~(((UINT32)0x0F) << 8);
            u4Reg |= (((UINT32)0x0A) << 8);
            CKGEN_WRITE32(0x30C, u4Reg);
        }
        else if (_u4SDMultiFuncCfg == 2)
        {
            // Set pinmux select to FCI
            // External fci daughter board for FCI pin set 2
            //CKGEN_WRITE32(REG_PINMUX_SEL1, 0x00A00000);            
            
            // Disable SD Multi-function 1
            // CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x07) << 6));            
            // Disable SD Multi-function 2
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));
            // Disable SD Multi-function 3
            // CKGEN_CLRBIT(REG_PINMUX_SEL2, (((UINT32)0x07) << 11) | (((UINT32)0x07) << 8));
            
            // Enable SD Multi-function 2
            CKGEN_SETBIT(REG_PINMUX_SEL1, (((UINT32)0x02) << 22) | (((UINT32)0x02) << 20));  
            // pull high SD card pin.
            // PAD_GPIO38    sd_d3(I/O) 
            // PAD_GPIO39    sd_d2(I/O) 
            // PAD_GPIO40    sd_d1(I/O) 
            // PAD_GPIO41    sd_d0(I/O) 
            // PAD_GPIO42    sd_cmd(I/O)
        }
        else if (_u4SDMultiFuncCfg == 3)
        {
            // Set pinmux select to FCI
            // CI pins for FCI pin set 3
            
            // Disable SD Multi-function 1
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x07) << 6));            
            // Disable SD Multi-function 2
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));
            // Disable SD Multi-function 3
            CKGEN_CLRBIT(REG_PINMUX_SEL2, (((UINT32)0x07) << 11) | (((UINT32)0x07) << 8));
            
            // Enable SD Multi-function 3
            CKGEN_SETBIT(REG_PINMUX_SEL2, (((UINT32)0x03) << 11) | (((UINT32)0x03) << 8));  
            
            // pull high SD card pin.
            // PAD_CI_MDO0	  sd_clk(O)  
            // PAD_CI_MOVAL	sd_d3(I/O) 
            // PAD_CI_MOSTRT	sd_d2(I/O) 
            // PAD_CI_MCLKO	sd_d1(I/O) 
            // PAD_CI_MISTRT	sd_d0(I/O) 
            // PAD_CI_MDI0	  sd_cmd(I/O)
            // PAD PULL Configuration for TSI group
            // PAD PULL Configuration for TSO group
            CKGEN_CLRBIT(0x300, (((UINT32)0x03) << 24) | (((UINT32)0x03) << 22));
            CKGEN_SETBIT(0x300, (((UINT32)0x02) << 24) | (((UINT32)0x02) << 22));    
        }
    }
    else if (i4Slot == MSP_SLOT)
    {
        if (_u4MSMultiFuncCfg == 1)
        {
            // Set pinmux select to FCI
            // On board connector for FCI pin set 1
            //CKGEN_WRITE32(REG_PINMUX_SEL1, 0x180003c0);
            
            // Disable SD Multi-function 1
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x07) << 8));
            // Disable SD Multi-function 2
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));
            // Disable SD Multi-function 3
            CKGEN_CLRBIT(REG_PINMUX_SEL2, (((UINT32)0x07) << 11) | (((UINT32)0x07) << 8));
            
            // Enable SD Multi-function 1
            CKGEN_SETBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 8));  
            
            // pull down MS card pin.
            // PAD PULL Configuration for GPIO12 group
            // PAD_GPIO12   ms_d3(I/O)
            // PAD_GPIO13   ms_d2(I/O)
            // PAD_GPIO14   ms_d1(I/O)
            // PAD_GPIO15   ms_d0(I/O)
        
            u4Reg = CKGEN_READ32(0x30C);      // For 5363
            u4Reg &= ~(((UINT32)0x03) << 14);
            u4Reg |= (((UINT32)0x01) << 14);
            CKGEN_WRITE32(0x30C, u4Reg);
        }
        else if (_u4MSMultiFuncCfg == 2) 
        {
            // Set pinmux select to FCI
            // External fci daughter board for FCI pin set 2
            //CKGEN_WRITE32(REG_PINMUX_SEL1, 0x00F00000);
            
            // Disable SD Multi-function 1
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x07) << 8));
            // Disable SD Multi-function 2
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));
            // Disable SD Multi-function 3
            CKGEN_CLRBIT(REG_PINMUX_SEL2, (((UINT32)0x07) << 11) | (((UINT32)0x07) << 8));
            
            // Enable SD Multi-function 2
            CKGEN_SETBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));  
           
            // pull down MS card pin.
            // PAD_GPIO38    ms_d3(I/O) 
            // PAD_GPIO39    ms_d2(I/O) 
            // PAD_GPIO40    ms_d1(I/O) 
            // PAD_GPIO41    ms_d0(I/O) 
        }
        else if (_u4MSMultiFuncCfg == 3)
        {
            // Set pinmux select to FCI
            // CI pins for FCI pin set 3
            
            // Disable SD Multi-function 1
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x07) << 8));
            // Disable SD Multi-function 2
            CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x03) << 22) | (((UINT32)0x03) << 20));
            // Disable SD Multi-function 3
            CKGEN_CLRBIT(REG_PINMUX_SEL2, (((UINT32)0x07) << 11) | (((UINT32)0x07) << 8));
            
            // Enable SD Multi-function 3
            CKGEN_SETBIT(REG_PINMUX_SEL2, (((UINT32)0x04) << 11) | (((UINT32)0x04) << 8));   
            
            // pull down MS card pin.
            // PAD_CI_MDO0	  ms_clk(O)   
            // PAD_CI_MOVAL	ms_d3(I/O)  
            // PAD_CI_MOSTRT	ms_d2(I/O)  
            // PAD_CI_MCLKO	ms_d1(I/O)  
            // PAD_CI_MISTRT	ms_d0(I/O)  
            // PAD_CI_MDI0	  ms_bs(O)  
            // PAD PULL Configuration for TSI group
            // PAD PULL Configuration for TSO group
            /*  => pull down by PCB, don't need to add pull down here
            CKGEN_CLRBIT(0x300, (((UINT32)0x03) << 24) | (((UINT32)0x03) << 22));
            CKGEN_SETBIT(0x300, (((UINT32)0x01) << 24) | (((UINT32)0x01) << 22));
            */
        }
    }

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5387)

    if (i4Slot == SDC_SLOT)
    {
        // Set pinmux select to FCI
        u4Reg = CKGEN_READ32(REG_PINMUX_SEL4);
        u4Reg &= ~(0x3U << 8);                                
        u4Reg |=   (0x4U << 8);
        CKGEN_WRITE32(REG_PINMUX_SEL4, u4Reg);

        // pull high SD card pin.
        // PAD PULL Configuration for PTSI group 2
        u4Reg = CKGEN_READ32(0x300);      // For 5387
        u4Reg &= ~(3U << 24);
        u4Reg |= (2U << 24);
        CKGEN_WRITE32(0x300, u4Reg);
    }
    else if (i4Slot == MSP_SLOT)
    {
    }

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5365)

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5395)
    if(i4Slot == SDC_SLOT)
    {
        CKGEN_CLRBIT(REG_PINMUX_SEL1, (((UINT32)0x3) << 20));
        CKGEN_SETBIT(REG_PINMUX_SEL1, (((UINT32)0x1) << 20));
    }
    else if (i4Slot == MSP_SLOT)
    {

    }

#endif
}

#if 0 // unused
//-------------------------------------------------------------------------
/** fgFCIHW_ReadBufFull
 *  Check Read fifo full or not
 *  @param VOID
 *  @retval  TRUE : read fifo full, FALSE : read fifo not full
 */
//-------------------------------------------------------------------------

BOOL fgFCIHW_ReadBufFull(VOID)
{
    return ((FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_FCI_RDFIFO_FULL) ? TRUE : FALSE);
}
#endif

//-------------------------------------------------------------------------
/** fgFCIHW_ReadBufEmpty
 *  Check Read fifo empty or not
 *  @param VOID
 *  @retval  TRUE : read fifo empty, FALSE : read fifo not empty
 */
//-------------------------------------------------------------------------

BOOL fgFCIHW_ReadBufEmpty(VOID)
{
    return ((FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_FCI_RDFIFO_EMPTY) ? TRUE : FALSE);
}

//-------------------------------------------------------------------------
/** fgFCIHW_WriteBufEmpty
 *  Check Write fifo empty or not
 *  @param VOID
 *  @retval  TRUE : write fifo empty, FALSE : write fifo not empty
 */
//-------------------------------------------------------------------------

BOOL fgFCIHW_WriteBufEmpty(VOID)
{
    return ((FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_FCI_WRFIFO_EMPTY) ? TRUE : FALSE);    
}

//-------------------------------------------------------------------------
/** fgFCIHW_WriteBufFull
 *  Check Write fifo full or not
 *  @param VOID
 *  @retval  TRUE : write fifo full, FALSE : write fifo not full
 */
//-------------------------------------------------------------------------

BOOL fgFCIHW_WriteBufFull(VOID)
{
    return ((FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_FCI_WRFIFO_FULL) ? TRUE : FALSE);    
}

//-------------------------------------------------------------------------
/** FCIHW_Init
 *  Set clock and reset FCI interface.
 *  @param VOID
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 FCIHW_Init(VOID)
{
    UINT32 u4Val;

    FCI_WRITE32(RW_FCI_CTRL_REG, 0);
    _Fcihw_ResetFifo();
    _Fcihw_ResetInt();

    /* Reset SD/MS/SM controller. */
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    u4Val &= ~(CTRL_SD_RST | CTRL_MS_RST | CTRL_SM_RST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_SD_RST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_MS_RST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);

#ifdef CC_FCI_SMXD_SUPPORT
#ifndef SMXD_USING_NAND_IF
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_SM_RST);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

    /* Set the initialized value of all FCI registers. */
    FCI_WRITE32(RW_MS_CTRL_REG, 0);
    FCI_WRITE32(RW_SD_CTRL_REG, 0);

#ifdef CC_FCI_SMXD_SUPPORT
#ifndef SMXD_USING_NAND_IF
    FCI_WRITE32(RW_SM_CTRL_REG, 0);
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

    FCI_WRITE32(RW_FCI_CTRL_REG, FCI_MS_BSYCNT(0));
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, FCI_SEL_SD | FCI_SRAMSEL(3));
    FCI_WRITE32(RW_SD_BUSYCNT_REG,
        MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);

    return S_OK;
}

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)

#ifndef MT8520_USE_SD2
//-------------------------------------------------------------------------
/** _Fcihw_SetSD1Clock
 *  Set SD clock
 *  @param i4SDSrcSel    Clock source selection.
 *  @param i4SDDivSel    Clock Div selection.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static void _Fcihw_SetSD1Clock(INT32 i4SDSrcSel, INT32 i4SDDivSel)
{
    UINT32 u4Val;

    ASSERT(i4SDSrcSel >= CLK_SEL_3_SD_27M);
    ASSERT(i4SDSrcSel <= CLK_SEL_3_SD_SYSPLL2_1_4);

    ASSERT(i4SDDivSel >= CLK_SEL_3_SD_DIV_27M);
    ASSERT(i4SDDivSel <= CLK_SEL_3_SD_DIV_27M_1_18);

    // Read register setting
    u4Val = CKGEN_READ32(REG_RW_CLK_SEL_3);

    // Apply new setting
    u4Val &= ~(CLK_SEL_3_SD_MASK | CLK_SEL_3_SD_DIV_MASK);
    u4Val |= ((i4SDSrcSel << 2) | (i4SDDivSel << 4));

    // Write back the value
    CKGEN_WRITE32(REG_RW_CLK_SEL_3, u4Val);
}

//-------------------------------------------------------------------------
/** _Fcihw_SetMSClock
 *  Set SD2 clock
 *  @param i4MSSrcSel    Clock source selection.
 *  @param i4MSDivSel    Clock Div selection.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static void _Fcihw_SetMSClock(INT32 i4MSSrcSel, INT32 i4MSDivSel)
{
    UINT32 u4Val;

    ASSERT(i4MSSrcSel >= CLK_SEL_3_MS_27M);
    ASSERT(i4MSSrcSel <= CLK_SEL_3_MS_SYSPLL2_1_5);

    ASSERT(i4MSDivSel >= CLK_SEL_3_MS_DIV_27M);
    ASSERT(i4MSDivSel <= CLK_SEL_3_MS_DIV_27M_1_18);

    // Read register setting
    u4Val = CKGEN_READ32(REG_RW_CLK_SEL_3);

    // Apply new setting
    u4Val &= ~(CLK_SEL_3_MS_MASK | CLK_SEL_3_MS_DIV_MASK);
    u4Val |= ((i4MSSrcSel << 12) | (i4MSDivSel << 8));

    // Write back the value
    CKGEN_WRITE32(REG_RW_CLK_SEL_3, u4Val);
}

#else

//-------------------------------------------------------------------------
/** _Fcihw_SetSD2Clock
 *  Set SD2 clock
 *  @param i4SD2SrcSel    Clock source selection.
 *  @param i4SD2DivSel    Clock Div selection.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
static void _Fcihw_SetSD2Clock(INT32 i4SD2SrcSel, INT32 i4SD2DivSel)
{
    UINT32 u4Val;

    ASSERT(i4SD2SrcSel >= CLK_SEL_3_SD2_27M);
    ASSERT(i4SD2SrcSel <= CLK_SEL_3_SD2_SYSPLL2_1_2);

    ASSERT(i4SD2DivSel >= CLK_SEL_3_SD2_DIV_27M);
    ASSERT(i4SD2DivSel <= CLK_SEL_3_SD2_DIV_27M_1_18);

    // Read register setting
    u4Val = CKGEN_READ32(REG_RW_CLK_SEL_3);

    // Apply new setting
    u4Val &= ~(CLK_SEL_3_SD2_MASK | CLK_SEL_3_SD2_DIV_MASK);
    u4Val |= ((i4SD2SrcSel << 14) | (i4SD2DivSel<<16));

    // Write back the value
    CKGEN_WRITE32(REG_RW_CLK_SEL_3, u4Val);
}
#endif // #ifdef MT8520_USE_SD2  
#endif  // #if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)



#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)
//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4SDMultiFuncCfg)
    {
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // E2+E4 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<24) | ((UINT32)1 <<25)); // E2+E4 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // E2+E4 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // E2+E4 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // E2+E4 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // E2+E4 for D3 pin                                
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<16)); // E2 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<24)); // E2 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<20)); // E2 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<4)); // E2 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<0)); // E2 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<28)); // E2 for D3 pin   

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<17)); // E2 for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<25)); // E2 for CMD pin                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<21)); // E2 for D0 pin                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<5)); // E2 for D1 pin                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<1)); // E2 for D2 pin                                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<29)); // E2 for D3 pin
            }
        }
        break;

        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // E2+E4 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // E2+E4 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // E2+E4 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // E2+E4 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // E2+E4 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<9) | ((UINT32)1 <<10)); // E2+E4 for D3 pin                                
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<16)); // E2 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<4)); // E2 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<20)); // E2 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<0)); // E2 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<14)); // E2 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<9)); // E2 for D3 pin   

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<17)); // E2 for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<5)); // E2 for CMD pin                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<21)); // E2 for D0 pin                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<1)); // E2 for D1 pin                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<15)); // E2 for D2 pin                                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<10)); // E2 for D3 pin 
            }
        }
        break;
                
        case 3:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // E2+E4 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // E2+E4 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // E2+E4 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // E2+E4 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // E2+E4 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4) | ((UINT32)1 <<5)); /// E2+E4 for D3 pin                                
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<16)); // E2 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<12)); // E2 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<20)); // E2 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8)); // E2 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0)); // E2 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4)); // E2 for D3 pin   

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<17)); // E2 for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<13)); // E2 for CMD pin                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<21)); // E2 for D0 pin                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<9)); // E2 for D1 pin                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<1)); // E2 for D2 pin                                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<5)); // E2 for D3 pin 
            }
        }
        break;
                
        case 4:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<24) | ((UINT32)1 <<25)); // E2+E4 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // E2+E4 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // E2+E4 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // E2+E4 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // E2+E4 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // E2+E4 for D3 pin                                
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<24)); // E2 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<20)); // E2 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<28)); // E2 for D0 pin                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<16)); // E2 for D1 pin                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<8)); // E2 for D2 pin                                                                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<12)); // E2 for D3 pin   

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<25)); // E2 for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<21)); // E2 for CMD pin                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<29)); // E2 for D0 pin                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<17)); // E2 for D1 pin                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<9)); // E2 for D2 pin                                                                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<13)); // E2 for D3 pin 
            }
        }
        break;
        
        case 5:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // E2+E4 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // E2+E4 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // E2+E4 for D0 pin                              
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0)); // E2 for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8)); // E2 for CMD pin                
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4)); // E2 for D0 pin  

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<1)); // E2 for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<9)); // E2 for CMD pin                
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<5)); // E2 for D0 pin  
            }
        }
        break;
    }
}


//-------------------------------------------------------------------------
/** FCIHW_MSDrivingCurrent
 *  Set MS clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4MSMultiFuncCfg)
    {
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<24) | ((UINT32)1 <<25)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // for D3 pin
            }
            else if (i4ClkMHz >= 10)
            {
                // Set driving current E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<17)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<25)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<21)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<5)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<1)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<29)); // for D3 pin

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<16)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<24)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<20)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<4)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<0)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<28)); // for D3 pin
            }
            else
            {
                // Set driving current E4
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<17)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<25)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<21)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<5)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<1)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<29)); // for D3 pin

                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<16)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<24)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<20)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<4)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<0)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_6, ((UINT32)1 <<28)); // for D3 pin
            }
        }
        break;

        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<9) | ((UINT32)1 <<10)); // for D3 pin
            }
            else if (i4ClkMHz >= 10)
            {
                // Set driving current E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<17)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<5)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<21)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<1)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<15)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<10)); // for D3 pin

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<16)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<4)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<20)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<0)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<14)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<9)); // for D3 pin

            }
            else
            {
                // Set driving current E4
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<17)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<5)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<21)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<1)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<15)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<10)); // for D3 pin

                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<16)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<4)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_0, ((UINT32)1 <<20)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<0)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<14)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_1, ((UINT32)1 <<9)); // for D3 pin
            }
        }
        break;

        case 3:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // for D3 pin
            }
            else if (i4ClkMHz >= 10)
            {
                // Set driving current E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<17)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<13)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<21)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<9)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<1)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<5)); // for D3 pin

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<16)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<12)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<20)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4)); // for D3 pin
            }
            else
            {
                // Set driving current E4
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<17)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<13)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<21)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<9)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<1)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<5)); // for D3 pin

                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<16)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<12)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<20)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4)); // for D3 pin
            }
        }
        break;

        case 4:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<24) | ((UINT32)1 <<25)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<16) | ((UINT32)1 <<17)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // for D3 pin
            }
            else if (i4ClkMHz >= 10)
            {
                // Set driving current E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<25)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<21)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<29)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<17)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<9)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<13)); // for D3 pin

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<24)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<20)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<28)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<16)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<8)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<12)); // for D3 pin

            }
            else
            {
                // Set driving current E4
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<25)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<21)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<29)); // for D0 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<17)); // for D1 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<9)); // for D2 pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<13)); // for D3 pin

                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<24)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<20)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<28)); // for D0 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<16)); // for D1 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<8)); // for D2 pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_7, ((UINT32)1 <<12)); // for D3 pin
            }
        }
        break;

        case 5:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // for D0 pin
            }
            else if (i4ClkMHz >= 10)
            {
                // Set driving current E8
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<1)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<9)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<5)); // for D0 pin

                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4)); // for D0 pin

            }
            else
            {
                // Set driving current E4
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<1)); // for CLK pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<9)); // for BS pin
                CKGEN_CLRBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<5)); // for D0 pin

                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<0)); // for CLK pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<8)); // for BS pin
                CKGEN_SETBIT(REG_RW_PAD_CTRL_10, ((UINT32)1 <<4)); // for D0 pin
            }
        }
        break;
    }
}

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)
//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{ //mhzhang
    switch(_u4SDMultiFuncCfg)
    {
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                // Set driving current E4+E8
                PDWNC_SETBIT(0xE4, ((UINT32)1<<13));  // E2 for CLK pin
                PDWNC_SETBIT(0xE8, ((UINT32)1<<13));  // E4 for CLK pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<20));  // E2 for CMD pin
                PDWNC_SETBIT(0xE8, ((UINT32)1<<20));  // E4 for CMD pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<22));  // E2 for D0 pin
                PDWNC_SETBIT(0xE8, ((UINT32)1<<22));  // E4 for D0 pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<17));  // E2 for D1 pin
                PDWNC_SETBIT(0xE8, ((UINT32)1<<17));  // E4 for D1 pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<18));  // E2 for D2 pin
                PDWNC_SETBIT(0xE8, ((UINT32)1<<18));  // E4 for D2 pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<19));  // E2 for D3 pin
                PDWNC_SETBIT(0xE8, ((UINT32)1<<19));  // E4 for D3 pin 			
            }
            else
            {
                // Set driving current E2
                PDWNC_SETBIT(0xE4, ((UINT32)1<<13));  // E2 for CLK pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<20));  // E2 for CMD pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<22));  // E2 for D0 pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<17));  // E2 for D1 pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<18));  // E2 for D2 pin
                PDWNC_SETBIT(0xE4, ((UINT32)1<<19));  // E2 for D3 pin
		 // clear driving current E4
                PDWNC_CLRBIT(0xE8, ((UINT32)1<<13));  // E4 for CLK pin
                PDWNC_CLRBIT(0xE8, ((UINT32)1<<20));  // E4 for CMD pin
                PDWNC_CLRBIT(0xE8, ((UINT32)1<<22));  // E4 for D0 pin
                PDWNC_CLRBIT(0xE8, ((UINT32)1<<17));  // E4 for D1 pin
                PDWNC_CLRBIT(0xE8, ((UINT32)1<<18));  // E4 for D2 pin
                PDWNC_CLRBIT(0xE8, ((UINT32)1<<19));  // E4 for D3 pin 
            }
        }
        break;

        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<4) | ((UINT32)1 <<5));    // E2+E4 for CLK pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<2) | ((UINT32)1 <<3));    // E2+E4 for CMD pin
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<10) | ((UINT32)1 <<11)); // E2+E4 for D0 pin
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // E2+E4 for D1 pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // E2+E4 for D2 pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<8) | ((UINT32)1 <<9));    // E2+E4 for D3 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<4));   // E2 for CLK pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<2));   // E2 for CMD pin
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<10)); // E2 for D0 pin
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<28)); // E2 for D1 pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<14)); // E2 for D2 pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<8));  // E2 for D3 pin
               // clear driving current E4
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<5));   // E4 for CLK pin
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<3));   // E4 for CMD pin
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<11)); // E4 for D0 pin
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<29)); // E4 for D1 pin
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<15)); // E4 for D2 pin
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<9));  // E4 for D3 pin
            }
        }
        break;

        case 3:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<22) | ((UINT32)1 <<23));  // E2+E4 for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<16) | ((UINT32)1 <<17));  // E2+E4 for CMD pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<28) | ((UINT32)1 <<29));  // E2+E4 for D0 pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10) | ((UINT32)1 <<11));  // E2+E4 for D1 pin
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30) | ((UINT32)1 <<31));  // E2+E4 for D2 pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4) | ((UINT32)1 <<5));     // E2+E4 for D3 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<22)); // E2 for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<16)); // E2 for CMD pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<28)); // E2 for D0 pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10)); // E2 for D1 pin
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30)); // E2 for D2 pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4));  // E2 for D3 pin
               // clear drving current E4
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<23)); // E4 for CLK pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<17)); // E4 for CMD pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<29)); // E4 for D0 pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<11)); // E4 for D1 pin
                CKGEN_CLRBIT(0x0124, ((UINT32)1 <<31)); // E4 for D2 pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<5));  // E4 for D3 pin
            }
        }
        break;

        case 4:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<20) | ((UINT32)1 <<21));  // E2+E4 for CLK pin
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<26) | ((UINT32)1 <<27));  // E2+E4 for CMD pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<18) | ((UINT32)1 <<19));  // E2+E4 for D0 pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<12) | ((UINT32)1 <<13));  // E2+E4 for D1 pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<0) | ((UINT32)1 <<1));     // E2+E4 for D2 pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<6) | ((UINT32)1 <<7));     // E2+E4 for D3 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<20));  // E2 for CLK pin
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<26));  // E2 for CMD pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<18));  // E2 for D0 pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<12));  // E2 for D1 pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<0));   // E2 for D2 pin
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<6));   // E2 for D3 pin
               //clear drving current E4
                CKGEN_CLRBIT(0x0114, ((UINT32)1 <<21));  // E4 for CLK pin
                CKGEN_CLRBIT(0x0114, ((UINT32)1 <<27));  // E4 for CMD pin
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<19));  // E4 for D0 pin
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<13));  // E4 for D1 pin
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<1));   // E4 for D2 pin
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<7));   // E4 for D3 pin
            }
        }
        break;

        case 5:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30) | ((UINT32)1 <<31)); // E2+E4 for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10) | ((UINT32)1 <<11)); // E2+E4 for CMD pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // E2+E4 for D0 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30)); // E2 for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10)); // E2 for CMD pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4)); // E2 for D0 pin

                CKGEN_CLRBIT(0x0124, ((UINT32)1 <<31)); // E2 for CLK pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<11)); // E2 for CMD pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<5)); // E2 for D0 pin
            }
        }
        break;
    }
}


//-------------------------------------------------------------------------
/** FCIHW_MSDrivingCurrent
 *  Set MS clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4MSMultiFuncCfg)
    {    
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<13));  //E2 for CLK
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<20)); //E2 for BS
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<22)); //E2 for D0
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<17));	 //E2 for D1
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<18)); //E2 for D2
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<19)); //E2 for D3
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<13)); //E4 for CLK
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<20)); //E4 for BS
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<22)); //E4 for D0
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<17));	//E4 for D1
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<18)); //E4 for D2
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<19));	 //E4 for D3	 
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
		 PDWNC_CLRBIT(0x0E4, (((UINT32)1)<<13));  //E2 for CLK
		 PDWNC_CLRBIT(0x0E4, (((UINT32)1)<<20)); //E2 for BS
		 PDWNC_CLRBIT(0x0E4, (((UINT32)1)<<22)); //E2 for D0
		 PDWNC_CLRBIT(0x0E4, (((UINT32)1)<<17));	 //E2 for D1
		 PDWNC_CLRBIT(0x0E4, (((UINT32)1)<<18)); //E2 for D2
		 PDWNC_CLRBIT(0x0E4, (((UINT32)1)<<19)); //E2 for D3
		 // set drving current E4
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<13)); //E4 for CLK
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<20)); //E4 for BS
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<22)); //E4 for D0
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<17));	//E4 for D1
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<18)); //E4 for D2
		 PDWNC_SETBIT(0x0E8, (((UINT32)1)<<19));	 //E4 for D3                                                  
            }
            else
            {
                // Set driving current E2
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<13));  //E2 for CLK
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<20)); //E2 for BS
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<22)); //E2 for D0
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<17));	 //E2 for D1
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<18)); //E2 for D2
		 PDWNC_SETBIT(0x0E4, (((UINT32)1)<<19)); //E2 for D3
		 // clear driving current E4
		 PDWNC_CLRBIT(0x0E8, (((UINT32)1)<<13)); //E4 for CLK
		 PDWNC_CLRBIT(0x0E8, (((UINT32)1)<<20)); //E4 for BS
		 PDWNC_CLRBIT(0x0E8, (((UINT32)1)<<22)); //E4 for D0
		 PDWNC_CLRBIT(0x0E8, (((UINT32)1)<<17));	//E4 for D1
		 PDWNC_CLRBIT(0x0E8, (((UINT32)1)<<18)); //E4 for D2
		 PDWNC_CLRBIT(0x0E8, (((UINT32)1)<<19));	 //E4 for D3	                
            }
        }
        break;
        
        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2/E4+E4/E8
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<4) | ((UINT32)1 <<5));    // E4+E8  for CLK pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<2) | ((UINT32)1 <<3));    // E2+E4 for BS pin                
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<10) | ((UINT32)1 <<11)); // E4+E8 for D0 pin                                
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // E4+E8 for D1 pin                                                
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // E2+E4 for D2 pin                                                                
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<8) | ((UINT32)1 <<9));   // E2+E4 for D3 pin                                
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2/E4
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<4));   // for CLK pin
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<2));   // for BS pin 
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<10)); // for D0 pin             
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<28)); // for D1 pin                             
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<14)); // for D2 pin                                             
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<8));  // for D3 pin 

		// set drving current E4/E8
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<5));   // for CLK pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<3));   // for BS pin 
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<11)); // for D0 pin             
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<29)); // for D1 pin                             
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<15)); // for D2 pin                                             
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<9));  // for D3 pin 
                                              
            }
            else
            {
                // Set driving current E2/E4
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<4));   // for CLK pin
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<2));   // for BS pin 
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<10)); // for D0 pin             
                CKGEN_SETBIT(0x0104, ((UINT32)1 <<28)); // for D1 pin                             
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<14)); // for D2 pin                                             
                CKGEN_SETBIT(0x0108, ((UINT32)1 <<8));  // for D3 pin 

		// clear drving current E4/E8
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<5));   // for CLK pin
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<3));   // for BS pin 
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<11)); // for D0 pin             
                CKGEN_CLRBIT(0x0104, ((UINT32)1 <<29)); // for D1 pin                             
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<15)); // for D2 pin                                             
                CKGEN_CLRBIT(0x0108, ((UINT32)1 <<9));  // for D3 pin 
    
            }
        }
        break;
        
        case 3:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<22) | ((UINT32)1 <<23));  // for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<16) | ((UINT32)1 <<17));  // for BS pin                
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<28) | ((UINT32)1 <<29));  // for D0 pin                                
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10) | ((UINT32)1 <<11));  // for D1 pin                                                
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30) | ((UINT32)1 <<31));  // for D2 pin                                                                
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4) | ((UINT32)1 <<5));     // for D3 pin                                
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<22)); // for CLK pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<16)); // for BS pin 
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<28)); // for D0 pin             
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<10)); // for D1 pin                             
                CKGEN_CLRBIT(0x0124, ((UINT32)1 <<30)); // for D2 pin                                             
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<4)); // for D3 pin 
                // set drving curent E4
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<23)); // for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<17)); // for BS pin 
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<29)); // for D0 pin             
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<11)); // for D1 pin                             
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<31)); // for D2 pin                                             
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<5)); // for D3 pin               
                                                     
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<22)); // for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<16)); // for BS pin 
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<28)); // for D0 pin             
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10)); // for D1 pin                             
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30)); // for D2 pin                                             
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4)); // for D3 pin 
                // clear drving curent E4
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<23)); // for CLK pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<17)); // for BS pin 
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<29)); // for D0 pin             
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<11)); // for D1 pin                             
                CKGEN_CLRBIT(0x0124, ((UINT32)1 <<31)); // for D2 pin                                             
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<5)); // for D3 pin         
            }
        }
        break;
        
        case 4:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // for CLK pin
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<26) | ((UINT32)1 <<27)); // for BS pin                
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<18) | ((UINT32)1 <<19)); // for D0 pin                                
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // for D1 pin                                                
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<0) | ((UINT32)1 <<1));    // for D2 pin                                                                
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<6) | ((UINT32)1 <<7));    // for D3 pin                                
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
                CKGEN_CLRBIT(0x0114, ((UINT32)1 <<20)); // for CLK pin
                CKGEN_CLRBIT(0x0114, ((UINT32)1 <<26)); // for BS pin 
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<18)); // for D0 pin             
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<12)); // for D1 pin                             
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<0)); // for D2 pin                                             
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<6)); // for D3 pin 
                // set drving current E4
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<21)); // for CLK pin
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<27)); // for BS pin 
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<19)); // for D0 pin             
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<13)); // for D1 pin                             
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<1)); // for D2 pin                                             
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<7)); // for D3 pin                                               
            }
            else
            {
                // Set driving current E2                
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<20)); // for CLK pin
                CKGEN_SETBIT(0x0114, ((UINT32)1 <<26)); // for BS pin 
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<18)); // for D0 pin             
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<12)); // for D1 pin                             
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<0)); // for D2 pin                                             
                CKGEN_SETBIT(0x0118, ((UINT32)1 <<6)); // for D3 pin 
               // clear driving current E4
                CKGEN_CLRBIT(0x0114, ((UINT32)1 <<21)); // for CLK pin
                CKGEN_CLRBIT(0x0114, ((UINT32)1 <<27)); // for BS pin 
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<19)); // for D0 pin             
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<13)); // for D1 pin                             
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<1)); // for D2 pin                                             
                CKGEN_CLRBIT(0x0118, ((UINT32)1 <<7)); // for D3 pin 
            }
        }
        break;
        
        case 5:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E4+E8
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30) | ((UINT32)1 <<31)); // for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10) | ((UINT32)1 <<11)); // for BS pin                
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // for D0 pin                             
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
                CKGEN_CLRBIT(0x0124, ((UINT32)1 <<30)); // for CLK pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<10)); // for BS pin 
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<4)); // for D0 pin     
                // set driving current E4
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<31)); // for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<11)); // for BS pin 
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<5)); // for D0 pin       
                                              
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x0124, ((UINT32)1 <<30)); // for CLK pin
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<10)); // for BS pin 
                CKGEN_SETBIT(0x0128, ((UINT32)1 <<4)); // for D0 pin     
                // clear drving current E4
                CKGEN_CLRBIT(0x0124, ((UINT32)1 <<31)); // for CLK pin
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<11)); // for BS pin 
                CKGEN_CLRBIT(0x0128, ((UINT32)1 <<5)); // for D0 pin    
            }
        }
        break;
    }
}

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)
//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{ // mhzhang
    switch(_u4SDMultiFuncCfg)
    {
        case 1: // multi-funciton 1
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x150, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // E2+E4 for CLK pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<2) | ((UINT32)1 <<3)); // E2+E4 for CMD pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // E2+E4 for D0 pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<21) | ((UINT32)1 <<22)); // E2+E4 for D1 pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<26) | ((UINT32)1 <<27)); // E2+E4 for D2 pin
                CKGEN_SETBIT(0x154, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // E2+E4 for D3 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x150, ((UINT32)1 <<8)); // E2 for CLK pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<2)); // E2 for CMD pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<14)); // E2 for D0 pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<21)); // E2 for D1 pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<26)); // E2 for D2 pin
                CKGEN_SETBIT(0x154, ((UINT32)1 <<0)); // E2 for D3 pin

                CKGEN_CLRBIT(0x150, ((UINT32)1 <<9)); // E4 for CLK pin
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<3)); // E4 for CMD pin
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<15)); // E4 for D0 pin
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<22)); // E4 for D1 pin
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<27)); // E4 for D2 pin
                CKGEN_CLRBIT(0x154, ((UINT32)1 <<1)); // E4 for D3 pin
            }
        }
        break;

        case 2: // multi-funciton 2
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x104, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // E2+E4 for CLK pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<2) | ((UINT32)1 <<3)); // E2+E4 for CMD pin
                CKGEN_SETBIT(0x104, ((UINT32)1 <<10) | ((UINT32)1 <<11)); // E2+E4 for D0 pin
                CKGEN_SETBIT(0x104, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // E2+E4 for D1 pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // E2+E4 for D2 pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // E2+E4 for D3 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x104, ((UINT32)1 <<4)); // E2 for CLK pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<2)); // E2 for CMD pin
                CKGEN_SETBIT(0x104, ((UINT32)1 <<10)); // E2 for D0 pin
                CKGEN_SETBIT(0x104, ((UINT32)1 <<28)); // E2 for D1 pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<14)); // E2 for D2 pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<8)); // E2 for D3 pin

                CKGEN_CLRBIT(0x104, ((UINT32)1 <<5)); // E2 for CLK pin
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<3)); // E2 for CMD pin
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<11)); // E2 for D0 pin
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<29)); // E2 for D1 pin
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<15)); // E2 for D2 pin
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<9)); // E2 for D3 pin
            }
        }
        break;

        case 3: // multi-function 3
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x114, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // E2+E4 for CLK pin
                CKGEN_SETBIT(0x114, ((UINT32)1 <<26) | ((UINT32)1 <<27)); // E2+E4 for CMD pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<18) | ((UINT32)1 <<19)); // E2+E4 for D0 pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // E2+E4 for D1 pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // E2+E4 for D2 pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<6) | ((UINT32)1 <<7)); /// E2+E4 for D3 pin
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x118, ((UINT32)1 <<20)); // E2 for CLK pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<26)); // E2 for CMD pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<18)); // E2 for D0 pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<12)); // E2 for D1 pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<0)); // E2 for D2 pin
                CKGEN_SETBIT(0x118, ((UINT32)1 <<6)); // E2 for D3 pin

                CKGEN_CLRBIT(0x118, ((UINT32)1 <<21)); // E2 for CLK pin
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<27)); // E2 for CMD pin
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<19)); // E2 for D0 pin
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<13)); // E2 for D1 pin
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<1)); // E2 for D2 pin
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<7)); // E2 for D3 pin
            }
        }
        break;
    }
}


//-------------------------------------------------------------------------
/** FCIHW_MSDrivingCurrent
 *  Set MS clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4MSMultiFuncCfg)
    {    
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x150, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // for CLK pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<2) | ((UINT32)1 <<3)); // for BS pin                
                CKGEN_SETBIT(0x150, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // for D0 pin                                
                CKGEN_SETBIT(0x150, ((UINT32)1 <<21) | ((UINT32)1 <<22)); // for D1 pin                                                
                CKGEN_SETBIT(0x150, ((UINT32)1 <<26) | ((UINT32)1 <<27)); // for D2 pin                                                                
                CKGEN_SETBIT(0x154, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // for D3 pin                                
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<8)); // for CLK pin
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<2)); // for BS pin 
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<14)); // for D0 pin             
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<21)); // for D1 pin                             
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<26)); // for D2 pin                                             
                CKGEN_CLRBIT(0x154, ((UINT32)1 <<0)); // for D3 pin 
                // set drivng current E4
                CKGEN_SETBIT(0x150, ((UINT32)1 <<9)); // for CLK pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<3)); // for BS pin 
                CKGEN_SETBIT(0x150, ((UINT32)1 <<15)); // for D0 pin        
                CKGEN_SETBIT(0x150, ((UINT32)1 <<22)); // for D1 pin                        
                CKGEN_SETBIT(0x150, ((UINT32)1 <<27)); // for D2 pin                                        
                CKGEN_SETBIT(0x154, ((UINT32)1 <<1)); // for D3 pin                                                      
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x150, ((UINT32)1 <<8)); // for CLK pin
                CKGEN_SETBIT(0x150, ((UINT32)1 <<2)); // for BS pin 
                CKGEN_SETBIT(0x150, ((UINT32)1 <<14)); // for D0 pin             
                CKGEN_SETBIT(0x150, ((UINT32)1 <<21)); // for D1 pin                             
                CKGEN_SETBIT(0x150, ((UINT32)1 <<26)); // for D2 pin                                             
                CKGEN_SETBIT(0x154, ((UINT32)1 <<0)); // for D3 pin
                // clear drving current E4
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<9)); // for CLK pin
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<3)); // for BS pin 
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<15)); // for D0 pin        
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<22)); // for D1 pin                        
                CKGEN_CLRBIT(0x150, ((UINT32)1 <<27)); // for D2 pin                                        
                CKGEN_CLRBIT(0x154, ((UINT32)1 <<1)); // for D3 pin                  
            }
        }
        break;
        
        case 2: // multi-funciton 2
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x104, ((UINT32)1 <<4) | ((UINT32)1 <<5)); // for CLK pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<2) | ((UINT32)1 <<3)); // for BS pin                
                CKGEN_SETBIT(0x104, ((UINT32)1 <<10) | ((UINT32)1 <<11)); // for D0 pin                                
                CKGEN_SETBIT(0x104, ((UINT32)1 <<28) | ((UINT32)1 <<29)); // for D1 pin                                                
                CKGEN_SETBIT(0x108, ((UINT32)1 <<14) | ((UINT32)1 <<15)); // for D2 pin                                                                
                CKGEN_SETBIT(0x108, ((UINT32)1 <<8) | ((UINT32)1 <<9)); // for D3 pin                                
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<4)); // for CLK pin
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<2)); // for BS pin 
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<10)); // for D0 pin             
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<28)); // for D1 pin                             
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<14)); // for D2 pin                                             
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<8)); // for D3 pin 
                // set drving current E4
                CKGEN_SETBIT(0x104, ((UINT32)1 <<5)); // for CLK pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<3)); // for BS pin 
                CKGEN_SETBIT(0x104, ((UINT32)1 <<11)); // for D0 pin        
                CKGEN_SETBIT(0x104, ((UINT32)1 <<29)); // for D1 pin                        
                CKGEN_SETBIT(0x108, ((UINT32)1 <<15)); // for D2 pin                                        
                CKGEN_SETBIT(0x108, ((UINT32)1 <<9)); // for D3 pin        
                                              
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x104, ((UINT32)1 <<4)); // for CLK pin
                CKGEN_SETBIT(0x108, ((UINT32)1 <<2)); // for BS pin 
                CKGEN_SETBIT(0x104, ((UINT32)1 <<10)); // for D0 pin             
                CKGEN_SETBIT(0x104, ((UINT32)1 <<28)); // for D1 pin                             
                CKGEN_SETBIT(0x108, ((UINT32)1 <<14)); // for D2 pin                                             
                CKGEN_SETBIT(0x108, ((UINT32)1 <<8)); // for D3 pin 
                // clear driving current E4
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<5)); // for CLK pin
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<3)); // for BS pin 
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<11)); // for D0 pin        
                CKGEN_CLRBIT(0x104, ((UINT32)1 <<29)); // for D1 pin                        
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<15)); // for D2 pin                                        
                CKGEN_CLRBIT(0x108, ((UINT32)1 <<9)); // for D3 pin     
            }
        }
        break;
        
        case 3: // multi-funciton 3
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
                // Set driving current E2+E4
                CKGEN_SETBIT(0x114, ((UINT32)1 <<20) | ((UINT32)1 <<21)); // for CLK pin
                CKGEN_SETBIT(0x114, ((UINT32)1 <<26) | ((UINT32)1 <<27)); // for BS pin                
                CKGEN_SETBIT(0x118, ((UINT32)1 <<28) | ((UINT32)1 <<19)); // for D0 pin                                
                CKGEN_SETBIT(0x118, ((UINT32)1 <<12) | ((UINT32)1 <<13)); // for D1 pin                                                
                CKGEN_SETBIT(0x118, ((UINT32)1 <<0) | ((UINT32)1 <<1)); // for D2 pin                                                                
                CKGEN_SETBIT(0x118, ((UINT32)1 <<6) | ((UINT32)1 <<7)); // for D3 pin                                
            }
            else if (i4ClkMHz >= 10)
            {
                // clear driving current E2
                CKGEN_CLRBIT(0x114, ((UINT32)1 <<20)); // for CLK pin
                CKGEN_CLRBIT(0x114, ((UINT32)1 <<26)); // for BS pin 
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<18)); // for D0 pin             
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<12)); // for D1 pin                             
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<0)); // for D2 pin                                             
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<6)); // for D3 pin 
                // set  driving current E4
                CKGEN_SETBIT(0x114, ((UINT32)1 <<21)); // for CLK pin
                CKGEN_SETBIT(0x114, ((UINT32)1 <<27)); // for BS pin 
                CKGEN_SETBIT(0x118, ((UINT32)1 <<19)); // for D0 pin        
                CKGEN_SETBIT(0x118, ((UINT32)1 <<13)); // for D1 pin                        
                CKGEN_SETBIT(0x118, ((UINT32)1 <<1)); // for D2 pin                                        
                CKGEN_SETBIT(0x118, ((UINT32)1 <<7)); // for D3 pin                                                       
            }
            else
            {
                // Set driving current E2
                CKGEN_SETBIT(0x114, ((UINT32)1 <<20)); // for CLK pin
                CKGEN_SETBIT(0x114, ((UINT32)1 <<26)); // for BS pin 
                CKGEN_SETBIT(0x118, ((UINT32)1 <<18)); // for D0 pin             
                CKGEN_SETBIT(0x118, ((UINT32)1 <<12)); // for D1 pin                             
                CKGEN_SETBIT(0x118, ((UINT32)1 <<0)); // for D2 pin                                             
                CKGEN_SETBIT(0x118, ((UINT32)1 <<6)); // for D3 pin 
                // clear  driving current E4
                CKGEN_CLRBIT(0x114, ((UINT32)1 <<21)); // for CLK pin
                CKGEN_CLRBIT(0x114, ((UINT32)1 <<27)); // for BS pin 
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<19)); // for D0 pin        
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<13)); // for D1 pin                        
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<1)); // for D2 pin                                        
                CKGEN_CLRBIT(0x118, ((UINT32)1 <<7)); // for D3 pin     
            }
        }
        break;
    }
}

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)
//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{
    if (_u4SDMultiFuncCfg == 1)
    {
        // PAD_GPIO5  sd_d3_0 
        // PAD_GPIO6  sd_d2_0 
        // PAD_GPIO7  sd_d1_0 
        // PAD_GPIO8  sd_d0_0 
        // PAD_GPIO9  sd_cmd_0     
        switch(i4ClkMHz)
        {
            case 54:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x03) << 6) | (((UINT32)0x03) << 8) | (((UINT32)0x03) << 10));
            }
            break;
            case 45:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x03) << 6) | (((UINT32)0x03) << 8) | (((UINT32)0x03) << 10));
            }
            break;
            case 33:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;
            case 30:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;
            case 27:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;
            case 13:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;           
            case 9:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;              
            case 6:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;              
            case 3:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;              
            case 1:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;                 
            default:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x00000FC0)) | (((UINT32)0x02) << 6) | (((UINT32)0x02) << 8) | (((UINT32)0x02) << 10));
            }
            break;
        }
    }
    else if (_u4SDMultiFuncCfg == 2)
    {
        // GPIO37	sd_clk_1/ms_clk(O)
        // GPIO38	sd_d3_1/ms_d3(I/O)
        // GPIO39	sd_d2_1/ms_d2(I/O)
        // GPIO40	sd_d1_1/ms_d1(I/O)
        // GPIO41	sd_d0_1/ms_d0(I/O)
        // GPIO42	sd_cmd_1/ms_bs(O)  
        switch(i4ClkMHz)
        {
            case 54:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
            }
            break;
            case 45:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
            }
            break;
            case 33:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;
            case 27:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;
            case 20:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;        
            case 13:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;           
            case 9:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;             
            case 6:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;                
            case 3:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;                
            case 1:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;           
            default:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;
        }
    }
    else if (_u4SDMultiFuncCfg == 3)
    {
        // PAD_CI_MDO0	  sd_clk(O)  
        // PAD_CI_MOVAL	sd_d3(I/O) 
        // PAD_CI_MOSTRT	sd_d2(I/O) 
        // PAD_CI_MCLKO	sd_d1(I/O) 
        // PAD_CI_MISTRT	sd_d0(I/O) 
        // PAD_CI_MDI0	  sd_cmd(I/O)
        switch(i4ClkMHz)
        {
            case 54:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x03) << 20) | (((UINT32)0x03) << 18));
            }
            break;
            case 45:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x03) << 20) | (((UINT32)0x03) << 18));
            }
            break;
            case 33:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x02) << 20) | (((UINT32)0x02) << 18));
            }
            break;
            case 27:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x02) << 20) | (((UINT32)0x02) << 18));
            }
            break;
            case 20:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x02) << 20) | (((UINT32)0x02) << 18));
            }
            break;        
            case 13:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x01) << 20) | (((UINT32)0x01) << 18));
            }
            break;           
            case 9:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x01) << 20) | (((UINT32)0x01) << 18));
            }
            break;             
            case 6:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x01) << 20) | (((UINT32)0x01) << 18));
            }
            break;                
            case 3:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x01) << 20) | (((UINT32)0x01) << 18));
            }
            break;                
            case 1:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x01) << 20) | (((UINT32)0x01) << 18));
            }
            break;           
            default:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x01) << 20) | (((UINT32)0x01) << 18));
            }
            break;
        }
    }
}


//-------------------------------------------------------------------------
/** FCIHW_MSDrivingCurrent
 *  Set MS clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz)
{
    if (_u4MSMultiFuncCfg == 1)
    {
        // GPIO10	ms_clk(O)
        // GPIO11	ms_bs(O)
        // GPIO12	ms_d3(I/O)
        // GPIO13	ms_d2(I/O)
        // GPIO14	ms_d1(I/O)
        // GPIO15	ms_d0(I/O)  
        switch(i4ClkMHz)
        {
            case 60:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x03) << 12) | (((UINT32)0x03) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x03) << 8));
            }
            break;
            case 54:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x03) << 12) | (((UINT32)0x03) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x03) << 8));
            }
            break;
            case 45:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x03) << 12) | (((UINT32)0x03) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x03) << 8));
            }
            break;
            case 33:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;
            case 27:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;
            case 20:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;        
            case 13:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;           
            case 9:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;                 
            case 6:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;              
            case 1:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;              
            case 0:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;     
            default:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x0000F000)) | (((UINT32)0x02) << 12) | (((UINT32)0x02) << 14));
                CKGEN_WRITE32(0x328, (CKGEN_READ32(0x328) & ~((UINT32)0x00000300)) | (((UINT32)0x02) << 8));
            }
            break;
        }
    }
    else if (_u4MSMultiFuncCfg == 2)
    {
        // GPIO37	sd_clk_1/ms_clk(O)
        // GPIO38	sd_d3_1/ms_d3(I/O)
        // GPIO39	sd_d2_1/ms_d2(I/O)
        // GPIO40	sd_d1_1/ms_d1(I/O)
        // GPIO41	sd_d0_1/ms_d0(I/O)
        // GPIO42	sd_cmd_1/ms_bs(O)  
        
        switch(i4ClkMHz)
        {
            case 60:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
            }
            break;
            case 54:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
            }
            break;
            case 45:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
            }
            break;
            case 33:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;
            case 27:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;
            case 20:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;        
            case 13:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;           
            case 9:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;           
            case 6:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;            
            case 1:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;            
            case 0:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;         
            default:
            {
                CKGEN_WRITE32(0x32C, (CKGEN_READ32(0x32C) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
            }
            break;
        }
    }
    else if (_u4MSMultiFuncCfg == 3)
    {
        // PAD_CI_MDO0	  ms_clk(O)   
        // PAD_CI_MOVAL	ms_d3(I/O)  
        // PAD_CI_MOSTRT	ms_d2(I/O)  
        // PAD_CI_MCLKO	ms_d1(I/O)  
        // PAD_CI_MISTRT	ms_d0(I/O)  
        // PAD_CI_MDI0	  ms_bs(O) 
        
        switch(i4ClkMHz)
        {
            case 60:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x03) << 20) | (((UINT32)0x03) << 18));
            }
            break;
            case 54:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x03) << 20) | (((UINT32)0x03) << 18));
            }
            break;
            case 45:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x03) << 20) | (((UINT32)0x03) << 18));
            }
            break;
            case 33:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x02) << 20) | (((UINT32)0x02) << 18));
            }
            break;
            case 27:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;
            case 20:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;        
            case 13:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;           
            case 9:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;           
            case 6:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;            
            case 1:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;            
            case 0:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x00) << 20) | (((UINT32)0x00) << 18));
            }
            break;         
            default:
            {
                CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x003C0000)) | (((UINT32)0x02) << 20) | (((UINT32)0x02) << 18));
            }
            break;
        }
    }
}

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5365)
//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4SDMultiFuncCfg)
    {
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
            }
            else
            {
            }
        }
        break;

        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {
            }
            else
            {
            }
        }
        break;
        
        default:
          break;
    }
}


//-------------------------------------------------------------------------
/** FCIHW_MSDrivingCurrent
 *  Set MS clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4MSMultiFuncCfg)
    {    
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {               
            }
            else if (i4ClkMHz >= 10)
            {                                                  
            }
            else
            {
            }
        }
        break;
        
        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {                                
            }
            else if (i4ClkMHz >= 10)
            {                                              
            }
            else
            {
            }
        }
        break;        
        
        default:
          break;  
    }
}

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5395)
//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{
    // mt5395 has only one sd function
            // Driving current setting  
            if (i4ClkMHz >= 27)
            {
                //0xF000d310[9:8]=10, 4 mA
                CKGEN_WRITE32(0x310, CKGEN_READ32(0x310) & ~((UINT32)0x300) | ((UINT32)0x200));
            }
            else
            {
                //0xF000d310[9:8]=0, 2 mA
                CKGEN_WRITE32(0x310, CKGEN_READ32(0x310) & ~((UINT32)0x300));
            }
}


//-------------------------------------------------------------------------
/** FCIHW_MSDrivingCurrent
 *  Set MS clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_MSDrivingCurrent(INT32 i4ClkMHz)
{
    switch(_u4MSMultiFuncCfg)
    {    
        case 1:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {               
            }
            else if (i4ClkMHz >= 10)
            {                                                  
            }
            else
            {
            }
        }
        break;
        
        case 2:
        {
            // Driving current setting
            if (i4ClkMHz >= 27)
            {                                
            }
            else if (i4ClkMHz >= 10)
            {                                              
            }
            else
            {
            }
        }
        break;      
        
        default:
          break;  
    }
}

#endif  // #if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)

//-------------------------------------------------------------------------
/** FCIHW_ChangeMSPClock
 *  Set MSP clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------

VOID FCIHW_ChangeMSPClock(INT32 i4ClkMHz)
{
#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)
    INT32 i4MSSrcSel, i4MSDivSel;

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 40:
            {
                _u4MSClockSetting = 40;           
                // Setting for MS (FCI 1)
                i4MSSrcSel = 1; // 36 Mhz (1/3*SYSPLL div 3)
                i4MSDivSel = 1;
                /*    
                // Setting for MS (FCI 2)
                i4MSSrcSel = 1; // 40.5 Mhz (1/2*SYSPLL div 3)
                i4MSDivSel = 2;
                */
            }

        case 27:
            {
                _u4MSClockSetting = 27;    
                // Setting for FCI 1 & FCI 2 is the same
                i4MSSrcSel = 0; // 27 Mhz
                i4MSDivSel = 0;
                break;
            }

        case 25:
            {
                _u4MSClockSetting = 25;    
                // Setting for MS (FCI 1)
                i4MSSrcSel = 0; // 27 Mhz
                i4MSDivSel = 0;
                /*
                // Setting for MS (FCI 2)
                i4MSSrcSel = 2; // 24.92 Mhz (1/13*SYSPLL)
                i4MSDivSel = 0;
                */
                break;
            }

        case 20:
            {
                _u4MSClockSetting = 20;    
                // Setting for MS (FCI 1)
                i4MSSrcSel = 1; // 21.6 Mhz (1/3*SYSPLL div 5)
                i4MSDivSel = 3;
                /*
                // Setting for MS (FCI 2)
                i4MSSrcSel = 1; // 20.25 Mhz (1/2*SYSPLL div 8)
                i4MSDivSel = 6;
                */
                break;
            }

        case 13:
        default:
            {
                _u4MSClockSetting = 13;    
                // Setting for SD1 & SD2 is the same
                i4MSSrcSel = 0; // 13.5 Mhz (27 Mhz div 2)
                i4MSDivSel = 1;
                break;
            }

        case 4:
            {
                _u4MSClockSetting = 4;    
                // Setting for SD1 & SD2 is the same
                i4MSSrcSel = 0; // 3.375 Mhz (27 Mhz div 2^3)
                i4MSDivSel = 3;
                break;
            }

        case 1:
            {
                _u4MSClockSetting = 1;    
                // Setting for SD1 & SD2 is the same
                i4MSSrcSel = 0; // 0.84375 Mhz (27 Mhz div 2^5)
                i4MSDivSel = 5;
                break;
            }

        case 0: // 421.875 Khz
            {
                _u4MSClockSetting = 0;    
                // Setting for SD1 & SD2 is the same
                i4MSSrcSel = 0; // 0.421875 Mhz (27 Mhz div 2^6)
                i4MSDivSel = 6;
                break;
            }
    }

#ifdef MT8520_USE_SD2
    _Fcihw_SetSD2Clock(i4MSSrcSel, i4MSDivSel);
#else
    _Fcihw_SetMSClock(i4MSSrcSel, i4MSDivSel);
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)
#if(CONFIG_DRV_FPGA_BOARD==1)
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 27:
            {
                _u4MSClockSetting = 27;
                FCI_WRITE32(0x58, 0x00);
                break;
            }

        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                FCI_WRITE32(0x58, 0x01);
                break;
            }

        case 6:
            {
                _u4MSClockSetting = 6;
                FCI_WRITE32(0x58, 0x02);
                break;
            }

        case 3:
            {
                _u4MSClockSetting = 3;
                FCI_WRITE32(0x58, 0x03);
                break;
            }

        case 1:
            {
                _u4MSClockSetting = 1;
                FCI_WRITE32(0x58, 0x04);
                break;
            }

        case 0:
            {
                _u4MSClockSetting = 0;
                FCI_WRITE32(0x58, 0x06);
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);

#else   // for RealChip  // mhzhang
    switch(i4ClkMHz)
    {
        case 54:
            {
                _u4MSClockSetting = 54;
                // 0x7000_0078 bit[22:16] = xxx_0001, MS_CLK = 54 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_8);
                break;
            }
            
        case 50:
            {
                _u4MSClockSetting = 50;
                // 0x7000_0078 bit[22:16] = xxx_0100, MS_CLK = 50 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_12);
                break;
            }
            
        case 43:
            {
                _u4MSClockSetting = 43;
                // 0x7000_0078 bit[22:16] = xxx_0010, MS_CLK = 43.2 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_10);
                break;
            }

        case 37:
            {
                _u4MSClockSetting = 37;
                // 0x7000_0078 bit[22:16] = xxx_0101, MS_CLK = 37 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_DMPLL_1_8);
                break;
            }
            
        case 36:
            {
                _u4MSClockSetting = 36;
                // 0x7000_0078 bit[22:16] = xxx_0011, MS_CLK = 36 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_12);
                break;
            }
            
        case 33:
            {
                _u4MSClockSetting = 33;
                // 0x7000_0078 bit[22:16] = xxx_0110, MS_CLK =  33.33 Mhz
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_18);
                break;
            }
            
        case 27:
            {
                _u4MSClockSetting = 27;
                // 0x7000_0078 bit[22:16] = xxx_0000, MS_CLK = 27 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M);
                break;
            }

        case 23:
            {
                _u4MSClockSetting = 23;
                // 0x7000_0078 bit[22:16] = xxx_0111, MS_CLK = 23.08 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_26);
                break;
            }

        case 18:
            {
                _u4MSClockSetting = 18;
                // 0x7000_0078 bit[22:16] = xxx_1000, MS_CLK = 18 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_24);
                break;
            }
                        
        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                // 0x7000_0078 bit[22:16] = 000_1001, MS_CLK = 13.50 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_2);
                break;
            }
            
        case 6:
            {
                _u4MSClockSetting = 6;
                // 0x7000_0078 bit[22:16] = 001_1001, MS_CLK = 6.75 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_4);
                break;
            }
            
        case 3:
            {
                _u4MSClockSetting = 3;
                // 0x7000_0078 bit[22:16] = 010_1001, MS_CLK = 3.38 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_8);
                break;
            }
            
        case 1:
            {
                _u4MSClockSetting = 1;
                // 0x7000_0078 bit[22:16] = 011_1001, MS_CLK = 1.69 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_16);
                break;
            }
            
        case 0:
            {
                _u4MSClockSetting = 0;
                // 0x7000_0078 bit[22:16] = 101_1001, MS_CLK = 421.855 KHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_64);
                break;
                
                // For reference                
                // 0x7000_0078 bit[22:16] = 100_1001, MS_CLK = 843.75 KHz OK
                // 0x7000_0078 bit[22:16] = 110_1001, MS_CLK = 210.94 KHz OK
                // 0x7000_0078 bit[22:16] = 111_1001, MS_CLK = 105.47 KHz OK

            }
    }

    x_thread_delay(10);
    
    FCIHW_MSDrivingCurrent(i4ClkMHz);	
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)

#if (CONFIG_DRV_FPGA_BOARD == 1)
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 27:
            {
                _u4MSClockSetting = 27;
                FCI_WRITE32(0x58, 0x00);
                break;
            }

        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                FCI_WRITE32(0x58, 0x01);
                break;
            }

        case 6:
            {
                _u4MSClockSetting = 6;
                FCI_WRITE32(0x58, 0x02);
                break;
            }

        case 3:
            {
                _u4MSClockSetting = 3;
                FCI_WRITE32(0x58, 0x03);
                break;
            }

        case 1:
            {
                _u4MSClockSetting = 1;
                FCI_WRITE32(0x58, 0x04);
                break;
            }

        case 0:
            {
                _u4MSClockSetting = 0;
                FCI_WRITE32(0x58, 0x06);
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);

#else // added by mhzhang on 2009/11/6 
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4MSClockSetting = 54;
                // 0x7000_0078 bit[22:16] = xxx_0001, MS_CLK = 54 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_8);
                break;
            }
            
        case 50:
            {
                _u4MSClockSetting = 50;
                // 0x7000_0078 bit[22:16] = xxx_0100, MS_CLK = 50 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_12);
                break;
            }
            
        case 43:
            {
                _u4MSClockSetting = 43;
                // 0x7000_0078 bit[22:16] = xxx_0010, MS_CLK = 43.2 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_10);
                break;
            }

        case 37:
            {
                _u4MSClockSetting = 37;
                // 0x7000_0078 bit[22:16] = xxx_0101, MS_CLK = 37.5 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_DMPLL_1_8);
                break;
            }
		
        case 36:
            {
                _u4MSClockSetting = 36;
                // 0x7000_0078 bit[22:16] = xxx_0011, MS_CLK = 36 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_12);
                break;
            }            
            
        case 33:
            {
                _u4MSClockSetting = 33;
                // 0x7000_0078 bit[22:16] = xxx_0110, MS_CLK = 33.3 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_18);
                break;
            }
            
        case 27:
            {
                _u4MSClockSetting = 27;
                // 0x7000_0078 bit[22:16] = xxx_0000, MS_CLK = 27.03 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M);
                break;
            }

        case 23:
            {
                _u4MSClockSetting = 23;
                // 0x7000_0078 bit[22:16] = xxx_0111, MS_CLK = 23.08 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_26);
                break;
            }

        case 18:
            {
                _u4MSClockSetting = 18;
                // 0x7000_0078 bit[22:16] = xxx_1000, MS_CLK = 18 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_24);
                break;
            }		
		
        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                // 0x7000_0078 bit[22:16] = 000_1001, MS_CLK = 13.50 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_2);
                break;
            }
            
        case 6:
            {
                _u4MSClockSetting = 6;
                // 0x7000_0078 bit[22:16] = 001_1001, MS_CLK = 6.75 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_4);
                break;
            }
            
        case 3:
            {
                _u4MSClockSetting = 3;
                // 0x7000_0078 bit[22:16] = 010_1001, MS_CLK = 3.375 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_8);
                break;
            }
            
        case 1:
            {
                _u4MSClockSetting = 1;
                // 0x7000_0078 bit[22:16] = 011_1001, MS_CLK = 1.688 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_16);
                break;
            }
            
        case 0:
            {
                _u4MSClockSetting = 0;
                // 0x7000_0078 bit[22:16] = 101_1001, MS_CLK = 0.422 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_64);
                break;
                
                // For reference                
                // 0x7000_0078 bit[22:16] = 100_1001, MS_CLK = 843.75 KHz OK
                // 0x7000_0078 bit[22:16] = 101_1001, MS_CLK = 421.885 KHz OK
                // 0x7000_0078 bit[22:16] = 110_1001, MS_CLK = 210.94 KHz OK
                // 0x7000_0078 bit[22:16] = 111_1001, MS_CLK = 105.47 KHz OK    
            }
    }

    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_MSDrivingCurrent(i4ClkMHz);	
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4MSClockSetting = 54;
                // 0x7000_0078 bit[22:16] = 001_0000, MS_CLK = 54.35 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_8);
                break;
            }
            
        case 50:
            {
                _u4MSClockSetting = 50;
                // 0x7000_0078 bit[22:16] = 100_0000, MS_CLK = 50.20 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_12);
                break;
            }
            
        case 43:
            {
                _u4MSClockSetting = 43;
                // 0x7000_0078 bit[22:16] = 010_0000, MS_CLK = 43.48 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_10);
                break;
            }
            
        case 36:
            {
                _u4MSClockSetting = 36;
                // 0x7000_0078 bit[22:16] = 011_0000, MS_CLK = 36.23 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL1_1_12);
                break;
            }
            
        case 34:
            {
                _u4MSClockSetting = 34;
                // 0x7000_0078 bit[22:16] = 101_0000, MS_CLK =  33.75 Mhz (DMPLL/8 = 270/8 = 33.75 Mhz)
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_DMPLL_1_8);
                break;
            }
            
        case 33:
            {
                _u4MSClockSetting = 33;
                // 0x7000_0078 bit[22:16] = 110_0000, MS_CLK = 33.47 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_SYSPLL2_1_18);
                break;
            }
            
        case 27:
            {
                _u4MSClockSetting = 27;
                // 0x7000_0078 bit[22:16] = 000_0000, MS_CLK = 27.03 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M);
                break;
            }
                        
        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                // 0x7000_0078 bit[22:16] = 111_0001, MS_CLK = 13.50 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_2);
                break;
            }
            
        case 6:
            {
                _u4MSClockSetting = 6;
                // 0x7000_0078 bit[22:16] = 111_0010, MS_CLK = 6.75 MHz  OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_4);
                break;
            }
            
        case 3:
            {
                _u4MSClockSetting = 3;
                // 0x7000_0078 bit[22:16] = 111_0011, MS_CLK = 3.375 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_8);
                break;
            }
            
        case 1:
            {
                _u4MSClockSetting = 1;
                // 0x7000_0078 bit[22:16] = 111_0100, MS_CLK = 1.688 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_16);
                break;
            }
            
        case 0:
            {
                _u4MSClockSetting = 0;
                // 0x7000_0078 bit[22:16] = 111_0110, MS_CLK = 0.422 MHz OK
                CKGEN_AgtSelClk(e_CLK_MS, CLK_CLK_MS_SEL_27M_1_64);
                break;
                
                // For reference                
                // 0x7000_0078 bit[22:16] = 111_0101, MS_CLK = 0.844 MHz OK
                // 0x7000_0078 bit[22:16] = 111_0111, MS_CLK = 0.211 MHz OK
                // 0x7000_0078 bit[22:16] = 111_1000, MS_CLK = 0.105 MHz OK
                // 0x7000_0078 bit[22:16] = 111_1XXX, MS_CLK = 0.105 MHz OK    
            }
    }
    
    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_MSDrivingCurrent(i4ClkMHz);

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 60:
            {
                _u4MSClockSetting = 60;   
                CLKSRC_ChangeMSPClock(60);
                break;
            }
            
        case 54:
            {
                _u4MSClockSetting = 54;   
                CLKSRC_ChangeMSPClock(54);            
                break;
            }
            
        case 50:
            {
                _u4MSClockSetting = 50;
                CLKSRC_ChangeMSPClock(50);
                break;
            }
            
        case 45:
            {
                _u4MSClockSetting = 45;
                CLKSRC_ChangeMSPClock(45);
                break;
            }
                        
        case 33:
            {
                _u4MSClockSetting = 33;
                CLKSRC_ChangeMSPClock(33);
                break;
            }
            
        case 27:
            {
                _u4MSClockSetting = 27;
                CLKSRC_ChangeMSPClock(27);
                break;
            }
                  
        case 20:
            {
                _u4MSClockSetting = 20;
                CLKSRC_ChangeMSPClock(20);
                break;
            }      
            
        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                CLKSRC_ChangeMSPClock(13);
                break;
            }
            
        case 9:
            {
                _u4MSClockSetting = 9;
                CLKSRC_ChangeMSPClock(9);
                break;
            }
            
        case 6:
            {
                _u4MSClockSetting = 6;
                CLKSRC_ChangeMSPClock(6);
                break;
            }
                        
        case 1:
            {
                _u4MSClockSetting = 1;
                CLKSRC_ChangeMSPClock(1);
                break;
            }
            
        case 0:
            {
                _u4MSClockSetting = 0;
                CLKSRC_ChangeMSPClock(0);
                break;
            }
    }
    
    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_MSDrivingCurrent(i4ClkMHz);
#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5365)

    UNUSED(i4ClkMHz);

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5395)

    UNUSED(i4ClkMHz);
  
#endif

    _Fcihw_ChangeSettings((INT32)_u4MSClockSetting);

    LOG(1, "MS Clock Set to %d Mhz\n", _u4MSClockSetting);
}

//-------------------------------------------------------------------------
/** FCIHW_ChangeSDCClock
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_ChangeSDCClock(INT32 i4ClkMHz)
{
#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)
    INT32 i4SDSrcSel, i4SDDivSel;

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 50:
            {
                _u4SDClockSetting = 50;
#ifdef MT8520_USE_SD2                
                // Setting for SD2 (FCI 2)
                i4SDSrcSel = 3; // 48.94 Mhz (1/2*SYSPLL2 div 4)
                i4SDDivSel = 2;
#else
                // Setting for SD1 (FCI 1)
                i4SDSrcSel = 1; // 46.28 Mhz (1/7*SYSPLL)
                i4SDDivSel = 0;
#endif
                break;
            }

        case 27:
            {
                _u4SDClockSetting = 27;
                // Setting for SD1 & SD2 is the same
                i4SDSrcSel = 0; // 27 Mhz
                i4SDDivSel = 0;
                break;
            }

        case 25:
            {
                _u4SDClockSetting = 25;
                // Setting for SD1 & SD2 is the same
                i4SDSrcSel = 2; // 24.92 Mhz (1/13*SYSPLL)
                i4SDDivSel = 0;
                break;
            }

        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                // Setting for SD1 & SD2 is the same
                i4SDSrcSel = 0; // 13.5 Mhz (27 Mhz div 2)
                i4SDDivSel = 1;
                break;
            }

        case 4:
            {
                _u4SDClockSetting = 4;
                // Setting for SD1 & SD2 is the same
                i4SDSrcSel = 0; // 3.375 Mhz (27 Mhz div 2^3)
                i4SDDivSel = 3;
                break;
            }

        case 1:
            {
                _u4SDClockSetting = 1;
                // Setting for SD1 & SD2 is the same
                i4SDSrcSel = 0; // 0.84375 Mhz (27 Mhz div 2^5)
                i4SDDivSel = 5;
                break;
            }

        case 0: // 421.875 Khz
            {
                _u4SDClockSetting = 0;
                // Setting for SD1 & SD2 is the same
                i4SDSrcSel = 0; // 0.421875 Mhz (27 Mhz div 2^6)
                i4SDDivSel = 6;
                break;
            }
    }

#ifdef MT8520_USE_SD2
    _Fcihw_SetSD2Clock(i4SDSrcSel, i4SDDivSel);
#else
    _Fcihw_SetSD1Clock(i4SDSrcSel, i4SDDivSel);
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)
#if (CONFIG_DRV_FPGA_BOARD == 1)
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 27:
            {
                _u4SDClockSetting = 27;
                FCI_WRITE32(0x58, 0x00);
                break;
            }

        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                FCI_WRITE32(0x58, 0x01);
                break;
            }

        case 6:
            {
                _u4SDClockSetting = 6;
                FCI_WRITE32(0x58, 0x02);
                break;
            }

        case 3:
            {
                _u4SDClockSetting = 3;
                FCI_WRITE32(0x58, 0x03);
                break;
            }

        case 1:
            {
                _u4SDClockSetting = 1;
                FCI_WRITE32(0x58, 0x04);
                break;
            }

        case 0:
            {
                _u4SDClockSetting = 0;
                FCI_WRITE32(0x58, 0x06);
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);

#else  //for RealChip  //mhzhang 2009/11/6
   switch (i4ClkMHz)
   {
        case 54:
            {
                _u4SDClockSetting = 54;
                // 0x7000_0078 bit[14:8] = xxx_0001, SD_CLK = 54 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_8);
                break;
            }

        case 50:
            {
                _u4SDClockSetting = 50;
                // 0x7000_0078 bit[14:8] = xxx_0100, SD_CLK = 50 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_12);
                break;
            }
            
        case 43:
            {
                _u4SDClockSetting = 43;
                // 0x7000_0078 bit[14:8] = 010_0010, SD_CLK = 43.2 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_10);
                break;
            }

        case 37:
            {
                _u4SDClockSetting = 37;
                // 0x7000_0078 bit[14:8] = xxx_0101, SD_CLK = 37.5 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_DMPLL_1_8);
                break;
            }

			
        case 36:
            {
                _u4SDClockSetting = 36;
                // 0x7000_0078 bit[14:8] = xxx_0011, SD_CLK = 36 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_12);
                break;
            }
            
        case 33:
            {
                _u4SDClockSetting = 33;
                // 0x7000_0078 bit[14:8] = xxx_0110, SD_CLK = 33.33MHz
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_18);
                break;
            }
            
        case 27:
            {
                _u4SDClockSetting = 27;
                // 0x7000_0078 bit[14:8] = xxx_0000, SD_CLK = 27 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M);
                break;
            }

        case 23:
            {
                _u4SDClockSetting = 23;
                // 0x7000_0078 bit[14:8] = xxx_0111, SD_CLK = 23.08 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_26);
                break;
            }

        case 18:
            {
                _u4SDClockSetting = 18;
                // 0x7000_0078 bit[14:8] = xxx_0111, SD_CLK = 23.08 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_24);
                break;
            }
                        
        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                // 0x7000_0078 bit[14:8] = 000_1001, SD_CLK = 13.50 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_2);
                break;
            }            

        case 6:
            {
                _u4SDClockSetting = 6;
                // 0x7000_0078 bit[14:8] = 001_1001, SD_CLK = 6.75 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_4);
                break;
            }            
            
        case 3:
            {
                _u4SDClockSetting = 3;
                //  0x7000_0078 bit[14:8] = 010_1001, SD_CLK = 3.38 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_8);
                break;
            }        
            
        case 1:
            {
                _u4SDClockSetting = 1;
                //  0x7000_0078 bit[14:8] = 011_0100, SD_CLK = 1.69 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_16);
                break;
            }      
            
        case 0:
            {
                _u4SDClockSetting = 0;
                //  0x7000_0078 bit[14:8] = 101_1001, SD_CLK = 421.885 KHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_64);
                break;

                // For reference
                // 0x7000_0078 bit[14:8] = 100_1001, SD_CLK = 843.75 KHz  OK
                // 0x7000_0078 bit[14:8] = 110_1001, SD_CLK = 210.94 KHz  OK
                // 0x7000_0078 bit[14:8] = 111_1001, SD_CLK = 105.47 KHz  OK

            }
    }
   
    x_thread_delay(100);
	FCIHW_SDDrivingCurrent(i4ClkMHz);
#endif 

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)
#if (CONFIG_DRV_FPGA_BOARD == 1)
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 27:
            {
                _u4SDClockSetting = 27;
                FCI_WRITE32(0x58, 0x00);
                break;
            }

        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                FCI_WRITE32(0x58, 0x01);
                break;
            }

        case 6:
            {
                _u4SDClockSetting = 6;
                FCI_WRITE32(0x58, 0x02);
                break;
            }

        case 3:
            {
                _u4SDClockSetting = 3;
                FCI_WRITE32(0x58, 0x03);
                break;
            }

        case 1:
            {
                _u4SDClockSetting = 1;
                FCI_WRITE32(0x58, 0x04);
                break;
            }

        case 0:
            {
                _u4SDClockSetting = 0;
                FCI_WRITE32(0x58, 0x06);
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);

#else // added by mhzhang on 2009/11/6
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4SDClockSetting = 54;
                // 0x7000_0078 bit[14:8] = xxx_0001, SD_CLK = 54 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_8);
                break;
            }

        case 50:
            {
                _u4SDClockSetting = 50;
                // 0x7000_0078 bit[14:8] = xxx_0100, SD_CLK = 50 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_12);
                break;
            }
            
        case 43:
            {
                _u4SDClockSetting = 43;
                // 0x7000_0078 bit[14:8] = xxx_0010, SD_CLK = 43.2 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_10);
                break;
            }

        case 37:
            {
                _u4SDClockSetting = 37;
                // 0x7000_0078 bit[14:8] = xxx_0101, SD_CLK = 37.5 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_DMPLL_1_8);
                break;
            }
		
        case 36:
            {
                _u4SDClockSetting = 36;
                // 0x7000_0078 bit[14:8] = xxx_0011, SD_CLK = 36 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_12);
                break;
            }
            
        case 33:
            {
                _u4SDClockSetting = 33;
                // 0x7000_0078 bit[14:8] = xxx_0110, SD_CLK = 33.3 Mhz
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_18);
                break;
            }
            
        case 27:
            {
                _u4SDClockSetting = 27;
                // 0x7000_0078 bit[14:8] = xxx_0000, SD_CLK = 27.03 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M);
                break;
            }

        case 23:
            {
                _u4SDClockSetting = 23;
                // 0x7000_0078 bit[14:8] = xxx_0111, SD_CLK = 23.08 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_26);
                break;
            }

        case 18:
            {
                _u4SDClockSetting = 18;
                // 0x7000_0078 bit[14:8] = xxx_1000, SD_CLK = 18 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_24);
                break;
            }
		
        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                // 0x7000_0078 bit[14:8] = 000_1001, SD_CLK = 13.50 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_2);
                break;
            }            

        case 6:
            {
                _u4SDClockSetting = 6;
                // 0x7000_0078 bit[14:8] = 001_1001, SD_CLK = 6.75 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_4);
                break;
            }            
            
        case 3:
            {
                _u4SDClockSetting = 3;
                //  0x7000_0078 bit[14:8] = 010_1001, SD_CLK = 3.38 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_8);
                break;
            }        
            
        case 1:
            {
                _u4SDClockSetting = 1;
                //  0x7000_0078 bit[14:8] = 011_1001, SD_CLK = 1.69 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_16);
                break;
            }      
            
        case 0:
            {
                _u4SDClockSetting = 0;
                //  0x7000_0078 bit[14:8] = 101_1001, SD_CLK = 420 KHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_64);
                break;

                // For reference
                // 0x7000_0078 bit[14:8] = 100_1001, SD_CLK = 840 KHz  OK
                // 0x7000_0078 bit[14:8] = 101_1001, SD_CLK = 420 KHz  OK
                // 0x7000_0078 bit[14:8] = 110_1001, SD_CLK = 210 KHz  OK
                // 0x7000_0078 bit[14:8] = 111_1001, SD_CLK = 105 KHz  OK                
            }
    }
    x_thread_delay(10);
    FCIHW_SDDrivingCurrent(i4ClkMHz);		
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4SDClockSetting = 54;
                // 0x7000_0078 bit[14:8] = 001_0000, SD_CLK = 54.35 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_8);
                break;
            }

        case 50:
            {
                _u4SDClockSetting = 50;
                // 0x7000_0078 bit[14:8] = 100_0000, SD_CLK = 50.20 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_12);
                break;
            }
            
        case 43:
            {
                _u4SDClockSetting = 43;
                // 0x7000_0078 bit[14:8] = 010_0000, SD_CLK = 43.48 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_10);
                break;
            }
            
        case 36:
            {
                _u4SDClockSetting = 36;
                // 0x7000_0078 bit[14:8] = 011_0000, SD_CLK = 36.23 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL1_1_12);
                break;
            }
            
        case 34:
            {
                _u4SDClockSetting = 34;
                // 0x7000_0078 bit[14:8] = 101_0000, SD_CLK = 33.75 Mhz (DMPLL/8 = 270/8 = 33.75 Mhz)
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_DMPLL_1_8);
                break;
            }
            
        case 33:
            {
                _u4SDClockSetting = 33;
                // 0x7000_0078 bit[14:8] = 110_0000, SD_CLK = 33.47
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_SYSPLL2_1_18);
                break;
            }
            
        case 27:
            {
                _u4SDClockSetting = 27;
                // 0x7000_0078 bit[14:8] = 000_0000, SD_CLK = 27.03 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M);
                break;
            }
                        
        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                // 0x7000_0078 bit[14:8] = 111_0001, SD_CLK = 13.50 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_2);
                break;
            }            

        case 6:
            {
                _u4SDClockSetting = 6;
                // 0x7000_0078 bit[14:8] = 111_0010, SD_CLK = 6.75 MHz OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_4);
                break;
            }            
            
        case 3:
            {
                _u4SDClockSetting = 3;
                //  0x7000_0078 bit[14:8] = 111_0011, SD_CLK = 3.375 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_8);
                break;
            }        
            
        case 1:
            {
                _u4SDClockSetting = 1;
                //  0x7000_0078 bit[14:8] = 111_0100, SD_CLK = 1.688 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_16);
                break;
            }      
            
        case 0:
            {
                _u4SDClockSetting = 0;
                //  0x7000_0078 bit[14:8] = 111_0110, SD_CLK = 0.422 MHz  OK
                CKGEN_AgtSelClk(e_CLK_SD, CLK_CLK_SD_SEL_27M_1_64);
                break;

                // For reference
                // 0x7000_0078 bit[14:8] = 111_0101, SD_CLK = 0.844 MHz  OK
                // 0x7000_0078 bit[14:8] = 111_0111, SD_CLK = 0.211 MHz  OK
                // 0x7000_0078 bit[14:8] = 111_1000, SD_CLK = 0.105 MHz  OK
                // 0x7000_0078 bit[14:8] = 111_1XXX, SD_CLK = 0.105 MHz  OK                
            }
    }

    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_SDDrivingCurrent(i4ClkMHz);

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4SDClockSetting = 54;
                CLKSRC_ChangeSDCClock(54);
                break;
            }

        case 45:
            {
                _u4SDClockSetting = 45;
                CLKSRC_ChangeSDCClock(45);
                break;
            }
                        
        case 33:
            {
                _u4SDClockSetting = 33;
                CLKSRC_ChangeSDCClock(33);
                break;
            }            
                        
        case 27:
            {
                _u4SDClockSetting = 27;
                CLKSRC_ChangeSDCClock(27);
                break;
            }
            
        case 20:
            {
                _u4SDClockSetting = 20;
                CLKSRC_ChangeSDCClock(20);
                break;
            }
                        
        case 13:
        default:
            {
                _u4SDClockSetting = 13;
                CLKSRC_ChangeSDCClock(13);
                break;
            }

        case 9:
            {
                _u4SDClockSetting = 9;
                CLKSRC_ChangeSDCClock(9);
                break;
            }      
            
        case 6:
            {
                _u4SDClockSetting = 6;
                CLKSRC_ChangeSDCClock(6);
                break;
            }
            
        case 3:
            {
                _u4SDClockSetting = 3;
                CLKSRC_ChangeSDCClock(3);
                break;
            }
            
        case 1:
            {
                _u4SDClockSetting = 1;
                CLKSRC_ChangeSDCClock(1);
                break;
            }
            
        case 0:
            {
                _u4SDClockSetting = 00;
                CLKSRC_ChangeSDCClock(00);
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_SDDrivingCurrent(i4ClkMHz);
#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5365)

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 42:
            {
                _u4SDClockSetting = 42;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x00);
#else
                CLKSRC_ChangeSDCClock(42);
#endif
                break;
            }
                        
        case 21:
            {
                _u4SDClockSetting = 21;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x01);
#else
                CLKSRC_ChangeSDCClock(21);
#endif
                break;
            }            
                   
        default:     
        case 10:
            {
                _u4SDClockSetting = 10;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x02);
#else
                CLKSRC_ChangeSDCClock(10);
#endif
                break;
            }
            
        case 5:
            {
                _u4SDClockSetting = 5;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x03);
#else
                CLKSRC_ChangeSDCClock(5);
#endif
                break;
            }
                        
        case 2:
            {
                _u4SDClockSetting = 2;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x04);
#else
                CLKSRC_ChangeSDCClock(2);
#endif
                break;
            }

        case 1:
        case 0:
            {
                _u4SDClockSetting = 1;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x05);
#else
                CLKSRC_ChangeSDCClock(1);
#endif
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_SDDrivingCurrent(i4ClkMHz);

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5395)

    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4SDClockSetting = 54;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x00);
#else
                CLKSRC_ChangeSDCClock(54);
#endif
                break;
            }
        case 48:
            {
                _u4SDClockSetting = 48;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x00);
#else
                CLKSRC_ChangeSDCClock(48);
#endif
                break;
            }
        case 27:
            {
                _u4SDClockSetting = 27;
#if (CONFIG_DRV_FPGA_BOARD == 1)
//                FCI_WRITE32(0x58, 0x00);
#else
                CLKSRC_ChangeSDCClock(27);
#endif
                break;
            }
                        
        default:
        case 13:
            {
                _u4SDClockSetting = 13;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x01);
#else
                CLKSRC_ChangeSDCClock(13);
#endif
                break;
            }            
                     
        case 9:
            {
                _u4SDClockSetting = 9;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x02);
#else
                CLKSRC_ChangeSDCClock(9);
#endif
                break;
            }
            
        case 5:
            {
                _u4SDClockSetting = 5;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x03);
#else
                CLKSRC_ChangeSDCClock(5);
#endif
                break;
            }
                        
        case 2:
            {
                _u4SDClockSetting = 2;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x04);
#else
                CLKSRC_ChangeSDCClock(1);
#endif
                break;
            }

        case 1:
        case 0:
            {
                _u4SDClockSetting = 1;
#if (CONFIG_DRV_FPGA_BOARD == 1)
                FCI_WRITE32(0x58, 0x05);
#else
                CLKSRC_ChangeSDCClock(1);
#endif
                break;
            }
    }

    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_SDDrivingCurrent(i4ClkMHz);

#endif  // #if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)

    _Fcihw_ChangeSettings((INT32)_u4SDClockSetting);

    LOG(1, "SD Clock Set to %d Mhz\n", _u4SDClockSetting);
}

//-------------------------------------------------------------------------
/** FCIHW_DmaWriteDramEnable
 *  Transfer data from memory card to FCI controller.
 *  @param pu1Buf       user's read buffer pointer.
 *  @param u4BlkSz     read block size in bytes.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_DmaWriteDramEnable(UINT8 *pu1Buf, UINT32 u4BlkSz)
{
    UINT32 u4Val;
    UINT32 u4UpAddr = 0;

    // DMA address must be double word alignment.
    ASSERT(pu1Buf);
    ASSERT(((UINT32)pu1Buf % 4) == 0);
    ASSERT(((UINT32)(pu1Buf + u4BlkSz) % 4) == 0);

    HalFlushInvalidateDCacheMultipleLine((UINT32)pu1Buf, u4BlkSz);
    _Fcihw_ResetFifo();
    _Fcihw_ResetDramClk();
    
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, (u4Val | CTRL_FCI_DMAEN) & ~(CTRL_FCI_FIFO_W_DIR));

    FCI_WRITE32(RW_DMA_LOW_BOUNDARY_REG, (UINT32)pu1Buf);
    FCI_WRITE32(RW_DMA_UP_BOUNDARY_REG, u4UpAddr);
}

//-------------------------------------------------------------------------
/** FCIHW_DmaReadDramEnable
 *  Transfer data from FCI controller to memory card.
 *  @param pu1Buf       user's read buffer pointer.
 *  @param u4BlkSz     write block size in bytes.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_DmaReadDramEnable(UINT8 *pu1Buf, UINT32 u4BlkSz)
{
    UINT32 u4Val;

    // DMA address must be double word alignment.
    ASSERT(pu1Buf);
    ASSERT(((UINT32)pu1Buf % 4) == 0);
    ASSERT(((UINT32)(pu1Buf + u4BlkSz) % 4) == 0);

    HalFlushDCacheMultipleLine((UINT32)pu1Buf, u4BlkSz);
    _Fcihw_ResetFifo();
    _Fcihw_ResetDramClk();

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_FCI_DMAEN | CTRL_FCI_FIFO_W_DIR);

    FCI_WRITE32(RW_DMA_DS_REG, u4BlkSz);

    // Notice: Setting DMA start address will trigger DMA to work.
    // So It should be the last command at DMA setup.
    FCI_WRITE32(RW_DMA_START_ADDR_REG, (UINT32)pu1Buf);
}

//-------------------------------------------------------------------------
/** FCIHW_DmaDisable
 *  Disable DMA transfer.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_DmaDisable(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val & ~(CTRL_FCI_DMAEN));
    _Fcihw_ResetFifo();
}

//-------------------------------------------------------------------------
/** SDCHW_InitSemaphore
 *  Make sure all SD semaphore is in lock state.
 *  @param  VOID
 *  @retval   VOID.
 */
//-------------------------------------------------------------------------
VOID SDCHW_InitSemaphore(VOID)
{
    INT32 i4Ret;

#ifdef FCI_SDMS_S_RST_INT
    i4Ret = x_sema_lock(_hFCISRSTSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
#endif

    i4Ret = x_sema_lock(_hFCISDDRSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

    i4Ret = x_sema_lock(_hFCISDCRSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550 use INTR_SD_DTRDYI instead
    i4Ret = x_sema_lock(_hFCISDWBFSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
    i4Ret = x_sema_lock(_hFCISDRBISema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
#endif
#endif

    UNUSED(i4Ret);

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    // Disable the interrupt
    VERIFY(BIM_DisableIrq(FCI_INT_VECTOR_ID));

    // Clear FCI Status variable
    _u4FCISStatus = 0x0;

    // Enable the interrupt
    VERIFY(BIM_EnableIrq(FCI_INT_VECTOR_ID));
#endif
}

//-------------------------------------------------------------------------
/** SDCHW_Init
 *  Initialize SD card interface.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID SDCHW_Init(VOID)
{
    SDC_T *prSDC = FCI_GetSDCSlot();
    
    prSDC = FCI_GetSDCSlot();
        
    _SDC_ChangeClock(SDC_LOW_CLOCK);

    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_RST | CTRL_DRAM_CLKRST | CTRL_DRAM_CLKEN);

    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_DRAM_CLKEN);

    if (_fgFCIContinueClock)
    {    
        // SD host must change to latch data at rising edge 0x08[25] = 0
        if (prSDC->fgHighSpeedMode)
        {        
            FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_CCO | CTRL_SD_SIEN | CTRL_SD_NEGDRIVE | CTRL_DRAM_CLKEN | CTRL_SD_DATASWAP | CTRL_SD_HS_ADJ);
        }
        else
        {
            FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_CCO | CTRL_SD_SIEN | CTRL_SD_NEGDRIVE | CTRL_DRAM_CLKEN | CTRL_SD_DATASWAP | CTRL_SD_RECEDGE);
        }          
    }
    else
    {
        // SD host must change to latch data at rising edge 0x08[25] = 0
        if (prSDC->fgHighSpeedMode)
        {
            FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_SIEN | CTRL_SD_NEGDRIVE | CTRL_DRAM_CLKEN | CTRL_SD_DATASWAP| CTRL_SD_HS_ADJ);
        }
        else
        {
            FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_SIEN | CTRL_SD_NEGDRIVE | CTRL_DRAM_CLKEN | CTRL_SD_DATASWAP | CTRL_SD_RECEDGE);
        }
    }

    FCI_WRITE32(RW_SD_BUSYCNT_REG,
        MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);

    _Fcihw_ResetInt();
    _Fcihw_ResetFifo();

    // check if ISR is registered ?
    if (_fgFCIIsrEn)
    {
        if (prSDC->u4CardType == FCI_SDIO_CARD)
        {
            // turn on interrupt.
            FCI_WRITE32(RW_FCI_INTR_CTRL_REG,
            INTR_SRAMSELI | FCI_SEL_SD | INTR_SD_ALLI |INTR_SDIO_INT | INTR_FCI_ALLI);
        }
        else
        {
            // turn on interrupt.
            FCI_WRITE32(RW_FCI_INTR_CTRL_REG,
            INTR_SRAMSELI | FCI_SEL_SD | INTR_SD_ALLI | INTR_FCI_ALLI);
        }
    }
    else
    {
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG, INTR_SRAMSELI | FCI_SEL_SD);
    }
}

//-------------------------------------------------------------------------
/** SDCHW_IsEmpty
 *  Polling SD card is exist or not.
 *  @param VOID.
 *  @retval  TRUE: card is not existed.
 *  @retval  FALSE: card is existed.
 */
//-------------------------------------------------------------------------
BOOL SDCHW_IsEmpty(VOID)
{
    return _fgSdCardRemove;
}

//-------------------------------------------------------------------------
/** SDCHW_CmdIdx
 *  SD command response index. 3 bits only. Due to insufficient register space.
 *  It show the LSB 3 bits of SD cmd.
 *  @param VOID.
 *  @return  SD command index.
 */
//-------------------------------------------------------------------------
UINT32 SDCHW_CmdIdx(VOID)
{
    return ((FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_SD_CMD_IDX) >> 24);
}

//-------------------------------------------------------------------------
/** SDCHW_ReadDataReg
 *  read SD data register.
 *  @param pu4Reg    point to register buffer.
 *  @param u4Size     read the number of register.
 *  @retval  S_OK       Success.
 *  @retval  Others      Fail.
 */
//-------------------------------------------------------------------------
INT32 SDCHW_ReadDataReg(UINT32 *pu4Reg, UINT32 u4Size)
{
    UINT32 i = 0;

    UINT32 u4Count = 0;
    UINT32 u4Val;

    ASSERT(pu4Reg != NULL);

    while (i < u4Size)
    {
#ifdef CC_CARD_DETECT_PIN
          // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
        if ((u4Val & INTR_FCI_RDFIFO_EMPTY) == 0)
        {
            pu4Reg[i] = FCI_READ32(RW_RD_DATA_REG);
            i++;
        }
        else
        {
            u4Count++;
            if (u4Count > 1000000)
            {
                return E_CMD_TIMEOUT;
            }
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** SDCHW_Set4BitOn
 *  set SD wide bus enable
 *  @param fgSet    TRUE: 4 bits data line, FALSE: 1 bit data line.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID SDCHW_Set4BitOn(BOOL fgSet)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);

    if (fgSet)
    {
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | CTRL_SD_WIDEBUSEN);

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)
        CKGEN_CLRBIT(REG_RW_PAD_CFG_5, ((UINT32)1 <<14));
        // 0x7000_00d4 bit[14] set to 1'b0, SD dat1 be bi-directional mode
#elif ((CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550) || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)) 
     #if (CONFIG_DRV_FPGA_BOARD == 1)
        FCI_WRITE32(0x58, FCI_READ32(0x58) & ~((UINT32)0x10));   // For 8550/8555 FPGA test only
     #else //mhzhang
        CKGEN_CLRBIT(0xE0,(( (UINT32)1)<<27)); //2009/11/5, [27]=0 enalbe PAD 4 bit mode
     #endif
#endif

        /* // For MT5362A/B
        // MT5360 Add : CKGEN 0x2000d410 [1]
        u4Val = CKGEN_READ32(REG_PINMUX_MISC); // MISC regiter
        u4Val &= ~(1U<<1);
        u4Val |= (0U<<1);   // 4 bit mode
        CKGEN_WRITE32(REG_PINMUX_MISC, u4Val);
        */
    }
    else
    {
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val&(~CTRL_SD_WIDEBUSEN));
#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)
        CKGEN_SETBIT(REG_RW_PAD_CFG_5, ((UINT32)1 <<14));
        // When SDIO 1-bit mode is needed, 0x7000_00d4 bit[14] should be set to 1'b1
#elif ((CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550) || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555))
     #if(CONFIG_DRV_FPGA_BOARD == 1)
        FCI_WRITE32(0x58, FCI_READ32(0x58) | (UINT32)0x10);   // For 8550/8555 FPGA test only
     #else // mhzhang
        CKGEN_SETBIT(0xE0, (((UINT32)1)<<27)); // 2009/11/5	 
     #endif      
#endif

        /* // For MT5362A/B
        // MT5360 Add : CKGEN 0x2000d410 [1]
        u4Val = CKGEN_READ32(REG_PINMUX_MISC); // MISC regiter
        u4Val &= ~(1U<<1);
        u4Val |= (1U<<1);   // 1 bit mode
        CKGEN_WRITE32(REG_PINMUX_MISC, u4Val);
        */
    }
}

//-------------------------------------------------------------------------
/** SDCHW_ReadResp
 *  read SD response.
 *  @param index    response index, range in 1~4..
 *  @return  response value.
 */
//-------------------------------------------------------------------------
UINT32 SDCHW_ReadResp(INT32 index)
{
    return FCI_READ32((UINT32)(RO_SD_RESP1_REG + (4 *(index - 1))));
}

//-------------------------------------------------------------------------
/** SDCHW_OpenWBFInt
 *  open SDWBF Interrupt and turn off SDDR interrupt.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID SDCHW_OpenWBFInt(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 use INTR_SD_DTRDYI instead
    u4Val &= ~INTR_SD_PFI;
    //u4Val |= INTR_SD_PFI | INTR_SD_CMDRDYI;
    u4Val |= INTR_SD_DTRDYI;
#else
    u4Val &= ~INTR_SD_DTRDYI;
    //u4Val |= INTR_SD_PFI | INTR_SD_CMDRDYI;
    u4Val |= INTR_SD_PFI;
#endif
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** SDCHW_WaitDataIdle
 *  wait the end of SD write buffer busy.
 *  @param VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 SDCHW_WaitDataIdle(VOID)
{
#ifdef FCIHW_POLLING
    UINT32 u4New, u4Time;
#endif

    INT32 i4Ret;
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE) && (CONFIG_DRV_FPGA_BOARD == 1)
    INT32 iCount = 0;
#endif

    if (_fgFCIIsrEn)
    {
#ifdef CC_CARD_DETECT_PIN
         // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

#if !(FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 use INTR_SD_DTRDYI instead
        //i4Ret = x_sema_lock_timeout(_hFCISDWBFSema, _u4FciWaitIntTime);
        i4Ret = x_sema_lock(_hFCISDWBFSema, X_SEMA_OPTION_WAIT);
#else
        //i4Ret = x_sema_lock_timeout(_hFCISDDRSema, _u4FciWaitSitTime);
        i4Ret = x_sema_lock(_hFCISDDRSema, X_SEMA_OPTION_WAIT);
#endif

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE) && (CONFIG_DRV_FPGA_BOARD == 1)
        if (iCount >= 1000)
#else
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
#endif
        {
            LOG(1, "_hFCISDWBFSema lock timeout <= Failed @@\n");
        }

        i4Ret = _Sdchw_WaitWBFBit();

        if (i4Ret == E_NO_RESPONSE)
        {
            LOG(1, "Wait Idle Err of no response IntrStat:0x%08x <= Failed @@\n",
            FCI_READ32(RW_FCI_INTR_STAT_REG));
        }
        return i4Ret;
    }

#ifdef FCIHW_POLLING
    u4New = (u4Time = _Fcihw_GetSec());

    while ((u4New - u4Time) < FCI_ISR_TIMEOUT_PERIOD)
    {
        i4Ret = _Sdchw_WaitWBFBit();

        if (i4Ret != E_NO_RESPONSE)
        {
            return i4Ret;
        }
        u4New = _Fcihw_GetSec();
    }
#endif

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** SDCHW_OpenDataRdyInt
 *  open SDDR Interrupt and turn off SDWBF interrupt.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID SDCHW_OpenDataRdyInt(VOID)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    //u4Val &= ~(INTR_SD_PFI |INTR_SD_CMDRDYI);
    u4Val &= ~(INTR_SD_PFI);
    u4Val |= INTR_SD_DTRDYI;
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
}

//-------------------------------------------------------------------------
/** SDCHW_WaitDataRdyOrTo
 *  Wait data line ready. Use in data read operation.
 *  @param VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 SDCHW_WaitDataRdyOrTo(VOID)
{
#ifdef FCIHW_POLLING
    UINT32 u4New, u4Time;
#endif

    INT32 i4Ret;
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE) && (CONFIG_DRV_FPGA_BOARD == 1)
    INT32 iCount = 0;
#endif

    if (_fgFCIIsrEn)
    {
#ifdef CC_CARD_DETECT_PIN
         // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        //i4Ret = x_sema_lock_timeout(_hFCISDDRSema, _u4FciWaitIntTime);
        i4Ret = x_sema_lock(_hFCISDDRSema, X_SEMA_OPTION_WAIT);

        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "_hFCISDDRSema lock timeout <= Failed @@\n");
        }

        i4Ret = _Sdchw_WaitDataRdyBit();

        if (i4Ret == E_NO_RESPONSE)
        {
            LOG(1, "Wait DataRdy Err of no response IntrStat:0x%08x <= Failed @@\n",
            FCI_READ32(RW_FCI_INTR_STAT_REG));
        }

        // wait FIFO empty.
        _Fcihw_CheckFifo();

        return i4Ret;
    }

#ifdef FCIHW_POLLING
    u4New = (u4Time = _Fcihw_GetSec());

    while ((u4New - u4Time) < FCI_ISR_TIMEOUT_PERIOD)
    {
        i4Ret = _Sdchw_WaitDataRdyBit();

        if (i4Ret != E_NO_RESPONSE)
        {
            return i4Ret;
        }
        u4New = _Fcihw_GetSec();
    }
#endif

    return E_NO_RESPONSE;
}

//-------------------------------------------------------------------------
/** SDCHW_SendCmd
 *  Send SD command and argument.
 *  @param u4Cmd    SD command.
 *  @param u4Arg      SD argument.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 SDCHW_SendCmd(UINT32 u4Cmd, UINT32 u4Arg)
{
    INT32 i4Ret;
    UINT32 u4Val;
    UINT32 u4Count = 0;

#if 0//(CONFIG_CHIP_VER_CURR < CONFIG_CHIP_VER_MT8550)     // MT8550/8555 solve this problem
    UINT32 u4CmdRB, u4RetryCnt = 0;
#endif

#ifndef FCIHW_INTERRUPT_WRITE_CLEAR
    // Because we need to check CR, DR, WBF semaphore according to interrupt status,
    // use interrupt reset to clear these interrupt status bits.
    _Fcihw_ResetInt();
#endif

    // Make sure all the semaphore is in lock state.
    SDCHW_InitSemaphore();

    FCI_WRITE32(RW_SD_ARG_REG, u4Arg);
    //x_thread_delay(1);
    LOG(7, "SdSendCmd Cmd:0x%08x Arg:0x%08x\n", u4Cmd, u4Arg);

#if 0//(CONFIG_CHIP_VER_CURR < CONFIG_CHIP_VER_MT8550)     // MT8550/8555 solve this problem
Retry:

    FCI_WRITE32(RW_SD_CTRL_REG, u4Cmd);

    u4CmdRB = FCI_READ32(RW_SD_CTRL_REG);

    if ((u4CmdRB != u4Cmd) && (u4RetryCnt < 1000))
    {
        u4RetryCnt++;
        goto Retry;
    }

    if(u4RetryCnt >= 1000)
    {
        LOG(1, "(%s,%d) Error : u4Cmd = 0x%x, u4CmdRB = 0x%x, Retry = %d\n", __FUNCTION__, __LINE__, u4Cmd, u4CmdRB, u4RetryCnt);
    }
#else
    FCI_WRITE32(RW_SD_CTRL_REG, u4Cmd);
#endif
        
    // wait command ready.
    i4Ret = _Sdchw_WaitCmdRdyOrTo();

    //NOTICE: R1B type response should wait for data line at 0.
    //handle R1B type response.
    if (SD_GET_TYPE(u4Cmd) == TYPE_R1b)
    {
        while (1)
        {
             // handle interrupt status only in card inserted state.
#ifdef CC_CARD_DETECT_PIN
            if (_fgFCICardRemove)
            {
                LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
                return E_NO_CARD;
            }
#endif

            u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
  #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // MT8550/8555 is different from MT53xx, RBI is not set as interrupt
            if (u4Val & INTR_SD_RESPBUSY)
  #else
            if (u4Val & INTR_FCI_SDRB)
  #endif
#else
            if (u4Val & INTR_SD_RESPBUSY)
#endif
            {
                u4Count++;

                LOG(7, "(%d) cmd=0x%08X, BUSY=0x%08X.\n", u4Count, u4Cmd, u4Val);

                // delay 10 ms and wait R1B response.
                //x_thread_delay(10);

                // set time out in 10 times of checking.
                // ASSERT(u4Count < 10);
                if (u4Count > 1000000)
                {
                    LOG(1, "%s(%d) Response Busy Error, u4Val = 0x%x, u4Count = %d !!\n", __FUNCTION__, __LINE__, u4Val, u4Count);
                    break;
                }
            }
            else
            {
                LOG(7, "Response busy u4Count = %d !!\n", u4Count);
                break;
            }
        }
    }

    if (i4Ret)
    {
        LOG(1, "%s(%d) SDCHW_SendCmd failed, u4Cmd = 0x%x, u4Arg = 0x%x\n", __FUNCTION__, __LINE__, u4Cmd, u4Arg);
    }
    
    return i4Ret;
}

//-------------------------------------------------------------------------
/** SDCHW_SetMultiFlag
 *  Set SD multiple block counter enable.
 *  @param fgMulti    TRUE: enable multi-block. FALSE: disable multi-block.
 *  @return  VOID.
 */
//-------------------------------------------------------------------------
VOID SDCHW_SetMultiFlag(BOOL fgMulti)
{
    if (fgMulti)
    {
        FCI_WRITE32(RW_FCI_CTRL_REG, FCI_READ32(RW_FCI_CTRL_REG) | CTRL_SD_MLBLCNEN);
    }
    else
    {
        FCI_WRITE32(RW_FCI_CTRL_REG,
        FCI_READ32(RW_FCI_CTRL_REG) & ~(CTRL_SD_MLBLCNEN));
    }
}

//-------------------------------------------------------------------------
/** SDCHW_SetSDIOInt
 *  Enable/Disable SDIO interrupt.
 *  @param fgEnable    Enable/Disable SDIO interrupt.
 *  @retval  void
 */
//-------------------------------------------------------------------------
#ifdef CC_FCI_SDIO_SUPPORT
void SDCHW_SetSDIOInt(BOOL fgEnable)
{
    UINT32 u4Val;

    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    if (!fgEnable)
    {
        u4Val &= ~(INTR_SDIO_INT);
    }
    else
    {
        u4Val |= INTR_SDIO_INT;
    }
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);
}
#endif  // #ifdef CC_FCI_SDIO_SUPPORT

/*
 * MSPHW export functions.
 */

//-------------------------------------------------------------------------
/** MSPHW_Init
 *  Initialize SD card interface.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID MSPHW_Init(VOID)
{ 
    _MSP_ChangeClock(MSP_NORM_CLOCK);

    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_MS_SIEN | FCI_MS_BSYCNT(0) | CTRL_MS_RST | CTRL_DRAM_CLKEN);

    if (_fgFCIContinueClock)
    {
        FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_MS_CCO | CTRL_MS_SIEN | FCI_MS_BSYCNT(0) | CTRL_DRAM_CLKEN);
    }
    else
    {
        FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_MS_SIEN | FCI_MS_BSYCNT(0) | CTRL_DRAM_CLKEN);
    }

    FCI_WRITE32(RW_MS_AUTO_CTRL_REG, CTRL_MS_RED);

    _Fcihw_ResetInt();
    _Fcihw_ResetFifo();

    if (_fgFCIIsrEn)
    {
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG,
        INTR_SRAMSELI | FCI_SEL_MS | INTR_MS_ALLI | INTR_FCI_ALLI);
    }
    else
    {
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG, INTR_SRAMSELI | FCI_SEL_MS);
    }
}

//-------------------------------------------------------------------------
/** MSPHW_IsEmpty
 *  Polling MS card is exist or not.
 *  @param VOID.
 *  @retval  TRUE: card is not existed.
 *  @retval  FALSE: card is existed.
 */
//-------------------------------------------------------------------------
BOOL MSPHW_IsEmpty(VOID)
{
    return _fgMsCardRemove;
}

//-------------------------------------------------------------------------
/** MSPHW_SetTpc
 *  set Transfer Protocol Command (TPC).
 *  @param pu4Data      point to command data.
 *  @param u4DataLen    command length.
 *  @param u4TpcCode    command code.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSPHW_SetTpc(const UINT32 *pu4Data, UINT32 u4DataLen, UINT32 u4TpcCode)
{
    INT32 i;

    UINT32 u4Val;
    MSP_T *prMSP;

    u4Val = FCI_READ32(RW_FCI_CTRL_REG);

    ASSERT(u4Val & CTRL_MS_SIEN); /* Must enable MS slot. */
    UNUSED(u4Val);

    prMSP = FCI_GetMSPSlot();
    _Msphw_CloseInt();   
    
    // Set read/write fifo direction and interrupt.
    switch (u4TpcCode)
    {
    case TPC_READ_REG:
    case TPC_GET_INT:
        _Msphw_OpenRdyInt();
        FCIHW_SetFifoRead();
        break;

    case TPC_READ_PAGE_DATA:
        if (prMSP->fg4BitEn && (!prMSP->fgLastpage))
        {
            _Msphw_OpenSitInt();
        }
        else
        {
            // serial mode only have RDY interrupt.
            _Msphw_OpenRdyInt();
        }
        FCIHW_SetFifoRead();
        break;

    case TPC_SET_CMD:
    case TPC_EX_SET_CMD:
    case TPC_WRITE_PAGE_DATA:
        _Msphw_OpenSitInt();
        FCIHW_SetFifoWrite();
        break;

    case TPC_SET_RW_REG_ADRS:
    case TPC_WRITE_REG:
        _Msphw_OpenRdyInt();
        FCIHW_SetFifoWrite();
        break;

    default:
        LOG(1, "Set wrong TpcCode:%x\n", u4TpcCode);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return E_FAIL;
    }

    if ((u4DataLen != 0) && (pu4Data != NULL))
    {
        // Reset Fifo, and then write data into fifo.
        _Fcihw_ResetFifo();
        for (i = 0; (UINT32)i < ((u4DataLen + 3) >> 2); i++)
        {
            FCI_WRITE32(WO_FCI_DATA_REG, pu4Data[i]);
        }
    }

#ifndef FCIHW_INTERRUPT_WRITE_CLEAR
    // Because we need to check RI semaphore according to interrupt status,
    // use interrupt reset to clear these interrupt status bits.
    _Fcihw_ResetInt();
#endif

    _Msphw_InitSemaphore();

    // Send TPC.
    LOG(7, "TPC set at 0x00 : 0x%08x\n", FCI_MS_PID(u4TpcCode) | FCI_MS_DATA(u4DataLen));

    FCI_WRITE32(RW_MS_CTRL_REG, FCI_MS_PID(u4TpcCode) | FCI_MS_DATA(u4DataLen));

#if 0 // Read Back check if the command is written successfully
    {
        UINT32 u4Tmp;
    
        u4Tmp = FCI_READ32(RW_MS_CTRL_REG);
        if ((u4Tmp & 0x0000F3FF) != (FCI_MS_PID(u4TpcCode) | FCI_MS_DATA(u4DataLen)))
        {
            UTIL_Printf("(%s) Read Back = 0x%08X, Espect = 0x%08X !!\n", __FUNCTION__, u4Tmp, (FCI_MS_PID(u4TpcCode) | FCI_MS_DATA(u4DataLen)));
        }
    }
#endif

    return S_OK;
}

//-------------------------------------------------------------------------
/** MSPHW_ReadParallelSif
 *  Read MS bus to get interrupt status and tranform into interrupt register value.
 *  @param VOID
 *  @return  MS INT register value.
 */
//-------------------------------------------------------------------------
UINT32 MSPHW_ReadParallelSif(VOID)
{
    UINT32 u4Val, u4Ret;

    u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
    u4Ret = ((u4Val & INTR_MS_CED) ? INT_CED : 0x00);
    u4Ret |= ((u4Val & INTR_MS_ERR) ? INT_ERR : 0x00);
    u4Ret |= ((u4Val & INTR_MS_BREQ) ? INT_BREQ : 0x00);
    u4Ret |= ((u4Val & INTR_MS_CMDNK) ? INT_CMDNK : 0x00);
    return u4Ret;
}

//-------------------------------------------------------------------------
/** MSPHW_WaitStatusBit
 *  Wait MS card status bit ready.
 *  @param u1BitChar    check bit field.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 MSPHW_WaitStatusBit(UINT8 u1BitChar)
{
    INT32 i4Ret = E_FAIL;

    UINT32 u4StatusBit;
#ifdef FCIHW_POLLING
    HAL_TIME_T timeOrg, timeNew, timeDelta;
#endif
    switch (u1BitChar)
    {
    case 'R':
    case 'r': /* Ready */
        u4StatusBit = INTR_MS_RDYI;
        break;

    case 'S':
    case 's': /* Serial Interrupt Transfer State */
    #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
        u4StatusBit = INTR_MS_RDYI;       // MT8550/8555, check RDY first then SIT.
    #else
        u4StatusBit = INTR_MS_SITI;
    #endif
        break;

    default:
        return E_FAIL;
    }

    // interrupt mode.
    if (_fgFCIIsrEn)
    {
#ifdef CC_CARD_DETECT_PIN
         // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        i4Ret = _Msphw_CheckBit(u4StatusBit);
        if (i4Ret == S_OK)
        {
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
            if (u1BitChar == 'S' || u1BitChar == 's')
            {
                goto MS_SIT_CHECK;
            }
#endif
            return S_OK;
        }

        i4Ret = x_sema_lock_timeout(_hFCIMSRDYSema, _u4FciWaitIntTime);

        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
#ifndef FCIHW_INTERRUPT_WRITE_CLEAR
            LOG(1, "MS Rdy Sema lock timeout, IntrStat:0x%08x\n",
                FCI_READ32(RW_FCI_INTR_STAT_REG));
#else
            LOG(1, "MS Rdy Sema lock timeout, _u4FCISStatus = 0x%08x, IntrStat:0x%08x\n", _u4FCISStatus,
                FCI_READ32(RW_FCI_INTR_STAT_REG));
#endif
        }

        i4Ret = _Msphw_CheckBit(u4StatusBit);

        if (i4Ret == E_NO_RESPONSE)
        {
            LOG(1, "(%d) WaitBit:0x%08x Err of no response IntrStat:0x%08x\n", __LINE__, u4StatusBit,
            FCI_READ32(RW_FCI_INTR_STAT_REG));
        }

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
        // Check SIT interrupt if RDYI
        if (i4Ret == S_OK && (u1BitChar == 'S' || u1BitChar == 's'))
        {    
MS_SIT_CHECK:

            i4Ret = _Msphw_CheckBit(INTR_MS_SITI);
            if (i4Ret == S_OK)
            {
                return S_OK;
            }
            
            i4Ret = x_sema_lock_timeout(_hFCIMSSITSema, _u4FciWaitSitTime);
            
            ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

            if (i4Ret == OSR_TIMEOUT)
            {
                LOG(1, "(%d) MS SIT Sema lock timeout, _u4FCISStatus = 0x%08x, IntrStat:0x%08x\n", __LINE__, _u4FCISStatus,
                FCI_READ32(RW_FCI_INTR_STAT_REG));
            }

            i4Ret = _Msphw_CheckBit(INTR_MS_SITI);

            if (i4Ret == E_NO_RESPONSE)
            {
                LOG(1, "(%d) WaitBit:0x%08x Err of no response IntrStat:0x%08x, 0x%08x\n", __LINE__, u4StatusBit,
                _u4FCISStatus, FCI_READ32(RW_FCI_INTR_STAT_REG));
            }
        }
#endif

        return i4Ret;
    }

#ifdef FCIHW_POLLING
    // polling mode.
    HAL_GetTime(&timeOrg);

    while (1)
    {
        i4Ret = _Msphw_CheckBit(u4StatusBit);

        if (i4Ret != E_NO_RESPONSE)
        {
            return i4Ret;
        }

        HAL_GetTime(&timeNew);
        HAL_GetDeltaTime(&timeDelta, &timeOrg, &timeNew);
        if (timeDelta.u4Seconds > FCI_ISR_TIMEOUT_PERIOD)
        {
            i4Ret = _Msphw_CheckBit(u4StatusBit);
            return i4Ret;
        }
    }
#else
    return i4Ret;
#endif
}

//-------------------------------------------------------------------------
/** MSPHW_ReadRegData
 *  Read MS card register value.
 *  @param pu1Reg    point to register buffer.
 *  @param u4RdSz    read size.
 *  @retval  VOID
 */
//-------------------------------------------------------------------------
VOID MSPHW_ReadRegData(UINT8 *pu1Reg, UINT32 u4RdSz)
{
    UINT32 i;

    UINT32 u4Val = 0;
    INT32 i4Ret;

    ASSERT(pu1Reg != NULL);

    // transfer 32 bits register into 8 bits user's register buffer.
    for (i = 0; i < u4RdSz; i++)
    {
        if ((i & 0x3) == 0)
        {
            // Read FIFO must not be empty.
            i4Ret = _Fcihw_WaitRxBufNotEmpty();
            UNUSED(i4Ret);
            u4Val = FCI_READ32(RW_RD_DATA_REG);
        }
        else
        {
            u4Val = (u4Val >> 8);
        }
        pu1Reg[i] = (u4Val & 0x0ff);
    }
}

#ifdef CC_FCI_SMXD_SUPPORT
//-------------------------------------------------------------------------
/** SMXDHW_IsEmpty
 *  Polling xD card is exist or not.
 *  @param VOID.
 *  @retval  TRUE: card is not existed.
 *  @retval  FALSE: card is existed.
 */
//-------------------------------------------------------------------------
BOOL SMXDHW_IsEmpty(VOID)
{
#ifdef CC_CARD_DETECT_PIN
    return _fgFCICardRemove;
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------
/** SMXDHW_Init
 *  Initialize SM/xD interface.
 *  @param VOID.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
#ifndef SMXD_USING_NAND_IF
VOID SMXDHW_Init(VOID)
{
    //CKGEN_AgtSelClk(e_CLK_SM, CLK_CLK_SM_SEL_27M_1_2);
    //CLKSRC_InitFCIClock();

    FCI_WRITE32(RW_FCI_CTRL_REG,
                CTRL_SM_RST | CTRL_SM_PWS | CTRL_FCI_FIFORST | CTRL_FCI_INTRST | CTRL_DRAM_CLKRST);

    // FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SM_SIEN | CTRL_SM_DATASWAP |
    //     CTRL_SM_CCB(0) | CTRL_DRAM_CLKEN);

    // Max Liao, external use 20 MHz in fpga, 2006/03/23.
    FCI_WRITE32(RW_FCI_CTRL_REG,
                CTRL_SM_SIEN | CTRL_SM_DATASWAP | CTRL_SM_CCB(1) | CTRL_DRAM_CLKEN);

    _Fcihw_ResetInt();

    _Fcihw_ResetFifo();

    if (_fgFCIIsrEn)
    {
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG,
                    INTR_SRAMSELI | FCI_SEL_SM | INTR_SM_ALLI | INTR_FCI_ALLI);
    }
    else
    {
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG, INTR_SRAMSELI | FCI_SEL_SM);
    }
}

//-------------------------------------------------------------------------
/** SMXDHW_SendCmd
 *  Send SD command and argument.
 *  @param u4Cmd    SD command.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------
INT32 SMXDHW_SendCmd(UINT32 u4Cmd)
{
    INT32 i4Ret;

    // Because we need to check RI semaphore according to interrupt status,
    // use interrupt reset to clear these interrupt status bits.
    _Fcihw_ResetInt();

    // Make sure all the semaphore is in lock state.
    _Smxdhw_InitSemaphore();

    FCI_WRITE32(RW_SM_CTRL_REG, u4Cmd);

    // wait interrupt.
    i4Ret = _Smxdhw_WaitDataRdyOrTo();

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    return i4Ret;
}
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT
#endif  // #ifdef CC_FCI_ENABLE
