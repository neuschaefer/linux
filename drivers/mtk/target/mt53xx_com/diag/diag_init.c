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

#include "x_os.h"
#include "x_start_mini.h"
#include "x_printf.h"
#include "x_hal_5381.h"
#include "x_mid_cli.h"
#include "x_assert.h"
#include "x_serial.h"

#include "x_timer.h"

#if 0
#include "mpv_cmd.h"
#include "sif_cmd.h"
#include "psr_cmd.h"
#include "aud_cmd.h"
#include "dmx_cmd.h"
#include "vdp_cmd.h"
#include "fbm_cmd.h"
#include "pmx_cmd.h"
#include "task_cmd.h"
#include "dbs_cmd.h"
#include "osd_cmd.h"
#include "gfx_cmd.h"
#include "gfxsc_cmd.h"
/*
#include "vin_cmd.h"
#include "pod_cmd.h"
#include "ide_cmd.h"
*/
#include "fci_cmd.h"
#include "nor_cmd.h"
/*
#include "nand_cmd.h"
*/
#include "ir_cmd.h"
/*
#include "i1394_cmd.h"
*/
#include "av_cmd.h"
/*
#include "dhrystone_cmd.h"
#include "jpg_cmd.h"
*/
#include "nim_cmd.h"
/*
#include "t32_cmd.h"
#include "nvgt_cmd.h"
#include "zip_cmd.h"
#include "fat_cmd.h"
*/
#include "uart_cmd.h"
//#include "crypto_cmd.h"
#include "eeprom_cmd.h"
#include "mid_cmd.h"
#include "gpio_cmd.h"
#include "memtest_cmd.h"
#include "nptv_cmd.h"
#include "msh_cmd.h"
#include "rtc_cmd.h"
#include "bwt_cmd.h"
#include "custom_cmd.h"
#endif

#include "x_mid.h"
#include "x_bim.h"
#include "x_util.h"

/******************************************************************************
* Configuration defintions.
******************************************************************************/

#ifndef ENABLE_VECTOR_N_TEXT_MEM_PROTECT
#define ENABLE_VECTOR_N_TEXT_MEM_PROTECT    0  /* 1 to enable memory protect */
#endif /* ENABLE_VECTOR_N_TEXT_MEM_PROTECT */

#define ENABLE_PROTECT_HEAP_AREA            0


/******************************************************************************
* Constant definitions
******************************************************************************/

#define MAX_HANDLE_NUM                  2048

#define CLI_MOD_NS      (sizeof(_pfCliGetTbl)/sizeof(CLI_GET_CMD_TBL_FUNC))

extern CLI_EXEC_T* GetCustomMwCmdTbl(void);

/******************************************************************************
* cli module table
******************************************************************************/

#include "mw_cli_list.h"

#if 0
CLI_GET_CMD_TBL_FUNC _pfCliGetTbl[] =
{
#if 0
	GetDmxMwCmdTbl,
	GetMpvMwCmdTbl,
	GetFbmMwCmdTbl,
	GetVdpMwCmdTbl,
	GetPmxMwCmdTbl,
/*
    GetPsrMwCmdTbl,
*/
	GetAudMwCmdTbl,
    GetOsdMwCmdTbl,
	GetGfxMwCmdTbl,
	GetGfxScMwCmdTbl,
/*
	GetPodMwCmdTbl,
        GetCryptoCmdTbl,
*/
	GetSifMwCmdTbl,
/*
	GetIdeCmdTbl,
*/
	GetFciCmdTbl,
	GetNorMwCmdTbl,
	GetIrMwCmdTbl,
/*
	GetNandCmdTbl,
*/
	GetAvMwCmdTbl,
/*
//    GetI1394CmdTbl,

        GetDhrystoneCmdTbl,
	GetJpgCmdTbl,
*/
	GetNimMwCmdTbl,
/*
	GetNvgtCmdTbl,
	GetT32CmdTbl,
        GetZipCmdTbl,
        GetFatCmdTbl,
        GetUartCmdTbl,
*/
    GetEepromMwCmdTbl,
    GetMidMwCmdTbl,
    GetDbsMwCmdTbl,
    GetGpioMwCmdTbl,
    GetMemTestMwCmdTbl,
    GetNptvMwCmdTbl,
	GetTaskMwCmdTbl,
    GetScriptMwCmdTbl,
    GetRtcMwCmdTbl,
    GetBwtMwCmdTbl,
#endif
    GetCustomMwCmdTbl,
};
#endif

// new protect regions
// 0x00000000 ~ 0x00000040 -> vector table, set readonly.
// Image$$RO$$Base ~ Image$$RO$$Limit -> code text or readonly data, set readonly.
// Image$$RW$$Base ~ End of memory size -> Heap, set CPU and GFX only modify.

#ifndef __KERNEL__
extern UINT32 Image$$RO$$Base;
extern UINT32 Image$$RW$$Base;
extern UINT32 Image$$RO$$Limit;
#endif //__KERNEL__

#if ENABLE_PROTECT_HEAP_AREA
extern UINT32 g_u4DramSize;
#endif /* ENABLE_PROTECT_HEAP_AREA */

#ifndef NDEBUG  // debug version, must set MMU as read only.
//extern void* g_pvPageTable;
#endif

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *      The memory intrusion handler
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
#ifndef CC_MEM_MID_DISABLE
static void _IntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4Addr)
{
    HAL_TIME_T dt;

    // Flush log data
    UTIL_LogFlush();

    HAL_GetTime(&dt);
    // Show intrusion info
    Printf("%u.%06u s MEMORY VIOLATION - agent: %s(%d), address: 0x%08x\n",
        dt.u4Seconds, dt.u4Micros, MID_AgentIdToString(eAgentId), (UINT32)eAgentId, u4Addr);
    Printf("System halted\n");

    // Reset MID
    // MID_Reset();

    // Halt system
    BSP_HaltSystem();
}
#endif

/*----------------------------------------------------------------------------
 * Function:
 *
 * Description:
 *      Enable Memory Intrusion Detection for RO region
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Succeed
 *      FALSE: Fail
 *---------------------------------------------------------------------------*/
#ifndef CC_MEM_MID_DISABLE
static BOOL _EnableMemoryIntrusionDetection(void)
{
    UINT32 *pu4LowAddr, *pu4HighAddr;
    UINT32 u4LowAddr, u4HighAddr, u4Region;
    PFN_MID_CALLBACK pfnOrgHandler;

    // Init MID
    if (!MID_Init())
    {
        return FALSE;
    }

    // Register handler
    pfnOrgHandler = MID_RegisterHandler(_IntrudeHandler);
    UNUSED(pfnOrgHandler);

#ifndef __KERNEL__
    // Compute the address range of the protected region
    pu4LowAddr = &Image$$RO$$Base;
#ifdef NDEBUG   //release version, protect until RW base.
    pu4HighAddr = &Image$$RW$$Base;
#else
    // debug version only detect RO region.
    pu4HighAddr = &Image$$RO$$Limit;
#endif

    Printf("\nImage ROBase:0x%08x ROLimit:0x%08x\n", (UINT32)pu4LowAddr,
                (UINT32)pu4HighAddr);
#endif //__KERNEL__


//    u4LowAddr = ((UINT32)pu4LowAddr + MID_ADDR_ALIGNMENT - 1) &
//        ~(MID_ADDR_ALIGNMENT - 1);
    u4LowAddr = 0;
//    u4HighAddr = ((UINT32)pu4HighAddr - MID_ADDR_ALIGNMENT + 1) & ~(MID_ADDR_ALIGNMENT - 1);
    // u4HighAddr must be aligned with 32bytes to follow data cache-line size.
    u4HighAddr = ((UINT32)pu4HighAddr & (~0x1FU));

#if 1 // def NDEBUG  // release version use read only region.
    if (!MID_AllocateFreeRegion(TRUE, &u4Region))
#else
    // debug version allocate a read/write region, for debugging
    if (!MID_AllocateFreeRegion(FALSE, &u4Region))
#endif
    {
        return FALSE;
    }

#if 1 // def NDEBUG  // release version use read only region.
    // Set RO section to read-only
    if (!MID_SetRegion(u4Region, MID_AGENT_NONE, u4LowAddr, u4HighAddr))
#else
    // Set RW section to read/write detect
    if (!MID_SetRegion(u4Region, MID_AGENT_NONE, u4LowAddr, u4HighAddr))
#endif
    {
        return FALSE;
    }
    Printf("Protect readonly memory from 0x%08x to 0x%08x\n", u4LowAddr, u4HighAddr);

    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        return FALSE;
    }

#if 0
    u4LowAddr = 0;
    u4HighAddr = 0x40;
    // Allocate a read-only region
    if (!MID_AllocateFreeRegion(TRUE, &u4Region))
    {
        return FALSE;
    }
    Printf("Protect memory from 0x00 to 0x40\n");

    // Set RO section to read-only
    if (!MID_SetRegion(u4Region, MID_AGENT_NONE, u4LowAddr, u4HighAddr))
    {
        return FALSE;
    }

    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        return FALSE;
    }
#endif

    u4LowAddr = (UINT32)(BSP_GetMemSize() - 0x4000); // &g_pvPageTable;
    u4HighAddr = 0xFFFFFFFFU; // dram size to 4GB are readonly.
    u4LowAddr = (u4LowAddr + MID_ADDR_ALIGNMENT - 1) & ~(MID_ADDR_ALIGNMENT - 1);
    u4HighAddr = (u4HighAddr - MID_ADDR_ALIGNMENT + 1) & ~(MID_ADDR_ALIGNMENT - 1);
    // Allocate a read-only region
    if (!MID_AllocateFreeRegion(TRUE, &u4Region))
    {
        return FALSE;
    }
    Printf("MMU protect: 0x%08x ~ 0x%08x\n", u4LowAddr, u4HighAddr);
    // Set RO section to read-only
    if (!MID_SetRegion(u4Region, MID_AGENT_NONE, u4LowAddr, u4HighAddr))
    {
        return FALSE;
    }
    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        return FALSE;
    }

#ifndef __KERNEL__
#if ENABLE_PROTECT_HEAP_AREA
    u4LowAddr = &Image$$RW$$Base;
    u4HighAddr = g_u4DramSize;
    Printf("Heap Region:0x%08x ~ 0x%08x\n", u4LowAddr, u4HighAddr);
    u4LowAddr = (u4LowAddr + MID_ADDR_ALIGNMENT - 1) & ~(MID_ADDR_ALIGNMENT - 1);
    u4HighAddr = (u4HighAddr - MID_ADDR_ALIGNMENT + 1) & ~(MID_ADDR_ALIGNMENT - 1);

    // Allocate a read-only region
    if (!MID_AllocateFreeRegion(FALSE, &u4Region))
    {
        return FALSE;
    }

    // Set RO section to read-only
    if (!MID_SetRegionEx(u4Region, MID_AGENT_GFX, u4LowAddr, u4HighAddr, MID_FLAG_CPUACCESS))
    {
        return FALSE;
    }
    Printf("Protect readonly memory from 0x%08x to 0x%08x\n", u4LowAddr, u4HighAddr);

    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        return FALSE;
    }
#endif /* ENABLE_PROTECT_HEAP_AREA */
#endif //__KERNEL__

#if ENABLE_VECTOR_N_TEXT_MEM_PROTECT
    /* Keep MID interrupt enable. */
#else
    /* Disable MID interrupt routine. */
    BIM_DisableIrq(VECTOR_DRAMC);
#endif /* ENABLE_VECTOR_N_TEXT_MEM_PROTECT */
    return TRUE;
}
#endif


/*---------------------------------------------------------------------------*/

CLI_EXEC_T _rNullTbl = {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR};
CLI_EXEC_T _arUserCmdTbl[CLI_MOD_NS + 1];

static INT32 _CLI_BasicStop(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	Printf("RS232 leave transparent mode! Console stop! \n");
	SerEnd();
	return 0;
//lint -e{818}
}

static INT32 _CLI_BasicEnTran(INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

    Printf("RS232 enter transparent mode! no 0 any more.\n");
    SerTransparent();
	return 0;
//lint -e{818}
}

/*----------------------------------------------------------------------------
 * Function: x_cli_start
 *
 * Description:
 *
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID x_cli_start(void)
{
	static CLI_EXEC_T arStopCmdTbl[] = {
        { "stop", "st", _CLI_BasicStop, NULL, "set UR0 to normal mode", CLI_GUEST },
        { "et", NULL, _CLI_BasicEnTran, NULL, "Enter RS232 transparent mode", CLI_GUEST },
        { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
    };

	UINT32 u4Idx;
	UINT32 u4CmdIdx;
	CLI_EXEC_T* prModCmdTbl;

/*
    // Initialize OSAI and handle libraries
    VERIFY(x_start_mini(NULL, MAX_HANDLE_NUM, pv_mem_addr, z_mem_size) == INITR_OK);

    Printf("5351 Diagnostic Program v0.5\n");
*/
	SerIsrReg();
	SerIsrEnable();

#ifndef CC_MEM_MID_DISABLE
    if (!_EnableMemoryIntrusionDetection())
    {
        Printf("Failed to initialize Memory Intrusion Detection!\n");
    }
    else
    {
        Printf("Successed to initialize Memory Intrusion Detection!\n");
    }
#endif /* CC_MEM_MID_DISABLE */

	CLI_Init();

	// install default command table
	// CLI_CmdTblAttach(CLI_GetDefaultCmdTbl());

	// initialize module command table
	for (u4Idx = 0; u4Idx < (CLI_MOD_NS + 1); u4Idx++)
	{
		_arUserCmdTbl[u4Idx] = _rNullTbl;
	}

	// install module command table
	u4CmdIdx = 0;
	for (u4Idx = 0; u4Idx < CLI_MOD_NS; u4Idx++)
	{
		prModCmdTbl = _pfCliGetTbl[u4Idx]();

		if ((prModCmdTbl != NULL) &&
			(prModCmdTbl->pszCmdStr != NULL) &&
			((prModCmdTbl->pfExecFun != NULL) || (prModCmdTbl->prCmdNextLevel != NULL)))
		{
			_arUserCmdTbl[u4CmdIdx] = *prModCmdTbl;
			u4CmdIdx++;
		}
	}

	CLI_CmdTblAttach(_arUserCmdTbl);
    x_cli_attach_cmd_tbl(_arUserCmdTbl, CLI_CAT_DRV, CLI_GRP_DRV);
    x_cli_attach_cmd_tbl(CLI_GetDefaultCmdTbl(), CLI_CAT_DRV, CLI_GRP_DRV);
    x_cli_attach_cmd_tbl(arStopCmdTbl, CLI_CAT_BASIC, CLI_GRP_NONE);
    x_cli_attach_cmd_tbl(arStopCmdTbl, CLI_CAT_MTK_TOOL, CLI_GRP_NONE);
}

//===========================================================================


#ifdef NO_MIDDLEWARE
#include "x_rm.h"
#include "x_rm_dev_types.h"

INT32 x_rm_reg_comp (DRV_COMP_REG_T*      pt_comp_id,
                        UINT8                ui1_num_inp_ports,
                        UINT8                ui1_num_out_ports,
                        const CHAR*          ps_comp_name,
                        UINT32               ui4_comp_flags,
                        DRV_COMP_FCT_TBL_T*  pt_comp_fct_tbl,
                        const VOID*          pv_comp_data,
                        SIZE_T               z_comp_data_len)
{
	return RMR_OK;
}

INT32 x_rm_reg_conn_list (const DRV_COMP_REG_T*  pt_comp_id,
                             const DRV_COMP_REG_T*  pt_conn_ids,
                             UINT16                 ui2_num_entries,
                             CONN_DIR_TYPE_T        e_conn_dir,
                             BOOL                   b_hard_wired)
{
	return RMR_OK;
}

INT32 x_rm_reg_comp_excl_list (const DRV_COMP_REG_T*  pt_comp_id,
                                  const DRV_COMP_REG_T*  pt_comp_excl_ids,
                                  UINT16                 ui2_num_entries)
{
	return RMR_OK;
}

INT32 x_rm_reg_conn_excl_list (const DRV_COMP_REG_T*  pt_comp_id,
                                  const DRV_COMP_REG_T*  pt_conn_excl_ids,
                                  UINT16                 ui2_num_entries)
{
	return RMR_OK;
}


INT32 x_rm_reg_group_name (const CHAR*            ps_group_name,
                              const DRV_COMP_REG_T*  pt_comp_ids,
                              UINT16                 ui2_num_entries)
{
	return RMR_OK;
}


#endif

