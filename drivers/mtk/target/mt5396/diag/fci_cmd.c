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

#ifdef CC_BD_FCI
#include "x_hal_ic.h"
#include "x_printf.h"
#else  // CC_DTV_FCI

#include "x_lint.h"

LINT_SAVE_AND_DISABLE

#include "x_hal_926.h"
#include "x_hal_5381.h"
#endif

#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_os.h"
#ifdef CC_BD_FCI
#include "x_rtos.h"
#endif
#include "x_drv_cli.h"
#define DEFINE_IS_LOG    CLI_IsLog
#include "x_debug.h"
#include "x_util.h"

#ifdef CC_BD_FCI
#if !(CONFIG_SECTION_BUILD_LINUX_KO)
#include "x_rand.h"
#endif
#include "drv_fci.h"
#include "x_gpio.h"
#include "x_ckgen.h"
#include "drv_config.h"
#else  // CC_DTV_FCI
#include "x_rand.h"
#endif

#include "fci_if.h"

#ifdef CC_BD_FCI
#include "x_debug.h"
#else
#define UTIL_Printf Printf
#endif

/********************************************************/

#ifdef CC_BD_FCI
#define CLIFCI_Parser(s)    do {    \
        UTIL_Printf(s "\n");             \
        x_cli_parser(s);              \
    } while (0)
#define CLI_Parser x_cli_parser
#else
#define CLIFCI_Parser(s)    do {    \
        UTIL_Printf(s "\n");             \
        CLI_Parser(s);              \
    } while (0)
#endif

#ifdef CC_FCI_ENABLE

#ifdef CC_BD_FCI
//typedef VOID (*FCI_CALLBACK)(INT32 fgStatus);

#ifdef CONFIG_DRV_DRAM128_MODEL
#define FCI_BUFFER_SIZE 16*1024*1024
#else
#define FCI_BUFFER_SIZE 32*1024*1024
#endif
#else  // CC_DTV_FCI
#define FCI_BUFFER_SIZE 16*1024*1024
#define FCI_DIAG_TEST
//#define CC_CARD_DETECT_PIN
//#define CC_FCI_SDIO_SUPPORT
//#define SDIO_DVBH_SUPPORT
#endif

#ifdef CC_BD_FCI
static UINT32 *_pu4FCI_R_Buffer = NULL;
static UINT32 *_pu4FCI_W_Buffer = NULL;
#else
static UINT32 *_pu4FCI_R_Buffer = (UINT32*) 0x01700000;
static UINT32 *_pu4FCI_W_Buffer = (UINT32*) 0x02700000;
#endif

extern UINT32 _u4SDMultiFuncCfg;
extern UINT32 _u4MSMultiFuncCfg;
#ifdef CC_CARD_DETECT_PIN
extern INT32 FCIHW_DetectCard(VOID);
#endif
extern UINT32 _u4SDClockSetting;
extern UINT32 _u4MSClockSetting;

extern UINT32 _u4SdHighClock;
extern UINT32 _u4SdNormalClock;
extern UINT32 _u4SdLowClock;

extern UINT32 _u4MspHighClock;
extern UINT32 _u4MspNormalClock;

extern BOOL  _fgHighSpeedEnable;

#define RANDOM_WRITE_PATTERN 0xF0E8D3C6

#ifdef CC_BD_FCI
#if !(CONFIG_SECTION_BUILD_LINUX_KO)
extern int     rand(void);
#endif
#endif

#define ALIGN_ADDR(addr, align) \
		(((align) == 0) ? (UINT32*)addr : (UINT32*)(((UINT32)addr + align - 1) & ~(align - 1)))

#ifdef CC_BD_FCI
#if 1 // channel A

#if CONFIG_DRV_LINUX_DATA_CONSISTENCY
#define FCI_BUFFER_ALLOC(pBuf, u4Size, u4Align) \
    pBuf = (UINT32*)x_alloc_aligned_mem(u4Size, u4Align); \
    ASSERT(pBuf != NULL);
#else
#define FCI_BUFFER_ALLOC(pBuf, u4Size, u4Align) \
    pBuf = (UINT32*)x_alloc_aligned_nc_mem(u4Size, u4Align); \
    ASSERT(pBuf != NULL);
#endif  // #if CONFIG_DRV_LINUX_DATA_CONSISTENCY

#else // channel B
#define FCI_BUFFER_ALLOC(pBuf, u4Size, u4Align) \
    pBuf = (UINT32*)x_mem_ch2_alloc(u4Size + u4Align); \
    ASSERT(pBuf != NULL); \
    pBuf = (UINT32*)NONCACHE(((UINT32)pBuf + (u4Align-1)) & (~(UINT32)(u4Align-1)));
#endif  // #if 1

#if (CONFIG_SECTION_BUILD_LINUX_KO)

int FCI_rand(void)
{
    HAL_TIME_T rTimeStart = {0};
    UINT32 u4Rand;
    
    HAL_GetTime(&rTimeStart);
    u4Rand = rTimeStart.u4Micros + ((rTimeStart.u4Micros >> 24) & 0x000000FF)
                                                 + ((rTimeStart.u4Micros >> 8 ) & 0x0000FF00)
                                                 + ((rTimeStart.u4Micros << 8 ) & 0x00FF0000)                                                 
                                                 + ((rTimeStart.u4Micros << 24 ) & 0xFF000000);                                                 
    
    return u4Rand;
}

#undef UINT64
#define UINT64 UINT32

#undef INT64
#define INT64 INT32

/*
unsigned long long __aeabi_ldivmod(unsigned int a, unsigned int b)
{
    unsigned int i = 1, v = 0;
    if (a < b) return (unsigned long long)v | ((unsigned long long)a << 32);
    while ((b << 1) <= a)
    {
        b <<= 1;
        i <<= 1;
    }
    while (i != 0)
    {
        if (a >= b)
        {
            a -= b;
            v |= i;
        }
        b >>= 1;
        i >>= 1;
    }
    return (unsigned long long)v | ((unsigned long long)a << 32);
}
*/
#endif // #if (CONFIG_SECTION_BUILD_LINUX_KO)

#endif // #ifdef CC_BD_FCI

static INT32 _FCIInitBuffer(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    if(_pu4FCI_R_Buffer == NULL)
    {
#ifdef CC_BD_FCI
        FCI_BUFFER_ALLOC(_pu4FCI_R_Buffer, FCI_BUFFER_SIZE, 0x20);
#else  // CC_DTV_FCI
        // _pu4FCI_R_Buffer = (UINT32*) x_mem_alloc(FCI_BUFFER_SIZE+4);
        _pu4FCI_R_Buffer = (UINT32*) 0x01700000;

        ASSERT(_pu4FCI_R_Buffer);

        // Get Aligned address
        _pu4FCI_R_Buffer = ALIGN_ADDR(_pu4FCI_R_Buffer,4);
#endif
    }

    if(_pu4FCI_W_Buffer == NULL)
    {
#ifdef CC_BD_FCI
        FCI_BUFFER_ALLOC(_pu4FCI_W_Buffer, FCI_BUFFER_SIZE, 0x20);
#else  // CC_DTV_FCI
        // _pu4FCI_W_Buffer = (UINT32*) x_mem_alloc(FCI_BUFFER_SIZE+4);
        _pu4FCI_W_Buffer = (UINT32*) 0x02700000;

        ASSERT(_pu4FCI_W_Buffer);

        // Get Aligned address
        _pu4FCI_W_Buffer = ALIGN_ADDR(_pu4FCI_W_Buffer,4);
#endif
    }

    UTIL_Printf("\n_pu4FCI_R_Buffer = 0x%X\n", _pu4FCI_R_Buffer);
    UTIL_Printf("_pu4FCI_W_Buffer = 0x%X\n", _pu4FCI_W_Buffer);

    UTIL_Printf("FCI buffer initialized.\n");

    return 0;
}

static INT32 _FCIInit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    CLI_Parser("d_l 5");

    i4Ret = FCI_Init();

    _FCIInitBuffer(i4Argc, szArgv);

    UTIL_Printf("FCI attached device, i4Ret = 0x%X.\n", i4Ret);

    return 0;
}

static INT32 _FCIStop(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = FCI_Stop();

    UTIL_Printf("FCI Stop, i4Ret = 0x%X.\n", i4Ret);

    return 0;
}

static INT32 _FCIList(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD || i4CardType == FCI_MMC_CARD)
    {
        UTIL_Printf("SDC Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_SD_CARD) ? "SD" : ((i4CardType == FCI_SDHC_CARD) ? "SDHC" : "MMC"))
            ,SD_GetModel(), (INT32)(SD_GetSize() >> 11));
    }
    else if(i4CardType == FCI_SDIO_CARD)
    {
        UTIL_Printf("SDC Slot - SDIO Card \n");
    }
    else
    {
        UTIL_Printf("SDC Slot have no device\n");
    }

    i4CardType = MS_GetCard();
    if (i4CardType != FCI_MS_CARD && i4CardType != FCI_MSPRO_CARD)
    {
        UTIL_Printf("MSP Slot have no device\n");
    }
    else
    {
        UTIL_Printf("MSP Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_MS_CARD) ? "MS" : "MSPRO")
            ,MS_GetModel(), (INT32)(MS_GetSize() >> 11));
    }

    return 0;
}
 
extern BOOL _fgFCIPolling;

static INT32 _FCIPolling(INT32 i4Argc, const CHAR ** szArgv)
{    
    if (i4Argc == 2)
    {
        _fgFCIPolling = StrToInt(szArgv[1]) ? TRUE : FALSE;
    }
    else
    {
        UTIL_Printf("fci.polling [1: On; 0: Off], Current _fgFCIPolling = %s\n", _fgFCIPolling ? "TRUE" : "FALSE");
        return 1;
    }
    
    return 0;
}
 
static INT32 _FCIPowerOn(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    FCI_PowerOn();

    return 0;
}

static INT32 _FCIPowerOff(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    FCI_PowerOff();

    return 0;
}

#ifdef FCI_DIAG_TEST
extern INT32 FCI_Diag(void);
static INT32 _FCIDiag(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = FCI_Diag();
    UTIL_Printf( "\n==================\n"
            " FCI Diag %s!"
            "\n==================\n", ((i4Ret==0) ? "Success" : "Fail"));
    return i4Ret;
}
#endif


/******************************************************************************
* Memory card specific functions
******************************************************************************/
static INT32 _Ms_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer, UINT32 u4DataPattern);
static INT32 _Ms_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer);
static INT32 _Sd_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer, UINT32 u4DataPattern);
static INT32 _Sd_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer);
static void   _FCI_OutputBuffer(UINT32 u4BufSz, UINT8 *pu1Buf);

extern VOID FCIHW_SetSRAMSEL(UINT32 u4SRAMSel);
extern INT32 _MSP_Detect(void);
extern INT32 _MSP_SetRWReg(void);
extern INT32 _MSP_ReadReg(void);
extern INT32 _MSP_WriteReg(void);
extern INT32 _MSP_MSSetCmd(CHAR *szStr);
extern INT32 _MSP_MSPSetCmd(CHAR *szStr);
extern INT32 _MSP_GetInt(void);
extern INT32 _MSP_ReadPage(UINT32 u4Block, UINT32 u4PgNo, UINT32 *pu4Result, UINT32 u4BufPtr);
extern INT32 _MSP_WritePage(UINT32 u4Block, UINT32 u4PgNo, UINT32 u4Extra, UINT32 u4BufPtr);
extern INT32 _MSP_BootWrite(void);
extern INT32 _MSP_ReadBlock(UINT32 u4Block, UINT32 u4ExtraMemBuf, UINT32 u4DataMemBuf);
extern INT32 _MSP_WriteBlock(UINT32 u4Block, UINT32 u4ExtraMemBuf, UINT32 u4DataMemBuf);
extern INT32 _MSP_EraseBlock(UINT32 u4Block);
extern INT32 _MSP_SetSerial(void);
extern INT32 _MSP_SetParallel(void);
extern INT32 _MSP_MsproReadAttribute(UINT8 *pu1Buf);
extern VOID MS_PIOModeAccessEnable(BOOL fgEnable);
extern UINT32 MSP_SetHighClock(UINT32 u4MspHighClock);
extern UINT32 MSP_SetNormClock(UINT32 u4MspNormClock);
extern INT32 SDC_SetMMCBusWidth(BOOL fg4BitEnable);
extern INT32 SDC_SetMMCBusWidth(BOOL fg4BitEnable);
extern VOID SD_SingleBlockAccessEnable(BOOL fgEnable);
extern VOID SD_PIOModeAccessEnable(BOOL fgEnable);
extern INT32 SDC_SetSDBusWidth(BOOL fg4BitEnable);
extern BOOL SD_IsPIOMode(VOID);
extern BOOL SD_IsHighSpeedMode(VOID);
extern BOOL SD_IsFifoTestMode(VOID);
extern BOOL SD_IsCmdTimeOutTestMode(VOID);
extern BOOL SD_IsDataTimeOutTestMode(VOID);
extern VOID FCIHW_ChangeSDCClock(INT32 i4ClkMHz);
extern VOID FCIHW_ChangeMSPClock(INT32 i4ClkMHz);

VOID MSClockChange(INT32 *pClockIndex, INT32 i4ClockUpperLimit, INT32 i4ClockLowerLimit)
{    
    UINT32 u4ClockSel[] = {FCI_MSP_CLOCK_SELECTION_STRING};

    INT32 i4ClockIndexLimit = 0;
    
    i4ClockIndexLimit = sizeof(u4ClockSel) / sizeof(UINT32);
        
SelectMSClock:

    (*pClockIndex)++;
    // Circular ring
    if (*pClockIndex >= i4ClockIndexLimit)
    {
        *pClockIndex = 0;
    }

    // Upper bound
    if (u4ClockSel[*pClockIndex] > i4ClockUpperLimit)
    {
        goto SelectMSClock;
    }

    // Lower bound
    if (u4ClockSel[*pClockIndex] < i4ClockLowerLimit)
    {
        goto SelectMSClock;
    }

    UTIL_Printf("\n@@ Clock setting = %d Mhz\n", u4ClockSel[*pClockIndex]);

    // Setup high/normal clock to avoid clock change when switch parallel / serial mode
    MSP_SetHighClock(u4ClockSel[*pClockIndex]);
    MSP_SetNormClock(u4ClockSel[*pClockIndex]);
    
    FCIHW_ChangeMSPClock(u4ClockSel[*pClockIndex]);
}

static INT32 _MSCLI_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT32 u4CardSize;
    UINT32 u4TestSize = 0x1000000;
    UINT32 u4SRAMSEL = 0;
    INT32 i4ClockUpperLimit = -1, i4ClockLowerLimit = -1;
    INT32 i4ClockIndex = 0;
    INT32 i4Loop = 0, i4LoopCount = 1;

    if (i4Argc >= 3)
    {
        i4ClockUpperLimit = StrToInt(szArgv[1]);      // Mhz
        i4ClockLowerLimit = StrToInt(szArgv[2]);      // Mhz
        i4LoopCount = StrToInt(szArgv[3]);      // Count

        i4ClockIndex = 0;
    }

    u4CardSize = MS_GetSize();

    UTIL_Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));
    UTIL_Printf("Current clock = %d Mhz\n", _u4MSClockSetting);

    for (i4Loop = 0; i4Loop<i4LoopCount; i4Loop++)
    {
        // write data to MS card.
        for (u4Addr=0; u4Addr<(u4CardSize/(32*1024)); u4Addr++)
        {
            // Setup FIFO size
            u4SRAMSEL = (u4Addr % 3)+1;  // 1, 2, 3
            FCIHW_SetSRAMSEL(u4SRAMSEL);

            UTIL_Printf("\nTest MS %d ~ %d MB, u4SRAMSEL = %d\n", u4Addr*16, (u4Addr+1)*16, u4SRAMSEL);

            _Ms_WriteFunc(u4TestSize, u4Addr*32768, (UINT32)_pu4FCI_W_Buffer, RANDOM_WRITE_PATTERN);
            _Ms_ReadFunc(u4TestSize, u4Addr*32768, (UINT32)_pu4FCI_R_Buffer);

            // Compare the result
            for(i=0; i<u4TestSize; i+=4)
            {
                if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
                {
                    UTIL_Printf("Test Failed : u4Addr = 0x%x, inconsistant at element %d \n", u4Addr, i/4);
                    break;
                }
            }

            if(i == u4TestSize)
            {
                UTIL_Printf("Test OK.\n");
            }

            // Change Clock Setting
            if (i4ClockUpperLimit != -1)
            {        
                MSClockChange(&i4ClockIndex, i4ClockUpperLimit, i4ClockLowerLimit);
            }
        }
        
        UTIL_Printf("\nTest Loop %d finish.\n", i4Loop);
    }    

    UTIL_Printf("\nTest MS card Complete.\n");

    return 0;
}

static INT32 _MSCLI_Performance(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT32 u4CardSize;
    UINT32 u4TestSize = 0x1000000;
    UINT32 u4SRAMSEL = 0;
    INT32 i4Ret, i4TestCnt = 0, i4TestLimit = 10;

    HAL_TIME_T rTimeStart = {0};
    HAL_TIME_T rTimeEnd = {0};
    HAL_TIME_T rTimeDelta = {0};
    HAL_TIME_T rWriteTimeTotal = {0};
    HAL_TIME_T rReadTimeTotal = {0};
    UINT64 u8TotalTime, u8WritePmsInt, u8WritePmsFit, u8ReadPmsInt, u8ReadPmsFit;
    UINT32 u4Lba, u4Rand;
    BOOL fgFailedStart = FALSE;
    UINT32 u4FailedCnt = 0;
    UINT32 u4LastFailedOfst = 0;

    if (i4Argc >= 2)
    {
        i4TestLimit = StrToInt(szArgv[1]);
    }
    
    UTIL_Printf("perm [Test Count] - test count = %d\n", i4TestLimit);

    u4CardSize = MS_GetSize();

    UTIL_Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));
    UTIL_Printf("Current clock = %d Mhz\n", _u4MSClockSetting);
    
    // Set write buffer content : 00~FF, 00~FF, ... (repeat)
    // randon value
#ifdef CC_BD_FCI
#if (CONFIG_SECTION_BUILD_LINUX_KO)
    u4Rand = FCI_rand();
#else
    u4Rand = rand() % 0xffffffff;
#endif
#else
    u4Rand = rand() % 0xffffffff;
#endif

    for(i=0; i<u4TestSize; i+=4)
    {
        *((UINT32*)(((UINT32)_pu4FCI_W_Buffer)+i)) = (i+u4Rand);
    }
    
    for (u4Addr=0, i4TestCnt = 0; u4Addr<(u4CardSize/(32*1024)) && i4TestCnt<i4TestLimit; u4Addr++, i4TestCnt++)
    {
        x_memset(_pu4FCI_R_Buffer, 0x00, u4TestSize);
        
        // Setup FIFO size
        u4SRAMSEL = (u4Addr % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);

        UTIL_Printf("\nTest MS %d ~ %d MB, u4SRAMSEL = %d\n", u4Addr*16, (u4Addr+1)*16, u4SRAMSEL);

        u4Lba = u4Addr*32768;

        // Write Test first     
        HAL_GetTime(&rTimeStart);
        i4Ret = MS_Write((UINT64)(u4Lba << 9), (UINT32)_pu4FCI_W_Buffer, u4TestSize);
        HAL_GetTime(&rTimeEnd);        
        HAL_GetDeltaTime(&rTimeDelta, &rTimeStart, &rTimeEnd);

        rWriteTimeTotal.u4Seconds += rTimeDelta.u4Seconds;        
        rWriteTimeTotal.u4Micros += rTimeDelta.u4Micros;
        
        UTIL_Printf("(%d) : Write Cost Time is %d.%06d seconds, Write Buffer = 0x%x\n", i4TestCnt, rTimeDelta.u4Seconds, rTimeDelta.u4Micros, _pu4FCI_W_Buffer);

        if (i4Ret)
        {
            UTIL_Printf("MS Write  Failed\n");
            break;
        }

        // Read Test later
        HAL_GetTime(&rTimeStart);
        i4Ret = MS_Read((UINT64)(u4Lba << 9), (UINT32)_pu4FCI_R_Buffer, u4TestSize);
        HAL_GetTime(&rTimeEnd);        
        HAL_GetDeltaTime(&rTimeDelta, &rTimeStart, &rTimeEnd);

        rReadTimeTotal.u4Seconds += rTimeDelta.u4Seconds;        
        rReadTimeTotal.u4Micros += rTimeDelta.u4Micros;
        
        UTIL_Printf("(%d) : Read Cost Time is %d.%06d seconds, Read Buffer = 0x%x\n", i4TestCnt, rTimeDelta.u4Seconds, rTimeDelta.u4Micros, _pu4FCI_R_Buffer);

        if (i4Ret)
        {
            UTIL_Printf("MS Read  Failed\n");
            break;
        }        

        // Compare the result
        for(i=0; i<u4TestSize; i+=4)
        {
            if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
            {
                if (fgFailedStart == FALSE)
                {
                    UTIL_Printf("Test Failed : u4Addr = 0x%x, inconsistant at element %d, address : 0x%x\n", u4Addr, i/4, i);
                    u4FailedCnt++;
                    u4LastFailedOfst = i;
                }
                
                fgFailedStart = TRUE;
            }
            else
            {
                if (fgFailedStart == TRUE)
                {
                    UTIL_Printf("Test OK : u4Addr = 0x%x, consistant at element %d, address : 0x%x, Failed %d Bytes\n", u4Addr, i/4, i, i - u4LastFailedOfst);

                    if ((i%512) != 0)  // Not 512 byte Aligned
                    {
                        UINT32 u4EndLength = 512-(i%512);
                        
                        UTIL_Printf("Not Aligned with the end of 512 byte : %d Bytes\n", u4EndLength);
                        
                        UTIL_Printf("[Debug] Write Buf1 = 0x%x\n",((UINT32)_pu4FCI_W_Buffer+u4LastFailedOfst));
                        _FCI_OutputBuffer(u4EndLength+(i - u4LastFailedOfst), (UINT8 *)((UINT32)_pu4FCI_W_Buffer+u4LastFailedOfst));
                        UTIL_Printf("[Debug] Read Buf2 = 0x%x\n",((UINT32)_pu4FCI_R_Buffer+u4LastFailedOfst));    
                        _FCI_OutputBuffer(u4EndLength+(i - u4LastFailedOfst), (UINT8 *)((UINT32)_pu4FCI_R_Buffer+u4LastFailedOfst));
                    }
                }
                
                fgFailedStart = FALSE;
            }
        }

        if(i == u4TestSize)
        {
            UTIL_Printf("Test OK.\n");
        }
        else
        {
            UTIL_Printf("Test Break.\n");
            break;
        }
    }


    UTIL_Printf("Total Failed Count : %d\n", u4FailedCnt);

    rWriteTimeTotal.u4Seconds += rWriteTimeTotal.u4Micros / 1000000;
    rWriteTimeTotal.u4Micros  = rWriteTimeTotal.u4Micros % 1000000;
    rReadTimeTotal.u4Seconds += rReadTimeTotal.u4Micros / 1000000;
    rReadTimeTotal.u4Micros  = rReadTimeTotal.u4Micros % 1000000;

    UTIL_Printf("Total Test Count : %d\n", i4TestCnt);    
    UTIL_Printf("Test Unit : %d MB\n", u4TestSize / 1024 / 1024);
    
    UTIL_Printf("Total Write Cost Time : %d.%06d seconds\n", rWriteTimeTotal.u4Seconds, rWriteTimeTotal.u4Micros);
    UTIL_Printf("Total Read Cost Time : %d.%06d seconds\n", rReadTimeTotal.u4Seconds, rReadTimeTotal.u4Micros);

    if (i4TestCnt != 0)
    {
        // Calculate Write performance  
        u8TotalTime = rWriteTimeTotal.u4Seconds*1000000;
        u8TotalTime += rWriteTimeTotal.u4Micros;    // => u8TotalTime is actual time * 1000000
    
        u8WritePmsInt = (UINT64)(u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000) / u8TotalTime;
        u8WritePmsFit = (u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000 - u8WritePmsInt * u8TotalTime);
    
        u8WritePmsFit *= 1000;
        u8WritePmsFit /= u8TotalTime; // <== performance result, Unit : Mbps
        
        UTIL_Printf("  => Write Performance : %d.%03d M bps\n", (UINT32)u8WritePmsInt, (UINT32)u8WritePmsFit);
        
        // Calculate Read performance                                                                    
        u8TotalTime = rReadTimeTotal.u4Seconds*1000000;                                                  
        u8TotalTime += rReadTimeTotal.u4Micros;    // => u8TotalTime is actual time * 1000000            
                                                                                                          
        u8ReadPmsInt = (UINT64)(u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000) / u8TotalTime;        
        u8ReadPmsFit = (u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000 - u8ReadPmsInt * u8TotalTime);
                                                                                                          
        u8ReadPmsFit *= 1000;                                                                            
        u8ReadPmsFit /= u8TotalTime; // <== performance result, Unit : Mbps                              
                                                                                                          
        UTIL_Printf("  => Read Performance : %d.%03d M bps\n", (UINT32)u8ReadPmsInt, (UINT32)u8ReadPmsFit);
    }
    return 0;
}

static INT32 _MSCLI_Attach(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = MS_DetectCardRemove();

    if(i4Ret == 0)
    {
        i4Ret = MS_Insert();
    }

    if (i4Ret!=0)
    {
        UTIL_Printf("Attach MS card failed.\n");
        return 1;
    }

    UTIL_Printf("Attach MS card successfully.\n");
    return 0;
}

static INT32 _MSCLI_Detach(INT32 i4Argc, const CHAR ** szArgv)
{
    MS_Remove();
    UTIL_Printf("Detach MSPslot device.\n");
    return 0;
}

static INT32 _MSCLI_PIOModeEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.ms.pio [1:Enable; 0:Disable], Current mode : %s\n", MS_IsPIOMode() ? "PIO":"DMA");
        return 1;
    }

    fgEnable = (StrToInt(szArgv[1]) ? TRUE : FALSE);

    MS_PIOModeAccessEnable(fgEnable);
    
    UTIL_Printf("MSCslot %s mode Access.\n", fgEnable ? "PIO":"DMA");
    
    return 0;
}

static INT32 _Ms_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer)
{
    UINT32 u4MemPtr;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    u4MemPtr = (UINT32)u4MemBuffer;

    // Clear buffer content
    x_memset((VOID *)u4MemPtr, 0x00, u4MemLen);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = MS_Read((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("Read Cost Time is %d.%06d seconds, Read Buffer = 0x%x\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros, u4MemPtr);

    if (i4Ret)
    {
        UTIL_Printf("MS Read failed\n");
    }
    else
    {
        UTIL_Printf("MS Read ok - %s Mode\n", MS_IsPIOMode() ? "PIO" : "DMA");
        _FCI_OutputBuffer(64, (UINT8 *)u4MemPtr);
    }

    return i4Ret;
}

static INT32 _MSCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.ms.read [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Ms_ReadFunc(u4MemLen, u4Lba, (UINT32)_pu4FCI_R_Buffer);

    return 0;
}

static INT32 _Ms_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer, UINT32 u4DataPattern)
{
    UINT32 u4MemPtr, i, u4Rand;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    u4MemPtr = (UINT32)u4MemBuffer;

    // randon value
#ifdef CC_BD_FCI
#if (CONFIG_SECTION_BUILD_LINUX_KO)
    u4Rand = FCI_rand();
#else   
    u4Rand = rand() % 0xffffffff;
#endif
#else
    u4Rand = rand() % 0xffffffff;
#endif

    // Set the buffer content : 00~FF, 00~FF, ... (repeat)
    for(i=0; i<u4MemLen; i+=4)
    {
        if (u4DataPattern == RANDOM_WRITE_PATTERN)    // no assign data pattern, use random number to write data
        {
            *((UINT32*)(u4MemPtr+i)) = (i+u4Rand);
        }
        else
        {
            *((UINT32*)(u4MemPtr+i)) = u4DataPattern;
        }
    }

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = MS_Write((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("Write Cost Time is %d.%06d seconds, Write Buffer = 0x%x\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros, u4MemPtr);

    if (i4Ret)
    {
        UTIL_Printf("MS Write  Failed\n");
    }
    else
    {
        UTIL_Printf("MS Write ok - %s Mode, u4Rand = 0x %02X %02X %02X %02X\n", MS_IsPIOMode() ? "PIO" : "DMA", (u4Rand & 0xFF), ((u4Rand>>8) & 0xFF), ((u4Rand>>16) & 0xFF), ((u4Rand>>24) & 0xFF));
    }

    return i4Ret;
}

static INT32 _MSCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba, u4DataPattern = RANDOM_WRITE_PATTERN;

    if (i4Argc != 3)
    {
        if (i4Argc == 4)
        {
            u4DataPattern = StrToInt(szArgv[3]); // in Byte base
            UTIL_Printf("fci.ms.write Data Pattern = 0x%x\n", u4DataPattern);
        }
        else
        {
            UTIL_Printf("fci.ms.write [lba] [memlen] [data Pattern]\n");
            return 1;
        }
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Ms_WriteFunc(u4MemLen, u4Lba, (UINT32)_pu4FCI_W_Buffer, u4DataPattern);

    return 0;
}

static BOOL fgCompareFunc(UINT32 *pu4Buf1, UINT32 *pu4Buf2, UINT32 u4MemLen)
{
    UINT32 i;
    
    // Compare the result
    for(i=0; i<u4MemLen; i+=4)
    {
        if(pu4Buf1[i/4] != pu4Buf2[i/4])
        {
            UTIL_Printf("Test Failed : inconsistant at element %d \n", i/4);
            break;
        }
    }

    if(i == u4MemLen)
    {
        return TRUE;
    }

    UTIL_Printf("[Debug] Write Buf1 = 0x%x\n",pu4Buf1);
    _FCI_OutputBuffer(u4MemLen, (UINT8 *)pu4Buf1);
    UTIL_Printf("[Debug] Read Buf2 = 0x%x\n",pu4Buf2);    
    _FCI_OutputBuffer(u4MemLen, (UINT8 *)pu4Buf2);

    return FALSE;
}

static INT32 _MSCLI_Compare(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4MemLen;
    BOOL fgSuccess;
    
    if (i4Argc != 2)
    {
        UTIL_Printf("fci.ms.compare [memlen]\n");
        return 1;
    }

    u4MemLen = StrToInt(szArgv[1]); // in Byte base

    fgSuccess = fgCompareFunc(_pu4FCI_W_Buffer, _pu4FCI_R_Buffer, u4MemLen);

    if (fgSuccess)
    {
        UTIL_Printf("Compare OK.\n");
    }
    
    return 0;
}

BOOL MSCrossSingleTest(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4WBuf, UINT32 u4RBuf, INT32 nTestOrder)
{
    BOOL fgSuccess = TRUE;
    INT32 i4Ret;

    UTIL_Printf(" [%s] - (%d)\n", __FUNCTION__, nTestOrder);
    
    UTIL_Printf(" [w 0x%X 0x%X]\n", u4Lba, u4MemLen);
    i4Ret = _Ms_WriteFunc(u4MemLen, u4Lba, u4WBuf, RANDOM_WRITE_PATTERN);

    if (i4Ret)
    {
        fgSuccess = FALSE;
    }
    
    if (fgSuccess)
    {
        UTIL_Printf(" [r 0x%X 0x%X]\n", u4Lba, u4MemLen);    
        i4Ret = _Ms_ReadFunc(u4MemLen, u4Lba, u4RBuf);

        if (i4Ret)
        {
            fgSuccess = FALSE;
        }
    }

    if (fgSuccess)
    {
        fgSuccess = fgCompareFunc((UINT32*)u4WBuf, (UINT32*)u4RBuf, u4MemLen);
    }

    UTIL_Printf(" ==> u4Lba = 0x%08X, u4MemLen = 0x%X => %s\n", u4Lba, u4MemLen, fgSuccess ? "OK" : "NG");

    return fgSuccess;
}

BOOL MSCrossTest(BOOL fg4BitMode)
{
    UINT32  u4MemLen;
    UINT32  u4Lba;
    BOOL fgSuccess;
    INT64 u8MSTotalLBA = MS_GetSize();               
    UINT8   *pucRBuf1 = (UINT8*)_pu4FCI_R_Buffer;
    UINT8   *pucRBuf2 = (UINT8*)((UINT32)_pu4FCI_R_Buffer + FCI_BUFFER_SIZE / 2);
    UINT8   *pucWBuf1 = (UINT8*)_pu4FCI_W_Buffer;
    UINT8   *pucWBuf2 = (UINT8*)((UINT32)_pu4FCI_W_Buffer + FCI_BUFFER_SIZE / 2);    

    // (1) 
    u4Lba = (UINT32)(u8MSTotalLBA / 5 * 3);        u4MemLen = 0x1000000;
    fgSuccess = MSCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2), ((UINT32)pucRBuf2), 1);

    if (!fgSuccess)        
    {
        UTIL_Printf(" MSCrossTest failed 1\n");
        return FALSE;
    }    

    // (2) 
    u4Lba = (UINT32)(u8MSTotalLBA / 3 * 2);        u4MemLen = 0x100000;
    fgSuccess = MSCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2)+u4MemLen, ((UINT32)pucRBuf2)+u4MemLen, 2);

    if (!fgSuccess)        
    {
        UTIL_Printf(" MSCrossTest failed 2\n");
        return FALSE;
    }    
    
    
    // (3) 
    u4Lba = (UINT32)(u8MSTotalLBA / 4);        u4MemLen = 0x8000;
    fgSuccess = MSCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf1)+u4MemLen, ((UINT32)pucRBuf1)+u4MemLen, 3);

    if (!fgSuccess)        
    {
        UTIL_Printf(" MSCrossTest failed 3\n");
        return FALSE;
    }    

    // (4) 
    u4Lba = (UINT32)(u8MSTotalLBA / 3);        u4MemLen = 0x10000;
    fgSuccess = MSCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2)+u4MemLen, ((UINT32)pucRBuf2)+u4MemLen, 4);

    if (!fgSuccess)        
    {
        UTIL_Printf(" MSCrossTest failed 4\n");
        return FALSE;
    }    

    // (5) 
    u4Lba = (UINT32)(u8MSTotalLBA / 2);        u4MemLen = 0x40000;
    fgSuccess = MSCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2)+u4MemLen, ((UINT32)pucRBuf2)+u4MemLen, 5);

    if (!fgSuccess)        
    {
        UTIL_Printf(" MSCrossTest failed 5\n");
        return FALSE;
    }    
    
    // (6) 
    u4Lba = 0x00;           u4MemLen = 0x1000;

    fgSuccess = MSCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf1)+u4MemLen, ((UINT32)pucRBuf1)+u4MemLen, 6);

    if (!fgSuccess)        
    {
        UTIL_Printf(" MSCrossTest failed 6\n");
        return FALSE;
    }    
    
    return TRUE;
}

static INT32 _MSCLI_CrossTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = -1, i, i4Count = 1;
    UINT8   *pucRBuf1 = (UINT8*)_pu4FCI_R_Buffer;
    UINT8   *pucRBuf2 = (UINT8*)((UINT32)_pu4FCI_R_Buffer + FCI_BUFFER_SIZE / 2);
    UINT8   *pucWBuf1 = (UINT8*)_pu4FCI_W_Buffer;
    UINT8   *pucWBuf2 = (UINT8*)((UINT32)_pu4FCI_W_Buffer + FCI_BUFFER_SIZE / 2);
    UINT32  u4FailedCount = 0; 
    UINT32  u4SRAMSEL = 0;
    BOOL fgSuccess;    
    UINT32 u4CardSize;    
    INT32 i4ClockIndex;
    INT32 i4ClockUpperLimit = -1, i4ClockLowerLimit = -1;    

    if (i4Argc == 2)
    {
      i4Count = StrToInt(szArgv[1]);
    }
    
    if (i4Argc == 4)
    {
        i4Count = StrToInt(szArgv[1]);      
        i4ClockUpperLimit = StrToInt(szArgv[2]);      // Mhz
        i4ClockLowerLimit = StrToInt(szArgv[3]);      // Mhz

        i4ClockIndex = 0;
    }
    
    UTIL_Printf("cross [Test Count] [Upper Clock] [Lower Clock] - test count = %d\n", i4Count, i4ClockUpperLimit, i4ClockLowerLimit);
        
    u4CardSize = MS_GetSize();

    UTIL_Printf("Test card size = %d MB. But width = %d\n", (u4CardSize/(2*1024)), MS_GetBusWidth());
    
    UTIL_Printf("[%s]\n      RBuf1 : 0x%x, RBuf2 : 0x%x\n      WBuf1 : 0x%x, WBuf2 : 0x%x\n", __FUNCTION__, pucRBuf1, pucRBuf2, pucWBuf1, pucWBuf2 );

    for (i=0; i<i4Count; i++)
    { 
        // Setup FIFO size
        u4SRAMSEL = (i % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);
        
        // DMA/PIO cross test
        MS_PIOModeAccessEnable((i%2) ? TRUE:FALSE);
        
        UTIL_Printf("# Count = %d (Total : %d), u4SRAMSEL = %d, %s Mode  :\n", i+1, i4Count, u4SRAMSEL, MS_IsPIOMode() ? "PIO":"DMA");
        UTIL_Printf("Current clock = %d Mhz\n", _u4MSClockSetting);

        if (MS_GetBusWidth() == 4)
        {
            UTIL_Printf(" @@ 4 Bit mode (Current : %d, Total : %d) : \n", i+1, i4Count);

            i4Ret = _MSP_SetParallel();
            
            if (i4Ret)
            {
                UTIL_Printf("Switch Bus Failed\n");
            }
            
            fgSuccess = MSCrossTest(TRUE);

            if (!fgSuccess)
            {
                u4FailedCount++;
                break;
            }
        
            i4Ret = _MSP_SetSerial();
        }

        UTIL_Printf(" @@ 1 Bit mode (Current : %d, Total : %d) : \n", i+1, i4Count);
        UTIL_Printf("Current clock = %d Mhz\n", _u4MSClockSetting);
            
        fgSuccess = MSCrossTest(FALSE);

        if (!fgSuccess)
        {
            u4FailedCount++;
            break;
        }
          
        // Change Clock Setting
        if (i4ClockUpperLimit != -1)
        {
            MSClockChange(&i4ClockIndex, i4ClockUpperLimit, i4ClockLowerLimit);
        }
    }
    
    UTIL_Printf("[%s] Total Test Count = (%d / %d), Failed Count = %d", __FUNCTION__, i, i4Count, u4FailedCount);

    return 0;
}

static INT32 _MSCLI_ChangeClock(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ClockSel;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.ms.clock [Mhz], Current clock = %d Mhz\n", _u4MSClockSetting);
        FCI_MSP_Clock_String;
        return 1;
    }

    u4ClockSel = StrToInt(szArgv[1]);

    UTIL_Printf("Clock setting = %d Mhz\n", u4ClockSel);

    FCIHW_ChangeMSPClock(u4ClockSel);

    return 0;
}

static INT32 _MSCLI_SetHighClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        UTIL_Printf("hiclock [xx] Mhz, Current clock = %d Mhz\n", _u4MspHighClock);
        return 1;
    }

    _u4MspHighClock = StrToInt(szArgv[1]);

    UTIL_Printf("High Clock setting = %d Mhz\n", _u4MspHighClock);

    return 0;
}

static INT32 _MSCLI_SetNormClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        UTIL_Printf("noclock [xx] Mhz, Current clock = %d Mhz\n", _u4MspNormalClock);
        return 1;
    }

    _u4MspNormalClock = StrToInt(szArgv[1]);

    UTIL_Printf("Normal Clock setting = %d Mhz\n", _u4MspNormalClock);

    return 0;
}
    
static INT32 _MSCLI_List(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = MS_GetCard();
    if (i4CardType != FCI_MS_CARD && i4CardType != FCI_MSPRO_CARD)
    {
        UTIL_Printf("MSP Slot have no device\n");
    }
    else
    {
        UTIL_Printf("MSP Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_MS_CARD) ? "MS" : "MSPRO")
            ,MS_GetModel(), (INT32)(MS_GetSize() >> 11));
    }

    return 0;
}

static INT32 _MSCLI_Config(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        UTIL_Printf("config [Multifunc No.], Current Config = %d\n", _u4MSMultiFuncCfg);
        return 1;
    }

    _u4MSMultiFuncCfg = StrToInt(szArgv[1]);

    return 0;
}

extern BOOL _fgFCIContinueClock;
static INT32 _MSCLI_CClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        UTIL_Printf("cclock [1: Enable, 0: Disable], Current Setting = %d\n", _fgFCIContinueClock);
        return 1;
    }

    _fgFCIContinueClock = StrToInt(szArgv[1]);

    return 0;
}

#ifdef CC_CARD_DETECT_PIN
static INT32 _MSCLI_CardDetect(INT32 i4Argc, const CHAR ** szArgv)
{
    UTIL_Printf("Card Remove = %d\n", FCIHW_DetectCard());
    return 0;
}
#endif

static INT32 _MSCLI_SetSerial(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _MSP_SetSerial();
    UTIL_Printf("Set serial ret:%d\n", i4Ret);
    return 0;
}

static INT32 _MSCLI_SetParallel(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _MSP_SetParallel();
    UTIL_Printf("Set parallel ret:%d\n", i4Ret);
    return 0;
}

static INT32 _MSCLI_RdBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4BlkNo;

    if (i4Argc < 2)
    {
        UTIL_Printf("%s [blkno]\n", szArgv[0]);
        return 1;
    }

    i4BlkNo = StrToInt(szArgv[1]);

    return _MSP_ReadBlock(i4BlkNo, (UINT32)_pu4FCI_W_Buffer, (UINT32)_pu4FCI_R_Buffer);
}

static INT32 _MSCLI_WrBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4BlkNo;

    if (i4Argc < 2)
    {
        UTIL_Printf("%s [blkno]\n", szArgv[0]);
        return 1;
    }

    i4BlkNo = StrToInt(szArgv[1]);

    if (i4BlkNo==0 || i4BlkNo==1)
    {
        UTIL_Printf("No 0 or 1 => don't write !!\n");
        return 0;
    }
    return _MSP_WriteBlock(i4BlkNo, (UINT32)_pu4FCI_W_Buffer, (UINT32)_pu4FCI_R_Buffer);
}

static INT32 _MSCLI_ErBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4BlkNo;

    if (i4Argc < 2) {
        UTIL_Printf("%s [blkno]\n", szArgv[0]);
        return 1;
    }

    i4BlkNo = StrToInt(szArgv[1]);

    return _MSP_EraseBlock(i4BlkNo);
}

static INT32 _MSCLI_BootWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _MSP_BootWrite();
    UTIL_Printf("BootWrite return %d\n", i4Ret);
    return i4Ret;
}

static INT32 _MSCLI_Detect(INT32 i4Argc, const CHAR ** szArgv)
{
    return _MSP_Detect();
}

static INT32 _MSCLI_SetRWReg(INT32 i4Argc, const CHAR ** szArgv)
{
    return _MSP_SetRWReg();
}

static INT32 _MSCLI_ReadReg(INT32 i4Argc, const CHAR ** szArgv)
{
    return _MSP_ReadReg();
}

static INT32 _MSCLI_WriteReg(INT32 i4Argc, const CHAR ** szArgv)
{
    return _MSP_WriteReg();
}

static INT32 _MSCLI_SetCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = MS_GetCard();

    if (i4CardType == FCI_MS_CARD)
    {
        if (i4Argc < 2)
        {
            UTIL_Printf("%s [read/write/end/erase/stop/sleep/clear/reset]\n", szArgv[0]);
            return 1;
        }

        return _MSP_MSSetCmd((CHAR *) szArgv[1]);
    }
    else if (i4CardType == FCI_MSPRO_CARD)
    {
        if (i4Argc < 2)
        {
            UTIL_Printf("%s [read/write/erase/sleep/stop/clear/getibd]\n", szArgv[0]);
            return 1;
        }
        return _MSP_MSPSetCmd((CHAR *) szArgv[1]);
    }

    return 0;
}

static INT32 _MSCLI_GetInt(INT32 i4Argc, const CHAR ** szArgv)
{
    return _MSP_GetInt();
}

static INT32 _MSCLI_ReadAttr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Block, u4PgNo, u4Data;
    INT32 i4Ret = -1;
    UINT8 *pu1Buf;
    UINT8 u1Overwrite, u1Manage;
    UINT16 u2Data;

    INT32 i4CardType;

    i4CardType = MS_GetCard();

    if (i4CardType == FCI_MS_CARD)
    {
        pu1Buf = (UINT8 *)_pu4FCI_W_Buffer;   // read to write buffer for write attr
        u4Block = u4PgNo = 0;
        if (i4Argc < 3)
        {
            UTIL_Printf("%s [blkno] [pgno]\n", szArgv[0]);
            return 1;
        }
        u4Block = StrToInt(szArgv[1]);
        u4PgNo = StrToInt(szArgv[2]);
        i4Ret = _MSP_ReadPage(u4Block, u4PgNo, &u4Data, (UINT32) pu1Buf);
        u1Overwrite = (u4Data >> 24) & 0x0ff;
        u1Manage = (u4Data >> 16) & 0x0ff;
        u2Data = (u4Data & 0x0ffff);

        UTIL_Printf("Overwrite:0x%02x Management:0x%02x Logic:0x%04x, u4Data = 0x%x\n",
            u1Overwrite, u1Manage, u2Data, u4Data);

        if (i4Ret!=0)
        {
            UTIL_Printf("ReadAttr failed\n");
            _MSP_MSSetCmd("reset");
            _MSP_MSSetCmd("reset");
        }
    }
    else if (i4CardType == FCI_MSPRO_CARD)
    {
        i4Ret = _MSP_MsproReadAttribute((UINT8 *)_pu4FCI_R_Buffer);

        if (i4Ret == 0)
        {
            UTIL_Printf("MSP ReadAttr OK.\n");
            _FCI_OutputBuffer(64, (UINT8 *)_pu4FCI_R_Buffer);
        }
        else
        {
            UTIL_Printf("MSP ReadAttr Failed.\n");
        }
    }

    return i4Ret;
}

static INT32 _MSCLI_WriteAttr(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Block, u4PgNo, u4Extra, u4Data;
    INT32 i4Ret;

    u4Block = u4PgNo = 0;
    if (i4Argc < 4)
    {
        UTIL_Printf("%s [blkno] [pgno] [extra]\n", szArgv[0]);
        return 1;
    }

    u4Block = StrToInt(szArgv[1]);
    u4PgNo = StrToInt(szArgv[2]);
    u4Extra = StrToInt(szArgv[3]);

    i4Ret = _MSP_WritePage(u4Block, u4PgNo, u4Extra, (UINT32) _pu4FCI_W_Buffer);
    if (i4Ret!=0)
    {
        UTIL_Printf("WriteAttr failed\n");
        _MSP_MSSetCmd("reset");
        _MSP_MSSetCmd("reset");
        return 1;
    }

    i4Ret = _MSP_ReadPage(u4Block, u4PgNo, &u4Data, (UINT32) _pu4FCI_R_Buffer);

    if (i4Ret!=0)
    {
        UTIL_Printf("WriteAttr read back failed\n");
        _MSP_MSSetCmd("reset");
        _MSP_MSSetCmd("reset");
        return 1;
    }

    UTIL_Printf("WrExtra:0x%08x RdExtra:0x%08x\n", u4Extra, u4Data);
    UTIL_Printf("Wr----0x%08x\n", (UINT32)_pu4FCI_W_Buffer);
    _FCI_OutputBuffer(64, (UINT8 *)_pu4FCI_W_Buffer);

    UTIL_Printf("Rd----0x%08x\n", (UINT32)_pu4FCI_R_Buffer);
    _FCI_OutputBuffer(64, (UINT8 *)_pu4FCI_R_Buffer);

    return i4Ret;
}

static INT32 _MSCLI_AtrTbl(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret, i, j;
    UINT32 u4Data;
    UINT8 *pu1Buf;
    UINT8 u1Overwrite, u1Manage;
    UINT16 u2Data;
    UINT32 u4BlkNo, u4PgNo, u4BlkNum;

    u4BlkNo = 0;
    u4BlkNum = 8192;

    if (i4Argc > 1)
    {
        u4BlkNo = StrToInt(szArgv[1]);
        u4BlkNum -= u4BlkNo;
    }

    if (i4Argc > 2)
    {
        u4BlkNum = StrToInt(szArgv[2]);
    }

    UTIL_Printf("BlkNo is %d, BlkNum is %d\n", u4BlkNo, u4BlkNum);
    pu1Buf = (UINT8 *)0x2f00000;
    for (i=u4BlkNo; i<u4BlkNum+u4BlkNo; i++)
    {
        if (u4BlkNum <= 2) { u4PgNo = 32; }
        else { u4PgNo = 1; }

        for (j=0; j<u4PgNo; j++)
        {
            i4Ret = _MSP_ReadPage(i, 0, &u4Data, (UINT32)pu1Buf);
            if (i4Ret==0)
            {
                u1Overwrite = (u4Data >> 24) & 0x0ff;
                u1Manage = (u4Data >> 16) & 0x0ff;
                u2Data = (u4Data & 0x0ffff);
                UTIL_Printf("%d,%d] Overwrite:0x%02x Management:0x%02x Logic:0x%04x\n", i, j,
                    u1Overwrite, u1Manage, u2Data);

                #if 0
                if ((pu1Buf[511] == 0xaa) || (pu1Buf[511] == 0x55)) {
                    UTIL_Printf("%d,%d] Overwrite:0x%02x Management:0x%02x Logic:0x%04x\n", i, j,
                        (u4Data >> 24), (u4Data >> 16) & 0x0ff, (u4Data & 0x0ffff));
                    return 0;
                }
                #endif
            }
            else
            {
                UTIL_Printf("%d,%d] ReadAttr failed\n", i, j);
                _MSP_MSSetCmd("reset");
                _MSP_MSSetCmd("reset");
            }
        }
    }

    return 0;
}

static void _FCI_OutputBuffer(UINT32 u4BufSz, UINT8 *pu1Buf)
{
    UINT32 u4Idx = 0;
    UINT8  *pu1TmpBuf = pu1Buf;

    for (u4Idx = 0; u4Idx < (u4BufSz); u4Idx ++)
    {
      if (u4Idx % 16 == 0)
      {
          UTIL_Printf("[0x%04X]", u4Idx);
      }

      UTIL_Printf("%02X", pu1TmpBuf[0]);

      if (u4Idx % 16 == 3 || u4Idx % 16 == 7 || u4Idx % 16 == 11)
      {
          UTIL_Printf(" ");
      }
      else if (u4Idx % 16 == 15)
      {
          UTIL_Printf("\n");
      }
      
      pu1TmpBuf ++;
    }
    
    /*
    for (u4Idx = 0; u4Idx < (u4BufSz); u4Idx += 16)
    {
      UTIL_Printf("[0x%04X]%02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X\n", u4Idx,
             pu1TmpBuf[0], pu1TmpBuf[1], pu1TmpBuf[2], pu1TmpBuf[3],
             pu1TmpBuf[4], pu1TmpBuf[5], pu1TmpBuf[6], pu1TmpBuf[7],
             pu1TmpBuf[8], pu1TmpBuf[9], pu1TmpBuf[10], pu1TmpBuf[11],
             pu1TmpBuf[12], pu1TmpBuf[13], pu1TmpBuf[14], pu1TmpBuf[15]);

      pu1TmpBuf += 16;
    }
    */
}

static INT32 _SDCLI_Compare(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4MemLen;
    BOOL fgSuccess;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sd.compare [memlen]\n");
        return 1;
    }

    u4MemLen = StrToInt(szArgv[1]); // in Byte base

    fgSuccess = fgCompareFunc(_pu4FCI_W_Buffer, _pu4FCI_R_Buffer, u4MemLen);

    if (fgSuccess)
    {
        UTIL_Printf("Compare OK.\n");
    }

    return 0;
}

static INT32 _SDCLI_SetParallel(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = -1;
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD)
    {
        i4Ret = SDC_SetSDBusWidth(TRUE);
    }
    else if (i4CardType == FCI_MMC_CARD)
    {
        i4Ret = SDC_SetMMCBusWidth(TRUE);
    }

    UTIL_Printf("Set Parallel ret:%d\n", i4Ret);

    return 0;
}

static INT32 _SDCLI_SetSerial(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = -1;
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD)
    {
        i4Ret = SDC_SetSDBusWidth(FALSE);
    }
    else if (i4CardType == FCI_MMC_CARD)
    {
        i4Ret = SDC_SetMMCBusWidth(FALSE);
    }

    UTIL_Printf("Set serial ret:%d\n", i4Ret);

    return 0;
}

VOID SDClockChange(INT32 *pClockIndex, INT32 i4ClockUpperLimit, INT32 i4ClockLowerLimit)
{    
    UINT32 u4ClockSel[] = {FCI_SD_CLOCK_SELECTION_STRING};

    INT32 i4ClockIndexLimit = 0;
    
    i4ClockIndexLimit = sizeof(u4ClockSel) / sizeof(UINT32);
        
SelectSDClock:

    (*pClockIndex)++;

    // Circular ring
    if (*pClockIndex >= i4ClockIndexLimit)
    {
        *pClockIndex = 0;
    }

    // Upper bound
    if (u4ClockSel[*pClockIndex] > i4ClockUpperLimit)
    {
        goto SelectSDClock;
    }

    // Lower bound
    if (u4ClockSel[*pClockIndex] < i4ClockLowerLimit)
    {
        goto SelectSDClock;
    }

    UTIL_Printf("\n@@ Clock setting = %d Mhz\n", u4ClockSel[*pClockIndex]);
    FCIHW_ChangeSDCClock(u4ClockSel[*pClockIndex]);
}

static INT32 _SDCLI_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT32 u4CardSize;
    UINT32 u4TestSize = 0x1000000;
    UINT32 u4SRAMSEL = 0;
    INT32 i4ClockIndex;
    INT32 i4ClockUpperLimit = -1, i4ClockLowerLimit = -1;
    INT32 i4Loop = 0, i4LoopCount = 1;

    if (i4Argc >= 3)
    {
        i4ClockUpperLimit = StrToInt(szArgv[1]);      // Mhz
        i4ClockLowerLimit = StrToInt(szArgv[2]);      // Mhz

        if (i4Argc > 3)
        {
            i4LoopCount = StrToInt(szArgv[3]);      // Count
        }
        
        i4ClockIndex = 0;
    }

    u4CardSize = SD_GetSize();

    UTIL_Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));
    UTIL_Printf("Current clock = %d Mhz\n", _u4SDClockSetting);

    for (i4Loop = 0; i4Loop<i4LoopCount; i4Loop++)
    {
        // write data to SD card.
        for (u4Addr=0; u4Addr<(u4CardSize/(32*1024)); u4Addr++)
        {
            // Setup FIFO size
            u4SRAMSEL = (u4Addr % 3)+1;  // 1, 2, 3
            FCIHW_SetSRAMSEL(u4SRAMSEL);

            UTIL_Printf("\nTest SD %d ~ %d MB, u4SRAMSEL = %d\n", u4Addr*16, (u4Addr+1)*16, u4SRAMSEL);

            _Sd_WriteFunc(u4TestSize, u4Addr*32768, (UINT32)_pu4FCI_W_Buffer, RANDOM_WRITE_PATTERN);
            _Sd_ReadFunc(u4TestSize, u4Addr*32768, (UINT32)_pu4FCI_R_Buffer);

            // Compare the result
            for(i=0; i<u4TestSize; i+=4)
            {
                if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
                {
                    UTIL_Printf("Test Failed : u4Addr = 0x%x, inconsistant at element %d \n", u4Addr, i/4);
                    break;
                }
            }

            if(i == u4TestSize)
            {
                UTIL_Printf("Test OK.\n");
            }
            else
            {
                UTIL_Printf("Test Break.\n");
                break;
            }

            // Change Clock Setting
            if (i4ClockUpperLimit != -1)
            {
                SDClockChange(&i4ClockIndex, i4ClockUpperLimit, i4ClockLowerLimit);
            }
        }
        
        UTIL_Printf("\nTest Loop %d finish.\n", i4Loop);
    }
    
    UTIL_Printf("\nTest SD card Complete.\n");

    return 0;
}

static INT32 _SDCLI_Performance(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT32 u4CardSize;
    UINT32 u4TestSize = 0x1000000;
    UINT32 u4SRAMSEL = 0;
    INT32 i4Ret, i4TestCnt = 0, i4TestLimit = 10;

    HAL_TIME_T rTimeStart = {0};
    HAL_TIME_T rTimeEnd = {0};
    HAL_TIME_T rTimeDelta = {0};
    HAL_TIME_T rWriteTimeTotal = {0};
    HAL_TIME_T rReadTimeTotal = {0};
    UINT64 u8TotalTime, u8WritePmsInt, u8WritePmsFit, u8ReadPmsInt, u8ReadPmsFit;
    UINT32 u4Lba, u4Rand;

    if (i4Argc >= 2)
    {
        i4TestLimit = StrToInt(szArgv[1]);
    }

    UTIL_Printf("perm [Test Count] - test count = %d\n", i4TestLimit);
        
    u4CardSize = SD_GetSize();

    UTIL_Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));
    UTIL_Printf("Current clock = %d Mhz\n", _u4SDClockSetting);
    
    // Set write buffer content : 00~FF, 00~FF, ... (repeat)
    // randon value
#ifdef CC_BD_FCI
#if (CONFIG_SECTION_BUILD_LINUX_KO)
    u4Rand = FCI_rand();
#else
    u4Rand = rand() % 0xffffffff;
#endif
#else
    u4Rand = rand() % 0xffffffff;
#endif

    for(i=0; i<u4TestSize; i+=4)
    {
        *((UINT32*)(((UINT32)_pu4FCI_W_Buffer)+i)) = (i+u4Rand);
    }
    
    for (u4Addr=0, i4TestCnt = 0; u4Addr<(u4CardSize/(32*1024)) && i4TestCnt<i4TestLimit; u4Addr++, i4TestCnt++)
    {
        // Setup FIFO size
        u4SRAMSEL = (u4Addr % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);

        UTIL_Printf("\nTest SD %d ~ %d MB, u4SRAMSEL = %d\n", u4Addr*16, (u4Addr+1)*16, u4SRAMSEL);

        u4Lba = u4Addr*32768;

        // Write Test first     
        HAL_GetTime(&rTimeStart);
        i4Ret = SD_Write((UINT64)(u4Lba << 9), (UINT32)_pu4FCI_W_Buffer, u4TestSize);
        HAL_GetTime(&rTimeEnd);        
        HAL_GetDeltaTime(&rTimeDelta, &rTimeStart, &rTimeEnd);

        rWriteTimeTotal.u4Seconds += rTimeDelta.u4Seconds;        
        rWriteTimeTotal.u4Micros += rTimeDelta.u4Micros;
        
        UTIL_Printf("(%d) : Write Cost Time is %d.%06d seconds, Write Buffer = 0x%x\n", i4TestCnt, rTimeDelta.u4Seconds, rTimeDelta.u4Micros, _pu4FCI_W_Buffer);

        if (i4Ret)
        {
            UTIL_Printf("SD Write  Failed\n");
            break;
        }

        // Read Test later
        HAL_GetTime(&rTimeStart);
        i4Ret = SD_Read((UINT64)(u4Lba << 9), (UINT32)_pu4FCI_R_Buffer, u4TestSize);
        HAL_GetTime(&rTimeEnd);        
        HAL_GetDeltaTime(&rTimeDelta, &rTimeStart, &rTimeEnd);

        rReadTimeTotal.u4Seconds += rTimeDelta.u4Seconds;        
        rReadTimeTotal.u4Micros += rTimeDelta.u4Micros;
        
        UTIL_Printf("(%d) : Read Cost Time is %d.%06d seconds, Read Buffer = 0x%x\n", i4TestCnt, rTimeDelta.u4Seconds, rTimeDelta.u4Micros, _pu4FCI_R_Buffer);

        if (i4Ret)
        {
            UTIL_Printf("SD Read  Failed\n");
            break;
        }        

        // Compare the result
        for(i=0; i<u4TestSize; i+=4)
        {
            if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
            {
                UTIL_Printf("Test Failed : u4Addr = 0x%x, inconsistant at element %d \n", u4Addr, i/4);
                break;
            }
        }

        if(i == u4TestSize)
        {
            UTIL_Printf("Test OK.\n");
        }
        else
        {
            UTIL_Printf("Test Break.\n");
            break;
        }
    }

    rWriteTimeTotal.u4Seconds += rWriteTimeTotal.u4Micros / 1000000;
    rWriteTimeTotal.u4Micros  = rWriteTimeTotal.u4Micros % 1000000;
    rReadTimeTotal.u4Seconds += rReadTimeTotal.u4Micros / 1000000;
    rReadTimeTotal.u4Micros  = rReadTimeTotal.u4Micros % 1000000;

    UTIL_Printf("Total Test Count : %d\n", i4TestCnt);    
    UTIL_Printf("Test Unit : %d MB\n", u4TestSize / 1024 / 1024);
    
    UTIL_Printf("Total Write Cost Time : %d.%06d seconds\n", rWriteTimeTotal.u4Seconds, rWriteTimeTotal.u4Micros);
    UTIL_Printf("Total Read Cost Time : %d.%06d seconds\n", rReadTimeTotal.u4Seconds, rReadTimeTotal.u4Micros);

    if (i4TestCnt != 0)
    {
        // Calculate Write performance  
        u8TotalTime = rWriteTimeTotal.u4Seconds*1000000;
        u8TotalTime += rWriteTimeTotal.u4Micros;    // => u8TotalTime is actual time * 1000000
    
        u8WritePmsInt = (UINT64)(u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000) / u8TotalTime;
        u8WritePmsFit = (u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000 - u8WritePmsInt * u8TotalTime);
    
        u8WritePmsFit *= 1000;
        u8WritePmsFit /= u8TotalTime; // <== performance result, Unit : Mbps
        
        UTIL_Printf("  => Write Performance : %d.%03d M bps\n", (UINT32)u8WritePmsInt, (UINT32)u8WritePmsFit);
        
        // Calculate Read performance                                                                    
        u8TotalTime = rReadTimeTotal.u4Seconds*1000000;                                                  
        u8TotalTime += rReadTimeTotal.u4Micros;    // => u8TotalTime is actual time * 1000000            
                                                                                                          
        u8ReadPmsInt = (UINT64)(u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000) / u8TotalTime;        
        u8ReadPmsFit = (u4TestSize / 1024 /1024 * 8 * i4TestCnt * 1000000 - u8ReadPmsInt * u8TotalTime);
                                                                                                          
        u8ReadPmsFit *= 1000;                                                                            
        u8ReadPmsFit /= u8TotalTime; // <== performance result, Unit : Mbps                              
                                                                                                          
        UTIL_Printf("  => Read Performance : %d.%03d M bps\n", (UINT32)u8ReadPmsInt, (UINT32)u8ReadPmsFit);
    }
    
    return 0;
}

static INT32 _SDCLI_Attach(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = SD_DetectCardRemove();

    if(i4Ret == 0)
    {
        i4Ret = SD_Insert();
    }

    if (i4Ret!=0)
    {
        UTIL_Printf("Attach SD card failed.\n");
        return 1;
    }

    UTIL_Printf("Attach SD card successfully.\n");

    return 0;
}

static INT32 _SDCLI_Detach(INT32 i4Argc, const CHAR ** szArgv)
{
    // FCI_SDCDetach(FCI_SD_CARD);
    SD_Remove();
    UTIL_Printf("Detach SDCslot device.\n");
    return 0;
}

static INT32 _SDCLI_PIOModeEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {        
        UTIL_Printf("fci.sd.pio [1:Enable; 0:Disable], Current mode : %s\n", SD_IsPIOMode() ? "PIO":"DMA");
        return 1;
    }

    fgEnable = (StrToInt(szArgv[1]) ? TRUE : FALSE);

    SD_PIOModeAccessEnable(fgEnable);
    
    UTIL_Printf("SDCslot %s mode Access.\n", fgEnable ? "PIO":"DMA");
    
    return 0;
}

static INT32 _Sd_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer)
{
    UINT32 u4MemPtr;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    u4MemPtr = (UINT32)u4MemBuffer;

    // Clear buffer content
    x_memset((VOID *)u4MemPtr, 0x00, u4MemLen);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SD_Read((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("Read Cost Time is %d.%06d seconds, Read Buffer = 0x%x\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros, u4MemPtr);

    if (i4Ret)
    {
        UTIL_Printf("SD Read failed\n");
    }
    else
    {
        UTIL_Printf("SD Read ok - %s Mode\n", SD_IsPIOMode() ? "PIO" : "DMA");

        _FCI_OutputBuffer(64, (UINT8 *)u4MemPtr);
    }

    return i4Ret;
}

static INT32 _SDCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sd.read [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Sd_ReadFunc(u4MemLen, u4Lba, (UINT32)_pu4FCI_R_Buffer);

    return 0;
}

static INT32 _SDCLI_SingleBlockRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sd.sr [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    // Enable Single Block access
    SD_SingleBlockAccessEnable(TRUE);

    _Sd_ReadFunc(u4MemLen, u4Lba, (UINT32)_pu4FCI_R_Buffer);

    // Disable Single Block access => multiple block access
    SD_SingleBlockAccessEnable(FALSE);

    return 0;
}

static INT32 _Sd_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4MemBuffer, UINT32 u4DataPattern)
{
    UINT32 u4MemPtr, i, u4Rand;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    u4MemPtr = (UINT32)u4MemBuffer;

    // randon value
#ifdef CC_BD_FCI
#if (CONFIG_SECTION_BUILD_LINUX_KO)
    u4Rand = FCI_rand();
#else
    u4Rand = rand() % 0xffffffff;
#endif
#else
    u4Rand = rand() % 0xffffffff;
#endif

    // Set the buffer content : 00~FF, 00~FF, ... (repeat)
    for(i=0; i<u4MemLen; i+=4)
    {
        if (u4DataPattern == RANDOM_WRITE_PATTERN)    // no assign data pattern, use random number to write data
        {
            *((UINT32*)(u4MemPtr+i)) = (i+u4Rand);
        }
        else
        {
            *((UINT32*)(u4MemPtr+i)) = u4DataPattern;
        }
    }

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SD_Write((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("Write Cost Time is %d.%06d seconds, Write Buffer = 0x%x\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros, u4MemPtr);

    if (i4Ret)
    {
        UTIL_Printf("SD Write  Failed\n");
    }
    else
    {
        UTIL_Printf("SD Write ok - %s Mode, u4Rand = 0x %02X %02X %02X %02X\n", SD_IsPIOMode() ? "PIO" : "DMA", (*((UINT32*)(u4MemPtr)) & 0xFF), ((*((UINT32*)(u4MemPtr))>>8) & 0xFF), ((*((UINT32*)(u4MemPtr))>>16) & 0xFF), ((*((UINT32*)(u4MemPtr))>>24) & 0xFF));
    }

    return i4Ret;
}

static INT32 _SDCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba, u4DataPattern = RANDOM_WRITE_PATTERN;

    if (i4Argc != 3)
    {
        if (i4Argc == 4)
        {
            u4DataPattern = StrToInt(szArgv[3]); // in Byte base
            UTIL_Printf("fci.sd.write Data Pattern = 0x%x\n", u4DataPattern);
        }
        else
        {
            UTIL_Printf("fci.sd.write [lba] [memlen] [data Pattern]\n");
            return 1;
        }
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base
    
    _Sd_WriteFunc(u4MemLen, u4Lba, (UINT32)_pu4FCI_W_Buffer, u4DataPattern);

    return 0;
}

static INT32 _SDCLI_SingleBlockWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sd.swrite [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    // Enable Single Block access
    SD_SingleBlockAccessEnable(TRUE);

    _Sd_WriteFunc(u4MemLen, u4Lba, (UINT32)_pu4FCI_W_Buffer, RANDOM_WRITE_PATTERN);

    // Disable Single Block access => multiple block access
    SD_SingleBlockAccessEnable(FALSE);

    return 0;
}

BOOL SDCrossSingleTest(UINT32 u4MemLen, UINT32 u4Lba, UINT32 u4WBuf, UINT32 u4RBuf, 
                                                BOOL fgWriteSingMultSel, BOOL fgReadSingMultSel, INT32 nTestOrder)
{
    BOOL fgSuccess = TRUE;
    INT32 i4Ret;

    UTIL_Printf(" [%s] - (%d)\n", __FUNCTION__, nTestOrder);

    UTIL_Printf(" [%s 0x%X 0x%X]\n", fgWriteSingMultSel ? "sw" : "w", u4Lba, u4MemLen);
     // Switch Single Block access => multiple block access
    SD_SingleBlockAccessEnable(fgWriteSingMultSel);       
    i4Ret = _Sd_WriteFunc(u4MemLen, u4Lba, u4WBuf, RANDOM_WRITE_PATTERN);

    if (i4Ret)
    {
        fgSuccess = FALSE;
    }

    if (fgSuccess)
    {
        UTIL_Printf(" [%s 0x%X 0x%X]\n", fgReadSingMultSel ? "sr" : "r", u4Lba, u4MemLen);
         // Switch Single Block access => multiple block access
        SD_SingleBlockAccessEnable(fgReadSingMultSel); 
        i4Ret = _Sd_ReadFunc(u4MemLen, u4Lba, u4RBuf);

        if (i4Ret)
        {
            fgSuccess = FALSE;
        }
    }
    
    if (fgSuccess)
    {
        fgSuccess = fgCompareFunc((UINT32*)u4WBuf, (UINT32*)u4RBuf, u4MemLen);
    }
    
    UTIL_Printf(" ==> u4Lba = 0x%08X, u4MemLen = 0x%X => %s\n", u4Lba, u4MemLen, fgSuccess ? "OK" : "NG");

    return fgSuccess;
}

BOOL SDCrossTest(BOOL fg4BitMode)
{
    UINT32  u4MemLen;
    UINT32  u4Lba;
    BOOL fgSuccess;              
    INT64 u8SDTotalLBA = SD_GetSize();    
    UINT8   *pucRBuf1 = (UINT8*)_pu4FCI_R_Buffer;
    UINT8   *pucRBuf2 = (UINT8*)((UINT32)_pu4FCI_R_Buffer + FCI_BUFFER_SIZE / 2);
    UINT8   *pucWBuf1 = (UINT8*)_pu4FCI_W_Buffer;
    UINT8   *pucWBuf2 = (UINT8*)((UINT32)_pu4FCI_W_Buffer + FCI_BUFFER_SIZE / 2);

    // (1) 
    u4Lba = (UINT32)(u8SDTotalLBA / 5 * 3);        u4MemLen = 0x1000000;
    fgSuccess = SDCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf1), ((UINT32)pucRBuf1), FALSE, FALSE, 1);

    if (!fgSuccess)        
    {
        UTIL_Printf(" SDCrossTest failed 1 (multi w, multi r)\n");
        return FALSE;
    }  
   
    // (2) 
    u4Lba = (UINT32)(u8SDTotalLBA / 3 * 2);        u4MemLen = 0x100000;
    fgSuccess = SDCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2)+u4MemLen, ((UINT32)pucRBuf2)+u4MemLen, FALSE, FALSE, 2);

    if (!fgSuccess)        
    {
        UTIL_Printf(" SDCrossTest failed 2 (multi w, multi r)\n");
        return FALSE;
    }    
    
    // (3) 
    u4Lba = (UINT32)(u8SDTotalLBA / 4);        u4MemLen = 0x8000;
    fgSuccess = SDCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf1)+u4MemLen, ((UINT32)pucRBuf1)+u4MemLen, FALSE, TRUE, 3);

    if (!fgSuccess)        
    {
        UTIL_Printf(" SDCrossTest failed 3 (multi w, single r)\n");
        return FALSE;
    }    

    // (4) 
    u4Lba = (UINT32)(u8SDTotalLBA / 3);        u4MemLen = 0x10000;
    fgSuccess = SDCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2)+u4MemLen, ((UINT32)pucRBuf2)+u4MemLen, TRUE, TRUE, 4);

    if (!fgSuccess)        
    {
        UTIL_Printf(" SDCrossTest failed 4 (single w, single r)\n");
        return FALSE;
    }    

    // (5) 
    u4Lba = (UINT32)(u8SDTotalLBA / 2);        u4MemLen = 0x40000;
    fgSuccess = SDCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf2)+u4MemLen, ((UINT32)pucRBuf2)+u4MemLen, FALSE, FALSE, 5);

    if (!fgSuccess)        
    {
        UTIL_Printf(" SDCrossTest failed 5 (multi w, multi r)\n");
        return FALSE;
    }    
    
    // (6) 
    u4Lba = 0x00;           u4MemLen = 0x1000;

    fgSuccess = SDCrossSingleTest(u4MemLen, u4Lba, ((UINT32)pucWBuf1)+u4MemLen, ((UINT32)pucRBuf1)+u4MemLen, TRUE, FALSE, 6);

    if (!fgSuccess)        
    {
        UTIL_Printf(" SDCrossTest failed 6 (single w, multi r)\n");
        return FALSE;
    }    
    
    return TRUE;
}

extern INT32 SDC_HiSpeedModeSet(BOOL fgHiSpeedModeSet);
static INT32 _SDCLI_FCIHiSpeedModeSet(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgHiSpeedModeSet;
    
    if (i4Argc == 2)
    {
      fgHiSpeedModeSet = StrToInt(szArgv[1]);
    }
    else
    {
        UTIL_Printf("himodset [1:High Speed Mode set, 0:Default Speed Mode set]\n");
        return TRUE;
    }

    SDC_HiSpeedModeSet(fgHiSpeedModeSet);
        
    return TRUE;
}

extern INT32 SDC_SelectCard(BOOL fgSelect, UINT32 u2NewRCA);
static INT32 _SDCLI_SelectCard(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgSelect = FALSE;
    UINT16 u2NewRCA = 0x00;
    UINT32 i4Ret;
    
    if (i4Argc == 2)
    {
        fgSelect = StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3)
    {
        fgSelect = StrToInt(szArgv[1]);
        u2NewRCA = (UINT16)StrToInt(szArgv[2]);
    }

    i4Ret = SDC_SelectCard(fgSelect, u2NewRCA);
    
    UTIL_Printf(" Select Card result, i4Ret = 0x%x\n", i4Ret);
        
    return TRUE;
}
  
static INT32 _SDCLI_CrossTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = -1, i, i4Count = 1;
    UINT8   *pucRBuf1 = (UINT8*)_pu4FCI_R_Buffer;
    UINT8   *pucRBuf2 = (UINT8*)((UINT32)_pu4FCI_R_Buffer + FCI_BUFFER_SIZE / 2);
    UINT8   *pucWBuf1 = (UINT8*)_pu4FCI_W_Buffer;
    UINT8   *pucWBuf2 = (UINT8*)((UINT32)_pu4FCI_W_Buffer + FCI_BUFFER_SIZE / 2);
    UINT32  u4FailedCount = 0;    
    UINT32  u4SRAMSEL = 0;
    INT32 i4CardType;
    UINT32 u4CardSize;    
    BOOL fgSuccess;

    INT32 i4ClockIndex;
    INT32 i4ClockUpperLimit = -1, i4ClockLowerLimit = -1;
    
    i4CardType = SD_GetCard();
    
    if (i4Argc == 2)
    {
      i4Count = StrToInt(szArgv[1]);
    }
   
    if (i4Argc == 4)
    {
        i4Count = StrToInt(szArgv[1]);
        i4ClockUpperLimit = StrToInt(szArgv[2]);      // Mhz
        i4ClockLowerLimit = StrToInt(szArgv[3]);      // Mhz

        i4ClockIndex = 0;
    }    
    
    UTIL_Printf("cross [Test Count] [Upper Clock] [Lower Clock] - test count = %d\n", i4Count, i4ClockUpperLimit, i4ClockLowerLimit);
    
    u4CardSize = SD_GetSize();

    UTIL_Printf("Test card size = %d MB. Bus width = %d\n", (u4CardSize/(2*1024)), SD_GetBusWidth());
    
    UTIL_Printf("[%s]\n      RBuf1 : 0x%x, RBuf2 : 0x%x\n      WBuf1 : 0x%x, WBuf2 : 0x%x\n", __FUNCTION__, pucRBuf1, pucRBuf2, pucWBuf1, pucWBuf2 );

    for (i=0; i<i4Count; i++)
    { 
        // Setup FIFO size
        u4SRAMSEL = (i % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);
                
        // DMA/PIO cross test
        SD_PIOModeAccessEnable((i%2) ? TRUE:FALSE);
    
        UTIL_Printf("# Count = %d (Total : %d), u4SRAMSEL = %d, %s Mode  :\n", i+1, i4Count, u4SRAMSEL, SD_IsPIOMode() ? "PIO":"DMA");
        UTIL_Printf("Current clock = %d Mhz\n", _u4SDClockSetting);

        if (SD_GetBusWidth() == 4)
        {            
            UTIL_Printf(" @@ 4 Bit mode (Current : %d, Total : %d) : \n", i+1, i4Count);
            
            if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD)
            {
                i4Ret = SDC_SetSDBusWidth(TRUE);
            }
            else if (i4CardType == FCI_MMC_CARD)
            {
                i4Ret = SDC_SetMMCBusWidth(TRUE);
            }

            if (i4Ret)
            {
                UTIL_Printf("Switch Bus Failed\n");
            }
            
            fgSuccess = SDCrossTest(TRUE);

            if (!fgSuccess)
            {
                u4FailedCount++;
                break;
            }
        
            if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD)
            {
                i4Ret = SDC_SetSDBusWidth(FALSE);
            }
            else if (i4CardType == FCI_MMC_CARD)
            {
                i4Ret = SDC_SetMMCBusWidth(FALSE);
            }
        }
        
        UTIL_Printf(" @@ 1 Bit mode (Current : %d, Total : %d) : \n", i+1, i4Count);
        UTIL_Printf("Current clock = %d Mhz\n", _u4SDClockSetting);
            
        fgSuccess = SDCrossTest(FALSE);

        if (!fgSuccess)
        {
            u4FailedCount++;
            break;
        }   
        
        // Change Clock Setting
        if (i4ClockUpperLimit != -1)
        {
            SDClockChange(&i4ClockIndex, i4ClockUpperLimit, i4ClockLowerLimit);
        }
    }
    
    UTIL_Printf("[%s] Total Test Count = (%d / %d), Failed Count = %d", __FUNCTION__, i, i4Count, u4FailedCount);

    return 0;
}

static INT32 _SDCLI_ChangeClock(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ClockSel;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sd.clock [Mhz], Current clock = %d Mhz\n", _u4SDClockSetting);
        FCI_SD_Clock_String;
        return 1;
    }

    u4ClockSel = StrToInt(szArgv[1]);

    UTIL_Printf("Clock setting = %d Mhz\n", u4ClockSel);

    FCIHW_ChangeSDCClock(u4ClockSel);

    return 0;
}

static INT32 _SDCLI_SetHighClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        UTIL_Printf("hiclock [xx] Mhz, Current clock = %d Mhz\n", _u4SdHighClock);
        return 1;
    }

    _u4SdHighClock = StrToInt(szArgv[1]);

    UTIL_Printf("High Clock setting = %d Mhz\n", _u4SdHighClock);

    return 0;
}

static INT32 _SDCLI_SetNormClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        UTIL_Printf("noclock [xx] Mhz, Current clock = %d Mhz\n", _u4SdNormalClock);
        return 1;
    }

    _u4SdNormalClock = StrToInt(szArgv[1]);

    UTIL_Printf("Normal Clock setting = %d Mhz\n", _u4SdNormalClock);

    return 0;
}

static INT32 _SDCLI_SetLowClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        UTIL_Printf("loclock [xx] Mhz, Current clock = %d Mhz\n", _u4SdLowClock);
        return 1;
    }

    _u4SdLowClock = StrToInt(szArgv[1]);

    UTIL_Printf("Low Clock setting = %d Mhz\n", _u4SdLowClock);

    return 0;
}

static INT32 _SDCLI_HighSpeedEnable(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc != 2)
    {
        UTIL_Printf("hispeeden [0 : disable, 1 : enable]\n");
        return 1;
    }

    _fgHighSpeedEnable = (BOOL)(StrToInt(szArgv[1]) ? TRUE : FALSE);

    UTIL_Printf("High Speed Enable = %s\n", _fgHighSpeedEnable ? "TRUE" : "FALSE");

    return 0;
}

static INT32 _SDCLI_SwitchHighSpeed(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgHighSpeed;

    if (i4Argc < 2)
    {
        UTIL_Printf("hispeed [1 : high speed, 0 : normal speed], Current Mode : %s Speed Mode\n", SD_IsHighSpeedMode() ? "High" : "Normal");
        return 1;
    }

    fgHighSpeed = StrToInt(szArgv[1]) ? TRUE : FALSE;

    SD_SwitchHighSpeed(fgHighSpeed);

    return 0;
}

static INT32 _SDCLI_FifoFullTest(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgFifoFullTest, fgHighSpeedTestBit;

    if (i4Argc < 3)
    {
        UTIL_Printf("fifotest [1/0 : Test enable/disable] [1/0 : Enable/Disable FCI high Speed Bit], Current Test Mode = %d\n", SD_IsFifoTestMode());
        return 1;
    }

    fgFifoFullTest = StrToInt(szArgv[1]) ? TRUE : FALSE;
    fgHighSpeedTestBit = StrToInt(szArgv[2]) ? TRUE : FALSE;

    SD_FifoFullTest(fgFifoFullTest, fgHighSpeedTestBit);

    return 0;
}

static INT32 _SDCLI_CmdTimeOutTest(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgCmdTimeOutTest;

    if (i4Argc < 2)
    {
        UTIL_Printf("cmdtotest [1 : Test enable, 0 : Test disable], Current Test Mode = %d\n", SD_IsCmdTimeOutTestMode());
        return 1;
    }

    fgCmdTimeOutTest = StrToInt(szArgv[1]) ? TRUE : FALSE;

    SD_CmdTimeOutTest(fgCmdTimeOutTest);

    return 0;
}
  
static INT32 _SDCLI_DataTimeOutTest(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgDataTimeOutTest;

    if (i4Argc < 2)
    {
        UTIL_Printf("datatotest [1 : Test enable, 0 : Test disable], Current Test Mode = %d\n", SD_IsDataTimeOutTestMode());
        return 1;
    }

    fgDataTimeOutTest = StrToInt(szArgv[1]) ? TRUE : FALSE;

    SD_DataTimeOutTest(fgDataTimeOutTest);

    return 0;
}

static INT32 _SDCLI_List(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD || i4CardType == FCI_MMC_CARD)
    {
        UTIL_Printf("SDC Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_SD_CARD) ? "SD" : ((i4CardType == FCI_SDHC_CARD) ? "SDHC" : "MMC"))
            ,SD_GetModel(), (INT32)(SD_GetSize() >> 11));
    }
    else if(i4CardType == FCI_SDIO_CARD)
    {
        UTIL_Printf("SDC Slot - SDIO Card \n");
    }
    else
    {
        UTIL_Printf("SDC Slot have no device\n");
    }

    return 0;
}

static INT32 _SDCLI_Config(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        UTIL_Printf("config [Multifunc No.], Current Config = %d\n", _u4SDMultiFuncCfg);
        return 1;
    }

    _u4SDMultiFuncCfg = StrToInt(szArgv[1]);

    return 0;
}

static INT32 _SDCLI_CClock(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        UTIL_Printf("cclock [1: Enable, 0: Disable], Current Setting = %d\n", _fgFCIContinueClock);
        return 1;
    }

    _fgFCIContinueClock = StrToInt(szArgv[1]);
    
    return 0;
}

#ifdef CC_CARD_DETECT_PIN
static INT32 _SDCLI_CardDetect(INT32 i4Argc, const CHAR ** szArgv)
{
    UTIL_Printf("Card Remove = %d", FCIHW_DetectCard());
    return 0;
}
#endif

#ifdef CC_FCI_SDIO_SUPPORT
// SDIO bus device
extern void WLAN_Main(BOOL fg4Bits);
extern INT32 _i4SDHighClock;
#ifdef SDIO_DVBH_SUPPORT
EXTERN INT32 DVBH_SetCmdTest(BOOL fgEnable);
EXTERN INT32 DVBH_DataTest(VOID);
EXTERN INT32 DVBH_InterruptTest(VOID);
#endif

// SDIO functions
EXTERN INT32  SDIO_IsSDIOCard(VOID);
EXTERN INT32 SDIO_EnableIO(UINT32 u4FuncID, FCI_CALLBACK pfnHISR);

#define IN
#define OUT

EXTERN INT32 STORGInit(VOID);
EXTERN INT32 SDIO_ReadMCR(UINT16 offset, UINT32 *pValue);
EXTERN INT32 SDIO_WriteMCR(UINT16 offset, UINT32 value);
EXTERN INT32 SDIO_WritePort(IN UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port);
EXTERN INT32 SDIO_ReadPort(OUT UINT8 *pucBuf, IN UINT16 u2Len, IN UINT16 u2Port);
EXTERN INT32 SDIO_SetBusWidth(BOOL fg4BitEnable);
EXTERN INT32 SDIO_AbortIO(UINT32 u4FuncID);
#ifdef CC_FCI_SDIO_SUPPORT
EXTERN INT32 SDC_DisableCD(UINT16 u2RCA);
#endif
EXTERN VOID SDIO_ShowCCCR(VOID);
EXTERN INT32 SDIO_Reset(VOID);
EXTERN INT32 SDIO_ReadCCCR(VOID);

static INT32 _SDIO_RegInit(INT32 i4Argc, const CHAR ** szArgv)
{
#ifdef CC_BD_FCI
#if !(CONFIG_SECTION_BUILD_LINUX_KO)
    INT32 i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    i4Ret = STORGInit();

    if (i4Ret != 0)
    {
        UTIL_Printf("[_SDIO_RegInit] Init storage pseudo device failed.\n");
        return 1;
    }
#endif  // #if !(CONFIG_SECTION_BUILD_LINUX_KO)
#else
    INT32 i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);
    
    i4Ret = STORGInit();

    if (i4Ret != 0)
    {
        UTIL_Printf("[_SDIO_RegInit] Init storage pseudo device failed.\n");
        return 1;
    }

#endif

    return 0;
}

static INT32 _SDIOCLI_Attach(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = SDIO_IsSDIOCard();
    if (!i4Ret)
    {
        UTIL_Printf("A SDIO Card\n");
    }
    else
    {
        UTIL_Printf("Not a SDIO Card\n");
    }

    return 0;
}

static INT32 _SDIOCLI_Detach(INT32 i4Argc, const CHAR ** szArgv)
{
    SD_Remove();
    UTIL_Printf("Detach SDCslot device.\n");

    return 0;
}

void _SDIO_Callback(INT32 i4Status)
{
    UTIL_Printf("_SDIO_Callback 0x%04X\n", i4Status);
}

static INT32 _SDIOCLI_EnableIO(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;
    UINT32 u4FuncID;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sdio.enableio [func_id]\n");
        return 1;
    }

    u4FuncID = StrToInt(szArgv[1]);

    i4Ret = SDIO_EnableIO(u4FuncID, _SDIO_Callback);

    UTIL_Printf("[_SDIO_EnableIO] 0x%04X\n", i4Ret);

    return 0;
}

static INT32 _SDIOCLI_ReadMCR(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4Offset, u4Val = 0;  // [Klocwork] UNINIT.STACK.MUST Analyze
    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sdio.readmcr [offset]\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SDIO_ReadMCR((UINT16) u4Offset, &u4Val);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("[ReadMCR] Offset 0x%04X = 0x%08X (0x%08X)\n", u4Offset, u4Val, i4Ret);
    UTIL_Printf("Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    return 0;
}

static INT32 _SDIOCLI_WriteMCR(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4Offset;
    UINT32 *pu4Val = _pu4FCI_W_Buffer;
    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sdio.writemcr [offset] [value]\n");
        return 1;
    }

    // Clear buffer content
    x_memset((VOID *)pu4Val, 0x00, 4);

    u4Offset = StrToInt(szArgv[1]);
    *pu4Val    = StrToInt(szArgv[2]);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SDIO_WriteMCR((UINT16) u4Offset, *pu4Val);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("[WriteMCR] Offset 0x%04X => 0x%08X (0x%08X)\n", u4Offset, *pu4Val, i4Ret);
    UTIL_Printf("Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    return 0;
}

static INT32 _SDIOCLI_ReadPort(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret, i;
    UINT32  u4Len, u4Port;
    UINT8   *pucBuf = (UINT8*)_pu4FCI_R_Buffer;
    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sdio.SDIO_ReadPort [Port] [Len]\n");
        return 1;
    }

    // WIFI card addr => 0x6000 readable
    u4Port = StrToInt(szArgv[1]);
    u4Len  = StrToInt(szArgv[2]);

    // Clear buffer content
    x_memset((VOID *)pucBuf, 0x00, u4Len);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SDIO_ReadPort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("[SDIO_ReadPort] Port 0x%04X, Len = 0x%04X (0x%08x)\n", u4Port, u4Len, i4Ret);
    UTIL_Printf("Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    for (i = 0; i < u4Len/8; i+=8)
    {
        UTIL_Printf("           0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
                pucBuf[i], pucBuf[i+1], pucBuf[i+2], pucBuf[i+3],
                pucBuf[i+4], pucBuf[i+5], pucBuf[i+6], pucBuf[i+7]);
    }

    return 0;
}

static INT32 _SDIOCLI_WritePort(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4Len, u4Port;
    UINT8   *pucBuf = (UINT8*)_pu4FCI_W_Buffer;
    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sdio.SDIO_WritePort [Port] [Len]\n");
        return 1;
    }

    // WIFI card addr => 0x3000 writable
    u4Port = StrToInt(szArgv[1]);
    u4Len  = StrToInt(szArgv[2]);

    // Clear buffer content
    x_memset((VOID *)pucBuf, 0x00, u4Len);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SDIO_WritePort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    UTIL_Printf("[WritePort] Port 0x%04X => 0x%04X (0x%08X)\n", u4Port, u4Len, i4Ret);
    UTIL_Printf("Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    return 0;
}

static INT32 _SDIOCLI_WpdTest(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fg4Bits;

    CLI_Parser("fci.d_l -l");

    if (i4Argc == 2)
    {
        fg4Bits = StrToInt(szArgv[1]);

        if (fg4Bits)
        {
            UTIL_Printf("SDIO_WpdTest in 4 bit mode.\n");
        }
        else
        {
            UTIL_Printf("SDIO_WpdTest in 1 bit mode.\n");
        }
    }
    else
    {
        fg4Bits = TRUE;   // Default 4 bit mode
    }

    WLAN_Main(fg4Bits);

    return 0;
}

static INT32 _SDIOCLI_ReadPMS(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = 0;
    UINT32  u4Count;
    UINT32  i, u4Len, u4Port;
    UINT8   *pucBuf = (UINT8*)_pu4FCI_R_Buffer;
    HAL_TIME_T _rTotalTime;
    UINT64 u8TotalTime, u8ReadPmsInt, u8ReadPmsFit;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sdio.rdpms [Packet Size] [Count]\n");
        return 1;
    }

    u4Len = StrToInt(szArgv[1]);
    u4Count = StrToInt(szArgv[2]);

    UTIL_Printf("Count = %d\n", u4Count);

    // Start test performance

    // Read Wifi Card port 0x6000, u4Len bytes
    u4Port = 0x6000;

    // Reset 0
    _rTotalTime.u4Seconds = 0;
    _rTotalTime.u4Micros = 0;

    for(i = 0; i<u4Count; i++)
    {
        if(i%1000 == 0)
        {
            UTIL_Printf(".");
        }

        HAL_GetTime(&rMsdTimeStart);

        i4Ret = SDIO_ReadPort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

        HAL_GetTime(&rMsdTimeEnd);
        HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

        _rTotalTime.u4Seconds += rMsdTimeDelta.u4Seconds;
        _rTotalTime.u4Micros += rMsdTimeDelta.u4Micros;

        if(i4Ret != 0)
        {
            UTIL_Printf("\nError when read performance test, failed at count = %d\n", i);
            break;
        }
    }

    UTIL_Printf("\n");

    if(_rTotalTime.u4Micros >= 1000000)
    {
        UTIL_Printf("   Original : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

        _rTotalTime.u4Seconds += _rTotalTime.u4Micros / 1000000;
        _rTotalTime.u4Micros %= 1000000;

        UTIL_Printf("   Transform : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);
    }

    UTIL_Printf("[SDIO_ReadPort] Port 0x%04X, Len = 0x%04X (iRet = 0x%08x), Count = %d.\n", u4Port, u4Len, i4Ret, i);

    UTIL_Printf("  => Total Transferred data bits = %d\n", u4Count * u4Len * 8);
    UTIL_Printf("  => Total Cost Time %d.%06d Sec\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

    // Calculate performance
    u8TotalTime = _rTotalTime.u4Seconds*1000000;
    u8TotalTime += _rTotalTime.u4Micros;    // => u8TotalTime is actual time * 1000000

    u8ReadPmsInt = u4Count * u4Len * 8 / u8TotalTime;  // integer part of performance

    u8ReadPmsFit = (u4Count * u4Len * 8 - u8ReadPmsInt * u8TotalTime);  // <== performance result, Unit : Mbps
    u8ReadPmsFit *= 1000;
    u8ReadPmsFit /= u8TotalTime; // <== performance result, Unit : Mbps

    UTIL_Printf("  => Performance : %d.%03d M bps\n", (UINT32)u8ReadPmsInt, (UINT32)u8ReadPmsFit);

    return 0;
}

static INT32 _SDIOCLI_WritePMS(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = 0;
    UINT32  u4Count;
    UINT32  i, u4Len, u4Port;
    UINT8   *pucBuf = (UINT8*)_pu4FCI_W_Buffer;
    HAL_TIME_T _rTotalTime;
    UINT64 u8TotalTime, u8ReadPmsInt, u8ReadPmsFlt;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta = {0};

    if (i4Argc != 3)
    {
        UTIL_Printf("fci.sdio.wtpms [Packet Size] [Count]\n");
        return 1;
    }

    u4Len = StrToInt(szArgv[1]);
    u4Count = StrToInt(szArgv[2]);

    UTIL_Printf("Count = %d\n", u4Count);

    // Start test performance

    // Read Wifi Card port 0x3000, u4Len bytes
    u4Port = 0x3000;

    // Reset 0
    _rTotalTime.u4Seconds = 0;
    _rTotalTime.u4Micros = 0;

    for(i = 0; i<u4Count; i++)
    {
        if(i%1000 == 0)
        {
            UTIL_Printf(".");
        }

        HAL_GetTime(&rMsdTimeStart);

        i4Ret = SDIO_WritePort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

        HAL_GetTime(&rMsdTimeEnd);
        HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

        _rTotalTime.u4Seconds += rMsdTimeDelta.u4Seconds;
        _rTotalTime.u4Micros += rMsdTimeDelta.u4Micros;

        if(i4Ret != 0)
        {
            UTIL_Printf("\nError when write performance test, failed at count = %d\n", i);
            break;
        }
    }

    UTIL_Printf("\n");

    if(_rTotalTime.u4Micros >= 1000000)
    {
        UTIL_Printf("   Original : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

        _rTotalTime.u4Seconds += _rTotalTime.u4Micros / 1000000;
        _rTotalTime.u4Micros %= 1000000;

        UTIL_Printf("   Transform : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);
    }

    UTIL_Printf("[SDIO_WritePort] Port 0x%04X, Len = 0x%04X (iRet = 0x%08x), Count = %d.\n", u4Port, u4Len, i4Ret, i);

    UTIL_Printf("  => Total Transferred data bits = %d\n", u4Count * u4Len * 8);
    UTIL_Printf("  => Total Cost Time %d.%06d Sec\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

    // Calculate performance
    u8TotalTime = _rTotalTime.u4Seconds*1000000;
    u8TotalTime += _rTotalTime.u4Micros;    // => u8TotalTime is actual time * 1000000

    u8ReadPmsInt = u4Count * u4Len * 8 / u8TotalTime;  // integer part of performance

    u8ReadPmsFlt = (u4Count * u4Len * 8 - u8ReadPmsInt * u8TotalTime);  // <== performance result, Unit : Mbps
    u8ReadPmsFlt *= 1000;
    u8ReadPmsFlt /= u8TotalTime; // <== performance result, Unit : Mbps

    UTIL_Printf("  => Performance : %d.%03d M bps\n", (UINT32)u8ReadPmsInt, (UINT32)u8ReadPmsFlt);

    return 0;
}

static INT32 _SDIOCLI_SetBusWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = -1;
    UINT32  u4Bits;

    if (i4Argc != 2)
    {
		UTIL_Printf("fci.sdio.SetBusWidth [1/4]\n");
		return 1;
    }

    u4Bits = StrToInt(szArgv[1]);

    if(u4Bits == 1)
    {
        i4Ret = SDIO_SetBusWidth(FALSE);
    }
    else if(u4Bits == 4)
    {
        i4Ret = SDIO_SetBusWidth(TRUE);
    }

    UTIL_Printf("[SDIO_SetBusWidth] %d Bits bus, i4Ret = 0x%x\n", u4Bits, i4Ret);

    return 0;
}

BOOL SDIOCrossTest(void)
{
    INT32   i4Ret = -1;
    UINT32 u4Val;
    UINT8   *pucRBuf1 = (UINT8*)_pu4FCI_R_Buffer;
    UINT8   *pucRBuf2 = (UINT8*)((UINT32)_pu4FCI_R_Buffer + FCI_BUFFER_SIZE / 2);
    UINT8   *pucWBuf1 = (UINT8*)_pu4FCI_W_Buffer;
    UINT8   *pucWBuf2 = (UINT8*)((UINT32)_pu4FCI_W_Buffer + FCI_BUFFER_SIZE / 2);
    BOOL fgSuccess = TRUE;
    
    UTIL_Printf(" [%s]\n", __FUNCTION__);

    UTIL_Printf(" [rm 0x20]");
    i4Ret = SDIO_ReadMCR((UINT16) 0x20, &u4Val);
    if (i4Ret || !(u4Val == 0x5911 || u4Val == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadMCR failed, i4Ret = 0x%x, u4Val = 0x%x\n", i4Ret, u4Val);    
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [wm 0x50 0xa5]");
    i4Ret = SDIO_WriteMCR((UINT16) 0x50, u4Val);
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WriteMCR failed, i4Ret = 0x%x, u4Val = 0x%x\n", i4Ret, u4Val);    
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }
    
    UTIL_Printf(" [rp 0x20 512]");    
    i4Ret = SDIO_ReadPort(pucRBuf1, (UINT16) 512, (UINT16) 0x20);
    if (i4Ret || !(*((UINT32*)pucRBuf1) == 0x5911 || *((UINT32*)pucRBuf1) == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadPort failed 1, i4Ret = 0x%x, *((UINT32*)pucRBuf1) = 0x%x\n", i4Ret, *((UINT32*)pucRBuf1));
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [rp 0x20 1024]");        
    i4Ret = SDIO_ReadPort(pucRBuf2, (UINT16) 1024, (UINT16) 0x20);
    if (i4Ret || !(*((UINT32*)pucRBuf2) == 0x5911 || *((UINT32*)pucRBuf2) == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadPort failed 2, i4Ret = 0x%x, *((UINT32*)pucRBuf2) = 0x%x\n", i4Ret, *((UINT32*)pucRBuf2));
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [rp 0x20 2048]"); 
    i4Ret = SDIO_ReadPort(pucRBuf1, (UINT16) 2048, (UINT16) 0x20);
    if (i4Ret || !(*((UINT32*)pucRBuf1) == 0x5911 || *((UINT32*)pucRBuf1) == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadPort failed 3, i4Ret = 0x%x, *((UINT32*)pucRBuf1) = 0x%x\n", i4Ret, *((UINT32*)pucRBuf1));
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [rp 0x20 4096]"); 
    i4Ret = SDIO_ReadPort(pucRBuf2, (UINT16) 4096, (UINT16) 0x20);
    if (i4Ret || !(*((UINT32*)pucRBuf2) == 0x5911 || *((UINT32*)pucRBuf2) == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadPort failed 4, i4Ret = 0x%x, *((UINT32*)pucRBuf2) = 0x%x\n", i4Ret, *((UINT32*)pucRBuf2));
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [rp 0x20 8192]"); 
    i4Ret = SDIO_ReadPort(pucRBuf1, (UINT16) 8192, (UINT16) 0x20);
    if (i4Ret || !(*((UINT32*)pucRBuf1) == 0x5911 || *((UINT32*)pucRBuf1) == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadPort failed 5, i4Ret = 0x%x, *((UINT32*)pucRBuf1) = 0x%x\n", i4Ret, *((UINT32*)pucRBuf1));
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [rp 0x20 16384]"); 
    i4Ret = SDIO_ReadPort(pucRBuf2, (UINT16) 16384, (UINT16) 0x20);
    if (i4Ret || !(*((UINT32*)pucRBuf2) == 0x5911 || *((UINT32*)pucRBuf2) == 0x5912))
    {
        UTIL_Printf("\n SDIO_ReadPort failed 6, i4Ret = 0x%x, *((UINT32*)pucRBuf2) = 0x%x\n", i4Ret, *((UINT32*)pucRBuf2));
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [wp 0x3000 512]"); 
    i4Ret = SDIO_WritePort(pucWBuf1, (UINT16) 512, (UINT16) 0x3000);
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WritePort failed 1, i4Ret = 0x%x\n", i4Ret);
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [wp 0x3000 1024]"); 
    i4Ret = SDIO_WritePort(pucWBuf2, (UINT16) 1024, (UINT16) 0x3000);
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WritePort failed 2, i4Ret = 0x%x\n", i4Ret);
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [wp 0x3000 2048]"); 
    i4Ret = SDIO_WritePort(pucWBuf1, (UINT16) 2048, (UINT16) 0x3000);
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WritePort failed 3, i4Ret = 0x%x\n", i4Ret);
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [wp 0x3000 4096]"); 
    i4Ret = SDIO_WritePort(pucWBuf2, (UINT16) 4096, (UINT16) 0x3000);        
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WritePort failed 4, i4Ret = 0x%x\n", i4Ret);
        fgSuccess = FALSE;
        return fgSuccess;
    }    
    else
    {
        UTIL_Printf(" => OK\n");
    }

    UTIL_Printf(" [wp 0x3000 8192]"); 
    i4Ret = SDIO_WritePort(pucWBuf1, (UINT16) 8192, (UINT16) 0x3000);
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WritePort failed 5, i4Ret = 0x%x\n", i4Ret);
        fgSuccess = FALSE;
        return fgSuccess;
    }
    else
    {
        UTIL_Printf(" => OK\n");
    }
    
    UTIL_Printf(" [wp 0x3000 16384]"); 
    i4Ret = SDIO_WritePort(pucWBuf2, (UINT16) 16384, (UINT16) 0x3000);        
    if (i4Ret)
    {
        UTIL_Printf("\n SDIO_WritePort failed 6, i4Ret = 0x%x\n", i4Ret);
        fgSuccess = FALSE;
        return fgSuccess;
    }    
    else
    {
        UTIL_Printf(" => OK\n");
    }

    return fgSuccess;
}

static INT32 _SDIOCLI_CrossTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = -1, i, i4Count = 1;
    UINT8   *pucRBuf1 = (UINT8*)_pu4FCI_R_Buffer;
    UINT8   *pucRBuf2 = (UINT8*)((UINT32)_pu4FCI_R_Buffer + FCI_BUFFER_SIZE / 2);
    UINT8   *pucWBuf1 = (UINT8*)_pu4FCI_W_Buffer;
    UINT8   *pucWBuf2 = (UINT8*)((UINT32)_pu4FCI_W_Buffer + FCI_BUFFER_SIZE / 2);
    BOOL fgSuccess;
    UINT32  u4FailedCount = 0;
    UINT32  u4SRAMSEL = 0;
    
    INT32 i4ClockIndex;
    INT32 i4ClockUpperLimit = -1, i4ClockLowerLimit = -1;
    
    if (i4Argc == 2)
    {
      i4Count = StrToInt(szArgv[1]);
    }

    if (i4Argc == 4)
    {
        i4Count = StrToInt(szArgv[1]);
        i4ClockUpperLimit = StrToInt(szArgv[2]);      // Mhz
        i4ClockLowerLimit = StrToInt(szArgv[3]);      // Mhz

        i4ClockIndex = 0;
    }
    
    UTIL_Printf("cross [Test Count] [Upper Clock] [Lower Clock] - test count = %d\n", i4Count, i4ClockUpperLimit, i4ClockLowerLimit);
    
    UTIL_Printf("[%s]\n      RBuf1 : 0x%x, RBuf2 : 0x%x\n      WBuf1 : 0x%x, WBuf2 : 0x%x\n", __FUNCTION__, pucRBuf1, pucRBuf2, pucWBuf1, pucWBuf2 );
        
    for (i=0; i<i4Count; i++)
    {
        // Setup FIFO size
        u4SRAMSEL = (i % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);
        
        UTIL_Printf("# Count = %d, u4SRAMSEL = %d :\n", i, u4SRAMSEL);
       
        i4Ret = SDIO_SetBusWidth(TRUE);
        if (i4Ret)
        {
            UTIL_Printf(" Failed to change 4 Bit bus\n");    
        }

        fgSuccess = SDIOCrossTest();

        if (!fgSuccess)
        {
            u4FailedCount++;
            break;
        }
        
        UTIL_Printf("  4 Bit mode test ... %s\n", fgSuccess ? "OK" : "NG");
        
        i4Ret = SDIO_SetBusWidth(FALSE);
        if (i4Ret)
        {
            UTIL_Printf(" Failed to change 1 Bit bus\n");    
        }

        fgSuccess = SDIOCrossTest();

        if (!fgSuccess)
        {
            u4FailedCount++;
            break;
        }        

        UTIL_Printf("  1 Bit mode test ... %s\n", fgSuccess ? "OK" : "NG");
               
        // Change Clock Setting
        if (i4ClockUpperLimit != -1)
        {
            SDClockChange(&i4ClockIndex, i4ClockUpperLimit, i4ClockLowerLimit);
        }
    }
    
    UTIL_Printf("[%s] Total Test Count = (%d / %d), Failed Count = %d", __FUNCTION__, i, i4Count, u4FailedCount);

    return 0;
}

static INT32 _SDIOCLI_ChangeClock(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ClockSel;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sdio.clock [Mhz], Current clock = %d Mhz\n", _u4SDClockSetting);
        FCI_SD_Clock_String;
        return 1;
    }

    u4ClockSel = StrToInt(szArgv[1]);

    UTIL_Printf("Clock setting = %d Mhz\n", u4ClockSel);

    FCIHW_ChangeSDCClock(u4ClockSel);

    return 0;
}

static INT32 _SDIOCLI_AbortIO(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;
    UINT32 u4FuncID;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sdio.abortio [func_id]\n");
        return 1;
    }

    u4FuncID = StrToInt(szArgv[1]);

    i4Ret = SDIO_AbortIO(u4FuncID);

    UTIL_Printf("[_SDIO_AbortIO] 0x%04X\n", i4Ret);

    return 0;
}

static INT32 _SDIOCLI_DisableCD(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4RCA;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sdio.DisableCD [RCA]\n");
        return 1;
    }

    u4RCA = StrToInt(szArgv[1]);

    i4Ret = SDC_DisableCD(u4RCA);

    UTIL_Printf("[DisableCD] (0x%04X)\n", i4Ret);

    return i4Ret;
}

static INT32 _SDIOCLI_ShowCCCR(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    SDIO_ShowCCCR();

    return 0;
}

static INT32 _SDIOCLI_ReadCCCR(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = SDIO_ReadCCCR();

    UTIL_Printf("[ReadCCCR] (0x%08X)\n", i4Ret);

    return 0;
}

static INT32 _SDIOCLI_Reset(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = SDIO_Reset();

    UTIL_Printf("[Reset] (0x%08X)\n", i4Ret);

    return i4Ret;
}

#ifdef SDIO_DVBH_SUPPORT

static INT32 _SDIOCLI_DvbhInit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    //CLI_Parser("fci.d_l -l");

    i4Ret = SDIO_IsSDIOCard();
    if ( i4Ret != 0)
    {
        UTIL_Printf("(%s):SDIO_IsSDIOCard fail.\n", __FUNCTION__);
    }

    return 0;
}

static INT32 _SDIOCLI_DvbhSetCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        UTIL_Printf("fci.sd.ds [enable].\n");
        return 1;
    }

    fgEnable = (BOOL)(StrToInt(szArgv[1]) > 0);

    DVBH_SetCmdTest(fgEnable);

    return 0;
}

static INT32 _SDIOCLI_DvbhDataTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = DVBH_DataTest();

    UNUSED(i4Ret);
    return 0;
}

static INT32 _SDIOCLI_DvbhIntTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = DVBH_InterruptTest();

    UNUSED(i4Ret);
    return 0;
}

#endif

#endif

#ifdef CC_FCI_SMXD_SUPPORT
/* SM/XD Debug/emulation functions. */
extern INT32 _SMXD_ReadBlock( UINT32 u4StBlock, UINT32 u4MemPtr, UINT32 u4NoPage );
extern INT32 _SMXD_WriteBlock( UINT32 u4StBlock, UINT32 u4MemPtr, UINT32 u4NoPage );
extern INT32 _SMXD_Reset(VOID);
extern INT32 _SMXD_StressTest(VOID);
extern INT32 _SMXD_PageUnitTest(UINT32 u4Blk, UINT32 u4Page);
extern INT32 _SMXD_EraseAll(VOID);
extern INT32 _SMXD_WriteMultiBlk(VOID);

static INT32 _SM_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr;
    UINT32 u4CardSize;

    u4CardSize = SMXD_GetSize();

    UTIL_Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));

    // generate 1mb byte pattern from 00 to ff
    // duplicate 1mb pattern into 16mb pattern @ 32mb+
    CLI_Parser("b.mord 0x01700000 1 0x100000 1 0");
    CLI_Parser("b.mc 0x01800000 0x01700000 0x100000");
    CLI_Parser("b.mc 0x01900000 0x01700000 0x200000");
    CLI_Parser("b.mc 0x01B00000 0x01700000 0x400000");
    CLI_Parser("b.mc 0x01F00000 0x01700000 0x800000");

    // write data to MS card.
    for (u4Addr=0; u4Addr<(u4CardSize/(32*1024)); u4Addr++)
    {
        UTIL_Printf("\nTest xD %d ~ %d MB.\n", u4Addr*16, (u4Addr+1)*16);

        CLI_Parser_Arg("fci.sm.w 0x01700000 0x1000000 %d", u4Addr*32768);
        CLI_Parser("b.ms 0x02700000 0 0x400000");
        CLI_Parser_Arg("fci.sm.r 0x02700000 0x1000000 %d", u4Addr*32768);
        CLI_Parser("b.mcmp 0x02700000 0x01700000 0x1000000");
    }

    UTIL_Printf("\nTest xD card successfully.\n");
    return 0;
}

static INT32 _SM_Status(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc != 1) {
        UTIL_Printf("fci.sm.status\n");
        return 1;
    }
    i4Ret = SMXD_GetStatus();
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_Reset(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc != 1) {
        UTIL_Printf("fci.sm.reset\n");
        return 1;
    }
    i4Ret = _SMXD_Reset();
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemPtr, u4MemLen, u4Lba;
    INT32 i4Ret;
    HAL_TIME_T rTime0, rTime1;

    if (i4Argc != 4) {
        UTIL_Printf("fci.sm.r [memptr] [memlen] [lba]\n");
        return 1;
    }
    u4MemPtr = StrToInt(szArgv[1]);
    u4MemLen = StrToInt(szArgv[2]);
    u4Lba = StrToInt(szArgv[3]);

    HAL_GetTime(&rTime0);

    i4Ret = SMXD_Read((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rTime1);

    UTIL_Printf("Time = %d sec.\n", (rTime1.u4Seconds - rTime0.u4Seconds));

    if (i4Ret != 0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemPtr, u4MemLen, u4Lba;
    INT32 i4Ret;
    HAL_TIME_T rTime0, rTime1;

    if (i4Argc != 4) {
        UTIL_Printf("fci.sm.w [memptr] [memlen] [lba]\n");
        return 1;
    }
    u4MemPtr = StrToInt(szArgv[1]);
    u4MemLen = StrToInt(szArgv[2]);
    u4Lba = StrToInt(szArgv[3]);

    HAL_GetTime(&rTime0);

    i4Ret = SMXD_Write((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rTime1);

    UTIL_Printf("Time = %d sec.\n", (rTime1.u4Seconds - rTime0.u4Seconds));

    if (i4Ret) {
        UTIL_Printf("SM/xd Write failed\n");
    } else {
        UTIL_Printf("SM/xd Write ok\n");
    }
    return 0;
}

static INT32 _SM_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4BlkIdx, u4BlkNum;
    INT32 i4Ret;

    if (i4Argc != 3) {
        UTIL_Printf("fci.sm.e [blkidx] [blknum]\n");
        return 1;
    }

    u4BlkIdx = StrToInt(szArgv[1]);
    u4BlkNum = StrToInt(szArgv[2]);
    i4Ret = SMXD_Erase(u4BlkIdx, u4BlkNum);
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}


static INT32 _SM_Attach(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = SMXD_Insert();
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_LoopTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc != 1) {
        UTIL_Printf("fci.sm.lt.\n");
        return 1;
    }

    i4Ret = _SMXD_StressTest();
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_EraseAll(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _SMXD_EraseAll();
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_WriteMultiBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _SMXD_WriteMultiBlk();
    if (i4Ret!=0) {
        UTIL_Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    UTIL_Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

/******************************************************************************
* FCI memory card command lists
******************************************************************************/
static CLI_EXEC_T arMSCmdTbl[] = {
    DECLARE_G_CMD(_MSCLI_Stress, stress, st, "fci.ms.st"),
    DECLARE_G_CMD(_MSCLI_Performance, perm, perm, "fci.ms.perm"),        
    DECLARE_G_CMD(_MSCLI_Attach, attach, at, "fci.ms.at"),
    DECLARE_G_CMD(_MSCLI_Detach, detach, de, "fci.ms.de"),
    DECLARE_G_CMD(_MSCLI_PIOModeEnable, pio, pio, "fci.sd.pio"),
    DECLARE_G_CMD(_MSCLI_Read, read, r, "fci.ms.r [lba] [memlen]"),
    DECLARE_G_CMD(_MSCLI_Write, write, w, "fci.ms.w [lba] [memlen]"),
    DECLARE_G_CMD(_MSCLI_Compare, compare, cp, "fci.ms.cp [memlen]"),
    DECLARE_G_CMD(_MSCLI_Detect, detect, dt, "fci.ms.dt"),
    DECLARE_G_CMD(_MSCLI_SetRWReg, setrwreg, setrwreg, "fci.ms.setrwreg"),
    DECLARE_G_CMD(_MSCLI_ReadReg, readreg, rdreg, "fci.ms.rdreg"),
    DECLARE_G_CMD(_MSCLI_WriteReg, writereg, wtreg, "fci.ms.wtreg"),
    DECLARE_G_CMD(_MSCLI_SetCmd, setcmd, setcmd, "fci.ms.setcmd [cmdstr]"),
    DECLARE_G_CMD(_MSCLI_GetInt, getint, getint, "fci.ms.getint"),
    DECLARE_G_CMD(_MSCLI_ReadAttr, rdattr, rdattr, "fci.ms.rdattr [blkno] [pgno]"),
    DECLARE_G_CMD(_MSCLI_WriteAttr, wtattr, wtattr, "fci.ms.wtattr [blkno] [pgno] [extra]"),
    DECLARE_G_CMD(_MSCLI_AtrTbl, atrtbl, atrtbl, "fci.ms.atrtbl"),
    DECLARE_G_CMD(_MSCLI_RdBlk, rdblk, rdblk, "fci.ms.rdblk"),
    DECLARE_G_CMD(_MSCLI_WrBlk, wrblk, wrblk, "fci.ms.wrblk"),
    DECLARE_G_CMD(_MSCLI_ErBlk, erblk, erblk, "fci.ms.erblk"),
    DECLARE_G_CMD(_MSCLI_BootWrite, bootwrite, bw, "fci.ms.bw"),
    DECLARE_G_CMD(_MSCLI_SetParallel, parallel, pl, "fci.ms.pl"),
    DECLARE_G_CMD(_MSCLI_SetSerial, serial, sl, "fci.ms.sl"),
    DECLARE_G_CMD(_MSCLI_CrossTest, cross, cross, "fci.ms.cross"),
    DECLARE_G_CMD(_MSCLI_ChangeClock,  clock,   clock, "fci.ms.clock"),
    DECLARE_G_CMD(_MSCLI_SetHighClock,  hiclock,   hiclock, "fci.ms.hiclock"),
    DECLARE_G_CMD(_MSCLI_SetNormClock,  noclock,   noclock, "fci.ms.noclock"),
    DECLARE_G_CMD(_MSCLI_List, list, ls, "fci.ms.ls"),
    DECLARE_G_CMD(_MSCLI_Config, config, config, "fci.ms.config"),
    DECLARE_G_CMD(_MSCLI_CClock, cclock, cclock, "fci.ms.cclock"),    
#ifdef CC_CARD_DETECT_PIN
    DECLARE_G_CMD(_MSCLI_CardDetect, carddetect, carddetect, "fci.ms.carddetect"),
#endif
    //Last CLI command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

static CLI_EXEC_T arSDCmdTbl[] = {
    DECLARE_G_CMD(_SDCLI_Stress, stress, st, "fci.sd.st"),
    DECLARE_G_CMD(_SDCLI_Performance, perm, perm, "fci.sd.perm"),
    DECLARE_G_CMD(_SDCLI_Attach, attach, at, "fci.sd.at"),
    DECLARE_G_CMD(_SDCLI_Detach, detach, de, "fci.sd.de"),
    DECLARE_G_CMD(_SDCLI_PIOModeEnable, pio, pio, "fci.sd.pio"),
    DECLARE_G_CMD(_SDCLI_Read, read, r, "fci.sd.r [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_Write, write, w, "fci.sd.w [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_SingleBlockRead, sread, sr, "fci.sd.sr [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_SingleBlockWrite, swrite, sw, "fci.sd.sw [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_Compare, compare, cp, "fci.sd.cp [memlen]"),
    DECLARE_G_CMD(_SDCLI_SetParallel, parallel, pl, "fci.sd.pl"),
    DECLARE_G_CMD(_SDCLI_SetSerial, serial, sl, "fci.sd.sl"),
    DECLARE_G_CMD(_SDCLI_CrossTest, cross, cross, "fci.sd.cross"),        
    DECLARE_G_CMD(_SDCLI_FCIHiSpeedModeSet, himodset, himodset, "fci.sd.himodset"),                
    DECLARE_G_CMD(_SDCLI_SelectCard, select, select, "fci.sd.select"),            
    DECLARE_G_CMD(_SDCLI_ChangeClock,  clock,   clock, "fci.sd.clock"),
    DECLARE_G_CMD(_SDCLI_SetHighClock,  hiclock,   hiclock, "fci.sd.hiclock"),
    DECLARE_G_CMD(_SDCLI_SetNormClock,  noclock,   noclock, "fci.sd.noclock"),
    DECLARE_G_CMD(_SDCLI_SetLowClock,  loclock,   loclock, "fci.sd.loclock"),
    DECLARE_G_CMD(_SDCLI_HighSpeedEnable,  hispeeden,   hispeeden, "fci.sd.hispeeden"),
    DECLARE_G_CMD(_SDCLI_SwitchHighSpeed, hispeed, hispeed, "fci.sd.hispeed"),
    DECLARE_G_CMD(_SDCLI_FifoFullTest, fifotest, fifotest, "fci.sd.fifotest"),        
    DECLARE_G_CMD(_SDCLI_CmdTimeOutTest, cmdtotest, cmdtotest, "fci.sd.cmdtotest"),                
    DECLARE_G_CMD(_SDCLI_DataTimeOutTest, datatotest, datatotest, "fci.sd.datatotest"),            
    DECLARE_G_CMD(_SDCLI_List, list, ls, "fci.sd.ls"),
    DECLARE_G_CMD(_SDCLI_Config, config, config, "fci.sd.config"),
    DECLARE_G_CMD(_SDCLI_CClock, cclock, cclock, "fci.sd.cclock"),

#ifdef CC_CARD_DETECT_PIN
    DECLARE_G_CMD(_SDCLI_CardDetect, carddetect, carddetect, "fci.sd.carddetect"),
#endif
    //Last CLI command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

#ifdef CC_FCI_SDIO_SUPPORT
static CLI_EXEC_T arSDIOCmdTbl[] = {
    DECLARE_G_CMD(_SDIO_RegInit,      init,     init, "fci.sdio.init"),
    DECLARE_G_CMD(_SDIOCLI_Attach,    attach,    at, "fci.sdio.at"),
    DECLARE_G_CMD(_SDIOCLI_Detach,    detach,    de, "fci.sdio.de"),
    DECLARE_G_CMD(_SDIOCLI_EnableIO,  enableio,  eio, "fci.sdio.enableio"),
    DECLARE_G_CMD(_SDIOCLI_ReadMCR,   readmcr,    rm, "fci.sdio.readmcr"),
    DECLARE_G_CMD(_SDIOCLI_WriteMCR,  writemcr,   wm, "fci.sdio.writemcr"),
    DECLARE_G_CMD(_SDIOCLI_ReadPort,  readport,   rp, "fci.sdio.SD2IO_ReadPort"),
    DECLARE_G_CMD(_SDIOCLI_WritePort, writePort,  wp, "fci.sdio.SD2IO_WritePort"),
    DECLARE_G_CMD(_SDIOCLI_WpdTest, wpdtest, wt, "fci.sdio.wpdtest"),
    DECLARE_G_CMD(_SDIOCLI_SetBusWidth, setbuswidth, sbw, "fci.sdio.setbuswidth"),
    DECLARE_G_CMD(_SDIOCLI_CrossTest, cross, cross, "fci.sdio.cross"),
    DECLARE_G_CMD(_SDIOCLI_ChangeClock,  clock,   clock, "fci.sdio.clock"),
    DECLARE_G_CMD(_SDCLI_SetHighClock,  hiclock,   hiclock, "fci.sdio.hiclock"),
    DECLARE_G_CMD(_SDCLI_SetNormClock,  noclock,   noclock, "fci.sdio.noclock"),
    DECLARE_G_CMD(_SDCLI_SetLowClock,  loclock,   loclock, "fci.sdio.loclock"),
    DECLARE_G_CMD(_SDIOCLI_ReadPMS,  rdpms,   rdpms, "fci.sdio.rdpms"),
    DECLARE_G_CMD(_SDIOCLI_WritePMS,  wtpms,   wtpms, "fci.sdio.wtpms"),

    DECLARE_G_CMD(_SDIOCLI_AbortIO,   abortio,   aio, "fci.sdio.abortio"),
    DECLARE_G_CMD(_SDIOCLI_DisableCD, disablecd, dcd, "fci.sdio.disablecd"),
    DECLARE_G_CMD(_SDIOCLI_ShowCCCR,  showcccr, cccr, "fci.sdio.showcccr"),
    DECLARE_G_CMD(_SDIOCLI_ReadCCCR,  readcccr, rcccr, "fci.sdio.readcccr"),
    DECLARE_G_CMD(_SDIOCLI_Reset,     reset,    reset, "fci.sdio.insert"),
    DECLARE_G_CMD(_SDCLI_CmdTimeOutTest, cmdtotest, cmdtotest, "fci.sdio.cmdtotest"),                
    DECLARE_G_CMD(_SDCLI_DataTimeOutTest, datatotest, datatotest, "fci.sdio.datatotest"),                
    DECLARE_G_CMD(_SDCLI_List, list, ls, "fci.sdio.ls"),
    DECLARE_G_CMD(_SDCLI_Config, config, config, "fci.sdio.config"),
    DECLARE_G_CMD(_SDCLI_CClock, cclock, cclock, "fci.sdio.cclock"),
#ifdef CC_CARD_DETECT_PIN
    DECLARE_G_CMD(_SDCLI_CardDetect, carddetect, carddetect, "fci.sdio.carddetect"),
#endif

#ifdef SDIO_DVBH_SUPPORT
    DECLARE_G_CMD(_SDIOCLI_DvbhInit, dvbhinit, di, "fci.sd.di"),
    DECLARE_G_CMD(_SDIOCLI_DvbhSetCmd, dvbhsetcmd, ds, "fci.sd.ds"),
    DECLARE_G_CMD(_SDIOCLI_DvbhDataTest, dvbhdatatest, dt, "fci.sd.dt"),
    DECLARE_G_CMD(_SDIOCLI_DvbhIntTest, dvbhInttest, dit, "fci.sd.dit"),
#endif
    //Last CLI command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};
#endif

#ifdef CC_FCI_SMXD_SUPPORT
CLI_EXEC_T arSMCmdTbl[] = {
    DECLARE_CMD(_SM_Stress, stresstest, st, "fci.sm.st"),
    DECLARE_CMD(_SM_Attach, attach, at, "fci.sm.at"),
    DECLARE_CMD(_SM_LoopTest, looptest, lt, "fci.sm.lt"),
    DECLARE_CMD(_SM_EraseAll, eraseall, ea, "fci.sm.ea"),
    DECLARE_CMD(_SM_WriteMultiBlk, writemultiblock, wm, "fci.sm.wm"),
    DECLARE_CMD(_SM_Read, read, r, "fci.sm.r [memptr] [memlen] [lba]"),
    DECLARE_CMD(_SM_Write, write, w, "fci.sm.w [memptr] [memlen] [lba]"),
    DECLARE_CMD(_SM_Erase, erase, e, "fci.sm.e [blkidx] [blknum]"),
    DECLARE_CMD(_SM_Status, status, status, "fci.sm.status"),
    DECLARE_CMD(_SM_Reset, reset, reset, "fci.sm.reset"),

    DECLARE_END_ITEM(),
};
#endif // #ifdef CC_FCI_SMXD_SUPPORT

/******************************************************************************
* FCI S/W IP
******************************************************************************/

/* Declare the debug on/off/level and RegTest functions */
CLIMOD_DEBUG_FUNCTIONS(FCI)
/*CLIMOD_REGTST_FUNCTIONS(FCI)*/
/* End of Declare */

/******************************************************************************
* FCI function list
******************************************************************************/
#endif // #ifdef CC_FCI_ENABLE

/******************************************************************************
* FCI function list
******************************************************************************/
static CLI_EXEC_T arFciCmdTbl[] = {

#ifdef CC_FCI_ENABLE
    DECLARE_G_CMD(_FCIInitBuffer, initbuf, initbuf, "fci.initbuf"),
    DECLARE_G_CMD(_FCIInit, init, init, "fci.init"),
    DECLARE_G_CMD(_FCIStop, stop, stop, "fci.stop"),
    DECLARE_G_CMD(_FCIList, list, ls, "fci.ls"),
    DECLARE_G_CMD(_FCIPolling, polling, polling, "fci.polling"),
    DECLARE_CMD(_FCIPowerOn, pwron, pwron, "fci.pwron"),
    DECLARE_CMD(_FCIPowerOff, pwroff, pwroff, "fci.pwroff"),

#ifdef FCI_DIAG_TEST
    DECLARE_G_CMD(_FCIDiag, fcidiag, fcidiag, "fci.fcidiag"),
#endif

#ifdef CC_BD_FCI
    CLIMOD_DEBUG_CLIENTRY_EX(FCI, CLI_GUEST),
#else  // CC_DTV_FCI
    CLIMOD_DEBUG_CLIENTRY(FCI),
#endif

    // MS/MSPro/SD/MMC sub commands.

    {"MS",   "ms",  NULL, arMSCmdTbl,  "MS commands",   CLI_GUEST},
    {"SD",   "sd",  NULL, arSDCmdTbl,  "SD commands",   CLI_GUEST},

#ifdef CC_FCI_SMXD_SUPPORT
    {"SM",   "sm",  NULL, arSMCmdTbl,  "SM commands",   CLI_GUEST},
#endif

#ifdef CC_FCI_SDIO_SUPPORT
    {"SDIO",   "sdio",  NULL, arSDIOCmdTbl,  "SDIO commands",   CLI_GUEST},
#endif

#endif // #ifdef CC_FCI_ENABLE

    //Last CLI command record, NULL

#ifdef CC_BD_FCI
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
#else  // CC_DTV_FCI
    DECLARE_END_ITEM(),
#endif
};

/********************************************************/

#ifdef CC_BD_FCI
static CLI_EXEC_T _rFciModCmdTbl =
{
    "fci",
    NULL,
    NULL,
    arFciCmdTbl,
    "Fci command",
    CLI_GUEST
};
#else  // CC_DTV_FCI
CLI_MAIN_COMMAND_ITEM(Fci)
{
    "fci",
    NULL,
    NULL,
    arFciCmdTbl,
    "Fci command",
    CLI_SUPERVISOR
};
#endif
/******************************************************************************
* Global Function
******************************************************************************/

#ifdef CC_BD_FCI
CLI_EXEC_T* GetFciCmdTbl(void)
{
    return &_rFciModCmdTbl;
}
#endif
