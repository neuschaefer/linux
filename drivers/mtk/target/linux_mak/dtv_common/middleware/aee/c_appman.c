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
 * $RCSfile: c_appman.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/3 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 7073777ef840de31d560641fd88f5e7f $
 *
 * Description: 
 *         This file contains the implementation of the appman APIs that are
 *         exported to applications.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "aee/c_appman.h"
#include "aee/x_appman.h"
#include "aee/aee.h"
#include "handle/handle.h"


/*-----------------------------------------------------------------------------
 * Name: c_app_start
 *
 * Description: This API starts an application. The application's init function
 *              will be called first since this API automatically sends a
 *              INIT message to the application's message loop.
 *
 * Inputs:  pt_fct_tbl  References the application's function table.
 *          pt_desc     References the application's description (resources,
 *                      authorization flags, etc).
 *          ps_name     References the application's name.
 *          pf_nfy      References the notify function.
 *          pv_tag      References the tag passed to the notify function.
 *
 * Outputs: ph_app  Contains the application handle.
 *
 * Returns: AEER_OK                    Routine successful.
 *          AEER_INV_ARG               Invalid argument.
 *          AEER_FAIL                  Routine failed.
 *          AEER_AEE_NO_RIGHTS         Permission denied.
 *          AEER_AEE_OUT_OF_RESOURCES  Not enough resources available.
 ----------------------------------------------------------------------------*/
INT32 c_app_start (APP_FCT_TBL_T*     pt_fct_tbl,
                   const APP_DESC_T*  pt_desc,
                   const CHAR*        ps_name,
                   x_app_nfy_fct      pf_nfy,
                   VOID*              pv_tag,
                   HANDLE_T*          ph_app)
{
    HANDLE_T  h_aux;
    INT32     i4_res;
    
    i4_res = aee_grab_handle_resource(AEE_FLAG,
                                      1,
                                      & h_aux);
    
    switch (i4_res)
    {
        case AEER_OK:
        {
            i4_res = x_app_start(pt_fct_tbl, pt_desc,
                                 ps_name,
                                 pf_nfy, pv_tag,
                                 ph_app);
            
            if (i4_res == AEER_OK)
            {
                handle_link_to_aux(h_aux, *ph_app);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_res = AEER_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_res = AEER_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: c_app_exit
 *
 * Description: This API sends a message to the application for it to exit.
 *
 * Inputs:  h_app        Contains the handle to the application.
 *          e_exit_mode  Contains the exit mode.
 *          pf_nfy       References the notify function.
 *          pv_tag       References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 c_app_exit (HANDLE_T         h_app,
                  APP_EXIT_MODE_T  e_exit_mode,
                  x_app_nfy_fct    pf_nfy,
                  VOID*            pv_tag)
{
    return x_app_exit(h_app, e_exit_mode, pf_nfy, pv_tag);
}

/*-----------------------------------------------------------------------------
 * Name: c_app_pause
 *
 * Description: This API sends a message to the application for it to pause.
 *
 * Inputs:  h_app   Contains the handle to the application.
 *          pf_nfy  References the notify function.
 *          pv_tag  References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 c_app_pause (HANDLE_T       h_app,
                   x_app_nfy_fct  pf_nfy,
                   VOID*          pv_tag)
{
    return x_app_pause(h_app, pf_nfy, pv_tag);
}

#ifdef MW_FAST_INIT
/*-----------------------------------------------------------------------------
 * Name: c_app_post_init
 *
 * Description: This API sends a message to the application for it to post init.
 *
 * Inputs:  h_app   Contains the handle to the application.
 *          pf_nfy  References the notify function.
 *          pv_tag  References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 c_app_post_init (HANDLE_T       h_app,
                   x_app_nfy_fct  pf_nfy,
                   VOID*          pv_tag)
{
    return x_app_post_init(h_app, pf_nfy, pv_tag);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: c_app_resume
 *
 * Description: This API sends a message to the application for it to resume.
 *
 * Inputs:  h_app   Contains the handle to the application.
 *          pf_nfy  References the notify function.
 *          pv_tag  References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK    Routine successful.
 *          AEER_FAIL  Routine failed.
 ----------------------------------------------------------------------------*/
INT32 c_app_resume (HANDLE_T       h_app,
                    x_app_nfy_fct  pf_nfy,
                    VOID*          pv_tag)
{
    return x_app_resume(h_app, pf_nfy, pv_tag);
}

/*-----------------------------------------------------------------------------
 * Name: c_app_send_msg
 *
 * Description: This API sends a message to the application. The format of the
 *              message is not known by the application manager.
 *
 * Inputs:  h_app      Contains the handle to the application.
 *          ui4_type   Contains the type of the message.
 *          pv_msg     References the message data. Can be NULL.
 *          z_msg_len  Contains the length (in bytes) of the message data.
 *          pf_nfy     References the notify function.
 *          pv_tag     References the tag passed to the notify function.
 *
 * Outputs: -
 *
 * Returns: AEER_OK              Routine successful.
 *          AEER_OUT_OF_MEMORY   Out of memory.
 *          AEER_FAIL            Routine failed.
 ----------------------------------------------------------------------------*/
INT32 c_app_send_msg (HANDLE_T       h_app,
                      UINT32         ui4_type,
                      const VOID*    pv_msg,
                      SIZE_T         z_msg_len,
                      x_app_nfy_fct  pf_nfy,
                      VOID*          pv_tag)
{
    return x_app_send_msg(h_app, ui4_type, pv_msg, z_msg_len, pf_nfy, pv_tag);
}
