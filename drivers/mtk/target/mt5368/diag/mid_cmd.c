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
 * $RCSfile: mid_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mid_cmd.c
 *  mid_cmd.c Memory intrusion detection commands
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// #include "cli_debug.h"
#include "x_debug.h"
#include "x_mid.h"
#include "x_dram.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_hal_926.h"
#include "x_bim.h"
#include "x_assert.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

//-----------------------------------------------------------------------------
// LOG macro related
//-----------------------------------------------------------------------------

CLIMOD_DEBUG_FUNCTIONS(MID)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static PFN_MID_CALLBACK _pfnOrgHandler = NULL;

static BOOL _fgHaltOnIntrusion = TRUE;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _IntrudeHandler() The MID interrupt hook function
 *  @param u4Region the region number which is intruded.
 *  @param eAgentId the agent id which intruded.
 *  @param u4Addr the address which is intruded.
 */
//-----------------------------------------------------------------------------
static void _IntrudeHandler(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4Addr)
{
    // Handling the intrusion event
    Printf("MEMORY VIOLATION - agent: %s, address: 0x%08x\n",
        MID_AgentIdToString(eAgentId), u4Addr);

    if (_fgHaltOnIntrusion)
    {
        Printf("System halted\n");
        // VERIFY(MID_Reset());
        BSP_HaltSystem();
    }

    // Chain back to original handler
    if (_pfnOrgHandler != NULL)
    {
        _pfnOrgHandler(u4Region, eAgentId, u4Addr);
    }
}

//=====================================================================

//-----------------------------------------------------------------------------
/** _CmdInit() initialized command function
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdInit(INT32 i4Argc, const CHAR** aszArgv)
{
    static BOOL _fgInit = FALSE;
    BOOL fgOk;

    if (_fgInit)
    {
        Printf("Already initialized\n");
        return 0;
    }

    Printf("MID initializing...");

    fgOk = MID_Init();

    // Register handler
    if (fgOk)
    {
        _pfnOrgHandler = MID_RegisterHandler(_IntrudeHandler);
        _fgInit = TRUE;
    }

    Printf("%s\n", fgOk ? "Done" : "Failed");

    return fgOk ? 0 : -1;
}

//-----------------------------------------------------------------------------
/** _CmdExit() free all MID resource command function
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdExit(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgOk;

    Printf("MID exiting...");

    // De-register handler
    MID_RegisterHandler(_pfnOrgHandler);

    fgOk = MID_Exit();
    Printf("%s\n", fgOk ? "Done" : "Failed");

    return fgOk ? 0 : -1;
}

//-----------------------------------------------------------------------------
/** _CmdReset() reset MID module command
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdReset(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgOk;

    Printf("MID reseting...");

    fgOk = MID_Reset();
    Printf("%s\n", fgOk ? "Done" : "Failed");

    return fgOk ? 0 : -1;
}


//-----------------------------------------------------------------------------
/** _ShowSetRegionUsage() to print set region usage.
 */
//-----------------------------------------------------------------------------
static void _ShowSetRegionUsage(void)
{
    Printf("Usage: set low_addr length [allowed_agent] [region(0-%u)] [-i] [-c] [-w] [0xAGENTLIST]\n",
        MID_MAX_REGIONS - 1);
}

//-----------------------------------------------------------------------------
/** _CmdSetRegion() set region settings command function
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetRegion(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4LowAddr, u4HighAddr, u4Region, u4Flags, u4AgentList;
    MID_AGENT_ID_T eAgentId = MID_AGENT_NONE;
    BOOL fgOk;

    u4Flags = 0;
    u4AgentList = 0;

    if (i4Argc < 2 || i4Argc > 7)
    {
        _ShowSetRegionUsage();
        return -1;
    }

    u4LowAddr = (UINT32)StrToInt(aszArgv[1]);
    u4HighAddr = u4LowAddr + (UINT32)StrToInt(aszArgv[2]);
    
    if (i4Argc >= 4)
    {
        eAgentId = MID_StringToAgentId(aszArgv[3]);
        if (eAgentId == MID_AGENT_INVALID)
        {
            Printf("Invalid agent: %s\n", aszArgv[3]);
            return -1;
        }
    }

    u4Flags = 0;
    if (i4Argc > 5)
    {
        if ((aszArgv[5][0]=='-') && (aszArgv[5][1]=='i'))
        {
            u4Flags |= MID_FLAG_INVERSE;
        }
        if ((aszArgv[5][0]=='-') && (aszArgv[5][1]=='c'))
        {
            u4Flags |= MID_FLAG_CPUACCESS;
        }
        if ((aszArgv[5][0]=='-') && (aszArgv[5][1]=='w'))
        {
            u4Flags |= MID_FLAG_WRITEABLE;
        }
        if ((aszArgv[5][0]=='0') && (aszArgv[5][1]=='x'))
        {
            u4AgentList = StrToInt(aszArgv[5]);
        }
    }

    if (i4Argc > 6)
    {
        if ((aszArgv[6][0]=='-') && (aszArgv[6][1]=='i'))
        {
            u4Flags |= MID_FLAG_INVERSE;
        }
        if ((aszArgv[6][0]=='-') && (aszArgv[6][1]=='c'))
        {
            u4Flags |= MID_FLAG_CPUACCESS;
        }
        if ((aszArgv[6][0]=='-') && (aszArgv[6][1]=='w'))
        {
            u4Flags |= MID_FLAG_WRITEABLE;
        }
        if ((aszArgv[6][0]=='0') && (aszArgv[6][1]=='x'))
        {
            u4AgentList = StrToInt(aszArgv[6]);
        }
    }

    if (i4Argc > 7)
    {
        if ((aszArgv[7][0]=='-') && (aszArgv[7][1]=='i'))
        {
            u4Flags |= MID_FLAG_INVERSE;
        }
        if ((aszArgv[7][0]=='-') && (aszArgv[7][1]=='c'))
        {
            u4Flags |= MID_FLAG_CPUACCESS;
        }
        if ((aszArgv[7][0]=='-') && (aszArgv[7][1]=='w'))
        {
            u4Flags |= MID_FLAG_WRITEABLE;
        }
        if ((aszArgv[7][0]=='0') && (aszArgv[7][1]=='x'))
        {
            u4AgentList = StrToInt(aszArgv[7]);
        }
    }

    if (i4Argc > 8)
    {
        if ((aszArgv[8][0]=='0') && (aszArgv[8][1]=='x'))
        {
            u4AgentList = StrToInt(aszArgv[8]);
        }
    }

    if (i4Argc >= 5)
    {
        u4Region = (UINT32)StrToInt(aszArgv[4]);
        if (u4Region >= MID_MAX_REGIONS)
        {
            Printf("Invalid region number: %u\n", u4Region);
            _ShowSetRegionUsage();
            return -1;
        }
    }
    else
    {
        // Find an available region
        if (!MID_AllocateFreeRegion((u4Flags & MID_FLAG_WRITEABLE) ? FALSE : TRUE, &u4Region))
        {
            Printf("No free region found\n");
            return -1;
        }
    }

    Printf("Setting region %u (0x%08x - 0x%08x) to %s...\n", u4Region,
        u4LowAddr, u4HighAddr, MID_AgentIdToString(eAgentId));

    fgOk = MID_SetRegionEx2(u4Region, eAgentId, u4LowAddr, u4HighAddr, u4Flags, u4AgentList);

    Printf("%s\n", fgOk ? "Succeeded" : "Failed");

    return fgOk ? 0 : -1;
}

//-----------------------------------------------------------------------------
/** _ShowResetRegionUsage() to print reset region usage.
 */
//-----------------------------------------------------------------------------
static void _ShowResetRegionUsage(void)
{
    Printf("Usage: resetregion region(0-%u)\n", MID_MAX_REGIONS - 1);
}

//-----------------------------------------------------------------------------
/** _CmdResetRegion() the reset region command function
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdResetRegion(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Region;
    BOOL fgOk;

    if (i4Argc != 2)
    {
        _ShowResetRegionUsage();
        return -1;
    }

    u4Region = (UINT32)StrToInt(aszArgv[1]);
    if (u4Region >= MID_MAX_REGIONS)
    {
        _ShowResetRegionUsage();
        return -1;
    }

    fgOk = MID_ResetRegion(u4Region);
    if (!fgOk)
    {
        Printf("failed\n");
    }

    return fgOk ? 0 : -1;
}

//-----------------------------------------------------------------------------
/** _CmdAgentList() the command to display all MID agent IDs
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 */
//-----------------------------------------------------------------------------
static INT32 _CmdAgentList(INT32 i4Argc, const CHAR** aszArgv)
{
    MID_ShowAgentList();
    return 0;
}

static void _CmdPrintAgentList(UINT32 u4AgentList)
{
    INT32 i;

    Printf("\t");
    for (i=0; i<31; i++)
    {
        if (u4AgentList & (1U << i))
        {
            if (i <= 22)
            {
                Printf(" +%s", MID_AgentIdToString((MID_AGENT_ID_T)i));
            }
            else
            {
                // Note: Because agent id = 23 is the bit 0 of group3.
                // We need to shift one bit to decode the agent id.
                Printf(" +%s", MID_AgentIdToString((MID_AGENT_ID_T)(i-1)));            
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _CmdRegionList() to list all regions settings
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 */
//-----------------------------------------------------------------------------
static INT32 _CmdRegionList(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Region;
    UINT32 u4Val;    
    UINT32 u4RegAddr;
    BOOL fgSet;
    UINT32 u4LowAddr;
    UINT32 u4HighAddr;
    BOOL fgWriteable;
    BOOL fgChannelA = !IS_DRAM_CHANNELB_ACTIVE();

    Printf("Select DRAM channel A.\n");
    TCMSET_CHANNELA_ACTIVE();

    for (u4Region = 0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;
        u4Val = DRAM_READ32(u4RegAddr);
        fgSet = (u4Val & MID_ENABLE) ? TRUE : FALSE;

        Printf("\n  Region %u %s: ", u4Region, (fgSet ? "enabled " : "disabled"));

        u4HighAddr = u4Val & 0x7FFFFFFF;
        
        u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;
        u4Val = DRAM_READ32(u4RegAddr);
        u4LowAddr = u4Val & 0x7FFFFFFF;
        fgWriteable = (u4Val & MID_WRITEABLE) ? TRUE: FALSE;
            
        Printf("0x%08X ~ 0x%08X: [%s]", u4LowAddr, u4HighAddr, (fgWriteable ? "RW" : "RO"));

        u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;
        u4Val = DRAM_READ32(u4RegAddr);
        
        _CmdPrintAgentList(u4Val);
    }
    Printf("\n");

    if (IS_DRAM_CHANNELB_SUPPORT())
    {
        Printf("Select DRAM channel B.\n");    
        TCMSET_CHANNELB_ACTIVE();

        for (u4Region = 0; u4Region < MID_MAX_REGIONS; u4Region++)
        {
            u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;
            u4Val = DRAM_READ32(u4RegAddr);
            fgSet = (u4Val & MID_ENABLE) ? TRUE : FALSE;

            Printf("\n  Region %u %s: ", u4Region, (fgSet ? "enabled " : "disabled"));

            u4HighAddr = u4Val & 0x7FFFFFFF;
            
            u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;
            u4Val = DRAM_READ32(u4RegAddr);
            u4LowAddr = u4Val & 0x7FFFFFFF;
            fgWriteable = (u4Val & MID_WRITEABLE) ? TRUE: FALSE;
                
            Printf("0x%08X ~ 0x%08X: [%s]", u4LowAddr, u4HighAddr, (fgWriteable ? "RW" : "RO"));

            u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;
            u4Val = DRAM_READ32(u4RegAddr);
            
            _CmdPrintAgentList(u4Val);
        }
        Printf("\n");

        // Restore to channel select.
        if (fgChannelA)
        {
            // Select DRAM channel A to set mid.
            TCMSET_CHANNELA_ACTIVE();
        }
        else
        {
            // Select DRAM channel B to set mid.
            TCMSET_CHANNELB_ACTIVE();
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _ShowEnableRegionUsage() to display the enable command usage.
 */
//-----------------------------------------------------------------------------
static void _ShowEnableRegionUsage(void)
{
    Printf("Usage: enable region(0-%u)\n", MID_MAX_REGIONS - 1);
}

//-----------------------------------------------------------------------------
/** _CmdEnableRegion() the enable region command function
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdEnableRegion(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Region;
    BOOL fgOk;

    if (i4Argc != 2)
    {
        _ShowEnableRegionUsage();
        return -1;
    }

    u4Region = (UINT32)StrToInt(aszArgv[1]);
    if (u4Region >= MID_MAX_REGIONS)
    {
        _ShowEnableRegionUsage();
        return -1;
    }

    fgOk = MID_EnableRegionProtect(u4Region);
    if (!fgOk)
    {
        Printf("failed\n");
    }

    return fgOk ? 0 : -1;
}

//-----------------------------------------------------------------------------
/** _ShowDisableRegionUsage() to display the disable command usage.
 */
//-----------------------------------------------------------------------------
static void _ShowDisableRegionUsage(void)
{
    Printf("Disable: disable region(0-%u)\n", MID_MAX_REGIONS - 1);
}

//-----------------------------------------------------------------------------
/** _CmdDisableRegion() the disable region command function
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdDisableRegion(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Region;
    BOOL fgOk;

    if (i4Argc != 2)
    {
        _ShowDisableRegionUsage();
        return -1;
    }

    u4Region = (UINT32)StrToInt(aszArgv[1]);
    if (u4Region >= MID_MAX_REGIONS)
    {
        _ShowDisableRegionUsage();
        return -1;
    }

    fgOk = MID_DisableRegionProtect(u4Region);
    if (!fgOk)
    {
        Printf("failed\n");
    }

    return fgOk ? 0 : -1;
}

//-----------------------------------------------------------------------------
/** _ShowHaltOnIntrusionUsage() to display the halt on command usage.
 */
//-----------------------------------------------------------------------------
static void _ShowHaltOnIntrusionUsage(void)
{
    Printf("Usage: haltonintrusion [on|off]\n");
}

//-----------------------------------------------------------------------------
/** _CmdHaltOnIntrusion() the command to change halt on settings on intrusion
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 *  @retval -1: Fail
 */
//-----------------------------------------------------------------------------
static INT32 _CmdHaltOnIntrusion(INT32 i4Argc, const CHAR** aszArgv)
{
    if (i4Argc > 2)
    {
        _ShowDisableRegionUsage();
        return -1;
    }

    if (i4Argc == 2)
    {
        if (x_strcmp(aszArgv[1], "on") == 0)
        {
            _fgHaltOnIntrusion = TRUE;
        }
        else if (x_strcmp(aszArgv[1], "off") == 0)
        {
            _fgHaltOnIntrusion = FALSE;
        }
        else
        {
            _ShowHaltOnIntrusionUsage();
            return -1;
        }
    }
    else
    {
        Printf("Halt on intrustion: %s\n", _fgHaltOnIntrusion ? "on" : "off");
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _CmdMemStatGet() the command to set memory statistic conf
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 */
//-----------------------------------------------------------------------------
static INT32 _CmdMemStatSet(INT32 i4Argc, const CHAR ** aszArgv)
{
    return 0;
}

//-----------------------------------------------------------------------------
/** _CmdMemStatGet() the command to get memory statistic conf & result.
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 */
//-----------------------------------------------------------------------------
static INT32 _CmdMemStatGet(INT32 i4Argc, const CHAR ** aszArgv)
{
    return 0;
}

//-----------------------------------------------------------------------------
/** _CmdMemFlush() the command to flush cpu cache to memory
 *  @param i4Argc the number of arguements
 *  @param aszArgv the string array of arguements
 *  @retval 0: Succeed
 */
//-----------------------------------------------------------------------------
static INT32 _CmdMemFlush(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Addr;
    UINT32 u4Size;

    if (i4Argc != 3)
    {
        return 0;
    }
    
    u4Addr = (UINT32)StrToInt(aszArgv[1]);
    u4Size = (UINT32)StrToInt(aszArgv[2]);

    HalFlushInvalidateDCacheMultipleLine(u4Addr, u4Size);

    return 0;
}

static INT32 _CmdMemSetInterrupt(INT32 i4Argc, const CHAR ** aszArgv)
{
    if ((UINT32)StrToInt(aszArgv[1]) == 0)
    {
        BIM_DisableIrq(VECTOR_DRAMC);
        if (IS_DRAM_CHANNELB_SUPPORT())
        {
            BIM_DisableIrq(VECTOR_DRAMC_CHB);
        }
        return 0;
    }
    BIM_EnableIrq(VECTOR_DRAMC);
    if (IS_DRAM_CHANNELB_SUPPORT())
    {
        BIM_EnableIrq(VECTOR_DRAMC_CHB);
    }
    return 0;
}


static INT32 _CmdResetStatus(INT32 i4Argc, const CHAR ** aszArgv)
{
    MID_ResetStatus(VECTOR_DRAMC);
    if (IS_DRAM_CHANNELB_SUPPORT())
    {
        MID_ResetStatus(VECTOR_DRAMC_CHB);    
    }
    return 0;
}

static INT32 _CmdQueryStatus(INT32 i4Argc, const CHAR ** aszArgv)
{
    BOOL fgChannelA = !IS_DRAM_CHANNELB_ACTIVE();

    Printf("Select DRAM channel A.\n");
    TCMSET_CHANNELA_ACTIVE();
    MID_QueryIntrusionStatus();

    if (IS_DRAM_CHANNELB_SUPPORT())
    {
        Printf("Select DRAM channel B.\n");    
        TCMSET_CHANNELB_ACTIVE();
        MID_QueryIntrusionStatus();

        // Restore to channel select.
        if (fgChannelA)
        {
            // Select DRAM channel A to set mid.
            TCMSET_CHANNELA_ACTIVE();
        }
        else
        {
            // Select DRAM channel B to set mid.
            TCMSET_CHANNELB_ACTIVE();
        }
    }

    return 0;
}

static INT32 _CmdSetDramChannel(INT32 i4Argc, const CHAR ** aszArgv)
{
     Printf("mid.ch [0: channel A, 1: channel B]\n");

    if ((UINT32)StrToInt(aszArgv[1]) == 1)
    {
        if (IS_DRAM_CHANNELB_SUPPORT())
        {
            TCMSET_CHANNELB_ACTIVE();
            Printf("Channel B is selected.\n");        
        }
        else
        {
            Printf("Channel B is not support.\n");
        }
    }
    else
    {
        TCMSET_CHANNELA_ACTIVE();
        Printf("Channel A is selected.\n");        
    }

    return 0;
}

// The MID CLI command array.
static CLI_EXEC_T _arMemoryProtectCmdTbl[] =
{
    {
        "init",
        "i",
        _CmdInit,
        NULL,
        "Initialize MID driver",
        CLI_SUPERVISOR
    },

    {
        "exit",
        "x",
        _CmdExit,
        NULL,
        "Shutdown MID driver",
        CLI_SUPERVISOR
    },

    {
        "reset",
        "r",
        _CmdReset,
        NULL,
        "Reset MID driver",
        CLI_SUPERVISOR
    },

    {
        "setregion",
        "sr",
        _CmdSetRegion,
        NULL,
        "Setup a region",
        CLI_SUPERVISOR
    },

    {
        "resetregion",
        "rr",
        _CmdResetRegion,
        NULL,
        "Reset a region",
        CLI_SUPERVISOR
    },

    {
        "agentlist",
        "al",
        _CmdAgentList,
        NULL,
        "Show DMA agent list",
        CLI_SUPERVISOR
    },

    {
        "regionlist",
        "rl",
        _CmdRegionList,
        NULL,
        "Show all region states",
        CLI_SUPERVISOR
    },

    {
        "enable",
        "e",
        _CmdEnableRegion,
        NULL,
        "Enable a region",
        CLI_SUPERVISOR
    },

    {
        "disable",
        "d",
        _CmdDisableRegion,
        NULL,
        "Disable a region",
        CLI_SUPERVISOR
    },

    {
        "haltonintrusion",
        "h",
        _CmdHaltOnIntrusion,
        NULL,
        "Halt on intrusion",
        CLI_SUPERVISOR
    },

    {
        "mem_stat_set",
        "mss",
        _CmdMemStatSet,
        NULL,
        "Set Memory Statistics config",
        CLI_SUPERVISOR
    },
    
    {
        "mem_stat_get",
        "msg",
        _CmdMemStatGet,
        NULL,
        "Get Memory Statistics config & stat",
        CLI_SUPERVISOR
    },
    
    {
        "memflush",
        "mf",
        _CmdMemFlush,
        NULL,
        "Flush memory cache",
        CLI_SUPERVISOR
    },

    {
        "intren",
        "ie",
        _CmdMemSetInterrupt,
        NULL,
        "Set on/off memory intrude interrupt [1|0]",
        CLI_GUEST
    },

    {
        "reset_status",
        "rs",
        _CmdResetStatus,
        NULL,
        "Reset Intrusion status",
        CLI_GUEST
    },    
    {
        "query_stauts",
        "q",
        _CmdQueryStatus,
        NULL,
        "Query Intrusion status",
        CLI_GUEST
    },        
    {
        "set_dram_channel",
        "ch",
        _CmdSetDramChannel,
        NULL,
        "Set dram channel",
        CLI_GUEST
    },        

    // LOG macro support
    CLIMOD_DEBUG_CLIENTRY(MID),

    // last cli command record, NULL
    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};

// The CLI command directory entry.
CLI_MAIN_COMMAND_ITEM(Mid)
{
    "mid",
    NULL,
    NULL,
    _arMemoryProtectCmdTbl,
    "Memory intrusion detection",
    CLI_SUPERVISOR
};

