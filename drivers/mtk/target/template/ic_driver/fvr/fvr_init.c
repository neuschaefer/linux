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
 * $RCSfile: fvr_init.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_init.c
 *  FVR driver - FVR
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "fvr_debug.h"
#include "pvr_imem.h"

#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_util.h"


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
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static HANDLE_T _hFVRTimer = NULL_HANDLE;

static REG_TEST_T _arFVRRgtList[] = 
{
    // offset, type, len, mask, has reset val, reset vla
    { 0x0000, eRD_ONLY, 4, 0x80000000, 1, 0x0 },
    { 0x0004, eRD_WR, 4, 0x0000ffff, 1, 0x30 },
    { 0x0008, eRD_WR, 4, 0x000000ff, 1, 0x4 },
    { 0x000C, eRD_WR, 4, 0x00000fff, 1, 0xBC },
    { 0x0040, eRD_ONLY, 4, 0xC0000C00, 1, 0x800 },
    //{ 0x0074, eRD_WR, 4, 0x80FF7FFF, 1, 0x0 },
    { 0x0084, eRD_WR, 4, 0x0000FFFF, 1, 0x1 },
    //{ 0x0088, eRD_WR, 4, 0xFFFFFFFF, 1, 0x0 },
    { 0x008C, eRD_WR, 4, 0xFFFFFFFF, 1, 0xFFFFFFFF },
    { 0x0184, eRD_WR, 4, 0x0000FFFF, 1, 0x0 },
    { 0x0404, eRD_WR, 4, 0x00050005, 1, 0x0 },
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVRTestRegisters
 *  @retval void
 */
//-----------------------------------------------------------------------------
static BOOL _FVRTestRegisters()
{
    UINT32 i, u4RegCount, u4RegVal;

    u4RegCount = sizeof(_arFVRRgtList) / sizeof(REG_TEST_T);
    for(i=0; i<u4RegCount; i++)
    {
        u4RegVal = FVR_READ32((_arFVRRgtList[i].u4Addr)/4);
        if((_arFVRRgtList[i].u4Mask & u4RegVal) != _arFVRRgtList[i].u4DfVal)
        {
            Printf("Register test fail\n");
            return FALSE;
        }
    }

    for(i=0; i<u4RegCount; i++)
    {
        if(_arFVRRgtList[i].eRegType== eRD_WR)
        {
            FVR_WRITE32(_arFVRRgtList[i].u4Addr/4, 0xFFFFFFFF);
        }
    }

    for(i=0; i<u4RegCount; i++)
    {
        if(_arFVRRgtList[i].eRegType == eRD_WR)
        {
            u4RegVal = FVR_READ32((_arFVRRgtList[i].u4Addr)/4);
            if((_arFVRRgtList[i].u4Mask & u4RegVal) != (_arFVRRgtList[i].u4Mask & 0xFFFFFFFF))
            {
                Printf("Register test fail\n");
                return FALSE;
            }
        }
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVRTestSram_Word
 *  @retval void
 */
//-----------------------------------------------------------------------------
static BOOL _FVRTestSram_Word(UINT32 u4BaseAddr, UINT32 u4ByteCount)
{
    UINT32 i;
    UINT32 *pu4Buf;

    // Special pattern - 0x5A
    pu4Buf = (UINT32*)u4BaseAddr;
    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            pu4Buf[i] = 0x5A5A5A5A;
        }
        else
        {
            pu4Buf[i] = 0xA5A5A5A5;
        }
    }

    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            if(pu4Buf[i] != 0x5A5A5A5A)
            {
                return FALSE;
            }
        }
        else
        {
            if(pu4Buf[i] != 0xA5A5A5A5)
            {
                return FALSE;
            }
        }
    }

    // Special pattern - 0xA5
    pu4Buf = (UINT32*)u4BaseAddr;
    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            pu4Buf[i] = 0xA5A5A5A5;
        }
        else
        {
            pu4Buf[i] = 0x5A5A5A5A;
        }
    }

    for(i=0; i<(u4ByteCount/4); i++)
    {
        if((i%2) == 0)
        {
            if(pu4Buf[i] != 0xA5A5A5A5)
            {
                return FALSE;
            }
        }
        else
        {
            if(pu4Buf[i] != 0x5A5A5A5A)
            {
                return FALSE;
            }
        }
    }

    // Increase pattern
    pu4Buf = (UINT32*)u4BaseAddr;
    for(i=0; i<(u4ByteCount/4); i++)
    {
        pu4Buf[i] = u4BaseAddr + i * 4;
    }

    for(i=0; i<(u4ByteCount/4); i++)
    {
        if(pu4Buf[i] != (u4BaseAddr + i * 4))
        {
            return FALSE;
        }
    }
        
    return TRUE;
}

#if 0
//-----------------------------------------------------------------------------
/** _FVRTestSram_HalfWord
 *  @retval void
 */
//-----------------------------------------------------------------------------
static BOOL _FVRTestSram_HalfWord(UINT32 u4BaseAddr, UINT32 u4ByteCount)
{
    UINT32 i;
    UINT16 *pu2Buf;

    // Special pattern - 0x5A
    pu2Buf = (UINT16*)u4BaseAddr;
    for(i=0; i<(u4ByteCount/2); i++)
    {
        if((i%2) == 0)
        {
            pu2Buf[i] = 0x5A5A;
        }
        else
        {
            pu2Buf[i] = 0xA5A5;
        }
    }

    for(i=0; i<(u4ByteCount/2); i++)
    {
        if((i%2) == 0)
        {
            if(pu2Buf[i] != 0x5A5A)
            {
                return FALSE;
            }
        }
        else
        {
            if(pu2Buf[i] != 0xA5A5)
            {
                return FALSE;
            }
        }
    }

    // Special pattern - 0xA5
    pu2Buf = (UINT16*)u4BaseAddr;
    for(i=0; i<(u4ByteCount/2); i++)
    {
        if((i%2) == 0)
        {
            pu2Buf[i] = 0xA5A5;
        }
        else
        {
            pu2Buf[i] = 0x5A5A;
        }
    }

    for(i=0; i<(u4ByteCount/2); i++)
    {
        if((i%2) == 0)
        {
            if(pu2Buf[i] != 0xA5A5)
            {
                return FALSE;
            }
        }
        else
        {
            if(pu2Buf[i] != 0x5A5A)
            {
                return FALSE;
            }
        }
    }

    // Increase pattern
    pu2Buf = (UINT16*)u4BaseAddr;
    for(i=0; i<(u4ByteCount/2); i++)
    {
        pu2Buf[i] = (UINT16)(i * 2);
    }

    for(i=0; i<(u4ByteCount/2); i++)
    {
        if(pu2Buf[i] != (UINT16)(i * 2))
        {
            return FALSE;
        }
    }
        
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVRTestSram_Byte
 *  @retval void
 */
//-----------------------------------------------------------------------------
static BOOL _FVRTestSram_Byte(UINT32 u4BaseAddr, UINT32 u4ByteCount)
{
    UINT32 i;
    UINT8 *pu1Buf;

    // Special pattern - 0x5A
    pu1Buf = (UINT8*)u4BaseAddr;
    for(i=0; i<u4ByteCount; i++)
    {
        if((i%2) == 0)
        {
            pu1Buf[i] = 0x5A;
        }
        else
        {
            pu1Buf[i] = 0xA5;
        }
    }

    for(i=0; i<u4ByteCount; i++)
    {
        if((i%2) == 0)
        {
            if(pu1Buf[i] != 0x5A)
            {
                return FALSE;
            }
        }
        else
        {
            if(pu1Buf[i] != 0xA5)
            {
                return FALSE;
            }
        }
    }

    // Special pattern - 0xA5
    pu1Buf = (UINT8*)u4BaseAddr;
    for(i=0; i<u4ByteCount; i++)
    {
        if((i%2) == 0)
        {
            pu1Buf[i] = 0xA5;
        }
        else
        {
            pu1Buf[i] = 0x5A;
        }
    }

    for(i=0; i<u4ByteCount; i++)
    {
        if((i%2) == 0)
        {
            if(pu1Buf[i] != 0xA5)
            {
                return FALSE;
            }
        }
        else
        {
            if(pu1Buf[i] != 0x5A)
            {
                return FALSE;
            }
        }
    }

    // Increase pattern
    pu1Buf = (UINT8*)u4BaseAddr;
    for(i=0; i<u4ByteCount; i++)
    {
        pu1Buf[i] = (UINT8)i;
    }

    for(i=0; i<u4ByteCount; i++)
    {
        if(pu1Buf[i] != (UINT8)i)
        {
            return FALSE;
        }
    }
        
    return TRUE;
}
#endif


//-----------------------------------------------------------------------------
/** _FVRTestSram
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _FVRTestSram(UINT32 u4BaseAddr, UINT32 u4ByteCount)
{
    BOOL fgRet;

    fgRet = TRUE;
#if 0
    if(!_FVRTestSram_Byte(u4BaseAddr, u4ByteCount))
    {
        LOG(3, "FVR SRAM byte test fail\n");
        fgRet = FALSE;
    }

    if(!_FVRTestSram_HalfWord(u4BaseAddr, u4ByteCount))
    {
        LOG(3, "FVR SRAM half-word test fail\n");
        fgRet = FALSE;
    }
#endif
    if(!_FVRTestSram_Word(u4BaseAddr, u4ByteCount))
    {
        fgRet = FALSE;
    }
    
    return fgRet;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVR_Diag
 *  @retval void
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Diag(void)
{
    BOOL fgRet;

    fgRet = TRUE;
#if 0
    // Reset test
    if (!_FVRTestReset())
    {
        fgRet = FALSE;
    }
#endif
    // Register access test
    if (!_FVRTestRegisters())
    {
        LOG(3, "FVR register test fail\n");
        fgRet = FALSE;
    }

    // SRAM memory test
    if (!_FVRTestSram(FVR_IMEM_BASE, FVR_IMEM_LEN))
    {
        LOG(3, "FVR I-Mem SRAM test fail\n");
        fgRet = FALSE;
    }

    if (!_FVRTestSram(FVR_DMEM_BASE, FVR_DMEM_LEN))
    {
        LOG(3, "FVR D-Mem SRAM test fail\n");
        fgRet = FALSE;    
    }
    
    if (!_FVRTestSram(FVR_PID_TABLE_BASE, FVR_PID_TABLE_LEN))
    {
        LOG(3, "FVR PID Table SRAM test fail\n");
        fgRet = FALSE;    
    }
    
    return fgRet;
}


//-----------------------------------------------------------------------------
/** _FVRIsuPRunning
 *  Check FVR status
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _FVRIsuPRunning(void)
{
    UINT32 u4State;

    u4State = (FVR_READ32(FVR_REG_FVR_uP_CTRL) >> 30) & 0x1;
    LOG(7, "FVR %s\n", u4State ? "halt" : "running");

    return 0 == u4State;
}


//-----------------------------------------------------------------------------
/** _FVREnableuP
 *  Enable FVR uP
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
void _FVREnableuP(BOOL fgEnable)
{
    UINT32 i;
    
    if (fgEnable)
    {
	    // Enable uP
    	FVR_WRITE32(FVR_REG_FVR_uP_CTRL, 0);
	   	while (1)
	   	{
	   		if (((FVR_READ32(FVR_REG_FVR_uP_CTRL) >> 30) & 0x1) == 0)
	   		{
	   			break;
	   		}
	   	}    	
    }
    else
    {
    	// Halt uP
	   	FVR_REG32(FVR_REG_FVR_uP_CTRL) |= 0x80000000;
	   	while (1)
	   	{
	   		if (((FVR_READ32(FVR_REG_FVR_uP_CTRL) >> 30) & 0x1) == 1)
	   		{
	   			break;
	   		}
	   	}

    	// Clear all interrupts
    	for (i = 0; i < FVR_INT_QUEUE_DEPTH; i++)
    	{
    		FVR_WRITE32(FVR_REG_REC_DBM_ERROR_STATUS, 1);
    		FVR_WRITE32(FVR_REG_REC_DBM_NON_ERROR_STATUS, 1);
    		FVR_WRITE32(FVR_REG_uP_ERROR_STATUS, 1);
    		FVR_WRITE32(FVR_REG_uP_NON_ERROR_STATUS, 1);
    	}
	}
}


//-----------------------------------------------------------------------------
/** _FVR_LoadIMem
 *  Load new uCode
 *
 *  @param  fgForceWrite    If halt uP first if uP is running
 *  @param  pu4IData        The new uCode array
 *  @param  u4Len           Length of new uCode (in word)
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _FVR_LoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len)
{
    UINT32 u4Data, i;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    ASSERT(pu4IData != NULL);

    // Check uP status
    if (_FVRIsuPRunning())
    {
        if (!fgForceWrite)
        {
            LOG(5, "FVR uP is running, can't load to I-mem!\n");
            return FALSE;
        }
        else
        {
            LOG(5, "FVR uP is running, now will halt it\n");
            _FVREnableuP(FALSE);
            while (_FVRIsuPRunning())
            {
                LOG(5, "wait uP stop...\n");
            }
        }
    }

    // Load to I-mem
    _FVR_Lock();
    
    for (i = 0; i < u4Len; i++)
    {
        IO_WRITE32(FVR_IMEM_BASE, i * 4, pu4IData[i]);       
    }

    // Verify
    for (i = 0; i < u4Len; i++)
    {
        u4Data = IO_READ32(FVR_IMEM_BASE, i * 4);
        if(u4Data != pu4IData[i])
        {
            LOG(1, "Load IMem error, at word %u, write 0x%08x, read 0x%08x\n",
                i, pu4IData[i], u4Data);

            _FVR_Unlock();    
            return FALSE;
        }
    }

    _FVR_Unlock();

    u2CodeVersion = pu4IData[0] & 0xffff;
    u1Major = (u2CodeVersion >> 8) & 0xf;
    u1Minor = u2CodeVersion & 0xff;
    LOG(5, "FVR uCode version: %u.%u\n", u1Major, u1Minor);

    // Reset demux (uP), so uP can run into the entry pointer of uCode
    VERIFY(_FVR_Reset());

    // Suppress warnings in lint and release build
    UNUSED(u1Major);
    UNUSED(u1Minor);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SoftInit
 *  Soft-initialize demux
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _FVR_SoftInit(void)
{
    UINT32 u4Ctrl, i;
    FVR_GBL_STRUCT_T* prGblPidStruct;

    _FVR_Lock();

    // Clear PID index table and PID structures
    x_memset((void*)&(FVR_GBL_PID_W(0)), 0, FVR_GBL_SIZE * 4);
    prGblPidStruct = _FVR_GetGBLStruct();
    x_memset((void*)prGblPidStruct, 0, sizeof(FVR_GBL_STRUCT_T));
    
    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_STRUCT_T* prPidStruct;

        FVR_PID_INDEX_TABLE(i) = 0xF0000000;
        
        x_memset((void*)&(FVR_PER_PID_S(i)), 0, FVR_PID_SIZE * 4);
        
        prPidStruct = _FVR_GetPidStruct(i);
        x_memset((void*)prPidStruct, 0, sizeof (FVR_PID_STRUCT_T));
    }

    _FVR_Unlock();

    // Initialize interrupt handler
    _FVR_InitISR();

    // Initialize common routines
    _FVR_InitAPI();

    _FVR_Lock();
    
    // Set PID struct size
    FVR_WRITE32(FVR_REG_CONFIG_1, ((FVR_GBL_SIZE * 4) << 8) | (FVR_PID_SIZE * 4));

    // Set record packet length
    u4Ctrl = FVR_READ32(FVR_REG_CONFIG_3);
    u4Ctrl &= ~0xFF;
    u4Ctrl |= 0xBC; // 188
    FVR_WRITE32(FVR_REG_CONFIG_3, u4Ctrl);

    // Disable insert timestamp
    u4Ctrl = FVR_READ32(FVR_REG_CONFIG_3);
    u4Ctrl &= ~0x800;
    FVR_WRITE32(FVR_REG_CONFIG_3, u4Ctrl);
    
    _FVR_Unlock();

    // Stop DBM first
    FVR_WRITE32(FVR_REG_FVR_DBM_CTRL, 0x0);
    while((FVR_READ32(FVR_REG_FVR_DBM_CTRL) & 0x800) != 0x800)
    {
        x_thread_delay(1);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Init()
{
    // Load default uCode (for normal transport stream)
    if (!_FVR_LoadIMem(TRUE, PVR_IMEM, PVR_IMEM_LEN))
    {
        return FALSE;
    }
    
    // Soft init
    if (!_FVR_SoftInit())
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Brief of _FVRPktTimeFunc.
 *  Details of _FVRPktTimeFunc (optional).
 *  HISR
 */
//-----------------------------------------------------------------------------
static void _FVRPktTimeFunc(HANDLE_T pt_tm_handle, VOID* pv_tag)
{
    FVR_GBL_STRUCT_T* prGBLStruct;
    UINT32 u4PktCount;

    if(pt_tm_handle != _hFVRTimer)
    {
        return;
    }

    if(!prGBLStruct->fgEnableTimeNotify)
    {
        return;
    }
    
    prGBLStruct = _FVR_GetGBLStruct();
    u4PktCount = FVR_GBL_PID_W(6);

    if(prGBLStruct->pfnTimeNotify != 0)
    {
        prGBLStruct->pfnTimeNotify(u4PktCount);
    }

    UNUSED(pv_tag);
}


//-----------------------------------------------------------------------------
/** _FVR_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Start()
{
    FVR_GBL_STRUCT_T* prGBLStruct;
    UINT32 i;

    // Reset packet count information
    prGBLStruct = _FVR_GetGBLStruct();
    prGBLStruct->u2CurPktCount = 0;
    prGBLStruct->u4LongPktCount = 0;
    FVR_GBL_PID_W(5) &= 0xFFFF0000;
    FVR_GBL_PID_W(6) = 0x0;

	// Clear all interrupts
	for (i = 0; i < FVR_INT_QUEUE_DEPTH; i++)
	{
		FVR_WRITE32(FVR_REG_REC_DBM_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_REC_DBM_NON_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_uP_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_uP_NON_ERROR_STATUS, 1);
	}

    // Enable all FVR interrupts
    FVR_WRITE32(FVR_REG_INTR_MASK, 0x50005);
    
    // Enable Record DBM
    _FVR_Lock();
    FVR_WRITE32(FVR_REG_FVR_DBM_CTRL, 0x400);
    _FVR_Unlock();
    
    while((FVR_READ32(FVR_REG_FVR_DBM_CTRL) & 0x800) != 0)
    {
        x_thread_delay(1);
    }

    if(prGBLStruct->fgEnableTimeNotify)
    {
        ASSERT(prGBLStruct->u4TimeToNotify > 0);
        
        // Init FVR timer
        if(_hFVRTimer == 0)
        {
            VERIFY(x_timer_create(&_hFVRTimer) == OSR_OK);
        }
    
        VERIFY(x_timer_start(_hFVRTimer, prGBLStruct->u4TimeToNotify,                         
                             X_TIMER_FLAG_REPEAT, _FVRPktTimeFunc, NULL) == OSR_OK);
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Stop()
{
    FVR_GBL_STRUCT_T* prGBLStruct;
    UINT32 i;
    
    // Disable Record DBM
    _FVR_Lock();
    FVR_WRITE32(FVR_REG_FVR_DBM_CTRL, 0x0);
    _FVR_Unlock();

    while((FVR_READ32(FVR_REG_FVR_DBM_CTRL) & 0x800) != 0x800)
    {
        x_thread_delay(1);
    }

    // Disable FVR interrupts
    FVR_WRITE32(FVR_REG_INTR_MASK, 0x0);

	// Clear all interrupts
	for (i = 0; i < FVR_INT_QUEUE_DEPTH; i++)
	{
		FVR_WRITE32(FVR_REG_REC_DBM_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_REC_DBM_NON_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_uP_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_uP_NON_ERROR_STATUS, 1);
	}

    prGBLStruct = _FVR_GetGBLStruct();
    if(prGBLStruct->fgEnableTimeNotify)
    {
        if (_hFVRTimer != 0)
        {
            VERIFY(x_timer_stop(_hFVRTimer) == OSR_OK);
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Reset
 *  Reset FVR, reset uP, need soft initialize again
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Reset(void)
{
	UINT32 i;

    // Reset FVR
    FVR_WRITE32(FVR_REG_GLOBAL_CTRL, 0x1);
    x_thread_delay(10);
    FVR_WRITE32(FVR_REG_GLOBAL_CTRL, 0x0);

    _FVR_Lock();

	// Clear all interrupts
	for (i = 0; i < FVR_INT_QUEUE_DEPTH; i++)
	{
		FVR_WRITE32(FVR_REG_REC_DBM_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_REC_DBM_NON_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_uP_ERROR_STATUS, 1);
		FVR_WRITE32(FVR_REG_uP_NON_ERROR_STATUS, 1);
	}

    _FVR_Unlock();

    // Enable uP
	FVR_WRITE32(FVR_REG_FVR_uP_CTRL, 0);    
   	while (1)
   	{
   		if (((FVR_READ32(FVR_REG_FVR_uP_CTRL) >> 30) & 0x1) == 0)
   		{
   			break;
   		}
   	}    	

    return TRUE;
}

