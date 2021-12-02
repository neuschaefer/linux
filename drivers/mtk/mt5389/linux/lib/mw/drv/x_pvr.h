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
 * $RCSfile: x_pvr.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Personal Video Recorder driver exported definitions.
 *---------------------------------------------------------------------------*/

#ifndef _X_PVR_H_
#define _X_PVR_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "res_mngr/x_rm.h"
#include "inc/x_mm_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------------*/

/* Get operations */
#define PVR_GET_TYPE_CAP_INFO          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  0))
#define PVR_GET_TYPE_CTRL              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))
#define PVR_GET_TYPE_BUFFER_INFO       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  2))
#define PVR_GET_TYPE_BUFFER_STATUS     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  3))
#define PVR_GET_TYPE_RESIDUAL_DATA     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  4))


/* Set operations */
#define PVR_SET_TYPE_INIT_CTRL         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  0))
#define PVR_SET_TYPE_DEINIT_CTRL       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1))
#define PVR_SET_TYPE_NFY_FCT           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2))
#define PVR_SET_TYPE_CTRL             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3)) | RM_SET_TYPE_ARG_NO_REF)
#define PVR_SET_TYPE_CTRL_OUTPUT      ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4)) | RM_SET_TYPE_ARG_NO_REF)
#define PVR_SET_TYPE_UNLOCK_BUFFER     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5))  
#define PVR_SET_TYPE_UNLOCK_METADATA  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  6)) | RM_SET_TYPE_ARG_NO_REF)
#define PVR_SET_TYPE_FLUSH_BUFFER      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  7))
#define PVR_SET_TYPE_EN_PIC_SEARCH    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  8)) | RM_SET_TYPE_ARG_NO_REF)
#define PVR_SET_TYPE_SWAP              (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  9))
#define PVR_SET_TYPE_ENCRYPTION        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10))
#define PVR_SET_TYPE_IV                (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11))

/* notification conditions */
typedef enum _PVR_NFY_COND_T
{
    PVR_COND_ERROR    = -2,
    PVR_COND_OVERFLOW = -1,
    PVR_COND_TICK     =  0
    
} PVR_NFY_COND_T;

/* Overflow reasons ((*x_pvr_nfy_fct)(e_nfy_cond == PVR_COND_OVERFLOW, 
                                      ui4_data => overflow reason) ) */
#define PVR_OVERFLOW_REASON_TS_BUF_FULL         ((UINT32) 1)
#define PVR_OVERFLOW_REASON_METADATA_BUF_FULL   ((UINT32) 2)

/* Packet swap type */
typedef enum _PVR_SWAP_TYPE_T
{
    PVR_SWAP_TYPE_DISABLE = 0,
    PVR_SWAP_TYPE_NULL_PKT,
    PVR_SWAP_TYPE_SECT_LIST
    
}   PVR_SWAP_TYPE_T;

/* Frame type mask (PVR_CTRL_CONFIG_T.ui1_nfy_frm_mask) */
#define PVR_FRAME_TYPE_MASK_I       MAKE_BIT_MASK_8(0)
#define PVR_FRAME_TYPE_MASK_P       MAKE_BIT_MASK_8(1)
#define PVR_FRAME_TYPE_MASK_B       MAKE_BIT_MASK_8(2)

/* (*x_pvr_nfy_fct) */
typedef BOOL (*x_pvr_nfy_fct) (VOID*              pv_tag,
                               PVR_NFY_COND_T     e_nfy_cond,
                               UINT32             ui4_data);


/*-----------------------------------------------------------------------------
                    structures
-----------------------------------------------------------------------------*/
typedef struct _PVR_CAP_INFO_T
{
    UINT32      ui4_align_buf_start;
    UINT32      ui4_align_buf_size;
    UINT32      ui4_max_pid_num;
    UINT32      ui4_max_encrypt_pid_num;
    UINT32      ui4_max_encrypt_key_num;
    
} PVR_CAP_INFO_T;    

/* PVR Ctrl initial config (PVR_SET_TYPE_INIT_CTRL) */
typedef struct _PVR_CTRL_CONFIG_T
{
    UINT32              ui4_tick_period;
    UINT8               ui1_nfy_frm_mask;
    SIZE_T              z_buf_size;
    VOID*               pv_buf_phy;
    VOID*               pv_buf_mw;
    
} PVR_CTRL_CONFIG_T;
    
/* Notify info setting (PVR_SET_TYPE_NFY_FCT) */
typedef struct _PVR_NFY_INFO_T
{
    VOID*               pv_tag;
    x_pvr_nfy_fct       pf_nfy;
    
} PVR_NFY_INFO_T;

/* Driver pre-allocated buffer info structure (PVR_GET_TYPE_BUFFER_INFO) */
typedef struct _PVR_BUFFER_INFO_T
{
    UINT8*      pui1_buf_addr;
    SIZE_T      z_buf_len;
    
} PVR_BUFFER_INFO_T;

/* Buffer status info structure (PVR_GET_TYPE_BUFFER_STATUS) */
typedef struct _PVR_BUFFER_STATUS_T
{
    const UINT8*      pui1_write_ptr;
    const UINT8*      pui1_read_ptr;
    
} PVR_BUFFER_STATUS_T;    

/* Residual data without notifying in driver info structure (PVR_GET_TYPE_RESIDUAL_DATA) */
typedef struct _PVR_RESIDUAL_DATA_INFO_T
{
    UINT8       *pui1_addr;
    SIZE_T      z_len;
    
}   PVR_RESIDUAL_DATA_INFO_T;

/* Unlock buffer info (PVR_SET_TYPE_UNLOCK_BUFFER) */
typedef struct _PVR_UNLOCK_BUFFER_INFO_T
{
    UINT8*      pui1_data_addr;
    SIZE_T      z_data_len;
    
} PVR_UNLOCK_BUFFER_INFO_T;

/* Section data */
typedef struct _PVR_SECT_DATA_T
{
    VOID*       pv_sect_data;
    SIZE_T      z_sect_len;
    
} PVR_SECT_DATA_T;    

/* Swap data when PVR_SWAP_INFO_T.e_swap_type == PVR_SWAP_TYPE_SECT_LIST */
typedef struct _PVR_SWAP_SECT_LIST_T
{
    UINT16                   ui2_num_sect;
    PVR_SECT_DATA_T*         at_sects;
    
} PVR_SWAP_SECT_LIST_T;

/* Packet swap info structure (PVR_SET_TYPE_SWAP) */
typedef struct _PVR_SWAP_INFO_T
{
    PVR_SWAP_TYPE_T          e_swap_type;
    
    union
    {
        /* PVR_SWAP_TYPE_DISABLE */
        /* nothing */
        
        /* PVR_SWAP_TYPE_NULL_PKT */
        /* nothing */
        
        /* PVR_SWAP_TYPE_SECT_LIST */
        PVR_SWAP_SECT_LIST_T    t_sect_list;
    } u;
    
}   PVR_SWAP_INFO_T;

/* Metadata info */
typedef struct _PVR_METADATA_T
{
    PVR_TICK_DETAIL_T        t_tick_detail;
    
} PVR_METADATA_T;

/* Tick data info (*x_pvr_nfy_fct)(PVR_COND_TICK, ui4_data->(PVR_TICK_DATA_T *)) */
typedef struct _PVR_TICK_DATA_T
{
    VOID*                    pv_metadata_tag;
    const PVR_METADATA_T*    pt_metadata;
    const UINT8*             pui1_data;
    UINT32                   ui4_pkt_count;

} PVR_TICK_DATA_T;


#endif /* _X_PVR_H_ */


