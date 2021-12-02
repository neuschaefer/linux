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

#ifndef IRIS_GPIO_H
#define IRIS_GPIO_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_gpio.h"
#include "x_pdwnc.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "eeprom_if.h"
#include "x_ldr_env.h"
#include "ir_if.h"
#include "nand_if.h"
#include "nor_if.h"
#include "Iris_LED.h"
#include "d_iris_cust.h"
#include "pe_if.h"
#include "aud_if.h"
#include "panel.h"
#ifdef CC_MTK_LOADER
#include "Iris_SMicroUpgrade.h"
#else
#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif
#endif /* CC_MTK_LOADER */

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define GPIO_POWER_ON_FUNC              (GPIO_IrisPowerOnFunc)
#define GPIO_POWER_OFF_FUNC             (GPIO_IrisPowerOffFunc)
#define GPIO_LED_CTRL_FUNC              (GPIO_IrisLedCtrlFunc)

#define IRIS_IR_LED_BLINK_PERIOD        150 // 150ms

// Strap GPIO definitaion on PCB
#define IRIS_GPIO_PANEL_PIN             GPIO(5)     // WXGA / FHD
#define IRIS_GPIO_PCB_PIN_1             GPIO(28)    // PTSI_D4
#define IRIS_GPIO_PCB_PIN_2             GPIO(29)    // PTSI_D5
#define IRIS_GPIO_FACTORY_PIN           GPIO(22)    // PTSI_VALID
#define IRIS_GPIO_PANEL_WXGA            0
#define IRIS_GPIO_PANEL_FHD             1
#define GPIO_PCB_TYPE_1_WXGA       0x00
#define GPIO_PCB_TYPE_1_FHD        0x01
#define GPIO_PCB_TYPE_2            0x02
#define GPIO_PCB_TYPE_3            0x03
#define IRIS_GPIO_FACTORY_SONY          0
#define IRIS_GPIO_FACTORY_FOXCONN       1

#ifdef CC_MTK_LOADER
//#define LOADER_UPGRADE_VER_CMP_FUNC   (LOADER_IrisUpgradeVerCmpFunc)
#define LOADER_UPGRADE_VER_CMP_FUNC     (NULL)
#define LOADER_UPGRADE_DLM_CB_FUNC (_LdrUpgradeDlmCbFunc)
#define LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC (_LdrUpgradeDlmGetTagVerCbFunc)
#define LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC (_LdrUpgradeDlmCmpTagVerCbFunc)
#endif /* CC_MTK_LOADER */

#define EEPROM_EDID_VERION_OFFSET	0x38B //(DRVCUST_EEPROM_DRIVER_SIZE - 21) last (14~21) is edid version
//-----------------------------------------------------------------------------
// Prototypes 
//-----------------------------------------------------------------------------
static void GPIO_IrisPowerOnFunc(void);
static void GPIO_IrisPowerOffFunc(UINT32 u4PowerDownEn);
static void GPIO_IrisLedCtrlFunc(LED_STATUS_T eLedStatus);
void Iris_LEDBlinkCtrl(D_LED_TYPE eLedType);

#ifdef CC_MTK_LOADER
//static INT32 LOADER_IrisUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer);
static void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize);
static void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer);
static INT32 _LdrUpgradeDlmCmpTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer, CHAR *szUpgVer);
static void FlashPQGetVersion(CHAR *szCurVer);
static BOOL FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer);
#else
static void IrisIRNfyFunc(UINT32 u4BtnState, UINT32 u4Key, 
                          IRRX_RAW_DATA_T *prRawData);
#endif /* CC_MTK_LOADER */

static void FlashAQGetVersion(CHAR *szCurVer);

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
//static UINT32 _u4IrisLedBlinkTime = 0;
#else
HANDLE_T _hIrisIRLedCtrlTimer = NULL;
#endif /* CC_MTK_LOADER */
BOOL _fgLedOn[D_LED_TOTAL + 1] = {FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL _fgLedBlink[D_LED_TOTAL + 1] = {FALSE, FALSE, FALSE, FALSE, FALSE};
BOOL _fgLedAllBlink = FALSE;
BOOL _fgLedStandbyDiable = FALSE;
UINT32 _u4gPanelType = IRIS_GPIO_PANEL_WXGA;
UINT32 _u4gPCBType = GPIO_PCB_TYPE_2;
UINT32 _u4gFactoryType = IRIS_GPIO_FACTORY_FOXCONN;
UINT32 _u4gIrisModelType = 0;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

void IrisLEDCtrlFunc(D_LED_TYPE eLedType, UINT32 u4LedState)
{
    INT32 i4Gpio;
    INT32 i4Val;

    switch (eLedType)
    {
    case D_LED_TOTAL:
        if ((D_LED_GENERIC_STATE)u4LedState == D_LED_GENERIC_ON)
        {
            i4Val = IRIS_LED_TIMER_ON_VAL;
            
            _fgLedOn[D_LED_POWER] = TRUE;
            _fgLedOn[D_LED_STANDBY] = TRUE;
            _fgLedOn[D_LED_TIMER] = TRUE;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            _fgLedOn[D_LED_TOTAL] = TRUE;
        }
        else
        {
            i4Val = 1 - IRIS_LED_TIMER_ON_VAL;
            
            _fgLedOn[D_LED_POWER] = FALSE;
            _fgLedOn[D_LED_STANDBY] = FALSE;
            _fgLedOn[D_LED_TIMER] = FALSE;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            _fgLedOn[D_LED_TOTAL] = FALSE;
        }
        
        VERIFY(GPIO_Output(IRIS_GPIO_LED_POWER, &i4Val)== i4Val);
        VERIFY(GPIO_Output(IRIS_GPIO_LED_STANDBY, &i4Val)== i4Val);
        VERIFY(GPIO_Output(IRIS_GPIO_LED_TIMER_GREEN, &i4Val)== i4Val);
        VERIFY(GPIO_Output(IRIS_GPIO_LED_TIMER_AMBER, &i4Val)== i4Val);
        break;

    case D_LED_POWER:
        i4Gpio = IRIS_GPIO_LED_POWER;
        if ((D_LED_POWER_STATE)u4LedState == D_LED_POWER_ON)
        {
            _fgLedOn[eLedType] = TRUE;
            i4Val = IRIS_LED_POWER_ON_VAL;
        }
        else
        {
            _fgLedOn[eLedType] = FALSE;
            i4Val = (1 - IRIS_LED_POWER_ON_VAL);
        }
        VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
        break;
        
    case D_LED_STANDBY:
        i4Gpio = IRIS_GPIO_LED_STANDBY;
        if ((D_LED_STANDBY_STATE)u4LedState == D_LED_STANDBY_ON)
        {
            _fgLedOn[eLedType] = TRUE;
            i4Val = IRIS_LED_STANDBY_ON_VAL;
        }
        else
        {
            _fgLedOn[eLedType] = FALSE;
            i4Val = (1 - IRIS_LED_STANDBY_ON_VAL);
        }
        VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
        break;
        
    case D_LED_TIMER:
        switch (u4LedState)
        {
        case D_LED_TIMER_RED:
            i4Gpio = IRIS_GPIO_LED_TIMER_GREEN;
            i4Val = 1 - IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = IRIS_GPIO_LED_TIMER_AMBER;
            i4Val = IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;
            
        case D_LED_TIMER_GREEN :
            i4Gpio = IRIS_GPIO_LED_TIMER_GREEN;
            i4Val = IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = IRIS_GPIO_LED_TIMER_AMBER;
            i4Val = 1 - IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;
            
        case D_LED_TIMER_ORANGE :
            i4Gpio = IRIS_GPIO_LED_TIMER_GREEN;
            i4Val = IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = IRIS_GPIO_LED_TIMER_AMBER;
            i4Val = IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = TRUE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;
            
        default :
            i4Gpio = IRIS_GPIO_LED_TIMER_GREEN;
            i4Val = 1 - IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER_2] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            i4Gpio = IRIS_GPIO_LED_TIMER_AMBER;
            i4Val = 1 - IRIS_LED_TIMER_ON_VAL;
            _fgLedOn[D_LED_TIMER] = FALSE;
            VERIFY(GPIO_Output(i4Gpio, &i4Val)== i4Val);
            break;
        }
        break;
    }
}

void Iris_DisableStandbyLEDCtrl(UINT8 u1Disable)
{

    // Disable standby LED control
    _fgLedStandbyDiable = (BOOL)(u1Disable);
}

void Iris_LEDBlinkCtrl(D_LED_TYPE eLedType)
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
    IrisLEDCtrlFunc(eLedType, u4LedState);
}

#ifdef CC_MTK_LOADER
#if 0
static INT32 LOADER_IrisUpgradeVerCmpFunc(CHAR *szCurVer, CHAR *szUpgVer)
{
    INT32 i4Ret;
    i4Ret = strcmp(szCurVer, szUpgVer); // i4Ret = szCurVer - szUpgVer.
    Printf("CurVer:%s UpgVer:%s\n", szCurVer, szUpgVer);
    return (0 > i4Ret) ? 0 : 1;   // 0 is ok to upgrade, upgrade when szCurVer < szUpgVer.
}
#endif
#else
static void IrisIRLEDCtrlFunc(HANDLE_T hHandle, VOID* pTag)
{
    UNUSED(hHandle);
    UNUSED(pTag);
    
    Iris_LEDBlinkCtrl(D_LED_POWER);
}

static void IrisIRNfyFunc(UINT32 u4BtnState, UINT32 u4Key, 
                          IRRX_RAW_DATA_T *prRawData)
{
    UINT32 u4LastCond = IRRX_COND_BTN_UP;

    UNUSED(u4Key);
    UNUSED(prRawData);
    
    if (u4BtnState == IRRX_COND_BTN_DOWN)
    {
        if (u4LastCond != u4BtnState)
        {
            UNUSED(x_timer_start(_hIrisIRLedCtrlTimer, IRIS_IR_LED_BLINK_PERIOD,
                                 X_TIMER_FLAG_REPEAT, IrisIRLEDCtrlFunc,
                                 NULL));
        }
    }
    else
    {
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        _fgLedOn[D_LED_POWER] = TRUE;
        x_timer_stop(_hIrisIRLedCtrlTimer);
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

     for (u4i=0; u4i<4;u4i++)
     {
         Printf("\n<NVM Upgrade>:The Byte  %d is  = %d\n",u4i, pauc_buffer[u4i]);
       u4MemLen =((pauc_buffer[u4i]<<(u4i*8)) + u4MemLen);
     }
     Printf("\n<NVM Upgrade>:The Data length is u4MemLen = %d\n",u4MemLen);
     Printf("\n  EEPROM Size is 0x%x \n",EEPROM_GetSize());

     if(((EEPROM_GetSize())!=(u4MemLen))||(u4MemLen>16*1024))
     {
        Printf("\n<NVM Upgrade>:The Buf  Size  ERROR!!!!\n");
        return DLM_OPEN_FILE_FAILED;
     }
     u4BufLen = (UINT32) *pi4_len;
     if(u4BufLen!=((u4MemLen*2)+4))
     {
            Printf("\n<NVM Upgrade>:The Buf  Length  ERROR!!!!\n");
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

#ifdef CC_Support_Sil9285
#define page9  0xE0
#define EDID_FIFO_ADDR  0x02
#define EDID_FIFO_DATA  0x03
#define EDID_FIFO_SEL   0x04
#define NVM_COMMAND     0x05
#define NVM_COMMAND_DONE 0x07

/*Main chip I2C define*/
#define Chip_I2C_ChannelId  2			//It CANNOT be changed
#define SIL9187_I2C_TIMING	0x100
#define I2C_AddrNum 				1

/*
@para: 1 is success , 0 is fail   
*/
static UINT8 _LdrSiI9187Read(UINT8 u1SlaveAddr, UINT32 u1WordAddr,UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
  
    INT32 i4ReturnValue;
    i4ReturnValue = SIF_X_Read(Chip_I2C_ChannelId,SIL9187_I2C_TIMING, u1SlaveAddr, I2C_AddrNum , u1WordAddr, pu1Buffer, u2ByteCount);

    if (i4ReturnValue>0)
    {
		return 1;
    }
    else
    {
	  return 0;
    }

}
/*
@para: 1 is success , 0 is fail   
*/ 
static UINT8 _LdrSiI9187Write(UINT8 u1SlaveAddr, UINT32 u1WordAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    INT32 i4ReturnValue;
   
    i4ReturnValue = SIF_X_Write(Chip_I2C_ChannelId, SIL9187_I2C_TIMING, u1SlaveAddr, I2C_AddrNum ,u1WordAddr, (UINT8 *)pu1Buffer, u2ByteCount);

    if (i4ReturnValue > 0)
    {

    		return 1;
    }
    else
    {
       //LOG(1,"Sil9187 write fail\n");
        return 0;
    }
}
/* The time probably need 2 seconds */
static void _LdrSiI9287EDIDWrite(UINT8* NVRAMtable) 
{
	
	UINT8 BIT_SEL_EDID0 = 0x01;    //select Port 0 SRAM
	UINT8 BIT_SEL_DEVBOOT = 0x20;  //Select SRAM boot area
	UINT8 FIFO_ADDR =0x00;
	UINT8 VAL_PRG_EDID = 0x03;  //Program NVRAM EDID data
	UINT8 VAL_PRG_DEVBOOT = 0x04;  //Program boot data
	UINT8 BIT_NVM_COMMAND_DONE = 0x01;
	UINT8 regdata;
       BOOL NVRAM_done = 1;
	BOOL NVRAMboot_done = 1;
	UINT16 i;
	
		/*		NVRAM data programming procedure	*/
       _LdrSiI9187Write(page9,EDID_FIFO_SEL,&BIT_SEL_EDID0,1);
	   _LdrSiI9187Write(page9,EDID_FIFO_ADDR,&FIFO_ADDR,1);
	   for(i=0;i<256;i++)
	   {
	   	//_LdrSiI9187Write(page9,EDID_FIFO_DATA,NVRAMtable,256);
	   	_LdrSiI9187Write(page9,EDID_FIFO_DATA,&NVRAMtable[i],1);
	   }
	   _LdrSiI9187Write(page9,NVM_COMMAND,&VAL_PRG_EDID,1);
	   vUtDelay10ms(60); //must delay to wait write done
	   if(_LdrSiI9187Read(page9,NVM_COMMAND_DONE,&regdata,1))
	   {
	   	 NVRAM_done =regdata& BIT_NVM_COMMAND_DONE;
	   	}
    
       /*		NVRAM boot data programming procedure	*/
       	_LdrSiI9187Write(page9,EDID_FIFO_SEL,&BIT_SEL_DEVBOOT,1);
	   	_LdrSiI9187Write(page9,EDID_FIFO_ADDR,&FIFO_ADDR,1);
	   	
	    	for(i=0;i<64;i++)
	   	{
	   	//_LdrSiI9187Write(page9,EDID_FIFO_DATA,NVRAMboottable,64);
	   		_LdrSiI9187Write(page9,EDID_FIFO_DATA,&NVRAMtable[i+256],1);
	   	}
	    	
	   	_LdrSiI9187Write(page9,NVM_COMMAND,&VAL_PRG_DEVBOOT,1);
	   	vUtDelay10ms(60); 
	  	if(_LdrSiI9187Read(page9,NVM_COMMAND_DONE,&regdata,1))
	  	{
	  		NVRAMboot_done=regdata& BIT_NVM_COMMAND_DONE;
	  	}
	  	if (NVRAM_done && NVRAMboot_done)
	  		printf("EDID USB upgrade success.......\n");
	  	else
	  		printf("EDID USB upgrade fail......\n");
}
#endif
static INT32 _LdrEdidUpgrade(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32* ui4_len)
{
	
	#ifdef CC_Support_Sil9285
		_LdrSiI9287EDIDWrite(pauc_buffer);
	#endif
    // download raw data to edid, pauc_buffer is raw data, ui4_len is data size
    return 0;
}

static void _LdrUpgradeDlmCbFunc(UINT32 u4BufSize)
{
    UNUSED(_LdrUpgradeDlmCbFunc);
    x_dlm_reg_append_callback("ueep", u4BufSize, _LdrUpEepromUpgrade);
    x_dlm_reg_append_callback("uimg", u4BufSize, _LdrUpFirmUpgrade);
    x_dlm_reg_append_callback("nvrm", u4BufSize, _LdrEepromUpgrade);
    x_dlm_reg_append_callback("edid", u4BufSize, _LdrEdidUpgrade);
}

// return the current version of corresponding tag
static void _LdrUpgradeDlmGetTagVerCbFunc(UCHAR* ucTagName, CHAR *szCurVer)
{
    UNUSED(_LdrUpgradeDlmGetTagVerCbFunc);

    if (x_strcmp((CHAR*)ucTagName, "load") == 0) // loader by luis
    {
#if 0
        // get version from dram
        LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
        x_strncpy(szCurVer, prLdrEnv->u.rHdr.szVersion, 8);
#endif
    }
    else if (x_strcmp((CHAR*)ucTagName, "firm") == 0) // firmware by luis
    {
#if 0
        IMAGE_UPGRADE_HEADER_T rImgUpHdr;
        LDR_FlashCopy((UINT8*)&rImgUpHdr, LOADER_MAX_SIZE, sizeof(IMAGE_UPGRADE_HEADER_T));
        x_strncpy(szCurVer, (CHAR*)rImgUpHdr.au1Version, 8);
#endif
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
    	EEPROM_Read((UINT64)EEPROM_EDID_VERION_OFFSET, (UINT32)szCurVer, 8);
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
         return (x_strcmp(szUpgVer, szCurVer) >= 0);
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

static void FlashAQGetVersion(CHAR *szCurVer)
{
#if 0 //temporarily disable AQ get version due to NANRPART_Read issue.
    UNUSED(FlashAQGetVersion);
	szCurVer = NULL;
	(void)szCurVer;
#else
    UINT8 u1AQPartition;
    UINT32 u2AQSize;    
    UINT64 u8Offset;

    UNUSED(FlashAQGetVersion);
    UNUSED(u2AQSize);
    UNUSED(u8Offset);    

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
#endif //if 1

}

static BOOL FlashAQCompVersion(CHAR *szCurVer, CHAR *szUpgVer)
{
    UNUSED(FlashAQCompVersion);
    if (x_strcmp(szUpgVer,szCurVer) >= 0)        
        return TRUE;
    else
        return FALSE;
}

static void GPIO_IrisLedCtrlFunc(LED_STATUS_T eLedStatus)
{
    switch(eLedStatus)
    {
    case eLedLoaderPowerOn:
        Printf("eLedLoaderPowerOn\n");
        //Power LED ON
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // VGA Standby LED OFF
        IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // TIMER LED OFF
        IrisLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        break;
        
    case eLedLoaderPowerOff:
        Printf("eLedLoaderPowerOff\n");
        // Standby LED ON
        if (_fgLedStandbyDiable)
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        }
        else
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        }
        // Power LED OFF
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        break;
        
    case eLedSystemPowerOn:
        Printf("eLedSystemPowerOn\n");
        // Power LED ON
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // VGA Standby LED OFF
        IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // TIMER LED OFF
        IrisLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        break;
        
    case eLedSystemPowerOff:
        Printf("eLedSystemPowerOff\n");
        // Standby LED ON
        if (_fgLedStandbyDiable)
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        }
        else
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        }
        // TIMER LED OFF
        IrisLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        // Power LED OFF
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        break;
        
    case eLedDarkLed:
    case eLedIrOn:
    case eLedIrOff:
        break;
        
    case eLedVgaStandby:
        Printf("eLedVgaStandby\r\n");
        //VGA Standby LED ON
        if (_fgLedStandbyDiable)
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        }
        else
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        }
        break;
        
#ifdef CC_MTK_LOADER        
    case eLedUpgradeBlink:
        Printf("eLedUpgradeBlink\r\n");
        LOADER_Iris_TimerOff();

        // Power LED OFF
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_OFF);
        // Standby LED ON
        IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        // Orange LED ON
        IrisLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_ORANGE);

        //Orange LED blinking On
        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = TRUE;
        _fgLedAllBlink = FALSE;
        
        LOADER_Iris_TimerOn();
        Printf("Begin USB Upgrade\r\n");
        break;
        
    case eLedUpgradeFinish:      
        Printf("eLedUpgradeFinish\r\n");
        LOADER_Iris_TimerOff();
        
        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = FALSE;
        _fgLedAllBlink = FALSE;
        
        // Power LED ON
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // Standby LED OFF
        IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // Orange LED OFF
        IrisLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        
        Printf("Finish USB Upgrade\r\n");
        break;
        
    case eLedUpgradeFail:
        Printf("eLedUpgradeFail\r\n");
        LOADER_Iris_TimerOff();
        _fgLedBlink[D_LED_POWER] = FALSE;
        _fgLedBlink[D_LED_STANDBY] = FALSE;
        _fgLedBlink[D_LED_TIMER] = FALSE;
        _fgLedAllBlink = FALSE;
        
        // Power LED ON
        IrisLEDCtrlFunc(D_LED_POWER, D_LED_POWER_ON);
        // Standby LED OFF
        IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        // Orange LED OFF
        IrisLEDCtrlFunc(D_LED_TIMER, D_LED_TIMER_OFF);
        break;
        
#endif /* CC_MTK_LOADER */

    default:
        break;
    }
}

static void GPIO_IrisReadStrapPin(void)
{
    UINT32 u4Val;

    _u4gPanelType = (UINT32)GPIO_Input(IRIS_GPIO_PANEL_PIN);
    _u4gFactoryType = (UINT32)GPIO_Input(IRIS_GPIO_FACTORY_PIN);

    _u4gPCBType = (UINT32)GPIO_Input(IRIS_GPIO_PCB_PIN_2);
    u4Val = (UINT32)GPIO_Input(IRIS_GPIO_PCB_PIN_1);
    _u4gPCBType |= (u4Val << 1);

    _u4gIrisModelType  = (_u4gPCBType == GPIO_PCB_TYPE_2) ?
        DRV_CUSTOM_IRIS_MODEL_120HZ_PANEL : DRV_CUSTOM_IRIS_MODEL_60HZ_PANEL;
    _u4gIrisModelType |= (_u4gPCBType == GPIO_PCB_TYPE_3) ?
        DRV_CUSTOM_IRIS_MODEL_AUD_5_1_CHANEL: DRV_CUSTOM_IRIS_MODEL_AUD_NORMAL;
    _u4gIrisModelType |= (_u4gPCBType == IRIS_GPIO_FACTORY_FOXCONN) ?
        DRV_CUSTOM_IRIS_MODEL_FACTORY_FOXCONN : DRV_CUSTOM_IRIS_MODEL_FACTORY_SONY;

    Printf("[GPIO_IrisReadStrapPin] _u4gModelType = %2x\n", _u4gIrisModelType);
}

static void GPIO_IrisPowerOnFunc(void)
{
    UINT32 i;
    INT32 i4Val;
    
#ifdef CC_MTK_LOADER
    UINT32 polarity;

    // Audio mute circuit mute in loader
    if (DRVCUST_InitQuery(eAudioMuteGpioNum, &i) == 0)
    {
        UNUSED(DRVCUST_InitQuery(eAudioMuteGpioPolarity, &polarity));
        GPIO_SetOut(i, polarity);
      	//GPIO_SetOut(204, 0);
    }

    GPIO_IrisLedCtrlFunc(eLedLoaderPowerOn);

#else /* CC_MTK_LOADER */
    GPIO_IrisLedCtrlFunc(eLedSystemPowerOn);
    
    if (x_timer_create(&_hIrisIRLedCtrlTimer) != OSR_OK)
    {
        LOG(5, "[GPIO_IrisPowerOnFunc] Create IR LED Blink timer fail !\n");
        return;
    }
    
    UNUSED(IRRC_RegCallbackFunc(IrisIRNfyFunc));

#endif /* CC_MTK_LOADER */

    INIT_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);
    GPIO_IrisReadStrapPin();
}

static void GPIO_IrisPowerOffFunc(UINT32 u4PowerDownEn)
{
    UINT32 i;
    INT32 i4Val;
        
    if (u4PowerDownEn & WAK_VGA)
    {
        GPIO_IrisLedCtrlFunc(eLedVgaStandby);
    }
    else
    {
        if (_fgLedStandbyDiable)
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_OFF);
        }
        else
        {
            IrisLEDCtrlFunc(D_LED_STANDBY, D_LED_STANDBY_ON);
        }
    }

    if (!DRVCUST_InitGet(eSupportDarkLed))
    {
#ifdef CC_MTK_LOADER
        GPIO_IrisLedCtrlFunc(eLedLoaderPowerOff);
#else /* CC_MTK_LOADER */
        GPIO_IrisLedCtrlFunc(eLedSystemPowerOff);
#endif /* CC_MTK_LOADER */
    }

    DOWN_SET_GPIO(eBackLightGpio, eBackLightOnVal);
    DOWN_SET_GPIO(ePanelPowerGpio, ePanelPowerOnVal);
    DOWN_SET_GPIO(ePowerCtrlGpio, ePowerCtrlOnVal);
}

#else
#error "reinclude Iris_gpio.h..."
#endif

