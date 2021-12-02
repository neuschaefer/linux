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
 * $RCSfile: dmx_init.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_init.c
 *  Demux driver - initialization
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_ide.h"
#include "dmx_pcr.h"

#undef CC_DMX_USE_CT
#ifndef DMX_NT_UCODE_DISABLE
#define CC_DMX_USE_CT
#endif  // DMX_NT_UCODE_DISABLE

#include "dmx_imem_onebyte.h"

#ifdef __MODEL_slt__
#include "dmx_imem_jchip.h"
#endif  // __MODEL_slt__

#ifdef ENABLE_MULTIMEDIA
#include "dmx_imem_drm.h"
#include "dmx_imem_ps.h"
#endif  // ENABLE_MULTIMEDIA

#include "dmx_imem_test.h"
#include "dmx_debug.h"
#include "x_lint.h"

#include "fvr.h"

LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_util.h"
#include "x_os.h"
#include "x_timer.h"
#include "x_bim.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Define EXT_TS2 to select the second external TS interface. By default the
// first external TS interface is selected
//
//#define EXT_TS2


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NUM_UP_TEST_RESULT_WORDS            16
#define DMX_MAX_DBM_WAIT_COUNT              10

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_INPUT_TYPE_T _eDmxInputType;
static DMX_FRONTEND_T _eDmxFEType = DMX_FE_ATSC_DEMOD;
static UINT32 _u4uCodeVersion = 0;

static UINT8 _u1DmxDTVFramer = 0xFF;

static BOOL _fgMicroProcessorStopped = FALSE;  // Is micro-processor stopped?


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DdmxIsFTIRunning
 *  Query if FTI is running or not
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsFTIRunning(void)
{
    UINT32 u4State;

    u4State = (DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3;
    LOG(7, "uP state %d\n", u4State);

    return 0 == u4State;
}


//-----------------------------------------------------------------------------
/** _DmxCkgenInit
 *  Select clock and pin mux for FTI
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxCkgenInit(BOOL fgOn)
{
    UINT32 u4Ctrl = 0;

    if(fgOn)
    {
        CKGEN_WRITE32(0x22c, 0xA);            // 216MHz
        //CKGEN_WRITE32(0x22c, 0x2);            // 180MHz
        //CKGEN_WRITE32(0x22c, 0x1);            // 162MHz

        u4Ctrl = (CKGEN_READ32(0x288) & (~0x80)) | 0x80;
        CKGEN_WRITE32(0x288, u4Ctrl);

        _DMX_Lock();

        u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
        u4Ctrl &= ~0x1;
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);// Power on to dram clock

        CKGEN_WRITE32(0x24C, 0x0);          // Frame0: ATSC demod

        // Set DMX SRAM used by DMX
        u4Ctrl = DMXCMD_READ32(DMX_REG_FTUPCR) & 0xFBFFFFFF;
        DMXCMD_WRITE32(DMX_REG_FTUPCR, u4Ctrl);

        _fgMicroProcessorStopped = FALSE;
        _DMX_Unlock();
    }
    else
    {
        _DMX_Lock();
        _fgMicroProcessorStopped = TRUE;
        _DMX_Unlock();
        u4Ctrl = CKGEN_READ32(0x288) & (~0x80);
        CKGEN_WRITE32(0x288, u4Ctrl);
    }
}


//-----------------------------------------------------------------------------
/** _DMX_CkgenToggle
 */
//-----------------------------------------------------------------------------
void _DMX_CkgenToggle(void)
{
    UINT32 i, u4Ctrl;

    // Force toggle clock to guarantee switch clock success
    u4Ctrl = CKGEN_READ32(0x24C) & 0xFFFFFF0F;
    for(i=0; i<5; i++)
    {
        HAL_Delay_us(1);
        CKGEN_WRITE32(0x24C, u4Ctrl | 0xF0);
        HAL_Delay_us(1);
        CKGEN_WRITE32(0x24C, u4Ctrl);
    }
}


//-----------------------------------------------------------------------------
/** _DMX_LoadIMem
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_LoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len)
{
    UINT32 u4Cmd, u4Data, i;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    ASSERT(pu4IData != NULL);

    // Check uP status
    if (_DmxIsFTIRunning())
    {
        if (!fgForceWrite)
        {
            LOG(7, "FTI uP is running, can't load to I-mem!\n");
            return FALSE;
        }
        else
        {
            LOG(7, "FTI uP is running, now will halt it\n");
            _DMX_EnableFTI(FALSE);
            while (_DmxIsFTIRunning())
            {
                LOG(5, "wait uP stop...\n");
            }
        }
    }

    // Clear the "FTuP instruction mode" flag.
    DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= ~(1 << 25);

    // Load to I-mem
    for (i = 0; i < u4Len; i++)
    {

        _DMX_Lock();

        // Issue a write command
        DMXCMD_WRITE32(DMX_REG_MEM_DATA, pu4IData[i]);
        u4Cmd = (UINT32)((1 << 0) | (2 << 8) | (i << 16));
        DMXCMD_WRITE32(DMX_REG_MEM_CMD,  u4Cmd);

        // Wait for ready
        while (1)
        {
            u4Cmd = DMXCMD_READ32(DMX_REG_MEM_CMD);
            if (((u4Cmd >> 8) & 0xf) == 0)
            {
                break;
            }
        }

        _DMX_Unlock();
    }

    // Verify
    for (i = 0; i < u4Len; i++)
    {
        _DMX_Lock();

        // Issue a read command
        u4Cmd = (UINT32)((1 << 0) | (1 << 8) | (i << 16));
        DMXCMD_WRITE32(DMX_REG_MEM_CMD, u4Cmd);

        // Wait for ready
        while (1)
        {
            u4Cmd = DMXCMD_READ32(DMX_REG_MEM_CMD);
            if (((u4Cmd >> 8) & 0xf) == 0)
            {
                break;
            }
        }

        _DMX_Unlock();

        // Verify
        u4Data = DMXCMD_READ32(DMX_REG_MEM_DATA);
        if (u4Data != pu4IData[i])
        {
            LOG(1, "Load IMem error, at word %u, write 0x%08x, read 0x%08x\n",
                i, pu4IData[i], u4Data);
            return FALSE;
        }
    }

    _u4uCodeVersion = pu4IData[0];
    u2CodeVersion = (UINT16)(pu4IData[0] & 0xffff);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0xf);
    u1Minor = (UINT8)(u2CodeVersion & 0xff);
    LOG(5, "Demux uCode version: %u.%u\n", u1Major, u1Minor);

    // Reset demux (uP), so uP can run into the entry pointer of uCode
    VERIFY(_DMX_Reset());

    // Suppress warnings in lint and release build
    UNUSED(u1Major);
    UNUSED(u1Minor);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxInitFVR
 */
//-----------------------------------------------------------------------------
static void _DmxInitFVR(void)
{
    UINT32 i, u4Ctrl;

    //-----------------------------------------------------
    // Reset DMX Record hardware
    //-----------------------------------------------------
    x_memset((void*)&(FVR_GBL_PID_W(0)), 0, FVR_GBL_SIZE * 4);
    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_INDEX_TABLE(i) = 0;
    }
    // Clear video PID
    for(i=0; i<FVR_PER_PID_NUM; i++)
    {
        x_memset((void*)&(FVR_PER_PID_VID_S(i)), 0, FVR_PER_PID_VIDEO_SIZE * 4);
    }

    if(!_DMX_GetIsPanicReset())
    {
        // Clear Initial Vector
        x_memset((void*)((UINT32*)FVR_PER_PID_IV_BASE), 0, FVR_PER_PID_IV_SIZE * 4);

        // Clear Key
        for(i=0; i<FVR_PER_PID_KEY_NUM; i++)
        {
            x_memset((void*)&(FVR_PER_PID_KEY_S(i)), 0, FVR_PER_PID_KEY_SIZE * 4);
        }
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF;
    u4Ctrl |= (FVR_PER_PID_OFFSET) << 16;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFD00FEFF;
    u4Ctrl |= (1 << 25);                    // Disable record path
    u4Ctrl |= ((FVR_PER_PID_KEY_SIZE * 4) << 16); // Set key pid size
    //u4Ctrl |= (1 << 8);                     // Direct map
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Disable insert timestamp
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
}


//-----------------------------------------------------------------------------
/** _DMX_TSSoftInit
 *  Soft-initialize demux
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TSSoftInit(void)
{
    UINT32 u4Ctrl, i;
    UINT32 u4RegVal;

    _DMX_Lock();

    //---------------------------------------------------------
    // Clear FVR SRAM
    //---------------------------------------------------------
    // Clear global table
    x_memset((void*)&(FVR_GBL_PID_W(0)), 0, FVR_GBL_SIZE * 4);
    // Clear Per-Pid sram
    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_INDEX_TABLE(i) = 0;
    }

    //---------------------------------------------------------
    // Clear DMX SRAM and struct
    //---------------------------------------------------------
    // Clear PID index table and PID structures
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_STRUCT_T* prPidStruct;
        UINT8 u1SerialNum;

        PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(PID_S(i)), 0, DMX_DMEM_ENTRY_LEN * 4);
        prPidStruct = _DMX_GetPidStruct(i);
        u1SerialNum = prPidStruct->u1SerialNumber;
        x_memset((void*)prPidStruct, 0, sizeof(PID_STRUCT_T));
        prPidStruct->u1SerialNumber = u1SerialNum;

        // Debug, reset continuity counter to 0xff
        PID_S_W(i, 3) = 0xffff0000;
    }

    // Clear Pattern match DMEM
    x_memset((void*)DMX_PATTERN_MATCH_BASE, 0, DMX_DMEM_PATTERN_MATCH_LEN * 4);

    // Set the byte 3 of CT_SETTING to 0xFF which represents an invalid
    // PID Index.  Other values greater than 31 will do, but I simply use 0xFF.
    // E-fuse
    u4RegVal = BIM_READ32(0x660);
    if(u4RegVal & 0x200000)
    {
        // Demod does not clear VCT.  DMX has the choice to clear VCT or not.
#ifdef CC_DMX_USE_CT
        CT_SETTING = 0xFF000002;  // DMX clears VCT.
#else
        CT_SETTING = 0xFF000000;  // DMX does not clear VCT.
#endif  // CC_DMX_USE_CT
    }
    else
    {
        // Demod clears VCT.
        CT_SETTING = 0xFF000001;  // DMX must use the CRC result from Demod.
#ifndef CC_DMX_USE_CT
        ASSERT(0);  // There shall be no such usage scenario.
#endif  // CC_DMX_USE_CT
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF0000;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    // Clear section filters
    for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
    {
        FILTER_STRUCT_T *prFilterStruct;

        prFilterStruct = _DMX_GetFilterStruct(i);
        x_memset((void*)prFilterStruct, 0, sizeof(FILTER_STRUCT_T));
    }

    x_memset((void*)&(PID_S(34)), 0, 64);

    _DMX_Unlock();

    // Clear CA Mem
    if(!_DMX_GetIsPanicReset())
    {
        x_memset((VOID*)DMX_SECTION_CA_EVEN_KEY, 0x0, 0x200);
        x_memset((VOID*)DMX_SECTION_CA_ODD_KEY, 0x0, 0x200);
    }

    // Initialize interrupt handler
    _DMX_InitISR();

    // Initialize common routines
    VERIFY(_DMX_InitAPI());

    _DMX_Lock();

    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, 0xf0c0f0c0);

    //-----------------------------------------------------
    // Record default setting
    //-----------------------------------------------------
    // Disable record
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) | (1 << 25);
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Set packet length to 188 bytes
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0x08;          // maximum burst size
    u4Ctrl = (u4Ctrl & 0xFFFF00FF) | (188 << 8);    // packet len = 188 bytes
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x47 << 16);  // TS packet sync byte
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);

    // Setup framer control
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xffffff00) | 0xff;      // Positive edge
//    u4Ctrl = (u4Ctrl & 0xffffff00) | 0x77;      // Negative edge
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    // Set DBM to normal mode and enable DBM
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl = (u4Ctrl & 0xcfffffff) | (1 << 30);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);

    // Set PID structure size
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1);
    u4Ctrl &= 0xffffEf00;                       // Select Group 1 DRAM agent
    u4Ctrl |= (DMX_DMEM_ENTRY_LEN * 4);         // Size of each PID data
    u4Ctrl |= (1 << 9);                         // DMA delay ack, debug
    u4Ctrl |= (1 << 10);                        // Enable multi-PID channel
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    _DMX_Unlock();

    _DMX_ResetFramer();

    return TRUE;
}

#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
/** _DmxPSSoftInit
 *  Soft-initialization demux for program stream
 */
//-----------------------------------------------------------------------------
static BOOL _DmxPSSoftInit(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    // Clear PID index table, PID data memory, and the PID structure
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_STRUCT_T* prPidStruct;

        PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(PID_S(i)), 0, DMX_DMEM_ENTRY_LEN * 4);
        prPidStruct = _DMX_GetPidStruct(i);
        x_memset((void*)prPidStruct, 0, sizeof(PID_STRUCT_T));

        // Debug, reset continuity counter to 0xff
        PID_S_W(i, 3) = 0xffff0000;
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF0000;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    // Clear Pattern match DMEM
    x_memset((void*)&(PID_S(DMX_DMEM_PATTERN_MATCH_INDEX)), 0, DMX_DMEM_PATTERN_MATCH_LEN * 4);
    PID_S_W(DMX_DMEM_PATTERN_MATCH_INDEX, 0) = ((UINT32)6 << 8);  // No. of pattern = 6.

    // Set "Stop Interrupt" to 0.
    // Set "VCD .DAT mode (Data Format)" to 0 (non-VCD format).
    // Word offset from PID index 33: 19
    PID_S_W(33, 19) &= 0xFFFF0000;

    _DMX_Unlock();
    VERIFY(_DMX_SetVideoType(DMX_VIDEO_MPEG));  // Set Start Code Patterns
    _DMX_InitISR();                             // Initialize ISR
    VERIFY(_DMX_InitAPI());                     // Initialize common routines
    _DMX_Lock();

    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, 0);

    // FIXME: Enable section CRC-checking
    // DMXCMD_REG32(DMX_REG_CONFIG2) |= (1 << 24);

    // Set packet length to 188 bytes
    VERIFY(_DMX_SetPacketSize(188));

    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xffffff00) | 0xff;  // Enable, Parallel, External Sync, Positive
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    // Set DBM to bypass mode and keep all TS packets
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl = u4Ctrl & 0xC3FFFFFF;
    u4Ctrl |= (1 << 30) | (1 << (26 + DDI_TS_INDEX_FOR_NO_FRAMER));
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);

    // Set PID structure size
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1);
    u4Ctrl &= 0xffffEf00;
    u4Ctrl |= (DMX_DMEM_ENTRY_LEN * 4);         // Size of each PID data
    u4Ctrl |= (1 << 9);                         // DMA delay ack, debug
    u4Ctrl |= (1 << 12);                        // Select Group 2 DRAM agent
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Spec: Set PES/PSI Steering Logic (word offset: 38) to bypass mode.
    // I suspect the statement above in the 5382P FTI Spec is no longer true.

    PID_W(0) &= ~0x01;                      // Init to not perform preparsing

    _DMX_Unlock();

    _DMX_ResetFramer();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxESSoftInit
 *  Soft-initialization demux for elementary stream
 */
//-----------------------------------------------------------------------------
static BOOL _DmxESSoftInit(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    // Clear PID index table, PID data memory, and the PID structure
    // Only 180 bytes are used, but we still clear most of the PID memory.
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_STRUCT_T* prPidStruct;

        PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(PID_S(i)), 0, DMX_DMEM_ENTRY_LEN * 4);
        prPidStruct = _DMX_GetPidStruct(i);
        x_memset((void*)prPidStruct, 0, sizeof(PID_STRUCT_T));
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF0000;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    _DMX_Unlock();
    VERIFY(_DMX_SetVideoType(DMX_VIDEO_MPEG));   // Set Start Code Patterns
    _DMX_InitISR();                             // Initialize ISR
    VERIFY(_DMX_InitAPI());                     // Initialize common routines
    _DMX_Lock();

    // Disable framer packet error handling
    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, 0);

    // FIXME: Enable section CRC-checking
    // DMXCMD_REG32(DMX_REG_CONFIG2) |= (1 << 24);

    // Set packet length to 188 bytes
    VERIFY(_DMX_SetPacketSize(188));

    // Setup framer control
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0xFF;  // Enable, Parallel, External Sync, Positive
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    // Set DBM to bypass mode and keep all TS packets
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl = u4Ctrl & 0xC3FFFFFF;
    u4Ctrl |= (1 << 30) | (1 << (26 + DDI_TS_INDEX_FOR_NO_FRAMER));
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);

    // Set PID structure size
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1);
    u4Ctrl &= 0xFFFFEF00;
    u4Ctrl |= DMX_DMEM_ENTRY_LEN * 4;           // Size of each PID data
    u4Ctrl |= (1 << 9);                         // DMA delay ack, debug
    u4Ctrl |= (1 << 12);                        // Select Group 2 DRAM agent
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    _DMX_Unlock();

    _DMX_ResetFramer();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxMMSoftInit
 *  Soft-initialization demux for container
 */
//-----------------------------------------------------------------------------
static BOOL _DmxMMSoftInit(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    // Clear PID index table, PID data memory, and the PID structure
    // Only 180 bytes are used, but we still clear most of the PID memory.
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_STRUCT_T* prPidStruct;

        PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(PID_S(i)), 0, DMX_DMEM_ENTRY_LEN * 4);
        prPidStruct = _DMX_GetPidStruct(i);
        x_memset((void*)prPidStruct, 0, sizeof(PID_STRUCT_T));
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF0000;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    _DMX_Unlock();
    VERIFY(_DMX_SetVideoType(DMX_VIDEO_MPEG));  // Set Start Code Patterns
    _DMX_InitISR();                             // Initialize ISR
    VERIFY(_DMX_InitAPI());                     // Initialize common routines
    _DMX_Lock();

    // Disable framer packet error handling
    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, 0);

    // FIXME: Enable section CRC-checking
    // DMXCMD_REG32(DMX_REG_CONFIG2) |= (1 << 24);

    // Set packet length to 188 bytes
    VERIFY(_DMX_SetPacketSize(188));

    // Setup framer control
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0xFF;  // Enable, Parallel, External Sync, Positive
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    // Set DBM to bypass mode and keep all TS packets
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl = u4Ctrl & 0xC3FFFFFF;
    u4Ctrl |= (1 << 30) | (1 << (26 + DDI_TS_INDEX_FOR_NO_FRAMER));
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);

    // Set PID structure size
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1);
    u4Ctrl &= 0xFFFFEF00;
    u4Ctrl |= DMX_DMEM_ENTRY_LEN * 4;           // Size of each PID data
    u4Ctrl |= (1 << 9);                         // DMA delay ack, debug
    u4Ctrl |= (1 << 12);                        // Select Group 2 DRAM agent
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    _DMX_Unlock();

    _DMX_ResetFramer();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DmxInitPlaybackTS
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitPlaybackTS(void)
{
    DMX_DDI_T rDDI;
    UINT32 u4Ctrl;

    // Load default uCode (for normal transport stream)
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM, FTI_IMEM_LEN))
    {
        return FALSE;
    }

    // Soft init
    if (!_DMX_TSSoftInit())
    {
        return FALSE;
    }

    _DmxInitFVR();

    // Select Group 2 DRAM agent
    DMXCMD_WRITE32(DMX_REG_CONFIG1, DMXCMD_READ32(DMX_REG_CONFIG1) | (1 << 12));

    // Important: Set framer after calling _DMX_TSSoftInit().
    // Be sure to set the register while the FTI is disabled.
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0xBB;  // Enable, Parallel, Internal Sync, Positive
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x03 << 16);  // Sync Lock
    u4Ctrl = (u4Ctrl & 0x00FFFFFF) | (0x01 << 24);  // Sync Loss
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    _DMX_ResetFramer();

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    // Init DDI, and set DDI properties.
    if(!_DMX_DDI_Init())
    {
        LOG(3, "Failed to initialize DDI!\n");
        return FALSE;
    }
    rDDI.eMode = DMX_DDI_MODE_SINGLE;
    rDDI.u4RateN = 0;       // (N/8M) MB/sec, 0/0: full speed
    rDDI.u4RateM = 0;
    if(!DMX_DDI_Set((UINT32)(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE), &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return FALSE;
    }

    // For 192 encrypt file
    if(!_DMX_PVRPlay_SetUseDescramble(FALSE))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxInitPS
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitPS(void)
{
    DMX_DDI_T rDDI;

    // Load default uCode (for Program Stream playback)
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM_PS, FTI_IMEM_PS_LEN))
    {
        return FALSE;
    }

    // Soft init
    if (!_DmxPSSoftInit())
    {
        return FALSE;   // Never get here for current implementation.
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    // For PS uCode version 3.14 and earilier, word offset 17 stores
    // "PES header length" and "PES header data byte 0/1".
    // For PS uCode version 3.15 and later, word offset 17 stores
    // "uCode state" and "PES header remaining length".
    // To eliminate a race condition, clear this word (the uCode state)
    // at appropriate time.  (2009/04/17)
    PID_W(17) = 0;

    // Init DDI, and set DDI properties.
    if(!_DMX_DDI_Init())
    {
        LOG(3, "Failed to initialize DDI!\n");
        return FALSE;
    }
    rDDI.eMode = DMX_DDI_MODE_SINGLE;
    rDDI.u4RateN = 0;       // (N/8M) MB/sec, 0/0: full speed
    rDDI.u4RateM = 0;
    if(!DMX_DDI_Set((UINT32)(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE), &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxInitES
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitES(void)
{
    DMX_DDI_T rDDI;

    // Load default uCode (for DivX/WMV DRM or ES playback)
    if (!_DMX_LoadIMem(TRUE, DRM_IMEM, DRM_IMEM_LEN))
    {
        return FALSE;
    }

    // Soft init
    if (!_DmxESSoftInit())
    {
        return FALSE;   // Never get here for current implementation.
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    // Init DDI, and set DDI properties.
    if(!_DMX_DDI_Init())
    {
        LOG(3, "Failed to initialize DDI!\n");
        return FALSE;
    }
    rDDI.eMode = DMX_DDI_MODE_SINGLE;
    rDDI.u4RateN = 0;       // (N/8M) MB/sec, 0/0: full speed
    rDDI.u4RateM = 0;
    if(!DMX_DDI_Set((UINT32)(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE), &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxInitMM
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitMM(void)
{
    DMX_DDI_T rDDI;

    // Load default uCode (for DivX/WMV DRM or ES playback)
    if (!_DMX_LoadIMem(TRUE, DRM_IMEM, DRM_IMEM_LEN))
    {
        return FALSE;
    }

    // Soft init
    if (!_DmxMMSoftInit())
    {
        return FALSE;   // Never get here for current implementation.
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    // Select Group 2 DRAM agent
    DMXCMD_WRITE32(DMX_REG_CONFIG1, DMXCMD_READ32(DMX_REG_CONFIG1) | (1 << 12));

    if(!_DMX_DDI_Init())
    {
        return FALSE;
    }

    rDDI.eMode = DMX_DDI_MODE_SINGLE;
    //rDDI.u4RateN = 32;  // 4MBytes
    //rDDI.u4RateM = 1;
    rDDI.u4RateN = 0;
    rDDI.u4RateM = 0;
    if(!DMX_DDI_Set((UINT32)(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE), &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return FALSE;
    }

    if(!_DMX_MM_Init())
    {
        return FALSE;
    }

    return TRUE;
}
#endif  // ENABLE_MULTIMEDIA

//-----------------------------------------------------------------------------
/** _DmxInitBroadcastTS
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitBroadcastTS(void)
{
    UINT32 u4Ctrl;

#ifdef DMX_MEASURE_PSI_TIME
    _DMX_PSI_ResetMaxTime();
#endif  // DMX_MEASURE_PSI_TIME
    // Load default uCode (for normal transport stream)
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM, FTI_IMEM_LEN))
    {
        return FALSE;
    }

    // Soft init
    if (!_DMX_TSSoftInit())
    {
        return FALSE;
    }

    _DmxInitFVR();

    _DMX_Lock();
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    // Set DBM output spacing
    u4Ctrl = (u4Ctrl & 0xff00ffff) | (8 << 16);
    // Set DBM max playback TS packets
    u4Ctrl = (u4Ctrl & 0xffff00ff) | (1 << 8);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);
    _DMX_Unlock();

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxInitDiag(void)
{
    // Load test uCode
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM_TEST_LATCH, FTI_IMEM_LEN_TEST_LATCH))
    {
        return FALSE;
    }

    // Reset FTI
    if (!_DMX_TSSoftInit())
    {
        return FALSE;
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    return TRUE;
}


#ifdef __MODEL_slt__
//-----------------------------------------------------------------------------
static BOOL _DmxInitJChip(void)
{
    // Load the uCode for J-chip SLT.
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM_JCHIP, FTI_IMEM_JCHIP_LEN))
    {
        return FALSE;
    }

    // Reset FTI
    if (!_DMX_TSSoftInit())
    {
        return FALSE;
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    return TRUE;
}
#endif  // __MODEL_slt__


//-----------------------------------------------------------------------------
/** _DmxSetFrontEnd
 */
//-----------------------------------------------------------------------------
static BOOL _DmxSetFrontEnd(DMX_FRONTEND_T eFEType, BOOL fgSwitchTSSrc)
{
    UINT32 u4Ctrl;

    _DMX_Lock();

    switch(eFEType)
    {
    case DMX_FE_ATSC_DEMOD:
        if(fgSwitchTSSrc)
        {
            u4Ctrl = CKGEN_READ32(0x24C) & 0xfffffff8;
            CKGEN_WRITE32(0x24C, u4Ctrl);
        }
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4Ctrl = (u4Ctrl & 0xffffff00) | 0xff;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        _u1DmxDTVFramer = 0;       // DTV framer
        break;

    case DMX_FE_DVB_DEMOD:

        _DMX_Unlock();
        LOG(3, "Not support internal DVB demod\n");
        return TRUE;

    case DMX_FE_EXT_PARALLEL:

        if(fgSwitchTSSrc)
        {
            u4Ctrl = CKGEN_READ32(0x24c) & 0xfffffff8;
            u4Ctrl |= 0x3;
            CKGEN_WRITE32(0x24c, u4Ctrl);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4Ctrl = (u4Ctrl & 0xffffff00) | 0xff;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        _u1DmxDTVFramer = 0;       // DTV framer
        break;

    case DMX_FE_EXT_SERIAL:

        if(fgSwitchTSSrc)
        {
            u4Ctrl = CKGEN_READ32(0x24c) & 0xfffffff8;
            u4Ctrl |= 0x3;
            CKGEN_WRITE32(0x24c, u4Ctrl);
        }

        // Setup framer control to internal sync
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4Ctrl = (u4Ctrl & 0xffffff00) | 0xdd;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        _u1DmxDTVFramer = 0;       // DTV framer
        break;

    default:
        _DMX_Unlock();
        LOG(1, "Unknown front end type\n");
        return FALSE;
    }

    _DMX_Unlock();

    // Force toggle clock to guarantee switch clock success
    _DMX_CkgenToggle();

    _DMX_ResetFramer();

    _eDmxFEType = eFEType;

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_ResetDbmSafely
 *
 *  Reset DBM safely (at packet boundary) in three steps.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetDbmSafely(VOID)
{
    BOOL fgRet = TRUE;
    UINT32 u4Reg, u4Counter;

    // 1/3. Disable DBM
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg &= ~(1 << 30);                            // Clear "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

    // 2/3. Polling the value of 0x20017410
    //      if bit[16..12] != 0x10
    //          then reset DBM
    u4Counter = 0;
    while (u4Counter < DMX_MAX_DBM_WAIT_COUNT)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
        if (((u4Reg >> 12) & 0x1F) == 0x10)
        {
            x_thread_delay(1);
            u4Counter++;
        }
        else
        {
            break;
        }
    }

    // 3/3. Reset DBM which also resets Framer and PID index table. Enable DBM.
    if (u4Counter == DMX_MAX_DBM_WAIT_COUNT)
    {
        LOG(3, "Failed to reset DBM safely (still outputing TS packets)!\n");
        fgRet = FALSE;
    }
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg |= 0x80000000;                            // Set "Reset" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    u4Reg &= 0x7FFFFFFF;                            // Clear "Reset" bit
    u4Reg |= (1 << 30);                             // Set "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

    return fgRet;
}


#ifdef ENABLE_MULTIMEDIA
//-----------------------------------------------------------------------------
/** _DMX_ClearInterruptQueue
 */
//-----------------------------------------------------------------------------
VOID _DMX_ClearInterruptQueue(VOID)
{
    UINT32 i;

    _DMX_Lock();

    DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
    }

    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_ResetFTuP
 *
 *  This function resets the uP, and sets its "boot address" to 0.
 */
//-----------------------------------------------------------------------------
VOID _DMX_ResetFTuP(VOID)
{
    // Reset uP and its booting address.
    UINT32 u4Reg;
    _DMX_Lock();
    _fgMicroProcessorStopped = TRUE;
    _DMX_Unlock();
    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg |= 0x81000000;                        // Disable "FTuP Stalling"
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);
    while (1)
    {
        if ((DMXCMD_READ32(DMX_REG_FTuP_CONTROL) & (1 << 30)) != 0)
        {
            break;
        }
    }

    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg &= 0xFeFF0000;        // Enable "FTuP Stalling"; Clear "Boot address"
    u4Reg |= 0x8;               // Change "Boot address" to 8.
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);        // Write it twice!
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);        // Write it twice!

    _DMX_ClearInterruptQueue();

    // Enable uP
    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg &= 0x7FFF0000;        // Change "Boot address" to 0.
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);
    _DMX_Lock();
    _fgMicroProcessorStopped = FALSE;
    _DMX_Unlock();
}
#endif  // ENABLE_MULTIMEDIA

//-----------------------------------------------------------------------------
/** _DMX_EnableFTI
 *  Enable or disable FTI
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_EnableFTI(BOOL fgEnable)
{
    UINT32 u4Ctrl, i;

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
    if (fgEnable)
    {
        // Enable FTI
        u4Ctrl &= 0x3fffffff;
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);

        // Enable uP
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= 0x7fffffff;
        _DMX_Lock();
        _fgMicroProcessorStopped = FALSE;
        _DMX_Unlock();
    }
    else
    {
        // Halt uP
        //DMXCMD_REG32(DMX_REG_FTuP_CONTROL) |= 0x80000000;
        // MT5391, turn on "bit[24]: disble uP stall"
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) |= 0x81000000;   // MT5391
        while (1)
        {
            if ((DMXCMD_READ32(DMX_REG_FTuP_CONTROL) & (1 << 30)) != 0)
            {
                break;
            }
        }

        // MT5391, turn off "bit[24]: disble uP stall"
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= 0xfeffffff;

        // Clear all interrupts
        for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
        {
            DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
            DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
        }

        // Halt FTI
        _DMX_Lock();
        _fgMicroProcessorStopped = TRUE;
        _DMX_Unlock();
        u4Ctrl = ((u4Ctrl & 0x3fffffff) | 0x40000000);
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);

        while(1)
        {
            // Check if FTI is halted
            if (0x1 != ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
            {
                x_thread_delay(1);
            }
            else
            {
                break;
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** _DMX_Reset
 *  Reset FTI
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Reset(void)
{
    UINT32 i;

    _DMX_Lock();

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    }

    // Reset FTI
    _fgMicroProcessorStopped = TRUE;
    DMXCMD_WRITE32(DMX_REG_CONTROL, 0x8f000000);

    _DMX_Unlock();

    // Wait for ready
    while(1)
    {
        // Check if FTI is halted
        if (0x1 != ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
        {
            x_thread_delay(1);
        }
        else
        {
            break;
        }
    }

#ifdef __PCR_RECOVERY__
    // Initialize PCR whenever demux is reset
    if(!_DMX_GetIsPanicReset())
    {
        _DMX_PcrInit();
    }
#endif  // __PCR_RECOVERY__

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFrontEnd
 *  Select front end
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFrontEnd(DMX_FRONTEND_T eFEType)
{
    if(_DMX_GetInputType() != DMX_IN_BROADCAST_TS)
    {
        // Only keep value, not do hardware switch
        _eDmxFEType = eFEType;
        return TRUE;
    }

    return _DmxSetFrontEnd(eFEType, TRUE);
}


//-----------------------------------------------------------------------------
/** _DMX_GetFrontEnd
 */
//-----------------------------------------------------------------------------
DMX_FRONTEND_T _DMX_GetFrontEnd(void)
{
    return _eDmxFEType;
}


//-----------------------------------------------------------------------------
/** _DMX_ResetFramer
 */
//-----------------------------------------------------------------------------
void _DMX_ResetFramer(void)
{
    UINT32 u4Ctrl;

    // Reset framer
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0xFF00);
    u4Ctrl |= 0xFF00;
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    //x_thread_delay(1);
    HAL_Delay_us(1);
    u4Ctrl &= (~0xFF00);
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    LOG(9, "Reset framer\n");
}


//-----------------------------------------------------------------------------
/** _DMX_GetFramerIndex
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_GetFramerIndex(void)
{
    return _u1DmxDTVFramer;
}


//-----------------------------------------------------------------------------
/** _DMX_Start
 *  Start demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Start(void)
{
    UINT32 u4Ctrl;

    _DMX_Lock();

    // Enable framers
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl |= (1 << 0) | (1 << 4);
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Stop
 *  Stop demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Stop(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    // Disable framers
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl &= ~((1 << 0) | (1 << 4));
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxTestFTuPLatch
 *  Test uP latch-up
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TestFTuPLatch(void)
{
    UINT32 i;
    BOOL fgRet;

    if(!_DMX_SelectInputType(DMX_IN_DIAG))
    {
        return FALSE;
    }

    // Wait for a while
    x_thread_delay(100);

    // Check result
    fgRet = TRUE;
    for (i = 0; i < NUM_UP_TEST_RESULT_WORDS; i++)
    {
        if (PID_W(i) != _au4TestResult[i])
        {
            LOG(5, "uP latch test failed at word %u: expect = 0x%08x,"
                " read = 0x%08x\n", i, _au4TestResult[i], PID_W(i));
            fgRet = FALSE;
            break;
        }
    }

    // Restore normal uCode
    if(!_DMX_Init())
    {
        return FALSE;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_Init
 *  Initialize demux - can run multiple times
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Init(void)
{
    _eDmxInputType = DMX_IN_NONE;

    _u1DmxDTVFramer = 0xFF;

    // Select clock and pin mux
    _DmxCkgenInit(TRUE);

    _DMX_SetErrLevel(DMX_ERR_RESET);

    if(!_DMX_SelectInputType(DMX_IN_BROADCAST_TS))
    {
        return FALSE;
    }

    if(_eDmxInputType == DMX_IN_BROADCAST_TS)
    {
#if defined (CC_DMX_TS_EXTERNAL_SERIAL)
        if(!_DMX_SetFrontEnd(DMX_FE_EXT_SERIAL))
        {
            return FALSE;
        }
#elif defined (CC_DMX_TS_EXTERNAL_PARALLEL)
        if(!_DMX_SetFrontEnd(DMX_FE_EXT_PARALLEL))
        {
            return FALSE;
        }
#else
        if(!_DMX_SetFrontEnd(DMX_FE_ATSC_DEMOD))
        {
            return FALSE;
        }
#endif  // CC_DMX_TS_EXTERNAL_SERIAL
    }

    if(!_DMX_SetVideoType(DMX_VIDEO_MPEG))
    {
        return FALSE;
    }

    // Initialize handler if integrated with VLD
    if (!_DMX_InitHandler())
    {
        return FALSE;
    }

    if(!_DMX_InitPSIHandler())
    {
        return FALSE;
    }

#ifdef ENABLE_MULTIMEDIA
#ifdef CC_DMX_AUDIO_PREBUF
    if(!_DMX_InitAudHandler())
    {
        return FALSE;
    }
#endif  // CC_DMX_AUDIO_PREBUF

    if(!_DMX_PVRPlay_Init())
    {
        return FALSE;
    }
#endif  // ENABLE_MULTIMEDIA

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SelectInputType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SelectInputType(DMX_INPUT_TYPE_T eInputType)
{
    _DMX_InitMemoryTracker();

    switch(eInputType)
    {
    case DMX_IN_DIAG:
        if(!_DmxInitDiag())
        {
            return FALSE;
        }
        break;

#ifdef __MODEL_slt__
    case DMX_IN_JCHIP:
        if(!_DmxInitJChip())
        {
            return FALSE;
        }
        break;
#endif  // __MODEL_slt__

    case DMX_IN_BROADCAST_TS:
        if(!_DmxInitBroadcastTS())
        {
            return FALSE;
        }
        _DMX_SetInputMode(DMX_HALF_PUSH);
        break;

#ifdef ENABLE_MULTIMEDIA
    case DMX_IN_PLAYBACK_TS:
        if(!_DmxInitPlaybackTS())
        {
            return FALSE;
        }
        _DMX_SetInputMode(DMX_HALF_PUSH);
        _DMX_DDI_SetPort(DMX_DDI_PORT_FRAMER);
        break;

    case DMX_IN_PLAYBACK_PS:
        if(!_DmxInitPS())
        {
            return FALSE;
        }
        _DMX_SetInputMode(DMX_HALF_PUSH);
        _DMX_DDI_SetPort(DMX_DDI_PORT_NO_FRAMER);
        break;

    case DMX_IN_PLAYBACK_MM:
        if(!_DmxInitMM())
        {
            return FALSE;
        }
        _DMX_SetInputMode(DMX_HALF_PUSH);
        _DMX_DDI_SetPort(DMX_DDI_PORT_NO_FRAMER);

    case DMX_IN_PLAYBACK_ES:
        if(!_DmxInitES())
        {
            return FALSE;
        }
        _DMX_SetInputMode(DMX_HALF_PUSH);
        _DMX_DDI_SetPort(DMX_DDI_PORT_NO_FRAMER);
        break;
#endif  // ENABLE_MULTIMEDIA

    default:
        LOG(1, "Unknown type!\n");
        return FALSE;
    }

    _DMX_Lock();
    _eDmxInputType = eInputType;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetInputType
 */
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T _DMX_GetInputType(VOID)
{
    DMX_INPUT_TYPE_T eInputType;

    _DMX_Lock();
    eInputType = _eDmxInputType;
    _DMX_Unlock();

    return eInputType;
}


//-----------------------------------------------------------------------------
/** _DMX_SetInputMode
 */
//-----------------------------------------------------------------------------
void _DMX_SetInputMode(DMX_INPUT_MODE_T rMode)
{
    UINT32 u4Reg;

    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    _DMX_Unlock();

    u4Reg &= 0x3fffffff;

    switch(rMode)
    {
    case DMX_PUSH:
        break;

    case DMX_HALF_PUSH:
        u4Reg |= (1U << 30);
        break;

    case DMX_PULL:
        u4Reg |= (3U << 30);
        break;

    default:
        ASSERT(0);
        break;
    }

    _DMX_Lock();
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    // Enable "DBM input full" signal to TS input device
    DMXCMD_WRITE32(DMX_REG_DBM_BUF_CTRL, 0);
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_SetIgnorePESLen
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetIgnorePESLen(BOOL fgEnable)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFF00);
    if(fgEnable)
    {
        CT_SETTING = u4Ctrl | 0x100;
    }
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_EnablePower
 */
//-----------------------------------------------------------------------------
VOID _DMX_EnablePower(BOOL fgEnable)
{
    _DmxCkgenInit(fgEnable);
}


//-----------------------------------------------------------------------------
/** _DMX_SetScrambleScheme
 *  fgOrg - false means use PES header to decide scramble state
 *          true means use scramble flag to decide scramble state
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetScrambleScheme(BOOL fgOrg)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFF0000);
    if(fgOrg)
    {
        CT_SETTING = (u4Ctrl | 0x10000);
    }
    _DMX_Unlock();

#ifdef TIME_SHIFT_SUPPORT
    FVR_SetScrambleScheme(fgOrg);
#endif // TIME_SHIFT_SUPPORT
}


//-----------------------------------------------------------------------------
/** _DMX_Version
 *  Show DMX version information
 */
//-----------------------------------------------------------------------------
VOID _DMX_Version(void)
{
    UINT32 u4Ctrl;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    LOG(1, "----------------------------------------\n");
    LOG(1, "DMX information\n");
    LOG(1, "----------------------------------------\n");

    u2CodeVersion = (UINT16)(_u4uCodeVersion & 0xffff);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0xf);
    u1Minor = (UINT8)(u2CodeVersion & 0xff);
    LOG(1, "Demux uCode version: %u.%u\n", u1Major, u1Minor);

    switch(_eDmxInputType)
    {
    case DMX_IN_NONE:
        LOG(1, "Inputtype: DMX_IN_NONE\n");
        break;
    case DMX_IN_DIAG:
        LOG(1, "Inputtype: DMX_IN_DIAG\n");
        break;
    case DMX_IN_BROADCAST_TS:
        LOG(1, "Inputtype: DMX_IN_BROADCAST_TS\n");
        break;
    case DMX_IN_PLAYBACK_TS:
        LOG(1, "Inputtype: DMX_IN_PLAYBACK_TS\n");
        break;
    case DMX_IN_PLAYBACK_PS:
        LOG(1, "Inputtype: DMX_IN_PLAYBACK_PS\n");
        break;
    case DMX_IN_PLAYBACK_MM:
        LOG(1, "Inputtype: DMX_IN_PLAYBACK_MM\n");
        break;
    case DMX_IN_PLAYBACK_ES:
        LOG(1, "Inputtype: DMX_IN_PLAYBACK_ES\n");
        break;
    default:
        LOG(1, "Inputtype: Unknown\n");
        break;
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    LOG(1, "Framer0: %s, %s, %s, %s\n",
            (u4Ctrl & 0x1) ? "Enable" : "Disable",
            (u4Ctrl & 0x2) ? "Parallel" : "Serial",
            (u4Ctrl & 0x4) ? "External sync" : "Internal sync",
            (u4Ctrl & 0x8) ? "Positive" : "Negative");

    LOG(1, "----------------------------------------\n");
}


//-----------------------------------------------------------------------------
/** _DMX_uCodeHasAW
 *  Return TRUE if the TS uCode supports AW.  Otherwise, return FALSE.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_uCodeHasAW(void)
{
#ifdef CC_DMX_USE_CT
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    u2CodeVersion = (UINT16)(FTI_IMEM[0] & 0xFFFF);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0x0F);
    u1Minor = (UINT8)(u2CodeVersion & 0xFF);

    // On 5387/5388/5363, CT can be configured for TS uCode 3.24 and above.
    // Although 3.21 does support CT configuration, it does not support the
    // case where (CT_SETTING == 2).  Thus, 3.24 is a better option.
    // The 5388 uCode has been renamed to 4.25 since 3.25.
    if (((u1Major == 3) && (u1Minor == 24)) ||
        ((u1Major == 4) && (u1Minor >= 25)))
    {
        UINT8 u1uCodeAwMode;

        _DMX_Lock();
        u1uCodeAwMode = (UINT8)((CT_SETTING) & 0xFF);
        _DMX_Unlock();

        if ((u1uCodeAwMode == 1) || (u1uCodeAwMode == 2))
        {
            return TRUE;
        }
    }
#endif  // CC_DMX_USE_CT

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_IsMicroProcessorStopped
 *  Return TRUE if the micro-processor is stopped according to the global
 *  variable _fgMicroProcessorStopped.  Otherwise, return FALSE.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_IsMicroProcessorStopped(void)
{
    BOOL fgMicroProcessorStopped;

    _DMX_Lock();
    fgMicroProcessorStopped = _fgMicroProcessorStopped;
    _DMX_Unlock();

    return fgMicroProcessorStopped;
}


