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
 * $RCSfile: pdwnc_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pdwnc_if.c
 *  pdwnc_if.c The PDWNC (Power DoWN Controller) interface
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "pdwnc_debug.h"
#include "pdwnc_entry.h"
#include "pdwnc_if.h"
#include "ir_if.h"
#include "rtc_if.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_hal_926.h"
#include "x_gpio.h"
#include "drvcust_if.h"
#include "sif_if.h"
#include "eeprom_if.h"
#include "pdwnc_t8032.h"

#ifndef CC_5391_LOADER
// Rocky: 2008/03/17
//#include "aud_if.h"
#endif /* CC_5391_LOADER */

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MT8297_RETRY 5

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// TODO: Remove after CKGEN check in
#if 1
#define GPIO_Output(n, v) (0)
#define GPIO_Enable(n, v) (0)  
#endif

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
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_5391_LOADER
// PDWNC module sub-interrupt routine list.
static volatile x_os_isr_fct _varIntList[MAX_PINT_ID+1];
static PFN_GPIO_CALLBACK _afnPDWNCGpioCallback[TOTAL_PDWNC_GPIO_NUM];
//static PFN_GPIO_CALLBACK _afnPDWNCServoGpioCallback[TOTAL_SERVOADC_NUM];
#endif /* CC_5391_LOADER */

static INT32 _gi4PDWNCInit = 0;
static UINT32 _fgPowerDownByAcCut = 0;

// static PDWNC_FuncTbl *pgrPDWNC_Func = NULL;
#define pgrPDWNC_Func   (PDWNC_EXT_GetFunc())

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------

INT32 PDWNC_ServoGpioInput(INT32 i4Gpio);

INT32 PDWNC_WriteErrorCode(UINT8 u1ErrorCode);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _ReadMT8297(UINT8 u1Addr, UINT32* pu4Buf)
{
    INT32 u4Retry = 0;
    while(SIF_Read(MT8297_SIF_CLK, MT8297_SIF_ADDR, u1Addr, (UINT8*)pu4Buf, 1) != 1)
    {
        if(++ u4Retry > MT8297_RETRY)
        {
            printf("MT8297 read fail at 0x%8x!\n", u1Addr);        
            return -1;
        }
    }
    return 0;
}

static INT32 _WriteMT8297(UINT8 u1Addr, UINT32* pu4Buf)
{
    INT32 u4Retry = 0;
    while(SIF_Write(MT8297_SIF_CLK, MT8297_SIF_ADDR, u1Addr, (UINT8*)pu4Buf, 1) != 1)
    {
        if(++ u4Retry > MT8297_RETRY)
        {
            printf("MT8297 write fail at 0x%8x!\n", u1Addr);        
            return -1;
        }
    }    
    return 0;
}


#ifndef CC_5391_LOADER
//-----------------------------------------------------------------------------
/** _PdwncHwIsr() is the interrupt routine of PDWNC module.
 *  @param u2VctId is not used, it should be VECTOR_PDWNC.
 */
//-----------------------------------------------------------------------------
static void _PdwncHwIsr(UINT16 u2VctId)
{
    UINT32 i, u4Val;

    UNUSED(u2VctId);
    
    u4Val = PDWNC_READ32(REG_RO_PDWNC_INTST);

    // Check interrupt from T8032 first
    if(!u4Val)
    {
        if(PDWNC_READ32(REG_RW_INT) & AUXINT_ST)    
        {
            if(_varIntList[PINTNO_T8032])
            {
                _varIntList[PINTNO_T8032](i);
            }
            else
            {
                LOG(1, "PDWNC(0x%08x) interrupt from T8032 no routine hook\n", u4Val);            
            }
            u4Val = PDWNC_READ32(REG_RW_INT);            
            u4Val |= AUXINT_CLR;
            PDWNC_WRITE32(REG_RW_INT, u4Val);            
        }
    }
    else
    {
        for (i = 0; i < MAX_PINT_ID; i++)
        {
            if (u4Val & _PINT(i))
            {
                if (_varIntList[i])
                {
                    _varIntList[i](i);
                    PDWNC_WRITE32(REG_WO_PDWNC_INTCLR, _PINT(i));                
                }
                else
                {
                    LOG(1, "PDWNC(0x%08x) interrupt %d no routine hook\n",
                        u4Val, i);
                    // Still need to clear this non-hook interrupt.
                    PDWNC_WRITE32(REG_WO_PDWNC_INTCLR, _PINT(i));
                }
            }
        }
    }
    /* Clear interrupt remove because sub-interrupt routine should handle 
     * their clear and if all clear on PDWNC, PDWNC of BIMCLR is clear, too.
     * If any interrupt happens at interrupt period, this routine will be
     * called because IRQCLR is not clear here.
     */
    // VERIFY(BIM_ClearIrq(VECTOR_PDWNC));
}

//-----------------------------------------------------------------------------
/** _PdwncHwSetIsr() is the interrupt enable/disable routine of PDWNC module.
 *  @param fgSet TRUE to enable interrupt, otherwise disable.
 */
//-----------------------------------------------------------------------------
static INT32 _PdwncHwSetIsr(BOOL fgSet)
{
    static BOOL fgCurSet = FALSE;
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);

    if (fgSet && (fgSet!=fgCurSet))
    {   /* enable irq. */
        fgCurSet = fgSet;
        i4Val = x_reg_isr(VECTOR_PDWNC, _PdwncHwIsr, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }
        VERIFY(BIM_ClearIrq(VECTOR_PDWNC));
    }
    else if (!fgSet && (fgSet!=fgCurSet))
    {   /* disable irq. */
        fgCurSet = fgSet;
        i4Val = x_reg_isr(VECTOR_PDWNC, NULL, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _PdwncGpioIsr() is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
static void _PdwncGpioIsr(UINT16 u2Gpio)
{
    BOOL fgStatus;

    if ((u2Gpio < TOTAL_PDWNC_GPIO_NUM) &&
        (_afnPDWNCGpioCallback[u2Gpio]!=NULL))
    {
        fgStatus = PDWNC_GpioInput((INT32)u2Gpio);
        _afnPDWNCGpioCallback[u2Gpio]((((INT32)u2Gpio)+PWR_GPIO0), fgStatus);
    }

    // Still need to clear this non-hook interrupt.
    PDWNC_WRITE32(REG_WO_PDWNC_INTCLR, _PINT(u2Gpio));
}


#endif /* CC_5391_LOADER */

//-----------------------------------------------------------------------------
/** _PdwncSetupGPIO() to setup PDWNC module GPIO wakeup settings
 *  @param pu4PowerDownEn is the power down wakeup enable register value.
 */
//-----------------------------------------------------------------------------
static void _PdwncSetupGPIO(UINT32 *pu4PowerDownEn)
{
    INT32 i, i4Val;
    UINT32 u4Val, u4Gpio, u4DkBit;
    SGL_GPIO_T   *prSingleGpioKeyList;
    GREED_KEY_T  *prGreedGpioKeyList;

    ASSERT(pu4PowerDownEn != NULL);

    u4DkBit = DRVCUST_InitGet(eKeyPadWakeupBit);
    i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
    if (i < 0)
    {
        return;
    }
    if (!((*pu4PowerDownEn) & WAK_IRRX))
    {
        u4DkBit = DKBIT_POWER;
    }

    // Do the GPIO wakeup initialization
    i = 0;
    while ((prSingleGpioKeyList) &&
            (prSingleGpioKeyList[i].u4Key != 0) &&
            (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prSingleGpioKeyList[i].u4Key & u4DkBit)
        {
            u4Gpio = prSingleGpioKeyList[i].u4Gpio - PWR_GPIO0;
            if (u4Gpio > (PWR_GPIO7 - PWR_GPIO0))
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            u4Val = PDWNC_READ32(REG_RW_PWDIOCFG);
            if (prSingleGpioKeyList[i].u4Val)
            {
                u4Val |= (GPIOX_POL(u4Gpio));
            }
            else
            {
                u4Val &= ~(GPIOX_POL(u4Gpio));
            }
            PDWNC_WRITE32(REG_RW_PWDIOCFG, u4Val);
            *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
        }
        i++;
    }

    i = DRVCUST_InitQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
    if (i < 0)
    {
        return;
    }

    // Do the GPIO wakeup initialization
    i = 0;
    while ((prGreedGpioKeyList) &&
            (prGreedGpioKeyList[i].u4Key != 0) &&
            (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prGreedGpioKeyList[i].u4Key & u4DkBit)
        {
            u4Gpio = prGreedGpioKeyList[i].u4GpioOut;
            i4Val = (INT32)prGreedGpioKeyList[i].u4Val;
            UNUSED(GPIO_Output((INT32)u4Gpio, &i4Val));
            u4Gpio = prGreedGpioKeyList[i].u4GpioIn - PWR_GPIO0;
            if (u4Gpio > (PWR_GPIO7 - PWR_GPIO0))
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            u4Val = PDWNC_READ32(REG_RW_PWDIOCFG);
            if (prGreedGpioKeyList[i].u4Val)
            {
                u4Val |= (GPIOX_POL(u4Gpio));
            }
            else
            {
                u4Val &= ~(GPIOX_POL(u4Gpio));
            }
            PDWNC_WRITE32(REG_RW_PWDIOCFG, u4Val);
            *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
        }
        i++;
    }

    // Do the special GPIO wakeup initialization
    i = DRVCUST_InitQuery(eSpecialWakeupGpio, (UINT32 *)&u4Gpio);
    if (i < 0)
    {
        return;
    }
    i = DRVCUST_InitQuery(eSpecialWakeupOnVal, (UINT32 *)&u4Val);
    if (i < 0)
    {
        return;
    }
    // Set to input mode.
    i4Val = 0;
    UNUSED(GPIO_Enable((INT32)u4Gpio, &i4Val));
    // Set the wakeup polarity.
    i4Val = (INT32)u4Val;
    u4Gpio = u4Gpio - PWR_GPIO0;
    u4Val = PDWNC_READ32(REG_RW_PWDIOCFG);
    if (i4Val)
    {
        u4Val |= (GPIOX_POL(u4Gpio));
    }
    else
    {
        u4Val &= ~(GPIOX_POL(u4Gpio));
    }
    PDWNC_WRITE32(REG_RW_PWDIOCFG, u4Val);
    *pu4PowerDownEn |= GPIO_WAKEN(WAK_GPIOX(u4Gpio));
}

static void _PdwncSetupIRRX(UINT32 u4PowerDownEn)
{
    INT32 i4Val;
    UINT32 u4Val;

    if ((u4PowerDownEn & WAK_IRRX) == 0)
    {
        return;
    }

    i4Val = DRVCUST_InitQuery(eIrrxType, &u4Val);
    if (i4Val < 0)
    {
        u4Val = IRRX_RC5_TYPE;
    }

    // To write u4PowerDownEn for Wakeup key setting.
    PDWNC_WRITE32(REG_RW_PDWAKE_EN, u4PowerDownEn);

    // Configuration
    i4Val = IRRX_InitMtkIrRegSetup(u4Val);
    UNUSED(i4Val);
}

static void _PdwncSetupVGA(UINT32 u4PowerDownEn)
{
    UINT32 u4Val;
    IC_VERSION_T eVer;

    if ((u4PowerDownEn & WAK_VGA) == 0)
    {
#ifndef CC_NO_ACD_STANDBY_SETTING
        Printf("Disable VGA wakeup\n");
        // Power off slicer module to reduce current
        u4Val = PDWNC_READ32(REG_RW_SYNCFG0);
        u4Val |= (7U << 29);
        PDWNC_WRITE32(REG_RW_SYNCFG0, u4Val);
        u4Val = PDWNC_READ32(REG_RW_SYNCFG2);
        u4Val |= (0x1fU << 0);
        PDWNC_WRITE32(REG_RW_SYNCFG2, u4Val);
#endif
        return;
    }
  
    // VGA registers setup
    u4Val = PDWNC_READ32(REG_RW_SYNCFG0);    

    eVer = BSP_GetIcVersion();    
    u4Val &= ~(SYNC_BIAS_PASV_MASK);    
    if (eVer < IC_VER_AC)
    {    
        u4Val |= (BYPS_SYNCPROSR | SYNC_BIAS_PASV_SEL(4));
    }
    else
    {
        u4Val |= SYNC_BIAS_PASV_SEL(4);
    }
    PDWNC_WRITE32(REG_RW_SYNCFG0, u4Val);
    LOG(7, "SYNCFG0: 0x%8x\n", u4Val);
    
    u4Val = PDWNC_READ32(REG_RW_SYNCFG1);
    
    if ((GET_SYNC_BIAS_PASV_SEL(u4Val)==3) &&
        (GET_SYNC_VLSEL_VAL(u4Val)==4) &&
        (GET_SYNC_VHSEL_VAL(u4Val)==4))
    {
        u4Val &= ~(SYNC_VLSEL_MASK | SYNC_VHSEL_MASK);
        u4Val |= (SYNC_VLSEL_VAL(7) | SYNC_VHSEL_VAL(2));
        PDWNC_WRITE32(REG_RW_SYNCFG1, u4Val);
    }
    LOG(7, "SYNCFG1: 0x%8x\n", u4Val);

    u4Val = PDWNC_READ32(REG_RW_SYNCFG2);    
    LOG(7, "SYNCFG2: 0x%8x\n", u4Val);
    
    u4Val = PDWNC_READ32(REG_RW_SYNCFG3);        
    LOG(7, "SYNCFG3: 0x%8x\n", u4Val);

    // To compatible with SOC/SOG.(480i/576i)
    u4Val = PDWNC_READ32(REG_RW_VGA_WAKEUP_CTRL0);
    u4Val &= ~(HRANGE_U_MASK);
    //u4Val |= SET_HRANGE_U(0x700) | AUTO_SWITCH_EN | C_DGLITCH_EN | V_DGLITCH_EN | H_DGLITCH_EN;
    u4Val |= SET_HRANGE_U(0xfff) | AUTO_SWITCH_EN | C_DGLITCH_EN | V_DGLITCH_EN | H_DGLITCH_EN;    
    u4Val |= SET_HRANGE_L(0x0);
    
    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL0, u4Val);

    u4Val = PDWNC_READ32(REG_RW_VGA_WAKEUP_CTRL1);

    //u4Val &= ~(VRANGE_L_MASK | PDN_USE_HLEN_TH);
    u4Val &= ~(VRANGE_L_MASK);
    
    u4Val |= SET_VRANGE_L(0x0);
    u4Val |= SET_VRANGE_U(0xfff);    
    
    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL1, u4Val);

         
    u4Val = PDWNC_READ32(REG_RW_VGA_WAKEUP_CTRL2);
    u4Val &= ~(CRANGE_L_MASK);
    
    u4Val |= SET_CRANGE_L(0x0);
    u4Val |= SET_CRANGE_U(0xfff);    
    
#ifdef CC_DISABLE_CSYNC_WAKEUP
    u4Val &= 0xf3ffffff;
#endif
    
    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL2, u4Val);

    u4Val = PDWNC_READ32(REG_RW_VGA_WAKEUP_CTRL3);
    
    u4Val &= 0xff000000;
    
    //u4Val |= 0x2004;
    u4Val |= (SET_H_DIFF_TH(0x3f) | SET_V_STABLE_TH(0x2) | SET_H_STABLE_TH(0x80));

    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL3, u4Val);

    u4Val = PDWNC_READ32(REG_RW_VGA_WAKEUP_CTRL4);
    u4Val |= SET_V_DIFF_TH(0x3f);
    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL4, u4Val);

    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL6, 0x0020800A);

    u4Val = PDWNC_READ32(REG_RW_VGA_WAKEUP_CTRL7);
    u4Val |= 0x001d442a;
    PDWNC_WRITE32(REG_RW_VGA_WAKEUP_CTRL7, u4Val);
      
}

static void _PdwncSetupServoADC(UINT32 *pu4PowerDownEn)
{
    INT32 i;
    UINT32 u4Val, u4Max, u4Min, u4DkBit;
    SGL_SERVO_T   *prSingleServoKeyList;
    
    ASSERT(pu4PowerDownEn != NULL);
#ifndef CC_NO_ACD_STANDBY_SETTING
    // if ServoADC is enabled, skip this setting, because this is set.
    if (*pu4PowerDownEn & WAK_ALLSRVAD)
    {
        Printf("Power down Enable:0x%08x\n", *pu4PowerDownEn);
        return;
    }
#endif

    u4DkBit = DRVCUST_InitGet(eKeyPadWakeupBit);
    i = DRVCUST_InitQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
    if (i < 0)
    {
        goto ServoExit;
    }

    ASSERT(pu4PowerDownEn != NULL);
    if (!((*pu4PowerDownEn) & WAK_IRRX))
    {
        u4DkBit = DKBIT_POWER;
    }

    // disable ServoADC working.
    _ReadMT8297(REGSIF_RW_SRVCFG1, &u4Val);
    u4Val &= ~(SRVCH_EN_SRVCFG);
    _WriteMT8297(REGSIF_RW_SRVCFG1, &u4Val);
    // Clear ServoADC value.
    u4Val = ADCOUT_CLR;
    _WriteMT8297(REGSIF_RW_SRVCLR, &u4Val);    

    // Clear ServoADC register
    for (i=0; i<MAX_SERVOADC_NUM; i++)
    {
        u4Val = 0x0;
        _WriteMT8297(REGSIF_RW_ADCTOP0 + i, &u4Val);                
        _WriteMT8297(REGSIF_RW_ADCBOT0 + i, &u4Val);            
        _WriteMT8297(REGSIF_RW_WAKCFG0 + i, &u4Val);                            
    }

    // Do the Servo wakeup initialization
    i = 0;
    while ((prSingleServoKeyList) &&
            (prSingleServoKeyList[i].u4Key != 0) &&
            (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
    {
        if (prSingleServoKeyList[i].u4Key & u4DkBit)
        {
            if ((prSingleServoKeyList[i].u4Top > 255) ||
                (prSingleServoKeyList[i].u4Servo > SERVO(6)) ||
                (prSingleServoKeyList[i].u4Btm > 255))
            {
                LOG(1, "Error Power button setting\n");
                i++;
                continue;
            }
            // In case of old style servo ADC numbering
            if(prSingleServoKeyList[i].u4Servo >= ADIN0)
            {
               prSingleServoKeyList[i].u4Servo -= ADIN0; 
            }
            
            // Make sure enable PAD
            u4Val = PDWNC_ReadServoADCChannelValue(prSingleServoKeyList[i].u4Servo);
            // Get the range
            _ReadMT8297(REGSIF_RW_ADCTOP0 + prSingleServoKeyList[i].u4Servo, &u4Max);
            _ReadMT8297(REGSIF_RW_ADCBOT0 + prSingleServoKeyList[i].u4Servo, &u4Min);            
            
            // Set Servo ADC value
            u4Val = CHTOP_VAL(((u4Max < prSingleServoKeyList[i].u4Top) ?
                                    (prSingleServoKeyList[i].u4Top) : u4Max)) |
                    CHBTM_VAL(((u4Min > prSingleServoKeyList[i].u4Btm) ?
                                    (prSingleServoKeyList[i].u4Btm) : u4Min)) |
                                (CH_WAK_MID_EN & CH_MASK);
            u4Val = CHTOP_VAL(((u4Max < prSingleServoKeyList[i].u4Top) ? (prSingleServoKeyList[i].u4Top) : u4Max));
            _WriteMT8297(REGSIF_RW_ADCTOP0 + prSingleServoKeyList[i].u4Servo, &u4Val);            
            u4Val = CHBTM_VAL(((u4Min > prSingleServoKeyList[i].u4Btm) ? (prSingleServoKeyList[i].u4Btm) : u4Min));            
            _WriteMT8297(REGSIF_RW_ADCBOT0 + prSingleServoKeyList[i].u4Servo, &u4Val);                        
            u4Val = CH_WAK_MID_EN & CH_MASK;
            _WriteMT8297(REGSIF_RW_WAKCFG0 + prSingleServoKeyList[i].u4Servo, &u4Val);                        
            
            *pu4PowerDownEn = (SRVAD_WAKEN(WAK_SRVADX(prSingleServoKeyList[i].u4Servo)) << 24);
        }
        i++;
    }

ServoExit:
    if (!(*pu4PowerDownEn & (WAK_ALLSRVAD << 24)))
    {
        Printf("Disable ServoADC\n");
        // No longer need to turn off ServoADC first.
        UNUSED(u4Val);
    }
    else
    {
        // Enable servo ADC wakeup
        u4Val = *pu4PowerDownEn >> 24;        
        _WriteMT8297(REGSIF_RW_SVOWAKEN, &u4Val);
    }
}

#ifndef CC_5391_LOADER
static void _PdwncSetupRTC(const UINT32* pu4PowerDownEn)
{
    UINT32 u4RtcHw;

    ASSERT(pu4PowerDownEn != NULL);
    if ((*pu4PowerDownEn & WAK_RTC) == 0)
    {
        return;
    }
    
    if(DRVCUST_OptQuery(eRtcHw, &u4RtcHw) != 0)
    {
        return;
    }
    if(u4RtcHw == (INT32)RTC_INTERNAL)
    {    
    }
    else
    {
        RTC_SetWakeup((UINT32 *)pu4PowerDownEn);
    }

}
#endif /* CC_5391_LOADER */

static void _PdwncSetupHDMI(UINT32 u4PowerDownEn)
{
    UINT32 u4Val;
    if ((u4PowerDownEn & WAK_HDMI) == 0)
    {
        PDWNC_WRITE32(REG_RW_HDMICFG0, 0);         // HDMI power down
        PDWNC_WRITE32(REG_RW_PSAVCFG0, 0);         // Disable HDMI
        return;
    }
#ifdef CC_NO_HDMI
        PDWNC_WRITE32(REG_RW_HDMICFG0, 0);         // HDMI power down
        PDWNC_WRITE32(REG_RW_PSAVCFG0, 0);         // Disable HDMI
#else
    u4Val = PDWNC_READ32(REG_RW_HDMICFG0);
    u4Val &= ~(3U << 10);
    u4Val |= (1U << 10);
    PDWNC_WRITE32(REG_RW_HDMICFG0, u4Val);     // HDMI current reduction
#endif
}

static const UINT8 _au1PinmuxFunctionMasks[TOTAL_PDWNC_GPIO_NUM] =
{
    3, 1, 1, 1, 1, 1, 3, 1, 1
};

static const UINT32 _au1PinmuxFunctionBits[TOTAL_PDWNC_GPIO_NUM] =
{
    0, 2, 3, 4, 5, 6, 7, 9, 14
};

static void _PdwncSetPinmux(INT32 i4Gpio, UINT32 u4Func)
{
    UINT32 u4Mask, u4Val, u4Bit;

    u4Bit = _au1PinmuxFunctionBits[i4Gpio];
    u4Mask = (_au1PinmuxFunctionMasks[i4Gpio] << u4Bit);
    u4Val = PDWNC_READ32(0x00B0);
    u4Val &= ~(u4Mask);
    u4Val |= (u4Func << u4Bit);
    PDWNC_WRITE32(0x00B0, u4Val);
}

static void _PdwncSetGpioPin(INT32 i4Gpio)
{
    switch(i4Gpio) {
    case 0: case 1: case 2: case 3: case 4: case 5:
        _PdwncSetPinmux(i4Gpio, 0);
        break;
    case 6: case 7: case 8:
        _PdwncSetPinmux(i4Gpio, 1);
        break;
    default:
        break;
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/* PDWNC_WriteErrorCode() to write the current error code to EEPROM & uP
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_WriteErrorCode(UINT8 u1ErrorCode)
{
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnWriteErrorCode);
    pgrPDWNC_Func->pfnWriteErrorCode(u1ErrorCode);
    return 0;
}

//-----------------------------------------------------------------------------
/* PDWNC_SetQueryRtcYear() to query RTC year register value
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT8 PDWNC_SetQueryRtcYear(const UINT8 *pu1Year)
{
    if (pu1Year)
    {
        BIM_WRITE32(REG_RW_RTC_YEAR, (UINT32)(*pu1Year));
    }
    return (BIM_READ32(REG_RW_RTC_YEAR));
}

//-----------------------------------------------------------------------------
/* PDWNC_ReadWakeupReason() to read the wakeup reason 
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_ReadWakeupReason(void)
{
    UINT32 u4Reason;

    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnReadWakeupReason);
    pgrPDWNC_Func->pfnReadWakeupReason(&u4Reason);
    return (u4Reason);
}


UINT32 PDWNC_PowerDownByAcCut(void)
{
    UNUSED(PDWNC_Init());
    return _fgPowerDownByAcCut;
}

//-----------------------------------------------------------------------------
/* PDWNC_Init() to initialize PDWNC driver
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_Init(void)
{
    UINT32 u4Val;
#ifndef CC_5391_LOADER
    UINT32 i;
    INT32 i4Val;
    void (* pfnOldIsr)(UINT16);
#endif /* CC_5391_LOADER */
    PFN_GPIO_POWER_FUNC pfnPowerOnFunc = NULL;

    if (_gi4PDWNCInit)
    {
        return 0;
    }

    _gi4PDWNCInit = 1;

#ifndef CC_5391_LOADER
    // Clear callback function array.
    for (i=0; i<TOTAL_PDWNC_GPIO_NUM;i++)
    {
        _afnPDWNCGpioCallback[i] = NULL;
    }
    for (i=0; i<TOTAL_SERVOADC_NUM;i++)
    {
        _afnPDWNCGpioCallback[i] = NULL;
    }

    _PDWNC_InitT8032();

    // Initialize interupt register.
    PDWNC_WRITE32(REG_RW_PDWNC_INTEN, 0);
    PDWNC_WRITE32(REG_RW_EXTINTCFG, 0);
    PDWNC_WRITE32(REG_WO_PDWNC_INTCLR, 0x3fff);

    // Hook PDWNC Gpio interrupt routine.
    for (i=0; i<TOTAL_PDWNC_GPIO_NUM; i++)
    {
        i4Val = PDWNC_RegIsr((UINT16)(PINTNO_GPIO0+i), _PdwncGpioIsr, &pfnOldIsr);
        if (i4Val != PDWNC_OK)
        {
            return -1;
        }
    }

    // Hook T8032 ISR
    PDWNC_RegIsr((UINT16)PINTNO_T8032, _PDWNC_T8032Isr, &pfnOldIsr);
    
#endif /* CC_5391_LOADER */

    if (DRVCUST_InitQuery(eGpioPowerOnFunc, (UINT32 *)(void *)&pfnPowerOnFunc) == 0)
    {
        pfnPowerOnFunc();
    }

    _fgPowerDownByAcCut = (PDWNC_SetQueryRtcYear(NULL)==0xff);

    // Enable servo ADC H/W mode
    u4Val &= ~SWEN_SRVCFG;
    u4Val |= HWEN_SRVCFG;
    _WriteMT8297(REGSIF_RW_SRVCFG0, &u4Val);

    // Set reset value to some registers.
    PDWNC_WRITE32(0xe0, 0);             // 60MHz XTAL current setting.
    PDWNC_WRITE32(0xe4, 0);             // 60MHz XTAL current setting.
    PDWNC_WRITE32(0xb4, 0x1907);        // HDMI power down
    PDWNC_WRITE32(0xc0, 0xe8884c88);    // SYNC slicer current reduction

    // Set servo ADC sampling rate
    /*
    _WriteMT8297(REGSIF_RW_SRV_RAT0, &u4Val);   
    _WriteMT8297(REGSIF_RW_SRV_RAT1, &u4Val);   
    */
    
#ifndef CC_5391_LOADER

    // Do the external power down function
    VERIFY(pgrPDWNC_Func->pfnInit);
    pgrPDWNC_Func->pfnInit();
    
#endif /* CC_5391_LOADER */

    return 0;
}

#ifndef CC_5391_LOADER
//-----------------------------------------------------------------------------
/** PDWNC_RegIsr() is to hook the sub-interrupt routine of PDWNC module.
 *  @param u2IntNo the interrupt number at PDWNC module.
 *  @param pfnIsr the hooked function to register, NULL to disable.
 *  @param ppfnOld the old hooked function.
 *  @retval PDWNC_OK: Succeed
 *  @retval PDWNC_FAIL: Fail
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld)
{
    UINT32 u4Val, u4State;

    VERIFY(PDWNC_Init()==0);

    if (u2IntNo > MAX_PINT_ID)
    {
        return PDWNC_FAIL;
    }

    if (ppfnOld != NULL)
    {
        *ppfnOld = _varIntList[u2IntNo];
    }
   
    if (pfnIsr == NULL)
    {
        if (u2IntNo == PINTNO_T8032)
        {
            u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
            if (u4Val==0)
            {
                VERIFY(_PdwncHwSetIsr(FALSE));
            }       
             _varIntList[u2IntNo] = pfnIsr;
        }
        else
        {
            u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
            u4Val &= ~(_PINT(u2IntNo));
            if (u4Val==0)
            {
                VERIFY(_PdwncHwSetIsr(FALSE));
            }
            u4State = HalCriticalStart();
            PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);
            HalCriticalEnd(u4State);
            _varIntList[u2IntNo] = pfnIsr;
        }
    }
    else
    {
        if (u2IntNo == PINTNO_T8032)
        {
            _varIntList[u2IntNo] = pfnIsr;
            u4State = HalCriticalStart();
            VERIFY(_PdwncHwSetIsr(TRUE));
            HalCriticalEnd(u4State);
        }
        else
        {
            _varIntList[u2IntNo] = pfnIsr;
            u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
            // Do not enable GPIO Intr, let GpioIntr function do it.
            u4Val |= (u2IntNo <= PINTNO_GPIO7) ? 0 : (_PINT(u2IntNo));
            u4State = HalCriticalStart();
            VERIFY(_PdwncHwSetIsr(TRUE));
            PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);
            HalCriticalEnd(u4State);        
        }
    }
    return PDWNC_OK;
}

//-----------------------------------------------------------------------------
/** PDWNC_ClearInt() to clear the interrupt flag in PDWNC status
 *  PDWNC_ClearInt() do bit clear on REG_RW_PDWNC_INTCLR register and the \n
 *  value of REG_RO_PDWNC_INTST will change, too.  If the REG_RO_PDWNC_INTST
 *  is 0, the interrupt status bit of PDWNC on BIM IRQST will be cleared, too.
 *  @param u2IntNo the sub-interrupt number of PDWNC
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL PDWNC_ClearInt(UINT16 u2IntNo)
{
    UINT32 u4Int, u4State;

    if (u2IntNo > MAX_PINT_ID)
    {
        return FALSE;
    }

    u4Int = _PINT(u2IntNo);
    u4State = HalCriticalStart();
    PDWNC_WRITE32(REG_WO_PDWNC_INTCLR, u4Int);
    HalCriticalEnd(u4State);
    return TRUE;
}
#endif /* CC_5391_LOADER */

//-----------------------------------------------------------------------------
/** PDWNC_Reboot() Reboot the system
 */
//-----------------------------------------------------------------------------
void PDWNC_Reboot()
{
    Printf("\n                                       ");
#ifndef CC_UBOOT
    HalDisableIRQ();
    HalDisableFIQ();
#endif /* CC_UBOOT */
    PDWNC_WRITE32(REG_RW_WATCHDOG_TMR, 0xffff0000);
    PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 1);
    while (1) { }
}

//-----------------------------------------------------------------------------
/** PDWNC_ReadServoADCChannelValue() Read the ServoADC Channel Value
 *  @param u4Channel the channel number
 *  @retval the channel adc value.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel)
{
#if 0 /* Software mode. */
    UINT32 i;

    PDWNC_WRITE32(REG_RW_PADCFG3, PDWNC_READ32(REG_RW_PADCFG3) & 0x7fffffff);
    PDWNC_WRITE32(SRVCFG_REG, SWEN_SRVCFG | CKEN_SRVCFG |
                                CHNNLSEL_SRVCFG(u4Channel));
    for (i=0; i<10000; i++);
    PDWNC_WRITE32(SRVSWT_REG, 1);
    while (PDWNC_READ32(SRVSWT_REG)!=0) ;
    for (i=0; i<10000; i++);
    return PDWNC_READ32(ADOUT0_REG+(u4Channel << 2));
#else
    UINT32 u4Val, u4Ret;

    // support 2 styles of servo adc numbering: 0 ~ 4 and 400 ~ 404
    if(u4Channel >= ADIN0)
    {
        u4Channel -= ADIN0;
    }
    // Maximum is 8 channel.
    VERIFY(u4Channel < TOTAL_SERVOADC_NUM);

    // Enable ServoADC interface module clock stop.
    //SIF_Read(MT8297_SIF_CLK, MT8297_SIF_ADDR, REGSIF_RW_PADCFG0);

    // Read ServoADC PAD setting.
    _ReadMT8297(REGSIF_RW_SRVCFG0, &u4Val);
    
    u4Val &= ~SWEN_SRVCFG;
    u4Val |= HWEN_SRVCFG;
    _WriteMT8297(REGSIF_RW_SRVCFG0, &u4Val);

    _ReadMT8297(REGSIF_RW_SRVCFG1, &u4Val);
    if ((u4Val & SRVCH_EN_CH(u4Channel)) == 0)
    {
        u4Val |= SRVCH_EN_CH(u4Channel);
        _WriteMT8297(REGSIF_RW_SRVCFG1, &u4Val);   
        // Wait until servo AD sampling done.   
        HAL_Delay_us(10);              
    }

    _ReadMT8297(REGSIF_RW_ADOUT0 + u4Channel, &u4Ret);
    
    return (u4Ret & 0xff);
#endif /* 0 */
}

//-----------------------------------------------------------------------------
/** PDWNC_IsWakeupByPowerButton() to check wakeup status is Power key pad.
 *  @retval 0 not wakeup by power key pad, 1 wakeup by power key pad.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_IsWakeupByPowerButton(void)
{
    INT32 i;
    UINT32 u4Val, u4Gpio, u4Servo;
    SGL_GPIO_T   *prSingleGpioKeyList;
    SGL_SERVO_T  *prSingleServoKeyList;
    GREED_KEY_T  *prGreedGpioKeyList;

    _ReadMT8297(REGSIF_RO_PDSTAT, &u4Val);
    u4Val <<= 24;
    u4Val |= PDWNC_READ32(REG_RO_PDWAKE_STAT);

    i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
    if (i == 0)
    {
        while ((prSingleGpioKeyList) &&
            (prSingleGpioKeyList[i].u4Key != 0) &&
            (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prSingleGpioKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Gpio = prSingleGpioKeyList[i].u4Gpio - PWR_GPIO0;
                if (u4Val & (1U << u4Gpio))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            i++;
        }
    }

    i = DRVCUST_InitQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
    if (i==0)
    {
        while ((prSingleServoKeyList) &&
            (prSingleServoKeyList[i].u4Key != 0) &&
            (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prSingleServoKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Servo = prSingleServoKeyList[i].u4Servo;
                // In case of old style servo ADC numbering
                if(u4Servo >= ADIN0)
                {
                    u4Servo -= ADIN0;
                }
                if (u4Val & (SRVAD_WAKEN(WAK_SRVADX(u4Servo)) << 24))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            i++;
        }
    }

    i = DRVCUST_InitQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
    if (i==0)
    {
        while ((prGreedGpioKeyList) && 
            (prGreedGpioKeyList[i].u4Key != 0) &&
            (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prGreedGpioKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Gpio = prGreedGpioKeyList[i].u4GpioIn - PWR_GPIO0;
                if (u4Val & (1U << u4Gpio))
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            i++;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void PDWNC_WatchDogReboot(void)
{
    if (PDWNC_READ32(REG_RW_WATCHDOG_EN))
    {
        PDWNC_Reboot();
    }
}

//-----------------------------------------------------------------------------
/** PDWNC_CreateWatchDogThread() to enable watch dog and set watch dog thread.
 */
//-----------------------------------------------------------------------------
void PDWNC_CreateWatchDogThread(void)
{
#ifndef CC_5391_LOADER
#if 0
    static HANDLE_T hWatchDog;

    PDWNC_WRITE32(REG_RW_WATCHDOG_EN, 0);
    if (x_thread_create(&hWatchDog, "WatchDog", 0x800, DRVCUST_OptGet(eReleaseWatchdogPriority),
            _WatchDogBody, 0, NULL) != OSR_OK)
    {
        LOG(1, "\nError: Create watchdog thread fail\n");
        return;
    }
#endif

    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnWatchDogInit);
    pgrPDWNC_Func->pfnWatchDogInit();

    VERIFY(pgrPDWNC_Func->pfnErrorHandleInit);
    pgrPDWNC_Func->pfnErrorHandleInit();
#endif /* CC_5391_LOADER */
}

//-----------------------------------------------------------------------------
/** PDWNC_IrLedCheck() to check ir blinking option
 */
//-----------------------------------------------------------------------------
void PDWNC_IrLedCheck(INT32 fgOn)
{
    static HAL_TIME_T rPrevTime = { 0, 0 };
    static INT32 fgPrev = 1;
    static INT32 fgInit = 0;
    static PFN_GPIO_LEDCTRL_FUNC pfnLedCtrlFunc = NULL;
    HAL_TIME_T rTime, rDelta;

    if (!fgInit)
    {
        UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
        fgInit = 1;
    }

    if (DRVCUST_InitGet(eIrrxFlagBlinking))
    {
        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

        if ((fgOn) && (!fgPrev) && ((rDelta.u4Seconds > 0) || (rDelta.u4Micros > 50000)))
        {
            pfnLedCtrlFunc(eLedIrOn);
            fgPrev = 1;
            HAL_GetTime(&rPrevTime);
        }
        else if ((!fgOn) && (fgPrev) && ((rDelta.u4Seconds > 0) || (rDelta.u4Micros > 100000)))
        {
            if (DRVCUST_InitGet(eSupportDarkLed))
            {
                pfnLedCtrlFunc(eLedDarkLed);
            }
            else
            {
                pfnLedCtrlFunc(eLedIrOff);
            }
            fgPrev = 0;
            HAL_GetTime(&rPrevTime);
        }
    }
}

//-----------------------------------------------------------------------------
/** PDWNC_EnterPowerDown() Enter Power Down mode.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_EnterPowerDown(UINT32 u4PowerDownEn, UINT32 u4PowerDownEvent)
{
    PFN_GPIO_POWEROFF_FUNC pfnPowerOffFunc = NULL;
    INT32 i4Gpio;

    // GPIO wakeup setup
    _PdwncSetupGPIO(&u4PowerDownEn);

    // ServoADC wakeup setup
    _PdwncSetupServoADC(&u4PowerDownEn);

    // IRRX wakeup setup
    _PdwncSetupIRRX(u4PowerDownEn);

    // VGA wakeup setup
    _PdwncSetupVGA(u4PowerDownEn);

#ifndef CC_5391_LOADER
    // RTC wakeup setup
    _PdwncSetupRTC(&u4PowerDownEn);
#endif /* CC_5391_LOADER */
    
    // HDMI wakeup setup
    _PdwncSetupHDMI(u4PowerDownEn);

    // Enable wakeup devices, except for ServoADC
    PDWNC_WRITE32(REG_RW_PDWAKE_EN, u4PowerDownEn & 0xFFFFFF);    

    // Do customization 
    if (DRVCUST_InitQuery(eGpioPowerOffFunc, (UINT32 *)(void *)&pfnPowerOffFunc) == 0)
    {
        pfnPowerOffFunc(u4PowerDownEn);
    }
    
    if(DRVCUST_InitGet(eFlagMt8292GpioSupport))
    {
        INT32 i4Val = 0;
        // If customization option does not set 8292 reset, it must be polled by H/W        
        i4Gpio = (INT32)DRVCUST_InitGet(eMt8292ResetGpio);
        if(i4Gpio != (INT32)0xFFFFFFFF)
        {
            VERIFY(GPIO_Output(i4Gpio, &i4Val) == 0);
        }
        UNUSED(i4Val);
    }
    
    if (DRVCUST_InitGet(ePdwncPolarity))
    {
        PDWNC_WRITE32(REG_RW_PDWNC_MISC, (PD_MISC_PDWNC_POLARITY | PDWNC_READ32(REG_RW_PDWNC_MISC)));
    }
    else
    {
        PDWNC_WRITE32(REG_RW_PDWNC_MISC, ((~PD_MISC_PDWNC_POLARITY) & PDWNC_READ32(REG_RW_PDWNC_MISC)));
    }

    // Setup Power Down
    Printf("Standby                         \n");
    // SerWaitTxBufClear();

    // Do the external power down for customize setting
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnPowerDown);
    pgrPDWNC_Func->pfnPowerDown(u4PowerDownEvent);
        
    PDWNC_SetupPowerDown();
    
    return 0;
}


//======================= PDWNC PWR GPIO functions ============================

//-----------------------------------------------------------------------------
/** PDWNC_GpioEnable() The GPIO input/output mode setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is input mode and 1 is output mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current mode of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as input mode, otherwise set as output mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is input mode, 1 is output
 *          mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioEnable(INT32 i4Gpio, const INT32 *pfgSet)
{
    UINT32 u4Val;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    u4Val = PDWNC_READ32(REG_RW_IOPADEN);
    if (pfgSet == NULL)
    {
        if (i4Gpio == (OPWR2_5V - PWR_GPIO0))
        {
            return ((u4Val & (0x2000)) ? 1 : 0);
        }
        else
        {
            return ((u4Val & (1U << i4Gpio)) ? 1 : 0);
        }
    }

    if (i4Gpio == (OPWR2_5V - PWR_GPIO0))
    {
        u4Val = (*pfgSet) ?
                (u4Val | (0x2000)) :
                (u4Val & ~(0x2000));
    }
    else
    {
        u4Val = (*pfgSet) ?
                (u4Val | (1U << i4Gpio)) :
                (u4Val & ~(1U << i4Gpio));
    }
    PDWNC_WRITE32(REG_RW_IOPADEN, u4Val);
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_GpioOutput() The GPIO output value setting functions. It will check
 *  the i4Gpio and set to related register bit as 0 or 1.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *          the bit of gpio number (0 or 1).  If it's not NULL, it must 
 *          reference to a integer.  If the integer is 0, this function 
 *          will set the bit of the gpio number as 0, otherwise set as 1.
 *  @retval If pfgSet is NULL, it return 0 or 1 (the bit value of the gpio
 *          number of output mode.  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioOutput(INT32 i4Gpio, const INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    u4Val = PDWNC_READ32(REG_RW_IOPADOUT);
    if (pfgSet == NULL)
    {
        if(i4Gpio == (OPWR2_5V - PWR_GPIO0))
        {
            return ((u4Val & 0x2000) ? 1 : 0);
        }
        else
        {
            return ((u4Val & (1U << i4Gpio)) ? 1 : 0);
        }
    }

    if(i4Gpio == (OPWR2_5V - PWR_GPIO0))
    {
        u4Val = (*pfgSet) ?
                (u4Val | 0x2000) :
                (u4Val & ~0x2000);
    }
    else
    {
        u4Val = (*pfgSet) ?
                (u4Val | (1U << i4Gpio)) :
                (u4Val & ~(1U << i4Gpio));
    }
    PDWNC_WRITE32(REG_RW_IOPADOUT, u4Val);

    // Set the output mode.
    i4Idx = 1;
    VERIFY(i4Idx==PDWNC_GpioEnable(i4Gpio, &i4Idx));

    _PdwncSetGpioPin(i4Gpio);
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_GpioInput()  The GPIO input reading functions. It will check the 
 *  i4Gpio and read related register bit to return.
 *  @param i4Gpio the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioInput(INT32 i4Gpio)
{
    UINT32 u4Val;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    _PdwncSetGpioPin(i4Gpio);
    u4Val = PDWNC_READ32(REG_RO_IOPADIN);
    if(i4Gpio == (OPWR2_5V - PWR_GPIO0) )
    {
        return ((u4Val & (0x2000)) ? 1U : 0);
    }
    return ((u4Val & (1U << i4Gpio)) ? 1U : 0);
}


//-----------------------------------------------------------------------------
/** PDWNC_GpioEnable() The GPIO input/output mode setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is input mode and 1 is output mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current mode of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as input mode, otherwise set as output mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is input mode, 1 is output
 *          mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ServoGpioEnable(INT32 i4Gpio, const INT32 *pfgSet)
{
    UINT32 u4Val;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_SERVOADC_NUM))
    {
        return -1;
    }

    // Servo ADC used as GPIO 
    _ReadMT8297(REGSIF_RW_ADPADG, &u4Val);
    if (pfgSet == NULL)
    {
        return ((u4Val & SRVIO_GP(i4Gpio)) ? 1 : 0);
    }
    u4Val = (*pfgSet) ?
            (u4Val | SRVIO_GP(i4Gpio)) :
            (u4Val & ~(SRVIO_GP(i4Gpio)));
    _WriteMT8297(REGSIF_RW_ADPADG, &u4Val);

    // Servo ADC used as GPO 
    _ReadMT8297(REGSIF_RW_ADPADE, &u4Val);
    u4Val = (*pfgSet) ?
            (u4Val | PAD_SERVOADC_EN(i4Gpio)) :
            (u4Val & ~(PAD_SERVOADC_EN(i4Gpio)));    
    _WriteMT8297(REGSIF_RW_ADPADE, &u4Val);
    
    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_ServoGpioOutput() The GPIO output value setting functions. It will check
 *  the i4Gpio and set to related register bit as 0 or 1.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *          the bit of gpio number (0 or 1).  If it's not NULL, it must 
 *          reference to a integer.  If the integer is 0, this function 
 *          will set the bit of the gpio number as 0, otherwise set as 1.
 *  @retval If pfgSet is NULL, it return 0 or 1 (the bit value of the gpio
 *          number of output mode.  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ServoGpioOutput(INT32 i4Gpio, const INT32 *pfgSet)
{
    UINT32 u4Val;
    INT32 i4Idx;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_SERVOADC_NUM))
    {
        return -1;
    }

    _ReadMT8297(REGSIF_RW_ADPADO, &u4Val);   
    if (pfgSet == NULL)
    {
        return ((u4Val & PAD_SRVIO_OUT(i4Gpio)) ? 1 : 0);
    }
    u4Val = (*pfgSet) ?
            (u4Val | PAD_SRVIO_OUT(i4Gpio)) :
            (u4Val & ~ PAD_SRVIO_OUT(i4Gpio));
            
    // Write pad output  
    _WriteMT8297(REGSIF_RW_ADPADO, &u4Val);   

    // Set to output mode.
    i4Idx = 1;
    VERIFY(i4Idx==PDWNC_ServoGpioEnable(i4Gpio, &i4Idx));

    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/** PDWNC_ServoGpioInput()  The GPIO input reading functions. It will check the 
 *  i4Gpio and read related register bit to return.
 *  @param i4Gpio the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ServoGpioInput(INT32 i4Gpio)
{
    UINT32 u4Val;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_SERVOADC_NUM))
    {
        return -1;
    }

    // Servo ADC used as GPIO
    _ReadMT8297(REGSIF_RW_ADPADG, &u4Val);
    u4Val |= SRVIO_GP(i4Gpio);    
    _WriteMT8297(REGSIF_RW_ADPADG, &u4Val);

    // Servo ADC used as GPI
    _ReadMT8297(REGSIF_RW_ADPADE, &u4Val);
    u4Val &= ~(SRVIO_GP_O(i4Gpio));    
    _WriteMT8297(REGSIF_RW_ADPADE, &u4Val);

    // Read pad input  
    _ReadMT8297(REGSIF_RW_ADPADI, &u4Val);

    return ((u4Val & PAD_SRVIO_IN(i4Gpio)) ? 1U : 0);

}

#ifndef CC_5391_LOADER
//-----------------------------------------------------------------------------
/** PDWNC_GpioIntrq() The GPIO interrupt enable setting functions. It will
 *  check the i4Gpio and set to related register bit as 0 or 1.  In this 
 *  function, 0 is interrupt disable mode and 1 is interrupt enable mode.
 *  @param i4Gpio the gpio number to set or read.
 *  @param pfgSet A integer pointer.  If it's NULL, this function will return
 *  the current setting of gpio number (0 or 1).  If it's not NULL, it must
 *  reference to a integer.  If the integer is 0, this function will set the
 *  mode of the gpio number as interrupt disable mode, otherwise set as
 *  interrupt enable mode.
 *  @retval If pfgSet is NULL, it return 0 or 1 (0 is interrupt disable mode,
 *          1 is interrupt enable mode.)  Otherwise, return (*pfgSet).
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioIntrq(INT32 i4Gpio, const INT32 *pfgSet)
{
    UINT32 u4Val;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    u4Val = PDWNC_READ32(REG_RW_IOPADEN);
    if (pfgSet == NULL)
    {
        return ((u4Val & (1U << i4Gpio)) ? 1 : 0);
    }
    u4Val = (*pfgSet) ?
            (u4Val | (1U << i4Gpio)) :
            (u4Val & ~(1U << i4Gpio));
    PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);

    return (*pfgSet);
}

//-----------------------------------------------------------------------------
/* PDWNC_GpioQuery() to query PDWNC Gpio settings.
 *  @param i4Gpio should be between 0~7.
 *  @param pi4Intr to get interrupt enable/disable status.
 *  @param pi4Fall to get falling callback enable/disable status.
 *  @param pi4Rise to get rising callback enable/disable status.
 *  @retval return 0 if successful, otherwise return -1.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise)
{
    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    if (pi4Intr)
    {
        *pi4Intr = (PDWNC_READ32(REG_RW_PDWNC_INTEN) & (1U << i4Gpio)) ? 1 : 0;
    }

    if ((PDWNC_READ32(REG_RW_PDWNC_INTEN) & (1U << i4Gpio))==0)
    {
        if (pi4Fall)
        {
            *pi4Fall = 0;
        }
        if (pi4Rise)
        {
            *pi4Rise = 0;
        }
        return 0;
    }

    if (pi4Fall)
    {
        *pi4Fall = (PDWNC_READ32(REG_RW_EXTINTCFG) & (1U << (i4Gpio+16))) ? 1 : 
                    ((PDWNC_READ32(REG_RW_EXTINTCFG) & (1U << i4Gpio)) ? 0 : 1);
    }
    if (pi4Rise)
    {
        *pi4Rise = (PDWNC_READ32(REG_RW_EXTINTCFG) & (1U << (i4Gpio+16))) ? 1 : 
                    ((PDWNC_READ32(REG_RW_EXTINTCFG) & (1U << i4Gpio)) ? 1 : 0);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/* PDWNC_GpioReg() to setup the PDWNC Gpio interrupt callback function, type,
 *      and state.
 *  @param i4Gpio should be between 0~7.
 *  @param eType should be one of enum GPIO_TYPE.
 *  @param pfnCallback the callback function.
 *  @retval 0 successful, -1 failed.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback)
{
    UINT32 u4Val;

    if ((i4Gpio < 0) || (i4Gpio >= TOTAL_PDWNC_GPIO_NUM))
    {
        return -1;
    }

    switch(eType)
    {
    case GPIO_TYPE_INTR_FALL:
        u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
        u4Val |= (1U << i4Gpio);        // Enable interrupt.
        PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);
        u4Val = PDWNC_READ32(REG_RW_EXTINTCFG);
        u4Val &= ~(1U << (i4Gpio+16));  // Disable double edge trigger.
        u4Val &= ~(1U << (i4Gpio+8));   // Set Edge trigger.
        u4Val &= ~(1U << i4Gpio);       // Set Falling Edge interrupt.
        PDWNC_WRITE32(REG_RW_EXTINTCFG, u4Val);
        break;
    case GPIO_TYPE_INTR_RISE:
        u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
        u4Val |= (1U << i4Gpio);        // Enable interrupt.
        PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);
        u4Val = PDWNC_READ32(REG_RW_EXTINTCFG);
        u4Val &= ~(1U << (i4Gpio+16));  // Disable double edge trigger.
        u4Val &= ~(1U << (i4Gpio+8));   // Set Edge trigger.
        u4Val |= (1U << i4Gpio);        // Set Rising Edge interrupt.
        PDWNC_WRITE32(REG_RW_EXTINTCFG, u4Val);
        break;
    case GPIO_TYPE_INTR_BOTH:
        u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
        u4Val |= (1U << i4Gpio);        // Enable interrupt.
        PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);
        u4Val = PDWNC_READ32(REG_RW_EXTINTCFG);
        u4Val |= (1U << (i4Gpio+16));   // Enable double edge trigger.
        PDWNC_WRITE32(REG_RW_EXTINTCFG, u4Val);
        break;
    case GPIO_TYPE_INTR_LEVEL:
        u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
        u4Val |= (1U << i4Gpio);        // Enable interrupt.
        PDWNC_WRITE32(REG_RW_PDWNC_INTEN, u4Val);
        u4Val = PDWNC_READ32(REG_RW_EXTINTCFG);
        u4Val &= ~(1U << (i4Gpio+16));  // Disable double edge trigger.
        u4Val |= (1U << (i4Gpio+8));    // Set Level trigger.
        u4Val &= ~(1U << i4Gpio);       // Always level low active.
        PDWNC_WRITE32(REG_RW_EXTINTCFG, u4Val);
        break;
    case GPIO_TYPE_NONE:
        u4Val = PDWNC_READ32(REG_RW_PDWNC_INTEN);
        u4Val &= ~(1U << i4Gpio);       // Disable interrupt.
        return 0;
    default:
        return -1;
    }

    if (pfnCallback)
    {
        _afnPDWNCGpioCallback[i4Gpio] = pfnCallback;
    }
    return 0;
}

#endif /* CC_5391_LOADER */

