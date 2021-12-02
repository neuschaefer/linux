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
 * $RCSfile: ir_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ir_hw.c
 *  ir_hw.c provides IRRX/IRTX register access functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_hal_arm.h"

// local header files
#include "ir_reg.h"
#include "ir_io.h"
#include "ir_debug.h"
#include "ir_if.h"

// external header files
#include "x_lint.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_pdwnc.h"
#include "x_printf.h"
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MAX_IRRX_DATA       (4)

#ifdef CC_MTK_LOADER
#define IRRX_NON_OSAI
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef IRRX_NON_OSAI
static PFN_IRRXCB_T _pfnIrRxCbFunc = NULL;
#endif /* IRRX_NON_OSAI */


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _IrRxHwBusyClr() to clear IRRX h/w buffer then it can receive next message.
 */
//-----------------------------------------------------------------------------
static void _IrRxHwBusyClr()
{
    vIO32WriteFldAlign(PDWNC_IRCLR, IRRX_CLR_BSY_IR, FLD_IRCLR);//IRRX_WRITE32(IRRX_CLEAR_BUSY_REG, IRRX_CLR_BSY_IR);
}

#ifndef IRRX_NON_OSAI
//-----------------------------------------------------------------------------
/** _IrRxHwIsr() the interrupt of IRRX from PDWNC interrupt ISR.
 *  @param u2VctId It must be PINTNO_IRRX.
 */
//-----------------------------------------------------------------------------
static void _IrRxHwIsr(UINT16 u2VctId)
{
    UINT32 u4Info;
    UINT32 au4IrRxData[MAX_IRRX_DATA];

    UNUSED(u2VctId);

    /* read IRRX IRQ data. */
    u4Info = u4IO32Read4B(PDWNC_IRH);//IRRX_READ32(IRRX_COUNT_HIGH_REG);
    au4IrRxData[0] = u4IO32Read4B(PDWNC_IRM);//IRRX_READ32(IRRX_COUNT_MID_REG);
    au4IrRxData[1] = u4IO32Read4B(PDWNC_IRL);//IRRX_READ32(IRRX_COUNT_LOW_REG);

    LOG(7, "PWD:0x%08x RxIsr Info:0x%08x 0x%08x%08x\n",
            u4IO32Read4B(PDWNC_INTSTA),//IRRX_READ32(REG_RO_PDWNC_INTST),
            u4Info, au4IrRxData[1], au4IrRxData[0]);

    if ((u4Info != 0) && (_pfnIrRxCbFunc != NULL))
    {
        _pfnIrRxCbFunc(u4Info, (UINT8 *)au4IrRxData);
    }

    /* clear interrupt. */
    VERIFY(PDWNC_ClearInt(PINTNO_IRRX));

    /* Do BIM clear IR busy bit, after interrupt clear */
    _IrRxHwBusyClr();
}

#if 0
#ifdef CC_CLI
//-----------------------------------------------------------------------------
/** _IrTxHwIsr() the interrupt of IRTX from BIM IRQ.
 *  @param u2VctId It must be VECTOR_IRTX.
 */
//-----------------------------------------------------------------------------
static void _IrTxHwIsr(UINT16 u2VctId)
{
    static INT32 i4Count = 0;

    UNUSED(u2VctId);
    /* clear interrupt. */
    VERIFY(BIM_ClearIrq(VECTOR_IRTX));
    Printf("%s(%d) Cnt:%d\n", __FUNCTION__, __LINE__, i4Count++);
}
#endif /* CC_CLI */
#endif
#endif /* IRRX_NON_OSAI */

//-----------------------------------------------------------------------------
/** _IRHW_RxSetIsr() to enable/disable interrupt of IRRX
 *  IRRX interrupt is a sub-interrupt of PDWNC interrupt, the enable/disable \n
 *  function is to call PDWNC function to set the PDWNC interrupt enable \n
 *  register.
 *  @param fgSet a boolean value, true to enable ISR, false to disable.
 *  @retval 0 is operating successfully, otherwise failed.
 */
//-----------------------------------------------------------------------------
static INT32 _IRHW_RxSetIsr(BOOL fgSet)
{
#ifndef IRRX_NON_OSAI
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);

    if (fgSet)
    {   /* enable irq. */
        i4Val = PDWNC_RegIsr(PINTNO_IRRX, _IrRxHwIsr, &pfnOldIsr);
        if (i4Val != PDWNC_OK)
        {
            return IR_FAIL;
        }

        VERIFY(PDWNC_ClearInt(PINTNO_IRRX));
    }
    else
    {   /* disable irq. */
        i4Val = PDWNC_RegIsr(PINTNO_IRRX, NULL, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return IR_FAIL;
        }
    }
#endif /* IRRX_NON_OSAI */
    return IR_SUCC;
}

#ifndef IRRX_NON_OSAI
#if 0
#ifdef CC_CLI
//-----------------------------------------------------------------------------
/** _IRHW_TxSetIsr() to enable/disable interrupt of IRTX
 *  @param fgSet a boolean value, true to enable ISR, false to disable.
 *  @retval 0 is operating successfully, otherwise failed.
 */
//-----------------------------------------------------------------------------
static INT32 _IRHW_TxSetIsr(BOOL fgSet)
{
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);

    if (fgSet)
    { /* enable irq. */
        i4Val = x_reg_isr(VECTOR_IRTX, _IrTxHwIsr, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return IR_FAIL;
        }
        VERIFY(BIM_ClearIrq(VECTOR_IRTX));
    }
    else
    { /* disable irq. */
        i4Val = x_reg_isr(VECTOR_IRTX, NULL, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return IR_FAIL;
        }
    }
    return IR_SUCC;
}
#endif /* CC_CLI */
#endif
#endif /* IRRX_NON_OSAI */


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/* HWIRRX export functions */

#if 0
//-----------------------------------------------------------------------------
/** IRHW_RxHwState() to get the IRRX internal state machine
 */
//-----------------------------------------------------------------------------
UINT32 IRHW_RxHwState()
{
    UINT32 u4Val, u4Val1, u4Val2;
    CRIT_STATE_T rState;
    /*
    rState = x_crit_start();
    u4Val1 = CKGEN_READ32(REG_PINMUX_SEL0);
    u4Val2 = BIM_READ32(REG_RW_DBG_MONSEL);
    CKGEN_WRITE32(REG_PINMUX_SEL0, 0x333c0);
    BIM_WRITE32(REG_RW_DBG_MONSEL, 0x60001);
    u4Val = CKGEN_READ32(REG_GPIO_IN1);
    u4Val = (u4Val >> 1) & 0x7;
    CKGEN_WRITE32(REG_PINMUX_SEL0, u4Val1);
    BIM_WRITE32(REG_RW_DBG_MONSEL, u4Val2);
    x_crit_end(rState);
    */
    return u4Val;
}
#endif

//-----------------------------------------------------------------------------
/** IRHW_RxRdConf() to read IRRX configration
 *  @param pi4Config to read IRRX_CONFIG_HIGH_REG
 *  @param pi4SaPeriod to read IRRX_CONFIG_LOW_REG
 *  @param pi4Threshold to read IRRX_THRESHOLD_REG
 */
//-----------------------------------------------------------------------------
void IRHW_RxRdConf(INT32 *pi4Config, INT32 *pi4SaPeriod, INT32 *pi4Threshold)
{
    if ((pi4Config == NULL) || (pi4SaPeriod==NULL) || (pi4Threshold==NULL))
    {
        ASSERT(0);
        LINT_SUPPRESS_BRACE(527);
        return;
    }
    *pi4Config = (INT32)u4IO32Read4B(PDWNC_IRCFGH);//IRRX_READ32(IRRX_CONFIG_HIGH_REG);
    *pi4SaPeriod = (INT32)u4IO32Read4B(PDWNC_IRCFGL);//IRRX_READ32(IRRX_CONFIG_LOW_REG);
    *pi4Threshold = (INT32)u4IO32Read4B(PDWNC_IRTHD);//IRRX_READ32(IRRX_THRESHOLD_REG);
}

//-----------------------------------------------------------------------------
/** IRHW_RxWrConf() to set IRRX configration
 *  @param i4Config to set IRRX_CONFIG_HIGH_REG
 *  @param i4SaPeriod to set IRRX_CONFIG_LOW_REG
 *  @param i4Threshold to set IRRX_THRESHOLD_REG
 */
//-----------------------------------------------------------------------------
void IRHW_RxWrConf(INT32 i4Config, INT32 i4SaPeriod, INT32 i4Threshold)
{   
    /* Set IRRX clock to 3Mhz / 64 */
    vIO32WriteFldAlign(PDWNC_IRCKSEL, IRRX_CLK_3MHZ_DIV_64, FLD_IRCLKSEL);//PDWNC_WRITE32(PDWNC_IRCKSEL, IRRX_CLK_3MHZ_DIV_64);

    /* Config IRRX registers */
    vIO32Write4B(PDWNC_IRCFGH, (UINT32)i4Config);//IRRX_WRITE32(IRRX_CONFIG_HIGH_REG, (UINT32)i4Config);
    vIO32Write4B(PDWNC_IRCFGL, (UINT32)i4SaPeriod);//IRRX_WRITE32(IRRX_CONFIG_LOW_REG, (UINT32)i4SaPeriod);
    vIO32Write4B(PDWNC_IRTHD, (UINT32)i4Threshold);//IRRX_WRITE32(IRRX_THRESHOLD_REG, (UINT32)i4Threshold);
}

//-----------------------------------------------------------------------------
/** IRHW_Init() to set IRRX configration and start IRRX ISR
 *  @param i4Config to set IRRX_CONFIG_HIGH_REG
 *  @param i4SaPeriod to set IRRX_CONFIG_LOW_REG
 *  @param i4Threshold to set IRRX_THRESHOLD_REG
 *  @retval 0 is operating successfully, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 IRHW_RxInit(INT32 i4Config, INT32 i4SaPeriod, INT32 i4Threshold)
{
#ifndef CC_UBOOT
    UINT32 u4State;
#endif /* CC_UBOOT */
    INT32 i, i4Ret;

    IRHW_RxWrConf(i4Config, i4SaPeriod, i4Threshold);
    i4Ret = _IRHW_RxSetIsr((BOOL)TRUE);

//    Printf("Before IRRX State: %d\n", IRHW_RxHwState());

#ifndef CC_UBOOT
    u4State = HalCriticalStart();
#endif /* CC_UBOOT */

    // wait 1ms for Busy Clr operation finish.
    for (i=0; i<30000; i++) { }

    _IrRxHwBusyClr();

    // wait 1ms for Busy Clr operation finish.
    for (i=0; i<30000; i++) { }

#ifndef CC_UBOOT
    HalCriticalEnd(u4State);
#endif /* CC_UBOOT */

//    Printf("After IRRX State: %d\n", IRHW_RxHwState());

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** IRHW_Stop() to stop IRRX ISR
 *  @retval 0 is operating successfully, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 IRHW_RxStop()
{
    INT32 i4Ret;
    i4Ret = _IRHW_RxSetIsr((BOOL)FALSE);
    return i4Ret;
}

#ifndef IRRX_NON_OSAI
//-----------------------------------------------------------------------------
/** IRHW_RxSetCallback() to register interrupt callback routine.
 *  @param pfnCallback the new register callback routine
 *  @param ppfnOld pointer to the old callback routine
 *  @retval 0 is operating successfully, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 IRHW_RxSetCallback(PFN_IRRXCB_T pfnCallback, PFN_IRRXCB_T *ppfnOld)
{
    if (ppfnOld != NULL)
    {
        *ppfnOld = _pfnIrRxCbFunc;
    }
    _pfnIrRxCbFunc = pfnCallback;
    return IR_SUCC;
}


#ifdef CC_CLI
/* HWIRTX export functions */

//-----------------------------------------------------------------------------
/** IRHW_TxRdConf() to read IRTX configuration
 *  @param pi4Config
 *  @param prWave
 *  @param pi4Modulation
 */
//-----------------------------------------------------------------------------
void IRHW_TxRdConf(INT32 *pi4Config, IRTXWAVE_T *prWave, INT32 *pi4Modulation)
{
    if ((pi4Config == NULL) || (prWave==NULL) || (pi4Modulation==NULL))
    {
        return;
    }
    *pi4Config = (INT32) (IRTX_READ32(IRTX_CONFIG_REG) & IRTX_INIT_CONF_MASK);
    prWave->u4SyncHigh = IRTX_READ32(IRTX_SYNC_HIGH_REG);
    prWave->u4SyncLow = IRTX_READ32(IRTX_SYNC_LOW_REG);
    prWave->u4Data0High = IRTX_READ32(IRTX_DATA0_HIGH_REG);
    prWave->u4Data0Low = IRTX_READ32(IRTX_DATA0_LOW_REG);
    prWave->u4Data1High = IRTX_READ32(IRTX_DATA1_HIGH_REG);
    prWave->u4Data1Low = IRTX_READ32(IRTX_DATA1_LOW_REG);
    *pi4Modulation = (INT32) IRTX_READ32(IRTX_MODULATION_REG);
}

//-----------------------------------------------------------------------------
/** IRHW_TxRdConf() to read IRTX configuration
 *  @param i4Config
 *  @param prWave
 *  @param i4Modulation
 */
//-----------------------------------------------------------------------------
void IRHW_TxWrConf(INT32 i4Config, const IRTXWAVE_T *prWave, INT32 i4Modulation)
{
    UINT32 u4Val;

    u4Val = IRTX_READ32(IRTX_CONFIG_REG);
    u4Val &= IRTX_CONF_BITNUM_MASK;
    IRTX_WRITE32(IRTX_CONFIG_REG,
                (((UINT32)i4Config & IRTX_INIT_CONF_MASK) | u4Val));

    ASSERT(prWave!=NULL);
    IRTX_WRITE32(IRTX_SYNC_HIGH_REG, prWave->u4SyncHigh);
    IRTX_WRITE32(IRTX_SYNC_LOW_REG, prWave->u4SyncLow);
    IRTX_WRITE32(IRTX_DATA0_HIGH_REG, prWave->u4Data0High);
    IRTX_WRITE32(IRTX_DATA0_LOW_REG, prWave->u4Data0Low);
    IRTX_WRITE32(IRTX_DATA1_HIGH_REG, prWave->u4Data1High);
    IRTX_WRITE32(IRTX_DATA1_LOW_REG, prWave->u4Data1Low);
//    IRTX_WRITE32(IRTX_SYNC_HIGH_REG, 0x3b4c0);
//    IRTX_WRITE32(IRTX_SYNC_LOW_REG, 0x1da60);
//    IRTX_WRITE32(IRTX_DATA0_HIGH_REG, 0x3b10);
//    IRTX_WRITE32(IRTX_DATA0_LOW_REG, 0x3b10);
//    IRTX_WRITE32(IRTX_DATA1_HIGH_REG, 0x3b10);
//    IRTX_WRITE32(IRTX_DATA1_LOW_REG, 0xb23e);

    IRTX_WRITE32(IRTX_MODULATION_REG, (UINT32)i4Modulation);
//    VERIFY(_IRHW_TxSetIsr(TRUE)==IR_SUCC);
}

//-----------------------------------------------------------------------------
/** IRHW_TxSendData() to read IRTX configuration
 *  @param *pi4DataArray the transferred data array
 *  @param i4BitNum the bitnumber of data array to transferred 
 */
//-----------------------------------------------------------------------------
void IRHW_TxSendData(const INT32 *pi4DataArray, INT32 i4BitNum)
{
    UINT32 i, u4Val;

    if ((pi4DataArray == NULL) || (i4BitNum == 0))
    {
        return;
    }

    /* must check trigger bit is 0, and then we can send the data. */
    do
    {
        u4Val = IRTX_READ32(IRTX_CONFIG_REG);
    } while (u4Val & IRTX_CONF_START_SEND);

    /* Modify bit number first, in RC5/RC6, with bitnum 0,
     * it will send data automatically. */
    u4Val = IRTX_READ32(IRTX_CONFIG_REG);
    u4Val &= ~IRTX_CONF_BITNUM_MASK;
    u4Val |= ((UINT32)(i4BitNum & 0x7f) << 8);
    IRTX_WRITE32(IRTX_CONFIG_REG, u4Val);

    /* ((i4BitNum+31) >> 5) means how many i4ords to send. */
    for (i=0; i < ((UINT32)(i4BitNum+31) >> 5); i++)
    {
        IRTX_WRITE32((IRTX_DATA_BYTE_3_0+(i*4)), ((UINT32)pi4DataArray[i]));
    }

    /* Then trigger the data send, don't do bit number
     * modification and triggerring at the same time. */
    u4Val |= IRTX_CONF_START_SEND;
    IRTX_WRITE32(IRTX_CONFIG_REG, u4Val);
}
#endif /* CC_CLI */
#endif /* IRRX_NON_OSAI */

