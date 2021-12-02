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
 * $RCSfile: tuner_ctrlr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/handle.h"
#include "dbg/x_dbg.h"
#include "dbg/x_dbg_drv.h"
#include "dbg/def_dbg_level_mw.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_tuner.h"
#include "tsl/x_tsl.h"
#include "svctx/util/tuner_ctrlr.h"
#include "svctx/svctx.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Analog tuner state */
typedef enum
{
    TUNER_CTRLR_STATE_CLOSE = 0,
    TUNER_CTRLR_STATE_OPEN,
    TUNER_CTRLR_STATE_DISCONN,
    TUNER_CTRLR_STATE_CONN
}   TUNER_CTRLR_STATE_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL                     b_tuner_ctrlr_init = FALSE;
static HANDLE_T                 h_rm_pipe = NULL_HANDLE;
static HANDLE_T                 h_rm_ana_tuner = NULL_HANDLE;
static TUNER_CTRLR_STATE_T      t_tuner_ctrlr_state = TUNER_CTRLR_STATE_CLOSE;
static TUNER_CTRLR_INFO_T       t_g_cached_tuner_ctrlr_info;
static BOOL                     b_g_cached_replace_freq;
static UINT32                   ui4_g_cached_freq_to_replace;

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static VOID _tuner_ctrlr_rm_nfy_fct(HANDLE_T  h_obj,
                                    UINT8     ui1_port,
                                    RM_COND_T e_nfy_cond,
                                    VOID*     pv_tag,
                                    UINT32    ui4_data);

static VOID _tuner_ctrlr_tuner_rm_nfy_fct(HANDLE_T  h_comp,
                                          UINT8     ui1_port,
                                          RM_COND_T e_nfy_cond,
                                          VOID*     pv_tag,
                                          UINT32    ui4_data);

static VOID _tuner_ctrlr_tsl_nfy(HANDLE_T   h_tsl,
                                 TSL_COND_T e_cond,
                                 UINT32     ui4_reason,
                                 VOID*      pv_tag,
                                 UINT32     ui4_data);

static INT32 _tuner_ctrlr_connect_ana_tuner(TUNER_CTRLR_INFO_T* pt_tunning_info);

static INT32 _tuner_ctrlr_disconnect_ana_tuner(VOID);

/*-----------------------------------------------------------------------------
 * Name: _tuner_ctrlr_rm_nfy_fct
 *
 * Description: This notify function is called by the Resource Manager.
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _tuner_ctrlr_rm_nfy_fct(HANDLE_T  h_obj,
                                    UINT8     ui1_port,
                                    RM_COND_T e_nfy_cond,
                                    VOID*     pv_tag,
                                    UINT32    ui4_data)
{
    /* To be implemented */
}


/*-----------------------------------------------------------------------------
 * Name: _tuner_ctrlr_tuner_rm_nfy_fct
 *
 * Description: This API is called by the Resource Manager to notify the
 *              controller of changes in the state or condition of the driver
 *              component.
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _tuner_ctrlr_tuner_rm_nfy_fct(HANDLE_T  h_comp,
                                          UINT8     ui1_port,
                                          RM_COND_T e_nfy_cond,
                                          VOID*     pv_tag,
                                          UINT32    ui4_data)
{
    /* To be implemented */
}


/*-----------------------------------------------------------------------------
 * Name: _tuner_ctrlr_tsl_nfy
 *
 * Description: This API is called when the TSL is modified (or closed).
 *
 * Inputs:  h_tsl       Contains the TSL handle.
 *          e_cond      Contains the new condition.
 *          ui4_reason  Specifies the reasong for the new condition.
 *          pv_tag      Contains the tag value passed when the TSL was opened.
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _tuner_ctrlr_tsl_nfy(HANDLE_T   h_tsl,
                                 TSL_COND_T e_cond,
                                 UINT32     ui4_reason,
                                 VOID*      pv_tag,
                                 UINT32     ui4_data)
{
    /* To be implemented */
}


/*-----------------------------------------------------------------------------
 * Name: _tuner_ctrlr_set_ana_tuner
 *
 * Description: This API perfroms tuner connection.
 *
 * Inputs:  pt_tunning_info         The tunning information.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_INV_ARG          One or more of the arguments contain
 *                                  invalid data. 
 *          SVCTXR_FAIL             Routine failed. 
 ----------------------------------------------------------------------------*/
static INT32 _tuner_ctrlr_set_ana_tuner(
                    TUNER_CTRLR_INFO_T* pt_tunning_info,
                    BOOL                b_replace_freq,
                    UINT32              ui4_freq_to_replace)
{
    INT32               i4_return;
    HANDLE_T            h_tsl;
    TSL_REC_T           t_tsl_rec;
    UINT32              ui4_tsl_ver;
    DRV_TYPE_T          e_drv_type;
    DRV_CONN_TYPE_T     e_conn_type;
    VOID*               pv_conn = NULL;
    SIZE_T              z_size = 0;
    BOOL                b_no_disc_nfy;
    RM_COND_T           e_rm_cond;
    
    if (pt_tunning_info == NULL)
    {
        return SVCTXR_FAIL;
    }
             
    if (t_tuner_ctrlr_state == TUNER_CTRLR_STATE_CONN
            && pt_tunning_info != NULL
            && t_g_cached_tuner_ctrlr_info.ui2_tsl_id     == pt_tunning_info->ui2_tsl_id
            && t_g_cached_tuner_ctrlr_info.ui2_tsl_rec_id == pt_tunning_info->ui2_tsl_rec_id
            && t_g_cached_tuner_ctrlr_info.ui4_tv_sys     == pt_tunning_info->ui4_tv_sys) {
        if (b_g_cached_replace_freq == b_replace_freq) {
            if (b_g_cached_replace_freq == FALSE) {
                return SVCTXR_OK;
            } else {
                if (ui4_g_cached_freq_to_replace == ui4_freq_to_replace) {
                    return SVCTXR_OK;
                }
            }
        }
    }

    /* Obtain tune data from TSL */
    i4_return = x_tsl_open(pt_tunning_info->ui2_tsl_id,
                           NULL,
                           _tuner_ctrlr_tsl_nfy,
                           &h_tsl);

    if (i4_return != TSLR_OK)
    {
        return SVCTXR_FAIL;
    }

    i4_return = x_tsl_get_rec(h_tsl,
                              pt_tunning_info->ui2_tsl_rec_id,
                              &t_tsl_rec,
                              &ui4_tsl_ver);

    if (i4_return != TSLR_OK)
    {
        x_tsl_close(h_tsl);
        return SVCTXR_FAIL;
    }

    if (x_tsl_close(h_tsl) != TSLR_OK)
    {
        return SVCTXR_FAIL;
    }        
    
    /* Fill in tune info */
    if (t_tsl_rec.uheader.t_desc.e_bcst_type == BRDCST_TYPE_ANALOG)
    {
        switch (t_tsl_rec.uheader.t_desc.e_bcst_medium)
        {
            case BRDCST_MEDIUM_ANA_CABLE:
                e_drv_type = DRVT_TUNER_CAB_ANA_SCART_OUT;
                e_conn_type = TUNER_CONN_TYPE_CAB_ANA_SCART_OUT;
                t_tsl_rec.udata.t_cab_ana.b_no_disc_nfy = FALSE;
                t_tsl_rec.udata.t_cab_ana.ui4_tv_sys_mask = pt_tunning_info->ui4_tv_sys;
                t_tsl_rec.udata.t_ter_ana.e_mod = MOD_PSK_8;
                if (b_replace_freq == TRUE) {
                    t_tsl_rec.udata.t_ter_ana.ui4_freq = ui4_freq_to_replace;
                }
                pv_conn = (VOID*)&t_tsl_rec.udata.t_cab_ana;
                z_size = sizeof(TUNER_CAB_ANA_TUNE_INFO_T);
                break;

            case BRDCST_MEDIUM_ANA_TERRESTRIAL:
                e_drv_type = DRVT_TUNER_TER_ANA_SCART_OUT;
                e_conn_type = TUNER_CONN_TYPE_TER_ANA_SCART_OUT;
                t_tsl_rec.udata.t_ter_ana.b_no_disc_nfy = FALSE;
                t_tsl_rec.udata.t_ter_ana.ui4_tv_sys_mask = pt_tunning_info->ui4_tv_sys;
                t_tsl_rec.udata.t_ter_ana.b_fine_tune = FALSE;
                t_tsl_rec.udata.t_ter_ana.e_mod = MOD_PSK_8;
                if (b_replace_freq == TRUE) {
                    t_tsl_rec.udata.t_ter_ana.ui4_freq = ui4_freq_to_replace;
                }
                pv_conn = (VOID*)&t_tsl_rec.udata.t_ter_ana;
                z_size = sizeof(TUNER_TER_ANA_TUNE_INFO_T);
                break;

            case BRDCST_MEDIUM_DIG_SATELLITE:
                e_drv_type = DRVT_TUNER_SAT_ANA_SCART_OUT;
                e_conn_type = TUNER_CONN_TYPE_SAT_ANA_SCART_OUT;
                t_tsl_rec.udata.t_sat_ana.b_no_disc_nfy = FALSE;
                pv_conn = (VOID*)&t_tsl_rec.udata.t_sat_ana;
                z_size = sizeof(TUNER_SAT_ANA_TUNE_INFO_T);
                break;
              
            default:
                pv_conn = NULL;
                i4_return = SVCTXR_FAIL;
        }
    }
    else
    {
        pv_conn = NULL;
        i4_return = SVCTXR_INV_ARG;
    }

    if (pv_conn != NULL)
    {
        if ((h_rm_ana_tuner == NULL_HANDLE) &&
            (t_tuner_ctrlr_state == TUNER_CTRLR_STATE_CLOSE))
        {   
            i4_return = rm_open_comp(h_rm_pipe,
                                     OBJ_COMP_REL_IGNORE,
                                     e_drv_type,
                                     ANY_COMP_ID,
                                     NULL,
                                     CTRL_TYPE_SINGLE,
                                     NULL,
                                     NULL,
                                     NULL,
                                     _tuner_ctrlr_tuner_rm_nfy_fct,
                                     &h_rm_ana_tuner,
                                     &e_rm_cond);
    
            switch (i4_return)
            {
                case RMR_OK:
                case RMR_ASYNC_NFY:
                case RMR_ARBITRATION:
                    t_tuner_ctrlr_state = TUNER_CTRLR_STATE_OPEN;
                    i4_return = SVCTXR_OK;
                    break;
    
                case RMR_NO_COMP_FOUND:
                case RMR_INV_NAME:
                case RMR_OUT_OF_HANDLES:
                case RMR_NOT_ENOUGH_MEM:
                default:
                    i4_return = SVCTXR_FAIL;
            }
        }
        else
        {
            i4_return = SVCTXR_OK;    
        }                        

        if (i4_return == SVCTXR_OK)
        {
            b_no_disc_nfy = TRUE;
            i4_return = rm_set(h_rm_ana_tuner,
                               TRUE,
                               ANY_PORT_NUM,
                               TUNER_SET_TYPE_NO_DISC_NFY,
                               (VOID*)(UINT32)b_no_disc_nfy,
                               sizeof(BOOL),
                               NULL);

            if (i4_return == RMR_OK)
            {
                i4_return = rm_connect(h_rm_ana_tuner,
                                       e_conn_type,
                                       pv_conn,
                                       z_size,
                                       NULL,
                                       &e_rm_cond);

                if (i4_return == RMR_OK)
                {
                    t_tuner_ctrlr_state = TUNER_CTRLR_STATE_CONN;
                    t_g_cached_tuner_ctrlr_info  = *pt_tunning_info;
                    b_g_cached_replace_freq      = b_replace_freq;
                    ui4_g_cached_freq_to_replace = ui4_freq_to_replace;
                    i4_return = SVCTXR_OK;
                }
                else if (i4_return == RMR_ASYNC_NFY)
                {
                    t_tuner_ctrlr_state = TUNER_CTRLR_STATE_CONN;
                    t_g_cached_tuner_ctrlr_info  = *pt_tunning_info;
                    b_g_cached_replace_freq      = b_replace_freq;
                    ui4_g_cached_freq_to_replace = ui4_freq_to_replace;                    
                    i4_return = SVCTXR_OK;
                }
                else
                {
                    i4_return = SVCTXR_FAIL;
                }
            }
            else
            {
                i4_return = SVCTXR_FAIL;
            }
        }
    }
    else
    {
        i4_return = SVCTXR_INV_ARG;

    }

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name: _tuner_ctrlr_connect_ana_tuner
 *
 * Description: This API perfroms tuner connection.
 *
 * Inputs:  pt_tunning_info         The tunning information.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_INV_ARG          One or more of the arguments contain
 *                                  invalid data. 
 *          SVCTXR_FAIL             Routine failed. 
 ----------------------------------------------------------------------------*/
static INT32 _tuner_ctrlr_connect_ana_tuner(TUNER_CTRLR_INFO_T* pt_tunning_info)
{
    INT32               i4_return;

    /* Check if it is under valid state */
    if (t_tuner_ctrlr_state == TUNER_CTRLR_STATE_CONN)
    {
        return SVCTXR_NO_ACTION;
    }       
    
    i4_return = _tuner_ctrlr_set_ana_tuner(
                        pt_tunning_info,
                        FALSE,
                        0);

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: _tuner_ctrlr_disconnect_ana_tuner
 *
 * Description: This API perfroms tuner disconnection.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_INV_ARG          One or more of the arguments contain
 *                                  invalid data.
 *          SVCTXR_FAIL             Routine failed.  
 ----------------------------------------------------------------------------*/
static INT32 _tuner_ctrlr_disconnect_ana_tuner(VOID)
{
    INT32           i4_return = SVCTXR_OK;
    RM_COND_T       e_rm_cond;  

    /* Check if it is under valid state */
    if (t_tuner_ctrlr_state == TUNER_CTRLR_STATE_CLOSE)
    {
        return SVCTXR_NO_ACTION;
    }    
    
    if (h_rm_ana_tuner != NULL_HANDLE)
    {        
        if (t_tuner_ctrlr_state == TUNER_CTRLR_STATE_CONN)
        {            
            i4_return = rm_disconnect(h_rm_ana_tuner,
                                      RM_DISC_TYPE_IGNORE,
                                      NULL,
                                      0,
                                      &e_rm_cond);
        
            if (i4_return == RMR_OK)
            {
                t_tuner_ctrlr_state = TUNER_CTRLR_STATE_DISCONN;
                i4_return = SVCTXR_OK;
            }
            else if (i4_return == RMR_ASYNC_NFY)
            {
                /* Nothing to do here */
                t_tuner_ctrlr_state = TUNER_CTRLR_STATE_DISCONN;
                i4_return = SVCTXR_OK;
            }
            else
            {
                i4_return = SVCTXR_FAIL;
            }
        }            
                
        if (i4_return == SVCTXR_OK)
        {                
            i4_return = rm_close(h_rm_ana_tuner,
                                 FALSE,
                                 NULL);
    
            if (i4_return == RMR_OK)
            {
                t_tuner_ctrlr_state = TUNER_CTRLR_STATE_CLOSE;
                h_rm_ana_tuner = NULL_HANDLE;
                i4_return = SVCTXR_OK;
            }
        }            
    }        
    else        
    {
        i4_return = SVCTXR_INV_ARG;    
    }        

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: tuner_ctrlr_init
 *
 * Description: This API initializes the Tuner Controller.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_FAIL             Routine failed. 
 ----------------------------------------------------------------------------*/
INT32 tuner_ctrlr_init(VOID)
{
    INT32       i4_return;
    RM_COND_T   e_rm_cond;

    /* Open a pipe */
    i4_return = rm_open_pipe(NULL,
                             NULL,
                             1,
                             0,
                             NULL,
                             _tuner_ctrlr_rm_nfy_fct,
                             &h_rm_pipe,
                             &e_rm_cond);

    if (i4_return != RMR_OK)
    {
        return SVCTXR_FAIL;
    }
    
    t_g_cached_tuner_ctrlr_info.ui2_tsl_id     = TSL_NULL_ID;
    t_g_cached_tuner_ctrlr_info.ui2_tsl_rec_id = TSL_NULL_REC_ID;
    t_g_cached_tuner_ctrlr_info.ui4_tv_sys     = TV_SYS_MASK_NONE;
    b_g_cached_replace_freq                    = FALSE;
    ui4_g_cached_freq_to_replace               = 0;

    b_tuner_ctrlr_init = TRUE;

    return SVCTXR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: tuner_ctrlr_enable_ana_tuner
 *
 * Description: This API enables the analog tuner driver component.
 *
 * Inputs:  pt_tunning_info         The tunning information.
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_NOT_INIT         The Tuner Controller is not
 *                                  initialized.
 *          SVCTXR_INV_ARG          One or more of the arguments contain
 *                                  invalid data.
 *          SVCTXR_FAIL             Routine failed.  
 ----------------------------------------------------------------------------*/
INT32 tuner_ctrlr_enable_ana_tuner(TUNER_CTRLR_INFO_T* pt_tunning_info)
{
    /* Check arguments */
    if (!b_tuner_ctrlr_init)
    {
        return SVCTXR_NOT_INIT;
    }

    if (h_rm_pipe == NULL_HANDLE)
    {
        return SVCTXR_INV_ARG;
    }

    return (_tuner_ctrlr_connect_ana_tuner(pt_tunning_info));
}


/*-----------------------------------------------------------------------------
 * Name: tuner_ctrlr_disable_ana_tuner
 *
 * Description: This API disables the analog tuner driver component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_NOT_INIT         The Tuner Controller is not
 *                                  initialized.
 *          SVCTXR_INV_ARG          One or more of the arguments contain
 *                                  invalid data.
 *          SVCTXR_FAIL             Routine failed.  
 ----------------------------------------------------------------------------*/
INT32 tuner_ctrlr_disable_ana_tuner(VOID)
{
    /* Check arguments */
    if (!b_tuner_ctrlr_init)
    {
        return SVCTXR_NOT_INIT;
    }

    if (h_rm_pipe == NULL_HANDLE)
    {
        return SVCTXR_INV_ARG;
    }
    
    return (_tuner_ctrlr_disconnect_ana_tuner());
}

/*-----------------------------------------------------------------------------
 * Name: tuner_ctrlr_set_ana_tuner
 *
 * Description: This API enables the analog tuner driver component.
 *
 * Inputs:  pt_tunning_info         The tunning information.
 *          b_replace_freq          The flag to indicate to replace freq
 *          ui4_freq_to_replace     The frequency to replace the pt_tunning_info's
 * Outputs: -
 *
 * Returns: SVCTXR_OK               Routine successful.
 *          SVCTXR_NOT_INIT         The Tuner Controller is not
 *                                  initialized.
 *          SVCTXR_INV_ARG          One or more of the arguments contain
 *                                  invalid data.
 *          SVCTXR_FAIL             Routine failed.  
 ----------------------------------------------------------------------------*/
INT32 tuner_ctrlr_set_ana_tuner(
                    TUNER_CTRLR_INFO_T* pt_tunning_info,
                    BOOL                b_replace_freq,
                    UINT32              ui4_freq_to_replace)
{
    /* Check arguments */
    if (!b_tuner_ctrlr_init)
    {
        return SVCTXR_NOT_INIT;
    }

    if (h_rm_pipe == NULL_HANDLE)
    {
        return SVCTXR_INV_ARG;
    }

    return _tuner_ctrlr_set_ana_tuner(pt_tunning_info, b_replace_freq, ui4_freq_to_replace);
}
