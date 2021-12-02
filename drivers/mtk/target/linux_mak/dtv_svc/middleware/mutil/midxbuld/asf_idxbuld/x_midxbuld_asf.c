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
 * $RCSfile: x_midxbuld_asf.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Ivan Wei $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the asf media index builder 
 *         function
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/midxbuld/asf_idxbuld/asf_midxbuld.h"
#include "mutil/midxbuld/asf_idxbuld/x_midxbuld_asf.h"
#include "mutil/midxbuld/x_midxbuld.h"
#include "mutil/midxbuld/asf_idxbuld/asf_midxbuld_cli.h"
#include "mutil/midxbuld/midxbuld_cli.h"

#else

#include "x_common.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "asf_midxbuld.h"
#include "x_midxbuld_asf.h"
#include "x_midxbuld.h"
#include "asf_midxbuld_cli.h"
#include "../midxbuld_cli.h"

#endif
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
 * Name:  x_midxbuld_asf_init
 *
 * Description: asf  media index builder initial function
 *
 * Inputs:  NULL 
 *          
 * Outputs: NULL 
 * 
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INITIAL_ERR        The library initialize failed.
 *
 * Note:  
 *
 * Author : lianming lin(mcn07123)
 * 
 * History:
 *   (1)2008-4-01 : initial
 ----------------------------------------------------------------------------*/


INT32 x_midxbuld_asf_init (VOID)
{
    INT32                       i4_ret;
    MIDXBULD_HANDLER_FCT_TBL_T  t_fct_tab = {0};
    MEDIA_FORMAT_T              t_format;

    i4_ret = midxbuld_init();
    if (i4_ret != MIDXBULDR_OK)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    
    t_fct_tab.pf_fopen = asf_midxbuld_fopen;
    t_fct_tab.pf_mopen = asf_midxbuld_mopen;
    t_fct_tab.pf_close = asf_midxbuld_close;
    t_fct_tab.pf_start_indexing = asf_midxbuld_start_indexing;
    t_fct_tab.pf_get_info= asf_midxbuld_get_info;
    t_fct_tab.pf_set_info= asf_midxbuld_set_info;
    t_fct_tab.pf_midxbuld_cli_init = asf_midxbuld_cli_init;


    t_format.e_media_type = MEDIA_TYPE_CONTAINER;
    t_format.t_media_subtype.u.e_contnr_subtype = MEDIA_CONTNR_SUBTYPE_ASF;

    /* register asf media index builder */
    i4_ret = midxbuld_register_handler(ASF_MEDIA_IDX_BUILDER_NAME, 
                                       &t_format, 
                                       &t_fct_tab);
    if (i4_ret != MIDXBULDR_OK)
    {
        return MIDXBULDR_INITIAL_ERR;
    }  

    /* register wma media index builder */
    t_format.e_media_type = MEDIA_TYPE_AUDIO_ES;
    t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_WMA;
    i4_ret = midxbuld_register_handler(WMA_MEDIA_IDX_BUILDER_NAME,
                                       &t_format,
                                       &t_fct_tab);
    if (i4_ret != MIDXBULDR_OK)
    {
        return MIDXBULDR_INITIAL_ERR;
    }

#ifdef CLI_LVL_ALL
    /* Initialize mfmtrecg CLI component */

    if (midxbuld_cli_init() != MIDXBULDR_OK)
    {
        return MIDXBULDR_CLI_ERROR;
    } 
#endif
    return MIDXBULDR_OK;
}


