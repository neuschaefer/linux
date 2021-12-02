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
 * $Author: jiangbo.wang $
 * $Date: 2012/07/16 $
 * $RCSfile: fvr_init.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_init.c
 *  FVR driver - FVR
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "fvr_debug.h"

#include "x_assert.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_util.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _afgFVRIsRecording[FVR_GBL_COUNT];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SetFVRTimer
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SetFVRTimer(void)
{
    FVR_GBL_STRUCT_T* prGBL;
    UINT32 u4Clk, u4Compare, u4Div, u4Reg;

    prGBL = _FVR_GetGBLStruct(0);
    if(prGBL == NULL)
    {
        return FALSE;
    }

    if(prGBL->u4TimeToNotify == 0)
    {
        FVR_GBL_ARY_W(0, 9) = 0x0;
        u4Reg = FVR_GBL_ARY_W(0, 10) & 0xFF000000;
        u4Reg |= 0x1;
        FVR_GBL_ARY_W(0, 10) = u4Reg;
        return TRUE;
    }

#ifndef CC_FPGA
    u4Clk = 324000000;
#else
        u4Clk = 27000000;
#endif

    u4Compare = 0xFFFF;
    u4Div = ((u4Clk / u4Compare) * prGBL->u4TimeToNotify) / 1000;
    FVR_GBL_ARY_W(0, 9) = ((u4Div & 0xFFFF) << 16) |
                        (u4Compare & 0xFFFF) ;

    u4Reg = FVR_GBL_ARY_W(0, 10) & 0xFF000000;
    u4Reg |= 0x1;
    FVR_GBL_ARY_W(0, 10) = u4Reg;

    LOG(7, "Time dividor: 0x%x, compare val: 0x%x\n", u4Div, u4Compare);
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _FVR_SoftInit
 *  Soft-initialize demux
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _FVR_SoftInit(void)
{
    UINT32 u4Ctrl, i;
    FVR_GBL_STRUCT_T* prGblPidStruct;

    _FVR_Lock();

    // Clear global 0 table and struct
    x_memset((void*)&(FVR_GBL_ARY_W(0, 0)), 0, FVR_GBL_SIZE * 4);
    prGblPidStruct = _FVR_GetGBLStruct(0);
    x_memset((void*)prGblPidStruct, 0, sizeof(FVR_GBL_STRUCT_T));

    // Clear global 1 table and struct
    x_memset((void*)&(FVR_GBL_ARY_W(1, 0)), 0, FVR_GBL_SIZE * 4);
    prGblPidStruct = _FVR_GetGBLStruct(1);
    x_memset((void*)prGblPidStruct, 0, sizeof(FVR_GBL_STRUCT_T));

    // Clear global 3 table and struct
    x_memset((void*)&(FVR_GBL_ARY_W(3, 0)), 0, FVR_GBL_SIZE * 4);
    prGblPidStruct = _FVR_GetGBLStruct(3);
    x_memset((void*)prGblPidStruct, 0, sizeof(FVR_GBL_STRUCT_T));
    
    // Clear PID index table and DMEM
    for (i = 0; i < DMX_NUM_FVR_HW_PID_COUNT; i++)
    {       
        FVR_PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(FVR_PER_PID_S(i)), 0, FVR_PID_SIZE * 4);
    }

    // Clear Per-Pid struct
    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_STRUCT_T* prPidStruct;
        UINT8 u1SerialNum;
        
        prPidStruct = _FVR_GetPidStruct(i);
        u1SerialNum = prPidStruct->u1SerialNumber;
        x_memset((void*)prPidStruct, 0, sizeof (FVR_PID_STRUCT_T));
        prPidStruct->u1SerialNumber = u1SerialNum;
    }

    _FVR_Unlock();    

    x_memset((void*)FVR_KEY_BASE, 0, (FVR_KEY_COUNT * 96));

    // Initialize common routines
    _FVR_InitAPI();

    _FVR_Lock();

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF;
    u4Ctrl |= (FVR_PER_PID_OFFSET) << 16;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);
    
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFD00FEFF;
    u4Ctrl |= (1 << 25);                    // Disable record path    
    u4Ctrl |= ((FVR_PID_SIZE * 4) << 16); 
    u4Ctrl |= (1 << 8);                     // Direct map
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Disable insert timestamp
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);

    FVR_GBL_ARY_W(0, 2) = 0x100;    // Disable record region 0
    FVR_GBL_ARY_W(1, 2) = 0x100;    // Disable record region 1
    FVR_GBL_ARY_W(3, 2) = 0x100;    // Disable record region 1

    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Init(void)
{
    UINT8 i;

    if(!_FVR_InitIsr())
    {
        return FALSE;
    }
    
    // Soft init
    if (!_FVR_SoftInit())
    {
        return FALSE;
    }

    if(!_DMX_GetIsPanicReset())
    {
        if(!_FVR_SwapInit())
        {
            return FALSE;
        }
    }
    
    _FVR_Lock();
    for(i=0; i<FVR_GBL_COUNT; i++)
    {
        _afgFVRIsRecording[i] = FALSE;
    }
    _FVR_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Start(UINT8 u1Idx)
{
    UINT32 u4Ctrl;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    if(_FVR_IsRecording(u1Idx))
    {
        return TRUE;
    }

    _FVR_SetIsToggleKey(u1Idx);
    
    // Enable DMEM global control
    FVR_GBL_ARY_W(u1Idx, 2) = 0x0;
    
    if(!_afgFVRIsRecording[0] && !_afgFVRIsRecording[1] && !_afgFVRIsRecording[3])
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFDFFFEFF;   
        u4Ctrl |= (1 << 8);     // Direct map
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);
        
        if(!_SetFVRTimer())
        {
            return FALSE;
        }
    }

    _afgFVRIsRecording[u1Idx] = TRUE;

    LOG(3, "FVR start, %d\n", u1Idx);
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_Stop(UINT8 u1Idx)
{
    UINT32 u4Ctrl;
    
    LOG(5, "%s\n", __FUNCTION__);

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    // Disable DMEM global control
    _FVR_Lock();
    FVR_GBL_ARY_W(u1Idx, 2) = 0x100;    // Disable record region 0
    _FVR_Unlock();

    _afgFVRIsRecording[u1Idx] = FALSE;

    if(!_afgFVRIsRecording[0] && !_afgFVRIsRecording[1] && !_afgFVRIsRecording[3])
    {    
        _FVR_Lock();   
        FVR_GBL_ARY_W(0, 9) = 0x0;                     // Disable timer interrupt 
        u4Ctrl = FVR_GBL_ARY_W(0, 10) & 0xFF000000;
        u4Ctrl |= 0x1;
        FVR_GBL_ARY_W(0, 10) = u4Ctrl;

        u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFDFFFFFF;
        u4Ctrl |= (1 << 25);
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);    
        _FVR_Unlock();
    }

    // Avoid race condition, but may call in HISR
    //x_thread_delay(5);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_IsRecording
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_IsRecording(UINT8 u1Idx)
{
    BOOL fgIsRecord;

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }

    _FVR_Lock();
    fgIsRecord = _afgFVRIsRecording[u1Idx];
    _FVR_Unlock();
    
    return fgIsRecord;
}


//-----------------------------------------------------------------------------
/** _FVR_GetResidual
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _FVR_GetResidual(UINT8 u1Idx, UINT32 *pu4Addr, UINT32 *pu4Size, UINT32 *pu4Index)
{
    FVR_GBL_STRUCT_T *prGBL;
    UINT32 u4Wp, u4PktIndex, u4PktRp, u4BufSize, u4DataLen;

    if((pu4Addr == NULL) || (pu4Size == NULL) || (pu4Index == NULL))
    {
        return FALSE;
    }

    if(u1Idx >= FVR_GBL_COUNT)
    {
        return FALSE;
    }
    
    if(_afgFVRIsRecording[u1Idx])
    {
        LOG(1, "Still recording\n");
        return FALSE;
    }

    prGBL = _FVR_GetGBLStruct(u1Idx);

    _FVR_Lock();
    u4Wp = VIRTUAL(FVR_GBL_ARY_W(u1Idx, 3));              // write pointer
    u4PktIndex = FVR_GBL_ARY_W(u1Idx, 6);
    u4PktRp = VIRTUAL(prGBL->u4PktRp);
    u4BufSize = prGBL->u4BufSize;
    _FVR_Unlock();

    u4DataLen = DATASIZE(u4PktRp, u4Wp, u4BufSize);    
       
    *pu4Addr = VIRTUAL(u4Wp);
    *pu4Size = u4DataLen;
    *pu4Index = u4PktIndex;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _FVR_SetScrambleScheme
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_SetScrambleScheme(BOOL fgOrg)
{
    UINT32 u4Reg;

    _FVR_Lock();
    u4Reg = FVR_GBL_ARY_W(0, 10) & 0xFFFFFF;
    if(fgOrg)
    {
        u4Reg |= 0x1000000;
        FVR_GBL_ARY_W(0, 10) = u4Reg;
    }
    _FVR_Unlock();
}

