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
 * $Author: pan.huang $
 * $Date: 2012/12/20 $
 * $RCSfile: io_ice_touchpad.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_vga.c
 *  ICE VGA driver interface.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mttype.h"
#include "ice.h"
#include "ice_remote_control.h"
#include "sif_sw_if.h"
#include "x_timer.h"
#include "x_assert.h"
#include "util.h"
#include "drvcust_if.h"
#include "x_hal_io.h"
#include "hw_pdwnc.h"
#include "hw_vdoin.h"
#include "sif_if.h"
#include "eeprom_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
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

#define CLOCK_DIV 0xc00  //  27M/0xcc <10kHz
#define I2C_RETRY_TIMES 8
#define READ_WRITE_RETRY_TIMES 5
//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------
extern INT32 GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val);
extern UINT8 bPDWNC_InstallTouchPad(void);
extern UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

/*
touchpad enter I2C mode.
*/
void vTouchPadEnterI2CMode(void)
{
    INT32 i4FuncNo = 1;//i2c mode
    UINT32 u4PinSel;
    GPIO_SetOut(16, 0);//SCL low; SDA high for 50ms, enter I2C mode.
    GPIO_SetOut(15, 1);
    vUtDelay10ms(6);
    GPIO_SetOut(16, 1);//SCL low; SDA high for 50ms, enter I2C mode.
    GPIO_SetOut(15, 1);
 //   vRegWriteFldAlign(PDWNC_PINMUX,1,Fld(2,0,AC_MSKB0));
  //  vRegWriteFldAlign(PDWNC_PINMUX,1,Fld(2,2,AC_MSKB0));
    BSP_PinGpioGet(15, &u4PinSel);
    BSP_PinSet(u4PinSel, i4FuncNo);
    BSP_PinGpioGet(16, &u4PinSel);
    BSP_PinSet(u4PinSel, i4FuncNo);
    vUtDelay10ms(5);
}

void vTouchPadLeaveI2CMode(void)
{
}

/*
used to readdata from touchpad.
buffer: for saving data;
*/
UINT8 bTouchPadReadData(UINT8 *buffer)
{
	
#if 1  // mark by hongjun.chu
    INT32 i4ReturnValue = 1;
    UINT8 bCmd = 0xaa;
    UINT8 bData[9] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    UINT8 i = 0;
    vTouchPadEnterI2CMode();   // This will affect tunkey a_mute GPIO(201)
	// and this will affect tunkey amp i2c
    for(i = 0 ; i< I2C_RETRY_TIMES; i++)
    {
        vUtDelay1ms(1);
        i4ReturnValue = SIF_X_Write(0, CLOCK_DIV, 0x94, 1 ,0x03, &bCmd, 1);
        if(i4ReturnValue == 0)
        {
            continue;
        }
        vUtDelay1ms(1);
        i4ReturnValue = SIF_X_Read(0, CLOCK_DIV, 0x95, 0 , 0, &bData[0], 9);
        if(i4ReturnValue == 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }
    //printf("bTouchPadReadData read status(%d, %d)...................................\n", i4ReturnValue, i);
    if(i4ReturnValue > 0)
    {
        buffer[0] = bData[0];
        buffer[1] = bData[1];
        buffer[2] = bData[2];
        buffer[3] = bData[3];
        buffer[4] = bData[4];
        buffer[5] = bData[5];
        buffer[6] = bData[6];
        buffer[7] = bData[7];
        buffer[8] = bData[8];
        return 1;
    }
    else
    {
        return 0;
    }
	#endif
}


/*
used to set touchpad.
type: 1 for sensing parameter; 0 for  configuration;
buffer: value to be set; 
*/
UINT8 bTouchPadWriteData(UINT8 type, UINT8 *buffer)
{
    INT32 i4ReturnValue = 1;
    UINT8 bCmd = 0xaa;
    UINT8 bOldData[9] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    UINT8 bData[9] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    UINT8 i = 0;
    vTouchPadEnterI2CMode();
    
//    i4ReturnValue = SIF_X_Write(u1ChannelId, u4ClkDiv, u1Dev, u1AddrNum ,u4Addr, (UINT8 *)pu1Buf, u4Count);
    //read first;
    for(i = 0 ; i< I2C_RETRY_TIMES; i++)
    {
        vUtDelay1ms(1);
        i4ReturnValue = SIF_X_Write(0, CLOCK_DIV, 0x94, 1 ,0x03, &bCmd, 1);
        if(i4ReturnValue == 0)
        {
            continue;
        }
        vUtDelay1ms(1);
        i4ReturnValue = SIF_X_Read(0, CLOCK_DIV, 0x95, 0 , 0, &bOldData[0], 9);
        if(i4ReturnValue == 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    printf("bTouchPadWriteData pre read status(%d, %d)...................................\n", i4ReturnValue, i);
    
    for(i = 0 ; i< I2C_RETRY_TIMES; i++)
    {
        vUtDelay1ms(1);
        switch(type)
        {
            case 1://sensing
            if((bOldData[0] != buffer[0])||(bOldData[1] != buffer[1])||(bOldData[2] != buffer[2])\
                ||(bOldData[3] != buffer[3])||(bOldData[4] != buffer[4])||(bOldData[5] != buffer[5]))
            {
                bData[0] = buffer[0];
                bData[1] = buffer[1];
                bData[2] = buffer[2];
                bData[3] = buffer[3];
                bData[4] = buffer[4];
                bData[5] = buffer[5];
                bData[6] = 0xaa;
                i4ReturnValue = SIF_X_Write(0, CLOCK_DIV, 0x94, 1 ,0x2, &bData[0], 7);
            }
            break;
            case 0://configuration
            if(bOldData[6] != buffer[0])
            {
                bData[0] = buffer[0];
                bData[1] = 0xaa;
                i4ReturnValue = SIF_X_Write(0, CLOCK_DIV, 0x94, 1 ,0x4, &bData[0], 2);
            }
            break;
            default:
            break;
        }
        if(i4ReturnValue == 0)
        {
            continue;
        }
        else
        {
            break;
        }

    }
    printf("bTouchPadWriteData write status(%d, %d)...................................\n", i4ReturnValue, i);
    if(i4ReturnValue > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/*
used to change sensing. after both vTouchPadSetSensing and vTouchPadSetConfiguration are called, value will be set;
sensing: pointer to sensing data, size is 6;
return value: 0 means fail; 1 means succeed.
*/
UINT8 bTouchPadSetSensing(UINT8 * sensing)
{
    UINT8 i = 0;
    for(i = 0; i < READ_WRITE_RETRY_TIMES; i++)
    {
        if(1 == bTouchPadWriteData(1, sensing))
        {
            return 1;
        }
    }
    return 0;
}

/*
used to change configuration. after both vTouchPadSetSensing and vTouchPadSetConfiguration are called, value will be set;
configuration: pointer to configuration data, size is 1;
return value: 0 means fail; 1 means succeed.
*/
UINT8 bTouchPadSetConfiguration(UINT8 * configuration)
{
    UINT8 i = 0;
    for(i = 0; i < READ_WRITE_RETRY_TIMES; i++)
    {
        if(1 == bTouchPadWriteData(0, configuration))
        {
            return 1;
        }
    }
    return 0;
}

/*
used to Get sensing;
sensing: pointer to sensing data, size is 6;
return value: 0 means fail; 1 means succeed.
*/
UINT8 bTouchPadGetSensing(UINT8 * sensing)
{
    UINT8 i = 0;
    UINT8 bTmp[9] = {0};
    for(i = 0; i < READ_WRITE_RETRY_TIMES; i++)
    {
        if(1 == bTouchPadReadData(&bTmp[0]))
        {
            sensing[0] = bTmp[0];
            sensing[1] = bTmp[1];
            sensing[2] = bTmp[2];
            sensing[3] = bTmp[3];
            sensing[4] = bTmp[4];
            sensing[5] = bTmp[5];
            return 1;
        }
    }
    return 0;
}

/*
used to Get configuration;
configuration: pointer to configuration data, size is 1;
return value: 0 means fail; 1 means succeed.
*/
UINT8 bTouchPadGetConfiguration(UINT8 *configuration)
{
    UINT8 i = 0;
    UINT8 bTmp[9] = {0};
    for(i = 0; i < READ_WRITE_RETRY_TIMES; i++)
    {
        if(1 == bTouchPadReadData(&bTmp[0]))
        {
            *configuration = bTmp[6];
            return 1;
        }
    }
    return 0;
}

/*
used to Get FW version ID;
pFwVersionID: pointer to FW version ID data, size is 1;
return value: 0 means fail; 1 means succeed.
*/
UINT8 bTouchPadGetFwVersionID(UINT8 * pFwVersionID)
{
    UINT8 i = 0;
    UINT8 bTmp[9] = {0};
    for(i = 0; i < READ_WRITE_RETRY_TIMES; i++)
    {
        if(1 == bTouchPadReadData(&bTmp[0]))
        {
            *pFwVersionID = (bTmp[7]&0x3f);
            return 1;
        }
    }
    return 0;
}

/*
please init touchpad as soon as possible;
don't put it through some ice init, because it's too late.
*/
void vTouchPadInit(void)
{

}

MT_RESULT_T _ICE_IO_TouchPad_Init(unsigned long arg)
{   
    MT_RESULT_T i4Ret = MTR_OK;
    vTouchPadInit();
    return i4Ret;
}

MT_RESULT_T _ICE_IO_TouchPad_GetVersion(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 rTouchPadVersion = 0;
    UINT8 * pTouchPadVersion = (UINT8 *)arg;

    if(0 == bTouchPadGetFwVersionID(&rTouchPadVersion))
    {
        i4Ret =  MTR_NOT_OK;
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(pTouchPadVersion, UINT8);
    COPY_TO_USER_ARG(pTouchPadVersion, rTouchPadVersion, UINT8);

    return MTR_OK;//i4Ret; for remove log temperently;
}
  
MT_RESULT_T _ICE_IO_TouchPad_GetSensing(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pTouchPadSensing = (UINT8 *)arg;
    UINT8 bSensing[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    if(0 == bTouchPadGetSensing(&bSensing[0]))
    {
        i4Ret =  MTR_NOT_OK;
    }
    printf("_ICE_IO_TouchPad_GetSensing:%d, %d, %d, %d, %d, %d\n", bSensing[0], bSensing[1], bSensing[2], bSensing[3], bSensing[4], bSensing[5]);
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pTouchPadSensing, 6);
    COPY_TO_USER_ARG_K(pTouchPadSensing, bSensing[0], 6);
    return i4Ret;
}

MT_RESULT_T _ICE_IO_TouchPad_GetConfiguration(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pTouchPadConfiguration = (UINT8 *)arg;
    UINT8 bConfiguration = 0;
    
    if(0 == bTouchPadGetConfiguration(&bConfiguration))
    {
        i4Ret =  MTR_NOT_OK;
    }
    printf("_ICE_IO_TouchPad_GetConfiguration: %d", bConfiguration);
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pTouchPadConfiguration, 1);
    COPY_TO_USER_ARG_K(pTouchPadConfiguration, bConfiguration, 1);
    return i4Ret;
}

MT_RESULT_T _ICE_IO_TouchPad_SetSensing(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pTouchPadSensing = (UINT8 *)arg;
    UINT8 bSensing[6] = {0, 0, 0, 0, 0, 0};

    USR_SPACE_ACCESS_VALIDATE_ARG_K(pTouchPadSensing, 6);
    COPY_FROM_USER_ARG_K(pTouchPadSensing, bSensing[0], 6);
    printf("_ICE_IO_TouchPad_SetSensing:%d, %d, %d, %d, %d, %d\n", bSensing[0], bSensing[1], bSensing[2], bSensing[3], bSensing[4], bSensing[5]);
    if(0 == bTouchPadSetSensing(&bSensing[0]))
    {
        i4Ret =  MTR_NOT_OK;
    }
    return i4Ret;
}

MT_RESULT_T _ICE_IO_TouchPad_SetConfiguration(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pTouchPadConfiguration = (UINT8 *)arg;
    UINT8 bConfiguration = 0;
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pTouchPadConfiguration, 1);
    COPY_FROM_USER_ARG_K(pTouchPadConfiguration, bConfiguration, 1);
    printf("_ICE_IO_TouchPad_SetConfiguration: %d", bConfiguration);

    if(0 == bTouchPadSetConfiguration(&bConfiguration))
    {
        i4Ret =  MTR_NOT_OK;
    }
    return i4Ret;
}

MT_RESULT_T _ICE_IO_TouchPad_SetButtonRange(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    INT32 i;
    SGL_SERVO_T   *prSingleServoKeyList;
    UINT32 * pArg = (UINT32 *)arg;
    UINT32 buffer[3];
    UINT32 u4Key;
    UINT8 bTop;
    UINT8 bBtm;
    UINT8 bTopOld;
    UINT8 bBtmOld;
    static UINT8 bIsTouchPadInstalled = 0;
    UINT32 dTouchPadTop1Offset;
    INT32 dWriteResult1 = 0;
    INT32 dWriteResult2 = 0;

    if(bIsTouchPadInstalled == 0)
    {
        bPDWNC_InstallTouchPad();
        bIsTouchPadInstalled = 1;
    }
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pArg, 12);
    COPY_FROM_USER_ARG_K(pArg, buffer[0], 12);
    u4Key = buffer[0];
    bTop = buffer[1];
    bBtm = buffer[2];
    
    i = DRVCUST_OptQuery(eSingleServoKeyList, (UINT32 *)(void *)&prSingleServoKeyList);
    if (i < 0)
    {
        i4Ret = MTR_NOT_OK;
        prSingleServoKeyList = NULL;
    }
    
    i = DRVCUST_InitQuery(eTouchPadTop1Offset, (UINT32 *)(void *)&dTouchPadTop1Offset);
    if (i < 0)
    {
        i4Ret = MTR_NOT_OK;
        prSingleServoKeyList = NULL;
    }

    i = 0;
    while ((prSingleServoKeyList) && 
        (prSingleServoKeyList[i].u4Key != 0) && 
        (prSingleServoKeyList[i].u4Key != DKBIT_NONE))
    {
        if(prSingleServoKeyList[i].u4Key == u4Key)
        {
            prSingleServoKeyList[i].u4Top = bTop;
            prSingleServoKeyList[i].u4Btm = bBtm;
            dWriteResult1 = EEPROM_Read((UINT64)(dTouchPadTop1Offset+(i*2)), (UINT32)(&bTopOld), 1);
            if(1 == dWriteResult1)
            {
                i4Ret = MTR_NOT_OK;
                break;
            }
            if(bTopOld != bTop)
            {
                dWriteResult1 = EEPROM_Write((UINT64)(dTouchPadTop1Offset+(i*2)), (UINT32)(&bTop), 1);
                if(1 == dWriteResult1)
                {
                    i4Ret = MTR_NOT_OK;
                    break;
                }
            }
                
            dWriteResult2 = EEPROM_Read((UINT64)(dTouchPadTop1Offset+(i*2)+1), (UINT32)(&bBtmOld), 1);
            if(1 == dWriteResult2)
            {
                i4Ret = MTR_NOT_OK;
                break;
            }
            if(bBtmOld != bBtm)
            {
                dWriteResult2 = EEPROM_Write((UINT64)(dTouchPadTop1Offset+(i*2)+1), (UINT32)(&bBtm), 1);
                if(1 == dWriteResult2)
                {
                    i4Ret = MTR_NOT_OK;
                    break;
                }
            }

            i4Ret = MTR_OK;
            break;
        }
        i4Ret = MTR_NOT_OK;
        i++;
    }
    return i4Ret;

}


MT_RESULT_T _ICE_IO_TouchPad_ReadServADC(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT32 * pArg = (UINT32 *)arg;
    UINT32 dServAdcNum = 0;
    UINT32 dServAdcVal = 0;
    UINT32 buffer[2];
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pArg, 8);
    COPY_FROM_USER_ARG_K(pArg, buffer[0], 8);

    dServAdcNum = buffer[0];
    dServAdcVal = PDWNC_ReadServoADCChannelValue(dServAdcNum);

    buffer[1] = dServAdcVal;


    //printf("buffer[0] = %d, buffer[1] = %d\n", buffer[0], buffer[1]);
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pArg, 8);
    COPY_TO_USER_ARG_K(pArg, buffer[0], 8);
    return i4Ret;
}
int ice_ioctl_touchpad(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;

    switch (cmd)
    {
        case ICE_IO_TOUCHPAD_INIT:
            i4Ret = _ICE_IO_TouchPad_Init(arg);
            break;
        case ICE_IO_TOUCHPAD_GETVERSION:
            i4Ret = _ICE_IO_TouchPad_GetVersion(arg);
            break;
        case ICE_IO_TOUCHPAD_GETSENSING:
            i4Ret = _ICE_IO_TouchPad_GetSensing(arg);
            break;
        case ICE_IO_TOUCHPAD_GETCONFIGURATION:
            i4Ret = _ICE_IO_TouchPad_GetConfiguration(arg);
            break;
        case ICE_IO_TOUCHPAD_SETSENSING:
            i4Ret = _ICE_IO_TouchPad_SetSensing(arg);
            break;
        case ICE_IO_TOUCHPAD_SETCONFIGURATION:
            i4Ret = _ICE_IO_TouchPad_SetConfiguration(arg);
            break;
        case ICE_IO_TOUCHPAD_SETBUTTONRANGE:
            i4Ret = _ICE_IO_TouchPad_SetButtonRange(arg);
            break;
         case ICE_IO_TOUCHPAD_READSERVADC:
            i4Ret = _ICE_IO_TouchPad_ReadServADC(arg);
            break;
        default:
            break;
    }
    return i4Ret;
 }
