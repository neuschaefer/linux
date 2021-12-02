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
 * asf_wmv8_intra.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0916b8010fb2d9ebf453b3d4efdbc5e9 $ Asa
 *
 * Description: 
 *         This file used for wmv8 x8intra detecting
 *         Author: lianming mcn07123 
 *         History:
 *         (1)2008-5-28  lianming mcn07123  initial
 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"  {
#endif

#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/u_common.h"
#include "inc/x_mm_common.h"
#include "handle/u_handle.h"
#include "file_mngr/x_fm.h"
#include "unicode/x_uc_str.h"
#include "dbg/u_dbg.h"
#include "dbg/x_dbg.h"
#include "mutil/minfo/minfo.h"
#include "asf_wmv8_intra.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "x_mm_common.h"
#include "u_handle.h"
#include "../../mm_util.h"
#include "x_uc_str.h"
#include "u_dbg.h"
#include "x_dbg.h"
#include "../minfo.h"
#include "asf_wmv8_intra.h"

#endif
/*-----------------------------------------------------------------------------
 * Name:  fgWMVX8intra
 *
 * Description: This function test wmv8 can be supported
 *
 * Inputs:  prHeaderInfo:specify the video sequence header
 *          
 * Outputs: NULL 
 * 
 * Returns: TRUE :        if not supported
 *          FALSE:        if supported
 *
 * NOTE:    NULL
 *
 * Author : Jason Hsiao
 * 
 * History:
 *   (1)2008-5-27 : initial
        
----------------------------------------------------------------------------*/
BOOL fgDecoderSupport(MINFO_INFO_VIDEO_ENC_T rDecoderType,SeqHeaderInfo* prHeaderInfo)
{
    INT32  i4SeqHdrData1;
    BOOL    fgXintra8Switch;

    if(rDecoderType == MINFO_INFO_VID_ENC_UNKNOWN)
    {
        return FALSE;
    }
    
    if(rDecoderType != MINFO_INFO_VID_ENC_WMV2 && rDecoderType != MINFO_INFO_VID_ENC_WMV3)
    {
        return TRUE;
    }
    else
    {
        x_memcpy(&i4SeqHdrData1,(prHeaderInfo->u1Addr),4);
        
        i4SeqHdrData1 = ((i4SeqHdrData1&0x000000FF)<<24) |
                        ((i4SeqHdrData1&0x0000FF00)<<8) | 
                        ((i4SeqHdrData1&0x00FF0000)>>8) | 
                        ((i4SeqHdrData1&0xFF000000)>>24);

        if((prHeaderInfo->u4Len) <4 )
            return TRUE;
            
        if(rDecoderType == MINFO_INFO_VID_ENC_WMV2)
        {
            if((prHeaderInfo->u4Len) >=4) 
            {
            
                fgXintra8Switch  = (i4SeqHdrData1& 0x00001000) >> 12;
                
                return (!fgXintra8Switch);
            }
            else
            {
                return TRUE;
            }
        }
        else if(rDecoderType == MINFO_INFO_VID_ENC_WMV3)
        {
            BOOL  fgYUV411;
            INT32 i4Profile;
            i4Profile = (i4SeqHdrData1& 0xc0000000) >> 30;
            fgYUV411 = (i4SeqHdrData1 & 0x20000000) >> 29;

            if(i4Profile ==WMV3_PC_PROFILE || fgYUV411)
            {
                return FALSE;
            }
            else if(i4Profile == WMV3_SIMPLE_PROFILE || i4Profile ==WMV3_MAIN_PROFILE)
            {
                if(!(i4SeqHdrData1&0x1))
                {
                    return FALSE;
                }
                else
                {
                    return TRUE;
                }
            }
            else
                return TRUE;      
        }
        else
        {
            return TRUE;
        }
    }
}
#ifdef __cplusplus
}
#endif


