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
 * MUSBStack-S HFI (Host Filesystem Interface).
 * $Revision: #1 $
 */

#ifndef __MUSB_HFI_H__
#define __MUSB_HFI_H__

#include "mu_stdint_drvif.h"

/** 
 * Introduction to Host Filesystem Interface.
 * The functions herein can be implemented by an OS to allow
 * the Mentor USB mass-storage driver to be hooked into the OS'
 * filesystem mechanisms, thus allowing the Mentor driver to be
 * used without additional effort.
 * The callbacks herein are implemented by the Mentor driver,
 * exposing the functionality needed by an OS to use the mass-storage device.
 * The above could also be applied to a custom, non-OS environment.
 * The driver is threadless (it reacts to callbacks from the BSR context
 * and the HFI implementation).  This eases its use in a non-OS environment,
 * and allows flexible and optimal HFI implementation for each OS.
 */

#define MUSB_MAKE64(L32, H32) ((uint64_t)(((uint64_t)(L32)) | (((uint64_t)(H32)) << 32)))


/************************* HFI CONSTANTS **************************/

#ifdef MUSB_8226

    #ifndef MUSB_UCFS_UNIT_COUNT
    #define MUSB_UCFS_UNIT_COUNT (12)
    #endif

#else /* 53xx, 82xx */

    #ifndef MUSB_UCFS_UNIT_COUNT
    #define MUSB_UCFS_UNIT_COUNT (20)
    #endif

#endif /* #ifdef MUSB_8226 */

#if defined(MUSB_8226) || defined(MUSB_LOADER)
    /** Maximum number of IO request from user */
    #define MUSB_HFI_MAX_IO_REQUEST        3
#else
    /** Maximum number of IO request from user */
    #define MUSB_HFI_MAX_IO_REQUEST        24
#endif

/** Maximum number of IO request from user */
#define MUSB_HFI_DRV_IO_REQUEST        4

/** Maximum number of UNICODE characters supported for serial number */
#define MUSB_HFI_MAX_VOLUME_SERIAL	31

/** Maximum number of UNICODE characters supported for disk vendor ID */
#define MUSB_HFI_MAX_DISK_VENDOR	15

/** Maximum number of UNICODE characters supported for disk product ID */
#define MUSB_HFI_MAX_DISK_PRODUCT	31

/** Maximum number of UNICODE characters supported for disk revision ID */
#define MUSB_HFI_MAX_DISK_REVISION	7

/** Maximum number of sectors in read/write operation */
#define MUSB_HFI_MAX_SECTOR_NO          0x10000

/** read write operation type. */
#define MUSB_HFI_OP_READ                        1
#define MUSB_HFI_OP_WRITE                      2

/** read write operation condition. */
#define MUSB_HFI_COND_FAIL             -1
#define MUSB_HFI_COND_READ_OK       1
#define MUSB_HFI_COND_WRITE_OK     2

/** medium status condition. */
#define HFI_STATUS_ABSENT   0
#define HFI_STATUS_FAIL        1
#define HFI_STATUS_READY     2

/** 
 * MUSB_HfiIoctlCmdType.
 * HFI Ioctl command Type.
 */
typedef enum
{
    MUSB_IOCTL_GET_MEDIUM_INFO,
    MUSB_IOCTL_GET_DEVICE_INFO,
    MUSB_IOCTL_GET_DEVICE_STATUS,
    MUSB_IOCTL_SET_TYPENFY,
    MUSB_IOCTL_SET_SOCKET_NFY,
    MUSB_IOCTL_SET_SOCKET_DEV_NFY,
    MUSB_IOCTL_GET_SOCKET_NUM,              /* Get socket number of (MUSB_Device *)pUsbDevice */
    MUSB_IOCTL_GET_TOTAL_MSD_SOCKET_NUM, /* Get total socket number of MSD Device on system */
    MUSB_IOCTL_SET_OCNFY,
    
    MUSB_IOCTL_END
} MUSB_HfiIoctlCmdType;

/** 
 * MUSB_HfiMediaType.
 * HFI Media Type.
 */
typedef enum
{
    /** Unknown media type */
    MUSB_HFI_MEDIA_UNKNOWN,
    /** Fixed media */
    MUSB_HFI_MEDIA_FIXED,
    /** Removable media */
    MUSB_HFI_MEDIA_REMOVABLE
} MUSB_HfiMediaType;

/** 
 * MUSB_HfiAccessType.
 * HFI Access Type.
 */
typedef enum
{
    /** device initial fail. Can not be accessed. */
    MUSB_HFI_ACCESS_FAIL,
    /** Random access read/write (e.g. hard disk, flash, floppy) */
    MUSB_HFI_ACCESS_RANDOM,
    /** Random access write-once (e.g. recordable CD/DVD) */
    MUSB_HFI_ACCESS_RANDOM_WRITE_ONCE,
    /** Random access read-only (e.g. CD/DVD) */
    MUSB_HFI_ACCESS_RANDOM_READ
} MUSB_HfiAccessType;

/** 
 * MUSB_HfiStatus.
 * HFI Operation Status.
 */
typedef enum
{
    /** success */
    MUSB_HFI_SUCCESS,
    /** error: unsupported block size (try another if possible) */
    MUSB_HFI_ERROR_UNSUPPORTED_BLOCKSIZE,
    /** error: media is currently write-protected */
    MUSB_HFI_ERROR_MEDIA_PROTECTED,
    /** error: media was removed before operation could complete */
    MUSB_HFI_ERROR_MEDIA_REMOVED,
    /** error: device was removed before operation could complete */
    MUSB_HFI_ERROR_DEVICE_REMOVED,
    /** error: transfer to/from media failed */
    MUSB_HFI_ERROR_TRANSFER,
    /** error: data verify failed */
    MUSB_HFI_ERROR_VERIFY,
    /** error: parameter verify failed */
    MUSB_HFI_ERROR_PARAMETER,
    /** error: device busy */
    MUSB_HFI_ERROR_BUSY,
    /** error: out of memory */
    MUSB_HFI_NO_MEMORY
} MUSB_HfiStatus;

/*************************** HFI TYPES ****************************/

/**
 * MUSB_HfiMediumInfo.
 * Storage medium information.
 * @field AccessType the medium's access type
 * @field dwBlockSize the fundamental medium block size, in bytes
 * @field dwBlockCountLo lower 32 bits of block count
 * @field dwBlockCountHi upper 32 bits of block count
 * up to MUSB_HFI_MAX_VOLUME_SERIAL UNICODE characters,
 * terminated by wide-NUL
 * @field bLun the medium's logical unit number in card reader.
 */
typedef struct
{
    MUSB_HfiAccessType AccessType;
    uint32_t dwBlockSize;
    uint32_t dwBlockCountLo;
    uint32_t dwBlockCountHi;
    uint8_t bLun;    
} MUSB_HfiMediumInfo;
 
/**
 * MUSB_HfiDeviceInfo.
 * Device information.
 * A mass-storage device driver fills this with any information
 * it knows before calling MUSB_HfiAddDevice.
 * 
 * @field wVendorId USB VID,
 * for the HFI implementation's use in generating a volume name
 * 
 * @field wProductId USB PID,
 * for the HFI implementation's use in generating a volume name
 * 
 * @field bcdDevice USB bcdDevice,
 * for the HFI implementation's use in generating a volume name
 * 
 * @field bBusAddress USB device address, 
 * for the HFI implementation's use in generating a volume name
 * 
 * @field bLunCount USB device logical unit number, 
 * for the HFI implementation's use in generating a volume name
 * 
 * @field bProtocol USB device mass storage class protocol code, 
 * for the HFI implementation's use in generating a volume name
 * 
 * @field pDiskVendor vendor string for the underlying disk, 
 * up to MUSB_HFI_MAX_DISK_VENDOR UNICODE characters,
 * terminated by wide-NUL
 * 
 * @field pDiskProduct product string for the underlying disk, 
 * up to MUSB_HFI_MAX_DISK_PRODUCT UNICODE characters,
 * terminated by wide-NUL
 * 
 * @field pDiskVendor revision string for the underlying disk, 
 * up to MUSB_HFI_MAX_DISK_REVISION UNICODE characters,
 * terminated by wide-NUL
 * 
 */
typedef struct
{
    uint16_t wVendorId;
    uint16_t wProductId;
    uint16_t bcdDevice;
    uint8_t bBusAddress;
    uint8_t bLunCount;    
    uint8_t bProtocol;    /*BOT, CBI, CB protocol*/
    const uint8_t *pDiskVendor;
    const uint8_t *pDiskProduct;
    const uint8_t *pDiskRevision;    
} MUSB_HfiDeviceInfo;

/** Handle created by the HFI implementation */
typedef void* MUSB_HfiVolumeHandle;

/**
 * Transfer completion callback.
 * Read and write are asynchronous requests, so the HFI implementation
 * supplies this callback to be notified on their completion.
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 * @param wActualBlocks the actual number of blocks successfully read
 * or written(/verified)
 */
typedef void (*MUSB_pfHfiTransferComplete)(MUSB_HfiVolumeHandle hVolume,
uint16_t wActualBlocks);
   
/**
 * Get information about the current medium.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ppMediumInfo where to store the medium information
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiGetMediumInfo)(void* pDeviceData,
    MUSB_HfiMediumInfo** ppMediumInfo);

/**
 * Get information about the current device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ppDeviceInfo where to store the medium information
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiGetDeviceInfo)(void* pDeviceData,
    MUSB_HfiDeviceInfo** ppDeviceInfo);

/**
 * Begin a read from a mass-storage device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ddwStartBlock 64 bits of starting block
 * @param dwByteCount how many consecutive bytes to read
 * @param pBuffer pointer to a buffer of sufficient size
 * to hold wBlockCount blocks in the device's dwBlockSize
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiReadDevice)(void* pDeviceData,
    uint64_t ddwStartBlock,
    uint32_t dwByteCount, uint8_t* pBuffer);

/**
 * Begin a write to a mass-storage device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @param ddwStartBlock 64 bits of starting block
 * @param dwByteCount how many consecutive bytes to write
 * @param pBuffer pointer to a buffer of wBlockCount consecutive blocks 
 * in the device's dwBlockSize
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiWriteDevice)(void* pDeviceData,
    uint64_t ddwStartBlock,
    uint32_t dwByteCount, uint8_t* pBuffer);

/**
 * Begin a check write protect to a mass-storage device.
 * @param pDeviceData pPrivateData from associated MUSB_HfiDevice
 * @return status code
 */
typedef MUSB_HfiStatus (*MUSB_pfHfiWriteProtect)(void* pDeviceData);

/**
 * This function is called when medium insert or removed. 
 * It is set from middle interface and called by MUSB_HfiMediumInserted 
 * or MUSB_HfiMediumRemoved.
 * @param pvTag middleware tag.
 * @return void
 * @see #MUSB_HfiMediumInserted
 * @see #MUSB_HfiMediumRemoved
 */
typedef void (*MUSB_pfHfiHotSwapNfy)(void *pvTag);

 /**
 * MUSB_HOTSWAP_NFY_INFO_T.
 * Device hot swap notify information.
 * It is set from middle interface and called by MUSB_HfiChildDeviceUpdate 
 * 
 * @field u4CompId OUT:Driver set u4CompId id
 * 
 * @field pvDevice OUT: Driver device pointer
 * 
 * @field u4Status OUT: device status
 * 
 * @field u4SocketNo OUT:  
 *    the socket number which the child device is attached to, Start from 0.
 * 
 * @field u4UsbPort OUT:Driver set h/w usb port number
 *
 * @field u4DevType IN: usb_socket_mw set device type at init call back
 *
 * @field pvTag IN: usb_socket_mw set notify tag
 *
 * @field pfNfy IN: usb_socket_mw set call back function
 * 
 */
typedef struct _MUSB_HOTSWAP_NFY_INFO_T
{
    UINT32 u4CompId;                    /* OUT:Driver set u4CompId id */
    void *pvDevice;                         /* OUT: Driver device pointer */  
    UINT32 u4Status;                      /* OUT: device status */
    UINT32 u4SocketNo;                 /* OUT:  the socket number which the child device is 
                                                            attached to, Start from 0 */   
    UINT32 u4UsbPort;                     /* OUT:Driver set h/w usb port number */
    UINT32 u4DevType;                   /* IN: usb_socket_mw set device type at init call back */    
    void *pvTag;                              /* IN: usb_socket_mw set notify tag */
    MUSB_pfHfiHotSwapNfy pfNfy;   /* IN: usb_socket_mw set call back function */
} MUSB_HOTSWAP_NFY_INFO_T;

/**
 * This function is called when medium read/write complete. 
 * It is set from middle interface and called by usb ISR when read/write finish.
 *
 * @param pvHfiNfyTag middleware tag.
 * @param e_nfy_cond read/write operation status.
 * @param pv_data driver tag, point to user's MUSB_IO_DATA_T *pt_data.
 * @return void
 */
typedef void (*MUSB_pfHfiNfy)(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data);

 /**
 * MUSB_NFY_INFO_T.
 * Device read/write async notify information.
 * It is set from middle interface and called by usb ISR when read/write finish.
 * 
 * @field pvHfiNfyTag middleware set notify tag.
 * 
 * @field pfHfiNfy middleware set read/write notify call back function.
 * 
 */
typedef struct _MUSB_NFY_INFO_T
{
    void *pvHfiNfyTag;
    MUSB_pfHfiNfy pfHfiNfy;
} MUSB_NFY_INFO_T;

 /**
 * MUSB_IO_DATA_T.
 * Device read/write async io parameter.
 * It is set from middle interface to perform read/write operation.
 * 
 * @field u8BlkNum device offset in block number.
 * 
 * @field u4Count read/write length in block number.
 * 
 * @field u4TransCount driver actual perform read/write length in block number.
 * 
 * @field b_ext_buf unused element.
 * 
 * @field pvData data buffer pointer.
 * 
 */
typedef struct _MUSB_IO_DATA_T
{
    uint64_t  u8BlkNum;
    uint32_t  u4Count;
    uint32_t  u4TransCount;
    BOOL    b_ext_buf;
    void   *pvData;
} MUSB_IO_DATA_T;
    
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
    void* pPrivateData;  /* MGC_MsdBotVolume* pVolume */
    MUSB_pfHfiGetMediumInfo pfGetMediumInfo;
    MUSB_pfHfiGetDeviceInfo pfGetDeviceInfo;    
    MUSB_pfHfiReadDevice pfReadDevice;
    MUSB_pfHfiWriteDevice pfWriteDevice;
    MUSB_pfHfiWriteProtect pfWriteProtect;    
} MUSB_HfiDevice;

/**
 * Element Meaning
 * 
 * @param MaxUnits Number of maximum units the driver can handle.
 * @param pfIoCtl Pointer to the device IoCtl function.
 * @param pfRWBurst Pointer to the device read/write multiple sector function. (optional)
 * @param pfWriteProtect Pointer to the device write protect checking function. (optional)
 * @param pfInitDevice Pointer to the device driver initialization function. (optional)
*/
typedef struct 
{
    uint32_t MaxUnits;
    int32_t (*pfIoCtl) (uint32_t u4Index, uint32_t u4Cmd, uint32_t u4Aux,void *pBuffer);
    int32_t (*pfRWBurst) (uint32_t  u4Index, uint32_t  u4Optype, void *pvData);
    int32_t (*pfWriteProtect) (uint32_t  u4Index, void *pvData);
    int32_t (*pfInitDevice) (uint32_t u4Index);
} MU_FS_DEVICE_TYPE;


/**
 * Data for bridge between HFI and uC/FS
 * @field pUsbDevice pointer to usb device.
 * @field pDevice pointer to HFI dispatch table
 * @field pt_data pointer to file manager read/write data structure.
 * @field rt_nfy_info pointer to file manager async read/write notify structure.
 * @field pt_socket_nfy_info pointer to device manager hot swap structure.
 * @field bSocketNo driver set device attached socket number of parent.
 * @field bMediumStatus driver set medium status.
 * @field bUsbPort driver set medium status.
 * @field bMediumStatus driver set this device on which usb port .
 * @field bIndex device index in total device list.
 */
typedef struct
{
    void* pUsbDevice;                        /* OUT: Driver set MUSB_Device* pUsbDevice */
    void *pvDevice;                            /* OUT: Driver set Hfi *pvDevice */
    void *pvData;                               /* IN:  FM set R/W data structure. */ 
    MUSB_NFY_INFO_T rt_nfy_info;   /* IN:  FM set async R/W notify function. */ 
    MUSB_HOTSWAP_NFY_INFO_T *pt_socket_nfy_info; /* IN:  DM set hot swap notify function. */ 
    uint8_t bSocketNo;                      /* OUT: Driver set device attached socket number of parent */
    uint8_t bMediumStatus;              /* OUT: Driver set medium status */ 
    uint8_t bUsbPort;                         /* OUT: Driver set this device on which usb port */     
    uint8_t bIndex;                            /* IN: device index in total device list. */    
} MGC_UcFsDevice;

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
extern MUSB_HfiStatus MUSB_HfiAddDevice(MUSB_HfiVolumeHandle* phVolume,
    const MUSB_HfiDeviceInfo* pInfo, MUSB_HfiDevice* pDevice);

/**
 * A mass-storage medium was inserted.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that the device corresponding to a volume has had media inserted
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 */
extern void MUSB_HfiMediumInserted(MUSB_HfiVolumeHandle hVolume,
    const MUSB_HfiMediumInfo* pMediumInfo);

/**
 * A mass-storage medium was removed.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that the medium in the device corresponding to a volume has been removed
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 */
extern void MUSB_HfiMediumRemoved(MUSB_HfiVolumeHandle hVolume);

/**
 * A mass-storage device was removed.
 * A mass-storage device driver calls this to inform the HFI implementation
 * that the device corresponding to a volume has been removed.
 * @param hVolume a volume handle as filled by a successful MUSB_HfiAddDevice
 */
extern void MUSB_HfiDeviceRemoved(MUSB_HfiVolumeHandle hVolume);

/**
 * Get file system read/write data structure of a mass-storage device.
 * User calls this use HFI.
 * @param void
 * @return pointer to MU_FS_DEVICE_TYPE.
 */
extern MU_FS_DEVICE_TYPE *MUSB_HfiDriverGet(void);

#endif	/* multiple inclusion protection */
