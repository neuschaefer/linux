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
 * Description:
 *         Soft parser interface
 *
 *---------------------------------------------------------------------------*/

#if 0
#include "x_lint.h"

// Do not lint this file
LINT_SAVE_AND_DISABLE

#include "psr_drvif.h"
#include "utils.h"
#include "x_assert.h"
#include "x_rand.h"
#include "x_printf.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_mid.h"
#include "fbm_drvif.h"
//===========================================================================

//#define DRAM_256MB_TEST

#define SRC_ADDR                        0x31000000
#ifdef DRAM_256MB_TEST
#define DST_ADDR                        0x3a000000
#else
#define DST_ADDR                        0x32000000
#endif
#define MAX_SIZE                        0xf7ffff
#define FIFO_SIZE                       0x100000 // Audio Fifo size

#define TEST_SIZE_STEP                  0x10000
#define TEST_SIZE_STEP_AUDIO            0x01000

static UINT32 _SrcAddr = 0;
//===========================================================================

extern UCHAR * _memcmp(UCHAR * pucS1, UCHAR * pucS2, UINT32 u4Size);
extern INT32 CLI_Parser(const CHAR * szCmdBuf);
extern void HalFlushInvalidateDCache(void);

static void _SetMidWriteRegion(UINT32 u4StartAddr, UINT32 u4Size)
{
	MID_ResetRegion(0);
	MID_ResetRegion(1);
	MID_ResetRegion(2);
    MID_SetRegion(0, MID_AGENT_CPU, 0, u4StartAddr);
    MID_SetRegion(1, MID_AGENT_CPU, u4StartAddr + u4Size, 0xFFFFFFF0);
	MID_EnableRegionProtect(0);	
	MID_EnableRegionProtect(1);	

    // List mid region 
	//CLI_Parser("mid.al");
}


static BOOL _SoftParserTest(UINT32 u4Size)
{
    static UINT32 _u4DstAddr = 0xffffffff;

    UINT32 u4SrcAddr, u4DstAddr, u4FifoStart, u4FifoEnd;
    UCHAR* puc;
    HAL_TIME_T t0, t1, dt0, dt1;

    VERIFY(PSR_SoftReset());

    u4SrcAddr = _SrcAddr;    
    u4FifoStart = _SrcAddr + MAX_SIZE;
    u4FifoEnd = (u4FifoStart + MAX_SIZE)&0xffffff00; // PSR_SOFT_FIFO_ALIGNMENT = 256

    if (_u4DstAddr >= (u4FifoEnd - u4Size))
    {
        _u4DstAddr = u4FifoStart;
    }    
    u4DstAddr = _u4DstAddr;
    
    ASSERT(u4DstAddr >= u4FifoStart && u4DstAddr < u4FifoEnd);

    // General transfer content
    GenerateRandomPattern(u4SrcAddr, u4Size);
    x_memset((void *)u4DstAddr, 0, u4Size);
    HalFlushInvalidateDCache();
    
    // Add MID protection/detection
    _SetMidWriteRegion(_u4DstAddr, u4Size);

    Printf("  Source: 0x%08x, destination: 0x%08x, size: 0x%08x\n",
        u4SrcAddr, u4DstAddr, u4Size);

    // Perform poll mode transfer
    HAL_GetTime(&t0);
    VERIFY(PSR_SoftTransfer(PSR_STREAM_UNKNOWN, 0, PHYSICAL(u4SrcAddr), u4Size,
        PHYSICAL(u4DstAddr), PHYSICAL(u4FifoStart), PHYSICAL(u4FifoEnd), TRUE));

    HAL_GetTime(&t1);
    HAL_GetDeltaTime(&dt0, &t0, &t1);

    // Check result
    /*
    puc = _memcmp_ring(u4SrcAddr, u4SrcAddr, u4SrcAddr + u4Size,
        u4DstAddr, u4FifoStart, u4FifoEnd, u4Size);
    */    
    puc = _memcmp((UCHAR *)u4SrcAddr, (UCHAR *)u4DstAddr, u4Size);
    if (puc != NULL)
    {
        Printf("Polling mode Transfer failed at 0x%08x\n", puc);
        return FALSE;
    }

    // Perform interrupt mode transfer
    x_memset((void *)u4DstAddr, 0, u4Size);
    HalFlushInvalidateDCache();
    
    HAL_GetTime(&t0);
    
    VERIFY(PSR_SoftTransfer(PSR_STREAM_UNKNOWN, 0, PHYSICAL(u4SrcAddr), u4Size,
        PHYSICAL(u4DstAddr), PHYSICAL(u4FifoStart), PHYSICAL(u4FifoEnd), FALSE));
    
    HAL_GetTime(&t1);
    HAL_GetDeltaTime(&dt1, &t0, &t1);

    // Check result
    /*
    puc = _memcmp_ring(u4SrcAddr, u4SrcAddr, u4SrcAddr + u4Size,
        u4DstAddr, u4FifoStart, u4FifoEnd, u4Size);
    */    
    puc = _memcmp((UCHAR *)u4SrcAddr, (UCHAR *)u4DstAddr, u4Size);
    if (puc != NULL)
    {
        Printf("Interrupt mode Transfer failed at 0x%08x\n", puc);     
        return FALSE;
    }

    Printf("  Used time - poll mode: %u.%06us, interrupt mode: %u.%06us\n",
        dt0.u4Seconds, dt0.u4Micros, dt1.u4Seconds, dt1.u4Micros);

    HAL_GetTime(&t0);
    HAL_GetTime(&t1);
    x_memcpy((void *)u4SrcAddr, (void *)u4DstAddr, u4Size);
    HalFlushInvalidateDCache();
    HAL_GetDeltaTime(&dt1, &t0, &t1);
    Printf("  CPU memcpy Used time - %u.%06us\n", dt1.u4Seconds, dt1.u4Micros);


    _u4DstAddr += u4Size;
    /*if (_u4DstAddr >= u4FifoEnd)
    {
        //_u4DstAddr -= (MAX_SIZE & 0xffffff00); // PSR_SOFT_FIFO_ALIGNMENT = 256
        _u4DstAddr = u4FifoStart;
    }*/

    return TRUE;
}

static BOOL _SoftParserAudioTransferTest(UINT32 u4Size)
{
    static UINT32 _u4DstAddr = DST_ADDR + FIFO_SIZE - (TEST_SIZE_STEP*5);

    UINT32 u4SrcAddr, u4DstAddr, u4FifoStart, u4FifoEnd;
    UCHAR* puc;
    HAL_TIME_T t0, t1, dt0, dt1;

    VERIFY(PSR_SoftReset());

    u4SrcAddr = SRC_ADDR;
    u4DstAddr = _u4DstAddr - (TEST_SIZE_STEP*5);
    u4FifoStart = DST_ADDR - FIFO_SIZE/2;
    u4FifoEnd = DST_ADDR + FIFO_SIZE/2;
    ASSERT(u4DstAddr >= u4FifoStart && u4DstAddr < u4FifoEnd);

    // General transfer content
    GenerateRandomPattern(u4SrcAddr, u4Size);

    // Add MID protection/detection
    _SetMidWriteRegion(PHYSICAL(u4FifoStart), FIFO_SIZE + 0x10);

    Printf("  Source: 0x%08x, destination: 0x%08x, size: 0x%08x\n",
        u4SrcAddr, u4DstAddr, u4Size);
    Printf("  Fifo Start: 0x%08x, fifo end: 0x%08x, size: 0x%08x\n",
        u4FifoStart, u4FifoEnd, FIFO_SIZE);

    // Perform poll mode transfer
    HAL_GetTime(&t0);
    VERIFY(PSR_SoftTransfer(PSR_STREAM_AUDIO, 0, PHYSICAL(u4SrcAddr), u4Size,
        PHYSICAL(u4DstAddr), PHYSICAL(u4FifoStart), PHYSICAL(u4FifoEnd), TRUE));
    HAL_GetTime(&t1);
    HAL_GetDeltaTime(&dt1, &t0, &t1);

    // Check result
    
    puc = _memcmp_ring(u4SrcAddr, u4SrcAddr, u4SrcAddr + u4Size,
        u4DstAddr, u4FifoStart, u4FifoEnd, u4Size);
    
    if (puc != NULL)
    {
        Printf("Transfer failed at 0x%08x\n", puc);
        return FALSE;
    }

    // Perform interrupt mode transfer
    HAL_GetTime(&t0);
    VERIFY(PSR_SoftTransfer(PSR_STREAM_AUDIO, 0, PHYSICAL(u4SrcAddr), u4Size,
        PHYSICAL(u4DstAddr), PHYSICAL(u4FifoStart), PHYSICAL(u4FifoEnd), FALSE));
    HAL_GetTime(&t1);
    HAL_GetDeltaTime(&dt0, &t0, &t1);

    Printf("  Used time - poll mode: %u.%06us, interrupt mode: %u.%06us\n",
        dt0.u4Seconds, dt0.u4Micros, dt1.u4Seconds, dt1.u4Micros);

    // Check result
    puc = _memcmp_ring(u4SrcAddr, u4SrcAddr, u4SrcAddr + u4Size,
        u4DstAddr, u4FifoStart, u4FifoEnd, u4Size);
    
    if (puc != NULL)
    {
        Printf("Transfer failed at 0x%08x\n", puc);
        return FALSE;
    }

/*
    _u4DstAddr += u4Size;
    if (_u4DstAddr >= u4FifoEnd)
    {
        _u4DstAddr -= FIFO_SIZE;
    }
*/
    return TRUE;
}

// (256 + 16) MB
#define DST_ADDR_FAR  (/*DST_ADDR + */0x11000000) 
// 128 MB
#define TMP_ADDR      (/*DST_ADDR +*/ 0x38000000) 

static BOOL _SoftParserTest512MB(UINT32 u4Size)
{
    static UINT32 _u4DstAddr = DST_ADDR_FAR;
    static UINT32 _u4TmpAddr = TMP_ADDR;

    UINT32 u4SrcAddr, u4DstAddr, u4FifoStart, u4FifoEnd;
    UCHAR* puc;
    HAL_TIME_T t0, t1, dt1;

    VERIFY(PSR_SoftReset());

    u4SrcAddr = SRC_ADDR;
    u4DstAddr = _u4DstAddr;
    u4FifoStart = _u4DstAddr;
    u4FifoEnd = _u4DstAddr + u4Size;
    ASSERT(u4DstAddr >= u4FifoStart && u4DstAddr < u4FifoEnd);

    // To suppress warning messages for release version
    UNUSED(u4FifoStart);
    UNUSED(u4FifoEnd);

    // General transfer content
    GenerateRandomPattern(u4SrcAddr, u4Size);

    // Add MID protection/detection
    //_SetMidWriteRegion(_u4DstAddr, u4Size);

    Printf("  Source: 0x%08x, destination: 0x%08x, size: 0x%08x\n",
        u4SrcAddr, u4DstAddr, u4Size);

    // Perform poll mode transfer
    HAL_GetTime(&t0);
    
    VERIFY(PSR_DmaTransfer(u4SrcAddr, u4Size, u4DstAddr));
    VERIFY(PSR_DmaTransfer(u4DstAddr, u4Size, _u4TmpAddr));
    
    HAL_GetTime(&t1);
    HAL_GetDeltaTime(&dt1, &t0, &t1);

    puc = _memcmp((UCHAR *)u4SrcAddr, (UCHAR *)_u4TmpAddr, u4Size);
    if (puc != NULL)
    {
        Printf("Transfer failed at 0x%08x\n", puc);
        return FALSE;
    }

    return TRUE;
}

//===========================================================================

BOOL SoftParserTest(void)
{
    UINT32 u4Ok, u4Loops, u4Size;
    FBM_POOL_T *prFbmPool;

    VERIFY(PSR_SoftInit());
    //CLI_Parser("mid.i");

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_TOTAL);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    ASSERT(prFbmPool->u4Size > ((UINT32)MAX_SIZE<<1));

    _SrcAddr = prFbmPool->u4Addr;
    	
    u4Ok = u4Loops = 0;
    for (u4Size = TEST_SIZE_STEP; u4Size < MAX_SIZE; u4Size += TEST_SIZE_STEP)
    {
        Printf("\n[%u/%u] Soft parser general DMA testing...\n", u4Ok, u4Loops);

        u4Loops++;
        if (_SoftParserTest(u4Size))
        {
            Printf("Successful\n");
            u4Ok++;
        }
        else
        {
            Printf("Failed\n");
        }
    }

    return (u4Loops == u4Ok) ? TRUE : FALSE;
}

BOOL SoftParserAudioTest(void)
{
    UINT32 u4Ok, u4Loops, u4Size;

    VERIFY(PSR_SoftInit());
    //CLI_Parser("mid.i");

    u4Ok = u4Loops = 0;
    for (u4Size = TEST_SIZE_STEP_AUDIO; u4Size < FIFO_SIZE; u4Size += TEST_SIZE_STEP_AUDIO*0x8)
    {
        Printf("\n[%u/%u] Soft parser audio fifo transfer testing...\n", u4Ok, u4Loops);

        u4Loops++;
        if (_SoftParserAudioTransferTest(u4Size))
        {
            Printf("Successful\n");
            u4Ok++;
        }
        else
        {
            Printf("Failed\n");
        }
    }
    
    return (u4Loops == u4Ok) ? TRUE : FALSE;
}

BOOL SoftParserTest512MB(void)
{
    UINT32 u4Ok, u4Loops, u4Size;

    VERIFY(PSR_SoftInit());
    //CLI_Parser("mid.i");

    u4Ok = u4Loops = 0;
    for (u4Size = TEST_SIZE_STEP; u4Size < MAX_SIZE; u4Size += TEST_SIZE_STEP)
    {
        Printf("\n[%u/%u] Soft parser general DMA testing...\n", u4Ok, u4Loops);

        u4Loops++;
       
        if (_SoftParserTest512MB(u4Size))
        {
            Printf("Successful\n");
            u4Ok++;
        }
        else
        {
            Printf("Failed\n");
        }
    }

    return (u4Loops == u4Ok) ? TRUE : FALSE;
}



// Restore Lint settings
LINT_RESTORE
#endif

