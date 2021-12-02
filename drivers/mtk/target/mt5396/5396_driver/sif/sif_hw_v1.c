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
 * $RCSfile: sif_hw_v1.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_hw_v1.c
 *  This file implement serial interface SIF function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "sif_debug.h"
#include "sif_hw.h"
#include "sif_isr.h"
#include "sif_if.h"
#include "drvcust_if.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_bim.h"
#include "x_os.h"
LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#ifdef NDEBUG
#define SIFHW_DEBUG      0
#else
#define SIFHW_DEBUG     1
#endif

/// define SIF_NON_OSAI not to use OSAI.
#ifdef CC_MTK_LOADER
#define SIF_NON_OSAI
#define SIF_POLLING_MODE
#endif /* CC_5381_LOADER */

/// Use polling status instead of interrupt handle SIF read/write.
// For MT5395, use interrupt mode by default
// #define SIF_POLLING_MODE

/// Retry machanism.
//#define SIF_RETRY_MODE

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// Max 256 bytes in a given word address.
#define SIF_BLK_SIZE      ((UINT32)256)

/// SIF has 2 data port register. Total 8 bytes, so page read/write unit is 8 bytes.
#define SIF_PAGE_SIZE   ((UINT32)8)

/// Max time of waiting SIF interrupt. Unit is ms.
#define SIF_SEMA_TIMEOUT   ((UINT32)200)

/// random read/write mode.
#define SIF_RANDOM_MODE   ((UINT32)0)
/// current address read/write mode.
#define SIF_CURRENT_MODE   ((UINT32)1)

/// Max retry count.
#ifdef SIF_RETRY_MODE
#define SIF_RETRY_COUNT      ((UINT32)10)
#endif
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
/** SIF device parameter.
 *  This data structure is used to store user's SIF parameter.
 */

typedef struct _SIFDEVPARAM_T
{
    UINT16 u2ClkDiv;  ///< parameter of divider to divide 27 Mhz for SCL. 12 bits width.
    UINT8 u1DevAddr;  ///< device address: 7 bits width.
    UINT32 u4WordAddrNum; ///< number of word address:  2 bits width, 0, 1, 2, 3.
    UINT32 u4WordAddr; ///< word address:  24 bits width.
    UINT8 *pu1Buf;    ///< point to user's buffer.
    UINT16 u2ByteCnt; ///< the data length that user want to read/write.
    UINT16 u2ProcCnt; ///< the data length that SIF read/write completely.
    BOOL fgTuner;       ///< output pins are TUNER_DATA & TUNER_CLK.
} SIFDEVPARAM_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#if SIFHW_DEBUG
#define SIF_WRITE32(offset, value)       _Sif_WriteReg(offset, value, __FILE__, __LINE__)
#define SIF_READ32(offset)               _Sif_ReadReg(offset, __FILE__, __LINE__)
#else /* SIFHW_DEBUG */
#define SIF_WRITE32(offset, value)       IO_WRITE32(BIM_BASE, (offset), (value))
#define SIF_READ32(offset)               IO_READ32(BIM_BASE, (offset))
#endif /* SIFHW_DEBUG */

/// Mask device address ACK bit, [9].
#define MASK_DEV_ADDR_ACK                  ((UINT32)0x0200)
/// Mask word address ACK bit, [8]. 
#define MASK_WORD_ADDR_ACK             ((UINT32)0x0100)
/// Mask data ACK bits, [7:0] 
#define MASK_DATA_ACK                          ((UINT32)0x00FF)
/// Mask data ACK bit. 
#define MASK_ACK                                  ((UINT32)0x0001)

/// SIF operation fail error code.
#define E_SIF_FAIL                                  ((UINT32)0xFF)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Forward declarations
//---------------------------------------------------------------------------
static VOID SIF_V1_Init(VOID);
static UINT16 SIF_V1_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                  UINT8 *pu1Buf, UINT16 u2ByteCnt);
static UINT16 SIF_V1_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr,
                   UINT8 *pu1Buf, UINT16 u2ByteCnt);
#ifdef CC_CLI
static VOID SIF_V1_Stop(VOID);
#endif
static VOID SIF_V1_ISRHandler(UINT16 u2Vector);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
/// SIF initialized flag.

static BOOL _fgSifInit = FALSE;

/// set true when call sif before OSAI initialize.
static BOOL _fgSifNoOSAI = FALSE;

#ifndef SIF_NON_OSAI
/// SIF multi-task protection semaphore.
static HANDLE_T _hSifUserSema = NULL_HANDLE;
#endif /* SIF_NON_OSAI */


/// SIF user's parameter structure.

static SIFDEVPARAM_T _rSifDevParm;

/// SIF retry counter.
#ifdef SIF_RETRY_MODE
static UINT32 _u4SifRetryCount = 0;
#endif

static SIF_STRUCT_T _rSifStruct =
{
    1,
    {
        SIF_V1_Init,
#ifdef CC_CLI        	
        SIF_V1_Stop,
#else
        NULL,
#endif
        SIF_V1_X_Read,
        SIF_V1_X_Write,     
        SIF_V1_ISRHandler           
    }
};

static PFN_SIF_CFG_BEFORE_ACTION_FUNC _pfnSifCfgBeforeActionFunc = NULL;
static PFN_SIF_CFG_AFTER_ACTION_FUNC _pfnSifCfgAfterActionFunc = NULL;   
static PFN_SIF_CFG_PIN_SET_FUNC _pfnSifCfgPinSetFunc = NULL;   

static BOOL _fgEnableVSyncTrigger = FALSE;
	
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#if 0
static void _Sif_IICErrorHandler(void)
{
#ifdef RECORD_EEPROM_ERROR
    Printf("_Sif_IICErrorHandler: I2C Bus Error\n");
    PDWNC_WriteErrorCodeEeprom( ERR_I2C_GENERAL );
#endif
}
#endif


//-------------------------------------------------------------------------
/** _Sif_HandleISR
 *  Handle SIF module interrupt.
 *  @param  u2Vector	ISR vector number.
 *  @return  VOID
 */
//-------------------------------------------------------------------------

#ifndef SIF_POLLING_MODE

static VOID _Sif_HandleISR(UINT16 u2Vector)
{
    if (u2Vector == VECTOR_SERIAL)
    {
        // Clear SIF interrupt status.
        // This must be done to prevent ISR being triggerd immediately.
        IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M0_INTCLR, SIF_M0_INTCLR);
        IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M1_INTCLR, SIF_M1_INTCLR);
        VERIFY(BIM_ClearIrq(VECTOR_SERIAL));
        
        // release semaphore
        VERIFY(_SIF_ISR_Unlock() == OSR_OK);
    }
}
#endif

//-------------------------------------------------------------------------
/** _Sif_WriteReg
 *  SIF register write.
 *  @param  u4Offset	offset from BIM_BASE address..
 *  @return  actual number of bytes is read. 
 */
//-------------------------------------------------------------------------
#if SIFHW_DEBUG

static void _Sif_WriteReg(UINT32 u4Offset, UINT32 u4Value, INT8 *szFile, INT32 Line)
{
    LOG(_W, "%s(%d) write 0x%08X with val 0x%08X.\n", szFile, Line, u4Offset, u4Value);

    IO_WRITE32(BIM_BASE, u4Offset, u4Value);
}
#endif

//-------------------------------------------------------------------------
/** _Sif_ReadReg
 *  SIF register read.
 *  @param  u4Offset	offset from BIM_BASE address..
 *  @return  actual number of bytes is read. 
 */
//-------------------------------------------------------------------------
#if SIFHW_DEBUG

static UINT32 _Sif_ReadReg(UINT32 u4Offset, INT8 *szFile, INT32 Line)
{
    UINT32 u4Value;

    u4Value = IO_READ32(BIM_BASE, u4Offset);
    LOG(_R, "%s(%d) read 0x%08X getting val 0x%08X.\n", szFile, Line, u4Offset, u4Value);
    return u4Value;
}
#endif

//-------------------------------------------------------------------------
/** _Sif_CheckAck
 *  SIF check read/write ack.
 *  @param  u4Ack	ack bits [9:0].
 *  @param  u4RWMode	random read or sequential read(without sending word address).
 *  @retval  0~7:  In read process, (return value + 1) == the number of byte to be read.
 *  @retval  1~8:  In write process, (return value == the number of byte to be written.
 *  @retval  E_SIF_FAIL: fail. 
 */
//-------------------------------------------------------------------------

static UINT32 _Sif_CheckAck(UINT32 u4Ack, UINT32 u4RWMode)
{
    UINT32 u4AckCnt = 0;

    // check device address ack bit.
    if ((u4Ack & MASK_DEV_ADDR_ACK) == 0)
    {
        LOG(7, "ERR: Device Address NACK = 0x%X.\n", u4Ack);
        return E_SIF_FAIL;
    }

    // check word address ack bit.
    if (u4RWMode == SIF_RANDOM_MODE)
    {
        if ((u4Ack & MASK_WORD_ADDR_ACK) == 0)
        {
            LOG(7, "ERR: Word Address NACK = 0x%X.\n", u4Ack);
            return E_SIF_FAIL;
        }
    }

    u4Ack &= MASK_DATA_ACK;

    // 1: ACK, 0: NACK.
    while (u4Ack & MASK_ACK)
    {
        u4AckCnt++;
        u4Ack >>= 1;
    }

    return u4AckCnt;
}

//-------------------------------------------------------------------------
/** _Sif_ReadData
 *  SIF random or sequential read.
 *  @param  u4ReadCnt	Number of bytes to read.  0 < u4ReadCnt <= 8.
 *  @param  u4ReadMode	random read or sequential read(without sending word address).
 *  @retval   0			SIF interrupt time out.
 *  @retval   1~8		Actual number of bytes to be read.
 *  @retval   0xFF 		Slave return NACK.  
 */
//-------------------------------------------------------------------------
static UINT32 _Sif_ReadData(UINT8 u1ChannelID, UINT32 u4ReadCnt, UINT32 u4ReadMode)
{
    volatile UINT32 u4Reg;
    UINT32 u4Val;
    INT32 i4Ret;
    UINT32 au4Sm0D[2];
    UINT32 u4AckCnt;

    ASSERT(u4ReadCnt);
    ASSERT(u4ReadCnt <= 8);
    ASSERT((u4ReadMode == SIF_CURRENT_MODE) || (u4ReadMode == SIF_RANDOM_MODE));

#ifndef SIF_POLLING_MODE

    // Make sure SIF semaphore is in lock state.
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {
        i4Ret = _SIF_ISR_TryLock();
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    }        
#endif

    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M0_INTCLR, SIF_M0_INTCLR);
#ifndef SIF_POLLING_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, SIF_M0_INTEN, SIF_M0_INTEN);
#endif // SIF_POLLING_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M1_INTCLR, SIF_M1_INTCLR);
#ifndef SIF_POLLING_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, SIF_M1_INTEN, SIF_M1_INTEN);
#endif // SIF_POLLING_MODE

    // set device address.

    SIF_WRITE32(RW_SM1_DEV_REG, _rSifDevParm.u1DevAddr);

    // set clk divider, and enable SIF interface.
    u4Reg = CLK_DIV(_rSifDevParm.u2ClkDiv) | //AS_HDMI | HDMIA | 
                WORD_LEN(_rSifDevParm.u4WordAddrNum) |HD1T | SM0EN;

    // Activate upon V-Sync
    if(_fgEnableVSyncTrigger)
    {
        u4Reg |= VSYNC_TRI_EN(1);
    }

    if (_rSifDevParm.fgTuner)
    {
        u4Reg |= PSEL;
    }
    else
    {
        u4Reg &= ~PSEL;
    }
    
    SIF_WRITE32(RW_SM1_CTRL0_REG, u4Reg);

    if(_pfnSifCfgPinSetFunc)
    {        
        _pfnSifCfgPinSetFunc(u1ChannelID);
    }

    // set word address when random read.
    if (u4ReadMode == SIF_RANDOM_MODE)
    {
        SIF_WRITE32(RW_SM1_WD_REG, _rSifDevParm.u4WordAddr);
        // set random read mode and trigger SIF.
        u4Reg = PGLEN(u4ReadCnt - 1) | TRI;
    }
    else
    {
        // set current address read mode and trigger SIF.
        u4Reg = PGLEN(u4ReadCnt - 1) | CURAR | TRI;
    }

    SIF_WRITE32(RW_SM1_CTRL1_REG, u4Reg);

#ifndef SIF_POLLING_MODE

    // wait semaphore, will be unlock by _Siflsr().
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {
        i4Ret = _SIF_ISR_Lock();
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            return 0;
        }
        // read TRI and check.
        u4Reg = SIF_READ32(RW_SM1_CTRL1_REG);
        ASSERT((u4Reg & TRI) == 0);        
    }
    else
    {
        u4Reg = TRI;

        while (u4Reg & TRI)
        {
            u4Reg = SIF_READ32(RW_SM1_CTRL1_REG);
        }
    }
#else

    u4Reg = TRI;

    while (u4Reg & TRI)
    {
        u4Reg = SIF_READ32(RW_SM1_CTRL1_REG);
    }
#endif

    u4AckCnt = _Sif_CheckAck(V1_ACK(u4Reg), u4ReadMode);

    // check ack.
    if (u4AckCnt == E_SIF_FAIL)
    {
        return E_SIF_FAIL;
    }

    // In read process, (return value + 1) == the number of byte to be read.
    u4AckCnt++;

    if (u4ReadCnt > u4AckCnt)
    {
        LOG(0, "ERR: u4ReadCnt = %d, u4AckCnt = %d.\n", u4ReadCnt, u4AckCnt);
        return E_SIF_FAIL;
    }

    au4Sm0D[0] = SIF_READ32(RW_SM1_D0_REG);
    au4Sm0D[1] = SIF_READ32(RW_SM1_D1_REG);

    // copy SIF data to user's buffer.
    VERIFY(NULL != x_memcpy((VOID *)_rSifDevParm.pu1Buf, (const VOID *)au4Sm0D, u4ReadCnt));

    return u4ReadCnt;
}

//-------------------------------------------------------------------------
/** _Sif_WriteData
 *  SIF page or sequential write.
 *  @param  u4WriteCnt	Number of bytes to write.  0 < u4WriteCnt <= 8.
 *  @retval   0			SIF interrupt time out.
 *  @retval   1~8		Actual number of bytes to be written.
 *  @retval   0xFF 		Slave return NACK. 
 */
//-------------------------------------------------------------------------
static UINT32 _Sif_WriteData(UINT8 u1ChannelID, UINT32 u4WriteCnt)
{
    volatile UINT32 u4Reg;
    UINT32 u4Val;
    INT32 i4Ret;
    UINT32 au4Sm0D[2] =
    {
        0, 0
    };

    UINT32 u4AckCnt;

    ASSERT(u4WriteCnt);
    ASSERT(u4WriteCnt <= 8);

#ifndef SIF_POLLING_MODE

    // Make sure SIF semaphore is in lock state.
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {
        i4Ret = _SIF_ISR_TryLock();
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
    }

#endif

    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M0_INTCLR, SIF_M0_INTCLR);
#ifndef SIF_POLLING_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, SIF_M0_INTEN, SIF_M0_INTEN);
#endif // SIF_POLLING_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTCLR_REG, SIF_M1_INTCLR, SIF_M1_INTCLR);
#ifndef SIF_POLLING_MODE
    IO_WRITE32MSK(BIM_BASE, RW_PINTEN_REG, SIF_M1_INTEN, SIF_M1_INTEN);
#endif // SIF_POLLING_MODE

    // set device address.

    SIF_WRITE32(RW_SM1_DEV_REG, _rSifDevParm.u1DevAddr);

    // set word address.
    SIF_WRITE32(RW_SM1_WD_REG, _rSifDevParm.u4WordAddr);

    // set clk divider, and enable SIF interface.
    u4Reg = CLK_DIV(_rSifDevParm.u2ClkDiv) | //AS_HDMI | HDMIA | 
                  WORD_LEN(_rSifDevParm.u4WordAddrNum) |HD1T | SM0EN;

    // Activate upon V-Sync
    if(_fgEnableVSyncTrigger)
    {
        u4Reg |= VSYNC_TRI_EN(1);
    }
    
    if (_rSifDevParm.fgTuner)
    {
        u4Reg |= PSEL;
    }
    else
    {
        u4Reg &= ~PSEL;
    }
                  
    SIF_WRITE32(RW_SM1_CTRL0_REG, u4Reg);

    // Set Pin mux
    if(_pfnSifCfgPinSetFunc)
    {        
        _pfnSifCfgPinSetFunc(u1ChannelID);
    }
    
    // copy user's data to SIF data port .
    // Notice: RW_SM0_D0_REG register should be DWORD access.
    // copy to local buffer first.
    VERIFY(NULL != x_memcpy((VOID *)au4Sm0D, (const VOID *)_rSifDevParm.pu1Buf, u4WriteCnt));

    // Notice: RW_SM0_D0_REG register should be DWORD access.
    // copy local buffer to RW_SM0_D0_REG, RW_SM0_D1_REG register.
    if (u4WriteCnt <= 4)
    {
        // fill to data port 0.
        SIF_WRITE32(RW_SM1_D0_REG, au4Sm0D[0]);
    }
    else
    {
        // 4 < u4WriteCnt <= 8
        // fill to data port 0.
        SIF_WRITE32(RW_SM1_D0_REG, au4Sm0D[0]);
        // fill to data port 1.
        SIF_WRITE32(RW_SM1_D1_REG, au4Sm0D[1]);
    }

    LOG(7, "_WriteSIFPage D0 = 0x%08X, D1 = 0x%08X.\n", au4Sm0D[0], au4Sm0D[1]);

    // set page write mode and trigger SIF.
    u4Reg = PGLEN(u4WriteCnt - 1) | WR | TRI;
    SIF_WRITE32(RW_SM1_CTRL1_REG, u4Reg);

#ifndef SIF_POLLING_MODE

    // wait semaphore, will be unlock by _Siflsr().
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {
        i4Ret = _SIF_ISR_Lock();
        ASSERT((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));

        if (i4Ret == OSR_TIMEOUT)
        {
            return 0;
        }

        // read TRI and check.
        u4Reg = SIF_READ32(RW_SM1_CTRL1_REG);

        ASSERT((u4Reg & TRI) == 0);
    }
    else
    {
        u4Reg = TRI;

        while (u4Reg & TRI)
        {
            // read TRI and check.
            u4Reg = SIF_READ32(RW_SM1_CTRL1_REG);
        }
    }
#else

    u4Reg = TRI;

    while (u4Reg & TRI)
    {
        // read TRI and check.
        u4Reg = SIF_READ32(RW_SM1_CTRL1_REG);
    }
#endif

    // check ACK.
    if (_rSifDevParm.u4WordAddrNum > 0)
    {
        u4AckCnt = _Sif_CheckAck(V1_ACK(u4Reg), SIF_RANDOM_MODE);
    }
    else
    {
        u4AckCnt = _Sif_CheckAck(V1_ACK(u4Reg), SIF_CURRENT_MODE);
    }

    return u4AckCnt;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
SIF_STRUCT_T* SIF_V1_GetStruct(void)
{
    return &_rSifStruct;
}


//-------------------------------------------------------------------------
/** SIF_Init
 *  Initialize SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
VOID SIF_V1_Init(VOID)
{
    INT32 i4Ret;
    UINT32 u4Val;
#ifndef SIF_POLLING_MODE

//    x_os_isr_fct pfnOldIsr;
#endif

    // OSAI not initialize, do not call function in x_os.h.
    if (_fgSifNoOSAI)
    {
        return;
    }

    if (!_fgSifInit)
    {
        // Disable SIF interrupt
        VERIFY(BIM_DisableIrq(VECTOR_SERIAL));

        // Clear the SIF pending interrupt before enable it.        
        VERIFY(BIM_ClearIrq(VECTOR_SERIAL));

        if (DRVCUST_InitQuery(eSifCfgBeforeAction, (UINT32 *)(void *)&_pfnSifCfgBeforeActionFunc) != 0)
        {
            _pfnSifCfgBeforeActionFunc = NULL;
        }

        if (DRVCUST_InitQuery(eSifCfgAfterAction, (UINT32 *)(void *)&_pfnSifCfgAfterActionFunc) != 0)
        {
            _pfnSifCfgAfterActionFunc = NULL;
        }

        if (DRVCUST_InitQuery(eSifCfgPinSetFunc, (UINT32 *)(void *)&_pfnSifCfgPinSetFunc) != 0)
        {
            _pfnSifCfgPinSetFunc = NULL;
        }

        // Activate upon V-Sync
        i4Ret = DRVCUST_InitQuery(eSifEnableVsyncHigh, &u4Val);
        if(i4Ret == 0 && u4Val == 1)
        {
            _fgEnableVSyncTrigger = TRUE;
        }

        Printf("SIF_Master1: old design\n");

#ifndef SIF_NON_OSAI
        // create semaphore
        VERIFY(x_sema_create(&_hSifUserSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
#endif /* SIF_NON_OSAI */

// ISR initialization moved to sif_isr.c

        // init local variable

        _fgSifInit = TRUE;
    }
}

//-------------------------------------------------------------------------
/** SIF_Stop
 *  terminate SIF module.
 *  @param  VOID
 *  @return  VOID
 */
//-------------------------------------------------------------------------
#ifdef CC_CLI
VOID SIF_V1_Stop(VOID)
{
#ifndef SIF_NON_OSAI
    x_os_isr_fct pfnOldIsr;
#endif /* SIF_NON_OSAI */

    // OSAI not initialize, do not call function in x_os.h.
    if (_fgSifNoOSAI)
    {
        return;
    }

    ASSERT(_fgSifInit);

    if (_fgSifInit)
    {

#ifdef MT5351_INTCTRL

        // MT5351_INTCTRL must control module interrupt.
        // clear interrupt.

        SIF_WRITE32(0x028C, 0x3F);

        // disable interrupt.
        SIF_WRITE32(0x0284, 0x00);
#endif

#ifndef SIF_NON_OSAI
        // dereg ISR

        VERIFY(x_reg_isr(VECTOR_SERIAL, NULL, &pfnOldIsr) == OSR_OK);

        // create semaphore
        VERIFY(x_sema_delete(_hSifUserSema) == OSR_OK);
#endif /* SIF_NON_OSAI */

#ifndef SIF_POLLING_MODE

        // create semaphore

        _SIF_ISR_DeleteSema();
#endif

        _fgSifInit = FALSE;
    }
}
#endif


//-------------------------------------------------------------------------
/** SIF_X_Read
 *  read data with word address from SIF module.
 *  @param  u2CHannelID 	I2C Bus channel
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddrNum 	Serial interface word address Number (0~3).
 *  @param  u4WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @retval   > 0		Actual number of bytes to be written.
 *  @retval   = 0		Error
 */
//-------------------------------------------------------------------------
UINT16 SIF_V1_X_Read(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum, UINT32 u4WordAddr, 
								UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT32 u4ReadCnt;
    UINT32 u4Ret;	
    UINT32 u4ReadMode;
    UINT16 u2Ret = 0;
    UINT8	u1fgTuner=SIF_BUS_SYSTEM_I2C;

    ASSERT( u1WordAddrNum <=3 );
    ASSERT( u4WordAddr < (1<<24) );

    // parameter check: channle ID   
    if (u1CHannelID > SIF_BUS_SECOND_I2C)
    {
	LOG(7,"wrong u1CHannelID\n");
       goto SIF_READ_EXIT_MARK;
    }

    // parameter check: clk divide is only 12 bits.
    if (u2ClkDiv > (1 << 12))
    {
       goto SIF_READ_EXIT_MARK;
    }

    // parameter check: device address must be even.
    if ((u1DevAddr %2) != 0)
    {
       goto SIF_READ_EXIT_MARK;
    }

    // parameter check: user's buffer pointer.
    if ((pu1Buf == NULL) || (u2ByteCnt == 0))
    {
       goto SIF_READ_EXIT_MARK;
    }
    
    switch( u1CHannelID )
    {
    	case SIF_BUS_SYSTEM_I2C:
		LOG(7,"BUS_SYSTEM_I2C selected\n");
		u1fgTuner= FALSE; 
		break;
    	case SIF_BUS_TUNER_I2C:
		LOG(7,"BUS_TUNER_I2C selected\n");
		u1fgTuner= TRUE; 
		break;
    }

   if (  u1WordAddrNum== 0 )
   {
   	u4ReadMode = SIF_CURRENT_MODE;
   }
   else
   {
   	u4ReadMode = SIF_RANDOM_MODE;
   }
    // parameter check: max 256 bytes in a given word address.
    if ( u2ByteCnt > SIF_BLK_SIZE )
    {
	u2ByteCnt =SIF_BLK_SIZE;
    }

    // check if SIF is initialized ?
    if (!_fgSifInit)
    {
        SIF_V1_Init();
    }
    
#ifndef SIF_NON_OSAI
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {
        // wait semaphore
        VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
#endif /* SIF_NON_OSAI */

#ifdef SIF_RETRY_MODE       
    // prepare retry when r/w fail.
    _u4SifRetryCount = 0;
SIF_RETRY_MARK1:    
#endif
    // copy to user's parameter structure.
    _rSifDevParm.u2ClkDiv = u2ClkDiv;
    _rSifDevParm.u1DevAddr = u1DevAddr;
    _rSifDevParm.u4WordAddrNum = u1WordAddrNum ;			//
    _rSifDevParm.u4WordAddr = u4WordAddr;					//
    _rSifDevParm.pu1Buf = pu1Buf;
    _rSifDevParm.u2ByteCnt = u2ByteCnt;
    _rSifDevParm.u2ProcCnt = 0;
    _rSifDevParm.fgTuner = u1fgTuner;
    
    while (_rSifDevParm.u2ByteCnt > 0)
    {
        u4ReadCnt = MIN(_rSifDevParm.u2ByteCnt, SIF_PAGE_SIZE);

//        u4Ret = _Sif_ReadData(u4ReadCnt, SIF_RANDOM_MODE);
        u4Ret = _Sif_ReadData(u1CHannelID, u4ReadCnt, u4ReadMode);

        if (u4ReadCnt != u4Ret)
        {
#ifdef SIF_RETRY_MODE       
            // r/w fail, retry again.
            _u4SifRetryCount ++;
            if (_u4SifRetryCount < SIF_RETRY_COUNT)
            {
                goto SIF_RETRY_MARK1;
            }
#endif            
#ifndef SIF_NON_OSAI
            // OSAI not initialize, do not call function in x_os.h.
            if (!_fgSifNoOSAI)
            {
                // release semaphore            
                VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);
            }
#endif /* SIF_NON_OSAI */
            u2Ret = _rSifDevParm.u2ProcCnt;
            goto SIF_READ_EXIT_MARK;
        }

        _rSifDevParm.pu1Buf += u4ReadCnt;
        _rSifDevParm.u4WordAddr += u4ReadCnt;
        _rSifDevParm.u2ByteCnt -= u4ReadCnt;
        _rSifDevParm.u2ProcCnt += u4ReadCnt;
    }

#ifndef SIF_NON_OSAI
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {
        // release semaphore
        VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);
    }
#endif /* SIF_NON_OSAI */
    u2Ret = u2ByteCnt;

SIF_READ_EXIT_MARK:
    return u2Ret;
}

//-------------------------------------------------------------------------
/** SIF_X_Write
 *  write data with word address to SIF module.
 *  @param  u2CHannelID 	I2C Bus channel
 *  @param  u2ClkDiv 	Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr 	Serial interface device address.
 *  @param  u1WordAddrNum 	Serial interface word address Number (0~3).
 *  @param  u4WordAddr 	Serial interface word address.
 *  @param  pu1Buf 		Pointer to user's buffer.
 *  @param  u2ByteCnt 	Number of byte to write. Max = 256 bytes.
 *  @retval   > 0		Actual number of bytes to be written.
 *  @retval   = 0		Error
 */
//-------------------------------------------------------------------------
UINT16 SIF_V1_X_Write(UINT8 u1CHannelID, UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 u1WordAddrNum , UINT32 u4WordAddr, 
								UINT8 *pu1Buf, UINT16 u2ByteCnt)
{
    UINT8 u4WriteCnt;
    UINT32 u4Ret; 
    UINT16 u2Ret = 0;
    UINT8	u1fgTuner = SIF_BUS_SYSTEM_I2C;

    ASSERT( u1WordAddrNum <=3 );
    ASSERT( u4WordAddr < (1<<24) );

    // parameter check: channle ID   
    if (u1CHannelID > SIF_BUS_SECOND_I2C)
    {
	LOG(7,"wrong u1CHannelID\n");
        goto SIF_WRITE_EXIT_MARK;
    }
    
    // parameter check: clk divide is only 12 bits.
    if (u2ClkDiv > (1 << 12))
    {
        goto SIF_WRITE_EXIT_MARK;
    }

    // parameter check: device address must be even.
    if ((u1DevAddr %2) != 0)
    {
        goto SIF_WRITE_EXIT_MARK;
    }

    // parameter check: user's buffer pointer.
    if ((pu1Buf == NULL) || (u2ByteCnt == 0))
    {
        goto SIF_WRITE_EXIT_MARK;
    }

    switch( u1CHannelID )
    {
    	case SIF_BUS_SYSTEM_I2C:
		LOG(7,"BUS_SYSTEM_I2C selected\n");
		u1fgTuner= FALSE; 
		break;
    	case SIF_BUS_TUNER_I2C:
		LOG(7,"BUS_TUNER_I2C selected\n");
		u1fgTuner= TRUE; 
		break;
    }
    
    // parameter check: max 256 bytes in a given word address.
    if ( u2ByteCnt > SIF_BLK_SIZE )
    {
	u2ByteCnt =SIF_BLK_SIZE;
    }

    // check if SIF is initialized ?
    if (!_fgSifInit)
    {
        SIF_V1_Init();
    }

#ifndef SIF_NON_OSAI
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {

        // wait semaphore
        VERIFY(x_sema_lock(_hSifUserSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }    
#endif /* SIF_NON_OSAI */
#ifdef SIF_RETRY_MODE                   
    // prepare retry when r/w fail.
    _u4SifRetryCount = 0;

//----- Parameter check end ---------//

SIF_RETRY_MARK3:    
#endif
    // copy to user's parameter structure.
    _rSifDevParm.u2ClkDiv = u2ClkDiv;
    _rSifDevParm.u1DevAddr = u1DevAddr;
    _rSifDevParm.u4WordAddrNum = u1WordAddrNum;   	//
    _rSifDevParm.u4WordAddr = u4WordAddr;			//
    _rSifDevParm.pu1Buf = pu1Buf;
    _rSifDevParm.u2ByteCnt = u2ByteCnt;
    _rSifDevParm.u2ProcCnt = 0;
    _rSifDevParm.fgTuner = u1fgTuner;

    while (_rSifDevParm.u2ByteCnt > 0)
    {
        u4WriteCnt = MIN(_rSifDevParm.u2ByteCnt, SIF_PAGE_SIZE);

        u4Ret = _Sif_WriteData(u1CHannelID, u4WriteCnt);

        if (u4WriteCnt != u4Ret)
        {
            LOG(7, "ERR: dev =0x%X, u4WriteCnt = %d, u4Ret = %d.\n", u1DevAddr, u4WriteCnt, u4Ret);

#ifdef SIF_RETRY_MODE               
            // r/w fail, retry again.
            _u4SifRetryCount ++;
            if (_u4SifRetryCount < SIF_RETRY_COUNT)
            {
                goto SIF_RETRY_MARK3;
            }
#endif
#ifndef SIF_NON_OSAI
            // OSAI not initialize, do not call function in x_os.h.
            if (!_fgSifNoOSAI)
            {

                // release semaphore
                VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);
            }
#endif /* SIF_NON_OSAI */
            u2Ret = _rSifDevParm.u2ProcCnt;
            goto SIF_WRITE_EXIT_MARK;
        }

        _rSifDevParm.pu1Buf += u4WriteCnt;
        _rSifDevParm.u4WordAddr += u4WriteCnt;
        _rSifDevParm.u2ByteCnt -= u4WriteCnt;
        _rSifDevParm.u2ProcCnt += u4WriteCnt;
    }

#ifndef SIF_NON_OSAI
    // OSAI not initialize, do not call function in x_os.h.
    if (!_fgSifNoOSAI)
    {

        // release semaphore
        VERIFY(x_sema_unlock(_hSifUserSema) == OSR_OK);
    }
#endif /* SIF_NON_OSAI */
    u2Ret = u2ByteCnt;

SIF_WRITE_EXIT_MARK:
    return u2Ret;
}

static VOID SIF_V1_ISRHandler(UINT16 u2Vector)
{
#ifndef SIF_POLLING_MODE
    _Sif_HandleISR(u2Vector);
#endif
}


