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
 * $Date: 2012/04/27 $
 * $RCSfile: fcihw_reg.c,v $
 * $Revision: #1 $
 *
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

#include "sdc_func.h"
#include "fci_type.h"
#include "fci_debug.h"
#include "fcihw_reg.h"
#include "fcihw_io.h"
#include "fcihw_func.h"
#include "fci_if.h"

#ifdef FCI_SDIO_SUPPORT
#include "sdio_func.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_clksrc.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END

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

// For MT5362A/B
//#define SD_CONTINUE_CLOCK
//#define MSP_CONTINUE_CLOCK

#if 0
#ifndef CC_NO_CARD_DETECT_PIN
#define FCI_CARD_DETECT_PIN     ((INT32)11)
#endif
#endif

#define CKGEN_SETBIT(offset, dBit)        CKGEN_WRITE32(offset, CKGEN_READ32(offset) | dBit)
#define CKGEN_CLRBIT(offset, dBit)        CKGEN_WRITE32(offset, CKGEN_READ32(offset) & (~dBit))

UINT32 _u4SDClockSetting = 0;
UINT32 _u4MSClockSetting = 0;
extern BOOL  _fgFCI4BitBusEnable;

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

/// MS RDY Binary semaphore.
static HANDLE_T _hFCIMSRDYSema;

/// SD DR Binary semaphore.
static HANDLE_T _hFCISDDRSema;

/// SD CR Binary semaphore.
static HANDLE_T _hFCISDCRSema;

/// SD WBF Binary semaphore.
static HANDLE_T _hFCISDWBFSema;

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR

/// SD RBI Binary semaphore.
static HANDLE_T _hFCISDRBISema;

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
#ifndef CC_NO_CARD_DETECT_PIN

#if 0
/// Previous card detection interrupt status.
static UINT32 _u4FCICDStatus = INTR_FCI_CDI;
#endif

/// Card remove status: FALSE = card inserted, TRUE = card removed.
static INT32 _fgFCICardRemove = TRUE;

extern INT32 _fgSdCardRemove;
extern INT32 _fgMsCardRemove;

#if 0
static FCI_CALLBACK _pfnCardDetect = NULL;

static UINT32 _u4FCICardDetectPin = FCI_CARD_DETECT_PIN;
#endif

#endif

// SDIO interrupt handler.
#ifdef FCI_SDIO_SUPPORT
static FCI_CALLBACK _pfnSDIOINT = NULL;
#endif

//static INT16 _u2FciIsrCnt = 0;

// SRAM FIFO Setting
static UINT32 _u4SRAMSel = 1;   // 01: SRAM size is 32 x 32 bits (128 bytes).

UINT32 _u4SDMultiFuncCfg = SD_MLT_FNC_CFG;
UINT32 _u4MSultiFuncCfg = MS_MLT_FNC_CFG;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#ifndef CC_NO_CARD_DETECT_PIN
//-------------------------------------------------------------------------
/** _Fcihw_GetCardStatus
 *  Get SD, MS, xD detection status.
 *  @param  VOID
 *  @return mask all cards detection status, 1 = card remove, 0 = card insert.
 */
//-------------------------------------------------------------------------
static INT32 _Fcihw_GetCardStatus(VOID)
{
#if 1
    //以下CKGEN 的 register 都 program成 0，VIND7才可做 GPIO 用：
    CKGEN_CLRBIT(0xC0, ((UINT32)0x01 << 26));    // 00C0[26]
    CKGEN_CLRBIT(0xC8, ((UINT32)0xFF << 24) | ((UINT32)0x3F << 16));    // 00C8[31:24], 00C8[22:16]
    CKGEN_CLRBIT(0xCC, ((UINT32)0x0F << 0));    // 00CC[3:0]
    CKGEN_CLRBIT(0xD0, ((UINT32)0x3F << 22));    // 00D0[28:22]
    CKGEN_CLRBIT(0xD4, ((UINT32)0x01 << 3));    // 00D4[3]
    
    // 設定I/O方向（0：input、1：output）
    CKGEN_CLRBIT(0x188, ((UINT32)0x01 << 4));    // 0188[4]

    _fgFCICardRemove = (CKGEN_READ32(0x1C8) & ((UINT32)0x01 << 4)) ? TRUE : FALSE; // 01C8[4]：input value

    _fgSdCardRemove = _fgMsCardRemove = _fgFCICardRemove;
    
    if (_fgFCICardRemove)
    {
        LOG(1, "Card Not Insert !!\n");
    }
    
    return _fgFCICardRemove;
#else
    return (GPIO_Input(_u4FCICardDetectPin) ? INTR_FCI_CDI : 0);
#endif
}
#endif    // #ifndef CC_NO_CARD_DETECT_PIN

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
#ifndef CC_NO_CARD_DETECT_PIN
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
#endif  // #ifndef CC_NO_CARD_DETECT_PIN
#endif

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
#ifdef CC_NO_CARD_DETECT_PIN
    return 1;
#else
    return _Fcihw_GetCardStatus();
#endif

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

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
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
#endif

#ifdef FCI_SDIO_SUPPORT
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
#endif  // #ifdef FCI_SDIO_SUPPORT
        break;

    case MSP_SLOT:
        if (INTR_MS_ALLI & u4IntrStatus)
        {
            // unlock the specific type semaphore.
            VERIFY(OSR_OK == x_sema_unlock(_hFCIMSRDYSema));
            LOG(7, "MSRI unlock.\n");

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // Save FCI Status for further usage
            _u4FCISStatus |= (FCI_READ32(RW_FCI_INTR_STAT_REG) & INTR_MS_ALLI);

            // Clear the interrupt
            FCI_WRITE32(RW_FCI_INTR_STAT_REG, INTR_MS_ALLI);
#endif
        }
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
#ifndef CC_NO_CARD_DETECT_PIN
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
    u4Status &= FCI_READ32(RW_FCI_INTR_CTRL_REG);

    LOG(7, "***** FCI HW Isr status is 0x%08x state 0x%08x*****\n", u4Status);

#if 0
#ifndef CC_NO_CARD_DETECT_PIN
    // handle card detection.
    _Fcihw_DetectCard(u4Status);
#endif
#endif

#ifndef CC_NO_CARD_DETECT_PIN
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

    // Disable Slot interrupt.
    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    u4Val &= ~(u4SlotIntr);
    u4Val &= ~(INTR_SRAMSELI);
    u4Val |= FCI_SRAMSEL(_u4SRAMSel);
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, u4Val);

    // Reset FCI interrupt.
    _Fcihw_ResetInt();

    // Reset FCI FIFO.
    _Fcihw_ResetFifo();

    // Reset FCI Dram domain.
    _Fcihw_ResetDramClk();
    
    // Reset slot.
    u4Val = FCI_READ32(RW_FCI_CTRL_REG);
    ASSERT((u4Val&(u4SlotRest)) == 0);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val | u4SlotRest);
    FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);
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

    // write Slot interrupt enable bit.
    u4Val = FCI_READ32(RW_FCI_INTR_CTRL_REG);
    u4Val &= ~(INTR_FCISELI | INTR_FCI_ALLI | u4SlotIntr);

    if (_fgFCIIsrEn)
    {
#ifdef CC_MT5360B
        // Bit 26 Set 1 : Let SDDI updated when command finish
        u4Val |= u4SlotIntr | u4SelSlot | INTR_FCI_ALLI | INTR_SDDI_DIRECT_IN;
#else
        u4Val |= u4SlotIntr | u4SelSlot | INTR_FCI_ALLI;
#endif
    }
    else
    {
#ifdef CC_MT5360B
        u4Val |= u4SelSlot | INTR_SDDI_DIRECT_IN;
#else
        u4Val |= u4SelSlot;
#endif
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

#ifndef CC_NO_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    u4Val = _u4FCISStatus;
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
    else if (u4Val & INTR_SD_NWRITEBUSY)
    {
        LOG(7, "%s(%d) %s IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__,
        FCI_READ32(RW_FCI_INTR_STAT_REG));
        return S_OK;
    }

    LOG(1, "Wait Idle is timeout. IntrStat:0x%08x\n", u4Val);
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

#ifndef CC_NO_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    u4Val = _u4FCISStatus;
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
    else if (u4Val & INTR_SD_DTRDYI)
    {
        LOG(7, "%s(%d) %s ok IntrStat:0x%08x\n", __FILE__, __LINE__, __FUNCTION__, u4Val);
        return S_OK;
    }

    LOG(1, "Wait DataRdy is timeout. IntrStat:0x%08x\n", u4Val);
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

#ifndef CC_NO_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    u4Val = _u4FCISStatus;
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
    else if (u4Val & INTR_SD_CMDRDYI)
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
#ifndef CC_NO_CARD_DETECT_PIN
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        i4Ret = x_sema_lock_timeout(_hFCISDCRSema, FCI_WAIT_INT_TIME);

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

#ifndef CC_NO_CARD_DETECT_PIN
     // handle interrupt status only in card inserted state.
    if (_fgFCICardRemove)
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
        return E_NO_CARD;
    }
#endif

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    u4Val = _u4FCISStatus;
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
    else if (u4Val & u4StatusBit)
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
    u4Val |= INTR_MS_SITI;
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
    INT32 i4Ret;

    i4Ret = x_sema_lock(_hFCIMSRDYSema, X_SEMA_OPTION_NOWAIT);
    //ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    if (!((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK)))
    {
        ASSERT(0);
    }
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

        i4Ret = x_sema_lock_timeout(_hFCISMRDYSema, FCI_WAIT_INT_TIME);

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

//-------------------------------------------------------------------------
/** CLKSRC_ChangeMSPClock
 *  Dummy function
 *  @param  VOID.
 *  @retval  S_OK  Success.
 *  @retval  Others  Fail.
 */
//-------------------------------------------------------------------------

VOID CLKSRC_ChangeMSPClock(INT32 i4ClkMHz)
{
    UNUSED(i4ClkMHz);
}

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

VOID FCIHW_SetSDIOINTFct(FCI_CALLBACK pfnCallback)
{
#ifdef FCI_SDIO_SUPPORT
    _pfnSDIOINT = pfnCallback;
#else
   UNUSED(pfnCallback);
#endif
}

//-------------------------------------------------------------------------
/** FCIHW_SetCardDetectFct
 *  Register FCI card insert/remove call back function.
 *  NOTICE: This function will be called in FCI HISR.
 *  @param  pfnCallback   call back funtion pointer.
 *  @return  VOID
 */
//-------------------------------------------------------------------------

VOID FCIHW_SetCardDetectFct(FCI_CALLBACK pfnCallback)
{
#ifndef CC_NO_CARD_DETECT_PIN
    _pfnCardDetect = pfnCallback;
#else
    UNUSED(pfnCallback);
#endif
}

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
#ifndef CC_NO_CARD_DETECT_PIN
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
            // SD data ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDDRSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

            // SD cmd ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDCRSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

            // SD write buffer finish semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDWBFSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            // SD Response Busy interrupt semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISDRBISema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif

            // MS ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCIMSRDYSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

#ifndef SMXD_USING_NAND_IF
            // SM ready semaphore.
            VERIFY(OSR_OK == x_sema_create(&_hFCISMRDYSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));
#endif

#if 0
// MT5391 default insert SDIO card, don't need card detect pin
#ifndef CC_NO_CARD_DETECT_PIN
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

#ifndef CC_NO_CARD_DETECT_PIN
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

            // SD card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCISDDRSema));
            VERIFY(OSR_OK == x_sema_delete(_hFCISDCRSema));
            VERIFY(OSR_OK == x_sema_delete(_hFCISDWBFSema));

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            VERIFY(OSR_OK == x_sema_delete(_hFCISDRBISema));
#endif

            // MS card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCIMSRDYSema));

#ifdef CC_FCI_SMXD_SUPPORT
#ifndef SMXD_USING_NAND_IF
            // SM card semaphore.
            VERIFY(OSR_OK == x_sema_delete(_hFCISMRDYSema));
#endif  // #ifndef SMXD_USING_NAND_IF
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

#ifndef CC_NO_CARD_DETECT_PIN
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

    SDC_T *prSDC;
    MSP_T *prMSP;

    _i4FCIPrevSlot = i4Slot;

    // It must to disable interrupt before register setting.
    switch (i4Slot)
    {
    case NONE_SLOT:
        break;

    case SDC_SLOT:
        // disable interrupt.
#ifdef FCI_SDIO_SUPPORT
        _Fcihw_ResetSlot(INTR_SD_ALLI |INTR_SDIO_INT, CTRL_SD_RST);
#else
        _Fcihw_ResetSlot(INTR_SD_ALLI, CTRL_SD_RST);
#endif
        FCI_WRITE32(RW_SD_BUSYCNT_REG,
            MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);

        u4Val = FCI_READ32(RW_FCI_CTRL_REG);
        // Clear MS, SM part.
        u4Val &= 0xFF0000FF;
        u4Val |= CTRL_SD_MLBLCNEN | CTRL_SD_NEGDRIVE | CTRL_SD_DATASWAP | CTRL_SD_SIEN |CTRL_SD_RECEDGE;
        u4Val &= ~CTRL_SD_WIDEBUSEN;

        prSDC = FCI_GetSDCSlot();
        if (prSDC->fg4BitEn)
        {
            u4Val |= CTRL_SD_WIDEBUSEN;
        }
        FCI_WRITE32(RW_FCI_CTRL_REG, u4Val);

#ifdef FCI_SDIO_SUPPORT
        // turn on interrupt.
        _Fcihw_SetupSlot(INTR_SD_ALLI |INTR_SDIO_INT, FCI_SEL_SD);
#else
        // turn on interrupt.
        _Fcihw_SetupSlot(INTR_SD_ALLI, FCI_SEL_SD);
#endif
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
            FCI_WRITE32(RW_MS_AUTO_CTRL_REG, MS_APID(7) | MS_ADS(1) | CTRL_MS_RED | CTRL_MS_D5/*| CTRL_MS_ND*/);
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
            FCI_WRITE32(RW_MS_AUTO_CTRL_REG, MS_APID(7) | MS_ADS(1) | CTRL_MS_RED);

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

//-------------------------------------------------------------------------
/** FCIHW_PowerSwitch
 *  Initailize all FCI card.
 *  @param  fgPowerOn TRUE : power on, FALSE : power off
 *  @retval   none
 */
//-------------------------------------------------------------------------
VOID FCIHW_PowerSwitch(BOOL fgPowerOn)
{
    //----------- Power Control GPIO setting -----------//
    UNUSED(fgPowerOn);
}


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
    FCI_WRITE32(RW_FCI_INTR_CTRL_REG, FCI_SEL_SM | FCI_SRAMSEL(3));
    FCI_WRITE32(RW_SD_BUSYCNT_REG,
        MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);

    return S_OK;
}

//-------------------------------------------------------------------------
/** FCIHW_SDDrivingCurrent
 *  Set SD clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID FCIHW_SDDrivingCurrent(INT32 i4ClkMHz)
{
    switch(i4ClkMHz)
    {
        case 54:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
        }
        break;
        case 45:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x03) << 28));
        }
        break;
        case 40:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;
        case 33:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;
        case 30:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;
        case 27:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;
        case 25:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;        
        case 13:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;           
        case 9:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
        }
        break;        
        default:
        {
            CKGEN_WRITE32(0x320, (CKGEN_READ32(0x320) & ~((UINT32)0x30000000)) | (((UINT32)0x02) << 28));
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
    UNUSED(i4ClkMHz);
    
    switch(_u4MSultiFuncCfg)
    {    
        case 1:
        {
        }
        break;
        
        default:
        break;
    }
}

//-------------------------------------------------------------------------
/** FCIHW_ChangeMSPClock
 *  Set MSP clock
 *  @param i4ClkMHz    clock setting (Mhz).
 *  @return  VOID
 */
//-------------------------------------------------------------------------

VOID FCIHW_ChangeMSPClock(INT32 i4ClkMHz)
{
    switch(i4ClkMHz)    // unit : Mhz
    {
        case 54:
            {
                _u4MSClockSetting = 54;               
                break;
            }
            
        case 50:
            {
                _u4MSClockSetting = 50;
                break;
            }
            
        case 43:
            {
                _u4MSClockSetting = 43;
                break;
            }
            
        case 36:
            {
                _u4MSClockSetting = 36;
                break;
            }
            
        case 34:
            {
                _u4MSClockSetting = 34;
                break;
            }
            
        case 33:
            {
                _u4MSClockSetting = 33;
                break;
            }
            
        case 27:
            {
                _u4MSClockSetting = 27;
                break;
            }
                        
        case 13:
        default:
            {
                _u4MSClockSetting = 13;
                break;
            }
            
        case 6:
            {
                _u4MSClockSetting = 6;
                break;
            }
            
        case 3:
            {
                _u4MSClockSetting = 3;
                break;
            }
            
        case 1:
            {
                _u4MSClockSetting = 1;
                break;
            }
            
        case 0:
            {
                _u4MSClockSetting = 0;
                break;
            }
    }
    
    // Wait clock stable
    x_thread_delay(10);
    
    FCIHW_MSDrivingCurrent(i4ClkMHz);

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
            
        case 40:
            {
                _u4SDClockSetting = 40;
                CLKSRC_ChangeSDCClock(40);
                break;
            }
            
        case 33:
            {
                _u4SDClockSetting = 33;
                CLKSRC_ChangeSDCClock(33);
                break;
            }            
            
        case 30:
            {
                _u4SDClockSetting = 30;
                CLKSRC_ChangeSDCClock(30);
                break;
            }
            
        case 27:
            {
                _u4SDClockSetting = 27;
                CLKSRC_ChangeSDCClock(27);
                break;
            }
            
        case 25:
            {
                _u4SDClockSetting = 25;
                CLKSRC_ChangeSDCClock(25);
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

    HalFlushInvalidateDCache();
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

    HalFlushInvalidateDCache();
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
    HalFlushInvalidateDCache();
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

    i4Ret = x_sema_lock(_hFCISDDRSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    i4Ret = x_sema_lock(_hFCISDCRSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    i4Ret = x_sema_lock(_hFCISDWBFSema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
    i4Ret = x_sema_lock(_hFCISDRBISema, X_SEMA_OPTION_NOWAIT);
    ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
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
    _SDC_ChangeClock(SDC_LOW_CLOCK);

    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_RST | CTRL_DRAM_CLKRST | CTRL_DRAM_CLKEN);

    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_DRAM_CLKEN);

#ifdef SD_CONTINUE_CLOCK
    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_CCO | CTRL_SD_SIEN | CTRL_SD_NEGDRIVE | CTRL_DRAM_CLKEN | CTRL_SD_DATASWAP | CTRL_SD_RECEDGE);
#else
    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_SD_SIEN | CTRL_SD_NEGDRIVE | CTRL_DRAM_CLKEN | CTRL_SD_DATASWAP | CTRL_SD_RECEDGE);
#endif

    FCI_WRITE32(RW_SD_BUSYCNT_REG,
        MAX_NAC_BSYCNT | MAX_NWP_BSYCNT | MAX_NCR_BSYCNT |CTRL_FCI_SDWC);

    _Fcihw_ResetInt();
    _Fcihw_ResetFifo();

    // check if ISR is registered ?
    if (_fgFCIIsrEn)
    {
#ifdef FCI_SDIO_SUPPORT
        // turn on interrupt.
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG,
        INTR_SRAMSELI | FCI_SEL_SD | INTR_SD_ALLI |INTR_SDIO_INT | INTR_FCI_ALLI);
#else
        // turn on interrupt.
        FCI_WRITE32(RW_FCI_INTR_CTRL_REG,
        INTR_SRAMSELI | FCI_SEL_SD | INTR_SD_ALLI | INTR_FCI_ALLI);
#endif
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
#ifndef CC_NO_CARD_DETECT_PIN
    return _fgFCICardRemove;
#else
    return FALSE;
#endif
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
#ifndef CC_NO_CARD_DETECT_PIN
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
    u4Val &= ~INTR_SD_DTRDYI;
    //u4Val |= INTR_SD_PFI | INTR_SD_CMDRDYI;
    u4Val |= INTR_SD_PFI;
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

    if (_fgFCIIsrEn)
    {
#ifndef CC_NO_CARD_DETECT_PIN
         // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        //i4Ret = x_sema_lock_timeout(_hFCISDWBFSema, FCI_WAIT_INT_TIME);
        i4Ret = x_sema_lock(_hFCISDWBFSema, X_SEMA_OPTION_WAIT);

        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
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

    if (_fgFCIIsrEn)
    {
#ifndef CC_NO_CARD_DETECT_PIN
         // handle interrupt status only in card inserted state.
        if (_fgFCICardRemove)
        {
            LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
            return E_NO_CARD;
        }
#endif

        //i4Ret = x_sema_lock_timeout(_hFCISDDRSema, FCI_WAIT_INT_TIME);
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
 //   UINT32 u4CmdRB, u4RetryCnt = 0;

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

    FCI_WRITE32(RW_SD_CTRL_REG, u4Cmd);
/*    
Retry:

    u4CmdRB = FCI_READ32(RW_SD_CTRL_REG);

    if (u4CmdRB != u4Cmd && u4RetryCnt < 1000)
    {
        u4RetryCnt++;
        goto Retry;
    }

    if(u4RetryCnt >= 1000)
    {
        LOG(1, "(%s,%d) Error : u4Cmd = 0x%x, u4CmdRB = 0x%x, Retry = %d\n", __FUNCTION__, __LINE__, u4Cmd, u4CmdRB, u4RetryCnt);
    }
*/  
    // wait command ready.
    i4Ret = _Sdchw_WaitCmdRdyOrTo();

    //NOTICE: R1B type response should wait for data line at 0.
    //handle R1B type response.
    if (SD_GET_TYPE(u4Cmd) == TYPE_R1b)
    {
        while (1)
        {
             // handle interrupt status only in card inserted state.
#ifndef CC_NO_CARD_DETECT_PIN
            if (_fgFCICardRemove)
            {
                LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);
                return E_NO_CARD;
            }
#endif

            u4Val = FCI_READ32(RW_FCI_INTR_STAT_REG);
#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
            if (u4Val & INTR_FCI_SDRB)
#else
            if (u4Val & INTR_SD_RESPBUSY)
#endif
            {
                u4Count++;

                LOG(7, "(%d) cmd=0x%08X, BUSY=0x%08X.\n", u4Count, u4Cmd, u4Val);

                // delay 10 ms and wait R1B response.
                x_thread_delay(10);

                // set time out in 10 times of checking.
                // ASSERT(u4Count < 10);
                if (u4Count > 100)
                {
                    LOG(1, "%s(%d) Response Busy Error, u4Count = %d !!\n", __FUNCTION__, __LINE__, u4Count);
                    break;
                }
            }
            else
            {
                break;
            }
        }
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
#ifdef FCI_SDIO_SUPPORT
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
#endif  // #ifdef FCI_SDIO_SUPPORT

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
    FCIHW_ChangeMSPClock(20);

    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_MS_SIEN | FCI_MS_BSYCNT(0) | CTRL_MS_RST | CTRL_DRAM_CLKEN);

#ifdef MSP_CONTINUE_CLOCK
    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_MS_CCO | CTRL_MS_SIEN | FCI_MS_BSYCNT(0) | CTRL_DRAM_CLKEN);
#else
    FCI_WRITE32(RW_FCI_CTRL_REG, CTRL_MS_SIEN | FCI_MS_BSYCNT(0) | CTRL_DRAM_CLKEN);
#endif

    FCI_WRITE32(RW_MS_AUTO_CTRL_REG, MS_APID(7) | MS_ADS(1) | CTRL_MS_RED);

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
#ifndef CC_NO_CARD_DETECT_PIN
    return _fgFCICardRemove;
#else
    return FALSE;
#endif
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
        u4StatusBit = INTR_MS_SITI;
        break;

    default:
        return E_FAIL;
    }

    // interrupt mode.
    if (_fgFCIIsrEn)
    {
#ifndef CC_NO_CARD_DETECT_PIN
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
            return S_OK;
        }

        i4Ret = x_sema_lock_timeout(_hFCIMSRDYSema, FCI_WAIT_INT_TIME);

        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            LOG(1, "MS Sema lock timeout\n");
        }

        i4Ret = _Msphw_CheckBit(u4StatusBit);

        if (i4Ret == E_NO_RESPONSE)
        {
            LOG(6, "WaitBit:0x%08x Err of no response IntrStat:0x%08x\n", u4StatusBit,
            FCI_READ32(RW_FCI_INTR_STAT_REG));
        }
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
#ifndef CC_NO_CARD_DETECT_PIN
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
    CLKSRC_InitFCIClock();

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
