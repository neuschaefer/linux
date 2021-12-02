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
 * $RCSfile: fm_buf.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/9 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 0e14e9db711671ed7aa83d4108b0f35e $
 *
 * Description: 
 *          This header file contains types and function prototypes for
 *          buffer cache.
 *---------------------------------------------------------------------------*/

#ifndef _FM_BUF_H_
#define _FM_BUF_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_lnk_list.h"
#include "fm_util.h"
#include "fm_dev.h"
#include "fm_ufs.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* In some scenarios, we may allocate 1024 BUFs(512Byte each), therefore there
   will cost 2048 handles, however the system's handle limit is 4096, and it is
   possible to run out of handles!
*/
#define FM_BUF_FORCE_PREFETCH 1

#if 1
#if 0
#define FM_BUF_BLOCK_SHIFT      (12)
#define FM_BUF_BLOCK_SIZE       (1 << FM_BUF_BLOCK_SHIFT) /* 4096 bytes */
#define FM_BUF_CHUNK_SIZE       (FM_BUF_BLOCK_SIZE * 16)  /* 32 KB */
#else
#define FM_BUF_BLOCK_SHIFT(pt_xent)   (_fm_buf_buf_shift(pt_xent))
#define FM_BUF_BLOCK_SIZE(pt_xent)    (1 << FM_BUF_BLOCK_SHIFT(pt_xent)) /* 512 bytes */
#define FM_BUF_CHUNK_SIZE             (128 * 1024)

/* 32 KB: Sony SDivx-020 parsing root directory (UDF): 20 secs
 *        STD200 maximum case test-> SubPath: > 8 secs
 *        STD200 maximum case test-> SubClip_entries: > 8 secs
 * 64 KB: Sony SDivx-020 parsing root directory (UDF): 11 secs
 *        STD200 maximum case test-> SubPath: ~ 7 secs
 *        STD200 maximum case test-> SubClip_entries: ~ 7 secs
 * 128 KB:Sony SDivx-020 parsing root directory (UDF): 11 secs
 *        STD200 maximum case test-> SubPath: < 7 secs
 *        STD200 maximum case test-> SubClip_entries: ~ 7 secs
 *        Mainly for 15688 files under a directory in media center
*/ 
#endif
#else 
/* for read CSS-protected disc case, can not read the encrypted LBA range 
   before the counter measure is taken. */
#define FM_BUF_BLOCK_SHIFT      (11)
#define FM_BUF_BLOCK_SIZE       (1 << FM_BUF_BLOCK_SHIFT) /* 2048 bytes */
#define FM_BUF_CHUNK_SIZE       (FM_BUF_BLOCK_SIZE * 1)  /* 2 KB */
#endif
#ifdef FM_USE_SMALL_MEMORY
#define FM_MAX_BUF_SPACE		    (128 * 1024)
#else
#define FM_MAX_BUF_SPACE        (512 * 1024) /* 512KB */
#endif
#if 0
#define FM_MAX_BUF_BLOCK        (FM_MAX_BUF_SPACE / FM_BUF_CHUNK_SIZE)
#endif
#define FM_BUF_FLUSH_TIME       (10000) /* 500 ms */

#define FM_BUF_LBN(addr, pt_xent)        ((addr) >> FM_BUF_BLOCK_SHIFT(pt_xent))
#define FM_BUF_ADDR(lbn, pt_xent)        ((lbn)  << FM_BUF_BLOCK_SHIFT(pt_xent))
#define FM_BUF_OFFSET(addr, pt_xent)     ((addr) & (FM_BUF_BLOCK_SIZE(pt_xent) - 1))

typedef struct _FM_BUF_T
{
    DLIST_ENTRY_T(_FM_BUF_T) t_hlink;   /* link to next buffer */
    DLIST_ENTRY_T(_FM_BUF_T) t_vlink;   /* link to state chain */

    VOID            *pv_alloc;      /* buffer allocated memroy */
    VOID            *pv_data;       /* buffer data, fit to alignment */
    UINT32          ui4_size;       /* buffer size */
    UINT32          ui4_align;      /* buffer alignment */
    UINT64          ui8_lbn;        /* logic block number into file */
    UINT32          ui4_sec_size;   /* block device sector size */
    UINT32          ui4_resid;      /* remaining I/O for this buffer */
    
    UINT16          ui2_flags;
#define FM_BUF_FLAG_INVAL       ((UINT16)0x0001)    /* Invalid buffer */
#define FM_BUF_FLAG_IO          ((UINT16)0x0002)    /* Performing I/O */
#define FM_BUF_FLAG_DIRTY       ((UINT16)0x0004)    /* Data is dirty */
#define FM_BUF_FLAG_RETRY       ((UINT16)0x0008)    /* Data to be reflush */
#define FM_BUF_FLAG_LOCKED      ((UINT16)0x0010)    /* It is being locked */
#define FM_BUF_FLAG_READ        ((UINT16)0x0020)    /* To read from device */
#define FM_BUF_FLAG_DROP        ((UINT16)0x0040)    /* Drop after I/O done */
#define FM_BUF_FLAG_ASYNC       ((UINT16)0x0100)    /* Async operation */
#define FM_BUF_FLAG_COPY_IN     ((UINT16)0x0200)    /* Async write */
#define FM_BUF_FLAG_IO_DONE     ((UINT16)0x0400)    /* No performing I/O */

    UINT16          ui2_want;
    UINT8           ui1_pri;

    FM_XENTRY_T     *pt_xent; /* device xent */
    /*FM_DEVICE_T     *pt_dev;*/
    HANDLE_T        h_mtx;
    HANDLE_T        h_sync;

    /* For buffer read done callback */
    FM_RW_ARG_T     *pt_rw_req;
    VOID            *pv_copy_addr;  /* User space data address */
    UINT32          ui4_copy_off;   /* The starting offset to copy in buffer */
    UINT32          ui4_copy_size;
} FM_BUF_T;


#define IS_BUF_INVAL(pt_buf)    \
    ((pt_buf)->ui2_flags & FM_BUF_FLAG_INVAL)
    
#define IS_BUF_DIRTY(pt_buf)    \
    ((pt_buf)->ui2_flags & FM_BUF_FLAG_DIRTY)
    
#define IS_BUF_RETRY(pt_buf)    \
    ((pt_buf)->ui2_flags & FM_BUF_FLAG_RETRY)
    
#define IS_BUF_LOCKED(pt_buf)   \
    ((pt_buf)->ui2_flags & FM_BUF_FLAG_LOCKED)
    
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern INT32 fm_buf_read_write(
    FM_RW_ARG_T *pt_rw_req,
    BOOL        b_read,
    BOOL        b_prefetch,
    UINT32      ui4_prefetch_limit);

/* Alias */
#define fm_buf_read(pt_req, b_prefetch, ui4_prefetch_limit) \
    fm_buf_read_write((pt_req), TRUE, b_prefetch, ui4_prefetch_limit)

#define fm_buf_write(pt_req, b_prefetch, ui4_prefetch_limit) \
    fm_buf_read_write((pt_req), FALSE, b_prefetch, ui4_prefetch_limit)
    
extern VOID fm_buf_done_nfy(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data);

#if 0
extern INT32 fm_buf_flush(
    FM_XENTRY_T *pt_xent,
    BOOL        b_sync);
#else
extern INT32 fm_buf_flush(
    FM_XENTRY_T *pt_xent,
    BOOL        b_sync,
    BOOL        b_discard_error);
#endif

extern VOID fm_buf_reclaim(
    FM_XENTRY_T *pt_xent,
    BOOL        b_sync);

extern INT32 fm_buf_init(
    VOID);

#endif /* _FM_BUF_H */
