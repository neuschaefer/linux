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
#include "x_lint.h"

LINT_SAVE_AND_DISABLE

#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_os.h"
#include "x_drv_cli.h"
#define DEFINE_IS_LOG   CLI_IsLog
#include "x_debug.h"
#include "x_util.h"
#include "x_timer.h"

/********************************************************/

#define CLIFCI_Parser(s)    do {    \
        Printf(s "\n");             \
        CLI_Parser(s);              \
    } while (0)

#include "x_rand.h"
#include "fci_if.h"
#ifdef CC_FCI_ENABLE

typedef VOID (*FCI_CALLBACK)(INT32 fgStatus);

//#define FCI_DIAG_TEST
//#define CC_NO_CARD_DETECT_PIN
// support SDIO CLI command..
#define FCI_SDIO_SUPPORT
//#define SDIO_DVBH_SUPPORT


#define FCI_BUFFER_SIZE 16*1024*1024

static UINT32 *_pu4FCI_R_Buffer = NULL;
static UINT32 *_pu4FCI_W_Buffer = NULL;

extern UINT32 _u4SDMultiFuncCfg;
extern UINT32 _u4MSultiFuncCfg;
#ifndef CC_NO_CARD_DETECT_PIN
extern INT32 FCIHW_DetectCard(VOID);
#endif
extern UINT32 _u4SDClockSetting;
extern UINT32 _u4MSClockSetting;

extern UINT32 _u4SdHighClock;
extern UINT32 _u4SdNormalClock;
extern UINT32 _u4SdLowClock;
extern BOOL  _fgHighSpeedEnable;

EXTERN VOID FCIHW_ChangeSDCClock(INT32 i4ClkMHz);
EXTERN VOID FCIHW_ChangeMSPClock(INT32 i4ClkMHz);

#define ALIGN_ADDR(addr, align) \
		(((align) == 0) ? (UINT32*)addr : (UINT32*)(((UINT32)addr + align - 1) & ~(align - 1)))

static INT32 _FCIInitBuffer(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    if(_pu4FCI_R_Buffer == NULL)
    {
        // _pu4FCI_R_Buffer = (UINT32*) x_mem_alloc(FCI_BUFFER_SIZE+4);
        _pu4FCI_R_Buffer = (UINT32*) 0x01700000;

        ASSERT(_pu4FCI_R_Buffer);

        // Get Aligned address
        _pu4FCI_R_Buffer = ALIGN_ADDR(_pu4FCI_R_Buffer,4);

        for(i=0; i<FCI_BUFFER_SIZE/sizeof(UINT32);i++)
        {
            _pu4FCI_R_Buffer[i] = i;
        }
    }

    if(_pu4FCI_W_Buffer == NULL)
    {
        // _pu4FCI_W_Buffer = (UINT32*) x_mem_alloc(FCI_BUFFER_SIZE+4);
        _pu4FCI_W_Buffer = (UINT32*) 0x02700000;

        ASSERT(_pu4FCI_W_Buffer);

        // Get Aligned address
        _pu4FCI_W_Buffer = ALIGN_ADDR(_pu4FCI_W_Buffer,4);

        for(i=0; i<FCI_BUFFER_SIZE/sizeof(UINT32);i++)
        {
            _pu4FCI_W_Buffer[i] = i;
        }
    }

    Printf("\n_pu4FCI_R_Buffer = 0x%X\n", _pu4FCI_R_Buffer);
    Printf("_pu4FCI_W_Buffer = 0x%X\n", _pu4FCI_W_Buffer);

    Printf("FCI buffer initialized.\n");

    return 0;
}

static INT32 _FCIInit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    CLI_Parser("d_l 5");

    i4Ret = FCI_Init();

    _FCIInitBuffer(i4Argc, szArgv);

    Printf("FCI attached device, i4Ret = 0x%X.\n", i4Ret);

    return 0;
}

static INT32 _FCIStop(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = FCI_Stop();

    Printf("FCI Stop, i4Ret = 0x%X.\n", i4Ret);

    return 0;
}

static INT32 _FCIList(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD || i4CardType == FCI_MMC_CARD)
    {
        Printf("SDC Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_SD_CARD) ? "SD" : "MMC")
            ,SD_GetModel(), (INT32)(SD_GetSize() >> 11));
    }
    else if(i4CardType == FCI_SDIO_CARD)
    {
        Printf("SDC Slot - SDIO Card \n");
    }
    else
    {
        Printf("SDC Slot have no device\n");
    }

    i4CardType = MS_GetCard();
    if (i4CardType != FCI_MS_CARD && i4CardType != FCI_MSPRO_CARD)
    {
        Printf("MSP Slot have no device\n");
    }
    else
    {
        Printf("MSP Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_MS_CARD) ? "MS" : "MSPRO")
            ,MS_GetModel(), (INT32)(MS_GetSize() >> 11));
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
static INT32 _FCIDiag(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = FCI_Diag();
    Printf( "\n==================\n"
            " FCI Diag %s!"
            "\n==================\n", ((i4Ret==0) ? "Success" : "Fail"));
    return i4Ret;
}
#endif

/******************************************************************************
* Memory card specific functions
******************************************************************************/
static INT32 _Ms_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba);
static INT32 _Ms_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba);
static INT32 _Sd_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba);
static INT32 _Sd_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba);
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
extern VOID CLKSRC_ChangeMSPClock(INT32 i4ClkMHz);
extern INT32 SDC_SetMMCBusWidth(BOOL fg4BitEnable);
extern INT32 SDC_SetMMCBusWidth(BOOL fg4BitEnable);
extern VOID SD_SingleBlockAccessEnable(BOOL fgEnable);
EXTERN VOID CLKSRC_ChangeSDCClock(INT32 i4ClkMHz);
extern INT32 SDC_SetSDBusWidth(BOOL fg4BitEnable);
    
static INT32 _MSCLI_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT32 u4CardSize;
    UINT32 u4TestSize = 0x1000000;
    UINT32 u4SRAMSEL = 0;
    INT32 u4ClockUpperLimit = -1, u4ClockLowerLimit = -1;
    INT32 i4ClockIndex, i4ClockIndexLimit;
    
    UINT32 u4ClockSel[] = {37,33,27,25,18,10,3,2};

    if (i4Argc >= 3)
    {
        u4ClockUpperLimit = StrToInt(szArgv[1]);      // Mhz
        u4ClockLowerLimit = StrToInt(szArgv[2]);      // Mhz

        i4ClockIndex = 0;
        i4ClockIndexLimit = sizeof(u4ClockSel) / sizeof(UINT32);
    }

    u4CardSize = MS_GetSize();

    Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));
    Printf("Current clock = %d Mhz\n", _u4MSClockSetting);

    // write data to MS card.
    for (u4Addr=0; u4Addr<(u4CardSize/(32*1024)); u4Addr++)
    {
        // Setup FIFO size
        u4SRAMSEL = (u4Addr % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);

        Printf("\nTest MS %d ~ %d MB, u4SRAMSEL = %d.\n", u4Addr*16, (u4Addr+1)*16, u4SRAMSEL);

        _Ms_WriteFunc(u4TestSize, u4Addr*32768);
        _Ms_ReadFunc(u4TestSize, u4Addr*32768);

        // Compare the result
        for(i=0; i<u4TestSize; i+=4)
        {
            if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
            {
                Printf("Test Failed : u4Addr = 0x%x, inconsistant at element %d \n", u4Addr, i/4);
                break;
            }
        }

        if(i == u4TestSize)
        {
            Printf("Test OK.\n");
        }
        
        // Change Clock Setting
        if (u4ClockUpperLimit != -1)
        {            
SelectMSClock:            

            i4ClockIndex++;

            // Circular ring
            if (i4ClockIndex >= i4ClockIndexLimit)
            {
                i4ClockIndex = 0;
            }

            // Upper bound
            if (u4ClockSel[i4ClockIndex] > u4ClockUpperLimit)
            {
                goto SelectMSClock;
            }

            // Lower bound
            if (u4ClockSel[i4ClockIndex] < u4ClockLowerLimit)
            {
                goto SelectMSClock;
            }
            
            Printf("\n@@ Clock setting = %d Mhz\n", u4ClockSel[i4ClockIndex]);
            FCIHW_ChangeMSPClock(u4ClockSel[i4ClockIndex]);
        }
    }

    Printf("\nTest MS card Complete.\n");

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
        Printf("Attach MS card failed.\n");
        return 1;
    }

    Printf("Attach MS card successfully.\n");
    return 0;
}

static INT32 _MSCLI_Detach(INT32 i4Argc, const CHAR ** szArgv)
{
    MS_Remove();
    Printf("Detach MSPslot device.\n");
    return 0;
}

static INT32 _Ms_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba)
{
    UINT32 u4MemPtr;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta;

    u4MemPtr = (UINT32)_pu4FCI_R_Buffer;

    // Clear buffer content
    x_memset((VOID *)u4MemPtr, 0x00, u4MemLen);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = MS_Read((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    Printf("Read Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    if (i4Ret)
    {
        Printf("MS Read failed\n");
    }
    else
    {
        Printf("MS Read ok\n");

        _FCI_OutputBuffer(64, (UINT8 *)u4MemPtr);
    }

    return i4Ret;
}

static INT32 _MSCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        Printf("fci.ms.read [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Ms_ReadFunc(u4MemLen, u4Lba);

    return 0;
}

static INT32 _Ms_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba)
{
    UINT32 u4MemPtr, i, u4Rand;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta;

    u4MemPtr = (UINT32)_pu4FCI_W_Buffer;

    // randon value
    u4Rand = rand() % 0xffffffff;

    // Set the buffer content : 00~FF, 00~FF, ... (repeat)
    for(i=0; i<u4MemLen; i+=4)
    {
        *((UINT32*)(u4MemPtr+i)) = (i+u4Rand) % 0x100000000;
    }

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = MS_Write((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    Printf("Write Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    if (i4Ret)
    {
        Printf("MS Write  Failed\n");
    }
    else
    {
        Printf("MS Write ok, u4Rand = 0x %02X %02X %02X %02X\n", (u4Rand & 0xFF), ((u4Rand>>8) & 0xFF), ((u4Rand>>16) & 0xFF), ((u4Rand>>24) & 0xFF));
    }

    return i4Ret;
}

static INT32 _MSCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        Printf("fci.ms.write [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Ms_WriteFunc(u4MemLen, u4Lba);

    return 0;
}

static INT32 _MSCLI_Compare(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4MemLen, i;

    if (i4Argc != 2)
    {
        Printf("fci.ms.compare [memlen]\n");
        return 1;
    }

    u4MemLen = StrToInt(szArgv[1]); // in Byte base

    // Compare the result
    for(i=0; i<u4MemLen; i+=4)
    {
        if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
        {
            Printf("Test Failed : inconsistant at element %d \n", i/4);
            break;
        }
    }

    if(i == u4MemLen)
    {
        Printf("Compare OK.\n");
    }

    return 0;
}

static INT32 _MSCLI_ChangeClock(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ClockSel;

    if (i4Argc != 2)
    {
        Printf("fci.ms.clock [60/54/50/45/37/33/27/25/18/10/3/2] Mhz, Current clock = %d Mhz\n", _u4MSClockSetting);  
        return 1;
    }

    u4ClockSel = StrToInt(szArgv[1]);
    
    Printf("Clock setting = %d Mhz\n", u4ClockSel);

    FCIHW_ChangeMSPClock(u4ClockSel);

    return 0;
}

static INT32 _MSCLI_List(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = MS_GetCard();
    if (i4CardType != FCI_MS_CARD && i4CardType != FCI_MSPRO_CARD)
    {
        Printf("MSP Slot have no device\n");
    }
    else
    {
        Printf("MSP Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_MS_CARD) ? "MS" : "MSPRO")
            ,MS_GetModel(), (INT32)(MS_GetSize() >> 11));
    }

    return 0;
}

static INT32 _MSCLI_Config(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        Printf("config [Multifunc No.], Current Config = %d\n", _u4MSultiFuncCfg);
        return 1;
    }

    _u4MSultiFuncCfg = StrToInt(szArgv[1]);
    
    return 0;
}

static INT32 _MSCLI_CardDetect(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("Card Remove = %d\n", FCIHW_DetectCard());
    return 0;
}

static INT32 _MSCLI_SetSerial(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _MSP_SetSerial();
    Printf("Set serial ret:%d\n", i4Ret);
    return 0;
}

static INT32 _MSCLI_SetParallel(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _MSP_SetParallel();
    Printf("Set parallel ret:%d\n", i4Ret);
    return 0;
}

static INT32 _MSCLI_RdBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4BlkNo;

    if (i4Argc < 2)
    {
        Printf("%s [blkno]\n", szArgv[0]);
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
        Printf("%s [blkno]\n", szArgv[0]);
        return 1;
    }

    i4BlkNo = StrToInt(szArgv[1]);

    if (i4BlkNo==0 || i4BlkNo==1)
    {
        Printf("No 0 or 1 => don't write !!\n");
        return 0;
    }
    return _MSP_WriteBlock(i4BlkNo, (UINT32)_pu4FCI_W_Buffer, (UINT32)_pu4FCI_R_Buffer);
}

static INT32 _MSCLI_ErBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4BlkNo;

    if (i4Argc < 2) {
        Printf("%s [blkno]\n", szArgv[0]);
        return 1;
    }

    i4BlkNo = StrToInt(szArgv[1]);

    return _MSP_EraseBlock(i4BlkNo);
}

static INT32 _MSCLI_BootWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _MSP_BootWrite();
    Printf("BootWrite return %d\n", i4Ret);
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
            Printf("%s [read/write/end/erase/stop/sleep/clear/reset]\n", szArgv[0]);
            return 1;
        }

        return _MSP_MSSetCmd((CHAR *) szArgv[1]);
    }
    else if (i4CardType == FCI_MSPRO_CARD)
    {
        if (i4Argc < 2)
        {
            Printf("%s [read/write/erase/sleep/stop/clear/getibd]\n", szArgv[0]);
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
            Printf("%s [blkno] [pgno]\n", szArgv[0]);
            return 1;
        }
        u4Block = StrToInt(szArgv[1]);
        u4PgNo = StrToInt(szArgv[2]);
        i4Ret = _MSP_ReadPage(u4Block, u4PgNo, &u4Data, (UINT32) pu1Buf);
        u1Overwrite = (u4Data >> 24) & 0x0ff;
        u1Manage = (u4Data >> 16) & 0x0ff;
        u2Data = (u4Data & 0x0ffff);

        Printf("Overwrite:0x%02x Management:0x%02x Logic:0x%04x, u4Data = 0x%x\n",
            u1Overwrite, u1Manage, u2Data, u4Data);

        if (i4Ret!=0)
        {
            Printf("ReadAttr failed\n");
            _MSP_MSSetCmd("reset");
            _MSP_MSSetCmd("reset");
        }
    }
    else if (i4CardType == FCI_MSPRO_CARD)
    {
        i4Ret = _MSP_MsproReadAttribute((UINT8 *)_pu4FCI_R_Buffer);

        if (i4Ret == 0)
        {
            Printf("MSP ReadAttr OK.\n");
            _FCI_OutputBuffer(64, (UINT8 *)_pu4FCI_R_Buffer);
        }
        else
        {
            Printf("MSP ReadAttr Failed.\n");
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
        Printf("%s [blkno] [pgno] [extra]\n", szArgv[0]);
        return 1;
    }

    u4Block = StrToInt(szArgv[1]);
    u4PgNo = StrToInt(szArgv[2]);
    u4Extra = StrToInt(szArgv[3]);

    i4Ret = _MSP_WritePage(u4Block, u4PgNo, u4Extra, (UINT32) _pu4FCI_W_Buffer);
    if (i4Ret!=0)
    {
        Printf("WriteAttr failed\n");
        _MSP_MSSetCmd("reset");
        _MSP_MSSetCmd("reset");
        return 1;
    }

    i4Ret = _MSP_ReadPage(u4Block, u4PgNo, &u4Data, (UINT32) _pu4FCI_R_Buffer);

    if (i4Ret!=0)
    {
        Printf("WriteAttr read back failed\n");
        _MSP_MSSetCmd("reset");
        _MSP_MSSetCmd("reset");
        return 1;
    }

    Printf("WrExtra:0x%08x RdExtra:0x%08x\n", u4Extra, u4Data);
    Printf("Wr----0x%08x\n", (UINT32)_pu4FCI_W_Buffer);
    _FCI_OutputBuffer(64, (UINT8 *)_pu4FCI_W_Buffer);

    Printf("Rd----0x%08x\n", (UINT32)_pu4FCI_R_Buffer);
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

    Printf("BlkNo is %d, BlkNum is %d\n", u4BlkNo, u4BlkNum);
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
                Printf("%d,%d] Overwrite:0x%02x Management:0x%02x Logic:0x%04x\n", i, j,
                    u1Overwrite, u1Manage, u2Data);

                #if 0
                if ((pu1Buf[511] == 0xaa) || (pu1Buf[511] == 0x55)) {
                    Printf("%d,%d] Overwrite:0x%02x Management:0x%02x Logic:0x%04x\n", i, j,
                        (u4Data >> 24), (u4Data >> 16) & 0x0ff, (u4Data & 0x0ffff));
                    return 0;
                }
                #endif
            }
            else
            {
                Printf("%d,%d] ReadAttr failed\n", i, j);
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

    for (u4Idx = 0; u4Idx < (u4BufSz + 15); u4Idx += 16)
    {
      printf("[0x%04X]%02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X%02X%02X\n", u4Idx,
             pu1TmpBuf[0], pu1TmpBuf[1], pu1TmpBuf[2], pu1TmpBuf[3],
             pu1TmpBuf[4], pu1TmpBuf[5], pu1TmpBuf[6], pu1TmpBuf[7],
             pu1TmpBuf[8], pu1TmpBuf[9], pu1TmpBuf[10], pu1TmpBuf[11],
             pu1TmpBuf[12], pu1TmpBuf[13], pu1TmpBuf[14], pu1TmpBuf[15]);

      pu1TmpBuf += 16;
    }
}

static INT32 _SDCLI_Compare(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 u4MemLen, i;

    if (i4Argc != 2)
    {
        Printf("fci.sd.compare [memlen]\n");
        return 1;
    }

    u4MemLen = StrToInt(szArgv[1]); // in Byte base

    // Compare the result
    for(i=0; i<u4MemLen; i+=4)
    {
        if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
        {
            Printf("Test Failed : inconsistant at element %d \n", i/4);
            break;
        }
    }

    if(i == u4MemLen)
    {
        Printf("Compare OK.\n");
    }

    return 0;
}

static INT32 _SDCLI_SetParallel(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = -1;
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD)
    {
        i4Ret = SDC_SetSDBusWidth(TRUE);
    }
    else if (i4CardType == FCI_MMC_CARD)
    {
        i4Ret = SDC_SetMMCBusWidth(TRUE);
    }

    Printf("Set Parallel ret:%d\n", i4Ret);

    return 0;
}

static INT32 _SDCLI_SetSerial(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret = -1;
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD)
    {
        i4Ret = SDC_SetSDBusWidth(FALSE);
    }
    else if (i4CardType == FCI_MMC_CARD)
    {
        i4Ret = SDC_SetMMCBusWidth(FALSE);
    }

    Printf("Set serial ret:%d\n", i4Ret);

    return 0;
}

static INT32 _SDCLI_Stress(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Addr, i;
    UINT32 u4CardSize;
    UINT32 u4TestSize = 0x1000000;
    UINT32 u4SRAMSEL = 0;
    INT32 u4ClockUpperLimit = -1, u4ClockLowerLimit = -1;
    INT32 i4ClockIndex, i4ClockIndexLimit;

    UINT32 u4ClockSel[] = {54,45,40,33,30,27,25,13,9,6,1,0};

    if (i4Argc >= 3)
    {
        u4ClockUpperLimit = StrToInt(szArgv[1]);      // Mhz
        u4ClockLowerLimit = StrToInt(szArgv[2]);      // Mhz

        i4ClockIndex = 0;
        i4ClockIndexLimit = sizeof(u4ClockSel) / sizeof(UINT32);
    }

    u4CardSize = SD_GetSize();

    Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));
    Printf("Current clock = %d Mhz\n", _u4SDClockSetting);

    // write data to SD card.
    for (u4Addr=0; u4Addr<(u4CardSize/(32*1024)); u4Addr++)
    {
        // Setup FIFO size
        u4SRAMSEL = (u4Addr % 3)+1;  // 1, 2, 3
        FCIHW_SetSRAMSEL(u4SRAMSEL);

        Printf("\nTest SD %d ~ %d MB, u4SRAMSEL = %d.\n", u4Addr*16, (u4Addr+1)*16, u4SRAMSEL);

        _Sd_WriteFunc(u4TestSize, u4Addr*32768);
        _Sd_ReadFunc(u4TestSize, u4Addr*32768);

        // Compare the result
        for(i=0; i<u4TestSize; i+=4)
        {
            if(_pu4FCI_W_Buffer[i/4] != _pu4FCI_R_Buffer[i/4])
            {
                Printf("Test Failed : u4Addr = 0x%x, inconsistant at element %d \n", u4Addr, i/4);
                break;
            }
        }

        if(i == u4TestSize)
        {
            Printf("Test OK.\n");
        }
        else
        {
            Printf("Test Break.\n");
            break;
        }
     
        // Change Clock Setting
        if (u4ClockUpperLimit != -1)
        {            
SelectSDClock:            

            i4ClockIndex++;

            // Circular ring
            if (i4ClockIndex >= i4ClockIndexLimit)
            {
                i4ClockIndex = 0;
            }

            // Upper bound
            if (u4ClockSel[i4ClockIndex] > u4ClockUpperLimit)
            {
                goto SelectSDClock;
            }

            // Lower bound
            if (u4ClockSel[i4ClockIndex] < u4ClockLowerLimit)
            {
                goto SelectSDClock;
            }
            
            Printf("\n@@ Clock setting = %d Mhz\n", u4ClockSel[i4ClockIndex]);
            FCIHW_ChangeSDCClock(u4ClockSel[i4ClockIndex]);
        }            
    }

    Printf("\nTest SD card Complete.\n");

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
        Printf("Attach SD card failed.\n");
        return 1;
    }

    Printf("Attach SD card successfully.\n");

    return 0;
}

static INT32 _SDCLI_Detach(INT32 i4Argc, const CHAR ** szArgv)
{
    // FCI_SDCDetach(FCI_SD_CARD);
    SD_Remove();
    Printf("Detach SDCslot device.\n");
    return 0;
}

static INT32 _Sd_ReadFunc(UINT32 u4MemLen, UINT32 u4Lba)
{
    UINT32 u4MemPtr;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta;

    u4MemPtr = (UINT32)_pu4FCI_R_Buffer;

    // Clear buffer content
    x_memset((VOID *)u4MemPtr, 0x00, u4MemLen);

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SD_Read((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    Printf("Read Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    if (i4Ret)
    {
        Printf("SD Read failed\n");
    }
    else
    {
        Printf("SD Read ok\n");

        _FCI_OutputBuffer(64, (UINT8 *)u4MemPtr);
    }

    return i4Ret;
}

static INT32 _SDCLI_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        Printf("fci.sd.read [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Sd_ReadFunc(u4MemLen, u4Lba);

    return 0;
}

static INT32 _SDCLI_SingleBlockRead(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        Printf("fci.sd.sr [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    // Enable Single Block access
    SD_SingleBlockAccessEnable(TRUE);

    _Sd_ReadFunc(u4MemLen, u4Lba);

    // Disable Single Block access => multiple block access
    SD_SingleBlockAccessEnable(FALSE);

    return 0;
}

static INT32 _Sd_WriteFunc(UINT32 u4MemLen, UINT32 u4Lba)
{
    UINT32 u4MemPtr, i, u4Rand;
    INT32 i4Ret;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta;

    u4MemPtr = (UINT32)_pu4FCI_W_Buffer;

    // randon value
    u4Rand = rand() % 0xffffffff;

    // Set the buffer content : 00~FF, 00~FF, ... (repeat)
    for(i=0; i<u4MemLen; i+=4)
    {
        *((UINT32*)(u4MemPtr+i)) = (i+u4Rand) % 0x100000000;
    }

    HAL_GetTime(&rMsdTimeStart);

    i4Ret = SD_Write((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rMsdTimeEnd);
    HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

    Printf("Write Cost Time is %d.%06d seconds\n", rMsdTimeDelta.u4Seconds, rMsdTimeDelta.u4Micros);

    if (i4Ret)
    {
        Printf("SD Write  Failed\n");
    }
    else
    {
        Printf("SD Write ok, u4Rand = 0x %02X %02X %02X %02X\n", (u4Rand & 0xFF), ((u4Rand>>8) & 0xFF), ((u4Rand>>16) & 0xFF), ((u4Rand>>24) & 0xFF));
    }

    return i4Ret;
}

static INT32 _SDCLI_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        Printf("fci.sd.write [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    _Sd_WriteFunc(u4MemLen, u4Lba);

    return 0;
}

static INT32 _SDCLI_SingleBlockWrite(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemLen, u4Lba;

    if (i4Argc != 3)
    {
        Printf("fci.sd.swrite [lba] [memlen]\n");
        return 1;
    }

    u4Lba = StrToInt(szArgv[1]);      // in Sector base
    u4MemLen = StrToInt(szArgv[2]); // in Byte base

    // Enable Single Block access
    SD_SingleBlockAccessEnable(TRUE);

    _Sd_WriteFunc(u4MemLen, u4Lba);

    // Disable Single Block access => multiple block access
    SD_SingleBlockAccessEnable(FALSE);

    return 0;
}

static INT32 _SDCLI_ChangeClock(INT32 i4Argc, const CHAR ** szArgv)
{ 
    UINT32 u4ClockSel; 

    if (i4Argc != 2)
    {   
        Printf("fci.sd.clock [54,45,40,33,30,27,25,13,9,6,1,0], Current clock = %d Mhz\n", _u4SDClockSetting);
        return 1;
    }

    u4ClockSel = StrToInt(szArgv[1]);
    
    Printf("Clock setting = %d Mhz\n", u4ClockSel);

    FCIHW_ChangeSDCClock(u4ClockSel);

    return 0;
}
     
static INT32 _SDCLI_SetHighClock(INT32 i4Argc, const CHAR ** szArgv)
{ 
    if (i4Argc != 2)
    {   
        Printf("hiclock [50, 27, 25, 13, 4, 1, 0] Mhz, Current clock = %d Mhz\n", _u4SdHighClock);
        return 1;
    }

    _u4SdHighClock = StrToInt(szArgv[1]);
    
    Printf("High Clock setting = %d Mhz\n", _u4SdHighClock);

    return 0;
}

static INT32 _SDCLI_SetNormClock(INT32 i4Argc, const CHAR ** szArgv)
{ 
    if (i4Argc != 2)
    {   
        Printf("noclock [50, 27, 25, 13, 4, 1, 0] Mhz, Current clock = %d Mhz\n", _u4SdNormalClock);
        return 1;
    }

    _u4SdNormalClock = StrToInt(szArgv[1]);
    
    Printf("Normal Clock setting = %d Mhz\n", _u4SdNormalClock);

    return 0;
}

static INT32 _SDCLI_SetLowClock(INT32 i4Argc, const CHAR ** szArgv)
{ 
    if (i4Argc != 2)
    {   
        Printf("loclock [50, 27, 25, 13, 4, 1, 0] Mhz, Current clock = %d Mhz\n", _u4SdLowClock);
        return 1;
    }

    _u4SdLowClock = StrToInt(szArgv[1]);
    
    Printf("Low Clock setting = %d Mhz\n", _u4SdLowClock);

    return 0;
}

static INT32 _SDCLI_HighSpeedEnable(INT32 i4Argc, const CHAR ** szArgv)
{ 
    if (i4Argc != 2)
    {   
        Printf("hispeeden [0 : disable, 1 : enable]\n");
        return 1;
    }

    _fgHighSpeedEnable = (BOOL)(StrToInt(szArgv[1]) ? TRUE : FALSE);
    
    Printf("High Speed Enable = %s\n", _fgHighSpeedEnable ? "TRUE" : "FALSE");

    return 0;
}

static INT32 _SDCLI_SwitchHighSpeed(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgHighSpeed;
    
    if (i4Argc < 2)
    {
        Printf("hispeed [1 : high speed, 0 : normal speed]\n");
        return 1;
    }

    fgHighSpeed = StrToInt(szArgv[1]) ? TRUE : FALSE;
    
    SD_SwitchHighSpeed(fgHighSpeed);
    
    return 0;
}

static INT32 _SDCLI_List(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4CardType;

    i4CardType = SD_GetCard();

    if (i4CardType == FCI_SD_CARD || i4CardType == FCI_MMC_CARD)
    {
        Printf("SDC Slot[%s] (%s) %d MB\n", ((i4CardType == FCI_SD_CARD) ? "SD" : "MMC")
            ,SD_GetModel(), (INT32)(SD_GetSize() >> 11));
    }
    else if(i4CardType == FCI_SDIO_CARD)
    {
        Printf("SDC Slot - SDIO Card \n");
    }
    else
    {
        Printf("SDC Slot have no device\n");
    }

    return 0;
}

static INT32 _SDCLI_Config(INT32 i4Argc, const CHAR ** szArgv)
{
    if (i4Argc < 2)
    {
        Printf("config [Multifunc No.], Current Config = %d\n", _u4SDMultiFuncCfg);
        return 1;
    }

    _u4SDMultiFuncCfg = StrToInt(szArgv[1]);
    
    return 0;
}

static INT32 _SDCLI_CardDetect(INT32 i4Argc, const CHAR ** szArgv)
{
    Printf("Card Remove = %d", FCIHW_DetectCard());
    return 0;
}      

#ifdef FCI_SDIO_SUPPORT
// SDIO bus device
extern void WLAN_Main(BOOL fg4Bits);
extern INT32 _i4SDHighClock;
#ifdef SDIO_DVBH_SUPPORT
EXTERN INT32 DVBH_SetCmdTest(BOOL fgEnable);
EXTERN INT32 DVBH_DataTest(VOID);
EXTERN INT32 DVBH_InterruptTest(VOID);
#endif

static HAL_TIME_T _rMsdTimeStart;
static HAL_TIME_T _rMsdTimeEnd;
static HAL_TIME_T _rMsdTimeDelta;

// SDIO functions
EXTERN INT32  SDIO_IsSDIOCard(VOID);
EXTERN INT32 SDIO_EnableIO(UINT32 u4FuncID, FCI_CALLBACK pfnHISR);

/* Type definition for unsigned integers */
typedef unsigned char           UINT_8, *PUINT_8;
typedef unsigned short          UINT_16, *PUINT_16;
typedef unsigned long           UINT_32, *PUINT_32, UINT, *PUINT;
typedef unsigned long long      UINT_64, *PUINT_64;

#define IN
#define OUT

EXTERN INT32 STORGInit(VOID);
EXTERN UINT_32 SDIO_ReadMCR(UINT_16 offset, UINT_32 *pValue);
EXTERN UINT_32 SDIO_WriteMCR(UINT_16 offset, UINT_32 value);
EXTERN UINT_32 SDIO_WritePort(IN PUINT_8 pucBuf, IN UINT_16 u2Len, IN UINT_16 u2Port);
EXTERN UINT_32 SDIO_ReadPort(OUT PUINT_8 pucBuf, IN UINT_16 u2Len, IN UINT_16 u2Port);
EXTERN INT32 SDIO_SetBusWidth(BOOL fg4BitEnable);
EXTERN INT32 SDIO_AbortIO(UINT32 u4FuncID);
EXTERN INT32 SDC_DisableCD(UINT16 u2RCA);
EXTERN VOID SDIO_ShowCCCR(VOID);
EXTERN INT32 SDIO_Reset(VOID);
EXTERN INT32 SDIO_ReadCCCR(VOID);

static INT32 _SDIO_RegInit(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = STORGInit();

    if (i4Ret != 0)
    {
        Printf("[_SDIO_RegInit] Init storage pseudo device failed.\n");
        return 1;
    }

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
		Printf("A SDIO Card\n");
	}
	else
    {
		Printf("Not a SDIO Card\n");
	}

    return 0;
}

static INT32 _SDIOCLI_Detach(INT32 i4Argc, const CHAR ** szArgv)
{
    SD_Remove();
    Printf("Detach SDCslot device.\n");

    return 0;
}

void _SDIO_Callback(INT32 i4Status)
{
    Printf("_SDIO_Callback 0x%04X\n", i4Status);
}

static INT32 _SDIOCLI_EnableIO(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;
    UINT32 u4FuncID;

    if (i4Argc != 2)
    {
		Printf("fci.sdio.enableio [func_id]\n");
		return 1;
	}

    u4FuncID = StrToInt(szArgv[1]);

    i4Ret = SDIO_EnableIO(u4FuncID, _SDIO_Callback);

    Printf("[_SDIO_EnableIO] 0x%04X\n", i4Ret);

    return 0;
}

static INT32 _SDIOCLI_ReadMCR(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4Offset, u4Val = 0;  // [Klocwork] UNINIT.STACK.MUST Analyze

    if (i4Argc != 2)
    {
		Printf("fci.sdio.readmcr [offset]\n");
		return 1;
	}

    u4Offset = StrToInt(szArgv[1]);

    HAL_GetTime(&_rMsdTimeStart);

    i4Ret = SDIO_ReadMCR((UINT16) u4Offset, &u4Val);

    HAL_GetTime(&_rMsdTimeEnd);
    HAL_GetDeltaTime(&_rMsdTimeDelta, &_rMsdTimeStart, &_rMsdTimeEnd);

    Printf("[ReadMCR] Offset 0x%04X = 0x%08X (0x%08X)\n", u4Offset, u4Val, i4Ret);
    Printf("Cost Time is %d.%06d seconds\n", _rMsdTimeDelta.u4Seconds, _rMsdTimeDelta.u4Micros);

    return 0;
}

static INT32 _SDIOCLI_WriteMCR(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4Offset;
    UINT32 *pu4Val = _pu4FCI_W_Buffer;

    if (i4Argc != 3)
    {
		Printf("fci.sdio.writemcr [offset] [value]\n");
		return 1;
	}

    u4Offset = StrToInt(szArgv[1]);
    *pu4Val    = StrToInt(szArgv[2]);

    // Clear buffer content
    x_memset((VOID *)pu4Val, 0x00, 4);

    HAL_GetTime(&_rMsdTimeStart);

    i4Ret = SDIO_WriteMCR((UINT16) u4Offset, *pu4Val);

    HAL_GetTime(&_rMsdTimeEnd);
    HAL_GetDeltaTime(&_rMsdTimeDelta, &_rMsdTimeStart, &_rMsdTimeEnd);

    Printf("[WriteMCR] Offset 0x%04X => 0x%08X (0x%08X)\n", u4Offset, *pu4Val, i4Ret);
    Printf("Cost Time is %d.%06d seconds\n", _rMsdTimeDelta.u4Seconds, _rMsdTimeDelta.u4Micros);

    return 0;
}

static INT32 _SDIOCLI_ReadPort(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret, i;
    UINT32  u4Len, u4Port;
    UINT8   *pucBuf = (UINT8*)_pu4FCI_R_Buffer;

    if (i4Argc != 3)
    {
        Printf("fci.sdio.SDIO_ReadPort [Port] [Len]\n");
        return 1;
    }

    // WIFI card addr => 0x6000 readable
    u4Port = StrToInt(szArgv[1]);
    u4Len  = StrToInt(szArgv[2]);

    // Clear buffer content
    x_memset((VOID *)pucBuf, 0x00, u4Len);

    HAL_GetTime(&_rMsdTimeStart);

    i4Ret = SDIO_ReadPort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

    HAL_GetTime(&_rMsdTimeEnd);
    HAL_GetDeltaTime(&_rMsdTimeDelta, &_rMsdTimeStart, &_rMsdTimeEnd);

    Printf("[SDIO_ReadPort] Port 0x%04X, Len = 0x%04X (0x%08x)\n", u4Port, u4Len, i4Ret);
    Printf("Cost Time is %d.%06d seconds\n", _rMsdTimeDelta.u4Seconds, _rMsdTimeDelta.u4Micros);

    for (i = 0; i < u4Len/8; i+=8)
    {
        Printf("           0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
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

    if (i4Argc != 3)
    {
        Printf("fci.sdio.SDIO_WritePort [Port] [Len]\n");
		return 1;
    }

    // WIFI card addr => 0x3000 writable
    u4Port = StrToInt(szArgv[1]);
    u4Len  = StrToInt(szArgv[2]);

    // Clear buffer content
    x_memset((VOID *)pucBuf, 0x00, u4Len);

    HAL_GetTime(&_rMsdTimeStart);

    i4Ret = SDIO_WritePort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

    HAL_GetTime(&_rMsdTimeEnd);
    HAL_GetDeltaTime(&_rMsdTimeDelta, &_rMsdTimeStart, &_rMsdTimeEnd);

    Printf("[WritePort] Port 0x%04X => 0x%04X (0x%08X)\n", u4Port, u4Len, i4Ret);
    Printf("Cost Time is %d.%06d seconds\n", _rMsdTimeDelta.u4Seconds, _rMsdTimeDelta.u4Micros);

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
            Printf("SDIO_WpdTest in 4 bit mode.\n");
        }
        else
        {
            Printf("SDIO_WpdTest in 1 bit mode.\n");
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
    UINT32 u4TotalTime, u4ReadPmsInt, u4ReadPmsFlt;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta;

    if (i4Argc != 3)
    {
        Printf("fci.sdio.rdpms [Packet Size] [Count]\n");
        return 1;
    }

    u4Len = StrToInt(szArgv[1]);
    u4Count = StrToInt(szArgv[2]);

    Printf("Count = %d\n", u4Count);

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
            Printf(".");
        }

        HAL_GetTime(&rMsdTimeStart);

        i4Ret = SDIO_ReadPort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

        HAL_GetTime(&rMsdTimeEnd);
        HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

        _rTotalTime.u4Seconds += rMsdTimeDelta.u4Seconds;
        _rTotalTime.u4Micros += rMsdTimeDelta.u4Micros;

        if(i4Ret != 0)
        {
            Printf("\nError when read performance test, failed at count = %d\n", i);
            break;
        }
    }

    Printf("\n");

    if(_rTotalTime.u4Micros >= 1000000)
    {
        Printf("   Original : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

        _rTotalTime.u4Seconds += _rTotalTime.u4Micros / 1000000;
        _rTotalTime.u4Micros %= 1000000;

        Printf("   Transform : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);
    }

    Printf("[SDIO_ReadPort] Port 0x%04X, Len = 0x%04X (iRet = 0x%08x), Count = %d.\n", u4Port, u4Len, i4Ret, i);

    Printf("  => Total Transferred data bits = %d\n", u4Count * u4Len * 8);
    Printf("  => Total Cost Time %d.%06d Sec\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

    // Calculate performance
    u4TotalTime = _rTotalTime.u4Seconds*1000000;
    u4TotalTime += _rTotalTime.u4Micros;    // => u4TotalTime is actual time * 1000000

    u4ReadPmsInt = u4Count * u4Len * 8 / u4TotalTime;  // integer part of performance

    u4ReadPmsFlt = (u4Count * u4Len * 8 - u4ReadPmsInt * u4TotalTime);  // <== performance result, Unit : Mbps
    u4ReadPmsFlt *= 1000;
    u4TotalTime /= 1000;
    u4ReadPmsFlt /= u4TotalTime; // <== performance result, Unit : Mbps

    Printf("  => Performance : %d.%03d M bps\n", u4ReadPmsInt, u4ReadPmsFlt/1000);

    return 0;
}

static INT32 _SDIOCLI_WritePMS(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = 0;
    UINT32  u4Count;
    UINT32  i, u4Len, u4Port;
    UINT8   *pucBuf = (UINT8*)_pu4FCI_W_Buffer;
    HAL_TIME_T _rTotalTime;
    UINT32 u4TotalTime, u4ReadPmsInt, u4ReadPmsFlt;

    HAL_TIME_T rMsdTimeStart;
    HAL_TIME_T rMsdTimeEnd;
    HAL_TIME_T rMsdTimeDelta;

    if (i4Argc != 3)
    {
        Printf("fci.sdio.wtpms [Packet Size] [Count]\n");
        return 1;
    }

    u4Len = StrToInt(szArgv[1]);
    u4Count = StrToInt(szArgv[2]);

    Printf("Count = %d\n", u4Count);

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
            Printf(".");
        }

        HAL_GetTime(&rMsdTimeStart);

        i4Ret = SDIO_WritePort(pucBuf, (UINT16) u4Len, (UINT16) u4Port);

        HAL_GetTime(&rMsdTimeEnd);
        HAL_GetDeltaTime(&rMsdTimeDelta, &rMsdTimeStart, &rMsdTimeEnd);

        _rTotalTime.u4Seconds += rMsdTimeDelta.u4Seconds;
        _rTotalTime.u4Micros += rMsdTimeDelta.u4Micros;

        if(i4Ret != 0)
        {
            Printf("\nError when write performance test, failed at count = %d\n", i);
            break;
        }
    }

    Printf("\n");

    if(_rTotalTime.u4Micros >= 1000000)
    {
        Printf("   Original : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

        _rTotalTime.u4Seconds += _rTotalTime.u4Micros / 1000000;
        _rTotalTime.u4Micros %= 1000000;

        Printf("   Transform : Cost Time is Sec = %d, Micro = %d\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);
    }

    Printf("[SDIO_WritePort] Port 0x%04X, Len = 0x%04X (iRet = 0x%08x), Count = %d.\n", u4Port, u4Len, i4Ret, i);

    Printf("  => Total Transferred data bits = %d\n", u4Count * u4Len * 8);
    Printf("  => Total Cost Time %d.%06d Sec\n", _rTotalTime.u4Seconds, _rTotalTime.u4Micros);

    // Calculate performance
    u4TotalTime = _rTotalTime.u4Seconds*1000000;
    u4TotalTime += _rTotalTime.u4Micros;    // => u4TotalTime is actual time * 1000000

    u4ReadPmsInt = u4Count * u4Len * 8 / u4TotalTime;  // integer part of performance

    u4ReadPmsFlt = (u4Count * u4Len * 8 - u4ReadPmsInt * u4TotalTime);  // <== performance result, Unit : Mbps
    u4ReadPmsFlt *= 1000;
    u4TotalTime /= 1000;
    u4ReadPmsFlt /= u4TotalTime; // <== performance result, Unit : Mbps

    Printf("  => Performance : %d.%03d M bps\n", u4ReadPmsInt, u4ReadPmsFlt/1000);

    return 0;
}

static INT32 _SDIOCLI_SetBusWidth(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret = -1;
    UINT32  u4Bits;

    if (i4Argc != 2)
    {
		Printf("fci.sdio.SetBusWidth [1/4]\n");
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

    Printf("[SDIO_SetBusWidth] %d Bits bus, i4Ret = 0x%x\n", u4Bits, i4Ret);

    return 0;
}

static INT32 _SDIOCLI_ChangeClock(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ClockSel; 

    if (i4Argc != 2)
    {
        Printf("fci.sd.clock [54,45,40,33,30,27,25,13,9,6,1,0] Mhz, Current clock = %d Mhz\n", _u4SDClockSetting);  
        return 1;
    }

    u4ClockSel = StrToInt(szArgv[1]);

    Printf("Clock setting = %d Mhz\n", u4ClockSel);

    FCIHW_ChangeSDCClock(u4ClockSel);

    return 0;
}

static INT32 _SDIOCLI_AbortIO(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;
    UINT32 u4FuncID;

    if (i4Argc != 2)
    {
        Printf("fci.sdio.abortio [func_id]\n");
        return 1;
    }

    u4FuncID = StrToInt(szArgv[1]);

    i4Ret = SDIO_AbortIO(u4FuncID);

    Printf("[_SDIO_AbortIO] 0x%04X\n", i4Ret);

    return 0;
}

static INT32 _SDIOCLI_DisableCD(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32   i4Ret;
    UINT32  u4RCA;

    if (i4Argc != 2)
    {
        Printf("fci.sdio.DisableCD [RCA]\n");
        return 1;
    }

    u4RCA = StrToInt(szArgv[1]);

    i4Ret = SDC_DisableCD(u4RCA);

    Printf("[DisableCD] (0x%04X)\n", i4Ret);

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

    Printf("[ReadCCCR] (0x%08X)\n", i4Ret);

    return 0;
}

static INT32 _SDIOCLI_Reset(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32  i4Ret;

    UNUSED(i4Argc);
    UNUSED(szArgv);

    i4Ret = SDIO_Reset();

    Printf("[Reset] (0x%08X)\n", i4Ret);

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
        printf("(%s):SDIO_IsSDIOCard fail.\n", __FUNCTION__);
    }

    return 0;
}

static INT32 _SDIOCLI_DvbhSetCmd(INT32 i4Argc, const CHAR ** szArgv)
{
    BOOL fgEnable;

    if (i4Argc != 2)
    {
        Printf("fci.sd.ds [enable].\n");
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

    Printf("Test card size = %d MB.\n", (u4CardSize/(2*1024)));

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
        Printf("\nTest xD %d ~ %d MB.\n", u4Addr*16, (u4Addr+1)*16);

        CLI_Parser_Arg("fci.sm.w 0x01700000 0x1000000 %d", u4Addr*32768);
        CLI_Parser("b.ms 0x02700000 0 0x400000");
        CLI_Parser_Arg("fci.sm.r 0x02700000 0x1000000 %d", u4Addr*32768);
        CLI_Parser("b.mcmp 0x02700000 0x01700000 0x1000000");
    }

    Printf("\nTest xD card successfully.\n");
    return 0;
}

static INT32 _SM_Status(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc != 1) {
        Printf("fci.sm.status\n");
        return 1;
    }
    i4Ret = SMXD_GetStatus();
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_Reset(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc != 1) {
        Printf("fci.sm.reset\n");
        return 1;
    }
    i4Ret = _SMXD_Reset();
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_Read(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemPtr, u4MemLen, u4Lba;
    INT32 i4Ret;
    HAL_TIME_T rTime0, rTime1;

    if (i4Argc != 4) {
        Printf("fci.sm.r [memptr] [memlen] [lba]\n");
        return 1;
    }
    u4MemPtr = StrToInt(szArgv[1]);
    u4MemLen = StrToInt(szArgv[2]);
    u4Lba = StrToInt(szArgv[3]);

    HAL_GetTime(&rTime0);

    i4Ret = SMXD_Read((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rTime1);

    Printf("Time = %d sec.\n", (rTime1.u4Seconds - rTime0.u4Seconds));

    if (i4Ret != 0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_Write(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4MemPtr, u4MemLen, u4Lba;
    INT32 i4Ret;
    HAL_TIME_T rTime0, rTime1;

    if (i4Argc != 4) {
        Printf("fci.sm.w [memptr] [memlen] [lba]\n");
        return 1;
    }
    u4MemPtr = StrToInt(szArgv[1]);
    u4MemLen = StrToInt(szArgv[2]);
    u4Lba = StrToInt(szArgv[3]);

    HAL_GetTime(&rTime0);

    i4Ret = SMXD_Write((UINT64)(u4Lba << 9), u4MemPtr, u4MemLen);

    HAL_GetTime(&rTime1);

    Printf("Time = %d sec.\n", (rTime1.u4Seconds - rTime0.u4Seconds));

    if (i4Ret) {
        Printf("SM/xd Write failed\n");
    } else {
        Printf("SM/xd Write ok\n");
    }
    return 0;
}

static INT32 _SM_Erase(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4BlkIdx, u4BlkNum;
    INT32 i4Ret;

    if (i4Argc != 3) {
        Printf("fci.sm.e [blkidx] [blknum]\n");
        return 1;
    }

    u4BlkIdx = StrToInt(szArgv[1]);
    u4BlkNum = StrToInt(szArgv[2]);
    i4Ret = SMXD_Erase(u4BlkIdx, u4BlkNum);
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}


static INT32 _SM_Attach(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = SMXD_Insert();
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_LoopTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    if (i4Argc != 1) {
        Printf("fci.sm.lt.\n");
        return 1;
    }

    i4Ret = _SMXD_StressTest();
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_EraseAll(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _SMXD_EraseAll();
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

static INT32 _SM_WriteMultiBlk(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i4Ret;

    i4Ret = _SMXD_WriteMultiBlk();
    if (i4Ret!=0) {
        Printf("%s failed.\n", __FUNCTION__);
        return 1;
    }
    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}
#endif  // #ifdef CC_FCI_SMXD_SUPPORT

/******************************************************************************
* FCI memory card command lists
******************************************************************************/
static CLI_EXEC_T arMSCmdTbl[] = {
    DECLARE_G_CMD(_MSCLI_Stress, stress, st, "fci.ms.st"),
    DECLARE_G_CMD(_MSCLI_Attach, attach, at, "fci.ms.at"),
    DECLARE_G_CMD(_MSCLI_Detach, detach, de, "fci.ms.de"),
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
    DECLARE_G_CMD(_MSCLI_ChangeClock,  clock,   clock, "fci.ms.clock"),
    DECLARE_G_CMD(_MSCLI_List, list, ls, "fci.ms.ls"),
    DECLARE_G_CMD(_MSCLI_Config, config, config, "fci.ms.config"),
#ifndef CC_NO_CARD_DETECT_PIN    
    DECLARE_G_CMD(_MSCLI_CardDetect, carddetect, carddetect, "fci.ms.carddetect"),
#endif
    //Last CLI command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

static CLI_EXEC_T arSDCmdTbl[] = {
    DECLARE_G_CMD(_SDCLI_Stress, stress, st, "fci.sd.st"),
    DECLARE_G_CMD(_SDCLI_Attach, attach, at, "fci.sd.at"),
    DECLARE_G_CMD(_SDCLI_Detach, detach, de, "fci.sd.de"),
    DECLARE_G_CMD(_SDCLI_Read, read, r, "fci.sd.r [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_Write, write, w, "fci.sd.w [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_SingleBlockRead, sread, sr, "fci.sd.sr [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_SingleBlockWrite, swrite, sw, "fci.sd.sw [lba] [memlen]"),
    DECLARE_G_CMD(_SDCLI_Compare, compare, cp, "fci.sd.cp [memlen]"),
    DECLARE_G_CMD(_SDCLI_SetParallel, parallel, pl, "fci.sd.pl"),
    DECLARE_G_CMD(_SDCLI_SetSerial, serial, sl, "fci.sd.sl"),
    DECLARE_G_CMD(_SDCLI_ChangeClock,  clock,   clock, "fci.sd.clock"),
    DECLARE_G_CMD(_SDCLI_SetHighClock,  hiclock,   hiclock, "fci.sd.hiclock"),
    DECLARE_G_CMD(_SDCLI_SetNormClock,  noclock,   noclock, "fci.sd.noclock"),
    DECLARE_G_CMD(_SDCLI_SetLowClock,  loclock,   loclock, "fci.sd.loclock"),   
    DECLARE_G_CMD(_SDCLI_HighSpeedEnable,  hispeeden,   hispeeden, "fci.sd.hispeeden"),  
    DECLARE_G_CMD(_SDCLI_SwitchHighSpeed, hispeed, hispeed, "fci.sd.hispeed"), 
    DECLARE_G_CMD(_SDCLI_List, list, ls, "fci.sd.ls"),
    DECLARE_G_CMD(_SDCLI_Config, config, config, "fci.sd.config"),    
    
#ifndef CC_NO_CARD_DETECT_PIN    
    DECLARE_G_CMD(_SDCLI_CardDetect, carddetect, carddetect, "fci.sd.carddetect"),
#endif
    //Last CLI command record, NULL
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

#ifdef FCI_SDIO_SUPPORT
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
    DECLARE_G_CMD(_SDCLI_List, list, ls, "fci.sdio.ls"),
    DECLARE_G_CMD(_SDCLI_Config, config, config, "fci.sdio.config"),        
#ifndef CC_NO_CARD_DETECT_PIN    
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
    DECLARE_CMD(_FCIPowerOn, pwron, pwron, "fci.pwron"),
    DECLARE_CMD(_FCIPowerOff, pwroff, pwroff, "fci.pwroff"),

#ifdef FCI_DIAG_TEST    
    DECLARE_G_CMD(_FCIDiag, diag, diag, "fci.diag"),
#endif

    CLIMOD_DEBUG_CLIENTRY(FCI),
    // MS/MSPro/SD/MMC sub commands.

    {"MS",   "ms",  NULL, arMSCmdTbl,  "MS commands",   CLI_GUEST},
    {"SD",   "sd",  NULL, arSDCmdTbl,  "SD commands",   CLI_GUEST},

#ifdef CC_FCI_SMXD_SUPPORT
    {"SM",   "sm",  NULL, arSMCmdTbl,  "SM commands",   CLI_GUEST},
#endif

#ifdef FCI_SDIO_SUPPORT
    {"SDIO",   "sdio",  NULL, arSDIOCmdTbl,  "SDIO commands",   CLI_GUEST},
#endif

#endif // #ifdef CC_FCI_ENABLE

    //Last CLI command record, NULL
    DECLARE_END_ITEM(),
};

/********************************************************/
CLI_MAIN_COMMAND_ITEM(Fci)
{
    "fci",
    NULL,
    NULL,
    arFciCmdTbl,
    "Fci command",
    CLI_SUPERVISOR
};

/******************************************************************************
* Global Function
******************************************************************************/
