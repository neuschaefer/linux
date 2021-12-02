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

#ifndef ATV_GPIO_H
#define ATV_GPIO_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_gpio.h"
#include "x_pdwnc.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "eeprom_if.h"
#include "x_ldr_env.h"
//calvin 20090618
#include "x_dlm.h"
// end here
#include "ir_if.h"
#include "ATV_LED.h"
#include "nand_if.h"
#include "nor_if.h"
#include "aud_if.h"
#include "pe_if.h"
#include "d_ATV_cust.h"
#include "panel.h"
#ifdef CC_MTK_LOADER
#include "ATV_SMicroUpgrade.h"
#else
#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif
#endif /* CC_MTK_LOADER */

//---------------------------------------------------------------------------
//Customer inlude file here:
//---------------------------------------------------------------------------
#include "sony_drv_inc/rgb_if.h"
#include "sony_drv_inc/sig_bstr_if.h"
#include "sony_drv_inc/temp_setup_if.h"
#include "sony_drv_inc/a_bl_ctrl.h"
#include "sony_drv_inc/utility_if.h"
#include "loader_imghdr.h"
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define SET_SIGNAL_USR_NOTIFY_FUNC (CUSTOMER_SetSignalBoosterUserDefineFunc)//singnal booster add by Hua 0609
#define GPIO_POWER_ON_FUNC      (GPIO_ATVPowerOnFunc)
#define GPIO_POWER_OFF_FUNC     (GPIO_ATVPowerOffFunc)
#define GPIO_LED_CTRL_FUNC      (GPIO_ATVLedCtrlFunc)

#define ATV_IR_LED_BLINK_PERIOD        150 // 150ms

#define GPIO_PCB_TYPE_1_WXGA       0x00
#define GPIO_PCB_TYPE_1_FHD        0x01
#define GPIO_PCB_TYPE_2            0x02
#define GPIO_PCB_TYPE_3            0x03
#ifdef CC_MTK_LOADER
//#define LOADER_UPGRADE_VER_CMP_FUNC (LOADER_ATVUpgradeVerCmpFunc)
#define LOADER_UPGRADE_VER_CMP_FUNC   (NULL)
#define LOADER_UPGRADE_DLM_CB_FUNC (_LdrUpgradeDlmCbFunc)
#define LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC (_LdrUpgradeDlmGetTagVerCbFunc)
#define LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC (_LdrUpgradeDlmCmpTagVerCbFunc)
#endif /* CC_MTK_LOADER */

#ifdef CC_MTK_LOADER
#define  IDX_CUSTOM_SHUTDOWN_RETRY_EN_FLAG   (0x2800)
#define  IDX_CUSTOM_SHUTDOWN_OFFON_CNT          (0x2801)
#define  IDX_CUSTOM_SHUTDOWN_RETRY_LIMIT        (0x2802)
#define  IDX_CUSTOM_SHUTDOWN_DUE_TO_ERROR     (0x2803)
#define  IDX_CUSTOM_ERROR_BLINK_TIMES            (0x2814)
#define  IDX_CUSTOM_SHUTDOWN_CHECK_LEN          (1)
#endif

#define  ATV_MODEL_TYPE_GPIO           (ADIN(2))

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
static void GPIO_ATVPowerOnFunc(void);
static void GPIO_ATVPowerOffFunc(UINT32 u4PowerDownEn);
static void GPIO_ATVLedCtrlFunc(LED_STATUS_T eLedStatus);
void ATV_LEDBlinkCtrl(D_LED_TYPE eLedType);

#ifdef CC_MTK_LOADER
//static INT32 LOADER_ATVUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer);
static void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize);
static void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer);
static INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer);
static void FlashPQGetVersion(CHAR *szCurVer);
#else
static void ATVIRNfyFunc(UINT32 u4BtnState, UINT32 u4Key,
                          IRRX_RAW_DATA_T *prRawData);
#endif /* CC_MTK_LOADER */
static void CUSTOMER_SetSignalBoosterUserDefineFunc(BOOL SetBstOnOffType); //singnal booster add by Hua 0609

#ifndef CC_MTK_LOADER
static void CUSTOMER_Init(void);//for customer initial
#endif
static void FlashAQGetVersion(CHAR *szCurVer);
static BOOL FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer);
static void FlashSysImgGetVersion(CHAR *szCurVer);

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
// Global variables
//-----------------------------------------------------------------------------
#ifdef CC_MTK_LOADER
//static UINT32 _u4ATVLedBlinkTime = 0;
#else
HANDLE_T _hATVIRLedCtrlTimer = NULL;
#endif /* CC_MTK_LOADER */
#if 0
BOOL     _fgLedOn[LED_TOTAL] = {FALSE, FALSE, FALSE};
BOOL     _fgLedBlink[LED_TOTAL] = {FALSE, FALSE, FALSE};
#endif
BOOL _fgLedOn[D_LED_TOTAL + 1] = {FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL _fgLedBlink[D_LED_TOTAL + 1] = {FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL _fgLedAllBlink = FALSE;

UINT32 _u4gPCBType = GPIO_PCB_TYPE_3;

static BOOL flagOntimerOn = FALSE; //by Hua yu 20090717

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

void ATVLEDCtrlFunc(D_LED_TYPE eLedType, UINT32 u4LedState)
{
    INT32 i4Gpio;
    INT32 i4Val;

    switch (eLedType)
    {
    case D_LED_TOTAL:
        if ((D_LED_GENERIC_STATE)u4LedState == D_LED_GENERIC_ON)
        {
            i4Val = ATV_LED_TIMER_ON_VAL;

            _fgLedOn[D_LED_POWER] = TRUE;
            _fgLedOn[D_LED_STANDBY] = TRUE;
            _fgLedOn[D_LED_TIMER] = TRUE;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            _fgLedOn[D_LED_TOTAL] = TRUE;
        }
        else
        {
            i4Val = 1 - ATV_LED_TIMER_ON_VAL;

            _fgLedOn[D_LED_POWER] = FALSE;
            _fgLedOn[D_LED_STANDBY] = FALSE;
            _fgLedOn[D_LED_TIMER] = FALSE;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            _fgLedOn[D_LED_TOTAL] = FALSE;
        }

        VERIFY(GPIO_Output(ATV_GPIO_LED_POWER, &i4Val)== i4Val);
        VERIFY(GPIO_Output(ATV_GPIO_LED_STANDBY, &i4Val)== i4Val);
        VERIFY(GPIO_Output(ATV_GPIO_LED_TIMER_GREEN, &i4Val)== i4Val);
        VERIFY(GPIO_Output(ATV_GPIO_LED_TIMER_AMBER, &i4Val)== i4Val);
        break;

    case D_LED_POWER:
        i4Gpio = ATV_GPIO_LED_POWER;
        if ((D_LED_POWER_STATE)u4LedState == D_LED_POWER_ON)
        {
            _fgLedOn[eLedType] = TRUE;
            i4Val = ATV_LED_POWER_ON_VAL;
        }
        else
        {
            _fgLedOn[eLedType] = FALSE;
            i4Val = (1 - ATV_LED_POWER_ON_VAL);
        }
        VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
        break;

    case D_LED_STANDBY:
        i4Gpio = ATV_GPIO_LED_STANDBY;
        if ((D_LED_STANDBY_STATE)u4LedState == D_LED_STANDBY_ON)
        {
            _fgLedOn[eLedType] = TRUE;
            i4Val = ATV_LED_STANDBY_ON_VAL;
        }
        else
        {
            _fgLedOn[eLedType] = FALSE;
            i4Val = (1 - ATV_LED_STANDBY_ON_VAL);
        }
        VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
        break;

    case D_LED_TIMER:
        switch (u4LedState)
        {
        case D_LED_TIMER_RED://20090617Hua
            i4Gpio = ATV_GPIO_LED_TIMER_GREEN;
            i4Val = 1 - ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = ATV_GPIO_LED_TIMER_AMBER;
            i4Val = ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);

			flagOntimerOn = FALSE; //by Hua yu 20090717
                break;
        case D_LED_TIMER_GREEN :
            i4Gpio = ATV_GPIO_LED_TIMER_GREEN;
            i4Val = ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = ATV_GPIO_LED_TIMER_AMBER;
            i4Val = 1 - ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);

			flagOntimerOn = FALSE; //by Hua yu 20090717
            break;

        case D_LED_TIMER_ORANGE ://20090617Hua
            i4Gpio = ATV_GPIO_LED_TIMER_GREEN;
            i4Val = ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = ATV_GPIO_LED_TIMER_AMBER;
            i4Val = ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);

			flagOntimerOn = TRUE; //by Hua yu 20090717
            break;

        default :
            i4Gpio = ATV_GPIO_LED_TIMER_GREEN;
            i4Val = 1 - ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = ATV_GPIO_LED_TIMER_AMBER;
            i4Val = 1 - ATV_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);

			flagOntimerOn = FALSE; //by Hua yu 20090717
            break;
        }
        break;
    }
}

void ATV_LEDBlinkCtrl(D_LED_TYPE eLedType)
{
    UINT32 u4LedState;

    // Control the LED blinking,
    // LED On : 500 ms & LED off : 500ms
    if (_fgLedOn[eLedType])
    {
        u4LedState = (eLedType == D_LED_TIMER) ?
                     (UINT32)D_LED_TIMER_OFF : (UINT32)D_LED_GENERIC_OFF;
    }
    else
    {
        u4LedState = (eLedType == D_LED_TIMER) ?
                     (UINT32)D_LED_TIMER_ORANGE : (UINT32)D_LED_GENERIC_ON;
    }
    ATVLEDCtrlFunc(eLedType, u4LedState);
}

#ifdef CC_MTK_LOADER
#if 0
static INT32 LOADER_ATVUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer)
{
    INT32 i4Ret;
    i4Ret = strcmp(szCurVer, szUpgVer); // i4Ret = szCurVer - szUpgVer.
    Printf("CurVer:%s UpgVer:%s\n", szCurVer, szUpgVer);
    return (0 > i4Ret) ? 0 : 1;   // 0 is ok to upgrade, upgrade when szCurVer < szUpgVer.
}
#endif
#else
static void ATVIRLEDCtrlFunc(HANDLE_T hHandle, VOID* pTag)
{
    UNUSED(hHandle);
    UNUSED(pTag);

    ATV_LEDBlinkCtrl(D_LED_POWER);
}

static void ATVIRNfyFunc(UINT32 u4BtnState, UINT32 u4Key,
                          IRRX_RAW_DATA_T *prRawData)
{
    UINT32 u4LastCond = IRRX_COND_BTN_UP;

    UNUSED(u4Key);
    UNUSED(prRawData);

    if (u4BtnState == IRRX_COND_BTN_DOWN)
    {
        if (u4LastCond != u4BtnState)
        {
            UNUSED(x_timer_start(_hATVIRLedCtrlTimer, ATV_IR_LED_BLINK_PERIOD,
                                 X_TIMER_FLAG_REPEAT, ATVIRLEDCtrlFunc,
                                 NULL));
        }
    }
    else
    {
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        _fgLedOn[D_LED_POWER] = TRUE;
        x_timer_stop(_hATVIRLedCtrlTimer);
    }

    u4LastCond = u4BtnState;
}
#endif /* CC_MTK_LOADER */

void vGetTagVersion(CHAR* ucTagName, CHAR * szVerStr)
{
    if (x_strcmp((CHAR*)ucTagName, "load") == 0) // loader by luis
    {
        // get version from dram
        LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
        x_strncpy(szVerStr, prLdrEnv->u.rHdr.szVersion, 8);
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0) // firmware by luis
    {       
        FlashSysImgGetVersion(szVerStr);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pqda") == 0) // PQ data by Norman
    {
    }
    else if (x_strcmp((CHAR*)ucTagName, "pnel") == 0) // panel data by Cosh
    {
         x_strcpy(szVerStr, Panel_GetVersion());
    }
    else if (x_strcmp((CHAR*)ucTagName, "aqda") == 0) // AQ data by Andrew
    {
        FlashAQGetVersion(szVerStr);
    }
    else if (x_strcmp((CHAR*)ucTagName, "ueep") == 0) // uP nvm by Cosh
    {
        #ifdef CC_DRV_SUPPORT_SMICRO
        SMICRO_VER rVerInfo;
        if (u4SMicroGetVersion(0, &rVerInfo) == SM_OK)
        {
            x_strcpy(szVerStr, (CHAR*)rVerInfo.au1NVMVer);
        }
        #endif        
    }
    else if (x_strcmp((CHAR*)ucTagName, "uimg") == 0) // uP image by Cosh
    {
        #ifdef CC_DRV_SUPPORT_SMICRO
        SMICRO_VER rVerInfo;
        if (u4SMicroGetVersion(0, &rVerInfo) == SM_OK)
        {
            x_strcpy(szVerStr, (CHAR*)rVerInfo.au1SWVer);
        }
        #endif    
    }
    else if (x_strcmp((CHAR*)ucTagName, "nvrm") == 0) // eeprom by Jifeng
    {
		EEPROM_Read((UINT64)(EEPROM_NVRAM_VERION_OFFSET), (UINT32)szVerStr, EEPROM_NVRAM_VERION_LENGTH);
		szVerStr[8]='\0';
    }
    else if (x_strcmp((CHAR*)ucTagName, "edid") == 0) // EDID by CI
    {
    }
    else
    {
         // default use force update version
    }
}


#ifdef CC_MTK_LOADER
#include "loader_if.h"
static INT32 _LdrUpEepromUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    // download raw data to uP eeprom, pauc_buffer is raw data, ui4_len is data size
    return 0;
}

static INT32 _LdrUpFirmUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    // download raw data to uP firmware, pauc_buffer is raw data, ui4_len is data size
    return 0;
}

static INT32 _LdrEepromUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* pi4_len)
{
    UINT32 u4i;
    UINT32 u4MemLen=0;
    UINT32 u4DataIndex, u4MaskIndex;
    UINT8   au1Buf[16*1024];
    UINT32 u4BufLen;
    INT32   i4Ret;

    if (NULL==pauc_buffer)
    {
        Printf("\n<NVM Upgrade>:pauc_buffer = NULL\n");
        return DLM_OPEN_FILE_FAILED;
    }

    if (NULL==pi4_len)
    {
        Printf("\n<NVM Upgrade>:pi4_len = NULL\n");
        return DLM_OPEN_FILE_FAILED;
    }
        
    for (u4i=0; u4i<4;u4i++)
    {
        Printf("\n<NVM Upgrade>:The Byte  %d is  = %d\n",u4i, pauc_buffer[u4i]);
        u4MemLen =((pauc_buffer[u4i]<<(u4i*8)) + u4MemLen);
    }
        
    u4BufLen = (UINT32) *pi4_len;
    
    Printf("\n<NVM Upgrade>:The Data length is u4MemLen = %d\n",u4MemLen);
    Printf("\n  EEPROM Size is 0x%x \n",EEPROM_GetSize());
    Printf("\n  Buf Length is 0x%x \n",u4BufLen);

    if (u4MemLen > (UINT32)EEPROM_GetSize())
    {
        Printf("\n<NVM Upgrade>:The Buf  Size  ERROR!!!!\n");
        return DLM_OPEN_FILE_FAILED;
    }
    
    if(u4BufLen!=((u4MemLen*2)+4))
    {
        Printf("\n<NVM Upgrade>:The Buf Length=%d  ERROR!!!!\n", u4BufLen);
        return DLM_OPEN_FILE_FAILED;
    }

    EEPROM_Read((UINT64)(0), (UINT32) au1Buf, u4MemLen);

    u4DataIndex =4;
    u4MaskIndex =u4MemLen+4;

    for(u4i=0;u4i<u4MemLen;u4i++)
    {
        if(pauc_buffer[u4i+u4MaskIndex]==0)
        {
            continue;
        }
        
        au1Buf[u4i] =   pauc_buffer[u4DataIndex+u4i];
        Printf("<_LdrEepromUpgrade>: Offset:  0x%2x;  Write Data: 0x%2x\n",u4i,au1Buf[u4i]);
    }

    i4Ret=EEPROM_Write((UINT64)(0), (UINT32) au1Buf, u4MemLen);
    if (i4Ret)
    {
        Printf("EepromFlash write failed return %d\n", i4Ret);
        return 1;
    }
    return DLM_OK;
}

static INT32 _LdrEdidUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
    // download raw data to edid, pauc_buffer is raw data, ui4_len is data size
    return 0;
}

static void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize)
{
    UNUSED(_LdrUpgradeDlmCbFunc);
    x_dlm_reg_append_callback("ueep", u4BufSize, _LdrUpEepromUpgrade);
    x_dlm_reg_append_callback("uimg", u4BufSize, _LdrUpFirmUpgrade);
    x_dlm_reg_append_callback("nvrm", 64*1024,   _LdrEepromUpgrade);
    x_dlm_reg_append_callback("edid", u4BufSize, _LdrEdidUpgrade);
}

// return the current version of corresponding tag
static void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer)
{
    UNUSED(_LdrUpgradeDlmGetTagVerCbFunc);

    if (x_strcmp((CHAR*)ucTagName, "load") == 0) // loader by luis
    {
        // get version from dram
        LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
        x_strncpy(szCurVer, prLdrEnv->u.rHdr.szVersion, 8);
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0) // firmware by luis
    {
        IMAGE_UPGRADE_HEADER_T rImgUpHdr;
        LDR_FlashCopy((UINT8*)&rImgUpHdr, LOADER_MAX_SIZE, sizeof(IMAGE_UPGRADE_HEADER_T));
        x_strncpy(szCurVer, (CHAR*)rImgUpHdr.au1Version, 8);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pqda") == 0) // PQ data by Norman
    {
		FlashPQGetVersion(szCurVer);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pnel") == 0) // panel data by Cosh
    {
         x_strcpy(szCurVer, Panel_GetVersion());
    }
    else if (x_strcmp((CHAR*)ucTagName, "aqda") == 0) // AQ data by Andrew
    {
        FlashAQGetVersion(szCurVer);
    }
    else if (x_strcmp((CHAR*)ucTagName, "ueep") == 0) // uP nvm by Cosh
    {
        #ifdef SMICRO_UPGRADE_H
        SMICRO_VER rVerInfo;
        if (u4SMicroGetVersion(0, &rVerInfo) == SM_OK)
        {
            x_strcpy(szCurVer, (CHAR*)rVerInfo.au1NVMVer);
        }
        #endif
    }
    else if (x_strcmp((CHAR*)ucTagName, "uimg") == 0) // uP image by Cosh
    {
        #ifdef SMICRO_UPGRADE_H
        SMICRO_VER rVerInfo;
        if (u4SMicroGetVersion(0, &rVerInfo) == SM_OK)
        {
            x_strcpy(szCurVer, (CHAR*)rVerInfo.au1SWVer);
        }
       #endif    
    }
    else if (x_strcmp((CHAR*)ucTagName, "nvrm") == 0) // eeprom by Jifeng
    {
		EEPROM_Read((UINT64)(EEPROM_NVRAM_VERION_OFFSET), (UINT32)szCurVer, EEPROM_NVRAM_VERION_LENGTH);
		szCurVer[8]='\0';
    }
    else if (x_strcmp((CHAR*)ucTagName, "edid") == 0) // EDID by CI
    {
    }
    else
    {
         // default use force update version
        x_strcpy(szCurVer, (char*)DRVCUST_InitGet(eLoaderUpgradeDlmForceVer));
    }
}

// compare if corresponding tag could be download or not.
static INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer)
{
    UNUSED(_LdrUpgradeDlmCmpTagVerCbFunc);

    // always allow to update
    if ((x_strcmp(szCurVer, (char*)DRVCUST_InitGet(eLoaderUpgradeDlmForceVer)) == 0) ||
        (x_strcmp(szUpgVer, (char*)DRVCUST_InitGet(eLoaderUpgradeDlmForceVer)) == 0) ||
        (*(UINT32*)szCurVer == 0xffffffff))
    {
        return 1;
    }

    // compare here, return 1 as allow, return 0 as NOT allow.
    if (x_strcmp((CHAR*)ucTagName, "load") == 0) // loader by luis
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0) // firmware by luis
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pqda") == 0) // PQ data by Norman
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "pnel") == 0) // panel data by Cosh
    {
        return 1;
    }
    else if (x_strcmp((CHAR*)ucTagName, "aqda") == 0) // AQ data by Andrew
    {
        return FlashAQCompVersion(szCurVer, szUpgVer);
    }
    else if (x_strcmp((CHAR*)ucTagName, "ueep") == 0) // uP nvm by Cosh
    {
        return 1;
    }
    else if (x_strcmp((CHAR*)ucTagName, "uimg") == 0) // uP image by Cosh
    {
        return 1;
    }
    else if (x_strcmp((CHAR*)ucTagName, "nvrm") == 0) // eeprom by Jifeng
    {
        return (x_strcmp(szUpgVer, szCurVer) >= 0);
    }
    else if (x_strcmp((CHAR*)ucTagName, "edid") == 0) // EDID by CI
    {
        return 1;
    }

    // this is the default rule
    return 1;
}

static void FlashPQGetVersion(CHAR *szCurVer)
{
#if 1
	szCurVer = NULL;
	(void)szCurVer;
#else
	UINT64 u8PqBaseAddr;
	struct tagFileHead aFlashPqFileHead;

    if (DRVCUST_InitGet(eFlagFlashPqEnable))
    {
        // Check if flash PQ address & block size definition.
        if ((((UINT64)DRVCUST_InitGet(eFlashPqBasePartition)<<32) +
            DRVCUST_InitGet(eFlashPqBaseAddress) == 0) ||
            (DRVCUST_InitGet(eFlashPqBlockSize) == 0))
        {
			szCurVer = NULL; // Address error.
        }

        // Get PQ base address.
        u8PqBaseAddr =
            ((UINT64)DRVCUST_InitGet(eFlashPqBasePartition)<<32) +
            DRVCUST_InitGet(eFlashPqBaseAddress) +
            DRVCUST_InitGet(eFlashPqOffset);

        // PQ base address must be 32-bit alignment.
        if ((u8PqBaseAddr % 4) != 0)
        {
			szCurVer = NULL; // Address error.
        }

	    if (DRVCUST_InitGet(eFlashPqUseNandFalsh))
	    {
	        NANDPART_Read(u8PqBaseAddr,
	            (UINT32)&aFlashPqFileHead,
	            (UINT32)(sizeof(struct tagFileHead)));
	    }
	    else
	    {
	        NOR_Read(u8PqBaseAddr,
	            (UINT32)&aFlashPqFileHead,
	            (UINT32)(sizeof(struct tagFileHead)));
	    }

        if (x_strcmp((CHAR *) (aFlashPqFileHead.bID), (CHAR *) FLASHPQ_ID) == 0)
        {
            x_strncpy(szCurVer, (CHAR*)aFlashPqFileHead.bQtyXmlVer, 8);
        }
        else
        {
			szCurVer = NULL;
        }
    }
	else
	{
		szCurVer = NULL; // No flashPQ function.
	}
#endif
}
#endif /* CC_MTK_LOADER */

static void GPIO_ATVLedCtrlFunc(LED_STATUS_T eLedStatus)
{

    switch(eLedStatus)
    {
    case eLedLoaderPowerOn:
        Printf("eLedLoaderPowerOn\n");
        //Power LED ON
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // VGA Standby LED OFF
        ATVLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // TIMER LED OFF
        ATVLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        break;

    case eLedLoaderPowerOff:
        Printf("eLedLoaderPowerOff\n");
        // Power LED OFF
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        break;

    case eLedSystemPowerOn:
        Printf("eLedSystemPowerOn\n");
        // Power LED ON
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // VGA Standby LED OFF
        ATVLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // TIMER LED OFF
        ATVLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        /*Choy Yee start*/
        GPIO_Input(GPIO(201));
        /*Choy Yee end*/
#ifdef CC_CVT
		GPIO_Input(GPIO(24)); //PTSI_D0
		GPIO_Input(GPIO(25)); //PTSI_D1

		GPIO_SetOut(GPIO(28),0); //PTSI_D4   init:L
		GPIO_Input(GPIO(0)); //GPIO0

		GPIO_SetOut(GPIO(27),1); //PTSI_D3  init:H
		GPIO_SetOut(GPIO(22),0); //PTSI_VALID init:L

		GPIO_Input(GPIO(11)); //AOMCLK
		GPIO_Input(GPIO(14)); //AOSDATA0
#endif
        break;

    case eLedSystemPowerOff:
        Printf("eLedSystemPowerOff\n");
        // TIMER LED OFF
        if (!flagOntimerOn ) //by Hua yu 20090717
        {
        ATVLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        }
        // Power LED OFF
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        break;

    case eLedDarkLed:
    case eLedIrOn:
    case eLedIrOff:
        break;

    case eLedVgaStandby:
        Printf("eLedVgaStandby\r\n");
        //VGA Standby LED ON
        ATVLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        break;

#ifdef CC_MTK_LOADER
    case eLedUpgradeStart:
        Printf("eLedUpgradeStart\r\n");
        // Power LED OFF
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        // Standby LED ON
        ATVLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        // Orange LED ON
        ATVLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_ORANGE);

        //Orange LED blinking On
        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = FALSE;
        _fgLedAllBlink = FALSE;
        break;

    case eLedUpgradeBlink:
        Printf("eLedUpgradeBlink\r\n");
        LOADER_ATV_TimerOff();

        // Power LED OFF
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        // Standby LED ON
        ATVLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        // Orange LED ON
        ATVLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_ORANGE);

        //Orange LED blinking On
        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = TRUE;
        _fgLedAllBlink = FALSE;

        LOADER_ATV_TimerOn();
        Printf("Begin USB Upgrade\r\n");
        break;

    case eLedUpgradeFinish:
        Printf("eLedUpgradeFinish\r\n");
        LOADER_ATV_TimerOff();

        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = FALSE;
        _fgLedAllBlink = FALSE;

        // Power LED ON
        ATVLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // Standby LED ON
        ATVLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        // Orange LED ON
        ATVLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_ORANGE);

        Printf("Finish USB Upgrade\r\n");
        break;

    case eLedUpgradeFail:
        Printf("eLedUpgradeFail\r\n");
        LOADER_ATV_TimerOff();
        _fgLedAllBlink = TRUE;
        LOADER_ATV_TimerOn();
        break;

#endif /* CC_MTK_LOADER */

    default:
        break;
    }
}

static void GPIO_ATVPowerOnFunc()
{

    UINT32 i;
    INT32 i4Val;

    Printf("\n<GPIO_ATVPowerOnFunc>: CC_DRAM_STRAP_MODE = %d  \n", CC_DRAM_STRAP_MODE);
    Printf("\n<GPIO_ATVPowerOnFunc>: DEFAULT_DRAM_TYPE = %d  \n", DEFAULT_DRAM_TYPE);
#ifdef CC_MTK_LOADER
    UINT8   au1RecoveryBit[4];
    UINT8   u1ErrorBlinkTimes;
    UINT32  u4BootReason;
    UINT32 polarity;

    // Audio mute circuit mute in loader
    if (DRVCUST_InitQuery(eAudioMuteGpioNum, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(eAudioMuteGpioPolarity, &polarity));
        GPIO_SetOut(i, polarity);
        //GPIO_SetOut(204, 0);
    }

    GPIO_ATVLedCtrlFunc(eLedLoaderPowerOn);

    // release reset of Demod chip
    if (DRVCUST_InitQuery(eDemodResetGpio, &i) == 0)
    {
        GPIO_SetOut(i, 0);
        HAL_Delay_us(20000); //delay 20ms
        GPIO_SetOut(i, 1);
        Printf("Reset Demod GPIO=%x, 0->1\r\n",i);
    }

	//check BootUp Reason, If AC On, Clear IDX_CUSTOM_SHUTDOWN_DUE_TO_ERROR and IDX_CUSTOM_SHUTDOWN_OFFON_CNT
	u4BootReason=PDWNC_ReadWakeupReason();
	if(u4BootReason == PDWNC_WAKE_UP_REASON_AC_POWER)
      {
            //Clear IDX_CUSTOM_SHUTDOWN_DUE_TO_ERROR
		au1RecoveryBit[0] = 0x00;
		EEPROM_Write((UINT64)IDX_CUSTOM_SHUTDOWN_DUE_TO_ERROR,(UINT32)au1RecoveryBit,(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
		//Clear IDX_CUSTOM_SHUTDOWN_OFFON_CNT
		au1RecoveryBit[2] = 0x00;
		EEPROM_Write((UINT64)IDX_CUSTOM_SHUTDOWN_OFFON_CNT,(UINT32)(au1RecoveryBit+2),(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
	}

	//begin to check Recovery Bit here
	EEPROM_Read((UINT64)IDX_CUSTOM_SHUTDOWN_DUE_TO_ERROR,(UINT32)au1RecoveryBit,(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
	EEPROM_Read((UINT64)IDX_CUSTOM_ERROR_BLINK_TIMES,(UINT32)(au1RecoveryBit+3),(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
      u1ErrorBlinkTimes = au1RecoveryBit[3];
	if(au1RecoveryBit[0]==0)
      {
		//Normal TV turn on
      }
	else
	{
		EEPROM_Read((UINT64)IDX_CUSTOM_SHUTDOWN_RETRY_EN_FLAG,(UINT32)(au1RecoveryBit+1),(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
		if(au1RecoveryBit[1])
		{
			EEPROM_Read((UINT64)IDX_CUSTOM_SHUTDOWN_OFFON_CNT,(UINT32)(au1RecoveryBit+2),(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
			 if(au1RecoveryBit[2]>=2)
			 {
			        //Set LED Blinking
			      GPIO_ATVLedCtrlFunc(eLedLoaderPowerOff);
			 	  PDWNC_SetSonyErrorCode(10,60);
			 	  PDWNC_SetSonyErrorCodeBlinking(u1ErrorBlinkTimes);
			         //Enter Standby
			        PDWNC_EnterPowerDown(WAK_IRRX,PDWNC_POWER_DOWN_UNKNOWN);//go to error shutdown
		       }
			 else
			 {
				 au1RecoveryBit[2]++;
				 EEPROM_Write((UINT64)IDX_CUSTOM_SHUTDOWN_OFFON_CNT,(UINT32)(au1RecoveryBit+2),(UINT32)IDX_CUSTOM_SHUTDOWN_CHECK_LEN);
				  //Normal TV turn on

			 }

		}
		else
		{
			//Set LED Blinking
			GPIO_ATVLedCtrlFunc(eLedLoaderPowerOff);
			PDWNC_SetSonyErrorCode(10,60);
			PDWNC_SetSonyErrorCodeBlinking(u1ErrorBlinkTimes);
			//Enter Standby
			PDWNC_EnterPowerDown(WAK_IRRX,PDWNC_POWER_DOWN_UNKNOWN);//go to error shutdown
		}


	}
	//End check Recovery Bit here
#else /* CC_MTK_LOADER */
    GPIO_ATVLedCtrlFunc(eLedSystemPowerOn);
#ifdef CC_DRV_SUPPORT_SMICRO
		VERIFY(BSP_PinSet(U1RX, 0) == OSR_OK);
#endif
    if (x_timer_create(&_hATVIRLedCtrlTimer) != OSR_OK)
        {
        LOG(5, "[GPIO_ATVPowerOnFunc] Create IR LED Blink timer fail !\n");
        return;
        }

    UNUSED(IRRC_RegCallbackFunc(ATVIRNfyFunc));
    CUSTOMER_Init();
#endif /* CC_MTK_LOADER */

    INIT_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);

}

static void GPIO_ATVPowerOffFunc(UINT32 u4PowerDownEn)
{
    UINT32 i;
    INT32 i4Val;
    //PDWNC_T8032_CMD_T rCmd;  //by Hua yu 20090717

    if (u4PowerDownEn /*& WAK_VGA  marked by Hua 20090623*/)
    {
        GPIO_ATVLedCtrlFunc(eLedVgaStandby);
#if 0//by Hua yu 20090717
        // Let uP to turn on VGA standby LED upon power down
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_POWER_DOWN_CONTROL;
        rCmd.u1SubCmd = PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_ENABLE;
        rCmd.au1Data[0] = (ATV_GPIO_LED_STANDBY-ADIN(0));
        rCmd.au1Data[1] = 1;
        PDWNC_T8032Cmd(&rCmd, NULL);
        printf("\nrCmd.au1Data[0]:%d \n",(ATV_GPIO_LED_STANDBY-ADIN(0)));
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_POWER_DOWN_CONTROL;
        rCmd.u1SubCmd = PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_OUTPUT;
        rCmd.au1Data[0] = (ATV_GPIO_LED_STANDBY-ADIN(0));
        rCmd.au1Data[1] = 1;
        PDWNC_T8032Cmd(&rCmd, NULL);

    }
    else
    {
        // Tell uP to disable VGA standby LED upon power down
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_CLEAR_POWER_DOWN_CONTROL;
        rCmd.u1SubCmd = 0;
        PDWNC_T8032Cmd(&rCmd, NULL);
#endif
    }


    if (!DRVCUST_InitGet(eSupportDarkLed))
    {

#ifdef CC_MTK_LOADER
            GPIO_ATVLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_MTK_LOADER */
            GPIO_ATVLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_MTK_LOADER */
    }

    DOWN_SET_GPIO(eBackLightGpio, eBackLightOnVal);
    DOWN_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);
    DOWN_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
}

static void FlashAQGetVersion(CHAR *szCurVer)
{
    UINT8 u1AQPartition;
    UINT32 u2AQSize;
    UINT64 u8Offset;

    UNUSED(FlashAQGetVersion);
    UNUSED(u2AQSize);
    UNUSED(u1AQPartition);
    UNUSED(u8Offset);

#if 0
    szCurVer = NULL;
	(void)szCurVer;
#else
    u1AQPartition = (UINT8)DRVCUST_InitGet(eAudioFlashAQPartition);
    if (u1AQPartition == 0xFF)
    {
        szCurVer[0] ='\0';
        //LOG(3,"AudioFlashAQPartition is not defined !!\n");
        return;
    }
    //u8Offset = ((UINT64)u1AQPartition << 32) | ((UINT64)FLASH_AQ_VER_OFFSET);

#ifdef CC_MTK_LOADER
    Loader_ReadNandFlash(u1AQPartition, FLASH_AQ_VER_OFFSET, (VOID*)szCurVer, FLASH_AQ_VER_LENGTH);
    //NANDPART_Read(u8Offset, (UINT32)szCurVer, FLASH_AQ_VER_LENGTH);
#else
    STORG_SyncNandRead(u1AQPartition,FLASH_AQ_VER_OFFSET,(VOID*)szCurVer,FLASH_AQ_VER_LENGTH);
#endif //CC_MTK_LOADER

#endif // if 1

}

static BOOL FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer)
{
    UNUSED(FlashAQCompVersion);
    if (x_strcmp(szUpgVer,szCurVer) >= 0)
        return TRUE;
    else
        return FALSE;
}

static void FlashSysImgGetVersion(CHAR *szCurVer)
{
    UINT8                   u1SysImgPartition;
    UINT64                  u8Offset;
    IMAGE_UPGRADE_HEADER_T  rImgUpHdr;
    

    u1SysImgPartition = (UINT8)DRVCUST_InitGet(eNANDFlashPartIdSysImgA);
    if (u1SysImgPartition == 0xFF)
    {
        szCurVer[0] ='\0';
        return;
    }
    
    u8Offset = ((UINT64)u1SysImgPartition << 32);
    NANDPART_Read(u8Offset, (UINT32)&rImgUpHdr, sizeof(IMAGE_UPGRADE_HEADER_T));
    x_strncpy(szCurVer, (CHAR*)rImgUpHdr.au1Version, 8);
}

/***************************************************************************/
//customer customization area
/***************************************************************************/
//1.Singnal Booster add by Hua 20090609
static void CUSTOMER_SetSignalBoosterUserDefineFunc(BOOL SetBstOnOffType)
{
#ifndef CC_MTK_LOADER
    sig_bstr_calc(SetBstOnOffType);
#endif
}

#ifndef CC_MTK_LOADER
//2.Customer initial function add by Hua 20090609
static void CUSTOMER_Init(void)
{
    /*regist your ir event callback*/
    /* EEP Map Check */
    EEP_map_check();
    /* RGB Init */
    RGB_Init();
    /* Temp Sensor Init */
    TEMP_SENSOR_Init();
    /* your device initial function here*/
    //Am_ligtht_sensor_dev_init();
    Dyn_BacklightCtrl_Init();
    SIG_BSTR_Init();
}
#endif


//End here

#else
#error "reinclude ATV_gpio.h..."
#endif  /* ATV_GPIO_H*/

