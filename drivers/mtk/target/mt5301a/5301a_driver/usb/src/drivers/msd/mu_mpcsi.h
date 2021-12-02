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
 * MUSBStack-S mass-storage Protocol/Command-Set Interface (PCSI)
 * $Revision: #1 $
 */

#ifndef __MUSB_MSD_PCSI_H__
#define __MUSB_MSD_PCSI_H__

#include "mu_cdi.h"
#include "mu_hfi_if.h"
#include "mu_bot.h"
#include "mu_list.h"

/**
 * Mass-storage Protocol/Command-Set Interface (MPCSI) introduction.
 * The MPCSI is the interface between the protocol (also called "transport")
 * and the command-set implementations.
 * In USB mass-storage devices, the protocol is given in the bInterfaceProtocol
 * field, and the command-set is given in the bInterfaceSubclass field.
 */

/************************** MPCSI TYPES ***************************/

/**
 * Command-complete callback.
 * Callback for command completion
 * @param pPrivateData private data
 * @param dwDataLength length of data
 * @param bUsbError TRUE if a USB error occurred while obtaining
 * the data and/or status (STALL, NAK limit, packet too short)
 * @param bWrapperStatus wrapper status byte from wrapper
 */
typedef void (*MGC_pfMsdProtocolCmdComplete)(void *pPrivateData, int32_t e_nfy_cond, void *pv_data);
/**
 * Start a mass-storage device.
 * Implementations should perform any protocol-specific device initialization,
 * and eventually call the command-set's discovery function.
 * @param pProtocolData protocol handler instance data
 * @param pDevice UCDI device to start
 * @return TRUE on success
 * @return FALSE on failure
 * @see #MGC_pfMsdCmdSetDiscoverDevice
 */
typedef uint8_t (*MGC_pfMsdProtocolStartDevice)(void* pProtocolData,
						const MUSB_Device* pDevice);

/**
 * Stop a mass-storage device.
 * Implementations should perform any protocol-specific device shutdown.
 * @param pProtocolData protocol handler instance data
 * @param pDevice UCDI device to stop
 * @return TRUE on success
 * @return FALSE on failure
 */
typedef uint8_t (*MGC_pfMsdProtocolStopDevice)(void* pProtocolData,
					       MUSB_Device* pDevice);

/**
 * Send a mass-storage command.
 * Using the appropriate transport, send a command to the device
 * and call the callback when data and status is received.
 * @param pProtocolData protocol handler instance data
 * @param pPrivateData caller private data; passed in completion callback
 * @param bLun LUN
 * @param pCmdBuffer a buffer containing the command
 * @param bCmdLength the length of the command buffer
 * @param pDataBuffer data buffer
 * @param dwDataLength the length of data to send or max expected to recv
 * @param bSend TRUE to send data; FALSE to receive
 * @param pfCmdComplete completion callback
 * @param bAllowDma TRUE to allow DMA; FALSE otherwise
 * @param bDelay delay, in milliseconds, before sending command (0 implies no delay)
 * @return TRUE on successful queueing of the command
 * @return FALSE on failure
 */
typedef uint8_t (*MGC_pfMsdProtocolSendCmd)(void* pProtocolData, 
					    void* pPrivateData,
					    MUSB_IO_DATA_T *pt_data,
					    uint8_t bLun,
					    const uint8_t* pCmdBuffer, 
					    uint8_t bCmdLength, 
					    uint8_t* pDataBuffer,
					    uint32_t dwDataLength,
					    uint8_t bSend, 
					    MGC_pfMsdProtocolCmdComplete pfCmdComplete);

/**
 * Set device information.
 * The implementation will set the fields required from the USB device
 * (vendor/device ID, product version, bus address).
 * @param pProtocolData protocol handler instance data
 * @param bLun LUN
 * @param pInfo pointer to device information; callee makes a copy
 */
typedef uint8_t (*MGC_pfMsdProtocolSetDeviceInfo)(void* pProtocolData,
						  uint8_t bLun,
						  const MUSB_HfiDeviceInfo* pInfo);

/**
 * Set medium information.
 * @param pProtocolData protocol handler instance data
 * @param bLun LUN
 * @param pInfo pointer to medium information; callee makes a copy
 */
typedef uint8_t (*MGC_pfMsdProtocolSetMediumInfo)(void* pProtocolData,
						  uint8_t bLun,
						  const MUSB_HfiMediumInfo* pInfo);

/**
 * Set readiness.
 * This should be called with bIsReady=TRUE only after
 * the device info has been set, and if the medium is removable,
 * after the medium information is available.
 * It should be called with bIsReady=FALSE if a removable medium
 * is removed, during formatting, and any other time the device
 * becomes unexpectedly unable to process commands.
 * @param pProtocolData protocol handler instance data
 * @param bLun LUN
 * @param bIsReady TRUE if medium is ready to accept a new command; 
 * FALSE otherwise
 */
typedef void (*MGC_pfMsdProtocolSetReady)(void* pProtocolData, 
					  uint8_t bLun,
					  uint8_t bIsReady);

/**
 * Set format progress.
 * @param pProtocolData protocol handler instance data
 * @param bLun LUN
 * @param bProgress format progress in percentage
 */
typedef void (*MGC_pfMsdProtocolSetFormatProgress)(void* pProtocolData, 
						   uint8_t bLun,
						   uint8_t bProgress);
/**
 * MGC_MsdProtocol.
 * A mass-storage protocol implementation (e.g. BOT, CB, CBI).
 */
typedef struct
{
    void* pProtocolData; /* MGC_MsdBotProtocol* pBot or MGC_MsdCbtProtocol* pCbt*/
    MGC_pfMsdProtocolStartDevice pfStartDevice;
    MGC_pfMsdProtocolStopDevice pfStopDevice;
    MGC_pfMsdProtocolSendCmd pfSendCmd;
    MGC_pfMsdProtocolSetDeviceInfo pfSetDeviceInfo;
    MGC_pfMsdProtocolSetMediumInfo pfSetMediumInfo;
    MGC_pfMsdProtocolSetReady pfSetReady;
    uint8_t bProtocol;    /*BOT, CBI, CB protocol*/
} MGC_MsdProtocol;

/**
 * Command-set-specific device discovery.
 * Using mechanisms specific to a device command set,
 * discover the device's capabilities.
 * It should accomplish this using the protocol's pfSendCmd,
 * and eventually result in a call to the protocol's pfSetDeviceInfo.
 * For example, a SCSI command set implementation would need to
 * issue commands like INQUIRY, READ_CAPACITY and/or 
 * READ_FORMAT_CAPACITY, and parse the results.
 * @param pInstance instance pointer
 * @param pProtocol pointer to protocol handler
 * @param bLun LUN
 * @see #MGC_pfMsdProtocolSendCmd
 * @see #MGC_pfMsdProtocolSetDeviceInfo
 */
typedef uint8_t (*MGC_pfMsdCmdSetDiscoverDevice)(void* pInstance, 
						 MGC_MsdProtocol* pProtocol,
						 uint8_t bLun);

/**
 * Mount completion callback.
 * @param pPrivateData protocol handler private data
 * @param bSuccess TRUE on success; FALSE on failure
 */
typedef void (*MGC_pfMsdMountComplete)(void* pPrivateData, uint8_t bSuccess);

/**
 * Command-set-specific mount handling.
 * Using mechanisms specific to a device command set,
 * perform any operations prudent when a device is mounted
 * (e.g. disable manual eject).
 * If a command needs to be sent, it should first call
 * the protocol's pfSetReady(FALSE) to prevent queuing further
 * commands, then use pfSendCmd, then eventually call pfSetReady(TRUE) 
 * if all goes well.
 * @param pInstance instance pointer
 * @param pfMountComplete mount completion callback
 * @param pPrivateData passed to pfMountComplete
 * @param pProtocol pointer to protocol handler
 * @param bLun LUN
 * @see #MGC_pfMsdProtocolSendCmd
 * @see #MGC_pfMsdProtocolSetReady
 */
typedef uint8_t (*MGC_pfMsdCmdSetMountDevice)(void* pInstance, 
					      MGC_pfMsdMountComplete pfMountComplete,
					      void* pPrivateData,
					      MGC_MsdProtocol* pProtocol,
					      uint8_t bLun);

/**
 * Command-set-specific unmount handling.
 * Using mechanisms specific to a device command set,
 * perform any operations prudent when a device is unmounted
 * (e.g. enable manual eject).
 * If a command needs to be sent, it should first call
 * the protocol's pfSetReady(FALSE) to prevent queuing further
 * commands, then use pfSendCmd, then eventually call pfSetReady(TRUE) 
 * if all goes well.
 * @param pInstance instance pointer
 * @param pProtocol pointer to protocol handler
 * @param bLun LUN
 * @see #MGC_pfMsdProtocolSendCmd
 * @see #MGC_pfMsdProtocolSetReady
 */
typedef uint8_t (*MGC_pfMsdCmdSetUnmountDevice)(void* pInstance, 
						MGC_MsdProtocol* pProtocol,
						uint8_t bLun);

/**
 * Command-set-specific read command generation.
 * Fill the given buffer with a command suitable to perform
 * a read under the given conditions.
 * @param pCmdBuffer buffer in which to store the command
 * @param bMaxCmdLength buffer length
 * @param dwBlockLo lower 32 bits of starting block
 * @param dwBlockHi upper 32 bits of starting block
 * @param wBlockCount how many blocks to read
 * @param bLun LUN
 * @return command length (the relevant portion of the command buffer)
 */
typedef uint8_t (*MGC_pfMsdCmdSetGetReadCmd)(uint8_t* pCmdBuffer,
					     uint8_t bMaxCmdLength,
					     uint32_t dwBlockLo,
					     uint32_t dwBlockHi,
					     uint16_t wBlockCount,
					     uint8_t bLun);

/**
 * Command-set-specific write command generation.
 * Fill the given buffer with a command suitable to perform
 * a write under the given conditions.
 * @param pCmdBuffer buffer in which to store the command
 * @param bMaxCmdLength buffer length
 * @param dwBlockLo lower 32 bits of starting block
 * @param dwBlockHi upper 32 bits of starting block
 * @param wBlockCount how many blocks to write
 * @param bLun LUN
 * @return command length (the relevant portion of the command buffer)
 */
typedef uint8_t (*MGC_pfMsdCmdSetGetWriteCmd)(uint8_t* pCmdBuffer,
					      uint8_t bMaxCmdLength,
					      uint32_t dwBlockLo,
					      uint32_t dwBlockHi,
					      uint16_t wBlockCount,
					      uint8_t bLun);

/**
 * Command-set-specific device flush.
 * Using mechanisms specific to a device command set,
 * cause the device to flush its cache (if any).
 * If a command needs to be sent, it should first call
 * the protocol's pfSetReady(FALSE) to prevent queuing further
 * commands, then use pfSendCmd, then eventually call pfSetReady(TRUE) 
 * if all goes well.
 * @param pInstance instance pointer
 * @param pProtocol pointer to protocol handler
 * @param bLun LUN
 * @see #MGC_pfMsdProtocolSendCmd
 * @see #MGC_pfMsdProtocolSetReady
 */
typedef uint8_t (*MGC_pfMsdCmdSetFlushDevice)(void* pInstance, 
					      MGC_MsdProtocol* pProtocol,
					      uint8_t bLun);

/**
 * Command-set-specific device format.
 * Using mechanisms specific to a device command set,
 * cause the device to initiate a low-level format (if supported).
 * If a command needs to be sent, it should first call
 * the protocol's pfSetReady(FALSE) to prevent queuing further
 * commands, then use pfSendCmd, then eventually call pfSetReady(TRUE) 
 * if all goes well.
 * @param pInstance instance pointer
 * @param pProtocol pointer to protocol handler
 * @param dwBlockSize block size to use
 * @param bLun LUN
 * @see #MGC_pfMsdProtocolSendCmd
 * @see #MGC_pfMsdProtocolSetReady
 */
typedef uint8_t (*MGC_pfMsdCmdSetFormatDevice)(void* pInstance, 
					       MGC_MsdProtocol* pProtocol,
					       uint32_t dwBlockSize,
					       uint8_t bLun);

/**
 * Command-set-specific abort format.
 * @param pInstance instance pointer
 * @param pProtocol pointer to protocol handler
 * @param bLun LUN
 * @see #MGC_pfMsdProtocolSendCmd
 * @see #MGC_pfMsdProtocolSetReady
 */
typedef uint8_t (*MGC_pfMsdCmdSetAbortFormat)(void* pInstance, 
					      MGC_MsdProtocol* pProtocol,
					      uint8_t bLun);

/**
 * Command-set-specific interrupt parsing (CBIT only).
 */
typedef void (*MGC_pfMsdCmdSetParseInterrupt)(void* pInstance, 
					      MGC_MsdProtocol* pProtocol,
					      const uint8_t* pMessage,
					      uint16_t wMessageLength,
					      uint8_t bLun);

/**
 * Command-set-specific check medium presence.
 * @param pInstance instance pointer
 * @param pProtocol pointer to protocol handler
 * @param bLun LUN
 */
/*
typedef uint8_t (*MGC_pfMsdCmdSetCheckMedium)(void* pInstance, 
					      MGC_MsdProtocol* pProtocol,
					      uint8_t bLun);
*/
typedef uint8_t (*MGC_pfMsdCmdSetCheckMedium)(const MGC_MsdProtocol* pProtocol, uint32_t dwCheckTime);

/**
 * Create a new command-set instance
 * @param bLunCount number of LUNs in device
 */
typedef void* (*MGC_pfMsdCmdSetCreateInstance)(uint8_t bLunCount);

/**
 * Destroy a command-set instance
 */
typedef void (*MGC_pfMsdCmdSetDestroyInstance)(void* pInstance);

/**
 * MGC_MsdCmdSet.
 * A mass-storage command set.
 * This allows the transport layer to use any supported command set
 * (e.g. SCSI emulation, RBC, MMC-2, etc.)
 */
typedef struct
{
    MGC_pfMsdCmdSetCreateInstance pfCreateInstance;
    MGC_pfMsdCmdSetDestroyInstance pfDestroyInstance;
    MGC_pfMsdCmdSetDiscoverDevice pfDiscoverDevice;
} MGC_MsdCmdSet;

/****************************** CONSTANTS ********************************/

typedef enum
{
    MGC_MSD_BOT_RESET_STAGE_NONE,
    MGC_MSD_BOT_RESET_STAGE_CMD,
    MGC_MSD_BOT_RESET_STAGE_CLEARIN,
    MGC_MSD_BOT_RESET_STAGE_CLEAROUT
} MGC_MsdBotResetStage;

typedef enum
{
    MGC_MSD_BOT_TRANSFER_STAGE_NONE,
    MGC_MSD_BOT_TRANSFER_STAGE_CBW,
    MGC_MSD_BOT_TRANSFER_STAGE_INDATA,
    MGC_MSD_BOT_TRANSFER_STAGE_OUTDATA,
    MGC_MSD_BOT_TRANSFER_STAGE_CSW
} MGC_MsdBotTransferStage;

#define MGC_BOT_CBW_TAG_INIT_VALUE   1

//  MTK Notice: Max Liao, 2007/08/30.
//  Command abort to handle tx/rx error.
#define MGC_MSD_COMMAND_ABORT      (uint32_t)0x03
//  Device remove.
#define MGC_MSD_DEV_REMOVE            (uint32_t)0x04

/******************************** TYPES **********************************/
struct _MGC_MsdBotVolume;

struct _MGC_MsdBotIrp;

/**
 * BOT procotol data.
 * @field Protocol PCSI data
 * @field ControlIrp IRP for control transfers (GetMaxLUN and Reset)
 * @field aSetup setup data for control transfers
 * @field aData receive data from control transfers
 * @field pCmdSet command set to use
 * @field dwTag CBW tag
 * @field hInPipe bulk-in pipe handle
 * @field hOutPipe bulk-out pipe handle
 * @field Csw CSW (keep here since only one command can be outstanding)
 * @field CswIrp IRP for CSW
 * @field pCurrentBotIrp pointer to bot Irp currently being serviced
 * @field aVolume array of volume data
 * @field bResetStage what stage of reset (if any) we are in
 * @field bInit TRUE during init
 * @field bLunCount number of LUNs
 */
typedef struct
{
    MGC_MsdProtocol Protocol;
    const void* pSelf;                  /* MGC_MsdBotProtocol* pBot */
    MUSB_ControlIrp ControlIrp;
    uint8_t aSetup[8];
    uint8_t aData[4];
    MUSB_Device* pDevice;
    MUSB_BusHandle hBus;         /* MGC_Port* pPort */
    MUSB_BusHandle hBusServices;         /* MUSB_SystemServices *pServices */
    const MUSB_InterfaceDescriptor* pIfaceDesc;
    const MUSB_EndpointDescriptor* pInEnd;
    const MUSB_EndpointDescriptor* pOutEnd;
    MGC_MsdCmdSet* pCmdSet;
    uint32_t dwTag;
    uint32_t dwMaxTransferSize;        
    MUSB_Pipe hInPipe;
    MUSB_Pipe hOutPipe;
    struct _MGC_MsdBotIrp* pCurrentBotIrp;
    struct _MGC_MsdBotVolume* aVolume;
    MGC_MsdBotResetStage bResetStage;
    MUSB_LinkedList BotList;
    struct _MGC_MsdBotIrp* pBotIrpBuf;
    
    uint8_t bInit;
    uint8_t bLunCount;

    uint8_t bBotRetry;
    uint8_t bBotCmdRetry;
    uint8_t bCswRetry;
    uint8_t bBotDataNakReset;    
} MGC_MsdBotProtocol;

/**
 * BOT volume data.
 * A volume (logical unit).
 * @field pBot BOT data pointer
 * @field pCmdSetInstance instance of command set for volume
 * @field pfCmdComplete PCSI callback
 * @field pCompleteParam parameter for PCSI callback
 * @field Cbw CBW for volume
 * @field CbwIrp IRP for CBW
 * @field DataIrp IRP for data
 * @field DeviceInfo HFI device info
 * @field Device HFI device (dispatch table)
 * @field pfTransferComplete HFI transfer completion callback
 * @field hVolume HFI volume handle (callback parameter)
 * @field bLun LUN
 * @field bIsReady TRUE if unit is ready; FALSE otherwise
 * @field bBusy TRUE if volume command in progress
 * @field bError TRUE if error on last/current command
 * @field bFormatProgress current format progress
 */
typedef struct _MGC_MsdBotVolume
{
    MGC_MsdBotProtocol* pBot;
    void* pCmdSetInstance;  /* MGC_ScsiCmdSetData* pScsi */
    MUSB_HfiDeviceInfo DeviceInfo;
    MUSB_HfiMediumInfo MediumInfo;
    MUSB_HfiDevice Device;
    MUSB_pfHfiTransferComplete pfHfiTransferComplete;
    MUSB_HfiVolumeHandle hVolume;
    uint8_t bLun;
    uint8_t bMediumAnnounced;
} MGC_MsdBotVolume;


typedef struct _MGC_MsdBotIrp
{
    MGC_MsdBotProtocol* pBot;
    void* pCmdSetInstance;  /* MGC_ScsiCmdSetData* pScsi */
    MGC_pfMsdProtocolCmdComplete pfCmdComplete;
    void* pCompleteParam;
    MUSB_IO_DATA_T *pt_data;
    uint32_t dwRequestLength;        /* Set user's request data length.*/
    uint32_t dwGrantLength;     /* Collect by mbot driver.*/
    MGC_MsdCbw Cbw;
    MGC_MsdCsw Csw;
    MUSB_Irp CbwIrp;
    MUSB_Irp DataIrp;
    MUSB_Irp CswIrp;          
}MGC_MsdBotIrp;

struct _MGC_MsdCbtVolume;

struct _MGC_MsdCbtIrp;

/**
 * CBT procotol data.
 * @field Protocol PCSI data
 * @field ControlIrp IRP for control transfers (GetMaxLUN and Reset)
 * @field aSetup setup data for control transfers
 * @field aData receive data from control transfers
 * @field pCmdSet command set to use
 * @field dwTag CBW tag
 * @field hInPipe bulk-in pipe handle
 * @field hOutPipe bulk-out pipe handle
 * @field pCurrentBotIrp pointer to bot Irp currently being serviced
 * @field aVolume array of volume data
 * @field bResetStage what stage of reset (if any) we are in
 * @field bInit TRUE during init
 * @field bLunCount number of LUNs
 */
typedef struct
{
    MGC_MsdProtocol Protocol;
    const void* pSelf;                  /* MGC_MsdCbtProtocol* pCbt */
    MUSB_ControlIrp ControlIrp;
    uint8_t aSetup[8];
    MUSB_Device* pDevice;
    MUSB_BusHandle hBus;         /* MGC_Port* pPort */
    MUSB_BusHandle hBusServices;         /* MUSB_SystemServices *pServices */
    const MUSB_InterfaceDescriptor* pIfaceDesc;
    const MUSB_EndpointDescriptor* pInEnd;
    const MUSB_EndpointDescriptor* pOutEnd;
    MGC_MsdCmdSet* pCmdSet;
    MUSB_Pipe hInPipe;
    MUSB_Pipe hOutPipe;
    struct _MGC_MsdCbtIrp* pCurrentCbtIrp;
    struct _MGC_MsdCbtVolume* aVolume;
    MGC_MsdBotResetStage bResetStage;
    MUSB_LinkedList CbtList;
    struct _MGC_MsdCbtIrp* pCbtIrpBuf;
    
    uint8_t bLunCount;

    uint8_t bCbtRetry;
    uint8_t bCbtCmdRetry;
} MGC_MsdCbtProtocol;

/**
 * CBT volume data.
 * A volume (logical unit).
 * @field pCbt CBT data pointer
 * @field pCmdSetInstance instance of command set for volume
 * @field pfCmdComplete PCSI callback
 * @field pCompleteParam parameter for PCSI callback
 * @field DeviceInfo HFI device info
 * @field Device HFI device (dispatch table)
 * @field pfTransferComplete HFI transfer completion callback
 * @field hVolume HFI volume handle (callback parameter)
 * @field bLun LUN
 * @field bIsReady TRUE if unit is ready; FALSE otherwise
 * @field bBusy TRUE if volume command in progress
 * @field bError TRUE if error on last/current command
 * @field bFormatProgress current format progress
 */
typedef struct _MGC_MsdCbtVolume
{
    MGC_MsdCbtProtocol* pCbt;
    void* pCmdSetInstance;  /* MGC_ScsiCmdSetData* pScsi */
    MUSB_HfiDeviceInfo DeviceInfo;
    MUSB_HfiMediumInfo MediumInfo;
    MUSB_HfiDevice Device;
    MUSB_pfHfiTransferComplete pfHfiTransferComplete;
    MUSB_HfiVolumeHandle hVolume;
    uint8_t bLun;
    uint8_t bMediumAnnounced;
} MGC_MsdCbtVolume;

typedef struct _MGC_MsdCbtIrp
{
    MGC_MsdCbtProtocol* pCbt;
    void* pCmdSetInstance;  /* MGC_ScsiCmdSetData* pScsi */
    MGC_pfMsdProtocolCmdComplete pfCmdComplete;
    void* pCompleteParam;
    MUSB_IO_DATA_T *pt_data;
    uint32_t dwRequestLength;        /* Set user's request data length.*/
    uint32_t dwGrantLength;     /* Collect by mbot driver.*/    
    MUSB_ControlIrp ControlIrp;
    uint8_t aControlCb[20];
    MUSB_Irp DataIrp;
} MGC_MsdCbtIrp;

typedef enum
{
    MGC_SCSI_STATE_NONE,
    /* device discovery sequence: */
    MGC_SCSI_STATE_INQUIRY,
    MGC_SCSI_STATE_INQUIRY_OK,    
    MGC_SCSI_STATE_VPD_SERIALNUMBER,
    MGC_SCSI_STATE_READ_FMT_CAPACITY,
    MGC_SCSI_STATE_READ_CAPACITY,
    MGC_SCSI_STATE_READ_CAPACITY_OK,
    MGC_SCSI_STATE_MODE_SENSE10,
    MGC_SCSI_STATE_MODE_SENSE6,
    MGC_SCSI_STATE_MODE_SENSE_OK,
    MGC_SCSI_STATE_MODE_SENSE_EXCEPTIONS,
    MGC_SCSI_STATE_MODE_SENSE_ALL,
    MGC_SCSI_STATE_TEST_UNIT_READY,
    MGC_SCSI_STATE_TEST_MEDIUM_INSERT,
    MGC_SCSI_STATE_TEST_MEDIUM_REMOVE,
    /* mount sequence: */
    MGC_SCSI_STATE_PREVENT_MEDIUM_REMOVE,
    /* unmount sequence: */
    MGC_SCSI_STATE_ALLOW_MEDIUM_REMOVE,
    MGC_SCSI_STATE_STOP_UNIT
}MGC_ScsiState;

typedef struct
{
    MGC_MsdProtocol *pProtocol;
    uint8_t aCmd[16];
    uint8_t aInquiryData[37];
    uint8_t aSenseData[18];
    uint8_t aCapacityData[8];
    uint8_t aModeSenseData[0xC0];
    uint32_t dwRetries;
    MGC_ScsiState bState;
    uint8_t bError;
    uint8_t bRemovable;
    uint8_t bLun;
    uint8_t bLunCount;
    uint8_t bNoMedium;
    uint8_t bModeSenseType; /*MGC_SCSI_MODE_SENSE10 or MGC_SCSI_MODE_SENSE*/
} MGC_ScsiCmdSetData;

#endif	/* multiple inclusion protection */
