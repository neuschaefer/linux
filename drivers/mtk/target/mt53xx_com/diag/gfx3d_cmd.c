/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * Description:
 *         The CLI commands for gpio driver
 *
 *---------------------------------------------------------------------------*/

#include "x_serial.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_stl_lib.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif
#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"
#include "x_timer.h"
#include "x_pdwnc.h"
#include "u_dbg_drv.h"

#include "g3d_emu_if.h"

//===========================================================================
// Configuration
#define FPD_TEST_ENABLE             0


//---------------------------------------------------------------------

static INT32 _GFX3DCLI_Emu(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Ret;
    i4Ret = i4Graphics3D_Drv_Emu();
    return i4Ret;
}

static INT32 _GFX3DCLI_Init(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Ret;
    G3D_EMU_CONF_T rConf = {0, 0, 0, "Z:\\g3d_emulation\\g3d_emu_list.txt"};
    if(i4Argc >= 2)
    {
        rConf.fgMemoryStartFromZero = (BOOL)StrToInt(aszArgv[1]);        
    }
    if(i4Argc >= 3)
    {
        x_strncpy(rConf.szEmuListName, aszArgv[2], x_strlen(aszArgv[2]));
        Printf("Emu List: %s\n", rConf.szEmuListName);        
    }    
    if(rConf.fgMemoryStartFromZero)
    {
        Printf("Use internal MMU!\n");
    }
    else
    {
        Printf("Use external MMU!\n");    
    }
    i4Ret = i4Graphics3D_Drv_Init(rConf);
    return i4Ret;
}

static INT32 _GFX3DCLI_Reset(INT32 i4Argc, const CHAR** aszArgv)
{
    //reset
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);        
    x_thread_delay(100);
    return 0;
}

static BOOL _fgLooping = TRUE;
static HANDLE_T _hGfx3dThread = 0x0;

static void _GFX3DThread(void* pvArg)
{
    INT32 i4Ret;
    UNUSED(i4Ret);
    while(1)
    {
        if(_fgLooping)
        {
            i4Ret = i4Graphics3D_Drv_Emu();
        }
        else
        {
            x_thread_delay(1000);
        }
    }
}

static INT32 _GFX3DCLI_LoopTest(INT32 i4Argc, const CHAR** aszArgv)
{
    INT32 i4Ret;
    G3D_EMU_CONF_T rConf = {0, 0, 0, "Z:\\g3d_emulation\\g3d_emu_list.txt"};
    if(i4Argc >= 2)
    {
        _fgLooping = (BOOL)StrToInt(aszArgv[1]);        
    }

    if(_fgLooping)
    {
        Printf("Start 3D GFX loop test\n");
    }
    else
    {
        Printf("Pause 3D GFX loop test\n");    
    }

    i4Ret = i4Graphics3D_Drv_Init(rConf);
    //reset
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);        
    x_thread_delay(100);    
    i4Ret = i4Graphics3D_Drv_Emu();
    //reset
    IO_WRITE32(0xF0040000, 0x20, 0x20);
    IO_WRITE32(0xF0048000, 0x100c, 0x20);        
    x_thread_delay(100);            
    if(_hGfx3dThread == 0x0)
    {
        if(x_thread_create(&_hGfx3dThread, "GFX3D", 4096, CLI_THREAD1_PRIORITY, _GFX3DThread, 0, NULL) != OSR_OK)
        {
            return -1;
        }
    }
    return i4Ret;
}

//===========================================================================

static CLI_EXEC_T arGfx3dCmdTbl[] =
{
    DECLARE_CMD(_GFX3DCLI_Emu, emu, emu, "gfx3d.emu"),
    DECLARE_CMD(_GFX3DCLI_Init, init, init, "gfx3d.init"),
    DECLARE_CMD(_GFX3DCLI_Reset, reset, reset, "gfx3d.reset"),    
    DECLARE_CMD(_GFX3DCLI_LoopTest, looptest, looptest, "gfx3d.looptest"),    
    DECLARE_END_ITEM(),
};

CLI_MAIN_COMMAND_ITEM(Gfx3d)
{
	"gfx3d",
	NULL,
	NULL,
	arGfx3dCmdTbl,
	"Gfx3d interface",
	CLI_GUEST
};

