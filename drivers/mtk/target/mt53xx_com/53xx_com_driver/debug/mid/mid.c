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
#include "x_bim.h"
#include "x_mid.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_lint.h"

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

static REGION_STATE_T _arRegionState[MID_MAX_REGIONS];

static PFN_MID_CALLBACK _pfnHandler = NULL;

static CRIT_STATE_T _rState;

static BOOL _fgInit = FALSE;
#if defined(CC_MT5387) && !defined(CC_MT5363)
static AGENT_MAPPING_T _arAgentList[] =
{
    // Group 1
    { MID_AGENT_AUDIO,            "audio" },                    // 0
    { MID_AGENT_DEMUX,            "demux_pvr" },                // 1
    { MID_AGENT_VDOIN,            "vbi_3d_tve" },               // 2
    { MID_AGENT_OSD,              "osd" },                      // 3
    { MID_AGENT_PSCAN,            "pscan_nr" },                 // 4
    { MID_AGENT_B2R,              "b2r" },                      // 5
    { MID_AGENT_CPU,              "cpu" },                      // 6
    { MID_AGENT_SCPOS,            "scpos" },                    // 7
    { MID_AGENT_MPEG1,            "vdecmc" },                   // 8
    { MID_AGENT_JPEG,             "vdecmisc" },                 // 9
    { MID_AGENT_GFXHI,            "gfxhi" },                    // 11
    { MID_AGENT_TEST0,            "test0" },                    // 13
//    { MID_AGENT_GROUP2,           "group2" },                   // 14
//    { MID_AGENT_GROUP3,           "group3" },                   // 15

    // Group 2
    { MID_AGENT_USB,              "usb" },                      // 16
    { MID_AGENT_IMGRZ,            "imgrz_gdma" },               // 17
    { MID_AGENT_DDI,              "ddi_dmxpull" },              // 18
    { MID_AGENT_DSP,              "dsp" },                      // 20
    { MID_AGENT_FCI,              "fci" },                      // 21
    { MID_AGENT_TEST1,            "test1" },                    // 22

    // Group 3
    { MID_AGENT_GFXLO,            "gfx" },                      // 23
    { MID_AGENT_TCM,              "tcm" },                      // 26
    { MID_AGENT_IDETEST,          "idetest" },                  // 27
    { MID_AGENT_RS232,            "rs232" },                    // 28
    { MID_AGENT_TEST2,            "test2" },                    // 29

    // Others
    { MID_AGENT_NONE,             "none" },
    { MID_AGENT_INVALID,          "invalid" }
};
#else // CC_MT5363 or new ic
static AGENT_MAPPING_T _arAgentList[] =
{
    // Group 1
    { MID_AGENT_AUDIO,            "audio" },                    // 0
    { MID_AGENT_DEMUX,            "demux_pvr" },                // 1
    { MID_AGENT_VDOIN,            "vbi_3d_tve" },               // 2
    { MID_AGENT_OSD,              "osd" },                      // 3
    { MID_AGENT_PSCAN,            "pscan_nr" },                 // 4
    { MID_AGENT_B2R,              "b2r" },                      // 5
    { MID_AGENT_CPU,              "cpu" },                      // 6
    { MID_AGENT_SCPOS,            "scaler_pip" },                    // 7
    { MID_AGENT_VDECMC,            "vdecmc" },                   // 8
    { MID_AGENT_JPEG,             "vld1_jpeg" },                 // 9
    {MID_AGENT_T32IC,             "t8032_cache_xdata"},    //10
    { MID_AGENT_GFXHI,            "gra_w_8281" },                    // 11
    { MID_AGENT_IMGRZ,            "imgrz" },               // 12    
    { MID_AGENT_DTCP,           "ethernet_dtcp" },                   // 13    
    { MID_AGENT_JPGDEC,            "jpgdec" },                    // 14
    { MID_AGENT_TEST0,           "test0" },                   // 15

    // Group 2
    { MID_AGENT_USB,              "usb" },                      // 16
    { MID_AGENT_GDMA,            "gdma" },               // 17
    { MID_AGENT_DDI,              "ddi_dmxpull" },              // 18
    { MID_AGENT_DSP1,              "dsp1" },                      // 19
    { MID_AGENT_DSP0,              "dsp0" },                      // 20
    { MID_AGENT_FCI,              "fci" },                      // 21
    { MID_AGENT_TEST1,            "test1" },                    // 22

    // Group 3
    { MID_AGENT_GFXLO,            "gfx" },                      // 23
    { MID_AGENT_TCM,              "tcm" },                      // 24
    { MID_AGENT_IDETEST,          "idetest" },                  // 25
    { MID_AGENT_RS232,            "rs232" },                    // 26
    { MID_AGENT_T32DMA,            "t8032_t8032dma" },                    // 27    
    { MID_AGENT_TEST2,            "test2" },                    // 29
		{ MID_AGENT_POD,            "pod" },                    // 29
    // Others
    { MID_AGENT_NONE,             "none" },
    { MID_AGENT_INVALID,          "invalid" }
};


#endif
static const UINT32 _arRegionUp[MID_MAX_REGIONS] = {
    PROT0UP, PROT1UP, PROT2UP, PROT3UP, PROT4UP, PROT5UP, PROT6UP
};

static const UINT32 _arRegionLw[MID_MAX_REGIONS] = {
    PROT0LW, PROT1LW, PROT2LW, PROT3LW, PROT4LW, PROT5LW, PROT6LW
};

static const UINT32 _arRegionAddr[MID_MAX_REGIONS] = {
    INTRU2, INTRU3, INTRU4, INTRU5, INTRU6, INTRU9, INTRU10
};

static const UINT32 _arRegionStatus[MID_MAX_REGIONS][2] = {
    { INTRU7, INTRUDE0 },
    { INTRU7, INTRUDE1 },
    { INTRU7, INTRUDE2 },
    { INTRU7, INTRUDE3 },
    { INTRU7, INTRUDE4 },
    { INTRU8, INRG5 },
    { INTRU8, INRG6 }
};

static const UINT32 _arRegionAgent[MID_MAX_REGIONS][2] = {
    { INTRU2, RG0AG },
    { INTRU3, RG1AG },
    { INTRU4, RG2AG },
    { INTRU5, RG3AG },
    { INTRU6, RG4AG },
    { INTRU8, INTRU_RG5_GP1AG },
    { INTRU8, INTRU_RG6_GP1AG }
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

#if 0
//-----------------------------------------------------------------------------
/** _IsReadOnly() Check if the given region is a read-only region
 *  @param u4Region: The region to be checked
 *  @retval TRUE: A read-only region
 *  @retval FALSE: Not a read-only region
 */
//-----------------------------------------------------------------------------
static BOOL _IsReadOnly(UINT32 u4Region)
{
    if (u4Region < MID_MAX_RW_REGIONS)
    {
        return 0;
    }
    else if (u4Region >= MID_MAX_REGIONS)
    {
        return 0;
    }

    // all regions 3/4/5/6 can be read only.
    return 1;
}
#endif

//-----------------------------------------------------------------------------
/** _ResetRegion() Reset a region to normal state
 *  @param u4Region: The region to be reset
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _ResetRegion(UINT32 u4Region)
{
    REGION_STATE_T* prRegion;

    if (u4Region >= MID_MAX_REGIONS)
    {
        return FALSE;
    }

    vIO32WriteFldAlign(_arRegionUp[u4Region], 0, MSK32BIT);
    vIO32WriteFldAlign(_arRegionLw[u4Region], 0, MSK32BIT);

    prRegion = &_arRegionState[u4Region];
    prRegion->fgSet = FALSE;
    prRegion->fgEnabled = FALSE;
    prRegion->u4LowAddr = 0;
    prRegion->u4HighAddr = 0;
    prRegion->eAgentId = MID_AGENT_INVALID;

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
    BOOL fgRet;
    REGION_STATE_T* prRegion;

    prRegion = &_arRegionState[u4Region];
    fgRet = FALSE;

    if (prRegion->fgSet)
    {
        if (!prRegion->fgEnabled)
        {
            vIO32WriteFldAlign(_arRegionUp[u4Region], 1, PROT0);
#if 0
            if (u4Region == 4)
            {
                DRAM_REG32(REG_REGION4UPPER) |= REGION_ENABLE;
            }
            else
            {
                DRAM_REG32(REG_REGION0UPPER + (u4Region * 8)) |= REGION_ENABLE;
            }
#endif
            prRegion->fgEnabled = TRUE;
        }

        fgRet = TRUE;
    }

    return fgRet;
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
    BOOL fgRet;
    REGION_STATE_T* prRegion;

    prRegion = &_arRegionState[u4Region];
    fgRet = FALSE;

    if (prRegion->fgSet)
    {
        if (prRegion->fgEnabled)
        {
            vIO32WriteFldAlign(_arRegionUp[u4Region], 0, PROT0);
#if 0
            if (u4Region == 4)
            {
                DRAM_REG32(REG_REGION4UPPER) &= ~REGION_ENABLE;
            }
            else
            {
                DRAM_REG32(REG_REGION0UPPER + (u4Region * 8)) &= ~REGION_ENABLE;
            }
#endif
            prRegion->fgEnabled = FALSE;
        }

        fgRet = TRUE;
    }

    return fgRet;
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
        LINT_SUPPRESS_NEXT_EXPRESSION(534)
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

#if 0
//-----------------------------------------------------------------------------
/** _GrayToBinary() Translate Gray code to binary
 *  @param u4Gray: The Gray code to be translated
 *  @return The binary code
 */
//-----------------------------------------------------------------------------
static UINT32 _GrayToBinary(UINT32 u4Gray)
{
    UINT32 u4Binary, i;

    u4Binary = 0;

    for (i = 0; i < 32; i++)
    {
        UINT32 u4Bit, b;

        u4Bit = 31 - i;
        b = ((u4Gray >> u4Bit) ^ u4Binary) & 0x1;
        u4Binary = (u4Binary << 1) | b;
    }

    return u4Binary;
}
#endif

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

    // Detect the region intruded
    for (u4Region = 0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        UINT32 u4IntrudeStatus; //  = DRAM_READ32(REG_INTRUDER_0 + (u4Region * 4));

        u4IntrudeStatus = IO32ReadFldAlign(_arRegionStatus[u4Region][0], _arRegionStatus[u4Region][1]);     // DRAM_READ32(REG_INTRUDER_4);

        if (u4IntrudeStatus) // ((u4IntrudeStatus & INTRU_INTRUDED) != 0)
        {
            UINT32 u4AgentId, u4Addr;

            // (u4IntrudeStatus << INTRU_ADDR_SHIFT) & INTRU_ADDR_MASK;
            u4Addr = IO32ReadFldAlign(_arRegionAddr[u4Region], RG0ADR) << MID_ADDR_ALIGN_SHIFT;

            u4AgentId = IO32ReadFldAlign(_arRegionAgent[u4Region][0], _arRegionAgent[u4Region][1]);
            // (u4IntrudeStatus >> INTRU_AGENTID_SHIFT) & INTRU_AGENTID_MASK;
            // ASSERT(u4AgentId <= (UINT32)MID_AGENT_GROUP3);
#if 0
            // Convert agent id if it's group 2 or 3
            if (u4AgentId == (UINT32)MID_AGENT_GROUP2)
            {
                UINT32 u4ExtStatus, u4GrayId, u4Group2Id;

                if (u4Region == 4)
                {
                    u4ExtStatus = DRAM_READ32(REG_INTRUDER_EXT2);
                    u4GrayId = (u4ExtStatus >> G2_REGION4_SHIFT) & G23_AGENT_ID_MASK;
                }
                else
                {
                    u4ExtStatus = DRAM_READ32(REG_INTRUDER_EXT);
                    u4GrayId = (u4ExtStatus >> (G2_REGION0_SHIFT - (u4Region * 4)))
                        & G23_AGENT_ID_MASK;
                }

                u4Group2Id = _GrayToBinary(u4GrayId);
                ASSERT(u4Group2Id < ((UINT32)GROUP3_FIRST_AGENT_ID - GROUP2_FIRST_AGENT_ID));

                u4AgentId = GROUP2_FIRST_AGENT_ID + u4Group2Id;
            }
            else if (u4AgentId == (UINT32)MID_AGENT_GROUP3)
            {
                UINT32 u4ExtStatus, u4GrayId, u4Group3Id;

                if (u4Region == 4)
                {
                    u4ExtStatus = DRAM_READ32(REG_INTRUDER_EXT2);
                    u4GrayId = (u4ExtStatus >> G3_REGION4_SHIFT) & G23_AGENT_ID_MASK;
                }
                else
                {
                    u4ExtStatus = DRAM_READ32(REG_INTRUDER_EXT);
                    u4GrayId = (u4ExtStatus >> (G3_REGION0_SHIFT - (u4Region * 4)))
                        & G23_AGENT_ID_MASK;
                }

                u4Group3Id = _GrayToBinary(u4GrayId);
                ASSERT(u4Group3Id <= ((UINT32)MID_AGENT_TEST2 - GROUP3_FIRST_AGENT_ID));

                u4AgentId = GROUP3_FIRST_AGENT_ID + u4Group3Id;
            }
#endif
            ASSERT(peAgentId!=NULL);
            ASSERT(pu4Addr!=NULL);
            ASSERT(pu4Region!=NULL);
            *peAgentId = (MID_AGENT_ID_T)u4AgentId;
            *pu4Addr = u4Addr;
            *pu4Region = u4Region;

            return TRUE;
        }
    }

    return FALSE;
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
    ASSERT(u2Vector == VECTOR_DRAMC);

    // Invoke user registered handler
    if (_pfnHandler != NULL)
    {
        MID_AGENT_ID_T eAgentId;
        UINT32 u4Region = 0, u4Addr = 0;

        VERIFY(_GetIntruder(&u4Region, &eAgentId, &u4Addr));

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
    REGION_STATE_T* prRegion;

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

    if (u4Region >= MID_MAX_REGIONS)
    {
        return FALSE;
    }

#if 0
    // Check read-only attribute with predefined region number
    fgReadOnly = _IsReadOnly(u4Region);
    if (fgReadOnly)
    {
        // Should be a read-only region
        if (eAgentId != MID_AGENT_NONE)
        {
            return FALSE;
        }
    }
    else
    {
        // Should be a writable region
        if (eAgentId == MID_AGENT_NONE)
        {
            return FALSE;
        }
    }

    if ((UINT32)eAgentId >= GROUP3_FIRST_AGENT_ID)
    {
        eAgentId = MID_AGENT_GROUP3;
    }
    else if ((UINT32)eAgentId >= GROUP2_FIRST_AGENT_ID)
    {
        eAgentId = MID_AGENT_GROUP2;
    }

    // Find new u4RegionBase...
    if (u4Region > MID_MAX_REGIONS)
    {
        return FALSE;
    }
    else if (u4Region == 4)
    {
        u4RegionBase = REG_REGION4UPPER;
    }
    else
    {
        u4RegionBase = REG_REGION0UPPER + (u4Region * 8);
    }
#endif

    // Check region state?
    _Lock();

    // Setup region on lower register
    vIO32WriteFldAlign(_arRegionLw[u4Region], (u4LowAddr >> MID_ADDR_ALIGN_SHIFT), PROT0LWA);
    vIO32WriteFldAlign(_arRegionLw[u4Region], (eAgentId & 0x3U), PROT0AG);
    vIO32WriteFldAlign(_arRegionLw[u4Region], ((eAgentId >> 2) & 0x7U), PROT0AG1);

    // Setup region on upper register
    vIO32WriteFldAlign(_arRegionUp[u4Region], (u4HighAddr >> MID_ADDR_ALIGN_SHIFT), PROT0UPA);
    vIO32WriteFldAlign(_arRegionUp[u4Region], ((u4Flags & MID_FLAG_CPUACCESS) ? 1 : 0), P0CPU);
    if (u4Region < MID_MAX_RW_REGIONS)
    {
        vIO32WriteFldAlign(_arRegionUp[u4Region], ((u4Flags & MID_FLAG_INVERSE) ? 1 : 0), P0MDE);
    }
    if (u4Region >= MID_MAX_RW_REGIONS)
    {
        vIO32WriteFldAlign(_arRegionUp[u4Region], ((u4Flags & MID_FLAG_WRITEABLE) ? 1 : 0), PROT3WR);
    }

    // Setup agent list region
    if (u4Region == 5)
    {
        vIO32WriteFldAlign(PROT5EN, u4AgentList, MSK32BIT);
    }
    if (u4Region == 6)
    {
        vIO32WriteFldAlign(PROT6EN, u4AgentList, MSK32BIT);
    }

#if 0
    DRAM_WRITE32((u4RegionBase + 4),
        ((u4LowAddr & INTRU_ADDR_MASK) >> INTRU_ADDR_SHIFT) |
        ((u4Flags & MID_FLAG_CPUACCESS) ? (REGION_CPU_ACCESS) : 0)
        );

    // Setup region on upper register
    if (fgReadOnly)
    {
        DRAM_WRITE32(u4RegionBase,
            ((u4HighAddr & INTRU_ADDR_MASK) >> INTRU_ADDR_SHIFT)
            );
        u4Flags = 0;
    }
    else
    {
        DRAM_WRITE32(u4RegionBase,
            ((u4HighAddr & INTRU_ADDR_MASK) >> INTRU_ADDR_SHIFT) |
            (((UINT32)eAgentId & AGENT_ID_MASK) << AGENT_ID_SHIFT) |
            ((u4Flags & MID_FLAG_INVERSE) ? (REGION_SEL_INV) : 0)
            );
    }
#endif

    // Update region state
    prRegion = &_arRegionState[u4Region];
    prRegion->fgSet = TRUE;
    prRegion->fgEnabled = FALSE;
    prRegion->u4LowAddr = u4LowAddr;
    prRegion->u4HighAddr = u4HighAddr;
    prRegion->eAgentId = ((u4Region < MID_SINGLE_REGIONS) ? eAgentId : MID_AGENT_NONE);
    prRegion->u4Flags = u4Flags | ((u4Region < MID_MAX_RW_REGIONS) ? MID_FLAG_WRITEABLE : 0);
    prRegion->u4AgentList = ((u4Region < MID_SINGLE_REGIONS) ? 0 : u4AgentList);
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

    if (!_fgInit)
    {
        return FALSE;
    }

    _Lock();

    if (fgReadOnly)
    {
        for (u4Region = MID_MAX_RW_REGIONS; u4Region < MID_MAX_REGIONS; u4Region++)
        {
            if (!_arRegionState[u4Region].fgSet)
            {
                // Found one
                fgRet = TRUE;
                break;
            }
        }
    }
    else
    {
        for (u4Region = 0; u4Region < MID_MAX_RW_REGIONS; u4Region++)
        {
            if (!_arRegionState[u4Region].fgSet)
            {
                // Found one
                fgRet = TRUE;
                break;
            }
        }
    }

    _Unlock();

    if (fgRet)
    {
        ASSERT(pu4Region!=NULL);
        *pu4Region = u4Region;
    }

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
/** MID_GetRegionState() Get state of a region
 *  @param u4Region -input- The region
 *  @param pu4RegionState -output- The region state
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_GetRegionState(UINT32 u4Region, REGION_STATE_T* pu4RegionState)
{
    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    ASSERT(pu4RegionState!=NULL);
    *pu4RegionState = _arRegionState[u4Region];

    _Unlock();

    return TRUE;
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
    const CHAR* szAgentName = "unknown";

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

    // u4Val = DRAM_READ32(REG_DRAMSTAT_EN);
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
}

//-----------------------------------------------------------------------------
/** MID_ResetStatus() Reset all intrusion status
 *  @retval None
 */
//-----------------------------------------------------------------------------
VOID MID_ResetStatus(void)
{

        // Clear interrupt
    LINT_SUPPRESS_NEXT_EXPRESSION(534)
    vIO32WriteFldAlign(CPU1PROT, 1, INTCLR);
    vIO32WriteFldAlign(CPU1PROT, 0, INTCLR);
    BIM_ClearIrq(VECTOR_DRAMC);

}

//-----------------------------------------------------------------------------
/** MID_QueryIntrusionStatus() Reset all intrusion status
 *  @retval None
 */
//-----------------------------------------------------------------------------
VOID MID_QueryIntrusionStatus(void)
{
    MID_AGENT_ID_T eAgentId;
    UINT32 u4Region = 0, u4Addr = 0;    


    // Detect the region intruded
    for (u4Region = 0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        UINT32 u4IntrudeStatus; //  = DRAM_READ32(REG_INTRUDER_0 + (u4Region * 4));

        u4IntrudeStatus = IO32ReadFldAlign(_arRegionStatus[u4Region][0], _arRegionStatus[u4Region][1]);     // DRAM_READ32(REG_INTRUDER_4);

        if (u4IntrudeStatus)
        {


            u4Addr = IO32ReadFldAlign(_arRegionAddr[u4Region], RG0ADR) << MID_ADDR_ALIGN_SHIFT;

            eAgentId = (MID_AGENT_ID_T)IO32ReadFldAlign(_arRegionAgent[u4Region][0], _arRegionAgent[u4Region][1]);

            Printf("MEMORY VIOLATION - agent: %s, address: 0x%08x\n",
            MID_AgentIdToString(eAgentId), u4Addr);
        }
    }  
}

