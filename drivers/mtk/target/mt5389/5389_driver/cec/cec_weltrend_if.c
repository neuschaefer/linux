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
 * $RCSfile: cec_weltrend_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file cec_weltrend_if.c
 *  This C file implements the CEC Weltrend basic hardware protocol functions.  
 */
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "cec_drv_if.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define WELTREND_I2C_TIMING 0x200
#define WELTREND_I2C_ADDR 0x26

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define SET_WELTREND_STATUS(arg)       \
        do {                           \
                WelErrSta |= (arg);    \
        } while (0)
#define CLR_WELTREND_STATUS(arg)       \
        do {                           \
                WelErrSta &= (~(arg)); \
        } while (0)
#define IS_WELTREND_STATUS(arg) ((WelErrSta & (arg)) > 0)


#ifdef SUPPORT_WELTREND_UP
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

static UINT8 WelErrSta = 0;
static BOOL fgWTWakenByUp = FALSE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** u1WeltrendRead
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
static UINT8 u1WeltrendRead(UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
#ifdef CC_WELTREND_UP_SUPPORT

    if (PDWNC_WeltrendRead(u1RegAddr, pu1Buffer, u2ByteCount) != 0)
    {
        LOG(3, "Weltrend read fail\n");

        SET_WELTREND_STATUS(I2C_READ_ERROR);
        return 1;
    }

    return 0;
#else /* CC_WELTREND_UP_SUPPORT */

    if (SIF_Read(WELTREND_I2C_TIMING, WELTREND_I2C_ADDR, u1RegAddr, pu1Buffer, u2ByteCount) == 0)
    {
        LOG(3, "Weltrend read fail\n");

        SET_WELTREND_STATUS(I2C_READ_ERROR);
        return 1;
    }

    return 0;
#endif /* CC_WELTREND_UP_SUPPORT */

}

//-------------------------------------------------------------------------
/** u1WeltrendWrite
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
static UINT8 u1WeltrendWrite(UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
#ifdef CC_WELTREND_UP_SUPPORT

    if (PDWNC_WeltrendWrite(u1RegAddr, pu1Buffer, u2ByteCount) != 0)
    {
        LOG(3, "[CEC] Weltrend write fail\n");

        SET_WELTREND_STATUS(I2C_WRITE_ERROR);
        return 1;
    }

    return 0;
#else /* CC_WELTREND_UP_SUPPORT */

    if (SIF_Write(WELTREND_I2C_TIMING, WELTREND_I2C_ADDR, u1RegAddr, pu1Buffer, u2ByteCount) == 0)
    {
        LOG(3, "[CEC] Weltrend write fail\n");

        SET_WELTREND_STATUS(I2C_WRITE_ERROR);
        return 1;
    }

    return 0;
#endif /* CC_WELTREND_UP_SUPPORT */

}

//-------------------------------------------------------------------------
/** vWTReadPendingData
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
static void vWTReadPendingData(void)
{
    UINT8 i2cdata[16];
    UINT8 len, i;

    u1WeltrendRead(0x0c, i2cdata, 1);

    if (i2cdata[0] & 0x01)
    {
        do
        {
            u1WeltrendRead(0x0c, i2cdata, 1);

            i2cdata[0] |= 0x02;
            u1WeltrendWrite(0x0c, i2cdata, 1);

            u1WeltrendRead(0x0c, i2cdata, 1);
            len = i2cdata[0] >> 4;
            LOG(3, "[CEC] weltrend message len: %x\n", len);
            u1WeltrendRead(0x10, i2cdata, len);
            LOG(3, "[CEC] weltrend message content: ");

            for (i = 0; i < len; i++)
            {
                LOG(3, "%x ", i2cdata[i]);
            }

            LOG(3, "\n");

            u1WeltrendRead(0x0b, i2cdata, 1);
        } while ((i2cdata[0] & 0xf) > 0);
    }
}
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** vWTCECOnOff
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTCECOnOff(UINT8 u1Onoff)
{
    UINT8 data;

    u1WeltrendRead(0x0a, &data, 1);
    data &= 0xfe;

    if (u1Onoff == TRUE)
    {
        data |= 0x01;
    }

    u1WeltrendWrite(0x0a, &data, 1);
}

//-------------------------------------------------------------------------
/** u1WTGetFWVersion
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
UINT8 u1WTGetFWVersion(void)
{
    UINT8 data;

    u1WeltrendRead(0x00, &data, 1);
    return data;
}

//-------------------------------------------------------------------------
/** vWTSetPhyAddr
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetPhyAddr(void)
{
    UINT8 data[2];

    data[0] = data[1] = 0x00;
    u1WeltrendWrite(0x08, data, 2);
}

//-------------------------------------------------------------------------
/** vWTSetVendorID
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetVendorID(UINT8 *data)
{
    u1WeltrendWrite(0x04, data, 3);
}

//-------------------------------------------------------------------------
/** vWTSetCECVersion
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetCECVersion(UINT8 *data)
{
    u1WeltrendWrite(0x07, data, 1);
}

//-------------------------------------------------------------------------
/** vWTSetMenuLang
 *   
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetMenuLang(UINT8 *data)
{
    u1WeltrendWrite(0x01, data, 3);
}

//-------------------------------------------------------------------------
/** u1WTGetWakeupReason
 *   see enum WT_WAKEUP_REASION
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
UINT8 u1WTGetWakeupReason(void)
{
    UINT8 data;

    u1WeltrendRead(0x30, &data, 1);
    data >>= 2;
    return data;
}

//-------------------------------------------------------------------------
/** vWTSetStandbyReason
 *   see enum WT_STANDBY_REASON
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetStandbyReason(UINT8 reason)
{
    UINT8 data;

    u1WeltrendRead(0x31, &data, 1);
    data &= 0xfe;

    if (reason == WT_STANDBY_PROTECTION)
    {
        data |= 0x01;
    }

    u1WeltrendWrite(0x31, &data, 1);
}

//-------------------------------------------------------------------------
/** vWTSetLatestErrCode
 *    
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetLatestErrCode(UINT8 u1code)
{
    u1WeltrendWrite(0x32, &u1code, 1);
}

//-------------------------------------------------------------------------
/** vWTSetErrorBuffer
 *    
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetErrorBuffer(UINT8 *errcode)
{
    u1WeltrendWrite(0x33, errcode, 5);
}

//-------------------------------------------------------------------------
/** u1WTGetClrErrBuffFlag
 *    
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
UINT8 u1WTGetClrErrBuffFlag(void)
{
    UINT8 data;

    u1WeltrendRead(0x38, &data, 1);

    return (data & 0x01);
}

//-------------------------------------------------------------------------
/** vWTSetConfirmation
 *    see enum WT_CONFIRMATION
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetConfirmation(UINT8 flag)
{
    UINT8 data;

    u1WeltrendRead(0x38, &data, 1);

    if (flag == WT_CONFIRM_ON)
    {
        data |= 0x04;
    }
    else if (flag == WT_CONFIRM_OFF)
    {
        data |= 0x08;
    }

    u1WeltrendWrite(0x38, &data, 1);
}

//-------------------------------------------------------------------------
/** vWTSetWakeTimer
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTSetWakeTimer(UINT8 *time)
{
    u1WeltrendWrite(0x39, time, 3);
}

//-------------------------------------------------------------------------
/** vWTWakeTimerOnOff
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTWakeTimerOnOff(UINT8 onoff)
{
    UINT8 data;

    u1WeltrendRead(0x38, &data, 1);
    data &= 0xdf;

    if (onoff == TRUE)
    {
        data |= 0x20;
    }

    u1WeltrendWrite(0x38, &data, 1);
}

//-------------------------------------------------------------------------
/** vWTGPIOWrite
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTGPIOWrite(UINT8 idx, UINT8 onoff)
{
    UINT8 j;

    switch (idx)
    {
    case WAKEUP: // PE_2
        // j=I2c_Read(PE_DAT);
        u1WeltrendRead(PE_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x04) : (j &= ~0x04);
        // I2c_Write(PE_DAT, j);
        u1WeltrendWrite(PE_DAT, &j, 1);
        break;

    case LED1: // PA_3
        // j=I2c_Read(PA_DAT);
        u1WeltrendRead(PA_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x08) : (j &= ~0x08);
        // I2c_Write(PA_DAT, j);
        u1WeltrendWrite(PA_DAT, &j, 1);
        break;

    case LED2: // PA_2
        // j=I2c_Read(PA_DAT);
        u1WeltrendRead(PA_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x04) : (j &= ~0x04);
        // I2c_Write(PA_DAT, j);
        u1WeltrendWrite(PA_DAT, &j, 1);
        break;

    case POWER_DOWN: // PD_0
        // j=I2c_Read(PD_DAT);
        u1WeltrendRead(PD_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x01) : (j &= ~0x01);
        // I2c_Write(PD_DAT, j);
        u1WeltrendWrite(PD_DAT, &j, 1);
        break;

    case CEC: // PB_1
        // j=I2c_Read(PB_DAT);
        u1WeltrendRead(PB_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x02) : (j &= ~0x02);
        // I2c_Write(PB_DAT, j);
        u1WeltrendWrite(PB_DAT, &j, 1);
        break;

    case INT_CEC: // PA_1
        // j=I2c_Read(PA_DAT);
        u1WeltrendRead(PA_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x02) : (j &= ~0x02);
        // I2c_Write(PA_DAT, j);
        u1WeltrendWrite(PA_DAT, &j, 1);
        break;

    case IR: // PB_2
        // j=I2c_Read(PB_DAT);
        u1WeltrendRead(PB_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x04) : (j &= ~0x04);
        // I2c_Write(PB_DAT, j);
        u1WeltrendWrite(PB_DAT, &j, 1);
        break;

    case KEY: // PC_0
        // j=I2c_Read(PC_DAT);
        u1WeltrendRead(PC_DAT, &j, 1);
        (onoff == 1) ? (j |= 0x01) : (j &= ~0x01);
        // I2c_Write(PC_DAT, j);
        u1WeltrendWrite(PC_DAT, &j, 1);
        break;

    default: // no define
        break;
    }
}

//-------------------------------------------------------------------------
/** vWTLEDCtrl
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTLEDCtrl(UINT8 idx, UINT8 onoff)
{
    if (0 == onoff)
    {
        onoff = 1;
    }
    else
    {
        onoff = 0;
    }

    if (idx == WT_LED_1)
    {
        vWTGPIOWrite(LED1, onoff);
    }
    else if (idx == WT_LED_2)
    {
        vWTGPIOWrite(LED2, onoff);
    }
}

//-------------------------------------------------------------------------
/** u1WTGPIORead
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
UINT8 u1WTGPIORead(UINT8 idx)
{
    UINT8 i, j;

    i = 0;

    switch (idx)
    {
    case WAKEUP: // PE_2
        // j=I2c_Read(PE_DAT);
        u1WeltrendRead(PE_DAT, &j, 1);
        if ((j & 0x04) == 0x04)
        {
            i = 1;
        }
        break;

    case LED1: // PA_3
        // j=I2c_Read(PA_DAT);
        u1WeltrendRead(PA_DAT, &j, 1);
        if ((j & 0x08) == 0x08)
            i = 1;
        break;

    case LED2: // PA_2
        // j=I2c_Read(PA_DAT);
        u1WeltrendRead(PA_DAT, &j, 1);
        if ((j & 0x04) == 0x04)
        {
            i = 1;
        }
        break;

    case POWER_DOWN: // PD_0
        // j=I2c_Read(PD_DAT);
        u1WeltrendRead(PD_DAT, &j, 1);
        if ((j & 0x01) == 0x01)
        {
            i = 1;
        }
        break;

    case CEC: // PB_1
        // j=I2c_Read(PB_DAT);
        u1WeltrendRead(PB_DAT, &j, 1);
        if ((j & 0x02) == 0x02)
        {
            i = 1;
        }
        break;

    case INT_CEC: // PA_1
        // j=I2c_Read(PA_DAT);
        u1WeltrendRead(PA_DAT, &j, 1);
        if ((j & 0x02) == 0x02)
        {
            i = 1;
        }
        break;

    case IR: // PB_2
        // j=I2c_Read(PB_DAT);
        u1WeltrendRead(PB_DAT, &j, 1);
        if ((j & 0x04) == 0x04)
        {
            i = 1;
        }
        break;

    case KEY: // PC_0
        // j=I2c_Read(PC_DAT);
        u1WeltrendRead(PC_ADC, &j, 1);
        /*if((j & 0x01) ==0x01)
        {
            i=1;
        }*/
        i = j;
        break;

    default: // no define
        i = 0xFF;
        break;
    }

    return i;
}

//-------------------------------------------------------------------------
/** vWeltrendBlinkErrorBuf
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWeltrendBlinkErrorBuf(BOOL fgEnable)
{
    UINT8 ui1_data;

    u1WeltrendRead(0x3c, &ui1_data, 1);

    if (fgEnable == TRUE)
    {
        ui1_data |= 0x01;
    }
    else
    {
        ui1_data &= 0xFE;
    }

    u1WeltrendWrite(0x3c, &ui1_data, 1);
}

//-------------------------------------------------------------------------
/** vWeltrendClearErrorBuf
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWeltrendClearErrorBuf(void)
{
    //PDWNC_WeltrendCheckBufClear();
}

//-------------------------------------------------------------------------
/** vWeltrendGPIOTest
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWeltrendGPIOTest(UINT8 bOnOff)
{
    UINT8 i;

    u1WeltrendRead(0x3c, &i, 1);

    if (bOnOff == SV_ON)
    {
        i |= 0x40;
    }
    else
    {
        i |= 0x80;
    }

    u1WeltrendWrite(0x3c, &i, 1);
}

//-------------------------------------------------------------------------
/** vWTNotifyPendingData
 *     
 *  @retval  
 *  @retval  
 */
//-------------------------------------------------------------------------
void vWTNotifyPendingData(void)
{
    UINT8 i2cdata[16];
    UINT8 len, i;

    CEC_FRAME_DESCRIPTION frame;

    if (!fgWTWakenByUp)
    {
        return;
    }

    fgWTWakenByUp = FALSE;

    u1WeltrendRead(0x0c, i2cdata, 1);

    if (i2cdata[0] & 0x01)
    {
        do
        {
            u1WeltrendRead(0x0c, i2cdata, 1);

            i2cdata[0] |= 0x02;
            u1WeltrendWrite(0x0c, i2cdata, 1);

            u1WeltrendRead(0x0c, i2cdata, 1);
            len = i2cdata[0] >> 4;
            u1WeltrendRead(0x10, i2cdata, len);

            Printf("[CEC] weltrend uP pending message\n");
            Printf("[CEC] weltrend message len: %x\n", len);
            Printf("[CEC] weltrend message content: ");

            for (i = 0; i < len; i++)
            {
                Printf("%x ", i2cdata[i]);
            }

            Printf("\n");
#ifdef NOTIFY_CECMW

            frame.size = len;
            frame.blocks.header.initiator = (i2cdata[0] >> 4);
            frame.blocks.header.destination = (i2cdata[0] & 0x0f);
            frame.blocks.opcode = i2cdata[1];

            for (i = 0; i < len - 2; i++)
            {
                frame.blocks.operand[i] = i2cdata[2 + i];
            }

            _Cec_NotifyRxResult(&frame);
#endif

            u1WeltrendRead(0x0b, i2cdata, 1);
        }while ((i2cdata[0] & 0xf) > 0);
    }
}

void vWTDebugMesg(void)
{
    UINT8 i2cdata[16];

    u1WeltrendRead(0x00, i2cdata, 1);
    LOG(3, "[CEC] weltrend firmware version: %x\n", i2cdata[0]);

    u1WeltrendRead(0x01, i2cdata, 3);
    LOG(3, "[CEC] weltrend menu language: %x %x %x\n", i2cdata[2], i2cdata[1], i2cdata[0]);

    u1WeltrendRead(0x04, i2cdata, 3);
    LOG(3, "[CEC] weltrend vendor id: %x %x %x\n", i2cdata[2], i2cdata[1], i2cdata[0]);

    u1WeltrendRead(0x07, i2cdata, 1);
    LOG(3, "[CEC] weltrend cec version: %x\n", i2cdata[0]);

    u1WeltrendRead(0x08, i2cdata, 2);
    LOG(3, "[CEC] weltrend physical address: %x %x\n", i2cdata[1], i2cdata[0]);

    u1WeltrendRead(0x0a, i2cdata, 1);

    if (i2cdata[0] & 0x01)
    {
        LOG(3, "[CEC] Weltrend CEC on\n");
    }
    else
    {
        LOG(3, "[CEC] Weltrend CEC off\n");
    }

    LOG(3, "[CEC] weltrend SUB-MCU status: %x\n", i2cdata[0] >> 1);

    if ((i2cdata[0] >> 1) & 0x01)
    {
        LOG(3, "[CEC] weltrend Sub-MCU alive\n");
    }

    if ((i2cdata[0] >> 1) & 0x02)
    {
        LOG(3, "[CEC] weltrend Sub-MCU CEC Active\n");
    }

    if ((i2cdata[0] >> 1) & 0x04)
    {
        LOG(3, "[CEC] weltrend Sub-MCU CEC Non-Active\n");
    }

    if ((i2cdata[0] >> 1) & 0x08)
    {
        LOG(3, "[CEC] weltrend Sub-MCU buffer overflow\n");
    }

    // u1WeltrendRead(0x0b, i2cdata, 1);
    // LOG(3, "[CEC] weltrend message number: %x\n", i2cdata[0]&0xf);

    // u1WeltrendRead(0x0c, i2cdata, 1);
    // LOG(3, "[CEC] weltrend message length: %x\n", i2cdata[0]>>4);
    u1WeltrendRead(0x0c, i2cdata, 1);

    if (i2cdata[0] & 0x01)
    {
        LOG(3, "[CEC] weltrend IS_NEW == 1\n");
    }
    else
    {
        LOG(3, "[CEC] weltrend IS_NEW == 0\n");
    }

    vWTReadPendingData();

    LOG(3, "[CEC] weltrend wakeup reason: %x\n", u1WTGetWakeupReason());
}
#endif /* #ifdef SUPPORT_WELTREND_UP */

