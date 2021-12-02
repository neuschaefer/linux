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
 * $RCSfile: eeprom_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file eeprom_cmd.c
 *  This file is the implementation of eeprom CLI definition.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "eeprom_if.h"
#include "uart_hw_drvif.h"
#include "x_stl_lib.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "x_timer.h"

#define DEFINE_IS_LOG        CLI_IsLog
#include "x_debug.h"
#include "x_serial.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define EEP_STACK_SZ        (8*1024)
#define EEP_PRIORITY        (100)
#define EEP_BUF_SZ          (0x100)
#define EEP_TOTAL_SZ        (0x800)

#define EEP_ELE_TYPE_U8     0
#define EEP_ELE_TYPE_U16    1
#define EEP_ELE_TYPE_U32    2
#define EEP_ELE_TYPE_U64    3
#define EEP_ELE_TYPE_I8     4
#define EEP_ELE_TYPE_I16    5
#define EEP_ELE_TYPE_I32    6
#define EEP_ELE_TYPE_I64    7
#define EEP_ELE_TYPE_TOTAL  8


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define EEP_SET_BYTE(data, bitoffset, value)  \
    (data &= ~((UINT32)0xFF << (bitoffset))); \
    (data |= (((value) & (UINT32)0xFF) << (bitoffset)))


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
static CHAR _aszTypeStr[EEP_ELE_TYPE_TOTAL + 1][4] =
{
    "U8",
    "U16",
    "U32",
    "U64",
    "I8",
    "I16",
    "I32",
    "I64",
    "NA",
};

#ifdef CC_CLI
//__align(16)
static volatile INT32 _i4EepromLoop = 1;

//__align(16)
#ifndef CC_MTK_LOADER
static HANDLE_T _hEepromThread;
//__align(16)

static UINT8 _au1EepromBuf[EEP_BUF_SZ];
//__align(16)

static BOOL _fgEepromTimeInit = FALSE;

#endif


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _EEPROMCLI_Init
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Init(INT32 i4Argc, const CHAR **szArgv)
{
    return EEPROM_Init();
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Stop
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Stop(INT32 i4Argc, const CHAR **szArgv)
{
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Reset
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Reset(INT32 i4Argc, const CHAR **szArgv)
{
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Diag
 *  EEPROM read/write stress test.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Diag(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 i4Ret;
    UINT32 u4SrcPtr = 0;
    UINT32 u4DstPtr = 0x2000000;
    UINT32 u4MemLen;
    UINT32 u4Offset;
    UINT32 u4EepSize;

    if (i4Argc != 1)
    {
        Printf("eeprom.diag.\n");
        return 1;
    }
    u4EepSize = EEPROM_GetSize();

    Printf("eep size = %d bytes.\n", u4EepSize);

    for (u4Offset = 0; u4Offset < u4EepSize; u4Offset++)
    {
    // set length as quasi random value.
    u4MemLen = u4EepSize - u4Offset;
    u4MemLen = (u4MemLen == 0) ? 1 : u4MemLen;
    u4SrcPtr += u4MemLen;

    Printf("u4SrcPtr= 0x%08X, u4Offset = %d, len = %d.\n", u4SrcPtr, u4Offset, u4MemLen);

    // write data.
    i4Ret = EEPROM_Write((UINT64)u4Offset, u4SrcPtr, u4MemLen);
    if (i4Ret)
    {
        Printf("EEPROM_Write fail. Ofst:0x%08x, Len:0x%08x.\n", u4Offset, u4MemLen);
        return 1;
    }

    // read data.
    i4Ret = EEPROM_Read((UINT64)u4Offset, u4DstPtr, u4MemLen);
    if (i4Ret)
    {
        Printf("EepromFlash read fail.\n");
        return 1;
    }

    // compare data.
    if (0 != x_memcmp((const VOID *)u4SrcPtr, (const VOID *)u4DstPtr, u4MemLen))
    {
        Printf("Compare failed\n");
        return 1;
    }
    }

    Printf("%s successfully.\n", __FUNCTION__);
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Enable
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Enable(INT32 i4Argc, const CHAR **szArgv)
{
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Disable
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Disable(INT32 i4Argc, const CHAR **szArgv)
{
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_GetTime
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
#ifndef CC_MTK_LOADER

static UINT32 _EEPROMCLI_GetTime(UINT32 *pu4Sec)
{
    HAL_TIME_T rTime;

    if (!_fgEepromTimeInit)
    {
        HAL_InitTimer();
        _fgEepromTimeInit = TRUE;
    }

    HAL_GetTime(&rTime);

    if (pu4Sec)
    {
        *pu4Sec = rTime.u4Seconds;
    }

    return rTime.u4Seconds;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_ReadThread
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static VOID _EEPROMCLI_ReadThread(VOID *pvArgv)
{
    INT32 i4Ret;
    UINT32 u4Offset, u4PrevSec, u4Seconds;

    Printf("Mtk EepromFlash reading Thread Start...\n");
    _i4EepromLoop = 1;
    u4Offset = 0;
    u4PrevSec = _EEPROMCLI_GetTime(NULL);

    while (_i4EepromLoop)
    {
        i4Ret = EEPROM_Read((UINT64)u4Offset, (UINT32)_au1EepromBuf, EEP_BUF_SZ);
        if (i4Ret)
        {
            Printf("Reading thread failed\n");
            _i4EepromLoop = 0;
        }

        u4Offset += EEP_BUF_SZ;

        if (u4Offset >= EEP_TOTAL_SZ)
        {
            u4Offset = 0;
        }

        u4Seconds = _EEPROMCLI_GetTime(NULL);
        if (u4Seconds > u4PrevSec + 5)
        {
#if 0
            Printf("EepromReading...\n");
#endif
            u4PrevSec = _EEPROMCLI_GetTime(NULL);
        }
    }

    Printf("Mtk EepromFlash reading thread Exit...\n");
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_ReadTest
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_ReadTest()
{
    UINT32 u4Offset, u4Len;

    INT32 i4Ret;

    for (u4Offset = 0; u4Offset < EEP_BUF_SZ; u4Offset++)
    {
        for (u4Len = 1; u4Len <= EEP_BUF_SZ - u4Offset; u4Len++)
        {
            i4Ret = EEPROM_Read((UINT64)u4Offset, (UINT32)_au1EepromBuf, u4Len);
            if (i4Ret)
            {
                Printf("EEPROM_Read(Ofst:0x%08x, Ptr:0x%08x, Len:0x%08x\n",
                       u4Offset, (UINT32)_au1EepromBuf, u4Len);
                return 1;
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_ReadTest_Max
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_ReadTest_Max()
{
    UINT32 u4Offset, u4Len = 1;

    UINT32 u4Data = 0;
    INT32 i4Ret;

    for (u4Offset = 0; u4Offset < EEP_TOTAL_SZ; u4Offset++)
    {
        i4Ret = EEPROM_Read((UINT64)u4Offset, (UINT32)&u4Data, 1);
        if (i4Ret || (u4Data != (u4Offset & 0xFF)))
        {
            Printf("ERR: EEPROM_Read(Ofst:0x%08x, Ptr:0x%08x, Len:0x%08x\n",
                   u4Offset, (UINT32)u4Data, u4Len);
            return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_WriteTest
 *  EEPROM
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_WriteTest()
{
    UINT32 u4Offset, u4Len;

    INT32 i4Ret;

    for (u4Offset = 0; u4Offset < EEP_BUF_SZ; u4Offset++)
    {
        _au1EepromBuf[u4Offset] = (u4Offset & 0x0ff);
    }

    for (u4Offset = 0; u4Offset < EEP_BUF_SZ; u4Offset++)
    {
        u4Len = 1;

        i4Ret = EEPROM_Write((UINT64)u4Offset, (UINT32)_au1EepromBuf + u4Offset, u4Len);
        if (i4Ret)
        {
            Printf("EEPROM_Read(Ofst:0x%08x, Ptr:0x%08x, Len:0x%08x\n",
                   u4Offset, (UINT32)_au1EepromBuf, u4Len);
            return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_TestSuit
 *  EEPROM create a thread and read EEPROM.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_WriteTest_Max()
{
    UINT32 u4Offset;

    UINT32 u4Data;

    for (u4Offset = 0; u4Offset < EEP_TOTAL_SZ; u4Offset++)
    {
        u4Data = u4Offset & 0xFF;
        VERIFY(0 == EEPROM_Write((UINT64)u4Offset, (UINT32)&u4Data, 1));
    }

    return 0;
}
#endif

//-------------------------------------------------------------------------
/** _EEPROMCLI_TestSuit
 *  EEPROM create a thread and read EEPROM.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_TestSuit(INT32 i4Argc, const CHAR **szArgv)
{
#ifndef CC_MTK_LOADER
    INT32 i, i4Ret;
    const CHAR*szStr;
    EEPNIMCFG_T rNimCfg;

    EEPDTV_Read((UINT64)EEPDTV_DTVCFG_FLAGS, (UINT32)(void *)&rNimCfg, sizeof(rNimCfg));
    rNimCfg.u1Tag = 0x5A;
    rNimCfg.u1TunerType = 0x00;
    rNimCfg.u1Reserved = 0x00;
    rNimCfg.u1Checksum = 0x5A;
    EEPDTV_Write((UINT64)EEPDTV_DTVCFG_FLAGS, (UINT32)(void *)&rNimCfg, sizeof(rNimCfg));

    for (i = 0; i < i4Argc; i++)
    {
        szStr = szArgv[i];
        if (szStr[0] == '-')
        {
            switch (szStr[1])
            {
            case 'S':
            case 's':
                return _EEPROMCLI_ReadTest();

            case 'E':
            case 'e':
                _EEPROMCLI_WriteTest_Max();
                _EEPROMCLI_ReadTest_Max();
                _i4EepromLoop = 0;
                return 0;

            case 'W':
            case 'w':
                return _EEPROMCLI_WriteTest();

            default:
                break;
            }
        }
    }
    /* back ground reading. */
    i4Ret = x_thread_create(&_hEepromThread,       "eepromead",
                            EEP_STACK_SZ,          EEP_PRIORITY,
                            _EEPROMCLI_ReadThread, 0,
                            NULL);

    if (i4Ret != OSR_OK)
    {
        Printf("Create thread to read eepromflash failed.\n");
        return 1;
    }
    Printf("Create thread to read eepromflash successfully.\n");
#endif

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Size
 *  EEPROM set size.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Size(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4OldSize;
    UINT32 u4NewSize;

    if (i4Argc != 2)
    {
        Printf("eeprom.size [new_size]\n");
        return 1;
    }
    u4OldSize = EEPROM_GetSize();
    u4NewSize = StrToInt(szArgv[1]);

    EEPROM_SetSize(u4NewSize);

    Printf("Old size = %d, New size = %d.\n", u4OldSize, u4NewSize);
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Page
 *  EEPROM set Page size.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Page(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4OldPage;
    UINT32 u4NewPage;

    if (i4Argc != 2)
    {
        Printf("eeprom.page [new_page_size]\n");
        return 1;
    }
    u4OldPage = EEPROM_GetPage();
    u4NewPage = StrToInt(szArgv[1]);

    if ((u4NewPage != 8) && (u4NewPage != 16) && (u4NewPage != 32))
    {
        Printf("Error! The page size supported is 8/16/32 bytes\n");
        return 1;
    }

    EEPROM_SetPage(u4NewPage);

    Printf("Old page size = %d, New page size = %d.\n", u4OldPage, u4NewPage);
    return 0;
}
#endif

//-------------------------------------------------------------------------
/** _EEPROMCLI_ReadByte
 *  EEPROM read single data byte at offset.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_ReadByte(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 _gau1Data[128]={0};
    UINT32 u4Offset, u4Len;
    INT32 i4Ret;

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("eeprom.rb [offset] [len]\n");
        return 1;
    }

    u4Len = 1;
    u4Offset = StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u4Len = StrToInt(szArgv[2]);
        if (u4Len >= 128)
        {
            u4Len = 128;
        }
    }

    i4Ret = EEPROM_Read((UINT64)u4Offset, (UINT32)_gau1Data, u4Len);
    if (i4Ret)
    {
        Printf("EepromFlash read failed\n");
        return 1;
    }

#ifndef CC_MTK_LOADER
#ifndef __KERNEL__
    if (UART_HwTransMode())
    {
        for (i4Ret = 0; i4Ret < u4Len; i4Ret++)
        {
            if ((i4Ret & 0xf)==0)
            {
                Printf("\n0x%08x | 0x%02x", u4Offset+i4Ret, _gau1Data[i4Ret]);
            }
            else
            {
                Printf(" 0x%02x", _gau1Data[i4Ret]);
            }
        }
        Printf("\n\n");
    }
    else
#endif
#endif /* CC_MTK_LOADER */
    {
        SerPollPutChar(_gau1Data[0]);
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Read
 *  EEPROM read data bytes at offset.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------

static INT32 _EEPROMCLI_Read(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Ret;
    UINT32 u4Offset, u4MemPtr, u4MemLen;

    if (i4Argc != 4)
    {
        Printf("eeprom.r [eeprom offset] [memptr] [memlen]\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4MemLen = StrToInt(szArgv[3]);

    i4Ret = EEPROM_Read((UINT64)u4Offset, u4MemPtr, u4MemLen);
    if (i4Ret)
    {
        Printf("EepromFlash read failed\n");
        return 1;
    }

    Printf("EepromFlash read successfully.\n");
    return 0;
}


//-------------------------------------------------------------------------
/** _EEPROMCLI_WriteByte
 *  EEPROM write one byte at offset.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_WriteByte(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset;
    UINT8 u1Data[0x10];
    INT32 i4Ret;
    UINT32 u4MemLen, i;

    if (i4Argc < 3)
    {
        Printf("eeprom.wb [eeprom offset] [byte1] [byte2] ...\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);

    u4MemLen = (i4Argc-2);
    Printf("Data length = %d.\n", u4MemLen);

    for (i = 0; i < u4MemLen; i++)
    {
        u1Data[i] = StrToInt(szArgv[2+i]);
    }

    i4Ret = EEPROM_Write((UINT64)u4Offset, (UINT32)u1Data, u4MemLen);
    if (i4Ret)
    {
        Printf("EepromFlash write failed return %d\n", i4Ret);
        return 1;
    }

    Printf("EepromFlash write successfully.\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Write
 *  EEPROM CLI write.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Write(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset, u4MemPtr, u4MemLen;

    INT32 i4Ret;
    HAL_TIME_T rTime;

    if (i4Argc != 4)
    {
        Printf("eeprom.w [eeprom offset] [memptr] [memlen]\n");
        return 1;
    }

    u4Offset = StrToInt(szArgv[1]);
    u4MemPtr = StrToInt(szArgv[2]);
    u4MemLen = StrToInt(szArgv[3]);

    HAL_GetTime(&rTime);
    Printf("Start %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);

    i4Ret = EEPROM_Write((UINT64)u4Offset, u4MemPtr, u4MemLen);
    if (i4Ret)
    {
        Printf("EEPROM_Write(Ofst:0x%08x, MemPtr:0x%08x, Len:0x%08x) return %d failed\n",
               u4Offset, u4MemPtr, u4MemLen, i4Ret);
        return 1;
    }

    HAL_GetTime(&rTime);
    Printf("End %d.%06d\n", rTime.u4Seconds, rTime.u4Micros);
    Printf("EepromFlash write successfully.\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Measure
 *  EEPROM CLI for stress test.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Measure(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 i4Ret;
    UINT32 i, u4Size, u4LoopIdx, u4LoopCnt;
    UINT8 u1GoldenVal = 0;
    static UINT8 *pu1TestBuf = NULL;
    static UINT8 *pu1GoldenBuf = NULL;

    if (i4Argc != 2)
    {
        Printf("eeprom.m [loopcnt]\n");
        return 1;
    }

    // reserve 4 bytes for nand write count.
    u4Size = EEPROM_GetSize() - 4;

    if (pu1GoldenBuf == NULL)
    {
        pu1GoldenBuf = x_mem_alloc(u4Size);
    }

    if (pu1TestBuf == NULL)
    {
        pu1TestBuf = x_mem_alloc(u4Size);
    }

    if (pu1GoldenBuf == NULL || pu1TestBuf == NULL)
    {
        ASSERT(0);
        return 1;
    }

    u4LoopCnt = StrToInt(szArgv[1]);
    for (u4LoopIdx = 0; u4LoopIdx < u4LoopCnt; u4LoopIdx++)
    {
        u1GoldenVal++;
        x_memset(pu1GoldenBuf, u1GoldenVal, u4Size);

        i4Ret = EEPROM_Write(0, (UINT32)pu1GoldenBuf, u4Size);
        if (i4Ret)
        {
            Printf("EEPROM_Write failed in loop %d!\n", u4LoopIdx);
            ASSERT(0);
            return 1;
        }

        i4Ret = EEPROM_Read(0, (UINT32)pu1TestBuf, u4Size);
        if (i4Ret)
        {
            Printf("EEPROM_Read failed in loop %d!\n", u4LoopIdx);
            ASSERT(0);
            return 1;
        }

        for (i = 0; i < u4Size; i++)
        {
            if (pu1GoldenBuf[i] != pu1TestBuf[i])
            {
                Printf("Eeprom compare fail at offset 0x%X in loop %d!\n", i, u4LoopIdx);
                ASSERT(0);
                return 1;
            }
        }

        if ((u4LoopIdx % 10000) == 0)
        {
            Printf("Eeprom stress test %d / %d pass.\n", u4LoopIdx, u4LoopCnt);
        }
    }

    Printf("Eeprom stress test succeed!\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Identify
 *  get EEPROM size.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @return  EEPROM size.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Identify(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4TotalByte;
    UINT32 u4Var, u4Ret;

    u4TotalByte = EEPROM_GetSize();
    Printf("Total number of Eeprom Bytes is 0x%x\n", u4TotalByte);

    u4Ret = DRVCUST_OptQuery(eSystemEepromBus, &u4Var);
    Printf("System EEP Bus: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eSystemEepromAddress, &u4Var);
    Printf("System EEP Address: ");    (u4Ret==0)?(Printf("0x%x\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eSystemEepromClkDiv, &u4Var);
    Printf("System EEP Clk Div: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eSystemEepromPageSize, &u4Var);
    Printf("System EEP Page Size: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eSysEepromWPGpio, &u4Var);
    Printf("System EEP WP Gpio: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Not set\n"));

    u4Ret = DRVCUST_OptQuery(eSysEepromWPEnablePolarity, &u4Var);
    Printf("System EEP WPEnb Polarity: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Not set\n"));

    u4Ret = DRVCUST_OptQuery(eHdcpEepromBus, &u4Var);
    Printf("HDCP EEP Bus: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eHdcpEepromAddress, &u4Var);
    Printf("HDCP EEP Address: ");    (u4Ret==0)?(Printf("0x%x\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eHdcpEepromClkDiv, &u4Var);
    Printf("HDCP EEP Clk Div: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Query Fail\n"));

    u4Ret = DRVCUST_OptQuery(eHdcpEepromWPGpio, &u4Var);
    Printf("HDCP EEP WP Gpio: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Not set\n"));

    u4Ret = DRVCUST_OptQuery(eHdcpEepromWPEnablePolarity, &u4Var);
    Printf("HDCP EEP WPEnb Polarity: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Not set\n"));

    u4Ret = DRVCUST_OptQuery(eEdidEepromWPGpio, &u4Var);
    Printf("Edid EEP WP Gpio: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Not set\n"));

    u4Ret = DRVCUST_OptQuery(eEdidEepromWPEnablePolarity, &u4Var);
    Printf("Edid EEP WPEnb Polarity: ");    (u4Ret==0)?(Printf("%d\n", u4Var)):(Printf("Not set\n"));

    return 0;
}

static INT32 _EEPROMCLI_SetCfg(INT32 i4Argc, const CHAR **szArgv)
{
    UINT8 u1Flag = 0;
    UINT32 u4FlagType, u4Value;
    DTVCFG_T rDtvCfg;

    if (i4Argc != 3)
    {
        Printf("eeprom.setcfg [flag#] [0/1]\n");
        Printf("flag[0]=DTVCFG_FLAG3_SUPPORT_CLI\n");
        return 1;
    }

    u4FlagType = StrToInt(szArgv[1]);
    u4Value = StrToInt(szArgv[2]);
    if (EEPDTV_GetCfg(&rDtvCfg))
    {
        Printf("Error: EEPROM read dtvcfg fail\n");
    }

    if (u4FlagType == 0)
    {
        u1Flag = (UINT8)DTVCFG_FLAG3_SUPPORT_CLI;
    }
    else
    {
        Printf("Error: no support flag\n");
        return 1;
    }

    if (u4Value)
    {
        rDtvCfg.u1Flags3 |= u1Flag;
    }
    else
    {
        rDtvCfg.u1Flags3 &= ~u1Flag;
    }

    if (EEPDTV_SetCfg(&rDtvCfg))
    {
        Printf("\nError: EEPROM write dtvcfg fail\n");
        return 1;
    }

    Printf("Done\n");

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_BatchWrite
 *  Batch Write EEPROM.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_BatchWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4EEPROMSize;
    UINT8 u1Data;
    UINT32 i, u4Offset, u4Len;
    UINT8 * pu1Data;

    if (i4Argc != 4)
    {
        Printf("eeprom.bw [eeprom offset] [data (UINT8)] [len]\n");
        return 1;
    }

    u4EEPROMSize = EEPROM_GetSize();

    u4Offset = StrToInt(szArgv[1]);
    u1Data = (UINT8) StrToInt(szArgv[2]);
    u4Len = StrToInt(szArgv[3]);

    Printf("EEPROM Size = 0x%x\n", u4EEPROMSize);
    if (u4Offset > u4EEPROMSize)
    {
        Printf("Offset is too large.\n");
        return 1;
    }

    if ((u4Offset + u4Len) > u4EEPROMSize)
    {
        Printf("Offset + Len over EEPROM size.\n");
        return 1;
    }

    pu1Data = (UINT8 *)x_mem_alloc(u4Len);
    if (!pu1Data)
    {
        Printf("Allocating memory failure.\n");
        return 1;
    }

    for (i = 0; i < u4Len; i++)
    {
        *(pu1Data + i) = u1Data;
    }

    Printf("Batch write EEPROM: offset 0x%x, data 0x%x len 0x%x\n", u4Offset, u1Data, u4Len);

    if (EEPROM_Write((UINT64)u4Offset, (UINT32)pu1Data, u4Len))
    {
        Printf("Write error.\n");
    }

    if (pu1Data)
    {
        x_mem_free(pu1Data);
    }

    Printf("Done.\n");

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_Dump
 *  Dump EEPROM contents
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_Dump(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4EEPROMSize;
    UINT32 u4Offset, u4Len, u4Idx;
    UINT8 * pu1Buf;

    if (i4Argc != 3)
    {
        Printf("eeprom.dump [eeprom offset] [len]\n");
        return 1;
    }

    u4EEPROMSize = EEPROM_GetSize();

    u4Offset = StrToInt(szArgv[1]);
    u4Len = StrToInt(szArgv[2]);

    Printf("EEPROM Size = 0x%x\n", u4EEPROMSize);
    if (u4Offset > u4EEPROMSize)
    {
        Printf("Offset is too large.\n");
        return 1;
    }

    if ((u4Offset + u4Len) > u4EEPROMSize)
    {
        Printf("Offset + Len over EEPROM size.\n");
        return 1;
    }

    pu1Buf = (UINT8 *) x_mem_alloc(u4Len);
    if (!pu1Buf)
    {
        Printf("Allocating memory failure.\n");
        return 1;
    }
    x_memset((VOID *)pu1Buf, 0, u4Len);

    if (EEPROM_Read((UINT64)u4Offset, (UINT32)pu1Buf, u4Len))
    {
        Printf("Read error.\n");
    }

    for (u4Idx = 0; u4Idx < u4Len; u4Idx++)
    {
        if (!(u4Idx % 8))
        {
            Printf("\n0x%08X :", u4Idx);
        }
        Printf(" 0x%02X,", pu1Buf[u4Idx]);
    }
    Printf("\n");

    if (pu1Buf)
    {
        x_mem_free(pu1Buf);
    }

    Printf("Done.\n");
    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_GetUartByte
 *  Get data from uart with time out.
 *  @param  pu1MemPtr   user's data buffer.
 *  @param  u4TimeOutSec    time out in second. u4TimeOutSec = 0 means no time out.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_GetUartByte(UINT8* pu1MemPtr, UINT32 u4TimeOutSec)
{
    HAL_TIME_T rOrgTime, rNewTime, rDeltaTime;

    ASSERT(pu1MemPtr);

    if (u4TimeOutSec > 0)
    {
        HAL_GetTime(&rOrgTime);

        *pu1MemPtr = SerPollGetChar();

        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDeltaTime, &rOrgTime, &rNewTime);

        if (rDeltaTime.u4Seconds > u4TimeOutSec)
        {
            Printf("uart rx time out.\n");
            return 1;
        }
    }
    else
    {
        *pu1MemPtr = SerPollGetChar();
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_UartWrite
 *  Get data from uart and write to EEPROM.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_UartWrite(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 u4Offset = 0, u4MemLen = 0;
    UINT8* pu1MemPtr = NULL;
    INT32 i4Ret = 1;
    INT32 i;
    UINT8 u1Data;
    // time out to get a byte =  2 second.
    UINT32 u4TimeOutSec = 2;

    // get 4 byte of data length from uart.
    for (i = 0; i < 4; i++)
    {
        if (_EEPROMCLI_GetUartByte(&u1Data, u4TimeOutSec))
        {
            goto EEP_EXIT;
        }
        EEP_SET_BYTE(u4MemLen, 8*i, u1Data);
    }

    if (u4MemLen == 0)
    {
        Printf("eep length zero.\n");
        goto EEP_EXIT;
    }

    // allocate user's data buffer.
    pu1MemPtr = x_mem_alloc(u4MemLen);
    if (!pu1MemPtr)
    {
        Printf("Allocate eep memory fail.\n");
        goto EEP_EXIT;
    }

    // get 4 byte of data offset from uart.
    for (i = 0; i < 4; i++)
    {
        if (_EEPROMCLI_GetUartByte(&u1Data, u4TimeOutSec))
        {
            goto EEP_EXIT;
        }
        EEP_SET_BYTE(u4Offset, 8*i, u1Data);
    }

    // get user's data from uart.
    for (i = 0; i < u4MemLen; i++)
    {
        if (_EEPROMCLI_GetUartByte(&u1Data, u4TimeOutSec))
        {
            goto EEP_EXIT;
        }
        pu1MemPtr[i] = u1Data;
    }

    // write to eeprom.
    i4Ret = EEPROM_Write((UINT64)u4Offset, (UINT32)pu1MemPtr, u4MemLen);

EEP_EXIT:
	if(pu1MemPtr)
		{
    		x_mem_free(pu1MemPtr);
		}

    if (i4Ret == 0)
    {
        SerPollPutChar('S');
        Printf("Eeprom write successfully.\n");
    }
    else
    {
        SerPollPutChar('F');
        Printf("Eeprom write fail.\n");
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _PrintElement
 *  Utility function used in _EEPROMCLI_LayoutList.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static void _PrintElement(UINT32 u4Idx, CHAR szName[], UINT8 u1Type, UINT32 u4NumOfEle, UINT32 u4Offset)
{
    u1Type = u1Type>EEP_ELE_TYPE_TOTAL?EEP_ELE_TYPE_TOTAL:u1Type;
    Printf("%-10d %-40s %-10s %-10d 0x%-8x\n", u4Idx, szName, _aszTypeStr[u1Type], u4NumOfEle, u4Offset);
}

//-------------------------------------------------------------------------
/** _EEPROMCLI_LayoutList
 *  List EEPROM layout.
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEPROMCLI_LayoutList(INT32 i4Argc, const CHAR **szArgv)
{
    DTVCFG_T rDtvCfg;
    UINT32 u4Idx = 0;
    UINT32 u4Offset = 0;
    INT32 i4Ret;

    i4Ret = EEPDTV_GetCfg(&rDtvCfg);
    if (i4Ret)
    {
        Printf("Eeprom layout read failed\n");
        return 1;
    }

    //     IDX NAME                          ID TYPE ELEM  EEP     OFF  AOFF
    Printf("%-10s %-40s %-10s %-10s %-10s\n", "IDX", "NAME", "TYPE", "ELEM", "OFF");
    for (u4Offset = 0; u4Offset < 80; u4Offset++)
    {
        Printf("-");
    }
    Printf("\n");

    if(DRVCUST_InitGet(eEepromHdmiHdcpSize) != 0)
    {
        u4Offset = DRVCUST_InitGet(eEepromHdmiHdcpOffset);
        _PrintElement(u4Idx++, "HDCP", EEP_ELE_TYPE_U8, DRVCUST_InitGet(eEepromHdmiHdcpSize), u4Offset);
    }

    if(DRVCUST_InitGet(eEepromDtvDriverSize) != 0)
    {
        u4Offset = DRVCUST_InitGet(eEepromDtvDriverOffset);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_MAGIC1", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_MAGIC1);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_MAGIC2", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_MAGIC2);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_FLAGS", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_UPGRADE", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_UPGRADE);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_UPGRADE_EXT", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_UPGRADE_EXT);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_FLAG2", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS2);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_FLAG3", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS3);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_FLAG4", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS4);
        _PrintElement(u4Idx++, "EEPDTV_DTVCFG_CHECKSUM", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_CHECKSUM);

        u4Offset += sizeof(DTVCFG_T);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_MAGIC1", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_MAGIC1);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_MAGIC2", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_MAGIC2);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_FLAGS", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_UPGRADE", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_UPGRADE);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_UPGRADE_EXT", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_UPGRADE_EXT);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_FLAG2", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS2);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_FLAG3", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS3);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_FLAG4", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS4);
        _PrintElement(u4Idx++, "2nd EEPDTV_DTVCFG_CHECKSUM", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_CHECKSUM);

        u4Offset += sizeof(DTVCFG_T);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_MAGIC1", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_MAGIC1);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_MAGIC2", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_MAGIC2);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_FLAGS", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_UPGRADE", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_UPGRADE);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_UPGRADE_EXT", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_UPGRADE_EXT);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_FLAG2", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS2);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_FLAG3", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS3);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_FLAG4", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_FLAGS4);
        _PrintElement(u4Idx++, "3rd EEPDTV_DTVCFG_CHECKSUM", EEP_ELE_TYPE_U8, 1, u4Offset + EEPDTV_DTVCFG_CHECKSUM);
    }

    if(DRVCUST_InitGet(eEepromNptvDriverSize) != 0)
    {
        u4Offset = DRVCUST_InitGet(eEepromNptvDriverOffset);
        _PrintElement(u4Idx++, "NPTVCFG", EEP_ELE_TYPE_U8, DRVCUST_InitGet(eEepromNptvDriverSize), u4Offset);
    }

    if(DRVCUST_InitGet(eEepromErrorCodeSize) != 0)
    {
        u4Offset = DRVCUST_InitGet(eEepromErrorCodeOffset);
        _PrintElement(u4Idx++, "DRVCUST_EEPROM_ERROR_CODE", EEP_ELE_TYPE_U8, DRVCUST_InitGet(eEepromErrorCodeSize), u4Offset);
    }

    if(DRVCUST_InitGet(eEepromStateCodeSize) != 0)
    {
        u4Offset = DRVCUST_InitGet(eEepromStateCodeOffset);
        _PrintElement(u4Idx++, "DRVCUST_EEPROM_STATE_CODE", EEP_ELE_TYPE_U8, DRVCUST_InitGet(eEepromStateCodeSize), u4Offset);
    }

    if(DRVCUST_InitGet(eEepromCECMenuLanguageOffset) != 0)
    {
        _PrintElement(u4Idx++, "EEPROM_CEC_MENU_LANGUAGE", EEP_ELE_TYPE_U8, 1, DRVCUST_InitGet(eEepromCECMenuLanguageOffset));
    }

    if(DRVCUST_InitGet(eEepromCECEnableOffset) != 0)
    {
        _PrintElement(u4Idx++, "EEPROM_CEC_ENABLE", EEP_ELE_TYPE_U8, 1, DRVCUST_InitGet(eEepromCECEnableOffset));
    }

    if(DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset) != 0)
    {
        _PrintElement(u4Idx++, "EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE", EEP_ELE_TYPE_U8, 1, DRVCUST_InitGet(eEepromCECOneTouchPlayEnableOffset));
    }

    return 0;
}

#ifdef CLI_SUPPORT

static INT32 _EEPROMCLI_history(INT32 i4Argc, const CHAR **szArgv)
{
	Printf("\n");
	EEPROM_OpertionCount();
	 return 0;
}
#endif
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
CLI_EXEC_T arLAYOUTCmdTbl[] =
{
    DECLARE_CMD(_EEPROMCLI_LayoutList, list, l, "eeprom.layout.l"),
    DECLARE_END_ITEM(),
};

/// EEPROM CLI function list array.
static CLI_EXEC_T arEepromCmdTbl [] =
{
#ifdef CC_CLI
    DECLARE_CMD(_EEPROMCLI_Init, init, i, "eeprom.init"),
    DECLARE_CMD(_EEPROMCLI_Stop, stop, stop, "eeprom.stop"),
    DECLARE_CMD(_EEPROMCLI_Reset, reset, r, "eeprom.reset"),
    DECLARE_CMD(_EEPROMCLI_Diag, diag, d, "eeprom.diag"),
    DECLARE_CMD(_EEPROMCLI_Enable, enable, en, "eeprom.en"),
    DECLARE_CMD(_EEPROMCLI_Disable, disable, dis, "eeprom.dis"),
    DECLARE_CMD(_EEPROMCLI_TestSuit, test_suit, test, "eeprom.test"),
    DECLARE_CMD(_EEPROMCLI_Size, size, size, "eeprom.size"),
    DECLARE_CMD(_EEPROMCLI_Page, page, page, "eeprom.page"),
#endif

    DECLARE_CMD(_EEPROMCLI_Read, read, r, "eeprom.r [eeprom-offset] [memptr] [memlen]"),
	DECLARE_CMD(_EEPROMCLI_Write, write, w, "eeprom.w [eeprom-offset] [memptr] [memlen]"),
#ifdef CLI_SUPPORT
	DECLARE_CMD(_EEPROMCLI_history, history, h, "eeprom.h print the write bytes"),
#endif
    DECLARE_CMD(_EEPROMCLI_ReadByte, readbyte, rb, "eeprom.rb [eeprom-offset]"),
    DECLARE_G_CMD(_EEPROMCLI_WriteByte, writebyte, wb, "eeprom.wb [eeprom-offset] [byteval]"),
    DECLARE_CMD(_EEPROMCLI_Identify, identify, id, "eeprom.id"),
    DECLARE_G_CMD(_EEPROMCLI_BatchWrite, batch write, bw, "eeprom.bw"),
    DECLARE_G_CMD(_EEPROMCLI_Dump, dump, dump, "eeprom.dump"),
    DECLARE_G_CMD(_EEPROMCLI_SetCfg, setcfg, setcfg, "eeprom.setcfg"),
    DECLARE_CMD(_EEPROMCLI_UartWrite, uartwrite, uw, "Get data from Uart and write to eeprom."),
    DECLARE_SUBLIST(LAYOUT, layout, l, "Layout command"),
    DECLARE_CMD(_EEPROMCLI_Measure, measure, m, "eeprom.m [loopcnt] for stress test"),

    DECLARE_END_ITEM(),
};

/// EEPROM CLI command structure.
CLI_MAIN_COMMAND_ITEM(Eeprom)
{
    "eeprom", NULL, NULL, arEepromCmdTbl, "Eeprom command", CLI_GUEST
};

LINT_EXT_HEADER_END


