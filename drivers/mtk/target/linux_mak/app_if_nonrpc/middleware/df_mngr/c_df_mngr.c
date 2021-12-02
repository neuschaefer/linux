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
 * $RCSfile: c_df_mngr.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: c8679ab5c7a96d6727163313dde1dfdc $
 *
 * Description:
 *         This file contain implementation of Download function manager API's,
 *         which are exported to application components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "x_df_mngr.h"
#include "handle/handle.h"
#include "aee/aee.h"

/*----------------------------------------------------------------------
 * Name: c_dfm_open
 *
 * Description: This function create a handle to the specified DFM 
 *              object by its name value.    
 *
 * Inputs: 
 *   ps_dfm_name:  Specifies the DFM object name.  
 *                
 *
 * Outputs:
 *   ph_dfm:      Handle to the DFM object.
 *
 * Returns: 
 *   DFMR_OK               DFM handle is returned. 
 *   DFMR_OUT_OF_HANDLE    No more handle available.
 *   DFMR_OUT_OF_MEM       Not enough memory to create the DFM control 
 *                         structure.
 *   DFMR_NOT_FOUND        The specified DFM name (e.g. 
 *                         'ps_dfm_name') is not found.
 *   DFMR_INV_ARG          'ph_dfm' is NULL.
 *   DFMR_AEE_OUT_OF_RESOURCES Can not allocate AEE resources.
 *   DFMR_AEE_NO_RIGHTS        No right for the AEE resources.  
 -----------------------------------------------------------------------*/
extern INT32    c_dfm_open(
                    const CHAR*         ps_dfm_name,
                    HANDLE_T*           ph_dfm
                    )
{
    HANDLE_T  h_aux;
    INT32     i4_rc;

    i4_rc = aee_grab_handle_resource(AEE_FLAG,
                                     1,
                                     &h_aux);

    switch (i4_rc)
    {
        case AEER_OK:
        {
            i4_rc = x_dfm_open(ps_dfm_name, ph_dfm);
            
            if (i4_rc == DFMR_OK)
            {
                handle_link_to_aux(h_aux, *ph_dfm);
            }
            else
            {
                aee_release_handle_resource(1);
            }
        }
        break;

        case AEER_OUT_OF_RESOURCES:
        {
            i4_rc = DFMR_AEE_OUT_OF_RESOURCES;
        }
        break;

        default:
        {
            i4_rc = DFMR_AEE_NO_RIGHTS;
        }
        break;
    }
    
    return i4_rc;
}
/*----------------------------------------------------------------------
 * Name: c_dfm_close
 *
 * Description: This function closes the specified handle to the DFM 
 *              object.  Note: the DFM data object that this
 *              handle refer to remain active. 
 *
 * Inputs: 
 *   h_svl  Handle to DFM object.
 *
 * Outputs: None
 *
 * Returns: 
 *   DFMR_OK           DFM handle is freed. 
 *   DFMR_INV_HANDLE   Bad handle.
 *   
 -----------------------------------------------------------------------*/
extern INT32    c_dfm_close(
                    HANDLE_T            h_dfm
                    )
{
    INT32 i4_rc;
    
    i4_rc = x_dfm_close(h_dfm);

    if ( i4_rc == DFMR_OK )
    {
        aee_release_handle_resource(1);
    }
    return i4_rc;
}


/*---------------------------------------------------------------------------
 * Name: c_dfm_load_logo
 *
 * Description: This API performs a DFManager load logo operation.
 *
 * Inputs:  h_dfm        Contains the DFM object.
 *          ui2_on_id    Contains the onid which shall be loaded.
 *          ui2_svc_id   Contains the service id which shall be loaded.
 *          pv_tag       Contains a notify tag value.
 *          pt_nfy       References the notify function.
 *
 * Outputs: ph_logo    Contains a handle which references the newly allocate
 *                    logo object.
 *          pe_cond   Contains the condition of the load logo object.
 *
 * Returns: DFMR_OK                   Routine successful.
 *          DFMR_NOT_INIT             The Table Manager is not initialized.
 *          DFMR_INV_ARG              One or more of the arguments contain
 *                                     invalid data.
 *          DFMR_INV_HANDLE           Invalid handle.
 *          DFMR_OUT_OF_HANDLES       No handles available.
 *          DFMR_OUT_OF_RESOURCES     Out of resources.
 *          DFMR_INV_HANDLE_STATE     Invalid handle state.
 *          DFMR_AEE_OUT_OF_RESOURCES
 *          DFMR_AEE_NO_RIGHTS
 --------------------------------------------------------------------------*/
extern INT32    c_dfm_load_logo(
                    HANDLE_T            h_dfm,
                    UINT16              ui2_on_id,
                    UINT16              ui2_svc_id,
                    UINT16              ui2_scan_idx,
                    VOID*               pv_tag,
                    x_dfm_nfy_fct       pt_nfy,
                    HANDLE_T*           ph_logo,
                    DFM_COND_T*         pe_cond
                    )
{
    INT32    i4_result;
    HANDLE_T h_aux;

    i4_result = aee_grab_handle_resource(AEE_FLAG, 1, &h_aux);

    switch(i4_result)
    {
    case AEER_OK:
        i4_result = x_dfm_load_logo(h_dfm, ui2_on_id, ui2_svc_id, ui2_scan_idx, pv_tag,
                                  pt_nfy, ph_logo, pe_cond);
                      
        if (i4_result == DFMR_OK)
        {
            handle_link_to_aux(h_aux, *ph_logo);
        }
        else
        {
            aee_release_handle_resource(1);
        }
        break;
    case AEER_OUT_OF_RESOURCES:
        i4_result = DFMR_AEE_OUT_OF_RESOURCES;
        break;
    default:
        i4_result = DFMR_AEE_NO_RIGHTS;
        break;
    }

    return(i4_result);
}
/*---------------------------------------------------------------------------
 * Name: c_dfm_get_cond
 *
 * Description: This API returns a load logo objects state or condition.
 *
 * Inputs:  h_dfm  Contains the DFM object.
 *          h_logo Contains the Logo object.
 *
 * Outputs: pe_cond  Contains the load logo state or condition.
 *
 * Returns: DFMR_OK          Routine successful.
 *          DFMR_NOT_INIT    The DFM is not initialized.
 *          DFMR_INV_ARG     One or more of the arguments contain invalid data.
 *          DFMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
extern INT32    c_dfm_get_cond(
                    HANDLE_T            h_dfm,
                    HANDLE_T            h_logo,
                    DFM_COND_T*         pe_cond
                    )
{
    return x_dfm_get_cond(h_dfm, h_logo, pe_cond);
}

/*---------------------------------------------------------------------------
 * Name: c_dfm_get_logo_type
 *
 * Description: This API performs a service logo get operation.
 *
 * Inputs:  h_dfm        Contains the DFM object.
 *          h_logo       Contains the Logo object.
 *
 * Outputs: pui1_logo_types      Contains the logo type returned by the get operation.
 *          pe_cond              Contains the condition of the logo object.
 *
 * Returns: DFMR_OK                Routine successful.
 *          DFMR_NOT_INIT          The DFM is not initialized.
 *          DFMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          DFMR_INV_HANDLE        Invalid handle.
 *          DFMR_NO_SUPPORT        No support.
 *          DFMR_INV_HANDLE_STATE  Invalid handle state.
 *          DFMR_NOT_ENOUGH_SPACE  Not enough space.
 *          DFMR_INV_GET_TYPE      Invalid et operation.
 *          DFMR_INV_GET_INFO      Invalid get operation data.
 --------------------------------------------------------------------------*/
extern INT32    c_dfm_get_logo_type(
                    HANDLE_T            h_dfm,
                    HANDLE_T            h_logo,
                    UINT8*              pui1_logo_types,/* |= */
                    DFM_COND_T*         pe_cond
                    )
{
    return x_dfm_get_logo_type(h_dfm, h_logo, pui1_logo_types, pe_cond);
}


/*---------------------------------------------------------------------------
 * Name: c_dfm_get_logo_data
 *
 * Description: This API performs a service logo get operation.
 *
 * Inputs:  h_dfm        Contains the DFM object.
 *          h_logo       Contains the Logo object.
 *          t_logo_type  Contains the logo type(e.g. SD,HD, simple logo).
 *
 * Outputs: ppt_img_data         Contains the WGL_IMG_T type returned by the 
 *                               get operation.
 *          pe_cond              Contains the condition of the logo object.
 *
 * Returns: DFMR_OK                Routine successful.
 *          DFMR_NOT_INIT          The DFM is not initialized.
 *          DFMR_INV_ARG           One or more of the arguments contain invalid
 *                                data.
 *          DFMR_INV_HANDLE        Invalid handle.
 *          DFMR_NO_SUPPORT        No support.
 *          DFMR_INV_HANDLE_STATE  Invalid handle state.
 *          DFMR_NOT_ENOUGH_SPACE  Not enough space.
 *          DFMR_INV_GET_TYPE      Invalid et operation.
 *          DFMR_INV_GET_INFO      Invalid get operation data.
 --------------------------------------------------------------------------*/
extern INT32    c_dfm_get_logo_data(
                    HANDLE_T            h_dfm,
                    HANDLE_T            h_logo,
                    DFM_LOGO_TYPE_T     t_logo_type,
                    WGL_HIMG_TPL_T*     pt_img,/*this is a WGL_HIMG_TPL_T, Don't free it*/
                    DFM_COND_T*         pe_cond
                    )
{
    return x_dfm_get_logo_data(h_dfm, h_logo, t_logo_type, pt_img, pe_cond);
}

/*---------------------------------------------------------------------------
 * Name: c_dfm_free_logo
 *
 * Description: This API frees a logo object.
 *
 * Inputs:  h_dfm   Contains the DFM object.
 *          h_logo  Contains the service logo object.
 *
 * Outputs: -
 *
 * Returns: DFMR_OK          Routine successful.
 *          DFMR_NOT_INIT    The DFM is not initialized.
 *          DFMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
extern INT32    c_dfm_free_logo(
                    HANDLE_T            h_dfm,
                    HANDLE_T            h_logo
                    )
{
    INT32 i4_rc;
    i4_rc = x_dfm_free_logo(h_dfm, h_logo);
    if ( i4_rc == DFMR_OK )
    {
        aee_release_handle_resource(1);
    }
    return i4_rc;
}

/*---------------------------------------------------------------------------
 * Name: c_dfm_change_svc
 *
 * Description: This API notify the change service for DFM.
 *
 * Inputs:  h_dfm   Contains the DFM object.
 *          ui2_onid  Contains onid.
 *          ui2_service_ID  Contains service id.
 *
 * Outputs: -
 *
 * Returns: DFMR_OK          Routine successful.
 *          DFMR_NOT_INIT    The DFM is not initialized.
 *          DFMR_INV_HANDLE  Invalid handle.
 --------------------------------------------------------------------------*/
extern INT32    c_dfm_change_svc(
                    HANDLE_T         h_dfm,
                    UINT16           ui2_onid,
                    UINT16           ui2_service_ID,
                    UINT16           ui2_scan_idx
                         )
{
    INT32 i4_rc;
    i4_rc = x_dfm_change_svc(h_dfm, ui2_onid,ui2_service_ID, ui2_scan_idx);
    if ( i4_rc == DFMR_OK )
    {
        aee_release_handle_resource(1);
    }
    return i4_rc;
}


