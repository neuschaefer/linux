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
 * This file contains the state machine of SICD.
 * $Revision: #1 $
 */
#ifdef MUSB_SICD

#include "mu_diag.h"
#include "mu_cdi.h"
#include "mu_mem.h"

#include "mu_hfi_sicd_if.h"
#include "mu_sicdstate.h"


/*************************** CONSTANTS ****************************/

/*************************** FORWARDS *****************************/
static void MGC_SicdIrpComplete(void* pParam, const void *pGeneralIrp);
static void MGC_SicdNotifyResetSuccessCbk( MUSB_SicdProtocol* pSicdProtocol);

/**************************** GLOBALS *****************************/
static MUSB_SicdStateControl MGC_SicdStateControlDataBackup;

/*************************** Internal FUNCTIONS ****************************/

/*******************************************************************************
**  MGC_ShiftBufferForSicdHeader
**
**  descriptions:
**      Remove the first 12 bytes SICD header in pIrp->pBuffer & shift the 
**      whole packet content.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_ShiftBufferForSicdHeader(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_Irp *pIrp;
    uint32_t dwPayLoadBufLen;
    uint8_t *pPayLoadBuf;
    MUSB_SicdStateControl *pSicdStateControl;
    uint32_t i;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    pIrp = &(pSicdStateControl->BulkIrp);

    MUSB_ASSERT(pIrp->dwActualLength >= MUSB_SICD_HEADER_MINIMUM_LENGTH);
    
    dwPayLoadBufLen = (pIrp->dwActualLength - MUSB_SICD_HEADER_MINIMUM_LENGTH); 
    pPayLoadBuf = pIrp->pBuffer;
    for ( i = 0 ; i < dwPayLoadBufLen ; i++ )
    {
        pPayLoadBuf[i] = pPayLoadBuf[i+MUSB_SICD_HEADER_MINIMUM_LENGTH];
    }

    // Error handling. In normal case, the if condition should not be TRUE !
    if ( pIrp->dwLength < dwPayLoadBufLen )
    {
         LOG(1, "Harmless Memory Overflow.", NULL);
    }
}

/*******************************************************************************
**  MGC_GoToReadyState
**
**  descriptions:
**      Set the state machine to ready state.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MGC_GoToReadyState(MUSB_SicdProtocol* pSicdProtocol)
{   
    MUSB_SicdStateControl *pSicdStateControl;
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    pSicdStateControl->bTotalStateCount = 0;
    pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}

/*******************************************************************************
**  MGC_SilentReset
**
**  descriptions:
**      Do silent bus reset & notify upper layer when success.
**      If reset failed, we will not notify upper layer.
**      Note! Silent bus reset is always triggered by SICD, not by middleware.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SilentReset(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_ASSERT(pSicdProtocol);
    
    if ( MUSB_SilentReset(pSicdProtocol->pDevice) == 0 )
    {
        // notify upper layer.
        MGC_SicdNotifyResetSuccessCbk(pSicdProtocol);
    }
    
    // goto ready state.
    (void)MGC_GoToReadyState(pSicdProtocol);
}

/*******************************************************************************
**  MGC_BackupStateControlData
**
**  descriptions:
**      Backup the sicd state control data. Since some device does not support
**      SICD cancel request, if we found cancel request failed, we should 
**      continue the original transfer. This function is used to backup the 
**      sicd state data.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_BackupStateControlData(const MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_ASSERT(pSicdProtocol);
    (void)MUSB_MemCopy(&MGC_SicdStateControlDataBackup, 
            &(pSicdProtocol->SicdStateControl), sizeof(MUSB_SicdStateControl));
}

/*******************************************************************************
**  MGC_RestoreStateControlData
**
**  descriptions:
**      Restore sicd state control data. This is used when we found that the device
**      does not support cancel(abort) request. We restore the state data and 
**      then continue to finish the states.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_RestoreStateControlData(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_ASSERT(pSicdProtocol);
    
    (void)MUSB_MemCopy(&(pSicdProtocol->SicdStateControl), 
        &MGC_SicdStateControlDataBackup, sizeof(MUSB_SicdStateControl));
}

/*******************************************************************************
**  MGC_HandleTimeOutStatus
**
**  descriptions:
**      This function is used to handle RX timeout. We will do auto retry for the 
**      current state. If it still failed doing auto retry, we will then silent 
**      reset the usb bus.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MGC_HandleTimeOutStatus(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdResult Result;
    MUSB_SicdStateControl *pSicdStateControl;
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    if (pSicdStateControl->dwRetryCount >= MUSB_SICD_MAX_OPERATION_RETRY_COUNT )
    {
       LOG(1, "SICD: RX timeout retry cmd fail!\n", NULL);
       return (MUSB_SicdResult)MUSB_E_SICD_CMD_RETRY_FAIL;
    }

    pSicdStateControl->dwRetryCount ++;
    LOG(1, "SICD: Retry cmd = %d\n", pSicdStateControl->dwRetryCount);   
   
   Result = MUSB_SicdHandleOperation(pSicdProtocol);
   return Result;
}

/*******************************************************************************
**  MGC_AnalysisDeviceStatusData
**
**  descriptions:
**      This is just a utility function to check the result of Get_DEV_STATUS request. 
**      If it's status ok, we will continue the state machine. If it's status busy, 
**      we will retry the get device status request. If it's transaction cancelled, 
**      doesn't know what to do, MUSB_ASSERT it.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MGC_AnalysisDeviceStatusData(const MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    MUSB_SicdDevice *pSicdDevice;
    MUSB_ControlIrp *pControlIrp;
    uint16_t wDataLength;
    uint16_t wDeviceStatusCode;
    uint8_t *pbTransferBuf = NULL;

    MUSB_ASSERT(pSicdProtocol);
    pSicdDevice = pSicdProtocol->pDevice->pDriverPrivateData;
    pControlIrp = &(pSicdDevice->ControlIrp);
   
    pbTransferBuf = pControlIrp->pInBuffer;
    wDataLength = MUSB_SWAP16P(&pbTransferBuf[0]);
    wDeviceStatusCode = MUSB_SWAP16P(&pbTransferBuf[2]);;
    UNUSED(wDataLength);
   
    switch(wDeviceStatusCode)
    {
    case MUSB_SICD_DEVICE_STATUS_OK:
       Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
       break;
       
    case MUSB_SICD_DEVICE_STATUS_BUSY:
       Result = (MUSB_SicdResult)MUSB_E_SICD_DEVICE_BUSY;
       break;
       
    case MUSB_SICD_DEVICE_TRANSACTION_CANCELLED:
       Result = (MUSB_SicdResult)MUSB_E_SICD_DEVICE_TRANSACTION_CANCELLED;
       //here we will add a operation for evaluating the value to  pu4SpecialParam
       break;
       
    default: 
        LOG(1, "AnalysisDeviceStatusData: %d\n", wDeviceStatusCode);
        Result = (MUSB_SicdResult)MUSB_E_SICD_DEVICE_BUSY;
        break;
    }
    return Result; //do not care status of device;
}

/*******************************************************************************
**  MGC_SicdNotifyDataCbk
**
**  descriptions:
**      Callback to notify upper layer the receive data has filled out the buffer, 
**      or all the receiving data has been put into the buffer. It should be 
**      noticed that if it is the first packet of data phase, we should remove the
**      first 12 byte sicd container header. 
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyDataCbk(MUSB_SicdProtocol* pSicdProtocol, int32_t ResultNum)
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */ 
    MUSB_SicdStateControl *pSicdStateControl;
    
    MUSB_ASSERT(pSicdProtocol);

    LOG(7, "MGC_SicdNotifyDataCbk\n", NULL);
    
    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            ResultNum, &(pSicdStateControl->SicdCommand.rDataBlock));
    }

    return;
}

/*******************************************************************************
**  MGC_SicdNotifyPhaseChangeCbk
**
**  descriptions:
**      This function is used to notify upper layer the phase has been changed.
**      After command phase complete, we will notify MUSB_HFI_NFY_PHASE_CMD_COMPLETE.
**      After data phase complete, we will notify MUSB_HFI_NFY_PHASE_DATA_COMPLETE.
**      After response phase complete, we will notify MUSB_HFI_NFY_PHASE_RESP_COMPLETE.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyPhaseChangeCbk(MUSB_SicdProtocol* pSicdProtocol, 
                                                MUSB_HFI_SICD_PHASE_E PhaseNum)
{
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SICD_BLOCK_T* pSicdBlock;
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */     
    MUSB_HFI_NFY_TYPE_E NfyType = MUSB_HFI_NFY_OPERATION_ERROR;
    MUSB_Irp *pIrp;
    uint32_t dwPayLoadBufLen;
    uint32_t i;
    uint8_t *pPayLoadBuf;

    LOG(7, "MGC_SicdNotifyPhaseChangeCbk\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    switch ( PhaseNum )
    {
    case MUSB_SICD_PHASE_COMMAND:
        pSicdBlock = &(pSicdStateControl->SicdCommand.rCommandBlock);
        NfyType = MUSB_HFI_NFY_PHASE_CMD_COMPLETE;
        break;
        
    case MUSB_SICD_PHASE_DATA:
        pSicdBlock = &(pSicdStateControl->SicdCommand.rDataBlock);
        NfyType = MUSB_HFI_NFY_PHASE_DATA_COMPLETE;

        break;
        
    case MUSB_SICD_PHASE_RESPONSE:
        pSicdBlock = &(pSicdStateControl->SicdCommand.rResponseBlock);
        NfyType = MUSB_HFI_NFY_PHASE_RESP_COMPLETE;

        // update response block content
        pIrp = &(pSicdStateControl->BulkIrp);

        pSicdBlock->dwPayLoadBufLen = MUSB_SWAP32P(&pIrp->pBuffer[0])-MUSB_SICD_HEADER_MINIMUM_LENGTH;
        pSicdBlock->wOperationCode = MUSB_SWAP16P(&pIrp->pBuffer[6]);

        dwPayLoadBufLen = pSicdBlock->dwPayLoadBufLen; // this is the real payload data

        // eliminate the first 12 bytes SICD header.
        pSicdBlock->pbPayLoadBuf = pIrp->pBuffer;
        pPayLoadBuf = pSicdBlock->pbPayLoadBuf;
        for ( i = 0 ; i < dwPayLoadBufLen ; i++ )
        {
            pPayLoadBuf[i] = pPayLoadBuf[i+MUSB_SICD_HEADER_MINIMUM_LENGTH];
        }
        break;
        
    default: 
        // Error Condition.
        LOG(1, "PhaseChangeCbk: %d.\n", PhaseNum);
        pSicdBlock = &(pSicdStateControl->SicdCommand.rCommandBlock);
        break;
    }
       
    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            (int32_t)NfyType, pSicdBlock);
    }
    
    return;
}

/*******************************************************************************
**  MGC_SicdNotifyOperationErrCbk
**
**  descriptions:
**      When the state machine can not be finished, we use this function to 
**      notify upper layer.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyOperationErrCbk(MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */     

    MUSB_ASSERT(pSicdProtocol);

    LOG(1, "MGC_SicdNotifyOperationErrCbk\n", NULL);

    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            (int32_t)MUSB_HFI_NFY_OPERATION_ERROR, NULL);
    }
    
    return;
}

/*******************************************************************************
**  MGC_SicdNotifyResetCbk
**
**  descriptions:
**      To notify upper layer the result of sicd reset request.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyResetCbk(MUSB_SicdProtocol* pSicdProtocol, 
    BOOL bSuccess)
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */
    int32_t dwStatus;

    LOG(5, "MGC_SicdNotifyResetCbk\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);

    dwStatus = ((bSuccess) ? 
        ((int32_t)MUSB_HFI_NFY_RESET_SUCCESS) : ((int32_t)MUSB_HFI_NFY_RESET_FAIL));
        
    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            dwStatus, NULL);
    }
    
    return;
}

/*******************************************************************************
**  MGC_SicdNotifyOperationAbortCbk
**
**  descriptions:
**      To notify upper layer the result of sicd cancel(abort) request.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyOperationAbortCbk(MUSB_SicdProtocol* pSicdProtocol, 
    BOOL bSuccess)
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */
    int32_t dwStatus;

    LOG(5, "MGC_SicdNotifyOperationAbortCbk\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);

    dwStatus = ((bSuccess) ? ((int32_t)MUSB_HFI_NFY_OPERATION_ABORT_SUCCESS) : 
        ((int32_t)MUSB_HFI_NFY_OPERATION_ABORT_FAIL));

    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            dwStatus, NULL);
    }
    
    return;
}

/*******************************************************************************
**  MGC_SicdNotifyGetExtEventDataCbk
**
**  descriptions:
**      To notify upper layer the result of sicd get extended event data request.
**      It has not bee tested! (Currently, all the device we have does not support
**      sicd event.)
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyGetExtEventDataCbk(MUSB_SicdProtocol* pSicdProtocol) 
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */
    MUSB_SICD_EXT_EVENT_BLOCK_T *pExeEventBlock;
    MUSB_Irp *pIrp;
    uint32_t dwPayLoadBufLen;
    uint8_t *pPayLoadBuf;
    uint32_t i;

    LOG(5, "MGC_SicdNotifyGetExtEventDataCbk\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);
    pExeEventBlock = &(pSicdProtocol->SicdStateControl.SicdCommand.rExtEventBlock);
    pIrp = &(pSicdProtocol->SicdStateControl.BulkIrp);
    
    pExeEventBlock->wEventCode = MUSB_SWAP16P(&pIrp->pBuffer[0]);
    pExeEventBlock->dwTransactionId = MUSB_SWAP32P(&pIrp->pBuffer[2]);
    pExeEventBlock->wNumOfParameters = MUSB_SWAP16P(&pIrp->pBuffer[6]);
    pExeEventBlock->pbPayloadBuf = pIrp->pBuffer;

    // update payload buffer
    dwPayLoadBufLen = pIrp->dwActualLength - 8; // this is the real payload data
    pPayLoadBuf = pIrp->pBuffer;
    for ( i = 0 ; i < dwPayLoadBufLen ; i++ )
    {
        pPayLoadBuf[i] = pPayLoadBuf[i+8];
    }
    (void)MUSB_MemSet(&pPayLoadBuf[i], (UINT8)0, pIrp->dwLength - pIrp->dwActualLength);

    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            (int32_t)MUSB_HFI_NFY_GET_EXT_EVENT_COMPLETE, pExeEventBlock);
    }

    return;
}

/*******************************************************************************
**  MGC_SicdNotifyEventCbk
**
**  descriptions:
**      When device sends a event to host, we use this function to notify upper
**      layer.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
#if MGC_SICD_OPEN_INTERRUPT_PIPE
static void MGC_SicdNotifyEventCbk(
                                                MUSB_SicdProtocol* pSicdProtocol) 
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */
    MUSB_SICD_BLOCK_T *pEventBlock;
    MUSB_Irp *pIrp;
    uint32_t dwPayLoadBufLen;
    uint8_t *pPayLoadBuf;
    uint32_t i;

    LOG(5, "MGC_SicdNotifyEventCbk\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);
    pEventBlock = &(pSicdProtocol->SicdStateControl.SicdCommand.rEventBlock);
    pIrp = &(pSicdProtocol->SicdStateControl.IntrIrp);
    dwPayLoadBufLen = MUSB_SWAP32P(&pIrp->pBuffer[0]);
    MUSB_ASSERT(dwPayLoadBufLen >= MUSB_SICD_HEADER_MINIMUM_LENGTH);
    dwPayLoadBufLen -= MUSB_SICD_HEADER_MINIMUM_LENGTH;
    
    pEventBlock->eDataDirection = MUSB_SICD_DATA_IN;
    pEventBlock->dwPayLoadBufLen = dwPayLoadBufLen;
    pEventBlock->eDataType = 
        (MUSB_SICD_DATA_TYPE_E)MUSB_SWAP16P(&pIrp->pBuffer[4]);
    pEventBlock->wOperationCode =  MUSB_SWAP16P(&pIrp->pBuffer[6]);;
    pEventBlock->dwTranscationID = MUSB_SWAP32P(&pIrp->pBuffer[8]);
    pEventBlock->pbPayLoadBuf = pIrp->pBuffer;

    // update payload buffer
    pPayLoadBuf = pIrp->pBuffer;
    for ( i = 0 ; i < dwPayLoadBufLen ; i++ )
    {
        pPayLoadBuf[i] = pPayLoadBuf[i+MUSB_SICD_HEADER_MINIMUM_LENGTH];
    }
    (void)MUSB_MemSet(&pPayLoadBuf[i], (UINT8)0, (pIrp->dwLength - pIrp->dwActualLength));

    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            (int32_t)MUSB_HFI_NFY_GOT_EVENT, pEventBlock);
    }
                              
    return;
}
#endif

/*******************************************************************************
**  MGC_SicdNotifyResetSuccessCbk
**
**  descriptions:
**      Notify upper when silent bus reset success.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdNotifyResetSuccessCbk(MUSB_SicdProtocol* pSicdProtocol) 
{
    MUSB_NFY_INFO_T *pSicdNfyInfo;   /* user's notify function. */

    LOG(5, "MGC_SicdNotifyResetSuccessCbk\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);

    pSicdNfyInfo = &pSicdProtocol->SicdNfyInfo;

    if (pSicdNfyInfo->pfHfiNfy)
    {
        pSicdNfyInfo->pfHfiNfy(pSicdNfyInfo->pvHfiNfyTag, 
            (int32_t)MUSB_HFI_NFY_SILENT_BUS_RESET, NULL);
    }
    
    return;
}

/*******************************************************************************
**  MGC_SicdCompleteStandardOperation
**
**  descriptions:
**      Check the returned IRP to decide what the state machine should do for 
**      the next step.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MGC_SicdCompleteStandardOperation(
                                                MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;
    MUSB_ControlIrp *pControlIrp;

    LOG(7, "MGC_SicdCompleteStandardOperation\n", NULL);

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }    
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];  

    switch(pStateFlow->eOpeartionCode)
    {
    case MUSB_SICD_GET_DEVICE_STATUS:
        // If STALL or NACK, it may be caused by operation not support. 
        // Case 1: Operation Abort OK, Get DevStatus Failed : Do nothing, pretend all OK.
        // Case 2: Operation Abort Failed, Get DevStatus OK : Will not enter here.
        // Case 3: Reset OK, Get DevStatus Failed : Do nothing, pretend All OK.
        // Case 4: Reset Failed, Get DevStatus Failed : Do nothing. 
        // 
        // The purpose of Get Device Status is just to keep retry when device is busy.
        // If the device does not support the request, just skip the result of this operation.
        pControlIrp = &(((MUSB_SicdDevice*)pSicdProtocol->pDevice->pDriverPrivateData)->ControlIrp);
        if ( (pControlIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED) || 
             (pControlIrp->dwStatus == (uint32_t)MUSB_STATUS_NACK_LIMIT) )
        {
            LOG(1, "The device does not support get status request!\n", NULL);
            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }
        else
        {
           LOG(5, "The device supports get status request!\n", NULL);
           Result = MGC_AnalysisDeviceStatusData(pSicdProtocol);
        }
        break;

    case MUSB_SICD_ABORT_REQUEST:
        // If STALL or NACK, it may be caused by operation not support. 
        // Should restore data block here. (to continue to finish the data phase)
        // Important Note 2008/03/13 : 
        // There are four cases in devices which does not support Abort : 
        // * 1. We should wait 30s, then continue next operation is OK. 
        // * 2. We should continue to finish data phase, then OK.
        // * 3. We should reset bus, then re-open session.
        // * 4. We should re-plugin the device !!
        // We only implemented the case 2 solution !! 
        // Maybe we should give up give file playback on this kind of devices.
        pControlIrp = &(((MUSB_SicdDevice*)pSicdProtocol->pDevice->pDriverPrivateData)->ControlIrp);
        if ( (pControlIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED) || 
             (pControlIrp->dwStatus == (uint32_t)MUSB_STATUS_NACK_LIMIT) )
        {
            LOG(1, "The device does not support abort request!\n", NULL);
            
            // use retry count exceeding to notify upper layer request error.
            pSicdStateControl->dwRetryCount = MUSB_SICD_MAX_OPERATION_RETRY_COUNT;
            
            MGC_SicdNotifyOperationAbortCbk(pSicdProtocol, (BOOL)FALSE);

            MGC_RestoreStateControlData(pSicdProtocol);
            Result = (MUSB_SicdResult)MUSB_SICD_DATA_PHASE_CONTINUE;
        }
        else
        {
            LOG(5, "The device supports abort request!\n", NULL);
            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }
        break;
        
    case MUSB_SICD_RESET_REQUEST:
        // if STALL or NACK, it may be caused by operation not support. 
        // Do nothing special here, we will notify upper layer later.
        pControlIrp = &(((MUSB_SicdDevice*)pSicdProtocol->pDevice->pDriverPrivateData)->ControlIrp);
        if ( (pControlIrp->dwStatus == (uint32_t)MUSB_STATUS_STALLED) || 
             (pControlIrp->dwStatus == (uint32_t)MUSB_STATUS_NACK_LIMIT) )
        {
            LOG(1, "The device does not support reset request!\n", NULL);
            
            // Use retry count exceeding to notify upper layer request error.
            pSicdStateControl->dwRetryCount = MUSB_SICD_MAX_OPERATION_RETRY_COUNT;

            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }
        else
        {
            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }

        break;
    
    case MUSB_SICD_CLEAR_FEATURE_REQUEST:
    case MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST:  // not tested
       Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
       break;

    case MUSB_SICD_TRANSFER_OPERATION:

    case MUSB_SICD_DATAPHASE_CTRL_BY_MW:
    case MUSB_SICD_DATAPHASE_CTRL_BY_SICD:
    case MUSB_SICD_GET_NEXT_DATA:
    case MUSB_SICD_SET_SILENT_BUS_RESET:
    case MUSB_SICD_RESET_OP_EVT:
    case MUSB_SICD_ABORT_OP_EVT:
    case MUSB_SICD_GET_EXT_EVENT_OP_EVT:
    default:
        LOG(1, "CompleteStandardOperation %d\n", pStateFlow->eOpeartionCode);
        break;
    }
    
    switch(Result)
    {
    case MUSB_SICD_SUCCESS:
       pSicdStateControl->bCurStateIndex++;
       pSicdStateControl->dwRetryCount = 0; 
       break;

    case MUSB_SICD_DATA_PHASE_CONTINUE:
        Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        break;

    case MUSB_E_SICD_DEVICE_BUSY:
        LOG(1, "SICD: Dev Busy!\n", NULL);
        if (pSicdStateControl->dwRetryCount >= MUSB_SICD_MAX_OPERATION_RETRY_COUNT)
        {
            LOG(1, "Exceed Retry Count!\n", NULL);
            pSicdStateControl->bCurStateIndex++; //retry FAIL
            pSicdStateControl->dwRetryCount = 0;            
        }
        else
        {
            LOG(1, "Retry! \n", NULL);
            pSicdStateControl->dwRetryCount++;
        }
       break;
       
    case MUSB_E_SICD_DEVICE_TRANSACTION_CANCELLED:  // triggered by device activate cancellation
        LOG(1, "SICD: Transaction cancelled!\n", NULL);
        //here we will add an operation for creating a new state flow
        break;
       
    default:
        //serious error unrecoveralbe
        LOG(1, "MGC_SicdCompleteStandardOperation default case error2\n", NULL);
        break;
    }
    return Result;
}

/*******************************************************************************
**  MGC_SicdCompleteStandardOperation
**
**  descriptions:
**      Check the returned IRP to decide what the state machine should do for 
**      the next step. Here we maintain data phase by using MUSB_SICD_DATA_PHASE_CONTINUE. 
**      The data phase can also be maintained by using circular buffer. 
**      It maybe some performance improvement. However, the source code will be 
**      more complicated. 
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MGC_SicdCompleteTransferOperation(
                                            MUSB_SicdProtocol* pSicdProtocol)
{
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;    
    MUSB_Irp *pIrp;
    uint8_t bIsFirstDataPacketOfDataPhase = FALSE;
    uint8_t bDataOutComplete = TRUE;   // check whether should notify IO complete or not.
    uint8_t bShouldContinueDataPhaseSilently = FALSE;
    MUSB_SICD_DATA_DIRECTION_E eDataDirection;
    uint32_t i;

    LOG(7, "MGC_SicdCompleteTransferOperation\n", NULL);

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }    
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];    
    pIrp = &(pSicdStateControl->BulkIrp);

    switch(pStateFlow->eOpeartionType)
    {
    case MUSB_SICD_CMD_OPERATION:
        if ( pIrp->dwActualLength >= MUSB_SICD_HEADER_MINIMUM_LENGTH )
        {
            MGC_SicdNotifyPhaseChangeCbk(pSicdProtocol, MUSB_SICD_PHASE_COMMAND);
            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }
        else    // error handling
        {
            LOG(1, "The command phase data size we sent is not greater than 12 bytes\n", NULL);
            MUSB_ASSERT(0);
        }
        break;
       
    case MUSB_SICD_DATA_OPERATION:
        // Note !! The data out direction is not completed yet.        
        eDataDirection = pSicdStateControl->SicdCommand.rDataBlock.eDataDirection;

        if ( eDataDirection == MUSB_SICD_DATA_IN )
        {
            // If this is the first complete callback of Data Phase In, get total size.
            if ((pSicdStateControl->dwDataPhaseTotalDataSize == (uint32_t)MUSB_SICD_DATA_DEFAULT_SIZE) &&
                (pIrp->dwActualLength < MUSB_SICD_HEADER_MINIMUM_LENGTH))
            {
                LOG(1, "1st short packet dwActualLength = %d.\n", pIrp->dwActualLength);
               
                MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
                pSicdStateControl->bCurStateIndex = pSicdStateControl->bTotalStateCount;
                return (MUSB_SicdResult)MUSB_SICD_SUCCESS;                
            }

            // Error Handling.
            // If this is the first complete callback of Data Phase In
            // and the returned data block is actually a response block, 
            // jump to response phase.
            if ( (pSicdStateControl->dwDataPhaseTotalDataSize == (uint32_t)MUSB_SICD_DATA_DEFAULT_SIZE) && 
                  (pIrp->dwActualLength >= MUSB_SICD_HEADER_MINIMUM_LENGTH) && 
                  (MUSB_SICD_RESPONSE_BLOCK == 
                    (MUSB_SICD_DATA_TYPE_E)MUSB_SWAP16P(&pIrp->pBuffer[4])) )
            {
                // skip data phase
                pSicdStateControl->bCurStateIndex++;

                // notify phase change to response phase
                MGC_SicdNotifyPhaseChangeCbk(pSicdProtocol, MUSB_SICD_PHASE_RESPONSE);
                Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
                break;
            }
        
            // If this is the first complete callback of Data Phase In, get total size.
            if ( (pSicdStateControl->dwDataPhaseTotalDataSize == (uint32_t)MUSB_SICD_DATA_DEFAULT_SIZE) && 
                  (pIrp->dwActualLength >= MUSB_SICD_HEADER_MINIMUM_LENGTH) && 
                 (MUSB_SICD_DATA_BLOCK == 
                    (MUSB_SICD_DATA_TYPE_E)MUSB_SWAP16P(&pIrp->pBuffer[4])) )
            {
                pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SWAP32P(&pIrp->pBuffer[0]);
                pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
                bIsFirstDataPacketOfDataPhase = TRUE;

                // Error handling for Creative Zen VPLUS.
                // In data phase, 12 bytes SICD header is in a single short packet.
                if ( (pSicdStateControl->dwDataPhaseTotalDataSize != MUSB_SICD_HEADER_MINIMUM_LENGTH) && 
                     (pIrp->dwActualLength == MUSB_SICD_HEADER_MINIMUM_LENGTH) )
                {
                    Result = (MUSB_SicdResult)MUSB_SICD_DATA_PHASE_CONTINUE;  
                    pSicdStateControl->dwDataPhaseCurrentReadOffset += MUSB_SICD_HEADER_MINIMUM_LENGTH;
                    break;
                }                
            }

            pSicdStateControl->dwDataPhaseCurrentReadOffset += pIrp->dwActualLength;

            // Compare client buffer size & actual packet size.
            if ( pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen > MUSB_SICD_HEADER_MINIMUM_LENGTH )
            {
                // If the actual packet length is bigger than buffer size, MUSB_ASSERT !!
                if ( pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen < 
                                                                pIrp->dwActualLength )
                {
                    LOG(1, "Payload buffer length is smaller than actual packet length\n", NULL);
                    LOG(1, "%d\n", pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen);
                    LOG(1, "%d\n", pIrp->dwActualLength);
                    MUSB_ASSERT( 0 );
                }

            }
            // Compare SICD internal generic buffer size & actual packet size.
            else 
            {
                // If the actual packet length is bigger than buffer size, MUSB_ASSERT !!
                if ( MUSB_SICD_MAX_CMD_RESPONSE_LENGTH < pIrp->dwActualLength )
                {
                    LOG(1, "Payload buffer length is smaller than actual packet length\n", NULL);
                    LOG(1, "%d\n", MUSB_SICD_MAX_CMD_RESPONSE_LENGTH);
                    LOG(1, "%d\n", pIrp->dwActualLength);
                    MUSB_ASSERT(  0 );
                }
            }
        }
        else 
        {
            // if this is the first data phase callback, minus length value with 12.(SICD header size)
            if ( pSicdStateControl->dwDataPhaseCurrentWriteOffset == 0 )
            {
                pIrp->dwActualLength-=MUSB_SICD_HEADER_MINIMUM_LENGTH;
            }
            pSicdStateControl->dwDataPhaseCurrentWriteOffset += pIrp->dwActualLength;

            // TRUE means we have not finish the first SICDDEV_SET_TYPE_SEND_NEXT_DATA Request.
            // Set result to MUSB_SICD_DATA_PHASE_CONTINUE to avoid notifying upper layer.
            if ( pSicdStateControl->dwDataPhaseCurrentWriteOffset < 
                        pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen )
            {
                bDataOutComplete = FALSE;
                bShouldContinueDataPhaseSilently = TRUE;
            }
        }

        // Error Handling. (Real size is smaller than that we expected.)
        // There are still some data we should read in, but the current packet is response packet.
        // Jump to response phase complete.
        // Since some device does not support abort operation, and we will try to continue reading.
        // In this case, some device will not send out all the content and will cause data length 
        // calculation error in sicd driver. Here is just error handling.
        if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
             (pSicdStateControl->dwDataPhaseCurrentReadOffset < pSicdStateControl->dwDataPhaseTotalDataSize) && 
             (pIrp->dwActualLength == MUSB_SICD_HEADER_MINIMUM_LENGTH) &&
             (MUSB_SWAP32P(&pIrp->pBuffer[0]) == MUSB_SICD_HEADER_MINIMUM_LENGTH) &&
             ((MUSB_SICD_DATA_TYPE_E)MUSB_SWAP16P(&pIrp->pBuffer[4]) == MUSB_SICD_RESPONSE_BLOCK) )
        {
            LOG(1, "Length calculation error! Jump to ready state\n", NULL);
            MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
            pSicdStateControl->bCurStateIndex++;
            pSicdStateControl->bCurStateIndex++;
            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
            break;
        }

        // Error Handling. (Real size is bigger than that we expected.)
        // Some device will return wrong data phase SICD header. (iRiver Clix)
        // For example, we want to read 1 byte, the device return total length in SICD header with 10k byte, 
        // and actual data is 512 + 13 byte. 
        // Solution : once we encounter a short packet, we should finish data phase.
        if ( pIrp->dwActualLength < pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen )
        {
            if ( (pSicdStateControl->dwDataPhaseCurrentReadOffset < pSicdStateControl->dwDataPhaseTotalDataSize) &&
                 (!bIsFirstDataPacketOfDataPhase) )
                 
            {
                pSicdStateControl->dwDataPhaseTotalDataSize = pSicdStateControl->dwDataPhaseCurrentReadOffset;
                LOG(1, "Device's Bug: Encounter short packet but data phase not finished!!\n", NULL);
            }
        }

        // Notify IO complete
        if (pIrp->dwStatus != MUSB_STATUS_OK )
        {
            // data error
            MGC_SicdNotifyDataCbk(pSicdProtocol, MUSB_HFI_COND_FAIL);
        }
        // we have more data to receive.
        else if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
                  (pSicdStateControl->dwDataPhaseCurrentReadOffset < pSicdStateControl->dwDataPhaseTotalDataSize) )
        {
            if ( bIsFirstDataPacketOfDataPhase ) 
            {
                // Here we update buffer size/pointer.
                // Then continue to receive and fill out the payload buffer 
                // without callback upper layer.
                // Note!! we use 512 for both HS & FS
                MGC_ShiftBufferForSicdHeader(pSicdProtocol);
                
                // Update buffer pointer
                pSicdStateControl->SicdCommand.rDataBlock.pbPayLoadBuf = 
                            pSicdStateControl->SicdCommand.pbBackupPtr+500;

                // Update buffer size
                // only one callback
                if ( pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen >= 
                        (pSicdStateControl->dwDataPhaseTotalDataSize - MUSB_SICD_HEADER_MINIMUM_LENGTH) )
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                            pSicdStateControl->SicdCommand.dwBackupSize-500;
                }
                else
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                            pSicdStateControl->SicdCommand.dwBackupSize-512;
                }
                pSicdStateControl->SicdCommand.bContinueDataPhaseWithoutCbk = TRUE;
            }
            else
            {
                // Recover buffer size & pointer.
                pSicdStateControl->SicdCommand.rDataBlock.pbPayLoadBuf = 
                                pSicdStateControl->SicdCommand.pbBackupPtr;

                if ( pSicdStateControl->SicdCommand.bContinueDataPhaseWithoutCbk )
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                        pSicdStateControl->SicdCommand.dwBackupSize - MUSB_SICD_HEADER_MINIMUM_LENGTH;
                }
                else
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                                pSicdStateControl->SicdCommand.dwBackupSize;
                }
                
                pSicdStateControl->SicdCommand.bContinueDataPhaseWithoutCbk = FALSE;
                
                MGC_SicdNotifyDataCbk(pSicdProtocol, MUSB_HFI_COND_READ_OK);

                // Since we have more data to receive, recover buffer size here.
                pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                                pSicdStateControl->SicdCommand.dwBackupSize;
            }
        }
        // Got the last data phase packet.
        else if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
                  (pSicdStateControl->dwDataPhaseCurrentReadOffset == pSicdStateControl->dwDataPhaseTotalDataSize) )
        {
            if ( pSicdStateControl->SicdCommand.bContinueDataPhaseWithoutCbk )
            {
                pSicdStateControl->SicdCommand.rDataBlock.pbPayLoadBuf = 
                                pSicdStateControl->SicdCommand.pbBackupPtr;
                                
                // only one callback
                if ( (pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen+500) >= 
                        (pSicdStateControl->dwDataPhaseTotalDataSize - MUSB_SICD_HEADER_MINIMUM_LENGTH) )
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                                                    pIrp->dwActualLength+500;
                }
                else
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                                                    pIrp->dwActualLength+512;
                }
            }
            else
            {
                pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = pIrp->dwActualLength;
                if ( pIrp->dwActualLength == pSicdStateControl->dwDataPhaseTotalDataSize )
                {
                    MGC_ShiftBufferForSicdHeader(pSicdProtocol);
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen -= MUSB_SICD_HEADER_MINIMUM_LENGTH;
                }
            }
            // Don't notify data ready, middleware will get this packet in data phase complete callback.
        }
        // Got more data than we expected.
        else if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
                  (pSicdStateControl->dwDataPhaseCurrentReadOffset > pSicdStateControl->dwDataPhaseTotalDataSize) )
        {
            // Note! Entering here means we receive more data than we expected.
            // This does not mean buffer overflow occurs.
            // We just write the extra data into client buffer withou overflow.
            LOG(1, "Data phase = %d, %d.\n", 
                pSicdStateControl->dwDataPhaseCurrentReadOffset, 
                pSicdStateControl->dwDataPhaseTotalDataSize);

            if ( bIsFirstDataPacketOfDataPhase ) 
            {
                MGC_ShiftBufferForSicdHeader(pSicdProtocol);
            }

            if ( pSicdStateControl->SicdCommand.bContinueDataPhaseWithoutCbk )
            {
                pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                        pSicdStateControl->SicdCommand.dwBackupSize - MUSB_SICD_HEADER_MINIMUM_LENGTH;
            }
            else
            {
                // if first packet, we callback upper layer with pSicdStateControl->dwDataPhaseTotalDataSize
                if ( bIsFirstDataPacketOfDataPhase )
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = pSicdStateControl->dwDataPhaseTotalDataSize - MUSB_SICD_HEADER_MINIMUM_LENGTH;
                }
                // if not first packet, we callback upper layer with 
                else
                {
                    pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = pIrp->dwActualLength - MUSB_SICD_HEADER_MINIMUM_LENGTH;
                }
            }
            
            // Update buffer size to correct value.
            pIrp->dwActualLength -= (pSicdStateControl->dwDataPhaseCurrentReadOffset -
                                        pSicdStateControl->dwDataPhaseTotalDataSize);
            
        }
        else if ( (eDataDirection == MUSB_SICD_DATA_OUT) && bDataOutComplete && 
                  (pSicdStateControl->dwDataPhaseCurrentWriteOffset < pSicdStateControl->dwDataPhaseTotalDataSize) )
        {
            MGC_SicdNotifyDataCbk(pSicdProtocol, MUSB_HFI_COND_WRITE_OK);
        }
        else if ( (eDataDirection == MUSB_SICD_DATA_OUT) && !bDataOutComplete )
        {
            // Don't notify upper layer, we silently start to send another data out packet.
        }
        else
        {
            // unexpected case!!;
            MUSB_ASSERT(0);
        }

        // There are still some data we should read in, continue to read.
        if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
             (pSicdStateControl->dwDataPhaseCurrentReadOffset < 
                pSicdStateControl->dwDataPhaseTotalDataSize) )
        {
            Result = (MUSB_SicdResult)MUSB_SICD_DATA_PHASE_CONTINUE;
        }
        // There are still some data we should write out, continue to write.
        else if ( (eDataDirection == MUSB_SICD_DATA_OUT) && 
                  (pSicdStateControl->dwDataPhaseCurrentWriteOffset < 
                                    pSicdStateControl->dwDataPhaseTotalDataSize) )
        {
            Result = (MUSB_SicdResult)MUSB_SICD_DATA_PHASE_CONTINUE;        
        }
        else    // data phase complete
        {
            // print log only for GetObject and GetPartialObject
            if ( (pSicdStateControl->SicdCommand.rCommandBlock.wOperationCode 
                == MUSB_SICD_GET_OBJ)
                ||  (pSicdStateControl->SicdCommand.rCommandBlock.wOperationCode 
                    == MUSB_SICD_GET_PARTIAL_OBJ) )
            {
                LOG(7, "Data Phase Total Transferred size: %d\n",
                    pSicdStateControl->dwDataPhaseCurrentReadOffset);
            }

            // If we use SICD generic buffer instead of client buffer, recover back here.
            if ( pSicdStateControl->SicdCommand.dwBackupSize < MUSB_SICD_HEADER_MINIMUM_LENGTH )
            {
                pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen = 
                                    pSicdStateControl->SicdCommand.dwBackupSize;
                for ( i = 0 ; i < pSicdStateControl->SicdCommand.rDataBlock.dwPayLoadBufLen ; i++ )
                {
                    pSicdStateControl->SicdCommand.rDataBlock.pbPayLoadBuf[i] = 
                                pIrp->pBuffer[i];
                }
            }
            
            // Notify phase change
            MGC_SicdNotifyPhaseChangeCbk(pSicdProtocol, MUSB_SICD_PHASE_DATA);
                                                
            Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }

        // For Data IN, if data phase controlled by middleware, do nothing here. 
        // We wait upper layer to invoke get/send next data to continue SICD state machine.
        if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
             (pSicdStateControl->bDataPhaseControlledByMiddleware) && 
             (Result == MUSB_SICD_DATA_PHASE_CONTINUE) &&
             (!bIsFirstDataPacketOfDataPhase) )
        {
            Result = (MUSB_SicdResult)MUSB_SICD_DATA_PHASE_CONTROLLED_BY_MW;
        }
        // For Data IN, if it's first packet and we have more data to receive, 
        // do nothing here and just continue data phase.
        else if ( (eDataDirection == MUSB_SICD_DATA_IN) && 
             (pSicdStateControl->bDataPhaseControlledByMiddleware) && 
             (Result == MUSB_SICD_DATA_PHASE_CONTINUE) &&
             (bIsFirstDataPacketOfDataPhase) )
        {
            // do nothing here.
        }
        // For Data OUT, if not ContinueDataPhaseSilently, it's default that 
        //  SICD state machine controlled by middleware.
        else if ( (eDataDirection == MUSB_SICD_DATA_OUT) && 
                  (!bShouldContinueDataPhaseSilently) && 
                  (Result == MUSB_SICD_DATA_PHASE_CONTINUE) )
        {
            Result = (MUSB_SicdResult)MUSB_SICD_DATA_PHASE_CONTROLLED_BY_MW;
        }

        break;
        
    case MUSB_SICD_RESPONSE_OPERATION:
        if ( (pIrp->dwActualLength >= MUSB_SICD_HEADER_MINIMUM_LENGTH) && 
             (pIrp->dwActualLength <= 512) && 
             (pIrp->dwActualLength == MUSB_SWAP32P(&pIrp->pBuffer[0])) )
        {        
            // allow to send next command when response ok.
            pSicdStateControl->bCurStateIndex++;
            if ( pSicdStateControl->bCurStateIndex >= pSicdStateControl->bTotalStateCount )
            {
                //there is no state flow list in state machine
                (void)MGC_GoToReadyState(pSicdProtocol);
            }
        
            // notify phase change to response phase
            MGC_SicdNotifyPhaseChangeCbk(pSicdProtocol, MUSB_SICD_PHASE_RESPONSE);
            return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        }
        else    // Error Handling
        {
            LOG(1, "Response length= %d.\n", pIrp->dwActualLength);
            // Try Response Phase Again
            Result = (MUSB_SicdResult)MUSB_SICD_RESPONSE_PHASE_CONTINUE;
            break;
        }

    case MUSB_SICD_NO_OPERATION:
    case MUSB_SICD_STANDARD_OPERATION:
    case MUSB_SICD_NOTIFY_OPERATION:
    default:
        //error handler
        LOG(1, "MGC_SicdCompleteTransferOperation: error default value1\n", NULL);
        break;
    }

    // Decide what to do next by checking Result. 
    if (Result == MUSB_SICD_SUCCESS)
    {
        pSicdStateControl->bCurStateIndex++;
    }
    
    return Result;
}

/*************************** FUNCTIONS ****************************/

/*******************************************************************************
**  MUSB_SicdSetDataPhaseController
**
**  descriptions:
**      Set the SICD Data Phase controller. The controller can be Sicd or Middleware.
**      If the controller is SICD, data phase will be automatically completed by 
**      SICD. If the controller is Middleware, it requires the Middleware to 
**      invoke GetNextData to continue data phase. 
**      The main reason for this function is to support reading objects which on
**      devices not support GetPartialObject.
**  parameters:
**      0       : Controlled by Middleware
**      else    : Controlled by SICD
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdSetDataPhaseControlledByMiddleware(
                            MUSB_SicdProtocol* pSicdProtocol, BOOL bByMw)
{
    MUSB_ASSERT(pSicdProtocol);
    LOG(5, "MUSB_SicdSetDataPhaseControlledByMiddleware %d\n", bByMw);
    pSicdProtocol->SicdStateControl.bDataPhaseControlledByMiddleware = (uint8_t)bByMw;
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}

/*******************************************************************************
**  MUSB_SicdStateInit
**
**  descriptions:
**      Initialize sicd state
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdStateInit(MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_SicdStateControl* pSicdStateControl;
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    pSicdStateControl->bTotalStateCount = 0;
    pSicdStateControl->bCurStateIndex = 0;
    pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_READY;
    
    pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
    pSicdStateControl->dwDataPhaseCurrentWriteOffset = 0;
    pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SICD_DATA_DEFAULT_SIZE;
    pSicdStateControl->bDataPhaseControlledByMiddleware = FALSE;

    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;   
}

/*******************************************************************************
**  MUSB_SicdStateExit
**
**  descriptions:
**      This function is just used for error handling.
**      When SICD state machine is waiting for an incoming Irp and the device be
**      unplugged, background thread will flush endpoint data and SICD state machine
**      will not receive MGC_SicdIrpComplete for Irp error. Thus, SICD state
**      machine will not notify upper layer with Operation Error Callback. 
**      This function is used to check whether we should notify Operation Error
**      Callback tp upper layer or not.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdStateExit(MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_SicdStateControl* pSicdStateControl;

    MUSB_ASSERT(pSicdProtocol);
    
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    // Means we have not finish state machine, we should notify upper layer
    // Operation Error Callback. 
    if ( (pSicdStateControl->bTotalStateCount != 0) && 
         (pSicdStateControl->bCurStateIndex < pSicdStateControl->bTotalStateCount) )
    {
        LOG(5, "MUSB_SicdStateExit\n", NULL);
        MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
    }

    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;   
}

/*******************************************************************************
**  MUSB_SicdHandleStandardOperation
**
**  descriptions:
**      Handle sicd class-specific or standard requests.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MUSB_SicdHandleStandardOperation(MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;    
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }       
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];    

    if(pStateFlow->eOpeartionType != MUSB_SICD_STANDARD_OPERATION)
    {
        LOG(1, "MUSB_SicdHandleStandardOperation state error\n", NULL);
        return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }
    
    switch(pStateFlow->eOpeartionCode)
    {
    case MUSB_SICD_GET_DEVICE_STATUS:
       Result = MUSB_SicdTransferGetDeviceStatus(pSicdProtocol);
       break;
       
    case MUSB_SICD_ABORT_REQUEST:
       Result = MUSB_SicdTransferCancelRequest(pSicdProtocol);
       break;
       
    case MUSB_SICD_CLEAR_FEATURE_REQUEST:
       Result = MUSB_SicdTransferClearFeatureRequest(pSicdProtocol);
       break;
       
    case MUSB_SICD_RESET_REQUEST:
       Result = MUSB_SicdTransferResetDeviceRequest(pSicdProtocol);
       break;

    case MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST: 
       Result = MUSB_SicdTransferGetExtEventDataRequest(pSicdProtocol);
       break;

    case MUSB_SICD_TRANSFER_OPERATION:
    case MUSB_SICD_DATAPHASE_CTRL_BY_MW:
    case MUSB_SICD_DATAPHASE_CTRL_BY_SICD:
    case MUSB_SICD_GET_NEXT_DATA:
    case MUSB_SICD_SET_SILENT_BUS_RESET:
    case MUSB_SICD_RESET_OP_EVT:
    case MUSB_SICD_ABORT_OP_EVT:
    case MUSB_SICD_GET_EXT_EVENT_OP_EVT:
    default:
        LOG(1, "MUSB_SicdHandleStandardOperation: error default value\n", NULL);
        return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }
    return Result;

}

/*******************************************************************************
**  MUSB_SicdHandleTransferOperation
**
**  descriptions:
**      Handle command/data/response transfers.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MUSB_SicdHandleTransferOperation(MUSB_SicdProtocol *pSicdProtocol, 
                                    MUSB_SicdOperationType SicdOperationType)
{
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;    
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }       
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];

    if (pStateFlow->eOpeartionType != SicdOperationType )
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }
   
    switch(SicdOperationType)
    {
    case MUSB_SICD_CMD_OPERATION:
        Result = MUSB_SicdTransferCommandBlock(pSicdProtocol);
        break;
       
    case MUSB_SICD_DATA_OPERATION:
        Result = MUSB_SicdTransferDataBlock(pSicdProtocol);
        break;
       
    case MUSB_SICD_RESPONSE_OPERATION:
        Result = MUSB_SicdTransferResponseBlock(pSicdProtocol);
        break;

    case MUSB_SICD_NO_OPERATION:
    case MUSB_SICD_STANDARD_OPERATION:
    case MUSB_SICD_NOTIFY_OPERATION:
    default:
        LOG(1, "MUSB_SicdHandleTransferOperation: error default value\n", NULL);
        Result = (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
        break;
    }
    return Result;
}

/*******************************************************************************
**  MUSB_SicdHandleNotifyOperation
**
**  descriptions:
**      Handle notify operations. The notify operation we mean here is to notify
**      upper layer. Currently, we only use this for reset request, cancel request
**      and get extended event data request.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static MUSB_SicdResult MUSB_SicdHandleNotifyOperation(MUSB_SicdProtocol *pSicdProtocol)
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

    switch ( pStateFlow->eOpeartionCode )
    {
    case MUSB_SICD_RESET_OP_EVT:
        // Case 1, dev supports Reset: 
        //      Since we will do retry for operation abort by getting devstatus, 
        //      exceeding retry count means retry failed.
        // Case 2, dev does not support reset: 
        //      Don't care the retry result of getting devstatus.
        if (pSicdStateControl->dwRetryCount >= MUSB_SICD_MAX_OPERATION_RETRY_COUNT )
        {
            MGC_SicdNotifyResetCbk(pSicdProtocol, (BOOL)FALSE);
        }
        else
        {
            MGC_SicdNotifyResetCbk(pSicdProtocol, (BOOL)TRUE);
        }
        break;
        
    case MUSB_SICD_ABORT_OP_EVT:
        // Case 1, dev supports Abort: 
        //      Since we will do retry for operation abort by getting devstatus, 
        //      exceeding retry count means retry failed. (but abort op is OK)
        // Case 2, dev does not support Abort: (will not enter here)
        //      The callback is invoked in MGC_SicdCompleteStandardOperation().
        // 
        // Therefore, here we only notify OK.
        MGC_SicdNotifyOperationAbortCbk(pSicdProtocol, (BOOL)TRUE);
        break;
        
    case MUSB_SICD_GET_EXT_EVENT_OP_EVT:
        MGC_SicdNotifyGetExtEventDataCbk(pSicdProtocol);
        break; 

    case MUSB_SICD_SET_SILENT_BUS_RESET:
        break;

    case MUSB_SICD_TRANSFER_OPERATION:
    case MUSB_SICD_GET_DEVICE_STATUS:
    case MUSB_SICD_ABORT_REQUEST:
    case MUSB_SICD_RESET_REQUEST:
    case MUSB_SICD_CLEAR_FEATURE_REQUEST:
    case MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST:
    case MUSB_SICD_DATAPHASE_CTRL_BY_MW:
    case MUSB_SICD_DATAPHASE_CTRL_BY_SICD:
    case MUSB_SICD_GET_NEXT_DATA:
    default:
        LOG(1, "MUSB_SicdHandleNotifyOperation: error default value\n", NULL);
        break;
    }

    pSicdStateControl->bCurStateIndex++;
    
    if ( pSicdStateControl->bCurStateIndex >= pSicdStateControl->bTotalStateCount )
    {
        //there is no state flow list in state machine
        Result = MGC_GoToReadyState(pSicdProtocol);
    }
    else
    {
        Result = MUSB_SicdHandleOperation(pSicdProtocol);
        if ( Result != MUSB_SICD_SUCCESS )
        {
            MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
        }
    }

    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}

/*******************************************************************************
**  MUSB_SicdHandleOperation
**
**  descriptions:
**      The entry point of handling all kinds of operations.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdHandleOperation(MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }        
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];

    if( (pSicdStateControl->bTotalStateCount == 0) ||
        (pSicdStateControl->bCurStateIndex >= pSicdStateControl->bTotalStateCount) )
    {
       return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }

    if(pStateFlow->eOpeartionType == MUSB_SICD_NO_OPERATION)
    {
       return (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
    }

    switch(pStateFlow->eOpeartionType)
    {
    case MUSB_SICD_STANDARD_OPERATION:
       Result = MUSB_SicdHandleStandardOperation(pSicdProtocol);
       break;
       
    case MUSB_SICD_CMD_OPERATION:
    case MUSB_SICD_DATA_OPERATION:
    case MUSB_SICD_RESPONSE_OPERATION:
        Result = MUSB_SicdHandleTransferOperation(pSicdProtocol, 
            pStateFlow->eOpeartionType);
       break;
       
    case MUSB_SICD_NOTIFY_OPERATION:
       Result = MUSB_SicdHandleNotifyOperation(pSicdProtocol);
       break;

    case MUSB_SICD_NO_OPERATION:
    default:
       Result = (MUSB_SicdResult)MUSB_E_SICD_DEVICE_STATE_ERROR;
       break;
    }
    return Result;
}

/*******************************************************************************
**  MUSB_SicdCreateSendOpeartionFlow
**
**  descriptions:
**      Fill the state machine data of MTP/PTP operation sending flow.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdCreateSendOpeartionFlow(MUSB_SicdProtocol *pSicdProtocol)
{
    uint8_t bTempCount = 0;
    MUSB_SicdStateControl* pSicdStateControl;

    LOG(7, "MUSB_SicdCreateSendOpeartionFlow\n", NULL);

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_CMD_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_TRANSFER_OPERATION;    
    bTempCount++;
    
    if(pSicdStateControl->SicdCommand.rDataBlock.eDataDirection != MUSB_SICD_NO_DATA)
    {
        pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_DATA_OPERATION;
        pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_TRANSFER_OPERATION;
        bTempCount++;        
    }

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_RESPONSE_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_TRANSFER_OPERATION;
    bTempCount++;

    pSicdStateControl->bCurStateIndex = 0;    
    pSicdStateControl->dwRetryCount = 0;    
    pSicdStateControl->bTotalStateCount = bTempCount;
    pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
    pSicdStateControl->dwDataPhaseCurrentWriteOffset = 0;

    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}

/*******************************************************************************
**  MUSB_SicdCreateClearFeatureFlow
**
**  descriptions:
**      Fill the state machine data of clear feature flow.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdCreateClearFeatureFlow(MUSB_SicdProtocol *pSicdProtocol)
{
    uint8_t bTempCount = 0;
    MUSB_SicdStateControl* pSicdStateControl;
    MUSB_SicdStateData arStateListTemp[MUSB_SICD_MAX_STATE_FLOW_COUNT];
    MUSB_SicdStateData *pSicdStateData1, *pSicdStateData2;

    LOG(7, "MUSB_SicdCreateClearFeatureFlow\n", NULL);

    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    
    if(pSicdStateControl->eDriverState == MUSB_SICD_DRIVER_STATE_UNKNOWN)
    {
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }
  
    //now start to creat state flow list...
    arStateListTemp[bTempCount].eOpeartionType = MUSB_SICD_STANDARD_OPERATION;
    arStateListTemp[bTempCount].eOpeartionCode = MUSB_SICD_CLEAR_FEATURE_REQUEST;
    bTempCount++;

    for(bTempCount=bTempCount; 
       bTempCount < ((pSicdStateControl->bTotalStateCount - pSicdStateControl->bCurStateIndex) + (uint8_t)1);
       bTempCount++)
    {
        if (bTempCount >= MUSB_SICD_MAX_STATE_FLOW_COUNT)
        {
            LOG(0, "bTempCount = %d OverFlow\n", bTempCount);
            return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
        }        

        pSicdStateData1 = &arStateListTemp[bTempCount];        

        if ((((pSicdStateControl->bCurStateIndex + bTempCount) - 1) >= 
              (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT) ||
              ((pSicdStateControl->bCurStateIndex + bTempCount) < 1))
        {
            LOG(0, "Index = %d OverFlow\n", ((pSicdStateControl->bCurStateIndex + bTempCount) - 1));
            return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
        }        

        pSicdStateData2 = &pSicdStateControl->StateFlow[(pSicdStateControl->bCurStateIndex + bTempCount) - 1];
        pSicdStateData1->eOpeartionType = pSicdStateData2->eOpeartionType;
        pSicdStateData1->eOpeartionCode = pSicdStateData2->eOpeartionCode;
    }
    
    (void)MUSB_MemCopy(pSicdStateControl->StateFlow, arStateListTemp, 
                                        sizeof(MUSB_SicdStateData) * bTempCount);
      
   
    pSicdStateControl->bCurStateIndex = 0;
    pSicdStateControl->dwRetryCount = 0;    
    pSicdStateControl->bTotalStateCount = bTempCount;
    pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
    pSicdStateControl->dwDataPhaseCurrentWriteOffset = 0;
    pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SICD_DATA_DEFAULT_SIZE;
    
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;    
}

/*******************************************************************************
**  MUSB_SicdCreateResetFlow
**
**  descriptions:
**      Fill the state machine data of sicd reset request flow.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdCreateResetFlow(MUSB_SicdProtocol *pSicdProtocol)
{
    uint8_t bTempCount = 0;
    MUSB_SicdStateControl *pSicdStateControl;

    LOG(7, "MUSB_SicdCreateResetFlow\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    
    //now start to creat state flow list...
    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_STANDARD_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_RESET_REQUEST;
    bTempCount++;

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_STANDARD_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_GET_DEVICE_STATUS;
    bTempCount++;

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_NOTIFY_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_RESET_OP_EVT;
    bTempCount++;  

    pSicdStateControl->bCurStateIndex = 0;
    pSicdStateControl->dwRetryCount = 0;    
    pSicdStateControl->bTotalStateCount = bTempCount;
    pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
    pSicdStateControl->dwDataPhaseCurrentWriteOffset = 0;
    pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SICD_DATA_DEFAULT_SIZE;
    
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}

/*******************************************************************************
**  MUSB_SicdCreateAbortFlow
**
**  descriptions:
**      Fill the state machine data of sicd cancel(abort) request flow.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdCreateAbortFlow(MUSB_SicdProtocol *pSicdProtocol)
{
    uint8_t bTempCount = 0;
    MUSB_SicdStateControl *pSicdStateControl;

    LOG(7, "MUSB_SicdCreateAbortFlow\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    if ( pSicdStateControl->eDriverState != MUSB_SICD_DRIVER_STATE_RUNNING )
    {
        LOG(1, "SICD: Driver Not Running\n", NULL);
        return (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    }
    
    pSicdStateControl->eDriverState = MUSB_SICD_DRIVER_STATE_STOPPING;
    MGC_BackupStateControlData(pSicdProtocol);

    //now start to creat state flow list...
    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_STANDARD_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_ABORT_REQUEST; 
    bTempCount++;

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_STANDARD_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_GET_DEVICE_STATUS;
    bTempCount++;    

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_NOTIFY_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_ABORT_OP_EVT;
    bTempCount++;  

    pSicdStateControl->bCurStateIndex = 0;
    pSicdStateControl->dwRetryCount = 0;
    pSicdStateControl->bTotalStateCount = bTempCount;
    pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
    pSicdStateControl->dwDataPhaseCurrentWriteOffset = 0;
    pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SICD_DATA_DEFAULT_SIZE;         
    
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;    
}

/*******************************************************************************
**  MUSB_SicdCreateGetExtEventDataFlow
**
**  descriptions:
**      Fill the state machine data of sicd get extended event data request flow.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdCreateGetExtEventDataFlow(MUSB_SicdProtocol *pSicdProtocol)
{
    uint8_t bTempCount = 0;
    MUSB_SicdStateControl *pSicdStateControl;

    LOG(7, "MUSB_SicdCreateGetExtEventDataFlow\n", NULL);
    
    MUSB_ASSERT(pSicdProtocol);
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);
    
    //now start to creat state flow list...
    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_STANDARD_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_GET_EXT_EVENT_DATA_REQUEST;
    bTempCount++;

    pSicdStateControl->StateFlow[bTempCount].eOpeartionType = MUSB_SICD_NOTIFY_OPERATION;
    pSicdStateControl->StateFlow[bTempCount].eOpeartionCode = MUSB_SICD_GET_EXT_EVENT_OP_EVT;
    bTempCount++;  

    pSicdStateControl->bCurStateIndex = 0;
    pSicdStateControl->dwRetryCount = 0;    
    pSicdStateControl->bTotalStateCount = bTempCount;
    pSicdStateControl->dwDataPhaseCurrentReadOffset = 0;
    pSicdStateControl->dwDataPhaseCurrentWriteOffset = 0;
    pSicdStateControl->dwDataPhaseTotalDataSize = MUSB_SICD_DATA_DEFAULT_SIZE;
    
    return (MUSB_SicdResult)MUSB_SICD_SUCCESS;
}

/*******************************************************************************
**  MUSB_SicdCreateSilentBusResetFlow
**
**  descriptions:
**      Create silent bus reset flow. This is used for devices which are not 
**      support device reset.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
MUSB_SicdResult MUSB_SicdCreateSilentBusResetFlow(MUSB_SicdProtocol *pSicdProtocol)
{
    MUSB_SicdResult Result;

    LOG(7, "MUSB_SicdCreateSilentBusResetFlow\n", NULL);
    
    Result = MGC_GoToReadyState(pSicdProtocol);    
    
    return Result;
}

/*******************************************************************************
**  MUSB_SicdControlIrpComplete
**
**  descriptions:
**      The callback function of sending contorl irp.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
void MUSB_SicdControlIrpComplete(void* pParam, MUSB_ControlIrp *pIrp)
{
    MGC_SicdIrpComplete(pParam, (const void*)pIrp);
    UNUSED(pIrp);
}
/*******************************************************************************
**  MUSB_SicdBulkIrpComplete
**
**  descriptions:
**      The callback function of sending/receiving bulk irp.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
void MUSB_SicdBulkIrpComplete(void *pParam, const MUSB_Irp *pBulkIrp)
{
    MGC_SicdIrpComplete(pParam, (const void*)pBulkIrp);    
}

/*******************************************************************************
**  MUSB_SicdInterruptIrpComplete
**
**  descriptions:
**      The callback function of receiving interrupt irp.
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
#if MGC_SICD_OPEN_INTERRUPT_PIPE
void MUSB_SicdInterruptIrpComplete(void *pParam, const MUSB_Irp *pIntrIrp)
{
    MUSB_SicdProtocol* pSicdProtocol;

    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pIntrIrp);

    if ( pIntrIrp->dwStatus == MUSB_STATUS_OK )
    {
        LOG(1, "SICD: Got interrupt pipe event ! \n", NULL);
        pSicdProtocol = pParam;
        MGC_SicdNotifyEventCbk(pSicdProtocol);
    }
    else
    {
        LOG(7, "SICD: Got interrupt packet but IRP status error ! \n", NULL);
    }
}
#endif

/*******************************************************************************
**  MGC_SicdIrpComplete
**
**  descriptions:
**      The main handling function for MUSB_SicdControlIrpComplete and 
**      MUSB_SicdBulkIrpComplete. 
**  parameters:
**  result:
**  return:
**  note:
*******************************************************************************/
static void MGC_SicdIrpComplete(void* pParam, const void *pGeneralIrp)
{
    MUSB_SicdProtocol* pSicdProtocol;
    MUSB_SicdStateControl *pSicdStateControl;
    MUSB_SicdStateData *pStateFlow;    
    MUSB_SicdResult Result = (MUSB_SicdResult)MUSB_E_SICD_DRIVER_ERROR;
    uint32_t dwStatus;
    uint8_t bIsControlPipeCallback = FALSE;

    LOG(7, "MGC_SicdIrpComplete\n", NULL);

    MUSB_ASSERT(pParam);
    MUSB_ASSERT(pGeneralIrp);
    pSicdProtocol = pParam;
    pSicdStateControl = &(pSicdProtocol->SicdStateControl);

    if (pSicdStateControl->bCurStateIndex >= (uint8_t)MUSB_SICD_MAX_STATE_FLOW_COUNT)
    {
        return;
    }    
    
    pStateFlow = &pSicdStateControl->StateFlow[pSicdStateControl->bCurStateIndex];  

    // it's control pipe irp
    if ( ((MUSB_ControlIrp*)pGeneralIrp)->pfIrpComplete == 
            MUSB_SicdControlIrpComplete )
    {
        bIsControlPipeCallback = TRUE;
        dwStatus = ((MUSB_ControlIrp*)pGeneralIrp)->dwStatus;
    }
    // it's data in/out pipe irp
    else
    {
        if ( pSicdStateControl->eDriverState == MUSB_SICD_DRIVER_STATE_STOPPING )
        {
            pSicdStateControl->dwDataPhaseCurrentReadOffset += 
                ((MUSB_Irp*)pGeneralIrp)->dwActualLength;
            LOG(7, "MGC_SicdIrpComplete drop one packet when aborting op\n", NULL);
            return;
        }
        dwStatus = ((MUSB_Irp*)pGeneralIrp)->dwStatus;
    }
    
    if ( dwStatus != MUSB_STATUS_OK )
    {
        LOG(1, "SICD: Last URB Error, Number: %X\n", dwStatus);
              
        switch ( dwStatus )
        {
        case MUSB_STATUS_STALLED:
            LOG(1, "SICD: Last URB Stalled\n", NULL);

            // it's control pipe, no need to clear feature
            if ( bIsControlPipeCallback )
            {
                // we will check whether should notify upper layer or not
                //          later in MGC_SicdCompleteStandardOperation()
                break;
            }
            
            // it's not control pipe, clear feature
            (void)MUSB_SicdCreateClearFeatureFlow(pSicdProtocol);
            Result = MUSB_SicdHandleOperation(pSicdProtocol);
            if ( Result != MUSB_SICD_SUCCESS )
            {
                MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
            }
            return;
                      
        case MUSB_STATUS_ACK_TIMEOUT:
        case MUSB_STATUS_NACK_LIMIT:
            Result = MGC_HandleTimeOutStatus(pSicdProtocol);
            if ( Result )
            {
                MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
                MGC_SilentReset(pSicdProtocol);
            }
            return;
           
        case MUSB_STATUS_TRANSMIT_ERROR:
        case MUSB_STATUS_RECEIVE_ERROR:
            LOG(1, "SICD: Last URB RX/TX Error\n", NULL);
            MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
            MGC_SilentReset(pSicdProtocol);
            return;
            
        default:
            LOG(1, "MGC_SicdIrpComplete: error default value1\n", NULL);
            MGC_SilentReset(pSicdProtocol);
            return;
        }
    }

    switch(pStateFlow->eOpeartionType)
    {
    case MUSB_SICD_STANDARD_OPERATION:  
        Result = MGC_SicdCompleteStandardOperation(pSicdProtocol);
        break;
        
    case MUSB_SICD_CMD_OPERATION:
    case MUSB_SICD_DATA_OPERATION:
        Result = MGC_SicdCompleteTransferOperation(pSicdProtocol);
        break;

    case MUSB_SICD_RESPONSE_OPERATION:
        Result = MGC_SicdCompleteTransferOperation(pSicdProtocol);
        if (Result == MUSB_SICD_SUCCESS)
        {
            return;
        }
        break;
        
    default:
        break;
    }
   
    switch(Result)
    {
    case MUSB_SICD_SUCCESS:
    case MUSB_SICD_DATA_PHASE_CONTINUE:
    case MUSB_SICD_RESPONSE_PHASE_CONTINUE:        
        if ( pSicdStateControl->bCurStateIndex >= pSicdStateControl->bTotalStateCount )
        {
            //there is no state flow list in state machine
            Result = MGC_GoToReadyState(pSicdProtocol);
        }
        else
        {
            Result = MUSB_SicdHandleOperation(pSicdProtocol);
            if ( Result != MUSB_SICD_SUCCESS )
            {
                MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
            }
        }
        break;

    case MUSB_SICD_DATA_PHASE_CONTROLLED_BY_MW:
        Result = (MUSB_SicdResult)MUSB_SICD_SUCCESS;
        break;
      
    case MUSB_E_SICD_DEVICE_BUSY:
        UNUSED(MUSB_Sleep(200));

        Result = MUSB_SicdHandleOperation(pSicdProtocol);
        if ( Result )
        {
            MGC_SicdNotifyOperationErrCbk(pSicdProtocol);
        }      
        break;
        
    case MUSB_E_SICD_DEVICE_TRANSACTION_CANCELLED:
        LOG(1, "MGC_SicdIrpComplete: MUSB_E_SICD_DEVICE_TRANSACTION_CANCELLED not handled\n", NULL);
        break;	  
        
    default:
        LOG(1, "MGC_SicdIrpComplete: error default value2\n", NULL);
        break;
    }
    
    if ((Result != MUSB_SICD_SUCCESS) &&
        (dwStatus != (uint32_t)MUSB_STATUS_PEER_REMOVED))
    {
        MGC_SilentReset(pSicdProtocol);
    }

    return;
}

#endif  /* #ifdef MUSB_SICD */
