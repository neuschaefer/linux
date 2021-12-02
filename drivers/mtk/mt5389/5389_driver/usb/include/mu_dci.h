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
 * MUSBStack-S DMA Controller Interface (DCI).
 * $Revision: #1 $
 */

#ifndef __MUSB_DCI_H__
#define __MUSB_DCI_H__

#include "mu_dsi.h"

/**
 * DMA Controller Interface (DCI) Introduction.
 * The purpose of this interface is to allow the UCD to use any
 * DMA controller, since this is an option in the Inventra USB cores.
 * The assumptions are:
 * <ul>
 * <li>A DMA controller will be tied to an Inventra USB core in the
 * way specified in the Inventra core product specification.
 * <li>A DMA controller's base address in the memory map correlates
 * somehow to the Inventra USB core it serves.
 * </ul>
 * The responsibilities of a DCI implementation include:
 * <ul>
 * <li>Allocating/releasing buffers for use with DMA
 * (this is OS-specific, but may also be specific to a
 * DMA controller, intervening busses, and a target's capabilities,
 * so the UCD cannot make assumptions or provide services here)
 * <li>Handling the details of moving multiple USB packets 
 * in cooperation with the Inventra USB core.
 * <li>Knowing the correlation between channels and the
 * Inventra core's local endpoint resources and data direction,
 * and maintaining a list of allocated/available channels.
 * <li>Updating channel status on interrupts,
 * whether shared with the Inventra core or separate.
 * <li>If the DMA interrupt is shared with the Inventra core,
 * handling it when called, and reporting whether it was the
 * source of interrupt.
 * </ul>
 */

/************************* DCI CONSTANTS **************************/

/**
 * DMA channel status.
 */
typedef enum _MUSB_DmaChannelStatus
{
    /** A channel's status is unknown */
    MUSB_DMA_STATUS_UNKNOWN,
    /** A channel is available (not busy and no errors) */
    MUSB_DMA_STATUS_FREE,
    /** A channel is busy (not finished attempting its transactions) */
    MUSB_DMA_STATUS_BUSY,
    /** A channel aborted its transactions due to a local bus error */
    MUSB_DMA_STATUS_BUS_ABORT,
    /** A channel aborted its transactions due to a core error */
    MUSB_DMA_STATUS_CORE_ABORT
} MUSB_DmaChannelStatus;

/*************************** DCI TYPES ****************************/

/**
 * MUSB_DmaChannel.
 * A DMA channel.
 * @field pPrivateData channel-private data; not to be interpreted by the UCD
 * @field wMaxLength the maximum number of bytes the channel can move
 * in one transaction (typically representing many USB maximum-sized packets)
 * @field dwActualLength how many bytes have been transferred
 * @field bStatus current channel status (updated e.g. on interrupt)
 * @field bDesiredMode TRUE if mode 1 is desired; FALSE if mode 0 is desired
 */
typedef struct
{
    void* pPrivateData;  /* MGC_HsDmaChannel* pImplChannel */
    uint32_t dwMaxLength;
    uint32_t dwActualLength;
    MUSB_DmaChannelStatus bStatus;
    uint8_t bDesiredMode;
} MUSB_DmaChannel;

/**
 * Start a DMA controller.
 * This is called by MUSB_StartController.
 * @param pPrivateData private data pointer from MUSB_DmaController
 * @return TRUE on success
 * @return FALSE on failure (e.g. no DMAC appears present)
 */
typedef uint8_t (*MUSB_pfDmaStartController)(void* pPrivateData);

/**
 * Stop a DMA controller.
 * This is called by MUSB_StopController.
 * @param pPrivateData the controller's private data pointer
 * @return TRUE on success
 * @return FALSE on failure; UCD may try again
 */
typedef uint8_t (*MUSB_pfDmaStopController)(void* pPrivateData);

/**
 * Allocate a DMA channel.
 * Allocate a DMA channel suitable for the given conditions.
 * @param pPrivateData the controller's private data pointer
 * @param bLocalEnd the local endpoint index (1-15)
 * @param bTransmit TRUE for transmit; FALSE for receive
 * @param bProtocol the USB protocol, as per USB 2.0 chapter 9
 * (0 => control, 1 => isochronous, 2 => bulk, 3 => interrupt)
 * @param wMaxPacketSize maximum packet size
 * @return a non-NULL pointer on success
 * @return NULL on failure (no channel available)
 */
typedef MUSB_DmaChannel* (*MUSB_pfDmaAllocateChannel)(
    void* pPrivateData, uint8_t bLocalEnd, 
    uint8_t bTransmit, uint8_t bProtocol, uint16_t wMaxPacketSize);

/**
 * Release a DMA channel.
 * Release a previously-allocated DMA channel.
 * The UCD guarantess to no longer reference this channel.
 * @param pChannel pointer to a channel obtained by 
 * a successful call to MUSB_DmaAllocateChannel
 */
typedef void (*MUSB_pfDmaReleaseChannel)(MUSB_DmaChannel* pChannel);

/**
 * Allocate DMA buffer.
 * Allocate a buffer suitable for DMA operations with the given channel.
 * @param pChannel pointer to a channel obtained by 
 * a successful call to MUSB_DmaAllocateChannel
 * @param dwLength length, in bytes, desired for the buffer
 * @return a non-NULL pointer to a suitable region (in processor space)
 * on success
 * @return NULL on failure
 */
typedef uint8_t* (*MUSB_pfDmaAllocateBuffer)(MUSB_DmaChannel* pChannel,
					     uint32_t dwLength);

/**
 * Release DMA buffer.
 * Release a DMA buffer previously acquiring by a successful call
 * to MUSB_DmaAllocateBuffer.
 * @param pChannel pointer to a channel obtained by 
 * a successful call to MUSB_DmaAllocateChannel
 * @param pBuffer the buffer pointer
 * @return TRUE on success
 * @return FALSE on failure (e.g. the controller owns the buffer at present)
 */
typedef uint8_t (*MUSB_pfDmaReleaseBuffer)(MUSB_DmaChannel* pChannel,
					   uint8_t* pBuffer);

/**
 * Program a DMA channel.
 * Program a DMA channel to move data at the core's request.
 * The local core endpoint and direction should already be known,
 * since they are specified in the AllocateChannel call.
 * @param pChannel pointer to a channel obtained by 
 * a successful call to MUSB_DmaAllocateChannel
 * @param wPacketSize the packet size
 * @param bMode TRUE if mode 1; FALSE if mode 0
 * @param pBuffer base address of data (in processor space)
 * @param dwLength the number of bytes to transfer;
 * guaranteed by the UCD to be no larger than the channel's
 * reported dwMaxLength
 * @return TRUE on success
 * @return FALSE on error
 */
typedef uint8_t (*MUSB_pfDmaProgramChannel)(MUSB_DmaChannel* pChannel, 
					    uint16_t wPacketSize, uint8_t bMode,
					    const uint8_t* pBuffer, uint32_t dwLength);

/**
 * Get DMA channel status.
 * Get the current status of a DMA channel, if the hardware allows.
 * @param pChannel pointer to a channel obtained by 
 * a successful call to MUSB_DmaAllocateChannel
 * @return current status 
 * (MUSB_DMA_STATUS_UNKNOWN if hardware does not have readable status)
 */
typedef MUSB_DmaChannelStatus (*MUSB_pfDmaGetChannelStatus)(
    MUSB_DmaChannel* pChannel);

/**
 * DMA ISR.
 * If present, this function is called by the UCD on every interrupt.
 * This is necessary because with the built-in DMA controller
 * (and probably some other configurations),
 * the DMA interrupt is shared with other core interrupts.
 * Therefore, this function should return quickly 
 * when there is no DMA interrupt.
 * When there is a DMA interrupt, this function should
 * perform any implementations-specific operations,
 * and update the status of all appropriate channels.
 * If the DMA controller has its own dedicated interrupt,
 * this function should do nothing.
 * This function is called BEFORE the UCD handles other interrupts.
 * @param pPrivateData the controller's private data pointer
 * @return TRUE if an interrupt was serviced
 * @return FALSE if no interrupt required servicing
 */
typedef uint8_t (*MUSB_pfDmaControllerIsr)(void* pPrivateData);

/**
 * DMA Controller.
 * This is in a struct to allow the stack to support
 * multiple cores of different types, 
 * since each may use a different type of DMA controller.
 * @field pPrivateData controller-private data;
 * not to be interpreted by the UCD
 * @field pfDmaStartController Start controller
 * @field pfDmaStopController Stop controller
 * @field pfDmaAllocateChannel Allocate a channel
 * @field pfDmaReleaseChannel Release a channel
 * @field pfDmaAllocateBuffer Allocate a buffer
 * @field pfDmaReleaseBuffer Release a buffer
 * @field pfDmaGetChannelStatus Get channel status
 * @field pfDmaControllerIsr DMA controller ISR; NULL if not needed
 */
typedef struct
{
    void* pPrivateData;
    MUSB_pfDmaStartController pfDmaStartController;
#ifndef MUSB_CODE_SHRINK    
    MUSB_pfDmaStopController pfDmaStopController;
#endif    
    MUSB_pfDmaAllocateChannel pfDmaAllocateChannel;
    MUSB_pfDmaReleaseChannel pfDmaReleaseChannel;
#ifdef MUSB_DMA_BUF 
    MUSB_pfDmaAllocateBuffer pfDmaAllocateBuffer;
    MUSB_pfDmaReleaseBuffer pfDmaReleaseBuffer;
#endif
    MUSB_pfDmaProgramChannel pfDmaProgramChannel;
    MUSB_pfDmaGetChannelStatus pfDmaGetChannelStatus;
    MUSB_pfDmaControllerIsr pfDmaControllerIsr;
} MUSB_DmaController;

/**
 * A DMA channel has new status.
 * This may be used to notify the UCD of channel status changes asynchronously.
 * This is useful if the DMA interrupt is different from the USB controller's
 * interrupt, so on some systems there may be no control over the order of
 * USB controller and DMA controller assertion.
 * @param pPrivateData the controller's private data pointer
 * @param bLocalEnd the local endpoint index (1-15)
 * @param bTransmit TRUE for transmit; FALSE for receive
 * @return TRUE if an IRP was completed as a result of this call;
 * FALSE otherwise
 */
typedef uint8_t (*MUSB_pfDmaChannelStatusChanged)(
    void* pPrivateData, uint8_t bLocalEnd, 
    uint8_t bTransmit);

/**
 * Instantiate a DMA controller.
 * Instantiate a software object representing a DMA controller.
 * @param pfDmaChannelStatusChanged channel status change notification function.
 * Normally, the UCD requests status in its interrupt handler.
 * For some DMA controllers, this may not be the correct time.
 * @param pDmaPrivate parameter for pfDmaChannelStatusChanged
 * @param pfSystemToBusAddress address translation function.
 * This may be used in implementations of AllocateBuffer to ensure
 * any bus region constraints are met.
 * @param pCoreBaseIsr the base address (in CPU space) of the core,
 * as required for ISRs (for use by the ControllerIsr call;
 * on most systems, this is a physical address).
 * It is assumed the DMA controller's base address will be related
 * to this in some way.
 * @param pCoreBaseBsr the base address (in CPU space) of the core,
 * as required in non-ISR code (for use by the ProgramChannel call;
 * on some systems, this is a user-virtual or kernel-virtual address).
 * It is assumed the DMA controller's base address will be related
 * to this in some way.
 * @return non-NULL pointer on success
 * @return NULL on failure (out of memory or exhausted 
 * a fixed number of controllers)
 */
typedef MUSB_DmaController* (*MUSB_pfNewDmaController)(
    MUSB_pfDmaChannelStatusChanged pfDmaChannelStatusChanged,
    void* pDmaPrivate,
    MUSB_pfSystemToBusAddress pfSystemToBusAddress,
    uint8_t* pCoreBaseIsr, 
    uint8_t* pCoreBaseBsr);

/**
 * Destroy DMA controller.
 * Destroy a previously-instantiated DMA controller.
 */
typedef void (*MUSB_pfDestroyDmaController)(MUSB_DmaController* pController);

/**
 * MUSB_DmaControllerFactory.
 * To allow for multi-core implementations and different
 * types of cores and DMA controllers to co-exist,
 * it is necessary to create them from factories.
 * @field pfNewDmaController create a DMA controller
 * @field pfDestroyDmaController destroy a DMA controller
 */
typedef struct
{
    MUSB_pfNewDmaController pfNewDmaController;
#ifndef MUSB_CODE_SHRINK    
    MUSB_pfDestroyDmaController pfDestroyDmaController;
#endif    
} MUSB_DmaControllerFactory;

/************************* DCI FUNCTIONS **************************/

/**
 * Set the DMA controller factory for use with all full-speed cores 
 * with the Mentor AHB bridge.
 * This must be called before MUSB_InitSystem.
 * @param pFactory pointer to a persistent DMA controller factory
 */
extern void MUSB_SetFsAhbDmaControllerFactory(MUSB_DmaControllerFactory* pFactory);

/**
 * Set the DMA controller factory for use with all high-speed cores.
 * This must be called before MUSB_InitSystem.
 * @param pFactory pointer to a persistent DMA controller factory
 */
extern void MUSB_SetHsDmaControllerFactory(MUSB_DmaControllerFactory* pFactory);

#endif	/* multiple inclusion protection */
