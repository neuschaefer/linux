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
#include "x_irq.h"
#include "nptv_vdec_if.h"
#ifdef CC_AUD_ANALOG_INPUT_SWITCH_PROTECT	
#include "../../aud/periph/codec_mt5365.h"
#endif

#ifdef DEFINE_IS_LOG
#undef DEFINE_IS_LOG
#endif
#ifdef CC_MTK_LOADER
#define DEFINE_IS_LOG(level, fmt...)   Printf(fmt)
#else /* CC_MTK_LOADER */
#define DEFINE_IS_LOG	IR_IsLog
#endif /* CC_MTK_LOADER */
#include "x_debug.h"

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


#if defined(LINUX_TURNKEY_SOLUTION) || defined(__KERNEL__)
#define LED_BLINK_ENABLE        0
#else
#define LED_BLINK_ENABLE        1
#endif

static void GPIO_MtkPowerOnFunc(void);
static void GPIO_MtkPowerOffFunc(UINT32 u4PowerDownEn);
static void GPIO_MtkLedCtrlFunc(LED_STATUS_T eLedStatus);
#ifdef CC_MTK_LOADER
static INT32 LOADER_MtkUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer);
static void LOADER_MtkEnvInitFunc(void *prLdrData, UINT32 fgValidFlag);
#ifdef LOADER_USB_UPGRADE
static void LOADER_MtkUpgradeDlmCbFunc(UINT32 u4BufSize);
#endif
#endif /* CC_MTK_LOADER */

#if LED_BLINK_ENABLE
#ifndef CC_MTK_LOADER
static void _LedBlinking_Task(HANDLE_T hHandle, VOID* pTag);
#endif
#endif

#if LED_BLINK_ENABLE
#if defined(CC_MTK_LOADER)
static void _LOADER_TimerOff(void);
static void _LOADER_TimerOn(void);
static void _LOADER_TimerIsr(void);
static void _LOADER_InitGPIOTimer(void);
#endif
#endif

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
#include "loader_if.h"

#ifndef PINMUX_INCLUDE_FILE
    #include "mtk/mtk_pin_setup.h"
#else
    #include PINMUX_INCLUDE_FILE
#endif


#ifdef CC_MTK_LOADER
static void LOADER_EnableHdmiDdc(void)
{
#ifndef CC_HDMI_EDID_DISABLE 
    BSP_PinSet(PIN_HDMI_0_SDA, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_0_SCL, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_1_SDA, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_1_SCL, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_2_SDA, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_2_SCL, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_3_SDA, PINMUX_FUNCTION1);
    BSP_PinSet(PIN_HDMI_3_SCL, PINMUX_FUNCTION1);
#endif
}

static void LOADER_DisableHdmiDdc(void)
{
	UINT32 u4HDMIHPD0GPIO, u4HDMIHPD1GPIO,u4HDMIHPD2GPIO,u4HDMIHPD3GPIO;

#ifndef CC_HDMI_EDID_DISABLE 
    BSP_PinSet(PIN_HDMI_0_SDA, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_0_SCL, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_1_SDA, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_1_SCL, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_2_SDA, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_2_SCL, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_3_SDA, PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_3_SCL, PINMUX_FUNCTION0);
	BSP_PinSet(PIN_HDMI_0_HPD,PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_1_HPD,PINMUX_FUNCTION0);
	BSP_PinSet(PIN_HDMI_2_HPD,PINMUX_FUNCTION0);
    BSP_PinSet(PIN_HDMI_3_HPD,PINMUX_FUNCTION0);
#endif
if((DRVCUST_InitQuery(eHDMIHPD0GPIONUM, &u4HDMIHPD0GPIO))==0)	
  {
   GPIO_SetOut(u4HDMIHPD0GPIO, 0);
  } 
if((DRVCUST_InitQuery(eHDMIHPD1GPIONUM, &u4HDMIHPD1GPIO))==0)	
  {
	 GPIO_SetOut(u4HDMIHPD1GPIO, 0);
   }	 
if((DRVCUST_InitQuery(eHDMIHPD2GPIONUM, &u4HDMIHPD2GPIO))==0)	
{
	GPIO_SetOut(u4HDMIHPD2GPIO, 0);
 	}

#if  defined(CC_MT5396)|| defined(CC_MT5368)
 if((DRVCUST_InitQuery(eHDMIHPD3GPIONUM, &u4HDMIHPD3GPIO))==0)	
{
	GPIO_SetOut(u4HDMIHPD3GPIO, 0);
 }
#endif

#if  defined(CC_MT5389)

              // PD_SYS   turn off TMDS
            IO_WRITE32(0xF002C13C,0xc00, ((IO_READ32(0xF002C13C,0xc00))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 0,1
			IO_WRITE32(0xF002C13C,0x800, ((IO_READ32(0xF002C13C,0x800))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 2
			

			//REG_HDMI_RX_Normal_0_CFG2  turn off TMDS 
			IO_WRITE32(0xf0062008, 0x100,(IO_READ32(0xF0062008,0x100))& 0xFFFEFFFF); // REG_HDMI_RX_Normal_0_CFG2  Port 0,1
			IO_WRITE32(0xf0062008, 0x0,  (IO_READ32(0xF0062008,0x0))& 0xFFFEFFFF);//Port 2

			// DDC DISABLE
    		IO_WRITE32(0xf002C008 ,0xC00 ,(IO_READ32(0xf002C008,0XC00)& 0xFFFF00FF)|0x00008500); //SYS_CTRL
			IO_WRITE32(0xf002C008 ,0X800 ,(IO_READ32(0xf002C008,0X800)& 0xFFFF00FF)|0x00008500);
			
#elif defined(CC_MT5396)

			 // PD_SYS	 turn off TMDS
		   IO_WRITE32(0xF002C13C,0xc00, ((IO_READ32(0xF002C13C,0xc00))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 0
		   IO_WRITE32(0xF002C13C,0x800, ((IO_READ32(0xF002C13C,0x800))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 1
		   IO_WRITE32(0xF002C13C,0x400, ((IO_READ32(0xF002C13C,0x400))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 2
		   IO_WRITE32(0xF002C13C,0x0,   ((IO_READ32(0xF002C13C,0x0))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 3

 			//REG_HDMI_RX_Normal_0_CFG2  turn off TMDS 
			IO_WRITE32(0xf0062008, 0x100,(IO_READ32(0xF0062008,0x100))& 0xFFFEFFFF); // REG_HDMI_RX_Normal_0_CFG2  Port 0
			IO_WRITE32(0xf0062008, 0x0,  (IO_READ32(0xF0062008,0x0))& 0xFFFEFFFF);//Port 1
			IO_WRITE32(0xf0062008, 0x200,(IO_READ32(0xF0062008,0x200))& 0xFFFEFFFF); // REG_HDMI_RX_Normal_0_CFG2  Port2
			IO_WRITE32(0xf0062008, 0x300,  (IO_READ32(0xF0062008,0x300))& 0xFFFEFFFF);//Port 3			

		  //SYS_CTRL DCC DISABLE
    		IO_WRITE32(0xf002C008 ,0xC00 ,(IO_READ32(0xf002C008,0XC00)& 0xFFFF00FF)|0x00008500); //SYS_CTRL
			IO_WRITE32(0xf002C008 ,0X800 ,(IO_READ32(0xf002C008,0X800)& 0xFFFF00FF)|0x00008500);
     		IO_WRITE32(0xf002C008 ,0x400 ,(IO_READ32(0xf002C008,0x400)& 0xFFFF00FF)|0x00008500); //SYS_CTRL
			IO_WRITE32(0xf002C008 ,0X0 ,(IO_READ32(0xf002C008,0X0)& 0xFFFF00FF)|0x00008500);

#elif defined(CC_MT5396)

         // PD_SYS	 turn off TMDS
           IO_WRITE32(0xF002C13C,0xc00, ((IO_READ32(0xF002C13C,0xc00))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 0
		   IO_WRITE32(0xF002C13C,0x800, ((IO_READ32(0xF002C13C,0x800))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 1
		   IO_WRITE32(0xF002C13C,0x400, ((IO_READ32(0xF002C13C,0x400))&0xFF00FFFF)|0x00CF0000); // PD_SYS PORT 2
      
         

          //REG_HDMI_RX_Normal_0_CFG2  turn off TMDS 
 
          	IO_WRITE32(0xf0062008, 0x100,(IO_READ32(0xF0062008,0x100))& 0xFFFEFFFF); // REG_HDMI_RX_Normal_0_CFG2  Port 0
			IO_WRITE32(0xf0062008, 0x0,  (IO_READ32(0xF0062008,0x0))& 0xFFFEFFFF);//Port 1
			IO_WRITE32(0xf0062008, 0x200,(IO_READ32(0xF0062008,0x200))& 0xFFFEFFFF); // REG_HDMI_RX_Normal_0_CFG2  Port2
 
          //SYS_CTRL DCC DISABLE
           	IO_WRITE32(0xf002C008 ,0xC00 ,(IO_READ32(0xf002C008,0XC00)& 0xFFFF00FF)|0x00008500); //SYS_CTRL
			IO_WRITE32(0xf002C008 ,0X800 ,(IO_READ32(0xf002C008,0X800)& 0xFFFF00FF)|0x00008500);
     		IO_WRITE32(0xf002C008 ,0x400 ,(IO_READ32(0xf002C008,0x400)& 0xFFFF00FF)|0x00008500); //SYS_CTRL
#endif  

}

static INT32 LOADER_MtkUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer)
{
    INT32 i4Ret;
    i4Ret = strcmp(szCurVer, szUpgVer); // i4Ret = szCurVer - szUpgVer.
    Printf("CurVer:%s UpgVer:%s\n", szCurVer, szUpgVer);
    return (0 > i4Ret) ? 0 : 1;   // 0 is ok to upgrade, upgrade when szCurVer < szUpgVer.
}

static void LOADER_MtkEnvInitFunc(void *prLdrData, UINT32 fgValidFlag)
{
	UINT32 u4Val ;
    UNUSED(prLdrData);
    UNUSED(fgValidFlag);
    UNUSED(LOADER_MtkUpgradeVerCmpFunc);

    //Printf("Pinmux Initial setting: totoal (%d) pins\n", sizeof(_gu4SysPinmuxInit)/(sizeof(_gu4SysPinmuxInit[0])));
    if(sizeof(_gu4SysPinmuxInit))
    {
        for(u4Val = 0;u4Val < sizeof(_gu4SysPinmuxInit)/(sizeof(_gu4SysPinmuxInit[0])); u4Val++)
        {
            BSP_PinSet(_gu4SysPinmuxInit[u4Val][0], _gu4SysPinmuxInit[u4Val][1]);
        }
    }
#ifdef CC_AUD_ANALOG_INPUT_SWITCH_PROTECT	
    GPIO_SetOut(GPIO_AIN0_L,0);
	GPIO_SetOut(GPIO_AIN0_R,0);
    GPIO_SetOut(GPIO_AIN1_L,0);
	GPIO_SetOut(GPIO_AIN1_R,0);
	GPIO_SetOut(GPIO_AIN2_L,0);
	GPIO_SetOut(GPIO_AIN2_R,0);
	GPIO_SetOut(GPIO_AIN3_L,0);
	GPIO_SetOut(GPIO_AIN3_R,0);
	GPIO_SetOut(GPIO_AIN4_L,0);
	GPIO_SetOut(GPIO_AIN4_R,0);
	GPIO_SetOut(GPIO_AIN5_L,0);
	GPIO_SetOut(GPIO_AIN5_R,0);
	GPIO_SetOut(GPIO_AIN6_L,0);
	GPIO_SetOut(GPIO_AIN6_R,0);    
#endif    
    
    if (fgValidFlag)
    {
        return;
    }

    // AUD_CLR_BIT(REG_INT_SDADC2, ADC_R_PD);
    IO_WRITE32(AUDIO_BASE,0x0248, (IO_READ32(AUDIO_BASE,0x0248)) & (~((0x1 << 13))));
    // AUD_CLR_BIT(REG_INT_SDADC2, ADC_L_PD);
    IO_WRITE32(AUDIO_BASE,0x0248, (IO_READ32(AUDIO_BASE,0x0248)) & (~((0x1 << 12))));
    // AUD_WRITE32(REG_INT_DACCTL10,(AUD_READ32(REG_INT_DACCTL10) & ~PD_CTL_MASK)|PD_ON);
    IO_WRITE32(AUDIO_BASE,0x01e8, (IO_READ32(AUDIO_BASE,0x01e8)) & (~(0xff << 4)));
    // AUD_CLR_BIT(REG_INT_DACCTL10,PD_ISOURCE);
    IO_WRITE32(AUDIO_BASE,0x01e8, (IO_READ32(AUDIO_BASE,0x01e8)) & (~((0x1 << 26))));
    // AUD_CLR_BIT(REG_INT_DACCTL10,PD_REF3);
    // AUD_CLR_BIT(REG_INT_DACCTL10,PD_REF2);
    // AUD_CLR_BIT(REG_INT_DACCTL10,PD_REF1);
    // AUD_CLR_BIT(REG_INT_DACCTL10,PD_REF0);
    IO_WRITE32(AUDIO_BASE,0x01e8, (IO_READ32(AUDIO_BASE,0x01e8)) & (~((0xf << 12))));
	u4Val = PDWNC_ReadWakeupReason();
	if((u4Val == PDWNC_WAKE_UP_REASON_AC_POWER) || (u4Val == PDWNC_WAKE_UP_REASON_WATCHDOG))
	{
		LOADER_DisableHdmiDdc();
		vInternalEDIDInit();
	}
	LOADER_EnableHdmiDdc();
}

#ifdef LOADER_USB_UPGRADE
#include "x_dlm.h"

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
/*
static void LOADER_MtkUpgradeDlmCbFunc(UINT32 u4BufSize)
{
    UNUSED(LOADER_MtkUpgradeDlmCbFunc);
    x_dlm_reg_append_callback("all", u4BufSize, LOADER_MtkAllUpgrade);
    x_dlm_reg_append_callback("firm", u4BufSize, LOADER_MtkFirmUpgrade);
    x_dlm_reg_append_callback("nor1", u4BufSize, LOADER_MtkFirmUpgrade);
}
*/
#endif  /* LOADER_USB_UPGRADE */
#endif /* CC_MTK_LOADER */



#if LED_BLINK_ENABLE
#if defined(CC_MTK_LOADER)//#ifdef CC_MTK_LOADER
static PFN_IRQ_HANDLER _pfnTimerLedIsr = NULL;

static void _LOADER_TimerOff(void)
{
    UINT32 u4Val;

    // disable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val &= ~((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);            
}

static void _LOADER_TimerOn(void)
{
    UINT32 u4Val;

    // Enable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= ((TMR1_CNTDWN_EN) | (TMR1_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
}



static void _LOADER_TimerIsr(void)
{
    static UINT32 _fgLed = 0;
    UINT32 i, u4Val, u4BlinkingLedGpio, u4BlinkingLedOnVal;
    
    if (BIM_IsIrqPending(VECTOR_T1))
    {
        if (DRVCUST_InitQuery(ePanelPowerGpio, &i) == 0)
        {
            UNUSED(DRVCUST_InitQuery(ePanelPowerOnVal, &u4Val));
            UNUSED(DRVCUST_InitQuery(eBootBlinkingLed, &u4BlinkingLedGpio));            
//Led_blinking function is only enabled while panel has not been turned on
            if(1)//if(GPIO_GetIn(i) != u4Val)
            {
                _fgLed = !_fgLed;
                GPIO_SetOut(u4BlinkingLedGpio, _fgLed);
                //Printf("[LedBlinkikng] toggle..\n");        
            }
            else//always turn on the led (it shoule be POWER LED in gerneal case)
            {
                UNUSED(DRVCUST_InitQuery(eBootBlinkingLedOnVal, &u4BlinkingLedOnVal));            
                GPIO_SetOut(u4BlinkingLedGpio, u4BlinkingLedOnVal);
                _LOADER_TimerOff();
            }
        }        
        BIM_ClearIrq(VECTOR_T1);        
    }
    
    if (_pfnTimerLedIsr) { _pfnTimerLedIsr(); }
}


static void _LOADER_InitGPIOTimer(void)
{
    UINT32 u4BusClock;
    UINT32 u4TimerClock;
    UINT32 u4LedBlinkingPeriod;
    if (DRVCUST_InitQuery(eBootBlinkingLedPeriod, &u4LedBlinkingPeriod) == 0)
    {    
        _pfnTimerLedIsr = RegisterIrqHandler(_LOADER_TimerIsr);

#if 1 //#ifdef  CC_MT5387
        u4BusClock = GET_XTAL_CLK()/1000;
        u4TimerClock = u4BusClock;
#else
        u4BusClock = BSP_GetDomainClock(SRC_BUS_CLK);
        u4TimerClock = (UINT32)(u4BusClock / 1000);
#endif
        u4TimerClock *= u4LedBlinkingPeriod;
        //Printf("[LedBlinkikng] u4TimerClock is %d\n", u4TimerClock);        
        // Setup Timer 0 and triger 1st interrupt.
        BIM_WRITE32(REG_RW_TIMER1_LLMT, u4TimerClock);
        BIM_WRITE32(REG_RW_TIMER1_HLMT, 0);
        BIM_WRITE32(REG_RW_TIMER1_LOW , u4TimerClock);
        BIM_WRITE32(REG_RW_TIMER1_HIGH, 0);

        _LOADER_TimerOff();
        
        BIM_EnableIrq(VECTOR_T1);

        HalEnableIRQ();
    }
}


#else
static HANDLE_T _hLedBlinkingTimer = NULL;
static void _LedBlinking_Task(HANDLE_T hHandle, VOID* pTag)
{
    static UINT32 _fgLed = 0;
    static UINT32 _StopBlinking = 0;
    UINT32 i, u4Val, u4BlinkingLedGpio, u4BlinkingLedOnVal;
    
    UNUSED(hHandle);
    UNUSED(pTag);
    if(_StopBlinking  == 1)
        return;
    if (DRVCUST_InitQuery(/*ePanelPowerGpio*/eBackLightGpio, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(/*ePanelPowerOnVal*/eBackLightOnVal, &u4Val));
        UNUSED(DRVCUST_InitQuery(eBootBlinkingLed, &u4BlinkingLedGpio));            
//Led_blinking function is only enabled while panel has not been turned on
        if((GPIO_Output(i, NULL) != u4Val) && (GPIO_Enable(i, NULL) == 1))
        {
            _fgLed = !_fgLed;
            GPIO_SetOut(u4BlinkingLedGpio, _fgLed);
             //Printf("[LdrLedBlinkikng Timer] Led toggle!\n");            
        }
        else//always turn on the led (it shoule be POWER LED in gerneal case)
        {
             Printf("[LedBlinkikng] Panel is turned on, stop blinking led!!!\n");
            UNUSED(DRVCUST_InitQuery(eBootBlinkingLedOnVal, &u4BlinkingLedOnVal));            
            GPIO_SetOut(u4BlinkingLedGpio, u4BlinkingLedOnVal);
            x_timer_stop(_hLedBlinkingTimer);
            VERIFY(x_timer_delete(_hLedBlinkingTimer) == OSR_OK);
            _hLedBlinkingTimer = NULL;
            _StopBlinking = 1;
        }
    }        
}
#endif
#endif

static void GPIO_MtkLedCtrlFunc(LED_STATUS_T eLedStatus)
{
    UINT32 i;
    INT32 i4Val;
#if LED_BLINK_ENABLE
#ifndef CC_MTK_LOADER    
    UINT32 u4LedBlinkingPeriod;
#endif
#endif

    switch(eLedStatus)
    {
    case eLedLoaderPowerOn:
        INIT_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        INIT_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
#if LED_BLINK_ENABLE
#if defined(CC_MTK_LOADER)//#ifdef CC_MTK_LOADER            
        if (DRVCUST_InitQuery(eFlagBootBlinkingLedSupport, &i) == 0)
        {
            Printf("[LdrLedBlinkikng] Start ot Init/Turn on timer!\n");
            _LOADER_InitGPIOTimer();
            _LOADER_TimerOn();
        }
        else
        {
            Printf("Not support [LdrLedBlinkikng]!!\n");
        }
#endif
#endif
        break;
    case eLedLoaderPowerOff:
        DOWN_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);
        DOWN_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        break;
    case eLedSystemPowerOn:
        INIT_SET_GPIO(ePowerLedGpio, ePowerLedOnVal);
        INIT_SET_GPIO(eBackupLedGpio, eBackupLedOnVal);

//start to blink led at tv stage        
#if LED_BLINK_ENABLE
#ifndef CC_MTK_LOADER    
        if (DRVCUST_InitQuery(eFlagBootBlinkingLedSupport, &i) == 0)
        {
            UNUSED(DRVCUST_InitQuery(eBootBlinkingLedPeriod, &u4LedBlinkingPeriod));
            if (x_timer_create(&_hLedBlinkingTimer) != OSR_OK)
            {
                Printf("[LedBlinkikng] Create timer fail !\n");
            }
            
            if (x_timer_start(_hLedBlinkingTimer, u4LedBlinkingPeriod, X_TIMER_FLAG_REPEAT,
                              _LedBlinking_Task, NULL) != OSR_OK)
            {
                Printf("[LedBlinkikng] Start timer fail !\n");
            }
        }
        else
        {
            Printf("Not support [LedBlinkikng]!!\n");
        }
#endif        
#endif
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
    /*if (DRVCUST_InitQuery(eDemodResetGpio, &i) == 0)
    {
        GPIO_SetOut(i, 0);
        HAL_Delay_us(20000); //delay 20ms
        GPIO_SetOut(i, 1);
        Printf("Reset Demod GPIO=%x, 0->1\r\n",i);
    }*/

#else /* CC_MTK_LOADER */
    UINT32 u4Val;
    if(sizeof(_gu4SysPinmuxInit))
    {
        for(u4Val = 0;u4Val < sizeof(_gu4SysPinmuxInit)/(sizeof(_gu4SysPinmuxInit[0])); u4Val++)
        {
            BSP_PinSet(_gu4SysPinmuxInit[u4Val][0], _gu4SysPinmuxInit[u4Val][1]);
        }
    }
    GPIO_MtkLedCtrlFunc(eLedSystemPowerOn);

    // Mute OSD before driver init.
    PMX_SetBg(0x00000000, FALSE);
    IO_WRITE32(OSD_BASE, 0x208, IO_READ32(OSD_BASE, 0x208) & 0xffff00ffU);
    IO_WRITE32(OSD_BASE, 0x308, IO_READ32(OSD_BASE, 0x308) & 0xffff00ffU);

    vDrvSetPWM(SrcPWM2, (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / 48000, 0x80, 0xff);
    UNUSED(polarity);

#endif /* CC_MTK_LOADER */

    INIT_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
    INIT_SET_GPIO(eDvdCtrlGpio, eDvdCtrlOnVal);

    // Set PAD_DEMOD_TSDATA1 as low for ethernet link down issue
    // ES1 IC will resolve this issue
    GPIO_SetOut(GPIO(96), 0);
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

    DOWN_SET_GPIO(eUSBVbus0Gpio, eUSBVbus0EnablePolarity);
    DOWN_SET_GPIO(eUSBVbus1Gpio, eUSBVbus1EnablePolarity);
    DOWN_SET_GPIO(eUSBVbus2Gpio, eUSBVbus2EnablePolarity);
    DOWN_SET_GPIO(eUSBVbus3Gpio, eUSBVbus3EnablePolarity);

}

#else
#error "reinclude mtk_gpio.h..."
#endif

