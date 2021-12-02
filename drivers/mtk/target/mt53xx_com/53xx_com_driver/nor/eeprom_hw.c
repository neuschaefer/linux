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
 * $Author: muran.xiang $
 * $Date: 2012/07/08 $
 * $RCSfile: eeprom_hw.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file eeprom_if.c
 *  This file is the implementation of EEPROM interface definition.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "drvcust_if.h"
#include "eeprom_if.h"
#include "nor_debug.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_timer.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif

#ifdef REPLACE_EEPROM_WITH_NOR
#include "nor_if.h"
#endif
LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// set EEPROM word size.
#define EEP_SIF_WORD_SIZE               (256)
/// EEPROM page size mask.
#define EEP_SIF_PAGE_MASK               (0xFF)
/// EEPROM MSB page size mask. This can support up to 64 kbits eeprom.
#define EEP_SIF_MSB_PAGE_MASK           (0xFF)

/// Device address LSB [b3, b2, b1] are from Data Offset MSB [A10,A9,A8].
#define EEP_GET_UPPER_ADDR(offset)      ((UINT8)(((offset) >> 7) & 0x0E))

/// Use OSAI function option
#if defined (CC_MTK_LOADER)
#define USE_OSAI            0
#else // defined (CC_MTK_LOADER)
#define USE_OSAI            1
#endif // defined (CC_MTK_LOADER)


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

/// ST M24C32 EEPROM decvice address.
static UINT8 _u1EEPROMDevBus  = 0;
static UINT8 _u1EEPROMDevAddr = 0xA0;

/// EEPROM decvice size (bytes).
static UINT32 _u4EEPROMSize = 0;

/// EEPROM decvice clock divider of 27 Mhz.
static UINT16 _u2EEPROMClkDiv = 0x200;

/// EEPROM Page Write size
static UINT8 _u1EEPROMPage = 8;

//EEPROM page write reset time
static UINT32 _u4EEPROMRstTime = 20;

//EEPROM enable retry 
static BOOL _fgEEPROMEnableRetry = FALSE;

static UINT32 _u4EEPROMSpeedInit = 0;
static UINT32 _u4EEPROMSizeInit = 0;
static UINT32 _u4EEPROMPageInit = 0;

/// Use OSAI function option
#if defined (CC_MTK_LOADER)
#else // defined (CC_MTK_LOADER)
static HANDLE_T _hUserSema     = NULL_HANDLE;
#endif // defined (CC_MTK_LOADER)

#ifdef CLI_SUPPORT


#define EEP_SUPPORT_TOTAL  5
static EEP_HISTORY eEP_HISTORY[EEP_SUPPORT_TOTAL];
#endif

#ifdef CC_MTK_LOADER
extern UINT8 * bEEPromBuf;
extern UINT32 u4EEPromBufSize;
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** EEPROM_SetSpeed
 *  Set EEPROM device I2C clk rate divider.
 *  @param I2C clk rate divider.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
#ifdef CLI_SUPPORT


VOID EEPROM_OpertionCount(VOID)
{
	INT32 i4Devnum,byte,max_write_byte=0,index=0;
	
	for(i4Devnum=0;i4Devnum<EEP_SUPPORT_TOTAL&& eEP_HISTORY[i4Devnum]._i4Devnum!=0;i4Devnum++)
		{
		for(byte =0;byte<=EEPROM_ARRAYINDEX;byte++)
			{
				if(eEP_HISTORY[i4Devnum]._i4writebytescount[byte] >max_write_byte)
					{
						max_write_byte=eEP_HISTORY[i4Devnum]._i4writebytescount[byte];
						index = byte;
					}
				Printf("u%dDev=0x%X, byte=0x%X, writebytescount=%d.\n",i4Devnum,
               		eEP_HISTORY[i4Devnum]._i4Devnum,
               		byte,
               		eEP_HISTORY[i4Devnum]._i4writebytescount[byte]
               		);
			}

		Printf("u%dDev=0x%X, max_write_byte=0x%X, writebytescount=%d.\n",i4Devnum,
               		eEP_HISTORY[i4Devnum]._i4Devnum,
               		max_write_byte,
               		eEP_HISTORY[i4Devnum]._i4writebytescount[index]
               		);
		
		}
		
}
#endif

VOID EEPROM_SetClkDiv(UINT16 u2Speed)
{
    if (u2Speed < 100)
    {
        LOG(1, "%s(%d) speed too fast.\n", __FILE__, __LINE__);
        return;
    }

    _u2EEPROMClkDiv = u2Speed;
}

//-------------------------------------------------------------------------
/** EEPROM_InitClkDiv
 *  get EEPROM device I2C clk rate divider.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID EEPROM_InitClkDiv(VOID)
{
    if (!_u4EEPROMSpeedInit)
    {
        _u2EEPROMClkDiv = (UINT16)DRVCUST_InitGet(eSystemEepromClkDiv);
        _u4EEPROMSpeedInit = 1;
    }
}

//-------------------------------------------------------------------------
/** EEPROM_Init
 *  Do necessary EEPROM initialization
 *  @param  VOID
 *  @retval   0        SUCCESS.
 *  @retval   <0    Error
 */
//-------------------------------------------------------------------------
INT16 EEPROM_Init(VOID)
{
    static BOOL fgEEPROM_Init=FALSE;
    UINT32 u4Var;
#if defined(CC_MTK_LOADER)
#else
    INT32 i4Ret;
#endif // defined(CC_MTK_LOADER)
    
    if (!fgEEPROM_Init )
    {
#if USE_OSAI
        // create semaphore
        VERIFY(x_sema_create(&_hUserSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif // USE_OSAI 

        SIF_Init();
        EEPROM_InitClkDiv();

#if defined(CC_MTK_LOADER)
        u4Var = DRVCUST_InitGet(eSystemEepromBus);
        _u1EEPROMDevBus = u4Var;

        u4Var = DRVCUST_InitGet(eSystemEepromAddress);
        _u1EEPROMDevAddr = u4Var;

        u4Var = DRVCUST_InitGet(eSystemEepromPageSize);
        _u1EEPROMPage = u4Var;

        u4Var = DRVCUST_InitGet(eSysEepromResetTime);
        _u4EEPROMRstTime = u4Var;

        u4Var = DRVCUST_InitGet(eSysEepromEnableRetry);
        _fgEEPROMEnableRetry = u4Var;
        
#else
        i4Ret = DRVCUST_OptQuery(eSystemEepromBus, &u4Var);
        if (i4Ret == 0)
        {
            _u1EEPROMDevBus = (UINT8) u4Var;
        }

        i4Ret = DRVCUST_OptQuery(eSystemEepromAddress, &u4Var);
        if (i4Ret == 0)
        {
            _u1EEPROMDevAddr = (UINT8) u4Var;
        }

        i4Ret = DRVCUST_OptQuery(eSystemEepromPageSize, &u4Var);
        if (i4Ret == 0)
        {
            _u1EEPROMPage = (UINT8) u4Var;
        }

        i4Ret = DRVCUST_OptQuery(eSysEepromResetTime, &u4Var);
        if (i4Ret == 0)
        {
            _u4EEPROMRstTime = (UINT8) u4Var;
        }

        i4Ret = DRVCUST_OptQuery(eSysEepromEnableRetry, &u4Var);
        if (i4Ret == 0)
        {
            _fgEEPROMEnableRetry = (UINT8) u4Var;
        }
#endif // defined(CC_MTK_LOADER)
        
        fgEEPROM_Init = TRUE;
    }
    return 0;
}


//-------------------------------------------------------------------------
/** EEPROM_SetSize
 *  Set EEPROM device size.
 *  @param device size in bytes
 *  @return  VOID.
 */
//-------------------------------------------------------------------------
VOID EEPROM_SetSize(UINT32 u4Size)
{
    _u4EEPROMSize = u4Size;
}


//-------------------------------------------------------------------------
/** EEPROM_GetSize
 *  get EEPROM device size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_GetSize(VOID)
{
    if (!_u4EEPROMSizeInit)
    {
        _u4EEPROMSize = DRVCUST_InitGet(eSystemEepromSize);
        _u4EEPROMSizeInit = 1;
    }

    return (INT32)_u4EEPROMSize;
}


//-------------------------------------------------------------------------
/** EEPROM_GetPage
 *  get EEPROM page write size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_GetPage(VOID)
{
    if (!_u4EEPROMPageInit)
    {
        _u1EEPROMPage = DRVCUST_InitGet(eSystemEepromPageSize);
        _u4EEPROMPageInit = 1;
    }

    return (INT32)_u1EEPROMPage;
}


//-------------------------------------------------------------------------
/** EEPROM_SetPage
 *  Set EEPROM page write size.
 *  @param device size in bytes
 *  @return  VOID.
 */
//-------------------------------------------------------------------------
VOID EEPROM_SetPage(UINT32 u4Size)
{
    _u1EEPROMPage = u4Size;
}


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifndef REPLACE_EEPROM_WITH_FLASH
static void _Delay(UINT32 u4Delay)
{
#if defined (CC_MTK_LOADER)
    HAL_Delay_us(u4Delay * 1000);
#else
    HAL_TIME_T rT0={0,0}, rT1={0,0}, rTDelta={0,0};
    HAL_GetTime(&rT0);
    do
    {
        x_thread_delay(5);
        HAL_GetTime(&rT1);    
        HAL_GetDeltaTime(&rTDelta, &rT0, &rT1);
    } while((rTDelta.u4Seconds * 1000000 + rTDelta.u4Micros) < u4Delay * 1000);
#endif
}

static VOID _EEPROM_SemaLock(VOID)
{
#if USE_OSAI
    // wait semaphore
    VERIFY(x_sema_lock(_hUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#endif // USE_OSAI   
}


static VOID _EEPROM_SemaUnlock(VOID)
{
#if USE_OSAI
    // release semaphore
    VERIFY(x_sema_unlock(_hUserSema) == OSR_OK);
#endif //USE_OSAI    
}

//-------------------------------------------------------------------------
/** _EEPROM_X_Read
 *  Generic EEPROM read function.
 *  @param  u8Offset     begin address to read.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     read data length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROM_X_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                            UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size)
{
    UINT32 u4Offset = (UINT32)u8Offset;

    UINT32 u4Index;
    UINT8 u1WordAddr;
    UINT16 u2ByteCnt;
    UINT16 u2Ret;


    // two bytes for two word address.
    UINT8 u1Data[2];
    UINT8 u1DevAddr;
    LOG(7, "eeprom para:%x(%x) u4MemLen:0x%08x\n", u4Offset, u4MemPtr, u4MemLen);

    if (u4MemLen == 0)
    {
        LOG(1, "%s(%d) u4MemLen:0x%08x\n", __FILE__, __LINE__, u4MemLen);
        return 1;
    }

    if ((u4Offset + u4MemLen) > u4Size)
    {
        LOG(1, "%s(%d) u4Offset+u4MemLen:0x%08x\n", __FILE__, __LINE__,
        u4Offset + u4MemLen);
        return 1;
    }


    SIF_Init();

    if( EEPROM_Init()!=0)
    {
        return 1;
    }

    _EEPROM_SemaLock();
    
    u4Index = 0;

    while (u4Index < u4MemLen)
    {
        if (u4Size <= 0x800)
        {
            // EEPROM word address in 0~255.
            u1WordAddr = u4Offset & EEP_SIF_PAGE_MASK;

            // residual data length in page
            u2ByteCnt = EEP_SIF_WORD_SIZE - u1WordAddr;
            // get min value between user's wanted length and residual data in page.
            u2ByteCnt = (UINT16)MIN((u4MemLen-u4Index), u2ByteCnt);

            // Device address LSB [b3, b2, b1] are from Data Offset MSB [A10,A9,A8].
            u1DevAddr = EEP_GET_UPPER_ADDR(u4Offset);

            // upper device address [b7 ~ b4] is fixed.
            u1DevAddr |= u1Addr;

            u2Ret = SIF_X_Read(u1Bus, u2ClkDiv, u1DevAddr, 1,
                               u1WordAddr, (UINT8 *)u4MemPtr, u2ByteCnt);

            if (u2Ret != u2ByteCnt)
            {
                LOG(3, "[EEPROM] Read WARN 0x%x at 0x%x: only %x expected %x\n", u1DevAddr, u4Offset, u2Ret, u2ByteCnt);
                _EEPROM_SemaUnlock();
                return 1;
            }
            // u2Ret update the actual read bytes.
            u2ByteCnt = u2Ret;
        }
        else
        {
            // EEPROM word address2 (MSB) in 0~32.
            u1Data[0] = (u4Offset >> 8) & EEP_SIF_MSB_PAGE_MASK;

            // EEPROM word address1 (LSB) in 0~255.
            u1Data[1] = (UINT8)(u4Offset & EEP_SIF_PAGE_MASK);

            u2ByteCnt = u4MemLen;

            u2Ret = SIF_X_Write(u1Bus, u2ClkDiv, u1Addr, 0, 0, u1Data, 2);

            // because write protected, this dummy write must be fail.
            if (u2Ret != 2)
            {
                _EEPROM_SemaUnlock();
                return 1;
            }
            // sequential read.
            u2Ret = SIF_X_Read(u1Bus, u2ClkDiv, u1Addr, 0,0,(UINT8 *)u4MemPtr, u2ByteCnt);

            if (u2Ret != u2ByteCnt)
            {
                LOG(3, "[EEPROM] Read WARN 0x%x at 0x%x: only %x expected %x\n", u1Addr, u4Offset, u2Ret, u2ByteCnt);                
                _EEPROM_SemaUnlock();
                return 1;
            }
        }

        u4Index += u2ByteCnt;
        u4Offset += u2ByteCnt;
        u4MemPtr += u2ByteCnt;
    }
	 _EEPROM_SemaUnlock();

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROM_X_Write
 *  Generic EEPROM write function.
 *  @param  u8Offset     begin address to write.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     writedata length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @param  i4SysWP     EEPROM write protect GPIO pin number
 *  @param  i4SysWPEnable EEPROM write protect enable polarity
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROM_X_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                             UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size,
                             INT32 i4SysWP, INT32 i4SysWPEnable)
{
    UINT32 u4Offset = (UINT32)u8Offset;
	#ifdef CLI_SUPPORT
	UINT32 Eepromoffset = u4Offset;
	#endif
    UINT32 u4Index;

    UINT16 u2ByteCnt;
    UINT16 u2Ret;
    INT32 fgSet;
    INT32 fgOldSet=-1;
    UINT32 u4Retry=0, u4Flag=0;
    UINT8 u1Data[34];
    UINT32 i;
    UINT8 *pData = (UINT8 *)u4MemPtr;
    UINT8 u1DevAddr;
    UINT8 u1WordAddr;

    if (u4MemLen == 0)
    {
        LOG(1, "%s(%d) u4MemLen:0x%08x\n", __FILE__, __LINE__, u4MemLen);
        return 1;
    }

    if ((u4Offset + u4MemLen) > u4Size)
    {
        LOG(1, "%s(%d) u4Offset+u4MemLen:0x%08x\n", __FILE__, __LINE__,
        u4Offset + u4MemLen);
        return 1;
    }

    SIF_Init();

    if( EEPROM_Init()!=0)
    {
        return 1;
    }

    _EEPROM_SemaLock();
    
    // disable write protect by GPIO.
    if ((i4SysWP >= 0) && (i4SysWPEnable >= 0))
    {
        u4Flag = 1;
#if !defined(CC_MT5365)
        VERIFY(BSP_PinGpioSel(i4SysWP) == 0);
#endif

        // query old WP status. If WP = 0, it may be in factory mode.
        fgOldSet = GPIO_Output(i4SysWP, NULL);

        if (fgOldSet == i4SysWPEnable)
        {
            fgSet = !i4SysWPEnable;
            VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
        }
    }

    u4Index = 0;

    while (u4Index < u4MemLen)
    {
        if (u4Size <= 0x800)
        {
            // EEPROM word address in 0~255.
            u1WordAddr = u4Offset & EEP_SIF_PAGE_MASK;

            // EEPROM should align the boundary length of 8 bytes when do page write.
            u2ByteCnt = _u1EEPROMPage - (u1WordAddr %_u1EEPROMPage);

            // Max EEPROM R/W length in one time = SIF hardware data port width.
            // Get min value between user's wanted length and EEPROM R/W unit.
            u2ByteCnt = (UINT16)MIN((u4MemLen-u4Index), u2ByteCnt);

            // Device address LSB [b3, b2, b1] are from Data Offset MSB [A10,A9,A8].
            u1DevAddr = EEP_GET_UPPER_ADDR(u4Offset);

            // upper device address [b7 ~ b4] is fixed.
            u1DevAddr |= u1Addr;
            
            u2Ret = SIF_X_Write(u1Bus, u2ClkDiv, u1DevAddr, 1,
                                u1WordAddr, (UINT8 *)u4MemPtr, u2ByteCnt);
			
            // handle EEPROM write error.
            if (u2Ret != u2ByteCnt)
            {
                u4Retry++;
                LOG(3, "[EEPROM] Write WARN 0x%x at 0x%x: only %x expected %x\n", u1DevAddr, u4Offset, u2Ret, u2ByteCnt);                

                // This is redundency disable write protection pin,
                // but it is useful when ARM or 8032 have bad operation on this pin
                if ((i4SysWP >= 0) && (i4SysWPEnable >= 0))
                {
#if !defined(CC_MT5365)          
                    VERIFY(BSP_PinGpioSel(i4SysWP) == 0);
#endif                
                    fgSet = !i4SysWPEnable;
                    VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
                }
                
                if (u4Retry > 10)
                {
                    if ((u4Flag) && (fgOldSet == i4SysWPEnable))
                    {
                        fgSet = i4SysWPEnable;
                        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
                    }
                    _EEPROM_SemaUnlock();
                    return 1;
                }
            }
            else
            {
                u4Retry = 0;
            }
        }
        else
        {
            // EEPROM word address2 (MSB) in 0~32.
            u1Data[0] = (u4Offset >> 8) & EEP_SIF_MSB_PAGE_MASK;

            // EEPROM word address1 (LSB) in 0~255.
            u1Data[1] = u4Offset & EEP_SIF_PAGE_MASK;

            // EEPROM should align the boundary length of 8 bytes when do page write.
            u2ByteCnt = _u1EEPROMPage - (u1Data[1] %_u1EEPROMPage);

            // Max EEPROM R/W length in one time = SIF hardware data port width.
            // Get min value between user's wanted length and EEPROM R/W unit.
            u2ByteCnt = (UINT16)MIN((u4MemLen-u4Index), u2ByteCnt);
			
			if(u2ByteCnt <= 32)
			{
	            // copy user's data.
	            for (i = 0; i < u2ByteCnt; i++)
	            {
	                u1Data[i + 2] = *(pData + u4Index + i);
	            }
			}
			else
			{
                 Printf("[EEPROM] _u1EEPROMPage is too large, more than 32\n");                
				 return 1;
			}

            u2Ret = SIF_X_Write(u1Bus, u2ClkDiv, u1Addr, 0, 0, u1Data, (UINT16) (u2ByteCnt + 2));

            // handle EEPROM write error.
            if (u2Ret != ((UINT16) (u2ByteCnt + 2)))
            {
                u4Retry++;
                LOG(3, "[EEPROM] Write WARN 0x%x at 0x%x: only %x expected %x\n", u1Addr, u4Offset, u2Ret, u2ByteCnt + 2);                               

                // This is redundency disable write protection pin,
                // but it is useful when ARM or 8032 have bad operation on this pin
                if ((i4SysWP >= 0) && (i4SysWPEnable >= 0))
                {
#if !defined(CC_MT5365)         
                    VERIFY(BSP_PinGpioSel(i4SysWP) == 0);
#endif
                    fgSet = !i4SysWPEnable;
                    VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
                }     
                
                if (u4Retry > 10)
                {
                    if ((u4Flag) && (fgOldSet == i4SysWPEnable))
                    {
                        fgSet = i4SysWPEnable;
                        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
                    }
                    _EEPROM_SemaUnlock();
                    return 1;
                }
                u2Ret = 0;
            }
            else
            {
                u4Retry = 0;
                // exclude 2 bytes of word address.
                u2Ret -= 2;
            }
        }

        // During internal write cycle, EEPROM disconnects itself from bus,
        // and writes a copy of the data from its internal latches  to the
        // memory cells.
        // Max write time (tw) is 10ms.
        // If SIF_Write get the NACK from slave, it will return the number of bytes
        // that SIF already write. User can continue to write the following data.
        // We use experimental value 20 ms.
        _Delay(_u4EEPROMRstTime);

        // u2Ret update the actual read bytes.
        u2ByteCnt = u2Ret;

        u4Index += u2ByteCnt;
        u4Offset += u2ByteCnt;
        u4MemPtr += u2ByteCnt;
    }

    // write protect by GPIO.
    if ((u4Flag) && (fgOldSet == i4SysWPEnable))
    {
        fgSet = i4SysWPEnable;
        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
    }
	#ifdef CLI_SUPPORT
	for(i=0;i<EEP_SUPPORT_TOTAL;i++)
		{
			if(eEP_HISTORY[i]._i4Devnum ==u1Addr)
				{
					UINT32 byte;
					for(byte=Eepromoffset ; byte<=Eepromoffset+u4MemLen;byte++)
					eEP_HISTORY[i]._i4writebytescount[byte]++;
					break;
				}
			if(!eEP_HISTORY[i]._i4Devnum)
				{
					UINT32 byte;
					eEP_HISTORY[i]._i4Devnum = u1Addr;
					for(byte=Eepromoffset ; byte<=Eepromoffset+u4MemLen;byte++)
					eEP_HISTORY[i]._i4writebytescount[byte]++;
					break;
				}

		}
	#endif
	_EEPROM_SemaUnlock();


    
    return 0;
}
#endif

//-------------------------------------------------------------------------
/** EEPROM_Read
 *  SYSTEM EEPROM read function.
 *  @param  u8Offset     begin address to read.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     read data length in bytes.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
#ifdef CC_MTK_LOADER
    UINT8 *buf;
    UINT8 i = 0;
    
    buf = (UINT8 *)u4MemPtr;
    if((bEEPromBuf == NULL) ||((u8Offset+u4MemLen) >= u4EEPromBufSize))
    {
        return 1;
    }
    else
    {
        for(i = 0; i<u4MemLen; i++)
        {
            buf[i] =  bEEPromBuf[u8Offset+i];
        }
    }
#else
    Printf("EEPROM_Read using error..................................!!!\n");
    return 1;
#endif
}


//-------------------------------------------------------------------------
/** EEPROM_Write
 *  SYSTEM EEPROM write function.
 *  @param  u8Offset     begin address to write.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     writedata length in bytes.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    Printf("EEPROM_Write using error..................................!!!\n");
    return 1;
}


//-------------------------------------------------------------------------
/** EEPROM_X_Read
 *  Generic EEPROM read function.
 *  @param  u8Offset     begin address to read.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     read data length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_X_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                    UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size)
{
#ifdef REPLACE_EEPROM_WITH_NOR
	return NOR_EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);
#else

#ifdef REPLACE_EEPROM_WITH_FLASH

#ifdef CC_UBOOT
    extern INT32 UBOOT_EEPROM_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
    return UBOOT_EEPROM_Read(DRVCUST_InitGet(eEepromDataMemOffset) + u8Offset, u4MemPtr, u4MemLen);
#else
    return NAND_EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);
#endif

#else

    INT32 i4Ret;
    INT32 i;
    
    EEPROM_Init();

    if(_fgEEPROMEnableRetry)
    {
        for (i=0; i<10; i++)
        {
            i4Ret = _EEPROM_X_Read(u8Offset, u4MemPtr, u4MemLen, u1Bus, u2ClkDiv, u1Addr, u4Size);
            if (i4Ret == 0)
            {
                return i4Ret;
            }
            else
            {
                Printf("_EEPROM_X_Read()  %d\n",i);
            }
            _Delay(_u4EEPROMRstTime);
        }
        Printf("_EEPROM_X_Read() failed\n");

        return 1;
    }
    else 
        return _EEPROM_X_Read(u8Offset, u4MemPtr, u4MemLen, u1Bus, u2ClkDiv, u1Addr, u4Size);
#endif   

#endif/*REPLACE_EEPROM_WITH_NOR*/
}


//-------------------------------------------------------------------------
/** EEPROM_X_Write
 *  Generic EEPROM write function.
 *  @param  u8Offset     begin address to write.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     writedata length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @param  i4SysWP     EEPROM write protect GPIO pin number
 *  @param  i4SysWPEnable EEPROM write protect enable polarity
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_X_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                     UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size, INT32 i4SysWP, INT32 i4SysWPEnable)
{
#ifdef REPLACE_EEPROM_WITH_NOR
		return NOR_EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);
#else

#ifdef REPLACE_EEPROM_WITH_FLASH

#ifdef CC_UBOOT
    ASSERT(0);
    return -1;
#else
    return NAND_EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);
#endif

#else

    INT32 i4Ret;
    INT32 i;
    
    EEPROM_Init();

    if(_fgEEPROMEnableRetry)
    {
        for (i=0; i<10; i++)
        {
            i4Ret = _EEPROM_X_Write(u8Offset, u4MemPtr, u4MemLen,
                             u1Bus, u2ClkDiv, u1Addr, u4Size, i4SysWP, i4SysWPEnable);
            if (i4Ret == 0)
            {
                return i4Ret;
            }
            else
            {
                Printf("_EEPROM_X_Write()  %d\n",i);
            }
            _Delay(_u4EEPROMRstTime);
        }
        Printf("_EEPROM_X_Write() failed\n");
        
        return 1;
    }
    else 
        return _EEPROM_X_Write(u8Offset, u4MemPtr, u4MemLen,
                             u1Bus, u2ClkDiv, u1Addr, u4Size, i4SysWP, i4SysWPEnable);
#endif

#endif/*REPLACE_EEPROM_WITH_NOR*/
}
