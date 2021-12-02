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
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "handle/handle.h"
#include "playback_handler_cb.h"
#include "dbg/def_dbg_level_mw.h"

PLAYBACK_CB_T* playback_cb_new(UINT32 ui4_size, UINT32  ui4_pre_size, UINT32 ui4_alignment)
{
    PLAYBACK_CB_T* pt_c_buf;

    pt_c_buf = (PLAYBACK_CB_T*) x_mem_alloc(sizeof(PLAYBACK_CB_T));

    if(pt_c_buf == NULL)
    {
        return pt_c_buf;
    }

    pt_c_buf->pui1_alloc = (UINT8*)x_mem_alloc(sizeof(UINT8)*ui4_size+ui4_alignment);

    if (ui4_alignment>0)
    {
        pt_c_buf->pui1_begin = (UINT8*)((((UINT32)pt_c_buf->pui1_alloc+(ui4_alignment-1))/ui4_alignment)*ui4_alignment);
    }
    else
    {
        pt_c_buf->pui1_begin = pt_c_buf->pui1_alloc;
    }
    if(pt_c_buf->pui1_begin == NULL)
    {
        return pt_c_buf;
    }

    pt_c_buf->ui4_size = ui4_size;
    pt_c_buf->pui1_head = pt_c_buf->pui1_begin;
    pt_c_buf->pui1_tail = pt_c_buf->pui1_begin;
    pt_c_buf->ui4_pre_size = ui4_pre_size;
    pt_c_buf->ui4_pre_space = 0;
    return pt_c_buf;
}

VOID playback_cb_delete(PLAYBACK_CB_T* pt_cb)
{
    if (pt_cb)
    {
        if (pt_cb->pui1_alloc)
        {
            x_mem_free(pt_cb->pui1_alloc);
            pt_cb->pui1_alloc = NULL;
        }

        x_mem_free(pt_cb);
    }
}


PLAYBACK_CB_T* playback_cb_reset(PLAYBACK_CB_T* pt_c_buf)
{
    pt_c_buf->pui1_head = pt_c_buf->pui1_begin;
    pt_c_buf->pui1_tail = pt_c_buf->pui1_begin;
    pt_c_buf->ui4_pre_space = 0;
    return pt_c_buf;
}


UINT32 playback_cb_get_head_pos(PLAYBACK_CB_T* pt_c_buf)
{
    return (UINT32)(pt_c_buf->pui1_head - pt_c_buf->pui1_begin);
}

UINT32 playback_cb_get_tail_pos(PLAYBACK_CB_T* pt_c_buf)
{
    return (UINT32)(pt_c_buf->pui1_tail - pt_c_buf->pui1_begin);
}

UINT32 playback_cb_get_free_size(PLAYBACK_CB_T* pt_c_buf)
{
    INT32 free;

    free = pt_c_buf->pui1_head - pt_c_buf->pui1_tail;

    if(free < 0) 
    {
        free = (INT32)(pt_c_buf->ui4_size - playback_cb_get_tail_pos(pt_c_buf) 
                                  + playback_cb_get_head_pos(pt_c_buf));
    }

    /* assume in this case it is ALL FREE instead of ALL FILLED */
    if(free == 0)
    {
        free = (INT32)pt_c_buf->ui4_size;
    }

    if (free >= (INT32)pt_c_buf->ui4_pre_space)
    {
        free -= (INT32)pt_c_buf->ui4_pre_space;
    }
    else
    {
        x_dbg_stmt("!!!! playback_cb_get_free_size data something wrong\n");
        free = 0;
    }

    return (UINT32)free;
}

UINT32 playback_cb_get_filled_size(PLAYBACK_CB_T* pt_c_buf)
{
    INT32 filled;

    filled = pt_c_buf->pui1_tail - pt_c_buf->pui1_head;

    if(filled < 0) 
    {
        filled = (INT32)(pt_c_buf->ui4_size - playback_cb_get_head_pos(pt_c_buf) 
                                    + playback_cb_get_tail_pos(pt_c_buf));
    }

    return (UINT32)filled;
}

CB_STATUS_T playback_cb_insert(PLAYBACK_CB_T* pt_c_buf,
                               UINT8* pui1_data,
                               UINT32 ui4_len)
{
    /* the = sign is to make sure that (pui1_head == pui1_tail) IS ONLY 
       INDICATING ALL FREE, NOT ALL FILLED situation
    */
    if(ui4_len >= playback_cb_get_free_size(pt_c_buf))
    {
        return CB_OVERFLOW;
    }

    /* wrap around case */
    if(playback_cb_get_tail_pos(pt_c_buf)+ui4_len > pt_c_buf->ui4_size)
    {
        UINT32 first_half;

        first_half = pt_c_buf->ui4_size - playback_cb_get_tail_pos(pt_c_buf);
        /* copy data to fill to the end of buffer region */
        x_memcpy(pt_c_buf->pui1_tail, pui1_data, first_half);
        /* copy remaining data to the beginning of buffer region */
        x_memcpy(pt_c_buf->pui1_begin, (pui1_data+first_half), (ui4_len-first_half));

        pt_c_buf->pui1_tail = pt_c_buf->pui1_begin + (ui4_len-first_half);
    }
    else /* normal case */
    {
        x_memcpy(pt_c_buf->pui1_tail, pui1_data, ui4_len);
        pt_c_buf->pui1_tail += ui4_len;
    }

    return CB_OK;
}

CB_STATUS_T playback_cb_add(PLAYBACK_CB_T* pt_c_buf,
                            UINT32 ui4_len)
{
    /* the = sign is to make sure that (pui1_head == pui1_tail) IS ONLY 
       INDICATING ALL FREE, NOT ALL FILLED situation
    */
    if(ui4_len >= playback_cb_get_free_size(pt_c_buf))
    {
        return CB_OVERFLOW;
    }

    /* wrap around case */
    if(playback_cb_get_tail_pos(pt_c_buf)+ui4_len > pt_c_buf->ui4_size)
    {
        UINT32 first_half;

        first_half = pt_c_buf->ui4_size - playback_cb_get_tail_pos(pt_c_buf);
        pt_c_buf->pui1_tail = pt_c_buf->pui1_begin + (ui4_len-first_half);
    }
    else /* normal case */
    {
        pt_c_buf->pui1_tail += ui4_len;
    }

    return CB_OK;
}

extern CB_STATUS_T playback_cb_minus(PLAYBACK_CB_T* pt_c_buf,
                                     UINT32 ui4_len)
{
    /* normal case */
    if (playback_cb_get_head_pos(pt_c_buf)>=ui4_len)
    {
        pt_c_buf->pui1_head -= ui4_len;
    }
    else /* wrap around case */
    {
        UINT32  ui4_left;
        
        ui4_left = ui4_len - playback_cb_get_head_pos(pt_c_buf);
        pt_c_buf->pui1_head = pt_c_buf->pui1_begin + (pt_c_buf->ui4_size-ui4_left);
    }
    
    pt_c_buf->ui4_pre_space -= ui4_len;
    
    return CB_OK;
}
                                     
CB_STATUS_T playback_cb_remove(PLAYBACK_CB_T* pt_c_buf,
                               UINT8* pui1_data,
                               UINT32* pui4_len)
{
    CB_STATUS_T status = CB_OK;

    if(*pui4_len > playback_cb_get_filled_size(pt_c_buf))
    {
        *pui4_len = playback_cb_get_filled_size(pt_c_buf);
        status = CB_UNDERFLOW;
    }

    if(pui1_data == NULL)
    {
        return CB_ERROR;
    }

    /* wrap around case */
    if(playback_cb_get_head_pos(pt_c_buf) + *pui4_len > pt_c_buf->ui4_size)
    {
        UINT32 first_half;

        first_half = pt_c_buf->ui4_size - playback_cb_get_head_pos(pt_c_buf);
        /* removing data from tail to the end of buffer region */
        x_memcpy(pui1_data, pt_c_buf->pui1_head, first_half);
        /* removing remaining data from the beginning of buffer region */
        x_memcpy((pui1_data+first_half), pt_c_buf->pui1_begin, (*pui4_len-first_half));

        pt_c_buf->pui1_head = pt_c_buf->pui1_begin + (*pui4_len-first_half);
    }
    else /* normal case */
    {
        x_memcpy(pui1_data, pt_c_buf->pui1_head, *pui4_len);
        pt_c_buf->pui1_head += *pui4_len;
    }

    pt_c_buf->ui4_pre_space += *pui4_len;
    if (pt_c_buf->ui4_pre_space>pt_c_buf->ui4_pre_size)
    {
        pt_c_buf->ui4_pre_space = pt_c_buf->ui4_pre_size;
    }

    return status;
}

CB_STATUS_T playback_cb_copy(PLAYBACK_CB_T* pt_c_buf,
                             UINT8* pui1_data,
                             UINT32* pui4_len,
                             UINT32 ui4_pos)
{
    CB_STATUS_T status = CB_OK;
    PLAYBACK_CB_T t_c_buf;

    if(pui1_data == NULL)
    {
        return CB_ERROR;
    }

    if (ui4_pos)
    {
        t_c_buf = *pt_c_buf;
        playback_cb_skip(pt_c_buf, &ui4_pos);
    }
                                

    if(*pui4_len > playback_cb_get_filled_size(pt_c_buf))
    {
        *pui4_len = playback_cb_get_filled_size(pt_c_buf);
        status = CB_UNDERFLOW;
    }

    /* wrap around case */
    if(playback_cb_get_head_pos(pt_c_buf) + *pui4_len > pt_c_buf->ui4_size)
    {
        UINT32 first_half;

        first_half = pt_c_buf->ui4_size - playback_cb_get_head_pos(pt_c_buf);
        /* removing data from tail to the end of buffer region */
        x_memcpy(pui1_data, pt_c_buf->pui1_head, first_half);
        /* removing remaining data from the beginning of buffer region */
        x_memcpy((pui1_data+first_half), pt_c_buf->pui1_begin, (*pui4_len-first_half));
    }
    else /* normal case */
    {
        x_memcpy(pui1_data, pt_c_buf->pui1_head, *pui4_len);
    }

    if (ui4_pos)
    {
        *pt_c_buf = t_c_buf;
    }

    return status;
}

CB_STATUS_T playback_cb_skip(PLAYBACK_CB_T* pt_c_buf,
                             UINT32* pui4_len)
{
    CB_STATUS_T status = CB_OK;

    if(*pui4_len > playback_cb_get_filled_size(pt_c_buf))
    {
        *pui4_len = playback_cb_get_filled_size(pt_c_buf);
        status = CB_UNDERFLOW;
    }

    /* wrap around case */
    if(playback_cb_get_head_pos(pt_c_buf) + *pui4_len > pt_c_buf->ui4_size)
    {
        UINT32 first_half;

        first_half = pt_c_buf->ui4_size - playback_cb_get_head_pos(pt_c_buf);
        pt_c_buf->pui1_head = pt_c_buf->pui1_begin + (*pui4_len-first_half);
    }
    else /* normal case */
    {
        pt_c_buf->pui1_head += *pui4_len;
    }
    
    pt_c_buf->ui4_pre_space += *pui4_len;
    if (pt_c_buf->ui4_pre_space>pt_c_buf->ui4_pre_size)
    {
        pt_c_buf->ui4_pre_space = pt_c_buf->ui4_pre_size;
    }
    
    return status;
}

