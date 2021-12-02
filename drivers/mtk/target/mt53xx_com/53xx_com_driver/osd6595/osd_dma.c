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
 * $RCSfile: osd_dma.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_dma.c
 *  This file handle 5382p OSD DMA funtionality.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_drvif.h"
#include "osd_hw.h"
#include "osd_debug.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drv_common.h"
#include "x_bim.h"
#include "sif_if.h"
#include "panel.h"
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif
LINT_EXT_HEADER_END

#ifdef CC_MTK_LOADER
#define x_sema_lock(x, y) OSR_OK
#define x_sema_unlock(x) OSR_OK
#define x_sema_lock_timeout(x, y) OSR_OK
#define x_reg_isr(x, y, z) OSR_OK
#define x_sema_create(x, y, z) OSR_OK
#define x_thread_create(...) OSR_OK
#endif /* CC_MTK_LOADER */

#define MT8282_DMA_Setting   1
#define MT8282_FAKER_MODE  1
#define MT8282_OSD_Shift_SW_Modify 0
#define MT8281_DMA_AUTO  1

//#ifdef CC_MT5363
//extern BOOL gfgSwap;
//#endif
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define OSD_DMA_SEND_PALETTE_ID_ONLY        1
#define OSD_DMA_TARGET_POOL_NS              2
#define OSD_DMA_UPDATE_PERIOD               4
#define OSD_DMA_IPOOL_SIZE                  512 // OSD DMA Instruction Pool Size
#define OSD_BOOTUP_DELAYSHOW_CNT       3

// (16*(4+1))*3 + 16*3= 288
//#define DUMP_SIZE 288

#define MT8280_OSD_HEADER_ADDRESS_1         0x80000
#define MT8280_OSD_HEADER_ADDRESS_2         0x80400
#define MT8280_OSD_PALETTE_ADDRESS_1        0x80800
#define MT8280_OSD_PALETTE_ADDRESS_2        0x80c00
#define MT8280_OSD_PALETTE_ADDRESS_3        0x81000
#define MT8280_OSD_PALETTE_ADDRESS_4        0x81400
#define MT8280_OSD_BITMAP_ADDRESS_1         0x82000
#define MT8280_OSD_BITMAP_ADDRESS_2         0x280000
#define MT8280_PALETTE_NUMBER               4

#define MT8280_REG_DMA_C0                   0x70C0
#define MT8280_REG_DMA_FIFO_RESET           0x70C4
#if MT8282_DMA_Setting 
#define MT8280_REG_DMA_ADDRESS              0x70b4
#define MT8282_OSD_FAKE_HDR_44               0x7244
#define MT8282_REG_FADING_RATIO             0x7209
#else
#define MT8280_REG_DMA_ADDRESS              0x70C8
#endif
#define MT8280_REG_OSD_HEADER               0x7204
#define MT8280_REG_OSD_UPDATE               0x7000
#define MT8280_REG_I2C_DMA                  0x70C5
#define MT8280_DMA_ALIGN                    1024

#define MT8283_REG_OSD_L_FADING_REG               0x2208
#define MT8283_REG_OSD_R_FADING_REG               0x2308

BOOL    fgRecoveryFadingValue,fgRecoveryFadingValueTrigger;
UINT8    u1TriggerFadingValueSyncCount;


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT32 u4HeaderAddr;
    UINT32 u4PaletteAddr;
    UINT32 u4BitmapAddr;
} OSD_DMA_TARGET_INFO_T;

typedef struct _OSD_DMA_INSTRUCTION_FIELD_T
{
    /* DWORD - 000 */
    UINT32          fgCen               :   1;
    UINT32          fgDen               :   1;
    UINT32          fgEoc               :   1;
    UINT32                              :   5;
    UINT32          u4CSize             :   7;
    UINT32                              :   1;
    UINT32          u4Delay             :   6;
    UINT32                              :   10;

    /* DWORD - 004 */
    UINT32          u4DAddr             :   29;
    UINT32                              :   3;

    /* DWORD - 008 */
    UINT32          u4DSize             :   24;
    UINT32                              :   8;

    /* DWORD - 00C */
    UINT32          u4TestPat           :   32;

LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_DMA_INSTRUCTION_FIELD_T;

/*lint -save -e960 */
typedef union _OSD_DMA_INSTRUCTION_T
{
    UINT32                          au4Reg[4];
    OSD_DMA_INSTRUCTION_FIELD_T     rField;
} OSD_DMA_INSTRUCTION_T;
/*lint -restore */

typedef struct _OSD_DMA_CTRANSFER_FIELD_T
{
    /* DWORD - 000 */
    UINT32          u4Addr1L            :   8;
    UINT32          u4Addr1H            :   8;
    UINT32          u4Data1             :   8;
    UINT32          u4Parity1           :   8;

    /* DWORD - 004 */
    UINT32          u4Addr2L            :   8;
    UINT32          u4Addr2H            :   8;
    UINT32          u4Data2             :   8;
    UINT32          u4Parity2           :   8;

    /* DWORD - 008 */
    UINT32          u4Addr3L            :   8;
    UINT32          u4Addr3H            :   8;
    UINT32          u4Data3             :   8;
    UINT32          u4Parity3           :   8;

    /* DWORD - 00C */
    UINT32          u4Addr4L            :   8;
    UINT32          u4Addr4H            :   8;
    UINT32          u4Data4             :   8;
    UINT32          u4Parity4           :   8;

LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) OSD_DMA_CTRANSFER_FIELD_T;

/*lint -save -e960 */
typedef union _OSD_DMA_CTRANSFER_T
{
    UINT32                          au4Reg[4];
    OSD_DMA_CTRANSFER_FIELD_T       rField;
} OSD_DMA_CTRANSFER_T;
/*lint -restore */


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CALLOC_C_TRANSFER(X) \
    do \
    { \
        (X) = (OSD_DMA_CTRANSFER_T*)(void*)&_pu4DmaIPool[_u4DmaIPoolIdx]; \
        VERIFY(NULL != x_memset((void*)(X), 0xff, sizeof(OSD_DMA_CTRANSFER_T))); \
        _u4DmaIPoolIdx += 4; \
    } \
    while (0)


//-----------------------------------------------------------------------------
// Static variable
//-----------------------------------------------------------------------------

static UINT32 _u4DmaInit = 0;
static OSD_OP_MODE_T _eMode = OSD_OP_NORMAL;
static CHAR* _acDmaModeName[3] = {"NORMAL", "DMA", "GFX"};

static HANDLE_T _hTransferRegion = (HANDLE_T)NULL; // transfer region protection
static HANDLE_T _hDmaSubmit = (HANDLE_T)NULL; // dma submit protection
static HANDLE_T _hDmaThread = (HANDLE_T)NULL; // thread lock

static volatile OSD_RGN_UNION_T* _pRegion = 0;
static volatile UINT32* _pu4DmaIPool = 0;
static UINT32 _u4DmaIPoolIdx = 0;

static UINT32 _u4DmaReConfig = 1;
static OSD_COLOR_MODE_T _eDmaSourceColorMode = OSD_CM_RESERVED_0;       // MT5382p Color mode
static UINT32 _u4DmaSourceWidth = 0;                                    // MT5382p Ihw
static UINT32 _u4DmaSourceHeight = 0;                                   // MT5382p Ivw

static UINT32 _u4DmaUpdatePeriod = OSD_DMA_UPDATE_PERIOD;
static UINT32 _u4DmaVsyncDelay = OSD_DMA_UPDATE_PERIOD;
static BOOL _fgEnableI2C = FALSE;
static UINT32 _u4PaletteUpdateCount = 0;

static UINT32 _u4DmaTargetIdx = 0;
static OSD_DMA_TARGET_INFO_T _arDmaTargetInfo[OSD_DMA_TARGET_POOL_NS];

#ifdef DRV_SUPPORT_MT8283
UINT8  _u1OSDDelayShow = 0;
UINT8  _u1OSDDelayShow_1 = 0;
#endif
//BOOL   _fgOSDShowDelayon = FALSE;


static UINT32 _au4TargetHeaderAddress[OSD_DMA_TARGET_POOL_NS] =
{
    MT8280_OSD_HEADER_ADDRESS_1,
    MT8280_OSD_HEADER_ADDRESS_2
};
static UINT32 _au4TargetBitmapAddress[OSD_DMA_TARGET_POOL_NS] =
{
    MT8280_OSD_BITMAP_ADDRESS_1,
    MT8280_OSD_BITMAP_ADDRESS_2
};
static UINT32 _au4PaletteMapping[MT8280_PALETTE_NUMBER];
static UINT32 _au4TargetPaletteAddress[MT8280_PALETTE_NUMBER] =
{
    MT8280_OSD_PALETTE_ADDRESS_1,
    MT8280_OSD_PALETTE_ADDRESS_2,
    MT8280_OSD_PALETTE_ADDRESS_3,
    MT8280_OSD_PALETTE_ADDRESS_4,
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _OsdDmaThread(void *pvArgs)
{
    UNUSED(pvArgs);

    while(1)
    {
        VERIFY(x_sema_lock(_hDmaThread, X_SEMA_OPTION_WAIT) == OSR_OK);
        IGNORE_RET(OSD_DMA_TransferRegion((UINT32)OSD_PLANE_2));
    }
}

static void _OsdDmaDumpInstruction(void)
{
    UINT32 i;
    //UINT8* pu1Data;
    UINT32* pu4Data;

    LOG(7, "DMA Instruction(0x%x)\n", (UINT32)_pu4DmaIPool);

    pu4Data = (UINT32*) _pu4DmaIPool;
    for (i = 0; i < _u4DmaIPoolIdx; i += 4)
    {
        LOG(7, "\t%08x %08x %08x %08x\n",
            pu4Data[i], pu4Data[i+1], pu4Data[i+2], pu4Data[i+3]);
    }
    UNUSED(pu4Data);
}

static void _OsdDmaSubmit(void)
{
    INT32 i4Temp;

#ifndef CC_MTK_LOADER
    HalFlushInvalidateDCache();
#endif
    _OsdDmaDumpInstruction();

    if (_eMode != OSD_OP_DMA)
    {
        return;
    }
    
    // enable ISR
    // OSD local interrupt enable register 0x20021030 bit9-DMA done, bit10-DMA ack
    // OSD local interrupt status register 0x20021034 bit9-DMA done, bit10-DMA ack
    //IO_REG32(OSD_BASE, 0x34) = (IO_REG32(OSD_BASE, 0x34) | 0x200);
    //IO_REG32(OSD_BASE, 0x30) = (IO_REG32(OSD_BASE, 0x30) | 0x200);
    IO_REG32(OSD_BASE, 0x34) = (IO_REG32(OSD_BASE, 0x34) | 0x600);
    IO_REG32(OSD_BASE, 0x30) = (IO_REG32(OSD_BASE, 0x30) | 0x600);

    // start transfer
    i4Temp = x_sema_lock(_hDmaSubmit, X_SEMA_OPTION_NOWAIT);
    UNUSED(i4Temp);
    //IO_WRITE32(OSD_BASE, 0x74, (UINT32)_pu4DmaIPool);
    IO_WRITE32MSK(OSD_BASE, 0x74, (UINT32)_pu4DmaIPool,0x0fffffff);
    #ifdef DRV_SUPPORT_EXTMJC_MODE7
    if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
    {
        IO_REG32(OSD_BASE, 0x70) = (IO_REG32(OSD_BASE, 0x70) | 0x4);
    }
    #else
    IO_REG32(OSD_BASE, 0x70) = (IO_REG32(OSD_BASE, 0x70) | 0x6);
    #endif

    // wait transfer complete
    if (x_sema_lock_timeout(_hDmaSubmit, 1000) != OSR_OK)
    {
        LOG(1, "DMA Time out!\n");
    }
    LOG(6, "DMA Success!\n");
}

static UINT8 _OsdDmaCalculateParity(UINT32 u4Address, UINT32 u4Data)
{
    UINT8 u1Parity;
    UINT8 b[32];
    UINT32 u4HammingCode, i;

    u4HammingCode = ((u4Address & 0xfff8) << 14) |
                    ((u4Address & 0x7) << 13) |
                    ((u4Data & 0xf0) << 5) |
                    ((u4Data & 0xe) << 4) |
                    ((u4Data & 1) << 3);

    for (i = 0; i < 32; i++)
    {
        b[i] = (u4HammingCode >> i)& 1;
    }

    b[1] = b[3] ^ b[5] ^ b[7] ^ b[9] ^ b[11] ^ b[13] ^ b[15] ^ b[17] ^ b[19] ^
           b[21] ^ b[23] ^ b[25] ^ b[27] ^ b[29];
    b[2] = b[3] ^ b[6] ^ b[7] ^ b[10] ^ b[11] ^ b[14] ^ b[15] ^ b[18] ^ b[19] ^
           b[22] ^ b[23] ^ b[26] ^ b[27];
    b[4] = b[5] ^ b[6] ^ b[7] ^ b[12] ^ b[13] ^ b[14] ^ b[15] ^ b[20] ^ b[21] ^
           b[22] ^ b[23] ^ b[28] ^ b[29];
    b[8] = b[9] ^ b[10] ^ b[11] ^ b[12] ^ b[13] ^ b[14] ^ b[15] ^ b[24] ^
           b[25] ^ b[26] ^ b[27] ^ b[28] ^ b[29];
    b[16] = b[17] ^ b[18] ^ b[19] ^ b[20] ^ b[21] ^ b[22] ^ b[23] ^ b[24] ^
            b[25] ^ b[26] ^ b[27] ^ b[28] ^ b[29];

    b[30] = b[1];
    for (i = 2; i <= 29; i++)
    {
        b[30] ^= b[i];
    }

    u1Parity = b[1] | (b[2] << 1) | (b[4] << 2) | (b[8] << 3) | (b[16] << 4) |
               (b[30] << 5);
    return u1Parity;
}

static void _OsdDmaI2CWriteReg(UINT32 u4Reg, UINT32 u4Data, BOOL fg8bitData)
{
		#ifdef DRV_SUPPORT_MT8283
		if(u4Reg !=0 )
			return;
		#endif
#ifndef CC_MTK_FPGA
    if (!fg8bitData)
    {
        IGNORE_RET(SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, u4Reg + 3, ((UINT8*)(&u4Data) + 3), 1));
        IGNORE_RET(SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, u4Reg + 2, ((UINT8*)(&u4Data) + 2), 1));
        IGNORE_RET(SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, u4Reg + 1, ((UINT8*)(&u4Data) + 1), 1));
    }

    IGNORE_RET(SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, u4Reg + 0, ((UINT8*)(&u4Data) + 0), 1));
#endif
    LOG(6, "I2C write 8280 reg(0x%x)=0x%x\n", u4Reg, u4Data);
}


static void _OsdDmaI2CWriteMem(UINT32 u4DstAddress, UINT32 u4SrcAddress,
                               UINT32 u4Size)
{
    UINT32 i = 0;
    #ifdef DRV_SUPPORT_MT8283
		if(u4DstAddress !=0 )
			return;
	#endif
    LOG(6, "I2C copy to 8280(0x%x) from 5382P(0x%x), size=%d\n",
        u4DstAddress, u4SrcAddress, u4Size);

#if MT8282_DMA_Setting
    //_OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS, u4DstAddress, FALSE);
    _OsdDmaI2CWriteReg(MT8280_REG_I2C_DMA, 0x7, TRUE);

    // MT8281 FIFO Depth Setting  MT8280_REG_DMA_FIFO_RESET,MT8280_REG_DMA_C0
   // _OsdDmaI2CWriteReg(MT8280_REG_DMA_C0, 0x3, TRUE);
   // _OsdDmaI2CWriteReg(MT8280_REG_DMA_C0, 0x2, TRUE);
    //_OsdDmaI2CWriteReg(MT8280_REG_DMA_C0, 0x3, TRUE);    
#else
    _OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS, u4DstAddress, FALSE);
    _OsdDmaI2CWriteReg(MT8280_REG_I2C_DMA, 0x4, TRUE);
    _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf3, TRUE);
    _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf2, TRUE);
#endif

    while (u4Size > 0)
    {

        if (u4Size < 256)
        {
            i = u4Size;
#if MT8282_DMA_Setting            
	    switch (i)
	    {
	    	case 16:
	    	case 32:
	    	case 48:
	    		_OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf0, TRUE);
	    		break;
	    	case 64:
	    		_OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf1, TRUE);
	    		break;
	    	case 128:
	    		_OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf2, TRUE);
	    		break;
	    	default:
	    		_OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf3, TRUE);
	    		break;
	    }	    	
#endif
        }
        else
        {
            i = 256;
#if MT8282_DMA_Setting            
           // _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf8, TRUE);
           _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf6, TRUE);
#endif
        }

        u4Size -= i;
#ifndef CC_MTK_FPGA
#if MT8282_DMA_Setting 
        IGNORE_RET(SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, 0x9981, (UINT8*)u4SrcAddress, i));      
	//_OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf8, TRUE);
	_OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xf6, TRUE);
#else
        IGNORE_RET(SIF_WriteMultipleSubAddr(0x100, 0xc, 0x2, 0x2534, (UINT8*)u4SrcAddress, i));
#endif
#endif
        u4SrcAddress += i;
        //x_thread_delay(1);
    }
#if MT8282_DMA_Setting 
    _OsdDmaI2CWriteReg(MT8280_REG_I2C_DMA, 0xB, TRUE); 
#else
    _OsdDmaI2CWriteReg(MT8280_REG_I2C_DMA, 0x0, TRUE);
#endif
}


static void _OsdDmaFillCTransfer(volatile OSD_DMA_CTRANSFER_T* prCTansfer,
                                 UINT32 u4TargetAddress, UINT32 u4Value,
                                 UINT32 u4Parity)
{
    if (prCTansfer == NULL)
    {
        return;
    }
    prCTansfer->rField.u4Addr1H = (u4TargetAddress >> 8) & 0xff;
    prCTansfer->rField.u4Addr1L = u4TargetAddress & 0xff;
    prCTansfer->rField.u4Data1 = u4Value & 0xff;
    if (u4Parity == 0)
    {
        prCTansfer->rField.u4Parity1 =
            _OsdDmaCalculateParity(u4TargetAddress, prCTansfer->rField.u4Data1);
    }
    else
    {
        prCTansfer->rField.u4Parity1 = u4Parity & 0xff;
    }

    u4TargetAddress++;
    prCTansfer->rField.u4Addr2H = (u4TargetAddress >> 8) & 0xff;
    prCTansfer->rField.u4Addr2L = u4TargetAddress & 0xff;
    prCTansfer->rField.u4Data2 = (u4Value >> 8) & 0xff;
    if (u4Parity == 0)
    {
        prCTansfer->rField.u4Parity2 =
            _OsdDmaCalculateParity(u4TargetAddress, prCTansfer->rField.u4Data2);
    }
    else
    {
        prCTansfer->rField.u4Parity2 = (u4Parity >> 8) & 0xff;
    }

    u4TargetAddress++;
    prCTansfer->rField.u4Addr3H = (u4TargetAddress >> 8) & 0xff;
    prCTansfer->rField.u4Addr3L = u4TargetAddress & 0xff;
    prCTansfer->rField.u4Data3 = (u4Value >> 16) & 0xff;
    if (u4Parity == 0)
    {
        prCTansfer->rField.u4Parity3 =
            _OsdDmaCalculateParity(u4TargetAddress, prCTansfer->rField.u4Data3);
    }
    else
    {
        prCTansfer->rField.u4Parity3 = (u4Parity >> 16) & 0xff;
    }

    u4TargetAddress++;
    prCTansfer->rField.u4Addr4H = (u4TargetAddress >> 8) & 0xff;
    prCTansfer->rField.u4Addr4L = u4TargetAddress & 0xff;
    prCTansfer->rField.u4Data4 = (u4Value >> 24) & 0xff;
    if (u4Parity == 0)
    {
        prCTansfer->rField.u4Parity4 =
            _OsdDmaCalculateParity(u4TargetAddress, prCTansfer->rField.u4Data4);
    }
    else
    {
        prCTansfer->rField.u4Parity4 = (u4Parity >> 24) & 0xff;
    }
}

static void _OsdDmaSetTargetAddr(UINT32 u4Address)
{
    volatile OSD_DMA_INSTRUCTION_T* _prInstruction;
    volatile OSD_DMA_CTRANSFER_T* prCTansfer;

    // MT8280 register
    // RXDMA initial
    //0x70C8=0x00800000  // dram write start address
    //0x70C4=0x000000F7  // FIFO reset
    //0x70C4=0x000000F6  // release reset
    // RXDMA 16-bit transfer mode
    //0x70C0=0x00000007
    //0x70C0=0x00000006 // reset RXDMA
    //0x70C0=0x00000007 // enable RXDMA, write protection enable

    _prInstruction = (OSD_DMA_INSTRUCTION_T*)(void*)&_pu4DmaIPool[_u4DmaIPoolIdx];
    _u4DmaIPoolIdx += 4;    // 16 byte

    _prInstruction->rField.fgCen = 1;
    _prInstruction->rField.fgDen = 0;
    _prInstruction->rField.fgEoc = 0;
    _prInstruction->rField.u4Delay = 0;
    _prInstruction->rField.u4DAddr = 0;
    _prInstruction->rField.u4DSize = 0;
    _prInstruction->rField.u4CSize = 2;

    CALLOC_C_TRANSFER(prCTansfer);
    //0x70C8=0x00800000  // dram write start address
    _OsdDmaFillCTransfer(prCTansfer, MT8280_REG_DMA_ADDRESS, u4Address, 0);

    CALLOC_C_TRANSFER(prCTansfer);
    //0x70C4=0x000000F7  // FIFO reset
    //0x70C4=0x000000F6  // release reset
    //0x70C0=0x00000007
    //0x70C0=0x00000006 // reset RXDMA
    prCTansfer->rField.u4Addr1H = (MT8280_REG_DMA_FIFO_RESET >> 8) & 0xff;
    prCTansfer->rField.u4Addr1L = MT8280_REG_DMA_FIFO_RESET & 0xff;
    prCTansfer->rField.u4Data1 = 0xF7;
    prCTansfer->rField.u4Parity1 =
        _OsdDmaCalculateParity(MT8280_REG_DMA_FIFO_RESET,
                               prCTansfer->rField.u4Data1);

    prCTansfer->rField.u4Addr2H = (MT8280_REG_DMA_FIFO_RESET >> 8) & 0xff;
    prCTansfer->rField.u4Addr2L = MT8280_REG_DMA_FIFO_RESET & 0xff;
    prCTansfer->rField.u4Data2 = 0xF6;
    prCTansfer->rField.u4Parity2 =
        _OsdDmaCalculateParity(MT8280_REG_DMA_FIFO_RESET,
                               prCTansfer->rField.u4Data2);

}

static void _OsdDmaSetTransferData(UINT32 u4Address, UINT32 u4Size, BOOL fgEoc)
{
    volatile OSD_DMA_INSTRUCTION_T* _prInstruction;

    ASSERT((u4Address & 0x3f) == 0);

    _prInstruction = (OSD_DMA_INSTRUCTION_T*)(void*)&_pu4DmaIPool[_u4DmaIPoolIdx];
    _u4DmaIPoolIdx += 4;    // 16 byte

    // align size to 64Byte
    u4Size = OSD_DMA_ALIGN(u4Size);

    _prInstruction->rField.fgCen = 0;
    _prInstruction->rField.fgDen = 1;
    _prInstruction->rField.fgEoc = fgEoc;
    _prInstruction->rField.u4Delay = 0;
    _prInstruction->rField.u4DAddr = u4Address;
    _prInstruction->rField.u4DSize = u4Size;
    _prInstruction->rField.u4CSize = 0;
}

#if MT8282_OSD_Shift_SW_Modify
static void _OsdDmaChgHeaderAddr(UINT32 u4Address)
{
    volatile OSD_DMA_INSTRUCTION_T* _prInstruction = NULL;
    volatile OSD_DMA_CTRANSFER_T* prCTansfer = NULL;
    if (_fgEnableI2C)
    {
        UNUSED(_prInstruction);
        UNUSED(prCTansfer);
#if MT8282_FAKER_MODE      
#if MT8281_DMA_AUTO
#else
	 _OsdDmaI2CWriteReg(MT8282_OSD_FAKE_HDR_44, u4Address >> 4, FALSE);
#endif
#else
        _OsdDmaI2CWriteReg(MT8280_REG_OSD_HEADER, u4Address >> 4, FALSE);
        _OsdDmaI2CWriteReg(MT8280_REG_OSD_UPDATE, 1 ,TRUE);
#endif
    }
    else
    {
        _prInstruction = (OSD_DMA_INSTRUCTION_T*)(void*)&_pu4DmaIPool[_u4DmaIPoolIdx];
        _u4DmaIPoolIdx += 4;    // 16 byte

        _prInstruction->rField.fgCen = 1;
        _prInstruction->rField.fgDen = 0;
        _prInstruction->rField.fgEoc = 1;       // End of Command
        _prInstruction->rField.u4Delay = 0;
        _prInstruction->rField.u4DAddr = 0;
        _prInstruction->rField.u4DSize = 0;
        _prInstruction->rField.u4CSize = 2;

        CALLOC_C_TRANSFER(prCTansfer);
        // OSD header register 0x7204[23:0] 16 byte unit
        _OsdDmaFillCTransfer(prCTansfer, MT8280_REG_OSD_HEADER, u4Address >> 4, 0);

        CALLOC_C_TRANSFER(prCTansfer);
        // OSD update shadow, 0x7000[0]=1
        prCTansfer->rField.u4Addr1H = (MT8280_REG_OSD_UPDATE >> 8) & 0xff;
        prCTansfer->rField.u4Addr1L = MT8280_REG_OSD_UPDATE & 0xff;
        prCTansfer->rField.u4Data1 = 1;
        prCTansfer->rField.u4Parity1 =
            _OsdDmaCalculateParity(MT8280_REG_OSD_UPDATE,
                                   prCTansfer->rField.u4Data1);
    }
}
#endif

static void _OsdDmaTransferHeader(UINT32 u4Address, UINT32 u4Size)
{
    UINT32 i;
    UINT32* pu4Data;

#if 1
    static UINT32    apui1_82_hdr_addr[OSD_DMA_TARGET_POOL_NS] = {0,0};
    static UINT8     apui1_82_hdr[OSD_DMA_TARGET_POOL_NS][1024];
    UINT32    j = 0;

    if ( u4Size>1024 ) //for fixing klocwork warning
        {
            Printf( "_OsdDmaTransferHeader() Error! u4Size overflow\r\n" );
            return;
        }

    if ( (apui1_82_hdr_addr[0] == 0) && (apui1_82_hdr_addr[1] == 0) )
    {
        x_memset( &apui1_82_hdr[0][0], 0, u4Size);
        x_memset( &apui1_82_hdr[1][0], 0xFF, u4Size);
    }

    /* Check if header is the same as old one */
    for( j=0; j<OSD_DMA_TARGET_POOL_NS; j++ )
    {
        if ( (apui1_82_hdr_addr[j] != 0) && (apui1_82_hdr[j] != NULL) )
        {
            if ( apui1_82_hdr_addr[j] == _arDmaTargetInfo[_u4DmaTargetIdx].u4HeaderAddr )
            {
                if ( x_memcmp((void*)&apui1_82_hdr[j][0], (void*)u4Address, u4Size)==0 )
                {
                    return;
                }
                else
                {
                    x_memcpy((void*)&apui1_82_hdr[j][0], (void*)u4Address, u4Size);
                    break;
                }
            }
        }
    }
    if ( j>=OSD_DMA_TARGET_POOL_NS )
    {
        for( j=0; j<OSD_DMA_TARGET_POOL_NS; j++ )
        {
            if ( apui1_82_hdr_addr[j] == 0 )
            {
                apui1_82_hdr_addr[j] = _arDmaTargetInfo[_u4DmaTargetIdx].u4HeaderAddr;
                x_memcpy((void*)&apui1_82_hdr[j][0], (void*)u4Address, u4Size);
                Printf( "Set hdr[%d] = 0x%X\r\n", j, apui1_82_hdr_addr[j] );
                break;
            }
        }
        if ( j>=OSD_DMA_TARGET_POOL_NS )
        {
            Printf( "_OsdDmaTransferHeader() Error\r\n" );
            return;
        }
    }
#endif

    if (_fgEnableI2C)
    {
        _OsdDmaI2CWriteMem(_arDmaTargetInfo[_u4DmaTargetIdx].u4HeaderAddr, u4Address, u4Size);
    }
    else
    {
        _OsdDmaSetTargetAddr(_arDmaTargetInfo[_u4DmaTargetIdx].u4HeaderAddr);
        _OsdDmaSetTransferData(u4Address, u4Size, FALSE);
    }

    pu4Data = (UINT32*) u4Address;
    LOG(7, "transfer header(0x%x)\n", u4Address);
    for (i = 0; i < 8; i += 4)
    {
        LOG(7, "\t%08x %08x %08x %08x\n",
            pu4Data[i], pu4Data[i+1], pu4Data[i+2], pu4Data[i+3]);
    }
    //remove warning
    UNUSED(pu4Data);
}


static void _OsdDmaTransferPalette(UINT32 u4Address, UINT32 u4Size)
{
#if 0
    static UINT8*   pui1_previous_pallet = NULL;

    static UINT32    apui1_82_pallet_cnt[OSD_DMA_TARGET_POOL_NS] = {0,0};
    static UINT32    apui1_82_pallet_addr[OSD_DMA_TARGET_POOL_NS] = {0,0};
    static UINT8     apui1_82_pallet[OSD_DMA_TARGET_POOL_NS][1024];
    static BOOL      b_enable_pallet = TRUE;
    static UINT32    ui4_write_cnt = OSD_DMA_TARGET_POOL_NS;
    UINT8 bData;
    UINT32    j = 0;

    if ( b_enable_pallet )
    {
        if ( apui1_82_pallet_addr[0] == 0 && apui1_82_pallet_addr[1] == 0 )
        {
            x_memset( &apui1_82_pallet[0][0], 0, u4Size);
            x_memset( &apui1_82_pallet[1][0], 0xFF, u4Size);
        }

        /* Check if palette is the same as old one */
        for( j=0; j<OSD_DMA_TARGET_POOL_NS; j++ )
        {
            if ( apui1_82_pallet_addr[j] != 0 && apui1_82_pallet[j] != NULL )
            {
                if ( apui1_82_pallet_addr[j] == _arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr )
                {
                    if ( x_memcmp( &apui1_82_pallet[j][0], u4Address, u4Size)==0 )
                    {
                        if ( apui1_82_pallet_cnt[j] > ui4_write_cnt )
                        {
                            return;
                        }
                        else
                        {
                            apui1_82_pallet_cnt[j]++;
                            break;
                        }
                    }
                    else
                    {
                        x_memcpy( &apui1_82_pallet[j][0], u4Address, u4Size);
                        apui1_82_pallet_cnt[j] = 0;
                        break;
                    }
                }
            }
        }
        if ( j>=OSD_DMA_TARGET_POOL_NS )
        {
            for( j=0; j<OSD_DMA_TARGET_POOL_NS; j++ )
            {
                if ( apui1_82_pallet_addr[j] == 0 )
                {
                    apui1_82_pallet_addr[j] = _arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr;
                    x_memcpy( &apui1_82_pallet[j][0], u4Address, u4Size);
                    Printf( "Set Pallet[%d] = 0x%X\r\n", j, apui1_82_pallet_addr[j] );
                    break;
                }
            }
            if ( j>=OSD_DMA_TARGET_POOL_NS )
            {
                Printf( "_OsdDmaTransferPalette() Error\r\n" );
                return;
            }
        }
    }
#endif

    LOG(5, "rx=0x%x, tx=0x%x, size=0x%x\n", _arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr, u4Address, u4Size);
    if (_fgEnableI2C)
    {
        _OsdDmaI2CWriteMem(_arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr, u4Address, u4Size);
    }
    else
    {
        _OsdDmaSetTargetAddr(_arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr);
        _OsdDmaSetTransferData(u4Address, u4Size, FALSE);
    }
}

static void _OsdDmaTransferBitmap(UINT32 u4Address, UINT32 u4Size)
{

    if (_fgEnableI2C)
    {
        // use I2C dma to set bitmap address   
#if MT8281_DMA_AUTO        
	_OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS, _arDmaTargetInfo[0].u4BitmapAddr, FALSE);
#else
        _OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS, _arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr, FALSE);
#endif
#if MT8282_DMA_Setting	
	 // 3. enable 8281 DMA         
	// I2C write 8281 0x70C0[0] = 1	       
	// 4. reset 8281 DMA        
	// I2C write 8281 0x70C0[16] = 0 	// reset      
	// I2C write 8281 0x70C0[16] = 1	// release reset 
#if MT8281_DMA_AUTO	
#else
	 _OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS, 0x3, TRUE);		
#endif
	 //_OsdDmaI2CWriteReg(0x70c2, 0x0, TRUE);		
	 //_OsdDmaI2CWriteReg(0x70c2, 0x1, TRUE);
#else        
        _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xff, TRUE);
        _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xfe, TRUE);
#endif        

        // use LVDS to transfer bitmap
        _u4DmaIPoolIdx = 0;
        _OsdDmaSetTransferData(u4Address, u4Size, TRUE);
        _OsdDmaSubmit();
    }
    else
    {
        _OsdDmaSetTargetAddr(_arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr);
        _OsdDmaSetTransferData(u4Address, u4Size, FALSE);
    }
}

static void _OsdDmaConfig(OSD_COLOR_MODE_T eColorMode, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 u4Idx;
    UINT32 u4Addr;

    if ((eColorMode == _eDmaSourceColorMode) &&
        (u4Width == _u4DmaSourceWidth) &&
        (u4Height == _u4DmaSourceHeight) &&
        (_u4DmaReConfig == 0))
    {
        return;
    }

    LOG(5, "DMA Reconfig CM(%d) Width(%d) Height(%d)\n",
        eColorMode, u4Width, u4Height);

    _eDmaSourceColorMode = eColorMode;
    _u4DmaSourceWidth = u4Width;
    _u4DmaSourceHeight = u4Height;
    _u4DmaReConfig = 0;

    for (u4Idx = 0; u4Idx < OSD_DMA_TARGET_POOL_NS; u4Idx++)
    {
        // header address
        u4Addr = _au4TargetHeaderAddress[u4Idx];
        _arDmaTargetInfo[u4Idx].u4HeaderAddr = u4Addr;

        // palette address
        u4Addr = _au4TargetPaletteAddress[u4Idx];
        _arDmaTargetInfo[u4Idx].u4PaletteAddr = u4Addr;

        // bitmap address
        u4Addr = _au4TargetBitmapAddress[u4Idx];
        _arDmaTargetInfo[u4Idx].u4BitmapAddr = u4Addr;

        LOG(5, "\t8280 Buffer(%d) region(0x%x) palette(0x%x) bitmap(0x%x)\n",
            u4Idx,
            _arDmaTargetInfo[u4Idx].u4HeaderAddr,
            _arDmaTargetInfo[u4Idx].u4PaletteAddr,
            _arDmaTargetInfo[u4Idx].u4BitmapAddr);
    }

    _u4DmaTargetIdx = 0;
}

static void _OsdDmaIsr(UINT16 u2Vector)
{
    UINT32 u4Status;
    UNUSED(u2Vector);

    u4Status=IO_REG32(OSD_BASE, 0x34);

    //Printf("OSD Isr: 0x%x!\n",u4Status);
    if(u4Status & 0x200) //DMA done
    {
    	//Printf("---DMA Done!\n");
    	IO_REG32(OSD_BASE, 0x34) = (IO_REG32(OSD_BASE, 0x34) & (~0x200));
       IO_REG32(OSD_BASE, 0x30) = (IO_REG32(OSD_BASE, 0x30) & (~0x200));

       //Printf("read checksum=: 0x%x!\n",IO_READ32(OSD_BASE, 0x58));
    }

    if(u4Status & 0x400) //DMA ack
    {
    	//Printf("---DMA Ack!\n");
    	IO_REG32(OSD_BASE, 0x34) = (IO_REG32(OSD_BASE, 0x34) & (~0x400));
       //IO_REG32(OSD_BASE, 0x30) = (IO_REG32(OSD_BASE, 0x30) & (~0x400));
    }
    
    #if 0
    UNUSED(u2Vector);
    

    // Disable ISR
    // OSD local interrupt enable register 0x20021030 bit8
    // OSD local interrupt status register 0x20021034 bit8
    IO_REG32(OSD_BASE, 0x34) = (IO_REG32(OSD_BASE, 0x34) & (~0x200));
    IO_REG32(OSD_BASE, 0x30) = (IO_REG32(OSD_BASE, 0x30) & (~0x200));

    #endif
    // clear global isr
    if (!BIM_ClearIrq(VECTOR_OSD))
    {
    }
    
    VERIFY(x_sema_unlock(_hDmaSubmit) == OSR_OK);
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void OSD_DMA_Init(void)
{
    if (_u4DmaInit == 0)
    {
        x_os_isr_fct pfnOldIsr;
        UINT32 u4MallocAddr;
        UINT32 u4Size;
        HANDLE_T hThread;


        fgRecoveryFadingValue = FALSE;
        fgRecoveryFadingValueTrigger= FALSE;
        u1TriggerFadingValueSyncCount = 0;

        //malloc OSD header addr
        u4Size = (sizeof(OSD_RGN_UNION_T) + OSD_DMA_ALIGN_SIZE);
        u4MallocAddr = (UINT32) x_mem_alloc(u4Size);
	 if (u4MallocAddr == NULL)
       {
          return;
       }
        VERIFY(NULL != x_memset((void*)u4MallocAddr, 0, u4Size));

        // 64 byte alignment (D-transfer Start Address)
        _pRegion = (OSD_RGN_UNION_T*) OSD_DMA_ALIGN(u4MallocAddr);

        //malloc DMA instruction pool
        u4Size = ((OSD_DMA_IPOOL_SIZE * 4) + OSD_DMA_ALIGN_SIZE);
        u4MallocAddr = (UINT32) x_mem_alloc(u4Size);
	 if (u4MallocAddr == NULL)
       {
          return;
       }
        VERIFY(NULL != x_memset((void*)u4MallocAddr, 0, u4Size));

        // 16 byte alignment (C-transfer Start Address)
        _pu4DmaIPool = (UINT32*) OSD_DMA_ALIGN(u4MallocAddr);

        // create semaphore
        VERIFY(x_sema_create(&_hTransferRegion, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

        // create semaphore
        VERIFY(x_sema_create(&_hDmaSubmit, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        // create semaphore
        VERIFY(x_sema_create(&_hDmaThread, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

        // register ISR
        VERIFY(x_reg_isr(VECTOR_OSD, _OsdDmaIsr, &pfnOldIsr) == OSR_OK);

        // create osd dma thread for i2c
        VERIFY(x_thread_create(&hThread, OSD_DMA_THREAD_NAME,
                               OSD_DMA_THREAD_STACK_SIZE,
                               OSD_DMA_THREAD_PRIORITY,
                               _OsdDmaThread, 0, NULL) == OSR_OK);

#ifdef CC_MTK_LOADER
        UNUSED(_hTransferRegion);
        UNUSED(_hDmaSubmit);
        UNUSED(_hDmaThread);
        UNUSED(_OsdDmaIsr);
        UNUSED(_OsdDmaThread);
        UNUSED(pfnOldIsr);
        UNUSED(hThread);
#endif
        _u4DmaInit = 1;
    }

    // slow down dma speed
#if MT8282_DMA_Setting
    //IO_WRITE32MSK(OSD_BASE, 0x70, 0x60000080, 0xffff0080);
    IO_WRITE32MSK(OSD_BASE, 0x70, 0xf0f00000, 0xffff0000);
    //IO_WRITE32MSK(OSD_BASE, 0x68, 0x500, 0x00003000);
    //IO_WRITE32MSK(OSD_BASE, 0x68, 0x500, 0x00003700);
    IO_WRITE32MSK(OSD_BASE, 0x74, 0xc0000000, 0xc0000000);
    #ifdef DRV_SUPPORT_EXTMJC_MODE7
    if (DRVCUST_InitGet(eFlagDDRQfp)) //mt5387 condition
    {
        IO_WRITE32MSK(OSD_BASE, 0x74, 0xe0000000, 0xf0000000);
    }
    #else
    IO_WRITE32MSK(OSD_BASE, 0x74, 0xc0000000, 0xc0000000);
    #endif
#else
    IO_WRITE32MSK(OSD_BASE, 0x70, 0xc0000000, 0xffff0000);
#endif
//IO_REG32(OSD_BASE, 0x34) = (IO_REG32(OSD_BASE, 0x34) | 0x400);
//    IO_REG32(OSD_BASE, 0x30) = (IO_REG32(OSD_BASE, 0x30) | 0x400);
}

void OSD_DMA_SetTarget(UINT32 *au4TargetAddr)
{
    if (au4TargetAddr == NULL)
    {
        return;
    }
    _au4TargetHeaderAddress[0] = au4TargetAddr[0];
    _au4TargetHeaderAddress[1] = au4TargetAddr[1];
    _au4TargetPaletteAddress[0] = au4TargetAddr[2];
    _au4TargetPaletteAddress[1] = au4TargetAddr[3];
    _au4TargetPaletteAddress[2] = au4TargetAddr[4];
    _au4TargetBitmapAddress[0] = au4TargetAddr[5];
    _au4TargetBitmapAddress[1] = au4TargetAddr[6];
    _u4DmaReConfig = 1;
}

void OSD_DMA_EnableI2C(BOOL fgEnable)
{
    _fgEnableI2C = fgEnable;
    LOG(5, "use I2C to replace DMA=%d\n", fgEnable);
}

OSD_OP_MODE_T OSD_DMA_GetMode(void)
{
    return _eMode;
}

void OSD_DMA_SetMode(OSD_OP_MODE_T eMode)
{
    INT32 i4Ret = 0;
    UINT32 u4SrcW, u4SrcH, u4DstW, u4DstH;

    if (_u4DmaInit == 0)
    {
        return;
    }

    if (eMode == _eMode)
    {
        return;
    }

    LOG(4, "OSD_DMA_SetMode, mode=%s\n", _acDmaModeName[(UINT32)eMode]);
    // lock mutex semaphore
    VERIFY(x_sema_lock(_hTransferRegion, X_SEMA_OPTION_WAIT) == OSR_OK);
    LOG(4, "OSD_DMA_SetMode, after lock _hTransferRegion\n");

    // if previous mode is DMA, enable OSD_PLANE_2
    if (_eMode == OSD_OP_DMA)
    {
        //IGNORE_RET(OSD_PLA_Enable((UINT32)OSD_PLANE_2, TRUE));
        //IGNORE_RET(OSD_PLA_SetGlobeEnable((UINT32)OSD_PLANE_2, TRUE));
        //LOG(5, "enable OSD_PLANE_2\n");
    }
    // if previous mode is GFX, restore OSD scaling ratio
    else if (_eMode == OSD_OP_GFX)
    {
        IGNORE_RET(_OSD_BASE_SetAutoPos(0));
        IGNORE_RET(_OSD_BASE_SetVsEdge(0));
        IGNORE_RET(_OSD_BASE_SetScrnVStartTop(0));

        // restore screen size
        IGNORE_RET(_OSD_BASE_SetScrnHSize(PANEL_GetPanelWidth()));

        // restore scaling factor
        IGNORE_RET(_OSD_SC_GetSrcHSize((UINT32)OSD_SCALER_2, &u4SrcW));
        IGNORE_RET(_OSD_SC_GetSrcVSize((UINT32)OSD_SCALER_2, &u4SrcH));
        IGNORE_RET(_OSD_SC_GetDstHSize((UINT32)OSD_SCALER_2, &u4DstW));
        IGNORE_RET(_OSD_SC_GetDstVSize((UINT32)OSD_SCALER_2, &u4DstH));
        IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_2, TRUE,
                   u4SrcW, u4SrcH, u4DstW*2, u4DstH));

        // disable graphic mode
        IO_WRITE32MSK(OSD_BASE, 0x68, 0, (1<<4) | (1<<2));
    }

    // if new mode is DMA, disable(hide) OSD_PLANE_2
    if (eMode == OSD_OP_DMA)
    {
    	 _OsdDmaI2CWriteReg(MT8282_REG_FADING_RATIO,0x00,TRUE);
    	 fgRecoveryFadingValue = TRUE;
    	 
        IGNORE_RET(OSD_PLA_Enable((UINT32)OSD_PLANE_2, FALSE));
        IGNORE_RET(OSD_PLA_Enable((UINT32)OSD_PLANE_3, FALSE));
        // reset the delay to leave enough delay such taht OSD1 could disable
        // before DMA work
        _u4DmaVsyncDelay = OSD_DMA_UPDATE_PERIOD;
        _eMode = eMode;
    }
    // if new mode is GFX, horizontal down scale to half size
    else if (eMode == OSD_OP_GFX)
    {
        _eMode = eMode;
        IGNORE_RET(_OSD_SC_GetSrcHSize((UINT32)OSD_SCALER_2, &u4SrcW));
        IGNORE_RET(_OSD_SC_GetSrcVSize((UINT32)OSD_SCALER_2, &u4SrcH));
        IGNORE_RET(_OSD_SC_GetDstHSize((UINT32)OSD_SCALER_2, &u4DstW));
        IGNORE_RET(_OSD_SC_GetDstVSize((UINT32)OSD_SCALER_2, &u4DstH));

        IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_2, TRUE,
                   u4SrcW, u4SrcH, u4DstW/2, u4DstH));

        IGNORE_RET(_OSD_BASE_SetAutoPos(1));
        IGNORE_RET(_OSD_BASE_SetVsEdge(1));

        // screen size, 0x2002101c = 1/2 at 120hz
        IGNORE_RET(_OSD_BASE_SetScrnHSize(PANEL_GetPanelWidth()/2));
        IGNORE_RET(_OSD_BASE_SetScrnVStartTop(1));

        // fix value?
        IGNORE_RET(_OSD_BASE_SetOht(PANEL_GetHTotal60Hz()/2));
        IGNORE_RET(_OSD_BASE_SetOvt(PANEL_GetVTotal60Hz()));
        IGNORE_RET(_OSD_BASE_SetScrnHStartOsd2(0xb6));

        // OSD plane 2 only
        // 60Hz or 120Hz
        // 0x20021068[4] = 1, enable graphic mode
        // 0x20021068[2] = 1 >> 60Hz
        // 0x20021068[2] = 0 >> 120Hz
        IO_WRITE32MSK(OSD_BASE, 0x68, 1<<4, (1<<4) | (1<<2));

        // Optional, Half LVDS freq if 8280 not good, 0x20021068[8] = 1
    }
    else if (eMode == OSD_OP_NORMAL)
    {
        _eMode = eMode;
        // delay to wait last dma finish then we could just enable OSD1
        #ifdef DRV_SUPPORT_MT8283
        _OSD_PLA_SetFading(OSD_PLANE_2,0);
        _OSD_PLA_UpdateHwReg(OSD_PLANE_2);
        _OSD_PLA_SetFading(OSD_PLANE_3,0);
        _OSD_PLA_UpdateHwReg(OSD_PLANE_3);
        #endif
        x_thread_delay(25);
        IGNORE_RET(OSD_PLA_Enable((UINT32)OSD_PLANE_2, TRUE));
        IGNORE_RET(OSD_PLA_Enable((UINT32)OSD_PLANE_3, TRUE));
        #ifdef DRV_SUPPORT_MT8283
        _u1OSDDelayShow_1=15;
        #endif
	 
    }

    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    // double update to make DMA hardware work
    x_thread_delay(25);
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    // unlock mutex semaphore
    VERIFY(x_sema_unlock(_hTransferRegion) == OSR_OK);
    LOG(4, "OSD_DMA_SetMode, after unlock _hTransferRegion\n");

    UNUSED(i4Ret);
}

INT32 OSD_DMA_TransferRegion(UINT32 u4Plane)
{
    INT32 i4Ret = (INT32)OSD_RET_OK;
    UINT32 u4Region, u4BmpWidth, u4BmpHeight, u4PitchSize = 0;
    UINT32 u4PaletteAddr, u4BitmapAddr, u4RegionAddress;
    UINT32 u4Idx;
#if MT8282_OSD_Shift_SW_Modify
  // UINT32  _u4DmaTargetPrevIdx;
    UINT32 DMAWritePoint;	
#endif

    if (_u4DmaInit == 0)
    {
        return -(INT32)OSD_RET_UNINIT;
    }

    if (_eMode != OSD_OP_DMA)
    {
        return -(INT32)OSD_RET_INV_DISPLAY_MODE;
    }
    
    //#ifdef CC_MT5363
    #if 0
    if(gfgSwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    }
    #endif

    //_u1OSDDelayShow = 1;
    //_fgOSDShowDelayon = TRUE;
#if 0
    _u1OSDDelayShow++;
    if(_u1OSDDelayShow>=OSD_BOOTUP_DELAYSHOW_CNT)
    {
    	if(_u1OSDDelayShow==OSD_BOOTUP_DELAYSHOW_CNT)
    	{
    		//drv_extmjc_set_osd_SetFading(0xff);
    		//u1ExtmjcWriteRegByI2C(MT8282_REG_FADING_RATIO,0xff);
    		_OsdDmaI2CWriteReg(MT8282_REG_FADING_RATIO,0xff,TRUE);
    	}else
    	{
    		_u1OSDDelayShow=OSD_BOOTUP_DELAYSHOW_CNT+1;
    	}
    }
#endif    

	#ifdef DRV_SUPPORT_MT8283
	if(_u1OSDDelayShow==0)
	{
		_u1OSDDelayShow=34;
	}
	#endif

    OSD_VERIFY_PLANE(u4Plane);

    // we only transfer one region, so don't care link list
    /*
    if ((u4Region = OSD_PLA_GetFirstRegion(u4Plane)) >= 0)
    {
        return -(INT32)OSD_RET_NO_ATTACH_REGION;
    }
    */
    u4Region = OSD_PLA_GetFirstRegion(u4Plane);
    
    // lock mutex semaphore
    LOG(5, "OSD_DMA_TransferRegion, before lock _hTransferRegion\n");
    VERIFY(x_sema_lock(_hTransferRegion, X_SEMA_OPTION_WAIT) == OSR_OK);
    LOG(5, "OSD_DMA_TransferRegion, after lock _hTransferRegion\n");

    // reset DMA instruction pool
    _u4DmaIPoolIdx = 0;

    // get header address
    if (_OSD_RGN_GetAddress(u4Region, &u4RegionAddress) != (INT32)OSD_RET_OK)
    {
        // unlock mutex semaphore
        VERIFY(x_sema_unlock(_hTransferRegion) == OSR_OK);
        return -(INT32)OSD_RET_ERR_INTERNAL;
    }

    // config header
    x_memcpy((void*)_pRegion, (void*)u4RegionAddress, sizeof(OSD_RGN_UNION_T));

    // for dummy lint error
    u4BmpWidth = _pRegion->rField.u4Ihw;
    u4BmpHeight = _pRegion->rField.u4Ivw;
    // to check if region format was changed
    _OsdDmaConfig((OSD_COLOR_MODE_T)_pRegion->rField.u4ColorMode, u4BmpWidth, u4BmpHeight);

    //u4PaletteAddr = _pRegion->rField.u4PaletteAddr << 4;
    //u4BitmapAddr = _pRegion->rField.u4DataAddr << 4;
    u4PaletteAddr = (_pRegion->rField.u4PaletteAddr | ((_pRegion->rField.pal_adr28)<<24))<< 4;
    u4BitmapAddr = (_pRegion->rField.u4DataAddr | ((_pRegion->rField.data_adr28)<<24))<< 4;

    LOG(5, "5382P region(0x%x) palette(0x%x) bitmap(0x%x)\n",
           u4RegionAddress, u4PaletteAddr, u4BitmapAddr);

    // (1) transfer OSD header
    // we only transfer one region
    _pRegion->rField.fgNextOsdEn = 0;
    // replcae bitmap and pallette address
    _pRegion->rField.u4DataAddr = (_arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr >> 4);
    _pRegion->rField.data_adr28= (_arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr >> 28);

    // looking for a real target palette address
    for (u4Idx = 0; u4Idx < MT8280_PALETTE_NUMBER; u4Idx++)
    {
        if (_au4PaletteMapping[u4Idx] == u4PaletteAddr)
        {
            _arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr = _au4TargetPaletteAddress[u4Idx];
            break;
        }
    }
    _pRegion->rField.u4PaletteAddr = (_arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr >> 4);
    _pRegion->rField.pal_adr28= (_arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr >> 28);
    _pRegion->rField.u4AlphaSel = 0;
    _pRegion->rField.u4YrSel = 1;
    _pRegion->rField.u4UgSel = 2;
    _pRegion->rField.u4VbSel = 3;

    LOG(5, "8280 region(0x%x) palette(0x%x) bitmap(0x%x)\n",
           _arDmaTargetInfo[_u4DmaTargetIdx].u4HeaderAddr,
           _arDmaTargetInfo[_u4DmaTargetIdx].u4PaletteAddr,
           _arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr);

    _OsdDmaTransferHeader((UINT32)_pRegion, sizeof(OSD_RGN_UNION_T));

    // (2) transfer palette
    if (_u4PaletteUpdateCount > 0)
    {
        if ((_pRegion->rField.u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT2) ||
            (_pRegion->rField.u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT4) ||
            (_pRegion->rField.u4ColorMode == (UINT32)OSD_CM_YCBCR_CLUT8) ||
            (_pRegion->rField.u4ColorMode == (UINT32)OSD_CM_RGB_CLUT2) ||
            (_pRegion->rField.u4ColorMode == (UINT32)OSD_CM_RGB_CLUT4) ||
            (_pRegion->rField.u4ColorMode == (UINT32)OSD_CM_RGB_CLUT8))
        {
            _OsdDmaTransferPalette(u4PaletteAddr, OSD_PALETTE_SIZE);
            _u4PaletteUpdateCount--;
        }
    }

    // (3) transfer bitmap
    OSD_GET_PITCH_SIZE(_pRegion->rField.u4ColorMode, u4BmpWidth, u4PitchSize);
    _OsdDmaTransferBitmap(u4BitmapAddr, (u4PitchSize * u4BmpHeight));

    // (4) switch 8280 OSD header addr register
#if MT8282_FAKER_MODE
	/*if(_u4DmaTargetIdx==0)
		_u4DmaTargetPrevIdx = OSD_DMA_TARGET_POOL_NS-1;
	else
		_u4DmaTargetPrevIdx = _u4DmaTargetIdx-1;	*/
     //_OsdDmaChgHeaderAddr(_arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr);
#else
    _OsdDmaChgHeaderAddr(_arDmaTargetInfo[_u4DmaTargetIdx].u4HeaderAddr);
#endif

    // submit instruction
    if (!_fgEnableI2C)
    {
        _OsdDmaSubmit();
    }

    // change OSD pool
#if MT8282_OSD_Shift_SW_Modify
    //Check DMA Buffer Status by WC/Artee 2009/1/16    
    {    		
    	UINT8 ui1_data;    		
    	UINT8 u1ICRead;		    		    		
    	u1ICRead = SIF_ReadMultipleSubAddr(0x100, 0xc, 2, 0x70d8, &ui1_data, 1);		
    	DMAWritePoint = 0;    		
    	DMAWritePoint +=ui1_data;    		
    	u1ICRead = SIF_ReadMultipleSubAddr(0x100, 0xc, 2, 0x70d9, &ui1_data, 1);    		
    	DMAWritePoint +=(UINT32)(ui1_data<<8);    		
    	u1ICRead = SIF_ReadMultipleSubAddr(0x100, 0xc, 2, 0x70da, &ui1_data, 1);    		
    	DMAWritePoint +=(UINT32)(ui1_data<<16);    		
    	u1ICRead = SIF_ReadMultipleSubAddr(0x100, 0xc, 2, 0x70db, &ui1_data, 1);    		
    	DMAWritePoint +=(UINT32)(ui1_data<<24);
    	
    	//if((_arDmaTargetInfo[PrevIndex].u4BitmapAddr+0x1c2000)!=DMAWritePoint)	   
    	/*if((_arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr+(u4PitchSize * u4BmpHeight))!=DMAWritePoint)	    		
    	{    			
    		_OsdDmaChgHeaderAddr(_arDmaTargetInfo[_u4DmaTargetPrevIdx].u4BitmapAddr);
    	}else*/
    	{
    		_OsdDmaChgHeaderAddr(_arDmaTargetInfo[_u4DmaTargetIdx].u4BitmapAddr);
    		if (++_u4DmaTargetIdx >= OSD_DMA_TARGET_POOL_NS)
    		{
        		_u4DmaTargetIdx= 0;
    		}	
    		 
    	}
    	UNUSED(u1ICRead);      
    }	

   
#else
    if (++_u4DmaTargetIdx >= OSD_DMA_TARGET_POOL_NS)
    {
        _u4DmaTargetIdx= 0;
    }
#endif    

	if(fgRecoveryFadingValue)
	{
		fgRecoveryFadingValue = FALSE;
		fgRecoveryFadingValueTrigger = TRUE;
#if 0		
		if(_u4DmaVsyncDelay<_u4DmaUpdatePeriod)
		{
			u1TriggerFadingValueSyncCount = _u4DmaUpdatePeriod+1;
		}else
		{
			u1TriggerFadingValueSyncCount = 0;
		}
#endif		

		if(_u4DmaVsyncDelay<3)
		{
			u1TriggerFadingValueSyncCount = _u4DmaVsyncDelay+_u4DmaUpdatePeriod-3;
		}else
		{
			u1TriggerFadingValueSyncCount = _u4DmaVsyncDelay-3;
		}
		if(u1TriggerFadingValueSyncCount==0)
		{
			u1TriggerFadingValueSyncCount = _u4DmaUpdatePeriod;
		}
	}

    // unlock mutex semaphore
    VERIFY(x_sema_unlock(_hTransferRegion) == OSR_OK);
    LOG(5, "OSD_DMA_TransferRegion, after unlock _hTransferRegion\n");

    return i4Ret;
}

INT32 OSD_DMA_WriteRegister(UINT32 u4Reg, UINT32 u4Value, BOOL fg8bitData, UINT32 u4Parity)
{
    INT32 i4Ret;
    volatile OSD_DMA_INSTRUCTION_T* _prInstruction;
    volatile OSD_DMA_CTRANSFER_T* prCTansfer;

    i4Ret = (INT32)OSD_RET_OK;

    // lock mutex semaphore
    VERIFY(x_sema_lock(_hTransferRegion, X_SEMA_OPTION_WAIT) == OSR_OK);

    // 16Bit register
    u4Reg &= 0xffff;

    if (fg8bitData)
    {
        // 8Bit Data
        u4Value &= 0xff;
        u4Parity &= 0xff;
    }

    LOG(5, "write 8280 reg(0x%x)=(0x%x), parity=(0x%x)\n", u4Reg, u4Value, u4Parity);

    // reset DMA instruction pool
    _u4DmaIPoolIdx = 0;

    _prInstruction = (OSD_DMA_INSTRUCTION_T*)(void*)&_pu4DmaIPool[_u4DmaIPoolIdx];
    _u4DmaIPoolIdx += 4;    // 16 byte

    _prInstruction->rField.fgCen = 1;
    _prInstruction->rField.fgDen = 0;
    _prInstruction->rField.fgEoc = 1;       // End of Command
    _prInstruction->rField.u4Delay = 0;
    _prInstruction->rField.u4DAddr = 0;
    _prInstruction->rField.u4DSize = 0;
    _prInstruction->rField.u4CSize = 1;

    CALLOC_C_TRANSFER(prCTansfer);

    if (fg8bitData)
    {
        prCTansfer->rField.u4Addr1H = (u4Reg >> 8) & 0xff;
        prCTansfer->rField.u4Addr1L = u4Reg & 0xff;
        prCTansfer->rField.u4Data1 = u4Value & 0xff;
        prCTansfer->rField.u4Parity1 = u4Parity & 0xff;
    }
    else
    {
        _OsdDmaFillCTransfer(prCTansfer, u4Reg, u4Value, u4Parity);
    }

    _OsdDmaSubmit();

    // Unlock Mutex Semaphore
    VERIFY(x_sema_unlock(_hTransferRegion) == OSR_OK);

    return i4Ret;
}

INT32 OSD_DMA_Copy(UINT32 u4DstAddress, UINT32 u4SrcAddress, UINT32 u4Size)
{
    INT32 i4Ret;

    i4Ret = (INT32)OSD_RET_OK;

    LOG(5, "copy to 8280(0x%x) from 5382P(0x%x), size=%d\n",
        u4DstAddress, u4SrcAddress, u4Size);

    // lock mutex semaphore
    VERIFY(x_sema_lock(_hTransferRegion, X_SEMA_OPTION_WAIT) == OSR_OK);

    // reset DMA instruction pool
    _u4DmaIPoolIdx = 0;

	
   if (_fgEnableI2C)
   {
	LOG(5,"DMA I2c enable\n");
#if MT8281_DMA_AUTO	
	_OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS,u4DstAddress,FALSE);
#endif
#if MT8282_DMA_Setting
#if MT8281_DMA_AUTO	
#else
	_OsdDmaI2CWriteReg(MT8280_REG_DMA_ADDRESS, 0x3, TRUE);	
#endif
	_OsdDmaI2CWriteReg(0x70c2, 0x0, TRUE);	
	_OsdDmaI2CWriteReg(0x70c2, 0x1, TRUE);
#else
       _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xff, TRUE);
       _OsdDmaI2CWriteReg(MT8280_REG_DMA_FIFO_RESET, 0xfe, TRUE);
#endif       
   	_OsdDmaI2CWriteReg(MT8280_REG_DMA_C0, 0x02, TRUE);
     	_OsdDmaI2CWriteReg(MT8280_REG_DMA_C0, 0x03, TRUE);
   }else
   {		
   	LOG(5,"DMA I2c disable\n");
    _OsdDmaSetTargetAddr(u4DstAddress);
   }

    _OsdDmaSetTransferData(u4SrcAddress, u4Size, TRUE);

    _OsdDmaSubmit();

    // Unlock Mutex Semaphore
    VERIFY(x_sema_unlock(_hTransferRegion) == OSR_OK);

    return i4Ret;
}

void OSD_DMA_OnOutputVsync(void)
{
    if (_u4DmaInit == 0)
    {
        return;
    }

    if (_eMode == OSD_OP_DMA)
    {
        if (_u4DmaVsyncDelay == 0)
        {
           #ifdef DRV_SUPPORT_MT8283
           UINT32 pdata;
	    u1ExtmjcReadRegByI2C(0x2050,(UINT8*)&pdata);
	    if(pdata!=0)
	    {
	     	 IGNORE_RET(OSD_DMA_TransferRegion((UINT32)OSD_PLANE_2));    
	     	 u1ExtmjcWriteRegByI2C(0x2050, 0x0);
	     	 LOG(5,"\n*** Redo OSD DMA***\n");
	    }
	    #endif
            // unlock sema
            //VERIFY(x_sema_unlock(_hDmaThread) == OSR_OK);
            //IGNORE_RET(OSD_DMA_TransferRegion((UINT32)OSD_PLANE_2));
#if 0            
	    if((_fgOSDShowDelayon) && (_u1OSDDelayShow != 3))
	    {
	    	_u1OSDDelayShow++;
	    	if(_u1OSDDelayShow==3)
	    	{
	    		_OsdDmaI2CWriteReg(MT8282_REG_FADING_RATIO,0xff,TRUE);
	    		Printf("\nFading Value setting");
	    	}
	    }
#endif	     
            _u4DmaVsyncDelay = _u4DmaUpdatePeriod;
        }

	  if((fgRecoveryFadingValueTrigger)&&(_u4DmaVsyncDelay==u1TriggerFadingValueSyncCount))
	  {
	    	 fgRecoveryFadingValueTrigger = FALSE;
	    	 #ifndef DRV_SUPPORT_MT8283
		 _OsdDmaI2CWriteReg(MT8282_REG_FADING_RATIO,0xff,TRUE);
	    	 #endif
	  }
        
	  #ifdef DRV_SUPPORT_MT8283
	  if(_u1OSDDelayShow>1)
        {
        	//UINT8 u1errmsg;
        	_u1OSDDelayShow--;
        	if(_u1OSDDelayShow==2)
        	{
        		//u1ExtmjcWriteRegByI2C(MT8283_REG_OSD_L_FADING_REG, 0x1f0ff);
        		//u1ExtmjcWriteRegByI2C(MT8283_REG_OSD_R_FADING_REG, 0x1f0ff);
        		//drv_extmjc_set_osd_fading(0xf0);
        		//gb_extmjc_OSD_DelayEnable=TRUE;
        		//drv_extmjc_set_osd_enable(TRUE,&u1errmsg);
        		drv_extmjc_set_osd_fading(0xf0);
        		//Printf("---OSD set fade----\n");
        	}
        }
	  #endif
        
        _u4DmaVsyncDelay--;
    }
    else
    {
    		#ifdef DRV_SUPPORT_MT8283
    		if(_u1OSDDelayShow_1>1)
        	{
        		_u1OSDDelayShow_1--;
        		if(_u1OSDDelayShow_1==2)
        		{
        			Printf("--OSD mmp mute---\n");
        			_OSD_PLA_SetFading(OSD_PLANE_2,0xff);
         			_OSD_PLA_UpdateHwReg(OSD_PLANE_2);
        			_OSD_PLA_SetFading(OSD_PLANE_3,0xff);
         			_OSD_PLA_UpdateHwReg(OSD_PLANE_3);
        		}
        		//Printf("ttt\n");
        	}
    		#endif
    }
}

void OSD_DMA_SetUpdatePeriod(UINT32 u4DmaUpdatePeriod)
{
    _u4DmaUpdatePeriod = u4DmaUpdatePeriod;
    LOG(5, "OSD_DMA_SetUpdatePeriod: _u4DmaUpdatePeriod=%d\n", _u4DmaUpdatePeriod);
}

void OSD_DMA_Dump(void)
{
    UINT32 u4Idx;

    Printf("OSD mode[%s, I2C=%d, PALETTE_ID_ONLY=%d]\n",
           _acDmaModeName[(UINT32)_eMode], _fgEnableI2C, OSD_DMA_SEND_PALETTE_ID_ONLY);

    for (u4Idx = 0; u4Idx < OSD_DMA_TARGET_POOL_NS; u4Idx++)
    {
        Printf("\t8280 header(%d) region(0x%x) palette(0x%x) bitmap(0x%x)",
            u4Idx,
            _arDmaTargetInfo[u4Idx].u4HeaderAddr,
            _arDmaTargetInfo[u4Idx].u4PaletteAddr,
            _arDmaTargetInfo[u4Idx].u4BitmapAddr);
        if (_u4DmaTargetIdx == u4Idx)
        {
            Printf("[A]\n");
        }
        else
        {
            Printf("\n");
        }
    }
    for (u4Idx = 0; u4Idx < MT8280_PALETTE_NUMBER; u4Idx++)
    {
        Printf("\tpalette id=%d, 538x pal_addr=0x%x, 8280 pal_addr=0x%x\n",
               u4Idx, _au4PaletteMapping[u4Idx], _au4TargetPaletteAddress[u4Idx]);
    }
    Printf("\tInstruction Pool Addr(0x%x)\n", (UINT32)_pu4DmaIPool);
}

void OSD_DMA_AfterPaletteUpdate(UINT32 u4PaletteId, UINT32 u4PaletteAddress)
{
#if (OSD_DMA_SEND_PALETTE_ID_ONLY)
    if (u4PaletteId >= MT8280_PALETTE_NUMBER)
    {
        return;
    }
    _au4PaletteMapping[u4PaletteId] = u4PaletteAddress;
#else
    _u4PaletteUpdateCount = OSD_DMA_TARGET_POOL_NS * 2;
#endif
    LOG(5, "OSD_DMA_AfterPaletteUpdate: u4PaletteId=%d, u4PaletteAddress=0x%x\n",
        u4PaletteId, u4PaletteAddress);
}

//---------------------------------------------------
// Set if LVDS clk is divided by 2 
//---------------------------------------------------
void OSD_DMA_SetLVDSClk(BOOL fgClkDiv2)
{
	if(fgClkDiv2)
	{
		IO_WRITE32MSK(OSD_BASE,0x68,0x100,0x100);
	}
	else
	{
		IO_WRITE32MSK(OSD_BASE,0x68,0,0x100);
	}
}


INT32 OSD_DMA_Set8281(UINT32 u4addr, UINT32 u4value)
{
    INT32 i4Ret;
    volatile OSD_DMA_INSTRUCTION_T* _prInstruction;
    volatile OSD_DMA_CTRANSFER_T* prCTansfer;
    UINT8 u1checksum=0xff,i;

    i4Ret = (INT32)OSD_RET_OK;

    
    // lock mutex semaphore
    VERIFY(x_sema_lock(_hTransferRegion, X_SEMA_OPTION_WAIT) == OSR_OK);

    // reset DMA instruction pool
    _u4DmaIPoolIdx = 0;

    _prInstruction = (OSD_DMA_INSTRUCTION_T*)(void*)&_pu4DmaIPool[_u4DmaIPoolIdx];
    _u4DmaIPoolIdx += 4;    // 16 byte

    _prInstruction->rField.fgCen = 1;
    _prInstruction->rField.fgDen = 0;
    _prInstruction->rField.fgEoc = 1;
    _prInstruction->rField.u4Delay = 0;
    _prInstruction->rField.u4DAddr = 0;
    _prInstruction->rField.u4DSize = 0;
    _prInstruction->rField.u4CSize = 1;

    CALLOC_C_TRANSFER(prCTansfer);
    _OsdDmaFillCTransfer(prCTansfer, u4addr, u4value, 0);
    for(i=0;i<16;i++)
    {
    	u1checksum ^= (*((UINT8 *)(prCTansfer)+i));
    }
    Printf("cal checksum=: 0x%x!\n",u1checksum);
    
    _OsdDmaSubmit();

    // Unlock Mutex Semaphore
    VERIFY(x_sema_unlock(_hTransferRegion) == OSR_OK);

    return i4Ret;
}

