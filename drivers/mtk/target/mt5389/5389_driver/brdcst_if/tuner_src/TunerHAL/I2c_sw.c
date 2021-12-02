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
 * $RCSfile: I2c_sw.c,v $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_sw_if.c
 *  This file implement serial interface SIF function by software controlled GPIO pin.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
//#include "sif_debug.h"
//#include "sif_hw.h"
//#include "sif_sw_if.h"
#include "I2c_sw.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"

#include "x_assert.h"
#include "x_bim.h"
#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// *********************************************************************
// Define I2C Read/Write Flag  for fgSeq_Read /vI2CRead
// *********************************************************************

#define SIF_FG_SEQREAD      1
#define SIF_FG_RANDREAD     0

// *********************************************************************
// Timing Definition for System Clock
// *********************************************************************
#define SIF_ACK_DELAY  200 // time out for acknowledge check
#define SIF_BUS_DELAY  255 // time out bus arbitration

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

static UINT32 _u4SIF_SDA;
static UINT32 _u4SIF_SCL;

// old function of SDA, SCL pin.
static INT32 _i4SIF_SDA_Func;
static INT32 _i4SIF_SCL_Func;
static INT32 _i4SIF_SDA_Pin;
static INT32 _i4SIF_SCL_Pin;

// mark initialization.
static UINT32 _u4SIF_Init = FALSE;

/// SIF multi-task protection semaphore.
static HANDLE_T _hSifUserSema = NULL_HANDLE;

//-----------------------------------------------------------------------------
/** NIM_GPIO_SetOut() to set gpio output value.
 *  @param i4GpioNum
 *  @param i4Val 0 or 1.
 *  @retval to return current gpio out register setting.
 */
//-----------------------------------------------------------------------------
INT32 NIM_GPIO_SetOut(INT32 i4GpioNum, INT32 i4Val)
{
    INT32 fgSet, i4Ret;

    fgSet = i4Val;
    i4Ret = GPIO_Output(i4GpioNum, &fgSet);
    if (i4Ret != i4Val)
    {
        return i4Ret;
    }
    return GPIO_Output(i4GpioNum, NULL);
}

//-----------------------------------------------------------------------------
/** GPIO_Input()  The GPIO input reading functions. It will check the
 *  i4GpioNum and read related register bit to return.
 *  @param i4GpioNum the gpio number to read.
 *  @retval 0 or 1.  (GPIO input value.)
 */
//-----------------------------------------------------------------------------
INT32 NIM_GPIO_GetIn(INT32 i4GpioNum)
{
    INT32 i4Mode;
    // Set to input mode.
    i4Mode = 0;
    VERIFY(GPIO_Enable(i4GpioNum, &i4Mode)==i4Mode);

    return GPIO_Input(i4GpioNum);
}

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** SIF_READSDA
 * read IIC SDA.
 *
 * @param VOID
 * @return 1 or 0.
 */
//-------------------------------------------------------------------------
static INT32 SIF_READSDA(void)
{
    return NIM_GPIO_GetIn((INT32)_u4SIF_SDA);
}

#if 0
static INT32 SIF_READSCL(void)
{
    return NIM_GPIO_GetIn((INT32)_u4SIF_SCL);
}
#endif

//-------------------------------------------------------------------------
/** SIF_SDALOW
 * output IIC SDA low.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static void SIF_SDALOW(void)
{
    NIM_GPIO_SetOut( _u4SIF_SDA , 0 );
}

//-------------------------------------------------------------------------
/** SIF_SDAHIGH
 * output IIC SDA high.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static void SIF_SDAHIGH(void)
{
    NIM_GPIO_SetOut( _u4SIF_SDA , 1 );
    NIM_GPIO_GetIn( _u4SIF_SDA );
}

//-------------------------------------------------------------------------
/** SIF_SCLLOW
 * output IIC SCL low.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static void SIF_SCLLOW(void)
{
    NIM_GPIO_SetOut( _u4SIF_SCL , 0 );
}

//-------------------------------------------------------------------------
/** SIF_SCLHIGH
 * output IIC SCL high.
 *
 * @param VOID
 * @return VOID.
 */
//-------------------------------------------------------------------------
static void SIF_SCLHIGH(void)
{
    NIM_GPIO_SetOut( _u4SIF_SCL , 1 );
    //SCL is pull-highed, it should be high if master release control
    NIM_GPIO_GetIn( _u4SIF_SCL );
}

//-------------------------------------------------------------------------
/** _Sifsw_Delay1us
 * Delay routine (local function)
 *
 * @param u4Value: u4Value = 1->1us, 2->2us
 * @return NONE
 */
//-------------------------------------------------------------------------
static void _Sifsw_Delay1us(UINT32 u4Value)
{
    volatile UINT32 u4Cnt;

    volatile UINT32 u4Loop;

    for (u4Cnt = 0; u4Cnt < u4Value; u4Cnt++)
    {
        for (u4Loop = 0; u4Loop < 25/*10*/; u4Loop++)
        {
            ;
        }
    }
}
//-------------------------------------------------------------------------
/** _Sifsw_SendStop
 * I2C stop routine (local function)
 *
 * @param bBitDelay delay between bit
 * @return NONE
 */
//-------------------------------------------------------------------------
 static void _Sifsw_SendStop(UINT8 bBitDelay)
{
    SIF_SDALOW();

    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SDAHIGH();
    _Sifsw_Delay1us(bBitDelay);
}

//-------------------------------------------------------------------------
/** _Sifsw_PutByte
 * I2C send UINT8 routine (local function)
 *
 * @param bValue UINT8 to be sent out
 * @param bBitDelay delay between bit
 * @return 1 or 0
 * @retval = 1 send UINT8 success
 * @retval = 0 send UINT8 fail (no ACK)
 */
//-------------------------------------------------------------------------
 static UINT8 _Sifsw_PutByte(UINT8 bValue, UINT8 bBitDelay)
{
    UINT8 bBitMask = 0x80;

    UINT8 bData;

    BOOL fgAckDetected=FALSE;
//    INT32 i;


    // step 1 : 8-bit data transmission
    while (bBitMask)
    {
        if (bBitMask & bValue)
        {
            SIF_SDAHIGH();
        }
        else
        {
            SIF_SDALOW();
        }

        _Sifsw_Delay1us(bBitDelay);

        SIF_SCLHIGH();
        _Sifsw_Delay1us(bBitDelay);

        SIF_SCLLOW();
//``
    #if 0
        if (bBitMask == 1)
        {
            SIF_SDALOW();
            SIF_READSDA();      //release SDA   //TC Wu
        }
    #endif
        _Sifsw_Delay1us(bBitDelay);

        bBitMask = bBitMask >> 1;
    }

    // step 2 : slave acknowledge check
//    SIF_SDAHIGH();
//``
//    SIF_SDALOW();
//    _Sifsw_Delay1us(bBitDelay * 4);
    SIF_READSDA();      //release SDA   //TC Wu
    _Sifsw_Delay1us(bBitDelay);

//    i = 100 + 10*bBitDelay;   //wait at least 10us

//    do
//    {
        bData = SIF_READSDA();
        if( bData == 0 )    //SDA Low?
        {
            fgAckDetected= TRUE;
//            break;
        }
        _Sifsw_Delay1us(1);
//    } while ( !fgAckDetected && (--i>0) );

    //_Sifsw_Delay1us(bBitDelay);
    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    //_Sifsw_Delay1us(bDelay);
    SIF_SCLLOW(); // end ACK polling
    _Sifsw_Delay1us(bBitDelay);

    if (fgAckDetected)
    {
        return (TRUE); // return TRUE if ACK detected
    }
    else
    {
    //    _Sifsw_SendStop(bBitDelay);
        return (FALSE); // return FALSE if time out
    }
}

//-------------------------------------------------------------------------
/** _Sifsw_GetByte
 * I2C read UINT8 routine (local function)
 *
 * @param prValue pointer of data to be sent out
 * @param fgSeqRead sequential read
 * @param bBitDelay delay between bit
 * @return NONE
 */
//-------------------------------------------------------------------------
 static void _Sifsw_GetByte(UINT8 *prValue, UINT8 fgSeqRead, UINT8 bBitDelay)
{
    UINT8 bBitMask = 0x80;

    UINT8 bData;

    //ASSERT(prValue);
	VERIFY(NULL != prValue);

    *prValue = 0;
//``
    SIF_SDAHIGH();
//    SIF_SDALOW();
//    SIF_READSDA();
//``
    _Sifsw_Delay1us(bBitDelay);

    // step 1 : 8-bit data reception
    while (bBitMask)
    {
        SIF_SCLHIGH();

        _Sifsw_Delay1us(bBitDelay);
        bData = SIF_READSDA();

        if (bData)
        {
            *prValue = *prValue | bBitMask;
        }

        //_Sifsw_Delay1us(bBitDelay);
        SIF_SCLLOW();             // ready for next clock out
        _Sifsw_Delay1us(bBitDelay);

        bBitMask = bBitMask >> 1; // shift bit mask & clock delay
    }

    // step 2 : acknowledgement to slave
    if (fgSeqRead)
    {
        SIF_SDALOW();
    }
    else
    {
        SIF_SDAHIGH();
    }

    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLLOW();
    _Sifsw_Delay1us(bBitDelay);

    SIF_READSDA();
//    SIF_SDAHIGH();    //TC Wu
    _Sifsw_Delay1us(bBitDelay);
}

//-------------------------------------------------------------------------
/** _Sifsw_SendStart
 * I2C send UINT8 with device address routine (local function)
 *
 * @param bDevAddr device address
 * @param bBitDelay delay between bit
 * @return 1 or 0
 * @retval = 1 send UINT8 success
 * @retval = 0 send UINT8 fail (no ACK)
 */
//-------------------------------------------------------------------------
static UINT8 _Sifsw_SendStart(UINT8 bDevAddr, UINT8 bBitDelay)
{
//``
//    SIF_SCLHIGH();
    SIF_SDAHIGH();

    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SDALOW();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLLOW();
    _Sifsw_Delay1us(bBitDelay);

    if (bDevAddr != 0)
        return (_Sifsw_PutByte(bDevAddr, bBitDelay));
    else
        return (TRUE); // return TRUE if ACK detected
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** I2CSW_Init
 * I2C initialization routine
 *
 * @param u4SDA     GPIO number as SDA.
 * @param u4SCL     GPIO number as SCL.
 * @return NONE
 */
//-------------------------------------------------------------------------
void I2CSW_Init(UINT32 u4SDA, UINT32 u4SCL)
{
    if (!_u4SIF_Init)
    {
        if ((_u4SIF_SDA != u4SDA) || (_u4SIF_SCL != u4SCL))
        {
            // Get old pin number, function, and store it.
#if (defined(CC_MT5363) || defined(CC_MT5387))/* Fixed MT5365 Compile Warning Error */
			_i4SIF_SDA_Func = BSP_PinGpioGet((INT32)u4SDA, (INT32 *)&_i4SIF_SDA_Pin);
			_i4SIF_SCL_Func = BSP_PinGpioGet((INT32)u4SCL, (INT32 *)&_i4SIF_SCL_Pin);
#else	//5365/95...
            _i4SIF_SDA_Func = BSP_PinGpioGet((INT32)u4SDA, (UINT32 *)&_i4SIF_SDA_Pin);
            _i4SIF_SCL_Func = BSP_PinGpioGet((INT32)u4SCL, (UINT32 *)&_i4SIF_SCL_Pin);
#endif            
            _u4SIF_SDA = u4SDA;

            // set SDA as GPIO by select pin mux
            VERIFY(BSP_PinGpioSel((INT32)_u4SIF_SDA) == 0);

            _u4SIF_SCL = u4SCL;
            // set SCL as GPIO.
            VERIFY(BSP_PinGpioSel((INT32)_u4SIF_SCL) == 0);
        }

    // create semaphore
        VERIFY(x_sema_create(&_hSifUserSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    _u4SIF_Init = TRUE;
/*
    SIF_SCLHIGH();
    _Sifsw_Delay1us(2);
    SIF_SDAHIGH();
    _Sifsw_Delay1us(2); */  //this part causes Non-Stop option failed
}

//-------------------------------------------------------------------------
/** I2CSW_Remove
 * I2C deactivate routine: restore sda and scl pin attribute.
 *
 * @param u4SDA     GPIO number as SDA.
 * @param u4SCL     GPIO number as SCL.
 * @return NONE
 */
//-------------------------------------------------------------------------
void I2CSW_Remove(void)
{
    if (!_u4SIF_Init)
    {
        return;
    }

// destroy semaphore
    VERIFY(x_sema_delete(_hSifUserSema) == OSR_OK);

    _u4SIF_Init = FALSE;

    // restore old function of SDA, SCL.
    VERIFY(BSP_PinSet((INT32)_i4SIF_SDA_Pin, (INT32)_i4SIF_SDA_Func) == 0);
    VERIFY(BSP_PinSet((INT32)_i4SIF_SCL_Pin, (INT32)_i4SIF_SCL_Func) == 0);

    // clear variable.
    _u4SIF_SDA = 0;
    _u4SIF_SCL = 0;
    _i4SIF_SDA_Pin = 0;
    _i4SIF_SCL_Pin = 0;
    _i4SIF_SDA_Func = 0;
    _i4SIF_SCL_Func = 0;
}

#if 0

//-------------------------------------------------------------------------
/** I2CSW_WaitAckOK
 * I2C ack testing routine
 *
 * @param bDevAddr device address
 * @param wDelayUs delay before next trial
 * @param bRetry retry times
 * @return 1 or 0
 * @retval = 1 device return ACK success
 * @retval = 0 device return ACK fail
 */
//-------------------------------------------------------------------------

BOOL I2CSW_WaitAckOK(UINT8 bDevAddr, UINT16 wDelayUs, UINT8 bRetry)
{
    bDevAddr &= 0xFE;

    do
    {
        if (!_Sifsw_SendStart(bDevAddr, 2))
        {
            _Sifsw_SendStop(2);

            bRetry--;

            if (wDelayUs != 0)
            {
                _Sifsw_Delay1us(wDelayUs *2);
            }
            if (bRetry != 0)
            {
                continue;
            }
        }

        _Sifsw_SendStop(2);
        break;
    } while (1);

    if (bRetry == 0)
    {
        return (FALSE);
    }

    return (TRUE);
}
#endif

//-------------------------------------------------------------------------
/** I2CSW_WriteData
 * I2C write command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be sent out
 * @param pbData pointer of data to be sent out
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @return 1 or 0
 * @retval = 1 write success
 * @retval = 0 write fail
 */
//-------------------------------------------------------------------------

UINT8 I2CSW_WriteData(UINT8 bDevAddr, UINT8 bSubAddr, UINT8 bDataCount, UINT8 *pbData,
                      UINT8 bOption)
{
UINT8 bBitDelay;
UINT8 bByteDelay;
BOOL fgNeedACKCheck;
BOOL fgRetSts = TRUE;

    //ASSERT(pbData);
	VERIFY(NULL != pbData);

// check if SIF is initialized ?
    if (!_u4SIF_Init)
    {
        if ((_u4SIF_SDA == 0) || (_u4SIF_SCL == 0))
            return (FALSE);
        I2CSW_Init(_u4SIF_SDA, _u4SIF_SCL);
    }

// wait semaphore
    VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    bDevAddr &= 0xFE;

    bBitDelay = bOption & 0x07;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >> 6);

#if 0       //stop if stop bit fail
    if ((!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck))
    {
        return (FALSE);
    }
#else
    _Sifsw_SendStart(bDevAddr, bBitDelay);
#endif

    _Sifsw_Delay1us(bByteDelay);

    if ( (!_Sifsw_PutByte(bSubAddr, bBitDelay)) && (fgNeedACKCheck) )
    {
    //    return (FALSE);
        fgRetSts = FALSE;
        goto lbExit;
    }

    _Sifsw_Delay1us(bByteDelay);

    while (bDataCount)
    {
        if ((!_Sifsw_PutByte(*(pbData++), bBitDelay))&& (fgNeedACKCheck)) // Data Content Write
        {                                   // Device Address exceeds the range
        //    return (FALSE);
            fgRetSts = FALSE;
            goto lbExit;
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataCount--;
    }

lbExit:
    _Sifsw_SendStop(bBitDelay);

// release semaphore
    VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);

//    return (TRUE);
    return fgRetSts;
}

//-------------------------------------------------------------------------
/** I2CSW_ReadData
 * I2C read command (general type)
 *
 * @param bDevAddr device address
 * @param bSubAddr sub-address
 * @param bDataCount numbber of bytes to be read
 * @param pbData pointer of data to be read
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 I2CSW_ReadData(UINT8 bDevAddr, UINT8 bSubAddr, UINT16 bDataCount, UINT8 *pbData,
                     UINT8 bOption)
{
UINT8 bBitDelay;
UINT8 bByteDelay;
BOOL fgNeedACKCheck;
BOOL fgRetSts = TRUE;

    //ASSERT(pbData);
    VERIFY(NULL != pbData);

// check if SIF is initialized ?
    if (!_u4SIF_Init)
    {
        if ((_u4SIF_SDA == 0) || (_u4SIF_SCL == 0))
            return (FALSE);
        I2CSW_Init(_u4SIF_SDA, _u4SIF_SCL);
    }

// wait semaphore
    VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    bDevAddr &= 0xFE;

    bBitDelay = bOption & 0x07;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >>6 );
#if 0
    if ( (!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck))
    {
        return (FALSE);
    }
#else
    _Sifsw_SendStart(bDevAddr, bBitDelay);
#endif
    _Sifsw_Delay1us(bByteDelay);

    if ( (!_Sifsw_PutByte(bSubAddr, bBitDelay))&& (fgNeedACKCheck) )
    {
    //    return (FALSE);
        fgRetSts = FALSE;
        goto lbExit;
    }

    _Sifsw_SendStop(bBitDelay);

    _Sifsw_Delay1us(bByteDelay);
    bDevAddr |= 0x01;

    if ((!_Sifsw_SendStart(bDevAddr, bBitDelay))&&(fgNeedACKCheck))
    {
    //    return (FALSE);
        fgRetSts = FALSE;
        goto lbExit;
    }

//    SIF_SDALOW();
//    _Sifsw_Delay1us(bBitDelay * 4);
    _Sifsw_Delay1us(bByteDelay);

    while (bDataCount)
    {
        if (bDataCount == 1)
        {
            _Sifsw_GetByte(pbData++, SIF_FG_RANDREAD, bBitDelay);
        }
        else
        {
            _Sifsw_GetByte(pbData++, SIF_FG_SEQREAD, bBitDelay);
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataCount--;
    }

lbExit:
    _Sifsw_SendStop(bBitDelay);

// release semaphore
    VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);

//    return (TRUE);
    return fgRetSts;
}

//-------------------------------------------------------------------------
/** I2CSW_WriteDataNoSubAddr
 * I2C write command (no ack type)
 *
 * @param bDevAddr device address
 * @param bDataCount numbber of bytes to be sent out
 * @param pbData pointer of data to be sent out
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @param bOption bit 7: Reserve
 * @param bOption bit 9: Not to send stop bit at the end of transaction
 * @param bOption bit10: Not to send start bit at the beginning of transaction
 * @return 1 or 0
 * @retval > 0 write success, number of bytes sent successfully
 * @retval = 0 write fail
 */
//-------------------------------------------------------------------------
UINT8 I2CSW_WriteDataNoSubAddr(UINT8 bDevAddr, UINT8 bDataCount, UINT8 *pbData, UINT32 bOption, UINT16 u2Delay,UINT16 u2SyncTimeout)
{
UINT8 bBitDelay;
UINT8 bByteDelay;
BOOL fgNeedACKCheck;
BOOL fgNoStart, fgReStart;
BOOL fgNoStop;
UINT8 bDataLeftCount = bDataCount;

    //ASSERT(pbData);
    VERIFY(NULL != pbData);

// check if SIF is initialized ?
    if (!_u4SIF_Init)
    {
        if ((_u4SIF_SDA == 0) || (_u4SIF_SCL == 0))
            return (FALSE);
        I2CSW_Init(_u4SIF_SDA, _u4SIF_SCL);
    }

// wait semaphore
    VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    bDevAddr &= 0xFE;

//  bBitDelay = bOption & 0x07;
    bBitDelay = u2Delay;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >> 6);
    fgNoStart = ((bOption & 0x400)>> 10);
    fgNoStop = ((bOption & 0x200) >> 9);
    fgReStart = ((bOption & 0x800) >> 11);

    if (!fgNoStart)                     // START BIT
    {
    UINT8 bDevAddrTmp = bDevAddr;

        if (fgReStart)
            bDevAddrTmp = 0;

        if ( (!_Sifsw_SendStart(bDevAddrTmp, bBitDelay)) && (fgNeedACKCheck))
        {
        //    _Sifsw_SendStop(bBitDelay);     // No ack and exit --> release bus first
        //    return (0);                     // target device no response
            bDataCount = bDataLeftCount;
            goto lbExit;
        }
        _Sifsw_Delay1us(bByteDelay);
    }

    while (bDataLeftCount)
    {
        if ( (!_Sifsw_PutByte(*(pbData++), bBitDelay)) && (fgNeedACKCheck)) // Data Content Write
        {
        //    _Sifsw_SendStop(bBitDelay);     // No ack and exit --> release bus first
        //    return (bDataCount - bDataLeftCount);       // returns successul bytes
            goto lbExit;
        }
        _Sifsw_Delay1us(bByteDelay);
        bDataLeftCount--;
    }

lbExit:
    if (!fgNoStop)                          // STOP BIT
    {
        _Sifsw_SendStop(bBitDelay);         // release bus
    }

// release semaphore
    VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);

    return (bDataCount - bDataLeftCount);   // returns successul bytes
}

//-------------------------------------------------------------------------
/** I2CSW_ReadDataNoSubAddr
 * I2C read command (no ack type)
 *
 * @param bDevAddr device address
 * @param bDataCount numbber of bytes to be rad
 * @param pbData pointer of data to be rad
 * @param bOption options for this command
 * @param bOption bit 2-0: bit delay (us)
 * @param bOption bit 5-3: UINT8 delay (us)
 * @param bOption bit 6: need to check ACK
 * @return 1 or 0
 * @retval > 0 write success, number of bytes sent successfully
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 I2CSW_ReadDataNoSubAddr(UINT8 bDevAddr, UINT16 bDataCount, UINT8 *pbData, UINT32 bOption, UINT16 u2Delay,UINT16 u2SyncTimeout)
{
UINT8 bBitDelay;
UINT8 bByteDelay;
BOOL fgNeedACKCheck;
UINT32 bDataLeftCount = bDataCount;

    //ASSERT(pbData);
    VERIFY(NULL != pbData);

// check if SIF is initialized ?
    if (!_u4SIF_Init)
    {
        if ((_u4SIF_SDA == 0) || (_u4SIF_SCL == 0))
            return (FALSE);
        I2CSW_Init(_u4SIF_SDA, _u4SIF_SCL);
    }

// wait semaphore
    VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    bDevAddr &= 0xFE;

//  bBitDelay = bOption & 0x07;
    bBitDelay = u2Delay;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) >>6);

    bDevAddr |= 0x01;

    if ( (!_Sifsw_SendStart(bDevAddr, bBitDelay)) && (fgNeedACKCheck) )
    {
    //    _Sifsw_SendStop(bBitDelay);     // No ack and exit --> release bus first
    //    return (0);
        bDataCount = bDataLeftCount;
        goto lbExit;
    }
    _Sifsw_Delay1us(bByteDelay);

    while (bDataLeftCount)
    {
        if (bDataLeftCount == 1)
        {
            _Sifsw_GetByte(pbData++, SIF_FG_RANDREAD, bBitDelay);
        }
        else
        {
            _Sifsw_GetByte(pbData++, SIF_FG_SEQREAD, bBitDelay);
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataLeftCount--;
    }

lbExit:
    _Sifsw_SendStop(bBitDelay);         // release bus

// release semaphore
    VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);

    return (bDataCount - bDataLeftCount);   // returns successul bytes
}
