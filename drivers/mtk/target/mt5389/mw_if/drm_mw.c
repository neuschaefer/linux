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
 * $RCSfile: drm_mw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_debug.h"
#include "x_bim.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_drm_drv.h"
#include "aud_if.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 drm_mw_get(DRV_COMP_ID_T* pt_comp_id, DRV_GET_TYPE_T e_get_type, const VOID* pv_data, SIZE_T z_size)
{
    INT32 i4_ret = RMR_OK;

    /* Check component ID */
    if ((pt_comp_id == NULL) || (pt_comp_id->e_type != DRVT_DRM))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_get_type)
    {
        case DIVX_DRM_GET_TYPE_HW_SECRET_KEY:
            if ((pv_data == NULL) || (z_size < sizeof(DIVX_DRM_HW_SECRET_KEY_T)))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            
            /* Call driver API to get the 32-bytes hardware secret key.... */
#ifdef CC_MT5363 
            AUD_Read32ByteID(1, (UINT8*)pv_data, 32);
#else
            AUD_Read32ByteID((UINT8*)pv_data, 32);
#endif            
            break;
        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
            break;
    }

    return i4_ret;
}

INT32 MW_DRM_Init(void)
{
   INT32 i4_ret;
   DRV_COMP_REG_T t_drm_comp_reg;
   DRV_COMP_FCT_TBL_T t_drm_comp_fct_tbl;

   /* Setup component ID */
   t_drm_comp_reg.e_type = DRVT_DRM;
   t_drm_comp_reg.e_id_type = ID_TYPE_IND;
   t_drm_comp_reg.u.t_ind.ui2_id = 0;
   t_drm_comp_reg.u.t_ind.pv_tag = NULL;
   t_drm_comp_reg.u.t_ind.ui1_port = 0;

   /* Setup component function table */
   t_drm_comp_fct_tbl.pf_rm_connect = NULL;
   t_drm_comp_fct_tbl.pf_rm_disconnect = NULL;
   t_drm_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)drm_mw_get;
   t_drm_comp_fct_tbl.pf_rm_set = NULL;

   /* Register component with the Resource Manager database */
   i4_ret = x_rm_reg_comp(&t_drm_comp_reg, 1, 1, NULL, 0, &t_drm_comp_fct_tbl, NULL, 0);
   if (i4_ret != RMR_OK)
   {
       LOG(1, "mw_drm_init(): x_rm_reg_comp failed [%d]\n", i4_ret);
   }
      
   return i4_ret;
}
