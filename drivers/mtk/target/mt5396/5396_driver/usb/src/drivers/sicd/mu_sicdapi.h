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

#ifndef _MUSB_SICD_API_H_
#define _MUSB_SICD_API_H_

/*
 * Prototypes for SICD class driver API
 * $Revision: #1 $
 */

#include "mu_sicderrorcode.h"
#include "mu_sicd.h"

#define MUSB_SICD_HEADER_MINIMUM_LENGTH     12

#define MUSB_SICD_MAX_STATE_FLOW_COUNT      4
#define MUSB_SICD_MAX_CMD_RESPONSE_LENGTH   32
#define MUSB_SICD_MAX_CMD_PARM_LENGTH \
    ((MUSB_SICD_MAX_CMD_RESPONSE_LENGTH) - (MUSB_SICD_HEADER_MINIMUM_LENGTH))
#define MUSB_SICD_MAX_EVENT_BUFFER_LENGTH   32
#define MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH 64
#define MUSB_SICD_MAX_OPERATION_RETRY_COUNT 10  // retry count for get device status or
                                                //                 auto retry when RX ack timeout

#define MUSB_SICD_LIST_NUM (9)

// SICD operation code.
#define MUSB_SICD_GET_DEVICE_INFO     (0x1001)
#define MUSB_SICD_OPEN_SESSION          (0x1002)
#define MUSB_SICD_CLOSE_SESSION          (0x1003)
#define MUSB_SICD_GET_OBJ                       (0x1009)
#define MUSB_SICD_GET_PARTIAL_OBJ     (0x101B)

#define MUSB_SICD_DATA_DEFAULT_SIZE (0xFFFFFFFF)
/**
 * MUSB_SicdDriverState
 *  Sicd Driver State Machine
 */
typedef enum
{
    MUSB_SICD_DRIVER_STATE_UNKNOWN,
    MUSB_SICD_DRIVER_STATE_READY,
    MUSB_SICD_DRIVER_STATE_RUNNING,
    MUSB_SICD_DRIVER_STATE_STOPPING
}MUSB_SicdDriverState;

/**
 * MUSB_SicdOperationType
 *  Sicd Operation Type
 */
typedef enum
{
    MUSB_SICD_NO_OPERATION,
    MUSB_SICD_STANDARD_OPERATION,
    MUSB_SICD_CMD_OPERATION,
    MUSB_SICD_DATA_OPERATION,
    MUSB_SICD_RESPONSE_OPERATION,
    MUSB_SICD_NOTIFY_OPERATION
}MUSB_SicdOperationType;

/**
 * MUSB_SicdOperationCode
 *  Sicd Operation Code
 */
typedef enum
{
    MUSB_SICD_TRANSFER_OPERATION,
    MUSB_SICD_GET_DEVICE_STATUS,
    MUSB_SICD_ABORT_REQUEST,
    MUSB_SICD_RESET_REQUEST,
    MUSB_SICD_CLEAR_FEATURE_REQUEST,
    MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST,

    // for devices not support GetPartialObject
    MUSB_SICD_DATAPHASE_CTRL_BY_MW,
    MUSB_SICD_DATAPHASE_CTRL_BY_SICD,
    MUSB_SICD_GET_NEXT_DATA,

    // for send object operation
    MUSB_SICD_SEND_NEXT_DATA,

    // for devices not support reset
    MUSB_SICD_SET_SILENT_BUS_RESET, 
    
    MUSB_SICD_RESET_OP_EVT,
    MUSB_SICD_ABORT_OP_EVT, 
    MUSB_SICD_GET_EXT_EVENT_OP_EVT
}MUSB_SicdOperationCode;

/**
 * MUSB_SicdInitQuery
 *  Sicd init query state.
 */
typedef enum
{
    MUSB_SICD_INIT_UNKNOW,
    MUSB_SICD_INIT_OPEN_SESSION,
    MUSB_SICD_INIT_DEVINFO,
    MUSB_SICD_INIT_CLOSE_SESSION,
 
    MUSB_SICD_INIT_END
}MUSB_SicdInitQuery;

/**
 * MUSB_SicdStateData.
 * Still Image Class Driver Packet Container
 *
 * @field eOpeartionType Operation Type
 * @field eOpeartionCode Operation Code
 */
typedef struct
{   
    MUSB_SicdOperationType   eOpeartionType;
    MUSB_SicdOperationCode   eOpeartionCode;
}MUSB_SicdStateData;

/**
 * MUSB_SICD_BLOCK_T.
 * Command, Data, Response, Event Block Structure
 */
typedef struct
{
    MUSB_SICD_DATA_TYPE_E eDataType;
    MUSB_SICD_DATA_DIRECTION_E eDataDirection;
    uint16_t wOperationCode;
    uint32_t dwPayLoadBufLen;
    uint32_t dwTranscationID;
    uint8_t* pbPayLoadBuf;
} MUSB_SICD_BLOCK_T;

/**
 * MUSB_SICD_BLOCK_T.
 * Extended Event Block Structure
 */
typedef struct
{
    uint16_t wEventCode;
    uint32_t dwTransactionId;
    uint16_t wNumOfParameters;
    uint8_t* pbPayloadBuf;
}MUSB_SICD_EXT_EVENT_BLOCK_T;

/**
 * MUSB_SicdCommand.
 * Still Image Class Driver Command Structure
 *
 * @field rCommandBlock Command block
 * @field rDataBlock Data block
 * @field rResponseBlock Response block
 * @field rEventBlock Event block
 * @field rExtEventBlock Extended event block
 * @field abGenericTransferBuffer Used when transfering command and response block. (and get extended event)
 * @field abEventBuffer Used when transfering event block
 * @field abTempOutBuffer Use this buffer to contain the 12 byte SICD data phase header and the first packet of data phase.
 * @field bContinueDataPhaseWithoutCbk
 * @field pbBackupPtr
 * @field dwBackupSize  This is used and meaningful only when SICD state controlled by SICD not by MW.
 *          Due to performance improvement, we will update buf size of first packet to 512 bytes, this var is used to recover.
 */
typedef struct 
{
    MUSB_SICD_BLOCK_T   rCommandBlock;
    MUSB_SICD_BLOCK_T   rDataBlock;
    MUSB_SICD_BLOCK_T   rResponseBlock;
    MUSB_SICD_BLOCK_T   rEventBlock;
    MUSB_SICD_EXT_EVENT_BLOCK_T rExtEventBlock;
    uint8_t             abGenericTransferBuffer[MUSB_SICD_MAX_CMD_RESPONSE_LENGTH];
    uint8_t             abEventBuffer[MUSB_SICD_MAX_EVENT_BUFFER_LENGTH];
    uint8_t             abTempOutBuffer[MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH];
    uint8_t             bContinueDataPhaseWithoutCbk;
    uint8_t*            pbBackupPtr;      /* handle 12 bytes SICD container header in data phase. */
    uint32_t            dwBackupSize;   /* handle 12 bytes SICD container header in data phase. */
}MUSB_SicdCommand;

/**
 * MUSB_SicdStateControl.
 * Still Image Class Driver State Control Structure
 *
 * @field bCurStateIndex Current State Index
 * @field bTotalStateCount Total State Count
 * @field bDataPhaseControlledByMiddleware 0:controlled by SICD, else: by Middleware
 * @field StateFlow The whole states we want to execute
 * @field SicdCommand Sicd command
 * @field dwDataPhaseTotalDataSize The total data size which will be in the data phase
 * @field dwDataPhaseCurrentReadOffset The total size we have read in data phase
 * @field BulkIrp Command, Data, Response phase Irp.
 * @field IntrIrp Interrupt Irp
 */
typedef struct 
{
    uint8_t     bCurStateIndex;
    uint8_t     bTotalStateCount;
    uint8_t     bDataPhaseControlledByMiddleware;
    MUSB_SicdDriverState eDriverState;    
    MUSB_SicdStateData StateFlow[MUSB_SICD_MAX_STATE_FLOW_COUNT];
    MUSB_SicdCommand SicdCommand;

    uint32_t    dwRetryCount;  
    uint32_t    dwDataPhaseTotalDataSize;   // IN: assigned in data phase.  OUT: assigned in MUSB_SicdReadDevice()
    uint32_t    dwDataPhaseCurrentReadOffset;
    uint32_t    dwDataPhaseCurrentWriteOffset;
    
    MUSB_Irp  BulkIrp;
#if MGC_SICD_OPEN_INTERRUPT_PIPE    
    MUSB_Irp  IntrIrp;
#endif
}MUSB_SicdStateControl;

/**
 * MUSB_MS_OS_DES_DEV_TYPE.
 * Specify the device type identified by Microsoft OS Descriptor.
 */
typedef enum
{
    MUSB_MS_OS_UNKNOWN,
    MUSB_MS_OS_DES_MTP,
    MUSB_MS_OS_DES_PTP
}MUSB_MS_OS_DES_DEV_TYPE;

/**
 *  This is just to avoid cylic structure problem.
 */
struct _MUSB_SicdVolume;

/**
 * MUSB_SicdProtocol.
 * Still Image Class Driver Protocol Data
 *
 * @field bVendorCode The vendor code retrieved during enumeration
 * @field aData The temp buffer used when getting Microsoft OS Descriptor
 * @field bDeviceType MTP or PTP or Unknown
 * @field aVolume Sicd Volume data
 * @field SicdStateControl Sicd state machine data
 * @field pDevice MUSB_Device 
 * @field hBus Bus handle
 * @field hBusServices Bus service
 * @field pIfaceDesc Interface descriptor
 * @field pInEnd Endpoint for Bulk-In pipe
 * @field pOutEnd Endpoint for Bulk-Out pipe
 * @field pIntrEnd Endpoint for Interrupt pipe
 * @field hInPipe Sicd Bulk-In pipe
 * @field hOutPipe Sicd Bulk-out pipe
 * @field hIntrPipe Sicd Interrupt pipe
 */
typedef struct
{
    struct _MUSB_SicdVolume *pVolume;
    MUSB_SicdStateControl SicdStateControl;    
    MUSB_Device* pDevice;
    MUSB_Pipe hInPipe;
    MUSB_Pipe hOutPipe;
#if MGC_SICD_OPEN_INTERRUPT_PIPE        
    MUSB_Pipe hIntrPipe;
#endif
    MUSB_NFY_INFO_T SicdNfyInfo;   /* user's notify function. */
    MUSB_SICD_IO_DATA_T SicdData;   
    MUSB_SicdInitQuery eSicdInitState;
    uint8_t *pSicdDevInfo;     /* Point to device information data structure. */
    uint8_t bIsGetPartialObj;
}MUSB_SicdProtocol;

/**
 * MUSB_SicdVolume.
 * Still Image Class Driver Volume Data
 *
 * @field pSicdProtocol Sicd protocol data pointer
 * @field DeviceInfo Hfi device info
 * @field Device Function pointers exported to Hfi (Host Filesystem Interface)
 * @field hVolume To keep the pointer of pUcFsDevice used for SICD
 * @field bIsReady TRUE after MUSB_HfiSicdAddDevice be invoked
 * @field bAnnounced Has already announced to Hfi or not.
 */
typedef struct _MUSB_SicdVolume
{
    MUSB_SicdProtocol* pSicdProtocol;
    MUSB_HfiDeviceInfo DeviceInfo;
    MUSB_HfiSicdDevice Device;
    MUSB_HfiVolumeHandle hVolume;
    uint8_t bAnnounced;
}MUSB_SicdVolume;

/**
 * MUSB_SicdDevice.
 * Still Image Class Driver Device Data
 *
 * @field bBusAddress
 * @field bInterfaceNumber
 * @field bAlternateSetting Not used for sicd
 * @field bProtocol Protocol id retrieved during enumeration
 * @field bSubclassb Subclass id retrieved during enumeration
 * @field hBus Bus handle
 * @field pUsbDevice Usb device pointer
 * @field pDriver Callback function pointers exported to UCD. (Usb Controller Driver)
 * @field pDeviceId No Used
 * @field Setup Usb request temp buffer.
 * @field ControlIrp
 * @field pBulkInEndpoint
 * @field pBulkOutEndpoint
 * @field pIntrInEndpoint
 * @field hVolume To keep the pointer of pUcFsDevice used for SICD
 * @field pProtocol Sicd protocol data pointer
 */
typedef struct 
{
    MUSB_BusHandle                  hBus;   /* MGC_Port* pImplPort */
    MUSB_Device                    *pUsbDevice;
    MUSB_DeviceDriver              *pDriver;
    const MUSB_InterfaceDescriptor *pIface;
    const MUSB_EndpointDescriptor *pInEnd;
    const MUSB_EndpointDescriptor *pOutEnd;
    const MUSB_EndpointDescriptor *pIntrEnd;
    MUSB_SicdProtocol*              pProtocol;
    MUSB_DeviceRequest              Setup;
    MUSB_ControlIrp                 ControlIrp;
    uint8_t aMODS[32];          /* Microsoft OS descriptor string */
    uint8_t bInterfaceNumber;
    uint8_t bVendorCode;
    uint8_t bDeviceType;    // decided by MS OS Descriptor
    
} MUSB_SicdDevice;

/**
 * Fill an array with the targetted peripheral list entry appropriate
 * for the SICD class driver, ending with the MUSB_TARGET_ACCEPT.
 * @param pList array
 * @param wListLength how many bytes are available in the array
 * @return how many bytes were filled in the array.
 * If this equals bListLength, the caller should assume there is insufficient space
 * in the array and the list entry is incomplete.
 */
extern uint16_t MUSB_FillSicdPeripheralList(uint8_t *pList, uint16_t wListLength, uint8_t bIndex);

/**
 * Get a pointer to the SICD driver
 */
extern MUSB_DeviceDriver* MUSB_GetSicdDriver(void);

#endif /* _MUSB_MSD_API_H_ */
