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
 * Description:
 *         Soft parser interface
 *
 *---------------------------------------------------------------------------*/

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "psr_reg.h"
#include "psr_hw.h"
#include "psr_utils.h"
#include "psr_drvif.h"
#include "psr_debug.h"


#include "x_bim.h"
#include "x_assert.h"
#include "x_os.h"

//#include "aud_if.h"

LINT_EXT_HEADER_END

//#define SW_TRANSFER

//---------------------------------------------------------------------
// Compiling directives


//---------------------------------------------------------------------
// Constant and type definitions

//---------------------------------------------------------------------
// Macros


//---------------------------------------------------------------------
// Static global variables

static BOOL _fgInit = FALSE;
static HANDLE_T _hSemInt, _hSemLock;

//---------------------------------------------------------------------
// Static functions


/*----------------------------------------------------------------------------
 * Function: _Reset
 *
 * Description:
 *      Reset soft parser
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Otherwise
 *---------------------------------------------------------------------------*/
static BOOL _Reset(void)
{
	// Reset parser
	PARSER_WRITE32(REG_PARCTL, 0);

	// Enable parser
	PARSER_WRITE32(REG_PARCTL, PARCTL_RST | PARCTL_PCKE);

	// Reset audio read pointer offset
	PARSER_WRITE32(REG_ARPOFF, 0);

	return TRUE;
}

#ifndef SW_TRANSFER
/*----------------------------------------------------------------------------
 * Function: _WaitPReady
 *
 * Description:
 *      Wait P DMA of soft parser to ready state
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Otherwise
 *---------------------------------------------------------------------------*/ 
static BOOL _WaitPReady(void)
{
	while ((PARSER_READ32(REG_PCMD) & PCMD_RDY) == 0)
	{
	}

	return TRUE;
}

/*----------------------------------------------------------------------------
 * Function: _WaitPOk
 *
 * Description:
 *      Wait P DMA of soft parser to complete transfer
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Otherwise
 *---------------------------------------------------------------------------*/
static BOOL _WaitPOk(void)
{
    while ((PARSER_READ32(REG_PCMD) & PCMD_OK) == 0)
    {
    }
    

    return TRUE;
}


/*----------------------------------------------------------------------------
 * Function: _PollMode
 *
 * Description:
 *      Poll mode handler after issuing P DMA command
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Transfer successful
 *      FALSE: Transfer failed
 *---------------------------------------------------------------------------*/
static BOOL _PollMode(void)
{
    BOOL fgRet;

    // Wait for complete
    fgRet = _WaitPOk();

    // Clear interrupt
    PARSER_WRITE32(REG_INTCLR, INT_PDMA);

    return fgRet;
}

/*----------------------------------------------------------------------------
 * Function: _InterruptMode
 *
 * Description:
 *      Interrupt mode handler after issuing P DMA command
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Transfer successful
 *      FALSE: Transfer failed
 *---------------------------------------------------------------------------*/

static BOOL _InterruptMode(void)
{
    // Wait for completion signal
    VERIFY(x_sema_lock(_hSemInt, X_SEMA_OPTION_WAIT) == OSR_OK);

    return TRUE;
}
#endif

/*----------------------------------------------------------------------------
 * Function: _SoftParserISR
 *
 * Description:
 *      ISR of soft parser
 *
 * Inputs:
 *      u2Vector: The IRQ vector, must be VECTOR_SPSR
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID _SoftParserISR(UINT16 u2Vector)
{
    static BOOL _fgISR = FALSE;

//    const UINT16 u2Mask = IRQ_PSR;

    // ISR can't be nested
    ASSERT(!_fgISR);
    UNUSED(_fgISR);                 // Avoid compile warning in release build
    _fgISR = TRUE;

    // Must be this IRQ
    ASSERT(u2Vector == VECTOR_P);
//    ASSERT(((1 << u2Vector) & u2Mask) != 0);
//    UNUSED(u2Mask);                 // Avoid compile warning in release build

    // Check if interrupt really happened
    if ((PARSER_READ32(REG_INTSTS) & INT_PDMA) != 0)
    {
        // Clear interrupt
        PARSER_WRITE32(REG_INTCLR, INT_PDMA);

        // Signal the completion of soft parser
        VERIFY(x_sema_unlock(_hSemInt) == OSR_OK);
    }

    // Clear Parser interrupt
    VERIFY(BIM_ClearIrq(VECTOR_P));

    _fgISR = FALSE;
}


//---------------------------------------------------------------------
// Exported functions


//---------------------------------------------------------------------
// Interface functions

/*----------------------------------------------------------------------------
 * Function: PSR_SoftInit
 *
 * Description:
 *      Initialize soft parser
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Otherwise
 *---------------------------------------------------------------------------*/
BOOL PSR_SoftInit(void)
{
    x_os_isr_fct pfnOldIsr;

    if (_fgInit)
    {
        // Already inited
        return TRUE;
    }

    // Reset soft parser
    VERIFY(_Reset());

    // Initialize a binary semaphore for interrupt signalling
    if (x_sema_create(&_hSemInt, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        return FALSE;
    }

    // Initialize a mutex semaphore for re-entrant protection
    if (x_sema_create(&_hSemLock, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        return FALSE;
    }

    // Register soft parser ISR
    if (x_reg_isr(VECTOR_P, _SoftParserISR, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }

    // Enable parser P DMA interrupt
    PARSER_REG32(REG_INTEN) |= INT_PDMA;

    // Enable UART interrupt
    VERIFY(BIM_EnableIrq(VECTOR_P));

    _fgInit = TRUE;

    return TRUE;
}


/*----------------------------------------------------------------------------
 * Function: PSR_SoftReset
 *
 * Description:
 *      Reset soft parser
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Successful
 *      FALSE: Otherwise
 *---------------------------------------------------------------------------*/
BOOL PSR_SoftReset(void)
{
    BOOL fgRet;

    fgRet = _Reset();

    return fgRet;
}

/*----------------------------------------------------------------------------
 * Function: PSR_SoftTransfer
 *
 * Description:
 *      Trigger a soft parser transfer
 *
 * Inputs:
 *      eStreamType: Stream type
 *      u1DeviceId: Device ID
 *      u4SrcAddr: Source address of data to be transferred
 *      u4Size: Data size
 *      u4DstAddr: Destination address
 *      u4DstFifoStart: Start address of destination ring FIFO
 *      u4DstFifoEnd: End address of destination ring FIFO
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: Data have been transferred to destication ring buffer
 *            successfully
 *      FALSE: Otherwise
 *---------------------------------------------------------------------------*/
BOOL PSR_SoftTransfer(PSR_STREAM_T eStreamType, UINT8 u1DeviceId,
    UINT32 u4SrcAddr, UINT32 u4Size,
    UINT32 u4DstAddr, UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd, BOOL fgPollMode)
{
#ifdef SW_TRANSFER
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;

    UNUSED(eStreamType);
    UNUSED(fgPollMode);

    // Check if destination address is within FIFO range
    if ((u4DstAddr < u4DstFifoStart) || (u4DstAddr >= u4DstFifoEnd))
    {
        return FALSE;
    }

    u4WritePtr = ((u4DstAddr + u4Size) >= u4DstFifoEnd) ?
                  ((u4DstAddr + u4Size) - (u4DstFifoEnd-u4DstFifoStart)) :
                  (u4DstAddr + u4Size);

    while (1)
    {
        u4ReadPtr = PSR_SoftGetAudioRp();
        if (u4WritePtr < u4DstAddr)
        {
            //  u4WritePtr  u4DesAddr
            if ((u4ReadPtr > u4WritePtr) && (u4ReadPtr <= u4DstAddr))
            {
                break;
            }
        }
        else
        {
            //  u4DesAddr   u4WritePtr
            if ((u4ReadPtr > u4WritePtr) || (u4ReadPtr <= u4DstAddr))
            {
                break;
            }
        }
        x_thread_delay(1);
    }


    if (u4WritePtr < u4DstAddr)
    {
        x_memcpy((VOID*)(VIRTUAL(u4DstAddr)),
                    (VOID*)(VIRTUAL(u4SrcAddr)),
                    (u4DstFifoEnd-u4DstAddr));
        x_memcpy((VOID*)(VIRTUAL(u4DstFifoStart)),
                    (VOID*)(VIRTUAL(u4SrcAddr+(u4DstFifoEnd-u4DstAddr))),
                    (u4Size-(u4DstFifoEnd-u4DstAddr)));
    }
    else
    {
        x_memcpy((VOID*)(VIRTUAL(u4DstAddr)),
                (VOID*)(VIRTUAL(u4SrcAddr)),
                u4Size);
    }
                  
    PSR_RiscSetAudioWp(u1DeviceId, u4WritePtr);
    return TRUE;    
#else
    UINT32 u4RegFifoStart, u4RegFifoEnd, u4Command;
    UINT8 u1MaxDeviceId;
    BITTYPE_T bt;
    BOOL fgRet;

    // Check init state
    if (!_fgInit)
    {
        return FALSE;
    }


    // Check if destination address is within FIFO range
    if ((u4DstAddr < u4DstFifoStart) || (u4DstAddr >= u4DstFifoEnd))
    {
        return FALSE;
    }

    // Software parser transfer has byte alignment constrain, but audio buffer have alignment
    // constrain
    if (eStreamType == PSR_STREAM_AUDIO)
    {
        // Check if FIFO start/end addresses are valid aligned
        if (!_PSR_IsAligned(u4DstFifoStart, PSR_SOFT_FIFO_ALIGNMENT) ||
            !_PSR_IsAligned(u4DstFifoEnd, PSR_SOFT_FIFO_ALIGNMENT))
        {
            return FALSE;
        }
    }
    // Check if transfer size is too large
    if ((u4Size == 0) || (u4Size > PSR_MAX_PDMA_SIZE))
    {
        return FALSE;
    }

    // Determine parameters according to bitstream type
    switch (eStreamType)
    {

    case PSR_STREAM_AUDIO:
        bt = BTYPE_AUDIO;
        u1MaxDeviceId = MAX_DSP_NUM;
        u4RegFifoStart = (u1DeviceId == 0) ? REG_A0STA: REG_A0STA;
        u4RegFifoEnd = (u1DeviceId == 0) ? REG_A0END : REG_A0END;
        break;

    default:
        bt = BTYPE_GENERAL;
        u1MaxDeviceId = 1;
        // FIXME ; To check
        // u4RegFifoStart = REG_RINGSTA;
        // u4RegFifoEnd = REG_RINGEND;
        u4RegFifoStart = 0;
        u4RegFifoEnd = 0;
        break;
    }

    // Check device ID
    if (u1DeviceId >= u1MaxDeviceId)
    {
        return FALSE;
    }

    // Enter critical section
    VERIFY(x_sema_lock(_hSemLock, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Wait for P DMA ready, fixme
    VERIFY(_WaitPReady());

    // Clear interrupt
    PARSER_WRITE32(REG_INTCLR, INT_PDMA);

    // Setup source address and length
    PARSER_WRITE32(REG_PADDR, u4SrcAddr);
    PARSER_WRITE32(REG_PLENG, u4Size);

    // Setup destination buffer range
    PARSER_WRITE32(u4RegFifoStart, u4DstFifoStart >> 8);
    PARSER_WRITE32(u4RegFifoEnd, u4DstFifoEnd >> 8);

    // Set bitstream type
    PARSER_WRITE32(REG_BITTYPE, (UINT32)bt);

    // Select read and write pointer to soft parser
    switch (bt)
    {

    case BTYPE_AUDIO:
        // Setup destination address
        PARSER_WRITE32(REG_AWP, u4DstAddr);
        break;

    default:
        // Setup destination address
        PARSER_WRITE32(REG_PWP, u4DstAddr);
        break;
    }

    // Setup audio bitstream level
    PARSER_WRITE32(REG_ARPOFF, 0x100);

    // Setup command
    u4Command = PCMD_GO;

    // FIXME ; To check
    /*
    if (bt == BTYPE_GENERAL)
    {
        u4Command |= PCMD_RING;
    }
    */
    if (fgPollMode)
    {
        // Disable interrupt
        PARSER_REG32(REG_INTEN) &= ~INT_PDMA;
    }
    else
    {
        // Make sure the interrupt semaphore is in lock state
        VERIFY(x_sema_lock(_hSemInt, X_SEMA_OPTION_NOWAIT) == OSR_WOULD_BLOCK);

        // Enable interrupt
        PARSER_REG32(REG_INTEN) |= INT_PDMA;
    }

    // Trigger P transfer
    PARSER_WRITE32(REG_PCMD, u4Command);

    // Wait for transfer result
    if (fgPollMode)
    {
        fgRet = _PollMode();
    }
    else
    {
        fgRet = _InterruptMode();
    }

    // Close software parser to let decoder get correct write pointer
    switch (bt)
    {
    case BTYPE_VIDEO:
        break;

    case BTYPE_AUDIO:
    	{
    	UINT32 u4WritePtr;
    	UINT32 u4FifoSZ;
    	u4FifoSZ = u4DstFifoEnd - u4DstFifoStart;

        //audio wp can't be afifo end addr
        u4WritePtr = ((u4DstAddr + u4Size) >= u4DstFifoEnd) ?
        	          ((u4DstAddr + u4Size) - u4FifoSZ) :
        	          (u4DstAddr + u4Size);

        VERIFY(PSR_RiscSetAudioWp(u1DeviceId, u4WritePtr));

    	}
        break;

    default:
        break;
    }

    // Exit critical section
    VERIFY(x_sema_unlock(_hSemLock) == OSR_OK);

    return fgRet;
#endif    
}

/*----------------------------------------------------------------------------
 * Function: PSR_SoftSetAudioFIFO
 *
 * Description:
 *      setup audio bitstream related input buffer
 *
 * Inputs:
 *      u1DeviceId: 0: first decoder,1:second decoder
 *      u4DstFifoStart: Start address of destination ring FIFO
 *      u4DstFifoEnd: End address of destination ring FIFO
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: setup successfully
 *      FALSE:
 *---------------------------------------------------------------------------*/
BOOL PSR_SoftSetAudioFIFO(UINT8 u1DeviceId, UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd)
{
    switch (u1DeviceId)
    {
        case 0:
        case 1:
        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        case 2:
        case 3:
        #endif
            // Setup audio write pointer
            PARSER_WRITE32(REG_AWP, u4DstFifoStart);
            PARSER_WRITE32(REG_A0STA,u4DstFifoStart>>8);
            PARSER_WRITE32(REG_A0END,u4DstFifoEnd>>8);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

UINT32 PSR_SoftGetAudioWp(void)
{
    return PARSER_READ32(REG_AWP);
}

UINT32 PSR_SoftGetAudioRp(void)
{
    return PARSER_READ32(REG_ARP);
}

BOOL PSR_RiscSetAudioWp(UINT8 u1DeviceId, UINT32 u4WritePointer)
{
    UNUSED(u1DeviceId);
    UNUSED(u4WritePointer);

    PARSER_WRITE32(REG_AWP, u4WritePointer); //Audio Description

/*
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;

    u4FifoStart = u4GetAFIFOStart(u1DeviceId);
    u4FifoEnd = u4GetAFIFOEnd(u1DeviceId);

    // Check if u4WritePointer address is within FIFO range
    if ((u4WritePointer < u4FifoStart) || (u4WritePointer > u4FifoEnd))
    {
        return FALSE;
    }

    LOG(9, "Dec(%d) Soft parser set wp = 0x%08x\n", u1DeviceId, u4WritePointer);

    switch(u1DeviceId)
    {
        case 0:
            // Setup audio write pointer
            PARSER_WRITE32(REG_AWPRISC0, u4WritePointer);
            // Select write pointer to risc
            PSR_SET_AWPSEL(0, WPSEL_RISC);
            break;
        case 1:
        	// Setup audio write pointer
            PARSER_WRITE32(REG_AWPRISC1, u4WritePointer);
        	// Select write pointer to risc
            PSR_SET_AWPSEL(1, WPSEL_RISC);
            break;
        default:
            return FALSE;
    }
*/
    return TRUE;
}

BOOL PSR_RiscSetAudFIFO(UINT8 u1DeviceId, UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd)
{
    switch(u1DeviceId)
    {
        case 0:
            // Setup audio write pointer
            PARSER_WRITE32(REG_AWP, u4DstFifoStart);
            PARSER_WRITE32(REG_A0STA,u4DstFifoStart>>8);
            PARSER_WRITE32(REG_A0END,u4DstFifoEnd>>8);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

#define PHYSICAL_512MB(u4Addr)  (((u4Addr & 0x20000000) > 0) ? \
                                PHYSICAL(u4Addr) : (u4Addr & 0x1fffffff))

BOOL PSR_DmaTransfer(UINT32 u4SrcAddr, UINT32 u4Size, UINT32 u4DstAddr)
{
    UINT32 u4SrcAddress;
    UINT32 u4DstAddress;

    u4SrcAddress = PHYSICAL_512MB(u4SrcAddr);
    u4DstAddress = PHYSICAL_512MB(u4DstAddr);
 
    return PSR_SoftTransfer(PSR_STREAM_UNKNOWN, 0, u4SrcAddress, u4Size,
    u4DstAddress, u4DstAddress, u4DstAddress + u4Size, FALSE);
}

