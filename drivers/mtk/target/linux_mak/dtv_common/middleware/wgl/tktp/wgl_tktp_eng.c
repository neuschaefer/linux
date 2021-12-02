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
 * $RCSfile: wgl_tktp_eng.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/6 $
 * $SWAuthor: TC Yin $
 * $MD5HEX: 2c46162b759413f7bb637f26bd65e2f5 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "wgl/wgl_bdr_tools.h"
#include "wgl/tktp/wgl_tktp_eng.h"
#include "wgl/tktp/wgl_tktp.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#undef  DBG_MODULE_NAME
#define DBG_MODULE_NAME "TKTPENG"

#define IN
#define OUT


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_eng_wait_here(
    IN      HANDLE_T                    h_sema_eng);


/*-----------------------------------------------------------------------------
                    functions
 ----------------------------------------------------------------------------*/
VOID wgl_tktp_eng_core_proc(VOID *pv_tag)
{
    TKTP_ENG_PARAM_T    *pt_param = (TKTP_ENG_PARAM_T *)pv_tag;
    TKTP_ENG_INFO_T     t_info;
    INT32               i4_ret;
    BOOL                b_keep_going;
    HANDLE_TYPE_T       e_type;

    if(pt_param->b_sleep_at_start)
    {
        INFORM_ERR(_tktp_eng_wait_here(pt_param->h_sema_eng));
    }
    
    b_keep_going = TRUE;
    while(b_keep_going)
    {
        /* check the validity of the widget handle */
        if (HR_OK!=x_handle_get_type(pt_param->h_tktp, &e_type) 
            || e_type!=HT_WGL_WIDGET_TKTP )
        {
            /* the widget has been destroyed, quit */
            b_keep_going = FALSE;
            break;
        }

        /* call the tick procedure */
        i4_ret = wgl_tktp_engine_tick_proc(pt_param->h_tktp, &t_info);
        
        if(!WGL_IS_FAIL(i4_ret))
        {
            switch(t_info.e_msg) 
            {
                case TKTP_ENG_MSG_NEXT_TICK:
                    x_thread_delay(t_info.ui4_next_delay);
        	        break;
                case TKTP_ENG_MSG_SLEEP:
                    INFORM_ERR(x_sema_lock(
                        t_info.h_sema_eng, 
                        X_SEMA_OPTION_WAIT));
        	        break;
                case TKTP_ENG_MSG_DESTROY:
                    b_keep_going = FALSE;
                    break;
                default:
                    INFORM_ERR(WGLR_INTERNAL_ERROR);
            }
        }        
        else
        {
            if(i4_ret==WGLR_INV_HANDLE)
            {
                /* the widget has been destroyed, quit */
                b_keep_going = FALSE;
                break;
            }
            else
            {
                INFORM_ERR(i4_ret);
            }
        }    
    }

    x_thread_exit();
    
}


/*-----------------------------------------------------------------------------
                    internal functions
 ----------------------------------------------------------------------------*/
static INT32 _tktp_eng_wait_here(
    IN      HANDLE_T                    h_sema_eng)
{
    INT32   i4_ret;
    /* lock the semaphore the first time */
    i4_ret = x_sema_lock(
        h_sema_eng, 
        X_SEMA_OPTION_NOWAIT);        
    if(i4_ret==OSR_WOULD_BLOCK)
    {
        INFORM_ERR(WGLR_INTERNAL_ERROR);
    }
    else if(i4_ret != OSR_OK)
    {
        INFORM_ERR(i4_ret);
    }

    /* lock the semaphore the second time and wait here*/
    INFORM_ERR(x_sema_lock(
        h_sema_eng, 
        X_SEMA_OPTION_WAIT));

    return WGLR_OK;
}

