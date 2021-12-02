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
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pinmux_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "hw_vdoin.h"
#include "hw_bim.h"
#include "x_assert.h"
#include "x_ckgen_mt82.h"
#include "x_gpio_mt82.h"
#include "pdwnc_t8032.h"
#include "pdwnc_srvad.h"
#include "pdwnc_drvif.h"
#include "pdwnc_gpio.h"
#include "rtc_if.h"
#include "ir_if.h"
#else
#include "ir_if.h"
#include "rtc_if.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_gpio.h"
#include "drvcust_if.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_hw.h"
#include "pdwnc_t8032.h"
#include "pdwnc_gpio.h"
#include "pdwnc_srvad.h"
#include "pdwnc_entry.h"
#include "pdwnc_drvif.h"
#include "sif_if.h"
#include "eeprom_if.h"
#include "pdwnc_if.h"
#endif
#include "drv_adcset.h"
#include "ostg_if.h"
#include "drv_display.h"
//#undef PDWNC_WRITE32
//#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configuration
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
/////Customize//////////////////////////////////////////////////
extern UINT32 DRVCUST_IrrxGet(QUERY_TYPE_T eQryType);

#ifdef CUSTOM_CFG_FILE
#include CUSTOM_CFG_FILE
#endif /* CUSTOM_CFG_FILE */

#include "drv_def_file.h"

#ifdef GPIO_INCLUDE_FILE
#include GPIO_INCLUDE_FILE
#endif
 
#ifdef PDWNC_EXT_IMPL
#include PDWNC_EXT_IMPL
#endif
#include "drv_default.h"

///////////////////////////////////////////////////////
#endif
#ifdef CC_MTK_LOADER
//#undef x_crit_start
//#undef x_crit_end
//#define x_crit_start() 0
//#define x_crit_end(x) UNUSED(x)
#define CRIT_STATE_T UINT8
#else
#include "x_os.h"
#include "sif_if.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MT8297_RETRY 5
#define SERVO_RETRY 5

#define VGAWAKEUP_VRANGE_U 0xfff
#define VGAWAKEUP_VRANGE_L 0x1c
#if defined(CC_MT5365) || defined(CC_MT5395)
	//27MHz
	#define CSYNC_CONT_THU_DEFAULT   (0x01a4)
	#define CSYNC_CONT_THL_DEFAULT   (0x00ff)
	#define CSYNC_CONT_THH_DEFAULT	(0x19c)
	#define VGAWAKEUP_HRANGE_U	(0x700)
	#define VGAWAKEUP_HRANGE_L	(0xc0)
	//this paramter is the time to try for each sync type , should bigger than V_STABLE_TH 
	#define VGAWAKEUP_FREERUN_CNT_TH  (0x2fffff)   // almost = 0x4cccc * 9
#else
	//3MHz
	#define CSYNC_CONT_THU_DEFAULT   (0x01a4 / 9)
	#define CSYNC_CONT_THL_DEFAULT   (0x00ff / 9)
	#define CSYNC_CONT_THH_DEFAULT	(0x19c /9)
	#define VGAWAKEUP_HRANGE_U	(0x199)
	#define VGAWAKEUP_HRANGE_L	(0x1c)
	#define VGAWAKEUP_FREERUN_CNT_TH  (0x4cccc)
#endif

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#ifndef CC_UP8032_ATV
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

#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
// PDWNC module sub-interrupt routine list.
static volatile x_os_isr_fct _varIntList[MAX_PDWNC_INT_ID+1];
#endif /* CC_MTK_LOADER */

static INT32 _gi4PDWNCInit = 0;
static UINT32 _fgPowerDownByAcCut = 0;

static BOOL _fgIrLedEnable = TRUE;
static PDWNC_SET_POWER_DOWN_REASON_T _grPowerDownReason;
#ifdef CC_MT5391
static const UINT8 _au1StrappingGpio[7] = 
{
    33,
    34,
    54,
    55,
    117,
    118,
    119
};

static const UINT8 _au1StrappingShift[7] =
{
    6,
    5,
    3,
    4,
    0,
    1,
    2
};
#endif

// static PDWNC_FuncTbl *pgrPDWNC_Func = NULL;
#define pgrPDWNC_Func   (PDWNC_EXT_GetFunc())


//-----------------------------------------------------------------------------
// Imported functions 
//-----------------------------------------------------------------------------
//EXTERN void HAL_Delay_us(UINT32 u4Micros);
extern void PDWNC_ResumeServoADC(void);

//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------

//INT32 PDWNC_ServoGpioInput(INT32 i4Gpio);

//INT32 PDWNC_WriteErrorCode(UINT8 u1ErrorCode);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
INT32 DRVCUST_GpioQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);


void HAL_Delay_us(UINT32 u4Micros){

	while(u4Micros > 0xff){

		vKrDelay2us(0xff/2);
		u4Micros -= 0xff;
	}
	if(u4Micros)
		vKrDelay2us(u4Micros/2);

}

void INIT_SET_GPIO(gpio, onval){
	UINT32 i,i4Val;

	if (DRVCUST_GpioQuery(gpio, &i)==0)              
	{                                               
		if (DRVCUST_GpioQuery(onval, (UINT32 *)&i4Val)==0)     
		{                                           \
			VERIFY(GPIO_Output((INT32)i, &i4Val) == i4Val);  
		}                                           
	}
}

void DOWN_SET_GPIO(gpio, onval){
	UINT32 i,i4Val;
	if (DRVCUST_GpioQuery(gpio, &i)==0)              
	{                                               
		if (DRVCUST_GpioQuery(onval, (UINT32 *)&i4Val)==0)     
		{                                           
			i4Val = !i4Val;                         
			VERIFY(GPIO_Output((INT32)i, &i4Val)== i4Val);  
		}                                           
	}
}


INT32 DRVCUST_GpioQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data)
{


    switch(eQryType)
    {

	//Usb upgrade
    case eLoaderUpgradeVerCmpFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_VER_CMP_FUNC;
        return 0;

#if 0
    // GPIO hook function
    case eGpioPowerOnFunc:
        *pu4Data = (UINT32)GPIO_POWER_ON_FUNC;
        return 0;
		
    case eGpioPowerOffFunc:
#ifndef GPIO_POWER_OFF_FUNC
        *pu4Data = (UINT32)GPIO_POWER_OFF_FUNC;
        return 0;
#else /* GPIO_POWER_OFF_FUNC */
        return -1;
#endif /* GPIO_POWER_OFF_FUNC */

    case eGpioLedCtrlFunc:
#ifdef GPIO_LED_CTRL_FUNC
        *pu4Data = (UINT32)GPIO_LED_CTRL_FUNC;
        return 0;
#else /* GPIO_LED_CTRL_FUNC */
        return -1;
#endif /* GPIO_LED_CTRL_FUNC */
#endif
// Keypad
    case eFlagButtonSupport:
        *pu4Data = FLAG_BUTTON_SUPPORT;
        return 0;
    case eSingleGpioKeyList:
        *pu4Data = (UINT32)SINGLE_GPIO_KEY_LIST;
        return 0;
    case eSingleServoKeyList:
        *pu4Data = (UINT32)SINGLE_SERVO_KEY_LIST;
        return 0;
    case eGreedGpioKeyList:
        *pu4Data = (UINT32)GREED_GPIO_KEY_LIST;
        return 0;
    case eMultiKeyList:
        *pu4Data = (UINT32)MULTI_KEY_LIST;
        return 0;
    case eKeyPadWakeupBit:
        *pu4Data = (UINT32)KEYPAD_WAKEUP_BIT;
        return 0;
    case eKeyPadPollingTime:
        *pu4Data = (UINT32)KEYPAD_POLLING_TIME;
        return 0;
    case eKeyPadCheckTime:
        *pu4Data = (UINT32)KEYPAD_CHECK_TIME;
        return 0;
   // MT8292 GPIO support
    case eFlagMt8292GpioSupport:
        *pu4Data = FLAG_MT8292_GPIO_SUPPORT;
        return 0;
    case eMt8292ResetGpio:
        *pu4Data = MT8292_RESET_GPIO;
        return 0;
    case eMt8292MuteGpio:
        *pu4Data = MT8292_MUTE_GPIO;
        return 0;
    case eMt8280ResetGpio:
        *pu4Data = MT8280_RESET_GPIO;
        return 0;

    case eDemodResetGpio:
#ifdef DEFAULT_DEMOD_RESET_GPIO
        *pu4Data = (UINT32) DEFAULT_DEMOD_RESET_GPIO;
        return 0;
#else /* DEFAULT_DEMOD_RESET_GPIO */
        return -1;
#endif /* DEFAULT_DEMOD_RESET_GPIO */

    // VGA wakeup setting by external controller
    case eVGAMaxHSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MAX_HSYNC_FREQ;
        return 0;
    case eVGAMinHSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MIN_HSYNC_FREQ;
        return 0;
    case eVGAMaxVSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MAX_VSYNC_FREQ;
        return 0;
    case eVGAMinVSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MIN_VSYNC_FREQ;
        return 0;

    // USB0 vbus control.
#ifdef USB0_VBUS_GPIO
    case eUSBVbus0Gpio:
        *pu4Data = USB0_VBUS_GPIO;
        return 0;
#endif
#ifdef USB0_VBUS_GPIO_POLARITY
    case eUSBVbus0EnablePolarity:
        *pu4Data = USB0_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB1 vbus control.
#ifdef USB1_VBUS_GPIO
    case eUSBVbus1Gpio:
        *pu4Data = USB1_VBUS_GPIO;
        return 0;
#endif
#ifdef USB1_VBUS_GPIO_POLARITY
    case eUSBVbus1EnablePolarity:
        *pu4Data = USB1_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB0 over current flag enable.
#ifdef USB0_OC_GPIO
    case eUSB0OCGpio:
        *pu4Data = USB0_OC_GPIO;
        return 0;
#endif
#ifdef USB0_OC_GPIO_POLARITY
    case eUSB0OCEnablePolarity:
        *pu4Data = USB0_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB1 over current flag enable.
#ifdef USB1_OC_GPIO
    case eUSB1OCGpio:
        *pu4Data = USB1_OC_GPIO;
        return 0;
#endif
#ifdef USB1_OC_GPIO_POLARITY
    case eUSB1OCEnablePolarity:
        *pu4Data = USB1_OC_GPIO_POLARITY;
        return 0;
#endif
/*
//STA335
    case eAudioSTA335ResetGpio:
        *pu4Data = AUDIO_STA335_RESET_GPIO;
        return 0;
#endif // AUDIO_STA335_RESET_GPIO 
#ifdef AUDIO_STA335_RESET_GPIO_POLARITY
    case eAudioSTA335ResetGpioPolarity:
        *pu4Data = AUDIO_STA335_RESET_GPIO_POLARITY;
        return 0;
#endif // AUDIO_STA335_RESET_GPIO_POLARITY 
*/
// Other Gpio
    case eExternalStatus:
        *pu4Data = FLAG_EXT_STATUS;
        return 0;
    case eStandbySetGpio:
        *pu4Data = STANDBY_SET_GPIO;
        return 0;
    case ePowerStatusGpio:
        *pu4Data = POWER_STATUS_GPIO;
        return 0;
    case eStdbyUsePowerBtnCount:
        *pu4Data = STDBY_USE_PWR_BTN_CNT;
        return 0;
    case eStdbyUsePrevPowerState:
        *pu4Data = STDBY_USE_PREV_PWR_STATE;
        return 0;
    case eStdbyUseVGAStatus:
        *pu4Data = STDBY_USE_VGA_STATUS;
        return 0;
    case ePowerCtrlDelayMs:
        *pu4Data = POWER_CTRL_DELAY_MS;
        return 0;
    case ePowerCtrlGpio:
#ifdef POWER_CTRL_GPIO
        *pu4Data = POWER_CTRL_GPIO;
        return 0;
#else
        return -1;
#endif
    case ePowerCtrlOnVal:
#ifdef POWER_CTRL_ON_VAL
        *pu4Data = POWER_CTRL_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eDvdCtrlGpio:
#ifdef DVD_CTRL_GPIO
        *pu4Data = DVD_CTRL_GPIO;
        return 0;
#else
        return -1;
#endif
    case eDvdCtrlOnVal:
#ifdef DVD_CTRL_ON_VAL
        *pu4Data = DVD_CTRL_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eSpecialWakeupGpio:
#ifdef SPECIAL_WAKEUP_GPIO
        *pu4Data = SPECIAL_WAKEUP_GPIO;
        return 0;
#else
        return -1;
#endif
    case eSpecialWakeupOnVal:
#ifdef SPECIAL_WAKEUP_ON_VAL
        *pu4Data = SPECIAL_WAKEUP_ON_VAL;
        return 0;
#else
        return -1;
#endif

// Led
    case ePowerLedGpio:
#ifdef POWER_LED_GPIO
        *pu4Data = POWER_LED_GPIO;
        return 0;
#else
        return -1;
#endif
    case ePowerLedOnVal:
#ifdef POWER_LED_ON_VAL
        *pu4Data = POWER_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eBackupLedGpio:
#ifdef BACKUP_LED_GPIO
        *pu4Data = BACKUP_LED_GPIO;
        return 0;
#else
        return -1;
#endif
    case eBackupLedOnVal:
#ifdef BACKUP_LED_ON_VAL
        *pu4Data = BACKUP_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eSupportDarkLed:
        *pu4Data = FLAG_SUPPORT_DARK_LED;
        return 0;
    case ePowerLedDarkVal:
#ifdef POWER_LED_DARK_VAL
        *pu4Data = POWER_LED_DARK_VAL;
        return 0;
#else
        return -1;
#endif
    case eBackupLedDarkVal:
#ifdef BACKUP_LED_DARK_VAL
        *pu4Data = BACKUP_LED_DARK_VAL;
        return 0;
#else
        return -1;
#endif
    case eFlagSupportPowerLedOff:
        *pu4Data = (UINT32)FLAG_SUPPORT_POWER_LED_OFF;
        return 0;

#ifdef COMPANION_CHIP_GPIO_NUM
    case eCompanionChipGpio:
        *pu4Data = (UINT32)COMPANION_CHIP_GPIO_NUM;
        return 0;
#endif
#ifdef COMPANION_CHIP_GPIO_POLARITY
    case eCompanionChipGpioPolarity:
        *pu4Data = (UINT32)COMPANION_CHIP_GPIO_POLARITY;
        return 0;
#endif

    case ePdwncPolarity:
        *pu4Data = (UINT32)PDWNC_POLARITY;
        return 0;

#ifdef SW_POWER_DETECT_VOLTAGE
    case eSWPowerDetectVoltage:
        *pu4Data = (UINT32)SW_POWER_DETECT_VOLTAGE;
        return 0;
#endif


    default:
	LogSD("query gpio item fail!  id = ",eQryType);
        return -1;
    }
}


UINT32 DRVCUST_GpioGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret;

    VERIFY(DRVCUST_GpioQuery(eQryType, &u4Ret) == 0);
    return u4Ret;
}
#endif

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
/** _PdwncHwIsr() is the interrupt routine of PDWNC module.
 *  @param u2VctId is not used, it should be VECTOR_PDWNC.
 */
//-----------------------------------------------------------------------------
static void _PdwncHwIsr(UINT16 u2VctId)
{
    UINT32 i = 0, u4Val;
    
    UNUSED(u2VctId);
    
    // Should we use this?? XXX discussion this.    
    u4Val = u4IO32Read4B(PDWNC_INTSTA) & u4IO32Read4B(PDWNC_ARM_INTEN);
/*
    // Check interrupt from T8032 first
//    if(u4IO32Read4B(REG_RW_INT) & AUXINT_ST)    
    if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))    
    {
        if(_varIntList[PDWNC_INTNO_AUX_INT])
        {
            _varIntList[PDWNC_INTNO_AUX_INT](i);
        }
        else
        {
            LOG(11, "PDWNC(0x%08x) interrupt from T8032 no routine hook\n", u4Val);            
        }
				vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
    }

    u4Val = u4IO32Read4B(PDWNC_INTSTA) & u4IO32Read4B(PDWNC_ARM_INTEN);
*/
for ( ; i < MAX_PDWNC_INT_ID; i++)
    {
        if (u4Val & _PINT(i))
        {
            if (_varIntList[i])
            {
                _varIntList[i](i);
                vIO32Write4B(PDWNC_INTCLR, _PINT(i));
            }
            else
            {
                LOG(11, "PDWNC(0x%08x) interrupt %d no routine hook\n",
                    u4Val, i);
                // Still need to clear this non-hook interrupt.
                vIO32Write4B(PDWNC_INTCLR, _PINT(i));
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
#ifdef CC_UP8032_ATV
	vEnableSysInt(PDWNCBIM_INT_EN);
        vClrSysInt(PDWNC_INT_CLR);
#else
        i4Val = x_reg_isr(VECTOR_PDWNC, _PdwncHwIsr, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }
        VERIFY(BIM_ClearIrq(VECTOR_PDWNC));
#endif
    }
    else if (!fgSet && (fgSet!=fgCurSet))
    {   /* disable irq. */
        fgCurSet = fgSet;
#ifdef CC_UP8032_ATV
	vDisableSysInt(PDWNCBIM_INT_EN);
#else
        i4Val = x_reg_isr(VECTOR_PDWNC, NULL, &pfnOldIsr);
        if (i4Val != OSR_OK)
        {
            return FALSE;
        }
#endif
    }
    return TRUE;
}
#endif /* CC_MTK_LOADER */

static void _PdwncSetupIRRX(UINT32 u4PowerDownEn)
{
    INT32 i4Val;
    UINT32 u4Val;

    if ((u4PowerDownEn & WAK_IRRX) == 0)
    {
        return;
    }
#ifdef CC_UP8032_ATV
	i4Val = DRVCUST_IrrxGet(eIrrxType);
#else
    i4Val = DRVCUST_InitQuery(eIrrxType, &u4Val);
#endif
    if (i4Val < 0)
    {
        u4Val = IRRX_RC5_TYPE;
    }

    // To write u4PowerDownEn for Wakeup key setting.

	 	vIO32Write4B(PDWNC_WAKEN, u4PowerDownEn);

    // Configuration
    i4Val = IRRX_InitMtkIrRegSetup(u4Val);
    UNUSED(i4Val);
}

/*static*/ void _PdwncSetupVGA(UINT32 u4PowerDownEn)
{
//    UINT32 u4Val;

#ifndef CC_MTK_LOADER
    vDrvSYNCSlicerWakeup(1);
#endif
	if((u4PowerDownEn & WAK_VGA) == 0)   // if(1)//if ((u4PowerDownEn & WAK_VGA) == 0)
    {
#if 1//#ifndef CC_NO_ACD_STANDBY_SETTING

        Printf("Disable VGA wakeup\n");
        // Power off slicer module to reduce current
#if !defined(CC_MT5365) && !defined(CC_MT5395)
        vIO32WriteFldAlign(SYNCFG0, 0x1, FLD_RG_SYNC_BG_PWD);
		vIO32WriteFldAlign(SYNCFG0, 0x1, FLD_RG_DET_SORS_PWD);        
		vIO32WriteFldAlign(SYNCFG0, 0x1, FLD_RG_DET_SINK_PWD);        
		vIO32WriteFldAlign(SYNCFG0, 0x1, FLD_RG_DET_PWD);        
#endif			
        HAL_Delay_us(5); 
        vIO32WriteFldAlign(SYNCFG2, 0x1, FLD_RG_FB_PWD);        

        vIO32WriteFldAlign(SYNCFG2, 0x1, FLD_RG_SOGY_SINK_PWD);        
        vIO32WriteFldAlign(SYNCFG2, 0x1, FLD_RG_SOGY_SORS_PWD);        
        vIO32WriteFldAlign(SYNCFG2, 0x1, FLD_RG_SYNC_PWD);        
#if !defined(CC_MT5365) && !defined(CC_MT5395)
        vIO32WriteFldAlign(SYNCFG2, 0x1, FLD_RG_SYNC_TST_PWD);                
#endif			
        HAL_Delay_us(5);          

        // Turn on VGA_TESTBUF_PWD
        vIO32WriteFldAlign(VGACFG1, 0x1, FLD_RG_VGA_TESTBUF_PWD);
        HAL_Delay_us(5);     

        // Disable VGA sync det clock
        vIO32WriteFldAlign(PDWNC_CLKPDN, 0x1, FLD_VGAPD);
        HAL_Delay_us(5);     

#if defined(CC_MT5365) || defined(CC_MT5395)//20100414
		vIO32WriteFldAlign(VGACFG0, 0x1, FLD_RG_VMUX_PWD);
		vIO32WriteFldAlign(VGACFG2, 0x1, FLD_RG_SOGY_ADC_PWD);
		vIO32WriteFldAlign(VGACFG2, 0x0, FLD_RG_AUTOBIAS_EN);//20100415, sc hwu suggestion		
		vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_VGA_SYNC_DIS);//20100415, chiahsien liu suggestion		
#endif
#endif
        return;
    }


        // David: force enable VGA sync circuit clock 20090219
		vIO32WriteFldAlign(PDWNC_CLKPDN, 0x0, FLD_VGAPD);
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, 0x1, FLD_AUTO_SWITCH_EN);

	 	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, VGAWAKEUP_HRANGE_U, FLD_HRANGE_U);
	 	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_0_REGISTER, VGAWAKEUP_HRANGE_L, FLD_HRANGE_L);
		HAL_Delay_us(5);
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, VGAWAKEUP_VRANGE_U, FLD_VRANGE_U);
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, VGAWAKEUP_VRANGE_L, FLD_VRANGE_L);
		HAL_Delay_us(5);


#if !defined(CC_MT5365) && !defined(CC_MT5395)
		//not used
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0x01c, FLD_CRANGE_L);
		vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_2_REGISTER, 0xfff, FLD_CRANGE_U);
#endif

#ifdef CC_DISABLE_CSYNC_WAKEUP	
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x1, FLD_C_SYNC_WAK_DIS);	
#endif
	HAL_Delay_us(5);
#ifdef CC_DISABLE_SOG_WAKEUP  	
        vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0x1, FLD_SOG_WAK_DIS);	
#endif
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER, /*0x06*/0xF, FLD_H_DIFF_TH);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER, 0x02, FLD_V_STABLE_TH);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_3_REGISTER, 0x80, FLD_H_STABLE_TH);
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_4_REGISTER, 0x3f, FLD_V_DIFF_TH);
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THL_DEFAULT & 0xff), FLD_CSYNC_CONT_THL);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THL_DEFAULT >> 8), FLD_CYSNC_CT_THL);	
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THU_DEFAULT & 0xff), FLD_CSYNC_CONT_THU);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_5_REGISTER, (CSYNC_CONT_THU_DEFAULT >> 8), FLD_CSYNC_CT_THU);	
	 HAL_Delay_us(5);
    
#if defined(CC_MT5365) || defined(CC_MT5395)
	 //new coding
	vIO32WriteFldMulti(PDWNC_VGA_WAKE_UP_CONTROL_6_REGISTER, P_Fld(0, FLD_HLEN_USE_VMASKX)| P_Fld(0x208, FLD_VMASK_S)| P_Fld(0xA, FLD_VMASK_E));	
#else
	//hlen_use_vmask =0 and vmask_s = 0x208 and vmask_e = 0xa
	vIO32Write4B(PDWNC_VGA_WAKE_UP_CONTROL_6_REGISTER, 0x0020800A);
#endif
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_7_REGISTER, (CSYNC_CONT_THH_DEFAULT & 0xff), FLD_CSYNC_CONT_SLICE_THH);
	HAL_Delay_us(5);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER, (CSYNC_CONT_THH_DEFAULT >> 8), FLD_CSYNC_CONT_THH);
	vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_8_REGISTER, VGAWAKEUP_FREERUN_CNT_TH, FLD_FREERUN_CNT_TH);
}

#ifndef CC_MTK_LOADER
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
#endif /* CC_MTK_LOADER */

static void _PdwncSetupHDMI(UINT32 u4PowerDownEn)
{
    if ((u4PowerDownEn & WAK_HDMI) == 0)
    {
			vIO32Write4B(PDWNC_PADCFG3, 0);// Disable HDMI
      return;
    }
#ifdef CC_NO_HDMI
        vIO32Write4B(PDWNC_PADCFG3, 0);// Disable HDMI
#endif
}

// Disable PDWNC Pinmux function, Move to ckgen/pinmux.c in MT5391.
#if 0
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
#endif

//-----------------------------------------------------------------------------
/* _PDWNC_SetupStrapping() to setup the PDWNC Gpio interrupt callback function, type,
 *      and state.
 *  @param i4Gpio should be between 0~7.
 *  @param eType should be one of enum GPIO_TYPE.
 *  @param pfnCallback the callback function.
 *  @retval 0 successful, -1 failed.
 */
//-----------------------------------------------------------------------------
void _PDWNC_SetupStrapping(void)
{
    return;
}

//-----------------------------------------------------------------------------
/* PDWNC_ReadWakeupReason() to read the wakeup reason 
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 _PDWNC_ReadWakeupStatus(void)
{
    static UINT32 u4Status;
    static BOOL fgHasRead = FALSE;

    if(fgHasRead)
    {
        return u4Status;
    }

    u4Status = u4IO32Read4B(PDWNC_PDSTAT);

    fgHasRead = TRUE;
    
    return (u4Status);
}

//-----------------------------------------------------------------------------
/* PDWNC_ReadWatchDogStatus() to read watchdog reset status
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 _PDWNC_ReadWatchDogStatus(void)
{
    static UINT32 u4Status;
    static BOOL fgHasRead = FALSE;

    if(fgHasRead)
    {
        return u4Status;
    }

    u4Status = (u4IO32Read4B(PDWNC_WDTCTL) & FLD_WDTALL_RST);

    fgHasRead = TRUE;
    
    return (u4Status);
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
#ifdef CC_FPGA
    return *pu1Year;
#else
    if (pu1Year)
    {
        vIO32WriteFldAlign(PDWNC_PROT, 0x87, FLD_PROT);
        vIO32WriteFldAlign(PDWNC_PROT, 0x78, FLD_PROT);
        vIO32Write4B(PDWNC_YEAR, (UINT32)(*pu1Year));
        vIO32WriteFldAlign(PDWNC_PROT, 0x0, FLD_PROT);
    }
#if defined(CC_MT8223)
    RTC_READ_POLLING();
    #endif
    return(u4IO32Read4B(PDWNC_YEAR));
#endif
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


//-----------------------------------------------------------------------------
/* PDWNC_PowerDownByAcCut()
 * 
 *  @param u1ErrorCode  Error code
 *  @retval   0		    SUCCESS.		
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_PowerDownByAcCut(void)
{
    UNUSED(PDWNC_Init());
    return _fgPowerDownByAcCut;
}
//-----------------------------------------------------------------------------
/* PDWNC_pm_suspend() to initialize PDWNC driver
 * 
 *  @param u1ErrorCode  Error code
 */
//-----------------------------------------------------------------------------
void PDWNC_pm_suspend(void)
{
    Printf("PDWNC_pm_suspend \n");
    _gi4PDWNCInit = 0;
}
//-----------------------------------------------------------------------------
/* PDWNC_pm_resume() to initialize PDWNC driver
 * 
 *  @param u1ErrorCode  Error code
 */
//-----------------------------------------------------------------------------
void PDWNC_pm_resume(void)
{
    UINT32 u4Val;
    PFN_GPIO_POWER_FUNC pfnPowerOnFunc = NULL;

    if (_gi4PDWNCInit)
    {
        Printf("Warning! PDWNC_pm_resume has been executed..\n");
        return ;
    }
    _gi4PDWNCInit = 1;
    BIM_EnableIrq(VECTOR_PDWNC);
// MT5391 Specific initialization
#ifdef CC_MT5391
    if (IS_XTAL_60MHZ())
    {
        u4Val = PDCK_SEL_XTAL_D20;
    }
    else
    {
        u4Val = PDCK_SEL_XTAL_D9;    
    }    
    _PDWNC_WriteMT8297(REGSIF_RW_CKGEN, &u4Val);
#endif

#ifndef CC_MTK_LOADER    
    // If use internal T8032, need to initialize something...
    if(DRVCUST_InitGet(eT8032uPOffset) != 0)
    {
        // Let ARM control other engine after loading image      
        // Note this should happen before _PDWNC_InitGpio to avoid interrupt before interrupt status/clear could be access
#ifndef CC_MT8223
        UINT32 i = 0, j = 0;              
        PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));//        vIO32Write4B(PDWNC_SHREG8,  PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));    
         	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            while(i < 100000) // Wait 100 ms for T8032's ack
            {
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    Printf("Preloader T8032 ack !\n");        
                    //if((PDWNC_READ8(REG_RW_AUX_DATA) & 0xff) != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    if(/*(u1IO32Read1B(PDWNC_SHREG0) & 0xff)*/PDWNC_T8032CMD_GET_RESPONSE()  != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    {      
                        Printf("Preloader T8032 ack mismatch!\n");        
                        continue;
                    }
                    HAL_Delay_us(1);
                    break;                
                }            
                i++;
            }
            vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
            if(i < 100000) // T8032 ack within 100 ms, we're done
            {     
                break;
            }
            // Any cleanup before re-issue interrupt ??        
            else
            {
                i = 0;
            }
            if(++j > 2) // Set j to interrupt re-issue retry count
            {
                Printf("T8032 ack timeout!\n");            
                break;
            }   
        }               
        //david: useless ...PDWNC_WRITE32(REG_RW_UP_CFG, PDWNC_READ32(REG_RW_UP_CFG) & ~(ENGINE_EN));    
#endif   
        PDWNC_INTEN(PDWNC_INTNO_AUX_INT);
        // Install GPIO ISR
        PDWNC_GPIO_Resume();

        u4Val = PDWNC_ReadWakeupReason();
        if((u4Val != PDWNC_WAKE_UP_REASON_AC_POWER) && (u4Val != PDWNC_WAKE_UP_REASON_WATCHDOG))
        {           
            LOG(5, "T8032 init wakeup case app stage...\n");                       
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }    
        else
        {
            LOG(5, "T8032 init AC on/off case app stage...\n");                            
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }
    }
    else
    {
        // Install GPIO ISR
        PDWNC_GPIO_Resume();
        LOG(5, "T8032 is disabled...\n");            
    }
#else
    u4Val = PDWNC_ReadWakeupReason();
    if(u4Val == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Year = 0xff;
        // To indicate it is last cut off by A/C
        VERIFY(PDWNC_SetQueryRtcYear(&u1Year) == u1Year);                
    }    
#endif /* CC_MTK_LOADER */

#ifdef CC_UP8032_ATV

	if(GPIO_POWER_ON_FUNC!=NULL)
		GPIO_POWER_ON_FUNC();

#else
    if (DRVCUST_InitQuery(eGpioPowerOnFunc, (UINT32 *)(void *)&pfnPowerOnFunc) == 0)
    {
        pfnPowerOnFunc();
    }
#endif	

    _fgPowerDownByAcCut = (PDWNC_SetQueryRtcYear(NULL)==0xff);

// MT5360 Specific initialization
    
#if defined(CC_MT5365) || defined(CC_MT5395)
	vIO32Write4B(PDWNC_XTALCFG, 0x0e000000);
#else
    // Restore default xtal setting.
	vIO32WriteFldAlign(PDWNC_XTALCFG, 0x0, FLD_XTALGM);
#endif
    // HDMI I2C settings
#if !defined(CC_MT8223)
    #if defined(CC_MT5365) || defined(CC_MT5395)
        u4Val = u4IO32Read4B(PDWNC_PADCFG1);
        u4Val &= ~(FLD_HDMI_SDA_MASK   | FLD_HDMI_SCL_MASK);
        vIO32Write4B(PDWNC_PADCFG1, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SCL_SMT);
    #else    //mt5387 and mt5363
        #if defined(CC_MT5387) && !defined(CC_MT5363)//mt5387 only
        u4Val = u4IO32Read4B(PDWNC_PADCFG1);
        u4Val &= ~(FLD_HDMI_SDA0_MASK	| FLD_HDMI_SCL0_MASK);
        vIO32Write4B(PDWNC_PADCFG1, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SDA0_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SCL0_SMT);
        #endif
        u4Val = u4IO32Read4B(PDWNC_PADCFG3);
        u4Val &= ~(FLD_HDMI_SDA1_MASK	| FLD_HDMI_SCL1_MASK);
        vIO32Write4B(PDWNC_PADCFG3, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG3, 0x1, FLD_HDMI_SDA1_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG3, 0x1, FLD_HDMI_SCL1_SMT);

        u4Val = u4IO32Read4B(PDWNC_PADCFG4);
        u4Val &= ~(FLD_HDMI_SDA2_MASK	| FLD_HDMI_SCL2_MASK);
        vIO32Write4B(PDWNC_PADCFG4, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG4, 0x1, FLD_HDMI_SDA2_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG4, 0x1, FLD_HDMI_SCL2_SMT);
    #endif        
#endif
    PDWNC_ResumeServoADC();

    // Set reset value to some registers.
    vIO32Write4B(SYNCFG0, 0xe8884c88);// SYNC slicer current reduction
	
#if defined(CC_MT5365) || defined(CC_MT5395)//20100414
		vIO32WriteFldAlign(VGACFG0, 0x0, FLD_RG_VMUX_PWD);
		vIO32WriteFldAlign(VGACFG2, 0x0, FLD_RG_SOGY_ADC_PWD);
		vIO32WriteFldAlign(VGACFG2, 0x1, FLD_RG_AUTOBIAS_EN);//20100415, sc hwu suggestion		
		vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_VGA_SYNC_DIS);//20100415, chiahsien liu suggestion		
#endif

#ifdef CC_DISABLE_SOG_WAKEUP	
//#if 1
        vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOG_EN);
        vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SYNC_PWD);
#endif
    
#ifndef CC_MTK_LOADER

    // Do the external power down function
    VERIFY(pgrPDWNC_Func->pfnInit);
    pgrPDWNC_Func->pfnInit();
    
#endif /* CC_MTK_LOADER */

    return ;
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
    PFN_GPIO_POWER_FUNC pfnPowerOnFunc = NULL;

    if (_gi4PDWNCInit)
    {
        return 0;
    }

    _gi4PDWNCInit = 1;


// MT5391 Specific initialization
#ifdef CC_MT5391
    if (IS_XTAL_60MHZ())
    {
        u4Val = PDCK_SEL_XTAL_D20;
    }
    else
    {
        u4Val = PDCK_SEL_XTAL_D9;    
    }    
    _PDWNC_WriteMT8297(REGSIF_RW_CKGEN, &u4Val);
#endif

#ifndef CC_MTK_LOADER    
    // If use internal T8032, need to initialize something...
    if(DRVCUST_InitGet(eT8032uPOffset) != 0)
    {
        // Let ARM control other engine after loading image      
        // Note this should happen before _PDWNC_InitGpio to avoid interrupt before interrupt status/clear could be access
#ifndef CC_MT8223
        UINT32 i = 0, j = 0;              
        PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));//        vIO32Write4B(PDWNC_SHREG8,  PDWNC_CMD_ARMT8032_ENGINE_SWITCH | (1 << 8));    
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            while(i < 100000) // Wait 100 ms for T8032's ack
            {
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    Printf("Preloader T8032 ack !\n");        
                    //if((PDWNC_READ8(REG_RW_AUX_DATA) & 0xff) != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    if(/*(u1IO32Read1B(PDWNC_SHREG0) & 0xff)*/PDWNC_T8032CMD_GET_RESPONSE()  != PDWNC_CMD_ARMT8032_ENGINE_SWITCH)
                    {      
                        Printf("Preloader T8032 ack mismatch!\n");        
                        continue;
                    }
                    HAL_Delay_us(1);
                    break;                
                }            
                i++;
            }
            vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
            if(i < 100000) // T8032 ack within 100 ms, we're done
            {     
                break;
            }
            // Any cleanup before re-issue interrupt ??        
            else
            {
                i = 0;
            }
            if(++j > 2) // Set j to interrupt re-issue retry count
            {
                Printf("T8032 ack timeout!\n");            
                break;
            }   
        }               
        //david: useless ...PDWNC_WRITE32(REG_RW_UP_CFG, PDWNC_READ32(REG_RW_UP_CFG) & ~(ENGINE_EN));    
#endif   
#ifndef CC_UP8032_ATV   
        // Install T8032 ISR
        VERIFY(_PDWNC_RegT8032Isr() == PDWNC_OK);
#endif
        // Install GPIO ISR
        VERIFY(_PDWNC_InitGpio() == 0);              

        u4Val = PDWNC_ReadWakeupReason();
        if((u4Val != PDWNC_WAKE_UP_REASON_AC_POWER) && (u4Val != PDWNC_WAKE_UP_REASON_WATCHDOG))
        {           
            LOG(5, "T8032 init wakeup case app stage...\n");                       
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }    
        else
        {
            LOG(5, "T8032 init AC on/off case app stage...\n");                            
            VERIFY(_PDWNC_SetT8032(PDWNC_T8032_SET_NORMAL) == 0);
        }
    }
    else
    {
        // Install GPIO ISR
        VERIFY(_PDWNC_InitGpio() == 0);    
        LOG(5, "T8032 is disabled...\n");            
    }
#else
    u4Val = PDWNC_ReadWakeupReason();
    if(u4Val == PDWNC_WAKE_UP_REASON_AC_POWER)
    {   
        UINT8 u1Year = 0xff;
        // To indicate it is last cut off by A/C
        VERIFY(PDWNC_SetQueryRtcYear(&u1Year) == u1Year);                
    }    
#endif /* CC_MTK_LOADER */

#ifdef CC_UP8032_ATV

	if(GPIO_POWER_ON_FUNC!=NULL)
		GPIO_POWER_ON_FUNC();

#else
    if (DRVCUST_InitQuery(eGpioPowerOnFunc, (UINT32 *)(void *)&pfnPowerOnFunc) == 0)
    {
        pfnPowerOnFunc();
    }
#endif	

    _fgPowerDownByAcCut = (PDWNC_SetQueryRtcYear(NULL)==0xff);

// MT5360 Specific initialization
    
#if defined(CC_MT5365) || defined(CC_MT5395)
	//vIO32Write4B(PDWNC_XTALCFG, 0x0e000000); david: in MT5365/MT5395, this register control by PCMCIA driver
#else
    // Restore default xtal setting.
	vIO32WriteFldAlign(PDWNC_XTALCFG, 0x0, FLD_XTALGM);
#endif
    // HDMI I2C settings
#if !defined(CC_MT8223)
    #if defined(CC_MT5365) || defined(CC_MT5395)
        u4Val = u4IO32Read4B(PDWNC_PADCFG1);
        u4Val &= ~(FLD_HDMI_SDA_MASK   | FLD_HDMI_SCL_MASK);
        vIO32Write4B(PDWNC_PADCFG1, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SDA_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SCL_SMT);
    #else    //mt5387 and mt5363
        #if defined(CC_MT5387) && !defined(CC_MT5363)//mt5387 only
        u4Val = u4IO32Read4B(PDWNC_PADCFG1);
        u4Val &= ~(FLD_HDMI_SDA0_MASK	| FLD_HDMI_SCL0_MASK);
        vIO32Write4B(PDWNC_PADCFG1, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SDA0_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG1, 0x1, FLD_HDMI_SCL0_SMT);
        #endif
        u4Val = u4IO32Read4B(PDWNC_PADCFG3);
        u4Val &= ~(FLD_HDMI_SDA1_MASK	| FLD_HDMI_SCL1_MASK);
        vIO32Write4B(PDWNC_PADCFG3, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG3, 0x1, FLD_HDMI_SDA1_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG3, 0x1, FLD_HDMI_SCL1_SMT);

        u4Val = u4IO32Read4B(PDWNC_PADCFG4);
        u4Val &= ~(FLD_HDMI_SDA2_MASK	| FLD_HDMI_SCL2_MASK);
        vIO32Write4B(PDWNC_PADCFG4, u4Val);
        vIO32WriteFldAlign(PDWNC_PADCFG4, 0x1, FLD_HDMI_SDA2_SMT);
        vIO32WriteFldAlign(PDWNC_PADCFG4, 0x1, FLD_HDMI_SCL2_SMT);
    #endif        
#endif
    


    PDWNC_InitServoADC();

    // Set reset value to some registers.
    vIO32Write4B(SYNCFG0, 0xe8884c88);// SYNC slicer current reduction
	
#if defined(CC_MT5365) || defined(CC_MT5395)//20100414
		vIO32WriteFldAlign(VGACFG0, 0x0, FLD_RG_VMUX_PWD);
		vIO32WriteFldAlign(VGACFG2, 0x0, FLD_RG_SOGY_ADC_PWD);
		vIO32WriteFldAlign(VGACFG2, 0x1, FLD_RG_AUTOBIAS_EN);//20100415, sc hwu suggestion		
		vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_VGA_SYNC_DIS);//20100415, chiahsien liu suggestion		
#endif

#ifdef CC_DISABLE_SOG_WAKEUP	
//#if 1
        vIO32WriteFldAlign(SYNCFG2, 1, FLD_RG_SOG_EN);
        vIO32WriteFldAlign(SYNCFG2, 0, FLD_RG_SYNC_PWD);
#endif
    
#ifndef CC_MTK_LOADER

    // Do the external power down function
    VERIFY(pgrPDWNC_Func->pfnInit);
    pgrPDWNC_Func->pfnInit();
    
#endif /* CC_MTK_LOADER */

    return 0;
}

#ifndef CC_MTK_LOADER
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

#ifndef CC_UP8032_ATV//prevent recursive call   PDWNC_RegIsr -> PDWNC_Init -> _PDWNC_InitGpio -> PDWNC_RegIsr
    VERIFY(PDWNC_Init()==0);
#endif
    if(u2IntNo > MAX_PDWNC_INT_ID)
    {
        return PDWNC_FAIL;
    }

    if (ppfnOld != NULL)
    {
        *ppfnOld = _varIntList[u2IntNo];
    }
   
    if (pfnIsr == NULL)
    {
      u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
      u4Val &= ~(_PINT(u2IntNo));
      if (u4Val==0)
      {
          VERIFY(_PdwncHwSetIsr(FALSE));
      }
#ifndef CC_MTK_LOADER
      u4State = HalCriticalStart();
#endif
      vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
#ifndef CC_MTK_LOADER
      HalCriticalEnd(u4State);
#endif
      _varIntList[u2IntNo] = pfnIsr;
    }
    else
    {

  		_varIntList[u2IntNo] = pfnIsr;
			u4Val = u4IO32Read4B(PDWNC_ARM_INTEN);
      u4Val |= IS_PINTNO_INTABLE_GPIO(u2IntNo) ? 0 : (_PINT(u2IntNo));		
#ifndef CC_MTK_LOADER
      u4State = HalCriticalStart();
#endif
      VERIFY(_PdwncHwSetIsr(TRUE));
      vIO32Write4B(PDWNC_ARM_INTEN, u4Val);
#ifndef CC_MTK_LOADER
      HalCriticalEnd(u4State);
#endif
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

    if (u2IntNo > MAX_PDWNC_INT_ID)//MAX_PINT_ID)
    {
        return FALSE;
    }

    u4Int = _PINT(u2IntNo);
#ifndef CC_MTK_LOADER
    u4State = HalCriticalStart();
#endif
          vIO32Write4B(PDWNC_INTCLR, u4Int);
#ifndef CC_MTK_LOADER
    HalCriticalEnd(u4State);
#endif
    return TRUE;
}
#endif /* CC_MTK_LOADER */

//-----------------------------------------------------------------------------
/** PDWNC_T8032Reboot() Use T8032 to Enter standby and then boot the system
 */
//-----------------------------------------------------------------------------
void PDWNC_T8032Reboot(UINT32 u4Seconds)
{
#ifndef CC_MTK_LOADER
    UINT32 u4State;

    if (DRVCUST_OptGet(eAudioAmpEnableGpioNum) != 0xffffffff)
    {
        // turn off audio amp
        GPIO_SetOut(DRVCUST_OptGet(eAudioAmpEnableGpioNum), !DRVCUST_OptGet(eAudioAmpGpioPolarity));
    }
    if (DRVCUST_OptGet(eAudioMuteGpioNum) != 0xffffffff)
    {
        // set audio mute
        GPIO_SetOut(DRVCUST_OptGet(eAudioMuteGpioNum), DRVCUST_OptGet(eAudioMuteGpioPolarity));
    }

#ifndef CC_UBOOT
    // turn off backlight
    vDrvTurnOffBacklt();
    // turn off lvds power
    vDrvTurnOffPanel();
#endif
#endif /* CC_MTK_LOADER */
#ifndef CC_MTK_LOADER
    u4State = HalCriticalStart();
#endif
    PDWNC_T8032CMD_SET_CMD(PDWNC_CMD_ARMT8032_COLD_REBOOT | (((u4Seconds > 12) ? (0x7F) : (u4Seconds*10)) << 8));
    PDWNC_T8032CMD_SET_DATA(0);

    vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
#ifndef CC_MTK_LOADER
    HalCriticalEnd(u4State);
#endif
}

//-----------------------------------------------------------------------------
/** PDWNC_Reboot() Reboot the system
 */
//-----------------------------------------------------------------------------
void PDWNC_Reboot()
{
#ifndef CC_UBOOT
    #ifdef __KERNEL__
        UINT32 u4State;
    #endif

#ifdef CC_UP8032_ATV
	vKrDisableInt();
#else
    #ifdef __KERNEL__
        u4State = HalCriticalStart();
        BIM_WRITE32(REG_MISC_IRQEN, 0);
        BIM_WRITE32(REG_IRQEN, 0);
        HalCriticalEnd(u4State);
    #else
      HalDisableIRQ();
      HalDisableFIQ();
    #endif
#endif
#endif /* CC_UBOOT */
    _PDWNC_SetupStrapping();
    
    Printf("\n                                       ");
    vIO32Write4B(PDWNC_WDT0,0x7fff0000);//vIO32Write4B(PDWNC_WDT0,0x0fff0000);
	vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) | 0x1);//	vIO32WriteFldAlign(PDWNC_WDTCTL, 0x1, FLD_WDT0E);
    while (1) { }
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
    
    // For compatible with both main chip servoADC & mcm servoADC
    _PDWNC_ReadServoADCWakeupStatus(&u4Servo);
    u4Val = (_PDWNC_ReadWakeupStatus() & ~(0xff << 20)) | u4Servo;

#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
#else
    i = DRVCUST_InitQuery(eSingleGpioKeyList, (UINT32 *)(void *)&prSingleGpioKeyList);
#endif
    if (i == 0)
    {
        while ((prSingleGpioKeyList) &&
            (prSingleGpioKeyList[i].u4Key != 0) &&
            (prSingleGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prSingleGpioKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Gpio = prSingleGpioKeyList[i].u4Gpio - OPCTRL(0);
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

#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
#else
    i = DRVCUST_InitQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
#endif
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
                if (u4Servo > 31)
                {
                    return 0;
                }                
                //if (u4Val & SRVAD_WAKEN(WAK_SRVADX(u4Servo)))
                if (u4Val & PDWNC_SRVAD_WAKEN(PDWNC_WAK_SRVADX(u4Servo)))                
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

#ifdef CC_UP8032_ATV
    i = DRVCUST_GpioQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
#else
    i = DRVCUST_InitQuery(eGreedGpioKeyList, (UINT32 *)(void *)&prGreedGpioKeyList);
#endif
    if (i==0)
    {
        while ((prGreedGpioKeyList) && 
            (prGreedGpioKeyList[i].u4Key != 0) &&
            (prGreedGpioKeyList[i].u4Key != DKBIT_NONE))
        {
            if (prGreedGpioKeyList[i].u4Key == DKBIT_POWER)
            {
                u4Gpio = prGreedGpioKeyList[i].u4GpioIn - OPCTRL(0);
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
    //if(u4IO32Read4B(PDWNC_WDTCTL))
     if (IO32ReadFldAlign(PDWNC_WDTCTL, FLD_WDT0E | FLD_WDT1E | FLD_WDT2E))
    {
        PDWNC_Reboot();
    }
}

#ifndef CC_UP8032_ATV
//-----------------------------------------------------------------------------
/** PDWNC_CreateWatchDogThread() to enable watch dog and set watch dog thread.
 */
//-----------------------------------------------------------------------------
void PDWNC_CreateWatchDogThread(void)
{
#ifndef CC_MTK_LOADER
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
#endif /* CC_MTK_LOADER */
}
#endif

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
#ifndef CC_UP8032_ATV
        UNUSED(DRVCUST_InitQuery(eGpioLedCtrlFunc, (UINT32 *)(void *)&pfnLedCtrlFunc));
#endif
        fgInit = 1;
    }

#ifdef CC_UP8032_ATV
    if (!_fgIrLedEnable && DRVCUST_IrrxGet(eIrrxFlagBlinking))
 #else        
    if (!_fgIrLedEnable && DRVCUST_InitGet(eIrrxFlagBlinking))
#endif
    {
        if (DRVCUST_InitGet(eSupportDarkLed))
        {
#ifdef CC_UP8032_ATV
	if(GPIO_LED_CTRL_FUNC!=NULL)
		GPIO_LED_CTRL_FUNC(eLedDarkLed);
#else        
            pfnLedCtrlFunc(eLedDarkLed);
#endif
        }
        else
        {
#ifdef CC_UP8032_ATV
	if(GPIO_LED_CTRL_FUNC!=NULL)
		GPIO_LED_CTRL_FUNC(eLedIrOff);
#else        
            pfnLedCtrlFunc(eLedIrOff);
#endif
        }
        return;
    }

#ifdef CC_UP8032_ATV
    if (DRVCUST_IrrxGet(eIrrxFlagBlinking))
#else
    if (DRVCUST_InitGet(eIrrxFlagBlinking))
#endif
    {
        HAL_GetTime(&rTime);
        HAL_GetDeltaTime(&rDelta, &rPrevTime, &rTime);

        if ((fgOn) && (!fgPrev) && ((rDelta.u4Seconds > 0) || (rDelta.u4Micros > 50000)))
        {
#ifdef CC_UP8032_ATV
	if(GPIO_LED_CTRL_FUNC!=NULL)
		GPIO_LED_CTRL_FUNC(eLedIrOn);
#else
            pfnLedCtrlFunc(eLedIrOn);
#endif
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
/** PDWNC_IrLedCheck() to check ir blinking option
 */
//-----------------------------------------------------------------------------
void PDWNC_EnableIrLed(BOOL fgEnable)
{
    _fgIrLedEnable = fgEnable;
}
#if 0
void PDWNC_T8032_WAKEUP()
{
	
		UINT32 u4Size = DRVCUST_InitGet(eT8032uPSize);
		UINT32 i,u4IntStatus;
		volatile UINT32* flash_addr = (volatile UINT32*)DRVCUST_InitGet(eT8032uPOffset);
		volatile UINT32* flash_addrPtr1;
		flash_addrPtr1=flash_addr;
		Printf("#define T8032FW_SIZE 0x%x\n",u4Size);
		
		UINT32* pu1Buf = x_mem_alloc(u4Size);
		if(NULL == pu1Buf) 
		{
			Printf("Can't allocate data by x_mem_alloc\n");
			return ;
		}
		volatile UINT32* pu1Ptr = (volatile UINT32*)pu1Buf;
		u4IntStatus = (UINT32)IO_READ32(PDWNC_T8032_INTEN,0);
		IO_WRITE32(PDWNC_T8032_INTEN,0,0);//disable all interrupt
#ifdef CC_MT8223
		Printf("\n--------------8223->-8032------------\n");
		IO_WRITE32(PDWNC_BASE,0x140,7);
#endif
		IO_WRITE32(PDWNC_BASE, 0x178, 0x79);//0xa5);// hold 8032
		vIO32WriteFldAlign(PDWNC_UP_ADDR, 0, FLD_UP_ADDR);
		 for(i = 0; i < u4Size; i+=4)
		{
			vIO32Write4B(PDWNC_UP_DATA, *flash_addrPtr1);
//			if(i%16 == 0)
//				Printf("\n");
//			Printf("0x%08x",*flash_addrPtr1);
			flash_addrPtr1++;
		 }
		for(i = 0; i < u4Size; i+=4)
		{
			*pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
	
	//		if(i%16 == 0)
	//			Printf("\n");
	//		Printf("0x%08x,", (*pu1Ptr));
			
			pu1Ptr++;  
		}	
		
		if(x_memcmp((void*)flash_addr, (void*)pu1Buf, u4Size) != 0)
			{
				Printf("Load T8032 FW fail!!\n");
				//return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
			}
		else
			Printf("Load T8032 FW success\n");
	
	
		IO_WRITE8(PDWNC_BASE, 4, 6); // CLOCK
		IO_WRITE8(PDWNC_BASE, 0x178, 0x7b);//switch 8032 run on srm
	
	
		
		IO_WRITE8(PDWNC_BASE, 0x140, 0x5); //release load code to srm
	
		IO_WRITE8(PDWNC_BASE, 0, 0x80); // CLOCK
	
		
		Printf("Register status:\n");
			Printf("0x28000 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x00)); 	
			Printf("0x28004 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x04));
			Printf("0x28140 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x140));
			Printf("0x28178 = 0x%8x\n",IO_READ32(PDWNC_BASE,0x178));
		BIM_WRITE32(0xe04, 0);//switch uart to t8032
		IO_WRITE32(PDWNC_T8032_INTEN,0,u4IntStatus);//restore all interrupt
		IO_WRITE8(PDWNC_BASE, 0x178, 0x7a);//release reset
	
		//for(i = 0; i < 0xf000000; i++);
		//BIM_WRITE32(0xe04, 0xe2);// switch uart to arm
				//Printf("8032 not run!!!\n");

}
#endif

//-----------------------------------------------------------------------------
/** PDWNC_EnterPowerDown() Enter Power Down mode.
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_EnterPowerDown(UINT32 u4PowerDownEn, UINT32 u4PowerDownEvent)
{
    PFN_GPIO_POWEROFF_FUNC pfnPowerOffFunc = NULL;
    INT32 i4Gpio;
#ifndef CC_MT8223
    UINT32 u4Cnt;
#endif
#ifdef CC_UP8032_ATV
    // GPIO wakeup setup
    __PDWNC_SetupGPIO(&u4PowerDownEn);

    // ServoADC wakeup setup
    __PDWNC_SetupServoADC(&u4PowerDownEn);
#else
    // GPIO wakeup setup
    _PDWNC_SetupGPIO(&u4PowerDownEn);

    // ServoADC wakeup setup
    _PDWNC_SetupServoADC(&u4PowerDownEn);
#endif
    // IRRX wakeup setup
    _PdwncSetupIRRX(u4PowerDownEn);

    // VGA wakeup setup
    _PdwncSetupVGA(u4PowerDownEn);

#ifndef CC_MTK_LOADER
    // RTC wakeup setup
    _PdwncSetupRTC(&u4PowerDownEn);
#endif /* CC_MTK_LOADER */
    
    // HDMI wakeup setup
    _PdwncSetupHDMI(u4PowerDownEn);

    // Do customization 
#ifdef CC_UP8032_ATV
	if(GPIO_POWER_OFF_FUNC!=NULL)
		GPIO_POWER_OFF_FUNC(u4PowerDownEn);
#else
    if (DRVCUST_InitQuery(eGpioPowerOffFunc, (UINT32 *)(void *)&pfnPowerOffFunc) == 0)
    {
        pfnPowerOffFunc(u4PowerDownEn);
    }
#endif
    
    if(DRVCUST_InitGet(eFlagMt8292GpioSupport))
    {
        INT32 i4Val = 0;
        // If customization option does not set 8292 reset, it must be polled by H/W        
        i4Gpio = (INT32)DRVCUST_InitGet(eMt8292ResetGpio);
        if(i4Gpio != -1)//if(i4Gpio != (INT32)0xFFFFFFFF)
        {
            VERIFY(GPIO_Output(i4Gpio, &i4Val) == 0);
        }
        UNUSED(i4Val);
    }
    
    if (DRVCUST_InitGet(ePdwncPolarity))
    {
        vIO32WriteFldAlign(PDWNC_PDMISC, 0x1, FLD_PDWN_POL);
    }
    else
    {
        vIO32WriteFldAlign(PDWNC_PDMISC, 0x0, FLD_PDWN_POL);
    }

    // Setup Power Down
    Printf("Standby                         \n");
    // SerWaitTxBufClear();
#ifdef CC_MT8223

#else
    //for the case of logo standby, T8032 may be hang by CEC driver at this monent...
    if(DRVCUST_InitGet(eT8032uPOffset) != 0)
    {
        u4Cnt = 0;
        while(IS_XDATA_AT_ARM() && (u4Cnt < 3))
        {
            x_thread_delay(100);
            u4Cnt++;
        }
        if(u4Cnt == 3)
        {
            Printf("Warning T8032 is still hang...\n");
        }
    }
#endif
#if 0
		PDWNC_T8032_WAKEUP();
#endif

#ifndef CC_UP8032_ATV
    // Do the external power down for customize setting
    VERIFY(pgrPDWNC_Func);
    VERIFY(pgrPDWNC_Func->pfnPowerDown);
    pgrPDWNC_Func->pfnPowerDown(u4PowerDownEvent, &u4PowerDownEn);
#endif

    // Enable wakeup devices
#ifdef CC_MT8223
		vIO32Write4B(PDWNC_WAKEN,  u4PowerDownEn|WAK_IRRX);
#else
        vIO32Write4B(PDWNC_WAKEN,  u4PowerDownEn);
#endif
    // To wait the completion of pending SIF transaction and stop all subsequent SIF operations
    SIF_WaitLastTransaction(500);
        
    PDWNC_SetupPowerDown();
    
    return 0;
}

//-----------------------------------------------------------------------------
/** PDWNC_EnterPowerState() to enter semi power state (Will not actually power down the system).
 */
//-----------------------------------------------------------------------------
UINT32 PDWNC_EnterPowerState(UINT32 u4PowerDownEvent)
{
    UINT32 u4State, u4NextState;

    // Do the external power down for customize setting
    VERIFY(pgrPDWNC_Func);
    if (pgrPDWNC_Func->pfnGetCurrentState &&
        pgrPDWNC_Func->pfnDetermineNextState)
    {
        pgrPDWNC_Func->pfnGetCurrentState(&u4State);
#ifndef CC_UP8032_ATV
        pgrPDWNC_Func->pfnDetermineNextState(u4State, u4PowerDownEvent, &u4NextState, NULL, NULL);
#endif
        pgrPDWNC_Func->pfnEnterNextState(u4NextState);
    }
    return 0;
}

UINT32 PDWNC_SetPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T rReason)
{
    _grPowerDownReason.u4Reason = rReason.u4Reason;
    _grPowerDownReason.u4Btn = rReason.u4Btn;
    return 0;
}

UINT32 PDWNC_QueryPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T *prReason)
{
    prReason->u4Reason = _grPowerDownReason.u4Reason;
    prReason->u4Btn = _grPowerDownReason.u4Btn;
    return 0;
}

BOOL PDWNC_SetupFpWakeup(BOOL *pfgEnable)
{
    static BOOL fgFpEnable = TRUE;
    if (pfgEnable)
    {
        fgFpEnable = *pfgEnable;
    }
    return fgFpEnable;
}

#ifdef CC_UP8032_ATV 
extern void vDrvIrISR(void);

void PDWNC_Isr(void)
{
	UINT32 isr_vector;

	isr_vector = u4IO32Read4B(PDWNC_INTSTA);
LogS("PDWNC_Isr");	
	if(isr_vector&_PINT(PINTNO_IRRX))
		vDrvIrISR();

	// Still need to clear this non-hook interrupt.
	vIO32Write4B(PDWNC_INTCLR, isr_vector);
}

#endif

