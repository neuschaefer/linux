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
 * $RCSfile: mid.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mid.c
 *  mid.c Memory intrusion detection module
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mid_debug.h"
#include "x_debug.h"
#include "x_mid.h"
#include "x_dram.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_lint.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    MID_AGENT_ID_T eAgentId;
    const CHAR* szAgentName;
} AGENT_MAPPING_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PFN_MID_CALLBACK _pfnHandler = NULL;

static CRIT_STATE_T _rState;

static BOOL _fgInit = FALSE;
static AGENT_MAPPING_T _arAgentList[] =
{
    // Group 1
    { MID_AGENT_AUDIO,            "audio" },                    // 0
    { MID_AGENT_ICUXDATA,       "icu_xdata" },             // 1
    { MID_AGENT_VDOIN,            "vbi_3d_tve" },           // 2
    { MID_AGENT_OSD,               "osd" },                       // 3
    { MID_AGENT_DI,                  "di" },                          // 4
    { MID_AGENT_SCALAR,         "scalar" },                    // 5
    { MID_AGENT_CPU,              "cpu" },                        // 6
    { MID_AGENT_DSP,               "dsp" },                       // 7
    { MID_AGENT_TEST0,            "test0" },                    // 15

    // Group 2
    { MID_AGENT_USB,              "usb" },                        // 16
    { MID_AGENT_PARSER,         "parser" },                   // 17
    { MID_AGENT_GRAPH,          "graph" },                    // 18
    { MID_AGENT_TCMLZHS,      "tcm_lzhs" },               // 19
    { MID_AGENT_8032RS232,   "8032_rs232" },         // 20
    { MID_AGENT_TEST1,              "test1" },                  // 22

    // Group 3
    { MID_AGENT_TEST2,            "test2" },                    // 29

    // Others
    { MID_AGENT_NONE,             "none" },
    { MID_AGENT_INVALID,          "invalid" }
};

//-----------------------------------------------------------------------------
// Inline functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _IsRegionValid() Check if the given region is valid
 *  @param u4Region: The region number
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
LINT_SUPPRESS_NEXT_EXPRESSION(129)
INLINE static BOOL _IsRegionValid(UINT32 u4Region)
{
    return (u4Region < MID_MAX_REGIONS);
}

//-----------------------------------------------------------------------------
/** _IsAgentValid() Check if an agent id is valid
 *  @param eAgentId: The agent id to be checked
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
LINT_SUPPRESS_NEXT_EXPRESSION(129)
INLINE static BOOL _IsAgentValid(MID_AGENT_ID_T eAgentId)
{
    return (eAgentId < MID_AGENT_MAX);
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _Align() Return the nearest aligned address equal to or greater than the \n
 *      given address
 *  @param u4Addr: The address
 *  @param u4Alignment: The alignment requirement in byte
 *  @return The aligned address
 */
//-----------------------------------------------------------------------------
static UINT32 _Align(UINT32 u4Addr, UINT32 u4Alignment)
{
    UINT32 u4Unaligned;

    if (u4Alignment <= 1)
    {
        return u4Addr;
    }

    u4Unaligned = u4Addr % u4Alignment;
    if (u4Unaligned != 0)
    {
        u4Addr += u4Alignment - u4Unaligned;
    }

    return u4Addr;
}

//-----------------------------------------------------------------------------
/** _IsAligned() Check if an address is aligned to the given alignment
 *  @param u4Addr: The address to be checked
 *  @param u4Alignment: The alignment
 *  @retval TRUE: The address is aligned
 *  @retval FALSE: Otherwise
 */
//-----------------------------------------------------------------------------
static BOOL _IsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
	return u4Addr == _Align(u4Addr, u4Alignment);
}

//-----------------------------------------------------------------------------
/** _Lock() Enter the critical section to access global data
 */
//-----------------------------------------------------------------------------
static void _Lock(void)
{
    _rState = x_crit_start();
}

//-----------------------------------------------------------------------------
/** _Unlock() Leave the critical section
 */
//-----------------------------------------------------------------------------
static void _Unlock(void)
{
    x_crit_end(_rState);
}

//-----------------------------------------------------------------------------
/** _ResetRegion() Reset a region to normal state
 *  @param u4Region: The region to be reset
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _ResetRegion(UINT32 u4Region)
{
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;    
    DRAM_WRITE32(u4RegAddr, 0);

    u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;    
    DRAM_WRITE32(u4RegAddr, 0);

    u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;
    DRAM_WRITE32(u4RegAddr, 0);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _EnableRegionProtect() Enable the intrusion detection of a region
 *  @param u4Region: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _EnableRegionProtect(UINT32 u4Region)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;
    
    u4Val = DRAM_READ32(u4RegAddr);
    u4Val |= MID_ENABLE;
    DRAM_WRITE32(u4RegAddr, u4Val);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DisableRegionProtect() Disable the intrusion detection of a region
 *  @param u4Retion: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DisableRegionProtect(UINT32 u4Region)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;
    
    u4Val = DRAM_READ32(u4RegAddr);
    u4Val &=~ MID_ENABLE;
    DRAM_WRITE32(u4RegAddr, u4Val);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _Reset() Reset all regions to normal state
 *  @reval TRUE: Succeed
 *  @reval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _Reset(void)
{
    UINT32 i;

    // Reset region states
    for (i = 0; i < MID_MAX_REGIONS; i++)
    {
        _DisableRegionProtect(i);

        if (!_ResetRegion(i))
        {
            return FALSE;
        }
    }

    // Clear interrupt
    MID_ResetStatus();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _GetIntruder() Find the intruder by checking hardware register states
 *  @param pu4Region: The region to be intruded
 *  @param peAgentId: The intruder
 *  @param pu4Addr: The intruded address
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _GetIntruder(UINT32* pu4Region, MID_AGENT_ID_T* peAgentId,
    UINT32* pu4Addr)
{
    UINT32 u4Region;   
    UINT32 u4InfoAddr;
    UINT32 u4InfoAgentAddr;
    UINT32 u4Val;
    UINT32 u4IntrudeAgentId;
    UINT32 u4IntrudeAddr;
    BOOL fgRet = FALSE;

    // Detect the region intruded
    for (u4Region = 0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        u4InfoAddr = MID_BASE + MID_INTRUD_INFO + (MID_INTRUD_INFO_OFFSET * u4Region);
        u4Val = DRAM_READ32(u4InfoAddr);
        if (u4Val & MID_INTRUD_DETECTED)
        {
            // Get intrude agent address.
            u4IntrudeAddr = u4Val & 0x7FFFFFFF;

            // Get intrude agent id.
            u4InfoAgentAddr = 
                MID_BASE + MID_INTRUD_AGENT_INFO + (MID_INTRUD_AGENT_INFO_OFFSET * (u4Region/4));
            u4Val = DRAM_READ32(u4InfoAgentAddr);
            u4IntrudeAgentId = (u4Val >> (8 * (u4Region & 0x3))) & 0x1F;

            Printf("MEMORY VIOLATION - Region-%d, AgentId: %s, Address: 0x%08X.\n", u4Region, 
                MID_AgentIdToString((MID_AGENT_ID_T)u4IntrudeAgentId), u4IntrudeAddr);

            // Report to upper layer.
            if (peAgentId)
            {
                *peAgentId = (MID_AGENT_ID_T)u4IntrudeAgentId;
            }

            if (pu4Addr)
            {
                *pu4Addr = u4IntrudeAddr;
            }

            if (pu4Region)
            {
                *pu4Region = u4Region;
            }

            fgRet = TRUE;
        }            
    }

    return fgRet;
 }

//---------------------------------------------------------------------
// Interrupt service routine

//-----------------------------------------------------------------------------
/** _MemoryIntruderIsr() This is the ISR of memory intrusion detection
 *  @param u2Vector: The IRQ vector, must be VECTOR_DRAMC
 */
//-----------------------------------------------------------------------------
static VOID _MemoryIntrudeIsr(UINT16 u2Vector)
{
    MID_AGENT_ID_T eAgentId;
    UINT32 u4Region = 0, u4Addr = 0;

    ASSERT(u2Vector == VECTOR_DRAMC);

    // Always check memory intrude.
    VERIFY(_GetIntruder(&u4Region, &eAgentId, &u4Addr));

    // Invoke user registered handler
    if (_pfnHandler != NULL)
    {
        _pfnHandler(u4Region, eAgentId, u4Addr);
    }

    // Clear interrupt
    while (BIM_IsIrqPending(VECTOR_DRAMC))
    {
        MID_ResetStatus();
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** MID_Init() Initialize MID module
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_Init(void)
{
    x_os_isr_fct pfnOldIsr;

    if (_fgInit)
    {
        return TRUE;
    }

    // Reset hardware
    if (!_Reset())
    {
        return FALSE;
    }

    // Register ISR handler
    if (x_reg_isr(VECTOR_DRAMC, _MemoryIntrudeIsr, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }

    _pfnHandler = NULL;
    _fgInit = TRUE;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_Exit() Shutdown MID
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_Exit(void)
{
    x_os_isr_fct pfnOldIsr;
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    // Uninstall ISR handler
    if (x_reg_isr(VECTOR_DRAMC, NULL, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }

    // Reset hardware
    fgRet = _Reset();
    _fgInit = FALSE;

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_Reset() Reset all regions to initial states
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_Reset(void)
{
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    _Lock();

    fgRet = _Reset();

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_RegisterHandler() Register a callback handler. The handler will be \n
 *      called when memory intrusion happened.
 *  @param pfnNewHandler: The new handler to be registered
 *  @return The old handler
 */
//-----------------------------------------------------------------------------
PFN_MID_CALLBACK MID_RegisterHandler(PFN_MID_CALLBACK pfnNewHandler)
{
    PFN_MID_CALLBACK pfnOldHandler;

    ASSERT(_fgInit);

    _Lock();

    pfnOldHandler = _pfnHandler;
    _pfnHandler = pfnNewHandler;

    _Unlock();

    return pfnOldHandler;
}

//-----------------------------------------------------------------------------
/** MID_SetRegionEx() Setup a region with flags
 *  @param u4Region: The region to be setup
 *  @param eAgentid: The only agent allowed to write the region
 *  @param u4LowAddr: The low address of the region
 *  @param u4HighAddr: The high address of the region
 *  @param u4Flags: The Inverse flag & CPU access flag.
 *  @param u4AgentList: region 5/6 agent on/off list.
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_SetRegionEx2(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags, UINT32 u4AgentList)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    if (!_fgInit)
    {
        return FALSE;
    }

    // Check region number and agent id
    if (!_IsRegionValid(u4Region) || !_IsAgentValid(eAgentId))
    {
        return FALSE;
    }

    // Check address range
    if (u4LowAddr > u4HighAddr)
    {
        return FALSE;
    }

    // Check address alignments
    if (!_IsAligned(u4LowAddr, MID_ADDR_ALIGNMENT) ||
        !_IsAligned(u4HighAddr, MID_ADDR_ALIGNMENT))
    {
        return FALSE;
    }

    // Check region state?
    _Lock();

    // Setup region on lower address register.
    u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;    
    u4Val = DRAM_READ32(u4RegAddr);
    u4Val |= u4LowAddr & 0x1FFFFFFC;
    DRAM_WRITE32(u4RegAddr, u4Val);

    // Setup write allow agent id on this region.
    u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;    
    u4Val = DRAM_READ32(u4RegAddr);

    if (eAgentId < MID_AGENT_NONE)
    {        
        u4Val |= 1 << (UINT32)eAgentId;
    }

    // Allow cpu to write this region.
    if (u4Flags & MID_FLAG_CPUACCESS)    
    {
        u4Val |= 1 << (UINT32)MID_AGENT_CPU;        
    }

    // Support multiple agent list to write this region.
    if (u4AgentList > 0)
    {
        u4Val |= 1 << (UINT32)MID_AGENT_CPU;
    }    
    DRAM_WRITE32(u4RegAddr, u4Val);

    // Setup region on upper address register.
    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;    
    u4Val = DRAM_READ32(u4RegAddr);
    u4Val |= u4HighAddr & 0x1FFFFFFC;
    if (u4Flags & MID_FLAG_INVERSE)
    {
        // Enable (region >= upper address)  || (region < lower address).
        u4Val |= MID_MODE;    
    }
    DRAM_WRITE32(u4RegAddr, u4Val);

    _Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_SetRegionEx() Setup a region with flags
 *  @param u4Region: The region to be setup
 *  @param eAgentid: The only agent allowed to write the region
 *  @param u4LowAddr: The low address of the region
 *  @param u4HighAddr: The high address of the region
 *  @param u4Flags: THe Inverse flag & CPU access flag.
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_SetRegionEx(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags)
{
    return MID_SetRegionEx2(u4Region, eAgentId, u4LowAddr, u4HighAddr, 0, 0);
}

//-----------------------------------------------------------------------------
/** MID_SetRegion() Setup a region
 *  @param u4Region: The region to be setup
 *  @param eAgentid: The only agent allowed to write the region
 *  @param u4LowAddr: The low address of the region
 *  @param u4HighAddr: The high address of the region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_SetRegion(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4LowAddr,
    UINT32 u4HighAddr)
{
    return MID_SetRegionEx(u4Region, eAgentId, u4LowAddr, u4HighAddr, 0);
}

//-----------------------------------------------------------------------------
/** MID_ResetRegion() Reset a region to initial state
 *  @param u4Region: The region to be reset
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_ResetRegion(UINT32 u4Region)
{
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    fgRet = _ResetRegion(u4Region);

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_AllocateFreeRegion() Find a free region
 *  @param fgReadOnly -input- If the region is a read-only one
 *  @param pu4Region -output- The region allocated
 *  @retval TRUE: A free region is found
 *  @retval FALSE: No free region for the given condition
 */
//-----------------------------------------------------------------------------
BOOL MID_AllocateFreeRegion(BOOL fgReadOnly, UINT32* pu4Region)
{
    BOOL fgRet = FALSE;
    UINT32 u4Region;
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    UNUSED(fgReadOnly);
    
    if (!_fgInit)
    {
        return FALSE;
    }

    _Lock();

    for (u4Region=0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;        
        u4Val = DRAM_READ32(u4RegAddr);
        
        if ((u4Val & MID_ENABLE) == 0)
        {
            if (pu4Region != NULL)
            {
                *pu4Region = u4Region;
            }

            fgRet = TRUE;
            break;
        }
    }

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_EnableRegionProtect() Enable the intrusion detection of a region
 *  @param u4Retion: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_EnableRegionProtect(UINT32 u4Region)
{
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    fgRet = _EnableRegionProtect(u4Region);

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_DisableRegionProtect() Disable the intrusion detection of a region
 *  @param u4Region: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_DisableRegionProtect(UINT32 u4Region)
{
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    fgRet = _DisableRegionProtect(u4Region);

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_ShowAgentList() show agent status
 *  @param void
 *  @return void
 */
//-----------------------------------------------------------------------------
void MID_ShowAgentList(void)
{
    UINT32 i;
    UINT32 u4NumAgents;
    
    Printf("DRAM agent list\n");
    u4NumAgents = (sizeof (_arAgentList) / sizeof (AGENT_MAPPING_T)) - 1;

    for (i = 0; i < u4NumAgents; i++)
    {
        Printf("  %2u: %s\n", (UINT32)_arAgentList[i].eAgentId, _arAgentList[i].szAgentName);    
    }    
}

//-----------------------------------------------------------------------------
/** MID_AgentIdToString() Convert an agent id to a readable string
 *  @param eAgentId: The agent id
 *  @return The string
 */
//-----------------------------------------------------------------------------
const CHAR* MID_AgentIdToString(MID_AGENT_ID_T eAgentId)
{
    UINT32 u4NumAgents, i;
    const CHAR* szAgentName = "unused";

    u4NumAgents = (sizeof (_arAgentList) / sizeof (AGENT_MAPPING_T)) - 1;

    for (i = 0; i < u4NumAgents; i++)
    {
        if (_arAgentList[i].eAgentId == eAgentId)
        {
            szAgentName = _arAgentList[i].szAgentName;
            break;
        }
    }

    return szAgentName;
}

//-----------------------------------------------------------------------------
/** MID_StringToAgentId() Convert a string to an agent id
 *  @param szAgentName The string of agent name
 *  @return The agent id
 */
//-----------------------------------------------------------------------------
MID_AGENT_ID_T MID_StringToAgentId(const CHAR* szAgentName)
{
    UINT32 i, u4NumAgents;
    MID_AGENT_ID_T eAgentId = MID_AGENT_INVALID;

    u4NumAgents = (sizeof (_arAgentList) / sizeof (AGENT_MAPPING_T)) - 1;

    for (i = 0; i < u4NumAgents; i++)
    {
        if (x_strcmp(_arAgentList[i].szAgentName, szAgentName) == 0)
        {
            eAgentId = _arAgentList[i].eAgentId;
            break;
        }
    }

    return eAgentId;
}

//-----------------------------------------------------------------------------
/** MID_CntBandWidthSet() Set mem bandwidth counter configuration
 *  @param fgEnable enable/disable setting
 *  @param szAgentName the agent name to set into configuration.
 *  @param u4Clk the bandwidth clock length of statistics.
 *  @return The agent group id 1/2/3, 0 is invalid.
 */
//-----------------------------------------------------------------------------
UINT32 MID_CntBandWidthSet(UINT32 fgEnable, const CHAR *szAgentName,
                UINT32 u4Clk)
{
#if 0
    MID_AGENT_ID_T eAgentId;

    UNUSED(u4Clk);
    if (szAgentName == NULL)
    {
        return 0;
    }

    eAgentId = MID_StringToAgentId(szAgentName);
    if (eAgentId == MID_AGENT_INVALID)
    {
        return 0;
    }

     if ((UINT32)eAgentId < GROUP2_FIRST_AGENT_ID)   /* group 1 */
    {
        vIO32WriteFldAlign(EMRS_BM, (UINT32)eAgentId, BMGP1AG); // u4Val = (u4Val & ~GPAG1_MASK) | GPAG1_ID((UINT32)eAgentId);
        if (fgEnable)
        {
            vIO32WriteFldAlign(EMRS_BM, 1, BMGP1); // u4Val |= GPAG1_EN;
        }
        else
        {
            vIO32WriteFldAlign(EMRS_BM, 0, BMGP1); // u4Val &= ~GPAG1_EN;
        }
        // DRAM_WRITE32(REG_DRAMSTAT_EN, u4Val);
        return 1;
    }
    else if ((UINT32)eAgentId < GROUP3_FIRST_AGENT_ID)  /* group 2 */
    {
        vIO32WriteFldAlign(EMRS_BM, (UINT32)eAgentId, BMGP2AG); // u4Val = (u4Val & ~GPAG2_MASK) | GPAG2_ID((UINT32)eAgentId);
        if (fgEnable)
        {
            vIO32WriteFldAlign(EMRS_BM, 1, BMGP2); // u4Val |= GPAG2_EN;
        }
        else
        {
            vIO32WriteFldAlign(EMRS_BM, 0, BMGP2); // u4Val &= ~GPAG2_EN;
        }
        // DRAM_WRITE32(REG_DRAMSTAT_EN, u4Val);
        return 2;
    }
    else /* group 3 */
    {
        vIO32WriteFldAlign(EMRS_BM, (UINT32)eAgentId, BMGP3AG); // u4Val = (u4Val & ~GPAG3_MASK) | GPAG3_ID((UINT32)eAgentId);
        if (fgEnable)
        {
            vIO32WriteFldAlign(EMRS_BM, 1, BMGP3); // u4Val |= GPAG3_EN;
        }
        else
        {
            vIO32WriteFldAlign(EMRS_BM, 0, BMGP3); // u4Val &= ~GPAG3_EN;
        }
        // DRAM_WRITE32(REG_DRAMSTAT_EN, u4Val);
        return 3;
    }
#else
    return 0;
#endif
}

//-----------------------------------------------------------------------------
/** MID_CntBandWidthGet() Get mem bandwidth counter configuration
 *  @param u4GrpId The agent group id which we want to know the mem bwc config.
 *  @param szAgentName -output- fill the agent name of the group id to caller.
 *  @param pu4AgentId -output- fill the agent id of the group id to caller.
 *  @param pu4Enable -output- the enable/disable mode agent id.
 *  @return The bytes of the agent of the group id.
 */
//-----------------------------------------------------------------------------
UINT32 MID_CntBandWidthGet(UINT32 u4GrpId, CHAR *szAgentName,
                UINT32 *pu4AgentId, UINT32 *pu4Enable)
{
#if 0
    UINT32 u4Val;
    const CHAR *pu4Name;

    if ((szAgentName == NULL) || (pu4AgentId == NULL) || (pu4Enable == NULL))
    {
        return 0;
    }

    // u4Val = DRAM_READ32(REG_DRAMSTAT_EN);        // 0x80
    switch(u4GrpId)
    {
    case 1:
        *pu4Enable = IO32ReadFldAlign(EMRS_BM, BMGP1);      // (u4Val >> 12) & 1;
        *pu4AgentId = IO32ReadFldAlign(EMRS_BM, BMGP1AG);   // (u4Val >> 8) & 0xf;
        u4Val = IO32ReadFldAlign(BMLEN1, BMLENGP1_CNT);     // DRAM_READ32(REG_GPAG1_STAT);
        break;
    case 2:
        *pu4Enable = IO32ReadFldAlign(EMRS_BM, BMGP2);      // (u4Val >> 13) & 1;
        *pu4AgentId = IO32ReadFldAlign(EMRS_BM, BMGP2AG);   // ((u4Val >> 4) & 0xf) + 0x10;
        u4Val = IO32ReadFldAlign(BMLEN2, BMLENGP2_CNT);     // DRAM_READ32(REG_GPAG2_STAT);
        break;
    case 3:
        *pu4Enable = IO32ReadFldAlign(EMRS_BM, BMGP3);      // (u4Val >> 14) & 1;
        *pu4AgentId = IO32ReadFldAlign(EMRS_BM, BMGP3AG);   // ((u4Val >> 0) & 0xf) + 0x10;
        u4Val = IO32ReadFldAlign(BMLEN3, BMLENGP3_CNT);     // DRAM_READ32(REG_GPAG3_STAT);
        break;
    default:
        return 0;
    }
    pu4Name = MID_AgentIdToString((MID_AGENT_ID_T)(*pu4AgentId));
    LINT_SUPPRESS_NEXT_EXPRESSION(534)
    x_strcpy(szAgentName, pu4Name);

    return u4Val;
#else
    return 0;
#endif
}

//-----------------------------------------------------------------------------
/** MID_ResetStatus() Reset all intrusion status
 *  @retval None
 */
//-----------------------------------------------------------------------------
VOID MID_ResetStatus(void)
{
    UINT32 u4RegAddr;
    UINT32 u4InfoAddr;
    UINT32 u4Val;
    BOOL fgLoop;
    UINT32 i;
    
    u4RegAddr = MID_BASE + MID_CLEAR_STATUS;
    
    DRAM_WRITE32(u4RegAddr, 0x1);    

    // Check each region status.
    do
    {
        fgLoop = FALSE;
        for (i = 0; i < MID_MAX_REGIONS; i++)
        {
            u4InfoAddr = MID_BASE + MID_INTRUD_INFO + (MID_INTRUD_INFO_OFFSET * i);
            u4Val = DRAM_READ32(u4InfoAddr);
            if (u4Val & MID_INTRUD_DETECTED)
            {
                fgLoop = TRUE;
            }
        }
    }
    while (fgLoop);

    DRAM_WRITE32(u4RegAddr, 0x0);

    BIM_ClearIrq(VECTOR_DRAMC);
}

//-----------------------------------------------------------------------------
/** MID_QueryIntrusionStatus() Reset all intrusion status
 *  @retval None
 */
//-----------------------------------------------------------------------------
VOID MID_QueryIntrusionStatus(void)
{
    // Just query intrude information, not to reset all MID status.
    if (!_GetIntruder(NULL, NULL, NULL))
    {
        Printf("No memory violation.\n");
    }
}

