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
 * $RCSfile: x_mm_hdlr_uop_ts.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the mp3 media info function
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "svctx/mm_hdlr/mm_hdlr.h"
#include "svctx/u_svctx.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"
#include "mm_hdlr_uop_ts.h"

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
/*-----------------------------------------------------------------------------
 * Name:  x_mm_hdlr_uop_ts_init
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/                            
INT32 x_mm_hdlr_uop_ts_init (void)
{
    INT32                           i4_ret;
    MEDIA_FORMAT_T                  t_format;
    MM_SVC_UOP_HANDLER_FCT_TBL_T    t_fct_tbl;

    t_fct_tbl.pf_is_hdlr =          ts_uop_hdlr_is_hdlr;
    t_fct_tbl.pf_close =            ts_uop_hdlr_close;
    t_fct_tbl.pf_open =             ts_uop_hdlr_open;
    t_fct_tbl.pf_get_dur =          ts_uop_hdlr_get_dur;
    t_fct_tbl.pf_get_tm =           ts_uop_hdlr_get_tm_code;
    t_fct_tbl.pf_btn =              ts_uop_handler_btn_fct;
    t_fct_tbl.pf_trick =            ts_uop_handler_trick_fct;
    t_fct_tbl.pf_srh   =            ts_uop_handler_search_fct;
    t_fct_tbl.pf_get_range =        ts_uop_handler_get_range_fct;

    t_format.e_media_type     = MEDIA_TYPE_TS;
    
    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_188;
    i4_ret = mm_hdlr_reg_uop_handler("ts_188_uop_hdlr",&t_format,&t_fct_tbl);

    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192;
    i4_ret = mm_hdlr_reg_uop_handler("ts_192_uop_hdlr",&t_format,&t_fct_tbl);

    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_SYSTEM_B;
    i4_ret = mm_hdlr_reg_uop_handler("ts_system_b_uop_hdlr",&t_format,&t_fct_tbl);

    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ZERO;
    i4_ret = mm_hdlr_reg_uop_handler("ts_192_zero_uop_hdlr",&t_format,&t_fct_tbl);

    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION;
    i4_ret = mm_hdlr_reg_uop_handler("ts_192_encryption_uop_hdlr",&t_format,&t_fct_tbl);

    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_TIME_SHIFT;
    i4_ret = mm_hdlr_reg_uop_handler("ts_time_shift_uop_hdlr",&t_format,&t_fct_tbl);

    t_format.t_media_subtype.u.e_ts_subtype = MEDIA_TS_SUBTYPE_TS_TIME_SHIFT_CMPB;
    i4_ret = mm_hdlr_reg_uop_handler("ts_time_shift_cmpb_uop_hdlr",&t_format,&t_fct_tbl);

    return i4_ret;
}

