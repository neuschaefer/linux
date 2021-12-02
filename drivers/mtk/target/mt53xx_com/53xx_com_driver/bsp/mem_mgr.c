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
 *
 * $Author: yingjoe.chen $
 * $Date: 2012/07/31 $
 * $RCSfile: mem_mgr.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file mem_mgr.c
 *  non-OS library.
 *
 */

#ifdef CC_MTK_LOADER

#include "x_typedef.h"
#include "x_timer.h"
#include "loader_imghdr.h"
#include <string.h>

static UINT32 g_u4MemPtr = (CC_LOADER_MEM_OFFSET + LOADER_MAX_SIZE*16); // for the loader at 0xd00000.

void *x_mem_alloc(UINT32 z_size)
{
#ifdef CC_UBOOT
    return malloc(z_size);
#else /* CC_UBOOT */
    void *p;

    p = (void *)g_u4MemPtr;
    g_u4MemPtr += (z_size + 7) & 0xfffffffc;  // for 4-bytes alignment, Add 4 bytes size for realloc.
    *(UINT32*)p = z_size;
    return ((char*)p)+4;
#endif /* CC_UBOOT */
}

void x_mem_free(void *pv_mem_block)
{
    /* do nothing. */
}

#ifndef CC_MTK_PRELOADER
void *x_mem_alloc_virtual(UINT32 z_size)
{
    return x_mem_alloc(z_size);
}

void *
x_mem_realloc(void *pv_mem, UINT32 z_new_size)
{
#ifdef CC_UBOOT
    return realloc(pv_mem, z_new_size);
#else
    void *ptr;
    UINT32 z_size = pv_mem ? (*(((UINT32*)pv_mem)-1)) : 0;

    if (z_size >= z_new_size)
        return pv_mem;

    ptr = x_mem_alloc(z_new_size);
    if (ptr && pv_mem)
    {
        if (z_size)
            memcpy(ptr, pv_mem, z_size);
    }

    return ptr;
#endif
}

void *
x_mem_calloc(UINT32 ui4_ns, UINT32 z_new_size)
{
    VOID *pv_mem = NULL;
    UINT32  *pui4_mem;

    pv_mem = x_mem_alloc(ui4_ns * z_new_size);

    if (pv_mem != NULL)
    {
        pui4_mem = (UINT32 *) pv_mem;
        memset(pui4_mem, 0, z_new_size);
    }
    return(pv_mem);
}

void *x_memset(void *s, int c, UINT32 n)
{
    return (void *)memset(s, c, n);
}
#endif /* CC_MTK_PRELOADER */

void *x_memcmp(void *dest, const void *src, UINT32 z_size)
{
#if defined(CC_MTK_PRELOADER) && defined(__GCC__)
    UINT8 *pu1Dest = (UINT8*)dest;
    UINT8 *pu1Src = (UINT8*)src;

    for( ; z_size > 0; z_size -= 1) 
    {
        if( (*pu1Dest) == (*pu1Src))
        {
            pu1Dest += 1;
            pu1Src +=1;
            continue;
        }
        else if((*pu1Dest) > (*pu1Src))
        {
            return (void*)1;
}
        else 
{
            return (void*)-1;
        }
    }
    return (void*)0;
#else
    return (void *)memcmp(dest, src, z_size);
#endif
}

void *x_memcpy(void *dest, const void *src, UINT32 z_size)
{
#if defined(CC_MTK_PRELOADER) && defined(__GCC__)
    UINT8 *pu1Dest = (UINT8*)dest;
    UINT8 *pu1Src = (UINT8*)src;
    
    for( ; z_size > 0; z_size -= 1) 
    {
        *pu1Dest = *(UINT8 *)pu1Src;
        pu1Dest += 1;
        pu1Src +=1;
    }

    return pu1Dest;
#else
    return (void *)memcpy(dest, src, z_size);
#endif
}

void __rt_raise(INT32 signal, INT32 type)
{
}

void __rt_SIGFPE()
{
}

#ifndef CC_MTK_PRELOADER
char *x_strcpy(char *dest, const char *src)
{
    return strcpy(dest, src);
}

char *x_strncpy(char *dest, const char *src, UINT32 n)
{
    return strncpy(dest, src, n);
}

UINT32 x_strlen(const char *s)
{
    return strlen(s);
}

int x_strncmp(const char *s1, const char *s2, UINT32 n)
{
    return strncmp(s1, s2, n);
}

int x_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

char *x_strncat(char *dest, const char *src, UINT32 n)
{
    return strncat(dest, src, n);
}

char *x_strcat(char *dest, const char *src)
{
    return strcat(dest, src);
}

void x_thread_delay(UINT32 u4Ms)
{
    HAL_Delay_us(u4Ms*1000);
}
#endif /* CC_MTK_PRELOADER */

#endif //CC_MTK_LOADER
