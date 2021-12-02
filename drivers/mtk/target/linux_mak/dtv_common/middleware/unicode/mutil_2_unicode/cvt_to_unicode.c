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
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "../u_uc.h"
#include "../u_uc_str.h"
#include "cvt_to_unicode.h"

#include "inc/x_common.h"
#include "../uc_str.h"
#include "../x_uc_str.h"

#ifndef ANDROID
#include "../uc_cmn/unicode/utypes.h"
#include "../uc_cmn/unicode/ustring.h"
#endif

#ifndef MW_DISABLE_KSC_UNICODE_CONVERTER
#include "kr.h"
#endif

#ifndef MW_DISABLE_JIS_UNICODE_CONVERTER
#include "shift_jis.h"
#endif

#ifndef MW_DISABLE_BIG5_UNICODE_CONVERTER
#include "big5.h"
#endif

#ifndef MW_DISABLE_GBK2312_UNICODE_CONVERTER
#include "gb_2312.h"
#endif

#ifndef MW_DISABLE_GB18030_UNICODE_CONVERTER
#include "gb_18030_2b.h"
#include "gb_18030_4b.h"
#endif

#ifndef MW_DISABLE_CODEPAGE_UNICODE_CONVERTER
#include "window_codepage.h"
#endif


static INT32 gb2312_to_nicode(UINT16* pui2_multi_byte, UINT32 ui4_len, UINT16* pui2_unicode)
{
    UINT32 i;
    
    for(i = 0; i < ui4_len; i ++)
    {
        if(pui2_multi_byte[i] <= 0x7F)
        {
            pui2_unicode[i] = pui2_multi_byte[i];
        }
#ifndef MW_DISABLE_GBK2312_UNICODE_CONVERTER
        else if((pui2_multi_byte[i] >= 0x4080) && 
                (pui2_multi_byte[i] < 0xff80)  && 
                (pui2_multi_byte[i] & 0x80) != 0
               ) /*search from table*/
        {
            UINT32 ui4_idx = 0;
            ui4_idx = (UINT32)(((pui2_multi_byte[i] >> 8) - 0x40) * 0x80) + (UINT32)(pui2_multi_byte[i] & 0x7F);
            if(ui4_idx < (UINT32)(sizeof(ac_gb_2312)))
            {
                pui2_unicode[i] = ac_gb_2312[ui4_idx];
            }
        }
#endif
        else
        {
            pui2_unicode[i] = 0x20;
        }
    }

    return UCR_OK;
}

static INT32 gb2312_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    UINT32 i;    
    UINT32 ui4_max_out_len = * pui4_dst_len;
    UCHAR * puac_multi_byte = (UCHAR*)pv_multi_byte;
    * pui4_dst_len = 0;

    for(i = 0; i < ui4_src_len && *pui4_dst_len < ui4_max_out_len; )
    {
        if (puac_multi_byte[i] <= 0x7F)
        {
            if (puac_multi_byte[i] != 0x0)
            {
                pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i];
                i ++;
            }
            else
            {
                if ((i + 1 < ui4_src_len) && (puac_multi_byte[i + 1] < 0x7F))
                {
                    pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i + 1];
                    i ++;
                    i ++;
                }
                else
                {
                    pui2_unicode[*pui4_dst_len] = 0x0;
                    i ++;
                }
            }
            
        }
        
        else if (puac_multi_byte[i] >= 0x80 && puac_multi_byte[i] <= 0xFE)
        {
            if (i + 1 <= ui4_src_len)
            {
#ifndef MW_DISABLE_GBK2312_UNICODE_CONVERTER
                if (puac_multi_byte[i + 1] >= 40 && puac_multi_byte[i + 1] <= 0xFE)
                {
                    UINT32 ui4_idx = 0;
                    ui4_idx = (UINT32)(((puac_multi_byte[i + 1]) - 0x40) * 0x80) + (UINT32)(puac_multi_byte[i] & 0x7F);
                    if(ui4_idx < (UINT32)(sizeof(ac_gb_2312)))
                    {
                        pui2_unicode[*pui4_dst_len] = ac_gb_2312[ui4_idx];
                    }
                    
                }
                else
#endif
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                }

                i ++;
                i ++;
                
            }
            else
            {
                *pui4_cut_pos = i;
                break;
            }
        }
        else
        {
            pui2_unicode[*pui4_dst_len] = 0x20;
            i ++;
        }
        
        *pui4_cut_pos = i;
        *pui4_dst_len  = *pui4_dst_len + 1;
    }
    return UCR_OK;
}

static INT32 big5_to_nicode(UINT16* pui2_multi_byte, UINT32 ui4_len, UINT16* pui2_unicode)
{
    UINT32 i;

    for(i = 0; i < ui4_len; i ++)
    {
        if(pui2_multi_byte[i] <= 0x7F)
        {
            pui2_unicode[i] = pui2_multi_byte[i];
        }
        
#ifndef MW_DISABLE_BIG5_UNICODE_CONVERTER
        else if((pui2_multi_byte[i] >= 0x4080) && 
                (pui2_multi_byte[i] < 0xff80)  && 
                (pui2_multi_byte[i] & 0x80) != 0
               ) /*search from table*/
        {
            UINT32 ui4_idx = 0;
            ui4_idx =(UINT32)(((pui2_multi_byte[i] >> 8) - 0x40) * 0x80) + (UINT32)((pui2_multi_byte[i] & 0x7F));
            if(ui4_idx < (UINT32)(sizeof(ac_big5)))
            {
                pui2_unicode[i] = ac_big5[ui4_idx];
            }
        }
#endif
        else
        {
            pui2_unicode[i] = 0x20;
        }
    }

    return UCR_OK;
}

static INT32 big5_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    UINT32 i;
    UINT32 ui4_max_out_len = * pui4_dst_len;
    UCHAR * puac_multi_byte = (UCHAR*)pv_multi_byte;
    * pui4_dst_len = 0;

    for(i = 0; i < ui4_src_len && *pui4_dst_len < ui4_max_out_len;)
    {
        if (puac_multi_byte[i] <= 0x7F)
        {
            if (puac_multi_byte[i] != 0x0)
            {
                pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i];
                i ++;
            }
            else
            {
                if ((i + 1 < ui4_src_len) && (puac_multi_byte[i + 1] < 0x7F))
                {
                    pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i + 1];
                    i ++;
                    i ++;
                }
                else
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                    i ++;
                }
            }
            
        }
        
        else if (puac_multi_byte[i] >= 0x80 && puac_multi_byte[i] <= 0xFE)
        {
            if (i + 1 <= ui4_src_len)
            {
            
#ifndef MW_DISABLE_BIG5_UNICODE_CONVERTER
                if (puac_multi_byte[i + 1] >= 40 && puac_multi_byte[i + 1] <= 0xFE)
                {
                    UINT32 ui4_idx = 0;
                    ui4_idx = (UINT32)(((puac_multi_byte[i + 1]) - 0x40) * 0x80) + (UINT32)(puac_multi_byte[i] & 0x7F);
                    if(ui4_idx < (UINT32)(sizeof(ac_big5)))
                    {
                        pui2_unicode[*pui4_dst_len] = ac_big5[ui4_idx];
                    }
                    
                }
                else 
#endif
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                }

                i ++;
                i ++;
                
            }
            else
            {
                *pui4_cut_pos = i;
                break;
            }
        }
        else
        {
            pui2_unicode[*pui4_dst_len] = 0x20;
            i ++;
        }
        
        *pui4_cut_pos = i;
        *pui4_dst_len  = *pui4_dst_len + 1;
    }
    return UCR_OK;
}

static INT32 shift_jis_to_nicode(UINT16* pui2_multi_byte, UINT32 ui4_len, UINT16* pui2_unicode)
{
    UINT32 i;

    for(i = 0; i < ui4_len; i ++)
    {
        if(pui2_multi_byte[i] <= 0x7F)
        {
            pui2_unicode[i] = pui2_multi_byte[i];
        }
#ifndef MW_DISABLE_JIS_UNICODE_CONVERTER
        else if((pui2_multi_byte[i] >= 0x4080) && 
                (pui2_multi_byte[i] < 0xfd80)  && 
                (pui2_multi_byte[i] & 0x80) != 0
               ) /*search from table*/
        {
            UINT32 ui4_idx = 0;
            ui4_idx = (UINT32)(((pui2_multi_byte[i] >> 8) - 0x40) * 0x80) + (UINT32)(pui2_multi_byte[i] & 0x7F);
            if(ui4_idx < (UINT32)(sizeof(ac_shift_jis)))
            {
                pui2_unicode[i] = ac_shift_jis[ui4_idx];
            }
        }
        else
#endif
        {
            pui2_unicode[i] = 0x20;
        }
    }

    return UCR_OK;
}

static INT32 shift_jis_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    UINT32 i;
    UINT32 ui4_max_out_len = * pui4_dst_len;
    UCHAR * puac_multi_byte = (UCHAR*)pv_multi_byte;
    * pui4_dst_len = 0;

    for(i = 0; i < ui4_src_len && *pui4_dst_len < ui4_max_out_len; )
    {
        if (puac_multi_byte[i] <= 0x7F)
        {
            if (puac_multi_byte[i] != 0x0)
            {
                pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i];
                i ++;
            }
            else
            {
                if ((i + 1 < ui4_src_len) && (puac_multi_byte[i + 1] < 0x7F))
                {
                    pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i + 1];
                    i ++;
                    i ++;
                }
                else
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                    i ++;
                }
            }
            
        }
        
        else if (puac_multi_byte[i] >= 0x80 && puac_multi_byte[i] <= 0xFE)
        {
            if (i + 1 <= ui4_src_len)
            {
#ifndef MW_DISABLE_JIS_UNICODE_CONVERTER
                if (puac_multi_byte[i + 1] >= 40 && puac_multi_byte[i + 1] <= 0xFE)
                {
                    UINT32 ui4_idx = 0;
                    ui4_idx = (UINT32)(((puac_multi_byte[i + 1]) - 0x40) * 0x80) + (UINT32)(puac_multi_byte[i] & 0x7F);
                    if(ui4_idx < (UINT32)(sizeof(ac_shift_jis)))
                    {
                        pui2_unicode[*pui4_dst_len] = ac_shift_jis[ui4_idx];
                    }
                    
                }
                else 
#endif
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                }

                i ++;
                i ++;
                
            }
            else
            {
                *pui4_cut_pos = i;
                break;
            }
        }
        else
        {
            pui2_unicode[*pui4_dst_len] = 0x20;
            i ++;
        }
        
        *pui4_cut_pos = i;
        *pui4_dst_len  = *pui4_dst_len + 1;
    }
    return UCR_OK;
}
static INT32 kr_to_nicode(UINT16* pui2_multi_byte, UINT32 ui4_len,UINT16* pui2_unicode)
{
    UINT32 i;

    for(i = 0; i < ui4_len; i ++)
    {
        if(pui2_multi_byte[i] <= 0x7F)
        {
            pui2_unicode[i] = pui2_multi_byte[i];
        }
        
#ifndef MW_DISABLE_KSC_UNICODE_CONVERTER

        else if((pui2_multi_byte[i] >= 0x4180) && 
                (pui2_multi_byte[i] < 0xff80)  && 
                (pui2_multi_byte[i] & 0x80) != 0
               ) /*search from table*/
        {
            UINT32 ui4_idx = 0;
            ui4_idx = (UINT32)(((pui2_multi_byte[i] >> 8) - 0x41) * 0x80) +  (UINT32)(pui2_multi_byte[i] & 0x7F);
            if(ui4_idx < (UINT32)(sizeof(ac_kr)))
            {
                pui2_unicode[i] = ac_kr[ui4_idx];
            }
        }
#endif
        else
        {
            pui2_unicode[i] = 0x20;
        }
    }

    return UCR_OK;
}

static INT32 kr_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    UINT32 i;
    UINT32 ui4_max_out_len = * pui4_dst_len;
    UCHAR * puac_multi_byte = (UCHAR*)pv_multi_byte;
    * pui4_dst_len = 0;

    for(i = 0; i < ui4_src_len && *pui4_dst_len < ui4_max_out_len;)
    {
        if (puac_multi_byte[i] <= 0x7F)
        {
            if (puac_multi_byte[i] != 0x0)
            {
                pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i];
                i ++;
            }
            else
            {
                if ((i + 1 < ui4_src_len) && (puac_multi_byte[i + 1] < 0x7F))
                {
                    pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i + 1];
                    i ++;
                    i ++;
                }
                else
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                    i ++;
                }
            }
            
        }
        
        else if (puac_multi_byte[i] >= 0x80 && puac_multi_byte[i] <= 0xFE)
        {
            if (i + 1 <= ui4_src_len)
            {
                
#ifndef MW_DISABLE_KSC_UNICODE_CONVERTER
                if (puac_multi_byte[i + 1] >= 40 && puac_multi_byte[i + 1] <= 0xFE)
                {
                    UINT32 ui4_idx = 0;
                    ui4_idx = (UINT32)(((puac_multi_byte[i + 1]) - 0x40) * 0x80) + (UINT32)(puac_multi_byte[i] & 0x7F);
                    if(ui4_idx < (UINT32)(sizeof(ac_kr)))
                    {
                        pui2_unicode[*pui4_dst_len] = ac_kr[ui4_idx];
                    }
                    
                }
                else
#endif
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                }

                i ++;
                i ++;
                
            }
            else
            {
                *pui4_cut_pos = i;
                break;
            }
        }
        else
        {
            pui2_unicode[*pui4_dst_len] = 0x20;
            i ++;
        }
        
        *pui4_cut_pos = i;
        *pui4_dst_len  = *pui4_dst_len + 1;
    }
    return UCR_OK;
}

static INT32  gb18030_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    UINT32 i;    
    UINT32 ui4_max_out_len = * pui4_dst_len;
    UCHAR * puac_multi_byte = (UCHAR*)pv_multi_byte;
    * pui4_dst_len = 0;
    
    for(i = 0; i < ui4_src_len && *pui4_dst_len < ui4_max_out_len; )
    {
        if (puac_multi_byte[i] <= 0x7F)
        {
            if (puac_multi_byte[i] != 0x0)
            {
                pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i];
                i ++;
            }/* one byte */
            else
            {
                if ((i + 1 < ui4_src_len) && (puac_multi_byte[i + 1] < 0x7F))
                {
                    pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i + 1];
                    i ++;
                    i ++;
                }
                else
                {
                    pui2_unicode[*pui4_dst_len] = 0x0;
                    i ++;
                }
            }
            
        }  
		
#ifndef MW_DISABLE_GB18030_UNICODE_CONVERTER             
        else if (puac_multi_byte[i] >= 0x81 && puac_multi_byte[i] <= 0xFE )
        {
            if (i + 1 <= ui4_src_len)
            {
                if (puac_multi_byte[i + 1] >= 0x40 && puac_multi_byte[i + 1] <= 0xFE)
                {
                    UINT32 ui4_idx = 0,ui4_b1,ui4_b2;

                    ui4_b2= (UINT32)(puac_multi_byte[i+1]- 0x40)+1;
                    ui4_b1= (UINT32)(puac_multi_byte[i]-0x81)*(UINT32)(0xff - 0x40 - 1);/* when the code is 0x**7*,the offset is 15,so here need minus 1*/
                    if(puac_multi_byte[i+1] >= 0x80)
                    {
                       ui4_b2-=1;/* when the code is 0x**7*,the offset is 15,so here need minus 1 */
                    }                         
                    
                    ui4_idx = ui4_b1 + ui4_b2 - 1;
                    if(ui4_idx < (UINT32)(sizeof(ac_gb_18030_2b)))
                    {
                        pui2_unicode[*pui4_dst_len] = ac_gb_18030_2b[ui4_idx];
                    }
                    i += 2;
                    
                }/* two byte*/
                else if (i + 2 <= ui4_src_len && i + 3 <= ui4_src_len &&
                         puac_multi_byte[i] <= 0x84 &&                
                         puac_multi_byte[i + 1] >= 0x30 && puac_multi_byte[i + 1] <= 0x39 &&
                         puac_multi_byte[i + 2] >= 0x81 && puac_multi_byte[i + 2] <= 0xFE &&
                         puac_multi_byte[i + 3] >= 0x30 && puac_multi_byte[i + 3] <= 0x39)
                {
                   
                    UINT32 ui4_idx = 0,ui4_b1,ui4_b2,ui4_b3,ui4_b4;
                    ui4_b4= (UINT32)(puac_multi_byte[i+3] - 0x30)+1;/* the fourth byte's offset */
                    ui4_b3= (UINT32)(puac_multi_byte[i+2] - 0x81)*10;/* the third byte's offset */
                    ui4_b2= (UINT32)(puac_multi_byte[i+1] - 0x30)*(UINT32)(0xff - 0x81)*10;/* the second byte's offset */
                    ui4_b1= (UINT32)(puac_multi_byte[i] - 0x81)*10*(UINT32)(0xff - 0x81)*10;  /* the first byte's offset */
                  
                    ui4_idx = ui4_b1 + ui4_b2 + ui4_b3 + ui4_b4 - 1; /* ac_gb_18031_4b index start with 0, so minus 1*/
                    if(ui4_idx <(UINT32)( sizeof(ac_gb_18030_4b)))
                    {
                        pui2_unicode[*pui4_dst_len] = ac_gb_18030_4b[ui4_idx];
                    }
                    i += 4;
                }/*four bytes: first byte 0x81-0x82, second byte 0x30-0x39, third byte 0x81-0xFE, fourth byte 0x30-0x39 */      
                else
                {
                    pui2_unicode[*pui4_dst_len] = 0x20;
                    i += 2;
                }

            }
            else
            {
                *pui4_cut_pos = i;
                break;
            }
        }

#endif		
        else
        {
            pui2_unicode[*pui4_dst_len] = 0x20;
            i ++;
        }
        
        *pui4_cut_pos = i;
        *pui4_dst_len  = *pui4_dst_len + 1;
    }
    return UCR_OK;
}

static INT32  utf8_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
    UINT16 *pDest = pui2_unicode;
    INT32 ch=0;
    INT32  index = 0;
    UCHAR* pSrc = (UCHAR*) pv_multi_byte;
    UINT32 ui4_max_out_len = * pui4_dst_len;

	*pui4_dst_len  = 0;

    while(index < ui4_src_len && *pui4_dst_len < ui4_max_out_len)
	{
        ch = pSrc[index++];
        if(ch <=0x7f)
		{
            *pDest++=(UINT16)ch;
			*pui4_dst_len  = *pui4_dst_len + 1;
        }
		else
        {
            ch=utf8_nextCharSafeBody(pSrc, (int32_t*)&index, (int32_t)ui4_src_len, ch, -1);
			
            if(ch<0)
			{
			    
				*pui4_cut_pos = index;
			    
            }
			else if(ch<=0xFFFF)
            {
                *(pDest++)=(UINT16)ch;
				* pui4_dst_len  = *pui4_dst_len + 1;
            }
        }
		*pui4_cut_pos = index;
    }

     return UCR_OK;
}


static INT32 codepage_to_nicode_ex(VOID* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
						   UINT16* pui2_unicode, UINT32* pui4_dst_len, E_MUL_BYTE_TYPE e_multi_type)
{
	UINT32 i;	 
	UINT32 ui4_max_out_len = * pui4_dst_len;
	UCHAR * puac_multi_byte = (UCHAR*)pv_multi_byte;
	* pui4_dst_len = 0;
	const UINT16*	 codepage;

#ifndef MW_DISABLE_CODEPAGE_UNICODE_CONVERTER            

	switch(e_multi_type)
	{
		 case MUL_BYTE_CODEPAGE1250:
			
				codepage = codepage1250;
				break;
				
		 case MUL_BYTE_CODEPAGE1251:
			
				codepage = codepage1251;
				break;
				
		 case MUL_BYTE_CODEPAGE1252:
			
				codepage = codepage1252;
				break;
				
		 case MUL_BYTE_CODEPAGE1253:
			
				codepage = codepage1253;
				break;
				
		 case MUL_BYTE_CODEPAGE1254:
			
				codepage = codepage1254;
				break;
				
		 case MUL_BYTE_CODEPAGE1255:
			
				codepage = codepage1255;
				break;
				
		 case MUL_BYTE_CODEPAGE1256:
			
				codepage = codepage1256;
				break;
				
		 case MUL_BYTE_CODEPAGE1257:
			
				codepage = codepage1257;
				break;
				
		 case MUL_BYTE_CODEPAGE1258:
			
				codepage = codepage1258;
				break;

		default:

				return UCR_ERR_ARG;
	}			
#endif      
	for(i = 0; i < ui4_src_len && *pui4_dst_len < ui4_max_out_len; )
	{
		if (puac_multi_byte[i] <= 0x7F)
		{
			if (puac_multi_byte[i] != 0x0)
			{
				pui2_unicode[*pui4_dst_len] = (int)puac_multi_byte[i];
				i ++;
			}/* one byte */
			else
			{
				if ((i + 1 < ui4_src_len) && (puac_multi_byte[i + 1] < 0x7F))
				{
					pui2_unicode[*pui4_dst_len] = (UINT16)puac_multi_byte[i + 1];
					i ++;
					i ++;
				}
				else
				{
					pui2_unicode[*pui4_dst_len] = 0x0;
					i ++;
				}
			}
			
		}	
		else if (puac_multi_byte[i] >= 0x80 && puac_multi_byte[i] <= 0xFF )
		{
			if (i + 1 <= ui4_src_len)
			{
#ifndef MW_DISABLE_CODEPAGE_UNICODE_CONVERTER            
				UINT8 ui1_idx = 0;				 
				ui1_idx =(UINT8)(puac_multi_byte[i] - 0x80);

				pui2_unicode[*pui4_dst_len] = codepage[ui1_idx];
	
#endif    
				i += 1; 			   
			}/* two bytes*/
			else
			{
				*pui4_cut_pos = i;
				break;
			}

		}
		else
		{
			pui2_unicode[*pui4_dst_len] = 0x20;
			i ++;
		}
		*pui4_cut_pos = i;
		*pui4_dst_len  = *pui4_dst_len + 1;
	}
	return UCR_OK;
}


extern INT32 multi_byte_to_unicode(UINT16* pui2_multi_byte, UINT32 ui4_len, 
                           E_MUL_BYTE_TYPE e_multi_type, UINT16* pui2_unicode)
{
    if(NULL == pui2_multi_byte || 
       NULL == pui2_unicode    ||
       0    == ui4_len
      )
    {
        return UCR_NO_SUPPORT;
    }
    

    if(MUL_BYTE_GB2312 == e_multi_type)
    {
        return gb2312_to_nicode(pui2_multi_byte, ui4_len, pui2_unicode);
    }

    if(MUL_BYTE_BIG5 == e_multi_type)
    {
        return big5_to_nicode(pui2_multi_byte, ui4_len, pui2_unicode);
    }
    if(MUL_BYTE_SHIFT_JIS == e_multi_type)
    {
        return shift_jis_to_nicode(pui2_multi_byte, ui4_len, pui2_unicode);
    }
    
    if(MUL_BYTE_KR == e_multi_type)
    {
        return kr_to_nicode(pui2_multi_byte, ui4_len, pui2_unicode);
    }

    return UCR_NO_SUPPORT;
}

extern INT32 multi_byte_to_unicode_ex(void* pv_multi_byte, UINT32 ui4_src_len, UINT32* pui4_cut_pos,
                           E_MUL_BYTE_TYPE e_multi_type, UINT16* pui2_unicode, UINT32* pui4_dst_len)
{
     if(NULL == pv_multi_byte || 
       NULL == pui2_unicode    ||
       0    == ui4_src_len     ||
       NULL == pui4_cut_pos    ||
       NULL == pui4_dst_len
      )
    {
        return UCR_NO_SUPPORT;
    }

    if(MUL_BYTE_GB2312 == e_multi_type)
    {
        return gb2312_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len);
    }

    if(MUL_BYTE_BIG5 == e_multi_type)
    {
        return big5_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len);
    }

    if(MUL_BYTE_SHIFT_JIS == e_multi_type)
    {
        return shift_jis_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len);
    }

    if(MUL_BYTE_KR == e_multi_type)
    {
        return kr_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len);
    }
	
    if(MUL_BYTE_GB18030 == e_multi_type)
    {
        return gb18030_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len);
	}

    if(MUL_BYTE_UTF8 == e_multi_type)
    {
        return utf8_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len);
	}
		
    if((e_multi_type >= MUL_BYTE_CODEPAGE1250) && (e_multi_type <= MUL_BYTE_CODEPAGE1258))
    {
        return codepage_to_nicode_ex(pv_multi_byte, ui4_src_len, pui4_cut_pos, pui2_unicode, pui4_dst_len, e_multi_type);
    }

    return UCR_NO_SUPPORT;
}


