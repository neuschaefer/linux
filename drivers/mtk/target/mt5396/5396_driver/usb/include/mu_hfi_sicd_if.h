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
 * MUSBStack-S HFI (Host Filesystem Interface) for SICD.
 * $Revision: #1 $
 */

#ifndef __MUSB_SICD_HFI_H__
#define __MUSB_SICD_HFI_H__

#include "mu_hfi_if.h"

#ifndef MUSB_SICD_UNIT_COUNT
#define MUSB_SICD_UNIT_COUNT (4)
#endif

// HFI Sicd Burst Command Type
typedef enum
{
    MUSB_SICD_RW_SEND_OPERATION,
    MUSB_SICD_RW_GET_EXTENDED_EVENT_DATA,
    MUSB_SICD_RW_SET_DEVICE_RESET,
    MUSB_SICD_RW_SET_OPERATION_ABORT,

    // for devices not support GetPartialObject
    MUSB_SICD_RW_GET_NEXT_DATA,

    // for SendObject Operation
    MUSB_SICD_RW_SEND_NEXT_DATA,

    // for unexpected error handling
    MUSB_SICD_RW_SET_SILENT_BUS_RESET       // used by CLI only, not by Middleware.

}MUSB_HfiSicdBurstCmdType_E;

// HFI Sicd IOCTL Command Type
typedef enum
{
    MUSB_SICD_IOCTL_GET_ALIGNMENT,
    MUSB_SICD_IOCTL_GET_DEVICE_STATUS,
    MUSB_SICD_IOCTL_GET_SOCKET_NUM,
    MUSB_SICD_IOCTL_GET_DEVICE_INFO,

    MUSB_SICD_IOCTL_SET_TYPENFY,
    MUSB_SICD_IOCTL_SET_HOTSWAP_NFY,

    // for devices not support GetPartialObject
    MUSB_SICD_IOCTL_DP_CTRL_BY_MW,
    MUSB_SICD_IOCTL_DP_CTRL_BY_SICD,

    MUSB_SICD_IOCTL_END
} MUSB_HFI_SICD_IOCTL_CMD_TYPE_E;


typedef enum
{
    MUSB_HFI_NFY_PHASE_CMD_COMPLETE = MUSB_HFI_COND_WRITE_OK+1, 
    MUSB_HFI_NFY_PHASE_DATA_COMPLETE, 
    MUSB_HFI_NFY_PHASE_RESP_COMPLETE,
    MUSB_HFI_NFY_GOT_EVENT, 
    MUSB_HFI_NFY_GET_EXT_EVENT_COMPLETE, 
    MUSB_HFI_NFY_OPERATION_ERROR, 
    MUSB_HFI_NFY_RESET_SUCCESS, 
    MUSB_HFI_NFY_RESET_FAIL, 
    MUSB_HFI_NFY_OPERATION_ABORT_SUCCESS, 
    MUSB_HFI_NFY_OPERATION_ABORT_FAIL, 
    MUSB_HFI_NFY_SKIP_COMPLETE, 
    MUSB_HFI_NFY_SILENT_BUS_RESET
}MUSB_HFI_NFY_TYPE_E;


typedef enum
{
    MUSB_SICD_PHASE_COMMAND,
    MUSB_SICD_PHASE_DATA,
    MUSB_SICD_PHASE_RESPONSE
}MUSB_HFI_SICD_PHASE_E;


typedef enum{
    MUSB_SICD_UNDEFINE,
    MUSB_SICD_CMD_BLOCK,
    MUSB_SICD_DATA_BLOCK,
    MUSB_SICD_RESPONSE_BLOCK, 
    MUSB_SICD_EVENT_BLOCK, 
    MUSB_SICD_RESERVE
}MUSB_SICD_DATA_TYPE_E;


typedef enum{
    MUSB_SICD_DATA_OUT,
    MUSB_SICD_DATA_IN,
    MUSB_SICD_NO_DATA
}MUSB_SICD_DATA_DIRECTION_E;


typedef struct{
    uint16_t wEventCode;
    uint32_t dwTranscationID;  
    uint32_t dwEventParam[3];
}MUSB_SICD_EVENT_T;


typedef struct 
{
    uint16_t    u2OperationCode;
    uint16_t    u2ParamNum;
    uint32_t    u4TransactionID;
    uint32_t    u4Param[5];
    uint32_t    dwDataBufLen;
    uint8_t     *pbDataBuf;
    uint32_t    dwTotalDataSize;        // used in direction out operation
    MUSB_SICD_DATA_DIRECTION_E eDataDirection;
}MUSB_SICD_IO_DATA_T;


#define MUSB_SICD_RM_RT_OK                        ((INT32)    0)
#define MUSB_SICD_RM_RT_DEVICE_BUSY               ((INT32)   -1)
#define MUSB_SICD_RM_RT_MEDIA_REMOVED             ((INT32)   -2)
#define MUSB_SICD_RM_RT_ALIGNMENT_ERROR           ((INT32)   -3)
#define MUSB_SICD_RM_RT_BUS_RESET_FAILED          ((INT32)   -4)
#define MUSB_SICD_RM_RT_NOT_IN_DATA_PHASE         ((INT32)   -5)
#define MUSB_SICD_RM_RT_NOT_IN_READY_PHASE        ((INT32)   -6)
#define MUSB_SICD_RM_RT_USB_ERROR                 ((INT32)   -7)
#define MUSB_SICD_UNKNOWN_ERROR                   ((INT32)   -8)

typedef enum
{
    MUSB_HFI_SICD_DEV_STATUS_ABSENT = HFI_STATUS_ABSENT,
    MUSB_HFI_SICD_DEV_STATUS_FAIL   = HFI_STATUS_FAIL,
    MUSB_HFI_SICD_DEV_STATUS_READY  = HFI_STATUS_READY
}MUSB_HFI_SICD_DEV_STATUS_E;

typedef struct
{
    UINT32 ui4_type;
    union
    {
        /*Get Type*/
        MUSB_HFI_SICD_DEV_STATUS_E  eDeviceStatus;
        MUSB_HfiDeviceInfo          tDeviceInfo;
        uint32_t                    dwAlignment;
        uint32_t                    dwCapability;

        /*Set Type*/
        MUSB_SICD_IO_DATA_T         tIoData;
        MUSB_NFY_INFO_T             tNfyInfo;
        MUSB_HOTSWAP_NFY_INFO_T     tHotswapNfyInfo;
        uint32_t                    dwOffset;

        /*Future extend*/
        void *pv_req;
    }uContent;

    int32_t dwReturn;
}MUSB_SICD_REQUEST_T;


/**
 * Get information about the current medium.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ppMediumInfo where to store the medium information
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiSicdGetMediumInfo)(void* pDeviceData,
    MUSB_HfiMediumInfo** ppMediumInfo);

/**
 * Get information about the current device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ppDeviceInfo where to store the medium information
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiSicdGetDeviceInfo)(void* pDeviceData,
    MUSB_HfiDeviceInfo** ppDeviceInfo);

/**
 * Begin a read from a mass-storage device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ddwStartBlock 64 bits of starting block
 * @param dwByteCount how many consecutive bytes to read
 * @param pBuffer pointer to a buffer of sufficient size
 * to hold wBlockCount blocks in the device's dwBlockSize
 * @param pfTransferComplete callback for completion
 * @param bAllowDma TRUE to allow DMA; FALSE otherwise
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiSicdReadDevice)(void* pDeviceData, 
                                                MUSB_SICD_IO_DATA_T* pt_data);

/**
 * Begin a write to a mass-storage device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ddwStartBlock 64 bits of starting block
 * @param dwByteCount how many consecutive bytes to write
 * @param pBuffer pointer to a buffer of wBlockCount consecutive blocks 
 * in the device's dwBlockSize
 * @param pwActualBlocks where to store the number of blocks successfully written
 * @param bAllowDma TRUE to allow DMA; FALSE otherwise
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiSicdWriteDevice)(void* pDeviceData,
                                                MUSB_SICD_IO_DATA_T* pt_data);
    
/**
 * This function checks for the presence of a medium in the given device.
 * The return value does not indicate medium presence;
 * a change in medium presence will be indicated by a call
 * to MUSB_HfiMediumInserted or MUSB_HfiMediumRemoved, as appropriate.
 * The purpose of this function is to provide the threadless mass-storage
 * class driver an opportunity to perform this check.
 * Its intended use is for a media-polling thread to call periodically,
 * if the application wishes to handle medium insertion/removal at any time.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @return status code
 * @see #MUSB_HfiMediumInserted
 * @see #MUSB_HfiMediumRemoved
 * @see #MUSB_HfiAddDevice
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiSicdCheckMedium)(void* pDeviceData);
    
/**
 * MUSB_HfiDevice.
 * Device driver callbacks (dispatch table).
 * @field pPrivateData device driver private data; 
 * not to be interpreted by the HFI implementation
 * @field pfGetMediumInfo device medium-info service
 * @field pfReadDevice device read service
 * @field pfWriteDevice device write service
 * @field pfCheckMedim medium-presence-check service
 */
typedef struct 
{
    void* pPrivateData;  /* MUSB_SicdVolume* pVolume */
    MUSB_pfHfiSicdGetMediumInfo pfGetMediumInfo;
    MUSB_pfHfiSicdGetDeviceInfo pfGetDeviceInfo;    
    MUSB_pfHfiSicdReadDevice pfReadDevice;
    MUSB_pfHfiSicdWriteDevice pfWriteDevice;
    MUSB_pfHfiSicdCheckMedium pfHfiCheckMedium;
} MUSB_HfiSicdDevice;

/************************* HFI FUNCTIONS **************************/

/**
 * Add a mass-storage device.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that a new device is available.
 * Typically, this happens when a USB mass-storage device is detected
 * and enumerated, and the mass-storage driver has determined that
 * it is ready for use.
 * The HFI implementation uses the device information to determine if
 * it can support the device, assign a volume name, and mount it.
 * Error messages on failure are the HFI implementation's responsibility, 
 * since the error messaging mechanism is OS-dependent.
 * 
 * @param phVolume where to store a volume handle on success
 * @param pInfo pointer to device information struct
 * @param pDevice pointer to struct with callbacks to access the device
 * @return MUSB_HFI_SUCCESS on success
 * @return MUSB_HFI_ERROR_UNSUPPORTED_BLOCKSIZE if the block size
 * is not supported; the device driver can try a different block
 * size if the underlying device supports multiple sizes
 */
extern MUSB_HfiStatus MUSB_HfiSicdAddDevice(MUSB_HfiVolumeHandle* phVolume,
    const MUSB_HfiDeviceInfo* pInfo, MUSB_HfiSicdDevice* pDevice);

/**
 * A mass-storage medium was inserted.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that the device corresponding to a volume has had media inserted
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 */
extern void MUSB_HfiSicdMediumInserted(MUSB_HfiVolumeHandle hVolume);

/**
 * A mass-storage medium was removed.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that the medium in the device corresponding to a volume has been removed
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 */
extern void MUSB_HfiSicdMediumRemoved(MUSB_HfiVolumeHandle hVolume);

/**
 * A mass-storage device was removed.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that the device corresponding to a volume has been removed.
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 */
extern void MUSB_HfiSicdDeviceRemoved(MUSB_HfiVolumeHandle hVolume);

extern MGC_UcFsDevice* MUSB_UcFsSicdDeviceGet(void);

/**
 * Get file system read/write data structure of a mass-storage device.
 * User calls this use HFI.
 * @param void
 * @return pointer to MU_FS_DEVICE_TYPE.
 */
extern MU_FS_DEVICE_TYPE *MUSB_HfiSicdDriverGet(void);

#endif	/* multiple inclusion protection */
