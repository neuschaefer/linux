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
/*-----------------------------------------------------------------------------
 * $RCSfile: rec_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: ql.wang $
 * $MD5HEX: ab263bfe093fb6a9cf962d72692852e1 $
 *
 * Description:
 *         This file contains Event Context Selector specific functions.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "util/x_lnk_list.h"
#include "rec_util/_rec_util.h"
#include "rec_util/rec_util_utils.h"
#include "dbg/dbg.h"
#include "os/inc/x_os.h"
#include "rec_util/_rec_util_grb.h"
#include "rec_util/rec_util_grb.h"



/*macro*/

/*
#define REC_UTIL_GRB_BUILD_PREFIX(high , low)    \
    ((UINT32)(((((UINT32)(high))&0x0000FFFF)<<16)|((((UINT32)(low))&0x0000FFFF))))

#define REC_UTIL_GRB_GET_PREFIX_HIGH(val)  \
    ((UINT32)((((UINT32)(val))&0xFFFF0000) >> 16))

#define REC_UTIL_GRB_GET_PREFIX_LOW(val)  \
    ((UINT32)((((UINT32)(val))&0x0000FFFF)))
*/

/*API Implement*/

/*to alloc a new slice buf*/
static INT32 _rec_util_grb_alloc_slice_buf(
    REC_UTIL_GRB_MEM_MODE_T     e_mem_mode,
    UINT32                      ui4_capacity_req,
    REC_UTIL_GRB_SLICE_BUF_T**  ppt_new_slice,
    UINT32*                     pui4_capacity_got)
{
    INT32                       i4_ret;
    REC_UTIL_GRB_SLICE_BUF_T*   pt_slice = NULL;
    UINT32                      ui4_struct_len;
    UINT32                      ui4_capacity_got = 0;
    UINT32                      ui4_slice_size_real = 0;
    VOID*                       pv_slice_phy = NULL;
    
    if(NULL == ppt_new_slice || 0 == ui4_capacity_req || NULL == pui4_capacity_got)
    {
        REC_UTIL_DBG_ERR(("_rec_util_grb_alloc_slice_buf param invalid.\r\n"));
        return REC_UTILR_GRB_FAIL;
    }

    *ppt_new_slice = NULL;
    *pui4_capacity_got = 0;
    
    do
    {
        /*alloc a new slice*/
        ui4_struct_len = REC_DB_ALIGN_DATA(sizeof(REC_UTIL_GRB_SLICE_BUF_T));
        i4_ret = REC_UTILR_OK;

        switch (e_mem_mode)
        {
        case REC_UTIL_GRB_MEM_MODE_NORMAL:
        {
            ui4_capacity_got = ui4_capacity_req;
            ui4_slice_size_real = ui4_struct_len + ui4_capacity_got;
            pt_slice = (REC_UTIL_GRB_SLICE_BUF_T *) x_mem_alloc (ui4_slice_size_real);
            if (NULL == pt_slice)
            {
                REC_UTIL_DBG_ERR(("_rec_util_grb_alloc_slice_buf alloc fail 1.\r\n"));
                i4_ret = REC_UTILR_GRB_FAIL;
                break;
            }
            
            pv_slice_phy = (VOID *) pt_slice;
            break;
        }
            
        case REC_UTIL_GRB_MEM_MODE_SHARE:
        {
            UINT32  ui4_slice_size_req = ui4_struct_len + ui4_capacity_req;
            
            ui4_slice_size_real = REC_DB_ALIGN_SHARE_SIZE (ui4_slice_size_req);
            ui4_capacity_got = ui4_slice_size_real - ui4_struct_len;
            pv_slice_phy = x_share_mem_alloc (ui4_slice_size_real);
            if (pv_slice_phy == NULL)
            {
                REC_UTIL_DBG_ERR(("_rec_util_grb_alloc_slice_buf(): alloc fail 2.\r\n"));
                i4_ret = REC_UTILR_GRB_FAIL;
                break;
            }
            
            pt_slice = (REC_UTIL_GRB_SLICE_BUF_T *) x_mmap_share_mem (pv_slice_phy, 
                                                                      ui4_slice_size_real);
            if (pt_slice == NULL)
            {
                REC_UTIL_DBG_ERR(("_rec_util_grb_alloc_slice_buf(): alloc fail 3.\r\n"));
                i4_ret = REC_UTILR_GRB_FAIL;
                break;
            }
            break;    
        }
            
        default:
            i4_ret = REC_UTILR_NOT_SUPPORT;
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        }
        
        if (i4_ret != REC_UTILR_OK)
        {
            break;
        }

        pt_slice->pv_slice_phy       = pv_slice_phy;
        pt_slice->z_slice_size       = (SIZE_T) ui4_slice_size_real;
        pt_slice->pui1_start_ptr     = ((UINT8 *) pt_slice) + ui4_struct_len;
        pt_slice->pui1_start_ptr_phy = ((UINT8 *) pv_slice_phy) + ui4_struct_len;
        pt_slice->pui1_read_ptr      = pt_slice->pui1_start_ptr;
        pt_slice->pui1_write_ptr     = pt_slice->pui1_start_ptr;
        pt_slice->e_slice_status     = REC_UTIL_SLICE_BUF_EMPTY;
        pt_slice->ui4_capacity       = ui4_capacity_got;  
        pt_slice->ui4_entry_num      = 0;

        x_memset(pt_slice->pui1_start_ptr, 0, pt_slice->ui4_capacity);

        i4_ret = REC_UTILR_GRB_OK;
    
    } while (0);

    if (REC_UTILR_GRB_OK == i4_ret)
    {
        *ppt_new_slice = pt_slice;
        *pui4_capacity_got = ui4_capacity_got;
    }

    return i4_ret;
}

/*to free a slice in ring buf*/
static INT32 _rec_util_grb_free_slice_buf  (
    REC_UTIL_GRB_MEM_MODE_T     e_mem_mode,
    REC_UTIL_GRB_SLICE_BUF_T**  ppt_slice)
{    
    INT32                       i4_ret;
    REC_UTIL_GRB_SLICE_BUF_T*   pt_slice;
    
    if(NULL == ppt_slice || NULL == (*ppt_slice))
    {
        REC_UTIL_DBG_ERR(("_rec_util_grb_free_slice_buf:param invalid.\r\n"));
        return REC_UTILR_GRB_FAIL;
    }

    do
    { 
        pt_slice = *ppt_slice;
        
        switch (e_mem_mode)
        {
        case REC_UTIL_GRB_MEM_MODE_NORMAL:
        {
            x_mem_free (pt_slice);
            break;
        }
        case REC_UTIL_GRB_MEM_MODE_SHARE:
        {
            VOID*   pv_slice_phy = pt_slice->pv_slice_phy;
            SIZE_T  z_slice_size = pt_slice->z_slice_size;
            
            x_munmap_share_mem ((VOID *) pt_slice, 
                                z_slice_size);
            x_share_mem_free (pv_slice_phy, 
                              z_slice_size);
            break;
        }
        default:
            REC_UTIL_ABORT (REC_UTIL_DBG_ABRT_CANNOT_HAPPEN);
        }
        
        *ppt_slice = NULL;
                
        i4_ret = REC_UTILR_GRB_OK;
         
    } while(0);
    
    return i4_ret;
}

/*to reset a slice in ring buf*/
static INT32 _rec_util_grb_reset_slice_buf(REC_UTIL_GRB_SLICE_BUF_T* pt_slice)
{    
    INT32                       i4_ret;
    
    if(NULL == pt_slice)
    {
        REC_UTIL_DBG_ERR(("_rec_util_grb_reset_slice_buf :param invalid.\r\n"));
        return REC_UTILR_GRB_FAIL;
    }

    do
    { 
        if(NULL == pt_slice->pui1_start_ptr)
        {
            REC_UTIL_DBG_ERR(("_rec_util_grb_reset_slice_buf :start ptr null.\r\n"));
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
        
        
        pt_slice->pui1_read_ptr     = pt_slice->pui1_start_ptr;
        pt_slice->pui1_write_ptr    = pt_slice->pui1_start_ptr;
        pt_slice->ui4_entry_num     = 0;
        pt_slice->e_slice_status    = REC_UTIL_SLICE_BUF_EMPTY;

        x_memset(pt_slice->pui1_start_ptr, 0, pt_slice->ui4_capacity);
        i4_ret = REC_UTILR_GRB_OK;
         
    }while(0);
    
    return i4_ret;
}


/*add one new slice buf to the tail of ring buf*/
static INT32 _rec_util_grb_add_slice_buf(
    REC_UTIL_GRB_T*             pt_ring_buf, 
    UINT32                      ui4_capacity,
    REC_UTIL_GRB_SLICE_BUF_T**  ppt_new_slice)
{
    INT32                       i4_ret;
    REC_UTIL_GRB_SLICE_BUF_T*   pt_slice         = NULL;
    UINT32                      ui4_capacity_req = 0;
    BOOL                        b_alloc_slice    = FALSE;
    
    if(NULL == pt_ring_buf)
    {
        REC_UTIL_DBG_ERR(("{rec_util} inital ring buf point invalid .\r\n"));
        return REC_UTILR_GRB_FAIL;
    }
    
    do
    {
        ui4_capacity_req = (pt_ring_buf->ui4_slice_cap_dft > ui4_capacity) ? 
                             pt_ring_buf->ui4_slice_cap_dft : ui4_capacity;

        ui4_capacity_req = REC_DB_ALIGN_DATA(ui4_capacity_req);
                
        pt_slice = DLIST_TAIL(&(pt_ring_buf->t_slice_buf_list));
        
        /*only have one slice and it's empty*/
        if(1 == pt_ring_buf->ui4_slice_num  &&
            pt_slice->e_slice_status == REC_UTIL_SLICE_BUF_EMPTY)
        {           
            if(pt_slice->ui4_capacity < ui4_capacity_req)
            {
                UINT32   ui4_capacity_got;

                DLIST_REMOVE(pt_slice,&(pt_ring_buf->t_slice_buf_list),t_link);
                /*realloc space*/
                i4_ret = _rec_util_grb_free_slice_buf(pt_ring_buf->e_mem_mode, &pt_slice);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_grb_add_slice_buf free fail.\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    break;
                }

                i4_ret = _rec_util_grb_alloc_slice_buf (pt_ring_buf->e_mem_mode,
                                                        ui4_capacity_req, 
                                                        &pt_slice,
                                                        &ui4_capacity_got);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_grb_add_slice_buf alloc fail.\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    break;
                }
                
                pt_ring_buf->ui4_slice_cap_dft = ui4_capacity_got;
                DLIST_INSERT_TAIL(pt_slice,&(pt_ring_buf->t_slice_buf_list),t_link);
            }
            else
            {
                /*reset to initial status*/
                i4_ret = _rec_util_grb_reset_slice_buf(pt_slice);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_grb_add_slice_buf reset fail.\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    break;
                }
            }
            
            i4_ret = REC_UTILR_GRB_OK;
        }
        else
        {
            /*add a new slice*/
            if(NULL == pt_ring_buf->pt_removed_slice_buf || 
                pt_ring_buf->pt_removed_slice_buf->ui4_capacity < ui4_capacity_req)
            {
                UINT32   ui4_capacity_got;

                i4_ret = _rec_util_grb_alloc_slice_buf(pt_ring_buf->e_mem_mode,
                                                       ui4_capacity_req, 
                                                       &pt_slice,
                                                       &ui4_capacity_got);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_grb_add_slice_buf free fail.\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    break;
                }
                
                b_alloc_slice = TRUE;
                pt_ring_buf->ui4_slice_cap_dft = ui4_capacity_got;
            }
            else
            {
                /*get backup slice buf*/
                pt_slice = pt_ring_buf->pt_removed_slice_buf;
                pt_ring_buf->pt_removed_slice_buf = NULL;
                
                i4_ret = _rec_util_grb_reset_slice_buf(pt_slice);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR(("_rec_util_grb_add_slice_buf reset fail.\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    break;
                } 
            }   
            
            ++(pt_ring_buf->ui4_slice_num);
            DLIST_INSERT_TAIL(pt_slice,&(pt_ring_buf->t_slice_buf_list),t_link);
            i4_ret = REC_UTILR_GRB_OK;
        }
    }while(0);

    if(REC_UTILR_GRB_OK == i4_ret)
    {
        if (NULL != ppt_new_slice)
        {
            *ppt_new_slice = pt_slice;
        }
    }   
    else
    {
        if(b_alloc_slice)
        {
            _rec_util_grb_free_slice_buf (pt_ring_buf->e_mem_mode, 
                                          &pt_slice);
        }
    }
    
    return i4_ret;
}


/*remove one slice buf from the head of ring buf*/
static INT32 _rec_util_grb_remove_head_slice  (REC_UTIL_GRB_T* pt_ring_buf)
{
    REC_UTIL_GRB_SLICE_BUF_T*   pt_slice = NULL;
    INT32                       i4_ret;
    
    if(NULL == pt_ring_buf)
    {
        REC_UTIL_DBG_ERR(("_rec_util_grb_remove_head_slice:param invalid .pt_ring_buf 0x%x\r\n", (UINT32)pt_ring_buf));
        return REC_UTILR_GRB_FAIL;
    }

    do
    { 
        pt_slice = DLIST_HEAD(&(pt_ring_buf->t_slice_buf_list));
        
        if(NULL != pt_slice)
        {
            if(pt_ring_buf->ui4_slice_num > 1)
            {
                /*slice num > 1 , to remove*/
                DLIST_REMOVE(pt_slice,&(pt_ring_buf->t_slice_buf_list),t_link);
                --(pt_ring_buf->ui4_slice_num);

                
                if( (NULL != pt_ring_buf->pt_removed_slice_buf))
                {
                    if((pt_ring_buf->pt_removed_slice_buf->ui4_capacity < 
                        pt_slice->ui4_capacity))
                    {
                        /*the backup slice capacity < current one, so free backup slice*/
                        i4_ret = _rec_util_grb_free_slice_buf(pt_ring_buf->e_mem_mode,
                                                              &(pt_ring_buf->pt_removed_slice_buf));
                        if(REC_UTILR_GRB_OK != i4_ret)
                        {
                            REC_UTIL_DBG_ERR((  "_rec_util_grb_remove_head_slice:free "
                                            "slice buf error.\r\n"));
                            i4_ret = REC_UTILR_GRB_FAIL;
                            break;
                        }
                        pt_ring_buf->pt_removed_slice_buf = pt_slice;
                    }
                    else
                    {
                        i4_ret = _rec_util_grb_free_slice_buf(pt_ring_buf->e_mem_mode,
                                                              &(pt_slice));
                        if(REC_UTILR_GRB_OK != i4_ret)
                        {
                            REC_UTIL_DBG_ERR((  "_rec_util_grb_remove_head_slice:free "
                                            "slice buf error.\r\n"));
                            i4_ret = REC_UTILR_GRB_FAIL; 
                        }
                        else
                        {
                            i4_ret = REC_UTILR_GRB_OK;
                        }
                        break;
                    }
                }
                else
                {
                    pt_ring_buf->pt_removed_slice_buf = pt_slice;
                    i4_ret = REC_UTILR_GRB_OK;
                    break;
                }
                
            }
            else
            {
                /*slice num ==1 , reset current slice*/
                i4_ret = _rec_util_grb_reset_slice_buf(pt_slice);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                   REC_UTIL_DBG_ERR(("_rec_util_grb_remove_head_slice reset fail.\r\n"));
                   i4_ret = REC_UTILR_GRB_FAIL;
                   break;
                } 
            }
        }
        else
        {
            REC_UTIL_DBG_ERR(("_rec_util_grb_remove_head_slice head slice null.\r\n"));
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
    }while(0);
    
    return i4_ret;
}

/*to calc the write pos for certain data length*/
static INT32 _rec_util_grb_get_valid_write_ptr(
    REC_UTIL_GRB_T*                 pt_ring_buf,
    UINT32                          ui4_size,
    UINT8**                         ppui1_ptr)
{
    INT32                       i4_ret;
    UINT32                      ui4_left_size       = 0;
    UINT8*                      pui1_write          = NULL;
    REC_UTIL_GRB_SLICE_BUF_T*   pt_slice            = NULL;
    
    if(NULL == pt_ring_buf || NULL == ppui1_ptr)
    {
        REC_UTIL_DBG_ERR((  "_rec_util_grb_get_valid_write_ptr:param invalid\r\n"));
                                            
        return REC_UTILR_GRB_FAIL;
    }
    
    pt_slice = DLIST_TAIL(&(pt_ring_buf->t_slice_buf_list));
    
    do
    {    
        /*in default, the max data size should be contained by two bytes*/
        if(ui4_size >= 0xFFFF || 0 == ui4_size)
        {
            REC_UTIL_DBG_ERR((  "_rec_util_grb_get_valid_write_ptr: entry size 0x%x"
                                " great than 0xFFFF", ui4_size));
                                
            i4_ret = REC_UTILR_GRB_FAIL;
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_NOT_WRITE_DONE);
            break;
        }
        
        pui1_write = pt_slice->pui1_write_ptr;

        /*left space in tail slice*/
        ui4_left_size = pt_slice->ui4_capacity - 
           ((UINT32)pt_slice->pui1_write_ptr - (UINT32)pt_slice->pui1_start_ptr);

        if(ui4_left_size >= ui4_size + REC_UTIL_GRB_ENTRY_OVERHEAD )
        {
            (*ppui1_ptr) = pui1_write;
            i4_ret = REC_UTILR_GRB_OK;
            break;
        }
        
        /*if the left size is insufficent , discard the left space with 0xFF , and request new slice*/
        if(0 != pt_slice->ui4_entry_num)
        {
            pt_slice->e_slice_status = REC_UTIL_SLICE_BUF_FULL;
            x_memset(pui1_write , 0xFF , ui4_left_size);
        }

        i4_ret = _rec_util_grb_add_slice_buf(pt_ring_buf , 
            (UINT32)ui4_size + REC_UTIL_GRB_ENTRY_OVERHEAD, &pt_slice);

        if(REC_UTILR_GRB_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("_rec_util_grb_get_valid_write_ptr: new slice buf "
                              "failed!\r\n"));
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
        
        (*ppui1_ptr) = pt_slice->pui1_write_ptr;

        if((UINT32)ui4_size + REC_UTIL_GRB_ENTRY_OVERHEAD  > pt_slice->ui4_capacity)
        {
            REC_UTIL_DBG_ERR(("rec util _rec_util_grb_get_valid_write_ptr :entry size 0x%x , slice size 0x%x.\r\n" , 
                (UINT32)ui4_size + REC_UTIL_GRB_ENTRY_OVERHEAD  ,pt_slice->ui4_capacity ));
            (*ppui1_ptr) = NULL;
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
        
    }while(0);
        
    return i4_ret;
}

/*Public API Implementation*/
INT32 rec_util_grb_init(
    REC_UTIL_GRB_HANDLE_T*          ph_grb,
    REC_UTIL_GRB_MEM_MODE_T         e_mem_mode,
    UINT32                          ui4_initial_capacity)
{
    INT32           i4_ret;
    REC_UTIL_GRB_T* pt_ring_buf     = NULL;
    BOOL            b_alloc_slice   = FALSE;
    
    if(NULL == ph_grb)
    {
        REC_UTIL_DBG_ERR(("rec_util_grb_init invalid param\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }
    
    do
    {
        pt_ring_buf = (REC_UTIL_GRB_T*)x_mem_alloc(sizeof(REC_UTIL_GRB_T));
        if(NULL == pt_ring_buf)
        {
            REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_init alloc mem error\r\n"));
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
        
        DLIST_INIT(&(pt_ring_buf->t_slice_buf_list));
        DLIST_INIT(&(pt_ring_buf->t_entry_list));

        pt_ring_buf->e_mem_mode             = e_mem_mode;
        pt_ring_buf->pt_removed_slice_buf   = NULL;
        pt_ring_buf->ui4_slice_cap_dft      = (ui4_initial_capacity > 64)? ui4_initial_capacity : 64;
        pt_ring_buf->ui4_slice_num          = 0;
        pt_ring_buf->ui4_entry_num          = 0;
    
        if(REC_UTILR_GRB_OK != _rec_util_grb_add_slice_buf(pt_ring_buf , 
                                                           pt_ring_buf->ui4_slice_cap_dft,
                                                           NULL))
        {
            REC_UTIL_DBG_ERR((  "{rec_util} inital ring buf failed .pt_ring_buf 0x%x \r\n", 
                                (UINT32)pt_ring_buf));
            
            x_mem_free(pt_ring_buf);
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
        b_alloc_slice = TRUE;
        /*create a lock*/
        /*
        if (x_sema_create(&(pt_ring_buf->h_lock),
                          X_SEMA_TYPE_BINARY, 
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
        }*/
        (*ph_grb) = (REC_UTIL_GRB_HANDLE_T)pt_ring_buf;
        i4_ret = REC_UTILR_GRB_OK;
        
    }while(0);

    if(REC_UTILR_GRB_OK != i4_ret)
    {
        if(b_alloc_slice)
        {
            _rec_util_grb_remove_head_slice(pt_ring_buf);
        }
    }
    return i4_ret;
}


VOID rec_util_grb_deinit(
    REC_UTIL_GRB_HANDLE_T  h_grb)
{
    REC_UTIL_GRB_T*           pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_SLICE_BUF_T* pt_slice = NULL;    
        
    if(NULL == pt_ring_buf)
    {
        REC_UTIL_DBG_ERR((  "{rec_util} rec_util_grb_deinit ring buf point" 
                            " invalid.pt_ring_buf 0x%x\r\n", (UINT32)pt_ring_buf));
        return ;
    }

   
    do
    {
        if(NULL!= pt_ring_buf->pt_removed_slice_buf)
        {
            DLIST_INSERT_HEAD(pt_ring_buf->pt_removed_slice_buf,
                              &(pt_ring_buf->t_slice_buf_list),t_link);
            pt_ring_buf->pt_removed_slice_buf = NULL;
        }
        
        while(NULL != (pt_slice = DLIST_HEAD(&pt_ring_buf->t_slice_buf_list)))
        {
            DLIST_REMOVE(pt_slice,&pt_ring_buf->t_slice_buf_list,t_link);
            if(REC_UTILR_GRB_OK != _rec_util_grb_free_slice_buf(pt_ring_buf->e_mem_mode,
                                                                &pt_slice))
            {
                REC_UTIL_DBG_ERR((  "{rec_util} rec_util_grb_deinit free slice " 
                                    "buf error\r\n"));
                REC_UTIL_ABORT(0);
                break;
            }
        }
        
        DLIST_INIT(&(pt_ring_buf->t_slice_buf_list));
        DLIST_INIT(&(pt_ring_buf->t_entry_list));
        pt_ring_buf->ui4_slice_cap_dft = 0;
        pt_ring_buf->ui4_slice_num     = 0;
        pt_ring_buf->ui4_entry_num     = 0;
        /*
            if (pt_ring_buf->h_lock != NULL_HANDLE)
            {
                 x_sema_delete(pt_ring_buf->h_lock);
                 pt_ring_buf->h_lock = NULL_HANDLE;
             }*/
        x_mem_free(pt_ring_buf);
    }while(0);
    
    return;
}

/*
extern VOID rec_util_grb_reset(
    REC_UTIL_GRB_HANDLE_T  h_grb)
{
    REC_UTIL_GRB_T* pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;

    UINT32 ui4_capacity = 0;
    if(NULL == pt_ring_buf)
    {
        REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_reset ring buf point invalid .pt_ring_buf 0x%x\r\n", 
                (UINT32)pt_ring_buf));
        return ;
    }

    ui4_capacity = pt_ring_buf->ui4_slice_cap_dft;
    rec_util_grb_deinit(h_grb);
    rec_util_grb_init(&h_grb, ui4_capacity);
}
*/

INT32 rec_util_grb_write_data(
    REC_UTIL_GRB_HANDLE_T              h_grb,
    VOID*                              pv_data,
    UINT32                             ui4_data_sz)
{
    INT32               i4_ret = REC_UTILR_GRB_OK;
    REC_UTIL_GRB_DATA_T t_data;

    if(NULL == pv_data || 0 == ui4_data_sz)
    {
        REC_UTIL_DBG_ERR((  "rec_util_grb_write_data param invalid\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }

    do
    {
        i4_ret = rec_util_grb_alloc_entry(h_grb, ui4_data_sz, &t_data);
        if(REC_UTILR_GRB_OK == i4_ret)
        {
            i4_ret = rec_util_grb_set_entry(h_grb, &t_data, 0, pv_data, ui4_data_sz);
            if(REC_UTILR_GRB_OK != i4_ret)
            {
                REC_UTIL_DBG_ERR((  "rec_util_grb_write_data set entry fail\r\n"));
                i4_ret = REC_UTILR_GRB_FAIL;
                break;
            }
        }
        else
        {
            REC_UTIL_DBG_ERR((  "rec_util_grb_write_data alloc entry fail\r\n"));
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
    }while(0);
    return i4_ret;
}


INT32 rec_util_grb_alloc_entry(
    REC_UTIL_GRB_HANDLE_T      h_grb,
    UINT32                     ui4_data_sz,
    REC_UTIL_GRB_DATA_T*       pt_data)
{
    INT32                      i4_ret;
    REC_UTIL_GRB_T*            pt_ring_buf      = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_SLICE_BUF_T*  pt_slice         = NULL;
    REC_UTIL_GRB_DATA_ENTRY_T* pt_entry         = NULL;
    UINT8*                     pui1_write       = NULL;
    UINT32                     ui4_align_len    = 0;
    
    if(NULL == pt_ring_buf || NULL == pt_data || 0 == ui4_data_sz)
    {
        REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_malloc_write_entry param invalid.\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }

    /*REC_UTIL_LOCK(pt_ring_buf->h_lock);*/

    
    do
    {
        ui4_align_len = ((ui4_data_sz+3)/4)* 4;
        /* = 0  , valid write entry and no entries were discarded ;
                < 0 , invlid write entry ; */
        i4_ret = _rec_util_grb_get_valid_write_ptr(pt_ring_buf, 
                                                    ui4_align_len, 
                                                    &pui1_write);
        if(REC_UTILR_GRB_OK != i4_ret)
        {
            REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_malloc_write_entry data "
                                 "size 0x%x too large.\r\n", ui4_data_sz));
            i4_ret = REC_UTILR_GRB_FAIL;
            break;
        }
                            
        pt_slice = DLIST_TAIL(&(pt_ring_buf->t_slice_buf_list));
        pt_entry = (REC_UTIL_GRB_DATA_ENTRY_T*)(pui1_write);
        pt_entry->ui2_total_length = (UINT16)ui4_align_len;
        pt_entry->ui2_data_length = (UINT16)ui4_data_sz;
        pui1_write += REC_UTIL_GRB_ENTRY_OVERHEAD ;
        
        /*add entry to entry list*/    
        DLIST_INSERT_TAIL(pt_entry,&pt_ring_buf->t_entry_list,t_link);
        ++(pt_slice->ui4_entry_num);
        ++(pt_ring_buf->ui4_entry_num);
                            
        pt_data->pv_data = (VOID*)((UINT8*)(pt_entry) + REC_UTIL_GRB_ENTRY_OVERHEAD);
        pt_data->ui4_data_sz = pt_entry->ui2_data_length;
        pt_data->pv_tag = (VOID*)pt_entry;
        pt_slice->pui1_write_ptr += (ui4_align_len + REC_UTIL_GRB_ENTRY_OVERHEAD );

        i4_ret = REC_UTILR_GRB_OK;
    }while(0);
    /*REC_UTIL_UNLOCK(pt_ring_buf->h_lock);*/
    return i4_ret;

}


INT32 rec_util_grb_set_entry(
    REC_UTIL_GRB_HANDLE_T             h_grb,
    REC_UTIL_GRB_DATA_T*              pt_data,
    UINT32                            ui4_ofst,
    VOID*                             pv_data,
    UINT32                            ui4_data_sz)
{
    INT32                       i4_ret;
    REC_UTIL_GRB_T*             pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_DATA_ENTRY_T*  pt_entry    = (REC_UTIL_GRB_DATA_ENTRY_T*)(pt_data->pv_tag);
    UINT8*                      pui1_write  = NULL;
    
    if(NULL == pt_ring_buf || NULL == pt_entry || NULL == pv_data )
    {
        REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_set_entry param invalid.\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }

    /*REC_UTIL_LOCK(pt_ring_buf->h_lock);*/
   
    do
    {
        if((ui4_ofst + ui4_data_sz) > (UINT32)(pt_entry->ui2_data_length))
        {
            REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_set_entry "
                                "REC_UTILR_GRB_WRITE_DATA_OVERRIDE.\r\n"));
            i4_ret = REC_UTILR_GRB_WRITE_DATA_OVERRIDE;
            break;
        }
        pui1_write = (UINT8*)(pt_entry) + REC_UTIL_GRB_ENTRY_OVERHEAD;
        x_memcpy(pui1_write + ui4_ofst, pv_data, ui4_data_sz);

        i4_ret = REC_UTILR_GRB_OK;
    }while(0);
    /*REC_UTIL_UNLOCK(pt_ring_buf->h_lock);*/
    
    return i4_ret;
    
}

INT32 rec_util_grb_free_head_entry(
    REC_UTIL_GRB_HANDLE_T      h_grb)

{
    INT32                       i4_ret;
    REC_UTIL_GRB_T*             pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_SLICE_BUF_T*   pt_slice    = NULL;
    REC_UTIL_GRB_DATA_ENTRY_T*  pt_entry    = NULL;
    
    UINT32 ui4_align_len = 0;
    
    if(NULL == pt_ring_buf || NULL == DLIST_HEAD(&(pt_ring_buf->t_slice_buf_list)))
    {
        REC_UTIL_DBG_ERR(("rec_util_grb_free_head_entry param invalid.\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }

    /*REC_UTIL_LOCK(pt_ring_buf->h_lock);*/
    do
    {
        if(0 == pt_ring_buf->ui4_entry_num)
        {
            REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_free_head_entry has no data\r\n"));
            i4_ret = REC_UTILR_GRB_OK;
            break;
        }
        pt_slice = DLIST_HEAD(&(pt_ring_buf->t_slice_buf_list));

        /*there are valid entry*/
        if(pt_slice->ui4_entry_num > 0)
        {
            pt_entry = DLIST_HEAD(&pt_ring_buf->t_entry_list);
            if(NULL == pt_entry)
            {
                REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_free_head_entry t_entry_list empty\r\n"));
                i4_ret = REC_UTILR_GRB_FAIL;
                break;
            }
            
            if(0 == pt_slice->ui4_entry_num)
            {
                if(1 == pt_ring_buf->ui4_slice_num)
                {
                    REC_UTIL_DBG_ERR(( "{rec_util} rec_util_grb_free_head_entry "
                                       "head slice empty\r\n"));
                    i4_ret = REC_UTILR_GRB_OK;
                    break;
                }
                else
                {
                    REC_UTIL_DBG_ERR((  "rec_util_grb_free_head_entry first slice "
                                        "can't be empty.\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    REC_UTIL_ABORT(0);
                    break;
                }
            }

            DLIST_REMOVE(pt_entry,&pt_ring_buf->t_entry_list,t_link);
       
            --(pt_slice->ui4_entry_num);
            --(pt_ring_buf->ui4_entry_num);            

            /*if current slice is empty , remove this head slice but can't free memory , for the caller might use the entry*/
            if(0 == pt_slice->ui4_entry_num)
            {
                i4_ret = _rec_util_grb_remove_head_slice( pt_ring_buf);
                if(REC_UTILR_GRB_OK != i4_ret)
                {
                    REC_UTIL_DBG_ERR((  "rec_util_grb_free_head_entry "
                                        "remove head slice fail!\r\n"));
                    i4_ret = REC_UTILR_GRB_FAIL;
                    break;
                }
            }
            else
            {    
                ui4_align_len = pt_entry->ui2_total_length;
                pt_slice->pui1_read_ptr += (ui4_align_len + (UINT16)REC_UTIL_GRB_ENTRY_OVERHEAD );
            }
            i4_ret = REC_UTILR_GRB_OK;
            break;
        }
        else
        {
            REC_UTIL_DBG_INFO((  "rec_util_grb_free_head_entry "
                                 "head slice list empty!\r\n"));
            i4_ret = REC_UTILR_GRB_OK;
            break;
        }
    }while(0);
    /*REC_UTIL_UNLOCK(pt_ring_buf->h_lock);*/
    return i4_ret;
}

INT32 rec_util_grb_seek_next_entry(
    REC_UTIL_GRB_HANDLE_T           h_grb,
    BOOL                            b_backward,
    REC_UTIL_GRB_DATA_T*            pt_data)
{
    REC_UTIL_GRB_T*             pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_DATA_ENTRY_T*  pt_entry    = NULL;
    
    INT32                       i4_ret;

    if(NULL == pt_ring_buf || NULL == pt_data)
    {
        REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_seek_entry param invalid.\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }

    /*REC_UTIL_LOCK(pt_ring_buf->h_lock);*/
    do{
        pt_entry = (REC_UTIL_GRB_DATA_ENTRY_T*)(pt_data->pv_tag);
        if(NULL == pt_entry)
        {
            if(!b_backward)
            {
                pt_entry = DLIST_HEAD(&pt_ring_buf->t_entry_list);
            }
            else
            {
                pt_entry = DLIST_TAIL(&pt_ring_buf->t_entry_list);
            }
        }
        else
        {
            if(!b_backward)
            {
                pt_entry = DLIST_NEXT(pt_entry,t_link);
            }
            else
            {
                pt_entry = DLIST_PREV(pt_entry,t_link);
            }
        }

        if(NULL == (pt_entry))
        {
            i4_ret = REC_UTILR_GRB_SEEK_ENTRY_FINISHED;
            pt_data->pv_tag = (VOID*)REC_UTIL_GRB_NULL_DATA;
            break;
        }        
        pt_data->pv_data        = (VOID*)((UINT8*)(pt_entry) + REC_UTIL_GRB_ENTRY_OVERHEAD);
        pt_data->ui4_data_sz    = pt_entry->ui2_data_length;
        pt_data->pv_tag         = (VOID*)pt_entry;
        
        i4_ret = REC_UTILR_GRB_OK;
    }while(0);
    /*REC_UTIL_UNLOCK(pt_ring_buf->h_lock);*/
    return i4_ret;
}




INT32 rec_util_grb_get_head_entry(
    REC_UTIL_GRB_HANDLE_T           h_grb,
    REC_UTIL_GRB_DATA_T*            pt_data)
{
    REC_UTIL_GRB_T*             pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_DATA_ENTRY_T*  pt_entry    = NULL;
    INT32                       i4_ret;
    
    if(NULL == pt_ring_buf || NULL == pt_data)
    {
        REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_get_head_entry param invalid.\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }

    /*REC_UTIL_LOCK(pt_ring_buf->h_lock);*/
    do{
        (pt_entry)= DLIST_HEAD(&(pt_ring_buf->t_entry_list));
        if(NULL == (pt_entry))
        {
            i4_ret = REC_UTILR_GRB_SEEK_ENTRY_FINISHED;
            pt_data->pv_tag = (VOID*)REC_UTIL_GRB_NULL_DATA;
            break;
        }        
        pt_data->pv_data        = (VOID*)((UINT8*)(pt_entry) + REC_UTIL_GRB_ENTRY_OVERHEAD);
        pt_data->ui4_data_sz    = pt_entry->ui2_data_length;
        pt_data->pv_tag         = (VOID*)pt_entry;

        i4_ret = REC_UTILR_GRB_OK;
    }while(0);
    /*REC_UTIL_UNLOCK(pt_ring_buf->h_lock);*/
    return i4_ret;
}


INT32 rec_util_grb_get_tail_entry(
    REC_UTIL_GRB_HANDLE_T           h_grb,
    REC_UTIL_GRB_DATA_T*            pt_data)
{
    REC_UTIL_GRB_T*                 pt_ring_buf = (REC_UTIL_GRB_T*)h_grb;
    REC_UTIL_GRB_DATA_ENTRY_T*      pt_entry    = NULL;
    INT32                           i4_ret;
    
    if(NULL == pt_ring_buf || NULL == pt_data)
    {
        REC_UTIL_DBG_ERR(("{rec_util} rec_util_grb_get_head_entry param invalid.\r\n"));
        return REC_UTILR_GRB_INV_ARG;
    }
    
    /*REC_UTIL_LOCK(pt_ring_buf->h_lock);*/
    do{
        (pt_entry)= DLIST_TAIL(&(pt_ring_buf->t_entry_list));
        if(NULL == (pt_entry))
        {
            i4_ret = REC_UTILR_GRB_SEEK_ENTRY_FINISHED;
            pt_data->pv_tag = (VOID*)REC_UTIL_GRB_NULL_DATA;
            break;
        }        
        pt_data->pv_data        = (VOID*)((UINT8*)(pt_entry) + REC_UTIL_GRB_ENTRY_OVERHEAD);
        pt_data->ui4_data_sz    = pt_entry->ui2_data_length;
        pt_data->pv_tag         = (VOID*)pt_entry;

        i4_ret = REC_UTILR_GRB_OK;
    }while(0);
    /*REC_UTIL_UNLOCK(pt_ring_buf->h_lock);*/

    return i4_ret;

}

INT32 rec_util_grb_get_data_addr (
    REC_UTIL_GRB_HANDLE_T        h_grb,
    const REC_UTIL_GRB_DATA_T*   pt_data,
    VOID**                       ppv_data_phy,
    VOID**                       ppv_data_virtual)
{
    REC_UTIL_GRB_T*            pt_ring_buf = (REC_UTIL_GRB_T *) h_grb;
    REC_UTIL_GRB_SLICE_BUF_T*  pt_slice;
    BOOL                       b_found;
    INT32                      i4_ret;

    if (NULL == pt_data)
    {
        REC_UTIL_DBG_ERR(("rec_util_grb_get_data_addr(): pt_data is null!\r\n"));
        return REC_UTILR_GRB_INV_ARG;
        
    }
    b_found = FALSE;
    DLIST_FOR_EACH (pt_slice, & pt_ring_buf->t_slice_buf_list, t_link)
    {
        if ( ((UINT32) pt_data->pv_data) >= ((UINT32) pt_slice->pui1_start_ptr) && 
             ((UINT32) pt_data->pv_data) < (((UINT32) pt_slice->pui1_start_ptr) + pt_slice->ui4_capacity))
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (b_found)
    {    
        if (ppv_data_phy != NULL)
        {
            *ppv_data_phy = (VOID *) (((UINT32) pt_data->pv_data) - ((UINT32) pt_slice->pui1_start_ptr) + 
                                         ((UINT32) pt_slice->pui1_start_ptr_phy));
        }
        
        if (ppv_data_virtual != NULL)
        {
            *ppv_data_virtual = pt_data->pv_data;
        }
        
        i4_ret = REC_UTILR_GRB_OK;
    }
    else
    {
        REC_UTIL_DBG_ERR(("rec_util_grb_get_data_addr(): param invalid.\r\n"));
        *ppv_data_phy = NULL;
        i4_ret = REC_UTILR_GRB_INV_ARG;
    }
    
    return i4_ret;
}    
