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
 * $IRRCSfile$ chip_spec_mw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *         This header file contains implementation Middleware interface for
 *         Chip Specification driver.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_typedef.h"
#include "x_rm_dev_types.h"
#include "x_rm.h"
#include "x_chip_spec_dev.h"
#include "x_chip_id.h"

/*-----------------------------------------------------------------------------
                    configurations
 ----------------------------------------------------------------------------*/
#define CHIP_SPEC_DRV_ID          0

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
        External function.
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL     b_chip_spec_init = FALSE;

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: CHIP_SPEC_Get
 *
 * Description: This API implements Middleware get functionality for Chip
 *              Specificaiton driver.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 CHIP_SPEC_Get(DRV_COMP_ID_T* pt_comp_id,
             DRV_GET_TYPE_T e_get_type,
             VOID*          pv_get_info,
             SIZE_T*        pz_get_info_len)
{
    SIZE_T          t_get_info_len;

    /* Check aguments */
    if (!b_chip_spec_init)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    if ((pt_comp_id == NULL) ||
        (pt_comp_id->e_type != DRVT_CHIP_SPEC) ||
        (pt_comp_id->ui2_id != CHIP_SPEC_DRV_ID))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    /* Perform get opertion */
    switch (e_get_type)
    {
        case  CHIPSPEC_GET_TYPE_CHIP_BOND:
            {
                t_get_info_len = sizeof(CHIPSPEC_CHIP_BOND_INFO_T);
                if (*pz_get_info_len < t_get_info_len)
                {
                    return RMR_DRV_NOT_ENOUGH_SPACE;
                }
                else
                {
                    *pz_get_info_len = t_get_info_len;
                }

                switch (((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->e_chip_bond)
                {
                    case CHIPSPEC_IS_CHIP_BOND_MHEG5:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_MHEG5Support();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_CIPLUS:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportCIPlus();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_TCON:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportTCON();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_CHINATV:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportCHINATV();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_MHEGHD:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportMHEGHD();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_OPERA:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportOPERA();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_JVM:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportJVM();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_ARIB:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportARIB();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_GINGA:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportGINGA();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_MHP:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportMHP();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_DMBT:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportDMBT();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_ATSC:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportATSC();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_BRAZIL:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportBRAZIL();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_DVB:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportDVB();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_HBBTV:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportHBBTV();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_ANDROID:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportAndroid();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_CINEMA:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportCINEMA();
                        break;
                    case CHIPSPEC_IS_CHIP_BOND_QJY:
                        ((CHIPSPEC_CHIP_BOND_INFO_T *)pv_get_info)->b_is_chip_bond = IS_SupportQJY();
                        break;
                    default:
                        return RMR_DRV_INV_GET_INFO;
                }
            }
            break;

        default:
            return RMR_DRV_INV_GET_INFO;
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: MW_CHIP_SPEC_Init
 *
 * Description: This API initializes Chip Specification driver Middleware
 *              adaptation layer.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 ----------------------------------------------------------------------------*/
INT32 MW_CHIP_SPEC_Init(void)
{
    INT32               i4_return;
    DRV_COMP_REG_T      t_chip_spec_comp_reg;
    DRV_COMP_FCT_TBL_T  t_chip_spec_comp_fct_tbl;

    if (b_chip_spec_init)
    {
        return RMR_OK;
    }

    /* Setup Chip Spec component ID */
    t_chip_spec_comp_reg.e_type = DRVT_CHIP_SPEC;
    t_chip_spec_comp_reg.e_id_type = ID_TYPE_IND;
    t_chip_spec_comp_reg.u.t_ind.ui2_id = CHIP_SPEC_DRV_ID;
    t_chip_spec_comp_reg.u.t_ind.pv_tag = NULL;
    t_chip_spec_comp_reg.u.t_ind.ui1_port = 0;

    /* Setup Chip Spec component function table */
    t_chip_spec_comp_fct_tbl.pf_rm_connect = NULL;
    t_chip_spec_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_chip_spec_comp_fct_tbl.pf_rm_get = CHIP_SPEC_Get;
    t_chip_spec_comp_fct_tbl.pf_rm_set = NULL;

    /* Register Chip Spec component with Resource Manager database */
    i4_return = x_rm_reg_comp(&t_chip_spec_comp_reg,
                              1,
                              1,
                              NULL,
                              0,
                              &t_chip_spec_comp_fct_tbl,
                              NULL,
                              0);

    b_chip_spec_init = TRUE;

    return i4_return;
}
