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
 * Top-level Hid class driver module
 * $Revision: #1 $
 */
#ifdef MUSB_HID

#include "mu_bits.h"
#include "mu_cdi.h"
#include "mu_descs.h"
#include "mu_diag.h"
#include "mu_mem.h"
#include "mu_stdio.h"
#include "mu_impl.h"

#include "mu_hid_if.h"
#include "mu_hid_descr.h"

static uint8_t MGC_bJoyStickInit = FALSE;

/* Joystick related object */
static MGC_HidJoystick MGC_arJoyStick[MGC_HID_JOYSTICK_NUM];

static MUSB_pfHidJoystickInsertNfy MGC_pfHidJoystckInsert;   
static MUSB_pfHidJoystickDataNfy MGC_pfHidJoystck;  

static MGC_HidJoystick *
MGC_JoystickGetIdPtr(MGC_HidDevice *pHidDevice, uint8_t bHidDevId)
{
    uint8_t bIndex;
    
    MUSB_ASSERT(pHidDevice);

    for(bIndex=0; bIndex<MGC_HID_JOYSTICK_NUM; bIndex++)
    {
        if ((MGC_arJoyStick[bIndex].bHidDevId == bHidDevId) && 
            (MGC_arJoyStick[bIndex].pHidDevice == pHidDevice))
        {
            return &MGC_arJoyStick[bIndex];
        }
    }
    
    return NULL;
}

void MUSB_JoystickInit(void)
{
    uint8_t bIndex = 0;

    if (MGC_bJoyStickInit)
    {
        return;
    }
    MGC_bJoyStickInit = TRUE;    

    for(bIndex=0; bIndex<MGC_HID_JOYSTICK_NUM; bIndex++)
    {
        MGC_arJoyStick[bIndex].bIndex = bIndex;
        MGC_arJoyStick[bIndex].pfHidJoystck = MGC_pfHidJoystck;
    }
}

void MUSB_JoystickRegister(MGC_HidDevice *pHidDevice)
{
    REPORT_STRUCT *pReport;
    uint8_t bIndex;
    uint8_t bIdCount = 0;    
    
    MUSB_ASSERT(pHidDevice);
    // Handle input report at interrupt In handler.
    pReport = &pHidDevice->rReport[MUSB_HID_INPUT_REPORT];
    MUSB_ASSERT(pReport->bTotalId >= 1);

    for(bIndex=0; bIndex<MGC_HID_JOYSTICK_NUM; bIndex++)
    {
        if ((bIdCount < pReport->bTotalId) && 
            (MGC_arJoyStick[bIndex].pHidDevice == NULL))
        {
            MGC_arJoyStick[bIndex].bHidDevId = bIdCount;
            MGC_arJoyStick[bIndex].pHidDevice = pHidDevice;

            // Notify upper layer a new joystick is inserted.
            if (MGC_pfHidJoystckInsert)
            {
                MGC_pfHidJoystckInsert(bIndex, TRUE);
            }
            /*Register the start */
            bIdCount ++;
        }
    }

    pHidDevice->pHidDriverTag = MGC_arJoyStick;
    return;
}

void MUSB_JoystickRelease(MGC_HidDevice *pHidDevice)
{
    uint8_t bIndex;
    
    MUSB_ASSERT(pHidDevice);

    for(bIndex=0; bIndex<MGC_HID_JOYSTICK_NUM; bIndex++)
    {    
        if (MGC_arJoyStick[bIndex].pHidDevice == pHidDevice)
        {
            // Notify upper layer a new joystick is removed.
            if (MGC_pfHidJoystckInsert)
            {
                MGC_pfHidJoystckInsert(bIndex, FALSE);
            }

            MGC_arJoyStick[bIndex].bHidDevId = 0;        
            MGC_arJoyStick[bIndex].pHidDevice = NULL;            
        }
    }

    pHidDevice->pHidDriverTag = NULL;
    return;
}

uint32_t MUSB_JoystickCount(void)
{
    uint8_t bIndex;
    uint8_t bCount = 0;    

    for(bIndex=0; bIndex<MGC_HID_JOYSTICK_NUM; bIndex++)
    {
        if (MGC_arJoyStick[bIndex].pHidDevice != NULL)
        {
            bCount ++;
        }
    }

    return bCount;
}

void *MUSB_JoystickGetData(uint8_t bIndex, void *pBuf)
{
    JOYSTICK_INFO_STRUCT *pJoyStick;

    if ((!MGC_bJoyStickInit) ||(bIndex >=MGC_HID_JOYSTICK_NUM))
    {
        return NULL;
    }

    if (MGC_arJoyStick[bIndex].pHidDevice == NULL)
    {
        return NULL;
    }

    pJoyStick = &MGC_arJoyStick[bIndex].rJoyStick;
    
    MUSB_MemCopy(pBuf, (void *)pJoyStick, sizeof(JOYSTICK_INFO_STRUCT));

    return pBuf;
}

void MUSB_JoystickSetInsertNotify(void *pBuf)
{
    MUSB_ASSERT(pBuf);

    MGC_pfHidJoystckInsert = (MUSB_pfHidJoystickInsertNfy)((uint32_t)pBuf);

    return;
}

void MUSB_JoystickSetDataNotify(void *pBuf)
{
    uint8_t bIndex = 0;

    MUSB_ASSERT(pBuf);
    MGC_pfHidJoystck = (MUSB_pfHidJoystickDataNfy)((uint32_t)pBuf);

    for(bIndex=0; bIndex<MGC_HID_JOYSTICK_NUM; bIndex++)
    {
        MGC_arJoyStick[bIndex].pfHidJoystck = MGC_pfHidJoystck;
    }   

    return;
}

uint32_t MUSB_JoystickEvent(MGC_HidDevice *pHidDevice)
{
    uint8_t *pInBuf = NULL;
    uint32_t dwOffset = 0; /*bit offset*/
    uint32_t index = 0;
    uint32_t i; 
    uint32_t j;
    REPORT_STRUCT *pReport;
    ID_REPORT_STRUCT *pIdReport;
    BASIC_REPORT_STRUCT *pBasicReport = NULL;
    MGC_HidJoystick *pHidJoyStick;
    JOYSTICK_INFO_STRUCT *pJoyStick;
    LOCAL_STATE_TABLE_STRUCT *pLocalItem;    
    uint8_t bChange = FALSE;
    uint8_t bOffset;
    uint8_t bBits;
    uint32_t usage = 0;
    uint32_t X = 0;
    uint32_t Y = 0;    
    uint32_t Button = 0; /* Max 32 buttons */

    MUSB_ASSERT(pHidDevice);
    // Handle input report at interrupt In handler.
    pReport = &pHidDevice->rReport[MUSB_HID_INPUT_REPORT];
    pInBuf = pHidDevice->intrInIrp.pBuffer;

    if (pReport->bTotalId > 1)
    {
        index = pInBuf[0];
        dwOffset += 8;
    }
    else
    {
        index = 1;
    }

    MUSB_ASSERT(index >= 1);
    MUSB_ASSERT(index <= MAX_ID_ALLOWED);

    // Get id report.    
    pIdReport = pReport->pIdReport[index -1];
    MUSB_ASSERT(pIdReport);

    // Get related joystick data structure.    
    pHidJoyStick = MGC_JoystickGetIdPtr(pHidDevice, (index -1));
    MUSB_ASSERT(pHidJoyStick);
    pJoyStick = &pHidJoyStick->rJoyStick;
    pHidDevice->bHidDriverNum = pReport->bTotalId;
    
    for (i=0; i < pIdReport->dwBasicReportNum; i++)
    {
        pBasicReport = pIdReport->pBasicReport[i];
        MUSB_ASSERT(pBasicReport);
        pLocalItem = &pBasicReport->LocalItem;

        if (pBasicReport->usage_page == USAGE_PAGE_GENERIC_DESKTOP)
        {
            if (pLocalItem->bUsage)
            {
                // Usage list is using. Check usage array and num.
                MUSB_ASSERT(pBasicReport->report_count == pLocalItem->usage_num);
                
                for (j=0; j < pBasicReport->report_count; j++)
                {
                    if (GET_USAGETAG(pLocalItem->usage[j]) == GDP_USAGE_X)                        
                    {
                        if (pJoyStick->bXYMax == 0)
                        {
                            pJoyStick->bXYMin = pBasicReport->logical_min;
                            pJoyStick->bXYMax = pBasicReport->logical_max;
                            LOG(5, "Joystick-%d: XY min=%d, max=%d.\n", pBasicReport->report_id, 
                                pBasicReport->logical_min, pBasicReport->logical_max);
                        }

                        index = dwOffset/8;
                        if (index >= pHidDevice->intrInIrp.dwActualLength)
                        {
                            return MGC_M_STATUS_ERROR;                                
                        }
                        bOffset = dwOffset % 8;
                        bBits = (uint8_t)pBasicReport->report_size;
                        X = MUSB_GetBitsFromByteStream(&pInBuf[index], bOffset, bBits);
                        dwOffset += bBits;
                    }
                    else if (GET_USAGETAG(pLocalItem->usage[j]) == GDP_USAGE_Y)
                    {
                        index = dwOffset/8;
                        if (index >= pHidDevice->intrInIrp.dwActualLength)
                        {
                            return MGC_M_STATUS_ERROR;                                
                        }                    
                        bOffset = dwOffset % 8;
                        bBits = (uint8_t)pBasicReport->report_size;
                        Y = MUSB_GetBitsFromByteStream(&pInBuf[index], bOffset, bBits);
                        dwOffset += bBits;
                    }
                    else
                    {
                        // Skip unknow item.
                        bBits = (uint8_t)(pBasicReport->report_size*pBasicReport->report_count);
                        dwOffset += bBits;
                    }                    
                }

            }
            else /* if (pLocalItem->bUsage) */
            {
                // Usage_min and usage_max is using.
                usage = GET_USAGETAG(pLocalItem->usage_min);
                while (usage < GET_USAGETAG(pLocalItem->usage_max))
                {
                    if (usage == GDP_USAGE_X)                        
                    {
                        if (pJoyStick->bXYMax == 0)
                        {
                            pJoyStick->bXYMin = pBasicReport->logical_min;
                            pJoyStick->bXYMax = pBasicReport->logical_max;
                            LOG(5, "Joystick-%d: XY min=%d, max=%d.\n", pBasicReport->report_id, 
                                pBasicReport->logical_min, pBasicReport->logical_max);
                        }

                        index = dwOffset/8;
                        if (index >= pHidDevice->intrInIrp.dwActualLength)
                        {
                            return MGC_M_STATUS_ERROR;                                
                        }                                        
                        bOffset = dwOffset % 8;
                        bBits = (uint8_t)pBasicReport->report_size;
                        X = MUSB_GetBitsFromByteStream(&pInBuf[index], bOffset, bBits);
                        dwOffset += bBits;
                    }
                    else if (usage == GDP_USAGE_Y)
                    {
                        index = dwOffset/8;
                        if (index >= pHidDevice->intrInIrp.dwActualLength)
                        {
                            return MGC_M_STATUS_ERROR;                                
                        }                                                            
                        bOffset = dwOffset % 8;
                        bBits = (uint8_t)pBasicReport->report_size;
                        Y = MUSB_GetBitsFromByteStream(&pInBuf[index], bOffset, bBits);
                        dwOffset += bBits;
                    }
                    else
                    {
                        // Skip unknow item.
                        bBits = (uint8_t)(pBasicReport->report_size*pBasicReport->report_count);
                        dwOffset += bBits;
                    }
                    // check next data.
                    usage ++;
                }
            }/* if (pLocalItem->bUsage) */
        }
        else if (pBasicReport->usage_page == USAGE_PAGE_BUTTON)
        {       
            MUSB_ASSERT(pBasicReport->report_size == 1);
            
            // Usage_min and usage_max is using.
            if (pJoyStick->bButtonBitsNum== 0)
            {
                pJoyStick->bButtonBitsNum = pBasicReport->report_count;
                LOG(5, "Joystick-%d: Button id=%d ~ %d.\n", pBasicReport->report_id, 
                    GET_USAGETAG(pLocalItem->usage_min), 
                    GET_USAGETAG(pLocalItem->usage_max));
                LOG(5, "Joystick-%d: Button bit=%d, count=%d.\n", 
                    pBasicReport->report_id, pBasicReport->report_size, pBasicReport->report_count);
            }

            index = dwOffset/8;
            if (index >= pHidDevice->intrInIrp.dwActualLength)
            {
                return MGC_M_STATUS_ERROR;                                
            }                                                            

            bOffset = dwOffset % 8;
            bBits = (uint8_t)(pBasicReport->report_size*pBasicReport->report_count);
            Button = MUSB_GetBitsFromByteStream(&pInBuf[index], bOffset, bBits);
            dwOffset += bBits;
        }
        else
        {
            // Treat as a padding bits.
            bBits = (uint8_t)(pBasicReport->report_size*pBasicReport->report_count);
            dwOffset += bBits;
        }                
    }

    // update to user's structure.
    if (X != pJoyStick->dwX)
    {
        pJoyStick->dwX = X;
        bChange = TRUE;
    }
    if (Y != pJoyStick->dwY)
    {
        pJoyStick->dwY = Y;
        bChange = TRUE;
    }
    if (Button != pJoyStick->dwButton)
    {
        pJoyStick->dwButton = Button;
        bChange = TRUE;        
    }

    if (!pBasicReport)
    {
        return MGC_M_STATUS_ERROR;
    }

    if (bChange)
    {
        pJoyStick->Index ++;
        LOG(7, "Joystick-%d-%d: X=%d, Y=%d, Button=0x%X.\n", 
            pBasicReport->report_id, pJoyStick->Index,
            pJoyStick->dwX, pJoyStick->dwY, pJoyStick->dwButton);

        if (pHidJoyStick->pfHidJoystck)
        {
            pHidJoyStick->pfHidJoystck(pHidJoyStick->bIndex, pJoyStick);
        }
    }
    
    return MUSB_STATUS_OK;    
}

#endif /* MUSB_HID */

