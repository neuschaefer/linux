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
 * SCSI client functions (driver interfacing, command transfer, and
 * command preparation)
 * $Revision: #1 $
 */

#include "mu_cdi.h"
#include "mu_mem.h"
#include "mu_bits.h"
#include "mu_descs.h"
#include "mu_stdio.h"
#include "mu_diag.h"
#include "mu_hfi_if.h"

#include "mu_scsi.h"

#include "mu_mpcsi.h"
#include "mu_impl.h"
#include "mu_mapi.h"
#include "mu_msd.h"
/****************************** CONSTANTS ********************************/

/* how many time to retry TEST_UNIT_READY: */
#define MGC_MSD_SCSI_TEST_UNIT_RETRIES   100

/* general retry count: */
#define MGC_MSD_SCSI_RETRIES 100

/* how many times to retry READ_CAPACITY if it fails but READ(10) of block 0 passes: */
#define MGC_MSD_CAPACITY_RETRIES    400

/* how many times to retry READ_FMT_CAPACITY: */
#define MGC_MSD_FMT_CAPACITY_RETRIES  2

/* check medium time: first time use smaller step to query */
#define MGC_MSD_CHECK_MEDIUM_1ST_TIME   (800)

/* check medium time */
#define MGC_MSD_CHECK_MEDIUM_TIME   (1000)

/* check medium retry time */
#define MGC_MSD_CHECK_MEDIUM_RETRY  10

/******************************** TYPES **********************************/

/******************************* FORWARDS ********************************/

static void *MGC_ScsiCmdSetCreateInstance(uint8_t bLunCount);

static void MGC_ScsiCmdSetDestroyInstance(void *pInstance);

static uint8_t MGC_ScsiCmdSetDiscoverDevice(void *pInstance, 
    MGC_MsdProtocol *pProtocol, uint8_t bLun);

static uint8_t MGC_ScsiDiscoverDevice(void *pInstance, 
    MGC_MsdProtocol *pProtocol, uint8_t bLun);

static uint8_t MGC_ScsiCmdSetCheckMedium(void *pInstance, 
    const MGC_MsdProtocol *pProtocol, uint8_t bLun);

static uint8_t MGC_ScsiCmdSetCheckMediumTimer(MUSB_BusHandle hBus, 
    MUSB_Device *pTargetDevice, uint32_t dwCheckTime);

/******************************* GLOBALS *********************************/

static MGC_MsdCmdSet MGC_ScsiCmdSet =
{
    MGC_ScsiCmdSetCreateInstance, 
    MGC_ScsiCmdSetDestroyInstance, 
    MGC_ScsiCmdSetDiscoverDevice
};

/****************************** FUNCTIONS ********************************/

MGC_MsdCmdSet *MGC_GetScsiCmdSet(void)
{
    return &MGC_ScsiCmdSet;
}

static void *MGC_ScsiCmdSetCreateInstance(uint8_t bLunCount)
{
    MGC_ScsiCmdSetData *pScsi = (MGC_ScsiCmdSetData *)MUSB_MemAlloc(sizeof(MGC_ScsiCmdSetData));

    if (pScsi)
    {
        MUSB_MemSet(pScsi, 0, sizeof(MGC_ScsiCmdSetData));
        pScsi->bLunCount = bLunCount;
    }

    return pScsi;
}

static void MGC_ScsiCmdSetDestroyInstance(void *pInstance)
{
    MGC_ScsiCmdSetData *pScsi;
    MGC_MsdProtocol *pProtocol;
    MGC_MsdBotProtocol *pBot;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_BusHandle hBus = NULL;
    MUSB_Device *pDevice = NULL;
    
    MUSB_ASSERT(pInstance);    
    pScsi = (MGC_ScsiCmdSetData *)pInstance;
    pProtocol = pScsi->pProtocol;

    if (!pProtocol)
    {
        MUSB_MemFree(pInstance);
        return;
    }

    if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
    {
        pBot = (MGC_MsdBotProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pBot);
        hBus = pBot->hBus;
        pDevice = pBot->pDevice;
    }
    else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
    {
        pCbt = (MGC_MsdCbtProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pCbt);    
        hBus = pCbt->hBus;
        pDevice = pCbt->pDevice;
    }  

    // check next device.
    if ((hBus) && (pDevice))
    {
        (void)MGC_ScsiCmdSetCheckMediumTimer(hBus, 
            pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
    }

    MUSB_MemFree(pInstance);
}

static uint8_t MGC_ScsiCheckMsdDevice(MGC_MsdDevice *pMsdDevice)
{
    MGC_MsdBotProtocol *pBot = NULL;
    MGC_MsdCbtProtocol *pCbt = NULL;
    MGC_MsdBotVolume *pBotVolume = NULL;
    MGC_MsdCbtVolume *pCbtVolume = NULL;
    MGC_ScsiCmdSetData* pScsi = NULL;
    uint8_t bLun;

    if ((!pMsdDevice) ||(!pMsdDevice->pProtocol) ||(!pMsdDevice->pProtocol->pProtocolData))
    {
        LOG(5, "Scsi: Null pointer.\n", NULL);
        return FALSE;
    }

    if (MGC_MSD_BOT_PROTOCOL == pMsdDevice->pProtocol->bProtocol)
    {
        pBot = pMsdDevice->pProtocol->pProtocolData;
        // check every Lun, make sure all devices on card reader are in medium checking state.
        for (bLun=0; bLun<pBot->bLunCount; bLun++)
        {                    
            pBotVolume = &(pBot->aVolume[bLun]);
            if (!pBotVolume->pCmdSetInstance)
            {
                LOG(5, "Scsi: bLun = %d, Null pointer.\n", bLun);
                return FALSE;
            }

            pScsi = pBotVolume->pCmdSetInstance;
            if ((pScsi->bState != MGC_SCSI_STATE_TEST_MEDIUM_REMOVE) && 
                (pScsi->bState != MGC_SCSI_STATE_TEST_MEDIUM_INSERT))
            {
                LOG(5, "Scsi: bLun = %d, Scsi init process not finish bState = %d.\n", 
                    bLun, pScsi->bState);
                return FALSE;
            }
        }
        
        // start check medium from Lun = 0.
        pBotVolume = &(pBot->aVolume[0]);
        pScsi = pBotVolume->pCmdSetInstance;
    }
    else if ((MGC_MSD_CB_PROTOCOL == pMsdDevice->pProtocol->bProtocol) ||
                (MGC_MSD_CBI_PROTOCOL == pMsdDevice->pProtocol->bProtocol))
    {
        pCbt = pMsdDevice->pProtocol->pProtocolData;
        // check every Lun, make sure all devices on card reader are in medium checking state.
        for (bLun=0; bLun<pCbt->bLunCount; bLun++)
        {  
            pCbtVolume = &(pCbt->aVolume[bLun]);        
            if (!pCbtVolume->pCmdSetInstance)
            {
                LOG(5, "Scsi: bLun = %d, Null pointer.\n", bLun);
                return FALSE;
            }

            pScsi = pCbtVolume->pCmdSetInstance;
            if ((pScsi->bState != MGC_SCSI_STATE_TEST_MEDIUM_REMOVE) && 
                (pScsi->bState != MGC_SCSI_STATE_TEST_MEDIUM_INSERT))
            {
                LOG(5, "Scsi: bLun = %d, Scsi init process not finish bState = %d.\n", 
                    bLun, pScsi->bState);
                return FALSE;
            }
        }

        // start check medium from Lun = 0.
        pCbtVolume = &(pCbt->aVolume[0]);
        pScsi = pCbtVolume->pCmdSetInstance;
    }

    // check if this MSD device is already init by SCSI query process.
    if ((!pScsi) || (!MGC_ScsiCmdSetCheckMedium(pScsi, pScsi->pProtocol, 0)))
    {    
        LOG(1, "Scsi: check pDevice = 0x%08X fail.\n", pMsdDevice->pUsbDevice);            
        return FALSE;
    }

    UNUSED(pMsdDevice);
    return TRUE;
}

static void MGC_ScsiTimerExpired(void *pPrivateData, MUSB_BusHandle hBus)
{
    MUSB_Device *pDevice;
    MUSB_Device *pTargetDevice;
    MGC_Port *pPort = (MGC_Port *)hBus;    
    uint16_t wCount;
    uint16_t wIndex;    
    uint8_t bStatus;

    MUSB_ASSERT(pPrivateData);
    pTargetDevice = (MUSB_Device *)pPrivateData;
    MUSB_ASSERT(pPort);
    
    // check if device is still on device list.   
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    if (wCount == 0)
    {
        LOG(1, "ScsiTimer: No device on list.\n", NULL);
        return;
    }
    
    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT) &&
            (pDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE) &&
            (pDevice == pTargetDevice))
        {
            LOG(7, "pParentDevice = 0x%08X, HubPort-%d:\n", 
                (uint32_t)pDevice->pParentUsbDevice, pDevice->bHubPort);
            LOG(7, "pDevice = 0x%08X, addr = %d, index = %d.\n", 
                (uint32_t)pDevice, pDevice->bBusAddress, wIndex);

            bStatus = MGC_ScsiCheckMsdDevice((MGC_MsdDevice *)pDevice->pDriverPrivateData);
            if (!bStatus)
            {
                break;
            }

            return;
        }
    }

    LOG(7, "HubPort-%d: Device = 0x%08X check fail. Check others.\n", 
        pTargetDevice->bHubPort, (uint32_t)pTargetDevice);

    // check pTargetDevice fail, and start to check other device again.   
    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE) &&
            (pDevice != pTargetDevice))
        {
            LOG(7, "pParentDevice = 0x%08X, HubPort-%d:\n", 
                (uint32_t)pDevice->pParentUsbDevice, pDevice->bHubPort);
            LOG(7, "pDevice = 0x%08X, addr = %d, index = %d.\n", 
                (uint32_t)pDevice, pDevice->bBusAddress, wIndex);
        
            bStatus = MGC_ScsiCheckMsdDevice((MGC_MsdDevice *)pDevice->pDriverPrivateData);
            if (!bStatus)
            {
                break;
            }

            return;
        }
    }    

    LOG(7, "ScsiTimer: No MSD Device on list. Stop timer.\n", NULL);    
}

static void MGC_ScsiAnnouceDevice(MGC_ScsiCmdSetData *pScsi, uint8_t bReady)
{
    MUSB_HfiDeviceInfo DeviceInfo;
    MUSB_HfiMediumInfo MediumInfo;
    MUSB_HfiAccessType AccessType = MUSB_HFI_ACCESS_RANDOM;
    uint32_t dwBlockCount = 0;
    const MGC_MsdStandardInquiryData *pInquiry = NULL;
    uint8_t bRet = FALSE;
    
    MUSB_ASSERT(pScsi);
    
    /* fill DeviceInfo */
    MUSB_MemSet(&DeviceInfo, 0, sizeof(DeviceInfo));

    LOG(0, "LUN = %d-%d.\n", pScsi->bLun, pScsi->bLunCount);

    if ((!pScsi->bNoMedium) && bReady)
    {
        switch (pScsi->aInquiryData[0] & 0x1f)
        {
        case MGC_SCSI_DEVICE_TYPE_DIRECT:
        case MGC_SCSI_DEVICE_TYPE_RBC:
            if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE10)
            {
                AccessType = (pScsi->aModeSenseData[3] & 0x80)
                                 ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
            }
            else if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE)
            {
                AccessType = (pScsi->aModeSenseData[2] & 0x80)
                                 ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
            }
            else
            {
                AccessType = MUSB_HFI_ACCESS_RANDOM_READ;
            }
                
            /* believe the RMB bit (this is a problem on some devices!) */
            pScsi->bRemovable = (pScsi->aInquiryData[1]&MGC_M_MSD_INQUIRY_RMB) ? TRUE : FALSE;
            break;

        case MGC_SCSI_DEVICE_TYPE_CARDREADER:
            AccessType = MUSB_HFI_ACCESS_RANDOM;
            /* correct removability in case we didn't infer it right */
            pScsi->bRemovable = TRUE;
            break;

        case MGC_SCSI_DEVICE_TYPE_SEQUENTIAL:
        case MGC_SCSI_DEVICE_TYPE_WORM:
        case MGC_SCSI_DEVICE_TYPE_OPTICAL:
            AccessType = MUSB_HFI_ACCESS_RANDOM_WRITE_ONCE;
            /* correct removability in case we didn't infer it right */
            pScsi->bRemovable = TRUE;
            break;

        case MGC_SCSI_DEVICE_TYPE_CDROM:
        case MGC_SCSI_DEVICE_TYPE_SCANNER:
            AccessType = MUSB_HFI_ACCESS_RANDOM_READ;
            /* correct removability in case we didn't infer it right */
            pScsi->bRemovable = TRUE;
            break;

        default:
            break;
        }

        /* generate pDiskVendor, pDiskProduct, pDiskRevision */
        pInquiry = (const MGC_MsdStandardInquiryData *)pScsi->aInquiryData;
        DeviceInfo.pDiskVendor = (const uint8_t *)&pInquiry->aVid[0];
        DeviceInfo.pDiskProduct = (const uint8_t *)&pInquiry->aPid[0];
        DeviceInfo.pDiskRevision = (const uint8_t *)&pInquiry->aRevision[0];

        /* generate dwBlockCount from READ_CAPACITY response */
        //  MTK Notice: Max Liao, 2006/10/19.
        //  Bug: aCapacityData report dwBlockCount = (total block - 1).
        dwBlockCount = MUSB_BIG_ENDIAN_SWAP32P(&pScsi->aCapacityData[0]);

        if (dwBlockCount > 0)
        {
            dwBlockCount++;
        }

        LOG(0, "ID = %s. [%s].\n", &pScsi->aInquiryData[8], 
            ((AccessType == MUSB_HFI_ACCESS_RANDOM_READ) ? ("RO") : ("RW")));
    }
    else
    {
        DeviceInfo.bLunCount = pScsi->bLunCount;    
        AccessType = MUSB_HFI_ACCESS_FAIL;
        /* set device info */
        bRet = pScsi->pProtocol->pfSetDeviceInfo(pScsi->pProtocol->pProtocolData, pScsi->bLun,
                                          &DeviceInfo);        
    }

    if (bReady)
    {
        /* set device info */
        bRet = pScsi->pProtocol->pfSetDeviceInfo(pScsi->pProtocol->pProtocolData, pScsi->bLun,
                                          &DeviceInfo);                
        /* prepare & set medium info */        
        MediumInfo.AccessType = AccessType;
        MediumInfo.dwBlockSize = MUSB_BIG_ENDIAN_SWAP32P(&pScsi->aCapacityData[4]);

        if (MediumInfo.dwBlockSize == 0)
        {
            MediumInfo.dwBlockSize = 512;
        }

        MediumInfo.dwBlockCountLo = dwBlockCount;
        MediumInfo.dwBlockCountHi = 0;
        MediumInfo.bLun = pScsi->bLun;
        bRet = pScsi->pProtocol->pfSetMediumInfo(pScsi->pProtocol->pProtocolData, pScsi->bLun,
                                          &MediumInfo);
        dwBlockCount /= ((1024*1024)/MediumInfo.dwBlockSize);
        LOG(0, "Block size = %d bytes, Total size = %d Mbytes.\n", MediumInfo.dwBlockSize, dwBlockCount);
    }

    /* Updating medium status */
    pScsi->pProtocol->pfSetReady(pScsi->pProtocol->pProtocolData, pScsi->bLun, bReady);
    
    UNUSED(bRet);
}

static uint8_t MGC_ScsiModeSense10(const MUSB_Device* pDevice)
{
    if (pDevice)
    {
        // Device totally can not support mode sense 10.
        if ((pDevice->DeviceDescriptor.idVendor == 0x4563) &&
            (pDevice->DeviceDescriptor.idProduct == 0xABCD))
        {
            return FALSE;
        }            
    }

    // Device can be tested by mode sense 10 first.
    // Maybe this device will not support mode sense 10.
    // Scsi state machine will continue to try mode sense 6.
    return TRUE;
}

static void MGC_ScsiCmdComplete(void *pPrivateData, int32_t e_nfy_cond, void *pv_data)
{
    uint8_t bAnnounce = FALSE;
    uint8_t bOpcode = 0;
    uint8_t bCmdLength = 0;
    uint8_t bLength = 0;
    uint8_t *pBuffer = NULL;
    uint8_t bSuccess = FALSE;
    uint8_t bReady = FALSE;
    MGC_ScsiCmdSetData *pScsi;
    MGC_MsdProtocol *pProtocol;
    uint8_t bLun;
    MGC_MsdBotProtocol *pBot = NULL;
    MGC_MsdBotVolume *pBotVolume = NULL;
    MGC_MsdCbtProtocol *pCbt = NULL;
    MGC_MsdCbtVolume *pCbtVolume = NULL;
    MUSB_BusHandle hBus = NULL;         /* MGC_Port* pPort */
    MUSB_Device* pDevice = NULL;
    uint8_t bWrapperStatus = (uint32_t)pv_data;
    
    MUSB_ASSERT(pPrivateData);
    pScsi = (MGC_ScsiCmdSetData *)pPrivateData;
    MUSB_ASSERT(pScsi);
    pProtocol = pScsi->pProtocol;
    MUSB_ASSERT(pProtocol);
    bLun = pScsi->bLun;

    if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
    {
        pBot = (MGC_MsdBotProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pBot);
        hBus = pBot->hBus;
        pDevice = pBot->pDevice;
    }
    else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
    {
        pCbt = (MGC_MsdCbtProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pCbt);    
        hBus = pCbt->hBus;
        pDevice = pCbt->pDevice;
    }  

    if (!hBus)
    {
        LOG(0, "hBus = NULL.\n", NULL);
        return;
    }
       
    UNUSED(e_nfy_cond);

    if (bWrapperStatus == MGC_MSD_COMMAND_ABORT)
    {   
        // check LUN directly.
        goto SCSI_CMD_CHECK_NEXT_LUN;
    }
    else if (bWrapperStatus == MGC_MSD_DEV_REMOVE)
    {
        // check next device.
        bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
            NULL, MGC_MSD_CHECK_MEDIUM_TIME);
        UNUSED(bSuccess);        
        return;
    }
    
    /* prepare for next command */
    MUSB_MemSet(pScsi->aCmd, 0, 16);

    /* 
     * Tevion 6-in-1 card reader problem (fails READ_FORMAT_CAPACITY for connected LUN):
     * Someone added a hack for Linux to work with it.  Add to unusual_devs.h:
     *   UNUSUAL_DEV(  0x0483, 0x1307, 0x0000, 0x9999, "Tevion", "Card Reader 6in1", US_SC_SCSI, US_PR_TEV6IN1, NULL, 0),
     * and to usb.c:
     *   case US_PR_TEV6IN1:
     *   ss->transport_name = "Bulk";
     *   ss->transport = usb_stor_Bulk_transport;
     *   ss->transport_reset = usb_stor_Bulk_reset;
     *   ss->max_lun = 3;
     *   break;
     * That is, if VID=0x0483, PID=0x1307, and 0<=bcdDevice<=0x9999, 
     * then set max LUN to 3 (presumably skips asking the device).
     * But, this has never made it into the official kernel...
     *
     * Windows works fine.  It always issues GET_MAX_LUN.  It then does, for each LUN:
     * 1. INQUIRY
     * 2. READ_FMT_CAPACITY (trying up to 3 times on errors, ultimately taking error to mean no medium)
     * Then, for each LUN which seems to have media:
     * 1. READ_CAPACITY
     * 2. READ(10) block 0
     * 3. MODE_SENSE page 1A
     * 4. MODE_SENSE page 3F
     * 5. READ_CAPACITY
     * 6. READ(10) block 0
     * 7. three READ_CAPACITY's
     * 8. two READ(10) block 0
     * 9. two READ_CAPACITY's
     * - it just goes on and on, with different repeats of the READ_CAPACITY and READ(10)
     */

    if (pScsi->bError)
    {
        pScsi->bError = FALSE;
        if (!bWrapperStatus)
        {
            LOG(7, "[%d-%d] SenKey=0x%X, ASC=0x%X, ASCQ=0x%X.\n", 
                pScsi->bLun, pScsi->bLunCount,
                pScsi->aSenseData[2], pScsi->aSenseData[12], pScsi->aSenseData[13]);
            
            /* analyze sense codes */
            if ((uint8_t)MGC_SCSI_SC_NOT_READY == (pScsi->aSenseData[2] & 0xf))
            {
                bAnnounce = TRUE;
                pScsi->bNoMedium = TRUE;
            }           
            else if ((uint8_t)MGC_SCSI_SC_UNIT_ATTENTION == (pScsi->aSenseData[2] & 0xf))
            {
                bAnnounce = FALSE;
            }
            else if ((uint8_t)MGC_SCSI_SC_NONE == (pScsi->aSenseData[2] & 0xf))
            {
                bAnnounce = TRUE;
                pScsi->bNoMedium = TRUE;
            }
            else if ((uint8_t)MGC_SCSI_SC_ILLEGAL_REQUEST == (pScsi->aSenseData[2] & 0xf))
            {
                MUSB_DIAG_STRING(1, "MSD: ILLEGAL_REQUEST");
                if (pScsi->bState == MGC_SCSI_STATE_MODE_SENSE6)
                {
                    pScsi->bState = MGC_SCSI_STATE_MODE_SENSE10;
                    MUSB_DIAG_STRING(1, "MSD: Mode sense 10 not support.");
                }
                else
                {
                    if (pScsi->dwRetries > 5)
                    {
                        // this device fail, report to MW.
                        bAnnounce = TRUE;
                        pScsi->bNoMedium = TRUE;
                        bReady = TRUE;
                    }
                    else
                    {
                        pScsi->bState = MGC_SCSI_STATE_NONE;
                        pScsi->dwRetries ++;
                    }
                }
            }
            else
            {
                //  Retry all the msd command sequence when CSW status is failed.
                if (pScsi->bState != MGC_SCSI_STATE_NONE)
                {
                    pScsi->bState = MGC_SCSI_STATE_NONE;
                }
            }
         }
    }

    if (bWrapperStatus)
    {
        if (pScsi->bState == MGC_SCSI_STATE_MODE_SENSE6)
        {
            pScsi->bState = MGC_SCSI_STATE_MODE_SENSE10;
            MUSB_DIAG_STRING(1, "MSD: Mode sense 10 not support.");
        }
        else
        {
            LOG(7, "Request Sense start.\n", NULL);        

            /* command failed: find out why */
            pScsi->bError = TRUE;

            bCmdLength = 6;
            bOpcode = MGC_SCSI_REQUEST_SENSE;
            pBuffer = pScsi->aSenseData;
            //  MTK Notice: Max Liao, 2006/10/19.
            //  Bug: REQUEST_SENSE sense data is only 18 bytes.
            bLength = 18;
            pScsi->aCmd[0] = bOpcode;
            pScsi->aCmd[4] = bLength;

            bSuccess = pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd,
                                            bCmdLength, pBuffer, bLength, FALSE,
                                            MGC_ScsiCmdComplete);

            if (!bSuccess)
            {            
                MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
                // check next device.
                bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                    pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
                UNUSED(bSuccess);
            }
            return;
        }
    }

    /* process result of last action and setup next one */
    if (!bAnnounce)
    {
        switch (pScsi->bState)
        {
        //  MTK Notice: Max Liao, 2006/10/31.
        //  Retry all the msd command sequence when CSW status is failed.    
        case MGC_SCSI_STATE_NONE:
            MUSB_DIAG_STRING(2, "MSD: Standard Inquiry start");            
            bCmdLength = 6;
            bOpcode = MGC_SCSI_INQUIRY;
            pBuffer = pScsi->aInquiryData;
            pScsi->aCmd[4] = 36;
            bLength = 36;
            pScsi->bState = MGC_SCSI_STATE_INQUIRY;
            break;
            
        case MGC_SCSI_STATE_INQUIRY:
            MUSB_DIAG_STRING(2, "MSD: Standard Inquiry complete");
            /* test unit ready */
            bCmdLength = 6;
            bLength = 0;
            bOpcode = MGC_SCSI_TEST_UNIT_READY;
            pScsi->bState = MGC_SCSI_STATE_INQUIRY_OK;
            break;

        case MGC_SCSI_STATE_INQUIRY_OK:
            MUSB_DIAG_STRING(2, "MSD: Test Unit Ready complete");
            /* read capacity */
            bCmdLength = 10;
            bOpcode = MGC_SCSI_READ_CAPACITY;
            pBuffer = pScsi->aCapacityData;
            bLength = 8;
            pScsi->bState = MGC_SCSI_STATE_READ_CAPACITY;
            break;

        case MGC_SCSI_STATE_READ_CAPACITY:
            MUSB_DIAG_STRING(2, "MSD: Read Capacity complete");
            /* test unit ready */
            bCmdLength = 6;
            bLength = 0;
            bOpcode = MGC_SCSI_TEST_UNIT_READY;
            pScsi->bState = MGC_SCSI_STATE_READ_CAPACITY_OK;
            break;

        case MGC_SCSI_STATE_READ_CAPACITY_OK:
            MUSB_DIAG_STRING(2, "MSD: Test Unit Ready complete");
            /* read capacity */
            if (MGC_ScsiModeSense10(pDevice))
            {
                bCmdLength = 10;
                bOpcode = MGC_SCSI_MODE_SENSE10;
                pScsi->bModeSenseType = MGC_SCSI_MODE_SENSE10;
                pScsi->aCmd[2] = 0x3F; // page code. 
                pScsi->aCmd[8] = 0xC0; // allocation length.
            }
            else
            {
                bCmdLength = 6;
                bOpcode = MGC_SCSI_MODE_SENSE;
                pScsi->bModeSenseType = MGC_SCSI_MODE_SENSE;
                pScsi->aCmd[2] = 0x3F; // page code. 
                pScsi->aCmd[4] = 0xC0; // allocation length.
            }
            pBuffer = pScsi->aModeSenseData;
            bLength = 0xC0;
            pScsi->bState = MGC_SCSI_STATE_MODE_SENSE6;
            break;

        case MGC_SCSI_STATE_MODE_SENSE10:
            MUSB_DIAG_STRING(2, "MSD: Mode Sense 10 fail");
            //  MTK Notice: Max Liao, 2006/11/20.
            //  MODE SENSE (6) is mandatory for all devices
            //  define in "Microsoft-iSCSI-Logo-Requirements-1.0.doc"
            /* read capacity */
            bCmdLength = 6;
            bOpcode = MGC_SCSI_MODE_SENSE;
            pScsi->bModeSenseType = MGC_SCSI_MODE_SENSE;
            pBuffer = pScsi->aModeSenseData;
            pScsi->aCmd[2] = 0x3F; // page code. 
            pScsi->aCmd[4] = 0xC0; // allocation length.
            bLength = 0xC0;
            pScsi->bState = MGC_SCSI_STATE_MODE_SENSE6;
            break;

        case MGC_SCSI_STATE_MODE_SENSE6:
            MUSB_DIAG_STRING(2, "MSD: Mode Sense complete");
            bCmdLength = 6;
            bLength = 0;
            bOpcode = MGC_SCSI_TEST_UNIT_READY;
            pScsi->bState = MGC_SCSI_STATE_TEST_UNIT_READY;
            break;

        case MGC_SCSI_STATE_TEST_UNIT_READY:
            MUSB_DIAG_STRING(2, "MSD: Test Unit Ready complete");
            /* if ready, done; otherwise try again */
            bAnnounce = TRUE;

//  MTK Notice: Max Liao, 2007/08/28.
// always report not ready, and wait device to be stable at first quiry.
            bReady = FALSE;          
            break;

        case MGC_SCSI_STATE_PREVENT_MEDIUM_REMOVE:
            /* inform client */
            return;

        case MGC_SCSI_STATE_ALLOW_MEDIUM_REMOVE:
            /* stop unit */
            bCmdLength = 6;
            bOpcode = MGC_SCSI_START_STOP_UNIT;
            pScsi->bState = MGC_SCSI_STATE_STOP_UNIT;
            break;

        case MGC_SCSI_STATE_STOP_UNIT:
            /* done */
            return;

        default:
            MUSB_DIAG1(1, "MSD/SCSI: Internal error; bad state ", pScsi->bState, 16, 2);
            return;
        }
    }

    if (!bAnnounce)
    {
        /* set common fields */
        pScsi->aCmd[0] = bOpcode;
        /* not announcing yet; send next command */

        bSuccess = pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd,
                                        bCmdLength, pBuffer, bLength, FALSE,
                                        MGC_ScsiCmdComplete);
        if (!bSuccess)
        {        
            MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
            // check next device.
            bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
            UNUSED(bSuccess);
        }
    }
    else
    {
SCSI_CMD_CHECK_NEXT_LUN:    

        // prepare to check medium 
        pScsi->bState = (bReady) ? 
            MGC_SCSI_STATE_TEST_MEDIUM_REMOVE : MGC_SCSI_STATE_TEST_MEDIUM_INSERT;

        MGC_ScsiAnnouceDevice(pScsi, bReady);

        //  MTK Notice: Max Liao, 2006/11/21.
        //  start pollling medium is inserted or removed.    
        if (pScsi->bLun == (pScsi->bLunCount - 1))
        {       
            bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                NULL, MGC_MSD_CHECK_MEDIUM_1ST_TIME);
            UNUSED(bSuccess);
        }
        else
        {
            //  check next LUN device.
            bLun++;

            if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
            {
                MUSB_ASSERT(pBot);
                pBotVolume = &(pBot->aVolume[bLun]);
                MUSB_ASSERT(pBotVolume);
                bSuccess = MGC_ScsiCmdSetDiscoverDevice(pBotVolume->pCmdSetInstance, 
                    &(pBot->Protocol), bLun);                
            }
            else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                        (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
            {
                MUSB_ASSERT(pCbt);
                pCbtVolume = &(pCbt->aVolume[bLun]);
                MUSB_ASSERT(pCbtVolume);
                bSuccess = MGC_ScsiCmdSetDiscoverDevice(pCbtVolume->pCmdSetInstance, 
                    &(pCbt->Protocol), bLun);
            }  

            if (!bSuccess)
            {
                MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
                // check next device.
                bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                    pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
                UNUSED(bSuccess);
            }            
        }
    }
}

static void MGC_ScsiDiscoverComplete(void *pPrivateData, int32_t e_nfy_cond, void *pv_data)
{
    uint8_t bAnnounce = FALSE;
    uint8_t bOpcode = 0;
    uint8_t bCmdLength = 0;
    uint8_t bLength = 0;
    uint8_t *pBuffer = NULL;
    uint8_t bSuccess = FALSE;
    uint8_t bReady = FALSE;
    MGC_ScsiCmdSetData *pScsi;
    MGC_MsdProtocol *pProtocol;
    uint8_t bLun;
    MGC_MsdBotProtocol *pBot = NULL;
    MGC_MsdBotVolume *pBotVolume = NULL;
    MGC_MsdCbtProtocol *pCbt = NULL;
    MGC_MsdCbtVolume *pCbtVolume = NULL;
    MUSB_BusHandle hBus = NULL;         /* MGC_Port* pPort */
    MUSB_Device* pDevice = NULL;
    uint8_t bWrapperStatus = (uint32_t)pv_data;
   
    MUSB_ASSERT(pPrivateData);
    pScsi = (MGC_ScsiCmdSetData *)pPrivateData;
    MUSB_ASSERT(pScsi);
    pProtocol = pScsi->pProtocol;
    MUSB_ASSERT(pProtocol);
    bLun = pScsi->bLun;

    if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
    {
        pBot = (MGC_MsdBotProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pBot);
        hBus = pBot->hBus;
        pDevice = pBot->pDevice;
    }
    else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
    {
        pCbt = (MGC_MsdCbtProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pCbt);    
        hBus = pCbt->hBus;
        pDevice = pCbt->pDevice;
    }  

    if (!hBus)
    {
        LOG(0, "hBus = NULL.\n", NULL);
        return;
    }
   
    UNUSED(e_nfy_cond);

    if ((bWrapperStatus == MGC_MSD_COMMAND_ABORT) &&
        (pScsi->bState != MGC_SCSI_STATE_MODE_SENSE6))
    {
        // check LUN directly.
        goto SCSI_DISCOVERY_CHECK_NEXT_LUN;
    }
    else if (bWrapperStatus == MGC_MSD_DEV_REMOVE)
    {
        // check next device.
        bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
            NULL, MGC_MSD_CHECK_MEDIUM_TIME);
        UNUSED(bSuccess);        
        return;
    }
    
    /* prepare for next command */
    MUSB_MemSet(pScsi->aCmd, 0, 16);

    /* 
     * Tevion 6-in-1 card reader problem (fails READ_FORMAT_CAPACITY for connected LUN):
     * Someone added a hack for Linux to work with it.  Add to unusual_devs.h:
     *   UNUSUAL_DEV(  0x0483, 0x1307, 0x0000, 0x9999, "Tevion", "Card Reader 6in1", US_SC_SCSI, US_PR_TEV6IN1, NULL, 0),
     * and to usb.c:
     *   case US_PR_TEV6IN1:
     *   ss->transport_name = "Bulk";
     *   ss->transport = usb_stor_Bulk_transport;
     *   ss->transport_reset = usb_stor_Bulk_reset;
     *   ss->max_lun = 3;
     *   break;
     * That is, if VID=0x0483, PID=0x1307, and 0<=bcdDevice<=0x9999, 
     * then set max LUN to 3 (presumably skips asking the device).
     * But, this has never made it into the official kernel...
     *
     * Windows works fine.  It always issues GET_MAX_LUN.  It then does, for each LUN:
     * 1. INQUIRY
     * 2. READ_FMT_CAPACITY (trying up to 3 times on errors, ultimately taking error to mean no medium)
     * Then, for each LUN which seems to have media:
     * 1. READ_CAPACITY
     * 2. READ(10) block 0
     * 3. MODE_SENSE page 1A
     * 4. MODE_SENSE page 3F
     * 5. READ_CAPACITY
     * 6. READ(10) block 0
     * 7. three READ_CAPACITY's
     * 8. two READ(10) block 0
     * 9. two READ_CAPACITY's
     * - it just goes on and on, with different repeats of the READ_CAPACITY and READ(10)
     */

    if (pScsi->bError)
    {   
        pScsi->bError = FALSE;
        if (!bWrapperStatus)
        {
            LOG(7, "[%d-%d] SenKey=0x%X, ASC=0x%X, ASCQ=0x%X.\n", 
                pScsi->bLun, pScsi->bLunCount,
                pScsi->aSenseData[2], pScsi->aSenseData[12], pScsi->aSenseData[13]);
        
            /* analyze sense codes */
            if ((uint8_t)MGC_SCSI_SC_NOT_READY == (pScsi->aSenseData[2] & 0xf))
            {
                bAnnounce = TRUE;
                pScsi->bNoMedium = TRUE;
            }
            else if ((uint8_t)MGC_SCSI_SC_UNIT_ATTENTION == (pScsi->aSenseData[2] & 0xf))
            {
                bAnnounce = TRUE;
                pScsi->bNoMedium = TRUE;
            }
            else if ((uint8_t)MGC_SCSI_SC_NONE == (pScsi->aSenseData[2] & 0xf))
            {
                bAnnounce = TRUE;
                pScsi->bNoMedium = TRUE;
            }
            //  MTK Notice: Max Liao, 2006/11/20.
            //  handle illegal request.
            else if ((uint8_t)MGC_SCSI_SC_ILLEGAL_REQUEST == (pScsi->aSenseData[2] & 0xf))
            {
                MUSB_DIAG_STRING(1, "MSD: ILLEGAL_REQUEST");
                if (pScsi->bState == MGC_SCSI_STATE_MODE_SENSE6)
                {
                    pScsi->bState = MGC_SCSI_STATE_MODE_SENSE10;
                    MUSB_DIAG_STRING(1, "MSD: Mode sense 10 not support.");
                }
                else
                {
                    if (pScsi->dwRetries > 5)
                    {
                        // this device fail, report to MW.
                        bAnnounce = TRUE;
                        pScsi->bNoMedium = TRUE;
                        bReady = TRUE;
                    }
                    else
                    {
                        pScsi->bState = MGC_SCSI_STATE_NONE;
                        pScsi->dwRetries ++;
                    }
                }
            }
            else
            {
                //  MTK Notice: Max Liao, 2006/10/31.
                //  Retry all the msd command sequence when CSW status is failed.
                if (pScsi->bState != MGC_SCSI_STATE_NONE)
                {
                    pScsi->bState = MGC_SCSI_STATE_NONE;
                }
            }
        }
    }

    if (bWrapperStatus)
    {
        if (pScsi->bState == MGC_SCSI_STATE_MODE_SENSE6)
        {
            pScsi->bState = MGC_SCSI_STATE_MODE_SENSE10;
            MUSB_DIAG_STRING(1, "MSD: Mode sense 10 not support.");
        }
        else
        {    
            /* command failed: find out why */
            pScsi->bError = TRUE;

            bCmdLength = 6;
            bOpcode = MGC_SCSI_REQUEST_SENSE;
            pBuffer = pScsi->aSenseData;
            //  MTK Notice: Max Liao, 2006/10/19.
            //  Bug: REQUEST_SENSE sense data is only 18 bytes.
            bLength = 18;
            pScsi->aCmd[0] = bOpcode;
            pScsi->aCmd[4] = bLength;

            bSuccess = pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd,
                                            bCmdLength, pBuffer, bLength, FALSE,
                                            MGC_ScsiDiscoverComplete);

            if (!bSuccess)
            {
                MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
                // check next device.
                bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                    pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
                UNUSED(bSuccess);
            }
            return;
        }
    }

    /* process result of last action and setup next one */
    if (!bAnnounce)
    {
        switch (pScsi->bState)
        {
        //  MTK Notice: Max Liao, 2006/10/31.
        //  Retry all the msd command sequence when CSW status is failed.    
        case MGC_SCSI_STATE_NONE:
            MUSB_DIAG_STRING(2, "MSD: Standard Inquiry start");            
            bCmdLength = 6;
            bOpcode = MGC_SCSI_INQUIRY;
            pBuffer = pScsi->aInquiryData;
            pScsi->aCmd[4] = 36;
            bLength = 36;
            pScsi->bState = MGC_SCSI_STATE_INQUIRY;
            break;
            
        case MGC_SCSI_STATE_INQUIRY:
            MUSB_DIAG_STRING(2, "MSD: Standard Inquiry complete");
            /* test unit ready */
            bCmdLength = 6;
            bLength = 0;
            bOpcode = MGC_SCSI_TEST_UNIT_READY;
            pScsi->bState = MGC_SCSI_STATE_INQUIRY_OK;
            break;

        case MGC_SCSI_STATE_INQUIRY_OK:
            MUSB_DIAG_STRING(2, "MSD: Test Unit Ready complete");
            /* read capacity */
            bCmdLength = 10;
            bOpcode = MGC_SCSI_READ_CAPACITY;
            pBuffer = pScsi->aCapacityData;
            bLength = 8;
            pScsi->bState = MGC_SCSI_STATE_READ_CAPACITY;
            break;

        case MGC_SCSI_STATE_READ_CAPACITY:
            MUSB_DIAG_STRING(2, "MSD: Read Capacity complete");
            /* test unit ready */
            bCmdLength = 6;
            bLength = 0;
            bOpcode = MGC_SCSI_TEST_UNIT_READY;
            pScsi->bState = MGC_SCSI_STATE_READ_CAPACITY_OK;
            break;

        case MGC_SCSI_STATE_READ_CAPACITY_OK:
            MUSB_DIAG_STRING(2, "MSD: Test Unit Ready complete");
            /* read capacity */
            if (MGC_ScsiModeSense10(pDevice))
            {
                bCmdLength = 10;
                bOpcode = MGC_SCSI_MODE_SENSE10;
                pScsi->bModeSenseType = MGC_SCSI_MODE_SENSE10;
                pScsi->aCmd[2] = 0x3F; // page code. 
                pScsi->aCmd[8] = 0xC0; // allocation length.
            }
            else
            {
                bCmdLength = 6;
                bOpcode = MGC_SCSI_MODE_SENSE;
                pScsi->bModeSenseType = MGC_SCSI_MODE_SENSE;
                pScsi->aCmd[2] = 0x3F; // page code. 
                pScsi->aCmd[4] = 0xC0; // allocation length.
            }
            pBuffer = pScsi->aModeSenseData;
            bLength = 0xC0;
            pScsi->bState = MGC_SCSI_STATE_MODE_SENSE6;
            break;

        case MGC_SCSI_STATE_MODE_SENSE10:
            MUSB_DIAG_STRING(2, "MSD: Mode Sense 10 fail");
            //  MTK Notice: Max Liao, 2006/11/20.
            //  MODE SENSE (6) is mandatory for all devices
            //  define in "Microsoft-iSCSI-Logo-Requirements-1.0.doc"
            /* read capacity */
            bCmdLength = 6;
            bOpcode = MGC_SCSI_MODE_SENSE;
            pScsi->bModeSenseType = MGC_SCSI_MODE_SENSE;            
            pBuffer = pScsi->aModeSenseData;
            pScsi->aCmd[2] = 0x3F; // page code. 
            pScsi->aCmd[4] = 0xC0; // allocation length.
            bLength = 0xC0;
            pScsi->bState = MGC_SCSI_STATE_MODE_SENSE6;
            break;

        case MGC_SCSI_STATE_MODE_SENSE6:
            MUSB_DIAG_STRING(2, "MSD: Mode Sense complete");
            bCmdLength = 6;
            bLength = 0;
            bOpcode = MGC_SCSI_TEST_UNIT_READY;
            pScsi->bState = MGC_SCSI_STATE_TEST_UNIT_READY;
            break;

        case MGC_SCSI_STATE_TEST_UNIT_READY:
            MUSB_DIAG_STRING(2, "MSD: Test Unit Ready complete");
            /* if ready, done; otherwise try again */
            bAnnounce = TRUE;
            bReady = TRUE;          
            break;

        case MGC_SCSI_STATE_PREVENT_MEDIUM_REMOVE:
            /* inform client */
            return;

        case MGC_SCSI_STATE_ALLOW_MEDIUM_REMOVE:
            /* stop unit */
            bCmdLength = 6;
            bOpcode = MGC_SCSI_START_STOP_UNIT;
            pScsi->bState = MGC_SCSI_STATE_STOP_UNIT;
            break;

        case MGC_SCSI_STATE_STOP_UNIT:
            /* done */
            return;

        default:
            MUSB_DIAG1(1, "MSD/SCSI: Internal error; bad state ", pScsi->bState, 16, 2);
            return;
        }
    }

    if (!bAnnounce)
    {
        /* set common fields */
        pScsi->aCmd[0] = bOpcode;
        /* not announcing yet; send next command */

        bSuccess = pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd,
                                        bCmdLength, pBuffer, bLength, FALSE,
                                        MGC_ScsiDiscoverComplete);
        if (!bSuccess)
        {
            MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
            // check next device.
            bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
            UNUSED(bSuccess);
        }
    }
    else
    {
SCSI_DISCOVERY_CHECK_NEXT_LUN:

        // prepare to check medium 
        pScsi->bState = (bReady) ? 
            MGC_SCSI_STATE_TEST_MEDIUM_REMOVE : MGC_SCSI_STATE_TEST_MEDIUM_INSERT;

        MGC_ScsiAnnouceDevice(pScsi, bReady);

        //  MTK Notice: Max Liao, 2006/11/21.
        //  start pollling medium is inserted or removed.    
        if (pScsi->bLun == (pScsi->bLunCount - 1))
        {                
            bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                NULL, MGC_MSD_CHECK_MEDIUM_1ST_TIME);
            UNUSED(bSuccess);
        }
        else
        {
            //  check next LUN device.
            bLun++;

            if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
            {
                MUSB_ASSERT(pBot);            
                pBotVolume = &(pBot->aVolume[bLun]);
                MUSB_ASSERT(pBotVolume);
                bSuccess = MGC_ScsiDiscoverDevice(pBotVolume->pCmdSetInstance, 
                    &(pBot->Protocol), bLun);                
            }
            else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                        (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
            {
                MUSB_ASSERT(pCbt);            
                pCbtVolume = &(pCbt->aVolume[bLun]);
                MUSB_ASSERT(pCbtVolume);
                bSuccess = MGC_ScsiDiscoverDevice(pCbtVolume->pCmdSetInstance, 
                    &(pCbt->Protocol), bLun);
            }  

            if (!bSuccess)
            {
                MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
                // check next device.
                bSuccess = MGC_ScsiCmdSetCheckMediumTimer(hBus, 
                    pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
                UNUSED(bSuccess);
            }            
        }
    }
}

static void MGC_ScsiCheckMedium(void *pPrivateData, int32_t e_nfy_cond, void *pv_data)
{
    uint8_t bOpcode = 0;
    uint8_t bCmdLength = 0;
    uint8_t bLength = 0;
    uint8_t *pBuffer = NULL;
    uint8_t bSuccess = FALSE;
    MGC_ScsiCmdSetData *pScsi;
    MGC_MsdProtocol *pProtocol;
    uint8_t bLun;
    MGC_MsdBotProtocol *pBot = NULL;
    MGC_MsdBotVolume *pBotVolume = NULL;
    MGC_MsdCbtProtocol *pCbt = NULL;
    MGC_MsdCbtVolume *pCbtVolume = NULL;
    MUSB_Device* pDevice = NULL;
    uint8_t bWrapperStatus = (uint32_t)pv_data;
    MGC_Port *pPort = NULL;
    MUSB_HfiAccessType AccessType = MUSB_HFI_ACCESS_RANDOM;
   
    MUSB_ASSERT(pPrivateData);
    pScsi = (MGC_ScsiCmdSetData *)pPrivateData;
    MUSB_ASSERT(pScsi);
    pProtocol = pScsi->pProtocol;
    MUSB_ASSERT(pProtocol);
    bLun = pScsi->bLun;

    if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
    {
        pBot = (MGC_MsdBotProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pBot);
        pPort = pBot->hBus;
        pDevice = pBot->pDevice;
    }
    else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
    {
        pCbt = (MGC_MsdCbtProtocol *)pProtocol->pProtocolData;
        MUSB_ASSERT(pCbt);    
        pPort = pCbt->hBus;
        pDevice = pCbt->pDevice;
    }  
    MUSB_ASSERT(pPort);
    if (!pPort)
    {
        LOG(0, "pPort = NULL.\n", NULL);
        return;
    }
    UNUSED(e_nfy_cond);

    if (bWrapperStatus == MGC_MSD_COMMAND_ABORT)
    {
        // check LUN directly.
        pScsi->bNoMedium = TRUE;
    }
    else if (bWrapperStatus == MGC_MSD_DEV_REMOVE)
    {
        // check next device.
        bSuccess = MGC_ScsiCmdSetCheckMediumTimer((MUSB_BusHandle)pPort, 
            NULL, MGC_MSD_CHECK_MEDIUM_TIME);
        UNUSED(bSuccess);        
        return;
    }
    
    /* prepare for next command */
    MUSB_MemSet(pScsi->aCmd, 0, 16);

    if (pScsi->bError)
    {    
        pScsi->bError = FALSE;
        if (!bWrapperStatus)
        {
            LOG(7, "[%d-%d] SenKey=0x%X, ASC=0x%X, ASCQ=0x%X.\n", 
                pScsi->bLun, pScsi->bLunCount,
                pScsi->aSenseData[2], pScsi->aSenseData[12], pScsi->aSenseData[13]);
        
            /* analyze sense codes */
            if (((uint8_t)MGC_SCSI_SC_NOT_READY == (pScsi->aSenseData[2] & 0xf))
                    && (MGC_SCSI_ASC_UNIT_NOT_READY == pScsi->aSenseData[12]))
            {
                pScsi->bRemovable = TRUE;
                if (2 == pScsi->aSenseData[13])
                {
                    /* needs a start unit */
                    bCmdLength = 6;

                    bOpcode = MGC_SCSI_START_STOP_UNIT;
                    pScsi->aCmd[4] = 3;
                }
            }
            else if (((uint8_t)MGC_SCSI_SC_UNIT_ATTENTION == (pScsi->aSenseData[2] & 0xf))
                         && (MGC_SCSI_ASC_MEDIUM_CHANGE == pScsi->aSenseData[12]))
            {
                if (pScsi->bState == MGC_SCSI_STATE_TEST_MEDIUM_REMOVE)
                {
                    // Medium is already in removable device.
                    // Maybe device write protect status change.
                    if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE10)
                    {
                        bCmdLength = 10;
                        bOpcode = MGC_SCSI_MODE_SENSE10;
                        pScsi->aCmd[2] = 0x3F; // page code. 
                        pScsi->aCmd[8] = 0xC0; // allocation length.
                    }
                    else
                    {
                        bCmdLength = 6;
                        bOpcode = MGC_SCSI_MODE_SENSE;
                        pScsi->aCmd[2] = 0x3F; // page code. 
                        pScsi->aCmd[4] = 0xC0; // allocation length.
                    }
                    pBuffer = pScsi->aModeSenseData;
                    bLength = 0xC0;

                    /* set common fields */
                    pScsi->aCmd[0] = bOpcode;

                    bSuccess = pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd,
                                                    bCmdLength, pBuffer, bLength, FALSE,
                                                    MGC_ScsiCheckMedium);

                    if (!bSuccess)
                    {
                        MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
                        // check next device.
                        bSuccess = MGC_ScsiCmdSetCheckMediumTimer((MUSB_BusHandle)pPort, 
                            pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
                        UNUSED(bSuccess);
                    }
                    return;                                        
                }
                else
                {
                    // wait medium to be stable.
                    pScsi->bNoMedium = TRUE;                    
                }
            }
            //  MTK Notice: Max Liao, 2006/11/17.
            //  Detect card reader when no card insert.	    
            else if (((uint8_t)MGC_SCSI_SC_UNIT_ATTENTION == (pScsi->aSenseData[2] & 0xf))
                         && ((MGC_SCSI_ASC_MEDIUM_NOT_PRESENT == pScsi->aSenseData[12])
                                 || (MGC_SCSI_ASC_MEDIUM_ERROR == pScsi->aSenseData[12])))
            {
                pScsi->bNoMedium = TRUE;
            }
            else if (((uint8_t)MGC_SCSI_SC_NOT_READY == (pScsi->aSenseData[2] & 0xf))
                         && (MGC_SCSI_ASC_MEDIUM_NOT_PRESENT == pScsi->aSenseData[12]))
            {
                pScsi->bNoMedium = TRUE;
            }
            else if (((uint8_t)MGC_SCSI_SC_NOT_READY == (pScsi->aSenseData[2] & 0xf))
                         && (pScsi->aSenseData[12] >= 0x80))  /*  0x80 ~ 0xFF is vendor specific */ 
            {
                pScsi->bNoMedium = TRUE;
            }
            else if ((uint8_t)MGC_SCSI_SC_NONE == (pScsi->aSenseData[2] & 0xf))
            {
                pScsi->bNoMedium = TRUE;
            }
            //  MTK Notice: Max Liao, 2006/11/20.
            //  handle illegal request.
            else if ((uint8_t)MGC_SCSI_SC_ILLEGAL_REQUEST == (pScsi->aSenseData[2] & 0xf))
            {
                MUSB_DIAG_STRING(1, "MSD: ILLEGAL_REQUEST");
            }
        }
    }

    if ((bWrapperStatus > 0) && (MGC_MSD_COMMAND_ABORT > bWrapperStatus))
    {
        /* command failed: find out why */
        pScsi->bError = TRUE;

        bCmdLength = 6;
        bOpcode = MGC_SCSI_REQUEST_SENSE;
        pBuffer = pScsi->aSenseData;
        //  MTK Notice: Max Liao, 2006/10/19.
        //  Bug: REQUEST_SENSE sense data is only 18 bytes.
        bLength = 18;
        pScsi->aCmd[0] = bOpcode;
        pScsi->aCmd[4] = bLength;

        bSuccess = pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd,
                                        bCmdLength, pBuffer, bLength, FALSE,
                                        MGC_ScsiCheckMedium);

        if (!bSuccess)
        {
            MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
            // check next device.
            bSuccess = MGC_ScsiCmdSetCheckMediumTimer((MUSB_BusHandle)pPort, 
                pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
            UNUSED(bSuccess);
        }
        return;
    }

    /* process result of last action and setup next one */
    switch (pScsi->bState)
    {
    case MGC_SCSI_STATE_TEST_MEDIUM_INSERT:
        LOG(7, "MSD: test medium insert = %d-%d-%d.\n", 
            pPort->bDeviceListIndex, pScsi->bLunCount, pScsi->bLun);
        if (!pScsi->bNoMedium)
        {
            if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
            {
                MUSB_ASSERT(pBot);
                pBotVolume = &(pBot->aVolume[0]);
                MUSB_ASSERT(pBotVolume);
                bSuccess = MGC_ScsiDiscoverDevice(pBotVolume->pCmdSetInstance, 
                    &(pBot->Protocol), 0);
            }
            else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                        (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
            {
                MUSB_ASSERT(pCbt);
                pCbtVolume = &(pCbt->aVolume[0]);
                MUSB_ASSERT(pCbtVolume);
                bSuccess = MGC_ScsiDiscoverDevice(pCbtVolume->pCmdSetInstance, 
                    &(pCbt->Protocol), 0);
            }  

            if (!bSuccess)
            {
                MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
                // check next device.
                bSuccess = MGC_ScsiCmdSetCheckMediumTimer((MUSB_BusHandle)pPort, 
                    pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
                UNUSED(bSuccess);
            }            
            return;
        }
        break;

    case MGC_SCSI_STATE_TEST_MEDIUM_REMOVE:
        LOG(7, "MSD: test medium remove = %d-%d-%d.\n", 
            pPort->bDeviceListIndex, pScsi->bLunCount, pScsi->bLun);
        if (pScsi->bNoMedium)
        {
            // medium is removed. start to update device information.
            MGC_ScsiAnnouceDevice(pScsi, FALSE);
            pScsi->bState = MGC_SCSI_STATE_TEST_MEDIUM_INSERT;
        }
        else
        {
            // check write protect status change.
            if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE10)
            {
                AccessType = (pScsi->aModeSenseData[3] & 0x80)
                                 ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
            }
            else if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE)
            {
                AccessType = (pScsi->aModeSenseData[2] & 0x80)
                                 ? MUSB_HFI_ACCESS_RANDOM_READ : MUSB_HFI_ACCESS_RANDOM;
            }

            if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
            {
                MUSB_ASSERT(pBot);
                pBotVolume = &(pBot->aVolume[bLun]);
                MUSB_ASSERT(pBotVolume);
                if (pBotVolume->MediumInfo.AccessType != AccessType)
                {
                    pBotVolume->MediumInfo.AccessType = AccessType;
                    
                    LOG(5, "MSD: bLun %d, AccessType change = %s.\n", bLun, 
                        ((AccessType == MUSB_HFI_ACCESS_RANDOM_READ) ? ("RO") : ("RW")));
                }
            }
            else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                        (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
            {
                MUSB_ASSERT(pCbt);
                pCbtVolume = &(pCbt->aVolume[bLun]);
                MUSB_ASSERT(pCbtVolume);
                if (pCbtVolume->MediumInfo.AccessType != AccessType)
                {
                    pCbtVolume->MediumInfo.AccessType = AccessType;

                    LOG(5, "MSD: bLun %d, AccessType change = %s.\n", bLun, 
                        ((AccessType == MUSB_HFI_ACCESS_RANDOM_READ) ? ("RO") : ("RW")));
                }
            }              
        }
        break;

    default:
        LOG(0, "MSD/SCSI: Internal error; bad state = %d ", pScsi->bState);
        break;
    }

    if (bLun < (pScsi->bLunCount - 1))
    {
        // check next LUN in this device.
        bLun++;

        if (MGC_MSD_BOT_PROTOCOL == pProtocol->bProtocol)
        {
            MUSB_ASSERT(pBot);
            pBotVolume = &(pBot->aVolume[bLun]);
            bSuccess = MGC_ScsiCmdSetCheckMedium(pBotVolume->pCmdSetInstance, 
                &(pBot->Protocol), bLun);                
        }
        else if ((MGC_MSD_CB_PROTOCOL == pProtocol->bProtocol) ||
                    (MGC_MSD_CBI_PROTOCOL == pProtocol->bProtocol))
        {
            MUSB_ASSERT(pCbt);
            pCbtVolume = &(pCbt->aVolume[bLun]);
            bSuccess = MGC_ScsiCmdSetCheckMedium(pCbtVolume->pCmdSetInstance, 
                &(pCbt->Protocol), bLun);
        }  

        if (!bSuccess)
        {
            MUSB_DIAG_STRING(1, "MSD/SCSI: Protocol SendCmd failed");
            // activate timer to check next device.
            bSuccess = MGC_ScsiCmdSetCheckMediumTimer((MUSB_BusHandle)pPort, 
                pDevice, MGC_MSD_CHECK_MEDIUM_TIME);
            UNUSED(bSuccess);    
        }
    }
    else
    {
        bSuccess = MGC_ScsiCmdSetCheckMediumTimer((MUSB_BusHandle)pPort, 
            NULL, MGC_MSD_CHECK_MEDIUM_TIME);
        UNUSED(bSuccess);    
        LOG(7, "Wait %d ms...\n\n", MGC_MSD_CHECK_MEDIUM_TIME);
    }
}

/*
 * Determine device characteristics with standard inquiry,
 * VPD S/N page, etc. and wait until device is ready
 */
static uint8_t MGC_ScsiCmdSetDiscoverDevice(void *pInstance, 
    MGC_MsdProtocol *pProtocol, uint8_t bLun)
{
    MGC_ScsiCmdSetData *pScsi;

    MUSB_ASSERT(pInstance);
    MUSB_ASSERT(pProtocol);
    pScsi = (MGC_ScsiCmdSetData *)pInstance;    
    
    pScsi->pProtocol = pProtocol;
    pScsi->bLun = bLun;

    /* clear retry counters etc. */
    pScsi->dwRetries = 0;
    pScsi->bRemovable = FALSE;
    pScsi->bNoMedium = FALSE;

    /* start things by sending INQUIRY command */
    pScsi->bState = MGC_SCSI_STATE_INQUIRY;
    MUSB_MemSet(pScsi->aCmd, 0, 16);
    pScsi->aCmd[0] = MGC_SCSI_INQUIRY;
    pScsi->aCmd[4] = 36;

    MUSB_ASSERT(pProtocol->pfSendCmd);
    return pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd, 6,
                                pScsi->aInquiryData, 36, FALSE, MGC_ScsiCmdComplete);
}

static uint8_t MGC_ScsiDiscoverDevice(void *pInstance, 
    MGC_MsdProtocol *pProtocol, uint8_t bLun)
{
    MGC_ScsiCmdSetData *pScsi;

    MUSB_ASSERT(pInstance);
    MUSB_ASSERT(pProtocol);
    pScsi = (MGC_ScsiCmdSetData *)pInstance;    
    
    pScsi->pProtocol = pProtocol;
    pScsi->bLun = bLun;

    /* clear retry counters etc. */
    pScsi->dwRetries = 0;
    pScsi->bRemovable = FALSE;
    pScsi->bNoMedium = FALSE;

    /* start things by sending INQUIRY command */
    pScsi->bState = MGC_SCSI_STATE_INQUIRY;
    MUSB_MemSet(pScsi->aCmd, 0, 16);
    pScsi->aCmd[0] = MGC_SCSI_INQUIRY;
    pScsi->aCmd[4] = 36;

    MUSB_ASSERT(pProtocol->pfSendCmd);
    return pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd, 6,
                                pScsi->aInquiryData, 36, FALSE, MGC_ScsiDiscoverComplete);
}

static uint8_t MGC_ScsiCmdSetCheckMedium(void *pInstance, 
    const MGC_MsdProtocol *pProtocol, uint8_t bLun)
{
    MGC_ScsiCmdSetData *pScsi;

    MUSB_ASSERT(pInstance);
    MUSB_ASSERT(pProtocol);
    pScsi = (MGC_ScsiCmdSetData *)pInstance;    

    // assume medium is inserted before test.
    pScsi->bNoMedium = FALSE;

    MUSB_MemSet(pScsi->aCmd, 0, 16);
    pScsi->aCmd[0] = MGC_SCSI_TEST_UNIT_READY;
    
    MUSB_ASSERT(pProtocol->pfSendCmd);    
    return pProtocol->pfSendCmd(pProtocol->pProtocolData, pScsi, NULL, bLun, pScsi->aCmd, 6, NULL, 0,
                                FALSE, MGC_ScsiCheckMedium);
}

// Search the next MSD device which is different to pTargetDevice and start timer to check it.
static uint8_t MGC_ScsiCmdSetCheckMediumTimer(MUSB_BusHandle hBus, 
    MUSB_Device *pTargetDevice, uint32_t dwCheckTime)
{
    MGC_Port *pPort = (MGC_Port *)hBus;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    uint16_t wStartIndex = 0;
    uint32_t dwStatus = 0;
     
    MUSB_ASSERT(pPort);

    //Timer Index MUSB_MSD_TIMER_IDX is reserved for Mass storage class driver.
    VERIFY(0 == MUSB_CancelTimer(hBus, MUSB_MSD_TIMER_IDX /*bTimerIndex*/));    

    // get 1st MSD device from ConnectedDeviceList.   
    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
    wStartIndex = (pPort->bDeviceListIndex >= wCount) ? 0: pPort->bDeviceListIndex;
    
    while (wIndex < wCount)
    {
        MUSB_ASSERT(wStartIndex < wCount);    
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wStartIndex);

        if ((pDevice) && 
            (pDevice != pTargetDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_CONNECT) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE))
        {
            pPort->bDeviceListIndex = wStartIndex + 1;

            LOG(7, "MSD device=0x%08X, index=%d, Count=%d\n", 
                (uint32_t)pDevice, wStartIndex, wCount);

            //Timer Index MUSB_MSD_TIMER_IDX is reserved for Mass storage class driver.
            dwStatus = MUSB_ArmTimer(hBus, MUSB_MSD_TIMER_IDX /*bTimerIndex*/, 
                dwCheckTime, MGC_ScsiTimerExpired, pDevice);
            VERIFY(0 == dwStatus);
                
            return MUSB_STATUS_OK;
        }

        wIndex ++;
        wStartIndex = (wStartIndex >= (wCount-1)) ? (0) : (wStartIndex+1); 
    }

    UNUSED(pTargetDevice);
    LOG(0, "MGC_ScsiCmdSetCheckMediumTimer Failed !\n");
    return MGC_M_STATUS_ERROR;
}

