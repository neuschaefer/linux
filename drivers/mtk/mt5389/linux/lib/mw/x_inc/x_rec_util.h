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
 * $RCSfile: x_rec_util.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/5 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: e5a2644d77eac094a5d2aa1496612a57 $
 *
 * Description:
 *         This contains Record Utility exported macros, definitons,
 *         and structures.
 *---------------------------------------------------------------------------*/

#ifndef _X_REC_UTIL_H_
#define _X_REC_UTIL_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
    
   
/*-----------------------------------------------------------------------------
                        structures
-----------------------------------------------------------------------------*/
    
/* parameter for PVR buffer allocation */
typedef struct _REC_UTIL_PBA_PARAM_T
{
    VOID*                       pv_rec_purpose;
    BRDCST_TYPE_T               e_brdcst_type;
    BRDCST_MEDIUM_T             e_brdcst_medium;
    UINT32                      ui4_mem_align;
    SIZE_T                      z_size;
    
} REC_UTIL_PBA_PARAM_T;

/* allocated PVR buffer info */
typedef struct _REC_UTIL_PBA_BUF_INFO_T
{
    UINT32          ui4_buf_id;
    VOID*           pv_buf;
    SIZE_T          z_size;
    
} REC_UTIL_PBA_BUF_INFO_T;    

/* bitrate map items */
typedef struct _REC_UTIL_BITRATE_MAP_ITEM_T
{
    BRDCST_TYPE_T       e_brdcst_type;
    BRDCST_MEDIUM_T     e_brdcst_medium;
    UINT32              ui4_byte_per_second;
    
} REC_UTIL_BITRATE_MAP_ITEM_T;
/*-----------------------------------------------------------------------------
 * Name: (*rec_util_pvr_buf_query_avail_fct)
 *
 * Description: This callback querys available PVR buffer size.
 *              
 * Inputs:  pv_tag       Contains the tag from REC_UTIL_SYS_CONFIG_T.pv_tag_pvr_buf_cb
 *          pt_param     Contains parameters for buffer allocation
 *      
 * Outputs: pz_max_size  Contains available PVR buffer size.
 *
 * Returns: TRUE         Success to query this info;
 *          FALSE        Fail to query this info.
 ----------------------------------------------------------------------------*/
typedef BOOL (*rec_util_pvr_buf_query_avail_fct) (
    VOID*                           pv_tag,
    const REC_UTIL_PBA_PARAM_T*     pt_param,
    SIZE_T*                         pz_max_size );

/*-----------------------------------------------------------------------------
 * Name: (*rec_util_pvr_buf_alloc_fct)
 *
 * Description: This callback requests to allocate a PVR buffer.
 *              
 * Inputs:  pv_tag       Contains the tag from REC_UTIL_SYS_CONFIG_T.pv_tag_pvr_buf_cb
 *          pt_param     Contains parameters for buffer allocation
 *      
 * Outputs: pt_buf_info  Contains allocated buffer info.
 *
 * Returns: TRUE         Success to allocate buffer.
 *          FALSE        Fail to allocate buffer.
 ----------------------------------------------------------------------------*/
typedef BOOL (*rec_util_pvr_buf_alloc_fct) (
    VOID*                           pv_tag,
    const REC_UTIL_PBA_PARAM_T*     pt_param,
    REC_UTIL_PBA_BUF_INFO_T*        pt_buf_info);

/*-----------------------------------------------------------------------------
 * Name: (*rec_util_pvr_buf_free_fct)
 *
 * Description: This callback requests to allocate a PVR buffer.
 *              
 * Inputs:  pv_tag       Contains the tag from REC_UTIL_SYS_CONFIG_T.pv_tag_pvr_buf_cb
 *          pt_buf_info  Contains allocated buffer info.
 *      
 * Outputs: -
 *
 * Returns: TRUE         Success to free buffer.
 *          FALSE        Fail to free buffer.
 ----------------------------------------------------------------------------*/
typedef BOOL (*rec_util_pvr_buf_free_fct) (
    VOID*                           pv_tag,
    const REC_UTIL_PBA_BUF_INFO_T*  pt_buf_info);
    
/* PVR Buffer function table */    
typedef struct _REC_UTIL_PVR_BUF_FCT_TBL_T
{
    rec_util_pvr_buf_query_avail_fct  pf_query_avail_pvr_buf; /* pvr buffer query available size callback */
    rec_util_pvr_buf_alloc_fct        pf_alloc_pvr_buf;       /* pvr buffer allocation callback */
    rec_util_pvr_buf_free_fct         pf_free_pvr_buf;        /* pvr buffer free callback */
    
} REC_UTIL_PVR_BUF_FCT_TBL_T;
    
/* system init config for record util */
typedef struct _REC_UTIL_SYS_CONFIG_T
{
    UINT32                       ui4_strct_ver;          /* structure version, should be 1 */
    UINT32                       ui4_flags;              /* reserved, should be 0 */
    UINT32                       ui4_write_fail_rate;    /* tolerant write fail rate */
    UINT16                       ui2_bitrate_map_items;  /* num of items in bitrate map array */
    REC_UTIL_BITRATE_MAP_ITEM_T* at_bitrate_map;         /* bitrate map array */
    UINT32                       ui4_pvr_buf_depth;      /* pvr buffer depth in milliseconds */
    VOID*                        pv_tag_pvr_buf_cb;      /* pvr buffer callback tag */
    REC_UTIL_PVR_BUF_FCT_TBL_T   t_fct_tbl_pvr_buf;      /* pvr buffer function table */
    
} REC_UTIL_SYS_CONFIG_T;
        

/*-----------------------------------------------------------------------------
                    Functions declarations
-----------------------------------------------------------------------------*/

/*extern INT32 x_rec_util_init(VOID);*/

extern INT32 x_rec_util_init(
    const REC_UTIL_SYS_CONFIG_T*    pt_sys_cfg);



#endif /*end of _X_REC_UTIL_H_ */


