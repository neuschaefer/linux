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
 *         The main routine of boot loader
 *
 *---------------------------------------------------------------------------*/

#include "x_os.h"
#include "u_irrc_btn_def.h"
#include "x_printf.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_serial.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_pdwnc.h"
#include "x_pinmux.h"
#include "x_gpio.h"
// #include "ir_if.h"
#include "nor_if.h"
#include "eeprom_if.h"
#include "drvcust_if.h"

// #include "zip.h"
// #include "xmodem.h"
// #include "loader_imghdr.h"
#include "loader_if.h"

//---------------------------------------------------------------------
// Config
//---------------------------------------------------------------------
#ifndef CC_EXTERNAL_WAKEUP_STATUS
#define CC_EXTERNAL_WAKEUP_STATUS       0
#endif /* CC_EXTERNAL_WAKEUP_STATUS */

#ifndef CC_ENABLE_EFUSE_DECODE
#define CC_ENABLE_EFUSE_DECODE          0
#endif /* CC_ENABLE_EFUSE_DECODE */

#ifndef CC_POWER_BTN_DIRECTLY_BOOT
#define CC_POWER_BTN_DIRECTLY_BOOT      1
#endif /* CC_POWER_BTN_DIRECTLY_BOOT */

//---------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------

#define DBG_LOG(x...)                   // Printf(x)

#define LOADER_VERSION_MAJOR            0

#define LOADER_VERSION_MINOR            2

#define CLI_PROMPT_STRING               "BOOT"

#define MAX_HANDLE_NUM                  2048

#define KEY_ESCAPE                      0x1b

#ifdef NDEBUG   // release version
#define BOOT_WAIT_TIME                  100         // 0.1 second
#else           // debug version
#define BOOT_WAIT_TIME                  1000        // 1 second
#endif

#define MAX_IMAGE_SIZE                  0x280000    // 2.5MB

#define FIRMWARE_FLASH_OFFSET           LOADER_MAX_SIZE

// #define FIRMWARE_EXEC_ADDR              0x2000      // 8KB
// #define FIRMWARE_EXEC_ADDR              0x8000      // 32KB

//---------------------------------------------------------------------
// External declarations
//---------------------------------------------------------------------
extern BOOL SerInByte(UCHAR* puc);
extern INT32 XMODEM_Receive(UCHAR *dest, INT32 destsz);

static INT32 _CmdPrintChipId(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("%08x %08x\n", BIM_READ32(REG_RO_CHIP_ID), BIM_READ32(REG_RO_FPGA_ID));
    return 0;
}

#if 0
/*----------------------------------------------------------------------------
 * Function: _EraseFlash
 *
 * Description:
 *      Erase a block of NOR Flash
 *
 * Inputs:
 *      u4Offset: The Flash address offset to be erased
 *      u4ByteLen: The length to be erased
 *
 * Outputs: -
 *
 * Returns:
 *      0: Successful
 *      Others: Failed
 *---------------------------------------------------------------------------*/
static INT32 _EraseFlash(UINT32 u4Offset, UINT32 u4ByteLen)
{
    SECTTBL_T rNorMap;
    INT32 i4Ret;
    UINT32 u4Seg, u4SegStart, u4SegEnd, u4Sect, u4SectStart, u4SectEnd, u4Size;

    i4Ret = 0;

    i4Ret = NOR_EraseMap(&rNorMap);
    if (i4Ret) {
        return 1;
    }

    u4Seg = u4Sect = u4Size = 0;
    u4SegStart = u4SegEnd = 0;
    for (u4Seg = 0; u4Seg < rNorMap.u4SegNum; u4Seg++) {
        u4SegEnd += (rNorMap.prSegInfo[u4Seg].u4SectNum * rNorMap.prSegInfo[u4Seg].u4SectSize);
        if (u4SegEnd <= u4Offset) {
            u4Size += (u4SegEnd - u4SegStart);
            u4SegStart = u4SegEnd;
            continue;
        }

        u4SectStart = u4SectEnd = u4SegStart;
        for (u4Sect = 0; u4Sect < rNorMap.prSegInfo[u4Seg].u4SectNum; u4Sect++) {
            u4SectEnd = u4SectStart + rNorMap.prSegInfo[u4Seg].u4SectSize;
            if (u4SectEnd <= u4Offset) {
                u4SectStart = u4SectEnd;
                u4Size += rNorMap.prSegInfo[u4Seg].u4SectSize;
                continue;
            }
            if (u4SectStart >= u4Offset + u4ByteLen) {
                u4SectStart = u4SectEnd;
                u4Size += rNorMap.prSegInfo[u4Seg].u4SectSize;
                continue;
            }
            i4Ret = NOR_EraseBlk(u4SectStart);
            if (i4Ret) {
                return 1;
            }
            u4SectStart = u4SectEnd;
            u4Size += rNorMap.prSegInfo[u4Seg].u4SectSize;
        }
        u4SegStart = u4SegEnd;
    }
    return 0;
}
#endif


//---------------------------------------------------------------------
// CLI commands
//---------------------------------------------------------------------


/*----------------------------------------------------------------------------
 * Function: _CmdUpgrade
 *
 * Description:
 *      The command to upgrade firmware
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static INT32 _CmdUpgrade(INT32 i4Argc, const CHAR** aszArgv)
{
#if 0
    IMAGE_HEADER_T* prHeader;
    void* pvBuf;
    INT32 i4Status, i4ZipState, i4Ret = -1;
    UINT32 u4ImageSize, u4DstSize, u4FirmwareExecAddr;

    pvBuf = x_mem_alloc(MAX_IMAGE_SIZE);
    if (pvBuf == NULL)
    {
        Printf("Out of memory!\n");
        return -1;
    }

    Printf("Waiting terminal to send file using XMODEM or XMODEM-1K...");

    SerIsrDisable();
    i4Status = XMODEM_Receive((UCHAR*)pvBuf, (INT32)MAX_IMAGE_SIZE);
    SerIsrEnable();

    if (i4Status < 0)
    {
        Printf("\nXmodem error: %s\n", XMODEM_GetErrorString(i4Status));
        goto _Exit;
    }

    Printf("\n%d bytes received\n", i4Status);

    // Checking if it's a valid zip file
    Printf("Verifying...");
    if (i4Status > MAX_IMAGE_SIZE)
    {
        Printf("\nFile too large! (limit is %u bytes)\n", MAX_IMAGE_SIZE);
        goto _Exit;
    }

    prHeader = (IMAGE_HEADER_T*)pvBuf;
    if (prHeader->u4Signature != IMAGE_SIGNATURE_2)
    {
        Printf("\nFile without Image Header, 0x%08x\n", prHeader->u4Signature);
        goto _Exit;
    }

    u4FirmwareExecAddr = prHeader->u4FirmwareExecAddr;

    i4ZipState = ZIP_Decompress((UINT32)pvBuf + sizeof (IMAGE_HEADER_T),
        i4Status, u4FirmwareExecAddr, &u4DstSize);
    if (i4ZipState != Z_OK)
    {
        Printf("invalid zip file\n");
        goto _Exit;
    }

    // Erase Flash first
    Printf("\nErasing Flash...");
    u4ImageSize = (UINT32)i4Status + sizeof (IMAGE_HEADER_T);
    if (_EraseFlash(FIRMWARE_FLASH_OFFSET, u4ImageSize) != 0)
    {
        Printf("failed!\n");
        goto _Exit;
    }

    // Then write image to Flash
    Printf("\nWriting image to Flash (0x%08x - 0x%08x)...",
        FIRMWARE_FLASH_OFFSET, FIRMWARE_FLASH_OFFSET + u4ImageSize - 1);
    if (NOR_Write(FIRMWARE_FLASH_OFFSET, (UINT32)pvBuf, u4ImageSize) != 0)
    {
        Printf("failed!\n");
        goto _Exit;
    }
    Printf("OK\n");

    i4Ret = 0;

_Exit:

    x_mem_free(pvBuf);
    return i4Ret;
#else
    return 0;
#endif
}


/*----------------------------------------------------------------------------
 * Function: _CmdErase
 *
 * Description:
 *      The command to erase firmware
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static INT32 _CmdErase(INT32 i4Argc, const CHAR** aszArgv)
{
#if 0
    UCHAR uc;

    // Ask user to confirm
    Printf("Erase firmware in flash (yes/no) ? ");
    uc = SerPollGetChar();
    if (uc != 'y' && uc != 'Y')
    {
        return 0;
    }

    // Erase the image header
    Printf("Erasing firmware...");
    if (_EraseFlash(FIRMWARE_FLASH_OFFSET, sizeof (IMAGE_HEADER_T)) != 0)
    {
        Printf("failed!\n");
        return -1;
    }

    Printf("OK\n");
#endif
    return 0;
}


/*----------------------------------------------------------------------------
 * Function: _CmdBoot
 *
 * Description:
 *      The command to boot firmware
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static INT32 _CmdBoot(INT32 i4Argc, const CHAR** aszArgv)
{
    LDR_DATA_T   *prLdrData;
    UINT32 fgRet;

    // UART change to command mode.
    SerNormalMode();

    // Get prLdrData
    prLdrData = LDR_GetLdrData();

    // Try Bootup with loading image to dram.
    if ((fgRet = LDR_ImageDecompress(prLdrData)) == 0)
    {
        // jump to work.
        LDR_ImageJump(prLdrData);
        // never return if jump.
    }

    // UART change to transparent mode.
    SerTransparent();

    return fgRet ? 0 : -1;
}

#if 0
/*----------------------------------------------------------------------------
 * Function: _CmdHighSpeed
 *
 * Description:
 *      The command to turn uart into high speed mode (115200 x 8)
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static INT32 _CmdHighSpeed(INT32 i4Argc, const CHAR** aszArgv)
{
    RS_232_SETUP_INFO_T rUartSetupInfo;
    UART_GetParameter(UART_PORT_0, &rUartSetupInfo);
    rUartSetupInfo.e_speed = RS_232_SPEED_921600;
    UART_SetParameter(UART_PORT_0, &rUartSetupInfo);
    return 0;
}
#endif

#if 0
static INT32 _CmdDtvEepromCfg(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i, i4Val, i4Ret;
    UINT8 au1Buffer[64];

    i4Val = 64; // EEPDTV_GetSize();
    Printf("DTV CFG size %d bytes\n", i4Val);

    i4Val = (i4Val < 64) ? i4Val : 64;

    if (i4Argc < 2)
    {
        // set to non osai mode.
        // SIF_SetNonOSAI(TRUE);

        i4Ret = EEPROM_Read(EEPROM_DTV_DRIVER_OFFSET, (UINT32)au1Buffer, i4Val);

        // set to osai mode.
        // SIF_SetNonOSAI(FALSE);

        if (i4Ret)
        {
            Printf("Read failed\n");
            return 1;
        }
        for (i = 0; i<i4Val; i++)
        {
            if (i%8==0)
            {
                Printf("\n0x%02x | 0x%02x", i, (INT32)au1Buffer[i]);
            }
            else
            {
                Printf(" 0x%02x", (INT32)au1Buffer[i]);
            }
        }
    }
    else
    {
        for (i=0; i<i4Argc-1; i++)
        {
            au1Buffer[i] = StrToInt(aszArgv[i+1]);
            if (i%8==0)
            {
                Printf("\n0x%02x | 0x%02x", i, (INT32)au1Buffer[i]);
            }
            else
            {
                Printf(" 0x%02x", (INT32)au1Buffer[i]);
            }
        }
        i4Ret = EEPROM_Write(EEPROM_DTV_DRIVER_OFFSET, (UINT32)au1Buffer, i4Argc-1);
        if (i4Ret)
        {
            Printf("Write failed\n");
            return 1;
        }
    }
    Printf("\n\n");
    return 0;
}
#endif


static INT32 _CmdLogo(INT32 i4Argc, const CHAR** aszArgv)
{
    LDR_LogoDisplay(NULL, StrToInt(aszArgv[1]));
    return 0;
}

//---------------------------------------------------------------------
// CLI command declarations
//---------------------------------------------------------------------


const CLI_EXEC_T _arLoaderCmdTbl[] CLI_MAIN_COMMAND =
{
    {
        "upgrade",
        "u",
        _CmdUpgrade,
        NULL,
        "Upgrade firmware",
        CLI_GUEST
    },

    {
        "erase",
        "e",
        _CmdErase,
        NULL,
        "Erase firmware",
        CLI_SUPERVISOR
    },

    {
        "boot",
        "bt",
        _CmdBoot,
        NULL,
        "Boot firmware",
        CLI_GUEST
    },

    {
        "logo",
        "logo",
        _CmdLogo,
        NULL,
        "Display logo",
        CLI_GUEST
    },

    {
        "chipid",
        "chipid",
        _CmdPrintChipId,
        NULL,
        "print chip id",
        CLI_GUEST
    },

#if 0
    {
        "highspeed",
        "hs",
        _CmdHighSpeed,
        NULL,
        "High speed uart mode",
        CLI_GUEST
    },

    {
        "dtvcfg",
        "dtvcfg",
        _CmdDtvEepromCfg,
        NULL,
        "DTV EEPROM Configuration",
        CLI_SUPERVISOR
    },
#endif

    // last cli command record, NULL
//    {
//        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
//    }
};


//---------------------------------------------------------------------
// Internal declarations
//---------------------------------------------------------------------
static UINT32 au4ArrayZI[10];
static UINT32 au4ArrayRW[2] = { 0x12345678, 0x9abcdef0 };

EXTERN UINT32 INT_Reset;
EXTERN UINT32 INT_Vectors;

//===========================================================================
   extern CLI_EXEC_T DATACLI_Nor;
// extern CLI_EXEC_T DATACLI_Osd;
// extern CLI_EXEC_T DATACLI_Pmx;
// extern CLI_EXEC_T DATACLI_MemTest;
  extern CLI_EXEC_T DATACLI_Eeprom;
   extern CLI_EXEC_T DATACLI_Nand;
// extern CLI_EXEC_T DATACLI_rMUSB;

void TEST_Function(UINT32 fgBool)
{
    if (au4ArrayRW[0] && au4ArrayZI[0] && fgBool)
    {
//      x_fm_mount(0, NULL, 0, NULL);
        au4ArrayRW[0] = 0;
        au4ArrayZI[0] = 0;
        Printf("0x%08x 0x%08x\n", &INT_Reset, &INT_Vectors,
            DATACLI_Nor,
//             DATACLI_Osd,
//              DATACLI_Pmx,
//            DATACLI_MemTest,
             DATACLI_Eeprom,
#ifdef CC_NAND_ENABLE
             DATACLI_Nand,
#endif
//            DATACLI_rMUSB,
            _arLoaderCmdTbl
            );
    }
}

