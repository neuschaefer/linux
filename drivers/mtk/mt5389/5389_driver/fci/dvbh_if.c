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

/** @file dvbh_if.c
 *  This C file implements the DVBH SDIO host functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "sdio_func.h"
#include "dvbh_if.h"
#include "dvbh_src.h" // MT5151 image binary file.

#ifdef CC_BD_FCI
#include "x_debug.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"

#include "x_ckgen.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//#define DVBH_READ_ONLY
#define DVBH_DOWNLOAD_CODE
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define DVBHISR_STACK_SZ        (1024)
#define DVBHISR_PRIORITY         (40)
#define DVBH_BLOCKSIZE           ((UINT32)512)

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
#ifdef DVBH_UNIT_TEST

__align(16)
static UINT8 _au1DVBHTxBuf[256];
static UINT8 _au1DVBHRxBuf[256];

static UINT32 _u4DVBHTstSize = 0;
static BOOL _fgDVBHTstEnable = FALSE;

static UINT32 _au4DVBHTxBuf[DVBH_IO_SIZE];
static UINT32 _au4DVBHRxBuf[DVBH_IO_SIZE];

static void *_pvDVBHTag = (void *)0x12345678;
#endif

#ifdef DVBH_MPFEC_LOOP_TEST
static UINT32 _au4MPFECRxBuf[DVBH_IO_SIZE];
static UINT32 _u4MPFECRxSize = 0;
#endif

#ifndef DVBH_UNIT_TEST
static DVBH_NOTIFY_T _rDVBHMWNofity;
static DVBH_NOTIFY_T _rDVBHDrvNofity;
#endif

static HANDLE_T _hDVBHIsrThread;
static HANDLE_T _hDVBHIsrSema;
static HANDLE_T _hDVBHEntrySema;

#define DVBH_ENTRY_LOCK \
    VERIFY(OSR_OK == x_sema_lock(_hDVBHEntrySema, X_SEMA_OPTION_WAIT))
    
#define DVBH_ENTRY_UNLOCK \
    VERIFY(OSR_OK == x_sema_unlock(_hDVBHEntrySema))

static UINT32 _u4DVBHInit = FALSE;

static UINT32 _u4IntOKCount = 0;
static UINT32 _u4LastIntStatus;

#ifdef DVBH_DOWNLOAD_CODE
static const UINT32 _au4DRAMPattern[] =
{
    0x00ff00ff, 0x55aa55aa,
    0xaa55aa55, 0xff00ff00,
    0xf0f00f0f, 0xa55aa55a,
    0x5aa55aa5, 0x0f0ff0f0,
    0x5aa5a55a, 0x5555aaaa,
    0xaaaa5555, 0xa55a5aa5,
    0x5a5a5a5a, 0xa5a5a5a5,
    0x5a5aa5a5, 0xa5a55a5a,
};
#define MAX_PAT             (sizeof(_au4DRAMPattern)/sizeof(UINT32))
#define MAX_PAT_BYTE        (MAX_PAT * 4)
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _Dvbh_GetReg
 *  get DVBH register.
 *  @param  u4RegAddr   DVBH register address.
 *  @param  pu4Val   point to value buffer. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------

static INT32 _Dvbh_GetReg(UINT32 u4RegAddr, UINT32 *pu4Val)
{
    INT32 i4Ret;

    // read back and check.
    i4Ret = SDIO_ReadBytes((UINT32)SDIO_FUNC_1, u4RegAddr, 
        DVBH_REG_SIZE, (UINT8 *)pu4Val, SDIO_OP_INCADR);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _Dvbh_SetReg
 *  Set DVBH register.
 *  @param  u4RegAddr   DVBH register address.
 *  @param  u4Val   set value. 
 *  @param  fgCheck   read after write and check new data is equal to write data. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_SetReg(UINT32 u4RegAddr, UINT32 u4Val, BOOL fgCheck)
{
    INT32 i4Ret;

    UINT32 u4NewVal = u4Val;

    // set new value to register.
    i4Ret = SDIO_WriteBytes((UINT32)SDIO_FUNC_1, u4RegAddr, 
        DVBH_REG_SIZE, (UINT8 *)&u4NewVal, SDIO_OP_INCADR);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // read back and check.
    if (fgCheck)
    {
        u4NewVal = 0;

        i4Ret = SDIO_ReadBytes((UINT32)SDIO_FUNC_1, u4RegAddr, DVBH_REG_SIZE, (UINT8 *)&u4NewVal,
                               SDIO_OP_INCADR);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
        if (u4NewVal != u4Val)
        {
            return -1;
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Dvbh_GetCBusReg
 *  Get register value by DVBH CBus.
 *  @param  u4RegAddr   DVBH register address.
 *  @param  pu4Val   point to register buffer. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
#ifdef DVBH_UNIT_TEST
static INT32 _Dvbh_GetCBusReg(UINT32 u4RegAddr, UINT32 *pu4Val)
{
    INT32 i4Ret;

    UINT32 u4Complete = 0;

    i4Ret = _Dvbh_SetReg(DVBH_CBUS_ADDR, u4RegAddr, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetReg(DVBH_CBUS_STATUS, DVBH_CBUS_READ, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // wait complete.
    while ((u4Complete & DVBH_CBUS_DONE) == 0)
    {
        i4Ret = _Dvbh_GetReg(DVBH_CBUS_STATUS, &u4Complete);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }

    return _Dvbh_GetReg(DVBH_CBUS_DATA, pu4Val);
}
#endif
//-------------------------------------------------------------------------
/** _Dvbh_SetCBusReg
 *  Set DVBH CBus register.
 *  @param  u4RegAddr   DVBH register address.
 *  @param  u4Val   set value. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_SetCBusReg(UINT32 u4RegAddr, UINT32 u4Val)
{
    INT32 i4Ret;

    UINT32 u4Complete = 0;

    i4Ret = _Dvbh_SetReg(DVBH_CBUS_ADDR, u4RegAddr, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
   
    i4Ret = _Dvbh_SetReg(DVBH_CBUS_DATA, u4Val, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
    
    // wait command complete.
    u4Complete = 0;
    while ((u4Complete & DVBH_CBUS_DONE) == 0)
    {
        i4Ret = _Dvbh_GetReg(DVBH_CBUS_STATUS, &u4Complete);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }

    i4Ret = _Dvbh_SetReg(DVBH_CBUS_STATUS, DVBH_CBUS_WRITE, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // wait command complete.
    u4Complete = 0;
    while ((u4Complete & DVBH_CBUS_DONE) == 0)
    {
        i4Ret = _Dvbh_GetReg(DVBH_CBUS_STATUS, &u4Complete);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }

    return S_OK;
}
//-------------------------------------------------------------------------
/** _Dvbh_CheckCBus
 *  check CBus with writing and reading register by Cbus.
 *  @param  void
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
#ifdef DVBH_UNIT_TEST
static INT32 _Dvbh_CheckCBus(void)
{
    INT32 i4Ret;

    UINT32 u4Val;
    UINT32 u4OldVal;
    UINT32 u4TestReg = 0x24000194;
    UINT32 u4PwnReg = 0x1C000004;
    
#if 0
    UINT32 u4TestReg1 = 0x1E000010;

    i4Ret = _Dvbh_SetCBusReg(u4TestReg1, 0xFFFFFFFF);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
#else
    i4Ret = _Dvbh_SetCBusReg(u4PwnReg, 0x000000FF);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_GetCBusReg(u4TestReg, &u4OldVal);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(u4TestReg, 0);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    u4Val = 0x55aa;
    i4Ret = _Dvbh_GetCBusReg(u4TestReg, &u4Val);

    if ((u4Val != 0) || (i4Ret != S_OK))
    {
        return -1;
    }

    i4Ret = _Dvbh_SetCBusReg(u4TestReg, 0x12345678);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    u4Val = 0x55aa;
    i4Ret = _Dvbh_GetCBusReg(u4TestReg, &u4Val);

    if ((u4Val != 0x12345678) || (i4Ret != S_OK))
    {
        return -1;
    }

    i4Ret = _Dvbh_SetCBusReg(u4TestReg, u4OldVal);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    u4Val = 0x55aa;
    i4Ret = _Dvbh_GetCBusReg(u4TestReg, &u4Val);

    if ((u4Val != u4OldVal) || (i4Ret != S_OK))
    {
        return -1;
    }
#endif
    return S_OK;
}
#endif

//-------------------------------------------------------------------------
/** _Dvbh_ReadBytes
 *  Read multi-bytes from DVBH SDIO device function by BYTE mode.
 *  @param  u4DvbhAddr   dram address in DVBH.
 *  @param  u4Size   read data size. 
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_ReadBytes(UINT32 u4DvbhAddr, UINT32 u4Size, UINT8 *pu1Buf)
{
    INT32 i4Ret;
    UINT32 u4BlockNum, u4ByteNum, u4DmaRdy = 0;

    ASSERT(u4DvbhAddr < DVBH_DMA_MAX_ADDR);
    ASSERT(pu1Buf);
    ASSERT(u4Size);

    u4BlockNum = u4Size / DVBH_BLOCKSIZE;
    u4ByteNum = u4Size % DVBH_BLOCKSIZE;

    // set function 1 DMA start address.
    i4Ret = _Dvbh_GetReg(DVBH_CMD, &u4DmaRdy);
    if ((i4Ret != S_OK) || (!(DVBH_DMA_READY & u4DmaRdy)))
    {
        return i4Ret;
    }

    // set function 1 DMA start address.
    i4Ret = _Dvbh_SetReg(DVBH_ADDR, u4DvbhAddr, DVBH_CHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // set function 1 DMA length.
    i4Ret = _Dvbh_SetReg(DVBH_SIZE, u4Size, DVBH_CHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // set function 1 DMA read from SDIO DRAM.
    i4Ret = _Dvbh_SetReg(DVBH_CMD, DVBH_DMA_GET, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    if (u4BlockNum > 0)
    {
        i4Ret = SDIO_ReadBlocks((UINT32)SDIO_FUNC_1, DVBH_DATA, 
            u4BlockNum, pu1Buf, SDIO_OP_FIXADR);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
        pu1Buf += (u4BlockNum * DVBH_BLOCKSIZE);
    }

    if (u4ByteNum > 0)
    {
        i4Ret = SDIO_ReadBytes((UINT32)SDIO_FUNC_1, DVBH_DATA, 
            u4ByteNum, pu1Buf, SDIO_OP_FIXADR);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Dvbh_WriteBytes
 *  Write multi-bytes to DVBH SDIO device function by BYTE mode.
 *  @param  u4DvbhAddr   dram address in DVBH.
 *  @param  u4Size   write data size. 
 *  @param  pu1Buf   point to user's data buffer.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_WriteBytes(UINT32 u4DvbhAddr, UINT32 u4Size, UINT8 *pu1Buf)
{
    INT32 i4Ret;
    UINT32 u4BlockNum = 0;
    UINT32 u4ByteNum = 0;
    UINT32 u4DmaRdy = 0;
    
    ASSERT(u4DvbhAddr < DVBH_DMA_MAX_ADDR);
    ASSERT(pu1Buf);
    ASSERT(u4Size);

    u4BlockNum = u4Size / DVBH_BLOCKSIZE;
    u4ByteNum = u4Size % DVBH_BLOCKSIZE;
    //u4ByteNum = u4Size;    

    // set function 1 DMA start address.
    i4Ret = _Dvbh_GetReg(DVBH_CMD, &u4DmaRdy);
    if ((i4Ret != S_OK) || (!(DVBH_DMA_READY & u4DmaRdy)))
    {
        return -1;
    }    
    
    // set function 1 DMA start address.
    i4Ret = _Dvbh_SetReg(DVBH_ADDR, u4DvbhAddr, DVBH_CHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // set function 1 DMA length.
    i4Ret = _Dvbh_SetReg(DVBH_SIZE, u4Size, DVBH_CHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // set function 1 DMA write to SDIO DRAM.
    i4Ret = _Dvbh_SetReg(DVBH_CMD, DVBH_DMA_SET, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }   

    if (u4BlockNum > 0)
    {
        i4Ret = SDIO_WriteBlocks((UINT32)SDIO_FUNC_1, DVBH_DATA, 
            u4BlockNum, pu1Buf, SDIO_OP_FIXADR);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
        pu1Buf += (u4BlockNum * DVBH_BLOCKSIZE);
    }

    if (u4ByteNum > 0)
    {
        i4Ret = SDIO_WriteBytes((UINT32)SDIO_FUNC_1, DVBH_DATA, 
            u4ByteNum, pu1Buf, SDIO_OP_FIXADR);
        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Dvbh_InitSDRAM
 *  Init SDRAM in DVBH device.
 *  @param  void
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
#ifdef DVBH_DOWNLOAD_CODE
#if 0
static INT32 _Dvbh_InitSDRAM(void)
{
    INT32 i4Ret;

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_INIT, DEFAULT_INIT);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_PARM, DEFAULT_PARM);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_REFRESH, DEFAULT_REFRESH);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_DLADJ, DEFAULT_DLADJ);
    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    return S_OK;
}
#else

static BOOL _Dvbh_DRAM_SetSampleTime(UINT8 u1RSampleTime)
{
    UINT32 u4RegHigh, u4RegLow;

    if(_Dvbh_GetCBusReg(DVBH_SDRAM_REFRESH, &u4RegLow) != 0)
    {
        return FALSE;
    }
    if(_Dvbh_GetCBusReg(DVBH_SDRAM_PARM, &u4RegHigh) != 0)
    {
        return FALSE;
    }

    // Clear first
    u4RegLow &= ~0x10000;
    u4RegHigh &= ~0x4000;

    if(u1RSampleTime == 0)
    {
        u4RegLow &= ~0x10000;
        u4RegHigh &= ~0x4000;
    }
    else if(u1RSampleTime == 1)
    {
        u4RegLow |= 0x10000;
        u4RegHigh &= ~0x4000;
    }
    else if(u1RSampleTime == 2)
    {
        u4RegLow &= ~0x10000;
        u4RegHigh |= 0x4000;
    }
    else if(u1RSampleTime == 3)
    {
        u4RegLow |= 0x10000;
        u4RegHigh |= 0x4000;
    }
    else
    {
        ASSERT(0);
    }

    if(_Dvbh_SetCBusReg(DVBH_SDRAM_REFRESH, u4RegLow) != 0)
    {
        return FALSE;
    }
    if(_Dvbh_SetCBusReg(DVBH_SDRAM_PARM, u4RegHigh) != 0)
    {
        return FALSE;
    }

    return TRUE;
}

static BOOL _Dvbh_DRAM_Calibrate(UINT8 *pu1Min, UINT8 *pu1Max)
{
    UINT32 au4TestBuf[MAX_PAT];
    UINT32 u4Addr, u4Delay;
    UINT32 i, j, u4Idx;
    BOOL afgCalResult[16];
    UINT8 u1Min = 0, u1Max = 0;

    if((pu1Min == NULL) || (pu1Max == NULL))
    {
        return FALSE;
    }

    *pu1Min = 0;
    *pu1Max = 0;

    for(u4Delay=0, u4Idx=0; u4Delay<0xFF; u4Delay+=0x10, u4Idx++)
    {
        afgCalResult[u4Idx] = TRUE;

        _Dvbh_SetCBusReg(DVBH_SDRAM_DLADJ, u4Delay);
        x_thread_delay(1);
        u4Addr = 0x0 + u4Idx * 0x10000;
        for(i=0; i<4; i++)
        {
            if(_Dvbh_WriteBytes(u4Addr, MAX_PAT_BYTE, (UINT8 *)_au4DRAMPattern) != 0)
            {
                return FALSE;
            }
            u4Addr += MAX_PAT_BYTE;
        }

        u4Addr = 0x0 + u4Idx * 0x10000;
        for(i=0; i<4; i++)
        {
            if(_Dvbh_ReadBytes(u4Addr, MAX_PAT_BYTE, (UINT8*)au4TestBuf) != 0)
            {
                return FALSE;
            }
            for(j=0; j<MAX_PAT; j++)
            {
                if(_au4DRAMPattern[j] != au4TestBuf[j])
                {
                    afgCalResult[u4Idx] = FALSE;
                    break;
                }
            }
            if(!afgCalResult[u4Idx])
            {
                break;
            }
            u4Addr += MAX_PAT_BYTE;
        }
    }

    u1Min = 0;
    u1Max = 0;
    for(i=0; i<16;i++)
    {
        if(afgCalResult[i])
        {
            u1Min = i;
            u1Max = 15;
            for(j=(i+1); j<16; j++, i++)
            {
                if(!afgCalResult[j])
                {
                    u1Max = j - 1;
                    break;
                }
            }

            if((*pu1Max - *pu1Min) < (u1Max - u1Min))
            {
                *pu1Min = u1Min;
                *pu1Max = u1Max;
            }
        }
    }

    return TRUE;
}

static INT32 _Dvbh_InitSDRAM(void)
{
    INT32 i4Ret;
    UINT8 i, u1Min, u1Max, u1Sel, u1Range;
    UINT8 u1RealMin = 0xFF, u1RealMax, u1Delay;

    UNUSED(u1RealMax);

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_PARM, DEFAULT_PARM);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_REFRESH, DEFAULT_REFRESH);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_DLADJ, DEFAULT_DLADJ);
    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    i4Ret = _Dvbh_SetCBusReg(DVBH_SDRAM_INIT, DEFAULT_INIT);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // Calibration
    u1Sel = 0;
    u1Range = 0;
    for(i=0; i<4; i++)
    {
        if(!_Dvbh_DRAM_SetSampleTime(i))
        {
            LOG(3, "Set sample time fail\n");
            return E_FAIL;
        }
        if(!_Dvbh_DRAM_Calibrate(&u1Min, &u1Max))
        {
            LOG(3, "Calibration fail\n");
            return E_FAIL;
        }

        if(u1Range < (u1Max - u1Min))
        {
            u1Sel = i;
            u1Range = u1Max - u1Min;
            u1RealMin = u1Min;
            u1RealMax = u1Max;
        }

        UTIL_Printf("Calibration: %d, delay:0x%02x ~ 0x%02x\n", i, u1Min * 0x10, u1Max * 0x10);
    }

    u1Delay = (u1RealMin + (u1Range / 2)) * 0x10;
    UTIL_Printf("Set: %d, 0x%x\n", u1Sel, u1Delay);

    if(!_Dvbh_DRAM_SetSampleTime(u1Sel))
    {
        LOG(3, "Set sample time fail\n");
        return E_FAIL;
    }

    if(_Dvbh_SetCBusReg(DVBH_SDRAM_DLADJ, (UINT32)u1Delay) != 0)
    {
        return E_FAIL;
    }

    return S_OK;
}
#endif

//-------------------------------------------------------------------------
/** _Dvbh_EnableCPU
 *  enable cpu in DVBH device.
 *  @param  void.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_EnableCPU(BOOL fgEnable)
{
    return _Dvbh_SetCBusReg(DVBH_BIM_CPU_ENABLE, fgEnable);
}

//-------------------------------------------------------------------------
/** _Dvbh_LoadImage
 *  download image to DVBH device SRAM.
 *  @param  void.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_LoadImage(void)
{
    INT32 i4Ret;

    UINT32 u4Size = DVBH_IMAGE_SIZE;
    UINT32 i;
    UINT32 u4Count, u4CompAddr, u4Index, u4ReadAddr;

    i4Ret = _Dvbh_WriteBytes(DVBH_RAM_BASE, u4Size, (UINT8 *)_u1DVBHImageSrc);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

//#ifdef DVBH_UNIT_TEST
#if 1
    // wait device to bootup.
    //x_thread_delay(1000);
    x_thread_delay(10);

    u4CompAddr = (UINT32)x_mem_alloc(0x10000);
    if(u4CompAddr == 0)
    {
        return -1;
    }

    u4Index = 0;
    u4ReadAddr = DVBH_RAM_BASE;
    while(u4Size > 0)
    {
        if(u4Size > 0xF000)
        {
            u4Count = 0xF000;
        }
        else
        {
            u4Count = u4Size;
        }

        i4Ret = _Dvbh_ReadBytes(u4ReadAddr, u4Count, (UINT8 *)u4CompAddr);

        if (i4Ret != S_OK)
        {
            x_mem_free((void*)u4CompAddr);
            return i4Ret;
        }
    
        // compare data.
        for (i = 0; i < u4Count; i++)
        {
            if (_u1DVBHImageSrc[u4Index + i] != (*(UINT8*)(u4CompAddr + i)))
            {
                LOG(1, "Compare Err: i = %d", i);
                x_mem_free((void*)u4CompAddr);
                return -1;
            }
        }

        u4Size -= u4Count;
        u4Index += u4Count;
        u4ReadAddr += u4Count;
    }

    x_mem_free((void*)u4CompAddr);
/*
    i4Ret = _Dvbh_ReadBytes(DVBH_RAM_BASE, u4Size, (UINT8 *)u4CompAddr);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // compare data.
    for (i = 0; i < u4Size; i++)
    {
        if (_au1DVBHTxBuf[i] != _u1DVBHImageSrc[i])
        {
            LOG(1, "Compare Err: i = %d", i);
            return -1;
        }
    }
*/
#endif

    i4Ret = _Dvbh_EnableCPU(TRUE);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    return S_OK;
}
#endif
//-------------------------------------------------------------------------
/** _Dvbh_DevInit
 *  Initail and config DVBH device.
 *  @param  void.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------

#ifndef DVBH_UNIT_TEST
static INT32 _Dvbh_DevInit(void)
{
    INT32 i4Ret;

    i4Ret = _Dvbh_SetReg(DVBH_INT_DEVICE, DVBH_INT_HST_CMD, DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
/*
    // set SDIO interrupt to host by Cbus.
    i4Ret = _Dvbh_SetCBusReg(DVBH_SDIO_INT_HOST, 0xFFFFFFFF);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
*/   
    return S_OK;
}
#endif  

//-------------------------------------------------------------------------
/** _Dvbh_SetIntDevice
 *  set interrupt to cpu in DVBH device.
 *  @param  void.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static INT32 _Dvbh_SetIntDevice(UINT32 u4IntStatus)
{
    INT32 i4Ret;
    UINT32 u4Status;
    
    i4Ret = _Dvbh_GetReg(DVBH_INT_DEVICE, &u4Status);
    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
    
    i4Ret = _Dvbh_SetReg(DVBH_INT_DEVICE, (u4Status |u4IntStatus), DVBH_NOCHECK);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _Dvbh_ServiceMw
 *  service middleware command/response from device.
 *  @param  u4IntStatus  interrupt status.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
#ifndef DVBH_UNIT_TEST
static void _Dvbh_ServiceMw(UINT32 u4IntStatus)
{
    if (_rDVBHMWNofity.pfnNfy)
    {
        _rDVBHMWNofity.pfnNfy(u4IntStatus, _rDVBHMWNofity.pvTag);
    }
}
//-------------------------------------------------------------------------
/** _Dvbh_ServiceDrv
 *  service driver command/response from device.
 *  @param  u4IntStatus  interrupt status.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
static void _Dvbh_ServiceDrv(UINT32 u4IntStatus)
{
    if (_rDVBHDrvNofity.pfnNfy)
    {
        _rDVBHDrvNofity.pfnNfy(u4IntStatus, _rDVBHDrvNofity.pvTag);
    }
}
#endif

//-------------------------------------------------------------------------
/** _Dvbh_ISRHandler
 *  DVBH SDIO interrupt task handler. 
 *  @param  pvArgv.
 *  @retval   VOID.   
 */
//-------------------------------------------------------------------------
static VOID _Dvbh_ISRHandler(VOID)
{
    UINT32 u4Mask = 0;
    UINT32 u4IntStatus = 0;
  
    VERIFY(S_OK == _Dvbh_GetReg(DVBH_INT_HOST, &u4IntStatus));

    VERIFY(S_OK == _Dvbh_GetReg(DVBH_INT_HOST_MASK, &u4Mask));

    // disable IRQ.
    u4Mask |= 0xFFFF0000;
    u4IntStatus &= (~u4Mask);

    VERIFY(S_OK == _Dvbh_SetReg(DVBH_INT_HOST, u4IntStatus, DVBH_NOCHECK));

#ifdef DVBH_UNIT_TEST

  #ifdef SDIO_INTERRUPT_CHECK_TEST

        if (u4IntStatus == 0x00000020)
        {
            _u4IntOKCount = 1;
            _u4LastIntStatus = 0x00000020;
        }
        else
        {
            if ((u4IntStatus != 0x00) && (u4IntStatus == (_u4LastIntStatus << 1)))
            {
                _u4IntOKCount++;

                UTIL_Printf("SDIO: INT Status = 0x%08X, _u4LastIntStatus = 0x%08X, _u4IntOKCount = %d.\n", u4IntStatus, _u4LastIntStatus, _u4IntOKCount);

                _u4LastIntStatus = u4IntStatus;

                if(_u4IntOKCount == 11)
                {
                    UTIL_Printf("\nSDIO Client -> HOST Interrupt Test OK !!");
                }
            }
        }
  #else
    LOG(1, "SDIO: interrupt status = 0x%08X.\n", u4IntStatus);    
  #endif

#else

    // handle IRQ.
    if (u4IntStatus & DVBH_INT_DEV_INIT)
    {
        LOG(7, "SDIO: Dev initial ok.\n");
        //VERIFY(S_OK == _Dvbh_DevInit());
    }

    if (u4IntStatus & DVBH_INT_DEV_RESP)
    {
        LOG(7, "SDIO: Dev response.\n");
        _Dvbh_ServiceMw(DVBH_INT_DEV_RESP);
    }

    if (u4IntStatus & DVBH_INT_DEV_CMD)
    {
        LOG(7, "SDIO: Dev command.\n");
        _Dvbh_ServiceMw(DVBH_INT_DEV_CMD);
    }

    if (u4IntStatus & DVBH_INT_DEV_DRV)
    {
        LOG(7, "SDIO: Dev driver data.\n");
        _Dvbh_ServiceDrv(DVBH_INT_DEV_DRV);
    }
    
    if (u4IntStatus & DVBH_INT_ERR_ALL)
    {
        LOG(7, "SDIO: Err interrupt = 0x%08X.\n", u4IntStatus);
    }
    
#endif    

#ifdef DVBH_MPFEC_LOOP_TEST
    // To integrate test with MPFEC.
    if (u4IntStatus & DVBH_INT_DEV_DRV)
    {              
#ifdef DVBH_MPFEC_LOOP_TEST_WRITEBACK
        LOG(1, "SDIO: Dev driver Int - WriteBack.\n");
#else
        LOG(1, "SDIO: Dev driver Int - No Write Back.\n");
#endif
        _u4MPFECRxSize = sizeof(_au4MPFECRxBuf);
        VERIFY(S_OK == DVBH_DrvGet(_au4MPFECRxBuf, &_u4MPFECRxSize));
        LOG(1, "SDIO: Read data to adr = 0x%x, Rx size = %d bytes.\n", _au4MPFECRxBuf, _u4MPFECRxSize);

#ifdef DVBH_MPFEC_LOOP_TEST_WRITEBACK
        VERIFY(S_OK == DVBH_DrvSet(_au4MPFECRxBuf, _u4MPFECRxSize));
        LOG(1, "SDIO: Write back from adr = 0x%x, Tx size = %d bytes.\n", _au4MPFECRxBuf, _u4MPFECRxSize);
#endif
    }

#endif
}

//-------------------------------------------------------------------------
/** _Dvbh_HISR
 *  DVBH SDIO HISR handler. This will be called by FCI HISR when SDIO interrupt happen.
 *  @param  fgIntStatus    FCI interrupt status (NOTICE: without the highest bit MMCIS).
 *  @retval   VOID.   
 */
//-------------------------------------------------------------------------
static void _Dvbh_HISR(INT32 fgIntStatus)
{
    // unlock the to let _Dvbh_ISRThread() handle.
    VERIFY(OSR_OK == x_sema_unlock(_hDVBHIsrSema));

    UNUSED(fgIntStatus);
}
//-------------------------------------------------------------------------
/** _Dvbh_ISRThread
 *  DVBH SDIO interrupt task handler. 
 *  @param  pvArgv.
 *  @retval   VOID.   
 */
//-------------------------------------------------------------------------
static VOID _Dvbh_ISRThread(VOID *pvArgv)
{
    UNUSED(pvArgv);
    
    // SD data ready semaphore.
    VERIFY(OSR_OK == x_sema_create(&_hDVBHIsrSema, X_SEMA_TYPE_BINARY,
            X_SEMA_STATE_LOCK));

    while(1)
    {
        // wait interrupt.
        VERIFY(OSR_OK == x_sema_lock(_hDVBHIsrSema, X_SEMA_OPTION_WAIT));

        _Dvbh_ISRHandler();        
    }
}
//-------------------------------------------------------------------------
/** _Dvbh_MWNofity
 *  DVBH SDIO interrupt task handler. 
 *  @param  u4IntStatus notify event status.
 *  @param  pvTag  mw register notify tag.
 *  @retval   VOID.   
 */
//-------------------------------------------------------------------------
#ifdef DVBH_UNIT_TEST
static VOID _Dvbh_MWNofity(UINT32 u4IntStatus, VOID *pvTag)
{
    INT32 i4Ret = 0;
    UINT32 i = 0;
    
    // handle IRQ.
    switch (u4IntStatus)
    {
    case DVBH_INT_DEV_CMD:
        LOG(7, "MW: handle Dev command.\n");

        _u4DVBHTstSize = DVBH_CMD_LEN;
        i4Ret = DVBH_MwGetCmd((UINT32 *)_au1DVBHRxBuf, &_u4DVBHTstSize);
        if (i4Ret != S_OK)
        {
            return;
        }
        
        // init data.
        for (i = 0; i < _u4DVBHTstSize; i++)
        {
            _au1DVBHTxBuf[i] = (UINT8)(~_au1DVBHRxBuf[i]);
        }
        
        i4Ret = DVBH_MwSetResp((UINT32 *)_au1DVBHTxBuf, _u4DVBHTstSize);
        if (i4Ret != S_OK)
        {
            return;
        }
        
        break;

    case DVBH_INT_DEV_RESP:
        LOG(7, "MW: handle Dev response.\n");

        _u4DVBHTstSize = DVBH_RESP_LEN;
        i4Ret = DVBH_MwGetResp((UINT32 *)_au1DVBHRxBuf, &_u4DVBHTstSize);

        if (i4Ret != S_OK)
        {
            return;
        }

        // compare data.
        for (i = 0; i < _u4DVBHTstSize; i++)
        {
            if (_au1DVBHRxBuf[i] != (UINT8)(~_au1DVBHTxBuf[i]))
            {
                LOG(1, "Compare Err: i = %d", i);

                SDIO_Print((UINT8 *)_au1DVBHRxBuf, _u4DVBHTstSize);
                return;
            }
        }

        if (!_fgDVBHTstEnable)
        {
            LOG(1, "Test Cmd stop.\n");
            return;
        }
        _u4DVBHTstSize += 4;
        if (_u4DVBHTstSize > DVBH_CMD_LEN)
        {
            _u4DVBHTstSize = 4;
        }

        LOG(7, "Test Cmd Size = %d.\n", _u4DVBHTstSize);
        
        i4Ret = DVBH_MwSetCmd((UINT32 *)_au1DVBHTxBuf, _u4DVBHTstSize);
        
        if (i4Ret != S_OK)
        {
            return;
        }

        break;

    case DVBH_INT_DEV_DRV:
    case DVBH_INT_ERR_CRC:
    case DVBH_INT_ERR_IO:
    case DVBH_INT_ERR_DMAR:
    case DVBH_INT_ERR_DMAW:
    case DVBH_INT_ERR_INACTIVE:    
        LOG(1, "MW: Err interrupt = 0x%08X.\n", u4IntStatus);
        break;

    default:
        break;
    }

}
#endif
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** DVBH_SetCmdTest
 *  DVBH host set command cli test function.
 *  @param  fgEnable enable/disable test.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
#ifdef DVBH_UNIT_TEST
INT32 DVBH_SetCmdTest(BOOL fgEnable)
{
    UINT32 i;
    INT32 i4Ret;
    
    if (_fgDVBHTstEnable == fgEnable)
    {
        return -1;
    }
    _fgDVBHTstEnable = fgEnable;

    if (_fgDVBHTstEnable)
    {
        for (i = 0; i < 256; i++)
        {
            _au1DVBHTxBuf[i] = (UINT8)(i & 0xFF);
        }
        _u4DVBHTstSize = 4;

        LOG(7, "Test Cmd Size = %d.\n", _u4DVBHTstSize);

        i4Ret = DVBH_MwSetCmd((UINT32 *)_au1DVBHTxBuf, _u4DVBHTstSize);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
    }
    LOG(7, "%s OK...\n", __FUNCTION__);
    return S_OK;    
}

//-------------------------------------------------------------------------
/** DVBH_InterruptTest
 *  DVBH host get interrupt from SDIO device test function.
 *  @param  void
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_InterruptTest(VOID)
{
    UINT32 i;
    INT32 i4Ret = 0;
    UINT32 _u4IntStatus = 0;
       
    for (i = 0; i < 16; i++)
    {
        _u4IntStatus = (1 << i);

        UTIL_Printf("[FCI]SDIO: interrupt status = 0x%08X.\n", _u4IntStatus);

        // set interrupt to SDIO device.
        i4Ret = _Dvbh_SetIntDevice(_u4IntStatus);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
        
        // wait interrupt happen.
        x_thread_delay(50);       
    }

    return S_OK;    
}

//-------------------------------------------------------------------------
/** DVBH_DataTest
 *  DVBH data read/write function test.
 *  @param  void   data address.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_DataTest(VOID)
{
    UINT32 i, j, z = 1;
    INT32 i4Ret = 0;
    UINT32 u4DvbhAddr = DVBH_DMA_ADDR;
    UINT32 u4Size;
    
    LOG(7, "%s start...\n", __FUNCTION__);

    i4Ret = _Dvbh_CheckCBus();

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    for (i = 0; i < DVBH_IO_SIZE; i++)
    {
        _au4DVBHTxBuf[i] = i;
    }

#ifdef DVBH_READ_ONLY
        //u4Size = DVBH_IO_SIZE << 2;
        u4Size = DVBH_IO_SIZE;
        i4Ret = _Dvbh_WriteBytes(u4DvbhAddr, u4Size, (UINT8 *)_au4DVBHTxBuf);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }
#endif

// for (z = 1; z < 0xFFFFFFF0; z++)
{
    for (j = 1; j < DVBH_IO_SIZE; j+=9)
    {
        if (!(j%100))
        {
            LOG(1, " [%d] Test size = %d.\n", z, j);
        }

#ifndef DVBH_READ_ONLY
        u4DvbhAddr = DVBH_DMA_ADDR + (j % 0x200);
        u4Size = j << 2;

        i4Ret = _Dvbh_WriteBytes(u4DvbhAddr, u4Size, (UINT8 *)_au4DVBHTxBuf);

        if (i4Ret != S_OK)
        {
            LOG(1, " _Dvbh_WriteBytes fail i4Ret = %d.\n", i4Ret);        
            return i4Ret;
        }
#else
        u4Size = j;        
#endif

        for (i = 0; i < j; i++)
        {
            _au4DVBHRxBuf[i] = 0xFFFFFFFF;
        }
        
        i4Ret = _Dvbh_ReadBytes(u4DvbhAddr, u4Size, (UINT8 *)_au4DVBHRxBuf);

        if (i4Ret != S_OK)
        {
            LOG(1, " _Dvbh_ReadBytes fail i4Ret = %d.\n", i4Ret);                
            return i4Ret;
        }

#ifndef DVBH_READ_ONLY
        // compare data.
        for (i = 0; i < j; i++)
        {
            if (_au4DVBHRxBuf[i] != _au4DVBHTxBuf[i])
            {
                LOG(1, "Compare Err: i = %d", i);
                return -1;
            }
        }
#endif
    }
}
    LOG(1, "[Test Finish] : %s OK...\n", __FUNCTION__);
    return S_OK;
}

//-------------------------------------------------------------------------
/** DVBH_DataRead
 *  DVBH data read/write function test.
 *  @param  void   data address.
 *  @retval   S_OK : Success.
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_DataRead(UINT32 u4ReadSize)
{
    UINT32 i;
    INT32 i4Ret = 0;
    UINT32 u4DvbhAddr = DVBH_DMA_ADDR;
    UINT32 u4Size;

    LOG(1, "%s start...\n", __FUNCTION__);

    i4Ret = _Dvbh_CheckCBus();

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    for (i = 0; i < DVBH_IO_SIZE; i++)
    {
        _au4DVBHTxBuf[i] = i;
    }

    u4Size = DVBH_IO_SIZE;
    i4Ret = _Dvbh_WriteBytes(u4DvbhAddr, u4Size, (UINT8 *)_au4DVBHTxBuf);

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    LOG(1, " Read size = %d.\n", u4ReadSize);

    for (i = 0; i < u4ReadSize; i++)
    {
        _au4DVBHRxBuf[i] = 0xFFFFFFFF;
    }

    i4Ret = _Dvbh_ReadBytes(u4DvbhAddr, u4ReadSize, (UINT8 *)_au4DVBHRxBuf);

    if (i4Ret != S_OK)
    {
        LOG(1, " _Dvbh_ReadBytes fail i4Ret = %d.\n", i4Ret);
        return i4Ret;
    }

    return S_OK;
}

INT32 DVBH_DataWrite(UINT32 u4WriteSize)
{
    UINT32 i;
    INT32 i4Ret = 0;
    UINT32 u4DvbhAddr = DVBH_DMA_ADDR;

    LOG(1, "%s start...\n", __FUNCTION__);

    i4Ret = _Dvbh_CheckCBus();

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    for (i = 0; i < DVBH_IO_SIZE; i++)
    {
        _au4DVBHTxBuf[i] = i;
    }

    LOG(1, "Write size = %d.\n", u4WriteSize);

    u4DvbhAddr = DVBH_DMA_ADDR;

    i4Ret = _Dvbh_WriteBytes(u4DvbhAddr, u4WriteSize, (UINT8 *)_au4DVBHTxBuf);

    if (i4Ret != S_OK)
    {
        LOG(1, " _Dvbh_WriteBytes fail i4Ret = %d.\n", i4Ret);
        return i4Ret;
    }

    return S_OK;
}

#endif
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** DVBH_MwSetNotify
 *  set middleware host interrupt notify function.
 *  @param  pfnNfy   MW notify callback function.
 *  @param  pvTag   MW tag. It will be used in notify callback function argument.
 *  @retval   void
 */
//-------------------------------------------------------------------------
void DVBH_MwSetNotify(DVBH_CALLBACK pfnNfy, void *pvTag)
{  
    DVBH_ENTRY_LOCK;

#ifndef DVBH_UNIT_TEST
    _rDVBHMWNofity.pfnNfy = pfnNfy;
    _rDVBHMWNofity.pvTag = pvTag;
#endif

    DVBH_ENTRY_UNLOCK;    
}

//-------------------------------------------------------------------------
/** DVBH_DrvSetNotify
 *  set driver host interrupt notify function.
 *  @param  pfnNfy   driver notify callback function.
 *  @param  pvTag   driver tag. It will be used in notify callback function argument.
 *  @retval   void
 */
//-------------------------------------------------------------------------
void DVBH_DrvSetNotify(DVBH_CALLBACK pfnNfy, void *pvTag)
{   
    DVBH_ENTRY_LOCK;

#ifndef DVBH_UNIT_TEST
    _rDVBHDrvNofity.pfnNfy = pfnNfy;
    _rDVBHDrvNofity.pvTag = pvTag;
#endif

    DVBH_ENTRY_UNLOCK;    
}

//-------------------------------------------------------------------------
/** DVBH_MwSendCmd
 *  Send command to SDIO device.
 *  @param  pu4Cmd   host command buffer.
 *  @param  u4CmdSize   commad buffer size. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_MwSetCmd(UINT32 *pu4Cmd, UINT32 u4CmdSize)
{
    INT32 i4Ret = S_OK;

    ASSERT(pu4Cmd);
    ASSERT((u4CmdSize > 0) && (u4CmdSize <= DVBH_CMD_LEN));

    DVBH_ENTRY_LOCK;

    // send command size to SDIO device.
    i4Ret = _Dvbh_WriteBytes(DVBH_CMD_SIZE, DVBH_REG_SIZE, (UINT8 *)&u4CmdSize);
    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // send command to SDIO device.
    i4Ret = _Dvbh_WriteBytes(DVBH_CMD_ADDR, u4CmdSize, (UINT8 *)pu4Cmd);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // set interrupt to SDIO device.
    i4Ret = _Dvbh_SetIntDevice(DVBH_INT_HST_CMD);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

_ERROR:
    DVBH_ENTRY_UNLOCK;

    return i4Ret;
}
//-------------------------------------------------------------------------
/** DVBH_MwGetResp
 *  Get response from SDIO device.
 *  @param  pu4Resp   point to response buffer.
 *  @param  pu4RespSize   pointer to response buffer size. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_MwGetResp(UINT32 *pu4Resp, UINT32 *pu4RespSize)
{
    INT32 i4Ret = S_OK;

    UINT32 u4RespBufAddr = 0;
    UINT32 u4RespBufSize = 0;

    ASSERT(pu4Resp);
    ASSERT(pu4RespSize);
    ASSERT(((*pu4RespSize) > 0) && ((*pu4RespSize) <= DVBH_RESP_LEN));

    DVBH_ENTRY_LOCK;

    // query response address and length.
    i4Ret = _Dvbh_ReadBytes(DVBH_DEV_RESP_ADDR, DVBH_REG_SIZE, (UINT8 *)&u4RespBufAddr);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    ASSERT(u4RespBufAddr < DVBH_DMA_MAX_ADDR);

    i4Ret = _Dvbh_ReadBytes(DVBH_DEV_RESP_SIZE, DVBH_REG_SIZE, (UINT8 *)&u4RespBufSize);

    if (i4Ret != S_OK)
    {
       goto _ERROR;
    }

    ASSERT((u4RespBufSize > 0) && (u4RespBufSize <= (*pu4RespSize)));

    (*pu4RespSize) = u4RespBufSize;
    
    // get response from SDIO device.
    i4Ret = _Dvbh_ReadBytes(u4RespBufAddr, u4RespBufSize, (UINT8 *)pu4Resp);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // set interrupt to SDIO device.
    i4Ret = _Dvbh_SetIntDevice(DVBH_INT_HST_DONE);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

_ERROR:
    DVBH_ENTRY_UNLOCK;

    return i4Ret;
}
//-------------------------------------------------------------------------
/** DVBH_SetResp
 *  Send response to SDIO device.
 *  @param  pu4Resp   point to response buffer.
 *  @param  u4RespSize   response buffer size. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_MwSetResp(UINT32 *pu4Resp, UINT32 u4RespSize)
{
    INT32 i4Ret = S_OK;

    ASSERT(pu4Resp);
    ASSERT(u4RespSize);

    DVBH_ENTRY_LOCK;

    u4RespSize = MIN(u4RespSize, DVBH_RESP_LEN);

    // send response size to SDIO device.
    i4Ret = _Dvbh_WriteBytes(DVBH_RESP_SIZE, DVBH_REG_SIZE, (UINT8 *)&u4RespSize);
    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }
    
    // send response to SDIO device.
    i4Ret = _Dvbh_WriteBytes(DVBH_RESP_ADDR, u4RespSize, (UINT8 *)pu4Resp);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // set interrupt to SDIO device.
    i4Ret = _Dvbh_SetIntDevice(DVBH_INT_HST_RESP);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }
    
_ERROR:
    DVBH_ENTRY_UNLOCK;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** DVBH_GetCmd
 *  Get command from SDIO device.
 *  @param  pu4Cmd   host command buffer.
 *  @param  pu4CmdSize   pointer to commad buffer size. 
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_MwGetCmd(UINT32 *pu4Cmd, UINT32 *pu4CmdSize)
{
    INT32 i4Ret = S_OK;

    UINT32 u4CmdBufAddr = 0;
    UINT32 u4CmdBufSize = 0;

    ASSERT(pu4Cmd);
    ASSERT(pu4CmdSize);    
    ASSERT(((*pu4CmdSize) > 0) && ((*pu4CmdSize) <= DVBH_CMD_LEN));

    DVBH_ENTRY_LOCK;

    // query response address and length.
    i4Ret = _Dvbh_ReadBytes(DVBH_DEV_CMD_ADDR, DVBH_REG_SIZE, (UINT8 *)&u4CmdBufAddr);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    i4Ret = _Dvbh_ReadBytes(DVBH_DEV_CMD_SIZE, DVBH_REG_SIZE, (UINT8 *)&u4CmdBufSize);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    ASSERT(u4CmdBufAddr < DVBH_DMA_MAX_ADDR);
    ASSERT((u4CmdBufSize > 0) && (u4CmdBufSize <= (*pu4CmdSize)));

    (*pu4CmdSize) = u4CmdBufSize;
    
    // get response from SDIO device.
    i4Ret = _Dvbh_ReadBytes(u4CmdBufAddr, u4CmdBufSize, (UINT8 *)pu4Cmd);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

_ERROR:
    DVBH_ENTRY_UNLOCK;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** DVBH_DrvGet
 *  Get driver response from SDIO device.
 *  @param  pu4Buf   point to host data uffer.
 *  @param  pu4Size   data buffer size. This will be filled in the actual data size.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_DrvGet(UINT32 *pu4Buf, UINT32 *pu4Size)
{
    INT32 i4Ret = S_OK;

    UINT32 u4BufAddr = 0;
    UINT32 u4BufSize = 0;

    ASSERT(pu4Buf);
    ASSERT(pu4Size);

    DVBH_ENTRY_LOCK;

    // query driver buffer address and length.
    i4Ret = _Dvbh_ReadBytes(DVBH_DEV_DRV_ADDR, DVBH_REG_SIZE, (UINT8 *)&u4BufAddr);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    i4Ret = _Dvbh_ReadBytes(DVBH_DEV_DRV_SIZE, DVBH_REG_SIZE, (UINT8 *)&u4BufSize);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    ASSERT(u4BufAddr < DVBH_DMA_MAX_ADDR);
    ASSERT((u4BufSize > 0) && (u4BufSize <= (*pu4Size)));

    // fill the actual data size.
    *pu4Size = u4BufSize;
    
    // get response from SDIO device.
    i4Ret = _Dvbh_ReadBytes(u4BufAddr, u4BufSize, (UINT8 *)pu4Buf);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // set interrupt to SDIO device.
    i4Ret = _Dvbh_SetIntDevice(DVBH_INT_HST_DRV_GET);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

_ERROR:
    DVBH_ENTRY_UNLOCK;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** DVBH_DrvSet
 *  Set driver data to SDIO device.
 *  @param  pu4Buf   point to host data uffer.
 *  @param  u4Size   data buffer size.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_DrvSet(UINT32 *pu4Buf, UINT32 u4Size)
{
    INT32 i4Ret = S_OK;

    ASSERT(pu4Buf);
    ASSERT(u4Size);

    DVBH_ENTRY_LOCK;

    // send command size to SDIO device.
    i4Ret = _Dvbh_WriteBytes(DVBH_HOST_DRV_SIZE, DVBH_REG_SIZE, (UINT8 *)&u4Size);
    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // send command to SDIO device.
    i4Ret = _Dvbh_WriteBytes(DVBH_HOST_DRV_ADDR, u4Size, (UINT8 *)pu4Buf);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

    // set interrupt to SDIO device.
    i4Ret = _Dvbh_SetIntDevice(DVBH_INT_HST_DRV_SET);

    if (i4Ret != S_OK)
    {
        goto _ERROR;
    }

_ERROR:
    DVBH_ENTRY_UNLOCK;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** DVBH_Init
 *  DVBH initial function.
 *  @param  void   data address.
 *  @retval   S_OK : Success.   
 *  @retval   Others : Fail.
 */
//-------------------------------------------------------------------------
INT32 DVBH_Init(VOID)
{
    INT32 i4Ret;

    LOG(7, "%s start...\n", __FUNCTION__);
   
    i4Ret = SDIO_EnableIO((UINT32)SDIO_FUNC_1, _Dvbh_HISR);
    if (i4Ret != S_OK)
    {
        LOG(1, "DVBH_Init failed");
        return i4Ret;
    }

    //SDIO_SetInt(FALSE);
    
   // create a task to handle interrupt.
    if (!_u4DVBHInit)
    {
        VERIFY(OSR_OK == x_sema_create(&_hDVBHEntrySema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_UNLOCK));
    
        i4Ret = x_thread_create(&_hDVBHIsrThread, "DVBHISR",
                                DVBHISR_STACK_SZ, DVBHISR_PRIORITY,
                                _Dvbh_ISRThread, 0,
                                NULL);

        if (i4Ret != OSR_OK)
        {
            LOG(1, "Create threadfailed.\n");
            return 1;
        }
        
        _u4DVBHInit = TRUE;
    }

    // set debug mode.
/*
i4Ret = _Dvbh_SetReg(DVBH_INT_HOST, 0xD8000000, DVBH_NOCHECK);
    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
*/    
#ifdef DVBH_UNIT_TEST
    DVBH_MwSetNotify(_Dvbh_MWNofity, _pvDVBHTag);

    i4Ret = _Dvbh_CheckCBus();
    
    if (i4Ret != S_OK)
    {
        return i4Ret;
    }
#endif

#ifdef DVBH_DOWNLOAD_CODE
    i4Ret = _Dvbh_InitSDRAM();

    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    // download code to DVBH device.
    i4Ret = _Dvbh_LoadImage();
    if (i4Ret != S_OK)
    {
        return i4Ret;
    }

    x_thread_delay(100);
#endif

    //SDIO_SetInt(TRUE);

    LOG(7, "%s OK...\n", __FUNCTION__);    
    return i4Ret;
}

#endif
