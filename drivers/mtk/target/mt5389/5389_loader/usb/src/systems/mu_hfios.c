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
 * uC/FS HFI implementation (bridge between mass-storage class
 * driver and uC/FS)
 * $Revision: #1 $
 */
#ifndef CC_USB_DISABLE

#include "mu_cdi.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_diag.h"
#include "mu_hfi_if.h"
#include "mu_mpcsi.h"
#include "mu_descs.h"
#include "mu_impl.h"
#include "musb_if.h"
#include "mu_msd.h"

#ifdef MUSB_HUB
#include "mu_hub.h"
#include "mu_hdef.h"
#include "mu_hcreq.h"
#include "mu_hcore.h"
#include "mu_hapi.h"
#endif

/****************************** TYPES *****************************/

/**************************** FORWARDS ****************************/
static int32_t MGC_UcFsDeviceIoCtl(uint32_t u4Index, uint32_t u4Cmd, 
    uint32_t u4Aux, void *pBuffer);
static int32_t MGC_UcFsDeviceRW(uint32_t u4Index, uint32_t u4Type, 
    void *pvData);
static int32_t MGC_UcFsDeviceWriteProtect(uint32_t u4Index, 
    void *pvData);
static int32_t MGC_UcFsDeviceInit(uint32_t u4Index);

/**************************** GLOBALS *****************************/
#ifndef MUSB_NONEOS
static HANDLE_T MGC_hSema;

#define MGC_CREATE_SEMAPHORE \
    VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&MGC_hSema, \
        X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK))

#define MGC_LOCK_SEMAPHORE \
    VERIFY(OSR_OK == x_sema_lock(MGC_hSema, X_SEMA_OPTION_WAIT))

#define MGC_UNLOCK_SEMAPHORE \
    VERIFY(OSR_OK == x_sema_unlock(MGC_hSema))

#else

// do not need to use semaphore to protect data.
#define MGC_CREATE_SEMAPHORE 
#define MGC_LOCK_SEMAPHORE
#define MGC_UNLOCK_SEMAPHORE

#endif

/** uC/FS units */
static MGC_UcFsDevice MGC_aUcFsDevice[MUSB_UCFS_UNIT_COUNT];

static uint8_t MGC_bInited = FALSE;

/* This structure contains pointers to the functions that all drivers
   of the file system must provide. */
/* NOTE: An extern of this should be added to fs_dev.h */
static const MU_FS_DEVICE_TYPE MGC_FS_driver =
{
MUSB_UCFS_UNIT_COUNT, /* number of units */
MGC_UcFsDeviceIoCtl,  /* IO control interface */
MGC_UcFsDeviceRW,
MGC_UcFsDeviceWriteProtect,
MGC_UcFsDeviceInit,
};

/*************************** FUNCTIONS ****************************/

static void MGC_UcFsInit(void)
{
    uint8_t bIndex;

    LOG(7, "MGC_UcFsInit().\n", NULL);

    MGC_bInited = TRUE;

    MUSB_MemSet(MGC_aUcFsDevice, 0, sizeof(MGC_aUcFsDevice));

    for (bIndex = 0; bIndex < (uint8_t)MUSB_UCFS_UNIT_COUNT; bIndex++)
    {
        MGC_aUcFsDevice[bIndex].bIndex = bIndex;
    }

    // create sync semphore.
    MGC_CREATE_SEMAPHORE;
}

/* Implementation */
MUSB_HfiStatus MUSB_HfiAddDevice(MUSB_HfiVolumeHandle *phVolume, const MUSB_HfiDeviceInfo *pInfo,
                                 MUSB_HfiDevice *pDevice)
{
    MGC_UcFsDevice *pUcFsDevice;
    MGC_MsdBotVolume* pBotVolume;
    MGC_MsdBotProtocol* pBot;
    MGC_MsdCbtVolume* pCbtVolume;
    MGC_MsdCbtProtocol* pCbt;
    MUSB_Device* pUsbDevice = NULL;        
    uint8_t bIndex;

    MUSB_ASSERT(phVolume);
    MUSB_ASSERT(pInfo);
    MUSB_ASSERT(pDevice);
    MUSB_ASSERT(MGC_bInited);

    MGC_LOCK_SEMAPHORE;

    /* use first available unit */
    for (bIndex = 0; bIndex < (uint8_t)MUSB_UCFS_UNIT_COUNT; bIndex++)
    {
        pUcFsDevice = &(MGC_aUcFsDevice[bIndex]);
        if (!pUcFsDevice->pvDevice)
        {
            pUcFsDevice->pvDevice = (void *)pDevice;

            if (pInfo->bProtocol == MGC_MSD_BOT_PROTOCOL)
            {
                pBotVolume = (MGC_MsdBotVolume *)pDevice->pPrivateData;
                MUSB_ASSERT(pBotVolume);
                pUcFsDevice->bSocketNo = pBotVolume->bLun;
                
                pBot = (MGC_MsdBotProtocol *)pBotVolume->pBot;
                MUSB_ASSERT(pBot);        
                pUsbDevice = pBot->pDevice;
            }
            else if ((pInfo->bProtocol == MGC_MSD_CB_PROTOCOL) ||
                        (pInfo->bProtocol == MGC_MSD_CBI_PROTOCOL))
            {
                pCbtVolume = (MGC_MsdCbtVolume *)pDevice->pPrivateData;
                MUSB_ASSERT(pCbtVolume);
                pUcFsDevice->bSocketNo = pCbtVolume->bLun;

                pCbt = (MGC_MsdCbtProtocol *)pCbtVolume->pCbt;
                MUSB_ASSERT(pCbt);           
                pUsbDevice = pCbt->pDevice;
            }            

            MUSB_ASSERT(pUsbDevice);
            if (pUsbDevice)
            {
                pUcFsDevice->pUsbDevice = (void *)pUsbDevice; /* save MUSB_Device* */
                MUSB_ASSERT(pUsbDevice->pPort);
                if (pUsbDevice->pPort)
                {
                    pUcFsDevice->bUsbPort = pUsbDevice->pPort->bUsbPort;
                }
            }
            else
            {
                pUcFsDevice->pUsbDevice = NULL;
                pUcFsDevice->bUsbPort = 0;
            }

            *phVolume = pUcFsDevice;

            LOG(7, "HfiAddDev bIndex = %d.\n", bIndex);

            LOG(0, "vid = 0x%04X, pid = 0x%04X, bMediumStatus = %d.\n",
                   pInfo->wVendorId,
                   pInfo->wProductId,
                   pUcFsDevice->bMediumStatus);

            MGC_UNLOCK_SEMAPHORE;
            
            return MUSB_HFI_SUCCESS;
        }
    }

    MGC_UNLOCK_SEMAPHORE;
    
    return MUSB_HFI_NO_MEMORY;
}

/* Implementation */
void MUSB_HfiMediumInserted(MUSB_HfiVolumeHandle hVolume, const MUSB_HfiMediumInfo *pMediumInfo)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    
    MUSB_ASSERT(hVolume);
    MUSB_ASSERT(pMediumInfo);
    pUcFsDevice = (MGC_UcFsDevice *)hVolume;

    if (pMediumInfo->AccessType != MUSB_HFI_ACCESS_FAIL)
    {
        pUcFsDevice->bMediumStatus = HFI_STATUS_READY;
    }
    else
    {
        pUcFsDevice->bMediumStatus = HFI_STATUS_FAIL;
        LOG(1, "Init Medium Fail.\n",  NULL);
    }

    LOG(0, "MUSB_HfiMediumInserted().\n",  NULL);

    // handle socket call back function.
    pt_hotswap_nfy_info = pUcFsDevice->pt_socket_nfy_info;    
    if ((pt_hotswap_nfy_info) && (pt_hotswap_nfy_info->pfNfy))
    {
        pt_hotswap_nfy_info->u4CompId = pUcFsDevice->bIndex;
        pt_hotswap_nfy_info->pvDevice = pUcFsDevice->pUsbDevice;
        pt_hotswap_nfy_info->u4Status = (uint32_t)pUcFsDevice->bMediumStatus;
        pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pUcFsDevice->bSocketNo;
        pt_hotswap_nfy_info->u4UsbPort = pUcFsDevice->bUsbPort;
        pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
    }

    LOG(7, "AccessType = %d, dwBlockSize = %d, dwBlockCountLo = 0x%08X.\n", pMediumInfo->AccessType,
           pMediumInfo->dwBlockSize, pMediumInfo->dwBlockCountLo);
}

/* Implementation */
void MUSB_HfiMediumRemoved(MUSB_HfiVolumeHandle hVolume)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    
    MUSB_ASSERT(hVolume);
    pUcFsDevice = (MGC_UcFsDevice *)hVolume;

    pUcFsDevice->bMediumStatus = HFI_STATUS_ABSENT;
    
    // handle socket call back function.
    pt_hotswap_nfy_info = pUcFsDevice->pt_socket_nfy_info;    
    if ((pt_hotswap_nfy_info) && (pt_hotswap_nfy_info->pfNfy))
    {
        pt_hotswap_nfy_info->u4CompId = pUcFsDevice->bIndex;    
        pt_hotswap_nfy_info->pvDevice = pUcFsDevice->pUsbDevice;
        pt_hotswap_nfy_info->u4Status = HFI_STATUS_ABSENT;
        pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pUcFsDevice->bSocketNo;
        pt_hotswap_nfy_info->u4UsbPort = pUcFsDevice->bUsbPort;        
        pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);   
    }
    
    LOG(0, "MUSB_HfiMediumRemoved().\n",  NULL);
}

/* Implementation */
void MUSB_HfiDeviceRemoved(MUSB_HfiVolumeHandle hVolume)
{
    MGC_UcFsDevice *pUcFsDevice;

    MUSB_ASSERT(hVolume);    
    pUcFsDevice = (MGC_UcFsDevice *)hVolume;

    MGC_LOCK_SEMAPHORE;

    pUcFsDevice->pvDevice = NULL;
    pUcFsDevice->pUsbDevice = NULL;
    pUcFsDevice->bSocketNo = 0;
    pUcFsDevice->bUsbPort = 0;

    MGC_UNLOCK_SEMAPHORE;

    LOG(0, "MUSB_HfiDeviceRemoved().\n",  NULL);
}

/* Implementation */
MU_FS_DEVICE_TYPE *MUSB_HfiDriverGet(void)
{
    return (MU_FS_DEVICE_TYPE *)&MGC_FS_driver;
}

static void MUSB_HfiChildDeviceUpdate(MUSB_Device *pHubUsbDevice, 
    MUSB_Device *pChildUsbDevice, uint32_t dwChildUsbDeviceStatus)
{
    uint32_t u4Status = 0;
    MUSB_Port* pPort;
    MUSB_SOCKET_NFY_INFO_T *prSocketInfo;
    
    UNUSED(pHubUsbDevice);
    MUSB_ASSERT(pChildUsbDevice);

    if (dwChildUsbDeviceStatus == MUSB_STATUS_CHILD_CONNECT)
    {
        switch (pChildUsbDevice->bDeviceClass)
        {
            case MUSB_CLASS_WIRELESS:
            case MUSB_CLASS_HID:
            case MUSB_CLASS_SICD:
            case MUSB_CLASS_MASS_STORAGE:
                 u4Status = (uint32_t)USB_STATUS_DEV_READY;
                break;
            case MUSB_CLASS_HUB:
                 u4Status = (uint32_t)USB_STATUS_HUB_READY;
                break;
            default:
                 u4Status = (uint32_t)USB_STATUS_UNSUPPORT;
                break;
        }          
    }
    else if (dwChildUsbDeviceStatus == MUSB_STATUS_CHILD_DISCONNECT)
    {
        switch (pChildUsbDevice->bDeviceClass)
        {
            case MUSB_CLASS_WIRELESS:        
            case MUSB_CLASS_HID:        
            case MUSB_CLASS_SICD:
            case MUSB_CLASS_MASS_STORAGE:
                 u4Status = (uint32_t)USB_STATUS_DEV_ABSENT;
                break;
            case MUSB_CLASS_HUB:
                 u4Status = (uint32_t)USB_STATUS_HUB_ABSENT;
                break;
            default:
                 u4Status = (uint32_t)USB_STATUS_UNSUPPORT_ABSENT;
                break;
        }          
    }
    else if (dwChildUsbDeviceStatus == MUSB_STATUS_CHILD_CONNECT_UNSUPPORT)
    {
        switch (pChildUsbDevice->bDeviceClass)
        {
            case MUSB_CLASS_HUB:
                 u4Status = (uint32_t)USB_STATUS_UNSUPPORT_HUB;
                break;

            default:
                 u4Status = (uint32_t)USB_STATUS_UNSUPPORT;
                break;
        }          
    }
    else if (dwChildUsbDeviceStatus == MUSB_STATUS_CHILD_DISCONNECT_UNSUPPORT)
    {
        switch (pChildUsbDevice->bDeviceClass)
        {
            case MUSB_CLASS_HUB:
                 u4Status = (uint32_t)USB_STATUS_UNSUPPORT_HUB_ABSENT;
                break;

            default:
                 u4Status = (uint32_t)USB_STATUS_UNSUPPORT_ABSENT;
                break;
        }          
    }        
        
    // execute ap callback function.
    pPort = pChildUsbDevice->pPort;
    MUSB_ASSERT(pPort);
    prSocketInfo = pPort->prSocketInfo;
    if ((prSocketInfo) && (prSocketInfo->pfSocketNfy))
    {   
        prSocketInfo->u4DeviceClass = pChildUsbDevice->bDeviceClass;
        prSocketInfo->u4Status = u4Status;
        prSocketInfo->pvParentDevice = pHubUsbDevice;
        prSocketInfo->pvDevice = pChildUsbDevice;
        // Check if device on hub. If device on hub, bHubPort starts from 1 ~ XX.
        // If device is on root, bHubPort must be 0.
        if (pHubUsbDevice)
        {
            MUSB_ASSERT(pChildUsbDevice->bHubPort > 0);
            // u4SocketNo starts from 0 ~ XX.
            prSocketInfo->u4SocketNo = pChildUsbDevice->bHubPort - 1;       
        }
        else
        {
            MUSB_ASSERT(pChildUsbDevice->bHubPort == 0);
            prSocketInfo->u4SocketNo = 0;
        }

        prSocketInfo->pfSocketNfy((void *)prSocketInfo);
    }
    
    return;
}

static int32_t MGC_UcFsDeviceIoCtl(uint32_t u4Index, uint32_t u4Cmd, uint32_t u4Aux, void *pBuffer)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;
    MUSB_Device *pUsbDevice;
    MUSB_NFY_INFO_T *pt_nfy_info;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    MGC_Port *pPort;    
    MUSB_Port *pUsbPort = NULL;
    uint16_t wPort;
    uint16_t wCount;
    uint16_t wIndex;
    uint8_t bMSDPortCount;
    
    MUSB_ASSERT(MUSB_UCFS_UNIT_COUNT > u4Index);

    /* This check makes sure, the device number is within valid range */
    switch (u4Cmd)
    {
    case MUSB_IOCTL_GET_MEDIUM_INFO:
        pUcFsDevice = &(MGC_aUcFsDevice[u4Index]);    
        pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
        if ((!pBuffer) ||(!pDevice))
        {            
            return -1;
        }

        VERIFY(MUSB_HFI_SUCCESS == pDevice->pfGetMediumInfo(pDevice->pPrivateData, pBuffer));
        break;

    case MUSB_IOCTL_GET_DEVICE_INFO:
        pUcFsDevice = &(MGC_aUcFsDevice[u4Index]);        
        pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
        if ((!pBuffer) ||(!pDevice))
        {            
            return -1;
        }
        VERIFY(MUSB_HFI_SUCCESS == pDevice->pfGetDeviceInfo(pDevice->pPrivateData, pBuffer));
        break;

    case MUSB_IOCTL_GET_DEVICE_STATUS:
        if (!pBuffer)
        { /* avoid writing into non existing buffers */
            return -1;
        }

        pUcFsDevice = &(MGC_aUcFsDevice[u4Index]);        
        *((int32_t *)pBuffer) = (int32_t)pUcFsDevice->bMediumStatus;
        break;

    case MUSB_IOCTL_SET_TYPENFY:
        pUcFsDevice = &(MGC_aUcFsDevice[u4Index]);        
        pt_nfy_info = (MUSB_NFY_INFO_T *)pBuffer;
        if (pt_nfy_info)
        {
            pUcFsDevice->rt_nfy_info.pvHfiNfyTag = pt_nfy_info->pvHfiNfyTag;
            pUcFsDevice->rt_nfy_info.pfHfiNfy = pt_nfy_info->pfHfiNfy;        
        }
        else
        {
            pUcFsDevice->rt_nfy_info.pvHfiNfyTag = NULL;
            pUcFsDevice->rt_nfy_info.pfHfiNfy = NULL;
        }
        break;

    case MUSB_IOCTL_SET_SOCKET_DEV_NFY:
        MUSB_ASSERT(pBuffer);    
        pt_hotswap_nfy_info = (MUSB_HOTSWAP_NFY_INFO_T *)pBuffer;

        pUcFsDevice = &(MGC_aUcFsDevice[u4Index]); 
        pUcFsDevice->pt_socket_nfy_info = pt_hotswap_nfy_info;

        // call back when device already insert.
        pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
        if ((pDevice) && (pUcFsDevice->bMediumStatus != HFI_STATUS_ABSENT))
        {
            if (pt_hotswap_nfy_info->pfNfy)
            {
                pt_hotswap_nfy_info->pvDevice = pUcFsDevice->pUsbDevice;
                pt_hotswap_nfy_info->u4Status = (uint32_t)pUcFsDevice->bMediumStatus;
                pt_hotswap_nfy_info->u4SocketNo = (uint32_t)pUcFsDevice->bSocketNo;
                pt_hotswap_nfy_info->u4UsbPort = pUcFsDevice->bUsbPort;                        
                pt_hotswap_nfy_info->pfNfy(pt_hotswap_nfy_info->pvTag);
            }
        }                            
        break;

    case MUSB_IOCTL_SET_SOCKET_NFY:
        // u4Aux is component id.
        MUSB_ASSERT(u4Aux < MUSB_MAX_CONTROLLERS);
        pUsbPort = MUSB_GetPort(u4Aux);
        MUSB_ASSERT(pUsbPort);     
        if (!pUsbPort)
        {
            return -1;
        }
        pUsbPort->prSocketInfo = (MUSB_SOCKET_NFY_INFO_T *)pBuffer;
        
        pPort = (MGC_Port *)pUsbPort->pPrivateData;
        MUSB_ASSERT(pPort);
        // call pfChildDeviceUpdate() if device is already inserted.       
        wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
        for (wIndex = 0; wIndex < wCount; wIndex++)
        {
            pUsbDevice = (MUSB_Device *)MUSB_ListFindItem(
                &(pPort->ConnectedDeviceList), wIndex);
            if (pUsbDevice)
            {
                MUSB_HfiChildDeviceUpdate(pUsbDevice->pParentUsbDevice, 
                    pUsbDevice, pUsbDevice->bSupport);
            }
        }
        break;

    case MUSB_IOCTL_GET_SOCKET_NUM:
        pUsbDevice = (MUSB_Device *)u4Aux;

        if ((!pBuffer) ||(!pUsbDevice))
        { /* avoid writing into non existing buffers */
            return -1;
        }

        *((int32_t *)pBuffer) = (int32_t)pUsbDevice->bPortCount;
        break;

    case MUSB_IOCTL_GET_TOTAL_MSD_SOCKET_NUM:
        if (!pBuffer)
        { /* avoid writing into non existing buffers */
            return -1;
        }

        bMSDPortCount = 0;
        
        for (wPort = 0; wPort < MUSB_MAX_CONTROLLERS; wPort++)
        {
            pUsbPort = MUSB_GetPort(wPort);
            MUSB_ASSERT(pUsbPort);
            if (pUsbPort)
            {
                pPort = (MGC_Port *)pUsbPort->pPrivateData;
                MUSB_ASSERT(pPort);
                if (pPort)
                {
                    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));
                    for (wIndex = 0; wIndex < wCount; wIndex++)
                    {
                        pUsbDevice = (MUSB_Device *)MUSB_ListFindItem(
                            &(pPort->ConnectedDeviceList), wIndex);

                        if ((pUsbDevice) && 
                            (pUsbDevice->bSupport == MUSB_STATUS_CHILD_CONNECT) &&
                            (pUsbDevice->bDeviceClass == MUSB_CLASS_MASS_STORAGE))
                        {
                            bMSDPortCount += pUsbDevice->bPortCount;                
                        }                    
                    }
                }            
            }
        }

        *((int32_t *)pBuffer) = (int32_t)bMSDPortCount;
        break;
        
    case MUSB_IOCTL_SET_OCNFY:
        // u4Aux is component id.
        MUSB_ASSERT(u4Aux < MUSB_MAX_CONTROLLERS);
        pUsbPort = MUSB_GetPort(u4Aux);
        MUSB_ASSERT(pUsbPort);     
        if (pUsbPort)
        {
            pUsbPort->pfOverCurrentNfy = (MUSB_pfOverCurrentNfy)((uint32_t)pBuffer);
        }
        
        break;
        
    default:
        break;
    }

    UNUSED(u4Aux);
    return 0; /* Return with zero if no problems have occurred. */
}

static int32_t MGC_UcFsDeviceRW(uint32_t u4Index, uint32_t u4Optype, 
    void *pvData)
{
    MUSB_HfiStatus eStatus;

    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;
    int32_t iResult = -1;
    MUSB_IO_DATA_T *pt_data = (MUSB_IO_DATA_T *)pvData;

    // check parameter.    
    if ((!pt_data) || (u4Index >= MUSB_UCFS_UNIT_COUNT) || (!pt_data->pvData)
            || (u4Optype < MUSB_HFI_OP_READ) || (u4Optype > MUSB_HFI_OP_WRITE))
    {
        return iResult;
    }
    
    MGC_LOCK_SEMAPHORE;
    
    pUcFsDevice = &(MGC_aUcFsDevice[u4Index]);
    pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pUcFsDevice->rt_nfy_info.pfHfiNfy);

    if ((pDevice) && (pUcFsDevice->bMediumStatus == HFI_STATUS_READY))
    {
        pUcFsDevice->pvData = pvData;

        if (u4Optype == MUSB_HFI_OP_READ)
        {
            eStatus = pDevice->pfReadDevice(pUcFsDevice, pt_data->u8BlkNum,
                                            pt_data->u4Count, (uint8_t *)pt_data->pvData);
        }
        else
        {
            eStatus = pDevice->pfWriteDevice(pUcFsDevice, pt_data->u8BlkNum,
                                             pt_data->u4Count, (uint8_t *)pt_data->pvData);
        }

        /* pend on semaphore for completion */
        if (eStatus == MUSB_HFI_SUCCESS)
        {
            iResult = 0;
        }
    }

    MGC_UNLOCK_SEMAPHORE;

    return iResult;
}

static int32_t MGC_UcFsDeviceWriteProtect(uint32_t u4Index, 
    void *pvData)
{
    MUSB_HfiStatus eStatus;

    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;
    int32_t iResult = -1;

    // check parameter.    
    if (u4Index >= MUSB_UCFS_UNIT_COUNT)
    {
        return iResult;
    }
    
    MGC_LOCK_SEMAPHORE;
    
    pUcFsDevice = &(MGC_aUcFsDevice[u4Index]);
    pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pUcFsDevice->rt_nfy_info.pfHfiNfy);

    if ((pDevice) && (pUcFsDevice->bMediumStatus == HFI_STATUS_READY))
    {
        pUcFsDevice->pvData = pvData;
    
        eStatus = pDevice->pfWriteProtect(pUcFsDevice);

        /* pend on semaphore for completion */
        if (eStatus == MUSB_HFI_SUCCESS)
        {
            iResult = 0;
        }
    }

    MGC_UNLOCK_SEMAPHORE;

    return iResult;
}

static int32_t MGC_UcFsDeviceInit(uint32_t u4Index)
{
    MGC_Port *pPort;    
    MUSB_Port *pUsbPort = NULL;
    MGC_EnumerationData *pEnumData;
    uint32_t i;

    if (!MGC_bInited)
    {
        MGC_UcFsInit();

        // Init call back function.
        for (i=0; i<MUSB_CountPorts(); i++)
        {
            pUsbPort = MUSB_GetPort(i);
            if (!pUsbPort)
            {
                return -1;
            }
            pPort = (MGC_Port *)pUsbPort->pPrivateData;
            MUSB_ASSERT(pPort);
            pEnumData = &(pPort->EnumerationData);
            MUSB_ASSERT(pEnumData);
            pEnumData->pfChildDeviceUpdate = MUSB_HfiChildDeviceUpdate;        
        }
    }

    MUSB_ASSERT(MGC_bInited);
    UNUSED(u4Index);    
    return 0;
}

#endif

