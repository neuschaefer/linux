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

/*
 * DMA implementation for high-speed controllers.
 * $Revision: #1 $
 */

#include "mu_impl.h"

#include "mu_hsdcf.h"

#include "mu_diag.h"
#include "x_hal_arm.h"
/* NASTY: */
#include "mu_hdrdf.h"
#include "mu_pippr.h"

/****************************** CONSTANTS ********************************/

#ifndef MUSB_QMU
#define MGC_O_HSDMA_BASE    0x200
#define MGC_O_HSDMA_INTR    0x200
#endif
#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
#define MGC_O_HSDMA_INTR_MASK    0x201   
#define MGC_O_HSDMA_INTR_CLEAR   0x202
#define MGC_O_HSDMA_INTR_SET     0x203
#endif

#define MGC_O_HSDMA_CONTROL 4
#define MGC_O_HSDMA_ADDRESS 8
#define MGC_O_HSDMA_COUNT   0xc

#define MGC_HSDMA_CHANNEL_OFFSET(_bChannel, _bOffset) ((MGC_O_HSDMA_BASE + (_bChannel << 4) + _bOffset))

/* control register (16-bit): */
#define MGC_S_HSDMA_ENABLE 0
#define MGC_S_HSDMA_TRANSMIT   1
#define MGC_S_HSDMA_MODE1   2
#define MGC_S_HSDMA_IRQENABLE  3
#define MGC_S_HSDMA_ENDPOINT 4
#define MGC_S_HSDMA_BUSERROR 8
#define MGC_S_HSDMA_BURSTMODE 9
#define MGC_M_HSDMA_BURSTMODE    (3 << MGC_S_HSDMA_BURSTMODE)
#define MGC_HSDMA_BURSTMODE_UNSPEC  0
#define MGC_HSDMA_BURSTMODE_INCR4   1
#define MGC_HSDMA_BURSTMODE_INCR8   2
#define MGC_HSDMA_BURSTMODE_INCR16  3

#define MGC_HSDMA_MIN_DMA_LEN        512
#define MGC_HSDMA_MAX_DMA_LEN        100000

/******************************** TYPES **********************************/

#ifdef MUSB_QMU
uint8_t bADataEnable = 0;
#endif
#if 0//defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
static uint8_t MGC_DMAChannelNumConfig[]=
{
 4,2,2,2
};
#endif

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)

struct _MGC_HsDmaController;

typedef struct
{
    MUSB_DmaChannel Channel;
    struct _MGC_HsDmaController *pController;
    uint32_t dwStartAddress;
    uint32_t dwCount;
    uint16_t wMaxPacketSize;
    uint8_t bIndex;
    uint8_t bEnd;
    uint8_t bProtocol;
    uint8_t bTransmit;
    uint8_t bMode;
} MGC_HsDmaChannel;

typedef struct _MGC_HsDmaController
{
    MUSB_DmaController Controller;
    MGC_HsDmaChannel aChannel[MUSB_HSDMA_CHANNELS];
    MUSB_pfSystemToBusAddress pfSystemToBusAddress;
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged;
    void *pDmaPrivate;
    uint8_t *pCoreBaseIsr;
    uint8_t *pCoreBaseBsr;
    uint8_t bChannelCount;
    uint8_t bmUsedChannels;
} MGC_HsDmaController;
#endif /* MUSB_HSDMA && have at least one channel */

#ifdef MUSB_QMU
extern void _MUSB_QMU_Transfer(uint8_t end_num, uint32_t u4Type, uint32_t framenum, uint8_t ishost);
//extern void _MUSB_QMU_Transfer_2(uint8_t end_num, uint8_t end_num2, uint32_t u4Type, uint32_t framenum, uint8_t ishost);
#endif

/******************************* FORWARDS ********************************/

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)
static uint8_t MGC_HsDmaStartController(void *pPrivateData);

#ifndef MUSB_CODE_SHRINK
static uint8_t MGC_HsDmaStopController(void *pPrivateData);
#endif
static MUSB_DmaChannel *MGC_HsDmaAllocateChannel(void *pPrivateData, uint8_t bLocalEnd,
                                                 uint8_t bTransmit, uint8_t bProtocol,
                                                 uint16_t wMaxPacketSize);

static void MGC_HsDmaReleaseChannel(MUSB_DmaChannel *pChannel);
#ifdef MUSB_DMA_BUF

static uint8_t *MGC_HsDmaAllocateBuffer(MUSB_DmaChannel *pChannel, uint32_t dwLength);

static uint8_t MGC_HsDmaReleaseBuffer(MUSB_DmaChannel *pChannel, uint8_t *pBuffer);
#endif

static uint8_t MGC_HsDmaProgramChannel(MUSB_DmaChannel *pChannel, uint16_t wPacketSize,
                                       uint8_t bMode, const uint8_t *pBuffer,
                                       uint32_t dwLength);

static MUSB_DmaChannelStatus MGC_HsDmaGetChannelStatus(MUSB_DmaChannel *pChannel);

static uint8_t MGC_HsDmaControllerIsr(void *pPrivateData);
#endif /* MUSB_HSDMA && have at least one channel */

static MUSB_DmaController *MGC_HsNewDmaController(
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged, void *pDmaPrivate,
    MUSB_pfSystemToBusAddress pfSystemToBusAddress, uint8_t *pCoreBaseIsr, 
    uint8_t *pCoreBaseBsr);

#ifndef MUSB_CODE_SHRINK
static void MGC_HsDestroyDmaController(MUSB_DmaController *pController);
#endif
/******************************* GLOBALS *********************************/
#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)
static MGC_HsDmaController _arMGC_HsDmaController[MUSB_MAX_CONTROLLERS];
#endif

MUSB_DmaControllerFactory MUSB_HdrcDmaControllerFactory =
{
    MGC_HsNewDmaController
#ifndef MUSB_CODE_SHRINK     
    , MGC_HsDestroyDmaController
#endif    
};

/****************************** FUNCTIONS ********************************/

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)

static uint8_t MGC_HsDmaStartController(void *pPrivateData)
{
    /* nothing to do */
    UNUSED(pPrivateData);
    return TRUE;
}

#ifndef MUSB_CODE_SHRINK
static uint8_t MGC_HsDmaStopController(void *pPrivateData)
{
    uint8_t bChannel;

    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pPrivateData);
    pController = (MGC_HsDmaController *)pPrivateData;
    pBase = pController->pCoreBaseBsr;
    MUSB_ASSERT(pBase);    
    for (bChannel = 0; bChannel < pController->bChannelCount; bChannel++)	
    {
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), 0);

        pImplChannel = (MGC_HsDmaChannel *)&(pController->aChannel[bChannel]);
        pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_FREE;
        pImplChannel->Channel.dwActualLength = 0;
    }

    pController->bmUsedChannels = 0;
    return TRUE;
}
#endif
static MUSB_DmaChannel *MGC_HsDmaAllocateChannel(void *pPrivateData, uint8_t bLocalEnd,
                                                 uint8_t bTransmit, uint8_t bProtocol,
                                                 uint16_t wMaxPacketSize)
{
    uint8_t bBit;

    MUSB_DmaChannel *pChannel = NULL;
    MGC_HsDmaChannel *pImplChannel = NULL;
    MGC_HsDmaController *pController;

    MUSB_ASSERT(pPrivateData);
    pController = (MGC_HsDmaController *)pPrivateData;

    if (bLocalEnd && (MUSB_ENDPOINT_XFER_BULK == bProtocol))
    {
        for (bBit = 0; (bBit < pController->bChannelCount) && (bBit < MUSB_HSDMA_CHANNELS); bBit++)
        {
            if (!(pController->bmUsedChannels&(1 << bBit)))
            {
                pController->bmUsedChannels |= (1 << bBit);

                pImplChannel = &(pController->aChannel[bBit]);
                pImplChannel->pController = pController;
                pImplChannel->wMaxPacketSize = wMaxPacketSize;
                pImplChannel->bIndex = bBit;
                pImplChannel->bEnd = bLocalEnd;
                pImplChannel->bProtocol = bProtocol;
                pImplChannel->bTransmit = bTransmit;
                pChannel = &(pImplChannel->Channel);
                pChannel->pPrivateData = pImplChannel;
                pChannel->bStatus = MUSB_DMA_STATUS_FREE;
                //  MTK Notice: Max Liao, 2006/11/16.
                //  v2.303 bug: max DMA length always set to 4G Mbytes - 1.
                pChannel->dwMaxLength = 0xFFFFFFFF;
                /* Tx => mode 1; Rx => mode 0 */
                pChannel->bDesiredMode = bTransmit;
                break;
            }
        }
    }

    return pChannel;
}

static void MGC_HsDmaReleaseChannel(MUSB_DmaChannel *pChannel)
{
    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;

    MUSB_ASSERT(pChannel);
    pImplChannel = (MGC_HsDmaChannel *)pChannel->pPrivateData;
    MUSB_ASSERT(pImplChannel);
    pController = pImplChannel->pController;
    MUSB_ASSERT(pController);
    pBase = pController->pCoreBaseBsr;
    MUSB_ASSERT(pBase);    

    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(pImplChannel->bIndex, MGC_O_HSDMA_CONTROL), 0);

    pImplChannel->pController->bmUsedChannels &= ~(uint8_t)(1 << pImplChannel->bIndex);
    pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_FREE;
}

#ifdef MUSB_DMA_BUF
static uint8_t *MGC_HsDmaAllocateBuffer(MUSB_DmaChannel *pChannel, uint32_t dwLength)
{
    uint8_t *pBuf;

    pBuf = MUSB_MemAlloc(dwLength);
    /* TODO: this may require platform-specific implementation */
    return pBuf;
}

static uint8_t MGC_HsDmaReleaseBuffer(MUSB_DmaChannel *pChannel, uint8_t *pBuffer)
{
    /* TODO: this may require platform-specific implementation */
    MUSB_MemFree(pBuffer);

    return TRUE;
}
#endif

static uint8_t MGC_HsDmaProgramChannel(MUSB_DmaChannel *pChannel, uint16_t wPacketSize,
                                       uint8_t bMode, const uint8_t *pBuffer,
                                       uint32_t dwLength)
{
    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;
    uint32_t dwCsr;
    uint16_t wCsr = 0;
    uint8_t bChannel;
    MGC_Port *pPort;
    MGC_EndpointResource *pEnd;
   
    /* reject below threshold (packet size) */
    if (dwLength < MGC_HSDMA_MIN_DMA_LEN)
    {
        return FALSE;
    }

    #if 0  // only test for FIFO 
    if(dwLength < MGC_HSDMA_MAX_DMA_LEN)
    {
        return FALSE;
    }
    #endif
    MUSB_ASSERT(pChannel);
    pImplChannel = (MGC_HsDmaChannel *)pChannel->pPrivateData;
    MUSB_ASSERT(pImplChannel);
    pController = pImplChannel->pController;
    MUSB_ASSERT(pController);   
    pBase = bMode ? pController->pCoreBaseBsr : pController->pCoreBaseIsr;
    MUSB_ASSERT(pBase);   
    dwCsr = (pImplChannel->bEnd << MGC_S_HSDMA_ENDPOINT) | (1 << MGC_S_HSDMA_ENABLE)
                         | (1 << MGC_S_HSDMA_IRQENABLE);
    bChannel = pImplChannel->bIndex;
    pPort = (MGC_Port *)pController->pDmaPrivate;
    MUSB_ASSERT(pPort);   
    pEnd = (MGC_EndpointResource *)MUSB_ArrayFetch(&(pPort->LocalEnds), pImplChannel->bEnd);
    MUSB_ASSERT(pEnd);   
    if (!pEnd)
    {
        LOG(0, "pEnd = NULL.\n", NULL);    
        return FALSE;        
    }

    /* remember the core's mode */
    pImplChannel->bMode = bMode;

    /* we always use the DMAC's mode 1 */
    if (bMode)
    {
        dwCsr |= 1 << MGC_S_HSDMA_MODE1 | MGC_M_HSDMA_BURSTMODE;
    }

    if (pImplChannel->bTransmit)
    HalDmaBufferStart((void *)pBuffer, dwLength, HAL_DMA_TO_DEVICE);
    else
    HalDmaBufferStart((void *)pBuffer, dwLength, HAL_DMA_FROM_DEVICE); 
    //  MTK Notice: Max Liao, 2006/12/08.
    //  flush data cache before dma.
    //HalFlushInvalidateDCache();
    /*
    #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    HalFlushInvalidateDCacheMultipleLine((UINT32)pBuffer,dwLength);
    #endif
    */
    if (pImplChannel->bTransmit)
    {
        //  MTK Notice: Max Liao, 2006/12/10.
        //  prevent client task and USB HISR race condition, set csr in MGC_HsDmaProgramChannel().
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd);

        if (bMode)
        {
            wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
        }
        else
        {
            wCsr &= (uint16_t)(~(MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE));
        }
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pEnd->bLocalEnd, wCsr);
        dwCsr |= 1 << MGC_S_HSDMA_TRANSMIT;
    }
    else
    {
        wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd);
        if (bMode)
        {
            wCsr |= (MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_DMAENAB);
            if (pPort->bIsHost)
            {
                //  MTK Notice: Max Liao, 2006/09/25.
                //  Request the actual number of packet to be received.
                MUSB_ASSERT(pEnd->wRxPacketSize > 0);
                MGC_DMA_Write32(pBase, M_REG_REQPKT(pEnd->bLocalEnd),
                                (((dwLength + pEnd->wRxPacketSize) - 1) /pEnd->wRxPacketSize));

                wCsr &= ~MGC_M_RXCSR_RXPKTRDY;
                // host use MGC_M_RXCSR_DMAMODE.
                wCsr |= (MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAMODE | MGC_M_RXCSR_H_REQPKT);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, wCsr);
            }
            else
            {
                wCsr |= (MGC_M_RXCSR_DMAMODE);
                MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, wCsr);
            }
        }
        else
        {
            wCsr &= (uint16_t)(~(MGC_M_RXCSR_AUTOCLEAR | MGC_M_RXCSR_H_AUTOREQ | MGC_M_RXCSR_DMAENAB
                          | MGC_M_RXCSR_DMAMODE));
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, wCsr);
        }
    }

    /* address/count */
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_ADDRESS),
                (uint32_t)pController->pfSystemToBusAddress(pBuffer));
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT), dwLength);

    /* control (this should start things) */
    pChannel->dwActualLength = 0L;
    pImplChannel->dwStartAddress = (uint32_t)pBuffer;
    pImplChannel->dwCount = dwLength;
    MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), dwCsr);

    UNUSED(wPacketSize);
    
    return TRUE;
}

static MUSB_DmaChannelStatus MGC_HsDmaGetChannelStatus(MUSB_DmaChannel *pChannel)
{
    uint32_t dwCount;

    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBase;
    uint8_t bChannel;
    uint32_t dwCsr;
    uint32_t dwCsrMask;

    MUSB_ASSERT(pChannel);
    pImplChannel = (MGC_HsDmaChannel *)pChannel->pPrivateData;
    MUSB_ASSERT(pImplChannel);
    pController = pImplChannel->pController;
    MUSB_ASSERT(pController);   
    pBase = pController->pCoreBaseIsr;
    MUSB_ASSERT(pBase);   
    bChannel = pImplChannel->bIndex;
    dwCsr = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));

    if (dwCsr&(1 << MGC_S_HSDMA_BUSERROR))
    {
        return MUSB_DMA_STATUS_BUS_ABORT;
    }

    dwCount = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT));
    pChannel->dwActualLength = pImplChannel->dwCount - dwCount;

    //  MTK Notice: Max Liao, 2006/12/15.
    //  handle last short packet in multiple packet DMA RX mode 1.  
    dwCsrMask = (uint32_t)
        ((1 << MGC_S_HSDMA_ENABLE) | (1 << MGC_S_HSDMA_MODE1) | (1 << MGC_S_HSDMA_IRQENABLE));

    if ((dwCsr & dwCsrMask) == dwCsrMask)
    {
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL), 0);
        return MUSB_DMA_STATUS_FREE;
    }

    if (dwCount)
    {
        return MUSB_DMA_STATUS_BUSY;
    }

    return MUSB_DMA_STATUS_FREE;
}

static uint8_t MGC_HsDmaControllerIsr(void *pPrivateData)
{
    uint8_t bAllowDma = FALSE;

    MGC_BsrItem qItem;
    MUSB_Irp *pIrp;
#ifdef MUSB_ISO

    MUSB_IsochIrp *pIsochIrp;
#endif

    MGC_EndpointResource *pEnd;
    uint8_t bChannel;
    uint32_t dwCsr;
    uint32_t dwCount;
    MGC_HsDmaChannel *pImplChannel;
    MGC_HsDmaController *pController;
    uint8_t *pBuffer = NULL;
    uint32_t dwReqLength = 0;
    uint8_t bComplete = FALSE;
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;
    uint8_t *pBase;
	#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    uint8_t bIntr,bIntrMask;
	#endif
    uint8_t bRet;
    
    MUSB_ASSERT(pPrivateData);
    pController = (MGC_HsDmaController *)pPrivateData;    
    pBase = pController->pCoreBaseIsr;
    MUSB_ASSERT(pBase);
	#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    bIntr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
	bIntrMask = MGC_Read8(pBase, MGC_O_HSDMA_INTR_MASK);
	bIntr = (bIntr&bIntrMask);
	#endif

	#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    if (!bIntr)
	#endif		
    {
        return FALSE;
    }
    //  MTK Notice: Max Liao, 2006/10/01.
    //  write clear interrupt register value.
	#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    MGC_Write8(pBase, MGC_O_HSDMA_INTR, bIntr);  
	#endif

    pPort = (MGC_Port *)pController->pDmaPrivate;
    MUSB_ASSERT(pPort);
    MUSB_ASSERT(pPort->pController);    
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

	for (bChannel = 0; (bChannel < pController->bChannelCount) && (bChannel < MUSB_HSDMA_CHANNELS); bChannel++)
    {
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
	    if (bIntr&(1 << bChannel))
		#endif		
        {
            pImplChannel = (MGC_HsDmaChannel *)&(pController->aChannel[bChannel]);

            dwCsr = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_CONTROL));
            dwCount = MGC_Read32(pBase, MGC_HSDMA_CHANNEL_OFFSET(bChannel, MGC_O_HSDMA_COUNT));
            pImplChannel->Channel.dwActualLength = pImplChannel->dwCount - dwCount;

            if (dwCsr&(1 << MGC_S_HSDMA_BUSERROR))
            {
                pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_BUS_ABORT;
                dwStatus = (uint32_t)MUSB_STATUS_DMA_ERROR;
            }
            else
            {
                pImplChannel->Channel.bStatus = MUSB_DMA_STATUS_FREE;
            }

            MGC_SelectEnd(pBase, pImplChannel->bEnd);
            if (pImplChannel->bTransmit)
            {

#ifndef MUSB_DISABLE_DB_DYNFIFO                                   
                /* Notice: double packet buffer fifo need to check fifo empty */  
                while ((MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pImplChannel->bEnd)) & 
                    (MGC_M_TXCSR_FIFONOTEMPTY|MGC_M_TXCSR_TXPKTRDY))
                {
                    /*
                    LOG(0, "DMA FIFO not empty 0x%X.\n", 
                        (MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, pImplChannel->bEnd)));
                    */
                    ;
                }                    
#endif

                //  MTK Notice: Max Liao, 2006/12/14.
                //  send last short packet in multiple packet transfer and tx single packet.
                if ((pImplChannel->dwCount %pImplChannel->wMaxPacketSize)
                        || (pImplChannel->dwCount <= pImplChannel->wMaxPacketSize))
                {
                    MUSB_DIAG_STRING(3, "DMA: txpktrdy.");
                    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, pImplChannel->bEnd,
                                   MGC_M_TXCSR_TXPKTRDY);
                }
                else
                {
                    MUSB_DIAG_STRING(3, "DMA: tx status change.");
                    HalDmaBufferDone((void *)pImplChannel->dwStartAddress,pImplChannel->dwCount, HAL_DMA_TO_DEVICE);
                    bRet = pController->pfDmaChannelStatusChanged(
                        pController->pDmaPrivate, pImplChannel->bEnd, pImplChannel->bTransmit);
                    UNUSED(bRet);                        
                }
            }
            else
            {
                /* 
                 * NOTE: assume mode 0 implies the residual handling case,
                 * and therefore use our NASTY hack, because this is all
                 * specific to the Inventra DMAC
                 */
                pEnd = (MGC_EndpointResource *)
                           MUSB_ArrayFetch(&(pPort->LocalEnds), pImplChannel->bEnd);
                if (!pEnd)
                {
                    LOG(0, "pEnd = NULL.\n", NULL);    
                    return FALSE;        
                }

                MUSB_ASSERT(pEnd->pRxIrp);

                pEnd->dwRxOffset += pImplChannel->dwCount;
                //pEnd->bDmaResidual = FALSE;

                switch (pEnd->bRxTrafficType)
                {
                case MUSB_ENDPOINT_XFER_BULK:
                case MUSB_ENDPOINT_XFER_INT:
                    pIrp = (MUSB_Irp *)pEnd->pRxIrp;
                    bAllowDma = pIrp->bAllowDma;
                    pBuffer = pIrp->pBuffer + pEnd->dwRxOffset;
                    dwReqLength = pIrp->dwLength;
                    if (pEnd->dwRxOffset > pIrp->dwLength)
                    {
                        LOG(1, "Error: pEnd->dwRxOffset=%d > pIrp->dwLength=%d.\n", 
                            pEnd->dwRxOffset, pIrp->dwLength);
                    }
                    if ((pEnd->dwRxOffset >= pIrp->dwLength)
                            || (pImplChannel->dwCount < pEnd->wRxPacketSize))
                    {
                        bComplete = TRUE;
                    }
                    break;
#ifdef MUSB_ISO

                case MUSB_ENDPOINT_XFER_ISOC:
                    pIsochIrp = (MUSB_IsochIrp *)pEnd->pRxIrp;
                    bAllowDma = pIsochIrp->bAllowDma;
                    pBuffer = pIsochIrp->pBuffer + pEnd->dwRxOffset;
#ifdef MUSB_ISO_STREAM                    
                    // under develop.
                    bComplete = FALSE;
#else
                    bComplete = (pIsochIrp->wCurrentFrame >= pIsochIrp->wFrameCount) ? TRUE : FALSE;
#endif
                    break;
#endif

                default:
                    break;
                }

                if (bComplete)
                {
                    MUSB_DIAG_STRING(3, "DMA: rx complete.");
                    HalDmaBufferDone((void *)pImplChannel->dwStartAddress,pImplChannel->dwCount, HAL_DMA_FROM_DEVICE);
                    //  MTK Notice: Max Liao, 2006/12/08.
                    //  release RX DMA channel usage before handle MGC_CompleteIrp().
                    if (pEnd->pRxDmaChannel)
                    {
                        MGC_HsDmaReleaseChannel(pEnd->pRxDmaChannel);
                        pEnd->pRxDmaChannel = NULL;
                    }

                    //  MTK Notice: Max Liao, 2006/12/14.
                    //  clear RXPKTRDY when DMA size is not the multiple of max packet size.
                    //  And receive single packet.
                    if ((pImplChannel->dwCount %pImplChannel->wMaxPacketSize)
                            || (pImplChannel->dwCount <= pImplChannel->wMaxPacketSize))
                    {
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pImplChannel->bEnd, 0);
                    }
                    else
                    {
                        //  MTK Notice: Max Liao, 2006/12/21.
                        //  Clear DMA control bits. This is tricky. 
                        //  Because in peripheral mode, RXPKTRDY will be set by H/W and 
                        //  it be clear by S/W when S/W want to clear DMA control bits. 		    
                        //  So to prevent it by writing RXPKTRDY(r/clear field) at S/W side.
                        MGC_WriteCsr16(pBase, MGC_O_HDRC_RXCSR, pImplChannel->bEnd,
                                       MGC_M_RXCSR_RXPKTRDY);
                    }

                    if (MGC_CompleteIrp(&qItem, pEnd, dwStatus, FALSE, pEnd->pRxIrp))
                    {
                        //  MTK Notice: Max Liao, 2006/12/08.
                        //  handle complete in USBBSR task when MGC_CompleteIrp() return TRUE.		    
                        pPort->bDmaCompleted = TRUE;
                        VERIFY(pServices->pfQueueBackgroundItem(pServices->pPrivateData, &qItem));
                    }
                    MUSB_ASSERT(pEnd->dwRxOffset == 0);
                }
                else
                {
                    dwReqLength = dwReqLength - pEnd->dwRxOffset;
                    if (pPort->bIsHost)
                    {
                        dwStatus = pPort->pfProgramStartReceive(
                            pPort, pEnd, pBuffer, dwReqLength, pEnd->pRxIrp, bAllowDma);
                        UNUSED(dwStatus);
                    }
                    else
                    {
                        if (dwReqLength > pEnd->wRxPacketSize)
                        {
                            VERIFY(MGC_HsDmaProgramChannel(
                                pEnd->pRxDmaChannel, pEnd->wRxPacketSize, 1, pBuffer, dwReqLength));
                        }
                        else
                        {
                            //  MTK Notice: Max Liao, 2006/12/19.
                            //  release dma channel when rx short packet in peripheral mode.
                            if (pEnd->pRxDmaChannel)
                            {
                                MGC_HsDmaReleaseChannel(pEnd->pRxDmaChannel);
                                pEnd->pRxDmaChannel = NULL;
                            }
                        }

                        /* ack recv, clear MGC_M_RXCSR_RXPKTRDY */
                        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, pEnd->bLocalEnd, 0);
                    }
                }
            }
        }
    }

    return TRUE;
}
#endif /* MUSB_HSDMA && have at least one channel */


static MUSB_DmaController *MGC_HsNewDmaController(
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged, void *pDmaPrivate,
    MUSB_pfSystemToBusAddress pfSystemToBusAddress, uint8_t *pCoreBaseIsr, 
    uint8_t *pCoreBaseBsr)
{
    MUSB_DmaController *pResult = NULL;
    #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    uint8_t intrmask = 0;
    uint8_t i;
    #endif

#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)

//  MTK Notice: Max Liao, 2007/04/04.
//  change to static allocation.
/*
    MGC_HsDmaController *
        pController = (MGC_HsDmaController *)MUSB_MemAlloc(sizeof(MGC_HsDmaController));
*/
    MGC_HsDmaController *pController;

    MUSB_ASSERT(MGC_bSystemCount < MUSB_MAX_CONTROLLERS);
    if (MGC_bSystemCount >= (uint8_t)MUSB_MAX_CONTROLLERS)    
    {
        return pResult;
    }
    pController = &_arMGC_HsDmaController[MGC_bSystemCount];
    //if (pController)
    {
        MUSB_MemSet(pController, 0, sizeof(MGC_HsDmaController));

        pController->pfSystemToBusAddress = pfSystemToBusAddress;
        pController->pfDmaChannelStatusChanged = pfDmaChannelStatusChanged;
        pController->pDmaPrivate = pDmaPrivate;
        UNUSED(pDmaPrivate);
        pController->pCoreBaseIsr = pCoreBaseIsr;
        UNUSED(pCoreBaseIsr);
        pController->pCoreBaseBsr = pCoreBaseBsr;
        UNUSED(pCoreBaseBsr);        
    #if 0//defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        pController->bChannelCount = MGC_DMAChannelNumConfig[MGC_bSystemCount]; 
    #else
        pController->bChannelCount = MUSB_HSDMA_CHANNELS;  
    #endif
    	for(i=0; i<pController->bChannelCount; i++)
    	{
    	    intrmask += (1<<i); 
    	}	
    	MGC_Write8(pController->pCoreBaseIsr, MGC_O_HSDMA_INTR_SET, intrmask);
        pController->Controller.pPrivateData = pController;
        pController->Controller.pfDmaStartController = MGC_HsDmaStartController;
#ifndef MUSB_CODE_SHRINK        
        pController->Controller.pfDmaStopController = MGC_HsDmaStopController;
#endif        
        pController->Controller.pfDmaAllocateChannel = MGC_HsDmaAllocateChannel;
        pController->Controller.pfDmaReleaseChannel = MGC_HsDmaReleaseChannel;
#ifdef MUSB_DMA_BUF

        pController->Controller.pfDmaAllocateBuffer = MGC_HsDmaAllocateBuffer;
        pController->Controller.pfDmaReleaseBuffer = MGC_HsDmaReleaseBuffer;
#endif

        pController->Controller.pfDmaProgramChannel = MGC_HsDmaProgramChannel;
        pController->Controller.pfDmaGetChannelStatus = MGC_HsDmaGetChannelStatus;
        pController->Controller.pfDmaControllerIsr = MGC_HsDmaControllerIsr;
        pResult = &(pController->Controller);
    }
#endif /* MUSB_HSDMA && have at least one channel */

    return pResult;
}

#ifndef MUSB_CODE_SHRINK 
static void MGC_HsDestroyDmaController(MUSB_DmaController *pController)
{
#if defined(MUSB_DMA) && defined(MUSB_HSDMA) && (MUSB_HSDMA_CHANNELS > 0)    
    MGC_HsDmaController *pHsController;

    MUSB_ASSERT(pController);
    
    pHsController = (MGC_HsDmaController *)pController->pPrivateData;
    UNUSED(pController);
    
    if (pHsController)
    {
        pHsController->Controller.pPrivateData = NULL;
        MUSB_MemFree(pHsController);
    }
#endif /* MUSB_HSDMA && have at least one channel */

}
#endif

//#ifdef MUSB_FUNCTION
#if 0

/*
* Send data in polling mode in CDC device mode.
*/
uint8_t MGC_DevPollTx(MGC_Port *pPort, MGC_EndpointResource *pEnd, 
    const uint8_t *pBuffer, uint32_t dwTotalBytes)
{
    uint8_t bIntrUsbValue;
    uint16_t wIntrTxValue;
    uint32_t dwDmaIntr;
    uint8_t *pBase;
    MGC_Controller *pController;
    MUSB_SystemServices *pServices;
    uint32_t dwIrqStatus;
    uint8_t bEnd;
    uint8_t bIndex;
    uint16_t wCsr;
    uint8_t bStatus = TRUE;
        
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pServices = pController->pSystemServices;
    MUSB_ASSERT(pServices);        
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);
    MUSB_ASSERT(pEnd);

    // disable USB interrupt.   
    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);      

    /* save index */
    bIndex = MGC_Read8(pBase, MGC_O_HDRC_INDEX);  

    bEnd = pEnd->bLocalEnd;
       
    // select ep
    MGC_SelectEnd(pBase, bEnd);  

     //  flush data cache before dma.
    //HalFlushInvalidateDCache();
    #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    HalFlushInvalidateDCacheMultipleLine((UINT32)pBuffer,dwTotalBytes);
    #endif
    // set multiple packet tx mode.    
    wCsr = MGC_ReadCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd);
    wCsr |= (MGC_M_TXCSR_AUTOSET | MGC_M_TXCSR_DMAENAB | MGC_M_TXCSR_DMAMODE);
    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, wCsr | MGC_M_TXCSR_MODE);
    
    // set channel 1 dma address.
    MGC_Write32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_ADDRESS), (uint32_t)pBuffer);

    // set data size.
    MGC_Write32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_COUNT), dwTotalBytes);

    // set multiple packet tx mode, trigger dma
    MGC_Write32(pBase, 
        MGC_HSDMA_CHANNEL_OFFSET(1, MGC_O_HSDMA_CONTROL), (0xF |(bEnd << 4)));

    // Check channel 2 DMA happen.
    dwDmaIntr = 0;
    while ((dwDmaIntr & 0x2) == 0)
    {
        // check disconnect.
        bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);    
        bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
        if (bIntrUsbValue > 0)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);
            bStatus = FALSE;
            goto out;            
        }

        dwDmaIntr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
    }
    MGC_Write32(pBase, MGC_O_HSDMA_INTR, 0x2);

    // trigger to send last packet.
    MGC_WriteCsr16(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);

    wIntrTxValue = 0;
    while (wIntrTxValue == 0)
    {
        // check disconnect.
        bIntrUsbValue = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);    
        bIntrUsbValue &= MGC_Read8(pBase, MGC_O_HDRC_INTRUSBE);
        if (bIntrUsbValue > 0)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, bIntrUsbValue);
            bStatus = FALSE;
            goto out;            
        }
        
        wIntrTxValue = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        wIntrTxValue &= MGC_Read16(pBase, MGC_O_HDRC_INTRTXE);
    }

    // handle tx ep interrupt.
    if ((wIntrTxValue & (1<<bEnd)) == 0)
    {
        bStatus = FALSE;
        goto out;
    }

    //  write clear interrupt register value.
    MGC_Write16(pBase, MGC_O_HDRC_INTRTX, (1<<bEnd));

out:

    /* restore index */
    MGC_Write8(pBase, MGC_O_HDRC_INDEX, bIndex);
    
    // Enable USB interrupt.         
    pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);

    return bStatus;
}
#endif

#ifdef MUSB_ISO_EMULATION
/*
 *  MGC_Host_SendISOPkt 
 *  TEST command: send ISO packet out
 */
void MGC_Host_SendISOPkt(uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance, uint32_t interval)
{
    uint32_t   intr;
    uint32_t   uframe = 0;
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t bIsMulti = FALSE;
    uint8_t reg;    
#endif

    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));

#ifdef MTK_MHDRC

    /* target addr & hub addr/port */
    if(pDevice->pParentUsbDevice)
    {
        bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
        bHubPort = pDevice->bHubPort;
        if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
        {
            bIsMulti = TRUE;
        }
    }

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);

    //  set special device address to test.
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXFUNCADDR), 
        100);
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBADDR), 
        (bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_TXHUBPORT), 
        bHubPort);
#else
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));        

    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif

    // set fifo size and address.
#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0xF);
#endif
    MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 8);
    
    // set tx maximum data size.  
    if (pktsize <= 1024)
    {
        if (pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_FULL)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 1023);        
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x400);
        }
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x800 |u4MaxPktSz));            
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0xC00);            
        }
    }
    else
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x1000 |u4MaxPktSz));
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x1400);
        }        
    }     
    
    // set tx polling interval.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXINTERVAL, bEnd, interval);

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x20);
    
    while (uframe < framenum)
    {    
         //  flush data cache before dma.
        //HalFlushInvalidateDCache();
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,pktsize);
        #endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), pktsize);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xB |(bEnd << 4)));
        
        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle dma interrupt.
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
            MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
        #endif

            // trigger packet send.
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);            
        }

        BIM_ClearIrq(VECTOR_USB);

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr);            
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += pktsize;
        uframe ++;        
    }

    // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x0);
    
    BIM_EnableIrq(VECTOR_USB);
}


/*
 *  MGC_Host_GetISOPkt 
 *  TEST command: receive ISO packet from device.
 */
void MGC_Host_GetISOPkt(uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance, uint32_t interval)
{
    uint32_t   intr;
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
    uint32_t csr;    
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t bHubAddr = 0;
    uint8_t bHubPort = 0;
    uint8_t bIsMulti = FALSE;
    uint8_t reg;    
#endif

    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));

#ifdef MTK_MHDRC

    /* target addr & hub addr/port */
    if(pDevice->pParentUsbDevice)
    {
        bHubAddr = pDevice->pParentUsbDevice->bBusAddress;
        bHubPort = pDevice->bHubPort;
        if(2 == pDevice->pParentUsbDevice->DeviceDescriptor.bDeviceProtocol)
        {
            bIsMulti = TRUE;
        }
    }

    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);        

    //  set special device address to test.
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXFUNCADDR), 
        100);
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBADDR), 
        (bIsMulti ? (0x80 | bHubAddr) : bHubAddr));
    MGC_Write8(pBase, MGC_BUSCTL_OFFSET(bEnd, MGC_O_MHDRC_RXHUBPORT), 
        bHubPort);
#else
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));        

    MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif

    // set fifo size and address.
#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
    
    // set rx maximum data size.   
    if (pktsize <= 1024)
    {
        if (pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_FULL)
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 1023);
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x400);
        }
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: Data1, Data0.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, (0x800 |u4MaxPktSz));       	
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0xC00);
        }
    }
    else
    {
        // balance data in two packet: Data2, Data1, Data0.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, (0x1000 |u4MaxPktSz));            
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);            
        }        
    }     

    // set rx polling interval.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXINTERVAL, bEnd, interval);       	

    while (count < framenum)
    {
        // request pkt from device.
        MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, MGC_M_RXCSR_H_REQPKT);       	

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // check if IncompRX happen.
        csr = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCSR, bEnd);
        if (csr & MGC_M_RXCSR_INCOMPRX)
        {
            MUSB_ASSERT((csr & MGC_M_RXCSR_INCOMPRX) == 0);
        }
                
        // handle usb interrupt.
        intr = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);      
        if (intr)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, intr);        
        }

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr);                
        }

        BIM_ClearIrq(VECTOR_USB);

         //  flush data cache before dma.
        //HalFlushInvalidateDCache();
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd));
        #endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), rxcount);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x9 |(bEnd << 4)));

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle dma interrupt.
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
		intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR); 
		#else 
	    intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
            #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
            MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		    #else
            MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
            #endif

            // clear rx status.
	    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));       	
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += rxcount;
        count ++;        
    }
    
    BIM_EnableIrq(VECTOR_USB);
}

/*
 *  MGC_Dev_SendISOPkt 
 *  TEST command: send ISO packet out
 */
void MGC_Dev_SendISOPkt(uint32_t bEnd, uint32_t pktsize, uint8_t *buf, uint32_t framenum, uint32_t balance)
{
    uint32_t   intr;
    uint32_t   uframe = 0;
    uint32_t  u4MaxPktSz = 0;
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    Printf("Dev_SendISOPkt:\n");
    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);
    
    // set fifo size and address.
#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_TXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_TXFIFOADD, 8);

    // set tx maximum data size.  
    if (pktsize <= 1024)
    {
        MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x400);       
    }
    else if ((pktsize > 1024) && (pktsize < 2048))
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {
            u4MaxPktSz = (pktsize + 1)/2;
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x800 |u4MaxPktSz));            
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0xC00);            
        }
    }
    else
    {
        // balance data in two packet: MData, Data1.
        if (balance)
        {    
            u4MaxPktSz = (pktsize + 2)/3;    
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, (0x1000 |u4MaxPktSz));
        }
        else
        {
            MGC_WriteCsr16(pBase, MGC_O_HDRC_TXMAXP, bEnd, 0x1400);
        }        
    }     

    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, (0x10 |bEnd));

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x60);
    Printf("Start Dev Send ISO packet. pktsize = 0x%x, framenum =%d\n", pktsize, framenum);  
    while (uframe < framenum)
    {
         //  flush data cache before dma.
        //HalFlushInvalidateDCache();
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,pktsize);
        #endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), pktsize);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0xB |(bEnd << 4)));

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));
        Printf("Handel DMA iterrupt pktsize = 0x%x.\n", pktsize);
        // handle dma interrupt.
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
			#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
			MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
			#else
			MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
			#endif

            // trigger packet send.
            MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR, bEnd, MGC_M_TXCSR_TXPKTRDY);            
        }

        BIM_ClearIrq(VECTOR_USB);

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRTX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRTX, intr);            
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += pktsize;
        uframe ++;        
    }

    // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0);
    
    BIM_EnableIrq(VECTOR_USB);
}


/*
 *  MGC_Dev_GetISOPkt 
 *  TEST command: receive ISO packet from host.
 */
uint32_t MGC_Dev_GetISOPkt(uint32_t bEnd, uint8_t *buf, uint32_t framenum)
{
    uint32_t   intr;
    uint32_t   count = 0;
    uint32_t   rxcount = 0;
    uint32_t csr;    
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;

    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    //  set special device address to test.
    MGC_Write8(pBase, MGC_O_HDRC_FADDR, 100);

    // set fifo size and address.
#if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0x9);
#else
    MGC_Write8(pBase, MGC_O_HDRC_RXFIFOSZ, 0xF);
#endif

    MGC_Write16(pBase, MGC_O_HDRC_RXFIFOADD, 8);
	
    // set rx maximum data size.   
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXMAXP, bEnd, 0x1400);       	

    // set rx ep type.
    MGC_WriteCsr16(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));
    
    // set ISO mode in device mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR2, bEnd, 0x40);
    
    Printf("Start Dev GetISOPacket count = %d, framenum = %d:\n", count, framenum);    
    #ifdef MUSB_QMU
    while (count < (framenum - 4))
    #else
    while (count < framenum)
    #endif
    {
        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));
        Printf(" Dev Get USB Interrupt count=%d:\n", count);
        // handle usb interrupt.
        intr = MGC_Read8(pBase, MGC_O_HDRC_INTRUSB);      
        if (intr)
        {
            MGC_Write8(pBase, MGC_O_HDRC_INTRUSB, intr);        
        }

        // handle ep interrupt.
        intr = MGC_Read16(pBase, MGC_O_HDRC_INTRRX);
        
        MUSB_ASSERT(intr);
        if(intr)
        {
            MGC_Write16(pBase, MGC_O_HDRC_INTRRX, intr);                
        }

        BIM_ClearIrq(VECTOR_USB);

         //  flush data cache before dma.
        //HalFlushInvalidateDCache();
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)buf,MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd));
        #endif
        // set dma address.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_ADDRESS), (uint32_t)buf);

        rxcount = MGC_ReadCsr16(pBase, MGC_O_HDRC_RXCOUNT, bEnd);

        // set data size.
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_COUNT), rxcount);
        
        // trigger dma
        MGC_Write32(pBase, MGC_HSDMA_CHANNEL_OFFSET(0, MGC_O_HSDMA_CONTROL), (0x9 |(bEnd << 4)));

        // wait irq.    
        while (!BIM_IsIrqPending(VECTOR_USB));

        // handle dma interrupt.
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
         intr = MGC_Read8(pBase, MGC_O_HSDMA_INTR);  
		#else
        intr = MGC_Read32(pBase, MGC_O_HSDMA_INTR);   
		#endif

        MUSB_ASSERT(intr);

        if(intr)
        {
        #if defined(CC_MT5368) || defined(CC_MT5396)|| defined(CC_MT5389)
	    MGC_Write8(pBase, MGC_O_HSDMA_INTR, intr);
		#else
        MGC_Write32(pBase, MGC_O_HSDMA_INTR, intr);
		#endif

            // clear rx status.
	    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
            MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));       	
        }

        BIM_ClearIrq(VECTOR_USB);

        buf += rxcount;
        count ++;        
    }
    
    BIM_EnableIrq(VECTOR_USB);

    return rxcount;
}

#endif /* MUSB_ISO_EMULATION enabled */

#ifdef MUSB_QMU

void MGC_QMU_Host_SendPkt(uint8_t btxEnd, uint32_t u4Type,uint32_t framenum, int8_t ishost)
{
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
#ifdef MTK_MHDRC
    uint8_t reg;    
#endif
    uint32_t bEnd = btxEnd;
    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));

#ifdef MTK_MHDRC
    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set tx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXTYPE, bEnd, reg);
#endif

    // set ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x20);

    BIM_EnableIrq(VECTOR_USB);
    _MUSB_QMU_Transfer(bEnd, u4Type, framenum, ishost);

        // clear ep tx mode.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_TXCSR2, bEnd, 0x0);
    MGC_QMU_Write8 (MGC_O_HDRC_INTRUSBE, 0xff);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRTXE, 0xff);
    MGC_QMU_Write16 (MGC_O_HDRC_INTRRXE, 0xff);
    return;
}


void MGC_QMU_Host_GetPkt(uint8_t rxend, uint32_t u4Type, uint32_t framenum, int8_t ishost)
{

    uint32_t csr;    
    uint8_t *pBase;
    MGC_Port *pPort;
    MUSB_Port *pUsbPort = NULL;
    MGC_Controller *pController;
    const MUSB_Device* pDevice = NULL;
    uint16_t wCount;
    uint8_t reg;    

    uint32_t bEnd = rxend;
    
    pUsbPort = MUSB_GetPort(0);
    MUSB_ASSERT(pUsbPort);        
    pPort = (MGC_Port *)pUsbPort->pPrivateData;
    MUSB_ASSERT(pPort);
    pController = pPort->pController;
    MUSB_ASSERT(pController);
    pBase = (uint8_t *)pController->pControllerAddressIst;
    MUSB_ASSERT(pBase);

    // disable USB interrupt.   
    BIM_DisableIrq(VECTOR_USB); 

    // select ep
    MGC_SelectEnd(pBase, bEnd);    

    // get the last device to test iso.
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    MUSB_ASSERT(wCount > 0);
    pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), (wCount-1));

#ifdef MTK_MHDRC
    reg = (0x10 |bEnd);
    /* speed field in proto reg */
    switch(pDevice->ConnectionSpeed)
    {
        case MUSB_CONNECTION_SPEED_LOW:
            // iso do not support low speed.
            MUSB_ASSERT(0);
            break;
        case MUSB_CONNECTION_SPEED_FULL:
            reg |= 0x80;
            break;
        default:
            reg |= 0x40;
    }
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, reg);        
#else
    // set rx ep type.
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXTYPE, bEnd, (0x10 |bEnd));        

   // MGC_Write32(pBase, M_REG_EPXADDR(bEnd), 100);
#endif
    BIM_EnableIrq(VECTOR_USB);
    _MUSB_QMU_Transfer(bEnd,u4Type, framenum, ishost);
        
    // clear rx status.
    csr = MGC_ReadCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd);
    MGC_WriteCsr8(pBase, MGC_O_HDRC_RXCSR, bEnd, (csr & ~MGC_M_RXCSR_RXPKTRDY));        
   
    return;
}
#endif
