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



#include "mttype.h"
#include "ice.h"
#include "ice_hardware.h"
#include "ice_ioctl.h"
#include "ice_remote_control.h"
#include "hw_vdoin.h"
#include "hw_pdwnc.h"
#include "drvcust_if.h"
#include "eeprom_if.h"
#include "util.h"
//#include "X_pdwnc.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_2AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_2AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_3AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_4AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_4AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_8AR(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(ICE_IOCTL_8ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return -1;                                          \
    }                                                       \

#define COPY_FROM_USER_8AR(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(ICE_IOCTL_8ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_AR(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define COPY_TO_USER_AR(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_mtvdec argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
    
#define COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)              \
        if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,    \
                           size))          \
        {                                                       \
            printk("mtal_ioctl_ci argument error8\n");      \
            return MTR_ERR_INV;                                          \
        }

#define COPY_TO_USER_ARG_K(ArgUsr, ArgKernel,size)              \
        if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                           size))          \
        {                                                       \
            printk("mtal_ioctl_ci argument error7\n");      \
            return MTR_ERR_INV;                                          \
        }  

#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
        if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                       size))              \
        {                                                       \
            printk("mtal_ioctl_ci argument error6\n");     \
            return MTR_ERR_INV;                                          \
        } 

/*
 i: LED number; 1--LED1, 2--LED2; >=2 not defined.
*/
extern void PDWNC_SetT8032LEDBlinkOn(UINT8 i);
extern void PDWNC_SetT8032LEDBlinkOff(UINT8 i);
extern void PDWNC_SetT8032LEDTurnOn(UINT8 i);
extern void PDWNC_SetT8032LEDTurnOff(UINT8 i);
extern void PDWNC_GetT8032LEDStutus(UINT8 i, UINT8 * bTimer);
extern UINT8 bTestMode;
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
extern UINT8 bEnableWatchDog[3];
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------
extern void PDWNC_TouchPadTableResume(void);
extern void PDWNC_MTK_WatchdogEnable(UINT32 enable);
extern void PDWNC_MTK_SetWatchdogTimeout(UINT32 timeout);
extern void _BTN_IsSystemCodeAllowed(UINT32 u4System);
extern UINT8 bPDWNC_InstallStandby(UINT8 bLoaderStandbyOrNot);
extern UINT32 PDWNC_MTK_GetWatchdogTimeout(void);
extern void PDWNC_MTK_SetStandbyLed(UINT32 mode);
extern 	void PDWNC_MTK_ActiveStandby(UINT32 active);
extern void WritePanelIndexToEeprom(UINT32 u4PanelIndex);



static MT_RESULT_T _ICE_HardWare_LockedPanel(unsigned long arg)
{
    ICE_IOCTL_2ARG_T    rArg;
    UINT32 LockedPanelFlag;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);

    LockedPanelFlag = (UINT32)rArg.ai4Arg[0];
    _BTN_IsSystemCodeAllowed(LockedPanelFlag);
    PDWNC_TouchPadTableResume();
    return MTR_OK;
}



static MT_RESULT_T _ICE_HardWare_WatchdogEnable(unsigned long arg)
{
    ICE_IOCTL_2ARG_T    rArg;
    UINT32 WatchdogEnable;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);
    WatchdogEnable = rArg.ai4Arg[0];
    
    PDWNC_MTK_WatchdogEnable(WatchdogEnable);


    return MTR_OK;
}
static MT_RESULT_T _ICE_HardWare_SetWatchdogTimeout(unsigned long arg)
{

    ICE_IOCTL_2ARG_T    rArg;
    UINT32 WatchdogTimeout;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);
    
    WatchdogTimeout = (UINT32)rArg.ai4Arg[0];
    PDWNC_MTK_SetWatchdogTimeout(WatchdogTimeout);
    return MTR_OK;
}


static MT_RESULT_T _ICE_HardWare_GetWatchdogTimeout(unsigned long arg)
{

    UINT32   * rArg;
    UINT32 WatchdogTimeout;

    WatchdogTimeout = PDWNC_MTK_GetWatchdogTimeout();

    rArg=(UINT32*)arg;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(rArg,UINT32);
    COPY_TO_USER_ARG(rArg,WatchdogTimeout,UINT32);

    return MTR_OK;
}

static MT_RESULT_T _ICE_HardWare_Led(unsigned long arg)
{
    
        ICE_IOCTL_2ARG_T    rArg;
        frost_uint number;
        frost_uint state;
        static UINT32 u4CntRecord[4] = {0, 0, 0, 0};
        UINT8 bTimer = 0;
    
        USR_SPACE_ACCESS_VALIDATE_2AR(arg);
        COPY_FROM_USER_2AR(arg, rArg);
    
        number = (frost_uint)rArg.ai4Arg[0];
        state = (frost_uint)rArg.ai4Arg[1];
    //    PDWNC_MTK_HardWare_Led(number,state);
        printf("number=%d, state=%d\n", number, state);
#if 0//east.yan20120119.
        if(number == 0x11)//for blink
        {
            if(state != 0)
            {
                PDWNC_SetT8032LEDBlinkOn(1);
            }
            else
            {
                PDWNC_SetT8032LEDBlinkOff(1);
            }
        }
#else
        switch (state)
        {
             case 0://blink off
                u4CntRecord[0] ++;
                PDWNC_SetT8032LEDBlinkOff(number);
                if(u4CntRecord[0] > 1)
                {
                    PDWNC_GetT8032LEDStutus(number, &bTimer);
                    vUtDelay10ms(2*(bTimer));
                }
                PDWNC_SetT8032LEDTurnOff(number);
                break;
                
             case 1://blink on.
                u4CntRecord[1] ++;
                PDWNC_SetT8032LEDBlinkOn(number);                
                break;
    
             case 2://Off
                u4CntRecord[2] ++;
                PDWNC_SetT8032LEDBlinkOff(number);
                if(u4CntRecord[2] > 1)
                {
                    PDWNC_GetT8032LEDStutus(number, &bTimer);
                    vUtDelay10ms(2*(bTimer));
                }
                PDWNC_SetT8032LEDTurnOff(number);
                break;
    
             case 3://On
                u4CntRecord[3] ++;
                PDWNC_SetT8032LEDBlinkOff(number);
                if(u4CntRecord[3] > 1)
                {
                    PDWNC_GetT8032LEDStutus(number, &bTimer);
                    vUtDelay10ms(2*(bTimer));
                }
                PDWNC_SetT8032LEDTurnOn(number);
                break;
    
             default:
                break;
        }
#endif
        
#if 0
        if(number == 0)//for led on/off
        {
            if(state != 0)
            {
                PDWNC_SetT8032LEDTurnOn(1);
            }
            else
            {
                PDWNC_SetT8032LEDTurnOff(1);
            }
        }
#endif
        return MTR_OK;
}

static MT_RESULT_T _ICE_HardWare_SetStandbyLed(unsigned long arg)
{

    ICE_IOCTL_2ARG_T    rArg;
    UINT32 mode;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);
    
    mode = (frost_uint)rArg.ai4Arg[0];
    PDWNC_MTK_SetStandbyLed(mode);

    return MTR_OK;
}

static MT_RESULT_T _ICE_HardWare_ActiveStandby(unsigned long arg)
{

    ICE_IOCTL_2ARG_T  rArg;
    UINT32 active;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);
    
    active = (frost_uint)rArg.ai4Arg[0];
    PDWNC_MTK_ActiveStandby(active);

    return MTR_OK;
}

//extern void PDWNC_Reboot();

static MT_RESULT_T _ICE_HardWare_PDWNC_Reboot(unsigned long arg)
{

    ICE_IOCTL_2ARG_T  rArg;
    UINT32 active;

    USR_SPACE_ACCESS_VALIDATE_2AR(arg);
    COPY_FROM_USER_2AR(arg, rArg);
    
    active = (frost_uint)rArg.ai4Arg[0];
    
    printf("\n driver call reboottttttttttttttt \n");
    PDWNC_Reboot();
    return MTR_OK;
}

MT_RESULT_T _ICE_LowPowerSetPowerUpMode(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pMode = (UINT8 *)arg;
    UINT8 bMode = 0;
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pMode, 1);
    COPY_FROM_USER_ARG_K(pMode, bMode, 1);
    printf("_ICE_LowPowerSetPowerUpMode: %d", bMode);

    bPDWNC_InstallStandby(bMode);
    return i4Ret;
}

MT_RESULT_T _ICE_HardWareWatchDog(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT32 * pu4Count = (UINT32 *)arg;
    UINT32 u4Count;


    USR_SPACE_ACCESS_VALIDATE_ARG_K(pu4Count, 4);
    COPY_FROM_USER_ARG_K(pu4Count, u4Count, 4);
    
    vIO32Write4B(PDWNC_WDT2,u4Count);
    if(bEnableWatchDog[2] == 0)
    {
        vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x0000ffff));
    }
    else
    {
        vIO32Write4B(PDWNC_WDTCTL, (u4IO32Read4B(PDWNC_WDTCTL) & 0x00ffffff) |0xff0000);
    }
    return i4Ret;
}


MT_RESULT_T _ICE_HardwareWatchDogEnableDisable(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 *pbEnableWatchDog = (UINT32 *)arg;
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pbEnableWatchDog, 3);
    COPY_FROM_USER_ARG_K(pbEnableWatchDog, bEnableWatchDog, 3);
    
    printf("bEnableWatchDog[0]=%d,bEnableWatchDog[1]=%d,,bEnableWatchDog[2]=%d...\n", \
                    bEnableWatchDog[0], bEnableWatchDog[1],bEnableWatchDog[2]);
    return i4Ret;
}



static MT_RESULT_T _ICE_HardWareInit(unsigned long arg)
{
    static UINT8 bIsHardwareInited = 0;
    UINT32 dPanelIndexOffset = 0;
    UINT8 bPanelIndex = 0;
    INT32 dReadResult = 0;
    MT_RESULT_T i4Ret = MTR_OK;
    
    if(bIsHardwareInited == 1)
    {
        return MTR_OK;
    }
    bIsHardwareInited = 1;
    DRVCUST_PanelQuery(ePanelIndexOffset1, &dPanelIndexOffset);
    dReadResult = EEPROM_Read((UINT64)dPanelIndexOffset, (UINT32)(&bPanelIndex), 1);
    if(1 == dReadResult)
    {
        i4Ret = MTR_NOT_OK;
    }
    if(bPanelIndex == 3)
    {
        bTestMode = 1;
    }
    else
    {
        bTestMode = 0;
    }
    WritePanelIndexToEeprom(DRVCUST_PanelGet(eDefaultPanelSelect));
    return MTR_OK;
}

static MT_RESULT_T _ICE_ScartWakeUpConfig(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pEnabelScartWakeUp = (UINT8 *)arg;
    UINT8 bEnabelScartWakeUp = 0;
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pEnabelScartWakeUp, 1);
    COPY_FROM_USER_ARG_K(pEnabelScartWakeUp, bEnabelScartWakeUp, 1);
    printf("_ICE_ScartWakeUpConfig: %d", bEnabelScartWakeUp);

    PDWNC_ConfigScartWakeUp(bEnabelScartWakeUp);
    return i4Ret;

}

static MT_RESULT_T _ICE_VgaWakeUpConfig(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pProfileVgaWakeupSetting = (UINT8 *)arg;
    UINT8 bProfileVgaWakeupSetting = 0;
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pProfileVgaWakeupSetting, 1);
    COPY_FROM_USER_ARG_K(pProfileVgaWakeupSetting, bProfileVgaWakeupSetting, 1);

    printf("_ICE_VgaWakeUpConfig profilesetting:%d\n", bProfileVgaWakeupSetting);

    PDWNC_ConfigVgaWakeUp(bProfileVgaWakeupSetting);
    return i4Ret;

}


static MT_RESULT_T _ICE_BootLogoConfig(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pProfileBootLogoSetting = (UINT8 *)arg;
    UINT8 bProfileBootLogoSetting = 0;
    int i;
    UINT32 u4BootLogoSettingOffset = 0;
    UINT8 bBootLogoSettingOld = 1;

    USR_SPACE_ACCESS_VALIDATE_ARG_K(pProfileBootLogoSetting, 1);
    COPY_FROM_USER_ARG_K(pProfileBootLogoSetting, bProfileBootLogoSetting, 1);

    printf("_ICE_VgaWakeUpConfig profilesetting:%d\n", bProfileBootLogoSetting);


    i = DRVCUST_InitQuery(eBootLogoSettingOffset, (UINT32 *)(void *)&u4BootLogoSettingOffset);
    if(1 ==EEPROM_Read(u4BootLogoSettingOffset, (UINT32)&bBootLogoSettingOld, 1))
    {
        printf("_ICE_BootLogoConfig: eep error#1\n");
        return; 
    }
    if(bBootLogoSettingOld != bProfileBootLogoSetting)
    {
        if(1 ==EEPROM_Write(u4BootLogoSettingOffset, (UINT32)&bProfileBootLogoSetting, 1))
        {
            printf("_ICE_BootLogoConfig: eep error#2\n");
            return; 
        } 
    }


    return i4Ret;

}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_hardware(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
        case ICE_IO_HARDWARE_LOCKEDFANEL:
            i4Ret = _ICE_HardWare_LockedPanel(arg);  
            break;
        case ICE_IO_HARDWARE_WATCHDOGENABLE:
            i4Ret = _ICE_HardWare_WatchdogEnable(arg);  
            break;
        case ICE_IO_HARDWARE_SETTIMEOUT:
            i4Ret = _ICE_HardWare_SetWatchdogTimeout(arg);  
            break;
        case ICE_IO_HARDWARE_GETTIMEOUT:
            i4Ret = _ICE_HardWare_GetWatchdogTimeout(arg);  
            break;
        case ICE_IO_HARDWARE_LED:
            i4Ret = _ICE_HardWare_Led(arg);  
            break;    
        case ICE_IO_HARDWARE_SETSTANDBYLED:
            i4Ret = _ICE_HardWare_SetStandbyLed(arg);  
            break;    
        case ICE_IO_HARDWARE_ACTIVE:
            i4Ret = _ICE_HardWare_ActiveStandby(arg);  
            break;    
        case ICE_IO_HARDWARE_PDWNREBOOT:
            i4Ret = _ICE_HardWare_PDWNC_Reboot(arg);  
            break;    
        case ICE_IO_HARDWARE_LOWPOWERSTANBYMODE:
            i4Ret = _ICE_LowPowerSetPowerUpMode(arg);
            break;
        case ICE_IO_HARDWARE_WATCHDOG:
            i4Ret = _ICE_HardWareWatchDog(arg);
            break;
        case ICE_IO_HARDWARE_INIT:
            i4Ret = _ICE_HardWareInit(arg);
            break;
        case ICE_IO_HARDWARE_EATCHDOG_ENABLE_DISABLE:
            i4Ret = _ICE_HardwareWatchDogEnableDisable(arg);
            break;
        case ICE_IO_HARDWARE_SCARTWAKEUP_CONFIG:
            i4Ret = _ICE_ScartWakeUpConfig(arg);
            break;
        case ICE_IO_HARDWARE_VGAWAKEUP_CONFIG:
            i4Ret = _ICE_VgaWakeUpConfig(arg);
            break;
        case ICE_IO_HARDWARE_BOOTLOGO_CONFIG:
            i4Ret = _ICE_BootLogoConfig(arg);
            break;
        default:
            break;
    }
    return i4Ret;
}

