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
 * MUSBStack-S mass-storage CBI/CB Protocol implementation
 * $Revision: #1 $
 */

#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_hfi_if.h"
#include "mu_mem.h"

#include "mu_mpcsi.h"
#include "mu_mapi.h"
#include "mu_impl.h"

#include "mu_scsi.h"

#define MGC_MSD_CBT_CMD_RETRY   (5)
#define MGC_MSD_CBT_DATA_RETRY   (10)
#define MGC_MSD_CBT_RETRY   (3)

/* Mass Storage Device CBT Command Execution State */
#define MGC_MSD_CBT_COMMAND_PASSED     0x00
#define MGC_MSD_CBT_COMMAND_FAILED      0x01
#define MGC_MSD_CBT_PHASE_ERROR             0x02

//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_CBT_TRANS_SIZE, 
// Mcbt driver will concatenate each MAX_MSD_CBT_TRANS_SIZE 
// size of transfer to fulfill with user's request.
#define MAX_MSD_CBT_TRANS_SIZE   (0x10000)
 
/******************************* FORWARDS ********************************/

static uint8_t MGC_CbtProtocolStartDevice(void *pProtocolData, const MUSB_Device *pDevice);

static uint32_t MGC_CbtProtocolRWRetry(MGC_MsdCbtIrp *pCbtIrp);

static void MGC_CbtProtocolCmdComplete(void *pPrivateData, 
                int32_t e_nfy_cond, void *pv_data, BOOL fgIsrCallback);

static uint8_t MGC_CbtProtocolSendCmd(void *pProtocolData, void *pPrivateData, MUSB_IO_DATA_T *pt_data,
                                      uint8_t bLun, const uint8_t *pCmdBuffer,
                                      uint8_t bCmdLength, uint8_t *pDataBuffer,
                                      uint32_t dwDataLength, uint8_t bSend,
                                      MGC_pfMsdProtocolCmdComplete pfCmdComplete);

static uint8_t MGC_CbtProtocolSetDeviceInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiDeviceInfo *pInfo);

static uint8_t MGC_CbtProtocolSetMediumInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiMediumInfo *pInfo);

static void MGC_CbtProtocolSetReady(void *pProtocolData, uint8_t bLun, uint8_t bIsReady);

static void MGC_CbtDataComplete(void *pParam, const MUSB_Irp *pIrp);

static MUSB_HfiStatus MGC_MsdCbtGetMediumInfo(void *pDeviceData, MUSB_HfiMediumInfo **ppMediumInfo);

static MUSB_HfiStatus MGC_MsdCbtGetDeviceInfo(void *pDeviceData, MUSB_HfiDeviceInfo **ppDeviceInfo);

static MUSB_HfiStatus MGC_MsdCbtReadDevice(void *pDeviceData,
                                           uint64_t ddwStartBlock,
                                           uint32_t dwByteCount,
                                           uint8_t *pBuffer);

static MUSB_HfiStatus MGC_MsdCbtWriteDevice(void *pDeviceData,
                                            uint64_t ddwStartBlock,
                                            uint32_t dwByteCount,
                                            uint8_t *pBuffer);

static MUSB_HfiStatus MGC_MsdCbtWriteProtect(void *pDeviceData);

static void MGC_MsdCbtVolumeInit(MGC_MsdCbtVolume *pVolume);

static void MGC_CbtCmdComplete(void *pParam, MUSB_ControlIrp *pIrp);

/******************************** GLOBALS ********************************/

static const uint8_t MGC_aMsdCBCmdData [] =
{
    (uint8_t)(MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE), 0, 0,
    0,    /* wValue */
    0, 0, /* wIndex=interface number (set in run-time copy) */
    12, 0 /* wLength */
};

static MUSB_Irp MGC_MsdRetryIrp;

static uint32_t MGC_CbtMaxIORequest = 
    ((MUSB_HFI_MAX_IO_REQUEST) + (MUSB_HFI_DRV_IO_REQUEST));
static uint32_t MGC_CbtIORequest = 0;

/******************************** FUNCTIONS ******************************/

MGC_MsdProtocol *MGC_CreateCbtInstance(MUSB_Device *pDevice,
                                       MUSB_BusHandle hBus,
                                       const MUSB_InterfaceDescriptor *pIfaceDesc,
                                       const MUSB_EndpointDescriptor *pInEnd,
                                       const MUSB_EndpointDescriptor *pOutEnd,
                                       MGC_MsdCmdSet *pCmdSet)
{
    MUSB_DeviceEndpoint InEnd;

    MUSB_DeviceEndpoint OutEnd;
    MUSB_Pipe hInPipe;
    MUSB_Pipe hOutPipe;
    MGC_MsdCbtProtocol *pCbt;
    MGC_Port *pImplPort;
    MGC_MsdCbtIrp *pCbtIrp;
    uint32_t i;
    
    MUSB_ASSERT(pDevice);
    MUSB_ASSERT(hBus);
    MUSB_ASSERT(pIfaceDesc);
    MUSB_ASSERT(pInEnd);
    MUSB_ASSERT(pOutEnd);
    MUSB_ASSERT(pCmdSet);

    pImplPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pImplPort->pController);
    MUSB_ASSERT(pImplPort->pController->pSystemServices);
    
    InEnd.pDevice = pDevice;
    OutEnd.pDevice = pDevice;

    switch (pDevice->ConnectionSpeed)
    {
    case MUSB_CONNECTION_SPEED_HIGH:
        InEnd.wNakLimit = 16;
        OutEnd.wNakLimit = 16;
        break;

    default:
        InEnd.wNakLimit = 255;
        OutEnd.wNakLimit = 255;
    }

    MUSB_MemCopy(&(InEnd.UsbDescriptor), pInEnd, sizeof(MUSB_EndpointDescriptor));
    MUSB_MemCopy(&(OutEnd.UsbDescriptor), pOutEnd, sizeof(MUSB_EndpointDescriptor));
    hInPipe = MUSB_OpenPipe(hBus, &InEnd, NULL);

    if (!hInPipe)
    {
        MUSB_DIAG_STRING(1, "MSD/CBT: failed to open IN pipe");
        return NULL;
    }

    hOutPipe = MUSB_OpenPipe(hBus, &OutEnd, NULL);

    if (!hOutPipe)
    {
        MUSB_DIAG_STRING(1, "MSD/CBT: failed to open OUT pipe");

        MUSB_ClosePipe(hInPipe);
        return NULL;
    }

    pCbt = (MGC_MsdCbtProtocol *)MUSB_MemAlloc(sizeof(MGC_MsdCbtProtocol));

    if (pCbt)
    {
        MUSB_MemSet(pCbt, 0, sizeof(MGC_MsdCbtProtocol));

        pCbt->pSelf = pCbt;
        pCbt->pDevice = pDevice;
        pCbt->hBus = hBus;
        pCbt->hBusServices = (MUSB_BusHandle)pImplPort->pController->pSystemServices;
        pCbt->pIfaceDesc = pIfaceDesc;
        pCbt->pCmdSet = pCmdSet;
        pCbt->pInEnd = pInEnd;
        pCbt->pOutEnd = pOutEnd;
        pCbt->hInPipe = hInPipe;
        pCbt->hOutPipe = hOutPipe;

        /* fill PCSI-side dispatch table */
        pCbt->Protocol.pProtocolData = pCbt;
        pCbt->Protocol.pfStartDevice = MGC_CbtProtocolStartDevice;
        pCbt->Protocol.pfStopDevice = NULL;
        pCbt->Protocol.pfSendCmd = MGC_CbtProtocolSendCmd;
        pCbt->Protocol.pfSetDeviceInfo = MGC_CbtProtocolSetDeviceInfo;
        pCbt->Protocol.pfSetMediumInfo = MGC_CbtProtocolSetMediumInfo;
        pCbt->Protocol.pfSetReady = MGC_CbtProtocolSetReady;

        /* init CBT list */
        MUSB_ListInit(&(pCbt->CbtList));

        /* alloc cbt buffer */
        pCbt->pCbtIrpBuf= 
            (MGC_MsdCbtIrp *)MUSB_MemAlloc(MGC_CbtMaxIORequest * (sizeof(MGC_MsdCbtIrp)));
        if (!pCbt->pCbtIrpBuf)
        {
            MUSB_MemFree(pCbt);
            goto _MGC_CREATE_CBT_EXIT;
        }

        // init pCbtIrpBuf
        pCbtIrp = pCbt->pCbtIrpBuf;    
        for (i=0; i<MGC_CbtMaxIORequest; i++)
        {
            MUSB_ASSERT(pCbtIrp);    
            pCbtIrp->pCbt = NULL;
            pCbtIrp ++;
        }    
        
        return &(pCbt->Protocol);
    }

_MGC_CREATE_CBT_EXIT:
    MUSB_ClosePipe(hInPipe);
    MUSB_ClosePipe(hOutPipe);
    MUSB_DIAG_STRING(1, "MSD/CBT: failed to allocate instance data");
    return NULL;
}

void MGC_DestroyCbtInstance(const MGC_MsdProtocol *pProtocol)
{
    uint8_t bIndex;
    uint16_t wCount;
    uint16_t wIndex;    
    MGC_MsdCbtVolume *pVolume;
    MGC_MsdCbtProtocol *pCbt;
    MGC_MsdCbtIrp *pCbtIrp;
    MUSB_IO_DATA_T *pt_data;
    
    MUSB_ASSERT(pProtocol);
    MUSB_ASSERT(pProtocol->pProtocolData);

    pCbt = (MGC_MsdCbtProtocol *)pProtocol->pProtocolData;

    if ((pCbt->ControlIrp.pDevice) && (pCbt->ControlIrp.pDevice->pPort))
    {
        VERIFY(MUSB_STATUS_OK == MUSB_CancelControlTransfer(
            pCbt->ControlIrp.pDevice->pPort, &(pCbt->ControlIrp)));
    }

    MUSB_ClosePipe(pCbt->hOutPipe);
    pCbt->hOutPipe = NULL;
    MUSB_ClosePipe(pCbt->hInPipe);
    pCbt->hInPipe = NULL;

    if (pCbt->aVolume)
    {
        /* loop through volumes and inform HFI implementation of destruction */
        for (bIndex = 0; bIndex < pCbt->bLunCount; bIndex++)
        {
            pVolume = &(pCbt->aVolume[bIndex]);
            if (pVolume->bMediumAnnounced)
            {
                pVolume->bMediumAnnounced = FALSE;

                /* already announced and not ready, so must be medium removal */
                if (pVolume->hVolume)
                {
#ifdef MUSB_HFI_SUPPORT
                    MUSB_HfiMediumRemoved(pVolume->hVolume);
                    MUSB_HfiDeviceRemoved(pVolume->hVolume);
#endif
                    pVolume->hVolume = NULL;
                }
            }
        }
    }
    
    // release pCurrentCbtIrp
    if (pCbt->pCurrentCbtIrp)
    {
        pCbtIrp = pCbt->pCurrentCbtIrp;
        pCbt->pCurrentCbtIrp = NULL;          
        if (pCbtIrp->pfCmdComplete)
        {
            pt_data = pCbtIrp->pt_data;        
            if (pt_data)
            {
                pt_data->u4TransCount =  0;
                pCbtIrp->pfCmdComplete(pCbtIrp->pCompleteParam, MUSB_HFI_COND_FAIL, pt_data);
            }
            else
            {
                pCbtIrp->pfCmdComplete(pCbtIrp->pCompleteParam, 
                    MUSB_HFI_COND_FAIL, (void *)MGC_MSD_CBT_COMMAND_FAILED);
            }        
        }
        
        // free pCbtIrp usage.
        pCbtIrp->pCbt = NULL;
    }    

    // clear cbt list.
    wCount = MUSB_ListLength(&(pCbt->CbtList));
    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        pCbtIrp = (MGC_MsdCbtIrp *)MUSB_ListFindItem(&(pCbt->CbtList), wIndex);
        MUSB_ASSERT(pCbtIrp);
        if (pCbtIrp)
        {
            if (pCbtIrp->pfCmdComplete)
            {
                pt_data = pCbtIrp->pt_data;        
                if (pt_data)
                {
                    pt_data->u4TransCount =  0;
                    pCbtIrp->pfCmdComplete(pCbtIrp->pCompleteParam, MUSB_HFI_COND_FAIL, pt_data);
                }
                else
                {
                    pCbtIrp->pfCmdComplete(pCbtIrp->pCompleteParam, 
                        MUSB_HFI_COND_FAIL, (void *)MGC_MSD_CBT_COMMAND_FAILED);
                }                        
            }    
            
            // free pCbtIrp usage.
            pCbtIrp->pCbt = NULL;
        }
    }

    if (pCbt->aVolume)
    {
        /* loop through volumes and inform HFI implementation of destruction */
        for (bIndex = 0; bIndex < pCbt->bLunCount; bIndex++)
        {
            pVolume = &(pCbt->aVolume[bIndex]);
            pCbt->pCmdSet->pfDestroyInstance(pVolume->pCmdSetInstance);
        }
        MUSB_MemFree(pCbt->aVolume);
    }

    // free list.
    MUSB_MemFree(pCbt->pCbtIrpBuf);

    pCbt->pSelf = NULL;
    MUSB_MemFree(pCbt);    
}

static uint8_t MGC_CbtProtocolStartDevice(void *pProtocolData, const MUSB_Device *pDevice)
{
    MGC_MsdCbtProtocol *pCbt;
    uint8_t bIndex;
    MGC_MsdCbtVolume *pVolume;
    uint8_t bRet = FALSE;
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;

    MUSB_ASSERT(pProtocolData);
    MUSB_ASSERT(pDevice);    
    pCbt = (MGC_MsdCbtProtocol *)pProtocolData;   

    // CB protocol do not support GET_MAX_LUN command.
    pCbt->bLunCount = 1;
    pCbt->aVolume = (MGC_MsdCbtVolume *)MUSB_MemAlloc(
                        pCbt->bLunCount *sizeof(MGC_MsdCbtVolume));
    if (pCbt->aVolume)
    {
        MUSB_MemSet(pCbt->aVolume, 0, pCbt->bLunCount *sizeof(MGC_MsdCbtVolume));

        for (bIndex = 0; bIndex < pCbt->bLunCount; bIndex++)
        {
            pVolume = &(pCbt->aVolume[bIndex]);

            pVolume->pCmdSetInstance = pCbt->pCmdSet->pfCreateInstance(pCbt->bLunCount);
            if (pVolume->pCmdSetInstance)
            {
                pVolume->pCbt = pCbt;

                pVolume->bLun = bIndex;
                MGC_MsdCbtVolumeInit(pVolume);
            }
        }

        pVolume = pCbt->aVolume;
        bRet = pCbt->pCmdSet->pfDiscoverDevice(pVolume->pCmdSetInstance, &(pCbt->Protocol), 0);

        pPort = (MGC_Port *)pCbt->hBus;
        MUSB_ASSERT(pPort);
        pUsbDevice = pCbt->pDevice;
        MUSB_ASSERT(pUsbDevice);

        // Report device to user's call back function.
        pUsbDevice->bPortCount = pCbt->bLunCount;
        pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;        

        pEnumData = &(pPort->EnumerationData);
        if (pEnumData->pfChildDeviceUpdate)
        {
            pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                pUsbDevice, MUSB_STATUS_CHILD_CONNECT);
        }        
        
    }

    return bRet;
}

static uint32_t MGC_CbtProtocolRWRetry(MGC_MsdCbtIrp *pCbtIrp)
{
    MGC_MsdCbtProtocol *pCbt;
    uint32_t dwBlockOld;
    uint32_t dwBlockNew;
    uint32_t dwBlockNo;
    uint32_t dwBlockSize;
    MGC_MsdCbtVolume *pVolume;
    uint32_t dwTransSize;
    uint8_t* pBuffer;    

    MUSB_ASSERT(pCbtIrp);
    pCbt = pCbtIrp->pCbt;
    MUSB_ASSERT(pCbt);

    // Get block size by lun in aVolume[], Control/Bulk only support 1 LUN.        
    pVolume = &(pCbt->aVolume[0]);
    MUSB_ASSERT(pVolume);   
    dwBlockSize = pVolume->MediumInfo.dwBlockSize;
    MUSB_ASSERT(dwBlockSize >= 512);

    // Get original start block offset.
    dwBlockOld = MUSB_BIG_ENDIAN_SWAP32P(&pCbtIrp->aControlCb[10]);

    // Set new start block offset and block number.
    dwBlockNew = dwBlockOld + (pCbtIrp->DataIrp.dwActualLength/dwBlockSize);

    //Max transfer size of READ10/WRITE10 command.
    // If user request > MAX_MSD_CBT_TRANS_SIZE, 
    // Mcbt driver will concatenate each MAX_MSD_CBT_TRANS_SIZE 
    // size of transfer to fulfill with user's request.
    dwTransSize = MUSB_MIN((pCbtIrp->dwRequestLength - pCbtIrp->dwGrantLength), 
        MAX_MSD_CBT_TRANS_SIZE);        
    dwBlockNo = dwTransSize/dwBlockSize;

    LOG(5, "[%d] R=%d, F=%d, Left=%d, T=%d.\n", pCbt->bCbtCmdRetry,
        pCbtIrp->dwRequestLength, pCbtIrp->dwGrantLength,
        (pCbtIrp->dwRequestLength - pCbtIrp->dwGrantLength), dwTransSize);

    LOG(5, "[%d] BN=%d, BNO=%d, BS=%d.\n", pCbt->bCbtCmdRetry,
        dwBlockNew, dwBlockNo, dwBlockSize);

    // Set new start block offset in Read10/Write10.
    pCbtIrp->aControlCb[10] = (uint8_t)((dwBlockNew >> 24) & 0xff);
    pCbtIrp->aControlCb[11] = (uint8_t)((dwBlockNew >> 16) & 0xff);
    pCbtIrp->aControlCb[12] = (uint8_t)((dwBlockNew >> 8) & 0xff);
    pCbtIrp->aControlCb[13] = (uint8_t)(dwBlockNew & 0xff);
    // Set new start block number in Read10/Write10.
    pCbtIrp->aControlCb[15] = (uint8_t)((dwBlockNo >> 8) & 0xff);
    pCbtIrp->aControlCb[16] = (uint8_t)(dwBlockNo & 0xff);           

    // prepare Control/Bulk transfer.
    pCbtIrp->ControlIrp.dwActualOutLength = 0;
    pCbtIrp->ControlIrp.dwActualInLength = 0;
    pCbtIrp->DataIrp.dwStatus = 0;
    pBuffer = (uint8_t*)((uint32_t)pCbtIrp->DataIrp.pBuffer + pCbtIrp->DataIrp.dwActualLength);            
    LOG(5, "[%d] OBuf=0x%08X, NBuf=0x%08X.\n", pCbt->bCbtCmdRetry,
        (uint32_t)pCbtIrp->DataIrp.pBuffer, (uint32_t)pBuffer);            
    pCbtIrp->DataIrp.pBuffer = pBuffer;
    // Note: DataIrp.dwLength records the unfinished data length.     
    pCbtIrp->DataIrp.dwLength = dwTransSize;
    pCbtIrp->DataIrp.dwActualLength = 0;

    return MUSB_StartControlTransfer(pCbtIrp->ControlIrp.pDevice->pPort, 
        &(pCbtIrp->ControlIrp));    
}

static void MGC_CbtProtocolCmdComplete(
    void *pPrivateData, int32_t e_nfy_cond, void *pv_data, BOOL fgIsrCallback)
{
    uint32_t dwStatus = 0;
    MGC_MsdCbtIrp *pCbtIrp;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_SystemServices *pServices;
    uint32_t dwIrqStatus;
    MUSB_IO_DATA_T *pt_data;    
    MUSB_Irp *pDataIrp;
    uint8_t bWrapperStatus = (uint32_t)pv_data;
    
    MUSB_ASSERT(pPrivateData);

    pCbtIrp = (MGC_MsdCbtIrp *)pPrivateData;
    pCbt = pCbtIrp->pCbt;
    MUSB_ASSERT(pCbt);
    pServices = (MUSB_SystemServices *)pCbt->hBusServices;
    MUSB_ASSERT(pServices);
    pDataIrp = &pCbtIrp->DataIrp;
    MUSB_ASSERT(pDataIrp);

    // update mcbt finish data length.
    pCbtIrp->dwGrantLength += pDataIrp->dwActualLength;

    // check if READ10/WRITE10 data match user's request.
    if (((bWrapperStatus != MGC_MSD_DEV_REMOVE) && 
           (bWrapperStatus != MGC_MSD_COMMAND_ABORT)) &&
        (pCbtIrp->dwRequestLength >= 512) && 
        (pCbtIrp->dwRequestLength > pCbtIrp->dwGrantLength))
    {    
        pCbt->bCbtCmdRetry++;

        // When bWrapperStatus = MGC_MSD_CBT_COMMAND_PASSED, 
        // It is normal case to send next command and process next section of data.        
        if ((bWrapperStatus == MGC_MSD_CBT_COMMAND_PASSED) ||
            (pCbt->bCbtCmdRetry++ < MGC_MSD_CBT_RETRY))
        {
            dwStatus = MGC_CbtProtocolRWRetry(pCbtIrp);            
            if (dwStatus == MUSB_STATUS_OK)
            {
                return;
            }
            else            
            {
                MUSB_DIAG1(1, "MSD/CBT: Cmd retry error: ", pCbt->bCbtRetry, 16, 0);
            }            
        }
    }

    // finally, it can not recover after 3 times of reset.
    pCbt->bCbtCmdRetry = 0;

    // free pCbtIrp usage.
    MUSB_ASSERT(pCbtIrp->pCbt);
    pCbtIrp->pCbt = NULL;

    /* call completion handler if present */
    if (pCbtIrp->pfCmdComplete)
    {
        pt_data = pCbtIrp->pt_data;        
        if (pt_data)
        {
            if ((e_nfy_cond == MUSB_HFI_COND_READ_OK) || 
                 (e_nfy_cond == MUSB_HFI_COND_WRITE_OK))
            {
                pt_data->u4TransCount =  pt_data->u4Count;            
            }
            else
            {
                pt_data->u4TransCount =  0;
            }
            pCbtIrp->pfCmdComplete(pCbtIrp->pCompleteParam, e_nfy_cond, pt_data);
        }
        else
        {
            pCbtIrp->pfCmdComplete(pCbtIrp->pCompleteParam, e_nfy_cond, pv_data);
        }        
    }

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    if (!fgIsrCallback)
    {
        dwStatus = pServices->pfClassLock(pServices->pPrivateData, 0);
        UNUSED(dwStatus);    
    }
#endif    
    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);      

    pCbt->pCurrentCbtIrp = NULL;

    // process next cbt operation in list.
    if (MUSB_ListLength(&(pCbt->CbtList)) > 0)
    {        
        pCbtIrp = (MGC_MsdCbtIrp *)MUSB_ListFindItem(&(pCbt->CbtList), 0);
        MUSB_ASSERT(pCbtIrp);
        if (pCbtIrp)
        {
            // remove from list.
            MUSB_ListRemoveItem(&(pCbt->CbtList), (const void *)pCbtIrp);   
            pCbt->pCurrentCbtIrp = pCbtIrp;

            pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);                
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
            if (!fgIsrCallback)
            {
                dwStatus = pServices->pfClassUnlock(pServices->pPrivateData, 0);
                UNUSED(dwStatus);
            }
#endif
            dwStatus = MUSB_StartControlTransfer(
                pCbtIrp->ControlIrp.pDevice->pPort, &(pCbtIrp->ControlIrp));    
            
            UNUSED(dwStatus);
            return;
        }
    }

    pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    if (!fgIsrCallback)
    {
        dwStatus = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwStatus);
    }
#endif    
}

static uint8_t MGC_CbtProtocolSendCmd(void *pProtocolData, void *pPrivateData, 
                                      MUSB_IO_DATA_T *pt_data, uint8_t bLun, 
                                      const uint8_t *pCmdBuffer, uint8_t bCmdLength, 
                                      uint8_t *pDataBuffer, uint32_t dwDataLength, uint8_t bSend,
                                      MGC_pfMsdProtocolCmdComplete pfCmdComplete)
{
    uint32_t dwStatus = 0;
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    uint32_t dwRet = 0;
#endif    
    uint32_t dwFound = 0;
    uint32_t dwIrqStatus;
    uint32_t i;    
    MGC_MsdCbtIrp *pCbtIrp;
    uint8_t bCmdSize = (uint8_t)MGC_MIN(bCmdLength, 12);
    MGC_MsdCbtProtocol *pCbt = (MGC_MsdCbtProtocol *)pProtocolData;
    MUSB_SystemServices *pServices;
    
    if ((!pCbt) || (pCbt != pCbt->pSelf))
    {
        return FALSE;
    }

    // Control/Bulk transfer only support 1 LUN.
    MUSB_ASSERT(bLun == 0);
    MUSB_ASSERT(pCmdBuffer);
    pServices = (MUSB_SystemServices *)pCbt->hBusServices;
    MUSB_ASSERT(pServices);

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    dwStatus = pServices->pfClassLock(pServices->pPrivateData, 0);
    UNUSED(dwStatus);    
#endif
    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);      
  
    // Max 10 cbt request in queue.
    if (MUSB_ListLength(&(pCbt->CbtList)) >= MGC_CbtMaxIORequest)
    {   
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif        
        return FALSE;
    }

    pCbtIrp = pCbt->pCbtIrpBuf;    
    // search empty pCbtIrp.
    for (i=0; i<MGC_CbtMaxIORequest; i++)
    {
        MUSB_ASSERT(pCbtIrp);    
        if (pCbtIrp->pCbt == NULL)
        {
            dwFound = 1;
            break;
        }
        pCbtIrp ++;
        MGC_CbtIORequest = MUSB_MAX(MGC_CbtIORequest, i);
        UNUSED(MGC_CbtIORequest);        
    }    
    if (dwFound == 0)
    {   
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif        
        return FALSE;
    }
    
    MUSB_MemSet(pCbtIrp, 0, sizeof(MGC_MsdCbtIrp));
    pCbtIrp->pCbt = pCbt;

    pCbtIrp->ControlIrp.pDevice = pCbt->pDevice;
    MUSB_MemCopy((void *)pCbtIrp->aControlCb, MGC_aMsdCBCmdData, 8);    
    MUSB_MemCopy((void *)&pCbtIrp->aControlCb[8], pCmdBuffer, bCmdSize);    
    pCbtIrp->ControlIrp.pOutBuffer = pCbtIrp->aControlCb;    
    pCbtIrp->ControlIrp.dwOutLength = 20;   /* including the 8-byte standard request */
    pCbtIrp->ControlIrp.pInBuffer = NULL;
    pCbtIrp->ControlIrp.dwInLength = 0;
    pCbtIrp->ControlIrp.pfIrpComplete = MGC_CbtCmdComplete;
    pCbtIrp->ControlIrp.pCompleteParam = pCbtIrp;

    // init DATA Irp
    if ((dwDataLength > 0) && (pDataBuffer))
    {        
        pCbtIrp->dwRequestLength = dwDataLength;    
        pCbtIrp->DataIrp.hPipe = (bSend) ? pCbt->hOutPipe : pCbt->hInPipe;
        pCbtIrp->DataIrp.pBuffer = pDataBuffer;
//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_CBT_TRANS_SIZE, 
// Mcbt driver will concatenate each MAX_MSD_CBT_TRANS_SIZE 
// size of transfer to fulfill with user's request.    
        pCbtIrp->DataIrp.dwLength = (uint32_t)
            MUSB_MIN(MAX_MSD_CBT_TRANS_SIZE, dwDataLength);        
        pCbtIrp->DataIrp.pfIrpComplete = MGC_CbtDataComplete;
        pCbtIrp->DataIrp.pCompleteParam = pCbtIrp;
        pCbtIrp->DataIrp.bAllowDma = TRUE;
        pCbtIrp->DataIrp.bIsrCallback = FALSE;
    }

    // init user's callback function
    pCbtIrp->pfCmdComplete = pfCmdComplete;
    pCbtIrp->pCompleteParam = pPrivateData;        
    pCbtIrp->pt_data = pt_data;        
    
    if (!pCbt->pCurrentCbtIrp)
    {              
        pCbt->bCbtRetry = 0;   

        dwStatus = MUSB_StartControlTransfer(
            pCbtIrp->ControlIrp.pDevice->pPort, &(pCbtIrp->ControlIrp));    
        if (dwStatus == 0)
        {
            pCbt->pCurrentCbtIrp = pCbtIrp;
        }

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);

        return (dwStatus == 0);
     }
    else
    {              
        dwStatus = MUSB_ListAppendItem(&(pCbt->CbtList), (void *)pCbtIrp, 0);
    
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif        
        return dwStatus;            
     }
}

static uint8_t MGC_CbtProtocolSetDeviceInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiDeviceInfo *pInfo)
{
    MGC_MsdCbtProtocol *pCbt;
    MGC_MsdCbtVolume *pVolume;

    MUSB_ASSERT(pProtocolData);
    MUSB_ASSERT(pInfo);
    pCbt = (MGC_MsdCbtProtocol *)pProtocolData;
    pVolume = &(pCbt->aVolume[bLun]);
    
    /* copy info */
    MUSB_MemCopy(&(pVolume->DeviceInfo), pInfo, sizeof(MUSB_HfiDeviceInfo));

    /* copy USB device info */
    pVolume->DeviceInfo.wVendorId = pCbt->pDevice->DeviceDescriptor.idVendor;
    pVolume->DeviceInfo.wProductId = pCbt->pDevice->DeviceDescriptor.idProduct;
    pVolume->DeviceInfo.bcdDevice = pCbt->pDevice->DeviceDescriptor.bcdDevice;
    pVolume->DeviceInfo.bBusAddress = pCbt->pDevice->bBusAddress;
    pVolume->DeviceInfo.bLunCount = pCbt->pDevice->bBusAddress;
    pVolume->DeviceInfo.bProtocol = pCbt->Protocol.bProtocol;

    return TRUE;
}

static uint8_t MGC_CbtProtocolSetMediumInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiMediumInfo *pInfo)
{
    MGC_MsdCbtProtocol *pCbt;
    MGC_MsdCbtVolume *pVolume;

    MUSB_ASSERT(pProtocolData);
    MUSB_ASSERT(pInfo);
    pCbt = (MGC_MsdCbtProtocol *)pProtocolData;
    pVolume = &(pCbt->aVolume[bLun]);

    MUSB_MemCopy(&(pVolume->MediumInfo), pInfo, sizeof(MUSB_HfiMediumInfo));
    return TRUE;
}

static void MGC_CbtProtocolSetReady(void *pProtocolData, uint8_t bLun, uint8_t bIsReady)
{
    MUSB_HfiStatus Status;
    MGC_MsdCbtProtocol *pCbt;
    MGC_MsdCbtVolume *pVolume;

    MUSB_ASSERT(pProtocolData);
    pCbt = (MGC_MsdCbtProtocol *)pProtocolData;
    pVolume = &(pCbt->aVolume[bLun]);

    if (bIsReady)
    {
        if (!pVolume->bMediumAnnounced)
        {
#ifdef MUSB_HFI_SUPPORT
            if (!pVolume->hVolume)
            {
                Status = MUSB_HfiAddDevice(&(pVolume->hVolume), &(pVolume->DeviceInfo),
                                           &(pVolume->Device));
                if (MUSB_HFI_SUCCESS != Status)
                {
                    MUSB_DIAG_STRING(1, "MSD/CBT: HfiAddDevice failed");
                    return;
                }
            }
            MUSB_ASSERT(pVolume->hVolume);
            MUSB_HfiMediumInserted(pVolume->hVolume, &(pVolume->MediumInfo));
#endif
            pVolume->bMediumAnnounced = TRUE;
        }        
    }
    else
    {
        if (pVolume->bMediumAnnounced)
        {
            pVolume->bMediumAnnounced = FALSE;
            
#ifdef MUSB_HFI_SUPPORT
            MUSB_ASSERT(pVolume->hVolume);
            /* already announced and not ready, so must be medium removal */
            MUSB_HfiMediumRemoved(pVolume->hVolume);
            MUSB_HfiDeviceRemoved(pVolume->hVolume);
            pVolume->hVolume = NULL;            
#endif
        }
    }    
}

static void MGC_MsdCbtVolumeInit(MGC_MsdCbtVolume *pVolume)
{
    MUSB_ASSERT(pVolume);

    /* fill HFI-side dispatch table */
    pVolume->Device.pPrivateData = pVolume;
    pVolume->Device.pfGetMediumInfo = MGC_MsdCbtGetMediumInfo;
    pVolume->Device.pfGetDeviceInfo = MGC_MsdCbtGetDeviceInfo;
    pVolume->Device.pfReadDevice = MGC_MsdCbtReadDevice;
    pVolume->Device.pfWriteDevice = MGC_MsdCbtWriteDevice;
    pVolume->Device.pfWriteProtect = MGC_MsdCbtWriteProtect;    
}

static uint32_t MGC_CbtCmdSilentReset(MGC_MsdCbtIrp *pCbtIrp, const uint8_t bRetry)
{       
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_ControlIrp *pCmdIrp;

    MUSB_ASSERT(pCbtIrp);
    pCbt = pCbtIrp->pCbt;
    MUSB_ASSERT(pCbt);

    // reset retry.
    pCbt->bCbtRetry = 0;
    
    // flush ep before silent reset.
    VERIFY(0 == MUSB_FlushPipe(pCbt->hInPipe));
    VERIFY(0 == MUSB_FlushPipe(pCbt->hOutPipe));
    
    // perform silent reset when device do not ack properly.
    // Set device to original speed. We give an option to change to full speed.
    dwStatus = MUSB_SilentReset(pCbt->pDevice);
    if (dwStatus != MUSB_STATUS_OK)
    {
        return dwStatus;
    }

    // retry pCbtIrp from CBW phase.
    if (bRetry)
    {
        // retry from CBW command.
        pCmdIrp = (MUSB_ControlIrp *)&pCbtIrp->ControlIrp;
        MUSB_ASSERT(pCmdIrp);
        // init CB command Irp.
        pCmdIrp->dwActualOutLength = 0;
        pCmdIrp->dwActualInLength = 0;
        pCmdIrp->dwStatus = 0; 

        dwStatus = MUSB_StartControlTransfer(
            pCbt->pDevice->pPort, pCmdIrp);
    }

    return dwStatus;
}

static void MGC_CbtCmdComplete(void *pParam, MUSB_ControlIrp *pIrp)
{
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_MsdCbtIrp *pCbtIrp;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_ControlIrp *pCmdIrp;
    uint32_t dwCbtStatus;
    
    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pIrp);    
    pCbtIrp = (MGC_MsdCbtIrp *)pParam;
    pCbt = pCbtIrp->pCbt;
    MUSB_ASSERT(pCbt);
    
    /* handle USB error: MUSB_STATUS_STALLED */
    if (pIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED)
    {
        MUSB_DIAG1(1, "MSD/CBT: USB error on CBW transfer: ", pIrp->dwStatus, 16, 0);

        // Command fail after retry so many times. Bus reset device and return error code.
        dwStatus= MGC_CbtCmdSilentReset(pCbtIrp, FALSE);

        dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
        
        /* call completion handler if present */
        MGC_CbtProtocolCmdComplete(pCbtIrp,
            MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
        return;
    }
    else if (pIrp->dwStatus > 0)
    {
        pCbt->bCbtRetry ++;
        LOG(5, "MSD/CBT:CMD retry =%d, dwStatus = 0x%X.\n", pCbt->bCbtRetry, pIrp->dwStatus);

        pCmdIrp = (MUSB_ControlIrp *)&pCbtIrp->ControlIrp;
        MUSB_ASSERT(pCmdIrp);
        if (pCbt->bCbtRetry < MGC_MSD_CBT_CMD_RETRY)
        {        
            // init CBW Irp.
            pCmdIrp->dwActualOutLength = 0;
            pCmdIrp->dwActualInLength = 0;
            pCmdIrp->dwStatus = 0; 

            dwStatus = MUSB_StartControlTransfer(
                pCbt->pDevice->pPort, &(pCbtIrp->ControlIrp));    
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
                /* call completion handler if present */
                MGC_CbtProtocolCmdComplete(pCbtIrp,
                    MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
            }
        }
        else
        {       
            dwStatus= MGC_CbtCmdSilentReset(pCbtIrp, FALSE);

            dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
        
            /* call completion handler if present */
            MGC_CbtProtocolCmdComplete(pCbtIrp,
                MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
        }
        
        return;
    }
    
    // success, reset retry.
    pCbt->bCbtRetry = 0;
    
    if (pCbtIrp->DataIrp.pBuffer)
    {
        dwStatus = MUSB_StartTransfer(&(pCbtIrp->DataIrp));
    }
    else
    {
        /* call completion handler if present */
        MGC_CbtProtocolCmdComplete(pCbtIrp,
            MUSB_HFI_COND_READ_OK, (void *)MGC_MSD_CBT_COMMAND_PASSED, FALSE);

        return;
    }

    if (dwStatus != MUSB_STATUS_OK)
    {        
        dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
    
        /* call completion handler if present */
        MGC_CbtProtocolCmdComplete(pCbtIrp,
            MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
    }
}

static void MGC_CbtDataComplete(void *pParam, const MUSB_Irp *pIrp)
{
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_MsdCbtIrp *pCbtIrp;
    MGC_MsdCbtProtocol *pCbt;
    uint32_t dwCbtStatus;
    int32_t e_nfy_cond;

    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pIrp);    
    pCbtIrp = (MGC_MsdCbtIrp *)pParam;
    pCbt = pCbtIrp->pCbt;
    MUSB_ASSERT(pCbt);

    /* handle USB error: MUSB_STATUS_STALLED, and short packet*/
    if (pIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED)
    {
        MUSB_DIAG1(1, "MSD/CBT: USB error on DATA transfer: ", pIrp->dwStatus, 16, 0);

        // Command fail after retry so many times. Bus reset device and return error code.
        dwStatus= MGC_CbtCmdSilentReset(pCbtIrp, FALSE);

        dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
        
        /* call completion handler if present */
        MGC_CbtProtocolCmdComplete(pCbtIrp,
            MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);

        return;
    }
    else if (pIrp->dwStatus > 0)
    {
        // Some card reader do not handle properly when device is reading and plugged out by user.
        // This will cause host nak time out and need to bus reset this card reader.
        // If IRP == nak limit and command == read10 or write10.
        if ((pIrp->dwStatus == (uint32_t)MUSB_STATUS_NACK_LIMIT) &&
            ((pCbtIrp->aControlCb[8] == MGC_SCSI_WRITE10) ||
              (pCbtIrp->aControlCb[8] == MGC_SCSI_READ10)))
        {                 
            dwStatus= MGC_CbtCmdSilentReset(pCbtIrp, FALSE);
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);            
            }
            else
            {
                // prevent further retry in MGC_CbtProtocolCmdComplete().
                dwCbtStatus = MGC_MSD_DEV_REMOVE;
            }

            /* call completion handler if present */
            MGC_CbtProtocolCmdComplete(pCbtIrp,
                MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
            return;            
        }

        pCbt->bCbtRetry ++;
        LOG(5, "Data retry =%d, dwStatus = 0x%X.\n", pCbt->bCbtRetry, pIrp->dwStatus);        

        // prepare retry irp.
        if (pCbt->bCbtRetry == 1)
        {
            MUSB_MemCopy((void *)&MGC_MsdRetryIrp, pIrp, sizeof(MUSB_Irp));
        }
        else
        {
            pCbtIrp->DataIrp.dwActualLength += pIrp->dwActualLength;
        }
        
        if (pCbt->bCbtRetry < MGC_MSD_CBT_DATA_RETRY)
        {
            MUSB_ASSERT(pIrp->dwLength > pIrp->dwActualLength);
            if (pIrp->dwLength > pIrp->dwActualLength)
            {
                MGC_MsdRetryIrp.dwLength = pIrp->dwLength -pIrp->dwActualLength;
                MGC_MsdRetryIrp.pBuffer = (uint8_t*)((uint32_t)pIrp->pBuffer + pIrp->dwActualLength); 
                MGC_MsdRetryIrp.dwActualLength = 0;
                MGC_MsdRetryIrp.dwStatus = 0; 
                dwStatus = MUSB_StartTransfer(&MGC_MsdRetryIrp);
                if (dwStatus != MUSB_STATUS_OK)
                {        
                    dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                        MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
                
                    /* call completion handler if present */
                    MGC_CbtProtocolCmdComplete(pCbtIrp,
                        MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
                }
                return;                
            }            
            // Success, continue to process csw. Ignore error.
        }
        else
        {              
            // Command fail after retry so many times. Bus reset device and return error code.
            dwStatus= MGC_CbtCmdSilentReset(pCbtIrp, FALSE);

            dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
            /* call completion handler if present */
            MGC_CbtProtocolCmdComplete(pCbtIrp,
                MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
            return;
        }
    }
   
    // Short packet happen in Read/Write operation.
    if ((pIrp->dwLength >= 0x200)  &&
        (pIrp->dwLength > pIrp->dwActualLength))
    {          
        LOG(1, "Short Packet: S=%d, R=%d.\n", pIrp->dwLength, pIrp->dwActualLength);
        dwStatus= MGC_CbtCmdSilentReset(pCbtIrp, FALSE);       
        if (dwStatus != MUSB_STATUS_OK)
        {        
            dwCbtStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);            
        }
        else
        {
            // prevent further retry in MGC_CbtProtocolCmdComplete().
            dwCbtStatus = MGC_MSD_DEV_REMOVE;
        }

        /* call completion handler if present */
        MGC_CbtProtocolCmdComplete(pCbtIrp,
            MUSB_HFI_COND_FAIL, (void *)dwCbtStatus, FALSE);
        return;            
    }        

    // prepare retry irp.
    if (pCbt->bCbtRetry > 0)
    {
        pCbtIrp->DataIrp.dwActualLength += pIrp->dwActualLength;
        // success, reset retry.
        pCbt->bCbtRetry = 0;
    }

    e_nfy_cond = (int32_t)((pCbtIrp->aControlCb[8] == MGC_SCSI_WRITE10) ? 
        MUSB_HFI_COND_WRITE_OK : MUSB_HFI_COND_READ_OK);

    /* call completion handler if present */
    MGC_CbtProtocolCmdComplete(pCbtIrp,
        e_nfy_cond, (void *)MGC_MSD_CBT_COMMAND_PASSED, FALSE);
}

/**************************************************************************
HFI functions
**************************************************************************/
static MUSB_HfiStatus MGC_MsdCbtGetMediumInfo(void *pDeviceData, MUSB_HfiMediumInfo **ppMediumInfo)
{
    MGC_MsdCbtVolume *pVolume;
    
    MUSB_ASSERT(pDeviceData);
    MUSB_ASSERT(ppMediumInfo);        
    pVolume = (MGC_MsdCbtVolume *)pDeviceData;
    
    *ppMediumInfo = &(pVolume->MediumInfo);

    return MUSB_HFI_SUCCESS;
}

static MUSB_HfiStatus MGC_MsdCbtGetDeviceInfo(void *pDeviceData, MUSB_HfiDeviceInfo **ppDeviceInfo)
{
    MGC_MsdCbtVolume *pVolume;
    
    MUSB_ASSERT(pDeviceData);
    MUSB_ASSERT(ppDeviceInfo);        
    pVolume = (MGC_MsdCbtVolume *)pDeviceData;

    *ppDeviceInfo = &(pVolume->DeviceInfo);

    return MUSB_HFI_SUCCESS;
}

static MUSB_HfiStatus MGC_MsdCbtReadDevice(void *pDeviceData,
                                           uint64_t ddwStartBlock,
                                           uint32_t dwByteCount,
                                           uint8_t *pBuffer)
{
    uint8_t bOk;
    uint8_t bCmdLength = 0;
    uint8_t aCmdBuffer[16];
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;            
    MGC_MsdCbtVolume *pVolume;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_NFY_INFO_T *pt_nfy_info;
    uint64_t ddwSize1;
    uint64_t ddwSize2;
    uint32_t dwBlockLo;
    uint16_t wBlockCount;
    uint32_t dwTransCount;

    MUSB_ASSERT(pDeviceData);
    pUcFsDevice = (MGC_UcFsDevice *)pDeviceData;
    MUSB_ASSERT(pUcFsDevice->pvDevice);
    pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pDevice->pPrivateData);    
    pVolume = (MGC_MsdCbtVolume *)pDevice->pPrivateData;
    MUSB_ASSERT(pVolume);    
    if (!pVolume->bMediumAnnounced)
    {
        return MUSB_HFI_ERROR_MEDIA_REMOVED;
    }

    MUSB_ASSERT(pVolume->MediumInfo.dwBlockSize);
//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_CBT_TRANS_SIZE, 
// Mcbt driver will concatenate each MAX_MSD_CBT_TRANS_SIZE 
// size of transfer to fulfill with user's request.    
    dwTransCount = MUSB_MIN(dwByteCount, MAX_MSD_CBT_TRANS_SIZE);    
    wBlockCount = (uint16_t)(dwTransCount/pVolume->MediumInfo.dwBlockSize);
    ddwSize1 = ddwStartBlock + (uint64_t)wBlockCount;
    ddwSize2 = MUSB_MAKE64(pVolume->MediumInfo.dwBlockCountLo, 
        pVolume->MediumInfo.dwBlockCountHi);

    if (ddwSize1 > ddwSize2)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }

    pt_nfy_info = &pUcFsDevice->rt_nfy_info;
    if (!pt_nfy_info->pfHfiNfy)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }    
    
    pCbt = pVolume->pCbt;
    MUSB_ASSERT(pCbt);

    dwBlockLo = (uint32_t)(ddwStartBlock & 0xFFFFFFFF);
    /* only use Read10, so only low 32-bits of block */
    aCmdBuffer[bCmdLength++ ] = MGC_SCSI_READ10;
    aCmdBuffer[bCmdLength++ ] = 0;
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((dwBlockLo >> 24) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((dwBlockLo >> 16) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((dwBlockLo >> 8) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)(dwBlockLo & 0xff);
    aCmdBuffer[bCmdLength++ ] = 0;
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((wBlockCount >> 8) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)(wBlockCount & 0xff);
    aCmdBuffer[bCmdLength++ ] = 0;
    
    bOk = MGC_CbtProtocolSendCmd(pCbt, pt_nfy_info->pvHfiNfyTag, 
                                (MUSB_IO_DATA_T *)pUcFsDevice->pvData,
                                 pVolume->bLun, aCmdBuffer,
                                 bCmdLength, pBuffer,
                                 dwByteCount, FALSE,
                                 pt_nfy_info->pfHfiNfy);

    if (!bOk)
    {
        return MUSB_HFI_ERROR_TRANSFER;
    }

    return MUSB_HFI_SUCCESS;
}

static MUSB_HfiStatus MGC_MsdCbtWriteDevice(void *pDeviceData,
                                            uint64_t ddwStartBlock,
                                            uint32_t dwByteCount,
                                            uint8_t *pBuffer)
{
    uint8_t bOk;
    uint8_t bCmdLength = 0;
    uint8_t aCmdBuffer[16];
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;            
    MGC_MsdCbtVolume *pVolume;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_NFY_INFO_T *pt_nfy_info;   
    uint64_t ddwSize1;
    uint64_t ddwSize2;
    uint32_t dwBlockLo;
    uint16_t wBlockCount;
    uint32_t dwTransCount;
    
    MUSB_ASSERT(pDeviceData);
    pUcFsDevice = (MGC_UcFsDevice *)pDeviceData;
    MUSB_ASSERT(pUcFsDevice->pvDevice);
    pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pDevice->pPrivateData);    
    pVolume = (MGC_MsdCbtVolume *)pDevice->pPrivateData;
    MUSB_ASSERT(pVolume);        
    if (!pVolume->bMediumAnnounced)
    {
        return MUSB_HFI_ERROR_MEDIA_REMOVED;
    }

    //  MTK Notice: Max Liao, 2006/11/20.
    //  check write protect.
    if (pVolume->MediumInfo.AccessType != MUSB_HFI_ACCESS_RANDOM)
    {
        return MUSB_HFI_ERROR_MEDIA_PROTECTED;
    }

    MUSB_ASSERT(pVolume->MediumInfo.dwBlockSize);
//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_CBT_TRANS_SIZE, 
// Mcbt driver will concatenate each MAX_MSD_CBT_TRANS_SIZE 
// size of transfer to fulfill with user's request.    
    dwTransCount = MUSB_MIN(dwByteCount, MAX_MSD_CBT_TRANS_SIZE);    
    wBlockCount = (uint16_t)(dwTransCount/pVolume->MediumInfo.dwBlockSize);
    ddwSize1 = ddwStartBlock + (uint64_t)wBlockCount;
    ddwSize2 = MUSB_MAKE64(pVolume->MediumInfo.dwBlockCountLo, pVolume->MediumInfo.dwBlockCountHi);

    if (ddwSize1 > ddwSize2)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }

    pt_nfy_info = &pUcFsDevice->rt_nfy_info;
    if (!pt_nfy_info->pfHfiNfy)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }    

    pCbt = pVolume->pCbt;
    MUSB_ASSERT(pCbt);

    dwBlockLo = (uint32_t)(ddwStartBlock & 0xFFFFFFFF);
    /* only use Write10, so only low 32-bits of block */
    aCmdBuffer[bCmdLength++ ] = MGC_SCSI_WRITE10;
    aCmdBuffer[bCmdLength++ ] = 0;
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((dwBlockLo >> 24) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((dwBlockLo >> 16) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((dwBlockLo >> 8) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)(dwBlockLo & 0xff);
    aCmdBuffer[bCmdLength++ ] = 0;
    aCmdBuffer[bCmdLength++ ] = (uint8_t)((wBlockCount >> 8) & 0xff);
    aCmdBuffer[bCmdLength++ ] = (uint8_t)(wBlockCount & 0xff);
    aCmdBuffer[bCmdLength++ ] = 0;

    bOk = MGC_CbtProtocolSendCmd(pCbt, pt_nfy_info->pvHfiNfyTag, 
                                (MUSB_IO_DATA_T *)pUcFsDevice->pvData,
                                 pVolume->bLun, aCmdBuffer,
                                 bCmdLength, pBuffer,
                                 dwByteCount, TRUE,
                                 pt_nfy_info->pfHfiNfy);

    if (!bOk)
    {
        return MUSB_HFI_ERROR_TRANSFER;
    }

    return MUSB_HFI_SUCCESS;
}

static MUSB_HfiStatus MGC_MsdCbtWriteProtect(void *pDeviceData)
{
    uint8_t bOk;
    uint8_t bCmdLength = 0;
    MGC_UcFsDevice *pUcFsDevice;    
    MUSB_HfiDevice *pDevice;        
    MGC_MsdCbtVolume *pVolume;
    MGC_ScsiCmdSetData* pScsi;
    MGC_MsdCbtProtocol *pCbt;
    MUSB_NFY_INFO_T *pt_nfy_info;   
    MUSB_IO_DATA_T *pt_data;
    
    MUSB_ASSERT(pDeviceData);
    pUcFsDevice = (MGC_UcFsDevice *)pDeviceData;    
    MUSB_ASSERT(pUcFsDevice->pvDevice);
    pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pDevice->pPrivateData);    
    pVolume = (MGC_MsdCbtVolume *)pDevice->pPrivateData;
    MUSB_ASSERT(pVolume);        
    if (!pVolume->bMediumAnnounced)
    {
        return MUSB_HFI_ERROR_MEDIA_REMOVED;
    }
    pScsi = pVolume->pCmdSetInstance;

    if ((pScsi->bModeSenseType != MGC_SCSI_MODE_SENSE10) &&
         (pScsi->bModeSenseType != MGC_SCSI_MODE_SENSE))
    {
        return MUSB_HFI_ERROR_MEDIA_PROTECTED;
    }

    pt_nfy_info = &pUcFsDevice->rt_nfy_info;
    if (!pt_nfy_info->pfHfiNfy)
    {
        return MUSB_HFI_ERROR_PARAMETER;
    }    

    pCbt = pVolume->pCbt;
    MUSB_ASSERT(pCbt);

    MUSB_MemSet(pScsi->aCmd, 0, 16);

    if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE10)
    {
        bCmdLength = 10;
        pScsi->aCmd[0] = MGC_SCSI_MODE_SENSE10;
        pScsi->aCmd[2] = 0x3F; // page code. 
        pScsi->aCmd[8] = 0xC0; // allocation length.
    }
    else if (pScsi->bModeSenseType == MGC_SCSI_MODE_SENSE)
    {
        bCmdLength = 6;
        pScsi->aCmd[0] = MGC_SCSI_MODE_SENSE;
        pScsi->aCmd[2] = 0x3F; // page code. 
        pScsi->aCmd[4] = 0xC0; // allocation length.
    }

    pt_data = (MUSB_IO_DATA_T *)pUcFsDevice->pvData;
    MUSB_ASSERT(pt_data);
    // save pScsi->aModeSenseData in pt_data->pvData structure.
    pt_data->pvData = pScsi;

    bOk = MGC_CbtProtocolSendCmd(pCbt, pt_nfy_info->pvHfiNfyTag, pt_data,
                                 pVolume->bLun, pScsi->aCmd,
                                 bCmdLength, pScsi->aModeSenseData,
                                 sizeof(pScsi->aModeSenseData), FALSE,
                                 pt_nfy_info->pfHfiNfy);
    if (!bOk)
    {
        return MUSB_HFI_ERROR_TRANSFER;
    }

    return MUSB_HFI_SUCCESS;
}
