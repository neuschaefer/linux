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
 * $RCSfile: sif_sw_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_sw_if.c
 *  This file implement serial interface SIF function by software controlled GPIO pin.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "sif_debug.h"
#include "sif_hw.h"
#include "sif_sw_drvif.h"

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
// Constant definitions
//---------------------------------------------------------------------------
#define SIF_BASE           BIM_BASE
#define SIF_REG_LENGTH      0

#define SIF_HDCP_SIZE  320

// HDMI EDID 1 EERPOM GPIO pin configuration.
#ifndef SIF_EDID1_SDA
#define SIF_EDID1_SDA  (35)
#endif
#ifndef SIF_EDID1_SCL
#define SIF_EDID1_SCL  (36)
#endif

// HDMI EDID 2 EERPOM GPIO pin configuration.
#ifndef SIF_EDID2_SDA
#define SIF_EDID2_SDA  (37)
#endif
#ifndef SIF_EDID2_SCL
#define SIF_EDID2_SCL  (38)
#endif

// HDMI EDID 3 EERPOM GPIO pin configuration.
#ifndef SIF_EDID3_SDA
#define SIF_EDID3_SDA  (44)
#endif
#ifndef SIF_EDID3_SCL
#define SIF_EDID3_SCL  (45)
#endif

#define SIF_EDID_SIZE  256

#define SIF_EEP_DEVICE_ADDR  0xA0
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
static UINT32 _u4SIF_Init;

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
    INT32 i4Val = 0;
    GPIO_Enable((INT32)_u4SIF_SDA , &i4Val );
    return GPIO_Input((INT32)_u4SIF_SDA);
}

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
    INT32 fgSet = 0;

    VERIFY(fgSet == GPIO_Output((INT32)_u4SIF_SDA, &fgSet));
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
    INT32 fgSet = 1;

    VERIFY(fgSet == GPIO_Output((INT32)_u4SIF_SDA, &fgSet));
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
    INT32 fgSet = 0;

    VERIFY(fgSet == GPIO_Output((INT32)_u4SIF_SCL, &fgSet));
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
    INT32 fgSet = 1;

    VERIFY(fgSet == GPIO_Output((INT32)_u4SIF_SCL, &fgSet));
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
        for (u4Loop = 0; u4Loop < 10; u4Loop++)
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
        _Sifsw_Delay1us(bBitDelay);

        bBitMask = bBitMask >> 1;
    }

    // step 2 : slave acknowledge check
    //SIF_SDAHIGH();
    SIF_SDALOW();
    _Sifsw_Delay1us(bBitDelay);

    bBitMask = SIF_ACK_DELAY;

    do
    {
        bData = SIF_READSDA();
    } while (bData && --bBitMask);

    //_Sifsw_Delay1us(bBitDelay);
    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    //_Sifsw_Delay1us(bDelay);
    SIF_SCLLOW(); // end ACK polling
    _Sifsw_Delay1us(bBitDelay);

    if (bBitMask)
    {
        return (TRUE); // return TRUE if ACK detected
    }
    else
    {
        _Sifsw_SendStop(bBitDelay);
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

    ASSERT(prValue);

    *prValue = 0;
    SIF_SDAHIGH();
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

    SIF_SDAHIGH();
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
    SIF_SDAHIGH();

    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLHIGH();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SDALOW();
    _Sifsw_Delay1us(bBitDelay);

    SIF_SCLLOW();
    _Sifsw_Delay1us(bBitDelay);

    return (_Sifsw_PutByte(bDevAddr, bBitDelay));
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** SIFSW_Init
 * I2C initialization routine
 *
 * @param u4SDA 	GPIO number as SDA.
 * @param u4SCL 	GPIO number as SCL.
 * @return NONE
 */
//-------------------------------------------------------------------------
void SIFSW_Init(UINT32 u4SDA, UINT32 u4SCL)
{
    if ((_u4SIF_SDA != u4SDA) || (_u4SIF_SCL != u4SCL))
    {
        // Get old pin number, function, and store it.
        _i4SIF_SDA_Func = BSP_PinGpioGet((INT32)u4SDA, (INT32 *)&_i4SIF_SDA_Pin);
        _i4SIF_SCL_Func = BSP_PinGpioGet((INT32)u4SCL, (INT32 *)&_i4SIF_SCL_Pin);

        _u4SIF_SDA = u4SDA;
        
        // set SDA as GPIO by select pin mux
        VERIFY(BSP_PinGpioSel((INT32)_u4SIF_SDA) == 0);

        _u4SIF_SCL = u4SCL;
        // set SCL as GPIO.
        VERIFY(BSP_PinGpioSel((INT32)_u4SIF_SCL) == 0);
    }

    _u4SIF_Init = TRUE;
    
    SIF_SCLHIGH();
    _Sifsw_Delay1us(2);
    SIF_SDAHIGH();
    _Sifsw_Delay1us(2);
}

//-------------------------------------------------------------------------
/** SIFSW_Remove
 * I2C deactivate routine: restore sda and scl pin attribute.
 *
 * @param u4SDA 	GPIO number as SDA.
 * @param u4SCL 	GPIO number as SCL.
 * @return NONE
 */
//-------------------------------------------------------------------------
void SIFSW_Remove(void)
{
    if (!_u4SIF_Init)
    {
        return;
    }
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
/** SIFSW_WaitAckOK
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

BOOL SIFSW_WaitAckOK(UINT8 bDevAddr, UINT16 wDelayUs, UINT8 bRetry)
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
/** SIFSW_WriteData
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

UINT8 SIFSW_WriteData(UINT8 bDevAddr, UINT8 bSubAddr, UINT8 bDataCount, UINT8 *pbData,
                      UINT8 bOption)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = bOption & 0x07;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) > 0);

    if ((fgNeedACKCheck) && (!_Sifsw_SendStart(bDevAddr, bBitDelay)))
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);

    if ((fgNeedACKCheck) && (!_Sifsw_PutByte(bSubAddr, bBitDelay)))
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);

    while (bDataCount)
    {
        if ((fgNeedACKCheck) && (!_Sifsw_PutByte(*(pbData++), bBitDelay))) // Data Content Write
        {
            return (FALSE);                                                // Device Address exceeds the range
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataCount--;
    }

    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

//-------------------------------------------------------------------------
/** SIFSW_ReadData
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
UINT8 SIFSW_ReadData(UINT8 bDevAddr, UINT8 bSubAddr, UINT16 bDataCount, UINT8 *pbData,
                     UINT8 bOption)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = bOption & 0x07;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) > 0);

    if ((fgNeedACKCheck) && (!_Sifsw_SendStart(bDevAddr, bBitDelay)))
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);

    if ((fgNeedACKCheck) && (!_Sifsw_PutByte(bSubAddr, bBitDelay)))
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);
    bDevAddr |= 0x01;

    if ((fgNeedACKCheck) && (!_Sifsw_SendStart(bDevAddr, bBitDelay)))
    {
        return (FALSE);
    }

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

    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

//-------------------------------------------------------------------------
/** SIFSW_WriteDataNoSubAddr
 * I2C write command (no ack type)
 *
 * @param bDevAddr device address
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
UINT8 SIFSW_WriteDataNoSubAddr(UINT8 bDevAddr, UINT8 bDataCount, UINT8 *pbData, UINT8 bOption)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = bOption & 0x07;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) > 0);

    if ((fgNeedACKCheck) && (!_Sifsw_SendStart(bDevAddr, bBitDelay)))
    {
        return (FALSE);
    }

    _Sifsw_Delay1us(bByteDelay);

    while (bDataCount)
    {
        if ((fgNeedACKCheck) && (!_Sifsw_PutByte(*(pbData++), bBitDelay))) // Data Content Write
        {
            return (FALSE);                                                // Device Address exceeds the range
        }

        _Sifsw_Delay1us(bByteDelay);
        bDataCount--;
    }

    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

//-------------------------------------------------------------------------
/** SIFSW_ReadDataNoSubAddr
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
 * @retval = 1 read success
 * @retval = 0 read fail
 */
//-------------------------------------------------------------------------
UINT8 SIFSW_ReadDataNoSubAddr(UINT8 bDevAddr, UINT16 bDataCount, UINT8 *pbData, UINT8 bOption)
{
    UINT8 bBitDelay;

    UINT8 bByteDelay;
    BOOL fgNeedACKCheck;

    ASSERT(pbData);

    bDevAddr &= 0xFE;

    bBitDelay = bOption & 0x07;
    bByteDelay = ((bOption & 0x38) >> 3);
    fgNeedACKCheck = ((bOption & 0x40) > 0);

    bDevAddr |= 0x01;

    if ((fgNeedACKCheck) && (!_Sifsw_SendStart(bDevAddr, bBitDelay)))
    {
        return (FALSE);
    }

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

    _Sifsw_SendStop(bBitDelay);

    return (TRUE);
}

/** SifEDIDReadByte
 * EDID I2C read command (one type only)
 * @param u1EDID_ID device address
 * @param u1SubAddr sub-address
 * @param pbData pointer of data to be read
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
UINT8 SifEDIDReadByte(UINT8 u1EDID_ID, UINT8 u1SubAddr, UINT8 * pbData)
{
    INT32 i4Ret=0;
    switch( u1EDID_ID )
    {
    	case 1:
    		SIFSW_Init(SIF_EDID1_SDA, SIF_EDID1_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
    		SIFSW_Remove();		
    		break;
    	case 2:
    		SIFSW_Init(SIF_EDID2_SDA, SIF_EDID2_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
    		SIFSW_Remove();		
    		break;
    	case 3:
    		SIFSW_Init(SIF_EDID3_SDA, SIF_EDID3_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
    		SIFSW_Remove();		
    		break;
	default:
		LOG(3,"[SifEDIDReadByte] parameter u1EDID_ID wrong!\n");
		i4Ret = 0	;
    		break;
    }
    return i4Ret;
}

/** SifEDIDWriteByte
 * EDID I2C write command (one type only)
 * @param u1EDID_ID device address
 * @param u1SubAddr sub-address
 * @param pbData pointer of data to be read
 * @return 1 or 0
 * @retval = 1 read success
 * @retval = 0 read fail
 */
 /*
UINT8 SifEDIDWriteByte(UINT8 u1EDID_ID, UINT8 u1SubAddr, UINT8 * pbData)
{
    INT32 i4Ret=0;
    switch( u1EDID_ID )
    {
    	case 1:
    		SIFSW_Init(SIF_EDID1_SDA, SIF_EDID1_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
     		SIFSW_Remove();		
   		break;
    	case 2:
    		SIFSW_Init(SIF_EDID2_SDA, SIF_EDID2_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
     		SIFSW_Remove();		
   		break;
    	case 3:
    		SIFSW_Init(SIF_EDID3_SDA, SIF_EDID3_SCL);
		i4Ret = SIFSW_ReadData( SIF_EEP_DEVICE_ADDR,u1SubAddr,1 ,pbData, 0xFF  );
     		SIFSW_Remove();		
   		break;
	default:
		LOG(3,"[SifEDIDWriteByte] parameter u1EDID_ID wrong!\n");
		i4Ret = 0	;
    		break;
    }
    return i4Ret;
}*/
