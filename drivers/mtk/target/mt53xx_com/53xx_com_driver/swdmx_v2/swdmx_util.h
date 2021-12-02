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
 * $RCSfile: swdmx_util.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_util.h
 */


#ifndef SWDMX_UTIL_H
#define SWDMX_UTIL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_mm_common.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SD_AAC_FAKE_HDR_SIZE 7

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT16 depth; 
    UINT16 pos;
    MM_RANGE_ELMT_IDXTBL_T *idx_tbl;    
}sd_entry_pos;

#define SD_MEM_TYPE_INTERNAL 1
#define SD_MEM_TYPE_DMA      2
#define SD_MEM_TYPE_VIRTUAL  3


typedef struct _sd_mem_list
{
    UINT32 type; 
    UINT32 size;
    void*  addr;
    void*  curr;
    struct _sd_mem_list *next;    
}sd_mem_list;

#define SD_MEM_HANDLE  (swdmx_inst->pr_mem_list)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
void swdmx_gen_aac_header(UINT8 *pb, UINT16 frame_size, UINT32 sample_rate, UINT32 channel_ns);
BOOL swdmx_find_tbl_entry_by_pts(MM_RANGE_ELMT_IDXTBL_T *idx_tbl, UINT32 pts, sd_entry_pos *pt_ret);
BOOL swdmx_find_tbl_entry_by_offset(MM_RANGE_ELMT_IDXTBL_T *idx_tbl, UINT64 offset, sd_entry_pos *pt_ret);


//memory manager
void swdmx_mem_init(sd_mem_list **ppt_list);
void swdmx_mem_deinit(sd_mem_list *pt_list);
void * swdmx_mem_alloc(sd_mem_list *pt_list, UINT32 type, UINT32 size);
#define swdmx_mem_valloc(_size) swdmx_mem_alloc(SD_MEM_HANDLE ,SD_MEM_TYPE_DMA, _size)
#define swdmx_mem_falloc(_size) swdmx_mem_alloc(SD_MEM_HANDLE ,SD_MEM_TYPE_VIRTUAL, _size)
#define swdmx_mem_free(_p) 


#endif // SWDMX_UTIL_H


