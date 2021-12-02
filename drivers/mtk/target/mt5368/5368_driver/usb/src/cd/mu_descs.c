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
 * Descriptor analysis library.
 * $Revision: #1 $
 */

#include "mu_descs.h"
#include "mu_arch.h"

/* 
 * Find an arbitrary descriptor
 */
const uint8_t *MUSB_FindDescriptor(const MUSB_ConfigurationDescriptor *pConfig,
                                   uint8_t bDescriptorType,
                                   uint8_t bIndex)
{
    uint16_t wBufferLength;

    int16_t wFoundIndex = -1;
    uint16_t wOffset = 0;
    const uint8_t *pBuffer;
    const MUSB_DescriptorHeader *pHeader;

    if (!pConfig)
    {
        return NULL;
    }

    pBuffer = (uint8_t *)pConfig;
    pHeader = (const MUSB_DescriptorHeader *)((uint32_t)pBuffer);

    if ((pConfig->bDescriptorType == MUSB_DT_CONFIG)
                        || (pConfig->bDescriptorType == MUSB_DT_OTHER_SPEED))
    {
        wBufferLength = pConfig->wTotalLength;
        /* handle trivial case */
        if ((pHeader->bDescriptorType == bDescriptorType) && !bIndex)
        {
            return (uint8_t *)pHeader;
        }

        /* general case */
        while ((wOffset < wBufferLength) && (wFoundIndex < (int16_t)bIndex))
        {
            pHeader = (const MUSB_DescriptorHeader *)((uint32_t)pBuffer + wOffset);

            MUSB_ASSERT(pHeader);

            if (pHeader->bLength < 1)
            {
                /* we're in the weeds */
                return NULL;
            }

            wOffset += pHeader->bLength;
            if (pHeader->bDescriptorType == bDescriptorType)
            {
                wFoundIndex++;
            }
        }
        if ((wOffset <= wBufferLength) && (wFoundIndex == (int16_t)bIndex))
        {
            return (uint8_t *)pHeader;
        }
    }

    return NULL;
}

#if 0
/*
 * Count alternate interfaces for each interface in a configuration
 */
uint8_t MUSB_CountAlternates(uint8_t *aCount, const MUSB_ConfigurationDescriptor *pConfig)
{
    const uint8_t *pDescriptor;

    const MUSB_InterfaceDescriptor *pIfaceDescriptor;
    uint8_t bAlternate = 0;
    uint8_t bInterface = 0;
    uint8_t bIndex = 0;
    uint8_t bResult = 0;

    /* sanity check */
    if (!aCount || !pConfig)
    {
        return 0;
    }

    /* find first interface */
    pDescriptor = MUSB_FindDescriptor(pConfig, MUSB_DT_INTERFACE, bIndex);

    /* count */
    while (pDescriptor && (bInterface < pConfig->bNumInterfaces))
    {
        bAlternate++;

        pIfaceDescriptor = (const MUSB_InterfaceDescriptor *)pDescriptor;

        if (pIfaceDescriptor->bInterfaceNumber > bInterface)
        {
            /* we jumped to next interface; fill current and move on */
            aCount[bResult++ ] = bAlternate - 1;

            bAlternate = 1;
            bInterface = pIfaceDescriptor->bInterfaceNumber;
        }
        pDescriptor = MUSB_FindDescriptor(pConfig, MUSB_DT_INTERFACE, ++bIndex);
    }

    if (bAlternate)
    {
        /* this should always happen for a properly-formed descriptor set */
        aCount[bResult++ ] = bAlternate;
    }

    return bResult;
}
#endif
/*
 * Find an interface descriptor in a configuration
 */

const MUSB_InterfaceDescriptor *MUSB_FindInterfaceDescriptor(
    const MUSB_ConfigurationDescriptor *pConfig, uint8_t bInterfaceNumber,
    uint8_t bAlternateSetting)
{
    uint16_t wBufferLength, wOffset;

    const MUSB_DescriptorHeader *pHeader;
    const MUSB_InterfaceDescriptor *pInterface = NULL;
    int16_t wFound = -1;

    if (pConfig && ((pConfig->bDescriptorType == MUSB_DT_CONFIG)
                        || (pConfig->bDescriptorType == MUSB_DT_OTHER_SPEED)))
    {
        wBufferLength = MUSB_SWAP16P((uint8_t *)&(pConfig->wTotalLength));

        wOffset = pConfig->bLength;
        /* search for the number */
        while ((wOffset < wBufferLength) && (wFound != (int16_t)bInterfaceNumber))
        {
            pHeader = (MUSB_DescriptorHeader *)((uint32_t)pConfig + wOffset);

            if (pHeader->bLength < 1)
            {
                /* we're in the weeds */
                return NULL;
            }

            wOffset += pHeader->bLength;
            if (pHeader->bDescriptorType == MUSB_DT_INTERFACE)
            {
                pInterface = (MUSB_InterfaceDescriptor *)((uint32_t)pHeader);
                wFound = (int16_t)pInterface->bInterfaceNumber;
            }
        }

        MUSB_ASSERT(pInterface);
        if ((wOffset == wBufferLength) && (wFound == (int16_t)bInterfaceNumber)
                && (pInterface->bAlternateSetting == bAlternateSetting))
        {
            return pInterface;
        }
        else if (wOffset < wBufferLength)
        {
            /* found number; now find alternate */
            wFound = -1;
            /* trivial case */
            if (!bAlternateSetting)
            {
                return pInterface;
            }
            /* general case */
            while ((wOffset < wBufferLength) && (wFound != (int16_t)bAlternateSetting))
            {
                pHeader = (MUSB_DescriptorHeader *)((uint32_t)pConfig + wOffset);

                if (pHeader->bLength < 1)
                {
                    /* we're in the weeds */
                    return NULL;
                }

                wOffset += pHeader->bLength;
                if (pHeader->bDescriptorType == MUSB_DT_INTERFACE)
                {
                    pInterface = (MUSB_InterfaceDescriptor *)((uint32_t)pHeader);
                    wFound = (int16_t)pInterface->bAlternateSetting;
                }
            }

            MUSB_ASSERT(pInterface);
            if (!pInterface)
            {
                return NULL;
            }
            if ((wOffset < wBufferLength) && (pInterface->bInterfaceNumber == bInterfaceNumber))
            {
                return pInterface;
            }
        }
    }

    return NULL;
}

/*
 * Find an endpoint descriptor in an interface
 */
const MUSB_EndpointDescriptor *MUSB_FindEndpointDescriptor(
    const MUSB_ConfigurationDescriptor *pConfig,
    const MUSB_InterfaceDescriptor *pInterfaceDescriptor,
    uint8_t bEndpointIndex)
{
    return (MUSB_EndpointDescriptor *)
               ((uint32_t)MUSB_FindDescriptorInBlock(pConfig, (const uint8_t *)pInterfaceDescriptor,
                                          MUSB_DT_ENDPOINT, bEndpointIndex));
}

/*
 * Find an arbitrary descriptor within a descriptor block.
 */
const uint8_t *MUSB_FindDescriptorInBlock(const MUSB_ConfigurationDescriptor *pConfig,
                                          const uint8_t *pDescriptorBlock,
                                          uint8_t bDescriptorType,
                                          uint8_t bIndex)
{
    uint8_t bLimitType;

    uint16_t wOffset;
    uint16_t wBufferLength;
    int16_t wFoundIndex = -1;
    const MUSB_DescriptorHeader *pHeader = (MUSB_DescriptorHeader *)((uint32_t)pDescriptorBlock);

    MUSB_ASSERT(pHeader);

    if (pConfig && ((pConfig->bDescriptorType == MUSB_DT_CONFIG)
                        || (pConfig->bDescriptorType == MUSB_DT_OTHER_SPEED)) && pDescriptorBlock)
    {
        bLimitType = pHeader->bDescriptorType;

        wOffset = pHeader->bLength;
        pHeader = (MUSB_DescriptorHeader *)((uint32_t)pDescriptorBlock + wOffset);
        wBufferLength = pConfig->wTotalLength - (uint16_t)((uint32_t)pDescriptorBlock - (uint32_t)pConfig);

        while ((wOffset < wBufferLength) && (pHeader->bDescriptorType != bLimitType)
                   && (wFoundIndex < (int16_t)bIndex))
        {
            pHeader = (MUSB_DescriptorHeader *)((uint32_t)pDescriptorBlock + wOffset);

            if (pHeader->bLength < 1)
            {
                /* we're in the weeds */
                return NULL;
            }

            wOffset += pHeader->bLength;
            if (pHeader->bDescriptorType == bDescriptorType)
            {
                wFoundIndex++;
            }
        }

        if ((wOffset < wBufferLength) && (pHeader->bDescriptorType != bLimitType))
        {
            return (uint8_t *)pHeader;
        }
        /* also the tail case */
        if ((wOffset == wBufferLength) && (wFoundIndex == (uint16_t)bIndex)
                && (pHeader->bDescriptorType == bDescriptorType))
        {
            return (uint8_t *)pHeader;
        }
    }

    return NULL;
}
