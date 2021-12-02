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

#define MGC_HID_NUM (GDP_USAGE_TABLETPCSYS)

static const CHAR *MGC_HidTypes[MGC_HID_NUM] = 
{"Device", "Pointer", "Mouse", "Device", "Joystick",
  "Gamepad", "Keyboard", "Keypad", "Multi-Axis Controller"};

static const uint8_t MGC_bHidSupport[MGC_HID_NUM] = 
{
    FALSE, /* "Device" */
    FALSE, /* "Pointer" */
    FALSE, /* "Mouse" */
    FALSE, /* "Device" */
    TRUE, /* "Joystick" */
    TRUE, /* "Gamepad" */
    FALSE, /* "Keyboard" */
    FALSE, /* "Keypad" */
    FALSE /* "Multi-Axis Controller" */
};

/******************************************************************************
 *Function    : MGC_DisplayGlobalItem()
 *Purpose     : Debuuging HID global items
 *Parameters  : global_state_table_P : Pointing to global items.
 *Called by   : Called diffrent HID routines
 *Returns     : None
 *****************************************************************************/
static void  
MGC_DisplayGlobalItem(uint32_t *pGlobalItem)
{
#ifdef MUSB_HID_DEBUG
    Printf("\nUsage_Page: 0x%X.\n", pGlobalItem[0]);

    Printf("logical_min: %d, logical_max: %d.\n", 
        pGlobalItem[1], pGlobalItem[2]);
    Printf("physical_min: %d, physical_max: %d.\n", 
        pGlobalItem[3], pGlobalItem[4]);

    Printf("unit_exponent: %d, unit: %d.\n", 
        pGlobalItem[5], pGlobalItem[6]);  

    Printf("report_size: %d, ", pGlobalItem[7]); 
    Printf("report_id: %d, ", pGlobalItem[8]);
    Printf("report_count: %d.\n", pGlobalItem[9]);
#else
    UNUSED(pGlobalItem);
#endif
}

/****************************************************************************
 *Function    : MGC_GetItemData()
 *Purpose     : To retrieve unsigned data.
 *Parameters  : pItem :: This contains item data.
 *Called by   : HID driver
 *Returns     : unsigned 32 bit value             
 ****************************************************************************/
static uint32_t MGC_GetItemData(ITEM_INFO_STRUCT *pItem)
{   
    MUSB_ASSERT(pItem);
    
    switch (pItem->bSize) 
    {
        case 1: return  (pItem->item_data & 0xFF);
        case 2: return  ( pItem->item_data & 0xFFFF);
        case 3: return  (pItem->item_data);
        default :return(0);
    }
}

/****************************************************************************
 *Function    : void MGC_UpdateBasicReportByGlobal()
 *Purpose     : This function update all the field related to global item in
 *              basic report structure.
 *Parameters  : pBasicReport:: Pointe to basic report
 *              global_state_table_P :: Pointer to the global items
 *Called by   : HID driver.
 *Returns     : none      
 *****************************************************************************/
static void 
MGC_UpdateBasicReportByGlobal( BASIC_REPORT_STRUCT * pBasicReport,
                          uint32_t *pGlobalItem)
{
    MUSB_ASSERT(pBasicReport);
    MUSB_ASSERT(pGlobalItem);
    
    pBasicReport->usage_page    = pGlobalItem[0];
    pBasicReport->logical_min   = pGlobalItem[1];
    pBasicReport->logical_max   = pGlobalItem[2];
    pBasicReport->physical_min  = pGlobalItem[3];
    pBasicReport->physical_max  = pGlobalItem[4];
    pBasicReport->unit_exponent = pGlobalItem[5];
    pBasicReport->unit          = pGlobalItem[6];
    pBasicReport->report_size   = pGlobalItem[7];
    pBasicReport->report_id     = pGlobalItem[8];
    pBasicReport->report_count  = pGlobalItem[9];
    return;
}
/****************************************************************************
 *Function    : MGC_UpdateBasicReportByLocal()
 *Purpose     : This function update all the field related to local item in
 *              basic report structure.
 *Parameters  : pBasicReport:: 
 *              pLocalItem ::
 *Called by   : HID driver
 *Returns     : none      
 *****************************************************************************/
static void 
MGC_UpdateBasicReportByLocal( BASIC_REPORT_STRUCT * pBasicReport,
                         LOCAL_STATE_TABLE_STRUCT *pLocalItem)
{
    MUSB_ASSERT(pBasicReport);
    MUSB_ASSERT(pLocalItem);

    MUSB_MemCopy((void *)&pBasicReport->LocalItem, 
        (void *)pLocalItem, sizeof(LOCAL_STATE_TABLE_STRUCT));
    return;
}

/****************************************************************************
 *Function    :MGC_ParseMainNonDataItem ()
 *Purpose     : This function handles non data items. Collection and
 *              end collection.
 *Parameters  : pItemStateTable:: Contains current state of items.
 *              main_data :: contains information about main item
 *Called by   : 
 *Returns     : MUSB_STATUS_OK
 *              MGC_M_STATUS_ERROR
 *****************************************************************************/
static uint8_t 
MGC_ParseMainNonDataItem(ITEM_STATE_TABLE_STRUCT *pItemStateTable,
                              uint32_t main_data, uint8_t begin_end_col)
{
    MGC_HidDevice *pHidDevice = NULL;
    COLLECTION_STRUCT *pCollection = NULL;
    COLLECTION_STRUCT *pParent     = NULL;
    LOCAL_STATE_TABLE_STRUCT *pLocalItem;    
    uint8_t status = MGC_M_STATUS_ERROR;
    uint16_t wUsage;

    MUSB_ASSERT(pItemStateTable);
    
    /*
     * Get HID device pointer.
     */
    pHidDevice = (MGC_HidDevice *)pItemStateTable->pHidDevice;
    pLocalItem = &(pItemStateTable->LocalItem);
    
    /*  Is item is begin collection? */
    if (begin_end_col == COLLECTION ) 
    {         
        if (main_data == MAIN_COLLECTION_APPLICATION)
        {
            wUsage = GET_USAGETAG(pLocalItem->usage[0]);
            /* Application usage normally only has one usage */
            if ((GET_USAGEPAGE(pLocalItem->usage[0]) == USAGE_PAGE_GENERIC_DESKTOP) &&
                (wUsage < 9))
            {
                LOG(5, "Hid type = %s.\n", MGC_HidTypes[wUsage]);                

                if (!pHidDevice->HidTypes)
                {   
                    pHidDevice->HidTypes = MGC_HidTypes[wUsage];
                    pHidDevice->bHidType = wUsage;
                }
                
                if (!MGC_bHidSupport[wUsage])
                {
                    LOG(5, "Hid type not support, wUsage = %d.\n", wUsage);
                    return MGC_M_STATUS_ERROR;
                }
            }
        }
            
        if (pHidDevice->bCollectionNum == MAX_COLLECTION_ALLOWED)
        {
            /* No space is available  to store collection hence change 
             * the config value and build the project and then run it.
             */
            return(MGC_M_STATUS_ERROR);
        }

        pCollection = &pHidDevice->aCollection[pHidDevice->bCollectionNum];
        pHidDevice->bCollectionNum++;
        
        MUSB_MemSet((void *)pCollection, 0,sizeof(COLLECTION_STRUCT));
 
        /* main_data contains information about Type of collection 
         * such as Application, Logical, or Physical etc...  */
        pCollection->bCollectType = main_data;

        if ( pItemStateTable->bCollectIndex == 0x00 )
        {
            /* Update collection Stack. */
            pItemStateTable->pCollectionBuf[pItemStateTable->bCollectIndex] = pCollection;
            pItemStateTable->bCollectIndex++;
            status = MUSB_STATUS_OK;
        }    /* End of if (pItemStateTable->bCollectIndex == 0x00)  */
        else    
        {   
            /* COLLECTION STACK IS not empty hence new collection has parent and
             * last entry in collection stack got a child so please take care 
             * while creating relationship between two collection. 
             */
            if (pItemStateTable->bCollectIndex < MAX_COLLECTION_ALLOWED)
            {
                /* Get pointer of parent and Child BEFORE updation of
                 * collection stack.
                 */               
                pParent = 
                    pItemStateTable->pCollectionBuf[pItemStateTable->bCollectIndex-1];

                /*     Update collection Stack    */
                pItemStateTable->pCollectionBuf
                     [pItemStateTable->bCollectIndex++] = pCollection;

                pParent->pChild[pParent->bTotalChild] = pCollection;
                pParent->bTotalChild++;
                pCollection->pParent = pParent;
                status = MUSB_STATUS_OK;
            }   /*  End of IF (pItemStateTable->coll_pt) */
            else
            {   
                pHidDevice->bCollectionNum--;                
                return MGC_M_STATUS_ERROR;
            }   /* End of Else */
        }   /* End of ELSE  Not first Collection going to stack*/

    }   /* End of if (begin_end_col == COLLECTION )  */
    else
    {   
         
        pItemStateTable->bCollectIndex--;
        pItemStateTable->pCollectionBuf[pItemStateTable->bCollectIndex] = NULL;
        status = MUSB_STATUS_OK;         
    }   /* End of else part as END COLLECTION  */

    return (status);
}

/****************************************************************************
 *Function    : MGC_ParseMainDataItem ()
 *Purpose     : This function handles IN items in MAIN ITEM.
 *Parameters  : pItemStateTable:: Contains current state of items.
 *              main_data :: contains information about main item
 *Called by   : 
 *Returns     : MUSB_STATUS_OK
 *              MGC_M_STATUS_ERROR
 *****************************************************************************/
static uint8_t 
MGC_ParseMainDataItem (ITEM_STATE_TABLE_STRUCT *pItemStateTable, 
                           uint32_t main_data, uint8_t report_type)
{
    MGC_HidDevice *pHidDevice = NULL;
    REPORT_STRUCT *pReport;
    ID_REPORT_STRUCT *pIdReport;
    BASIC_REPORT_STRUCT *pBasicReport;
    uint32_t *pGlobalItem; 
    LOCAL_STATE_TABLE_STRUCT *pLocalItem;
    uint8_t coll_stack_position;

    uint32_t total_size = 0;
    uint16_t index;

    MUSB_ASSERT(pItemStateTable);

    /* Initialize pointers  */
    pHidDevice = (MGC_HidDevice *)pItemStateTable->pHidDevice;
    pReport = &pHidDevice->rReport[report_type];

    /* 
     * Get states of all the state table. 
     */
    pGlobalItem = pItemStateTable->dwGlobalItem;
    pLocalItem = &(pItemStateTable->LocalItem);
    
    coll_stack_position = pItemStateTable->bCollectIndex;

    /* 
     * This "for" loop to find out index of report according to report id. 
     */

    for ( index= 0; index< pReport->bTotalId ; index++)
    {
        if (pReport->pIdReport[index]->report_id == pGlobalItem[GLOBAL_ITEM_TAG_REPORT_ID])
        {
            break;
        }
    }
     
    /* If index is less than bTotalId it means given id has registry in
     * the report database so don't increament bTotalId. Allocate basic
     * report structure and copy all the state table items to that.
     */
    if (index < pReport->bTotalId)
    {    
        /* ID already exist and it can proceed with available data structure */
        pIdReport = pReport->pIdReport[index];

        /* Check for reserved MAIN item. If yes then only increament
         * total_report_size and come back.
         */
        if ( (pLocalItem->usage_num == 0x00)
             && (pLocalItem->usage_min == 0x00)
             && (pLocalItem->usage_max == 0x00))
        {
            total_size = (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_SIZE]) * 
                          (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_COUNT]);
            /*
             * For next basic report this total size will be offset.
             */

            pIdReport->dwTotalBasicReportSize += total_size;
            return (MUSB_STATUS_OK);
        }

        if (pIdReport->dwBasicReportNum == MAX_BASIC_REPORT_ALLOWED)
        {
            return ( MGC_M_STATUS_ERROR);
        }
        
        /* 
         * Allocate memory for basic report as well 
         * as collection for basic report. 
         */
        pBasicReport = (BASIC_REPORT_STRUCT *)MUSB_MemAlloc(sizeof(BASIC_REPORT_STRUCT));
        if ( pBasicReport == NULL)
        {
            return (MGC_M_STATUS_ERROR);
        }

        MUSB_MemSet((void *)pBasicReport, 0,sizeof(BASIC_REPORT_STRUCT));
        
        pBasicReport->collection =
        pItemStateTable->pCollectionBuf[(coll_stack_position-1)];
           
        pIdReport->pBasicReport[pIdReport->dwBasicReportNum] = 
                                                                pBasicReport;

        /* UPDATE Global, local as well as main item in 
         * basic report structure.
         */

        MGC_UpdateBasicReportByGlobal(pBasicReport, pGlobalItem);
        MGC_UpdateBasicReportByLocal(pBasicReport, pLocalItem);
        pBasicReport->main_data = main_data;
 
        /* Set offset of this basic report in a given report.*/
        pBasicReport->report_offset = 
                                 pIdReport->dwTotalBasicReportSize;
                
         /* Update the total size of basic report for given ID */
        total_size = (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_SIZE]) * 
                          (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_COUNT]);
                       
        /* Total size of report for a given id is stored in
         * pIdReport->dwTotalBasicReportSize. For next basic
         * report pIdReport->dwTotalBasicReportSize will be
         * the offset.
         */
        pIdReport->dwTotalBasicReportSize += total_size;

        /* Increament number of baic report in the given id */
        pIdReport->dwBasicReportNum++;

        pBasicReport->pIdReport =pIdReport;
    }   /* End of if (index <num_of_id) */
    else
    {             
        /* This is new id. allocate it and increament number of id by 1*/
        if ( pReport->bTotalId  == MAX_ID_ALLOWED)
        {
            return (MGC_M_STATUS_ERROR);
        }

        pIdReport = (ID_REPORT_STRUCT *)MUSB_MemAlloc(sizeof (ID_REPORT_STRUCT));
        if(pIdReport == NULL)
        {
            return (MGC_M_STATUS_ERROR);
        }

        MUSB_MemSet((void *) pIdReport, 0, sizeof(ID_REPORT_STRUCT));

        pReport->pIdReport[index] = pIdReport;
        pIdReport->report_id = pGlobalItem[GLOBAL_ITEM_TAG_REPORT_ID];

        /* Check for reserved MAIN item. If yes then only increament
         * total_report_size and come back. 
         */
        if ( (pLocalItem->usage_num == 0x00)
             && (pLocalItem->usage_min == 0x00)
             && (pLocalItem->usage_max == 0x00))
        {
            total_size = (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_SIZE]) * 
                          (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_COUNT]);
            
            pIdReport->dwTotalBasicReportSize += total_size;
            return (MUSB_STATUS_OK);
        }

        /* Allocate memory for basic report as well as collection. */
        pBasicReport = (BASIC_REPORT_STRUCT *)MUSB_MemAlloc(sizeof (BASIC_REPORT_STRUCT));
        if ( pBasicReport == NULL)
        {
            return (MGC_M_STATUS_ERROR);
        }

        MUSB_MemSet((void *)pBasicReport, 0,sizeof( BASIC_REPORT_STRUCT));
 
        pBasicReport->collection =
        pItemStateTable->pCollectionBuf[(coll_stack_position-1)];

            
        pIdReport->pBasicReport[pIdReport->dwBasicReportNum] = 
                                                                pBasicReport;

        MGC_UpdateBasicReportByGlobal(pBasicReport, pGlobalItem);
        MGC_UpdateBasicReportByLocal(pBasicReport, pLocalItem);
        pBasicReport->main_data = main_data;

        pBasicReport->report_offset = pIdReport->dwTotalBasicReportSize;

        total_size = (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_SIZE]) * 
                          (pGlobalItem[GLOBAL_ITEM_TAG_REPORT_COUNT]);

        pIdReport->dwTotalBasicReportSize += total_size;
                        
        pIdReport->dwBasicReportNum++;
       
        pReport->bTotalId++;
       
        pBasicReport->pIdReport = pIdReport;
    
    }

    return (MUSB_STATUS_OK);
}

/****************************************************************************
 *Function    : MGC_DecodeMainItem()
 *Purpose     : To store local items in the item state table.
 *Parameters  : pItemStateTable:: Contains current state of items.
 *              pItem :: This would use for storing an item.
 *Called by   : MGC_DecodeReportDscr()
 *Returns     : MUSB_STATUS_OK
 *              MGC_M_STATUS_ERROR
 *****************************************************************************/
static uint8_t 
MGC_DecodeMainItem(ITEM_STATE_TABLE_STRUCT *pItemStateTable,
                      ITEM_INFO_STRUCT *pItem)
{
    uint32_t main_data;

    MUSB_ASSERT(pItemStateTable);
    MUSB_ASSERT(pItem);

     /* 
     * MAIN data is always +ve. Since it gives information about either 
     * collection type in case of non data MAIN item or information like
     * relative | absolute, variable | array, in case of data MAIN item.
     */
    main_data = MGC_GetItemData(pItem);

    if (pItem->bTag == MAIN_ITEM_TAG_INPUT ) 
    {
        return MGC_ParseMainDataItem (pItemStateTable, main_data, IN_REPORT);
    }

    if (pItem->bTag == MAIN_ITEM_TAG_OUTPUT ) 
    {
        return MGC_ParseMainDataItem (pItemStateTable, main_data, OUT_REPORT);
    }

    if (pItem->bTag == MAIN_ITEM_TAG_COLLECTION )
    {
        return MGC_ParseMainNonDataItem(pItemStateTable, main_data, COLLECTION);
    }

    if (pItem->bTag == MAIN_ITEM_TAG_FEATURE ) 
    {
        return MGC_ParseMainDataItem (pItemStateTable, main_data, FEATURE_REPORT);
    }

    if (pItem->bTag == MAIN_ITEM_TAG_END_COLLECTION )
    {
        return MGC_ParseMainNonDataItem(pItemStateTable, 
            main_data, END_COLLECTION);
    }
    
    return (MGC_M_STATUS_ERROR);
} /* End of function MGC_DecodeMainItem()  */

/****************************************************************************
 *Function    : MGC_DecodeGlobalItem()
 *Purpose     : To store global items in the item state table.
 *Parameters  : pItemStateTable:: Contains current state of items.
 *              pItem :: This would use for storing an item.
 *Called by   : MGC_DecodeReportDscr()
 *Returns     : MUSB_STATUS_OK
 *              MGC_M_STATUS_ERROR
 *****************************************************************************/
static uint8_t 
MGC_DecodeGlobalItem(ITEM_STATE_TABLE_STRUCT *pItemStateTable,
                       ITEM_INFO_STRUCT *pItem )
{
    uint8_t status = MUSB_STATUS_OK;

    MUSB_ASSERT(pItemStateTable);
    MUSB_ASSERT(pItem);

    if (pItem->bTag == GLOBAL_ITEM_TAG_PUSH )        
    {
        LOG(5, "Hid Err: TAG_PUSH not support.\n", NULL);     
        status = MGC_M_STATUS_ERROR;
    } 
    else
    {      
       if (pItem->bTag == GLOBAL_ITEM_TAG_POP )
       {
            LOG(5, "Hid Err: TAG_PUSH not support.\n", NULL);     
            status = MGC_M_STATUS_ERROR;            
        }
        else
        {
            if (pItem->bTag <= GLOBAL_ITEM_TAG_REPORT_COUNT)
            {
                pItemStateTable->dwGlobalItem[pItem->bTag] = pItem->item_data;
                status = MUSB_STATUS_OK;
            }
            else
            {   
                /* Unknown Global Tag has found. its error condition. */
                status = MGC_M_STATUS_ERROR;
            }
        }          
    }

    return (status);
}

/****************************************************************************
 *Function    : MGC_DecodeLocalItem();
 *Purpose     : To store local items in the item state table.
 *Parameters  : pItemStateTable:: Contains current state of items.
 *              pItem :: This would use for storing an item.
 *Called by   : MGC_DecodeReportDscr()
 *Returns     : MUSB_STATUS_OK
 *              MGC_M_STATUS_ERROR
 *****************************************************************************/
static uint8_t 
MGC_DecodeLocalItem(ITEM_STATE_TABLE_STRUCT *pItemStateTable,
                      ITEM_INFO_STRUCT *pItem)
{
    uint32_t local_data;
    uint32_t usage_page = 0x0000;
    uint32_t index;
    LOCAL_STATE_TABLE_STRUCT *pLocalItem;

    MUSB_ASSERT(pItemStateTable);
    MUSB_ASSERT(pItem);
    
    /*
     * Retrieve data from the item. Local data is always unsigned uint32_t
     * according to HID specification.
     */  
    local_data = MGC_GetItemData(pItem);

    if (pItem->bSize < 0x03 )
    {
        usage_page = 
            (pItemStateTable->dwGlobalItem[GLOBAL_ITEM_TAG_USAGE_PAGE] << 16);
    }

    /* 
     *  When usage size is less than 4 byte  it means it has to get USAGE_PAGE
     *  from global state table and put it with the USAGE id.
     *  First 16 bit (MSB) is USAGE PAGE and next 16 bits is USAGE ID.
     */
    pLocalItem = &pItemStateTable->LocalItem;
    if (pItem->bTag == LOCAL_ITEM_TAG_USAGE ) 
    {
        if (!pLocalItem->bUsage)
        {
            pLocalItem->bUsage = TRUE;
        }
        index = pLocalItem->usage_num;
        if (index >= MAX_USAGE_ALLOWED)
        {
            return MGC_M_STATUS_ERROR;
        }
        pLocalItem->usage[index] = (local_data | usage_page);
        pLocalItem->usage_num ++;
    }
    else if (pItem->bTag == LOCAL_ITEM_TAG_USAGE_MIN) 
    {
        pLocalItem->bUsage = FALSE;
        pLocalItem->usage_min = (local_data | usage_page);
    }
    else if (pItem->bTag == LOCAL_ITEM_TAG_USAGE_MAX ) 
    {
        pLocalItem->bUsage = FALSE;
        pLocalItem->usage_max = (local_data | usage_page);
    }
    
    return (MUSB_STATUS_OK);
}

/****************************************************************************
 *Function    : MGC_GetItemInfo()
 *Purpose     : This function is used to store an item ( item prefix and data)
 *              in a item info structure from raw report descriptor.
 *Parameters  : begin :: starting pointer to the item.
 *              end   :: End pointer to.
 *              pItem :: This would use for storing an item.
 *Called by   : MGC_DecodeReportDscr()
 *Returns     : uint8_t * ( pointer to next item in raw report descriptor).
 *****************************************************************************/
static uint8_t *
MGC_GetItemInfo(uint8_t *pReportDscr, uint8_t *pReportDscrLength, 
                       ITEM_INFO_STRUCT *pItem)
{
    uint8_t length;

    MUSB_ASSERT(pReportDscr);
    MUSB_ASSERT(pReportDscrLength);
    MUSB_ASSERT(pItem);
    
    /* First byte tells about  Tag, Size and Type of  a item. */
    pItem->bPrefix = *pReportDscr++;
    (*pReportDscrLength)--;

    /* Start from LSB : First 2 bits is size, Next 2 bits is bType, Next 4 bits
     * represent bTag.
     */
    pItem->bSize = pItem->bPrefix & 0x3;
    pItem->bType = ((pItem->bPrefix >> 2) & 0x3); 
    pItem->bTag = ((pItem->bPrefix >> 4) & 0xF);

    if(pItem->bTag != LONG_ITEM_TAG)
    {
        if (((*pReportDscrLength) < pItem->bSize ) ||  
            (pItem->bSize > 0x03 ))
        {
            /* Short Item max 3 bytes  */
            return NULL;
        }

        pItem->item_data = 0;
        if (pItem->bSize != 0x00) 
        {
            /* For bSize 1,2,3 number of bytes should be 1, 2, 4. We are storing
             * signed as well as unsigned data so that retrieval will be easy.
             */
            length = (1 << (pItem->bSize - 1));
            MUSB_MemCopy((void *)&pItem->item_data, (void *)pReportDscr, length);
            pReportDscr += length; 
            *(pReportDscrLength) -= length;           
        }    
        return pReportDscr;
    }
    else
    {
        LOG(1, "Do not support Long items.\n", NULL);
        return NULL;
    }    
}

/******************************************************************************
 *Function    : MGC_DecodeReportDscr()
 *Purpose     : This function is used to decode report descriptor and store it
 *              in the hid device structure.
 *Parameters  : report_dscr_P :: Raw report descriptor is stored here.
 *              pReportDscrLength :: length of report descriptor.
 *              pHidDevice :: Pointer to HID descriptor.
 *Called by   : MGC_Handle_HID_Device()
 *Returns     : MUSB_STATUS_OK
 *              MUSB_STATUS_NO_MEMORY
 *****************************************************************************/
static uint8_t  MGC_DecodeReportDscr (uint8_t *report_dscr_P, 
    uint16_t pReportDscrLength, MGC_HidDevice *pHidDevice)
{
    ITEM_INFO_STRUCT rItem;
    ITEM_STATE_TABLE_STRUCT rItemStateTable;
    uint8_t *pReportDscr = report_dscr_P;
    uint8_t status = MUSB_STATUS_OK;       

    MUSB_ASSERT(report_dscr_P);
    MUSB_ASSERT(pHidDevice);
    
    /*
     * rItemStateTable is used to store state of each and every item.
     */
    LOG(5, "DecodeReportDscr.\n", NULL);

    MUSB_MemSet(&rItemStateTable, 0, sizeof(ITEM_STATE_TABLE_STRUCT));
    rItemStateTable.pHidDevice = pHidDevice;
    
    /* 
     * Control comes here means rItem as well as item state table
     * is initialized. Now it can start fetching items one by one and
     * storing in item state table.
     */
    while (pReportDscrLength > 0x00)
    {
        pReportDscr = MGC_GetItemInfo(pReportDscr, 
                                                  (uint8_t *)&pReportDscrLength, 
                                                  &rItem);    
        if (!pReportDscr) 
        { 
             LOG(5, "Hid Err: pReportDscr Null.\n", NULL);
             /* There is no item left to decode from report descriptor */
            break;
        }
             
        switch (rItem.bType) 
        {      
            case ITEM_TYPE_MAIN:
                MGC_DisplayGlobalItem(rItemStateTable.dwGlobalItem); 
                status = MGC_DecodeMainItem (&rItemStateTable, &rItem);
                /* After every MAIN item it has to reset local items. */
                MUSB_MemSet(&rItemStateTable.LocalItem, 0,  
                                   sizeof(rItemStateTable.LocalItem));
                break;
            case ITEM_TYPE_GLOBAL:
                status = MGC_DecodeGlobalItem(&rItemStateTable,&rItem);
                break;
            case ITEM_TYPE_LOCAL:
                status = MGC_DecodeLocalItem(&rItemStateTable,&rItem);
                break;
            case ITEM_TYPE_RESERVED:
                break;

            default:
                break;
        }   /* End of CASE statement  */
        if (status != MUSB_STATUS_OK)
        {
            /* If given item is not decoded properly then no need 
             * to go ahead. Stop processing and return error condition.
             */
            break;
        }

    }    /* End of while (report_dsr_length > 0x00) loop  */

    return status;
}

uint32_t MUSB_ParseReportDescr(MGC_HidDevice *pHidDevice)
{
    uint8_t *report_dscr_P;
    uint16_t pReportDscrLength;
    uint8_t status;
    #ifdef MUSB_HID_DEBUG    
    uint16_t n;
    #endif

    if ((!pHidDevice) ||(!pHidDevice->pHidDescr))
    {
        return MGC_M_STATUS_ERROR;
    }

    report_dscr_P = pHidDevice->pReportDescr;
    pReportDscrLength = pHidDevice->pHidDescr->wDescriptorLength;

    #ifdef MUSB_HID_DEBUG
    /* Following will display the received report */    
    for (n = 0; n < pReportDscrLength; n=n+2)
    {
        Printf("0x%02X, 0x%02X.\n", report_dscr_P[n], report_dscr_P[n+1]);
    }
    Printf("\n");
    #endif

    status = MGC_DecodeReportDscr (report_dscr_P, pReportDscrLength, pHidDevice);
    if (status != MUSB_STATUS_OK)
    {
        
        LOG(1, "Hid Err: DecodeReportDscr\n", NULL);
        return status;
    }  

    return MUSB_STATUS_OK;
}

/******************************************************************************
 * FUNCTION             : MGC_Get_Bits_From_Byte_Stream()
 * PURSPOSE             : This is mainly finds the range of the offset and
 *                        and depends on the value of "number of bits + offset"
 *                        it selects the corresponding function to get the value
 *                        of the bits required from the given offset.
 * PARAMETERS           : data_ptr: Pointer to the byte stream, from which we 
 *                                  are extracting the bits.
 *                                : It should be the starting byte pointer 
 *                      : p       : Offset from where we need to get the bits
 *                                  This value should be between 
 *                                  "0 to 7".
 *                      : n       : Number of bits required and its value 
 *                                : should be between "0 to 32".
 * RETURN VALUE         : value   : retuns the value of the bits and it is
 *                                  independent of native endianity.
 *                        err     : Its value is -1 if steram pointer is NULL;
 *                                : Its value -2 if p+n > 32. 
 *                                : Its value is 0, if the request is success
 * CALLED BY            : Different driver functions
 ******************************************************************************/
uint32_t MUSB_GetBitsFromByteStream(uint8_t *data_ptr, uint8_t p, uint8_t n)
{
    uint32_t value;                /* This is for storing the bits value*/

    MUSB_ASSERT(data_ptr);
    MUSB_ASSERT((p + n) <= 32);
    
    if ( (p + n) <= 32 )
    {
       value = MUSB_SWAP32P(data_ptr);
       if(n == 32)    
       {
           MUSB_ASSERT(p == 0);
            return (value);
       }
       else
       {
           value = ( (value >> (p) ) & BIT_MASK(n) );
           return (value);
       }           
    }

    return (0);
} /* End of MGC_Get_Bits_From_Byte_Stream() */

#endif /* MUSB_HID */

