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
 * x_minfo_aud_es.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the audio es only media info function
           Author: lianming mcn07123 
           History:
           (1)2008-3-10  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
 
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "inc/u_mm_common.h"

#include "mutil/minfo/minfo_cli.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/minfo_dbg.h"
#include "mutil/minfo/aud_es_info/aud_es_minfo.h"
#include "mutil/minfo/aud_es_info/aud_es_minfo_cli.h"

#else
#include "x_os.h"

#include "x_common.h"
#include "u_mm_common.h"

#include "../u_minfo.h"
#include "../minfo.h"
#include "../minfo_dbg.h"
#include "../minfo_cli.h"
#include "aud_es_minfo.h"
#include "aud_es_minfo_cli.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define AUD_ES_AAC_MEDIA_INFO_NAME "aud_es_aac_minfo"
#define AUD_ES_AC3_MEDIA_INFO_NAME "aud_es_ac3_minfo"
#define AUD_ES_DTS_MEDIA_INFO_NAME "aud_es_dts_minfo"

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
 * Name:  x_minfo_aud_es_init
 *
 * Description: audio es  media info initial function
 *
 * Inputs:  NULL
 *          
 * Outputs: NULL
 *
 * Returns: MINFOR_OK                 The API is successful. 
 *          MINFOR_INITIAL_ERR        The library initialize failed.
 *
 * Author : lianming lin(mcn07123)
 *
 * History:
 *   (1)2009-07-29 : initial
 ----------------------------------------------------------------------------*/                            
INT32 x_minfo_aud_es_init (VOID)
{
    INT32                       i4_ret     = MINFOR_OK; 
    MINFO_HANDLER_FCT_TBL_T     t_fct_tab  = {0};  
    MEDIA_FORMAT_T              t_format;

    /*init variables*/
    x_memset(&t_fct_tab, 0 ,sizeof(MINFO_HANDLER_FCT_TBL_T));
    x_memset(&t_format, 0, sizeof(MEDIA_FORMAT_T));

    /*init minfo module, this is a reentry function*/
    i4_ret = minfo_init();
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("x_minfo_aud_es_init:minfo_init failed(%d)\r\n",
                   i4_ret));
        return MINFOR_INITIAL_ERR;
    }
    
    t_fct_tab.pf_is_hdlr        = aud_es_minfo_is_handler;
    t_fct_tab.pf_fopen          = aud_es_minfo_fopen;
    t_fct_tab.pf_mopen          = aud_es_minfo_mopen;
    t_fct_tab.pf_close          = aud_es_minfo_close;
    t_fct_tab.pf_get_info       = aud_es_minfo_get_info;
    t_fct_tab.pf_parse          = aud_es_minfo_parse;
    t_fct_tab.pf_get_rec_nums   = aud_es_minfo_get_num_recs;
    t_fct_tab.pf_minfo_cli_init = aud_es_minfo_cli_init;

    /* register audio es aac media info */
    t_format.e_media_type                       = MEDIA_TYPE_AUDIO_ES;
    t_format.t_media_subtype.u.e_aud_subtype    = MEDIA_AUD_SUBTYPE_AAC;
    i4_ret = minfo_register_handler(AUD_ES_AAC_MEDIA_INFO_NAME, 
                                    &t_format, 
                                    &t_fct_tab);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("x_minfo_aud_es_init: aac minfo_register_handler failed(%d)\r\n", 
                   i4_ret));
        return MINFOR_INITIAL_ERR;
    }  
      
    /* register audio es ac3 media info */
    t_format.e_media_type                    = MEDIA_TYPE_AUDIO_ES;
    t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_AC3;
    i4_ret = minfo_register_handler(AUD_ES_AC3_MEDIA_INFO_NAME,
                                    &t_format,
                                    &t_fct_tab);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("x_minfo_aud_es_init: ac3 minfo_register_handler failed(%d)\r\n",
                   i4_ret));
        return MINFOR_INITIAL_ERR;
    }
    
    /* register audio es dts media info */
    t_format.e_media_type                    = MEDIA_TYPE_AUDIO_ES;
    t_format.t_media_subtype.u.e_aud_subtype = MEDIA_AUD_SUBTYPE_DTS;
    i4_ret = minfo_register_handler(AUD_ES_DTS_MEDIA_INFO_NAME,
                                    &t_format,
                                    &t_fct_tab);
    if (i4_ret != MINFOR_OK)
    {
        DBG_ERROR(("x_minfo_aud_es_init: dts minfo_register_handler failed(%d)\r\n",
                   i4_ret));
        return MINFOR_INITIAL_ERR;
    }
    
#ifdef CLI_LVL_ALL
    /* Initialize minfo CLI component */
    i4_ret = minfo_cli_init();
    if (i4_ret != MINFOR_OK)
    {
        return MINFOR_CLI_ERROR;
    } 
#endif
      
    return MINFOR_OK;
}
