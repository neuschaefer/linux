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
 * MUSBStack-S mass-storage BOT Protocol implementation
 * $Revision: #1 $
 */

#include "mu_bot.h"
#include "mu_cdi.h"
#include "mu_diag.h"
#include "mu_hfi_if.h"
#include "mu_mem.h"

#include "mu_mpcsi.h"
#include "mu_mapi.h"
#include "mu_impl.h"

#include "mu_scsi.h"

#define MGC_MSD_BOT_CBW_RETRY   (5)
#define MGC_MSD_BOT_DATA_RETRY   (10)
#define MGC_MSD_BOT_DATA_NAK_RETRY   (6)  // 6 secs.
#define MGC_MSD_BOT_CSW_RETRY   (10)
#define MGC_MSD_BOT_RETRY   (3)

//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_BOT_TRANS_SIZE, 
// Mbot driver will concatenate each MAX_MSD_BOT_TRANS_SIZE 
// size of transfer to fulfill with user's request.
#define MAX_MSD_BOT_TRANS_SIZE   (0x10000)

// For Slow device usage.
#define MAX_MSD_BOT_SMALL_TRANS_SIZE   (0x1000)
/******************************* FORWARDS ********************************/

static uint8_t MGC_BotProtocolStartDevice(void *pProtocolData, const MUSB_Device *pDevice);
static uint32_t MGC_BotProtocolRWRetry(MGC_MsdBotIrp *pBotIrp);
static void MGC_BotProtocolCmdComplete(void *pPrivateData, 
                int32_t e_nfy_cond, void *pv_data, BOOL fgIsrCallback);
static uint8_t MGC_BotProtocolSendCmd(void *pProtocolData, void *pPrivateData, MUSB_IO_DATA_T *pt_data,
                                      uint8_t bLun, const uint8_t *pCmdBuffer,
                                      uint8_t bCmdLength, uint8_t *pDataBuffer,
                                      uint32_t dwDataLength, uint8_t bSend,
                                      MGC_pfMsdProtocolCmdComplete pfCmdComplete);

static uint8_t MGC_BotProtocolSetDeviceInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiDeviceInfo *pInfo);

static uint8_t MGC_BotProtocolSetMediumInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiMediumInfo *pInfo);

static void MGC_BotProtocolSetReady(void *pProtocolData, uint8_t bLun, uint8_t bIsReady);

static void MGC_BotControlIrpComplete(void *pParam, MUSB_ControlIrp *pIrp);

static void MGC_BotCbwComplete(void *pParam, const MUSB_Irp *pIrp);

static void MGC_BotCswComplete(void *pParam, const MUSB_Irp *pIrp);

static void MGC_BotDataComplete(void *pParam, const MUSB_Irp *pIrp);

static MUSB_HfiStatus MGC_MsdBotGetMediumInfo(void *pDeviceData, MUSB_HfiMediumInfo **ppMediumInfo);

static MUSB_HfiStatus MGC_MsdBotGetDeviceInfo(void *pDeviceData, MUSB_HfiDeviceInfo **ppDeviceInfo);

static MUSB_HfiStatus MGC_MsdBotReadDevice(void *pDeviceData,
                                           uint64_t ddwStartBlock,
                                           uint32_t dwByteCount,
                                           uint8_t *pBuffer);

static MUSB_HfiStatus MGC_MsdBotWriteDevice(void *pDeviceData,
                                            uint64_t ddwStartBlock,
                                            uint32_t dwByteCount,
                                            uint8_t *pBuffer);

static MUSB_HfiStatus MGC_MsdBotWriteProtect(void *pDeviceData);

/******************************** GLOBALS ********************************/

static const uint8_t MGC_aMsdBotGetMaxLunData [] =
{
    (uint8_t)(MUSB_DIR_IN | MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE), MGC_MSD_BOT_GET_MAXLUN, 0,
    0,    /* wValue */
    0, 0, /* wIndex=interface number (set in run-time copy) */
    1, 0, /* wLength */
};

static const uint8_t MGC_aMsdBotResetData [] =
{
    (uint8_t)(MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE), MGC_MSD_BOT_RESET, 0, 0, /* wValue */
    0, 0,                                                                       /* wIndex=interface number (set in run-time copy) */
//  MTK Notice: Max Liao, 2006/10/18.
//  Bug: wLength must set to 0 at BOT reset cmd.
    0, 0, /* wLength */
};

static const uint8_t MGC_aMsdBotClearHaltData [] =
{
    (uint8_t)MUSB_RECIP_ENDPOINT, MUSB_REQ_CLEAR_FEATURE, MUSB_FEATURE_ENDPOINT_HALT, 0, 0,
    0,    /* endpoint (set in run-time copy) */
    0, 0, /* wLength */
};

static MUSB_Irp MGC_MsdRetryIrp;

static uint32_t MGC_BotMaxIORequest = 
    ((MUSB_HFI_MAX_IO_REQUEST) + (MUSB_HFI_DRV_IO_REQUEST));
static uint32_t MGC_BotIORequest = 0;

/******************************** FUNCTIONS ******************************/
BOOL _MGC_CheckCmdQueueFull(MGC_MsdBotIrp *pBotIrp, BOOL fgIsDrvRequest)
{
    UINT32 u4HfiIoRequest = 0;
    UINT32 u4DrvIoRequest = 0;
    UINT32 i;


    for (i=0; i<MGC_BotMaxIORequest; i++)
    {
        MUSB_ASSERT(pBotIrp);    
        if (pBotIrp->pBot != NULL)
        {
            if ( pBotIrp->pt_data == NULL )
            {
                u4DrvIoRequest++;
            }
            else
            {
                u4HfiIoRequest++;
            }
        }
        else
        {
            break;
        }
        pBotIrp ++;
    }    

    if ( u4HfiIoRequest >= MUSB_HFI_MAX_IO_REQUEST && !fgIsDrvRequest )
    {
        LOG(0, "USB Too Many request from middleware(%d), Drop It!\n", u4HfiIoRequest);
        return TRUE;
    }
    
    if ( u4DrvIoRequest >= MUSB_HFI_DRV_IO_REQUEST && fgIsDrvRequest)
    {
        LOG(0, "USB Diver IO Request Full\n");
        ASSERT(0);
        return TRUE;
    }
    
    return FALSE;
}

MGC_MsdProtocol *MGC_CreateBotInstance(MUSB_Device *pDevice,
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
    MGC_MsdBotProtocol *pBot;
    MGC_Port *pImplPort;
    MGC_MsdBotIrp *pBotIrp;
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
        MUSB_DIAG_STRING(1, "MSD/BOT: failed to open IN pipe");
        return NULL;
    }

    hOutPipe = MUSB_OpenPipe(hBus, &OutEnd, NULL);

    if (!hOutPipe)
    {
        MUSB_DIAG_STRING(1, "MSD/BOT: failed to open OUT pipe");
        return NULL;
    }

    pBot = (MGC_MsdBotProtocol *)MUSB_MemAlloc(sizeof(MGC_MsdBotProtocol));

    if (pBot)
    {
        MUSB_MemSet(pBot, 0, sizeof(MGC_MsdBotProtocol));

        pBot->pSelf = pBot;
        pBot->pDevice = pDevice;
        pBot->hBus = hBus;
        pBot->hBusServices = (MUSB_BusHandle)pImplPort->pController->pSystemServices;
        pBot->pIfaceDesc = pIfaceDesc;
        pBot->pCmdSet = pCmdSet;
        pBot->pInEnd = pInEnd;
        pBot->pOutEnd = pOutEnd;
        pBot->hInPipe = hInPipe;
        pBot->hOutPipe = hOutPipe;

        /* fill PCSI-side dispatch table */
        pBot->Protocol.pProtocolData = pBot;
        pBot->Protocol.pfStartDevice = MGC_BotProtocolStartDevice;
        pBot->Protocol.pfStopDevice = NULL;
        pBot->Protocol.pfSendCmd = MGC_BotProtocolSendCmd;
        pBot->Protocol.pfSetDeviceInfo = MGC_BotProtocolSetDeviceInfo;
        pBot->Protocol.pfSetMediumInfo = MGC_BotProtocolSetMediumInfo;
        pBot->Protocol.pfSetReady = MGC_BotProtocolSetReady;
       

        /* init bot list */
        MUSB_ListInit(&(pBot->BotList));

        /* alloc bot buffer */
        pBot->pBotIrpBuf= (MGC_MsdBotIrp *)MUSB_MemAlloc(
            MGC_BotMaxIORequest * (sizeof(MGC_MsdBotIrp)));
        if (!pBot->pBotIrpBuf)
        {
            MUSB_MemFree(pBot);
            goto _MGC_CREATE_BOT_EXIT;
        }

        // init pBotIrpBuf
        pBotIrp = pBot->pBotIrpBuf;    
        for (i=0; i<MGC_BotMaxIORequest; i++)
        {
            MUSB_ASSERT(pBotIrp);    
            pBotIrp->pBot = NULL;
            pBotIrp ++;
        }    
        
        return &(pBot->Protocol);
    }

_MGC_CREATE_BOT_EXIT:
    MUSB_ClosePipe(hInPipe);
    MUSB_ClosePipe(hOutPipe);
    MUSB_DIAG_STRING(1, "MSD/BOT: failed to allocate instance data");
    return NULL;
}

void MGC_DestroyBotInstance(const MGC_MsdProtocol *pProtocol)
{
    uint8_t bIndex;
    uint16_t wCount;
    uint16_t wIndex;    
    MGC_MsdBotVolume *pVolume;
    MGC_MsdBotProtocol *pBot;
    MGC_MsdBotIrp *pBotIrp;
    MUSB_IO_DATA_T *pt_data;
    
    MUSB_ASSERT(pProtocol);
    MUSB_ASSERT(pProtocol->pProtocolData);

    pBot = (MGC_MsdBotProtocol *)pProtocol->pProtocolData;

    //  MTK Notice: Max Liao, 2006/11/16.
    //  check pBot->ControlIrp.pDevice before access it.
    if ((pBot->ControlIrp.pDevice) && (pBot->ControlIrp.pDevice->pPort))
    {
        VERIFY(MUSB_STATUS_OK == MUSB_CancelControlTransfer(
            pBot->ControlIrp.pDevice->pPort, &(pBot->ControlIrp)));
    }

    MUSB_ClosePipe(pBot->hOutPipe);
    pBot->hOutPipe = NULL;
    MUSB_ClosePipe(pBot->hInPipe);
    pBot->hInPipe = NULL;

    if (pBot->aVolume)
    {
        /* loop through volumes and inform HFI implementation of destruction */
        for (bIndex = 0; bIndex < pBot->bLunCount; bIndex++)
        {
            pVolume = &(pBot->aVolume[bIndex]);
            if (pVolume->bMediumAnnounced)
            {
                pVolume->bMediumAnnounced = FALSE;
                MUSB_ASSERT (pVolume->hVolume);
                /* already announced and not ready, so must be medium removal */
                MUSB_HfiMediumRemoved(pVolume->hVolume);
            }

            if (pVolume->hVolume)
            {
                MUSB_HfiDeviceRemoved(pVolume->hVolume);
                pVolume->hVolume = NULL;
            }           
        }
    }
    
    // release CurrentBotIrp
    if (pBot->pCurrentBotIrp)
    {
        pBotIrp = pBot->pCurrentBotIrp;
        pBot->pCurrentBotIrp = NULL;
        if (pBotIrp->pfCmdComplete)
        {
            pt_data = pBotIrp->pt_data;        
            if (pt_data)
            {
                pt_data->u4TransCount =  0;
                pBotIrp->pfCmdComplete(pBotIrp->pCompleteParam, MUSB_HFI_COND_FAIL, pt_data);
            }
            else
            {
                pBotIrp->pfCmdComplete(pBotIrp->pCompleteParam, 
                    MUSB_HFI_COND_FAIL, (void *)MGC_MSD_BOT_COMMAND_FAILED);
            }        
        }
        
        // free pBotIrp usage.
        pBotIrp->pBot = NULL;
    }    

    // clear bot list.
    wCount = MUSB_ListLength(&(pBot->BotList));
    for (wIndex = 0; wIndex < wCount; wIndex++)
    {
        pBotIrp = (MGC_MsdBotIrp *)MUSB_ListFindItem(&(pBot->BotList), wIndex);
        MUSB_ASSERT(pBotIrp);
        if (pBotIrp)
        {
            if (pBotIrp->pfCmdComplete)
            {
                pt_data = pBotIrp->pt_data;        
                if (pt_data)
                {
                    pt_data->u4TransCount =  0;
                    pBotIrp->pfCmdComplete(pBotIrp->pCompleteParam, MUSB_HFI_COND_FAIL, pt_data);
                }
                else
                {
                    pBotIrp->pfCmdComplete(pBotIrp->pCompleteParam, 
                        MUSB_HFI_COND_FAIL, (void *)MGC_MSD_BOT_COMMAND_FAILED);
                }                        
            }    
            
            // free pBotIrp usage.
            pBotIrp->pBot = NULL;
        }
    }

    if (pBot->aVolume)
    {
        /* loop through volumes and inform HFI implementation of destruction */
        for (bIndex = 0; bIndex < pBot->bLunCount; bIndex++)
        {
            pVolume = &(pBot->aVolume[bIndex]);
            pBot->pCmdSet->pfDestroyInstance(pVolume->pCmdSetInstance);
        }
        MUSB_MemFree(pBot->aVolume);
    }

    // free list.
    MUSB_MemFree(pBot->pBotIrpBuf);

    pBot->pSelf = NULL;
    MUSB_MemFree(pBot);    
}

static void MGC_MsdBotReset(MGC_MsdBotProtocol *pBot)
{
    uint32_t dwStatus;

    MUSB_ASSERT(pBot);
    
    MUSB_DIAG_STRING(1, "MSD/BOT: performing BOT reset");

    pBot->bResetStage = MGC_MSD_BOT_RESET_STAGE_CMD;
    MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotResetData, sizeof(MGC_aMsdBotResetData));
    pBot->aSetup[4] = pBot->pIfaceDesc->bInterfaceNumber;
    pBot->ControlIrp.pInBuffer = NULL;
    pBot->ControlIrp.dwInLength = 0;
    dwStatus = MUSB_StartControlTransfer(pBot->ControlIrp.pDevice->pPort, &(pBot->ControlIrp));

    UNUSED(dwStatus);
    return;
}

static uint8_t MGC_BotProtocolStartDevice(void *pProtocolData, const MUSB_Device *pDevice)
{
    uint32_t dwStatus;

    MGC_MsdBotProtocol *pBot;

    MUSB_ASSERT(pProtocolData);
    MUSB_ASSERT(pDevice);    
    pBot = (MGC_MsdBotProtocol *)pProtocolData;   
    
    /* start GetMaxLUN */
    MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotGetMaxLunData, sizeof(MGC_aMsdBotGetMaxLunData));
    pBot->aSetup[4] = pBot->pIfaceDesc->bInterfaceNumber;
    pBot->ControlIrp.pDevice = pDevice;
    pBot->ControlIrp.pOutBuffer = pBot->aSetup;
    pBot->ControlIrp.dwOutLength = 8;
    pBot->ControlIrp.pInBuffer = pBot->aData;
    pBot->ControlIrp.dwInLength = 1;
    pBot->ControlIrp.pfIrpComplete = MGC_BotControlIrpComplete;
    pBot->ControlIrp.pCompleteParam = pBot;
    pBot->bInit = TRUE;
    dwStatus = MUSB_StartControlTransfer(pDevice->pPort, &(pBot->ControlIrp));

    return (MUSB_STATUS_OK == dwStatus) ? TRUE : FALSE;
}

static uint32_t MGC_BotProtocolRWRetry(MGC_MsdBotIrp *pBotIrp)
{
    MGC_MsdBotProtocol *pBot;
    uint32_t dwBlockOld;
    uint32_t dwBlockNew;
    uint32_t dwBlockNo;
    uint32_t dwBlockSize;
    MGC_MsdBotVolume *pVolume;
    uint32_t dwTransSize;
    uint8_t* pBuffer;    

    MUSB_ASSERT(pBotIrp);
    pBot = pBotIrp->pBot;
    MUSB_ASSERT(pBot);

    // Get block size by lun in aVolume[].        
    pVolume = &(pBot->aVolume[pBotIrp->Cbw.bCbwLun]);
    MUSB_ASSERT(pVolume);   
    dwBlockSize = pVolume->MediumInfo.dwBlockSize;
    MUSB_ASSERT(dwBlockSize >= 512);

    // Get original start block offset.
    dwBlockOld = MUSB_BIG_ENDIAN_SWAP32P(&pBotIrp->Cbw.aCbwCb[2]);

    // Set new start block offset and block number.
    dwBlockNew = dwBlockOld + (pBotIrp->DataIrp.dwActualLength/dwBlockSize);

    //Max transfer size of READ10/WRITE10 command.
    // If user request > MAX_MSD_BOT_TRANS_SIZE, 
    // Mbot driver will concatenate each MAX_MSD_BOT_TRANS_SIZE 
    // size of transfer to fulfill with user's request.
    dwTransSize = MUSB_MIN((pBotIrp->dwRequestLength - pBotIrp->dwGrantLength), 
        pBot->dwMaxTransferSize);        
    dwBlockNo = dwTransSize/dwBlockSize;

    LOG(7, "[%d] R=%d, F=%d, Left=%d, T=%d.\n", pBot->bBotCmdRetry,
        pBotIrp->dwRequestLength, pBotIrp->dwGrantLength,
        (pBotIrp->dwRequestLength - pBotIrp->dwGrantLength), dwTransSize);

    LOG(7, "[%d] BN=%d, BNO=%d, BS=%d.\n", pBot->bBotCmdRetry,
        dwBlockNew, dwBlockNo, dwBlockSize);

    // Increase Bot tag.
    pBotIrp->Cbw.dCbwTag = MUSB_SWAP32(++(pBot->dwTag));
    // Set new data length in CBW.
    pBotIrp->Cbw.dCbwDataTransferLength = MUSB_SWAP32(dwTransSize);            
    // Set new start block offset in Read10/Write10.
    pBotIrp->Cbw.aCbwCb[2] = (uint8_t)((dwBlockNew >> 24) & 0xff);
    pBotIrp->Cbw.aCbwCb[3] = (uint8_t)((dwBlockNew >> 16) & 0xff);
    pBotIrp->Cbw.aCbwCb[4] = (uint8_t)((dwBlockNew >> 8) & 0xff);
    pBotIrp->Cbw.aCbwCb[5] = (uint8_t)(dwBlockNew & 0xff);
    // Set new start block number in Read10/Write10.
    pBotIrp->Cbw.aCbwCb[7] = (uint8_t)((dwBlockNo >> 8) & 0xff);
    pBotIrp->Cbw.aCbwCb[8] = (uint8_t)(dwBlockNo & 0xff);           

    // prepare CBW/DATA/CSW.
    pBotIrp->CbwIrp.dwStatus = 0;
    pBotIrp->CbwIrp.dwActualLength = 0;
    pBotIrp->DataIrp.dwStatus = 0;
    pBuffer = (uint8_t*)((uint32_t)pBotIrp->DataIrp.pBuffer + pBotIrp->DataIrp.dwActualLength);            
    LOG(7, "[%d] OBuf=0x%08X, NBuf=0x%08X.\n", pBot->bBotCmdRetry,
        (uint32_t)pBotIrp->DataIrp.pBuffer, (uint32_t)pBuffer);            
    pBotIrp->DataIrp.pBuffer = pBuffer;
    // Note: DataIrp.dwLength records the unfinished data length.     
    pBotIrp->DataIrp.dwLength = dwTransSize;
    pBotIrp->DataIrp.dwActualLength = 0;
    pBotIrp->CswIrp.dwStatus = 0;
    pBotIrp->CswIrp.dwActualLength = 0;
    // clear csw buffer.
    MUSB_MemSet((void *)&pBotIrp->Csw, 0, sizeof(MGC_MsdCsw));

    MUSB_ASSERT(pBot->pDevice);
    /* Device need some delay between CBW/DATA/CSW and next command */
    if (pBot->pDevice->ConnectionSpeed != MUSB_CONNECTION_SPEED_HIGH)
    {
        (void)MUSB_Sleep(10);
    }

    return MUSB_StartTransfer(&(pBotIrp->CbwIrp));
}

static void MGC_BotProtocolCmdComplete(
    void *pPrivateData, int32_t e_nfy_cond, void *pv_data, BOOL fgIsrCallback)
{
    uint32_t dwStatus = 0;
    MGC_MsdBotIrp *pBotIrp;
    MGC_MsdBotProtocol *pBot;
    MUSB_SystemServices *pServices;
    uint32_t dwIrqStatus;
    MUSB_IO_DATA_T *pt_data;    
    MUSB_Irp *pDataIrp;
    uint8_t bWrapperStatus = (uint32_t)pv_data;
    
    MUSB_ASSERT(pPrivateData);
    pBotIrp = (MGC_MsdBotIrp *)pPrivateData;
    pBot = pBotIrp->pBot;
    MUSB_ASSERT(pBot);
    pServices = (MUSB_SystemServices *)pBot->hBusServices;
    MUSB_ASSERT(pServices);
    pDataIrp = &pBotIrp->DataIrp;
    MUSB_ASSERT(pDataIrp);

    // update mbot finish data length.
    pBotIrp->dwGrantLength += pDataIrp->dwActualLength;

    // check if READ10/WRITE10 data match user's request.
    if (((bWrapperStatus != MGC_MSD_DEV_REMOVE) && 
           (bWrapperStatus != MGC_MSD_COMMAND_ABORT)) &&
        (pBotIrp->dwRequestLength >= 512) && 
        (pBotIrp->dwRequestLength > pBotIrp->dwGrantLength))
    {    
        pBot->bBotCmdRetry++;

        // When bWrapperStatus = MGC_MSD_BOT_COMMAND_PASSED, 
        // It is normal case to send next command and process next section of data.        
        if ((bWrapperStatus == MGC_MSD_BOT_COMMAND_PASSED) ||
            (pBot->bBotCmdRetry++ < MGC_MSD_BOT_RETRY))
        {
            dwStatus = MGC_BotProtocolRWRetry(pBotIrp);            
            if (dwStatus == MUSB_STATUS_OK)
            {
                return;
            }
            else            
            {
                MUSB_DIAG1(1, "MSD/BOT: Bot cmd retry error: ", pBot->bBotRetry, 16, 0);
            }            
        }
    }

    // finally, it can not recover after 3 times of reset.
    pBot->bBotCmdRetry = 0;

    // free pBotIrp usage.
    MUSB_ASSERT(pBotIrp->pBot);
    pBotIrp->pBot = NULL;

    /* call completion handler if present */
    if (pBotIrp->pfCmdComplete)
    {
        pt_data = pBotIrp->pt_data;        
        if (pt_data)
        {
            if ((e_nfy_cond == MUSB_HFI_COND_READ_OK) || (e_nfy_cond == MUSB_HFI_COND_WRITE_OK))
            {
                pt_data->u4TransCount =  pt_data->u4Count;            
            }
            else
            {
                pt_data->u4TransCount =  0;
            }
            pBotIrp->pfCmdComplete(pBotIrp->pCompleteParam, e_nfy_cond, pt_data);
        }
        else
        {
            pBotIrp->pfCmdComplete(pBotIrp->pCompleteParam, e_nfy_cond, pv_data);
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

    pBot->pCurrentBotIrp = NULL;

    // process next bot operation in list.
    if (MUSB_ListLength(&(pBot->BotList)) > 0)
    {        
        pBotIrp = (MGC_MsdBotIrp *)MUSB_ListFindItem(&(pBot->BotList), 0);
        MUSB_ASSERT(pBotIrp);
        // remove from list.
        MUSB_ListRemoveItem(&(pBot->BotList), (const void *)pBotIrp);   
        pBot->pCurrentBotIrp = pBotIrp;

        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);                
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        if (!fgIsrCallback)
        {
            dwStatus = pServices->pfClassUnlock(pServices->pPrivateData, 0);
            UNUSED(dwStatus);
        }
#endif

        MUSB_ASSERT(pBot->pDevice);
        /* Some full speed device need to delay between CBW/DATA/CSW and next command */        
        if (pBot->pDevice->ConnectionSpeed != MUSB_CONNECTION_SPEED_HIGH)
        {
            (void)MUSB_Sleep(10);
        }

        dwStatus = MUSB_StartTransfer(&(pBotIrp->CbwIrp));
        UNUSED(dwStatus);
        return;
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

static uint8_t MGC_BotProtocolSendCmd(void *pProtocolData, void *pPrivateData, 
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
    MGC_MsdBotIrp *pBotIrp;
    uint8_t bCmdSize = (uint8_t)MGC_MIN(bCmdLength, 16);
    MGC_MsdBotProtocol *pBot = (MGC_MsdBotProtocol *)pProtocolData;
    MUSB_SystemServices *pServices;
    
    if ((!pBot) || (pBot != pBot->pSelf) ||(!pBot->pDevice))
    {
        LOG(0, "MGC_BotProtocolSendCmd Failed 3\n");
        return FALSE;
    }

    MUSB_ASSERT(pCmdBuffer);
    pServices = (MUSB_SystemServices *)pBot->hBusServices;
    MUSB_ASSERT(pServices);

#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
    dwStatus = pServices->pfClassLock(pServices->pPrivateData, 0);
    UNUSED(dwStatus);    
#endif
    dwIrqStatus = pServices->pfIrqLock(pServices->pPrivateData);      
  
    // Max 10 bot request in queue.
    if (MUSB_ListLength(&(pBot->BotList)) >= MGC_BotMaxIORequest)
    {   
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif
        LOG(1, "USB IORequest full, %d:%d\n", MUSB_ListLength(&(pBot->BotList)), MGC_BotMaxIORequest);
        return FALSE;
    }

    pBotIrp = pBot->pBotIrpBuf;
    // Check command queue is full or not. 
    // (check MUSB_HFI_MAX_IO_REQUEST, MUSB_HFI_DRV_IO_REQUEST separately)
    if( _MGC_CheckCmdQueueFull(pBotIrp, ((pt_data==NULL)?1:0)) )
    {
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif  
        return FALSE;
    }

    pBotIrp = pBot->pBotIrpBuf;
    // search empty pBotIrp.
    for (i=0; i<MGC_BotMaxIORequest; i++)
    {
        MUSB_ASSERT(pBotIrp);    
        if (pBotIrp->pBot == NULL)
        {
            dwFound = 1;
            break;
        }
        pBotIrp ++;
        MGC_BotIORequest = MUSB_MAX(MGC_BotIORequest, i);
        UNUSED(MGC_BotIORequest);
    }    
    if (dwFound == 0)
    {   
        LOG(1, "USB IORequest error = %d\n", i);
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif  
        return FALSE;
    }
    
    MUSB_MemSet(pBotIrp, 0, sizeof(MGC_MsdBotIrp));
    pBotIrp->pBot = pBot;
    
    pBotIrp->Cbw.dCbwSignature = MUSB_SWAP32(MGC_MSD_BOT_CBW_SIGNATURE);
    pBotIrp->Cbw.dCbwTag = MUSB_SWAP32(++(pBot->dwTag));

    // Support slow device: Full/low speed device.
    if ((pBot->pDevice->DeviceDescriptor.idVendor == 0x054C) &&
         (pBot->pDevice->DeviceDescriptor.idProduct == 0x03D8))
    {
        /* Sony MP3 device: 
             idVendor:                        0x054C = Sony Corporation
             idProduct:                       0x03D8
             bcdDevice:                       0x0001        
        */                 
        pBot->dwMaxTransferSize = MAX_MSD_BOT_SMALL_TRANS_SIZE;
    }
    else
    {
        pBot->dwMaxTransferSize = MAX_MSD_BOT_TRANS_SIZE;
    }

//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_BOT_TRANS_SIZE, 
// Mbot driver will concatenate each MAX_MSD_BOT_TRANS_SIZE 
// size of transfer to fulfill with user's request.    
    pBotIrp->Cbw.dCbwDataTransferLength = (uint32_t)
        MUSB_SWAP32(MUSB_MIN(pBot->dwMaxTransferSize, dwDataLength));
    pBotIrp->Cbw.bmCbwFlags = (uint8_t)(bSend ? MGC_MSD_BOT_DIR_OUT : MGC_MSD_BOT_DIR_IN);
    pBotIrp->Cbw.bCbwLun = bLun;
    pBotIrp->Cbw.bCbwCbLength = bCmdSize;
    MUSB_MemCopy((void *)pBotIrp->Cbw.aCbwCb, pCmdBuffer, bCmdSize);

    // init CBW Irp.
    pBotIrp->CbwIrp.hPipe = pBot->hOutPipe;
    pBotIrp->CbwIrp.pBuffer = (uint8_t *)&(pBotIrp->Cbw);
    pBotIrp->CbwIrp.dwLength = 31;
    pBotIrp->CbwIrp.pfIrpComplete = MGC_BotCbwComplete;
    pBotIrp->CbwIrp.pCompleteParam = pBotIrp;
    pBotIrp->CbwIrp.bAllowDma = TRUE;

    /* Some full speed device need to delay between CBW/DATA/CSW and next command */        
    pBotIrp->CbwIrp.bIsrCallback = 
        (pBot->pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_HIGH) ? TRUE:FALSE;
   
    // init DATA Irp
    if ((dwDataLength > 0) && (pDataBuffer))
    {        
        pBotIrp->dwRequestLength = dwDataLength;
        pBotIrp->DataIrp.hPipe = (bSend) ? pBot->hOutPipe : pBot->hInPipe;
        pBotIrp->DataIrp.pBuffer = pDataBuffer;
        pBotIrp->DataIrp.dwLength = (uint32_t)
            MUSB_MIN(pBot->dwMaxTransferSize, dwDataLength);
        pBotIrp->DataIrp.pfIrpComplete = MGC_BotDataComplete;
        pBotIrp->DataIrp.pCompleteParam = pBotIrp;
        pBotIrp->DataIrp.bAllowDma = TRUE;

        /* Some full speed device need to delay between CBW/DATA/CSW and next command */                
        pBotIrp->DataIrp.bIsrCallback = 
            (pBot->pDevice->ConnectionSpeed == MUSB_CONNECTION_SPEED_HIGH) ? TRUE:FALSE;
    }

    // init CSW Irp.
    pBotIrp->CswIrp.hPipe = pBot->hInPipe;
    pBotIrp->CswIrp.pBuffer = (uint8_t *)&(pBotIrp->Csw);
    pBotIrp->CswIrp.dwLength = MGC_MSD_BOT_CSW_LENGTH;
    pBotIrp->CswIrp.pfIrpComplete = MGC_BotCswComplete;
    pBotIrp->CswIrp.pCompleteParam = pBotIrp;
    pBotIrp->CswIrp.bAllowDma = TRUE;
    pBotIrp->CswIrp.bIsrCallback = FALSE;

    // init user's callback function
    pBotIrp->pfCmdComplete = pfCmdComplete;
    pBotIrp->pCompleteParam = pPrivateData;        
    pBotIrp->pt_data = pt_data;        
    
    if (!pBot->pCurrentBotIrp)
    {              
        pBot->bBotRetry = 0;   
        pBot->bBotDataNakReset = FALSE;
        dwStatus = MUSB_StartTransfer(&(pBotIrp->CbwIrp));
        if (dwStatus == 0)
        {
            pBot->pCurrentBotIrp = pBotIrp;
        }
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
        if ( dwStatus != 0 )
        {
            LOG(0, "MGC_BotProtocolSendCmd Failed 1\n");
        }
        return (dwStatus == 0);
     }
    else
    {              
        dwStatus = MUSB_ListAppendItem(&(pBot->BotList), (void *)pBotIrp, 0);
    
        pServices->pfIrqUnlock(pServices->pPrivateData, dwIrqStatus);
#if !defined(MUSB_8226) && !defined(MUSB_LOADER)
        dwRet = pServices->pfClassUnlock(pServices->pPrivateData, 0);
        UNUSED(dwRet);
#endif  
        if ( dwStatus == 0 )
        {
            LOG(0, "MGC_BotProtocolSendCmd Failed 2\n");
        }
        return dwStatus;            
     }
}

static uint8_t MGC_BotProtocolSetDeviceInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiDeviceInfo *pInfo)
{
    MGC_MsdBotProtocol *pBot;
    MGC_MsdBotVolume *pVolume;

    MUSB_ASSERT(pProtocolData);
    MUSB_ASSERT(pInfo);
    pBot = (MGC_MsdBotProtocol *)pProtocolData;
    pVolume = &(pBot->aVolume[bLun]);
    
    /* copy info */
    MUSB_MemCopy(&(pVolume->DeviceInfo), pInfo, sizeof(MUSB_HfiDeviceInfo));

    /* copy USB device info */
    pVolume->DeviceInfo.wVendorId = pBot->pDevice->DeviceDescriptor.idVendor;
    pVolume->DeviceInfo.wProductId = pBot->pDevice->DeviceDescriptor.idProduct;
    pVolume->DeviceInfo.bcdDevice = pBot->pDevice->DeviceDescriptor.bcdDevice;
    pVolume->DeviceInfo.bBusAddress = pBot->pDevice->bBusAddress;
    pVolume->DeviceInfo.bLunCount = pBot->pDevice->bBusAddress;
    pVolume->DeviceInfo.bProtocol = pBot->Protocol.bProtocol;

    return TRUE;
}

static uint8_t MGC_BotProtocolSetMediumInfo(void *pProtocolData, uint8_t bLun,
                                            const MUSB_HfiMediumInfo *pInfo)
{
    MGC_MsdBotProtocol *pBot;
    MGC_MsdBotVolume *pVolume;

    MUSB_ASSERT(pProtocolData);
    MUSB_ASSERT(pInfo);
    pBot = (MGC_MsdBotProtocol *)pProtocolData;
    pVolume = &(pBot->aVolume[bLun]);

    MUSB_MemCopy(&(pVolume->MediumInfo), pInfo, sizeof(MUSB_HfiMediumInfo));
    return TRUE;
}

static void MGC_BotProtocolSetReady(void *pProtocolData, uint8_t bLun, uint8_t bIsReady)
{
    MUSB_HfiStatus Status;
    MGC_MsdBotProtocol *pBot;
    MGC_MsdBotVolume *pVolume;

    MUSB_ASSERT(pProtocolData);
    pBot = (MGC_MsdBotProtocol *)pProtocolData;
    pVolume = &(pBot->aVolume[bLun]);

    if (!pVolume->hVolume)
    {
        Status = MUSB_HfiAddDevice(&(pVolume->hVolume), &(pVolume->DeviceInfo),
                                   &(pVolume->Device));
        if (MUSB_HFI_SUCCESS != Status)
        {
            MUSB_DIAG_STRING(1, "MSD/BOT: HfiAddDevice failed");
            return;
        }
    }
    MUSB_ASSERT(pVolume->hVolume);

    if (bIsReady)
    {
        if (!pVolume->bMediumAnnounced)
        {
            MUSB_HfiMediumInserted(pVolume->hVolume, &(pVolume->MediumInfo));
            pVolume->bMediumAnnounced = TRUE;
        }        
    }
    else
    {
        if (pVolume->bMediumAnnounced)
        {
            pVolume->bMediumAnnounced = FALSE;            
            /* already announced and not ready, so must be medium removal */
            MUSB_HfiMediumRemoved(pVolume->hVolume);
        }
    }
}

static void MGC_MsdBotVolumeInit(MGC_MsdBotVolume *pVolume)
{
    MUSB_ASSERT(pVolume);

    /* fill HFI-side dispatch table */
    pVolume->Device.pPrivateData = pVolume;
    pVolume->Device.pfGetMediumInfo = MGC_MsdBotGetMediumInfo;
    pVolume->Device.pfGetDeviceInfo = MGC_MsdBotGetDeviceInfo;
    pVolume->Device.pfReadDevice = MGC_MsdBotReadDevice;
    pVolume->Device.pfWriteDevice = MGC_MsdBotWriteDevice;
    pVolume->Device.pfWriteProtect = MGC_MsdBotWriteProtect;
}

void MGC_BotControlIrpComplete(void *pParam, MUSB_ControlIrp *pIrp)
{
    uint8_t bIndex;
    MGC_MsdBotVolume *pVolume;
    MGC_MsdBotIrp *pBotIrp;    
    MGC_MsdBotProtocol *pBot = (MGC_MsdBotProtocol *)pParam;
    uint32_t dwStatus;
    uint8_t bRet = FALSE;
    MGC_EnumerationData *pEnumData;
    MGC_Port* pPort;
    MUSB_Device *pUsbDevice;
    
    if (!pBot || (pBot != pBot->pSelf))
    {
        return;
    }

    MUSB_ASSERT(pIrp);
    dwStatus = pIrp->dwStatus;

    if (pBot->bInit)
    {
        /* we must be initializing, so it is GetMaxLUN */
        pBot->bInit = FALSE;
        /* device can stall it, meaning 1 LUN; otherwise a 1-byte "top LUN" */
        pBot->bLunCount = (MUSB_STATUS_OK == pIrp->dwStatus) ? (pBot->aData[0] + 1) : 1;
        pBot->aVolume = (MGC_MsdBotVolume *)MUSB_MemAlloc(
                            pBot->bLunCount *sizeof(MGC_MsdBotVolume));
        if (pBot->aVolume)
        {
            MUSB_MemSet(pBot->aVolume, 0, pBot->bLunCount *sizeof(MGC_MsdBotVolume));

            for (bIndex = 0; bIndex < pBot->bLunCount; bIndex++)
            {
                pVolume = &(pBot->aVolume[bIndex]);

                pVolume->pCmdSetInstance = pBot->pCmdSet->pfCreateInstance(pBot->bLunCount);
                if (pVolume->pCmdSetInstance)
                {
                    pVolume->pBot = pBot;

                    pVolume->bLun = bIndex;
                    MGC_MsdBotVolumeInit(pVolume);
                }
            }
            /* kick off command-set-specific device discovery for first LUN */
            //  MTK Notice: Max Liao, 2006/11/23.
            //  pVolumn must start with right LUN.
            pVolume = pBot->aVolume;
            bRet = pBot->pCmdSet->pfDiscoverDevice(pVolume->pCmdSetInstance, &(pBot->Protocol), 0);
            UNUSED(bRet);
        }

        pPort = (MGC_Port *)pBot->hBus;
        MUSB_ASSERT(pPort);
        pUsbDevice = pBot->pDevice;
        MUSB_ASSERT(pUsbDevice);

        // Report device to user's call back function.
        pUsbDevice->bPortCount = pBot->bLunCount;
        pUsbDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;

        pEnumData = &(pPort->EnumerationData);
        if (pEnumData->pfChildDeviceUpdate)
        {
            pEnumData->pfChildDeviceUpdate(pUsbDevice->pParentUsbDevice,
                pUsbDevice, MUSB_STATUS_CHILD_CONNECT);
        }        
    }
    else if (pBot->bCswRetry)
    {
        pBotIrp = pBot->pCurrentBotIrp;
        if (!pBotIrp)
        {
            return;
        }
        /* data was stalled, so try CSW after clearing */
        pBot->bCswRetry = FALSE;
        //MUSB_FlushPipe(pBot->hInPipe);
        dwStatus = MUSB_StartTransfer(&(pBotIrp->CswIrp));
    }
    else
    {
        /* must be reset */
            switch (pBot->bResetStage)
            {
            case MGC_MSD_BOT_RESET_STAGE_CMD:
                //  MTK Notice: Max Liao, 2006/05/05.	    
                //  Bug fix: Mentor do not clear data toggle bit when send clear_feature to endpoint.	
                VERIFY(0 == MUSB_FlushPipe(pBot->hInPipe));

                /* start clear-in stage */
                pBot->bResetStage = MGC_MSD_BOT_RESET_STAGE_CLEARIN;
                MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotClearHaltData,
                             sizeof(MGC_aMsdBotClearHaltData));
                pBot->aSetup[4] = pBot->pInEnd->bEndpointAddress;
                pBot->ControlIrp.pInBuffer = NULL;
                pBot->ControlIrp.dwInLength = 0;
                dwStatus = MUSB_StartControlTransfer(pBot->ControlIrp.pDevice->pPort,
                                                     &(pBot->ControlIrp));
                break;

            case MGC_MSD_BOT_RESET_STAGE_CLEARIN:
                //  MTK Notice: Max Liao, 2006/05/05.	    
                //  Bug fix: Mentor do not clear data toggle bit when send clear_feature to endpoint.	
                VERIFY(0 == MUSB_FlushPipe(pBot->hOutPipe));

                /* start clear-out stage */
                pBot->bResetStage = MGC_MSD_BOT_RESET_STAGE_CLEAROUT;
                MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotClearHaltData,
                             sizeof(MGC_aMsdBotClearHaltData));
                pBot->aSetup[4] = pBot->pOutEnd->bEndpointAddress;
                pBot->ControlIrp.pInBuffer = NULL;
                pBot->ControlIrp.dwInLength = 0;
                dwStatus = MUSB_StartControlTransfer(pBot->ControlIrp.pDevice->pPort,
                                                     &(pBot->ControlIrp));
                break;

            case MGC_MSD_BOT_RESET_STAGE_CLEAROUT:
                pBot->bResetStage = MGC_MSD_BOT_RESET_STAGE_NONE;
                VERIFY(0 == MUSB_FlushPipe(pBot->hInPipe));
                VERIFY(0 == MUSB_FlushPipe(pBot->hOutPipe));

                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBot->pCurrentBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)MGC_MSD_BOT_COMMAND_FAILED, FALSE);
                break;

            default:
                break;
            } /* END: switch on reset stage */
            if (MUSB_STATUS_OK != dwStatus)
            {
                MUSB_DIAG_STRING(1, "MSD/BOT: reset command submit failed; probably fatal");
            }
    }

    UNUSED(pIrp);
}

static uint32_t MGC_BotSilentReset(MGC_MsdBotIrp *pBotIrp, MUSB_Irp *pIrp)
{       
    MGC_MsdBotProtocol *pBot;

    MUSB_ASSERT(pBotIrp);
    MUSB_ASSERT(pIrp);    
    pBot = pBotIrp->pBot;
    MUSB_ASSERT(pBot);
    
    // reset retry.
    pBot->bBotRetry = 0;
    
    // Cancel irp.
    VERIFY(0 == MUSB_CancelTransfer(pIrp));

    // flush ep before silent reset.
    VERIFY(0 == MUSB_FlushPipe(pBot->hInPipe));
    VERIFY(0 == MUSB_FlushPipe(pBot->hOutPipe));
    
    // perform silent reset when device do not ack properly.
    return MUSB_SilentReset(pBot->pDevice);
}
static void MGC_BotCbwComplete(void *pParam, const MUSB_Irp *pIrp)
{
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_MsdBotIrp *pBotIrp;
    MGC_MsdBotProtocol *pBot;
    MUSB_Irp *pCbwIrp;
    uint32_t dwBotStatus;
    
    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pIrp);    
    pBotIrp = (MGC_MsdBotIrp *)pParam;
    pBot = pBotIrp->pBot;
    MUSB_ASSERT(pBot);
    
    /* handle USB error: MUSB_STATUS_STALLED */
    if (pIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED)
    {
        MUSB_DIAG1(1, "MSD/BOT: USB error on CBW transfer: ", pIrp->dwStatus, 16, 0);
        MGC_MsdBotReset(pBot);
        return;
    }
    else if (pIrp->dwStatus > 0)
    {
        pBot->bBotRetry ++;
        LOG(5, "CBW retry =%d, dwStatus = 0x%X.\n", pBot->bBotRetry, pIrp->dwStatus);

        pCbwIrp = (MUSB_Irp *)&pBotIrp->CbwIrp;
        MUSB_ASSERT(pCbwIrp);
        if (pBot->bBotRetry < MGC_MSD_BOT_CBW_RETRY)
        {
            // init CBW Irp.
            pCbwIrp->dwActualLength = 0;
            pCbwIrp->dwStatus = 0; 
            dwStatus = MUSB_StartTransfer(pCbwIrp);
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
            }
        }
        else
        {       
            dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pCbwIrp);

            dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);

            /* call completion handler if present */
            MGC_BotProtocolCmdComplete(pBotIrp,
                MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);            
        }        
        return;
    }

    MUSB_ASSERT(pBot->pDevice);
    /* Some full speed device need to delay between CBW/DATA/CSW and next command */        
    if (pBot->pDevice->ConnectionSpeed != MUSB_CONNECTION_SPEED_HIGH)
    {
        (void)MUSB_Sleep(10);
    }
    
    // success, reset retry.
    pBot->bBotRetry = 0;
    
    if (pBotIrp->Cbw.dCbwDataTransferLength)
    {
        dwStatus = MUSB_StartTransfer(&(pBotIrp->DataIrp));
    }
    else
    {
        dwStatus = MUSB_StartTransfer(&(pBotIrp->CswIrp));
    }

    if (dwStatus != MUSB_STATUS_OK)
    {        
        dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
    
        /* call completion handler if present */
        MGC_BotProtocolCmdComplete(pBotIrp,
            MUSB_HFI_COND_FAIL, (void *)dwBotStatus, (BOOL)pIrp->bIsrCallback);
    }
}

static void MGC_BotDataComplete(void *pParam, const MUSB_Irp *pIrp)
{
    uint32_t dwStatus = MUSB_STATUS_OK;
    MGC_MsdBotIrp *pBotIrp;
    MGC_MsdBotProtocol *pBot;
    uint32_t dwBotStatus;
    MUSB_Irp *pDataIrp;
    uint32_t dwIndex;
    MGC_MsdBotVolume *pVolume;
    uint32_t dwBlockSize;
    
    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pIrp);    
    pBotIrp = (MGC_MsdBotIrp *)pParam;
    pBot = pBotIrp->pBot;
    MUSB_ASSERT(pBot);
        
    /* handle USB error: MUSB_STATUS_STALLED, and short packet*/
    if (pIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED)
    {
        MUSB_DIAG1(1, "MSD/BOT: USB error on DATA transfer: ", pIrp->dwStatus, 16, 0);
        pBot->bCswRetry = TRUE;

        if (pBotIrp->Cbw.bmCbwFlags & MGC_MSD_BOT_DIR_IN)
        {
            MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotClearHaltData,
                         sizeof(MGC_aMsdBotClearHaltData));

            pBot->aSetup[4] = pBot->pInEnd->bEndpointAddress;
            pBot->ControlIrp.pInBuffer = NULL;
            pBot->ControlIrp.dwInLength = 0;
            dwStatus = MUSB_StartControlTransfer(pBot->ControlIrp.pDevice->pPort,
                                                 &(pBot->ControlIrp));
        }
        else
        {
            MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotClearHaltData,
                         sizeof(MGC_aMsdBotClearHaltData));

            pBot->aSetup[4] = pBot->pOutEnd->bEndpointAddress;
            pBot->ControlIrp.pInBuffer = NULL;
            pBot->ControlIrp.dwInLength = 0;
            dwStatus = MUSB_StartControlTransfer(pBot->ControlIrp.pDevice->pPort,
                                                 &(pBot->ControlIrp));
        }

        if (dwStatus != MUSB_STATUS_OK)
        {        
            dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
        
            /* call completion handler if present */
            MGC_BotProtocolCmdComplete(pBotIrp,
                MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
        }            
        return;
    }
    else if (pIrp->dwStatus > 0)
    {
        pBot->bBotRetry ++;
        LOG(5, "Data retry =%d, dwStatus = 0x%X.\n", pBot->bBotRetry, pIrp->dwStatus);        
    
        // Some card reader do not handle properly when device is reading and plugged out by user.
        // This will cause host nak time out and need to bus reset this card reader.
        // If IRP == nak limit and command == read10 or write10.
        if ((pIrp->dwStatus == (uint32_t)MUSB_STATUS_NACK_LIMIT) &&
            (pIrp->dwLength >= 512) &&
            (pBot->bBotRetry >= MGC_MSD_BOT_DATA_NAK_RETRY))
        {                 
            dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pIrp);
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);

                return;
            }

            // Check nak limt reset already happen.
            // Prevent recursive reset due to device nak limit.
            if (pBot->bBotDataNakReset)
            {
                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)MGC_MSD_COMMAND_ABORT, FALSE);

                pBot->bBotDataNakReset = FALSE;
                return;
            }
            // Set Nak reset to true, and prevent recursive reset device due to nak limit.
            pBot->bBotDataNakReset = TRUE;

            // prepare to retry Bot command.
            pBotIrp->CbwIrp.dwStatus = 0;
            pBotIrp->CbwIrp.dwActualLength = 0;
            pBotIrp->DataIrp.dwStatus = 0;
            pBotIrp->DataIrp.dwActualLength = 0;
            pBotIrp->CswIrp.dwStatus = 0;
            pBotIrp->CswIrp.dwActualLength = 0;
            dwStatus = MUSB_StartTransfer(&(pBotIrp->CbwIrp));
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
            }

            return;            
        }

        // prepare retry irp.
        if (pBot->bBotRetry == 1)
        {
            MUSB_MemCopy((void *)&MGC_MsdRetryIrp, pIrp, sizeof(MUSB_Irp));
        }
        else
        {
            pBotIrp->DataIrp.dwActualLength += pIrp->dwActualLength;
        }
        
        if (pBot->bBotRetry < MGC_MSD_BOT_DATA_RETRY)
        {
            if (pIrp->dwLength > pIrp->dwActualLength)
            {
                MGC_MsdRetryIrp.dwLength = pIrp->dwLength -pIrp->dwActualLength;
                MGC_MsdRetryIrp.pBuffer = (uint8_t*)((uint32_t)pIrp->pBuffer + pIrp->dwActualLength); 
                MGC_MsdRetryIrp.dwActualLength = 0;
                MGC_MsdRetryIrp.dwStatus = 0; 
                dwStatus = MUSB_StartTransfer(&MGC_MsdRetryIrp);
                if (dwStatus != MUSB_STATUS_OK)
                {        
                    dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                        MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
                
                    /* call completion handler if present */
                    MGC_BotProtocolCmdComplete(pBotIrp,
                        MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
                }
                return;                
            }            
            // Success, continue to process csw. Ignore error.
        }
        else
        {              
            // Command fail after retry so many times. Bus reset device and return error code.
            dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pIrp);

            dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);

            /* call completion handler if present */
            MGC_BotProtocolCmdComplete(pBotIrp,
                MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
            return;            
        }
    }

    //  MTK Notice: Max Liao, 2006/11/21.
    //  Some devices return CSW directly bypass DATA stage.
    //  MTK Notice: Max Liao, 2007/11/01.
    //  AIGO MP5 player will return csw = 14 bytes.
    if ((pIrp->dwActualLength == MGC_MSD_BOT_CSW_LENGTH) ||
        (pIrp->dwActualLength == (MGC_MSD_BOT_CSW_LENGTH + 1)))
    {
        // check if CSW signature.
        if ((pIrp->pBuffer[0] == 0x55) && 
             (pIrp->pBuffer[1] == 0x53) && 
             (pIrp->pBuffer[2] == 0x42) && 
             (pIrp->pBuffer[3] == 0x53))
        {
            pDataIrp = (MUSB_Irp *)pIrp;
            // this data must be CSW, modify dwActualLength = 13 to prevent device silent reset.
            pDataIrp->dwActualLength = MGC_MSD_BOT_CSW_LENGTH;
            MUSB_MemCopy((void *)&(pBotIrp->Csw), 
                (void *)pDataIrp->pBuffer, MGC_MSD_BOT_CSW_LENGTH);

            MGC_BotCswComplete(pBotIrp, pDataIrp);
            return;
        }
    }

    // Short packet happen in Read10/Write10 operation.
    if ((pIrp->dwLength >= 512)  &&
        (pIrp->dwLength > pIrp->dwActualLength))
    {          
        // Get block size by lun in aVolume[].
        pVolume = &(pBot->aVolume[pBotIrp->Cbw.bCbwLun]);
        MUSB_ASSERT(pVolume);   
        dwBlockSize = pVolume->MediumInfo.dwBlockSize;
        MUSB_ASSERT(dwBlockSize >= 512);        
    
        //Newman MP4-02 from SZ bug, 2008/06/20.
        //We want to get 246*512 bytes, but it return 118*512 bytes + 13 bytes CSW.
        if ((pIrp->dwActualLength > MGC_MSD_BOT_CSW_LENGTH) &&
            (((pIrp->dwActualLength -MGC_MSD_BOT_CSW_LENGTH) % dwBlockSize) == 0))
        {
            dwIndex = pIrp->dwActualLength - MGC_MSD_BOT_CSW_LENGTH;
           
            // check if CSW signature.
            if ((pIrp->pBuffer[dwIndex] == 0x55) && 
                 (pIrp->pBuffer[dwIndex + 1] == 0x53) && 
                 (pIrp->pBuffer[dwIndex + 2] == 0x42) && 
                 (pIrp->pBuffer[dwIndex + 3] == 0x53))
            {
                LOG(1, "Short Packet but CSW: S=%d, R=%d.\n", 
                    pIrp->dwLength, pIrp->dwActualLength);

                // Actual data length is without CSW.
                pBotIrp->DataIrp.dwActualLength = dwIndex;
                
                pBotIrp->CswIrp.dwActualLength = MGC_MSD_BOT_CSW_LENGTH;
                pBotIrp->CswIrp.dwStatus = pIrp->dwStatus;
                
                MUSB_MemCopy((void *)pBotIrp->CswIrp.pBuffer, 
                    (void *)&pIrp->pBuffer[dwIndex], MGC_MSD_BOT_CSW_LENGTH);

                MGC_BotCswComplete(pBotIrp, &pBotIrp->CswIrp);
                return;
            }
        }

        LOG(1, "Short Packet: S=%d, R=%d.\n", pIrp->dwLength, pIrp->dwActualLength);
        dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pIrp);

        dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);

        /* call completion handler if present */
        MGC_BotProtocolCmdComplete(pBotIrp,
            MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
        return;            
    }        

    // prepare retry irp.
    if (pBot->bBotRetry > 0)
    {
        pBotIrp->DataIrp.dwActualLength += pIrp->dwActualLength;
        // success, reset retry.
        pBot->bBotRetry = 0;
    }

    MUSB_ASSERT(pBot->pDevice);
    /* Some full speed device need to delay between CBW/DATA/CSW and next command */        
    if (pBot->pDevice->ConnectionSpeed != MUSB_CONNECTION_SPEED_HIGH)
    {
        (void)MUSB_Sleep(10);
    }

    /* get CSW status */
    dwStatus = MUSB_StartTransfer(&(pBotIrp->CswIrp));
    if (dwStatus != MUSB_STATUS_OK)
    {        
        dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
        /* call completion handler if present */
        MGC_BotProtocolCmdComplete(pBotIrp,
            MUSB_HFI_COND_FAIL, (void *)dwBotStatus, (BOOL)pIrp->bIsrCallback);
    }            
}

static void MGC_BotCswComplete(void *pParam, const MUSB_Irp *pIrp)
{
    uint32_t dwStatus = 0;
    MGC_MsdBotIrp *pBotIrp;
    MGC_MsdBotProtocol *pBot;
    int32_t e_nfy_cond = MUSB_HFI_COND_FAIL;
    MUSB_Irp *pCswIrp;
    uint32_t dwBotStatus;
    uint32_t dCswDataResidue;
    
    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pIrp);

    pBotIrp = (MGC_MsdBotIrp *)pParam;
    pBot = pBotIrp->pBot;
    MUSB_ASSERT(pBot);
    
    /* handle USB error: MUSB_STATUS_STALLED */
    if (pIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED)
    {
        MUSB_DIAG1(1, "MSD/BOT: USB error on CSW: ", pIrp->dwStatus, 16, 0);

        if (((uint32_t)MUSB_STATUS_STALLED == pIrp->dwStatus) && !pBot->bCswRetry)
        {
            pBot->bCswRetry = TRUE;

            MUSB_MemCopy(pBot->aSetup, MGC_aMsdBotClearHaltData, sizeof(MGC_aMsdBotClearHaltData));
            pBot->aSetup[4] = pBot->pInEnd->bEndpointAddress;
            pBot->ControlIrp.pInBuffer = NULL;
            pBot->ControlIrp.dwInLength = 0;
            dwStatus = MUSB_StartControlTransfer(pBot->ControlIrp.pDevice->pPort,
                                                 &(pBot->ControlIrp));
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
            }
            return;
        }

        pBot->bCswRetry = FALSE;
        MGC_MsdBotReset(pBot);
        return;
    }
    else if (pIrp->dwStatus > 0)
    {
        pBot->bBotRetry ++;
        LOG(5, "CSW retry =%d, dwStatus = 0x%X.\n", pBot->bBotRetry, pIrp->dwStatus);        
        pCswIrp = (MUSB_Irp *)&pBotIrp->CswIrp;
        MUSB_ASSERT(pCswIrp);
        if (pBot->bBotRetry < MGC_MSD_BOT_CSW_RETRY)
        {        
            // init CBW Irp.
            pCswIrp->dwActualLength = 0;
            pCswIrp->dwStatus = 0; 
            dwStatus = MUSB_StartTransfer(pCswIrp);
            if (dwStatus != MUSB_STATUS_OK)
            {        
                dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                    MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
                /* call completion handler if present */
                MGC_BotProtocolCmdComplete(pBotIrp,
                    MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
            }
        }
        else
        {       
            dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pCswIrp);

            dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
                MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);

            /* call completion handler if present */
            MGC_BotProtocolCmdComplete(pBotIrp,
                MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);            
        }
        return;
    }    
    
    /* check dwTag and signature */
    if ((MGC_MSD_BOT_COMMAND_PASSED == pBotIrp->Csw.bCswStatus) &&
        ((pIrp->dwActualLength != MGC_MSD_BOT_CSW_LENGTH) ||
         (pBotIrp->Csw.dCswSignature != MGC_MSD_BOT_CSW_SIGNATURE) ||
        (pBotIrp->Csw.dCswTag != pBotIrp->Cbw.dCbwTag)))
    {       
        LOG(5, "CSW: pIrp->dwActualLength=%d.\n", pIrp->dwActualLength);
        LOG(5, "CSW: pBotIrp->Csw.dCswSignature=0x%08X.\n", pBotIrp->Csw.dCswSignature);
        LOG(5, "CSW: pBotIrp->Csw.dCswTag=0x%08X.\n", pBotIrp->Csw.dCswTag);
        LOG(5, "CSW: pBotIrp->Cbw.dCbwTag=0x%08X.\n", pBotIrp->Cbw.dCbwTag);

        dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pIrp);

        dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
        /* call completion handler if present */
        MGC_BotProtocolCmdComplete(pBotIrp,
            MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);

        return;
    }

    /* check for explicitly-signalled phase error */
    if (MGC_MSD_BOT_PHASE_ERROR == pBotIrp->Csw.bCswStatus)
    {
        MUSB_DIAG_STRING(1, "MSD/BOT: phase error");
        dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pIrp);

        dwBotStatus = (uint32_t)((dwStatus == (uint32_t)MUSB_STATUS_PEER_REMOVED) ?
            MGC_MSD_DEV_REMOVE: MGC_MSD_COMMAND_ABORT);
            
        /* call completion handler if present */
        MGC_BotProtocolCmdComplete(pBotIrp,
            MUSB_HFI_COND_FAIL, (void *)dwBotStatus, FALSE);
        return;                    
    }
    else if (MGC_MSD_BOT_COMMAND_PASSED == pBotIrp->Csw.bCswStatus)
    {
        // check data residue.
        if (pBotIrp->Csw.dCswDataResidue > pBotIrp->Cbw.dCbwDataTransferLength)
        {
            //  MTK Notice: Max Liao, 2006/11/20.
            //  Some device will return dCswDataResidue in big endian order !! 
            //  Windows do not check this value.                        
            dCswDataResidue = MUSB_BIG_ENDIAN_SWAP32(pBotIrp->Csw.dCswDataResidue);
            if (dCswDataResidue > pBotIrp->Cbw.dCbwDataTransferLength)
            {
                MUSB_DIAG_STRING(1, "MSD/BOT: DataResidue error");
                dwStatus= MGC_BotSilentReset(pBotIrp, (MUSB_Irp *)pIrp);
                UNUSED(dwStatus);       
             }
        }
        else
        {
            e_nfy_cond = (int32_t)((pBotIrp->Cbw.bmCbwFlags & MGC_MSD_BOT_DIR_IN) ? 
                MUSB_HFI_COND_READ_OK : MUSB_HFI_COND_WRITE_OK);
        }    
    }

    /* call completion handler if present */
    MGC_BotProtocolCmdComplete(pBotIrp,
        e_nfy_cond, (void *)((uint32_t)(pBotIrp->Csw.bCswStatus)), FALSE);        
}

/**************************************************************************
HFI functions
**************************************************************************/
static MUSB_HfiStatus MGC_MsdBotGetMediumInfo(void *pDeviceData, MUSB_HfiMediumInfo **ppMediumInfo)
{
    MGC_MsdBotVolume *pVolume;
    
    MUSB_ASSERT(pDeviceData);
    MUSB_ASSERT(ppMediumInfo);        
    pVolume = (MGC_MsdBotVolume *)pDeviceData;
    
    *ppMediumInfo = &(pVolume->MediumInfo);

    return MUSB_HFI_SUCCESS;
}

static MUSB_HfiStatus MGC_MsdBotGetDeviceInfo(void *pDeviceData, MUSB_HfiDeviceInfo **ppDeviceInfo)
{
    MGC_MsdBotVolume *pVolume;
    
    MUSB_ASSERT(pDeviceData);
    MUSB_ASSERT(ppDeviceInfo);        
    pVolume = (MGC_MsdBotVolume *)pDeviceData;

    *ppDeviceInfo = &(pVolume->DeviceInfo);

    return MUSB_HFI_SUCCESS;
}

static MUSB_HfiStatus MGC_MsdBotReadDevice(void *pDeviceData,
                                           uint64_t ddwStartBlock,
                                           uint32_t dwByteCount,
                                           uint8_t *pBuffer)
{
    uint8_t bOk;
    uint8_t bCmdLength = 0;
    uint8_t aCmdBuffer[16];
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;    
    MGC_MsdBotVolume *pVolume;
    MGC_MsdBotProtocol *pBot;
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
    pVolume = (MGC_MsdBotVolume *)pDevice->pPrivateData;
    MUSB_ASSERT(pVolume);    
    if (!pVolume->bMediumAnnounced)
    {
        return MUSB_HFI_ERROR_MEDIA_REMOVED;
    }

    pBot = pVolume->pBot;
    MUSB_ASSERT(pBot);
    MUSB_ASSERT(pVolume->MediumInfo.dwBlockSize);

//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_BOT_TRANS_SIZE, 
// Mbot driver will concatenate each MAX_MSD_BOT_TRANS_SIZE 
// size of transfer to fulfill with user's request.
    dwTransCount = MUSB_MIN(dwByteCount, pBot->dwMaxTransferSize);
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
    
    bOk = MGC_BotProtocolSendCmd(pBot, pt_nfy_info->pvHfiNfyTag, 
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

static MUSB_HfiStatus MGC_MsdBotWriteDevice(void *pDeviceData,
                                            uint64_t ddwStartBlock,
                                            uint32_t dwByteCount,
                                            uint8_t *pBuffer)
{
    uint8_t bOk;
    uint8_t bCmdLength = 0;
    uint8_t aCmdBuffer[16];
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;        
    MGC_MsdBotVolume *pVolume;
    MGC_MsdBotProtocol *pBot;
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
    pVolume = (MGC_MsdBotVolume *)pDevice->pPrivateData;
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

    pBot = pVolume->pBot;
    MUSB_ASSERT(pBot);
    MUSB_ASSERT(pVolume->MediumInfo.dwBlockSize);
//Max transfer size of READ10/WRITE10 command.
// If user request > MAX_MSD_BOT_TRANS_SIZE, 
// Mbot driver will concatenate each MAX_MSD_BOT_TRANS_SIZE 
// size of transfer to fulfill with user's request.    
    dwTransCount = MUSB_MIN(dwByteCount, pBot->dwMaxTransferSize);
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

    bOk = MGC_BotProtocolSendCmd(pBot, pt_nfy_info->pvHfiNfyTag, 
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

static MUSB_HfiStatus MGC_MsdBotWriteProtect(void *pDeviceData)
{
    uint8_t bOk;
    uint8_t bCmdLength = 0;
    MGC_UcFsDevice *pUcFsDevice;
    MUSB_HfiDevice *pDevice;            
    MGC_MsdBotVolume *pVolume;
    MGC_ScsiCmdSetData* pScsi;
    MGC_MsdBotProtocol *pBot;
    MUSB_NFY_INFO_T *pt_nfy_info;   
    MUSB_IO_DATA_T *pt_data;
    
    MUSB_ASSERT(pDeviceData);
    pUcFsDevice = (MGC_UcFsDevice *)pDeviceData;
    MUSB_ASSERT(pUcFsDevice->pvDevice);
    pDevice = (MUSB_HfiDevice *)pUcFsDevice->pvDevice;
    MUSB_ASSERT(pDevice->pPrivateData);    
    pVolume = (MGC_MsdBotVolume *)pDevice->pPrivateData;
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

    pBot = pVolume->pBot;
    MUSB_ASSERT(pBot);

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

    bOk = MGC_BotProtocolSendCmd(pBot, pt_nfy_info->pvHfiNfyTag, pt_data,
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
