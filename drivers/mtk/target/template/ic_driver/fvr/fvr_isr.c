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
 * $RCSfile: fvr_isr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_isr.c
 *  Demux driver - FVR Interrupt Handler
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr.h"
#include "fvr_debug.h"

#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_926.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define VECTOR_FVR              18

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
EXTERN BOOL _DMX_IDE_SendPacket(UINT8 u1Idx, UINT32 u4BufStart,
                UINT32 u4BufEnd, UINT32 u4Rp, UINT32 u4Wp);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVRPacketCountInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRPacketCountInt(UINT8 u1Pidx, UINT32 u4PacketAddr)
{
    FVR_GBL_STRUCT_T *prGBL;
    UINT32 u4BufStart, u4BufEnd, u4PktRp, u4Wp, u4DataLen, u4PktCount;

    ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

    //Printf("_FVRPacketCountInt %d\n", i++);

    prGBL = _FVR_GetGBLStruct();

    u4BufStart = prGBL->u4BufStart;
    u4BufEnd = prGBL->u4BufEnd + 1;
    u4PktRp = prGBL->u4PktRp;
    u4Wp = u4PacketAddr;

    ASSERT((u4PacketAddr >= u4BufStart) && (u4PacketAddr < u4BufEnd));
    prGBL->u4Wp = u4PacketAddr;

    // Calculate length
    u4DataLen = DATASIZE(u4PktRp, u4Wp, (prGBL->u4BufSize));

    u4PktCount = FVR_GBL_PID_W(6);

    if(prGBL->pfnPktCountNotify != NULL)
    {
        prGBL->pfnPktCountNotify(u4PktRp, u4DataLen, u4PktCount);
        prGBL->u4PktRp = u4PacketAddr;        
    }
    else
    {
        if (DMX_IDE_IsEnabled())
        {
            _DMX_IDE_SendPacket(0, u4BufStart, u4BufEnd, prGBL->u4Rp, u4Wp);
            // Update read pointer
            prGBL->u4Rp = u4Wp;
            FVR_GBL_PID_W(4) = PHYSICAL(u4Wp);
        }
    }

}

//-----------------------------------------------------------------------------
static void _FVRTestTimeSwap(UINT32 u4StartAddr)
{
    UINT32 i;
    UINT8 *pu1Tmp;

    pu1Tmp = (UINT8*)(u4StartAddr - 188);
    for(i=0; i<188; i++)
    {
        pu1Tmp[i] = i;
    }    
}

//-----------------------------------------------------------------------------
/** _FVRPacketSwapInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRPacketSwapInt(UINT8 u1Pidx, UINT32 u4StartAddr)
{  
    FVR_PID_STRUCT_T* prPid;
//    UINT32 u4Addr;
        
    prPid = _FVR_GetPidStruct(u1Pidx);
    ASSERT(prPid->fgPktSwap);
#if 1
    _FVRTestTimeSwap(u4StartAddr);
#else
    if(prPid->pfnSwapNotify != NULL)
    {
        u4Addr = u4StartAddr - 188;
        prPid->pfnSwapNotify(u1Pidx, u4Addr);
    }
#endif
}


//-----------------------------------------------------------------------------
/** _FVRPicSearchInt
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRPicSearchInt(UINT8 u1Pidx, UINT8 u1Pattern, UINT32 u4PktCount)
{
    FVR_PID_STRUCT_T* prPid;
    UINT8 u1RetType;
    
    prPid = _FVR_GetPidStruct(u1Pidx);
    ASSERT(prPid->fgPicSearch);

    switch(u1Pattern)
    {
    case 0:
        u1RetType = FVR_PIC_I_TYPE;
        break;
    case 1:
        u1RetType = FVR_PIC_P_TYPE;
        break;
    case 2:
        u1RetType = FVR_PIC_B_TYPE;
        break;
    default:
        u1RetType = FVR_PIC_UNKNOWN;
        break;
    }

    if((u1RetType & prPid->u1PicSearchMask) != 0)
    {    
        if(prPid->pfnPicSearchNotify != NULL)
        {
            prPid->pfnPicSearchNotify(u1Pidx, u1RetType, u4PktCount);
        }
    }
}



//-----------------------------------------------------------------------------
/** _FVRRecDbmErrorInt
 *  Error handler of Rec DBM
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRRecDbmErrorInt(void)
{
    ASSERT(0);      // Should never happen

    // Clear interrupt
    FVR_WRITE32(FVR_REG_REC_DBM_ERROR_STATUS, 1);

    LOG(3, "FVR DBM error!\n");
}


//-----------------------------------------------------------------------------
/** _FVRuPErrorInt
 *  Error handler of uP
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRuPErrorInt(void)
{
	UINT32 u4Status;
	//UINT8 u1Pidx, u1Type;

    // Get interrupt status
	u4Status = FVR_READ32(FVR_REG_uP_NON_ERROR_STATUS);

    // Clear interrupt
    FVR_WRITE32(FVR_REG_uP_ERROR_STATUS, 1);
    
#if 0    
	u1Pidx = (u4Status >> 16) & 0xff;
	u1Type = u4Status & 0xff;

	if (u1Pidx < FVR_NUM_PID_INDEX)
	{
	    FVR_PID_STRUCT_T* prPidStruct;

	    prPidStruct = _FVR_GetPidStruct(u1Pidx);
        if (!fgEnabled)
        {
            // Error interrupt on a disabled pid, caused by race condition
            // between disabling pid and raising interrupt.
            // Simply do nothing here
            LOG(5, "Interrupt on disabled: pidx %u, status 0x%08x\n",
                u1Pidx, u4Status);

            return;
        }

	    prPidStruct->rCounters.u4FTuPErrors++;
	    prPidStruct->rCounters.u4TotalErrors++;
	}
	else
	{
	    LOG(3, "FVR FTuP error: invalid pid index %u\n", u1Pidx);
	}
#endif
    LOG(3, "FVR uP error happen, INT = %x !!!!!! \n", u4Status);

}


//-----------------------------------------------------------------------------
/** _FVRDispatchDBMInterrupt
 *  Dispatch DBM interrupts
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRDispatchDBMInterrupt(void)
{
    ASSERT(0);  // Should never happen
    // Clear interrupt
    FVR_WRITE32(FVR_REG_REC_DBM_NON_ERROR_STATUS, 1);
}


//-----------------------------------------------------------------------------
/** _FVRDispatchuPInterrupt
 *  Dispatch uP interrupts
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRDispatchuPInterrupt(void)
{
    UINT32 u4Status, u4Status2;
    UINT8 u1Pidx, u1Reason, u1Pattern, u1PendInt;
    
    while(1)
    {
        u4Status = FVR_READ32(FVR_REG_uP_NON_ERROR_STATUS);
        u4Status2 = FVR_READ32(FVR_REG_uP_NON_ERROR2_STATUS);

        u1Reason = GET_BYTE(u4Status, 0);
        u1Pattern = GET_BYTE(u4Status, 1);
        u1Pidx = GET_BYTE(u4Status, 2);
        u1PendInt = GET_BYTE(u4Status, 3);

        if(u1PendInt == 0)
        {
            break;
        }

        ASSERT(u1Pidx < FVR_NUM_PID_INDEX);

        HalFlushInvalidateDCache();

        // I,P,B notification
        if(u1Reason & 0x4)
        {
            _FVRPicSearchInt(u1Pidx, u1Pattern, u4Status2);
        }
        
        // Packet swap notification
        if(u1Reason & 0x2)
        {
            _FVRPacketSwapInt(u1Pidx, u4Status2);
        }

        // Packet count notification
        if(u1Reason & 0x1)
        {
            _FVRPacketCountInt(u1Pidx, u4Status2);
        }
        
        // Clear interrupt
        FVR_WRITE32(FVR_REG_uP_NON_ERROR_STATUS, 1);        
    }
    
}


//-----------------------------------------------------------------------------
/** _FVRDispatchInterrupt
 *  Dispatch FVR interrupt
 *
 *  @param  u4Status        Interrupt status word
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _FVRDispatchInterrupt(UINT32 u4Status)
{
    // Check if spurious interrupt
    if (u4Status == 0)
    {
        LOG(3, "Spurious interrupt!\n");
        return;
    }

    // Check if error interrupt
    if (u4Status & FVR_INT_ERR_MASK)
    {
        LOG(7, "Error INT: 0x%08x\n", u4Status);

        if (u4Status & FVR_INT_ERR_REC_DBM)
        {
            _FVRRecDbmErrorInt();
        }

        if (u4Status & FVR_INT_ERR_uP)
        {
            _FVRuPErrorInt();
        }
    }

    // Handle normal cases
    if (u4Status & FVR_INT_STATUS_MASK)
    {
        if (u4Status & FVR_INT_STATUS_REC_DBM)
        {
            _FVRDispatchDBMInterrupt();
        }
    
        if (u4Status & FVR_INT_STATUS_uP)
        {
            _FVRDispatchuPInterrupt();
        }
    }
    

}


//-----------------------------------------------------------------------------
/** _FVRIrqHandler
 *  FVR interrupt handler
 *
 *  @param  u2Vector        The IRQ vector
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _FVRIrqHandler(UINT16 u2Vector)
{
    static BOOL _fgISR = FALSE;

    ASSERT(!_fgISR);
    UNUSED(_fgISR);                 // Avoid compile warning in release build
    _fgISR = TRUE;

    ASSERT(u2Vector == VECTOR_FVR);
    if (!BIM_IsIrqPending(VECTOR_FVR))
    {
        LOG(3, "Spurious demux global interrupt!\n");
    }

    // Check if it's demux interrupt
    while (BIM_IsIrqPending(VECTOR_FVR))
    {
    	UINT32 u4Status, u4Mask;

        u4Status = FVR_READ32(FVR_REG_INTR_STATUS);
        u4Mask = FVR_READ32(FVR_REG_INTR_MASK);
        u4Status &= u4Mask;

        if (u4Status)
        {
            _FVRDispatchInterrupt(u4Status);
        }
        else
        {
            LOG(3, "Spurious demux local interrupt!\n");
        }

        // Clear interrupt
        VERIFY(BIM_ClearIrq(VECTOR_FVR));
    }

    _fgISR = FALSE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _FVR_InitISR
 *  Initialize interrupt handler
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_InitISR(void)
{
    static BOOL _fgInited = FALSE;
    x_os_isr_fct pfnOldIsr;

    // Register ISR
    if (!_fgInited)
    {        
        if (x_reg_isr(VECTOR_FVR, _FVRIrqHandler, &pfnOldIsr) != OSR_OK)
        {
            LOG(1, "Error: fail to register demux ISR!\n");
        }
        UNUSED(pfnOldIsr);

        _fgInited = TRUE;
    }

    // Enable all FVR interrupts
    FVR_WRITE32(FVR_REG_INTR_MASK, 0x50005);

	LOG(7, "Enable FVR interrupt\n");
}


