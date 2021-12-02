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
 * Top-level Audio class driver module
 * $Revision: #1 $
 */
#ifdef MUSB_AUD

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_hfi_if.h"
#include "mu_mem.h"
#include "mu_stdio.h"

#include "mu_impl.h"

#include "mu_audio.h"

#define MGC_AUD_CONFIG_TIME   (500)

#define MGC_AUD_DEVICE_NUM   (2)

/**************************** GLOBALS *****************************/
static const uint8_t MGC_aAudPeripheralList [] =
{
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_PER_INTERFACE, (uint8_t)MUSB_TARGET_INTERFACE, 0,
    (uint8_t)MUSB_TARGET_CLASS, MUSB_CLASS_AUDIO, (uint8_t)MUSB_TARGET_ACCEPT
};

/** Mass Storage device driver */
static MUSB_DeviceDriver MGC_AudDeviceDriver =
{
    NULL, MUSB_AudConnect, MUSB_AudDisconnect, NULL, NULL,
};

static MGC_AudDevice *arAudDevice[MGC_AUD_DEVICE_NUM];

static MUSB_AUDParam MGC_rAudDefaultPlayParm = 
{
    MUSB_AUD_PCM,  /* wFormatTag */
    2,                          /* bNrChannels */
    16,                        /* bBitResolution */
    44100                   /* dwSamFreq */    
};
    
static void MGC_AudCtrlCallback(void *pContext, MUSB_ControlIrp *pControlIrp);

/*************************** FUNCTIONS ****************************/
static void MGC_AudGetASEp(MUSB_InterfaceDescriptor *pIface, 
    MUSB_AUDParam *prPlayParm, MUSB_EndpointDescriptor **ppEnd, 
    MUSB_ASDataEndpointDescriptor **ppASEnd, MUSB_InterfaceDescriptor **ppAltIface)
{
    MUSB_AUDescriptorHeader *pDescriptor = NULL;
    MUSB_InterfaceDescriptor *pAltIface;       
    MUSB_ASInterfaceDescriptor *pASIface = NULL;
    MUSB_ASFormatTypeDescriptor *pASFmtType = NULL;
    uint32_t i;
    uint32_t dwSamFreq;
    uint8_t bFormatOK = FALSE;
    uint8_t bParamOK = FALSE;
    uint8_t *pbPtr;
    
    MUSB_ASSERT(pIface);
    MUSB_ASSERT(prPlayParm);

    pAltIface = pIface;
    pDescriptor = (MUSB_AUDescriptorHeader *)((uint32_t)pAltIface);

    // Only search the same interface number.
    while (pIface->bInterfaceNumber == pAltIface->bInterfaceNumber)
    {
        // Check AS interface.       
        if (pDescriptor->bDescriptorType == MUSB_CS_INTERFACE)
        {
            if (pDescriptor->bDescriptorSubtype == MUSB_AS_GENERAL)
            {
                pASIface = (MUSB_ASInterfaceDescriptor *)((uint32_t)pDescriptor);            
                if (prPlayParm->wFormatTag == pASIface->wFormatTag)
                {
                    bFormatOK = TRUE;
                }
            }
            else if (pDescriptor->bDescriptorSubtype == MUSB_FORMAT_TYPE)
            {
                pASFmtType = (MUSB_ASFormatTypeDescriptor *)((uint32_t)pDescriptor);            
                if ((bFormatOK) && 
                    (pASFmtType->bNrChannels == prPlayParm->bNrChannels) &&
                    (pASFmtType->bBitResolution == prPlayParm->bBitResolution))
                {
                    // Search sample frequency
                    MUSB_ASSERT(pASFmtType->bLength > 8);
                    for (i=0; i<(uint32_t)(pASFmtType->bLength-8); i=i+3)
                    {
                        // transform 3 bytes of sample frequency.
                        pbPtr = (uint8_t *)((uint32_t)pASFmtType+8+i);
                        dwSamFreq = MUSB_SWAP24P(pbPtr);
                        LOG(5, "AUD Ch= %d, Bit =%d, Freq=%d.\n", 
                            pASFmtType->bNrChannels, pASFmtType->bBitResolution, dwSamFreq);   
                        
                        if (prPlayParm->dwSamFreq == dwSamFreq)
                        {
                            bParamOK = TRUE;
                        }
                    }                    
                }
            }
        }
        else if (pDescriptor->bDescriptorType == MUSB_DT_INTERFACE)
        {
            // Check another alternative if.
            bFormatOK = FALSE;
            bParamOK = FALSE;
            pAltIface = (MUSB_InterfaceDescriptor *)((uint32_t)pDescriptor);
            LOG(5, "AUD If = %d, Alt =%d.\n", pAltIface->bInterfaceNumber, pAltIface->bAlternateSetting);   
        }

        pDescriptor = (MUSB_AUDescriptorHeader *)((uint32_t)pDescriptor + pDescriptor->bLength);

        if (bFormatOK && bParamOK)
        {
            MUSB_ASSERT(pDescriptor->bDescriptorType == MUSB_DT_ENDPOINT);
            if (ppEnd)
            {
                *ppEnd = (MUSB_EndpointDescriptor *)((uint32_t)pDescriptor);
            }
            if (ppASEnd)
            {
                pDescriptor = (MUSB_AUDescriptorHeader *)
                    ((uint32_t)pDescriptor + pDescriptor->bLength);
                MUSB_ASSERT(pDescriptor->bDescriptorType == MUSB_CS_ENDPOINT);               
                *ppASEnd = (MUSB_ASDataEndpointDescriptor *)((uint32_t)pDescriptor);
            }

            if (ppAltIface)
            {
                *ppAltIface = pAltIface;
            }           
            
            return ;
        }
    }
   
    return;
}

static void MGC_AudGetACSubTypeIface(MUSB_InterfaceDescriptor *pIface, 
    uint8_t bDesSubtype, void **p1, void **p2)
{
    MUSB_AUDescriptorHeader *pDescriptor = NULL;
    MUSB_ACInterfaceHeaderDescriptor *pACIfHeader = NULL;
    MUSB_ACInputTerminalDescriptor *pInTerm = NULL;
    MUSB_ACOutputTerminalDescriptor *pOutTerm = NULL;    
    uint16_t wLength = 0;

    MUSB_ASSERT(pIface);
    // At least one return object pointer.
    MUSB_ASSERT(p1);
    *p1 = NULL;
    pDescriptor = (MUSB_AUDescriptorHeader *)((uint32_t)pIface + pIface->bLength);

    if ((pDescriptor->bDescriptorType == MUSB_CS_INTERFACE) &&
        (pDescriptor->bDescriptorSubtype == MUSB_AC_HEADER))
    {
        pACIfHeader = (MUSB_ACInterfaceHeaderDescriptor *)((uint32_t)pDescriptor);
        
        // Check next AC descriptor.
        while (wLength < pACIfHeader->wTotalLength)
        {
            wLength += pDescriptor->bLength;
            pDescriptor = (MUSB_AUDescriptorHeader *)((uint32_t)pDescriptor + pDescriptor->bLength);
            
            if ((pDescriptor->bDescriptorType == MUSB_CS_INTERFACE) &&
                (pDescriptor->bDescriptorSubtype == bDesSubtype) &&
                 ((pDescriptor->bDescriptorSubtype == MUSB_AC_INPUT_TERMINAL) ||
                   (pDescriptor->bDescriptorSubtype == MUSB_AC_OUTPUT_TERMINAL)))
            {
                if (bDesSubtype == MUSB_AC_INPUT_TERMINAL)
                {
                    pInTerm = (MUSB_ACInputTerminalDescriptor *)((uint32_t)pDescriptor);
                    if (pInTerm->wTerminalType == MUSB_USB_TERMINAL)
                    {
                        *p1 = pDescriptor;                
                        break;
                    }
                }
                else if (bDesSubtype == MUSB_AC_OUTPUT_TERMINAL)
                {
                    pOutTerm = (MUSB_ACOutputTerminalDescriptor *)((uint32_t)pDescriptor);
                    if (pOutTerm->wTerminalType == MUSB_USB_TERMINAL)
                    {
                        *p1 = pDescriptor;                
                        break;
                    }
                }
            }

            if ((pDescriptor->bDescriptorType == MUSB_CS_INTERFACE) &&
                 (pDescriptor->bDescriptorSubtype != MUSB_AC_INPUT_TERMINAL) &&
                 (pDescriptor->bDescriptorSubtype != MUSB_AC_OUTPUT_TERMINAL))
            {
                if (pDescriptor->bDescriptorSubtype == bDesSubtype)
                {
                    *p1 = pDescriptor;                
                    break;
                }            
            }
        }

        if (p2)
        {   
            *p2 = NULL;
            while (wLength < pACIfHeader->wTotalLength)
            {
                wLength += pDescriptor->bLength;
                pDescriptor = (MUSB_AUDescriptorHeader *)((uint32_t)pDescriptor + pDescriptor->bLength);
                if ((pDescriptor->bDescriptorType == MUSB_CS_INTERFACE) &&
                    (pDescriptor->bDescriptorSubtype == bDesSubtype))
                {
                    *p2 = pDescriptor;
                    break;
                }            
            }
        }
    }
}

static void MGC_AudParseCtrlIface(MGC_AudDevice *pAudDevice)
{
    MUSB_InterfaceDescriptor *pIface;
    MUSB_ACInputTerminalDescriptor *pInTerm;
    MUSB_ACOutputTerminalDescriptor *pOutTerm;
    MUSB_ACFeatureUnitDescriptor *pFeature1;
    MUSB_ACFeatureUnitDescriptor *pFeature2;
    uint32_t i;

    MUSB_ASSERT(pAudDevice);    
    pIface = pAudDevice->pControlIfaceHead;
    MUSB_ASSERT(pIface);

    // Get input terminal descriptor.
    MGC_AudGetACSubTypeIface(pIface, MUSB_AC_INPUT_TERMINAL, 
        (void *)&pInTerm, NULL);
    MGC_AudGetACSubTypeIface(pIface, MUSB_AC_OUTPUT_TERMINAL, 
        (void *)&pOutTerm, NULL);
    MGC_AudGetACSubTypeIface(pIface, MUSB_AC_FEATURE_UNIT, 
        (void *)&pFeature1, (void *)&pFeature2);

    LOG(5, "AUD InTerm, Id=%d, AssId=%d.\n", pInTerm->bTerminalID, pInTerm->bAssocTerminal);
    LOG(5, "AUD OutTerm, Id=%d, AssId=%d.\n", pOutTerm->bTerminalID, pInTerm->bAssocTerminal);

    // check input terminal and its related feature unit.
    if (pFeature1 && pInTerm &&
        (pFeature1->bSourceID == pInTerm->bTerminalID))
    {
        pAudDevice->pItFeature = pFeature1;
        
        LOG(5, "AUD Input Feature, Id=%d, SourceId=%d.\n", 
            pFeature1->bUnitID, pFeature1->bSourceID);
        MUSB_ASSERT(pFeature1->bLength > 7);
        for (i=0; i<(uint32_t)(pFeature1->bLength-7); i++)
        {
            if (pFeature1->bmaControls[i] != 0)
            {
                pAudDevice->bItCtrlNum++;
                LOG(5, "AUD Input bmaControls[%d]=0x%X.\n", i, pFeature1->bmaControls[i]);
            }
        }
    }

    if (pFeature2 && pInTerm &&
        (pFeature2->bSourceID == pInTerm->bTerminalID))
    {
        pAudDevice->pItFeature = pFeature2;
        
        LOG(5, "AUD Input Feature, Id=%d, SourceId=%d.\n", 
            pFeature2->bUnitID, pFeature2->bSourceID);
        MUSB_ASSERT(pFeature2->bLength > 7);
        for (i=0; i<(uint32_t)(pFeature2->bLength-7); i++)
        {
            if (pFeature2->bmaControls[i] != 0)
            {
                pAudDevice->bItCtrlNum++;
                LOG(5, "AUD Input bmaControls[%d]=0x%X.\n", i, pFeature2->bmaControls[i]);
            }
        }
    }

    // check output terminal and its related feature unit.
    if (pFeature1 && pOutTerm &&
        (pFeature1->bUnitID == pOutTerm->bSourceID))
    {
        pAudDevice->pOtFeature = pFeature1;
        
        LOG(5, "AUD Output Feature, Id=%d, pOutTerm->bSourceID=%d.\n", 
            pFeature1->bUnitID, pOutTerm->bSourceID);
        MUSB_ASSERT(pFeature1->bLength > 7);
        for (i=0; i<(uint32_t)(pFeature1->bLength-7); i++)
        {
            if (pFeature1->bmaControls[i] != 0)
            {
                pAudDevice->bOtCtrlNum++;
                LOG(5, "AUD Output bmaControls[%d]=0x%X.\n", i, pFeature1->bmaControls[i]);
            }
        }
    }

    if (pFeature2 && pOutTerm &&
        (pFeature2->bUnitID == pOutTerm->bSourceID))
    {
        pAudDevice->pOtFeature = pFeature2;
        
        LOG(5, "AUD Output Feature, Id=%d, pOutTerm->bSourceID=%d.\n", 
            pFeature2->bUnitID, pOutTerm->bSourceID);
        MUSB_ASSERT(pFeature2->bLength > 7);
        for (i=0; i<(uint32_t)(pFeature2->bLength-7); i++)
        {
            if (pFeature2->bmaControls[i] != 0)
            {
                pAudDevice->bOtCtrlNum++;
                LOG(5, "AUD Output bmaControls[%d]=0x%X.\n", i, pFeature2->bmaControls[i]);
            }
        }
    }    
}

static uint32_t MGC_AudSetAttr(MGC_AudDevice *pAudDevice, 
    uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, 
    uint16_t wIndex, uint16_t wLength, uint8_t *pBuf)
{
    MUSB_DeviceRequest *pSetup;   
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;
    uint32_t i;
    
    MUSB_ASSERT(pAudDevice);
    MUSB_ASSERT(MUSB_AUD_CTRL_LENGTH >= wLength);
    MUSB_ASSERT(pBuf);
    
    pSetup = (MUSB_DeviceRequest *)pAudDevice->aSetupTx;
    pControlIrp = &(pAudDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Interface Alternative Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        bmRequestType,
        bRequest, 
        wValue,
        wIndex, 
        wLength);

    /** Fill user's data */
    for (i=0; i<wLength; i++)
    {
        pAudDevice->aSetupTx[8+i] = pBuf[i];
    }
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pAudDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        (8+wLength),    /* Including user's out data */
        NULL, 
        0, 
        MGC_AudCtrlCallback);

    dwStatus = MUSB_StartControlTransfer(pAudDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "AUD Err: Set Attr failed, dwStatus: %d.\n", dwStatus);
    }

    return (dwStatus);
}

static uint32_t MGC_AudGetAttr(MGC_AudDevice *pAudDevice, 
    uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint16_t wLength, uint8_t *pBuf)
{
    MUSB_DeviceRequest *pSetup;   
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pAudDevice);
    
    pSetup = (MUSB_DeviceRequest *)pAudDevice->aSetupTx;
    pControlIrp = &(pAudDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Interface Alternative Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        (MUSB_DIR_IN| MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE),
        bRequest, 
        wValue,
        wIndex, 
        wLength);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pAudDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        sizeof(MUSB_DeviceRequest), 
        pBuf, 
        wLength, 
        MGC_AudCtrlCallback);

    dwStatus = MUSB_StartControlTransfer(pAudDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "AUD Err: Get Interface failed, dwStatus: %d.\n", dwStatus);
    }

    return (dwStatus);
}

static uint32_t MGC_AudSetInfAlt(MGC_AudDevice *pAudDevice, 
    MUSB_InterfaceDescriptor *pIface)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pAudDevice);
    MUSB_ASSERT(pIface);
    
    pSetup = (MUSB_DeviceRequest *)pAudDevice->aSetupTx;
    pControlIrp = &(pAudDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Interface Alternative Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_INTERFACE),
        MUSB_REQ_SET_INTERFACE, 
        pIface->bAlternateSetting,
        pIface->bInterfaceNumber, 
        0);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pAudDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        8, /* Only setup phase data */ 
        NULL, 
        0, 
        MGC_AudCtrlCallback);

    dwStatus = MUSB_StartControlTransfer(pAudDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "AUD Err: Set Interface failed, dwStatus: 0x%X.\n", dwStatus);
    }

    return (dwStatus);
}

static void MGC_AudCtrlCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_AudDevice *pAudDevice = NULL;
    MUSB_ACFeatureUnitDescriptor *pFeature = NULL;
    MUSB_ACVolume *pVolume = NULL;
    uint8_t bCtrlIdx = 0;
    uint8_t bInterfaceNumber = 0;    
    uint8_t bFound;
    uint32_t i;
    
    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
   
    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(1, "AUD Err: Callback Status = 0x%X.\n", pControlIrp->dwStatus);
        return;
    }

    pAudDevice = (MGC_AudDevice *)pContext;

    /* Since IRP is executed properly hence update hub state to its next state.
     */
    pAudDevice->bState = pAudDevice->bNextState;

    LOG(7, "AUD State: %d.\n", pAudDevice->bState);

    if (MUSB_AUD_STATE_SETOUTFREQ == pAudDevice->bState)
    {
        pAudDevice->bNextState = MUSB_AUD_STATE_PLAYING;
        // Set endpoint control: sampling frequency control.
        (void)MGC_AudSetAttr(pAudDevice, 
            (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_ENDPOINT), /*bmRequestType*/
            MUSB_AC_SET_CUR, /* bRequest */ 
            (MUSB_AC_SAMPLING_FREQ_CONTROL<<8), 
            pAudDevice->pIsoOutEnd->bEndpointAddress, 
            MUSB_AC_SAMPLING_FREQ_LENGTH, 
            (uint8_t *)&(pAudDevice->prIsoOutParm->dwSamFreq));
        return;
    } 

    if (MUSB_AUD_STATE_PLAYING == pAudDevice->bState)
    {
        // Set interface alternative setting.
        (void)MUSB_ScheduleIsochTransfer(0, &(pAudDevice->OutIsochIrp));
        return;
    } 

    if (MUSB_AUD_STATE_SETINFREQ == pAudDevice->bState)
    {
        pAudDevice->bNextState = MUSB_AUD_STATE_RECORDING;
        // Set endpoint control: sampling frequency control.
        (void)MGC_AudSetAttr(pAudDevice, 
            (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_ENDPOINT), /*bmRequestType*/
            MUSB_AC_SET_CUR, /* bRequest */ 
            (MUSB_AC_SAMPLING_FREQ_CONTROL<<8), 
            pAudDevice->pIsoInEnd->bEndpointAddress, 
            MUSB_AC_SAMPLING_FREQ_LENGTH, 
             (uint8_t *)&(pAudDevice->prIsoInParm->dwSamFreq));
        return;
    } 

    if (MUSB_AUD_STATE_RECORDING == pAudDevice->bState)
    {
        // Set interface alternative setting.
        (void)MUSB_ScheduleIsochTransfer(0, &(pAudDevice->InIsochIrp));
        return;
    } 

    if (MUSB_AUD_STATE_SETOUTVOL == pAudDevice->bState)
    {
        pFeature = pAudDevice->pOtFeature;
        bCtrlIdx = pAudDevice->bOtCtrlIdx + 1;
        
        while(bCtrlIdx < pAudDevice->bOtCtrlNum)
        {
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                pAudDevice->bOtCtrlIdx = bCtrlIdx;
                pVolume = &pAudDevice->rOtVolume[bCtrlIdx];

                pVolume->wCur = (((pVolume->dwValueField*pVolume->wUsrCur)/100)
                    + pVolume->wMin) & 0xFFFF;                
                pVolume->wCur = (pVolume->wCur/pVolume->wRes) * pVolume->wRes;
                    
                (void)MGC_AudSetAttr(pAudDevice,
                    (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE), /*bmRequestType*/
                    MUSB_AC_SET_CUR, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wCur));
                return;
            }
            bCtrlIdx++;
        }
        return;
    } 

    if (MUSB_AUD_STATE_SETINVOL == pAudDevice->bState)
    {
        pFeature = pAudDevice->pItFeature;
        bCtrlIdx = pAudDevice->bItCtrlIdx + 1;
        
        while(bCtrlIdx < pAudDevice->bItCtrlNum)
        {
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                pAudDevice->bItCtrlIdx = bCtrlIdx;
                pVolume = &pAudDevice->rItVolume[bCtrlIdx];

                pVolume->wCur = (((pVolume->dwValueField*pVolume->wUsrCur)/100)
                    + pVolume->wMin) & 0xFFFF;                
                pVolume->wCur = (pVolume->wCur/pVolume->wRes) * pVolume->wRes;
                    
                (void)MGC_AudSetAttr(pAudDevice,
                    (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE), /*bmRequestType*/
                    MUSB_AC_SET_CUR, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wCur));
                return;
            }
            bCtrlIdx++;
        }
        return;
    }

    if (MUSB_AUD_STATE_SETOUTINF == pAudDevice->bState)
    {
        pAudDevice->bNextState = MUSB_AUD_STATE_UNMUTE;
        // set mute by setting alt0 of In interface.
        pAudDevice->pASInIface = pAudDevice->pASInIfaceHead;
        (void)MGC_AudSetInfAlt(pAudDevice, pAudDevice->pASInIface);
        return;
    }
    
    bInterfaceNumber = pAudDevice->pControlIfaceHead->bInterfaceNumber;

    // check next chanel when init end state.
    while((pAudDevice->bItCtrlIdx < pAudDevice->bItCtrlNum) ||
        (pAudDevice->bOtCtrlIdx < pAudDevice->bOtCtrlNum))
    {
        // Decide to control input feature.
        if (pAudDevice->bItCtrlIdx < pAudDevice->bItCtrlNum)
        {
            pFeature = pAudDevice->pItFeature;
            bCtrlIdx = pAudDevice->bItCtrlIdx;
            pVolume = &pAudDevice->rItVolume[bCtrlIdx];
        }
        else
        {
            // Decide to control output feature.    
            if (pAudDevice->bOtCtrlIdx < pAudDevice->bOtCtrlNum)
            {
                pFeature = pAudDevice->pOtFeature;
                bCtrlIdx = pAudDevice->bOtCtrlIdx;
                pVolume = &pAudDevice->rOtVolume[bCtrlIdx];
            }
        }
        MUSB_ASSERT(pFeature);
        MUSB_ASSERT(pVolume);

        // Un-Mute state.   
        if (MUSB_AUD_STATE_UNMUTE == pAudDevice->bState)
        {
            pAudDevice->bNextState = MUSB_AUD_STATE_GETMIN;
            
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_MUTE_CONTROL)
            {
                pVolume->bMute = 0;
                (void)MGC_AudSetAttr(pAudDevice, 
                    (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE), /*bmRequestType*/
                    MUSB_AC_SET_CUR, /* bRequest */ 
                    ((MUSB_AC_MUTE_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_MUTE_LENGTH, 
                    &pVolume->bMute);
                return;
            }
            else
            {
                pAudDevice->bState = MUSB_AUD_STATE_GETMIN;
            }
        }

        // Get Min volume state.   
        if (MUSB_AUD_STATE_GETMIN == pAudDevice->bState)
        {
            pAudDevice->bNextState = MUSB_AUD_STATE_GETMAX;
            
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                (void)MGC_AudGetAttr(pAudDevice, 
                    MUSB_AC_GET_MIN, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wMin));
                return;
            }
            else
            {
                pAudDevice->bState = MUSB_AUD_STATE_GETMAX;
            }
        }

        // Get Max volume state.   
        if (MUSB_AUD_STATE_GETMAX == pAudDevice->bState)
        {
            pAudDevice->bNextState = MUSB_AUD_STATE_GETRES;
            
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                (void)MGC_AudGetAttr(pAudDevice, 
                    MUSB_AC_GET_MAX, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wMax));
                return;            
            }
            else
            {
                pAudDevice->bState = MUSB_AUD_STATE_GETRES;
            }
        }

        // Get Res volume state.   
        if (MUSB_AUD_STATE_GETRES == pAudDevice->bState)
        {
            pAudDevice->bNextState = MUSB_AUD_STATE_GETCUR;
            
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                (void)MGC_AudGetAttr(pAudDevice, 
                    MUSB_AC_GET_RES, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wRes));
                return;            
            }
            else
            {
                pAudDevice->bState = MUSB_AUD_STATE_GETCUR;
            }
        }

        // Get Cur volume state.   
        if (MUSB_AUD_STATE_GETCUR == pAudDevice->bState)
        {
            pAudDevice->bNextState = MUSB_AUD_STATE_SETCUR;
            
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                (void)MGC_AudGetAttr(pAudDevice, 
                    MUSB_AC_GET_CUR, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wCur));
                return;            
            }
            else
            {
                pAudDevice->bState = MUSB_AUD_STATE_SETCUR;
            }
        }

        // Set Cur volume state.   
        if (MUSB_AUD_STATE_SETCUR == pAudDevice->bState)
        {           
            pAudDevice->bNextState = MUSB_AUD_STATE_INT_END;
            
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                if ((pVolume->wMax <= MUSB_AC_MAX_VOLUME) &&
                    (pVolume->wMin <= MUSB_AC_MAX_VOLUME))
                {
                    pVolume->dwValueField = pVolume->wMax - pVolume->wMin;
                }
                else if ((pVolume->wMax <= MUSB_AC_MAX_VOLUME) &&
                            (pVolume->wMin > MUSB_AC_MAX_VOLUME))
                {
                    pVolume->dwValueField = pVolume->wMax + (0x10000-pVolume->wMin);
                }
                else if ((pVolume->wMax > MUSB_AC_MAX_VOLUME) &&
                            (pVolume->wMin > MUSB_AC_MAX_VOLUME))
                {
                    pVolume->dwValueField = pVolume->wMax - pVolume->wMin;
                }

                // Default volume = 50% of loudness.
                pVolume->wUsrCur = 50; 
                pVolume->wCur = (((pVolume->dwValueField*pVolume->wUsrCur)/100)
                    + pVolume->wMin) & 0xFFFF;                
                pVolume->wCur = (pVolume->wCur/pVolume->wRes) * pVolume->wRes;

                (void)MGC_AudSetAttr(pAudDevice,
                    (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE), /*bmRequestType*/
                    MUSB_AC_SET_CUR, /* bRequest */ 
                    ((MUSB_AC_VOLUME_CONTROL<<8)|bCtrlIdx), 
                    ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
                    MUSB_AC_VOLUME_LENGTH, 
                    (uint8_t *)&(pVolume->wCur));
                return;            
            }
            else
            {
                pAudDevice->bState = MUSB_AUD_STATE_INT_END;
            }
        }

        // Init End state.   
        if (MUSB_AUD_STATE_INT_END == pAudDevice->bState)
        {
            pAudDevice->bState = MUSB_AUD_STATE_UNMUTE;
            
            if (pAudDevice->bItCtrlIdx < pAudDevice->bItCtrlNum)
            {
                bCtrlIdx = pAudDevice->bItCtrlIdx;
                pVolume = &pAudDevice->rItVolume[bCtrlIdx];
            
                LOG(5, "ItVol[%d]=%d%, Mute=%d,Min=0x%X,Max=0x%X,Res=0x%X,Cur=0x%X.\n", 
                    bCtrlIdx, pVolume->wUsrCur, pVolume->bMute, pVolume->wMin, 
                    pVolume->wMax, pVolume->wRes, pVolume->wCur);
                pAudDevice->bItCtrlIdx ++;
            }
            else
            {
                if (pAudDevice->bOtCtrlIdx < pAudDevice->bOtCtrlNum)
                {
                    bCtrlIdx = pAudDevice->bOtCtrlIdx;
                    pVolume = &pAudDevice->rOtVolume[bCtrlIdx];
                    LOG(5, "OtVol[%d]=%d%, Mute=%d,Min=0x%X,Max=0x%X,Res=0x%X,Cur=0x%X.\n", 
                        bCtrlIdx, pVolume->wUsrCur, pVolume->bMute, pVolume->wMin, 
                        pVolume->wMax, pVolume->wRes, pVolume->wCur);                
                    pAudDevice->bOtCtrlIdx ++;
                }
            }
        }
    }

    bFound = FALSE;
    for (i=0; i<MGC_AUD_DEVICE_NUM; i++)
    {
        if (!arAudDevice[i])
        {
            arAudDevice[i] = pAudDevice;
            bFound = TRUE;
            break;
        }
    }
    MUSB_ASSERT(bFound);                 

    // all channels are already set.
    pAudDevice->bItCtrlIdx = 0;
    pAudDevice->bOtCtrlIdx = 0;
    pAudDevice->bState = MUSB_AUD_STATE_READY;
    pAudDevice->bNextState = MUSB_AUD_STATE_FREE;
}

/**
 * Allocate per-device data
 */
static MGC_AudDevice *MGC_AudDeviceInit(void)
{
    MGC_AudDevice *pAudDevice;

    pAudDevice = (MGC_AudDevice *)MUSB_MemAlloc(sizeof(MGC_AudDevice));
    if (NULL == pAudDevice)
    {
        LOG(1, "AUD Err: Insufficient memory.\n", NULL);       
        return (NULL);
    }

    MUSB_MemSet((void *)pAudDevice, 0, sizeof(MGC_AudDevice));
    pAudDevice->pDriver = &MGC_AudDeviceDriver;

    return pAudDevice;
}

static void MGC_AudTimerExpired(void *pControllerPrivateData, uint16_t wTimerIndex)
{
    MGC_Port *pPort;
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    MGC_AudDevice *pAudDevice;
    
    MUSB_ASSERT(pControllerPrivateData);
    MUSB_ASSERT(wTimerIndex == MUSB_AUD_TIMER_IDX);

    pPort = (MGC_Port *)pControllerPrivateData;
    MUSB_ASSERT(pPort->pController);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_AUDIO) &&
            (pDevice->pDriverPrivateData))
        {
            // temp setting.
            pDevice->bSupport = MUSB_STATUS_CHILD_CONNECT;
        
            pAudDevice = (MGC_AudDevice *)pDevice->pDriverPrivateData;    

            LOG(1, "AUD: Start to set interface: 0x%08X.\n", (uint32_t)pAudDevice);

            pAudDevice->bNextState = MUSB_AUD_STATE_SETOUTINF;
            // set mute at out interface.
            pAudDevice->pASOutIface = pAudDevice->pASOutIfaceHead;
            (void)MGC_AudSetInfAlt(pAudDevice, pAudDevice->pASOutIface);
        }
    }
}

/** Callback function when device acknowledges set config reqeust. */
static void MGC_AudSetConfigCallback(void *pContext, MUSB_ControlIrp *pControlIrp)
{
    MGC_AudDevice *pAudDevice;
    MGC_Port *pPort;
    MUSB_SystemServices *pServices;

    MUSB_ASSERT(pContext);
    MUSB_ASSERT(pControlIrp);
   
    if (MUSB_STATUS_OK != pControlIrp->dwStatus)
    {
        LOG(1, "AUD Err: Set Config Callback Status = 0x%X.\n", pControlIrp->dwStatus);
        return;
    }

    pAudDevice = (MGC_AudDevice *)pContext;

    /* Since IRP is executed properly hence update hub state to its next state.
     */
    pAudDevice->bState = pAudDevice->bNextState;
    
    /* Set the Current Configuration Descriptor to Default as Set Config is Success */
    pAudDevice->pUsbDevice->pCurrentConfiguration = 
        pAudDevice->pUsbDevice->apConfigDescriptors[0];

    /* Device is connected */
    LOG(5, "AUD HubPort-%d: Aud Device = 0x%08X Config OK.\n", 
        pAudDevice->pUsbDevice->bHubPort, (uint32_t)pAudDevice->pUsbDevice);

    MUSB_ASSERT(pAudDevice->hBus);
    pPort = (MGC_Port *)pAudDevice->hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    

    //Timer Index MUSB_AUD_TIMER_IDX is reserved for Mass storage class driver.
    /* kill timer */
    (void)pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_AUD_TIMER_IDX/*bTimerIndex*/);
    
    (void)pServices->pfArmTimer(pServices->pPrivateData, 
        MUSB_AUD_TIMER_IDX/*bTimerIndex*/, 
        MGC_AUD_CONFIG_TIME/*dwTime*/, FALSE, MGC_AudTimerExpired, pPort);    
}

/** After getting connect callback,  device driver calls this function to configure device */
static uint32_t MGC_AudConfigureDevice(MGC_AudDevice *pAudDevice)
{
    MUSB_DeviceRequest *pSetup;
    MUSB_ControlIrp *pControlIrp;
    uint32_t dwStatus;

    MUSB_ASSERT(pAudDevice);
    
    pSetup = (MUSB_DeviceRequest *)pAudDevice->aSetupTx;
    pControlIrp = &(pAudDevice->ControlIrp);

    /** Prepare the Setup Packet for sending Set Config Request */
    MGC_PREPARE_SETUP_PACKET(pSetup, 
        (MUSB_DIR_OUT | MUSB_TYPE_STANDARD | MUSB_RECIP_DEVICE),
        MUSB_REQ_SET_CONFIGURATION, 
        pAudDevice->pConfig->bConfigurationValue,
        0, 
        0);
    
    /** Fill Control Irp */
    MGC_FILL_CONTROL_IRP(pAudDevice, 
        pControlIrp, 
        (uint8_t *)pSetup, 
        8,  /* Only setup phase data */ 
        NULL, 
        0, 
        MGC_AudSetConfigCallback);

    dwStatus = MUSB_StartControlTransfer(pAudDevice->pUsbDevice->pPort, pControlIrp);
    if (dwStatus)
    {
        /* Log an Error and return state */
        LOG(1, "AUD Err: Set Configuration Request failed = 0x%X.\n", dwStatus);
    }

    return (dwStatus);
} /* End MGC_MsdConfigureDevice () */

static void MGC_AudIsoOutComplete(void *pPrivateData, MUSB_IsochIrp *pIrp)
{
    MGC_AudDevice *pAudDevice;
   
    LOG(5, "AUD MGC_AudIsoOutComplete.\n", NULL);

    MUSB_ASSERT(pPrivateData);
    MUSB_ASSERT(pIrp);         

    pAudDevice = (MGC_AudDevice *)pPrivateData;

    pAudDevice->bState = MUSB_AUD_STATE_READY;
    pAudDevice->bNextState = MUSB_AUD_STATE_READY;    
}

static void MGC_AudIsoInComplete(void *pPrivateData, MUSB_IsochIrp *pIrp)
{
    MGC_AudDevice *pAudDevice;
   
    LOG(5, "AUD MGC_AudIsoInComplete.\n", NULL);

    MUSB_ASSERT(pPrivateData);
    MUSB_ASSERT(pIrp);         

    pAudDevice = (MGC_AudDevice *)pPrivateData;

    pAudDevice->bState = MUSB_AUD_STATE_READY;
    pAudDevice->bNextState = MUSB_AUD_STATE_READY;
}

uint16_t MUSB_FillAudClassPeripheralList(uint8_t *pList, uint16_t wListLength)
{
    uint16_t wResult = MUSB_MIN((uint16_t)sizeof(MGC_aAudPeripheralList), wListLength);

    MUSB_MemCopy(pList, MGC_aAudPeripheralList, wResult);
    return wResult;
}

MUSB_DeviceDriver *MUSB_GetAudClassDriver()
{
    return &MGC_AudDeviceDriver;
}

/** This function is called when Mass Storage device is connected.*/
uint8_t MUSB_AudConnect(void *pPrivateData, MUSB_BusHandle hBus, MUSB_Device *pUsbDevice,
                        const uint8_t *pPeripheralList)
{
    MGC_AudDevice *pAudDevice;
    uint8_t bStatus = FALSE;
    uint8_t bIndex;
    MUSB_InterfaceDescriptor *pIface;
    MUSB_ConfigurationDescriptor *pConfig;
    MUSB_DeviceEndpoint rEnd;
    MUSB_EndpointDescriptor *pEnd = NULL;
    
    MUSB_ASSERT(pUsbDevice);
    pConfig = (MUSB_ConfigurationDescriptor *)pUsbDevice->apConfigDescriptors[0];
    
    UNUSED(pPrivateData);
    UNUSED(pPeripheralList);
    
    /* Device is connected */
    LOG(5, "AUD Device Connected.\n", NULL);

    pAudDevice = MGC_AudDeviceInit();
    if (!pAudDevice)
    {
        LOG(5, "AUD init fail.\n", NULL);
        return bStatus;
    }    
    pAudDevice->pConfig = pConfig;
    
    /* find first interface with supported subclass/protocol combination */
    for (bIndex = 0; bIndex < pConfig->bNumInterfaces; bIndex++)
    {
        pIface = (MUSB_InterfaceDescriptor *)
            MUSB_FindInterfaceDescriptor(pConfig, bIndex, 0);

        if (pIface && (MUSB_CLASS_AUDIO == pIface->bInterfaceClass))
        {
            switch(pIface->bInterfaceSubClass)
            {
                case MUSB_AUDIOCONTROL:
                pAudDevice->pControlIfaceHead = pIface;
                MGC_AudParseCtrlIface(pAudDevice);
                break;

                case MUSB_AUDIOSTREAMING:
                // Always out interface first.
                if (!pAudDevice->pASOutIfaceHead)
                {
                    pAudDevice->pASOutIfaceHead = pIface;
                }
                else
                {
                    pAudDevice->pASInIfaceHead = pIface;
                }               
                break;

                default:
                break;
            }
        }
    }

    if ((!pAudDevice->pControlIfaceHead) ||
        (!pAudDevice->pASOutIfaceHead) ||
        (!pAudDevice->pASInIfaceHead))
    {
        LOG(5, "AUD initerface fail.\n", NULL);
        return bStatus;
    }

    pUsbDevice->pDriverPrivateData = pAudDevice;

    /* Initialize */
    pAudDevice->pUsbDevice = pUsbDevice;
    pAudDevice->hBus = hBus;

    pAudDevice->bState = MUSB_AUD_STATE_CONNECT;
    pAudDevice->bNextState = MUSB_AUD_STATE_CONFIGURED;

    /*Open out pipe*/
    MGC_AudGetASEp(pAudDevice->pASOutIfaceHead, 
        &MGC_rAudDefaultPlayParm, &pEnd, NULL, NULL);
    rEnd.pDevice = pUsbDevice;
    MUSB_MemCopy(&(rEnd.UsbDescriptor), pEnd, sizeof(MUSB_EndpointDescriptor));
    rEnd.wNakLimit = 0;
    pAudDevice->hIsoOutPipe = MUSB_OpenPipe(pAudDevice->hBus, &rEnd, NULL);
    if (!pAudDevice->hIsoOutPipe)
    {
        LOG(1, "AUD: failed to open Out pipe", NULL);
        return FALSE;
    }

    /*Open in pipe*/
    MGC_AudGetASEp(pAudDevice->pASInIfaceHead, 
        &MGC_rAudDefaultPlayParm, &pEnd, NULL, NULL);
    rEnd.pDevice = pUsbDevice;
    MUSB_MemCopy(&(rEnd.UsbDescriptor), pEnd, sizeof(MUSB_EndpointDescriptor));
    rEnd.wNakLimit = 0;
    pAudDevice->hIsoInPipe = MUSB_OpenPipe(pAudDevice->hBus, &rEnd, NULL);
    if (!pAudDevice->hIsoInPipe)
    {
        LOG(1, "AUD: failed to open In pipe", NULL);
        return FALSE;
    }

    /* Configure the device */
    if (MUSB_STATUS_OK == MGC_AudConfigureDevice(pAudDevice))
    {
        bStatus = TRUE;                          
    }
    
    return (bStatus);
} /* End MUSB_MsdConnect() */

/** Disconnect Handler for Mass Storage Device Driver */
uint8_t MUSB_AudDisconnect(void *pPrivateData, MUSB_BusHandle hBus, MUSB_Device *pUsbDevice)
{
    MGC_AudDevice *pAudDevice;
    MGC_Port *pPort = NULL;
    MUSB_SystemServices *pServices = NULL;    
    MUSB_Device *pDevice = NULL;
    uint16_t wCount = 0;
    uint16_t wIndex = 0;
    uint8_t bStatus = FALSE;

    MUSB_ASSERT(pUsbDevice);
    pAudDevice = (MGC_AudDevice *)pUsbDevice->pDriverPrivateData;

    UNUSED(pPrivateData);

    /* Check against the USB device and bus handle */
    if ((!pAudDevice) || 
        (!hBus) || 
        (hBus != pAudDevice->hBus) || 
        (pUsbDevice != pAudDevice->pUsbDevice))
    {
        LOG(1, "AUD Err: Disconnect.\n", NULL);

        return FALSE;
    }

    for (wIndex=0; wIndex<MGC_AUD_DEVICE_NUM; wIndex++)
    {
        if (arAudDevice[wIndex] == pAudDevice)
        {
            arAudDevice[wIndex] = NULL;
            break;
        }
    }

    if (pAudDevice->hIsoInPipe)
    {
        MUSB_ClosePipe(pAudDevice->hIsoInPipe);
    }
    if (pAudDevice->hIsoOutPipe)
    {
        MUSB_ClosePipe(pAudDevice->hIsoOutPipe);
    }

    MUSB_MemFree(pAudDevice);

    MUSB_ASSERT(hBus);
    pPort = (MGC_Port *)hBus;
    MUSB_ASSERT(pPort->pController);
    pServices = pPort->pController->pSystemServices;
    MUSB_ASSERT(pServices);    
    
    //Timer Index MUSB_MSD_CONF_TIMER_IDX is reserved for Mass storage class driver.
    /* kill timer */
    bStatus = pServices->pfCancelTimer(pServices->pPrivateData, 
        MUSB_AUD_TIMER_IDX/*bTimerIndex*/);
    UNUSED(bStatus);

    wCount = MUSB_ListLength(&(pPort->ConnectedDeviceList));

    // Check if any device is not handled by pProtocol->pfStartDevice().    
    for (wIndex=0; wIndex < wCount; wIndex++)
    {
        pDevice = (MUSB_Device *)MUSB_ListFindItem(&(pPort->ConnectedDeviceList), wIndex);

        if ((pDevice != pUsbDevice) && 
            (pDevice->bSupport == MUSB_STATUS_CHILD_UNKNOW) &&        
            (pDevice->bDeviceClass == MUSB_CLASS_AUDIO))
        {
            bStatus = pServices->pfArmTimer(pServices->pPrivateData, 
                MUSB_AUD_TIMER_IDX/*bTimerIndex*/, 
                MGC_AUD_CONFIG_TIME/*dwTime*/, FALSE, MGC_AudTimerExpired, pPort);    

            MUSB_ASSERT(bStatus);               
            break;
        }
    }

    pUsbDevice->pDriverPrivateData = NULL;
    LOG(5, "AUD Disconnect Successfully.\n", NULL);

   return TRUE;
} /* End MUSB_MsdDisconnect () */

uint32_t MUSB_AudVolumeSet(uint32_t dwIndex, uint8_t bIsPlay, uint16_t wVolume)
{
    MGC_AudDevice *pAudDevice;
    MUSB_ACFeatureUnitDescriptor *pFeature = NULL;
    MUSB_ACVolume *pVolume = NULL;
    uint8_t bCtrlIdx = 0;
    uint8_t bSetCtrlIdx = 0;
    uint8_t bInterfaceNumber = 0;    
    uint32_t dwStatus;

    pAudDevice = arAudDevice[dwIndex];

    if ((dwIndex >= MGC_AUD_DEVICE_NUM) ||
         (pAudDevice == NULL) ||
         (wVolume > 100) ||
         (pAudDevice->bState < MUSB_AUD_STATE_READY))
    {
        return MGC_M_STATUS_ERROR;
    }

    pAudDevice = arAudDevice[dwIndex];
    bInterfaceNumber = pAudDevice->pControlIfaceHead->bInterfaceNumber;

    if (!bIsPlay)
    {
        // Recording path.
        pFeature = pAudDevice->pOtFeature;
        bCtrlIdx = 0;
        while(bCtrlIdx < pAudDevice->bOtCtrlNum)
        {
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                pAudDevice->rOtVolume[bCtrlIdx].wUsrCur = wVolume;
                if (!pVolume)
                {
                    pVolume = &pAudDevice->rOtVolume[bCtrlIdx];
                    pAudDevice->bNextState = MUSB_AUD_STATE_SETOUTVOL;            
                    pAudDevice->bOtCtrlIdx = bCtrlIdx;
                    bSetCtrlIdx = bCtrlIdx;
                }
            }
            bCtrlIdx++;
        }
    }
    else
    {
        // Playing path.
        pFeature = pAudDevice->pItFeature;
        bCtrlIdx = 0;
        while(bCtrlIdx < pAudDevice->bItCtrlNum)
        {
            if (pFeature->bmaControls[bCtrlIdx] & MUSB_AC_VOLUME_CONTROL)
            {
                pAudDevice->rItVolume[bCtrlIdx].wUsrCur = wVolume;
                if (!pVolume)
                {
                    pVolume = &pAudDevice->rItVolume[bCtrlIdx];                
                    pAudDevice->bNextState = MUSB_AUD_STATE_SETINVOL;                        
                    pAudDevice->bItCtrlIdx = bCtrlIdx;
                    bSetCtrlIdx = bCtrlIdx;                    
                }
            }
            bCtrlIdx++;
        }
    }

    if (!pVolume)
    {
        LOG(5, "AUD: Not support volume.\n", NULL);    
        return MGC_M_STATUS_ERROR;
    }

    pVolume->wCur = (((pVolume->dwValueField*pVolume->wUsrCur)/100)
        + pVolume->wMin) & 0xFFFF;                
    pVolume->wCur = (pVolume->wCur/pVolume->wRes) * pVolume->wRes;
        
    LOG(5, "AUD: Set %s Volume = %d%, wCur=0x%X.\n", 
        ((bIsPlay) ? "Play": "Rec" ), wVolume, pVolume->wCur);
        
    dwStatus = MGC_AudSetAttr(pAudDevice,
        (MUSB_DIR_OUT| MUSB_TYPE_CLASS| MUSB_RECIP_INTERFACE), /*bmRequestType*/
        MUSB_AC_SET_CUR, /* bRequest */ 
        ((MUSB_AC_VOLUME_CONTROL<<8)|bSetCtrlIdx), 
        ((pFeature->bUnitID<<8)|(bInterfaceNumber)), 
        MUSB_AC_VOLUME_LENGTH, 
        (uint8_t *)&(pVolume->wCur));
    if (MUSB_STATUS_OK != dwStatus)
    {
        return MGC_M_STATUS_ERROR;
    }

    return MUSB_STATUS_OK;
}

uint32_t MUSB_AudStreamSet(uint32_t dwIndex, 
    MUSB_AUDParam *prPlayParm, MUSB_AudStreamIrp *prAudStreamIrp)
{
    MGC_AudDevice *pAudDevice;
    MUSB_InterfaceDescriptor *pASOutIfaceHead;
    MUSB_EndpointDescriptor *pOutEnd = NULL;
    MUSB_ASDataEndpointDescriptor *pASOutEnd = NULL;       
    MUSB_InterfaceDescriptor *pAltIface = NULL;
    MGC_Pipe *pPipe;
    uint16_t wMaxPacketSize;
    uint32_t dwStatus;

    pAudDevice = arAudDevice[dwIndex];

    if ((dwIndex >= MGC_AUD_DEVICE_NUM) ||
         (pAudDevice == NULL) ||
         (prPlayParm == NULL) ||
         (prAudStreamIrp == NULL) ||
         (pAudDevice->bState < MUSB_AUD_STATE_READY))
    {
        return MGC_M_STATUS_ERROR;
    }

    pASOutIfaceHead = pAudDevice->pASOutIfaceHead;

    // Match PCM, channels, sample per bits, sample frequency.
    MGC_AudGetASEp(pASOutIfaceHead, 
        prPlayParm, &pOutEnd, &pASOutEnd, &pAltIface);
    if ((!pOutEnd) ||(!pASOutEnd) || (!pAltIface))
    {
        LOG(1, "AUD: (1) MGC_AudGetASEp fail.\n", NULL);
        return MGC_M_STATUS_ERROR;
    }

    pAudDevice->pIsoOutEnd = pOutEnd;
    pAudDevice->pIsoASOutEnd = pASOutEnd;
    
    // Prepare ISO out IRP.
    MUSB_MemSet(&pAudDevice->OutIsochIrp, 0, sizeof(MUSB_IsochIrp));    

    // Some sound card will use single endpoint to support multiple sampling frequency.
    // We must calculate a suitable maxpktsize to support streaming.
    wMaxPacketSize = ((prPlayParm->dwSamFreq + (1000-1))/1000)*
        (prPlayParm->bBitResolution/8)*(prPlayParm->bNrChannels);
    wMaxPacketSize = MUSB_MIN(wMaxPacketSize, pOutEnd->wMaxPacketSize);
  
    MUSB_ASSERT(wMaxPacketSize > 0);
    
    LOG(5, "AUD: ISO Tx MaxPktSize = %d, Interval=%d.\n", 
        wMaxPacketSize, pOutEnd->bInterval);

    pAudDevice->prIsoOutParm = prPlayParm;
        
    // Modify wPacketSize.
    pPipe = (MGC_Pipe *)pAudDevice->hIsoOutPipe;
    MUSB_ASSERT(pPipe);
    MUSB_ASSERT(pPipe->pLocalEnd);
    if (pPipe->pLocalEnd->wPacketSize != wMaxPacketSize)
    {
        pPipe->pLocalEnd->wPacketSize = wMaxPacketSize;    
    }    
        
    pAudDevice->OutIsochIrp.hPipe = pAudDevice->hIsoOutPipe;
    pAudDevice->OutIsochIrp.pBuffer = prAudStreamIrp->pBuffer;
    pAudDevice->OutIsochIrp.dwLength = wMaxPacketSize;
    pAudDevice->OutIsochIrp.dwBufSize = prAudStreamIrp->dwBufSize;
    pAudDevice->OutIsochIrp.dwThreshold = prAudStreamIrp->dwThreshold;
    pAudDevice->OutIsochIrp.prIsoPtr = 
        (MUSB_IsoPtr *)((uint32_t)prAudStreamIrp->prAudIsoPtr);
    pAudDevice->OutIsochIrp.pfIrpComplete = MGC_AudIsoOutComplete;
    pAudDevice->OutIsochIrp.pCompleteParam = pAudDevice;

    if (pASOutEnd->bmAttributes & MUSB_BM_SAMPLING_FREQ_MASK)
    {
        pAudDevice->bNextState = MUSB_AUD_STATE_SETOUTFREQ;
    }
    else
    {
        LOG(5, "AS Endpoint not support sampling freq control.\n", NULL); 
        pAudDevice->bNextState = MUSB_AUD_STATE_PLAYING;
    }    
   
    // Set interface alternative setting.
    dwStatus = MGC_AudSetInfAlt(pAudDevice, pAltIface);
    if (MUSB_STATUS_OK != dwStatus)
    {
        return MGC_M_STATUS_ERROR;
    }

    UNUSED(prAudStreamIrp);
    return MUSB_STATUS_OK;
}

uint32_t MUSB_AudStreamGet(uint32_t dwIndex, 
    MUSB_AUDParam *prPlayParm, MUSB_AudStreamIrp *prAudStreamIrp)
{
    MGC_AudDevice *pAudDevice;
    MUSB_InterfaceDescriptor *pASInIfaceHead;
    MUSB_EndpointDescriptor *pInEnd = NULL;
    MUSB_ASDataEndpointDescriptor *pASInEnd = NULL;           
    MUSB_InterfaceDescriptor *pAltIface = NULL;
    MGC_Pipe *pPipe;    
    uint16_t wMaxPacketSize;
    uint32_t dwStatus;

    pAudDevice = arAudDevice[dwIndex];

    if ((dwIndex >= MGC_AUD_DEVICE_NUM) ||
         (pAudDevice == NULL) ||
         (prPlayParm == NULL) ||
         (prAudStreamIrp == NULL) ||
         (pAudDevice->bState < MUSB_AUD_STATE_READY))
    {
        return MGC_M_STATUS_ERROR;
    }

    pASInIfaceHead = pAudDevice->pASInIfaceHead;

    // Match PCM, channels, sample per bits, sample frequency.
    MGC_AudGetASEp(pASInIfaceHead, 
        prPlayParm, &pInEnd, &pASInEnd, &pAltIface);
    if ((!pInEnd) || (!pASInEnd) || (!pAltIface))
    {
        LOG(1, "AUD: (2) MGC_AudGetASEp fail.\n", NULL);
        return MGC_M_STATUS_ERROR;
    }

    pAudDevice->pIsoInEnd = pInEnd;
    pAudDevice->pIsoASInEnd = pASInEnd;

    // Prepare ISO out IRP.
    MUSB_MemSet(&pAudDevice->InIsochIrp, 0, sizeof(MUSB_IsochIrp));    

    LOG(5, "AUD: ISO Rx MaxPktSize = %d, Interval=%d.\n", 
        pInEnd->wMaxPacketSize, pInEnd->bInterval);

    wMaxPacketSize = pInEnd->wMaxPacketSize;
    MUSB_ASSERT(wMaxPacketSize > 0);

    pAudDevice->prIsoInParm = prPlayParm;

    // Modify wPacketSize.
    pPipe = (MGC_Pipe *)pAudDevice->hIsoInPipe;
    MUSB_ASSERT(pPipe);
    MUSB_ASSERT(pPipe->pLocalEnd);
    if (pPipe->pLocalEnd->wRxPacketSize != wMaxPacketSize)
    {
        pPipe->pLocalEnd->wRxPacketSize = wMaxPacketSize;    
    }    
    
    pAudDevice->InIsochIrp.hPipe = pAudDevice->hIsoInPipe;
    pAudDevice->InIsochIrp.pBuffer = prAudStreamIrp->pBuffer;
    pAudDevice->InIsochIrp.dwLength = wMaxPacketSize;
    pAudDevice->InIsochIrp.dwBufSize = prAudStreamIrp->dwBufSize;
    pAudDevice->InIsochIrp.dwThreshold = prAudStreamIrp->dwThreshold;
    pAudDevice->InIsochIrp.prIsoPtr = 
        (MUSB_IsoPtr *)((uint32_t)prAudStreamIrp->prAudIsoPtr);    
    pAudDevice->InIsochIrp.pfIrpComplete = MGC_AudIsoInComplete;
    pAudDevice->InIsochIrp.pCompleteParam = pAudDevice;

    if (pASInEnd->bmAttributes & MUSB_BM_SAMPLING_FREQ_MASK)
    {
        pAudDevice->bNextState = MUSB_AUD_STATE_SETINFREQ;
    }
    else
    {
        LOG(5, "AS Endpoint not support sampling freq control.\n", NULL); 
        pAudDevice->bNextState = MUSB_AUD_STATE_RECORDING;
    }    
    
    // Set interface alternative setting.
    dwStatus = MGC_AudSetInfAlt(pAudDevice, pAltIface);
    if (MUSB_STATUS_OK != dwStatus)
    {
        return MGC_M_STATUS_ERROR;
    }

    UNUSED(prAudStreamIrp);
    return MUSB_STATUS_OK;
}

#endif /* MUSB_AUD */

