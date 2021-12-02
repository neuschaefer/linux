/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_util.c
 *  Software program stream demux driver - utillity fuctions
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_common.h"
#include "x_typedef.h"
#include "swdmx_util.h"
#include "x_os.h"
#include "x_debug.h"
#include "x_hal_5381.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void swdmx_gen_aac_header(UINT8 *pb, UINT16 frame_size, UINT32 sample_rate, UINT32 channel_ns)
{
    static UINT32 sample_rate_to_idx[] = {
        96000,88000,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000};
    UINT8 sample_rate_idx, i;

    frame_size += SD_AAC_FAKE_HDR_SIZE;
    /* 8b: syncword */
    pb[0] = 0xFF;
    /* 4b: syncword */
    /* 1b: mpeg id = 1 */
    /* 2b: layer = 0 */
    /* 1b: protection absent */
    pb[1] = 0xF9;
    /* 2b: profile */
    /* 4b: sampling_frequency_index */
    /* 1b: private = 0 */
    /* 1b: channel_configuration */
    sample_rate_idx = 4;  //default
    for (i = 0; i < sizeof(sample_rate_to_idx)/sizeof(UINT32); i++)
    {
        if (sample_rate_to_idx[i] == sample_rate)
        {
           sample_rate_idx = i;
           break;
        }
    }

    pb[2] = (1 << 6) |
            ((sample_rate_idx<< 2) & 0x3C) |
            ((channel_ns >> 2) & 0x1);
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    pb[3] = ((channel_ns & 0x3) << 6) |
            ((frame_size >> 11) & 0x3);
    /* 8b: aac_frame_length */
    pb[4] = ((frame_size >> 3) & 0xFF);
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    pb[5] = ((frame_size << 5) & 0xE0) |
            ((0x7FF >> 6) & 0x1F);
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    pb[6] = ((0x7FF << 2) & 0xFC);

}


BOOL swdmx_valid_hdr_bps(UINT32 u4HdrBps, UINT32 u4DspBps)
{
    // These values are defined in 11172-3 page 21.
    static UINT16 au2NormalBps[] = {16,  24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320};
    #define N_BPS sizeof(au2NormalBps)/sizeof(au2NormalBps[0])
    const UINT16 u2Precision = 1;
    UINT16 u2HdrBps = (UINT16)(u4HdrBps * 8 / 1000);
    UINT16 u2DspBps = (UINT16)(u4DspBps * 8 / 1000);
    UINT16 u2HdrPtn = N_BPS;
    UINT16 u2DspPtn = N_BPS;
    UINT32 i;

    for(i = 0; i < N_BPS; i++)
    {
        if ((u2HdrBps >= au2NormalBps[i] - u2Precision) && (u2HdrBps <= au2NormalBps[i] + u2Precision))
        {
            u2HdrPtn = i;
        }
        if ((u2DspBps >= au2NormalBps[i] - u2Precision) && (u2DspBps <= au2NormalBps[i] + u2Precision))
        {
            u2DspPtn = i;
        }
    }
    if ((u2HdrPtn < N_BPS) && (u2HdrPtn == u2DspPtn))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL swdmx_find_tbl_entry_by_pts(MM_RANGE_ELMT_IDXTBL_T *idx_tbl, UINT32 pts, sd_entry_pos *pt_ret)
{
    MM_RANGE_ELMT_IDXTBL_T * pCurBuldIdxTbl = idx_tbl;
    UINT32 low, high, mid;
    UINT32 u4temp1,u4temp2;

    ASSERT(pCurBuldIdxTbl || pt_ret);

    pt_ret->depth = 0;
    pCurBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T * )idx_tbl;
    while (pCurBuldIdxTbl->pt_next_partial)
    {
        if (pCurBuldIdxTbl->ui4_number_of_entry)
        {
            return FALSE;
        }
        u4temp1 = pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[pCurBuldIdxTbl->ui4_number_of_entry - 1].ui4_pts;
        if (u4temp1 >= pts) //need next idx table
        {
            break;
        }
        pt_ret->depth++;
        pCurBuldIdxTbl = pCurBuldIdxTbl->pt_next_partial;
    }

    pt_ret->idx_tbl = pCurBuldIdxTbl;

    //binary search 
    low  = 0;
    mid  = 0;
    high = pCurBuldIdxTbl->ui4_number_of_entry - 1;
    while ( low <= high ){
        mid = (low + high) >> 1;
        u4temp1 = pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[mid].ui4_pts;
        if (mid < high)
        {
            u4temp2 = pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[mid + 1].ui4_pts;
        }
        else
        {
            u4temp2 = u4temp1;
        }

        if (pts >= u4temp1 && pts <= u4temp2) 
            break;
        
        if(u4temp1 > pts)
        {
            high = mid - 1;
        }
        else
        {
            low  = mid + 1;
        }
    }

    pt_ret->pos     = mid;

    return TRUE;

}

BOOL swdmx_find_tbl_entry_by_offset(MM_RANGE_ELMT_IDXTBL_T *idx_tbl, UINT64 offset, sd_entry_pos *pt_ret)
{
    MM_RANGE_ELMT_IDXTBL_T * pCurBuldIdxTbl = idx_tbl;
    UINT32 low, high, mid;
    UINT64 utemp1,utemp2;

    ASSERT(pCurBuldIdxTbl || pt_ret);

    pt_ret->depth = 0;
    pCurBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T * )idx_tbl;
    while (pCurBuldIdxTbl->pt_next_partial)
    {
        if (pCurBuldIdxTbl->ui4_number_of_entry)
        {
            return FALSE;
        }
        utemp1 = pCurBuldIdxTbl->ui8_base_offset + pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[pCurBuldIdxTbl->ui4_number_of_entry - 1].ui4_relative_offset;
        if (utemp1 >= offset) //need next idx table
        {
            break;
        }
        pt_ret->depth++;
        pCurBuldIdxTbl = pCurBuldIdxTbl->pt_next_partial;
    }

    pt_ret->idx_tbl = pCurBuldIdxTbl;

    //binary search 
    low  = 0;
    mid  = 0;
    high = pCurBuldIdxTbl->ui4_number_of_entry - 1;
    while ( low <= high ){
        mid = (low + high) >> 1;
        utemp1 = pCurBuldIdxTbl->ui8_base_offset + pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[mid].ui4_relative_offset;
        if (mid < high)
        {
            utemp2 = pCurBuldIdxTbl->ui8_base_offset + pCurBuldIdxTbl->u.pt_pb_idx_tbl_entry[mid + 1].ui4_relative_offset;
        }
        else
        {
            utemp2 = utemp1;
        }

        if (offset >= utemp1 && offset <= utemp2) 
            break;
        
        if(utemp1 > offset)
        {
            high = mid - 1;
        }
        else
        {
            low  = mid + 1;
        }
    }

    pt_ret->pos     = mid;

    return TRUE;

}

static void swdmx_mem_free_all(sd_mem_list *pt_list)
{
    //free 
    while (pt_list)
    {
        sd_mem_list *pt;
        switch(pt_list->type)
        {
        case SD_MEM_TYPE_DMA:
            BSP_FreeAlignedDmaMemory((UINT32)pt_list->addr);
            break;
        case SD_MEM_TYPE_VIRTUAL:
            x_mem_free(pt_list->addr);
            break;
        default:
            ASSERT(0);
            break;
        }
        
        pt = pt_list->next;
        x_mem_free(pt_list);
        pt_list = pt;
    }
}


void swdmx_mem_init(sd_mem_list **ppt_list)
{
    //init internal buffer
    if (!(*ppt_list)) 
    {
        sd_mem_list *pt;
        pt = x_mem_alloc(sizeof(sd_mem_list));

        if (!pt)
        {
            LOG(0, "swdmx_mem_init fail\n");
            return;
        }
        pt->type = SD_MEM_TYPE_INTERNAL;
        pt->size = 0x8000; //32k
        pt->addr = (void *)BSP_AllocAlignedDmaMemory(pt->size, 16);
        pt->curr = pt->addr;
        pt->next = NULL;
        if (!(pt->addr))
        {
            LOG(0, "swdmx_mem_init fail\n");
            return;
        }

        *ppt_list = pt;
    }

    if (*ppt_list)
    {
        (*ppt_list)->curr = (*ppt_list)->addr;
    }

    swdmx_mem_free_all((*ppt_list)->next);
    
}

void swdmx_mem_deinit(sd_mem_list *pt_list)
{
    if (pt_list)
    {
        swdmx_mem_free_all(pt_list->next);
        pt_list->next = NULL;
    }
}

static void * swdmx_mem_internal_alloc(sd_mem_list *pt_list, UINT32 size)
{
    void *addr = NULL;
    ASSERT(pt_list);

    do{
        UINT32 temp;
        if (pt_list->type != SD_MEM_TYPE_INTERNAL)
        {
            break;
        }
        //always 16 alignment
        temp = (UINT32)(pt_list->curr);
        temp = (temp + 15)&0XFFFFFFF0;
        if ((temp + size) > ((UINT32)(pt_list->addr) + pt_list->size))
        {
            break;
        }
        addr = (void *)temp;
        pt_list->curr = (void *)(temp + size);    
    }while(0);
    
    return addr;
}


void * swdmx_mem_alloc(sd_mem_list *pt_list, UINT32 type, UINT32 size)
{
    void *addr;
    sd_mem_list *pt;
    ASSERT(pt_list);
    ASSERT(type == SD_MEM_TYPE_DMA || type == SD_MEM_TYPE_VIRTUAL);
    
    addr = swdmx_mem_internal_alloc(pt_list, size);
    if (!addr)
    {
        pt = x_mem_alloc(sizeof(sd_mem_list));

        if (!pt) 
        {   
            return NULL;
        }

        pt->type = type;
        pt->size = size;
        pt->addr = (void *)((type == SD_MEM_TYPE_DMA) ? (void *)BSP_AllocAlignedDmaMemory(pt->size, 16) : (void *)x_mem_alloc((SIZE_T)pt->size));
        pt->curr = pt->addr;
        pt->next = NULL;
        addr = pt->addr;

        if (!addr) 
        {
            x_mem_free(pt);
            return NULL;
        }

        while(pt_list->next) pt_list = pt_list->next;
        pt_list->next = pt;
    }
    return addr;
}



