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
 * $RCSfile: dlm_gentool.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_dlm.h"
#include "dlm_digest.h"
#include "dlm_fm.h"
#include "dlm_util.h"
#include "dlm.h"


#ifndef DLM_AES_ALGORITHM_ENCRYPT
#define DLM_AES_ALGORITHM_ENCRYPT
#endif

#ifdef DLM_AES_ALGORITHM_ENCRYPT
#include "dlm_aes.h"
#else
#include "efuse.h"
#endif

static UCHAR uac_internal_buf[X_INTERNAL_BUFFER_SIZE]; /* this code should run in linux or windows */
static UINT32 ui4_iv;

/*-----------------------------------------------------------------------------
 * Name: x_dlm_generate_hash
 *
 * Description: Compute codefile hash code
 *
 * Inputs:  filename, the file to compute hash code
 * Outputs: digest, the hash result
 * Returns: DLM_OK, compute hash done
 *          DLM_OPEN_FILE_FAILED, open file failed
 ----------------------------------------------------------------------------*/
INT32 x_dlm_generate_hash(CHAR *filename, UCHAR *digest)
{
    INT32 in_file;
    INT32 i4_size;

    if ((in_file = dlm_fopen(filename, kDlm_Read)) == -1)
    {
        x_dbg_stmt("Open file for hash module failed\n");
        return DLM_OPEN_FILE_FAILED;
    }
    dlm_digest_init();

    while (!dlm_feof(in_file))
    {
        i4_size = dlm_fread(in_file, (CHAR*)uac_internal_buf, X_INTERNAL_BUFFER_SIZE);

        if (i4_size != -1)
        {        
            dlm_digest_update(uac_internal_buf,i4_size);
        }
    }

    dlm_digest_final(digest);

    dlm_fclose(in_file);
    return DLM_OK; 
}

#ifndef DLM_AES_ALGORITHM_ENCRYPT
/*-----------------------------------------------------------------------------
 * Name: x_dlm_update_signature
 *
 * Description: This API let build codefile utility to patch its signature
 *
 * Inputs:  file, the file to update signature
 *          p_sig, the signature to fill into file
 * Outputs: NA
 * Returns: DLM_OK, update ok
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_DEL_FILE_FAILED, remove file failed
 *          DLM_RENAME_FILE_FAILED, rename filename failed
 ----------------------------------------------------------------------------*/

INT32 x_dlm_update_signature(CHAR *file, CHAR *p_sig)
{
    INT32 in_file, out_file;
    INT32 i4_cnt = sizeof (DEFAULT_CODE_FILE_HDR_T) - 32;

    if ((in_file = dlm_fopen (file, kDlm_Read)) == -1)
    {
        x_dbg_stmt("Open %s failed\n", file);
        return DLM_OPEN_FILE_FAILED;
    }

    if ((out_file = dlm_fopen ("/mnt/ms/_up_sig.tmp", kDlm_Write)) == -1)
    {
        x_dbg_stmt("open _up_sig.tmp failed\n");
        return DLM_OPEN_FILE_FAILED;
    }

    /* copy common header */
    dlm_fread(in_file, (CHAR*)uac_internal_buf, i4_cnt);
    dlm_fwrite(out_file, (CHAR*)uac_internal_buf, i4_cnt);

    /* reinitialize vector */
    ui4_iv = DLM_APPEND_DATA_IV;

    /* encrypted digest */
    /* dlm_dump_buffer("x_dlm_update_signature key", uac_internal_buf, 4); */
    dlm_encrypt_efuse((UCHAR *)p_sig, 32, *((UINT32 *)uac_internal_buf), &ui4_iv);
    dlm_dump_buffer("Encrypted digest:", (UCHAR*)p_sig, 32);

    /* update digest */
    dlm_fwrite(out_file, (CHAR*)p_sig, 32);
    /* seek space */
    dlm_fread(in_file, (CHAR*) uac_internal_buf, 32);

    /* copy remainder data */
    while (!dlm_feof(in_file))
    {
        i4_cnt = dlm_fread(in_file, (CHAR*)uac_internal_buf, X_INTERNAL_BUFFER_SIZE);
        if (i4_cnt != -1)
        {
            dlm_fwrite(out_file, (CHAR*)uac_internal_buf, i4_cnt);
            x_dbg_stmt(".");
        }          
    }          
    x_dbg_stmt("\n");

    dlm_fclose(in_file);
    dlm_fclose(out_file);

    /* update filename */
    if (dlm_delete_file(file)==-1)
    {
        x_dbg_stmt("Remove file %s failed\n", file);
        return DLM_DEL_FILE_FAILED;
    }

    if (dlm_rename_file("/mnt/ms/_up_sig.tmp", file) == -1)
    {
        x_dbg_stmt("Rename file %s failed\n", file);
        return DLM_RENAME_FILE_FAILED;
    }

    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_add_append_file
 *
 * Description: Add user specify data to codefile
 *
 * Inputs:  pt_append, user defined data
 *          file, the file describe
 *          ui4_key, the key to encrypt data
 * Outputs: NA
 * Returns: DLM_OK, add data to append file ok
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write data failed
 ----------------------------------------------------------------------------*/
INT32 dlm_add_append_file(Append_T *pt_append, INT32 file, UINT32 ui4_key)
{
    INT32 in_file;
    CHAR buf[1024];
    INT32 i4_val_len;

    while (pt_append)
    {
        i4_val_len = dlm_file_size(pt_append->ps_filename);

        /* reinitialize vector */
        ui4_iv = DLM_APPEND_DATA_IV;

        in_file = dlm_fopen (pt_append->ps_filename, kDlm_Read);
        if (in_file == -1)
        {
            x_dbg_stmt("Open append data file %s failed\n", pt_append->ps_filename);
            return DLM_OPEN_FILE_FAILED;
        }

        pt_append->ui4_length = i4_val_len;

        if (dlm_fwrite(file, (CHAR*)pt_append->uc_type, 4) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }

        if (dlm_fwrite(file, (CHAR*)&pt_append->uac_privacy, 4) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }

        if (dlm_fwrite(file, (CHAR*)&(pt_append->ui4_length), 4) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }

        /* copy append data */
        while (!dlm_feof(in_file))
        {
            i4_val_len = dlm_fread(in_file, (CHAR*)buf, 1024);
            if (i4_val_len != -1)
            {
                /* if append data need scramble */
                if (pt_append->uac_privacy[0])
                {
                    dlm_encrypt_efuse((unsigned char *) buf, i4_val_len, ui4_key, &ui4_iv);
                }

                if (dlm_fwrite(file, (CHAR*)buf, i4_val_len) == -1)
                {
                    x_dbg_stmt("Copy append data %s failed\n", pt_append->ps_filename);
                    return DLM_WRITE_DATA_FAILED;
                }
            }
        }

        dlm_fclose (in_file);
        pt_append = pt_append->pt_next;
    }

    return DLM_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_dlm_generate_code_file
 *
 * Description: This API provide user to upload or generate a new code file
 *
 * Inputs:  pt_hdr, the default header
 *          pt_append, specify which data need append to codefile
 *          filename, the filnal filename of codefile
 * Outputs: no
 * Returns: DLM_OK, codefile is generated
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write append data to code file failed
 ----------------------------------------------------------------------------*/
/*
 *   4       64       2      4          32                  32
 * +-----+--------+-------+--------+------------+----------------------------+
 * | ID  |Version | Lenght | Model Name | Encrypted code file digest |
 * +-----+--------+-------+--------+------------+----------------------------+
 * | Append data                                                           |
 * +-----------------------------------------------------------------------+ 
 * | Append data                                                           |
 * +-----------------------------------------------------------------------+ 
 * | ....                                                                  |
 * +-----------------------------------------------------------------------+
 * 
 * Append data header
 *    1    1       4       n
 * +----+-----+--------+---------------------------------------------------+
 * | T  |  P  |    L   |  V                                                |
 * +----+-----+--------+---------------------------------------------------+
 * T: type
 *    255 for end of append data
 * P: privacy
 *    0: value is plain text
 *    1: value is encrypted
 * L: value length
 * V: Append data 
 */
INT32 x_dlm_generate_code_file(DEFAULT_CODE_FILE_HDR_T *pt_user_hdr, Append_T *pt_append, CHAR *filename)
{
    INT32 out_file;
    UINT32 ui4_key = DLM_HDR_KEY;
    Append_T *pt_append_backup = pt_append;
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T*) uac_internal_buf;

    if ((out_file = dlm_fopen (filename, kDlm_Write)) == -1)
    {
        x_dbg_stmt("Open output codefile %s failed\n", filename);
        return DLM_OPEN_FILE_FAILED;
    }

    x_memcpy(pt_hdr, pt_user_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T));

    /* accumulate total size */
    pt_hdr->ui4_total_size = sizeof( DEFAULT_CODE_FILE_HDR_T );
    while (pt_append_backup)
    {
        pt_hdr->ui4_total_size += 12;
        pt_hdr->ui4_total_size += dlm_file_size(pt_append_backup->ps_filename);
        pt_append_backup = pt_append_backup->pt_next;
    }
    x_dbg_stmt("Code size = %u\n", (unsigned int)pt_hdr->ui4_total_size);

    /* story my magic */
    x_memcpy(pt_hdr->s_magic, DLM_HDR_MAGIC, 8);

#if HDR_ENCRYPTED
    ui4_iv = DLM_HDR_IV; /* use different IV */
    /* dlm_dump_buffer("Key", (UCHAR*) pt_hdr->uac_id, 4); */
    dlm_encrypt_efuse((UCHAR *) pt_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32, ui4_key, &ui4_iv);
    dlm_dump_buffer("Encrypted Header:", (UCHAR*) pt_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32);
#endif

    if (dlm_fwrite(out_file, (CHAR*)pt_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T )) == -1)
    {
        dlm_fclose (out_file);
        return DLM_WRITE_DATA_FAILED;
    }

    /* use uac_id as a key */
    if (dlm_add_append_file(pt_append, out_file, *((UINT32*) pt_user_hdr->uac_id)) != 0)
    {
        x_dbg_stmt("Add append data failed\n");
        exit(-1);
    }

    dlm_fclose (out_file);
    return DLM_OK;
} 

#else
/*-----------------------------------------------------------------------------
 * Name: x_dlm_update_signature
 *
 * Description: This API let build codefile utility to patch its signature
 *
 * Inputs:  file, the file to update signature
 *          p_sig, the signature to fill into file
 * Outputs: NA
 * Returns: DLM_OK, update ok
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_DEL_FILE_FAILED, remove file failed
 *          DLM_RENAME_FILE_FAILED, rename filename failed
 ----------------------------------------------------------------------------*/

INT32 x_dlm_update_signature(CHAR *file, CHAR *p_sig)
{
    INT32 in_file, out_file;
    INT32 i4_cnt = sizeof (DEFAULT_CODE_FILE_HDR_T) - 32;
    UINT32 ui4_key = DLM_HDR_KEY;
    UCHAR uc_key[16];
    UCHAR uc_vector[16];
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 2 * sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 3 * sizeof(UINT32), &ui4_key, sizeof(UINT32));

    if ((in_file = dlm_fopen (file, kDlm_Read)) == -1)
    {
        x_dbg_stmt("Open %s failed\n", file);
        return DLM_OPEN_FILE_FAILED;
    }

    if ((out_file = dlm_fopen ("/mnt/ms/_up_sig.tmp", kDlm_Write)) == -1)
    {
        x_dbg_stmt("open _up_sig.tmp failed\n");
        return DLM_OPEN_FILE_FAILED;
    }

    /* copy common header */
    dlm_fread(in_file, (CHAR*)uac_internal_buf, i4_cnt);
    dlm_fwrite(out_file, (CHAR*)uac_internal_buf, i4_cnt);
   
    dlm_dump_buffer("Source digest:", (UCHAR*)p_sig, 32);
    dlm_aes((UCHAR *)p_sig,
            NULL,
            32,
            uc_key,
            uc_vector,
            AES_ENCRYPT);

    dlm_dump_buffer("Encrypted digest:", (UCHAR*)p_sig, 32);

    /* update digest */
    dlm_fwrite(out_file, (CHAR*)p_sig, 32);
    /* seek space */
    dlm_fread(in_file, (CHAR*) uac_internal_buf, 32);

    /* copy remainder data */
    while (!dlm_feof(in_file))
    {
        i4_cnt = dlm_fread(in_file, (CHAR*)uac_internal_buf, X_INTERNAL_BUFFER_SIZE);
        if (i4_cnt != -1)
        {
            dlm_fwrite(out_file, (CHAR*)uac_internal_buf, i4_cnt);
            x_dbg_stmt(".");
        }          
    }          
    x_dbg_stmt("\n");

    dlm_fclose(in_file);
    dlm_fclose(out_file);

    /* update filename */
    if (dlm_delete_file(file)==-1)
    {
        x_dbg_stmt("Remove file %s failed\n", file);
        return DLM_DEL_FILE_FAILED;
    }

    if (dlm_rename_file("/mnt/ms/_up_sig.tmp", file) == -1)
    {
        x_dbg_stmt("Rename file %s failed\n", file);
        return DLM_RENAME_FILE_FAILED;
    }

    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_add_append_file
 *
 * Description: Add user specify data to codefile
 *
 * Inputs:  pt_append, user defined data
 *          file, the file describe
 *          ui4_key, the key to encrypt data
 * Outputs: NA
 * Returns: DLM_OK, add data to append file ok
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write data failed
 ----------------------------------------------------------------------------*/
INT32 dlm_add_append_file(Append_T *pt_append, INT32 file, UINT32 ui4_key)
{
    INT32 in_file;
    CHAR buf[1024];
    INT32 i4_val_len;
    INT32 i4_ret;
    UCHAR uc_digest[32];
    UCHAR uc_magic[16];
    UCHAR uc_key[16];
    UCHAR uc_vector[16];

    while (pt_append)
    {
        x_memset(uc_vector, 0, 16);
        x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));

        x_memset(uc_digest, 0, 32);
        x_memset(uc_magic, 0, 16);
        x_memcpy(uc_magic, DLM_APPEND_MAGIC, 16);
        i4_ret = x_dlm_generate_hash(pt_append->ps_filename,uc_digest);
        if (i4_ret != DLM_OK)
        {
            return i4_ret;
        }
        dlm_dump_buffer("Append Header Key",uc_key,16);
        
        dlm_aes(uc_magic, 
             NULL,
		     16,  
		     uc_key,
		     uc_vector,
		     AES_ENCRYPT);
        dlm_aes(uc_digest, 
             NULL,
		     32,  
		     uc_key,
		     uc_vector,
		     AES_ENCRYPT);
        
        i4_val_len = dlm_file_size(pt_append->ps_filename);
        
        /* reinitialize vector */
        ui4_iv = DLM_APPEND_DATA_IV;

        in_file = dlm_fopen (pt_append->ps_filename, kDlm_Read);
        if (in_file == -1)
        {
            x_dbg_stmt("Open append data file %s failed\n", pt_append->ps_filename);
            return DLM_OPEN_FILE_FAILED;
        }
        
        pt_append->ui4_length = i4_val_len;
            
        if (dlm_fwrite(file, (CHAR*)pt_append->uc_type, 4) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }

        if (dlm_fwrite(file, (CHAR*)&pt_append->uac_privacy, 4) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }

        if (dlm_fwrite(file, (CHAR*)&(pt_append->ui4_length), 4) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }
        if (dlm_fwrite(file, (CHAR *)uc_magic, 16) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }
        
        if (dlm_fwrite(file, (CHAR *)uc_digest, 32) == -1)
        {
            x_dbg_stmt("Write append data %s type failed\n", pt_append->ps_filename);
            return DLM_WRITE_DATA_FAILED;
        }
        /* copy append data */
        while (!dlm_feof(in_file))
        {
            i4_val_len = dlm_fread(in_file, (CHAR*)buf, 1024);
            if (i4_val_len != -1)
            {
                /* if append data need scramble */
                if (pt_append->uac_privacy[0])
                {
                    dlm_aes((unsigned char *)buf, 
                             NULL,
                		     i4_val_len,  
                		     uc_key,
                		     uc_vector,
                		     AES_ENCRYPT);
                    
                }
                
                if (dlm_fwrite(file, (CHAR*)buf, i4_val_len) == -1)
                {
                    x_dbg_stmt("Copy append data %s failed\n", pt_append->ps_filename);
                    return DLM_WRITE_DATA_FAILED;
                }
            }
        }
        dlm_fclose (in_file);
        pt_append = pt_append->pt_next;
    }

    return DLM_OK;
}



/*-----------------------------------------------------------------------------
 * Name: x_dlm_generate_code_file
 *
 * Description: This API provide user to upload or generate a new code file
 *
 * Inputs:  pt_hdr, the default header
 *          pt_append, specify which data need append to codefile
 *          filename, the filnal filename of codefile
 * Outputs: no
 * Returns: DLM_OK, codefile is generated
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write append data to code file failed
 ----------------------------------------------------------------------------*/
/*
 *   4       64       2      4          32                  32
 * +-----+--------+-------+--------+------------+----------------------------+
 * | ID  |Version | Lenght | Model Name | Encrypted code file digest |
 * +-----+--------+-------+--------+------------+----------------------------+
 * | Append data                                                           |
 * +-----------------------------------------------------------------------+ 
 * | Append data                                                           |
 * +-----------------------------------------------------------------------+ 
 * | ....                                                                  |
 * +-----------------------------------------------------------------------+
 * 
 * Append data header
 *    1    1       4       n
 * +----+-----+--------+---------------------------------------------------+
 * | T  |  P  |    L   |  V                                                |
 * +----+-----+--------+---------------------------------------------------+
 * T: type
 *    255 for end of append data
 * P: privacy
 *    0: value is plain text
 *    1: value is encrypted
 * L: value length
 * V: Append data 
 */
INT32 x_dlm_generate_code_file(DEFAULT_CODE_FILE_HDR_T *pt_user_hdr, Append_T *pt_append, CHAR *filename)
{
    INT32 out_file;
    UINT32 ui4_key = DLM_HDR_KEY;
    Append_T *pt_append_backup = pt_append;
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T*) uac_internal_buf;
    UCHAR uc_key[16];
    UCHAR uc_vector[16];
    INT32 i =0;
    
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key,&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32),&ui4_key,sizeof(UINT32));

    if ((out_file = dlm_fopen (filename, kDlm_Write)) == -1)
    {
        x_dbg_stmt("Open output codefile %s failed\n", filename);
        return DLM_OPEN_FILE_FAILED;
    }

    x_memcpy(pt_hdr, pt_user_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T));

    /* accumulate total size */
    pt_hdr->ui4_total_size = sizeof( DEFAULT_CODE_FILE_HDR_T );
    while (pt_append_backup)
    {
        pt_hdr->ui4_total_size += 60;
    
        pt_hdr->ui4_total_size += dlm_file_size(pt_append_backup->ps_filename);
        pt_append_backup = pt_append_backup->pt_next;
    }
    x_dbg_stmt("Code size = %u\n", (unsigned int)pt_hdr->ui4_total_size);

    /* story my magic */
    x_memcpy(pt_hdr->s_magic, DLM_HDR_MAGIC, 8);

#if HDR_ENCRYPTED
    dlm_aes((UCHAR *) pt_hdr,
            NULL,
            sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32,
            uc_key,
            uc_vector,
            AES_ENCRYPT);
    dlm_dump_buffer("Encrypted Header:", (UCHAR*) pt_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32);
#endif

    if (dlm_fwrite(out_file, (CHAR*)pt_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T )) == -1)
    {
        dlm_fclose (out_file);
        return DLM_WRITE_DATA_FAILED;
    }

    /* use uac_id as a key */
    if (dlm_add_append_file(pt_append, out_file, *((UINT32*) pt_user_hdr->uac_id)) != 0)
    {
        x_dbg_stmt("Add append data failed\n");
        exit(-1);
    }

    dlm_fclose (out_file);
    return DLM_OK;
} 

#endif

#if CLI_LVL_ALL

#endif

