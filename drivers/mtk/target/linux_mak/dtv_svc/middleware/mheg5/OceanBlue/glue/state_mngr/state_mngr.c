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
 *
 * SWAuthor: Chun Zhou
 *
 * Description: 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "dbg/u_dbg.h"
#include "dbg/x_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "os/inc/x_os.h"
#include "mheg5/u_mheg5.h"
#include "mheg5/x_mheg5.h"
#include "mheg5/mheg5.h"
#include "inc/x_common.h"
#include "dt/x_dt.h"
#include "chip_spec/u_chip_spec.h"
#include "chip_spec/x_chip_spec.h"
#include "mheg5/OceanBlue/glue/state_mngr/state_mngr.h"
#include "mheg5/OceanBlue/glue/si_loader/sil.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#ifdef MHEG5_IC_SUPPORT
#include "mheg5/OceanBlue/glue/ic/mheg5_ic.h"
#endif
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"

/* MHEG-5 stack header files */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_control.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL

#include "mheg5/OceanBlue/glue/misc/mheg5_misc.h"

#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_misc_get_dbg_level()

#endif

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 stmg_init()
{
    
    return MHEG5R_OK;
}

INT32 stmg_mheg5_open(MHEG5_SETUP_T* pt_setup)
{
    tmMHEG5InstanceSetup_t  t_setup;
    tmMHEG5Err_t            t_err;
    INT32                   i4_ret;
    BOOL                    b_is_bond_enable = FALSE;

    DBG_INFO(("{MHEG5 Stack} %s ....\n", __FUNCTION__));

    i4_ret = MHEG5R_INIT_FAIL;

    if (pt_setup == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if (pt_setup->ui1_ver != 0)
    {
        return MHEG5R_INV_ARG;
    }
    
    /* initialize MHEG-5 stack */
#ifdef MW_MHEG5_ARGB8888_SUPPORT
    t_setup.receiverCapabilities.colourDepth = (U8BIT)32;
#else
    t_setup.receiverCapabilities.colourDepth = (U8BIT)16;
#endif
#ifndef MHEG5_HD_SUPPORT
    b_is_bond_enable = FALSE;
    i4_ret = CHIPSPECR_ERROR;
#else
    i4_ret = x_chip_spec_get_MHEGHD_bond(&b_is_bond_enable);
#endif
    if (i4_ret == CHIPSPECR_OK && b_is_bond_enable)
    {
        t_setup.receiverCapabilities.scalingCapabilities  = MHEG5_SCALE_CAPS_HDGRAPHICS_AND_HDVIDEO;
        #ifdef MHEG5_IC_SUPPORT
        t_setup.receiverCapabilities.mhegExtensionSupport = (U8BIT)MHEG_EXTENSION_INTERACTION_CHANNEL;
        #else
        t_setup.receiverCapabilities.mhegExtensionSupport = (U8BIT)MHEG_EXTENSION_NONE;
        #endif
        t_setup.receiverCapabilities.screenWidth  = (U16BIT)1280;
        t_setup.receiverCapabilities.screenHeight = (U16BIT)720;
    }
    else
    {
        t_setup.receiverCapabilities.scalingCapabilities  = MHEG5_SCALE_CAPS_DOUBLE;
        t_setup.receiverCapabilities.mhegExtensionSupport = (U8BIT)MHEG_EXTENSION_NONE;
        t_setup.receiverCapabilities.screenWidth  = (U16BIT)720;
        t_setup.receiverCapabilities.screenHeight = (U16BIT)576;
    }

    #ifdef MHEG5_HD_SUPPORT    
    t_setup.receiverCapabilities.mhegExtensionSupport |= MHEG_EXTENSION_IC_STREAMING_HD;    
    #endif

    t_setup.memoryRegionHandle               = pt_setup->pv_mem_rgn;
    t_setup.memoryRegionSize                 = (U32BIT) pt_setup->z_mem_rgn;
    t_setup.taskPriority                     = (U32BIT) pt_setup->ui1_task_pri;
    t_setup.numSectionBuffers                = (U32BIT) pt_setup->ui4_sec_buf_num;
    t_setup.numberOfSimultaneousAudioStreams = 1;
    t_setup.simultaneousSubtitlesAndGraphics = (BOOLEAN) pt_setup->b_sub_with_gfx;

    x_strncpy(
        (CHAR*) (t_setup.manufacturerPlatformString), 
        pt_setup->s_dvp_name,
        sizeof(t_setup.manufacturerPlatformString) - 1);

    t_setup.receiverCapabilities.videoOffScreenCapabilities  = TRUE;
    t_setup.receiverCapabilities.bitmapOffScreenCapabilities = TRUE;
    
    t_setup.nvmSize = MHEG5_NVM_MINIMUM_SIZE;

#ifdef MHEG5_IC_SUPPORT
    t_setup.streamerBuffer = x_mem_alloc(0x300000);

    if (t_setup.streamerBuffer != NULL)
    {
        t_setup.streamerBufferSize = 0x300000;
        t_setup.receiverCapabilities.mhegExtensionSupport |= MHEG_EXTENSION_IC_STREAMING;
    }
    else
    {
        t_setup.streamerBufferSize = 0;
    }
#endif    
    t_setup.countryProfile = CPROFILE_ALL;

    t_err = tmMHEG5Open(&t_setup);
    
    /* convert error codes */
    i4_ret = mheg5_obs_convert_err_code(t_err);

    if (MHEG5R_OK == i4_ret)
    {
        MHEG5_DBG_INFO(("{MHEG5 STM MW} MHEG-5 engine OPENED...\n\n"));
    }

    return i4_ret;
}
 
INT32 stmg_mheg5_close(VOID)
{
    tmMHEG5Err_t            t_err;
    INT32                   i4_ret;
    
    DBG_INFO(("{MHEG5 Stack} %s ....\n", __FUNCTION__));
    
    t_err = tmMHEG5Close();

    i4_ret = mheg5_obs_convert_err_code(t_err);

    /* reset global info */
    stmg_init();

    return i4_ret;
}
 

INT32 stmg_mheg5_start_exec(MHEG5_START_EXEC_T* pt_start_exec)
{
    tmMHEG5DvbLocator_t     tDvbLocator;
    tmMHEG5Err_t            t_err;
    INT32                   i4_ret;

    DBG_INFO(("{MHEG5 Stack} %s .....\n", __FUNCTION__));

    if (pt_start_exec == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if ((pt_start_exec->ui1_ver != 0) || (pt_start_exec->pt_dvb_loc == NULL))
    {
        return MHEG5R_INV_ARG;
    }
    
    /* restart MHEG-5 stack with a new service */
    i4_ret = x_mheg5_set_dvb_locator( pt_start_exec->pt_dvb_loc->ui2_on_id, 
                                      pt_start_exec->pt_dvb_loc->ui2_ts_id,
                                      pt_start_exec->pt_dvb_loc->ui2_svc_id );

    if (i4_ret != MHEG5R_OK)
    {
        return i4_ret;
    }
    
    tDvbLocator.original_network_id = (U16BIT)pt_start_exec->pt_dvb_loc->ui2_on_id;
    tDvbLocator.transport_stream_id = (U16BIT)pt_start_exec->pt_dvb_loc->ui2_ts_id;
    tDvbLocator.service_id          = (U16BIT)pt_start_exec->pt_dvb_loc->ui2_svc_id;
    tDvbLocator.ComponentTag        = (S32BIT)pt_start_exec->pt_dvb_loc->i4_comp_tag;

    t_err = tmMHEG5Start(&tDvbLocator);
    
    i4_ret = mheg5_obs_convert_err_code(t_err);

    return i4_ret;
}
 
INT32 stmg_mheg5_stop_exec( MHEG5_STOP_EXEC_T* pt_stop_exec)
{
    tmMHEG5Err_t t_err;
    INT32        i4_ret;
    DBG_INFO(("{MHEG5 Stack} %s ....\n", __FUNCTION__));

    if (pt_stop_exec == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if (pt_stop_exec->ui1_ver != 0)
    {
        return MHEG5R_INV_ARG;
    }

    t_err = tmMHEG5Stop((U8BIT)(pt_stop_exec->ui1_acts));

    i4_ret = mheg5_obs_convert_err_code(t_err);

    return i4_ret;
}
     
INT32 stmg_mheg5_pause_exec(MHEG5_PAUSE_EXEC_T* pt_pause_exec)
{
    tmMHEG5Err_t t_err;
    INT32        i4_ret;
    
    DBG_INFO(("{MHEG5 Stack} %s, pt_pause_exec->b_on = %d\n", __FUNCTION__, (UINT8)pt_pause_exec->b_on));
    
    if (pt_pause_exec == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if (pt_pause_exec->ui1_ver != 0)
    {
        return MHEG5R_INV_ARG;
    }
#ifdef MHEG5_IC_SUPPORT
    if (pt_pause_exec->b_on == FALSE)
    {
        mheg5_ics_handle_engine_pause();
    }
#endif
    t_err = tmMHEG5SetActiveState((BOOLEAN)(pt_pause_exec->b_on));

    i4_ret = mheg5_obs_convert_err_code(t_err);
#ifdef MHEG5_IC_SUPPORT
    if (pt_pause_exec->b_on == TRUE)
    {
        mheg5_ics_handle_engine_resume();
    }
#endif
    return i4_ret;
}

