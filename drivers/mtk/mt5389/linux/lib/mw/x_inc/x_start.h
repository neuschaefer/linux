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
 * $RCSfile: x_start.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Allen Horng $
 * $MD5HEX: c589c0eea0e5fa86862cd59821fe9e34 $
 *
 * Description:
 *         This header file contains Middleware startup definitions, which
 *         are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_START_H_
#define _X_START_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_START
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_dbg.h"
#include "x_start_common.h"

#ifndef NO_DT_INIT
#include "x_dt.h"
#endif

#ifndef NO_WGL_INIT
#include "u_gl.h"
#endif

#ifndef NO_DM_INIT
#include "u_dm.h"    
#endif

#ifndef NO_ATV_MANAGER_INIT
#include "x_atv_mngr.h"
#endif

#ifndef NO_DSM_INIT
#include "u_dsm.h"
#endif

#ifdef SYS_DRV_POST_START
typedef INT32 (*x_sys_drv_post_start) (VOID *pv_data);
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

typedef struct _X_CONFIG_T
{
    GEN_CONFIG_T  t_rtos_config;         /* RTOS specific configuration parameters */
    GEN_CONFIG_T  t_drv_config;          /* Driver specific configuration parameters */
    GEN_CONFIG_T  t_custom_config;       /* Customization specific configuration parameters */
    GEN_CONFIG_T  t_appl_config;         /* Application specific configuration parameters */

    /* Debug trace buffer and output device setup */
    UINT16  ui2_dbg_num_trace_recs;

    const CHAR*  ps_dbg_output_name;

    DBG_OUTPUT_TYPE_T  e_dbg_output_type;

    const VOID*  pv_dbg_output_info;

    const CHAR*  ps_model_name;
    const CHAR*  ps_serial_number;

    /* Handle library parameters */
    UINT16  ui2_num_handles;

    /* OS library parameters */
    VOID*  pv_mem_addr;

    SIZE_T  z_mem_size;

#ifndef NO_RM_INIT
    /* Resource Manager library parameters */
    UINT16  ui2_num_comps;
    THREAD_DESCR_T  t_rm_thread;
#endif

#ifndef NO_DT_INIT
    /* System DateTime library */
    DT_CONFIG_T  t_dt_config;
#endif

#ifdef MW_ISDB_SUPPORT /* Download Function */
#ifndef NO_DFM_INIT
    /* Download Function Manager */
    UINT16                      ui2_num_svc_objs;
    UINT16                      ui2_num_logo_objs;
#endif

#endif

#ifndef NO_DSM_INIT
    /* Device Status Manager. */
    DSM_CONFIG_T  t_dsm_config;
#endif
    
#ifndef NO_DB_CORE_INIT
    /* Core Database parameters */
    UINT16  ui2_cdb_num_db;
#endif

#ifndef NO_SVL_INIT
    /* SVL init parameters */
    UINT16  ui2_svl_max_num;
#endif

#ifndef NO_TSL_INIT
    /* TSL init parameters */
    UINT16  ui2_tsl_max_num;
#endif

#ifndef NO_NWL_INIT
    /* NWL init parameters */
    UINT16  ui2_nwl_max_num;
#endif    

#ifndef NO_SATL_INIT
    /* SATL init parameters */
    UINT16  ui2_satl_max_num;
#endif    
    

#ifndef NO_CM_INIT
    /* Connection Manager parameters */
    UINT16  ui2_cm_num_handlers;
#endif

#ifndef NO_IOM_INIT
    /* IO Manager parameters */
    BOOL            b_apdemon_irrc_enabled;
    THREAD_DESCR_T  t_iom_thread;
#endif

#ifndef NO_PM_INIT
    /* Process Manager library parameters */
    UINT16  ui2_pm_num_ca;
    UINT16  ui2_pm_num_service;
    UINT16  ui2_pm_num_comps;
    THREAD_DESCR_T  t_pm_thread;
#endif

#ifndef NO_SM_INIT
    /* Stream Manager library parameters */
    THREAD_DESCR_T  t_sm_thread;
#endif

#ifndef NO_SB_INIT
    /* SVL Buidler parameters */
    THREAD_DESCR_T  t_sb_thread;
#endif

#ifndef NO_EVCTX_INIT
    /* Event Context parameters */
    THREAD_DESCR_T  t_evctx_thread;
#endif

#ifndef NO_EDB_INIT
    /* Event Database parameters */
    UINT8           ui1_edb_max_num;
    THREAD_DESCR_T  t_edb_thread;
    THREAD_DESCR_T  t_edb_section_thread;
    SIZE_T          z_edb_mem_size;
#endif


#ifndef NO_RRCTX_INIT
    /* Rating Region Context parameters */
    THREAD_DESCR_T  t_rrctx_thread;
#endif

#ifndef NO_FLM_INIT
    /* Favorites List Manager init parameters */
    UINT16 ui2_flm_max_num;
    UINT16 ui2_flm_max_size;
#endif

#ifndef NO_ATV_MANAGER_INIT
    /* ATV manager configuration */
    X_ATV_MANAGER_CONFIG_T t_atv_mngr_cfg;
#endif

#ifndef NO_WGL_INIT
    /* Widget library parameters */
    UINT32          ui4_width;
    UINT32          ui4_height;
    GL_COLORMODE_T  e_colormode;
    BOOL            b_auto_scale_view;
    BOOL            b_no_inter_compose;
    BOOL            b_support_compose_anim;
    BOOL            b_osd_compress;
    BOOL            b_compressed_compose;
    UINT16          ui2_clut_sz;
    GL_COLOR_T*     pt_clut;
    GL_COLOR_T      t_color_key;
    UINT16          ui2_num_widgets;
    UINT8           ui1_max_floated_widget;
    UINT16          ui2_key_first_repeat_delay;
    UINT16          ui2_key_repeat_delay;
    THREAD_DESCR_T  t_wm_thread;
#endif

#ifndef NO_SVCTX_INIT
    /* service context parameters */
    UINT16         ui2_svctx_max_num;
    THREAD_DESCR_T t_svctx_thread;
#endif

    /* CLI parameters */
    THREAD_DESCR_T  t_cli_thread;

#ifndef NO_SECM_INIT
    /* Section Manager library parameters. */
    UINT16  ui2_log_filter;
    UINT16  ui2_sec_req;
    UINT16  ui2_max_req;
    THREAD_DESCR_T  t_secm_thread;
#endif

#ifndef NO_DM_INIT
    DM_INIT_PARM_T t_dm_init;
#endif

#ifndef NO_FONT_INIT
    /* Font Engine parameters */
    UINT32  ui4_hori_dpi;
    UINT32  ui4_vert_dpi;
#endif

#ifndef NO_VBIF_INIT
    /* VBI Filter parameters */
    THREAD_DESCR_T  t_vbif_thread;
#endif

#ifndef NO_CECM_INIT
    /* CEC Manager parameters */
    THREAD_DESCR_T  t_cecm_thread;
#endif

#ifdef SYS_DRV_POST_START
    x_sys_drv_post_start pt_drv_post_start_fct;
#endif

}   X_CONFIG_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* The following API's must be implemented by the manufacturer. */
extern INT32 x_appl_init (GEN_CONFIG_T*  pt_config);
extern INT32 x_custom_init (GEN_CONFIG_T*  pt_config);
extern INT32 x_custom_init_boot(GEN_CONFIG_T*  pt_config);
extern INT32 x_drv_init (GEN_CONFIG_T*  pt_config);


/* The following API is expported by the Middleware. */
/*------------------------------------------------------------------*/
/*! @brief This API initializes the configuration structure to default
 *              values.
 *  @param [in]     pt_config - References the configuration structure.
 *  @param [out]    pt_config - References the initialized configuration structure.
 *  @return Return the error code.
 *  @retval         INITR_OK  -     Routine successful.
 *  @retval         INITR_INV_ARG - Invalid argument.
 */
/*------------------------------------------------------------------*/
extern INT32 x_start_def_config (X_CONFIG_T*  pt_config);

/*------------------------------------------------------------------*/
/*! @brief This API is the entry point for the Middleware software
 *              initialization.
 *  @param [in]     pt_config - References the configuration structure.
 *  @param [out]     - None
 *  @return Return the error code.
 *  @retval         INITR_OK  -     Routine successful.
 *  @retval         INITR_ALREDAY_INIT - Alreday initialized.
 *  @retval         INITR_INV_ARG - Invalid argument.
 */
/*------------------------------------------------------------------*/
extern INT32 x_start (X_CONFIG_T*  pt_config);

/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_START */
/*----------------------------------------------------------------------------*/

#endif /* _X_START_H_ */
