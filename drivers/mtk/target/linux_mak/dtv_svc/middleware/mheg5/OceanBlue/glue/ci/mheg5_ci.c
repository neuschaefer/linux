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
 * $RCSfile: mheg5_os.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all os related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "dbg/u_dbg.h"
#include "dbg/x_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "handle/u_handle.h"
#include "os/inc/x_os.h"
#include "os/inc/u_os.h"
#include "ci/x_ci.h"
#include "ci/u_ci.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/x_mheg5.h"
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"
#include "mheg5/OceanBlue/glue/ci/mheg5_ci.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_ci.h"

/* defines */
#ifdef CI_PLUS_SUPPORT
#define CI_MSG_CI_FILE_ACK         MHEG5_MSG_SPECIAL_START
#define MHEG5_CI_MSG_SEND_RETRY    ((UINT16) 10)

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_ci_get_dbg_level()

static UINT16    _ui2_mheg5_ci_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
#endif


/* data */
static BOOL                         _b_ci_app_began = FALSE;
static HANDLE_T                     _h_ci = NULL_HANDLE;
static HANDLE_T                     _h_ci_lock = NULL_HANDLE;
static x_mheg5_ci_nfy               _pf_mheg5_ci_nfy_app = NULL;
static VOID*                        _pv_mheg5_ci_nfy_app_tag = NULL;

/* Internal function */
static VOID _ci_lock(VOID);

static VOID _ci_unlock(VOID);

static VOID _mheg5_ci_nfy_fct(   
    HANDLE_T            h_ci,
    VOID*               pv_nfy_tag,
    CI_MHEG5_NFY_COND_T e_nfy_cond,
    VOID*               pv_data);



static VOID _ci_lock()
{
    x_sema_lock(_h_ci_lock, X_SEMA_OPTION_WAIT);
}


static VOID _ci_unlock()
{   
    x_sema_unlock(_h_ci_lock);
}

static VOID _mheg5_ci_nfy_fct(   
    HANDLE_T            h_ci,
    VOID*               pv_nfy_tag,
    CI_MHEG5_NFY_COND_T e_nfy_cond,
    VOID*               pv_data)
{
    MHEG5_MSG_T     t_msg;
    
    if (h_ci != _h_ci)
    {
        return;
    }

    switch(e_nfy_cond)
    {
        case CI_NFY_COND_FILE_ACK:
        {
            MHEG5_CI_FILE_ACK_T* pt_ack;

            pt_ack = x_mem_alloc(sizeof(MHEG5_CI_FILE_ACK_T));
            if (pt_ack == NULL)
            {
                return;
            }

            pt_ack->b_file_ok      = ((CI_FILE_ACK_T*)pv_data)->b_file_ok;
            pt_ack->pui1_file_data = ((CI_FILE_ACK_T*)pv_data)->pu1fileAck;
            pt_ack->ui4_file_size  = ((CI_FILE_ACK_T*)pv_data)->ui4_length;
            pt_ack->pv_tag         = pv_nfy_tag;
            
            t_msg.ui1_module   = MHEG5_MOD_CI;
            t_msg.ui2_msg_type = CI_MSG_CI_FILE_ACK;
            t_msg.pv_nfy_tag   = (VOID*)pt_ack;
            
            x_mheg5_send_msg(MHEG5_OBS_ENG_CODE, &t_msg, MHEG5_CI_MSG_SEND_RETRY);
        }
        break;

        default:
            break;    
    }
}


INT32 mheg5_ci_init()
{
    INT32 i4_ret;

    _b_ci_app_began = FALSE;
    
    i4_ret = x_sema_create(&_h_ci_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    if (OSR_OK != i4_ret)
    {
        DBG_ERROR(("{MHEG5 Stack} %s create sema lock for MHEG5 CI plus failed(%d).\n", __FUNCTION__, i4_ret));
        _h_ci_lock = NULL_HANDLE;
        return MHEG5R_OUT_OF_RESOURCES;
    }

    i4_ret = x_ci_mheg5_reg_nfy(&_h_ci, NULL, _mheg5_ci_nfy_fct);
    if (CIR_OK != i4_ret)
    {
        DBG_ERROR(("{MHEG5 Stack} %s register CI mheg5 notify function failed(%d).\n", __FUNCTION__, i4_ret));

        x_sema_delete(_h_ci_lock);
        _h_ci_lock = NULL_HANDLE;
        _h_ci = NULL_HANDLE;
        return MHEG5R_REG_CI_NFY_FAIL;
    }
    
    return MHEG5R_OK;
    
}

INT32 mheg5_ci_proc_msg(MHEG5_MSG_T* pt_msg)
{
    MHEG5_CI_NFY_T t_nfy;

    switch (pt_msg->ui2_msg_type)
    {
        case MHEG5_MSG_COMP_PRGS:
        {
            tmMHEG5ProgressMessage_t progressMessage;

            progressMessage = (tmMHEG5ProgressMessage_t)((UINT32)(pt_msg->pv_nfy_tag));

            switch (progressMessage)
            {
                case MHEG5_CI_APPLICATION_START_FAILED:
                {        
                    _ci_lock();
                    
                    _b_ci_app_began = FALSE;
                    
                    /*x_dbg_stmt("MHEG5_MSG_COMP_PRGS: MHEG5_CI_APPLICATION_START_FAILED\n");*/

                    if (_pf_mheg5_ci_nfy_app)
                    {
                        t_nfy.ui4_nfy_flag = MHEG5_CI_NFY_COMP_PRGS;
                        t_nfy.u.e_prgs     = MHEG5_PRGS_CI_APPLICATION_START_FAILED;
                        
                        _pf_mheg5_ci_nfy_app(&t_nfy, _pv_mheg5_ci_nfy_app_tag);
                    }

                    _ci_unlock();
                }
                break;
        
                case MHEG5_CI_APPLICATION_STOPPED:
                {
                    _ci_lock();
                    
                    _b_ci_app_began = FALSE;
                    
                    /*x_dbg_stmt("MHEG5_MSG_COMP_PRGS: MHEG5_CI_APPLICATION_STOPPED\n");*/

                    if (_pf_mheg5_ci_nfy_app)
                    {
                        t_nfy.ui4_nfy_flag = MHEG5_CI_NFY_COMP_PRGS;
                        t_nfy.u.e_prgs = MHEG5_PRGS_CI_APPLICATION_STOPPED;

                        _pf_mheg5_ci_nfy_app(&t_nfy, _pv_mheg5_ci_nfy_app_tag);
                    }
                    
                    _ci_unlock();
                }
                break;
        
                case MHEG5_CI_APPLICATION_STARTED:
                {
                    _ci_lock();
                    
                    _b_ci_app_began = TRUE;
                    
                    /*x_dbg_stmt("MHEG5_MSG_COMP_PRGS: MHEG5_CI_APPLICATION_STARTED\n");*/

                    if (_pf_mheg5_ci_nfy_app)
                    {
                        t_nfy.ui4_nfy_flag = MHEG5_CI_NFY_COMP_PRGS;
                        t_nfy.u.e_prgs = MHEG5_PRGS_CI_APPLICATION_STARTED;
                        
                        _pf_mheg5_ci_nfy_app(&t_nfy, _pv_mheg5_ci_nfy_app_tag);
                    }
                    
                    _ci_unlock();
                }
                break;

                default:
                    break;
                
            }
        }
        break;

        case CI_MSG_CI_FILE_ACK:
        {
            MHEG5_CI_FILE_ACK_T* pt_ci_file_ack;

            pt_ci_file_ack = (MHEG5_CI_FILE_ACK_T*)(pt_msg->pv_nfy_tag);
            DBG_INFO(("{MHEG5 Stack}DBG_INFO %s ,CI_MSG_CI_FILE_ACK.\n", __FUNCTION__));
            DBG_ERROR(("{MHEG5 Stack}DBG_ERROR %s ,CI_MSG_CI_FILE_ACK.\n", __FUNCTION__));
            
            #if 1
            DBG_INFO(("tmMHEG5CiFileAcknowledge(%d, %p, %lu), file dats is:\n",pt_ci_file_ack->b_file_ok, pt_ci_file_ack->pui1_file_data, pt_ci_file_ack->ui4_file_size));
            {
                UINT32 i = 0;
                
                for (i=0;i<pt_ci_file_ack->ui4_file_size;)
                {
                    DBG_INFO(("%02x  ", pt_ci_file_ack->pui1_file_data[i]));
                    i++;
                    if (i%16 == 0)
                    {
                        x_dbg_stmt("\n");
                    }
                }
                DBG_INFO(("\n"));
            }
            #endif
            
            tmMHEG5CiFileAcknowledge((BOOLEAN)pt_ci_file_ack->b_file_ok, (U8BIT *) pt_ci_file_ack->pui1_file_data, (U32BIT) pt_ci_file_ack->ui4_file_size);

            x_mem_free(pt_ci_file_ack);
        }
        break;

        default:
            break;
    }
    
    return MHEG5R_OK;
}


INT32 mheg5_ci_start_ci_app(MHEG5_CI_START_APP_T* pt_start_ci_app)
{
    INT32 i4_ret;
    tmMHEG5Err_t t_err;

    if (pt_start_ci_app == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if ((pt_start_ci_app->ui1_ver != 0) ||
        (pt_start_ci_app->s_ci_app_name == NULL))
    {
        return MHEG5R_INV_ARG;
    }

    _ci_lock();
    
    if (_b_ci_app_began == TRUE)
    {
        _ci_unlock();
        return MHEG5R_CI_START_APP_BUSY;
    }

    t_err = tmMHEG5StartCIApplication((U8BIT *)(pt_start_ci_app->s_ci_app_name));
    
    /*x_dbg_stmt("tmMHEG5StartCIApplication(%s) => %d\n", pt_start_ci_app->s_ci_app_name, t_err);*/

    _ci_unlock();

    i4_ret = mheg5_obs_convert_err_code(t_err);

    return i4_ret;
    
}

INT32 mheg5_ci_stop_ci_apps(MHEG5_CI_STOP_APPS_T*  pt_stop_ci_apps)
{
    INT32 i4_ret;
    tmMHEG5Err_t t_err;

    if (pt_stop_ci_apps == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if (pt_stop_ci_apps->ui1_ver != 0)
    {
        return MHEG5R_INV_ARG;
    }
        
    _ci_lock();
    if (_b_ci_app_began == FALSE)
    {
        _ci_unlock();
        return MHEG5R_CI_NO_APP_STARTED;
    }

    t_err = tmMHEG5StopCIApplications();
    /*x_dbg_stmt("tmMHEG5StopCIApplications() => %d\n", t_err);*/

    _ci_unlock();

    i4_ret = mheg5_obs_convert_err_code(t_err);

    return i4_ret;
}

INT32 mheg5_ci_reg_nfy(x_mheg5_ci_nfy pf_mheg5_ci_nfy, VOID* pv_tag)
{
    _ci_lock();

    _pf_mheg5_ci_nfy_app = pf_mheg5_ci_nfy;
    _pv_mheg5_ci_nfy_app_tag = pv_tag;

    _ci_unlock();

    return MHEG5R_OK; 
}
#endif


tmMHEG5Err_t tmMHEG5CiFileRequest(U8BIT* data, U32BIT length)
{
#ifdef CI_PLUS_SUPPORT

    INT32 i4_ret;
    CI_FILE_REQUEST_T t_ci_file_req;
    
    /*x_dbg_stmt("tmMHEG5CiFileRequest(%p, %lu) \n", (UINT8*)data, (UINT32)length);*/

    if (data == NULL || length == 0)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    _ci_lock();
    
    /*if (_b_ci_app_began == FALSE)
    {
        _ci_unlock();
        return MHEG5_ERR_CI_SESSION_NOT_ESTABLISHED;
    }*/

    t_ci_file_req.pu1fileReq = (UINT8*)data;
    t_ci_file_req.ui4_length = (UINT32)length;

    i4_ret = x_ci_mheg5_set(CI_MHEG5_SET_TYPE_FILE_REQUEST, (VOID*)&t_ci_file_req);

    _ci_unlock();

    if (i4_ret != CIR_OK)
    {
        if (i4_ret == CIR_SESSION_NOT_ESTABLISHED)
        {
            return MHEG5_ERR_CI_SESSION_NOT_ESTABLISHED;
        }
        else
        {
            return MHEG5_ERR_OTHER;
        }
    }
    else
    {
        return MHEG5_OK;
    }
    
#else

    return MHEG5_ERR_CI_SESSION_NOT_ESTABLISHED;

#endif
}


tmMHEG5Err_t tmMHEG5CiAckData(U8BIT* pData)
{
#ifdef CI_PLUS_SUPPORT
    INT32 i4_ret;
    CI_FREE_ACK_DATA_T t_ci_free_ack;
    
    /*x_dbg_stmt("tmMHEG5CiAckData(%p) \n", pData);*/

    if (pData == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    t_ci_free_ack.pu1fileAck = (UINT8*)pData;

    i4_ret = x_ci_mheg5_set(CI_MHEG5_SET_TYPE_FREE_ACK_DATA, (VOID*)&t_ci_free_ack);
    if (i4_ret != CIR_OK)
    {
        return MHEG5_ERR_OTHER;
    }

    return MHEG5_OK;
#else
    return MHEG5_ERR_OTHER;
#endif
}

tmMHEG5Err_t tmMHEG5RequestMPEGDecoder(
    tmMHEG5DecoderRequest_t request,
    BOOLEAN *pResult)
{
#ifdef CI_PLUS_SUPPORT
    if (pResult == NULL || ((request != MHEG5_DECODER_REQUEST) && (request != MHEG5_DECODER_RELEASE)))
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    /*x_dbg_stmt("tmMHEG5RequestMPEGDecoder(%d, %p) \n", request, pResult);*/

    /* No real implementation now */
    /* 
           TRUE if access to the MPEG decoder is granted
           FALSE if access to the MPEG decoder is not granted,
           or if request is MHEG5_DECODER_RELEASE
      */
     *pResult = TRUE;
     if (request == MHEG5_DECODER_RELEASE)
     {
        *pResult = FALSE;
     }

     return MHEG5_OK;
     
#else
    return MHEG5_ERR_OTHER;
#endif
}

tmMHEG5Err_t tmMHEG5SuppressMHEGGraphics(BOOLEAN suppress)
{
    return MHEG5_ERR_OTHER;
}

tmMHEG5Err_t tmMHEG5IsKeySupported(tmMHEG5KeyPress_t key,BOOLEAN *supported)
{
    return MHEG5_ERR_OTHER;
}

#ifdef CI_PLUS_SUPPORT
#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: mheg5_ci_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 mheg5_ci_get_dbg_level(VOID)
{
    return _ui2_mheg5_ci_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: mheg5_ci_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL mheg5_ci_set_dbg_level(UINT16 ui2_level)
{

    _ui2_mheg5_ci_dbg_level = ui2_level;
    return TRUE;

}

#endif
#endif

