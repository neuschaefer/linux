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
 * $RCSfile: dlm_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#include "x_dlm.h"
#include "dlm_digest.h"
#include "dlm_fm.h"
#include "dlm_util.h"
#include "dlm.h"

#ifdef DLM_AES_ALGORITHM_ENCRYPT
#include "dlm_aes.h"
#else
#include "efuse.h"
#endif


#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
    #include <linux/string.h>
#else /* CC_UBOOT */
#ifdef CC_MTK_LOADER
    #include "x_dbg.h"
    #include "x_os.h"
#else
    #include <stdio.h>
    #include <stdlib.h>
#endif /* CC_MTK_LOADER */
    #include <string.h>
#endif /* CC_UBOOT */
#else
    #include "os/inc/x_os.h"
    #include "dbg/x_dbg.h"
#endif

/* store to save append data header */
static Append_T *pt_tag_in_file = NULL;
static CHAR *ps_file = NULL;
extern INT32     i4_codefile_header_offset;
extern INT32     dlm_locate_codefile_header(CHAR *filename, DLM_IO_T *pt_user_io);
/**
 * dlm_add_search_codefile_tag
 * 
 * Add tag into store
 * 
 * @param puac_tag: input tag
 * 
 * @return VOID
 */
static INT32 dlm_add_search_codefile_tag(Append_DATA_TLV_T *pt_hdr)
{
    Append_T *pt_new = (Append_T*) x_mem_alloc(sizeof(Append_T));
    INT32 i4_len;

    if (pt_new == NULL)
    {
        return -1;
    }

    x_memset(pt_new, 0, sizeof(Append_T));

    x_memcpy(pt_new->uc_type, pt_hdr->uc_type, 4);

    i4_len = pt_hdr->uc_length[3] * 256 * 256 * 256 + 
             pt_hdr->uc_length[2] * 256 * 256 + 
             pt_hdr->uc_length[1] * 256 + 
             pt_hdr->uc_length[0];

    pt_new->ui4_length = (UINT32) i4_len;
    pt_new->uac_privacy[0] = pt_hdr->uc_privacy[0];

    /* insert */
    if (pt_tag_in_file)
    {
        pt_new->pt_next = pt_tag_in_file;
    }
    pt_tag_in_file = pt_new;

    return 0;
}

/**
 * dlm_free_search_codefile_tag
 * 
 * free search tag list
 * 
 * @return VOID
 */
static VOID dlm_free_search_codefile_tag(VOID)
{
    Append_T *pt_tmp;

    while (pt_tag_in_file)
    {
        pt_tmp = pt_tag_in_file;
        pt_tag_in_file = pt_tag_in_file->pt_next;
        x_mem_free(pt_tmp);
    }
    pt_tag_in_file = NULL;
}

/**
 * x_dlm_get_codefile_tag_list
 * 
 * Get codefile tag for future process
 * 
 * @param ps_filepath: codefile name
 * 
 * @return Append_T*
 *         if NULL,   codefile doesn't exist any tag
 *         others,    tag list
 */
Append_T *x_dlm_get_codefile_tag_list(CHAR *ps_filepath)
{    
    INT32 i4_file, i4_ret;
    CHAR  ac_buffer[256];

    Append_DATA_TLV_T       *pt_append_hdr = (Append_DATA_TLV_T *) ac_buffer;
    DEFAULT_CODE_FILE_HDR_T *pt_header = (DEFAULT_CODE_FILE_HDR_T *)ac_buffer;
    UINT32      ui4_file_size = 0;

#ifdef DLM_AES_ALGORITHM_ENCRYPT
    UCHAR uc_key[16];
    UCHAR uc_vector[16];
    UINT32 ui4_key = DLM_HDR_KEY;
    x_memset(uc_vector,0,16);
    x_memcpy(uc_key,&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32),&ui4_key,sizeof(UINT32));
#else
        UINT32      ui4_iv;
#endif
    if(ps_file && 
      (ps_filepath == ps_file))
    {
        /* cache data */
        return pt_tag_in_file;
    }
    else
    {
        /* new search */
        dlm_free_search_codefile_tag();
    }

    /* locate header */
    i4_ret = dlm_locate_codefile_header(ps_filepath, NULL);
    if (i4_ret != DLM_OK)
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", ps_filepath));
        return NULL;
    }
    
    /* open source */
    if ((i4_file = dlm_fopen(ps_filepath, kDlm_Read)) == -1)
    {
        DLM_TRACE(("Open download file %s failed\n", ps_filepath));
        goto QUIT;
    }

    /* skip header */
    if (dlm_fread(i4_file, (CHAR *)ac_buffer, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset) == -1)
        goto QUIT;
    
#if HDR_ENCRYPTED
    pt_header = (DEFAULT_CODE_FILE_HDR_T *) (ac_buffer+i4_codefile_header_offset);
#ifdef DLM_AES_ALGORITHM_ENCRYPT
    dlm_aes((UCHAR*)ac_buffer+i4_codefile_header_offset,
            NULL,
            sizeof(DEFAULT_CODE_FILE_HDR_T)-32,
            uc_key,
            uc_vector,
            AES_DECRYPT);
#else
    ui4_iv = DLM_HDR_IV;
    dlm_decrypt_efuse((UCHAR*)ac_buffer+i4_codefile_header_offset, sizeof(DEFAULT_CODE_FILE_HDR_T)-32, DLM_HDR_KEY, &ui4_iv);
#endif
#endif
    /* check magic */
    if (x_memcmp(pt_header->s_magic, DLM_HDR_MAGIC, 8) != 0)
    {
        goto QUIT;
    }

    ui4_file_size = pt_header->ui4_total_size;
    ui4_file_size -= sizeof(DEFAULT_CODE_FILE_HDR_T);

    while (!dlm_feof(i4_file) && ui4_file_size>0)
    {
        INT32 i4_len;

        if (dlm_fread(i4_file, (CHAR *)ac_buffer, sizeof(Append_DATA_TLV_T)) == -1)
        {
            goto QUIT;
        }

        dlm_add_search_codefile_tag(pt_append_hdr);

        i4_len = pt_append_hdr->uc_length[3] * 256 * 256 * 256 + 
                 pt_append_hdr->uc_length[2] * 256 * 256 + 
                 pt_append_hdr->uc_length[1] * 256 + 
                 pt_append_hdr->uc_length[0];

        if(dlm_fseek(i4_file, (UINT32) i4_len) == -1)
        {
            goto QUIT;
        }
        ui4_file_size -= (sizeof(Append_DATA_TLV_T) + i4_len);
    }

    dlm_fclose (i4_file);
    return pt_tag_in_file;

QUIT:
    dlm_free_search_codefile_tag();
    dlm_fclose (i4_file);
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_dump_buffer
 *
 * Description: utility to dump memory buffer
 *
 * Inputs:  ps_msg,    prompt message
 *          puac_buf,  memory buffer ti dump data
 *          ui4_len,   buffer size
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID dlm_dump_buffer(char *ps_msg, UCHAR* puac_buf, UINT32 ui4_len)
{
#if DEBUG_DLM_TRACE
    INT32 i4_i, i4_j, i4_cnt = 16;
    UCHAR *pc_tmp = puac_buf;

    x_dbg_stmt("%s\n", ps_msg);

    for (i4_i =0; i4_i < (INT32) ui4_len; i4_i++)
    {
        i4_cnt--;
        x_dbg_stmt("%02x ", *(pc_tmp++));

        if (i4_cnt == 0)
        {
            x_dbg_stmt("   ");
            for (i4_j=0; i4_j<16; i4_j++)
            {
                if (i4_i != 0)
                {
                    if(*(pc_tmp-16+i4_j) > 32 && 
                       *(pc_tmp-16+i4_j) <= 126)
                    {
                        x_dbg_stmt("%c", *(pc_tmp-16+i4_j));
                    }
                    else
                        x_dbg_stmt(".");
                }
            }
            x_dbg_stmt("\n");
            i4_cnt = 16;
        }
    }
    x_dbg_stmt("\n");
#endif
}

/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
CHAR* dlm_strtok(register char *s, register char *delim)
{
	register char *spanp;
	int c, sc;
	char *tok;
	static char *last;


	if (s == NULL && (s = last) == NULL)
		return (NULL);

	/*
	 * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
	 */
cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}

	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	/*
	 * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
	 * Note that delim must have one NUL; we stop if we see that, too.
	 */
	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}


