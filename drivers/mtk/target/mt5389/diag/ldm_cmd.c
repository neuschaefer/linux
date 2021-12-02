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
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_os.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#define DEFINE_IS_LOG   LDM_IsLog
#include "x_debug.h"
#include "x_util.h"
// #include "x_chr_dev.h"

#include "x_timer.h"
#include "drv_common.h"
#include "x_ckgen.h"
// #include "storage_if.h"

#if defined(CC_MT5365) || defined(CC_MT5395)

#include "ldm_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define DEFINE_IS_LOG   LDM_IsLog
#define LDM_BASE                              (BIM_BASE)

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define CKGEN_MASK32(offset, value, mask) \
        CKGEN_WRITE32(offset, (CKGEN_READ32(offset) & ~mask) | value)
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
//__align(16) static UINT8 _au1GoldenBuf[SFLASH_TESTBUF_SIZE];
//__align(16) static UINT8 _au1TestBuf[SFLASH_TESTBUF_SIZE];

//-------------------------------------------------------------------------
/** _LDMCLI_Init
 */
//-------------------------------------------------------------------------
static INT32 _LDMCLI_Init(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Panel;
    if(i4Argc != 2)
    {
        Printf("ldm.init type    (0: LGD8x10, 1: LGD8x2, 2: LGD6x2, 3: TPV 16x2)\n");
        return 1;
    }

    u4Panel = StrToInt(szArgv[1]);

    switch (u4Panel) {
        case 0:
            Printf("[LDM] LDM init: LGD8x10\n");
            //u4LDMHW_Init(LDM_PANEL_LGD, 8, 10);
            break;
        case 1:
            Printf("[LDM] LDM init: LGD8x2\n");
            //u4LDMHW_Init(LDM_PANEL_LGD, 8, 2);
            break;
        case 2:
            Printf("[LDM] LDM init: LGD6x2\n");
            //u4LDMHW_Init(LDM_PANEL_LGD, 6, 2);
            break;
        case 3:
            Printf("[LDM] LDM init: AS3693 16x2\n");
            //u4LDMHW_Init(LDM_PANEL_AMS, 16, 2);
            break;
        default:
            Printf("[LDM] Unknown panel type\n");
            break;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** _LDMCLI_Pinmux
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _LDMCLI_Pinmux(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4platform;

    if(i4Argc != 2)
    {
        Printf("ldm.pinmux type  (0 or 1)\n");
        return 1;
    }
    u4platform = StrToInt(szArgv[1]);

    Printf("[LDM] Set pinmux type %d\n", u4platform);
    switch (u4platform) {
        case 0:
#if defined(CC_MT5365)
            // LED_VSYNC
            CKGEN_MASK32(REG_PINMUX_SEL1, 0x00030000, 0x00030000);
            // LED_CLK
            CKGEN_MASK32(REG_PINMUX_SEL3, 0x30000000, 0x30000000);
            // LED_DOUT , LED_CS
            CKGEN_MASK32(REG_PINMUX_SEL4, 0x0C000000, 0x0C000000);

            Printf("[LDM] VSYNC, CS, CLK, DOUT: ");
            Printf("DEMOD_TSCLK, DEMOD_TSDATA0, DEMOD_TSVAL, DEMOD_TSSYNC\n");
#elif defined(CC_MT5395)
            // bit24: LDM_CS, bit25: LDM_DO/LDM_CLK, bit27: LDM_VSYNC
    #ifndef REG_PINMUX_SEL1
        #define REG_PINMUX_SEL1 0x0404
    #endif
            CKGEN_MASK32(REG_PINMUX_SEL1, 0x0B000000, 0x0B000000);
            Printf("[LDM] VSYNC, CS, CLK, DOUT: ");
            Printf("monA25, monA22, monA24, monA23\n");
#endif
            break;
        case 1:
#if defined(CC_MT5365)
            // PINMUX5[11] = 1: LED_VSYNC, LED_CLK, LED_DOUT, LED_CS
            CKGEN_MASK32(REG_PINMUX_SEL5, 0x0800, 0x0800);
            Printf("[LDM] VSYNC, CS, CLK, DOUT: ");
            Printf("GPIO20, GPIO23, GPIO21, GPIO22\n");
#elif defined(CC_MT5395)
            Printf("[LDM] Pinmux type is NOT support yet\n");
#endif
            break;
        default:
            Printf("[LDM] Pinmux type is NOT support yet\n");
            break;
    }
    return 0;
}

//-------------------------------------------------------------------------
/** _LDMCLI_Dark
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _LDMCLI_Dark(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Len;
    UINT32 u4Val;
    Printf("[LDM] Enter manual mode and set all dimming value to 0\n");

    LDM_MASK32(0x2b8, 0, 0x4);
    u4Val = LDM_READ32(0x2b0);
    u4Len = ((u4Val >> 20) & 0x3ff) * ((u4Val >> 4) & 0x3ff); // N * M
    LDM_WRITE32(0x290, 0);
    for (i=0; i<u4Len; i++)
    {
        LDM_WRITE32(0x298, 0);
    }

    LDM_WRITE32(0x290, 0);
    return 0;
}

//-------------------------------------------------------------------------
/** _LDMCLI_SetValue
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _LDMCLI_SetValue(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i, u4Len, u4Val;

    if(i4Argc != 2 && i4Argc != 3)
    {
        Printf("ldm.setvalue value [repeat]\n");
        return 1;
    }

    u4Val = StrToInt(szArgv[1]);
    if (i4Argc == 3)
    {
        u4Len = StrToInt(szArgv[2]);
    }
    else
    {
        u4Len = 1;
    }

    Printf("[LDM] SetValue %d (%d time(s))\n", u4Val, u4Len);
    for (i=0; i<u4Len; i++)
    {
        LDM_WRITE32(0x298, u4Val);
    }
    return 0;
}

//-------------------------------------------------------------------------
/** _LDMCLI_SWMode
 *  @param  i4Argc      user's CLI input length.
 *  @param  szArgv      user's CLI input string.
 *  @retval  0  SUCCESS.
 *  @retval  1  FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _LDMCLI_SWMode(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 fgEnable;

    if(i4Argc != 2)
    {
        Printf("ldm.sw enable  (0: Disable, 1: Enable)\n");
        return 1;
    }
    fgEnable = StrToInt(szArgv[1]);

    Printf("[LDM] Software Mode %s\n", fgEnable ? "Enable" : "Disable");
    //vLDMHW_SetISR(fgEnable);
    return 0;
}

#if 0
static INT32 _LDMCLI_AS3693(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i;
    UINT8 u1Addr, u1Reg;
    UINT8 data[128];

    if (i4Argc < 4)
    {
        Printf("ldm.as addr reg [data0, ... ]\n");
        return 1;
    }

    u1Addr = (UINT8)StrToInt(szArgv[1]);
    u1Reg  = (UINT8)StrToInt(szArgv[2]);
    for (i=0; i<128 && i < i4Argc-3; i++)
    {
        data[i] = StrToInt(szArgv[i+3]);
    }

    Printf("[LDM] send AS3693 SPI to Addr: %02X, Reg: %02X, len: %d\n", u1Reg, u1Addr, i);
    _LDMHW_AS3693SPI(u1Addr, u1Reg, data, i);
    return 0;
}
#endif

#endif // #if defined(CC_MT5365) || defined(CC_MT5395)
/******************************************************************************
* Command table
******************************************************************************/
//CLIMOD_DEBUG_FUNCTIONS(LDM)
static CLI_EXEC_T arLdmCmdTbl[] = {
#if defined(CC_MT5365) || defined(CC_MT5395)
//    CLIMOD_DEBUG_CLIENTRY(LDM),
    DECLARE_CMD(_LDMCLI_Init, init, i, "init panel_type"),
    DECLARE_CMD(_LDMCLI_Pinmux, pinmux, pm, "pinmux type"),
    DECLARE_CMD(_LDMCLI_Dark, dark, dark, "dark"),
    DECLARE_CMD(_LDMCLI_SetValue, setvalue, sv, "setval value [repeat]"),
    DECLARE_CMD(_LDMCLI_SWMode, swmode, sw, "sw enable  (0: Disable, 1: Enable)"),
    //DECLARE_CMD(_LDMCLI_AS3693, as3693, as, "ldm.as addr reg [data0, ... ] , set data to AS3693 SPI"),
#endif // #if defined(CC_MT5365) || defined(CC_MT5395)
    DECLARE_END_ITEM(),
};

/*
 * Do not set static and Add "CLI_MAIN_COMMAND" to add command dir into cli.
 */
CLI_MAIN_COMMAND_ITEM(Ldm)
{
    "ldm", NULL, NULL, arLdmCmdTbl, "LDM command", CLI_GUEST
};

#if 0
/******************************************************************************
* Global Function
******************************************************************************/

#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetLdmMwCmdTbl(void)
#else
CLI_EXEC_T* GetLdmCmdTbl(void)
#endif
{
    return _arLdmModCmdTbl;
}
#endif

