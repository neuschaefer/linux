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
 * $RCSfile: eeprom_hw.c,v $
 * $Revision: #1 $
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

LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// EEPROM read/write SIF hardware unit size. 1st data byte as LSB address.
#define EEP_SIF_UNIT_SIZE    (4)
/// set EEPROM page size = SIF page size.
#define EEP_SIF_PAGE_SIZE      (256)
/// EEPROM page size mask. 
#define EEP_SIF_PAGE_MASK    (0xFF)
/// EEPROM MSB page size mask. This can support up to 64 kbits eeprom.
#define EEP_SIF_MSB_PAGE_MASK    (0x1F)

/// device address [b3, b2, b1] are from data address [A10, A9, A8].
#define EEP_GET_UPPER_ADDR(addr)    ((UINT8)(((addr) >> 7) & 0x0E))

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

/// EEPROM decvice clock divider of 27 Mhz.
static UINT16 _u2EEPROMClkDiv = 0x200;

/// ST M26C16 EEPROM decvice address.
static const UINT8 _u1EEPROMDev = 0xA0;
/// EEPROM decvice size (bytes). 
static UINT32 _u4EEPROMSize = 0;

static UINT32 _u4EEPROMSpeedInit = 0;
static UINT32 _u4EEPROMSizeInit = 0;
#ifdef __KERNEL__
#define EEPROM_ENABLE_RAMEMULATED_ROM    
#endif
#ifdef EEPROM_ENABLE_RAMEMULATED_ROM
static BOOL _fgEEPROMShadowEnabled = TRUE; 
static BOOL  _fgEEPROMShadowInitialed = FALSE;
static UINT8* _arcEEPROMShadowMemory;       //static char _arcEEPROMShadowMemory[(0x8000)];  
#endif
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifdef EEPROM_ENABLE_RAMEMULATED_ROM
/*static*/ INT32 _EEPROM_ShadowCheckAndInit(void)
{
    UINT32 u4EEP_Size ;
    UINT32 i;

    if( !_fgEEPROMShadowInitialed )
    {
        u4EEP_Size = EEPROM_GetSize();        
        VERIFY( ((VOID*)_arcEEPROMShadowMemory) = x_mem_alloc( u4EEP_Size) );
        for(i=0;i<u4EEP_Size;i++)
        {
            _arcEEPROMShadowMemory[i]=0xFF;
        }
        _fgEEPROMShadowInitialed = TRUE;
   }
#if 0   //debug
    for(i=0;i<u4EEP_Size;i++)
    {
        if( i%16 == 0 )
        {
            Printf("\n0x%08X: ", i);
        }

        Printf("%02X ",_arcEEPROMShadowMemory[i]);
    }
#endif
    return (_fgEEPROMShadowInitialed?1:0);
}

/*static*/ INT32 _EEPROM_ShadowRead(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Index;
    UINT32 u4Offset = (UINT32)u8Offset;    
    UINT8 * pu1EEPShadow ;

    _EEPROM_ShadowCheckAndInit();
    
    //parameter, boundry checking
    if (u4MemLen == 0)
    {
        LOG(1, "[_EEPROM_ShadowRead] %s(%d) u4MemLen:0x%08x\n", __FILE__, __LINE__, u4MemLen);
        return 1;
    }
    if ((u4Offset + u4MemLen) > (UINT32)EEPROM_GetSize())
    {
        LOG(1, "[_EEPROM_ShadowRead] %s(%d) u4Offset+u4MemLen:0x%08x\n", __FILE__, __LINE__,
        u4Offset + u4MemLen);
        return 1;
    }

    pu1EEPShadow = _arcEEPROMShadowMemory    ;
    
    for( u4Index=0; u4Index<u4MemLen; u4Index++)
    {
        *((UINT8 *)u4MemPtr + u4Index) = *(pu1EEPShadow+u8Offset+u4Index) ;
    }

    return 0;        
}
/*static*/ INT32 _EEPROM_ShadowWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Index;
    UINT32 u4Offset = (UINT32)u8Offset;    
    UINT8 * pu1EEPShadow;

    _EEPROM_ShadowCheckAndInit();

    //parameter, boundry checking
    if (u4MemLen == 0)
    {
        LOG(1, "[_EEPROM_ShadowRead] %s(%d) u4MemLen:0x%08x\n", __FILE__, __LINE__, u4MemLen);
        return 1;
    }
    if ((u4Offset + u4MemLen) > (UINT32)EEPROM_GetSize())
    {
        LOG(1, "[_EEPROM_ShadowRead] %s(%d) u4Offset+u4MemLen:0x%08x\n", __FILE__, __LINE__,
        u4Offset + u4MemLen);
        return 1;
    }

    pu1EEPShadow = _arcEEPROMShadowMemory    ;
    
    for( u4Index=0; u4Index<u4MemLen; u4Index++)
    {
        *(pu1EEPShadow+u8Offset+u4Index) =  *((UINT8 *)u4MemPtr + u4Index);
    }    
    return 0;        
}
#endif
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
#ifdef EEPROM_ENABLE_RAMEMULATED_ROM
INT32 EEPROM_SetShadowMode( INT32 * pi4_mode )
{
    ASSERT( (pi4_mode == NULL) || ( *pi4_mode ==0 ) || ( *pi4_mode ==1 ));
    if( pi4_mode != NULL)
    {
      if(*pi4_mode==1)
      {
        _fgEEPROMShadowEnabled= TRUE;
      }
      else  //*pi4_mode==0
      {
        _fgEEPROMShadowEnabled= FALSE;
      }
    }
    return (_fgEEPROMShadowEnabled?1:0);
}

INT32 EEPROM_GetShadowMode(void)
{
      return (_fgEEPROMShadowEnabled?1:0);
}

#endif
//-------------------------------------------------------------------------
/** EEPROM_SetSpeed
 *  Set EEPROM device I2C clk rate divider.
 *  @param I2C clk rate divider.
 *  @return  VOID
 */
//-------------------------------------------------------------------------
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
/** EEPROM_Read
 *  EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.		
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Offset = (UINT32)u8Offset;

    UINT32 u4Index;
    UINT8 u1WordAddr;
    UINT16 u2ByteCnt;
    UINT16 u2Ret;
    
    // two bytes for two word address.
    UINT8 u1Data[2];
    UINT8 u1DevAddr;

#ifdef EEPROM_ENABLE_RAMEMULATED_ROM
    if ( EEPROM_GetShadowMode() )
    {
        return _EEPROM_ShadowRead( u8Offset,  u4MemPtr,  u4MemLen);
    }
#endif

    if (u4MemLen == 0)
    {
        LOG(1, "%s(%d) u4MemLen:0x%08x\n", __FILE__, __LINE__, u4MemLen);
        return 1;
    }

    if ((u4Offset + u4MemLen) > (UINT32)EEPROM_GetSize())
    {
        LOG(1, "%s(%d) u4Offset+u4MemLen:0x%08x\n", __FILE__, __LINE__,
        u4Offset + u4MemLen);
        return 1;
    }


    SIF_Init();

    EEPROM_InitClkDiv();
    
    u4Index = 0;

    while (u4Index < u4MemLen)
    {
        // EEPROM word address in 0~255.
        u1WordAddr = u4Offset & EEP_SIF_PAGE_MASK;

        // residual data length in page
        u2ByteCnt = EEP_SIF_PAGE_SIZE - u1WordAddr;
        // get min value between user's wanted length and residual data in page.
        u2ByteCnt = (UINT16)MIN((u4MemLen-u4Index), u2ByteCnt);

        if (EEPROM_GetSize() <= 0x800)
        {
            // lower device address [b3, b2, b1] are from address [A10, A9, A8].
            u1DevAddr = EEP_GET_UPPER_ADDR(u4Offset);

            // upper device address [b7 ~ b4] is fixed.
            u1DevAddr |= _u1EEPROMDev;

            u2Ret = SIF_Read(_u2EEPROMClkDiv, u1DevAddr, u1WordAddr,
        (UINT8 *)u4MemPtr, u2ByteCnt);

            if (u2Ret != u2ByteCnt)
            {
//                UNUSED(PDWNC_WriteErrorCodeBuf(ERR_I2C_SYS_EEPROM));
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
            u1Data[1] = u4Offset & EEP_SIF_PAGE_MASK;

            // send device and word address first. dummy write.
            u2Ret = SIF_WriteNoSubAddr(_u2EEPROMClkDiv, _u1EEPROMDev, u1Data, 2);

            // because write protected, this dummy write must be fail.
            if (u2Ret != 2)
            {
                return 1;
            }
            // sequential read.
            u2Ret = SIF_ReadNoSubAddr(_u2EEPROMClkDiv, _u1EEPROMDev, (UINT8 *)u4MemPtr, u2ByteCnt);
            if (u2Ret != u2ByteCnt)
            {
//                UNUSED(PDWNC_WriteErrorCodeBuf(ERR_I2C_SYS_EEPROM));
                return 1;
            }
        }

        u4Index += u2ByteCnt;
        u4Offset += u2ByteCnt;
        u4MemPtr += u2ByteCnt;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** EEPROM_Write
 *  EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.		
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Offset = (UINT32)u8Offset;

    UINT32 u4Index;

    UINT16 u2ByteCnt;
    UINT16 u2Ret;
    INT32 fgSet;
    INT32 fgOldSet;    
    UINT32 u4Retry = 0;
    // two bytes for two word address.
    UINT8 u1Data[EEP_SIF_UNIT_SIZE + 2];
    UINT32 i;
    UINT8 *pData = (UINT8 *)u4MemPtr;
    UINT8 u1DevAddr;
    UINT8 u1WordAddr;
    INT32 i4SysWP;
    INT32 i4SysWPEnable;

#ifdef EEPROM_ENABLE_RAMEMULATED_ROM
    if ( EEPROM_GetShadowMode() )
    {
        return _EEPROM_ShadowWrite( u8Offset,  u4MemPtr,  u4MemLen);
    }
#endif

    if (u4MemLen == 0)
    {
        LOG(1, "%s(%d) u4MemLen:0x%08x\n", __FILE__, __LINE__, u4MemLen);
        return 1;
    }

    if ((u4Offset + u4MemLen) > (UINT32)EEPROM_GetSize())
    {
        LOG(1, "%s(%d) u4Offset+u4MemLen:0x%08x\n", __FILE__, __LINE__,
        u4Offset + u4MemLen);
        return 1;
    }

    i4SysWP = (INT32)DRVCUST_InitGet(eSysEepromWPGpio);
    i4SysWPEnable = (INT32)DRVCUST_InitGet(eSysEepromWPEnablePolarity);

    // disable write protect by GPIO.
    VERIFY(BSP_PinGpioSel(i4SysWP) == 0);
    
    // query old WP status. If WP = 0, it may be in factory mode.
    fgOldSet = GPIO_Output(i4SysWP, NULL);
   
    if (fgOldSet == i4SysWPEnable)
    {
        fgSet = !i4SysWPEnable;
        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
    }
    
    SIF_Init();

    EEPROM_InitClkDiv();

    u4Index = 0;

    while (u4Index < u4MemLen)
    {
        if (EEPROM_GetSize() <= 0x800)
        {
            // EEPROM word address in 0~255.
            u1WordAddr = u4Offset & EEP_SIF_PAGE_MASK;

            // EEPROM should align the boundary length of 8 bytes when do page write.
            u2ByteCnt = EEP_SIF_UNIT_SIZE - (u1WordAddr %EEP_SIF_UNIT_SIZE);

            // Max EEPROM R/W length in one time = SIF hardware data port width.
            // Get min value between user's wanted length and EEPROM R/W unit.
            u2ByteCnt = (UINT16)MIN((u4MemLen-u4Index), u2ByteCnt);

            // lower device address [b3, b2, b1] are from address [A10, A9, A8].
            u1DevAddr = EEP_GET_UPPER_ADDR(u4Offset);
            // upper device address [b7 ~ b4] is fixed.
            u1DevAddr |= _u1EEPROMDev;

            u2Ret = SIF_Write(_u2EEPROMClkDiv, u1DevAddr, u1WordAddr,
        (UINT8 *)u4MemPtr, u2ByteCnt);

            // handle EEPROM write error.
            if (u2Ret != u2ByteCnt)
            {
                u4Retry++;
                if (u4Retry > 10)
                {
                    if (fgOldSet == i4SysWPEnable)
                    {                                        
                        fgSet = i4SysWPEnable;
                        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
                    }
//                    UNUSED(PDWNC_WriteErrorCodeBuf(ERR_I2C_SYS_EEPROM));
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
            u2ByteCnt = EEP_SIF_UNIT_SIZE - (u1Data[1] %EEP_SIF_UNIT_SIZE);

            // Max EEPROM R/W length in one time = SIF hardware data port width.
            // Get min value between user's wanted length and EEPROM R/W unit.
            u2ByteCnt = (UINT16)MIN((u4MemLen-u4Index), u2ByteCnt);

            // copy user's data.
            for (i = 0; i < u2ByteCnt; i++)
            {
                u1Data[i + 2] = *(pData + u4Index + i);
            }

            u2Ret = SIF_WriteNoSubAddr(_u2EEPROMClkDiv, _u1EEPROMDev, u1Data, u2ByteCnt + 2);

            // handle EEPROM write error.
            if (u2Ret != (u2ByteCnt + 2))
            {
                u4Retry++;
                if (u4Retry > 10)
                {
                    if (fgOldSet == i4SysWPEnable)
                    {
                        fgSet = i4SysWPEnable;
                        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
                    }    
//                    UNUSED(PDWNC_WriteErrorCodeBuf(ERR_I2C_SYS_EEPROM));
                    return 1;
                }
            }
            else
            {
                u4Retry = 0;
            }
            // exclude 2 bytes of word address.
            u2Ret -= 2;
        }

        // During internal write cycle, EEPROM disconnects itself from bus,
        // and writes a copy of the data from its internal latches  to the 
        // memory cells.
        // Max write time (tw) is 10ms.
        // If SIF_Write get the NACK from slave, it will return the number of bytes
        // that SIF already write. User can continue to write the following data.
        // We use experimental value 20 ms.
        x_thread_delay(20);

        // u2Ret update the actual read bytes.
        u2ByteCnt = u2Ret;

        u4Index += u2ByteCnt;
        u4Offset += u2ByteCnt;
        u4MemPtr += u2ByteCnt;
    }

    // write protect by GPIO.
    if (fgOldSet == i4SysWPEnable)
    {
        fgSet = i4SysWPEnable;
        VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
    }
    return 0;
}

