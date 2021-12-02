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
 * $RCSfile: cust_drv_mw.c,v $
 * $Revision: #2 $
 * $Date: 2012/07/25 $
 * $Author: weibo.li $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#if defined(TARGET_SPEC_DRV_CUST_HDR_FILE) || defined(LINUX_TURNKEY_SOLUTION)
/*-----------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------------*/

#include "u_common.h"
#include "x_rm.h"
#include "u_rm_dev_types.h"
#include "x_os.h"

#ifdef LINUX_TURNKEY_SOLUTION
#define TARGET_SPEC_DRV_CUST_HDR_FILE "x_rm.h"
#endif
#include "x_os.h"
#include "u_drv_cust.h"
#include "cust_drv_mw.h"

#ifdef LINUX_TURNKEY_SOLUTION
extern VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
extern int rm_copy_to_user(VOID *pv_user,VOID* pv_kernel,UINT32 size);
#endif

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _custom_drv_set
 *
 * Description: _custom_drv_set
 *
 * Inputs:  pt_comp_id
 *          e_set_type
 *          pv_set_info
 *          z_set_info_len
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 _custom_drv_set(
    DRV_COMP_ID_T*                pt_comp_id,
    DRV_SET_TYPE_T                e_set_type,
    const VOID*                   pv_set_info,
    SIZE_T                        z_set_info_len
)
{
    DRV_CUST_OPERATION_INFO_T*    pt_op_info         = NULL;
    INT32                         i4_ret             = -1;
#ifdef __KERNEL__
    VOID*                         pv_kernel_set_info = NULL;
#endif

    if ((pv_set_info == NULL)
        || (z_set_info_len != sizeof(DRV_CUST_OPERATION_INFO_T)))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    pt_op_info = (DRV_CUST_OPERATION_INFO_T*)pv_set_info;

    /* call the matching function */
    switch(pt_op_info->e_op_type)
    {
        /* Audio */
        case DRV_CUSTOM_OPERATION_TYPE_AUD_SET:
        {
#ifdef __KERNEL__
            /* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_aud_set_info.e_aud_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_aud_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_aud_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_aud_set_info.pv_set_info,
                                      pt_op_info->u.t_aud_set_info.z_size);

                    i4_ret = d_custom_aud_set(
                                pt_op_info->u.t_aud_set_info.e_aud_type,
                                pv_kernel_set_info,
                                pt_op_info->u.t_aud_set_info.z_size,
                                pt_op_info->u.t_aud_set_info.b_store);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
            else
            {
                i4_ret = d_custom_aud_set(
                            pt_op_info->u.t_aud_set_info.e_aud_type,
                            pt_op_info->u.t_aud_set_info.pv_set_info,
                            pt_op_info->u.t_aud_set_info.z_size,
                            pt_op_info->u.t_aud_set_info.b_store);
            }
#else
            i4_ret = d_custom_aud_set(
                        pt_op_info->u.t_aud_set_info.e_aud_type,
                        pt_op_info->u.t_aud_set_info.pv_set_info,
                        pt_op_info->u.t_aud_set_info.z_size,
                        pt_op_info->u.t_aud_set_info.b_store);
#endif
            break;
        }

        /* Tuner */
        case DRV_CUSTOM_OPERATION_TYPE_TUNER_SET:
        {
#ifdef __KERNEL__
			/* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_tuner_set_info.e_tuner_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_tuner_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_tuner_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_tuner_set_info.pv_set_info,
                                      pt_op_info->u.t_tuner_set_info.z_size);

                    i4_ret = d_custom_tuner_set(
                                pt_op_info->u.t_tuner_set_info.e_tuner_type,
                                pt_op_info->u.t_tuner_set_info.e_tuner_set_type,
                                pv_kernel_set_info,
                                pt_op_info->u.t_tuner_set_info.z_size);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
            else
            {
                i4_ret = d_custom_tuner_set(
                            pt_op_info->u.t_tuner_set_info.e_tuner_type,
                            pt_op_info->u.t_tuner_set_info.e_tuner_set_type,
                            pt_op_info->u.t_tuner_set_info.pv_set_info,
                            pt_op_info->u.t_tuner_set_info.z_size);
            }
#else
            i4_ret = d_custom_tuner_set(
                        pt_op_info->u.t_tuner_set_info.e_tuner_type,
                        pt_op_info->u.t_tuner_set_info.e_tuner_set_type,
                        pt_op_info->u.t_tuner_set_info.pv_set_info,
                        pt_op_info->u.t_tuner_set_info.z_size);
#endif
            break;
        }

        /* Video */
        case DRV_CUSTOM_OPERATION_TYPE_VID_SET:
        {
#ifdef __KERNEL__
            /* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_vid_set_info.e_vid_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_vid_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_vid_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_vid_set_info.pv_set_info,
                                      pt_op_info->u.t_vid_set_info.z_size);

                    i4_ret = d_custom_vid_set(
                            pt_op_info->u.t_vid_set_info.e_vid_type,
                            pv_kernel_set_info,
                            pt_op_info->u.t_vid_set_info.z_size,
                            pt_op_info->u.t_vid_set_info.b_store);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
            else
            {
                i4_ret = d_custom_vid_set(
                            pt_op_info->u.t_vid_set_info.e_vid_type,
                            pt_op_info->u.t_vid_set_info.pv_set_info,
                            pt_op_info->u.t_vid_set_info.z_size,
                            pt_op_info->u.t_vid_set_info.b_store);
            }
#else
            i4_ret = d_custom_vid_set(
                        pt_op_info->u.t_vid_set_info.e_vid_type,
                        pt_op_info->u.t_vid_set_info.pv_set_info,
                        pt_op_info->u.t_vid_set_info.z_size,
                        pt_op_info->u.t_vid_set_info.b_store);
#endif
            break;
        }

        /* Vdec */
        case DRV_CUSTOM_OPERATION_TYPE_VID_DEC_SET:
        {
#ifdef __KERNEL__
            /* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_vid_dec_set_info.e_vid_dec_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_vid_dec_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_vid_dec_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_vid_dec_set_info.pv_set_info,
                                      pt_op_info->u.t_vid_dec_set_info.z_size);

                    i4_ret = d_custom_vid_dec_set(
                            pt_op_info->u.t_vid_dec_set_info.e_vid_dec_type,
                            pv_kernel_set_info,
                            pt_op_info->u.t_vid_dec_set_info.z_size,
                            pt_op_info->u.t_vid_dec_set_info.b_store);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
            else
            {
	            i4_ret = d_custom_vid_dec_set(
	                        pt_op_info->u.t_vid_dec_set_info.e_vid_dec_type,
	                        pt_op_info->u.t_vid_dec_set_info.pv_set_info,
	                        pt_op_info->u.t_vid_dec_set_info.z_size,
	                        pt_op_info->u.t_vid_dec_set_info.b_store);
            }
#else
            i4_ret = d_custom_vid_dec_set(
                        pt_op_info->u.t_vid_dec_set_info.e_vid_dec_type,
                        pt_op_info->u.t_vid_dec_set_info.pv_set_info,
                        pt_op_info->u.t_vid_dec_set_info.z_size,
                        pt_op_info->u.t_vid_dec_set_info.b_store);
#endif
            break;
        }

#ifdef DRV_SUPPORT_EXTMJC
        /* EXTMJC */
        case DRV_CUSTOM_OPERATION_TYPE_EXTMJC_SET:
        {
#ifdef __KERNEL__
            /* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_extmjc_set_info.e_extmjc_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_extmjc_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_extmjc_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_extmjc_set_info.pv_set_info,
                                      pt_op_info->u.t_extmjc_set_info.z_size);

                    i4_ret = d_custom_extmjc_set(
                            pt_op_info->u.t_extmjc_set_info.e_extmjc_type,
                            pv_kernel_set_info,
                            pt_op_info->u.t_extmjc_set_info.z_size,
                            pt_op_info->u.t_extmjc_set_info.b_store);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
            else
            {
	            i4_ret = d_custom_extmjc_set(
	                        pt_op_info->u.t_extmjc_set_info.e_extmjc_type,
	                        pt_op_info->u.t_extmjc_set_info.pv_set_info,
	                        pt_op_info->u.t_extmjc_set_info.z_size,
	                        pt_op_info->u.t_extmjc_set_info.b_store);
            }
#else
            i4_ret = d_custom_extmjc_set(
                        pt_op_info->u.t_extmjc_set_info.e_extmjc_type,
                        pt_op_info->u.t_extmjc_set_info.pv_set_info,
                        pt_op_info->u.t_extmjc_set_info.z_size,
                        pt_op_info->u.t_extmjc_set_info.b_store);
#endif
            break;
        }
#endif

        /* Misc */
        case DRV_CUSTOM_OPERATION_TYPE_MISC_SET:
        {
#ifdef __KERNEL__
			/* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_misc_set_info.e_misc_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_misc_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_misc_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_misc_set_info.pv_set_info,
                                      pt_op_info->u.t_misc_set_info.z_size);

                    i4_ret = d_custom_misc_set(
                                pt_op_info->u.t_misc_set_info.e_misc_type,
                                pv_kernel_set_info,
                                pt_op_info->u.t_misc_set_info.z_size,
                                pt_op_info->u.t_misc_set_info.b_store);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
			else
			{
	            i4_ret = d_custom_misc_set(
	                        pt_op_info->u.t_misc_set_info.e_misc_type,
	                        pt_op_info->u.t_misc_set_info.pv_set_info,
	                        pt_op_info->u.t_misc_set_info.z_size,
	                        pt_op_info->u.t_misc_set_info.b_store);
			}
#else
            i4_ret = d_custom_misc_set(
                        pt_op_info->u.t_misc_set_info.e_misc_type,
                        pt_op_info->u.t_misc_set_info.pv_set_info,
                        pt_op_info->u.t_misc_set_info.z_size,
                        pt_op_info->u.t_misc_set_info.b_store);
#endif
            break;
        }
        /* Customer's specific set API */
        case DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_SET:
        {
#ifdef __KERNEL__
			/* only do rm_copy_from_user() when the argument is a pointer! */
            if (DRV_CUSTOM_ARG_USE_REF_OFFSET <= pt_op_info->u.t_cust_spec_set_info.e_cust_spec_type)
            {
                /* allocate kernel memory as a temporary space */
                pv_kernel_set_info = (VOID*)x_mem_alloc(pt_op_info->u.t_cust_spec_set_info.z_size);

                if ((NULL != pv_kernel_set_info)
                    && (NULL != pt_op_info->u.t_cust_spec_set_info.pv_set_info))
                {
                    /* get the set info from user space */
                    rm_copy_from_user(pv_kernel_set_info,
                                      pt_op_info->u.t_cust_spec_set_info.pv_set_info,
                                      pt_op_info->u.t_cust_spec_set_info.z_size);

                    i4_ret = d_custom_cust_spec_set(
                            pt_op_info->u.t_cust_spec_set_info.e_cust_spec_type,
                            pv_kernel_set_info,
                            pt_op_info->u.t_cust_spec_set_info.z_size,
                            pt_op_info->u.t_cust_spec_set_info.b_store);

                    /* free the kernel memory */
                    x_mem_free(pv_kernel_set_info);
                }
            }
            else
            {
                i4_ret = d_custom_cust_spec_set(
                            pt_op_info->u.t_cust_spec_set_info.e_cust_spec_type,
                            pt_op_info->u.t_cust_spec_set_info.pv_set_info,
                            pt_op_info->u.t_cust_spec_set_info.z_size,
                            pt_op_info->u.t_cust_spec_set_info.b_store);
            }
#else
            i4_ret = d_custom_cust_spec_set(
                        pt_op_info->u.t_cust_spec_set_info.e_cust_spec_type,
                        pt_op_info->u.t_cust_spec_set_info.pv_set_info,
                        pt_op_info->u.t_cust_spec_set_info.z_size,
                        pt_op_info->u.t_cust_spec_set_info.b_store);
#endif
            break;
        }
        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
    }

    if (i4_ret < RMR_OK)
    {
        i4_ret = RMR_DRV_SET_FAILED;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: _custom_drv_get
 *
 * Description: _custom_drv_get
 *
 * Inputs:  pt_comp_id
 *          e_get_type
 *
 * Outputs: pv_get_info
 *          pz_get_info_len
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 _custom_drv_get(
    DRV_COMP_ID_T*                pt_comp_id,
    DRV_GET_TYPE_T                e_get_type,
    VOID*                         pv_get_info,
    SIZE_T*                       pz_get_info_len
)
{
    DRV_CUST_OPERATION_INFO_T*    pt_op_info         = NULL;
    INT32                         i4_ret             = -1;
#ifdef __KERNEL__
    VOID*                         pv_kernel_get_info = NULL;
    SIZE_T                        z_size             = 0;
#endif

    if ((pv_get_info == NULL)
        || (*pz_get_info_len != sizeof(DRV_CUST_OPERATION_INFO_T)))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    pt_op_info = (DRV_CUST_OPERATION_INFO_T*)pv_get_info;

    /* call the matching function */
    switch(pt_op_info->e_op_type)
    {
        /* Audio */
        case DRV_CUSTOM_OPERATION_TYPE_AUD_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_aud_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_aud_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_aud_get(
                            pt_op_info->u.t_aud_get_info.e_aud_type,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_aud_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_aud_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_aud_get(
                        pt_op_info->u.t_aud_get_info.e_aud_type,
                        pt_op_info->u.t_aud_get_info.pv_get_info,
                        pt_op_info->u.t_aud_get_info.pz_size);
#endif
            break;
        }

        case DRV_CUSTOM_OPERATION_TYPE_AUD_GET_MIN_MAX:
        {
#ifdef __KERNEL__
            INT16    i2_min = 0;
            INT16    i2_max = 0;

            i4_ret = d_custom_aud_get_min_max(
                        pt_op_info->u.t_aud_get_min_max_info.e_aud_type,
                        &i2_min,
                        &i2_max);

            /* copy back to user space */
            if (0 == i4_ret)
            {
                rm_copy_to_user((VOID*)(pt_op_info->u.t_aud_get_min_max_info.pi2_min),
                                (VOID*)(&i2_min),
                                sizeof(INT16));

                rm_copy_to_user((VOID*)(pt_op_info->u.t_aud_get_min_max_info.pi2_max),
                                (VOID*)(&i2_max),
                                sizeof(INT16));
            }
#else
            i4_ret = d_custom_aud_get_min_max(
                        pt_op_info->u.t_aud_get_min_max_info.e_aud_type,
                        pt_op_info->u.t_aud_get_min_max_info.pi2_min,
                        pt_op_info->u.t_aud_get_min_max_info.pi2_max);
#endif
            break;
        }

        /* Tuner */
        case DRV_CUSTOM_OPERATION_TYPE_TUNER_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_tuner_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_tuner_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_tuner_get(
                            pt_op_info->u.t_tuner_get_info.e_tuner_type,
                            pt_op_info->u.t_tuner_get_info.e_tuner_get_type,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_tuner_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_tuner_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_tuner_get(
                        pt_op_info->u.t_tuner_get_info.e_tuner_type,
                        pt_op_info->u.t_tuner_get_info.e_tuner_get_type,
                        pt_op_info->u.t_tuner_get_info.pv_get_info,
                        pt_op_info->u.t_tuner_get_info.pz_size);
#endif
            break;
        }
        /* Video */
        case DRV_CUSTOM_OPERATION_TYPE_VID_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_vid_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_vid_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_vid_get(
                            pt_op_info->u.t_vid_get_info.e_vid_type,
                            pt_op_info->u.t_vid_get_info.e_vid_inp,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_vid_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_vid_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_vid_get(
                        pt_op_info->u.t_vid_get_info.e_vid_type,
                        pt_op_info->u.t_vid_get_info.e_vid_inp,
                        pt_op_info->u.t_vid_get_info.pv_get_info,
                        pt_op_info->u.t_vid_get_info.pz_size);
#endif
            break;
        }

        case DRV_CUSTOM_OPERATION_TYPE_VID_GET_GAMMA:
        {
#ifdef __KERNEL__
            z_size = sizeof(GAMMA_INFO_T);

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_vid_get_gamma_info.pu1GammaTbl,
                                  z_size);

                i4_ret = d_custom_vid_get_gamma(
                            (GAMMA_INFO_T*)pv_kernel_get_info);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_vid_get_gamma_info.pu1GammaTbl,
                                    pv_kernel_get_info,
                                    z_size);
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_vid_get_gamma(
                        pt_op_info->u.t_vid_get_gamma_info.pu1GammaTbl);
#endif
            break;
        }
        case DRV_CUSTOM_OPERATION_TYPE_VID_GET_MIN_MAX:
        {
#ifdef __KERNEL__
            UINT16    ui2_min = 0;
            UINT16    ui2_max = 0;

            i4_ret = d_custom_vid_get_min_max(
                        pt_op_info->u.t_vid_get_min_max_info.e_vid_type,
                        pt_op_info->u.t_vid_get_min_max_info.e_vid_inp,
                        &ui2_min,
                        &ui2_max);

            /* copy back to user space */
            if (0 == i4_ret)
            {
                rm_copy_to_user((VOID*)(pt_op_info->u.t_vid_get_min_max_info.pui2_min),
                                (VOID*)(&ui2_min),
                                sizeof(UINT16));

                rm_copy_to_user((VOID*)(pt_op_info->u.t_vid_get_min_max_info.pui2_max),
                                (VOID*)(&ui2_max),
                                sizeof(UINT16));
            }
#else
            i4_ret = d_custom_vid_get_min_max(
                        pt_op_info->u.t_vid_get_min_max_info.e_vid_type,
                        pt_op_info->u.t_vid_get_min_max_info.e_vid_inp,
                        pt_op_info->u.t_vid_get_min_max_info.pui2_min,
                        pt_op_info->u.t_vid_get_min_max_info.pui2_max);
#endif
            break;
        }

        /* Vid dec */
        case DRV_CUSTOM_OPERATION_TYPE_VID_DEC_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_vid_dec_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_vid_dec_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_vid_dec_get(
                            pt_op_info->u.t_vid_dec_get_info.e_vid_dec_type,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_vid_dec_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_vid_dec_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_vid_dec_get(
                        pt_op_info->u.t_vid_dec_get_info.e_vid_dec_type,
                        pt_op_info->u.t_vid_dec_get_info.pv_get_info,
                        pt_op_info->u.t_vid_dec_get_info.pz_size);
#endif
            break;
        }

#ifdef DRV_SUPPORT_EXTMJC
        /* EXTMJC */
        case DRV_CUSTOM_OPERATION_TYPE_EXTMJC_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_extmjc_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_extmjc_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_extmjc_get(
                            pt_op_info->u.t_extmjc_get_info.e_extmjc_type,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_extmjc_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_extmjc_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_extmjc_get(
                        pt_op_info->u.t_extmjc_get_info.e_extmjc_type,
                        pt_op_info->u.t_extmjc_get_info.pv_get_info,
                        pt_op_info->u.t_extmjc_get_info.pz_size);
#endif
            break;
        }
#endif

        /* Misc */
        case DRV_CUSTOM_OPERATION_TYPE_MISC_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_misc_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_misc_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_misc_get(
                            pt_op_info->u.t_misc_get_info.e_misc_type,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_misc_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_misc_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_misc_get(
                        pt_op_info->u.t_misc_get_info.e_misc_type,
                        pt_op_info->u.t_misc_get_info.pv_get_info,
                        pt_op_info->u.t_misc_get_info.pz_size);
#endif
            break;
        }

        case DRV_CUSTOM_OPERATION_TYPE_MISC_GET_MIN_MAX:
        {
#ifdef __KERNEL__
            UINT16    ui2_min = 0;
            UINT16    ui2_max = 0;

            i4_ret = d_custom_misc_get_min_max(
                        pt_op_info->u.t_misc_get_min_max_info.e_misc_type,
                        &ui2_min,
                        &ui2_max);

            /* copy back to user space */
            if (0 == i4_ret)
            {
                rm_copy_to_user((VOID*)(pt_op_info->u.t_misc_get_min_max_info.pui2_min),
                                (VOID*)(&ui2_min),
                                sizeof(UINT16));

                rm_copy_to_user((VOID*)(pt_op_info->u.t_misc_get_min_max_info.pui2_max),
                                (VOID*)(&ui2_max),
                                sizeof(UINT16));
            }
#else
            i4_ret = d_custom_misc_get_min_max(
                        pt_op_info->u.t_misc_get_min_max_info.e_misc_type,
                        pt_op_info->u.t_misc_get_min_max_info.pui2_min,
                        pt_op_info->u.t_misc_get_min_max_info.pui2_max);
#endif
            break;
        }
        /* Customer's specific get API */
        case DRV_CUSTOM_OPERATION_TYPE_CUST_SPEC_GET:
        {
#ifdef __KERNEL__
            /* get the size from user space that we need to allocate */
            rm_copy_from_user((VOID*)(&z_size),
                              pt_op_info->u.t_cust_spec_get_info.pz_size,
                              sizeof(SIZE_T));

            /* allocate kernel memory as a temporary space */
            pv_kernel_get_info = (VOID*)x_mem_alloc(z_size);

            if (NULL != pv_kernel_get_info)
            {
                /* also copy get info from user space */
                rm_copy_from_user(pv_kernel_get_info,
                                  pt_op_info->u.t_cust_spec_get_info.pv_get_info,
                                  z_size);

                i4_ret = d_custom_cust_spec_get(
                            pt_op_info->u.t_cust_spec_get_info.e_cust_spec_type,
                            pv_kernel_get_info,
                            &z_size);

                /* copy back to user space */
                if (0 == i4_ret)
                {
                    rm_copy_to_user(pt_op_info->u.t_cust_spec_get_info.pv_get_info,
                                    pv_kernel_get_info,
                                    z_size);

                    rm_copy_to_user(pt_op_info->u.t_cust_spec_get_info.pz_size,
                                    (VOID*)(&z_size),
                                    sizeof(SIZE_T));
                }

                /* free the kernel memory */
                x_mem_free(pv_kernel_get_info);
            }
#else
            i4_ret = d_custom_cust_spec_get(
                        pt_op_info->u.t_cust_spec_get_info.e_cust_spec_type,
                        pt_op_info->u.t_cust_spec_get_info.pv_get_info,
                        pt_op_info->u.t_cust_spec_get_info.pz_size);
#endif
            break;
        }
        default:
            return RMR_DRV_INV_GET_INFO;

    }

    if (i4_ret < RMR_OK)
    {
        i4_ret = RMR_DRV_GET_FAILED;
    }

    return i4_ret;
}
/*-----------------------------------------------------------------------------
 * Name: custom_drv_init
 *
 * Description: custom_drv_init
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 custom_drv_init(
    VOID
)
{
    DRV_COMP_REG_T        t_cust_drv_reg     = {0};
    DRV_COMP_FCT_TBL_T    t_cust_drv_fct_tbl = {0};
    INT32                 i4_ret;

    /* Setup component function table */
    t_cust_drv_fct_tbl.pf_rm_connect    = NULL;
    t_cust_drv_fct_tbl.pf_rm_disconnect = NULL;
    t_cust_drv_fct_tbl.pf_rm_get        = _custom_drv_get;
    t_cust_drv_fct_tbl.pf_rm_set        = _custom_drv_set;

    /* Setup component ID */
    t_cust_drv_reg.e_type           = DRVT_CUST_DRV;
    t_cust_drv_reg.e_id_type        = ID_TYPE_IND;
    t_cust_drv_reg.u.t_ind.ui2_id   = DRV_CUST_COMP_ID;
    t_cust_drv_reg.u.t_ind.ui1_port = 1;
    t_cust_drv_reg.u.t_ind.pv_tag   = NULL;

    /* Register component with the Resource Manager database */
    i4_ret = x_rm_reg_comp(&t_cust_drv_reg,
                           1,
                           1,
                           NULL,
                           0,
                           &t_cust_drv_fct_tbl,
                           NULL,
                           0);

    if (i4_ret != RMR_OK)
    {
        return i4_ret;
    }

    return RMR_OK;
}

#endif /* TARGET_SPEC_DRV_CUST_HDR_FILE */

