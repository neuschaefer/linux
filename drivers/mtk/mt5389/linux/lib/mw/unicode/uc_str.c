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
 * $RCSfile: uc_str.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/7 $
 * $SWAuthor: Ben Tsai $
 * $MD5HEX: 2aac742e9d322ab92fd0653a907999f7 $
 *
 * Description:
 *         This file contains exported 'Unicode String' functions.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__ 
#include "os/inc/x_os.h"
#include "unicode/uc_str.h"
#include "unicode/x_uc_str.h"
#else
#include "x_os.h"
#include "uc_str.h"
#include "x_uc_str.h"
#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Macros for ASCII only */
#define TO_LOWER(_c)    ( (_c)-'A'+'a' )
#define TO_UPPER(_c)    ( (_c)-'a'+'A' )
#define IS_LOWER(_c)    ( (_c)<='z' && (_c)>='a' )
#define IS_UPPER(_c)    ( (_c)<='Z' && (_c)>='A' )


#ifndef UC_INIT_DBG_LEVEL
#define UC_INIT_DBG_LEVEL       5
#endif
static UINT16                   ui2_uc_dbg_level = UC_INIT_DBG_LEVEL;


/*-----------------------------------------------------------------------------
 * Name:  uc_w2s_case_cnv
 *
 * Description: This API will convert a UTF-16 string to upper or lower case.
 *              Currently, the ps_locale parameter is reserved for future,
 *              and only NULL is accpetable. In this case, it will always
 *              assume that ASCII is used when doing conversion.
 *
 * Inputs:  ui4_dst_len  The buffer size of w2s_dst. The unit is the number
 *                       of UTF16_T characters.
 *          w2s_src      String to be converted.
 *          ps_locale    Locale string. Reserved for future.
 *          e_case       UC_CASE type enumeration. Upper or Lower case.
 *
 * Outputs: w2s_dst      The converted string.
 *
 * Returns: UCR_NO_SUPPORT          Specified locale is not supported.
 *          UCR_NO_ENOUGH_BUFFER    Size of w2s_dst is not enough.
 *          UCR_NO_MEMORY           Run out of system memory.
 *          UCR_CNV_FAIL            Fail to convert between UTF-8 and UTF-16
 *          UCR_OK                  Success
 ----------------------------------------------------------------------------*/
INT32 uc_w2s_case_cnv(
    UTF16_T*        w2s_dst,
    UINT32          ui4_dst_len,
    const UTF16_T*  w2s_src,
    const CHAR*     ps_locale,
    UC_CASE         e_case)
{
    /* Todo : Only for ASCII now. Fix me */

    UINT32  ui4_src_len;
    UINT32  ui4_i;
    CHAR*   ps_buf;
    INT32   i4_ret;

    if(ps_locale != NULL)
    {
        return UCR_NO_SUPPORT;
    }

    /* allocate memory buffer */
    ui4_src_len = x_uc_w2s_strlen(w2s_src);
    if(ui4_dst_len < ui4_src_len)
    {
        return UCR_NO_ENOUGH_BUFFER;
    }

    ps_buf = (CHAR*)x_mem_alloc(sizeof(CHAR) * (ui4_src_len+1));
    if(ps_buf == NULL)
    {
        return UCR_NO_MEMORY;
    }

    /* convert to UTF-8 */
    i4_ret = x_uc_w2s_to_ps(w2s_src, ps_buf, ui4_src_len+1);
    if(i4_ret != UCR_OK)
    {
        x_mem_free(ps_buf);
        return UCR_CNV_FAIL;
    }

    /* convert to Lower Case */
    for(ui4_i=0; ui4_i<ui4_src_len; ui4_i++)
    {
        if( e_case==UC_CASE_UPPER && IS_LOWER(ps_buf[ui4_i]) )
        {
            ps_buf[ui4_i] = TO_UPPER(ps_buf[ui4_i]);
        }
        else if( e_case==UC_CASE_LOWER && IS_UPPER(ps_buf[ui4_i]) )
        {
            ps_buf[ui4_i] = TO_LOWER(ps_buf[ui4_i]);
        }
    }

    /* convert to UTF-16 */
    i4_ret = x_uc_ps_to_w2s(ps_buf, w2s_dst, ui4_dst_len);
    if(i4_ret != UCR_OK)
    {
        x_mem_free(ps_buf);
        return UCR_CNV_FAIL;
    }

    /* free memory buffer */
    x_mem_free(ps_buf);

    return UCR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: uc_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 uc_get_dbg_level(
    VOID)
{
    return ui2_uc_dbg_level;
}
