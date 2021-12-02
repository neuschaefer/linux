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
 * 
 * $Revision: #1 $
 */
#ifdef MUSB_SICD

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"

#include "mu_sicdutil.h"


/*************************** CONSTANTS ****************************/
/**************************** GLOBALS *****************************/

/*************************** Internal FUNCTIONS ****************************/

/*******************************************************************************
**  _MUSBSicd_FindCurrentPipeEndpointNumber
**
**  descriptions:
**       Find the end point number of current state. 
**  parameters:
**  result:
**  return:
**      End point number.
**  note:
*******************************************************************************/
static uint8_t _MUSBSicd_FindCurrentPipeEndpointNumber(MUSB_SicdProtocol *pSicdProtocol)
{
    uint8_t bEndpointNumber=0;
    MUSB_SicdStateControl *pSicdStateControl;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    switch ( pSicdStateControl->StateFlow[1].eOpeartionType )
    {
    case MUSB_SICD_CMD_OPERATION:
        bEndpointNumber = ((MGC_Pipe*)(pSicdProtocol->hInPipe))->pLocalEnd->bBusEnd;
        break;
        
    case MUSB_SICD_DATA_OPERATION:
        if ( pSicdStateControl->SicdCommand.rDataBlock.eDataDirection == MUSB_SICD_DATA_OUT )
        {
            bEndpointNumber = ((MGC_Pipe*)(pSicdProtocol->hInPipe))->pLocalEnd->bBusEnd;
        }
        else
        {
            bEndpointNumber = ((MGC_Pipe*)(pSicdProtocol->hInPipe))->pLocalEnd->bRxBusEnd;
        }
        break;
        
    case MUSB_SICD_RESPONSE_OPERATION:
        bEndpointNumber = ((MGC_Pipe*)(pSicdProtocol->hInPipe))->pLocalEnd->bRxBusEnd;
        break;

    case MUSB_SICD_STANDARD_OPERATION:
        LOG(1, "clear feature for control pipe!\n", NULL);
        bEndpointNumber = 0;
        break;

    case MUSB_SICD_NO_OPERATION:
    case MUSB_SICD_NOTIFY_OPERATION:
    default: 
        LOG(1, "_MUSBSicd_FindCurrentPipeEndpointNumber unknown!\n", NULL);
        bEndpointNumber = 0;
        break;
    }

    return bEndpointNumber;
}

/*************************** FUNCTIONS ****************************/

/*******************************************************************************
**  MUSB_SicdStartInterruptPipeListening
**
**  descriptions:
**      Start listening interrupt pipe. Whenever we receive an interrupt packet, 
**      MUSB_SicdInterruptIrpComplete will be invoked.
**      (This function has not been tested!!)
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
#if MGC_SICD_OPEN_INTERRUPT_PIPE
MUSB_SicdResult MUSB_SicdStartInterruptPipeListening(MUSB_SicdProtocol* pSicdProtocol)
{
    uint32_t dwStatus;
    MUSB_SicdResult Result;
    MUSB_SicdStateControl *pSicdStateControl;
    uint8_t *pbRecvBuffer;
    MUSB_Irp* pIrp;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    
    pbRecvBuffer = pSicdStateControl->SicdCommand.abEventBuffer;
    (void)MUSB_MemSet(pbRecvBuffer, (uint8_t)0, MUSB_SICD_MAX_EVENT_BUFFER_LENGTH);

    pIrp = &(pSicdProtocol->SicdStateControl.IntrIrp);
    MUSB_MemSet((void *)pIrp, 0, sizeof(MUSB_Irp));    
    pIrp->hPipe = pSicdProtocol->hIntrPipe;
    pIrp->pBuffer = pbRecvBuffer;
    pIrp->dwLength = MUSB_SICD_MAX_EVENT_BUFFER_LENGTH;
    pIrp->pfIrpComplete = MUSB_SicdInterruptIrpComplete;
    pIrp->pCompleteParam = pSicdProtocol;

    dwStatus = MUSB_StartTransfer(pIrp);
    Result = (MUSB_STATUS_OK == dwStatus) ? 
        (MUSB_SicdResult)MUSB_SICD_SUCCESS : (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    
    return Result;
}
#endif

/*******************************************************************************
**  MUSB_SicdGetNextData
**
**  descriptions:
**      Get next Data Phase data.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdGetNextData(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;    
    MUSB_SicdResult Result;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }    
    
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];

    if( (pSicdStateControl->eDriverState != 
                                            MUSB_SICD_DRIVER_STATE_RUNNING) ||
        (pStateFlow->eOpeartionType != 
                                                MUSB_SICD_DATA_OPERATION) )
    {
           LOG(1, "MUSB_SicdGetNextData: State Error !\n", NULL);
           return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }

    Result = MUSB_SicdHandleOperation(pSicdProtocol);
    if ( Result != MUSB_SICD_SUCCESS )
    {
        LOG(1, "SICD: Can't Handle Op\n", Result);
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
    }
    
    return Result;
}

/*******************************************************************************
**  MUSB_SicdOperationAbort
**
**  descriptions:
**      Handle abort.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdOperationAbort(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;    
    MUSB_SicdResult Result;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }    
    
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];

    if ( (pSicdStateControl->eDriverState != MUSB_SICD_DRIVER_STATE_RUNNING) ||
         (pStateFlow->eOpeartionType == MUSB_SICD_RESPONSE_OPERATION) )
    {
        LOG(1, "SICD: Abort in driver not running or response phase! \n", NULL);
        return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }
    
    Result = MUSB_SicdCreateAbortFlow(pSicdProtocol);
    if(Result != MUSB_SICD_SUCCESS)
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;    
        LOG(1, "SICD: Create Abort State Error %d\n", Result);
        return Result;
    }
    
    //Driver state machine enter to STOPPING state
    Result = MUSB_SicdHandleOperation(pSicdProtocol);
    if ( Result != MUSB_SICD_SUCCESS )
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;    
        LOG(1, "SICD: Can't Handle Op %d\n", Result);
    }
    
    return Result;  
}

/*******************************************************************************
**  MUSB_SicdDeviceReset
**
**  descriptions:
**      Handle device reset.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdDeviceReset(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdResult Result;
    MUSB_SicdStateControl* pSicdStateControl;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    if (pSicdStateControl->eDriverState != MUSB_SICD_DRIVER_STATE_READY)
    {
        LOG(1, "SICD: eDriverState error = %d.\n", pSicdStateControl->eDriverState);
        return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }

    Result = MUSB_SicdCreateResetFlow(pSicdProtocol);
    if(Result != MUSB_SICD_SUCCESS)
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;    
        LOG(1, "SICD: Fail to create reset flow %d\n", Result);
        return Result;
    }

    Result = MUSB_SicdHandleOperation(pSicdProtocol);
    if ( Result != MUSB_SICD_SUCCESS )
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
        LOG(1, "SICD: Can't Handle Op %d\n", Result);
    }

    return Result;
}

/*******************************************************************************
**  MUSB_SicdGetExtEventData
**
**  descriptions:
**      Handle get extended event data.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdGetExtEventData(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdResult Result;
    MUSB_SicdStateControl* pSicdStateControl;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    if (pSicdStateControl->eDriverState != MUSB_SICD_DRIVER_STATE_READY)
    {
       LOG(1, "SICD: eDriverState error = %d\n", pSicdStateControl->eDriverState);
       return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }

    pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_RUNNING;
    
    Result = MUSB_SicdCreateGetExtEventDataFlow(pSicdProtocol);
    if(Result != MUSB_SICD_SUCCESS)
    {
       pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
       LOG(1, "SICD: Fail to create get next data flow %d\n", Result);
       return Result;
    }

    Result = MUSB_SicdHandleOperation(pSicdProtocol);
    if ( Result != MUSB_SICD_SUCCESS )
    {
        pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
        LOG(1, "SICD: Can't Handle Op %d\n", Result);
    }

    return Result;
}

/*******************************************************************************
**  MUSB_SicdSilentReset
**
**  descriptions:
**      Do bus reset. This is used for devices which are not support device reset.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdSilentBusReset(MUSB_SicdProtocol* pSicdProtocol)
{
    uint32_t dwStatus = MUSB_STATUS_OK;

    MUSB_ASSERT(pSicdProtocol);

    // flush ep befor silent reset.
    VERIFY(0 == MUSB_FlushPipe(pSicdProtocol->hInPipe));
    VERIFY(0 == MUSB_FlushPipe(pSicdProtocol->hOutPipe));

    #if MGC_SICD_OPEN_INTERRUPT_PIPE
    // Cancel irp.
    VERIFY(0 == MUSB_CancelTransfer(&(pSicdProtocol->SicdStateControl.IntrIrp)));
    VERIFY(0 == MUSB_FlushPipe(pSicdProtocol->hIntrPipe));
    #endif

    // Since middleware will not invoke bus reset directly, we dont use
    // MUSB_HFI_NFY_SILENT_BUS_RESET to notify upper layer.
    dwStatus = MUSB_SilentReset(pSicdProtocol->pDevice);
    
    if (dwStatus != MUSB_STATUS_OK)
    {
        LOG(1, "MUSB_SicdSilentBusReset error %d\n", dwStatus);
        return dwStatus;
    }

    // return to ready state
    (void)MUSB_SicdCreateSilentBusResetFlow(pSicdProtocol);
    
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}


/*******************************************************************************
**  MUSB_SicdTransferCommandBlock
**
**  descriptions:
**      Transfer command block.
**      Important Note !! The callback of command phase is executed in BSR.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferCommandBlock(MUSB_SicdProtocol* pSicdProtocol)
{
    uint32_t dwStatus;
    MUSB_SicdResult Result;
    MUSB_SICD_BLOCK_T* pCommandBlock;
    MUSB_SICD_CONTAINER* pContainer;
    MUSB_Irp* pIrp;
    
    MUSB_ASSERT(pSicdProtocol);
    pCommandBlock = &(pSicdProtocol->SicdStateControl.SicdCommand.rCommandBlock);
    MUSB_ASSERT(pCommandBlock);

    if ( pCommandBlock->dwPayLoadBufLen > 
        (MUSB_SICD_MAX_CMD_RESPONSE_LENGTH-MUSB_SICD_HEADER_MINIMUM_LENGTH) )
    {
        LOG(1, "SICD: Payload length error %d.\n", pCommandBlock->dwPayLoadBufLen);
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }

    pContainer = (MUSB_SICD_CONTAINER *)
        &(pSicdProtocol->SicdStateControl.SicdCommand.abGenericTransferBuffer[0]);
    pIrp = &(pSicdProtocol->SicdStateControl.BulkIrp);
    MUSB_MemSet((void *)pIrp, 0, sizeof(MUSB_Irp));

    pContainer->wContainerType = (uint16_t)pCommandBlock->eDataType;
    pContainer->wOperationCode = pCommandBlock->wOperationCode;
    pContainer->dwTransactionId = pCommandBlock->dwTranscationID;
    pContainer->dwContainerLength = 
        pCommandBlock->dwPayLoadBufLen+MUSB_SICD_HEADER_MINIMUM_LENGTH;

    pIrp->hPipe = pSicdProtocol->hOutPipe;
    pIrp->pBuffer = (uint8_t*)pContainer;
    pIrp->dwLength = pContainer->dwContainerLength;
    pIrp->pfIrpComplete = MUSB_SicdBulkIrpComplete;
    pIrp->pCompleteParam = pSicdProtocol;

    dwStatus = MUSB_StartTransfer(pIrp);
    Result = (MUSB_STATUS_OK == dwStatus) ? (MUSB_SicdResult)MUSB_SICD_SUCCESS : 
                                            (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    
    return Result;
}

/*******************************************************************************
**  MUSB_SicdTransferDataBlock
**
**  descriptions:
**      Transfer data block.
**      Important Note !! The callback of data phase is executed in BSR.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferDataBlock(MUSB_SicdProtocol* pSicdProtocol)
{
    uint32_t dwStatus;
    MUSB_SicdResult Result;
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SICD_BLOCK_T* pSicdIoDataBlock;
    MUSB_SicdCommand* pSicdCommand;
    uint8_t *pbPayloadBuffer;
    MUSB_Irp *pIrp;
    uint32_t dwBufferLengthTmp;
    uint32_t i,j;
    uint32_t dwRemainDataSize=0;
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    pSicdCommand = &(pSicdStateControl->SicdCommand);
    pSicdIoDataBlock = &(pSicdCommand->rDataBlock);
    MUSB_ASSERT(pSicdIoDataBlock->pbPayLoadBuf);
    pIrp = &(pSicdProtocol->SicdStateControl.BulkIrp);
    MUSB_MemSet(pIrp, 0, sizeof(MUSB_Irp));

    if ( pSicdIoDataBlock->eDataDirection == MUSB_SICD_DATA_OUT )
    {
        // SICD state machine should be handled by upper layer(MW) when DATA OUT.
        // Note that if the out data size is larger then MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH, 
        //  there will be multiple SICDDEV_SET_TYPE_SEND_NEXT_DATA request from MW.
        
        // It's the first packet of the first data out request, 
        //  handle the 12 bytes SICD header here.
        if ( pSicdStateControl->dwDataPhaseCurrentWriteOffset == 0 )
        {
            MUSB_ASSERT(pSicdCommand->abTempOutBuffer);
            (void)MUSB_MemSet(pSicdCommand->abTempOutBuffer, (uint8_t)0, 
                                        MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH);
            
            (*(uint32_t*)&pSicdCommand->abTempOutBuffer[0]) = (uint32_t)(pSicdStateControl->dwDataPhaseTotalDataSize+MUSB_SICD_HEADER_MINIMUM_LENGTH);   // Container Length
            (*(uint16_t*)&pSicdCommand->abTempOutBuffer[4]) = (uint16_t)(MUSB_SICD_DATA_BLOCK);   // Container Type
            (*(uint16_t*)&pSicdCommand->abTempOutBuffer[6]) = (uint16_t)(pSicdIoDataBlock->wOperationCode);  // Code
            (*(uint32_t*)&pSicdCommand->abTempOutBuffer[8]) = (uint32_t)(pSicdIoDataBlock->dwTranscationID); // Transaction Id

            for ( i = 0 ; (i < (MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH-MUSB_SICD_HEADER_MINIMUM_LENGTH)) && 
                                    (i < pSicdIoDataBlock->dwPayLoadBufLen) ; i++ )
            {
                pSicdCommand->abTempOutBuffer[i+MUSB_SICD_HEADER_MINIMUM_LENGTH] = 
                                            pSicdIoDataBlock->pbPayLoadBuf[i];
            }

            pbPayloadBuffer = pSicdCommand->abTempOutBuffer;
            dwBufferLengthTmp = i+MUSB_SICD_HEADER_MINIMUM_LENGTH;
        }
        // It's the first data out request but not the first packet, 
        //      handle the rest bytes of the first data out request.
        else if ( pSicdStateControl->dwDataPhaseCurrentWriteOffset < 
                                            pSicdIoDataBlock->dwPayLoadBufLen )
        {
            MUSB_ASSERT(pSicdCommand->abTempOutBuffer);
            (void)MUSB_MemSet(pSicdCommand->abTempOutBuffer, (uint8_t)0, 
                                        MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH);

            // Copy send buffer provided by upper layer to abTempOutBuffer.
            // i : index of abTempOutBuffer
            // j : index of send buffer provided by upper layer.
            for (   i = 0,
                    (j = pSicdStateControl->dwDataPhaseCurrentWriteOffset) ; 
                    (i < MUSB_SICD_MAX_TEMP_OUT_BUFFER_LENGTH) &&
                    (j < pSicdIoDataBlock->dwPayLoadBufLen) ; i++, j++ )
            {
                pSicdCommand->abTempOutBuffer[i] = 
                                            pSicdIoDataBlock->pbPayLoadBuf[j];
            }

            pbPayloadBuffer = pSicdCommand->abTempOutBuffer;
            dwBufferLengthTmp = i;
        }
        // It's not the first data out request, just send it out.
        else
        {
            pbPayloadBuffer = pSicdIoDataBlock->pbPayLoadBuf;
            dwBufferLengthTmp = pSicdIoDataBlock->dwPayLoadBufLen;
        }
        pIrp->hPipe = pSicdProtocol->hOutPipe;
        pIrp->pBuffer = pbPayloadBuffer;
        pIrp->dwLength = dwBufferLengthTmp;
    }
    else if ( pSicdIoDataBlock->eDataDirection == MUSB_SICD_DATA_IN )
    {
        // Client buffer size is greater than 12 bytes.
        if ( pSicdIoDataBlock->dwPayLoadBufLen > MUSB_SICD_HEADER_MINIMUM_LENGTH )
        {
            pbPayloadBuffer = pSicdIoDataBlock->pbPayLoadBuf;
            dwBufferLengthTmp = pSicdIoDataBlock->dwPayLoadBufLen;

            // if this is first data phase packet, we update buf len to not bigger than 512 bytes.
            if ( pSicdStateControl->dwDataPhaseCurrentReadOffset == 0 )
            {
                // This is to improve performance. 
                // Since we will do memory copy for all data of first data ready callback, 
                // here we do this to do memory copy only for 512-12 byte. 
                // We will do DMA for all later data phase data.
                if ( dwBufferLengthTmp > 512 )
                {
                    dwBufferLengthTmp = 512;
                }
            }
        }
        // Client buffer size is smaller than 12 byte
        else    // Upper layer doesn't provide read in buffer
        {
            LOG(1,"Client buffer is not bigger than 12 bytes %d.\n", 
                pSicdIoDataBlock->dwPayLoadBufLen);
            pbPayloadBuffer = pSicdStateControl->SicdCommand.abGenericTransferBuffer;
            dwBufferLengthTmp = MUSB_SICD_MAX_CMD_RESPONSE_LENGTH;
        }
        
        // If it's not first data phase packet, calculate remain data size.
        if ( pSicdStateControl->dwDataPhaseCurrentReadOffset != 0 )
        {
            dwRemainDataSize = pSicdStateControl->dwDataPhaseTotalDataSize - 
                            pSicdStateControl->dwDataPhaseCurrentReadOffset;
        }

        // If remain data size is smaller than buffer size, we just acquire
        // the remain data size data.
        if ( (dwRemainDataSize != 0) && (dwRemainDataSize < dwBufferLengthTmp) )
        {
            dwBufferLengthTmp = dwRemainDataSize;
        }
        
        (void)MUSB_MemSet(pbPayloadBuffer, (uint8_t)0, dwBufferLengthTmp);
        pIrp->hPipe = pSicdProtocol->hInPipe;
        pIrp->pBuffer = pbPayloadBuffer;
        pIrp->dwLength = dwBufferLengthTmp;
    }
    
    pIrp->pfIrpComplete = MUSB_SicdBulkIrpComplete;
    pIrp->pCompleteParam = pSicdProtocol;
    pIrp->bIsrCallback = (uint8_t)FALSE;
    pIrp->bAllowDma = (uint8_t)TRUE;

    dwStatus = MUSB_StartTransfer(pIrp);
    Result = (MUSB_SicdResult)
        ((MUSB_STATUS_OK == dwStatus) ? MUSB_SICD_SUCCESS : MUSB_E_SICD_DRIVER_ERROR);
    
    return Result;
}

/*******************************************************************************
**  MUSB_SicdTransferResponseBlock
**
**  descriptions:
**      Transfer response block.
**      Important Note !! The callback of response phase is executed in BSR.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferResponseBlock(MUSB_SicdProtocol* pSicdProtocol)
{
    uint32_t dwStatus;
    MUSB_SicdResult Result;
    MUSB_SicdStateControl *pSicdStateControl;
    uint8_t *pbRecvBuffer;
    MUSB_Irp* pIrp;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    
    pbRecvBuffer = pSicdStateControl->SicdCommand.abGenericTransferBuffer;
    (void)MUSB_MemSet(pbRecvBuffer, (uint8_t)0, MUSB_SICD_MAX_CMD_RESPONSE_LENGTH);
    pIrp = &(pSicdProtocol->SicdStateControl.BulkIrp);
    MUSB_MemSet(pIrp, 0, sizeof(MUSB_Irp));
    
    pIrp->hPipe = pSicdProtocol->hInPipe;
    pIrp->pBuffer = pbRecvBuffer;
    pIrp->dwLength = MUSB_SICD_MAX_CMD_RESPONSE_LENGTH;
    pIrp->pfIrpComplete = MUSB_SicdBulkIrpComplete;
    pIrp->pCompleteParam = pSicdProtocol;
    pIrp->bIsrCallback = (uint8_t)FALSE;

    dwStatus = MUSB_StartTransfer(pIrp);
    Result = (MUSB_SicdResult)
        ((MUSB_STATUS_OK == dwStatus) ? MUSB_SICD_SUCCESS : MUSB_E_SICD_DRIVER_ERROR);
    
    return Result;
}

/*******************************************************************************
**  MUSB_SicdTransferGetDeviceStatus
**
**  descriptions:
**      Transfer sicd class-specific request : get device status.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferGetDeviceStatus(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pSicdProtocol);
    pSicdDevice = pSicdProtocol->pDevice->pDriverPrivateData;
    pSetup = &(pSicdDevice->Setup);
    pControlIrp = &(pSicdDevice->ControlIrp);
    
    LOG(5, "SICD: get device status\n", NULL);

    /** Prepare the Setup Packet for sending Set Config Request */
    pSetup->bmRequestType = (uint8_t)  (MUSB_DIR_IN | MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE);
    pSetup->bRequest      = (uint8_t)  MUSB_SICD_GET_STATUS_REQUEST_CODE;
    pSetup->wValue        = (uint16_t) 0;
    pSetup->wIndex        = (uint16_t) 0;
    pSetup->wLength       = (uint16_t) MUSB_SICD_MAX_CMD_RESPONSE_LENGTH;

    /** Fill Control Irp */
    pControlIrp->pDevice           = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer        = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength       = sizeof(MUSB_DeviceRequest);
    pControlIrp->pInBuffer         = pSicdProtocol->SicdStateControl.SicdCommand.abGenericTransferBuffer;
    pControlIrp->dwInLength        = MUSB_SICD_MAX_CMD_RESPONSE_LENGTH;
    pControlIrp->dwStatus          = 0;
    pControlIrp->dwActualOutLength = 0;
    pControlIrp->dwActualInLength  = 0;
    pControlIrp->pfIrpComplete     = MUSB_SicdControlIrpComplete;
    pControlIrp->pCompleteParam    = (void *)pSicdProtocol;

    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        LOG(1, "SICD Error: get device status failed, dwStatus: %d\n", dwStatus);
    }

    return dwStatus;
}

/*******************************************************************************
**  MUSB_SicdTransferCancelRequest
**
**  descriptions:
**      Transfer sicd class-specific request : cancel(abort) request.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferCancelRequest(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;
    uint32_t dwTransactionId;
    uint8_t *pbDataBuf;

    MUSB_ASSERT(pSicdProtocol);
    pSicdDevice = pSicdProtocol->pDevice->pDriverPrivateData;
    pControlIrp = &(pSicdDevice->ControlIrp);
    dwTransactionId = pSicdProtocol->SicdStateControl.SicdCommand.rCommandBlock.dwTranscationID;
    pSetup = (MUSB_DeviceRequest*)&(pSicdProtocol->SicdStateControl.SicdCommand.abGenericTransferBuffer[0]);
    pbDataBuf = &(pSicdProtocol->SicdStateControl.SicdCommand.abGenericTransferBuffer[8]);
    (void)MUSB_MemSet(pSetup, (uint8_t)0, MUSB_SICD_MAX_CMD_RESPONSE_LENGTH);
    
    LOG(5, "SICD: abort operation\n", NULL);

    /** Prepare the Setup Packet for sending Set Config Request */
    pSetup->bmRequestType = (uint8_t)  (MUSB_DIR_OUT | MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE);
    pSetup->bRequest      = (uint8_t)  MUSB_SICD_CANCEL_REQUEST_CODE;
    pSetup->wValue        = (uint16_t) 0;
    pSetup->wIndex        = (uint16_t) 0;
    pSetup->wLength       = (uint16_t) MUSB_SICD_CANCEL_REQUEST_DATA_LENGTH;

    *pbDataBuf = (uint8_t)(MUSB_SICD_CANELLATION_CODE & 0xFF);
    *(pbDataBuf + 1) = (uint8_t)((MUSB_SICD_CANELLATION_CODE >> 8));
    *(pbDataBuf + 2) = (uint8_t)(dwTransactionId);
    *(pbDataBuf + 3) = (uint8_t)(dwTransactionId >> 8);
    *(pbDataBuf + 4) = (uint8_t)(dwTransactionId >> 16);
    *(pbDataBuf + 5) = (uint8_t)(dwTransactionId >> 24);

    /** Fill Control Irp */
    pControlIrp->pDevice           = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer        = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength       = (uint32_t)(MUSB_SICD_CANCEL_REQUEST_LENGTH+MUSB_SICD_CANCEL_REQUEST_DATA_LENGTH);
    pControlIrp->pInBuffer         = NULL;
    pControlIrp->dwInLength        = 0;
    pControlIrp->dwStatus          = 0;
    pControlIrp->dwActualOutLength = 0;
    pControlIrp->dwActualInLength  = 0;
    pControlIrp->pfIrpComplete     = MUSB_SicdControlIrpComplete;
    pControlIrp->pCompleteParam    = (void *)pSicdProtocol;

    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        LOG(1, "SICD Error: get device status failed, dwStatus: %d\n", dwStatus);
    }

    return dwStatus;

}

/*******************************************************************************
**  MUSB_SicdTransferClearFeatureRequest
**
**  descriptions:
**      Transfer clear feature request.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferClearFeatureRequest(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;
    uint8_t bEndPointNumber;

    MUSB_ASSERT(pSicdProtocol);
    pSicdDevice = pSicdProtocol->pDevice->pDriverPrivateData;
    pSetup = &(pSicdDevice->Setup);
    pControlIrp = &(pSicdDevice->ControlIrp);
    bEndPointNumber = _MUSBSicd_FindCurrentPipeEndpointNumber(pSicdProtocol);
    
    LOG(5, "SICD: clear feature on device\n", NULL);

    /** Prepare the Setup Packet for sending Set Config Request */
    pSetup->bmRequestType = (uint8_t) (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_ENDPOINT);
    pSetup->bRequest      = (uint8_t) MUSB_REQ_CLEAR_FEATURE;
    pSetup->wValue        = (uint16_t) MUSB_FEATURE_ENDPOINT_HALT;
    pSetup->wIndex        = (uint16_t) bEndPointNumber;
    pSetup->wLength       = (uint16_t) 0;

    /** Fill Control Irp */
    pControlIrp->pDevice           = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer        = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength       = sizeof(MUSB_DeviceRequest);
    pControlIrp->pInBuffer         = NULL;
    pControlIrp->dwInLength        = 0;
    pControlIrp->dwStatus          = 0;
    pControlIrp->dwActualOutLength = 0;
    pControlIrp->dwActualInLength  = 0;
    pControlIrp->pfIrpComplete     = MUSB_SicdControlIrpComplete;
    pControlIrp->pCompleteParam    = (void *)pSicdProtocol;

    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        LOG(1, "SICD Error: Clear Feature Request failed, dwStatus: %d\n", dwStatus);
    }

    return dwStatus;
}

/*******************************************************************************
**  MUSB_SicdTransferResetDeviceRequest
**
**  descriptions:
**      Transfer sicd class specific request : reset device.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferResetDeviceRequest(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pSicdProtocol);
    pSicdDevice = pSicdProtocol->pDevice->pDriverPrivateData;
    pSetup = &(pSicdDevice->Setup);
    pControlIrp = &(pSicdDevice->ControlIrp);
    
    LOG(5, "SICD: reset device\n", NULL);

    /** Prepare the Setup Packet for sending Set Config Request */
    pSetup->bmRequestType = (uint8_t)  (MUSB_DIR_OUT | MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE);
    pSetup->bRequest      = (uint8_t)  MUSB_SICD_RESET_REQUEST_CODE;
    pSetup->wValue        = (uint16_t) 0;
    pSetup->wIndex        = (uint16_t) 0;
    pSetup->wLength       = (uint16_t) 0;

    /** Fill Control Irp */
    pControlIrp->pDevice           = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer        = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength       = sizeof(MUSB_DeviceRequest);
    pControlIrp->pInBuffer         = NULL;
    pControlIrp->dwInLength        = 0;
    pControlIrp->dwStatus          = 0;
    pControlIrp->dwActualOutLength = 0;
    pControlIrp->dwActualInLength  = 0;
    pControlIrp->pfIrpComplete     = MUSB_SicdControlIrpComplete;
    pControlIrp->pCompleteParam    = (void *)pSicdProtocol;

    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        LOG(1, "SICD Error: reset device failed, dwStatus: %d\n", dwStatus);
    }

    return dwStatus;
}

/*******************************************************************************
**  MUSB_SicdTransferResetDeviceRequest
**
**  descriptions:
**      Transfer sicd class specific request : get extended event data.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdTransferGetExtEventDataRequest(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;
    uint8_t *pbRecvBuffer;

    MUSB_ASSERT(pSicdProtocol);
    pSicdDevice = pSicdProtocol->pDevice->pDriverPrivateData;
    pSetup = &(pSicdDevice->Setup);
    pControlIrp = &(pSicdDevice->ControlIrp);
    pbRecvBuffer = pSicdProtocol->SicdStateControl.SicdCommand.abGenericTransferBuffer;
    
    LOG(5, "SICD: get ext event data\n", NULL);

    /** Prepare the Setup Packet for sending Set Config Request */
    pSetup->bmRequestType = (uint8_t)  (MUSB_DIR_OUT | MUSB_TYPE_CLASS | MUSB_RECIP_INTERFACE);
    pSetup->bRequest      = (uint8_t)  MUSB_SICD_GET_EXT_EVENT_REQUEST_CODE;
    pSetup->wValue        = (uint16_t) 0;
    pSetup->wIndex        = (uint16_t) 0;
    pSetup->wLength       = (uint16_t) 0;

    /** Fill Control Irp */
    pControlIrp->pDevice           = pSicdDevice->pUsbDevice;
    pControlIrp->pOutBuffer        = (const uint8_t*)pSetup;
    pControlIrp->dwOutLength       = sizeof(MUSB_DeviceRequest);
    pControlIrp->pInBuffer         = pbRecvBuffer;
    pControlIrp->dwInLength        = MUSB_SICD_MAX_CMD_RESPONSE_LENGTH;
    pControlIrp->dwStatus          = 0;
    pControlIrp->dwActualOutLength = 0;
    pControlIrp->dwActualInLength  = 0;
    pControlIrp->pfIrpComplete     = MUSB_SicdControlIrpComplete;
    pControlIrp->pCompleteParam    = (void *)pSicdProtocol;

    dwStatus = MUSB_StartControlTransfer(pSicdDevice->pUsbDevice->pPort, pControlIrp);

    if (dwStatus)
    {
        LOG(1, "SICD Error: get ext event data failed, dwStatus: %d\n", dwStatus);
    }

    return dwStatus;
}

#endif  /* #ifdef MUSB_SICD */

