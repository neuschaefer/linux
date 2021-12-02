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
 * $RCSfile: dbg_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the debug libraries references to the Resource
 *         Manager.  references the implementation of the debug library. Note
 *         that many API's are only enabled if this module is compiled with
 *         the define DEBUG set.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/


#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "res_mngr/drv/x_rs_232.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm_dev_types.h"


/* If the Debug library is not compiled with the definition  */
/* DEBUG don't allocate space for code and data. Also, if no */
/* external debug device is used then compile the following  */
/* API's. In case EXT_DBG_DEV is set then the manufacturer   */
/* must have supplied the following two API's.               */
#ifdef DEBUG
#ifndef EXT_DBG_DEV
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static HANDLE_T  h_output_dev;                /* References the output device. */

static x_dbg_data_rec_fct  pf_dbg_data_rec;   /* References the data receive callback. */


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static VOID handle_nfy (HANDLE_T   h_obj,
                        UINT8      ui1_port,
                        RM_COND_T  e_nfy_cond,
                        VOID*      pv_tag,
                        UINT32     ui4_data);


/*-----------------------------------------------------------------------------
 * Name: handle_nfy
 *
 * Description: This is a dummy handle callback function, called by the 
 *              Resource Manager. It does not do anything.
 *
 * Inputs:  h_obj       Contains the handle.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_tag      Contains the notify tag.
 *          ui4_data    Contains additional data, depending on the notify
 *                      condition.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID handle_nfy (HANDLE_T   h_obj,
                        UINT8      ui1_port,
                        RM_COND_T  e_nfy_cond,
                        VOID*      pv_tag,
                        UINT32     ui4_data)
{
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_open_output
 *
 * Description: This API opens the debug libraries output device.
 *
 * Inputs:  ps_output_name  Specifies the output device for debug statements.
 *          e_output_type   Indicates the output information referenced by
 *                          pv_setup_info.
 *          pv_output_info  References the output information.
 *
 * Outputs: -
 *
 * Returns: DBGR_OK         Routine successful.
 *          DBGR_OPEN_FAIL  The open operation failed for some reason.
 ----------------------------------------------------------------------------*/
INT32 x_dbg_open_output (const CHAR*        ps_output_name,
                         DBG_OUTPUT_TYPE_T  e_output_type,
                         const VOID*        pv_output_info)
{
    INT32                      i4_return;
    SIZE_T                     z_get_len;
    RM_COND_T                  e_cond;
    RS_232_CAPABILITY_INFO_T   t_rs_232_cap_info;
    const RS_232_SETUP_INFO_T* pt_rs_232_setup_info;


    i4_return = DBGR_OPEN_FAIL;

    switch (e_output_type)
    {
		case DBG_OUTPUT_TYPE_IGNORE:
			i4_return = DBGR_OK;

			h_output_dev    = NULL_HANDLE;
			pf_dbg_data_rec = NULL;
			break;

        case DBG_OUTPUT_TYPE_SERIAL_PORT:
            /* Open a RS-232 output device. A RS-232 device requires some setup */
            /* information. Hence, "pv_output_info" is not allowed to be NULL.  */
            if (pv_output_info != NULL)
            {
                if (rm_open_comp (NULL_HANDLE,
                                  OBJ_COMP_REL_IGNORE,
                                  DRVT_COM_RS_232,
                                  ANY_COMP_ID,
                                  ps_output_name,
                                  CTRL_TYPE_SINGLE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  handle_nfy,
                                  &h_output_dev,
                                  &e_cond) == RMR_OK)
                {
                    if (rm_cond_chg (e_cond, RM_COND_IGNORE, COND_GRP_OBJ_STATE) == RM_COND_OPENED)
                    {
                        /* Only proceed if the component has been opened. */
                        /* First, get the serial ports capability and make certain that */
                        /* the settings in argument "pv_output_info" are within the     */
                        /* capabilities of the serial port.                             */
                        z_get_len = sizeof (RS_232_CAPABILITY_INFO_T);

                        if (rm_get (h_output_dev,
                                    FALSE,
                                    ANY_PORT_NUM,
                                    RS_232_GET_TYPE_CAPABILITY,
                                    ((VOID*) &t_rs_232_cap_info),
                                    &z_get_len,
                                    NULL) == RMR_OK)
                        {
                            pt_rs_232_setup_info = (const RS_232_SETUP_INFO_T*) pv_output_info;

                            if (((t_rs_232_cap_info.ui4_speed    & (1 << (pt_rs_232_setup_info->e_speed)))    != 0)  &&
                                ((t_rs_232_cap_info.ui4_data_len & (1 << (pt_rs_232_setup_info->e_data_len))) != 0)  &&
                                ((t_rs_232_cap_info.ui4_parity   & (1 << (pt_rs_232_setup_info->e_parity)))   != 0)  &&
                                ((t_rs_232_cap_info.ui4_stop_bit & (1 << (pt_rs_232_setup_info->e_stop_bit))) != 0))
                            {
                                /* Ok, the debug output port settings are within the serail */
                                /* port drivers capability. Hence, perform the setup.       */
                                if (rm_set (h_output_dev,
                                            FALSE,
                                            ANY_PORT_NUM,
                                            RS_232_SET_TYPE_SETUP,
                                            pv_output_info,
                                            sizeof (RS_232_SETUP_INFO_T),
                                            NULL) == RMR_OK)
                                {
                                    i4_return = DBGR_OK;
                                }
                            }
                        }
                    }
                }

                if (i4_return != DBGR_OK)
                {
                    /* Oops, some error occured. Cleanup. */
                    rm_close (h_output_dev, FALSE, NULL);
                }
            }
            break;

        default:
            /* Simply ignore all other types. */
            break;
    }

    /* Make certain that the output device handle */
    /* is set to NULL_HANDLE if not successful.   */
    if (i4_return != DBGR_OK)
    {
        h_output_dev    = NULL_HANDLE;
        pf_dbg_data_rec = NULL;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_put_stmt
 *
 * Description: This API sends a debug statements to the output device.
 *
 * Inputs:  ps_stmt  References the statement.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_dbg_put_stmt (const CHAR* ps_stmt)
{
    INT32         i4_state;
    DRV_TYPE_T    e_drv_type;
    HANDLE_TYPE_T e_type;


    /* If the handle is unknown then abort. Serious error. */
    if (x_handle_get_type (h_output_dev, &e_type) != HR_OK)
    {
        e_type = DRVT_UNKNOWN;
    }

    switch (e_type)
    {
        case RMT_ELEM:
            if (rm_get_comp_info_from_handle (h_output_dev, &e_drv_type, NULL, NULL, NULL, NULL) == RMR_OK)
            {
                switch (e_drv_type)
                {
                    case DRVT_COM_RS_232:
                        /* Serial port. Simply send one character after */
                        /* the next to the output device.               */
                        i4_state = RMR_OK;

                        while ((i4_state == RMR_OK)  &&
                               ((*ps_stmt) != '\0'))
                        {
                            i4_state = rm_set (h_output_dev,
                                               FALSE,
                                               ANY_PORT_NUM,
                                               RS_232_SET_TYPE_XMT_DATA,
                                               ((const VOID*) ((UINT32) (*(ps_stmt++)))),
                                               1,
                                               NULL);
                        }
                        break;

                    default:
                        /* Unsupported output device. Simply ignore */
                        /* the debug statment and add support.      */
                        break;
                }
            }
            break;

        default:
            /* Unsupported output device. Simply ignore */
            /* the debug statment and add support.      */
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_dbg_reg_data_rec
 *
 * Description: This API registers a data receive callback function which must
 *              be called for every received data Byte.
 *
 * Inputs:  pf_data_rec  References the callback function.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID x_dbg_reg_data_rec (x_dbg_data_rec_fct  pf_data_rec)
{
	CRIT_STATE_T t_state;


	t_state = x_crit_start ();

    pf_dbg_data_rec = pf_data_rec;

	x_crit_end (t_state);
}

#endif
#endif
