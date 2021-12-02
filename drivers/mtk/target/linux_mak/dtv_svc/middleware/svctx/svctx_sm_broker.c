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
 * $RCSfile: svctx_sm_broker.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/6 $
 * $SWAuthor: Weider Chang $
 * $MD5HEX: a7b5992f37975582d6d8dbacc02ac58d $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "handle/handle.h"

#include "svctx/svctx_sm_broker.h"
#include "svctx/svctx.h"
#include "svctx/svctx_dbg.h"
#include "strm_mngr/video_hdlr/u_sm_video_hdlr.h"
#include "strm_mngr/x_sm.h"
/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
#define SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE     MAKE_BIT_MASK_32(0)
#define SVCTX_SM_BROKER_CMD_BIT_SRC_NAME        MAKE_BIT_MASK_32(1)
#define SVCTX_SM_BROKER_CMD_BIT_SVC_LST_ID      MAKE_BIT_MASK_32(2)
#define SVCTX_SM_BROKER_CMD_BIT_SVL_REC_ID      MAKE_BIT_MASK_32(3)

#define SVCTX_SM_BROKER_CMD_BIT_TUNER          (SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE| \
                                                SVCTX_SM_BROKER_CMD_BIT_SRC_NAME   | \
                                                SVCTX_SM_BROKER_CMD_BIT_SVC_LST_ID | \
                                                SVCTX_SM_BROKER_CMD_BIT_SVL_REC_ID)

#define SVCTX_SM_BROKER_CMD_BIT_AVC            (SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE| \
                                                SVCTX_SM_BROKER_CMD_BIT_SRC_NAME)                                                

#define MAX_SIZE_OF_BROKER_COMMAND             (5)

#if 0
#undef  DBG_ERROR
#define DBG_ERROR(_stmt)    x_dbg_stmt _stmt
#endif
#if 0
#undef  DBG_API
#define DBG_API(_stmt)      x_dbg_stmt _stmt
#endif
#if 0
#undef  DBG_INFO
#define DBG_INFO(_stmt)     x_dbg_stmt _stmt
#endif
/*-----------------------------------------------------------------------------
 * variable declarations
 *---------------------------------------------------------------------------*/
static HANDLE_T  h_g_smb_module_sync_sema = NULL_HANDLE; 
/*-----------------------------------------------------------------------------
 * private methods declarations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * private methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_class_lock
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _smb_class_lock(
                    VOID
                    )
{
    INT32 i4_ret;
    
    if (h_g_smb_module_sync_sema == NULL_HANDLE) {
        i4_ret = x_sema_create(
                    &h_g_smb_module_sync_sema,
                    X_SEMA_TYPE_MUTEX,
                    X_SEMA_STATE_UNLOCK
                    );
        if(i4_ret != OSR_OK) {
            DBG_ERROR((_ERROR_HEADER"create sema for smb failed, ret=%d\r\n", i4_ret));
            return SVCTXR_FAIL;
        }
    }

    i4_ret = x_sema_lock(h_g_smb_module_sync_sema, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sema_lock(h_g_smb_module_sync_sema) failed, i4_ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_class_unlock
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _smb_class_unlock(
                    VOID
                    )
{
    INT32 i4_ret;

    i4_ret = x_sema_unlock(h_g_smb_module_sync_sema);
    if (i4_ret != OSR_OK) {
        DBG_ERROR((_ERROR_HEADER"x_sema_unlock(h_g_smb_module_sync_sema) failed, i4_ret=%d\r\n", i4_ret));
        return SVCTXR_FAIL;
    }

    return SVCTXR_OK;
} 
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_set_vbi_ref_info
 * Description
 *      Only for Digital Channel to tell video stream what's closed caption stream
 *      handle
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *-----------------------------------------------------------------------------*/
static INT32 _smb_set_vbi_ref_info(
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker
                    )
{
#ifdef SM_VSH_SET_TYPE_VBI_REF_INFO
    SM_VSH_VBI_REF_INFO_T t_ref_info;
    INT32 i4_ret;

    ASSERT(pt_vbi_broker != NULL, ("%s(), pt_vbi_broker is NULL\r\n", __func__));

    if (pt_vbi_broker->b_is_digital_vbi == FALSE) {
        return SVCTXR_OK;
    }

    t_ref_info.e_vbi_ref_type = SM_VSH_VBI_REF_INFO_TYPE_CC_STRM_HDL;

    i4_ret = svctx_get_running_stream_handle(
                    pt_vbi_broker->h_svctx,
                    ST_CLOSED_CAPTION,
                    &t_ref_info.u.h_cc_strm
                    );
    if (i4_ret != SVCTXR_OK) {
        return SVCTXR_FAIL;
    }

    if (x_handle_valid(t_ref_info.u.h_cc_strm) == FALSE) {
        return SVCTXR_FAIL;
    }

    i4_ret = x_sm_set(
                    pt_vbi_broker->h_stream,
                    SM_VSH_SET_TYPE_VBI_REF_INFO,
                    &t_ref_info,
                    sizeof(SM_VSH_VBI_REF_INFO_T)
                    );
    if (i4_ret != SMR_OK) {
        return SVCTXR_FAIL;
    }
#endif
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_free_obj
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
VOID _smb_free_obj(
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker
                    )
{
    if (pt_vbi_broker != NULL) {
        handle_free(pt_vbi_broker->h_svctx, FALSE);
        x_mem_free(pt_vbi_broker);
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_free_fct
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static BOOL _smb_free_fct(
                    HANDLE_T                    h_handle,
                    HANDLE_TYPE_T               e_type,
                    VOID*                       pv_obj,
                    VOID*                       pv_tag,
                    BOOL                        b_req_handle
                    )
{
    SVCTX_VBI_BROKER_T* pt_vbi_broker = (SVCTX_VBI_BROKER_T*)pv_obj;

    if ( b_req_handle == TRUE) {
        return TRUE;
    }

    if ( pt_vbi_broker == NULL || e_type != SVCTXT_VBI_BROKER ) {
        return TRUE;
    }

    _smb_free_obj(pt_vbi_broker);

    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_get_obj
 * Description
 * Input arguments
 * Output arguments
 *      None
 * Returns
 *      None
 *-----------------------------------------------------------------------------*/
static INT32 _smb_get_obj(
                    HANDLE_T                    h_vbi_broker,
                    SVCTX_VBI_BROKER_T**        ppt_vbi_broker
                    )
{
    INT32          i4_ret;
    VOID*          pv_obj;
    HANDLE_TYPE_T  e_type;

    i4_ret = handle_get_type_obj(h_vbi_broker, &e_type, &pv_obj);
    if (i4_ret != HR_OK || e_type != SVCTXT_VBI_BROKER || pv_obj == NULL) {
        DBG_ERROR((_ERROR_HEADER"handle_get_type_obj(h_svctx=%d) failed!\r\n"
                "i4_ret=%d, e_type=%d, pv_obj=%d",
                h_vbi_broker, i4_ret, e_type, pv_obj));
        if (i4_ret == HR_INV_HANDLE) {
            i4_ret = SVCTXR_INV_HANDLE;
        } else {
            i4_ret = SVCTXR_FAIL;
        }
    } else {
        *ppt_vbi_broker = (SVCTX_VBI_BROKER_T*)pv_obj;
        i4_ret          = SVCTXR_OK;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_svctx_listener_nfy
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static VOID _smb_svctx_listener_nfy (
                    HANDLE_T                    h_svctx,
                    SVCTX_COND_T                e_nfy_cond,
                    SVCTX_NTFY_CODE_T           e_code,
                    STREAM_TYPE_T               e_stream_type,
                    VOID*                       pv_nfy_tag
                    )
{
    HANDLE_T            h_vbi_broker  = (HANDLE_T)pv_nfy_tag;
    SVCTX_VBI_BROKER_T* pt_vbi_broker = NULL;
    HANDLE_T            h_stream      = NULL_HANDLE;
    INT32               i4_ret;
    
    DBG_API((_API_HEADER"%s(h_svctx = 0x%x, e_nfy_cond=%d, e_code=%d)\r\n", 
                    __func__, 
                    h_svctx,
                    e_nfy_cond,
                    e_code
                    ));

    switch(e_code) {
    case SVCTX_NTFY_CODE_INTERRUPTED:
        break;
    case SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED:
    case SVCTX_NTFY_CODE_STREAM_STOPPED:
        if (ST_VIDEO != e_stream_type) {
            /* not handle it */
            return;
        }
        break;
    case SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED:
    case SVCTX_NTFY_CODE_STREAM_STARTED:  
        if (ST_VIDEO != e_stream_type && ST_CLOSED_CAPTION != e_stream_type) {
            /* not handle it */
            return;
        }
        
        i4_ret = svctx_get_running_stream_handle(h_svctx, ST_VIDEO, &h_stream);
        if (i4_ret != SVCTXR_OK) {
            /* not handle it */
            return;
        }
        break;
    default:
        return;
    }

    _smb_class_lock();
    do {
        i4_ret = _smb_get_obj(h_vbi_broker, &pt_vbi_broker);
        if (i4_ret != SVCTXR_OK) {
            break;
        }
    
        if (e_code == SVCTX_NTFY_CODE_INTERRUPTED) {
            pt_vbi_broker->e_cond = SVCTX_SM_BROKER_COND_UNAVAILABLE;
        } else if (e_code == SVCTX_NTFY_CODE_STREAM_STOPPED
                        || e_code == SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED) {
            pt_vbi_broker->e_cond = SVCTX_SM_BROKER_COND_TRANSITION;
        } else if (e_code == SVCTX_NTFY_CODE_STREAM_STARTED
                        || e_code == SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED) {
            if (ST_CLOSED_CAPTION == e_stream_type) {
                /* cc stream is always play after video stream is started */
                _smb_set_vbi_ref_info(pt_vbi_broker);
                break;
            }
            
            if (pt_vbi_broker->e_cond == SVCTX_SM_BROKER_COND_AVAILABLE 
                && pt_vbi_broker->h_stream == h_stream) {
                break;
            }
            
            pt_vbi_broker->e_cond   = SVCTX_SM_BROKER_COND_AVAILABLE;
            pt_vbi_broker->h_stream = h_stream;
            
            DBG_INFO((_INFO_HEADER"%s() invoking x_sm_set(SM_VSH_SET_TYPE_VBI_NFY)\r\n", __func__));
            i4_ret = x_sm_set(
                        pt_vbi_broker->h_stream,
                        SM_VSH_SET_TYPE_VBI_NFY,
                        &pt_vbi_broker->t_vbi,
                        sizeof(SM_VSH_VBI_NFY_T)
                        );
            if (i4_ret != SMR_OK) {
                DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VBI_NFY) failed, i4_ret=%d\r\n", i4_ret));
            }
        } else {
            break;
        }
    
        if (pt_vbi_broker->pf_nty != NULL) {
            pt_vbi_broker->pf_nty(
                    h_vbi_broker,
                    pt_vbi_broker->e_cond,
                    pt_vbi_broker->pv_tag,
                    0
                    );
        }

        if (pt_vbi_broker->e_cond == SVCTX_SM_BROKER_COND_UNAVAILABLE) {
            x_svctx_vbi_broker_close(h_vbi_broker);
        }
    } while (0);

    _smb_class_unlock();
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_compose_commands
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static INT32 _smb_compose_commands (
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker,
                    SVCTX_SM_BROKER_CMD_T*          pt_cmd_sets
                    )
{
    UINT32 ui4_idx = 0;
    
    pt_cmd_sets[ui4_idx].e_code            = SVCTX_SM_BROKER_CMD_CODE_DEVICE_TYPE;
    pt_cmd_sets[ui4_idx++].u.e_device_type = pt_vbi_broker->e_device_type;
    pt_cmd_sets[ui4_idx].e_code            = SVCTX_SM_BROKER_CMD_CODE_SRC_NAME;
    pt_cmd_sets[ui4_idx++].u.ps_text       = pt_vbi_broker->s_src_name;
    
    if (pt_vbi_broker->e_device_type == DEV_TUNER) {
        pt_cmd_sets[ui4_idx].e_code        = SVCTX_SM_BROKER_CMD_CODE_SVC_LST_ID;
        pt_cmd_sets[ui4_idx++].u.ui2_data  = pt_vbi_broker->ui2_svl_lst_id;
        pt_cmd_sets[ui4_idx].e_code        = SVCTX_SM_BROKER_CMD_CODE_SVL_REC_ID;
        pt_cmd_sets[ui4_idx++].u.ui2_data  = pt_vbi_broker->ui2_svl_rec_id;
    }
    
    pt_cmd_sets[ui4_idx].e_code = SVCTX_SM_BROKER_CMD_CODE_END;
    
    return SVCTXR_OK;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_attach_to_svctx
 * Description      -
 * Input arguments  -
 * Output arguments -
 * Returns          -
 *-----------------------------------------------------------------------------*/
static INT32 _smb_attach_to_svctx(
                    HANDLE_T                        h_vbi_broker,
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker
                    )
{
    SVCTX_SM_BROKER_CMD_T t_cmd_sets[MAX_SIZE_OF_BROKER_COMMAND] = {{0}};
    SVCTX_T*              pt_svctx                               = NULL;
    INT32                 i4_ret                                 = SVCTXR_OK;

    DBG_API((_API_HEADER"%s(pt_vbi_broker = 0x%x)\r\n", __func__, pt_vbi_broker));

    do {
        i4_ret = _smb_compose_commands(pt_vbi_broker, t_cmd_sets);
        if (i4_ret != SVCTXR_OK) {
            break;
        }
            
        i4_ret = svctx_get_vbi_broker(t_cmd_sets, &pt_svctx, pt_vbi_broker);
        if (i4_ret != SVCTXR_OK) {
            break;
        }    
        
        if (x_handle_valid(pt_vbi_broker->h_stream) == TRUE) {
            pt_vbi_broker->e_cond = SVCTX_SM_BROKER_COND_AVAILABLE;
        } else {
            pt_vbi_broker->e_cond = SVCTX_SM_BROKER_COND_TRANSITION;
        }
        
        i4_ret = x_svctx_open(pt_svctx->s_name, &pt_vbi_broker->h_svctx);
        if (i4_ret != SVCTXR_OK) {
            break;
        }
        
        if (pt_vbi_broker->e_cond == SVCTX_SM_BROKER_COND_AVAILABLE) {
            DBG_INFO((_INFO_HEADER"%s(), invoking x_sm_set(SM_VSH_SET_TYPE_VBI_NFY)\r\n", __func__));
            i4_ret = x_sm_set(
                    pt_vbi_broker->h_stream,
                    SM_VSH_SET_TYPE_VBI_NFY,
                    &pt_vbi_broker->t_vbi,
                    sizeof(SM_VSH_VBI_NFY_T)
                    );
            if (i4_ret != SMR_OK) {
                DBG_ERROR((_ERROR_HEADER"x_sm_set(SM_VSH_SET_TYPE_VBI_NFY) failed, i4_ret=%d\r\n", i4_ret));
                i4_ret = SVCTXR_FAIL;
                break;
            }
            _smb_set_vbi_ref_info(pt_vbi_broker);
        }

        i4_ret = x_svctx_set_listener(
                    pt_vbi_broker->h_svctx,
                    _smb_svctx_listener_nfy,
                    (VOID*)(UINT32)(h_vbi_broker)
                    );
    } while (0);
    
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_pares_commands
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static INT32 _smb_pares_commands (
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker,
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets
                    )
{
    UINT32 ui4_cmd_bits = 0;
    
    if (pt_cmd_sets == NULL) {
        return SVCTXR_INV_ARG;
    }

    while (pt_cmd_sets->e_code != SVCTX_SM_BROKER_CMD_CODE_END) {
        switch(pt_cmd_sets->e_code) {
        case SVCTX_SM_BROKER_CMD_CODE_DEVICE_TYPE:
            pt_vbi_broker->e_device_type = DEV_GET_GROUP_ID(pt_cmd_sets->u.e_device_type);
            ui4_cmd_bits                |= SVCTX_SM_BROKER_CMD_BIT_DEVICE_TYPE;
            break;
        case SVCTX_SM_BROKER_CMD_CODE_SRC_NAME:
            x_strcpy(pt_vbi_broker->s_src_name, pt_cmd_sets->u.ps_text);
            ui4_cmd_bits |= SVCTX_SM_BROKER_CMD_BIT_SRC_NAME;
            break;
        case SVCTX_SM_BROKER_CMD_CODE_SVC_LST_ID:
            pt_vbi_broker->ui2_svl_lst_id = pt_cmd_sets->u.ui2_data;
            ui4_cmd_bits                 |= SVCTX_SM_BROKER_CMD_BIT_SVC_LST_ID;
            break;
        case SVCTX_SM_BROKER_CMD_CODE_SVL_REC_ID:
            pt_vbi_broker->ui2_svl_rec_id = pt_cmd_sets->u.ui2_data;
            ui4_cmd_bits                 |= SVCTX_SM_BROKER_CMD_BIT_SVL_REC_ID;
            break;
        default:
            break;
        }
        pt_cmd_sets++;
    }

    if (pt_vbi_broker->e_device_type == DEV_TUNER
            && ui4_cmd_bits == SVCTX_SM_BROKER_CMD_BIT_TUNER) {
        return SVCTXR_OK;
    }
    
    if (pt_vbi_broker->e_device_type == DEV_AVC
            && ui4_cmd_bits == SVCTX_SM_BROKER_CMD_BIT_AVC) {
        return SVCTXR_OK;
    }
    
    return SVCTXR_INV_ARG;
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_init_obj
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static VOID _smb_init_obj (
                    SVCTX_VBI_BROKER_T*             pt_vbi_broker,
                    SM_VSH_VBI_NFY_T*               pt_vbi,
                    x_svctx_vbi_broker_nfy_fct      pf_nfy,
                    VOID*                           pv_tag
                    )
{
    pt_vbi_broker->h_svctx          = NULL_HANDLE;
    pt_vbi_broker->h_stream         = NULL_HANDLE;
    pt_vbi_broker->h_conn           = NULL_HANDLE;
    pt_vbi_broker->e_device_type    = DEV_UNKNOWN;
    pt_vbi_broker->s_src_name[0]    = '\0';
    pt_vbi_broker->ui2_svl_lst_id   = SVL_NULL_ID;
    pt_vbi_broker->ui2_svl_rec_id   = SVL_NULL_REC_ID;
    pt_vbi_broker->b_is_digital_vbi = FALSE;
    pt_vbi_broker->e_cond           = SVCTX_SM_BROKER_COND_TRANSITION;
    pt_vbi_broker->pf_nty           = pf_nfy;
    pt_vbi_broker->pv_tag           = pv_tag;
    
    if (pt_vbi != NULL) {
        pt_vbi_broker->t_vbi = *pt_vbi;
    }
}
/*-----------------------------------------------------------------------------
 * Name
 *      _smb_post_create
 * Description
 * Input arguments
 * Output arguments
 * Returns
 *      TRUE
 *      FALSE
 *-----------------------------------------------------------------------------*/
static INT32 _smb_post_create(
                    VOID*                   pv_tag,
                    struct _SVCTX_MSG_T*    pt_msg
                    )
{
    HANDLE_T            h_vbi_broker  = (HANDLE_T)(UINT32)pv_tag;
    SVCTX_VBI_BROKER_T* pt_vbi_broker = NULL;
    INT32               i4_ret = SVCTXR_OK;

    DBG_INFO((_INFO_HEADER"%s(h_vbi_broker=%d)\r\n", __func__, h_vbi_broker));
    
    /* exectue in SVCTX's thread */
    _smb_class_lock();
    do {
        if (x_handle_valid(h_vbi_broker) == FALSE) {
            i4_ret = SVCTXR_INV_HANDLE;
            break;
        }
        
        i4_ret = _smb_get_obj(h_vbi_broker, &pt_vbi_broker);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        if (pt_vbi_broker == NULL)
        {
            break;
        }
                    
        i4_ret = _smb_attach_to_svctx(h_vbi_broker, pt_vbi_broker);
        if (i4_ret != SVCTXR_OK && pt_vbi_broker != NULL) {
            pt_vbi_broker->e_cond = SVCTX_SM_BROKER_COND_UNAVAILABLE;
        	if (pt_vbi_broker->pf_nty != NULL) {
        	    pt_vbi_broker->pf_nty(
        	            h_vbi_broker,
        	            pt_vbi_broker->e_cond,
        	            pt_vbi_broker->pv_tag,
        	            0
        	            );
        	}
        	
        	x_svctx_vbi_broker_close(h_vbi_broker);
        }
    } while (0);
    _smb_class_unlock();
    
    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * public methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: svctx_vbi_broker_get_conn_handle
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 svctx_vbi_broker_get_conn_handle (
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets,    
                    HANDLE_T*                       ph_conn_handle
                    )
{
    SVCTX_VBI_BROKER_T  t_vbi_broker = {0};
    SVCTX_T*            pt_svctx     = NULL;
    INT32               i4_ret;

    if (svctx_is_init() == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (ph_conn_handle == NULL) {
        return SVCTXR_INV_ARG;
    }

    i4_ret = svctx_get_vbi_broker(
                    pt_cmd_sets,
                    &pt_svctx,
                    &t_vbi_broker
                    );
    if (i4_ret == SVCTXR_OK) {
        *ph_conn_handle = t_vbi_broker.h_conn;
    }
    
    return i4_ret;    
}
/*-----------------------------------------------------------------------------
 * export methods implementations
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: x_svctx_vbi_broker_open
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_vbi_broker_open (
                    const SVCTX_SM_BROKER_CMD_T*    pt_cmd_sets,
                    SM_VSH_VBI_NFY_T*               pt_vbi,
                    x_svctx_vbi_broker_nfy_fct      pf_nfy,
                    VOID*                           pv_tag,
                    HANDLE_T*                       ph_vbi_broker,
                    SVCTX_SM_BROKER_COND_T*         pe_cond
                    )
{
    SVCTX_VBI_BROKER_T* pt_vbi_broker = NULL;
    SVCTX_VBI_BROKER_T  t_vbi_broker  = {0};
    INT32               i4_ret;

    DBG_API((_API_HEADER"%s()\r\n", __func__));
    
    if (svctx_is_init() == FALSE) {
        return SVCTXR_NOT_INIT;
    }

    if (pt_vbi == NULL || pt_vbi->pf_vbi_nfy == NULL
            || pf_nfy == NULL || ph_vbi_broker == NULL ) {
        return SVCTXR_INV_ARG;
    }

    _smb_class_lock();
    do {
        /* preprocess the commands in pre creation*/
        _smb_init_obj(&t_vbi_broker, NULL, NULL, NULL);        
        i4_ret = _smb_pares_commands(&t_vbi_broker, pt_cmd_sets);
        if (i4_ret != SVCTXR_OK) {
            break;
        }
                
        pt_vbi_broker = (SVCTX_VBI_BROKER_T*) x_mem_alloc(sizeof(SVCTX_VBI_BROKER_T));        
        if (pt_vbi_broker == NULL) {
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }
        
        *pt_vbi_broker        = t_vbi_broker;
        pt_vbi_broker->t_vbi  = *pt_vbi;
        pt_vbi_broker->pf_nty = pf_nfy;
        pt_vbi_broker->pv_tag = pv_tag;

        if (pe_cond != NULL) {
            *pe_cond = pt_vbi_broker->e_cond;
        }
        
        i4_ret = handle_alloc(
                    SVCTXT_VBI_BROKER,
                    (VOID*)pt_vbi_broker,
                    NULL,
                    _smb_free_fct,
                    ph_vbi_broker
                    );
        if ( i4_ret != HR_OK) {
            DBG_ERROR((_ERROR_HEADER"handle_alloc(SM_BROKER) failed, i4_ret=%d\r\n", i4_ret));
            i4_ret = SVCTXR_NO_RESOURCE;
            break;
        }
        
        /* send to svctx's thread to do the post creation */
        i4_ret = svctx_request_context_switch(
                    _smb_post_create,
                    (VOID*)(UINT32)(*ph_vbi_broker)
                    );
    } while (0);

    if (i4_ret != SVCTXR_OK) {
        _smb_free_obj(pt_vbi_broker);
        if (pe_cond != NULL) {
            *pe_cond = SVCTX_SM_BROKER_COND_UNAVAILABLE;
        }
    }
    _smb_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_vbi_broker_close
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_vbi_broker_close (
                    HANDLE_T                        h_vbi_broker
                    )
{
    INT32 i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s()\r\n", __func__));
    
    _smb_class_lock();
    do {
        if (x_handle_valid(h_vbi_broker) == FALSE) {
            i4_ret = SVCTXR_INV_HANDLE;
            break;
        }

        i4_ret = handle_free(h_vbi_broker, FALSE);
        if (i4_ret != HR_OK) {
           /*  DBG_ABORT(DBG_MOD_SVCTX); */
           DBG_ERROR((_ERROR_HEADER"%s(), invoke handle_free(h_vbi_broker=%d) failed! ret=%d\r\n",
                    __func__,
                    h_vbi_broker,
                    i4_ret
                    ));
        }

    } while (0);
    _smb_class_unlock();

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: x_svctx_vbi_broker_video_sm_get
 *
 * Description: -
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 *-----------------------------------------------------------------------------*/
INT32 x_svctx_vbi_broker_video_sm_get(
                    HANDLE_T                        h_vbi_broker,
                    SM_GET_TYPE_T                   e_get_type,
                    VOID*                           pv_get_info,
                    SIZE_T*                         pz_get_info_size
                    )
{
    SVCTX_VBI_BROKER_T* pt_vbi_broker;
    INT32               i4_ret = SVCTXR_OK;

    DBG_API((_API_HEADER"%s()\r\n", __func__));
    
    _smb_class_lock();
    do {
        if (x_handle_valid(h_vbi_broker) == FALSE) {
            i4_ret = SVCTXR_INV_HANDLE;
            break;
        }
        
        i4_ret = _smb_get_obj(h_vbi_broker, &pt_vbi_broker);
        if (i4_ret != SVCTXR_OK) {
            break;
        }

        i4_ret = x_sm_get(
                    pt_vbi_broker->h_stream,
                    e_get_type,
                    pv_get_info,
                    pz_get_info_size
                    );
        if (i4_ret != SMR_OK) {
            i4_ret = SVCTXR_FAIL;
        }
    } while (0);
    _smb_class_unlock();

    return i4_ret;
}
