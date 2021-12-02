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
 *
 *---------------------------------------------------------------------------*/

#ifndef X_HAL_ARM_H
#define X_HAL_ARM_H

#include "x_typedef.h"
#include "x_lint.h"

//-----------------------------------------------------------------------------
/** Specify DMA data direction
 *  used in HalDmaBufferStart() / HalDmaBufferDone()
 */
//-----------------------------------------------------------------------------
enum HalDmaDirection {
    HAL_DMA_TO_DEVICE,
    HAL_DMA_FROM_DEVICE,
    HAL_DMA_BIDIRECTION,
    HAL_DMA_NONE,
};

//===========================================================================
// IRQ/FIQ related functions

externC void HalEnableIRQ(void);
externC void HalDisableIRQ(void);
externC void HalEnableFIQ(void);
externC void HalDisableFIQ(void);

#ifndef __KERNEL__
externC UINT32 HalCriticalStart(void);
externC UINT32 HalCriticalSemiStart(void);
externC void HalCriticalEnd(UINT32 u4Flags);
#endif

//===========================================================================
// Cache related functions

externC BOOL HalIsICacheEnabled(void);
externC BOOL HalIsDCacheEnabled(void);
externC UINT32 HalGetICacheSize(void);
externC UINT32 HalGetDCacheSize(void);
externC UINT32 HalGetICacheLineSize(void);
externC UINT32 HalGetDCacheLineSize(void);
externC void HalEnableICache(void);
externC void HalDisableICache(void);
externC void HalEnableDCache(void);
externC void HalDisableDCache(void);
externC void HalEnableCaches(void);
externC void HalDisableCaches(void);
externC void HalInvalidateAllCaches(void);
externC void HalInvalidateICache(void);
externC void HalInvalidateICacheSingleLine(UINT32 u4Addr);
externC void HalInvalidateDCache(void);
externC void HalInvalidateDCacheSingleLine(UINT32 u4Addr);
externC void HalFlushDCacheSingleLine(UINT32 u4Addr);
externC void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr);
externC void HalInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
externC void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
externC void HalFlushInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
externC void HalFlushDCache(void);
externC void HalFlushInvalidateDCache(void);
externC void HalFlushWriteBuffer(void);
externC void HalInstructionMemoryBarrier(void);
externC void HalIOMMUInvalidateDCacheMultipleLine(UINT32 u4VirAddr, UINT32 u4PhyAddr, UINT32 u4Size);
externC void HalIOMMUFlushDCacheMultipleLine(UINT32 u4VirAddr, UINT32 u4PhyAddr, UINT32 u4Size);
externC void HalIOMMUFlushInvalidateDCacheMultipleLine(UINT32 u4VirAddr, UINT32 u4PhyAddr, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Called when DMA on a buffer is about to start.
 * 
 *  This function will do proper cache operation.
 *  CPU should not access DMA buffer after this function call.
 * 
 *  @param pBuffer:  Virtual address of the buffer
 *  @param size: Size of the buffer.
 *  @param direction: the direction of DMA
 */
//-----------------------------------------------------------------------------
#ifndef __GNUC__
externC void HalDmaBufferStart(void *pBuffer, UINT32 size, enum HalDmaDirection direction);
#else
static inline void HalDmaBufferStart(void *pBuffer, UINT32 size, enum HalDmaDirection direction)
{
    switch (direction)
    {
        case HAL_DMA_TO_DEVICE:
            HalFlushDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        case HAL_DMA_FROM_DEVICE:
            HalInvalidateDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        case HAL_DMA_BIDIRECTION:
            HalFlushInvalidateDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        default:
            break;
    }
}

static inline void HalIOMMUDmaBufferStart(void *pVirBuffer, void * pPhyBuffer, UINT32 size, enum HalDmaDirection direction)
{
    switch (direction)
    {
        case HAL_DMA_TO_DEVICE:
            HalIOMMUFlushDCacheMultipleLine((UINT32)pVirBuffer, (UINT32)pPhyBuffer, size);
            break;

        case HAL_DMA_FROM_DEVICE:
            HalIOMMUInvalidateDCacheMultipleLine((UINT32)pVirBuffer, (UINT32)pPhyBuffer, size);
            break;

        case HAL_DMA_BIDIRECTION:
            HalIOMMUFlushInvalidateDCacheMultipleLine((UINT32)pVirBuffer, (UINT32)pPhyBuffer, size);
            break;

        default:
            break;
    }
}
#endif

//-----------------------------------------------------------------------------
/**  Called when DMA on a buffer is done.
 * 
 *  Device DMA should not access DMA buffer after this function call.
 * 
 *  NOTE:
 *     The parameter should match that of BSP_DmaBufferStart
 *  
 *  @param pBuffer: Virtual address of the buffer
 *  @param size: Size of the buffer.
 *  @param direction: the direction of DMA
 */
//-----------------------------------------------------------------------------
#ifndef __GNUC__
externC void HalDmaBufferDone(void *pBuffer, UINT32 size, enum HalDmaDirection direction);
#else
static inline void HalDmaBufferDone(void *pBuffer, UINT32 size, enum HalDmaDirection direction)
{
    switch (direction)
    {
        case HAL_DMA_FROM_DEVICE:
        case HAL_DMA_BIDIRECTION:
            HalInvalidateDCacheMultipleLine((UINT32)pBuffer, size);
            break;

        case HAL_DMA_TO_DEVICE:
        default:
            break;
    }
}

static inline void HalIOMMUDmaBufferDone(void *pVirBuffer, void *pPhyBuffer, UINT32 size, enum HalDmaDirection direction)
{
    switch (direction)
    {
        case HAL_DMA_FROM_DEVICE:
        case HAL_DMA_BIDIRECTION:
            HalIOMMUInvalidateDCacheMultipleLine((UINT32)pVirBuffer, (UINT32) pPhyBuffer, size);
            break;

        case HAL_DMA_TO_DEVICE:
        default:
            break;
    }
}
#endif

//===========================================================================
// MMU related functions

externC BOOL HalIsMMUEnabled(void);
externC void HalEnableMMU(void);
externC void HalDisableMMU(void);
externC void HalInvalidateTLB(void);
externC BOOL HalInitMMU(UINT32 u4Addr);
externC void HalSet1MSectionTable(UINT32 u4PhyAddr, UINT32 u4VirtAdr);
externC void HalSet4KPageTable(UINT32 u4PhyAddr, UINT32 u4VirtAdr, UINT32* pu4PageTableAddress);
externC UINT32 HalGetMMUTableAddress(void);
externC UINT32 HalCpuGetPhyPgd(void);


//===========================================================================
// TCM mode

externC void HalSetInstTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable);
externC void HalSetDataTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable);


//===========================================================================
// Sleep mode

externC void HalWaitForInterrupt(void);

//===========================================================================
// Endian related

UINT32 BYTESWAP32(UINT32 u4Value);

// LINT_SUPPRESS_NEXT_EXPRESSION(129)
// INLINE UINT16 BYTESWAP16(UINT16 u2Value);

// LINT_SUPPRESS_NEXT_EXPRESSION(129)
// INLINE UINT16 BYTESWAP16(UINT16 u2Value)
// {
// 	return (UINT16)((u2Value >> 8) | (u2Value << 8));
// }

// HTONL: Convert an UINT32 from host byte order to network byte order
// NTOHL: Convert an UINT32 from network byte order to host byte order
// HTONS: Convert an UINT16 from host byte order to network byte order
// NTOHS: Convert an UINT16 from network byte order to host byte order

#ifdef __BIG_ENDIAN

#define BIG_ENDIAN
#define HTONL(x)	(x)
#define NTOHL(x)	(x)
#define HTONS(x)	(x)
#define NTOHS(x)	(x)

#else	// __BIG_ENDIAN

#define LITTLE_ENDIAN
#define HTONL(x)	BYTESWAP32((x))
#define NTOHL(x)	BYTESWAP32((x))
#define HTONS(x)	((UINT16)((((UINT16)(x)) >> 8) | (((UINT16)(x)) << 8)))
#define NTOHS(x)	((UINT16)((((UINT16)(x)) >> 8) | (((UINT16)(x)) << 8)))

#endif	// __BIG_ENDIAN


#endif  // X_HAL_ARM_H
