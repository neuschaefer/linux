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
 * uC/FS HFI implementation (bridge between SICD class driver and uC/FS)
 * $Revision: #1 $
 */
#ifndef CC_USB_DISABLE
#include "mu_cdi.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_diag.h"
#include "mu_hfi_sicd_if.h"
#include "mu_descs.h"
#include "mu_impl.h"

#include "mu_sicdstate.h"
#include "mu_sicdapi.h"
#include "mu_sicdutil.h"

/****************************** TYPES *****************************/

/**************************** FORWARDS ****************************/
static int32_t MUSB_UcFsSicdDeviceIoCtl(uint32_t u4Index, uint32_t u4Cmd, 
                                                uint32_t u4Aux, void *pBuffer);
                                                
static int32_t MUSB_UcFsSicdDeviceRW(uint32_t u4Index, uint32_t u4Type, 
                                                    void *pvData);
                                                    
static int32_t MUSB_UcFsSicdDeviceInit(uint32_t u4Index);

/**************************** GLOBALS *****************************/
#ifndef MUSB_NONEOS
static HANDLE_T SICD_hSema;

#define SICD_CREATE_SEMAPHORE \
    VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&SICD_hSema, \
        X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK))

#define SICD_LOCK_SEMAPHORE \
    VERIFY(OSR_OK == x_sema_lock(SICD_hSema, X_SEMA_OPTION_WAIT))

#define SICD_UNLOCK_SEMAPHORE \
    VERIFY(OSR_OK == x_sema_unlock(SICD_hSema))

#else

// do not need to use semaphore to protect data.
#define SICD_CREATE_SEMAPHORE 
#define SICD_LOCK_SEMAPHORE
#define SICD_UNLOCK_SEMAPHORE

#endif

/** uC/FS units */
static MGC_UcFsDevice MUSB_aUcFsSicdDevice[MUSB_SICD_UNIT_COUNT];

static uint8_t SICD_bInited = FALSE;

/* This structure contains pointers to the functions that all drivers
   of the file system must provide. */
/* NOTE: An extern of this should be added to fs_dev.h */
static const MU_FS_DEVICE_TYPE MGC_FS_SICD_driver =
{
MUSB_SICD_UNIT_COUNT,       /* number of units */
MUSB_UcFsSicdDeviceIoCtl,   /* IO control interface */
MUSB_UcFsSicdDeviceRW,
NULL,
MUSB_UcFsSicdDeviceInit,
};

static int32_t _MUSBSicd_TranslateErrorCode(MUSB_HfiStatus eStatus, uint32_t u4Optype)
{
    int32_t iResult;
    switch ( eStatus ) 
    {
    case MUSB_HFI_ERROR_MEDIA_REMOVED:
        iResult = (int32_t)MUSB_SICD_RM_RT_MEDIA_REMOVED;
        break;
        
    case MUSB_HFI_ERROR_BUSY: 
        if ( u4Optype == (uint32_t)MUSB_SICD_RW_GET_EXTENDED_EVENT_DATA )
        {
            iResult = (int32_t)MUSB_SICD_RM_RT_NOT_IN_READY_PHASE;
        }
        else if ( (u4Optype == (uint32_t)MUSB_SICD_RW_GET_NEXT_DATA) ||
                  (u4Optype == (uint32_t)MUSB_SICD_RW_SET_OPERATION_ABORT) )  
        {
            iResult = (int32_t)MUSB_SICD_RM_RT_NOT_IN_DATA_PHASE;
        }
        else
        {
            iResult = (int32_t)MUSB_SICD_RM_RT_DEVICE_BUSY;
        }
        break;
        
    case MUSB_HFI_ERROR_VERIFY: 
        iResult = (int32_t)MUSB_SICD_RM_RT_ALIGNMENT_ERROR;
        break;
        
    case MUSB_HFI_ERROR_TRANSFER: 
        if ( u4Optype == (uint32_t)MUSB_SICD_RW_SET_SILENT_BUS_RESET )
        {
            // Since we no longer support SILENT_BUS_RESET to middleware, 
            // this error code will be used when using CLI. 
            iResult = (int32_t)MUSB_SICD_RM_RT_BUS_RESET_FAILED;
        }
        else
        {
            iResult = (int32_t)MUSB_SICD_RM_RT_USB_ERROR;
        }
        break;
    
    default : 
        iResult = (int32_t)MUSB_SICD_UNKNOWN_ERROR;
        break;
    }
    return iResult;
}

/*************************** FUNCTIONS ****************************/
static void MGC_UcFsSicdInit(void)
{
    uint8_t bIndex;

    LOG(7, "MGC_UcFsSicdInit().\n", NULL);

    SICD_bInited = TRUE;

    MUSB_MemSet(MUSB_aUcFsSicdDevice, 0, sizeof(MUSB_aUcFsSicdDevice));

    for (bIndex = 0; bIndex < (uint8_t)MUSB_SICD_UNIT_COUNT; bIndex++)
    {
        MUSB_aUcFsSicdDevice[bIndex].bIndex = bIndex;
    }

    // create sync semphore.
    SICD_CREATE_SEMAPHORE;
}

/* Implementation */
MUSB_HfiStatus MUSB_HfiSicdAddDevice(MUSB_HfiVolumeHandle *phVolume, 
                                            const MUSB_HfiDeviceInfo *pInfo,
                                            MUSB_HfiSicdDevice *pDevice)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_SicdVolume* pSicdVolume;
    MUSB_SicdProtocol* pSicdProtocol;
    MUSB_Device* pUsbDevice;    
    uint8_t bIndex;

    MUSB_ASSERT(phVolume);
    MUSB_ASSERT(pInfo);
    MUSB_ASSERT(pDevice);
    MUSB_ASSERT(SICD_bInited);

    SICD_LOCK_SEMAPHORE;

    /* use first available unit */
    for (bIndex = 0; bIndex < (uint8_t)MUSB_SICD_UNIT_COUNT; bIndex++)
    {
        pUcFsDevice = &(MUSB_aUcFsSicdDevice[bIndex]);
        if (!pUcFsDevice->pvDevice)
        {
            pUcFsDevice->pvDevice = (void*)pDevice;

            // save MUSB_Device.
            pSicdVolume = pDevice->pPrivateData;
            MUSB_ASSERT(pSicdVolume);
            pSicdProtocol = pSicdVolume->pSicdProtocol;
            MUSB_ASSERT(pSicdProtocol);
            pUsbDevice = pSicdProtocol->pDevice;
            MUSB_ASSERT(pUsbDevice);
            pUcFsDevice->pUsbDevice = (void *)pUsbDevice; /* save MUSB_Device* */
            MUSB_ASSERT(pUsbDevice->pPort);
            pUcFsDevice->bUsbPort = pUsbDevice->pPort->bUsbPort;
            // Driver set device attached socket number of parent 
            pUcFsDevice->bSocketNo = pUsbDevice->bHubPort;
            
            *phVolume = pUcFsDevice;

            LOG(7, "vid = 0x%04X, pid = 0x%04X, bMediumStatus = %d.\n",
                   pInfo->wVendorId,
                   pInfo->wProductId,
                   pUcFsDevice->bMediumStatus);

            SICD_UNLOCK_SEMAPHORE;

            return MUSB_HFI_SUCCESS;
        }
    }

    SICD_UNLOCK_SEMAPHORE;    

    return MUSB_HFI_NO_MEMORY;
}

/* Implementation */
void MUSB_HfiSicdMediumInserted(MUSB_HfiVolumeHandle hVolume)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    
    MUSB_ASSERT(hVolume);
    pUcFsDevice = (MGC_UcFsDevice *)hVolume;

    pUcFsDevice->bMediumStatus = HFI_STATUS_READY;

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
}

/* Implementation */
void MUSB_HfiSicdMediumRemoved(MUSB_HfiVolumeHandle hVolume)
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

    LOG(5, "SicdMediumRemoved().\n",  NULL);
}

/* Implementation */
void MUSB_HfiSicdDeviceRemoved(MUSB_HfiVolumeHandle hVolume)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_ASSERT(hVolume);
    pUcFsDevice = (MGC_UcFsDevice *)hVolume;

    SICD_LOCK_SEMAPHORE;
    
    pUcFsDevice->pvDevice = NULL;
    pUcFsDevice->pUsbDevice = NULL;    
    pUcFsDevice->bSocketNo = 0;
    pUcFsDevice->bUsbPort = 0;

    SICD_UNLOCK_SEMAPHORE;

    LOG(7, "SicdDeviceRemoved().\n",  NULL);
}

/* Implementation */
MU_FS_DEVICE_TYPE *MUSB_HfiSicdDriverGet(void)
{
    return (MU_FS_DEVICE_TYPE *)&MGC_FS_SICD_driver;
}

MGC_UcFsDevice* MUSB_UcFsSicdDeviceGet(void)
{
    if ( SICD_bInited )
    {
        return MUSB_aUcFsSicdDevice;
    }
    else
    {
        return NULL;
    }
}

static int32_t MUSB_UcFsSicdDeviceIoCtl(uint32_t u4Index, uint32_t u4Cmd, 
                                                uint32_t u4Aux, void *pBuffer)
{
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiSicdDevice *pDevice = NULL;
    MUSB_NFY_INFO_T *pt_nfy_info;
    MUSB_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MUSB_Device *pUsbDevice;
    
    MUSB_ASSERT(MUSB_SICD_UNIT_COUNT > u4Index);

    pUcFsDevice = &(MUSB_aUcFsSicdDevice[u4Index]);
    pDevice = (MUSB_HfiSicdDevice*)pUcFsDevice->pvDevice;
    
    /* This check makes sure, the device number is within valid range */
    switch (u4Cmd)
    {
    case MUSB_SICD_IOCTL_GET_SOCKET_NUM:
        pUsbDevice = (MUSB_Device *)u4Aux;

        if ((!pBuffer) ||(!pUsbDevice))
        { /* avoid writing into non existing buffers */
            return -1;
        }

        *((int32_t *)pBuffer) = (int32_t)pUsbDevice->bPortCount;
        break;
    
    case MUSB_SICD_IOCTL_GET_DEVICE_INFO:
        if (!pBuffer)
        {            
            return (int32_t)MUSB_SICD_RM_RT_ALIGNMENT_ERROR;
        }

        if ( !pDevice )
        {
            LOG(1, "SicdDeviceIoCtl : pDevice NULL\n", NULL);
            return (int32_t)MUSB_SICD_RM_RT_MEDIA_REMOVED;
        }
        VERIFY(MUSB_HFI_SUCCESS == 
                    pDevice->pfGetDeviceInfo(pDevice->pPrivateData, pBuffer));
        break;

    case MUSB_SICD_IOCTL_GET_DEVICE_STATUS:
        if ( !pBuffer )
        { /* avoid writing into non existing buffers */
            return (int32_t)MUSB_SICD_RM_RT_ALIGNMENT_ERROR;
        }

        *((int32_t *)pBuffer) = (int32_t)pUcFsDevice->bMediumStatus;
        break;

    case MUSB_SICD_IOCTL_DP_CTRL_BY_MW : 
        if ( !pDevice )
        {
            LOG(1, "SicdDeviceIoCtl : pDevice NULL\n", NULL);
            return (int32_t)MUSB_SICD_RM_RT_MEDIA_REMOVED;
        }
        
        SicdIoData.u2OperationCode = (uint16_t)MUSB_SICD_DATAPHASE_CTRL_BY_MW;
        (void)pDevice->pfWriteDevice((void*)pUcFsDevice, &SicdIoData);
        break;

    case MUSB_SICD_IOCTL_DP_CTRL_BY_SICD : 
        if ( !pDevice )
        {
            LOG(1, "SicdDeviceIoCtl : pDevice NULL\n", NULL);
            return (int32_t)MUSB_SICD_RM_RT_MEDIA_REMOVED;
        }
        
        SicdIoData.u2OperationCode = (uint16_t)MUSB_SICD_DATAPHASE_CTRL_BY_SICD;
        (void)pDevice->pfWriteDevice((void*)pUcFsDevice, &SicdIoData);
        break;

    case MUSB_SICD_IOCTL_SET_TYPENFY:
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

    case MUSB_SICD_IOCTL_SET_HOTSWAP_NFY:
        MUSB_ASSERT(pBuffer);
        
        pt_hotswap_nfy_info = (MUSB_HOTSWAP_NFY_INFO_T *)pBuffer;
        pUcFsDevice->pt_socket_nfy_info = pt_hotswap_nfy_info;

        // call back when device already insert.
        if ((pUcFsDevice->pvDevice) && (pUcFsDevice->bMediumStatus != HFI_STATUS_ABSENT))
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
           
    default:
        MUSB_ASSERT(0);
        break;
    }
    
    UNUSED(u4Aux);
    return MUSB_SICD_RM_RT_OK; /* Return with zero if no problems have occurred. */
}

static int32_t MUSB_UcFsSicdDeviceRW(uint32_t u4Index, uint32_t u4Optype, 
                                                        void *pvData)
{
    int32_t iResult = MUSB_SICD_RM_RT_OK;
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiSicdDevice *pDevice;
    MUSB_HfiStatus eStatus = MUSB_HFI_SUCCESS;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MUSB_SICD_IO_DATA_T *pt_data = (MUSB_SICD_IO_DATA_T *)pvData;

    // check parameter.    
    if ( u4Index >= MUSB_SICD_UNIT_COUNT )
    {
        return (int32_t)MUSB_SICD_UNKNOWN_ERROR;
    }

    SICD_LOCK_SEMAPHORE;
    
    pUcFsDevice = &(MUSB_aUcFsSicdDevice[u4Index]);
    pDevice = (MUSB_HfiSicdDevice*)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pUcFsDevice->rt_nfy_info.pfHfiNfy);

    if ((pDevice) && (pUcFsDevice->bMediumStatus == HFI_STATUS_READY))
    {
        pUcFsDevice->pvData = pvData;

        switch ( u4Optype )
        {
        case MUSB_SICD_RW_SEND_OPERATION : 
            MUSB_ASSERT(pt_data);
            eStatus = pDevice->pfReadDevice((void*)pUcFsDevice, pt_data);
            break;

        case MUSB_SICD_RW_GET_EXTENDED_EVENT_DATA : 
            SicdIoData.u2OperationCode = (uint16_t)MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST;
            eStatus = pDevice->pfWriteDevice((void*)pUcFsDevice, &SicdIoData);
            break;
            
        case MUSB_SICD_RW_SET_DEVICE_RESET : 
            SicdIoData.u2OperationCode = (uint16_t)MUSB_SICD_RESET_REQUEST;
            eStatus = pDevice->pfWriteDevice((void*)pUcFsDevice, &SicdIoData);
            break;
            
        case MUSB_SICD_RW_SET_OPERATION_ABORT : 
            SicdIoData.u2OperationCode = (uint16_t)MUSB_SICD_ABORT_REQUEST;
            eStatus = pDevice->pfWriteDevice((void*)pUcFsDevice, &SicdIoData);
            break;

        case MUSB_SICD_RW_GET_NEXT_DATA : 
            MUSB_ASSERT(pt_data);            
            pt_data->u2OperationCode = (uint16_t)MUSB_SICD_GET_NEXT_DATA;
            eStatus = pDevice->pfWriteDevice((void*)pUcFsDevice, pt_data);
            break;

        // Not expose to middleware, this is only used in CLI.
        case MUSB_SICD_RW_SET_SILENT_BUS_RESET : 
            SicdIoData.u2OperationCode = (uint16_t)MUSB_SICD_SET_SILENT_BUS_RESET;
            eStatus = pDevice->pfWriteDevice((void*)pUcFsDevice, &SicdIoData);
            break;
            
        case MUSB_SICD_RW_SEND_NEXT_DATA:
            // TODO
            MUSB_ASSERT(0);
            break;

        default: 
            LOG(1, "SicdDeviceRW : error %d.\n", u4Optype);
            return -1;
        }

        if (eStatus != MUSB_HFI_SUCCESS)
        {
            iResult = _MUSBSicd_TranslateErrorCode(eStatus, u4Optype);
        }
    }
    else
    {
        iResult = (int32_t)MUSB_SICD_RM_RT_USB_ERROR;
    }
    

    SICD_UNLOCK_SEMAPHORE;
    
    return iResult;
}

static int32_t MUSB_UcFsSicdDeviceInit(uint32_t u4Index)
{
    if (!SICD_bInited)
    {
        MGC_UcFsSicdInit();
    }

    MUSB_ASSERT(SICD_bInited);
    UNUSED(u4Index);    
    return 0;
}

#endif

