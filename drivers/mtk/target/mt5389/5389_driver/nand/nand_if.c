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
 * $RCSfile: nand_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_if.c
 *  nand_if.c contains the interface function of nand device.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "nand_debug.h"
#include "nandhw_reg.h"
#include "nandhw_if.h"
#include "nand_if.h"
#include "nand_part.h"
#include "storage_if.h"

#include "x_lint.h"
#include "x_typedef.h"
#include "x_bim.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_util.h"
#include "x_printf.h"
#include "x_timer.h"
#include "x_pinmux.h"
#include "x_ckgen.h"

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define NUM_ELM(table)  (sizeof(table) / sizeof(table[0]))

#if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
extern void NANDPinShareLock(void);
extern void NANDPinShareUnlock(void);
#define NAND_ENTRY                      \
{                                       \
    NANDPinShareLock();                 \
    NANDHW_EnablePinmux(TRUE);          \
}
#define NAND_EXIT                       \
{                                       \
    NANDHW_EnablePinmux(FALSE);         \
    NANDPinShareUnlock();               \
}
#else
static HANDLE_T _hNANDEntrySema = NULL_HANDLE;
#define NAND_ENTRY                      \
{                                       \
    NAND_ApiLock(&_hNANDEntrySema);     \
    NANDHW_EnablePinmux(TRUE);          \
}
#define NAND_EXIT                       \
{                                       \
    NANDHW_EnablePinmux(FALSE);         \
    NAND_ApiUnLock(&_hNANDEntrySema);   \
}
#endif  // __KERNEL__

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL   _fgNANDInit = FALSE;
static UINT32 _u4NANDPgSz = 0;
static UINT32 _u4NANDBlkSz = 0;
static UINT32 _u4NANDTotalBlk = 0;
static UINT32 _u4PgBitCount = 0;

static UINT32 _u4Data[NAND_MAX_PAGE_SIZE>>2];
static UINT32 _u4Spare[NAND_MAX_OOB_SIZE>>2];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#if !defined(CC_MTK_LOADER)
//-----------------------------------------------------------------------------
/** _NAND_LockInit()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
static VOID _NAND_LockInit(HANDLE_T *hhSema)
{
    if (*hhSema == NULL_HANDLE)
    {
        VERIFY(OSR_OK == x_sema_create(hhSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK));
    }
}
#endif

//-----------------------------------------------------------------------------
/** NAND_ApiLock()
 *  Lock sempahore for thread safe
 */
//-----------------------------------------------------------------------------
VOID NAND_ApiLock(HANDLE_T *hhSema)
{
#if !defined(CC_MTK_LOADER)
    _NAND_LockInit(hhSema);
    VERIFY(OSR_OK == x_sema_lock(*hhSema, X_SEMA_OPTION_WAIT));
#endif
}

//-------------------------------------------------------------------------
/** NAND_ApiUnLock
 *  Unlock semaphore for thread safe.
 */
//-------------------------------------------------------------------------
VOID NAND_ApiUnLock(HANDLE_T *hhSema)
{
#if !defined(CC_MTK_LOADER)
    VERIFY(OSR_OK == x_sema_unlock(*hhSema));
#endif
}

//-----------------------------------------------------------------------------
/** _NAND_Check()
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
static INT32 _NAND_Check(void)
{
    NANDDEV_T rNandDev;

    NAND_Init();

    NANDHW_GetInfo(&rNandDev);

    if (rNandDev.u4PgSz == 0)
    {
        Printf("No flash found!!\n");
        return 1;
    }

    return 0;
}

#if defined(CC_NAND_WRITE_VERIFY)
INT32 NAND_VerifyAttr(UINT64 u8Offset, UINT32 u4MemLen)
{
   INT32 i4Ret = 0;
   UINT32 i, u4PgStart, u4PgCnt;
   
   if (_NAND_Check() != 0)
   {
	   return 1;
   }
   
   NAND_ENTRY;

   u4PgStart = (UINT32)(u8Offset >> _u4PgBitCount);
   u4PgCnt = (u4MemLen >> _u4PgBitCount);

   for (i = 0; i < u4PgCnt; i++)
   {
      i4Ret = NANDHW_VerifyPageAttr(u4PgStart+i);
	  if (i4Ret != 0)
	  {
	     break;
	  }
   }

   NAND_EXIT;
   
   return i4Ret;
}
#endif

//-----------------------------------------------------------------------------
/** _NAND_RawReadAttr() provide the nand raw device access read function.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
static INT32 _NAND_RawReadAttr(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4SparePtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    UINT8 *pu1Buf;
    UINT32 i, u4PgStart, u4PgCnt;
    NAND_CONTROL_T rCtrl;
    
    NANDHW_GetControl(NAND_CTRL_FLAGS_ALL, &rCtrl);

    u4PgStart = (UINT32)(u8Offset >> _u4PgBitCount);
    u4PgCnt = (u4MemLen >> _u4PgBitCount);
    pu1Buf = (UINT8 *)u4MemPtr;

#if NAND_MULTIPAGE_ACCESS
    if ((_u4NANDPgSz > 512) && (rCtrl.fgDMA == TRUE) && (rCtrl.rEccType == NAND_ECC_HARD))
    {
        UINT32 u4Loop = u4PgCnt / NAND_NFI_MDMA_PAGENUM;
        UINT32 u4LeftPg = u4PgCnt % NAND_NFI_MDMA_PAGENUM;

        for (i = 0; i < u4Loop; i++)
        {
            i4Ret = NANDHW_ReadMultiPageAttr((UINT32 *)(void *)(pu1Buf + (i*NAND_NFI_MDMA_PAGENUM*_u4NANDPgSz)), 
                        (UINT32 *)u4SparePtr, u4PgStart + i*NAND_NFI_MDMA_PAGENUM, NAND_NFI_MDMA_PAGENUM);
            if (i4Ret)
            {
                goto HandleError;
            }
        }

        if (u4LeftPg)
        {
            i4Ret = NANDHW_ReadMultiPageAttr((UINT32 *)(void *)(pu1Buf + (u4Loop*NAND_NFI_MDMA_PAGENUM*_u4NANDPgSz)), 
                        (UINT32 *)u4SparePtr, u4PgStart + u4Loop*NAND_NFI_MDMA_PAGENUM, u4LeftPg);
            if (i4Ret)
            {
                goto HandleError;
            }
        }
    }
    else
#endif
    {
        for (i = 0; i < u4PgCnt; i++)
        {
            i4Ret = NANDHW_ReadPageAttr((UINT32 *)(void *)(pu1Buf + (i*_u4NANDPgSz)), 
                        (UINT32 *)u4SparePtr, u4PgStart + i);
            if (i4Ret)
            {
                goto HandleError;
            }
        } 
    }

HandleError:
    if (i4Ret)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "_NAND_RawReadAttr fail (MemPtr:0x%08x, Page:0x%08x)\n", 
            (UINT32)(pu1Buf + (i*_u4NANDPgSz)), u4PgStart+i);
#else
        LOG(0, "_NAND_RawReadAttr fail (MemPtr:0x%08x, Page:0x%08x)\n", 
            (UINT32)(pu1Buf + (i*_u4NANDPgSz)), u4PgStart+i);
#endif
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NAND_RawWriteAttr() provide the nand raw device access write function.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
static INT32 _NAND_RawWriteAttr(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4SparePtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    UINT8 *pu1Buf;
    UINT32 i, u4PgStart, u4PgCnt;
    NAND_CONTROL_T rCtrl;
    
    NANDHW_GetControl(NAND_CTRL_FLAGS_ALL, &rCtrl);

    u4PgStart = (UINT32)(u8Offset >> _u4PgBitCount);
    u4PgCnt = (u4MemLen >> _u4PgBitCount);
    pu1Buf = (UINT8 *)u4MemPtr;

#if 0//NAND_MULTIPAGE_ACCESS
    if ((_u4NANDPgSz > 512) && (rCtrl.fgDMA == TRUE) && (rCtrl.rEccType == NAND_ECC_HARD) )
    {
        UINT32 u4Loop = u4PgCnt / NAND_NFI_MDMA_PAGENUM;
        UINT32 u4LeftPg = u4PgCnt % NAND_NFI_MDMA_PAGENUM;

        for (i = 0; i < u4Loop; i++)
        {
            i4Ret = NANDHW_WriteMultiPageAttr((UINT32 *)(void *)(pu1Buf + (i*NAND_NFI_MDMA_PAGENUM*_u4NANDPgSz)), 
                        (UINT32 *)u4SparePtr, u4PgStart + i*NAND_NFI_MDMA_PAGENUM, NAND_NFI_MDMA_PAGENUM);
            if (i4Ret)
            {
                goto HandleError;
            }
        }

        if (u4LeftPg)
        {
            i4Ret = NANDHW_WriteMultiPageAttr((UINT32 *)(void *)(pu1Buf + (u4Loop*NAND_NFI_MDMA_PAGENUM*_u4NANDPgSz)), 
                        (UINT32 *)u4SparePtr, u4PgStart + u4Loop*NAND_NFI_MDMA_PAGENUM, u4LeftPg);
            if (i4Ret)
            {
                goto HandleError;
            }
        }
    }
    else
#endif
    {
        for (i = 0; i < u4PgCnt; i++)
        {
            i4Ret = NANDHW_WritePageAttr((UINT32 *)(void *)(pu1Buf + (i*_u4NANDPgSz)), 
                        (UINT32 *)u4SparePtr, u4PgStart + i);
            if (i4Ret)
            {
                    goto HandleError;
            }
        }
    }

HandleError:
    if (i4Ret)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "_NAND_RawWriteAttr fail (MemPtr:0x%08x, Page:0x%08x)\n", (UINT32)(pu1Buf + (i*_u4NANDPgSz)), u4PgStart+i);
#else
        LOG(0, "_NAND_RawWriteAttr fail (MemPtr:0x%08x, Page:0x%08x)\n", (UINT32)(pu1Buf + (i*_u4NANDPgSz)), u4PgStart+i);
#endif
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _NAND_Pinmux() to switch NAND pinmux.
 */
//-----------------------------------------------------------------------------
static void _NAND_Pinmux(void)
{
    UINT32 i, u4Ver;
    
    u4Ver = (UINT32)BSP_GetIcVersion();
    if ((u4Ver & 0xFFFF0000) == 0x53650000)
    {
        static UINT32 _NAND_pinmux_tbl[] = 
        {
            PIN_POCE0_, PIN_POOE_,  PIN_PDD0,   PIN_PDD1,
            PIN_POCE1_, PIN_PARB_,  PIN_POWE_,  PIN_PAALE,
            PIN_PACLE,  PIN_PDD2,   PIN_PDD3,   PIN_PDD4,
            PIN_PDD5,   PIN_PDD6,   PIN_PDD7
        };

        for (i = 0; i < NUM_ELM(_NAND_pinmux_tbl); i++)
        {
            BSP_PinSet(_NAND_pinmux_tbl[i], PINMUX_FUNCTION0);
        }
        
        LOG(1, "Switch NAND pinmux\n");
    }
}

//-----------------------------------------------------------------------------
/** NAND_Init() to initialize the nand device and nand driver.
 */
//-----------------------------------------------------------------------------
void NAND_Init()
{
    UINT32 i, u4Tmp;
    NANDDEV_T rNandDev;
    NAND_CONTROL_T rCtrl;

    if (_fgNANDInit)
    {
        return;
    }

    _NAND_Pinmux();

    NAND_ENTRY

    if (NANDHW_Init() != 0)
    {
        ASSERT(0);
    }

    NANDHW_GetInfo(&rNandDev);
    _u4NANDPgSz = rNandDev.u4PgSz;
    _u4NANDBlkSz = rNandDev.u4BlkSz;
    _u4NANDTotalBlk = rNandDev.u4TotalBlk;

    if (_u4NANDPgSz == 0)
    {
        LOG(1, "Unknow NAND flash\n");
    }
    else
    {
        u4Tmp = _u4NANDPgSz;
        for (i = 1; i < 32; i++)
        {
            u4Tmp = u4Tmp >> 1;
            if ((u4Tmp & 0x1) == 0x1)
            {
                _u4PgBitCount = i;
                break;
            }
        }
    }

#if NAND_ENABLE_ISR
    rCtrl.fgIsr = TRUE;
#else
    rCtrl.fgIsr = FALSE;
#endif
    rCtrl.fgDMA = TRUE;
    rCtrl.rEccType = NAND_ECC_HARD;
    NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rCtrl);

    _fgNANDInit = TRUE;
    NAND_EXIT
}

//-----------------------------------------------------------------------------
/** NAND_Stop() to disable the interrupt of nand device.
 */
//-----------------------------------------------------------------------------
void NAND_Stop()
{
    if (!_fgNANDInit)
    {
        return;
    }
    _fgNANDInit = FALSE;
}

//-----------------------------------------------------------------------------
/** NAND_Reset() to reset the nand device.
 */
//-----------------------------------------------------------------------------
void NAND_Reset()
{
    if (!_fgNANDInit)
    {
        return;
    }
    
    NAND_ENTRY
    NANDHW_Reset();
    NAND_EXIT
    
    NAND_Stop();
    NAND_Init();
}

//-----------------------------------------------------------------------------
/** NAND_SetControl()
 */
//-----------------------------------------------------------------------------
void NAND_SetControl(UINT32 u4Flag, NAND_CONTROL_T* prCtrl)
{
    NAND_Init();
    NANDHW_SetControl(u4Flag, prCtrl);
}

//-----------------------------------------------------------------------------
/** NAND_GetControl()
 */
//-----------------------------------------------------------------------------
void NAND_GetControl(UINT32 u4Flag, NAND_CONTROL_T* prCtrl)
{
    NAND_Init();
    NANDHW_GetControl(u4Flag, prCtrl);
}

//-----------------------------------------------------------------------------
/** NAND_ReadID() get nand chip information.
 */
//-----------------------------------------------------------------------------
INT32 NAND_ReadID(UINT32 *pu4ID)
{
    if(pu4ID == NULL)
    {
        return -1;
    }

    NAND_Init();

    NAND_ENTRY;
    NANDHW_ReadID(pu4ID);
    NAND_EXIT;

    return 0;
}

//-----------------------------------------------------------------------------
/** NAND_GetInfo() get nand chip information.
 */
//-----------------------------------------------------------------------------
INT32 NAND_GetInfo(NANDDEV_T *prNand)
{
    NAND_Init();

    if (prNand == NULL)
    {
        return 1;
    }

    NANDHW_GetInfo(prNand);
    return 0;
}

/* Below will do nand raw device table maintainance task. */

//-----------------------------------------------------------------------------
/** NAND_ReadAttr()
 */
//-----------------------------------------------------------------------------
INT32 NAND_ReadAttr(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4SparePtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    if (_NAND_Check() != 0)
    {
        return 1;
    }

    NAND_ENTRY;
    i4Ret = _NAND_RawReadAttr(u8Offset, u4MemPtr, u4SparePtr, u4MemLen);
    NAND_EXIT;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NAND_Write()
 */
//-----------------------------------------------------------------------------
INT32 NAND_WriteAttr(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4SparePtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    if (_NAND_Check() != 0)
    {
        return 1;
    }

    NAND_ENTRY;
    i4Ret = _NAND_RawWriteAttr(u8Offset, u4MemPtr, u4SparePtr, u4MemLen);
    NAND_EXIT;

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NAND_Read()
 */
//-----------------------------------------------------------------------------
INT32 NAND_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    return NAND_ReadAttr(u8Offset, u4MemPtr, (UINT32)_u4Spare, u4MemLen);
}

//-----------------------------------------------------------------------------
/** NAND_Write()
 */
//-----------------------------------------------------------------------------
INT32 NAND_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    x_memset(_u4Spare, 0xFF, sizeof(_u4Spare));
    return NAND_WriteAttr(u8Offset, u4MemPtr, (UINT32)_u4Spare, u4MemLen);
}

//-----------------------------------------------------------------------------
/** NAND_Erase()
 */
//-----------------------------------------------------------------------------
INT32 NAND_Erase(UINT32 u4PageIdx, UINT32 u4PageNum)
{
    INT32 i4Ret = 0;
    UINT32 i, u4PgInBlk, u4BlkStart, u4BlkEnd;

    if (u4PageNum == 0)
    {
        return 0;
    }
    
    if (_NAND_Check() != 0)
    {
        return 1;
    }
    
    NAND_ENTRY;
    u4PgInBlk = _u4NANDBlkSz / _u4NANDPgSz;
    u4BlkStart = u4PageIdx / u4PgInBlk;
    u4BlkEnd = (u4PageIdx + u4PageNum - 1) / u4PgInBlk;

    for (i = u4BlkStart; i <= u4BlkEnd; i++)
    {
        if (NANDHW_IsBadBlk(i) == 0)
        {
            if (NANDHW_EraseBlock(i))
            {
                NANDHW_MarkBadBlk(i);
                i4Ret = -1;
            }
        }
    }
    
    NAND_EXIT;
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NAND_Erase()
 */
//-----------------------------------------------------------------------------
INT32 NAND_EraseForce(UINT32 u4PageIdx, UINT32 u4PageNum)
{
    INT32 i4Ret = 0;
    NAND_CONTROL_T rCtrl;
    UINT32 i, u4PgInBlk, u4BlkStart, u4BlkEnd;

    if (u4PageNum == 0)
    {
        return 0;
    }
    
    if (_NAND_Check() != 0)
    {
        return 1;
    }
    
    NANDHW_GetControl(NAND_CTRL_FLAGS_ALL, &rCtrl);
    
    NAND_ENTRY;
    u4PgInBlk = _u4NANDBlkSz / _u4NANDPgSz;
    u4BlkStart = u4PageIdx / u4PgInBlk;
    u4BlkEnd = (u4PageIdx + u4PageNum - 1) / u4PgInBlk;

#if 0//NAND_MULTIPAGE_ACCESS
    if ((_u4NANDPgSz > 512) && (rCtrl.fgDMA == TRUE) && (rCtrl.rEccType == NAND_ECC_HARD))
    {
        UINT32 u4BlkCnt = u4BlkEnd - u4BlkStart + 1;
        UINT32 u4Loop = u4BlkCnt / NAND_NFI_MDMA_PAGENUM;
        UINT32 u4LeftBlk = u4BlkCnt % NAND_NFI_MDMA_PAGENUM;
        
        for (i = 0; i < u4Loop; i++)
        {
            if (NANDHW_EraseMultiBlock(u4BlkStart + i*NAND_NFI_MDMA_PAGENUM, NAND_NFI_MDMA_PAGENUM))
            {
                i4Ret = -1;
            }
        }

        if (u4LeftBlk)
        {
            if (NANDHW_EraseMultiBlock(u4BlkStart + u4Loop*NAND_NFI_MDMA_PAGENUM, u4LeftBlk))
            {
                i4Ret = -1;
            }
        }
    }
    else
#endif
    {
        for (i = u4BlkStart; i <= u4BlkEnd; i++)
        {
            if (NANDHW_EraseBlock(i))
            {
                NANDHW_MarkBadBlk(i);
                i4Ret = -1;
            }
        }
    }

    NAND_EXIT;
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NAND_RdAttr()
 */
//-----------------------------------------------------------------------------
INT32 NAND_RdAttr(UINT64 u8Offset, UINT32 u4DataMemPtr, UINT32 u4ByteCount)
{
    INT32 i4Ret;
    
    i4Ret = NAND_ReadAttr(u8Offset, (UINT32)_u4Data, (UINT32)_u4Spare, _u4NANDPgSz);
    x_memcpy((void *)u4DataMemPtr, _u4Spare, u4ByteCount);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NAND_WrAttr()
 */
//-----------------------------------------------------------------------------
INT32 NAND_WrAttr(UINT64 u8Offset, UINT32 u4DataMemPtr, UINT32 u4ByteCount)
{
    INT32 i4Ret;
    
    x_memset(_u4Data, 0xFF, sizeof(_u4Data));
    x_memset(_u4Spare, 0xFF, sizeof(_u4Spare));
    x_memcpy(_u4Spare, (void *)u4DataMemPtr, u4ByteCount);
    
    i4Ret = NAND_WriteAttr(u8Offset, (UINT32)_u4Data, (UINT32)_u4Spare, _u4NANDPgSz);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** NAND_GetSize()
*  @return  sector count(512B).
*/
//-------------------------------------------------------------------------
INT64 NAND_GetSize()
{
    UINT32 u4Num;
    NAND_Init();

    u4Num = _u4NANDTotalBlk/1000;
    return (((INT64)(u4Num*1000)*_u4NANDBlkSz) >> 9);
}

//-----------------------------------------------------------------------------
/** NAND_BlankCheck()
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NAND_BlankCheck(UINT64 u8Offset, UINT32 u4PageCount)
{
    if (_NAND_Check() != 0)
    {
        return 1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** NAND_BlankCheckAttr()
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NAND_BlankCheckAttr(UINT64 u8Offset, UINT32 u4PageCount)
{
    if (_NAND_Check() != 0)
    {
        return 1;
    }
    return 0;
}

//-----------------------------------------------------------------------------
/** NAND_IsBadBlk()
 */
//-----------------------------------------------------------------------------
BOOL NAND_IsBadBlk(UINT32 u4BlkNum)
{
    BOOL fgRet;

    NAND_ENTRY;
    fgRet = NANDHW_IsBadBlk(u4BlkNum);
    NAND_EXIT;

    return fgRet;
}

//-----------------------------------------------------------------------------
/** NAND_MarkBadBlk()
 */
//-----------------------------------------------------------------------------
BOOL NAND_MarkBadBlk(UINT32 u4BlkNum)
{
    BOOL fgRet;

    if (u4BlkNum > _u4NANDTotalBlk)
    {
        return FALSE;
    }

    NAND_ENTRY;
    fgRet = NANDHW_MarkBadBlk(u4BlkNum);
    NAND_EXIT;

    return fgRet;
}

//-----------------------------------------------------------------------------
/** NAND_ShowDevList()
 */
//-----------------------------------------------------------------------------
void NAND_ShowDevList(void)
{
    NANDDev_ShowDevList();
}

