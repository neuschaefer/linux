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
 *      Memory test commands
 *
 *---------------------------------------------------------------------------*/

#include "x_printf.h"
#ifdef __MW_CLI_DEF__

    #include "x_mid_cli.h"
#else

    #include "x_drv_cli.h"
#endif

#define DEFINE_IS_LOG       MEMTEST_IsLog
#include "x_debug.h"

#include "x_stl_lib.h"
#include "x_assert.h"

#include "x_timer.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "c_model.h"
#include "x_dram.h"
#include "x_ckgen.h"
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#if defined(CC_MT5395) || defined(CC_MT5365)    
    #define NONCACHE(addr)                                (((addr) & 0x0fffffff) | 0x40000000)
#elif defined(CC_MT5363)    
    #define NONCACHE(addr)                                (((addr) & 0x0fffffff) | 0x60000000)
#else    
    #define NONCACHE(addr)                                (((addr) & 0x0fffffff) | 0x30000000)
#endif

//#define DRAM_HWONLINECALDQS

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MEMTEST_DEFAULT_ADDR        0x2000000
#define MEMTEST_DEFAULT_SIZE        0x10000
#define MEMTEST_DEFAULT_LOOP        100
#define RECORD_DQS_REG                  (IO_VIRT + 0x8108)

#define HWCALI_BUFFERSIZE     8192

#define MEMTEST_BUFFERSIZE     7800

// 0x703C, 0x7040 shift real address offset.
#ifdef CC_MT5363
#define HWCALI_ALIGNBITS     5
#else
#define HWCALI_ALIGNBITS     6
#endif

// reserve address align and 3 bytes check data.
#define HWCALI_ACTUALBUFFERSIZE (HWCALI_BUFFERSIZE + (1<<HWCALI_ALIGNBITS))
#define HWCALI_START 0
#define HWCALI_PROCESS 1
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#define BINARY2GRAY(u4Bin)   ((u4Bin) ^ ((u4Bin) >> 1))

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void MEM_Test( UINT32 u4Loop, UINT32 u4Addr, UINT32 u4Size );
extern void VDP_Memory_Test(UINT32 u4Color);
extern UINT32 AUD_DspGetPc(void);
extern int memLineBounceTest(unsigned int u4Start, unsigned int u4Size,
    unsigned int verbose);
extern int memButterflyTest(unsigned int u4Start, unsigned int u4Size,
    unsigned int verbose);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
#ifdef DRAM_HWONLINECALDQS
static void _DDR_DQSCalDisable(void);
#endif
static void _DDR_ThreadInit(void);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef DRAM_HWONLINECALDQS
//base[29:6] (unit:byte)
static UINT32 au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE]; 
static UINT32 u4HwCalDQSEnable = 0;
static UINT32 u4HWCaliState = HWCALI_START;
#endif
static UINT32 u4Cmemrw = 0;
static BOOL fgCmemrwAbort = TRUE;
static BOOL fgDRAMThreadInit = FALSE;
static UINT32 *pu4M8K = NULL;
static HAL_TIME_T rOrgTime, rNewTime, rDiffTime;
static UINT32 u4InitSec;
static HANDLE_T phDRAMThreadHdl;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifdef DRAM_HWONLINECALDQS
static UINT32 _DDR_Gray2Binary(UINT32 num)
{
        UINT32 temp = num ^ (num>>8);
        temp ^= (temp>>4);
        temp ^= (temp>>2);
        temp ^= (temp>>1);
        return temp;
}
#endif

#ifndef NDEBUG
static UINT32 _DDR_4BitTo4Byte( UINT32 i ) 
{ 
    return (((i & 8) ? 0xFF000000 : 0) | ((i & 4) ? 0x00FF0000 : 0) | 
                  ((i & 2) ? 0x0000FF00 : 0) | ((i & 1) ? 0x000000FF : 0));
}

static INT32 _DDR_NextIter( INT32 iter )
{
    INT32 new_iter;

    INT32 i = (iter >> 8) & 255;
    INT32 j = iter & 255;

    if (i<=j)
    {
        new_iter = (j << 8) | i;

        if (j == (i + 1))
        {
            new_iter = ((i + 1) << 8) | 15;

            if (iter == 0x0E0F)
            {
                new_iter = 0x0F0E;
            }
        }

        if (i == j)
        {
            new_iter = 0x000F; //iter must be 0 now.
        }
    }
    else
    {
        new_iter = (j << 8) | (i - 1);

        if (i == (j + 1))
        {
            new_iter = ((i - 1) << 8) | (j - 1);

            if (j == 0)
            {
                new_iter = 0;
            }
        }
    }

    return new_iter;
}

static void _DDR_CpuDspMemTest(void)
{
    UINT32 u4PcDsp0[10];
    UINT32 i;
    UINT32 u4Err = FALSE;

    if (!pu4M8K)
    {
        return;
    }
    
    for (i=0; i<10; i=i+2)
    {
        u4PcDsp0[i] = AUD_DspGetPc();

        if (memLineBounceTest((unsigned int)pu4M8K, MEMTEST_BUFFERSIZE, 0) == 0)
        {
            // Test error, show red at whole panel.
            VDP_Memory_Test(0);
            while(1);
        }

        u4PcDsp0[i+1] = AUD_DspGetPc();
        
        if (memButterflyTest((unsigned int)pu4M8K, MEMTEST_BUFFERSIZE, 0) == 0)
        {
            // Test error, show red at whole panel.
            VDP_Memory_Test(0);
            while(1);
        }

        x_thread_delay(100);
    }

    // check DSP0 pc counter are all the same.
    for (i=1; i<10; i++)
    {
        if (u4PcDsp0[0] != u4PcDsp0[i])
        {
            u4Err = TRUE;
            break;
        }
    }

    if (u4Err == FALSE)
    {
        // Test error, show red at whole panel.
        VDP_Memory_Test(0);
        while(1);
    }
}

static UINT32 _DDR_IsDQSFail(void)
{
    UINT32 u4Addr = NONCACHE((UINT32)pu4M8K);
    UINT32 u4Val;
    UINT32 ib = 17;
    INT32 iter, c, iter_mod_3 = 0;
    UINT32 u4temp, i;

    for (c = 0; c<32; c++)
    {
        // 1 c is 4Byte*241, 8 c is nearly (<) 8K Bytes
        if ((c & 7) == 0)
        {
            //start of write or read
            u4Addr = NONCACHE((UINT32)pu4M8K);

            ib = 17;
            iter_mod_3 = 0;
        }

        iter = 0;

        do
        {
            if (c & 16)
            {
                ib = 32;
            }

            u4Val = _DDR_4BitTo4Byte(iter & 255) ^ (1 << ib);

            if (c & 8)
            {
                u4temp = IO_READ32(0, u4Addr);

                if (u4temp ^ u4Val)
                {
                    Printf("Iter:%d, Read Addr=0x%X.\n", iter, u4Addr);
                    Printf("Expect Val=0x%08X, Read Val=0x%08X.\n", u4Val, u4temp);

                    Printf("Try to read 200 times...\n");
                    for (i = 0; i<200; i++)
                    {
                        u4temp = IO_READ32(0, u4Addr);
                        if (u4temp ^ u4Val)
                        {
                            Printf("(%03d) Read Val = 0x%08X, XOR Value=0x%X.\n", 
                                i, u4temp, (u4temp ^ u4Val));
                        }
                    }

                    return TRUE;
                }
            }

            IO_WRITE32(0, u4Addr, u4Val);

            if (iter_mod_3 == 0)
            {
                ib = (ib + 13) & 31;

                iter_mod_3 = 3;
            }

            iter_mod_3--;
            u4Addr += 4;
            iter = _DDR_NextIter(iter);
        } while (iter != 0);
    }

    return FALSE;
}

static INT32 _DDR_Cmdmemoryrw( INT32 i4Argc, const CHAR ** aszArgv )
{
    if (_DDR_IsDQSFail() == FALSE)
    {
        Printf("memrw ok\n");
    }
    else
    {
        Printf("memrw fail\n");
    }

    return 0;
}

static INT32 _DDR_CmdMemoryTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 u4Loop, u4Addr, u4Size;

    u4Loop = MEMTEST_DEFAULT_LOOP;
    u4Addr = MEMTEST_DEFAULT_ADDR;
    u4Size = MEMTEST_DEFAULT_SIZE;

    if (i4Argc<2 || i4Argc>4)
    {
        Printf("run loop [addr = 0x%x] [size = 0x%x]\n", 
            MEMTEST_DEFAULT_ADDR, MEMTEST_DEFAULT_SIZE);

        return 0;
    }

    u4Loop = (UINT32)StrToInt(aszArgv[1]);

    if (i4Argc>2)
    {
        u4Addr = (UINT32)StrToInt(aszArgv[2]);
    }

    if (i4Argc>3)
    {
        u4Size = (UINT32)StrToInt(aszArgv[3]);
    }

    MEM_Test(u4Loop, u4Addr, u4Size);

    return 0;
}

static INT32 _DDR_BandWidthMontorTest( INT32 i4Argc, const CHAR ** aszArgv )
{   
    UINT32 u4AgentId = 0x1F;  /* 0x1F = all system */
    UINT32 u4Val = 0;

    if (i4Argc != 2)
    {
        u4AgentId = 0x1F; /* 0x1F = all system */
    }
    else
    {
        u4AgentId = (UINT32)StrToInt(aszArgv[1]);
    }

    u4AgentId = (u4AgentId > 0x1F) ? 0x1F: u4AgentId;

    // Set monitor cycle.
    DRAM_WRITE32(0x18C, 0x0FFFFFFF);

    if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
    {
        DRAM_WRITE32(0x180, (u4AgentId << 8));
        DRAM_WRITE32(0x180, 0x8000 | (u4AgentId << 8));
    }
    else if ((u4AgentId > 15) && (u4AgentId <= 22))  // Group 2.
    {
        DRAM_WRITE32(0x180, ((u4AgentId-16) << 16));
        DRAM_WRITE32(0x180, 0x80000 | ((u4AgentId-16) << 16));
    }
    else if ((u4AgentId > 22) && (u4AgentId <= 29))  // Group 3.
    {
        DRAM_WRITE32(0x180, ((u4AgentId-20) << 16));
        DRAM_WRITE32(0x180, 0x800000 | ((u4AgentId-20) << 16));
    }
    else
    {
        // all system bandwidth.
        DRAM_WRITE32(0x180, (u4AgentId << 8));
        DRAM_WRITE32(0x180, 0x8000 | (u4AgentId << 8));
    }

    Printf("Start check Agent id = 0x%X.....Waiting !\n", u4AgentId);
    
    while (DRAM_READ32(0x19C) != DRAM_READ32(0x18C))
    {
        x_thread_delay(1000);       
    }
      
    if ((u4AgentId <= 15) ||(u4AgentId == 31))  // Group 1.
    {
        u4Val = DRAM_READ32(0x190);
        u4Val /= (DRAM_READ32(0x18C)/1000);
    }
    else if ((u4AgentId > 15) && (u4AgentId <= 22))  // Group 2.
    {
        u4Val = DRAM_READ32(0x194);
        u4Val /= (DRAM_READ32(0x18C)/1000);
    }
    else if ((u4AgentId > 22) && (u4AgentId <= 29))  // Group 3.
    {
        u4Val = DRAM_READ32(0x198);
        u4Val /= (DRAM_READ32(0x18C)/1000);
    }

    Printf("Result: Agent id = 0x%X, bandwidth=%d.%d%%.\n", 
        u4AgentId, (u4Val/10), (u4Val%10));

    return 0;    
}

static INT32 _DDR_Cmdcm( INT32 i4Argc, const CHAR ** aszArgv )
{
    if (i4Argc < 2)
    {
        Printf("Usage: cm [time in secs] [abort 1:compare fail abort, 0: compare fail not abort].\n");
        Printf("[time=n] n = 0 disable test.\n");
        Printf("[abort=1 or 0] default abort=1.\n");
    }
    else if (i4Argc == 2)
    {
        u4Cmemrw = (UINT32)StrToInt(aszArgv[1]);
        fgCmemrwAbort = TRUE;
    }    
    else if (i4Argc == 3)
    {
        u4Cmemrw = (UINT32)StrToInt(aszArgv[1]);
        fgCmemrwAbort = (BOOL)(StrToInt(aszArgv[2]) > 0);        
    }

    // Check and create DDR thread to do memory test.
    if (u4Cmemrw > 0)
    {
        _DDR_ThreadInit();
    }

    Printf("Current Time = %d secs.\n", u4Cmemrw);
    Printf("Current compare fail abort => %s.\n", ((fgCmemrwAbort) ? "TRUE": "FALSE"));
    return 0;
}

static void _DDR_SetDQDelay(UINT32 u4DQSth, UINT32 u4Bin, UINT32 u4DQInputDelay[])
{
    UINT32 u4Val; 
    UINT32 u4OldBin;
    UINT32 i;
    
    u4OldBin = u4DQInputDelay[(2*u4DQSth)];
    for (i=0; i<4; i++)
    {
        u4Val = (u4OldBin>>(8*i)) & 0xFF;
        u4Val += u4Bin;
        u4OldBin &= ~(0xFF << (8*i));
        u4OldBin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B0 + (8*u4DQSth), u4OldBin);

    u4OldBin = u4DQInputDelay[(2*u4DQSth)+1];
    for (i=0; i<4; i++)
    {
        u4Val = (u4OldBin>>(8*i)) & 0xFF;
        u4Val += u4Bin;
        u4OldBin &= ~(0xFF << (8*i));
        u4OldBin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B4 + (8*u4DQSth), u4OldBin);
}

static void _DDR_SetDQSDelay(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Val, u4Gray;

    u4Gray = BINARY2GRAY(u4Bin);

    // Setup 7-bit DQS offset
    u4Val = DRAM_READ32(0x3A0 + (4*(u4DQSth >> 2)));
    u4Val = (u4Val & (~(0xffU << (8*(u4DQSth & 3)))));
    u4Val |= (u4Gray << (8*(u4DQSth & 3)));

    // Update manual DQS input delay only while MANUDQSUPD=1 and DLLFRZ.   
    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00040000)); 
    
    DRAM_WRITE32(0x3A0+ (4*(u4DQSth >> 2)), u4Val);

    // On-line manual DQS input delay adjust enabling.
    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00020000)); 

    x_thread_delay(10);

    DRAM_WRITE32(0x234, (DRAM_READ32(0x234) & (~0x00060000)));     
}

static INT32 _DDR_DQSGatingWindowTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 u4OldVal[4];
    UINT32 u4Val;    
    UINT32 u4SetVal = 0;    
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 3;
    UINT32 u4SelectByte = 0;
    UINT32 i;
    
    if (i4Argc < 4)
    {
        Printf("g [Inc=1:Dec=0] [Gap in secs] [Selected Byte].\n");
        return 0;
    }
    else
    {
        u4IncDelay = (UINT32)StrToInt(aszArgv[1]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[2]);        
        u4SelectByte = (UINT32)StrToInt(aszArgv[3]);
    }

    u4Val = DRAM_READ32(0x388);   
    Printf("Current DQSCTL=0x%X, 0x7388=0x%X.\n", (DRAM_READ32(0x380) & 0xFFF), u4Val);   

    for (i=0; i<4; i++)
    {       
        u4OldVal[i] = ((u4Val >> 8*i) & 0x7F);
        Printf("DQS%d: Gating=%d (Binary Coded).\n", i, u4OldVal[i]);
    }

    for(u4Step=0; u4Step<0x80; u4Step=u4Step+3)
    {
        if (u4IncDelay)
        {
            u4Val = (u4OldVal[u4SelectByte] + u4Step) & 0x7F;
            u4SetVal = DRAM_READ32(0x388);
            u4SetVal &= ~ (0x7F << (8*u4SelectByte));
            u4SetVal |= (u4Val << (8*u4SelectByte));
            Printf("\nByte%d: Step=+%d, Value=%d, 0x7388=0x%X.\n", 
                u4SelectByte, u4Step, u4Val, u4SetVal);                
            DRAM_WRITE32(0x388, u4SetVal); // Need to do early	
        }
        else
        {
            if (u4OldVal[u4SelectByte] > u4Step)
            {
                u4Val = (u4OldVal[u4SelectByte] - u4Step) & 0x7F;
                u4SetVal = DRAM_READ32(0x388);
                u4SetVal &= ~ (0x7F << (8*u4SelectByte));
                u4SetVal |= (u4Val << (8*u4SelectByte));
                Printf("\nByte%d: Step=-%d, Value=%d, 0x7388=0x%X.\n", 
                    u4SelectByte, u4Step, u4Val, u4SetVal);                                    
                DRAM_WRITE32(0x388, u4SetVal); // Need to do early	
            }
            else
            {
                return 0;
            }
        }

        Printf("Wait %d secs...\n", u4TimeGap);
        for (i=0; i<u4TimeGap*10; i++)
        {
            Printf("=");
            x_thread_delay(100);
        }
        Printf("End.\n");
    }

    return 0;    
}

static INT32 _DDR_DQSDelayTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 u4Val;    
    UINT32 i;    
    UINT32 u4Gray;
    UINT32 u4Rec;
    UINT32 u4Bytes = 2;
    UINT32 u4SelectByte = 0;
    INT32 i4Avg[8];
    INT32 i4Min;
    INT32 i4Max;
    INT32 i4Size;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4DQInputDelay[8];
    
    Printf("DRAM Byte Number = %d, Loader calibration infor:\n", u4Bytes);

    for (i=0; i<u4Bytes; i++)
    {
        u4Rec = HAL_READ32(RECORD_DQS_REG + i * 4);	        
        i4Avg[i] = ((u4Rec >> 15) & 0x01) ? -((u4Rec >> 8) | 0xffffff00) : ((u4Rec >> 8) & 0xFF);
        i4Min = ((u4Rec >> 31) & 0x01) ? -((u4Rec >> 24) | 0xffffff00) : (u4Rec >> 24);
        i4Max = (u4Rec >> 16) & 0xFF;
        i4Size = i4Max- i4Min + 1;
        Printf("Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", i, i4Min, i4Max, i4Size, i4Avg[i]);
    }

    if (i4Argc < 3)
    {
        Printf("dqs [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        

#ifdef DRAM_HWONLINECALDQS
        // Wait h/w calibration stop.
        while ((u4HwCalDQSEnable) && (u4HWCaliState != HWCALI_START))
        {
            x_thread_delay(1000);
        }
        // Suspend h/w calibration.
        u4HwCalDQSEnable = 0;
        _DDR_DQSCalDisable();
#endif        
    }

    // Keep DQ input delay.
    for (i=0; i<2*u4Bytes; i++)
    {
        u4DQInputDelay[i] = DRAM_READ32(0x3B0 + 4*i);
    }

    for(u4Step=1; u4Step < 32; u4Step++)
    {
        if (u4IncDelay)
        {
            Printf("\nStep: +%d\n", u4Step);
            u4Val = i4Avg[u4SelectByte]+u4Step;
            u4Gray = BINARY2GRAY(u4Val);

            Printf("Byte %d, Middle = %d (DQS: HEX=0x%X, GrayCode=0x%X).\n", 
                u4SelectByte, u4Val, u4Val, u4Gray);

            _DDR_SetDQSDelay(u4SelectByte, u4Val);
        }
        else
        {
            Printf("\nStep: -%d\n", u4Step);

            if (i4Avg[u4SelectByte] >= u4Step)
            {
                u4Val = i4Avg[u4SelectByte]-u4Step;
                u4Gray = BINARY2GRAY(u4Val);

                Printf("Byte %d, Middle = %d (DQS: HEX=0x%X, GrayCode=0x%X).\n", 
                    u4SelectByte, u4Val, u4Val, u4Gray);

                _DDR_SetDQSDelay(u4SelectByte, u4Val);
            }
            else
            {
                u4Val = u4Step - i4Avg[u4SelectByte];
                u4Gray = BINARY2GRAY(u4Val);

                Printf("Byte %d, Middle = -%d (DQS=0, DQ: HEX=0x%X, GrayCode=0x%X).\n", 
                    u4SelectByte, u4Val, u4Val, u4Gray);

                // Set DQS = 0, DQ start to increase.
                _DDR_SetDQSDelay(u4SelectByte, 0);
                _DDR_SetDQDelay(u4SelectByte, u4Val, u4DQInputDelay);
            }            
        }        

        Printf("Wait %d secs...\n", u4TimeGap);
        for (i=0; i<u4TimeGap*10; i++)
        {
            Printf("=");
            x_thread_delay(100);
        }
        Printf("End.\n");
    }

#ifdef DRAM_HWONLINECALDQS
    // Resume h/w calibration.
    u4HwCalDQSEnable = 1;
    u4HWCaliState = HWCALI_START;
#endif

    return 0;    
}
    
static INT32 _DDR_DQSOutputDelayTest( INT32 i4Argc, const CHAR ** aszArgv )
{
    UINT32 i;
    UINT32 tmp;
    UINT32 u4SelectByte = 0;
    UINT32 u4DQSori;
    UINT32 u4DQori[4];
    UINT32 u4DQSnew;
    UINT32 u4delay;
    UINT32 u4Step;
    UINT32 u4IncDelay = 0;
    UINT32 u4TimeGap = 20;
    UINT32 u4Bytes = 2;
    
    if (i4Argc < 3)
    {
        Printf("dqso [Byte] [Inc=1:Dec=0] [Gap in secs].\n");
        return 0;
    }
    else
    {
        u4SelectByte = (UINT32)StrToInt(aszArgv[1]);        
        u4IncDelay = (UINT32)StrToInt(aszArgv[2]);
        u4TimeGap = (UINT32)StrToInt(aszArgv[3]);        
    }

    u4DQSori = DRAM_READ32(0x354);
    for(i=0; i<u4Bytes; i++)
    {
        u4DQori[i] = DRAM_READ32(0x360 + i*4);
    }

    for(u4Step=1; u4Step < 32; u4Step++)
    {
        if (u4IncDelay)
        {
            Printf("\nStep: +%d\n", u4Step);
        }
        else
        {
            Printf("\nStep: -%d\n", u4Step);
        }        

        if(u4IncDelay)
        {
            tmp = 1;
            for(i=0; i<32; i+=4)    //scan bit
            {                
                if( (0x000f & (DRAM_READ32(0x360 + (u4SelectByte*4))>>i)) == 0 )
                {
                    tmp = 0;
                }
            }

            if(tmp==0)      //+DQS
            {
                u4delay = (DRAM_READ32(0x354)>>(u4SelectByte*4)) & 0x0000000f;
                if(u4delay==0xf)
                {
                    break;
                }
                else
                {
                    u4delay++;
                }

                u4DQSnew = (DRAM_READ32(0x354) & ~(0x0000000f << u4SelectByte*4)) | (u4delay << u4SelectByte*4) ;
                Printf("DQSdelay(0x7354)=0x%08X.\n", u4DQSnew);
                DRAM_WRITE32(0x354, u4DQSnew);                
            }
            else            // -DQ
            {
                u4DQSnew = 0;
                for(i=0; i<32; i+=4)
                {
                    u4delay = 0x000f & (DRAM_READ32(0x360 + u4SelectByte*4)>>i) ;
                    u4delay--;
                    u4DQSnew = u4DQSnew | (u4delay<<i);                    
                }

                Printf("DQdelay(0x%04X)=0x%08X.\n", 0x7360+u4SelectByte*4, u4DQSnew);
                DRAM_WRITE32(0x360+u4SelectByte*4, u4DQSnew);              
            }

        }
        else
        {
            if( 0x000f & (DRAM_READ32(0x354)>>(u4SelectByte*4))) 
            {
                tmp = 1;
            }
            else
            {
                tmp = 0;
            }

            if(tmp==1)      //-DQS
            {
                u4delay = (DRAM_READ32(0x354)>>(u4SelectByte*4)) & 0x0000000f;
                u4delay--;

                u4DQSnew = (DRAM_READ32(0x354) & ~(0x0000000f << u4SelectByte*4)) | (u4delay << u4SelectByte*4) ;
                Printf("DQSdelay(0x7354)=0x%08X.\n", u4DQSnew);
                DRAM_WRITE32(0x354, u4DQSnew);                
            }
            else            // +DQ
            {
                u4DQSnew = 0;
                for(i=0; i<32; i+=4)
                {
                    u4delay = 0x000f & (DRAM_READ32(0x360 + u4SelectByte*4)>>i) ;
                    if(u4delay != 0x0f)
                    {
                        u4delay++;
                    }
                    
                    u4DQSnew = u4DQSnew | (u4delay<<i);
                }

                Printf("DQdelay(0x%04X)=0x%08X.\n", (0x7360+u4SelectByte*4), u4DQSnew);
                DRAM_WRITE32(0x360+u4SelectByte*4, u4DQSnew);
            }

        }

        Printf("Wait %d secs...\n", u4TimeGap);
        x_thread_delay(u4TimeGap*1000);
    }

    Printf("restore...\n");
    DRAM_WRITE32(0x354, u4DQSori);
    for (i=0; i<u4Bytes; i++)
    {
        DRAM_WRITE32(0x360 + i*4, u4DQori[i]);
    }

    return 0;    
}
#endif /* #ifndef NDEBUG */

#ifdef DRAM_HWONLINECALDQS
static void _DDR_DQSCalDisable(void) 
{ 
    DRAM_WRITE32(0x220, (DRAM_READ32(0x220) & ~0xFF));
}

static void _DDR_DQSCalStart(void)
{
    UINT32 u4addr;

    u4addr = ((UINT32)(au4HwCaliBuffer) + 0x10);
   
    //audio pattern; loop=4, post write enabling for test agent 2.
    DRAM_WRITE32(0x218, 0x001F110D);
    DRAM_WRITE32(0x220, 0x04);
    //test agent2 address
    DRAM_WRITE32(0x210, u4addr);
    //test agent2 offset
    DRAM_WRITE32(0x214, HWCALI_BUFFERSIZE);
    //enable DQS cal.
    DRAM_WRITE32(0x220, 0x24);
}

static BOOL _DDR_IsDQSCalDone(void)
{
    // CALI_DONE_MON: calibration result is updated, SW can disable calibration.
    // From 0 - > 1 : Calibration done.
    // From 1 - > 0 : Calibration is turned off at the internal of DRAMC.
    return ((DRAM_READ32(0x280) & 0x80000000) == 0) ? FALSE : TRUE;
}

static void _DDR_UpdateDQS(void)
{
    UINT32 dq, dqs_min, dqs_max, dqs_avg, manualdqs, newmanualdqs;
    UINT32 i;
    UINT32 u4OldVal; 
    UINT32 u4NewVal;
    UINT32 u4Bytes = 2;

    
    dq = DRAM_READ32(0x290);
    dqs_min = DRAM_READ32(0x294);
    dqs_max = DRAM_READ32(0x298);
    dqs_avg = DRAM_READ32(0x29c);  // binary coded.

    manualdqs = DRAM_READ32(0x3A0); // binary coded.
    newmanualdqs = manualdqs;

    for (i = 0; i<u4Bytes; i++)
    {
        u4NewVal = (dqs_avg >> (8 * i)) & 0xFF;
        u4OldVal = _DDR_Gray2Binary((manualdqs >> (8 * i)) & 0xFF);

        if (u4NewVal > (u4OldVal + 1))
        {
            newmanualdqs &= ~(0xFF << (8 * i));
            newmanualdqs |= BINARY2GRAY(u4OldVal + 1) << (8 * i);
            LOG(5, "DRAM: Byte %d, h/w NewVal=%d > OldVal=%d, Set=%d.\n", 
                i, u4NewVal, u4OldVal, (u4OldVal + 1));            
        }
        else if (u4NewVal < (u4OldVal - 1))
        {
            newmanualdqs &= ~(0xFF << (8 * i));        
            newmanualdqs |= BINARY2GRAY(u4OldVal - 1) << (8 * i);
            LOG(5, "DRAM: Byte %d, h/w NewVal=%d < OldVal=%d, Set=%d.\n", 
                i, u4NewVal, u4OldVal, (u4OldVal - 1));            
        }
    }

    // Need to update center value of DQS.
    if (newmanualdqs != manualdqs)
    {   
        for (i=0; i<u4Bytes; i++)
        {
            dq = (INT32)DRAM_READ32(0x290);
            dq = (dq >> (8*i)) & 0xFF;
            
            dqs_min = (INT32)DRAM_READ32(0x294);
            dqs_min = (dqs_min >> (8*i)) & 0xFF;
            
            dqs_max = (INT32)DRAM_READ32(0x298);
            dqs_max = (dqs_max >> (8*i)) & 0xFF;            
            
            dqs_avg = (INT32)DRAM_READ32(0x29C);
            dqs_avg = (dqs_avg >> (8*i)) & 0xFF;            
            
            if (dq != 0)
            {
                dqs_min = dqs_min - dq;
            }

            LOG(1, "DRAM: HW Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", 
                i, dqs_min, dqs_max, (dqs_max- dqs_min + 1), dqs_avg);
        }
         
        do 
        {
            // Note: This delay can not be removed.
            // After disable h/w calibration, it will actually being disabled at next refresh cycle.
            // At this critical section, we can not turn on MANUDQSUPD=1.
            x_thread_delay(10);

        // Check if Calibration is turned off at the internal of DRAMC.
        } while(_DDR_IsDQSCalDone());
            
        // Update manual DQS input delay only while MANUDQSUPD=1 and DLLFRZ.
        DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00040000)); 
        
        DRAM_WRITE32(0x3A0, newmanualdqs);

        // On-line manual DQS input delay adjust enabling.
        DRAM_WRITE32(0x234, (DRAM_READ32(0x234) | 0x00020000)); 

        x_thread_delay(10);

        DRAM_WRITE32(0x234, (DRAM_READ32(0x234) & (~0x00060000)));                 
    }
}

static INT32 _DDR_OnlineDQSCal( INT32 i4Argc, const CHAR ** aszArgv )
{
    if (i4Argc != 2)
    {
        Printf("  [0:Disable /1:Enable].\n");
    }
    else
    {
        u4HwCalDQSEnable = (UINT32)StrToInt(aszArgv[1]);
    }

    if (u4HwCalDQSEnable == 0)
    {
        Printf("Current Setting: Disable\n");
    }
    else
    {
        Printf("Current Setting: Enable\n");
    }

    return 0;
}
#endif // #ifdef DRAM_HWONLINECALDQS

static INT32 _DDR_SSCTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 range = 0;
    UINT32 modulation;
    UINT32 u4Val;	

    if(i4Argc >= 2)
    {
        range = StrToInt(aszArgv[1]);
        if (range == 0)
        {
            Printf("DRAM: Disable SSC.\n");
            /* Disable Spectrum */
            CKGEN_WRITE32(0x158, CKGEN_READ32(0x158) | 0x40 );
            return 0;        
        }
    }    	

    if(i4Argc < 3)
    {
        Printf("ssc n m.\n");
        Printf("  ,n: +-n/10000 percent spreading range\n");
        Printf("  ,m: modulation freq Khz, m must >=1\n");        
        return 0;
    }    	

    modulation = StrToInt(aszArgv[2]);

    Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);
    
    if (modulation == 0)
    {		
        Printf("Modulation freq Khz, m must >=1\n");
        return 0;
    }

    /* Disable Spectrum */
    CKGEN_WRITE32(0x158, CKGEN_READ32(0x158) | 0x40 );

    /* Setup Step. */	       
    //an example for SYSPLL 324MHz only.
    // 4*(2^16)*(324/27)*(RANGE/10000)*modulation/(27000)
    // => ((2^18)*(12)*(modulation*RANGE))/(27000*10000)	       
    u4Val = (modulation*range * 116/10000) & 0x0000ffff;//for 27mhz dmss output (sysplll:324) 
    CKGEN_WRITE32(0x15C, (CKGEN_READ32(0x15C) & ~0xFFFF) | u4Val);

    /* Setup Period. */
    u4Val = (27000 /4) / modulation;//for 27.0mhz dmss output (sysplll:324) = 30k
    CKGEN_WRITE32(0x158, (CKGEN_READ32(0x158) & 0xFFFF) | (u4Val<<16));

    //enable ssc
    CKGEN_WRITE32(0x158, CKGEN_READ32(0x158) & ~0x40 );

    /* Set Dmpll reference clock from Xtal to DMSS */
    CKGEN_WRITE32(0x110, (CKGEN_READ32(0x110) |0x00020000));

    return 0;
}

static INT32 _DDR_InforQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Val;    
    UINT32 u4Mux;
    UINT32 u4WODT;    
    UINT32 u4RODT;    
    UINT32 i;    
    UINT32 u4Rec;
    UINT32 u4Bytes = 2;
    INT32 i4Avg;
#ifdef DRAM_HWONLINECALDQS    
    INT32 i4DQMax;
#endif
    INT32 i4Min;
    INT32 i4Max;
    INT32 i4Size;
    UINT32 u4TotalSecs = 0;
    UINT32 u4Secs = 0;   
    UINT32 u4Mins = 0;
    UINT32 u4Hrs = 0;    
    UINT32 range;
    UINT32 modulation;

    Printf("DRAM: Clock=%d MHz, CL=%d, Column=%d, Size=%d MBytes.\n",  
        (TCMGET_DDR_CLK()/1000000), 
        TCMGET_DDR_CL(), 
        TCMGET_COLADDR(), 
        (BSP_GetMemSize()/0x100000));

    // Reduce/Full strength, Write ODT, Read ODT setting.
        u4Val = DRAM_READ32(0x10);
    u4WODT = ((u4Val & 0x00440000) >> 16) & 0xFF;
        u4RODT = (DRAM_READ32(0x310) & 0xF);    
    Printf("DRAM: %s strength, WODT=%s, RODT=%s.\n", 
        ((u4Val & 0x00020000) ? "Reduce": "Full"), 
        ((u4WODT == 0x0004) ? "75 ohm" : 
        ((u4WODT == 0x0040) ? "150 ohm" : 
        ((u4WODT == 0x0044) ? "50 ohm" : "Disable"))),
        ((u4RODT ==  1) ? "75 ohm" : 
        (u4RODT ==  2) ? "150 ohm" : 
        (u4RODT ==  3) ? "50 ohm" : 
        (u4RODT ==  4) ? "37.5 ohm" : 
        (u4RODT ==  5) ? "25 ohm" : 
        (u4RODT ==  6) ? "30 ohm" : 
        (u4RODT ==  7) ? "21.5 ohm" : "Disable"));

    for (i=0; i<u4Bytes; i++)
    {
        u4Rec = HAL_READ32(RECORD_DQS_REG + i * 4);	        
        i4Avg = ((u4Rec >> 15) & 0x01) ? -(0xFF -(u4Rec >> 8)+1) : ((u4Rec >> 8) & 0xFF);
        i4Min = ((u4Rec >> 31) & 0x01) ? -(0xFF -(u4Rec >> 24)+1) : (u4Rec >> 24);
        i4Max = (u4Rec >> 16) & 0xFF;
        i4Size = i4Max- i4Min + 1;
        Printf("DRAM: SW Byte %d : DQS(%d ~ %d), Size %d, Middle = %d.\n", 
            i, i4Min, i4Max, i4Size, i4Avg);
    }

#ifdef DRAM_HWONLINECALDQS
    Printf("DRAM: HW calibration %s.\n", (u4HwCalDQSEnable) ? "Enable":"Disable");

    if (u4HwCalDQSEnable)
    {   
        for (i=0; i<u4Bytes; i++)
        {
            i4DQMax = (INT32)DRAM_READ32(0x290);
            i4DQMax = (i4DQMax >> (8*i)) & 0xFF;
            
            i4Min = (INT32)DRAM_READ32(0x294);
            i4Min = (i4Min >> (8*i)) & 0xFF;
            
            i4Max = (INT32)DRAM_READ32(0x298);
            i4Max = (i4Max >> (8*i)) & 0xFF;            
            
            i4Avg = (INT32)DRAM_READ32(0x29C);
            i4Avg = (i4Avg >> (8*i)) & 0xFF;            
            
            if (i4DQMax != 0)
            {
                i4Min = i4Min - i4DQMax;
            }

            i4Size = i4Max- i4Min + 1;

            u4Val = DRAM_READ32(0x3A0);
            u4Val = _DDR_Gray2Binary((u4Val >> (8 * i)) & 0x7F);            
            
            Printf("DRAM: HW Byte %d : DQS(%d ~ %d), Size %d, Middle = %d, 0x7018=%d.\n", 
                i, i4Min, i4Max, i4Size, i4Avg, u4Val);
    }

        u4Val = (UINT32)au4HwCaliBuffer;
        Printf("DRAM: HW calibration au4HwCaliBuffer addr = 0x%08X.\n", u4Val);

        u4Val = u4Val >> HWCALI_ALIGNBITS;

        Printf("DRAM: HW calibration Test addr=0x%08X ~ 0x%08X.\n", 
            ((u4Val + 2) << HWCALI_ALIGNBITS), 
            (((u4Val + 2) << HWCALI_ALIGNBITS) + (HWCALI_BUFFERSIZE*4)));        
    }
#endif

    Printf("DRAM: Memrw u4Cmemrw = %d secs.\n", u4Cmemrw);
    Printf("DRAM: Memrw compare fail abort => %s.\n", ((fgCmemrwAbort) ? "TRUE": "FALSE"));
    u4Val = MEMTEST_BUFFERSIZE;
    Printf("DRAM: Memrw au4M8K: 0x%08X ~ 0x%08X, length=%d Bytes.\n", 
        NONCACHE((UINT32)pu4M8K), NONCACHE(((UINT32)pu4M8K) + u4Val -1), u4Val);

    if (u4Cmemrw > 0)
    {    
        HAL_GetTime(& rNewTime);
        HAL_GetDeltaTime(& rDiffTime, & rOrgTime, & rNewTime);
        rOrgTime = rNewTime;
       
        u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
        u4Hrs = u4TotalSecs/(3600);
        u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
        u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
        Printf("DRAM: Memrw Total Time=%d hrs:%d mins:%d secs.\n", 
            u4Hrs, u4Mins, u4Secs);
    }    

    // SSC parameter.    
    u4Val = CKGEN_READ32(0x158);    
    u4Mux = CKGEN_READ32(0x110);
    if (((u4Val & 0x40) == 0) && (u4Mux & 0x20000))
    {
        Printf("DRAM: SSC Enable.\n");

        u4Val = CKGEN_READ32(0x158);
        u4Val = (u4Val >> 16) & 0xFFF;
        modulation = (u4Val > 0) ? ((27000 /4)/u4Val) : 0;

        u4Val = CKGEN_READ32(0x15C);
        u4Val = u4Val & 0xFFFF;
        range = (u4Val > 0) ? ((u4Val * 10000)/(modulation * 116)) : 0;
        Printf("DRAM: SSC range=%d, mod_freq=%d.\n", range, modulation);        
    }
    else
    {
        Printf("DRAM: SSC Disable.\n");
    }

    return 0;    
}

static void _DDR_Thread( void * pvArgs )
{
#ifndef NDEBUG
    UINT32 u4run = 0;
    UINT32 u4TotalSecs = 0;
    UINT32 u4Secs = 0;   
    UINT32 u4Mins = 0;
    UINT32 u4Hrs = 0;    
#endif    
    UINT32 u4ThreadDelay = 5; // Unit in ms.
#ifdef DRAM_HWONLINECALDQS   
    UINT32 u4HWCaliGap = 0;
    UINT32 u4HWCaliLoop = 0;
#endif

    UNUSED(pvArgs);

    // Wait 300 ms to avoid race condition with CLI thread.
    x_thread_delay(300); 
    Printf("DRAM: _DDR_Thread is created.\n");

    HAL_GetTime(& rOrgTime);
    u4InitSec = rOrgTime.u4Seconds;

    while (1)
    {
        // Thread delay is depended on memory read/write test being enabled or not.
        u4ThreadDelay = (u4Cmemrw > 0) ? 5 : 500;
        x_thread_delay(u4ThreadDelay); 

#ifndef NDEBUG
        /*
            Memory test part.
        */
        if (u4Cmemrw)
        {
            if (!pu4M8K)
            {
                pu4M8K = (UINT32 *)x_mem_alloc(MEMTEST_BUFFERSIZE);
                if (pu4M8K == NULL)
                {
                    Printf("pu4M8K allocate fail.\n");
                    u4Cmemrw = 0;
                    continue;
                }
            }
        
            HAL_GetTime(& rNewTime);
            HAL_GetDeltaTime(& rDiffTime, & rOrgTime, & rNewTime);
            
            if (rDiffTime.u4Seconds >= u4Cmemrw)
            {                
                u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
                u4Hrs = u4TotalSecs/(3600);
                u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
                u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
                Printf("DRAM: Memrw Run=%d, Total Time=%d hrs:%d mins:%d secs.\n", 
                    u4run, u4Hrs, u4Mins, u4Secs);
                                
                rOrgTime = rNewTime;
            }

            // LineBounce, ButterflyTest, and DSP PC counter test.
            _DDR_CpuDspMemTest();

            u4run++;
            if (_DDR_IsDQSFail())
            {
                u4TotalSecs = rNewTime.u4Seconds - u4InitSec;
                u4Hrs = u4TotalSecs/(3600);
                u4Mins = (u4TotalSecs - (u4Hrs*3600))/60;
                u4Secs = u4TotalSecs - (u4Hrs*3600) - (u4Mins*60);
                Printf("DRAM: Memrw Run=%d, Total Time=%d hrs:%d mins:%d secs.\n", 
                    u4run, u4Hrs, u4Mins, u4Secs);

                if (fgCmemrwAbort)
                {
                    // Test error, show red at whole panel.
                    VDP_Memory_Test(0);
                    while(1);
                }
            }
        }
#endif

#ifdef DRAM_HWONLINECALDQS
        /*
            h/w calibration part.
        */
        if (u4HwCalDQSEnable)
        {
            ASSERT(u4ThreadDelay != 0);
            
            // Wait 1000 ms to start h/w calibration.               
            if (u4HWCaliGap ++ > (1000/u4ThreadDelay))
            {
                u4HWCaliGap = 0;

                switch(u4HWCaliState)
                {
                    case HWCALI_START:
                        u4HWCaliLoop++;
                        LOG(7, "DRAM: (%d) H/W Cal DQS start...\n", u4HWCaliLoop);

                        // Fill some information to check if memory violation by h/w calibration.
                        au4HwCaliBuffer[0] = (UINT32)au4HwCaliBuffer;
                        au4HwCaliBuffer[1] = (UINT32)0x53875363;
                        au4HwCaliBuffer[2] = (UINT32)0x12345678;
                        au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-3] = (UINT32)au4HwCaliBuffer;
                        au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-2] = (UINT32)0x53875363;
                        au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-1] = (UINT32)0x12345678;
                        
                        _DDR_DQSCalStart();                        
                        u4HWCaliState = HWCALI_PROCESS;
                        break;
                        
                    case HWCALI_PROCESS:
                        if (!_DDR_IsDQSCalDone())
                        {
                            LOG(0, "DRAM: (%d) Fail: H/W Cal DQS not finish.\n", u4HWCaliLoop);                            
                        }
                        else
                        {                        
                            LOG(7, "DRAM: (%d) Pass: H/W Cal DQS done.\n", u4HWCaliLoop);
                            _DDR_DQSCalDisable();

                            // Check if memory violation by h/w calibration.
                            if ((au4HwCaliBuffer[0] != (UINT32)au4HwCaliBuffer) ||
                                 (au4HwCaliBuffer[1] != (UINT32)0x53875363) ||
                                 (au4HwCaliBuffer[2] != (UINT32)0x12345678) ||
                                 (au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-3] != (UINT32)au4HwCaliBuffer) ||
                                 (au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-2] != (UINT32)0x53875363) ||
                                 (au4HwCaliBuffer[HWCALI_ACTUALBUFFERSIZE-1] != (UINT32)0x12345678))
                            {
                                Printf("DRAM: Memory violation !!\n");
                                ASSERT(0);
                            }
                            
                            _DDR_UpdateDQS();
                            u4HWCaliState = HWCALI_START;
                        }
                        break;

                    default:
                        break;
                }                
            }
        }
        
#endif // #ifdef DRAM_HWONLINECALDQS
        
    }
}

static void _DDR_ThreadInit(void)
{
   /*
        Memory Test Thread.
    */
    if (fgDRAMThreadInit)    
    {
        return;
    }
    
    fgDRAMThreadInit = TRUE;

    /* Dram Thread */
    (void)x_thread_create(&phDRAMThreadHdl, 
        "DDRThread", 1024,  128, _DDR_Thread, 0, NULL);
}

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
#ifndef NDEBUG
 CLIMOD_DEBUG_FUNCTIONS(MEMTEST)
#endif

static CLI_EXEC_T _arMemoryTestCmdTbl [] =
{
#ifndef NDEBUG
    CLIMOD_DEBUG_CLIENTRY(MEMTEST),

    { "run", "r", _DDR_CmdMemoryTest, NULL, "DRAM Memory test", CLI_GUEST },
    { "memrw", "memrw", _DDR_Cmdmemoryrw, NULL, "DRAM memory test only once", CLI_GUEST },
    { "bm", "bm", _DDR_BandWidthMontorTest, NULL, "DRAM Bandwidth monitor test", CLI_GUEST },
    { "cm", "cm", _DDR_Cmdcm, NULL, "DRAM continuous memory test", CLI_GUEST },
    { "dqsg", "g", _DDR_DQSGatingWindowTest, NULL, "DRAM DQS gating window test", CLI_GUEST },    
    { "dqs", "dqs", _DDR_DQSDelayTest, NULL, "DRAM DQS delay test", CLI_GUEST },
    { "dqso", "dqso", _DDR_DQSOutputDelayTest, NULL, "DRAM DQS output delay test", CLI_GUEST },
#endif
#ifdef DRAM_HWONLINECALDQS
    { "cal", "cal", _DDR_OnlineDQSCal, NULL, "DRAM Online DQS Calibration",  CLI_GUEST },
#endif
    { "ssc", "ssc", _DDR_SSCTest, NULL, "DRAM spread spectrum",  CLI_GUEST },
    { "query", "q", _DDR_InforQuery, NULL, "DRAM Information Query",  CLI_GUEST },
    
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
void Drv_DRAMInit(void)
{
    // Check and create DDR thread to do memory test.
    if (u4Cmemrw > 0)
    {
        _DDR_ThreadInit();
        return;
    }

#ifdef DRAM_HWONLINECALDQS
    _DDR_ThreadInit();
#endif
}

CLI_MAIN_COMMAND_ITEM(MemTest)
{
    "memtest", NULL, NULL, _arMemoryTestCmdTbl, "Memory test", CLI_GUEST
};

