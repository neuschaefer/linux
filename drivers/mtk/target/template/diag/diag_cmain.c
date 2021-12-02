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
 * $RCSfile: diag_cmain.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


/** @file diag_cmain.c
 *  diag_cmain.c is User main program
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_os.h"
#include "x_start_mini.h"
#include "x_printf.h"

#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_serial.h"
// #include "x_util.h"
#include "x_hal_5381.h"

//#include "fbm_drvif.h"
//#include "srm_drvif.h"

#include "x_lint.h"

//-----------------------------------------------------------------------------
// Config
//-----------------------------------------------------------------------------
#ifndef CC_DEMO_PROGRAM
#define CC_DEMO_PROGRAM     1
#endif /* CC_DEMO_PROGRAM */


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_HANDLE_NUM                  2048

#define CLI_MOD_NS (sizeof(_pfCliGetTbl)/sizeof(CLI_GET_CMD_TBL_FUNC))


//-----------------------------------------------------------------------------
// EXTERN variables
//-----------------------------------------------------------------------------

#ifndef __KERNEL__
EXTERN UINT32 Image$$RO$$Base;
EXTERN UINT32 Image$$RO$$Limit;
EXTERN UINT32 Image$$RW$$Base;
EXTERN UINT32 Image$$RW$$Limit;
EXTERN UINT32 Image$$BSS$$Base;
EXTERN UINT32 Image$$BSS$$Limit;
EXTERN UINT32 Image$$DATACLI$$Base;
EXTERN UINT32 Image$$DATACLI$$Limit;
#endif /* code not in Linux */

//-----------------------------------------------------------------------------
// EXTERN functions
//-----------------------------------------------------------------------------

// EXTERN CLI_EXEC_T* GetT32CmdTbl(void);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


#include "cli_list.h"

// static CLI_GET_CMD_TBL_FUNC _pfCliGetTbl[] =
// {
//     GetT32CmdTbl,
// };


static CLI_EXEC_T _rNullTbl = {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR};
static CLI_EXEC_T _arUserCmdTbl[CLI_MOD_NS + 1];


//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

#if 0 // ndef NDEBUG

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
static void _IntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4Addr)
{
    // Flush log data
    UTIL_LogFlush();

    // Show intrusion info
    Printf("MEMORY VIOLATION - agent: %s, address: 0x%08x\n",
        MID_AgentIdToString(eAgentId), u4Addr);
    Printf("System halted\n");

    // Reset MID
    VERIFY(MID_Reset());

    // Halt system
    BSP_HaltSystem();
}


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
static BOOL _EnableMemoryIntrusionDetection(void)
{
    UINT32 *pu4LowAddr, *pu4HighAddr;
    UINT32 u4LowAddr, u4HighAddr, u4Region;
    PFN_MID_CALLBACK pfnOrgHandler;

    // Compute the address range of the protected region
    pu4LowAddr = &Image$$RO$$Base;
    pu4HighAddr = &Image$$RO$$Limit;

    u4LowAddr = ((UINT32)pu4LowAddr + MID_ADDR_ALIGNMENT - 1) &
        ~(MID_ADDR_ALIGNMENT - 1);
    u4HighAddr = ((UINT32)pu4HighAddr - 1) & ~(MID_ADDR_ALIGNMENT - 1);

    // Init MID
    if (!MID_Init())
    {
        return FALSE;
    }

    // Register handler
    pfnOrgHandler = MID_RegisterHandler(_IntrudeHandler);
    UNUSED(pfnOrgHandler);

    // Allocate a read-only region
    if (!MID_AllocateFreeRegion(FALSE, &u4Region))
    {
        return FALSE;
    }

    // Set RO section to read-only
    if (!MID_SetRegion(u4Region, MID_AGENT_RESERVED, u4LowAddr, u4HighAddr))
    {
        return FALSE;
    }

    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        return FALSE;
    }

    return TRUE;
}

#endif  // NDEBUG


static void _InitCliInput(void* pvArg)
{
    UINT32 u4Idx;
    UINT32 u4CmdIdx;
    CLI_EXEC_T* prModCmdTbl;

#ifndef __KERNEL__
    Printf("5381 Diagnostic Program v0.1\n");
    Printf("RO:0x%08x-0x%08x RW:0x%08x-0x%08x ZI:0x%08x-0x%08x CLI:0x%08x-0x%08x\n",
                        (UINT32)&Image$$RO$$Base, (UINT32)&Image$$RO$$Limit,
                        (UINT32)&Image$$RW$$Base, (UINT32)&Image$$RW$$Limit,
                        (UINT32)&Image$$BSS$$Base, (UINT32)&Image$$BSS$$Limit,
                        (UINT32)&Image$$DATACLI$$Base, (UINT32)&Image$$DATACLI$$Limit);

    SerIsrReg();
    SerIsrEnable();
#endif /* code not in Linux  */

    CLI_Init();

    // initialize module command table
    for (u4Idx = 0; u4Idx < (UINT32)(CLI_MOD_NS + 1); u4Idx++)
    {
        _arUserCmdTbl[u4Idx] = _rNullTbl;
    }

    // install module command table
    u4CmdIdx = 0;
    for (u4Idx = 0; u4Idx < (UINT32)CLI_MOD_NS; u4Idx++)
    {
        ASSERT(_pfCliGetTbl[u4Idx] != NULL);
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

#if 0
    if (!_EnableMemoryIntrusionDetection())
    {
        Printf("Failed to initialize Memory Intrusion Detection!\n");
    }
#endif  // NDEBUG

#if 0
    if (GPIO_Init() != 0)
    {
        Printf("GPIO initialized failed\n");
    }

    // initail HDCP SRAM.
    if (EEPHDCP_Init() != 0)
    {
        Printf("EEPHDCP initialized failed\n");
    }
#endif

    SerTransparent();

#ifdef __KERNEL__
    // In Linux, kernel thread cannot terminated by itself.
    while (1)
    {
        x_thread_delay(0x0fffffff);
    }
#endif /* __KERNEL__ */

#ifdef __MODEL_slt__
    CLI_Parser("av.log 0");
    CLI_Parser("nav.i 3");
#endif

}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** x_sys_start() The entry point of user application
 *  @param pv_mem_addr: The start address of available memory
 *  @param z_mem_size: Size in byte of available memory
 *  @param pv_arg: Optional argument, not used
 */
//-----------------------------------------------------------------------------
VOID x_sys_start(VOID* pv_mem_addr, SIZE_T z_mem_size, VOID* pv_arg)
{
    HANDLE_T hThread1;

    // Initialize OSAI and handle libraries
    VERIFY(x_start_mini(NULL, MAX_HANDLE_NUM, pv_mem_addr, z_mem_size) == INITR_OK);

//    FBM_Init();

//    SRM_Init();

    // Run demo program
    if (x_thread_create(&hThread1, "InitCli", (16*1024), 100,
        _InitCliInput, 0, NULL) != OSR_OK)
    {
		Printf("\nError: Create Init CLI thread fail\n");
    }
}

