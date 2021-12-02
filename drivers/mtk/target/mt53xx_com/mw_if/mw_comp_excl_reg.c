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
 * $RCSfile: mw_comp_excl_reg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: $
 *
 * Description: 
 *         This header file contains specific implementation.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_rm.h"
#include "mw_comp_excl_reg.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MW_COMP_EXCL_MAX_SIZE      32

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static const DRV_COMP_ID_T   t_mw_comp_excl_lst_end = MW_COMP_EXCL_LST_END;

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
VOID  _mw_drv_comp_id_2_drv_comp_reg( 
    const DRV_COMP_ID_T*      pt_drv_comp,
    DRV_COMP_REG_T*           pt_drv_reg )
{
    if ( !pt_drv_comp || !pt_drv_reg )
    {
        return;
    }
    
    pt_drv_reg->e_id_type = ID_TYPE_IND;
    pt_drv_reg->e_type = pt_drv_comp->e_type;
    pt_drv_reg->u.t_ind.ui2_id = pt_drv_comp->ui2_id;
    pt_drv_reg->u.t_ind.pv_tag = pt_drv_comp->pv_tag;
    pt_drv_reg->u.t_ind.ui1_port = ( (pt_drv_comp->b_sel_out_port)?
                                     pt_drv_comp->u.ui1_out_port:
                                     pt_drv_comp->u.ui1_inp_port );
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 mw_comp_excl_reg(
    const DRV_COMP_ID_T*      pt_drv_comp_lst )
{
    INT32                i4_ret, i4_i = 1;
    DRV_COMP_REG_T       t_src;
    DRV_COMP_REG_T       at_dest[MW_COMP_EXCL_MAX_SIZE];
    
    if ( !pt_drv_comp_lst ||
         x_memcmp( &pt_drv_comp_lst[0], 
                   &t_mw_comp_excl_lst_end, 
                   sizeof(DRV_COMP_ID_T) ) == 0 )
    {
        return -1;
    }
    
    _mw_drv_comp_id_2_drv_comp_reg( &pt_drv_comp_lst[0],
                                    &t_src );
    
    while( x_memcmp( &pt_drv_comp_lst[i4_i], 
                     &t_mw_comp_excl_lst_end, 
                     sizeof(DRV_COMP_ID_T) ) != 0 )
    {
        if ( i4_i >= MW_COMP_EXCL_MAX_SIZE )
        {
            return -1;
        }
        
        _mw_drv_comp_id_2_drv_comp_reg( &pt_drv_comp_lst[i4_i],
                                        &at_dest[i4_i-1] );
        ++i4_i;
    }
    if ( i4_i <= 2 )
    {
        return -1;
    }
    
    i4_ret = x_rm_reg_comp_excl_list( &t_src,
                                      at_dest,
                                      (i4_i-1) );
    if ( i4_ret != RMR_OK )
    {
        return -1;
    }
    
    return RMR_OK;
}

