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
 * $RCSfile: mm_util_8859_cvt.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file exports all input function API for media utility 
 *---------------------------------------------------------------------------*/

#ifndef _MM_UTIL_TABLE_H_
#define _MM_UTIL_TABLE_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "mm_util.h"

#else

#include "u_common.h"
#include "u_handle.h"
#include "x_dbg.h"
#include "u_dbg.h"
#include "mm_util.h"

#endif

#ifdef MINFO_ID3_ENCODE_BY_LANGUAGE
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
typedef enum {
    MM_UTIL_TABLE_ISO_UNKNOWN,
    MM_UTIL_TABLE_ISO_8859_1,
    MM_UTIL_TABLE_ISO_8859_2,
    MM_UTIL_TABLE_ISO_8859_3,
    MM_UTIL_TABLE_ISO_8859_4,
    MM_UTIL_TABLE_ISO_8859_5,
    MM_UTIL_TABLE_ISO_8859_6,
    MM_UTIL_TABLE_ISO_8859_7,
    MM_UTIL_TABLE_ISO_8859_8,
    MM_UTIL_TABLE_ISO_8859_9,
    MM_UTIL_TABLE_ISO_8859_10,
    MM_UTIL_TABLE_ISO_8859_11,
    MM_UTIL_TABLE_ISO_8859_12,
    MM_UTIL_TABLE_ISO_8859_13,
    MM_UTIL_TABLE_ISO_8859_14,
    MM_UTIL_TABLE_ISO_8859_15,
    MM_UTIL_TABLE_ISO_8859_16,
    MM_UTIL_TABLE_ISO_8859_9_HUN,
    MM_UTIL_TABLE_ISO_10646_1,
    MM_UTIL_TABLE_KSC_5601,
    MM_UTIL_TABLE_GB_2312,
    MM_UTIL_TABLE_ISO_10646_1_ZH_TW,
    MM_UTIL_TABLE_ISO_10646_1_UTF_8
}   E_MM_UTIL_CHAR_TBL;

typedef struct _ID3_TAG_LCTBL_T
{
    ISO_639_LANG_T      e_langage;
    E_MM_UTIL_CHAR_TBL  e_chr_table;
}ID3_TAG_LCTBL_T;

/* Convert 8859-* to Unicode 16 */
extern INT32
mm_cvt_8859_ps_to_w2s(UINT16          ui2_part_num,
                              const CHAR*     ps_src,
                              UINT16          ui2_src_len,
                              UTF16_T*        w2s_dst,
                              UINT16*         pui2_dst_len);

/* Convert languange to 8859-* */
extern E_MM_UTIL_CHAR_TBL 
mm_cvt_lang_to_chartbl( ISO_639_LANG_T e_lang );

#endif
#endif /* _MM_UTIL_TABLE_H_ */
