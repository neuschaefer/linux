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
 * $RCSfile: sbtl_register.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description:
 *
 * History:
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include "os/inc/x_os.h"
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_register.h"


#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"

#include "sbtl_dbg.h"
#include "sbtl_struct.h"
#include "sbtl_register.h"

#endif /*__KERNEL__*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
typedef struct _SBTL_MNGR_REG_T
{
    MM_SBTL_TYPE_T                      e_sbtl_type;
    SBTL_MNGR_FCT_TBL_T*                pt_fct_tbl;

    SLIST_ENTRY_T(_SBTL_MNGR_REG_T)     t_link;
} SBTL_MNGR_REG_T;

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
static SLIST_T(_SBTL_MNGR_REG_T)  t_mngr_reg_list = {NULL};

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID sbtl_register_init(VOID)
{
    SLIST_INIT(&t_mngr_reg_list);
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_register(MM_SBTL_TYPE_T          e_sbtl_type,
                    SBTL_MNGR_FCT_TBL_T*    pt_fct_tbl)
{
    SBTL_MNGR_REG_T* pt_reg;

    ASSERT(pt_fct_tbl, ("sbtl_register pt_fct_tbl=NULL!!\n"));
    if (NULL==pt_fct_tbl)
    {
        return MMSBTLR_INV_ARG;
    }
    
    SLIST_FOR_EACH(pt_reg, &t_mngr_reg_list, t_link)
    {
        if (pt_reg->e_sbtl_type == e_sbtl_type)
        {
            return MMSBTLR_EXIST;
        }
    }

    pt_reg = x_mem_alloc(sizeof(SBTL_MNGR_REG_T));

    ASSERT(pt_reg, ("sbtl_register pt_reg=NULL!!\n"));
    if (NULL==pt_reg)
    {
        return MMSBTLR_CORE;
    }
    
    pt_reg->e_sbtl_type = e_sbtl_type;
    pt_reg->pt_fct_tbl = pt_fct_tbl;

    SLIST_INSERT_HEAD(pt_reg, &t_mngr_reg_list, t_link);

    return MMSBTLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_register_match_mngr(SBTL_ENGINE_T*           pt_engine,
                               MM_SBTL_TYPE_T           e_sbtl_type,
                               SBTL_MNGR_FCT_TBL_T**    ppt_fct_tbl,
                               UINT8*                   pt_final_type)
{
    SBTL_MNGR_REG_T* pt_reg;

    ASSERT(pt_engine, ("sbtl_register_match_mngr pt_engine=NULL!!\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(ppt_fct_tbl, ("sbtl_register_match_mngr ppt_fct_tbl=NULL!!\n"));
    if (NULL==ppt_fct_tbl)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_final_type, ("sbtl_register_match_mngr pt_final_type=NULL!!\n"));
    if (NULL==pt_final_type)
    {
        return MMSBTLR_INV_ARG;
    }

    *pt_final_type = e_sbtl_type;
    
    SLIST_FOR_EACH(pt_reg, &t_mngr_reg_list, t_link)
    {
        if (pt_reg->e_sbtl_type == e_sbtl_type)
        {
            ASSERT(pt_reg->pt_fct_tbl, ("sbtl_register_match_mngr pt_reg->pt_fct_tbl=NULL!!\n"));
            if (NULL==pt_reg->pt_fct_tbl)
            {
                return MMSBTLR_INV_ARG;
            }
            if (!pt_reg->pt_fct_tbl->b_need_verify)
            {
                *ppt_fct_tbl = pt_reg->pt_fct_tbl;
                *pt_final_type = e_sbtl_type;
                return MMSBTLR_OK;
            }
            else
            {
                ASSERT(pt_reg->pt_fct_tbl->pf_verify, ("sbtl_register_match_mngr pt_reg->pt_fct_tbl->pf_verify=NULL!!\n"));
                if (NULL==pt_reg->pt_fct_tbl->pf_verify)
                {
                    return MMSBTLR_INV_ARG;
                }
                
                if (pt_reg->pt_fct_tbl->pf_verify(pt_engine, pt_final_type))
                {
                    *ppt_fct_tbl = pt_reg->pt_fct_tbl;
                    return MMSBTLR_OK;
                }
            }
        }
    }

    return MMSBTLR_NOT_FOUND;
}




