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
 * $RCSfile: dlm_api.c,v $
 * $Revision: #4 $
 * $Date: 2012/09/29 $
 * $Author: jiangbo.wang $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_dlm.h"
#include "dlm_fm.h"
#include "x_dbg.h"
#include "dlm_digest.h"
#include "dlm_util.h"
#include "dlm.h"

#ifndef DLM_AES_ALGORITHM_ENCRYPT
#include "efuse.h"
#else
#include "dlm_aes.h"
#endif

#ifdef NO_MIDDLEWARE
    #ifdef CC_MTK_LOADER
        #include "x_stl_lib.h"
    #endif   
#else
    #include "start/x_version.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static dlm_hdr_parse_fn         dlm_header_parse_fn = NULL;
static dlm_progress_notify_fn   dlm_notify_progress = NULL;
static dlm_progress_notify_fn   dlm_notify_tag_progress = NULL;
static dlm_ver_chk_fn           dlm_ver_compare_fn = NULL;
static DLM_APPEND_BLOCK_T      *pt_append_data_callback_list = NULL;
static DEFAULT_CODE_FILE_HDR_T *pt_myHdr = NULL, *pt_newHdr = NULL;
static UCHAR                   *puac_internal_buf = NULL;
static UINT32                   ui4_internal_buf_size = X_INTERNAL_BUFFER_SIZE;
static UCHAR                    uc_g_flag = 0; /* super flag to disable integgrity check */
INT32                           i4_codefile_header_offset = 0;
#ifndef DLM_AES_ALGORITHM_ENCRYPT
static UINT32                   ui4_iv;
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/* Append Data Blocl Memory allocate API */
static DLM_APPEND_BLOCK_T* dlm_parser_new(VOID);

/* Internal Code File Header integrity check API */
static INT32  dlm_validate_id(UCHAR *pc_hdr);
static INT32  dlm_validate_version(UCHAR *pc_hdr); 
static INT32  dlm_validate_model_name(UCHAR *pc_hdr); 
static INT32  dlm_validate_digest(CHAR *ps_file, DLM_IO_T *pt_user_io);
static VOID   dlm_backup_download_hdr(VOID);

static VOID dlm_free_append_buffer(DLM_APPEND_BLOCK_T *pt_append_data_block);
/* Dispatch Append Data to proper own */
static DLM_APPEND_BLOCK_T *dlm_find_append_data_type(Append_DATA_TLV_T *pt_append_data_tlv);

static INT32 dlm_read_code_header(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_size);
INT32 dlm_locate_codefile_header(CHAR *filename, DLM_IO_T *pt_user_io);

#ifdef NO_MIDDLEWARE
#else
INT32 dlm_cli_init( VOID );
#endif

/*-----------------------------------------------------------------------------
 * Name: x_dlm_init
 *
 * Description: The Api to initialize download module
 *
 * Inputs:  NA
 * Outputs: NA
 * Returns: DLM_OK, ok
 *          DLM_ALLOCATE_MEMORY_FAILED, failed
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_DLM_API
INT32 x_dlm_init(VOID) 
{
    /* allocate internal buffer */
    puac_internal_buf = x_mem_alloc(ui4_internal_buf_size);
    if ( puac_internal_buf == NULL )
    {
        return DLM_ALLOCATE_MEMORY_FAILED;
    }

    pt_myHdr = (DEFAULT_CODE_FILE_HDR_T *) x_mem_alloc(sizeof(DEFAULT_CODE_FILE_HDR_T));
    if (pt_myHdr == NULL)
    {
        return DLM_ALLOCATE_MEMORY_FAILED;
    }

    x_memset(pt_myHdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T));

#ifdef NO_MIDDLEWARE
#else
    /* Customer Name */
    if ( x_strlen(x_sys_get_customer()) >= 4 )
        x_memcpy(pt_myHdr->uac_id, x_sys_get_customer(), (SIZE_T)4);
    else
        x_strcpy((CHAR*)pt_myHdr->uac_id, x_sys_get_customer()); 

    /* Version */
    x_strcpy(pt_myHdr->s_version, x_sys_get_version());

    /* model name */
    x_strcpy((CHAR*)pt_myHdr->s_model, x_sys_get_model_name());

    /* Header Magic */
    x_memcpy(pt_myHdr->s_magic, DLM_HDR_MAGIC, (SIZE_T)8);

#ifdef CLI_LVL_ALL
    dlm_cli_init();
#endif /* CLI_LVL_ALL */
#endif    

#if 0  /* for linux tool testing */   
    /* Module test */
    /* Customer Name */
    x_strcpy(pt_myHdr->uac_id, "Phil"); 

    /* Version */
    x_strcpy(pt_myHdr->s_version, "CQAM_IDTV_001005_1");

    /* model name */
    x_strcpy(pt_myHdr->s_model, "mt5381_demo");

    /* Header Magic */
    x_memcpy(pt_myHdr->s_magic, DLM_HDR_MAGIC, (SIZE_T)8);
#endif

    return DLM_OK;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_dlm_boot_init
 *
 * Description: The Api to initialize download module
 *
 * Inputs:  pac_hdr,   point to hdr buffer
 * Outputs: NA
 * Returns: DLM_OK
 ----------------------------------------------------------------------------*/
INT32 x_dlm_boot_init(CHAR *pac_hdr)
{
    /* allocate internal buffer */
    puac_internal_buf = x_mem_alloc(ui4_internal_buf_size);
    if ( puac_internal_buf == NULL )
    {
        return DLM_ALLOCATE_MEMORY_FAILED;
    }

    pt_myHdr = (DEFAULT_CODE_FILE_HDR_T *) x_mem_alloc(sizeof(DEFAULT_CODE_FILE_HDR_T));
    if (pt_myHdr == NULL)
    {
        return DLM_ALLOCATE_MEMORY_FAILED;
    }
    x_memset(pt_myHdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T));
    x_memcpy(pt_myHdr, pac_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T));

	/*Workaround start*/
	//For Vestel Encrypted binary(CI+).
	//Due to loader can't get the encrypted key and upgrade failed, set the key here.
	//The key is according to upg_pkg.mak CUSTOMER defined in Makefile (vestel_linux).
	x_memcpy(pt_myHdr->uac_id,"vest",4);
	/*Workaround end*/

    return DLM_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_dlm_set_buffer
 *
 * Description: The Api to change dlm internal buffer size
 *
 * Inputs:  ui4_k_bytes, unit is k Bytes.
 * Outputs: NA
 * Returns: DLM_OK, ok
 *          DLM_ALLOCATE_MEMORY_FAILED, failed
 ----------------------------------------------------------------------------*/
INT32 x_dlm_set_buffer(UINT32 ui4_k_bytes)
{
    UINT32 ui4_bytes = ui4_k_bytes * 1024;

    if ( puac_internal_buf )
    {
        x_mem_free(puac_internal_buf);
    }

    puac_internal_buf = x_mem_alloc(ui4_bytes);
    if ( puac_internal_buf == NULL )
    {
        DLM_TRACE(("Allocate DLM internal buffer failed\n"));
        return DLM_ALLOCATE_MEMORY_FAILED;
    }

    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_get_download_hdr
 *
 * Description: The Api return download file header if have
 *
 * Inputs:  NA
 * Outputs: NA
 * Returns: NULL or point to download header
 ----------------------------------------------------------------------------*/
CHAR *x_dlm_get_download_hdr(VOID)
{
    if ( pt_newHdr )
    {
        return(CHAR *) pt_newHdr;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_release
 *
 * Description: Release DLM allocate buffer
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_DLM_API
VOID x_dlm_release(VOID)
{
    DLM_APPEND_BLOCK_T *pt_entry =  pt_append_data_callback_list;

    while ( pt_entry )
    {
        pt_append_data_callback_list = pt_entry->pt_next;
        x_mem_free (pt_entry);
        pt_entry = pt_append_data_callback_list;
    }

    if ( pt_myHdr )
    {
        x_mem_free(pt_myHdr);
    }

    if ( pt_newHdr )
    {
        x_mem_free(pt_newHdr);
    }
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_dlm_register_progress_notify
 *
 * Description: user may register this api to monitor download or upload progress
 *
 * Inputs:  
 *              fn, user progress notify function
 * Outputs:     
 * Returns:     DLM_OK 
 ----------------------------------------------------------------------------*/
INT32 x_dlm_register_progress_notify(dlm_progress_notify_fn fn)
{
    dlm_notify_progress = fn;

    return DLM_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_dlm_register_tag_progress_notify
 *
 * Description: user may register this api to monitor current tag download or upload progress
 *
 * Inputs:  
 *              fn, user progress notify function
 * Outputs:     
 * Returns:     DLM_OK 
 ----------------------------------------------------------------------------*/
INT32 x_dlm_register_tag_progress_notify(dlm_progress_notify_fn fn)
{
    dlm_notify_tag_progress = fn;

    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_reg_hdr_callback
 *
 * Description: User may replace our default file header, in case they need 
 *              use their API to check file header, hence this API provide user 
 *              to register their check API.
 *
 * Inputs:  
 *              fn, user file header check API
 *              ui4_hdr_len, user define file header size
 * Outputs:     
 * Returns:     DLM_OK 
 ----------------------------------------------------------------------------*/
INT32  x_dlm_reg_hdr_callback(dlm_hdr_parse_fn fn, UINT32 ui4_hdr_len)
{
    dlm_header_parse_fn = fn;
    return DLM_OK;
}

INT32 x_dlm_reg_append_callback2(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn2 fn)
{
    DLM_APPEND_BLOCK_T* pt_new;

    /* lookup current list and check is there any tag duplicated */
    /* lookup current list and check is there any API duplicated */
    pt_new = pt_append_data_callback_list;
    while ( pt_new )
    {
        if ( x_memcmp(pt_new->uc_type, pc_type, 4) ==0 )
        {
            return DLM_DUPLICATE_TAG;
        }
#if defined(DLM_EXCLUDE_SAME_TAG_CALLBACK)
        if ( pt_new->pf_callback2 == fn )
        {
            return DLM_DUPLICATE_API;
        }
#endif
        pt_new = pt_new->pt_next;
    }

    /* sanity check 1 */
    if ( (pt_new = dlm_parser_new()) == NULL )
        return DLM_ALLOCATE_MEMORY_FAILED;

    /*
        sanity check 2
        if pc_type did not contain 4 bytes....
        To save code size, I think this rule should define in design spec.
    */

    x_memcpy(pt_new->uc_type, pc_type, 4);
    pt_new->pauc_buffer = NULL;
    pt_new->ui4_buf_len = ui4_buf_size;
    pt_new->pf_callback2 = fn;
    pt_new->pf_callback = NULL;

    if ( pt_append_data_callback_list )
        pt_new->pt_next = pt_append_data_callback_list;

    pt_append_data_callback_list = pt_new;

    return DLM_OK;
}
/* } */

/*-----------------------------------------------------------------------------
 * Name: x_dlm_reg_append_callback
 *
 * Description: The codefile from contain many user define data, they may firmware
 *              or other internal data, when donwload process is running it will 
 *              dispath user defined data to each user, hence, user "MUST" provide
 *              their own API to process this data.
 *
 * Inputs:  pc_type, 4 bytes tag, user defined, this tag "MUST" same as build code 
 *                   file
 *          ui4_buf_size, buffer size for this data
 *          fn,  user defined callback
 *               
 * Outputs: 
 * Returns: DLM_OK, if there did not exsit any failed
 *          DLM_DUPLICATE_TAG, the tag already in use
 *          DLM_DUPLICATE_API, the API already in use
 ----------------------------------------------------------------------------*/
INT32 x_dlm_reg_append_callback(UCHAR *pc_type, UINT32 ui4_buf_size, dlm_append_data_fn fn)
{
    DLM_APPEND_BLOCK_T* pt_new;

    /* lookup current list and check is there any tag duplicated */
    /* lookup current list and check is there any API duplicated */
    pt_new = pt_append_data_callback_list;
    while ( pt_new )
    {
        if ( x_memcmp(pt_new->uc_type, pc_type, 4) ==0 )
        {
            return DLM_DUPLICATE_TAG;
        }
#if defined(DLM_EXCLUDE_SAME_TAG_CALLBACK)
        if ( pt_new->pf_callback == fn )
        {
            return DLM_DUPLICATE_API;
        }
#endif
        pt_new = pt_new->pt_next;
    }

    /* sanity check 1 */
    if ( (pt_new = dlm_parser_new()) == NULL )
        return DLM_ALLOCATE_MEMORY_FAILED;

    /* 
        sanity check 2 
        if pc_type did not contain 4 bytes.... 
        To save code size, I think this rule should define in design spec.
    */

    x_memcpy(pt_new->uc_type, pc_type, (SIZE_T)4);
    pt_new->pauc_buffer = NULL;
    pt_new->ui4_buf_len = ui4_buf_size;
    pt_new->pf_callback = fn;
    pt_new->pf_callback2 = NULL;

    if ( pt_append_data_callback_list )
        pt_new->pt_next = pt_append_data_callback_list;

    pt_append_data_callback_list = pt_new;

    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_backup_download_hdr
 *
 * Description: This API will check file integrity as user defined
 *
 * Inputs:  filename
 * Outputs: no
 * Returns: DLM_OK check ok
 *          others failed
 ----------------------------------------------------------------------------*/
static VOID dlm_backup_download_hdr(VOID)
{
    /* header is store in our default buffer */
    if ( pt_newHdr == NULL )
    {
        pt_newHdr = (DEFAULT_CODE_FILE_HDR_T *) x_mem_alloc(sizeof(DEFAULT_CODE_FILE_HDR_T));
        if (pt_newHdr == NULL)
        {
            return;
        }
        x_memset(pt_newHdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T));
    }

    x_memcpy(pt_newHdr, puac_internal_buf + i4_codefile_header_offset , sizeof(DEFAULT_CODE_FILE_HDR_T));
}

#ifndef DLM_AES_ALGORITHM_ENCRYPT
/*-----------------------------------------------------------------------------
 * Name: x_dlm_integrity_check
 *
 * Description: This API will check file integrity as user defined
 *
 * Inputs:  filename
 * Outputs: no
 * Returns: DLM_OK check ok
 *          others failed
 ----------------------------------------------------------------------------*/

INT32 x_dlm_integrity_check(CHAR *filename, DLM_IO_T *pt_user_io)
{
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)puac_internal_buf;
    INT32 i4_file, i4_ret;
    UINT32 ui4_key = DLM_HDR_KEY;
    DLM_IO_T *pt_io = pt_user_io;

    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    if ( dlm_header_parse_fn )
    {
        DLM_TRACE(("Vendor has define its own integrity check API\n"));
        return dlm_header_parse_fn(filename);
    }
    else
    {
        DLM_TRACE(("Use internal integrity check API\n"));

        /* locate header */
        i4_ret = dlm_locate_codefile_header(filename, pt_io);
        if ( i4_ret != DLM_OK )
        {
            DLM_TRACE(("Locate codefile header failed - %s\n", filename));
            return i4_ret;
        }

        if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
        {
            DLM_TRACE(("Open file %s for integrity check failed\n", filename));
            return DLM_OPEN_FILE_FAILED;
        }

        i4_ret = pt_io->read_data(i4_file, (CHAR *)pt_file_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset);
        if ( i4_ret < (INT32) sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset || 
             i4_ret == -1 )
        {
            DLM_TRACE(("Read file %s header failed\n", filename));
            pt_io->close_source(i4_file);
            return DLM_READ_FILE_FAILED;
        }
        pt_io->close_source (i4_file);

#if HDR_ENCRYPTED
        pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)(puac_internal_buf + i4_codefile_header_offset);

        dlm_dump_buffer("Encrypted header:", (UCHAR*)pt_file_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32);
        ui4_iv = DLM_HDR_IV; /* use different IV */
        dlm_decrypt_efuse(puac_internal_buf + i4_codefile_header_offset, (UINT32) (sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32), ui4_key, &ui4_iv);
#endif
        dlm_dump_buffer("Header:", (UCHAR*)pt_file_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32);


        dlm_backup_download_hdr();

        /* 
         * verify control flag 
         * Current define 
         *    0x00000000            (only digest is verified)
         *    0x10000000           verify ID
         *    0x20000000           verify version
         *    0x40000000           verify Model Name
        */
        if ( pt_file_hdr->uac_control[0] & kValidateID )
        {
            if ( dlm_validate_id(puac_internal_buf + i4_codefile_header_offset) != DLM_OK )
            {
                return DLM_ID_NOT_MATCH;
            }
        }

        if ( pt_file_hdr->uac_control[0] & kValidateModelName )
        {
            if ( dlm_validate_model_name(puac_internal_buf + i4_codefile_header_offset) != DLM_OK )
            {
                return DLM_MODEL_NAME_NOT_MATCH;
            }
        }

        if ( pt_file_hdr->uac_control[0] & kValidateVersion )
        {
            if ( dlm_validate_version(puac_internal_buf + i4_codefile_header_offset) != DLM_OK )
            {
                return DLM_VERSION_NOT_MATCH;
            }
        }

        /* validate digest */
        return dlm_validate_digest(filename, pt_io);
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_download
 *
 * Description: User can use this API to download codefile from USB flash
 *
 * Inputs:  filename, code file name
 * Outputs: NA
 * Returns: DLM_OK, download finished
 ----------------------------------------------------------------------------*/
INT32 x_dlm_download(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_check)
{
    INT32 i4_file;
    DLM_APPEND_BLOCK_T *pt_append_data_block = NULL;
    Append_DATA_TLV_T   t_append_data_tlv; 
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;

    INT32       i4_append_len = 0;
    INT32       i4_buffer_len = 0;
    INT32       i4_append_buf_remain = 0;
    INT32       i4_append_buf_content_size = 0;
    UCHAR      *p_buf_offset = NULL;
    UCHAR      *p_app_offset = NULL;
    INT32       i4_ret;
    DLM_IO_T*   pt_io = pt_user_io;
    UINT32      ui4_file_size = 0;
    UINT32      ui4_consume_size = 0;
    UINT32      ui4_percentage = 0;
    INT32       i4_flag;
    INT32       i4_adjust = 0;
	INT32       i4_decrypted_len = 0;

    /* append data may has extra hdr */
    INT32 i4_extra_app_hdr_len = 0;

    if ( pt_io == NULL )
        pt_io = x_dlm_get_default_io();

    /* locate header */
    i4_ret = dlm_locate_codefile_header(filename, pt_user_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", filename));
        return i4_ret;
    }

    /* open source */
    if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
    {
        DLM_TRACE(("Open download file %s failed\n", filename));
        return DLM_OPEN_FILE_FAILED;
    }

    /* Read code file header from storage */
    i4_ret = pt_io->read_data(i4_file, (CHAR *)puac_internal_buf, sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset);

    /* if no data or size not equal to code file header size, something wrong!!! */
    if ( i4_ret == -1 || i4_ret < (INT32) sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset )
    {
        pt_io->close_source (i4_file);
        return DLM_READ_FILE_FAILED;
    }
#if HDR_ENCRYPTED
    pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf + i4_codefile_header_offset);
    ui4_iv = DLM_HDR_IV;
    dlm_decrypt_efuse(puac_internal_buf + i4_codefile_header_offset, (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32), DLM_HDR_KEY, &ui4_iv);
#endif

    ui4_file_size = pt_hdr->ui4_total_size;

    /* backup download hdr for future use */
    dlm_backup_download_hdr();

    ui4_consume_size = sizeof(DEFAULT_CODE_FILE_HDR_T);

    /**
     * main loop to iterator codefile tag
     * i4_buffer_len: unread buffer size in current block
     * i4_append_len: the counter to accumulate how many append data
     * is unread.
     */
    while ( !pt_io->eof_data(i4_file) || i4_buffer_len || i4_append_len )
    {
        /* renew buffer */
        if ( i4_buffer_len == 0 )
        {
            /* the counter to adjust i4_buffer_len if read data block is > i4_buffer_len */
            i4_adjust = 0;

            i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);

            /* error check here */
            if ( i4_buffer_len == -1 )
            {
                /* free buffer */
                dlm_free_append_buffer(pt_append_data_block);
                pt_io->close_source (i4_file);
                return DLM_READ_FILE_FAILED;
            }

            /* try again, our FM has issue before, left this code to make sure the file is end */
            if ( i4_buffer_len == 0 )
            {
                continue;
            }

            /* ui4_consume_size, the counter to record how many bytes is used */
            ui4_consume_size += i4_buffer_len;

            /* notify progress */
            ui4_percentage = (UINT32)(ui4_consume_size * 100/ui4_file_size);

            if ( dlm_notify_progress )
                dlm_notify_progress((INT32)ui4_percentage);
#if 0 /* for lint happy */               
            else
                ;/*DLM_TRACE(("Progress %u, %u = %d\n", ui4_consume_size, ui4_file_size, ui4_percentage));*/
#endif
            /* let ring buffer start again */
            p_buf_offset = puac_internal_buf;
        }

        /* start new append data block */
        if ( i4_append_len == 0 )
        {
            CHAR *pac_tlv = (CHAR *) &t_append_data_tlv;

            i4_flag = 1;

            /* 
             * if remain data is less than append data header size (12), 
             * need require more data...
             */
            if ( i4_buffer_len < 12 )
            {
                i4_adjust = i4_buffer_len;

                /* consume remain data from internal buffer */
                x_memcpy(pac_tlv, p_buf_offset, (SIZE_T)i4_adjust);
                ui4_consume_size += i4_adjust;

                /* read new data from storage */
                i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);

                if ( i4_buffer_len )
                {
                    /* append remain data */
                    INT32 i4_offset = 12 - i4_adjust;

                    p_buf_offset = puac_internal_buf;
                    x_memcpy(pac_tlv + i4_adjust, p_buf_offset, (SIZE_T)i4_offset);
                    i4_adjust = i4_offset;

                    ui4_consume_size += i4_buffer_len;

                    /* notify progress */
                    ui4_percentage = (UINT32)(ui4_consume_size * 100/ui4_file_size);

                    if ( dlm_notify_progress )
                        dlm_notify_progress((INT32)ui4_percentage);
#if 0 /* for lint happy */                        
                    else
                        ;/*DLM_TRACE(("Progress %u, %u = %d\n", ui4_consume_size, ui4_file_size, ui4_percentage));*/
#endif                        
                }
                else
                {
                    /* free buffer */
                    dlm_free_append_buffer(pt_append_data_block);
                    pt_io->close_source (i4_file);
                    return DLM_READ_FILE_FAILED;
                }
            }
            else
            {
                x_memcpy(&t_append_data_tlv, p_buf_offset, (SIZE_T) 12);
                i4_adjust = 0;
            }

            /* reinitialize vector */
            ui4_iv = DLM_APPEND_DATA_IV;

            i4_append_len = t_append_data_tlv.uc_length[3] * 256 * 256 * 256 + 
                            t_append_data_tlv.uc_length[2] * 256 * 256 + 
                            t_append_data_tlv.uc_length[1] * 256 + 
                            t_append_data_tlv.uc_length[0];
			
			i4_decrypted_len = 0;

            dlm_dump_buffer("Append", p_buf_offset, 64);

            /* free allocate buffer */
            dlm_free_append_buffer(pt_append_data_block);

            pt_append_data_block = dlm_find_append_data_type(&t_append_data_tlv);

            if ( pt_append_data_block )
            {
                i4_append_buf_remain = (INT32) pt_append_data_block->ui4_buf_len;
                pt_append_data_block->pauc_buffer = (UCHAR *)x_mem_alloc (pt_append_data_block->ui4_buf_len);
                if ( pt_append_data_block->pauc_buffer == NULL )
                {
                    DLM_TRACE(("Allocate buffer for append data failed\n"));
                    pt_io->close_source (i4_file);
                    return DLM_ALLOCATE_MEMORY_FAILED;
                }
                p_app_offset = pt_append_data_block->pauc_buffer;

            }
            else
            {
                i4_append_buf_remain = 0;
                p_app_offset = NULL;
            }

            if ( i4_adjust )
            {
                i4_buffer_len -= i4_adjust;
                p_buf_offset += i4_adjust;
                i4_adjust = 0;
            }
            else
            {
                i4_buffer_len -= 12; /* append data header */
                p_buf_offset += 12;
            }

            /* if there exist extra hdr */
            /**************************************
             * check if APPEND data has extra HDR *
             * the hdr is start with magic "iMtK",*
             * this magic is defned in dlm.h      *
             **************************************/
            if ( i4_flag )
            {
                UCHAR uac_ahdr[8];
                UCHAR uac_ahdr_back[8];
                DLM_TRACE(("\nProcess tag \"%c%c%c%c\" length = %d, %02x.%02x.%02x.%02x\n",
                           (CHAR)t_append_data_tlv.uc_type[0],
                           (CHAR)t_append_data_tlv.uc_type[1], 
                           (CHAR)t_append_data_tlv.uc_type[2], 
                           (CHAR)t_append_data_tlv.uc_type[3],  
                           (int)i4_append_len, 
                           (int)t_append_data_tlv.uc_length[3],
                           (int)t_append_data_tlv.uc_length[2],
                           (int)t_append_data_tlv.uc_length[1],
                           (int)t_append_data_tlv.uc_length[0]));

                if ( i4_buffer_len < 8 )
                {
                    /** move remain data to dest */
                    /* printf("i4_buffer_len < 8\n"); */
                    x_memcpy(uac_ahdr, p_buf_offset, (SIZE_T)i4_buffer_len);
                    i4_adjust = i4_buffer_len;
                    ui4_consume_size += i4_adjust;
                    i4_append_len -= i4_adjust; 

                    /* read new data from storage */
                    i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);

                    if ( i4_buffer_len )
                    {
                        /* append remain data */
                        INT32 i4_offset = 8 - i4_adjust;

                        p_buf_offset = puac_internal_buf;
                        if (i4_offset < 8 && i4_offset > 0)
                        {
                            x_memcpy(uac_ahdr + i4_adjust, p_buf_offset, (SIZE_T)i4_offset);
                        }
                        p_buf_offset += i4_offset;
                        i4_buffer_len -= i4_offset;
                        
                        i4_append_len -= i4_offset; 

                        ui4_consume_size += i4_buffer_len;
                        /* notify progress */
                        ui4_percentage = (UINT32)(ui4_consume_size * 100/ui4_file_size);

                        if ( dlm_notify_progress )
                            dlm_notify_progress((INT32)ui4_percentage);
#if 0 /* make lint happy */                            
                        else
                            ;/*DLM_TRACE(("Progress %u, %u = %d\n", ui4_consume_size, ui4_file_size, ui4_percentage));*/
#endif
                        i4_adjust = 0;
                    }
                    else
                    {
                        /* free buffer */
                        dlm_free_append_buffer(pt_append_data_block);
                        pt_io->close_source (i4_file);
                        return DLM_READ_FILE_FAILED;
                    }
                }
                else
                {
                    x_memcpy(uac_ahdr, p_buf_offset, (SIZE_T)8);
                    i4_buffer_len -= 8; /* append data header */
                    p_buf_offset += 8;
                    i4_append_len -= 8;
                    i4_adjust = 0;
                }
				x_memcpy(uac_ahdr_back, uac_ahdr, 8);
                if ( t_append_data_tlv.uc_privacy[0] )
                {
                    dlm_dump_buffer("Encrypted Append Hdr ", uac_ahdr, 8);
                    
                    ui4_iv = DLM_APPEND_DATA_IV;
#if 0 /* dgb info */
                    DLM_TRACE(("\nKey = %c%c%c%c, %x, Iv = %x\n", 
                               pt_myHdr->uac_id[0],
                               pt_myHdr->uac_id[1],
                               pt_myHdr->uac_id[2],
                               pt_myHdr->uac_id[3],
                               *((UINT32*)pt_myHdr->uac_id),
                               ui4_iv));
#endif                        
                    dlm_decrypt_efuse(uac_ahdr, 8, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
                    dlm_dump_buffer("Decrypted Append Hdr ", uac_ahdr, 8);
					i4_decrypted_len += 8;
                }

                if ( x_memcmp(uac_ahdr, DLM_APPEND_HDR_MAGIC, 4) == 0 )
                {
                    i4_flag = 0; /* each append data only check once */
                    /* the folllowing expresion is base on little endian */
                    i4_extra_app_hdr_len = uac_ahdr[4] + uac_ahdr[5] * 256 + uac_ahdr[6] * 256 * 256 + uac_ahdr[7] * 256 * 256 * 256 ;

                    DLM_TRACE(("    Real extra_app_hdr_len = %d\n", i4_extra_app_hdr_len));
                    /* adjust i4_extra_app_hdr_len len to 4 bytes alignment */
                    if ( i4_extra_app_hdr_len % 4 )
                        i4_extra_app_hdr_len += (4 - (i4_extra_app_hdr_len %4));

                    DLM_TRACE(("    Adjust extra_app_hdr_len = %d\n", i4_extra_app_hdr_len));
                }
                else
                {
                    ui4_iv = DLM_APPEND_DATA_IV;
                    i4_extra_app_hdr_len = 0;

                    if (p_app_offset)
                    {
                        x_memcpy(p_app_offset, uac_ahdr_back, (SIZE_T) 8);
                        i4_append_buf_content_size += 8;
                        i4_append_buf_remain -= 8;
						p_app_offset += 8;
                    }
                }
            }
        }

        if ( i4_buffer_len <= 0 )
        {
            /* read data from storage */
            i4_buffer_len = 0;
            continue;
        }

        if ( i4_extra_app_hdr_len )
        {
            /* skip this hdr */
            if ( i4_buffer_len > i4_extra_app_hdr_len )
            {
                /* to refresh IV, we need exec this no matter the data is need or not */
                if ( t_append_data_tlv.uc_privacy[0] )
                {
                	INT32 i4_ssss = 0;
                	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
						if (i4_extra_app_hdr_len > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
							i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
						}else {
							i4_ssss = i4_extra_app_hdr_len;
						}
					}
					if (i4_ssss > 0){
	                    dlm_decrypt_efuse(p_buf_offset, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
						i4_decrypted_len += i4_ssss;
						//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);
					    //dlm_dump_buffer("data ", p_buf_offset, i4_decrypted_len);
						
					}
                }
                i4_buffer_len -= i4_extra_app_hdr_len;
                p_buf_offset += i4_extra_app_hdr_len;
                i4_append_len -= i4_extra_app_hdr_len;
            }
            else
            {
                /* to refresh IV, we need exec this no matter the data is need or not */
                UCHAR* puc_extra_app_hdr_buffer = NULL;  
                INT32  i4_adjust_app_hdr = 0;              
                puc_extra_app_hdr_buffer = (UCHAR*)x_mem_alloc((SIZE_T)(i4_extra_app_hdr_len * sizeof(UCHAR))); 
                if (puc_extra_app_hdr_buffer == NULL)                
                {                    
                    DLM_TRACE(("Allocate buffer for append data hdr failed\n"));                  
                    dlm_free_append_buffer(pt_append_data_block);             
                    pt_io->close_source (i4_file);                  
                    return DLM_ALLOCATE_MEMORY_FAILED;           
                }                         
                x_memcpy(puc_extra_app_hdr_buffer, p_buf_offset, (SIZE_T)i4_buffer_len);   
                i4_adjust_app_hdr = i4_buffer_len;             
                ui4_consume_size += i4_adjust_app_hdr;            
                i4_append_len -= i4_adjust_app_hdr;            
                i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);  
                if (i4_buffer_len)          
                {                 
                    INT32 i4_offset_app_hdr = 0;               
                    i4_offset_app_hdr = i4_extra_app_hdr_len - i4_adjust_app_hdr;       
                    p_buf_offset = puac_internal_buf;          
                    x_memcpy(puc_extra_app_hdr_buffer + i4_adjust_app_hdr, p_buf_offset, (SIZE_T)i4_offset_app_hdr);
                    p_buf_offset += i4_offset_app_hdr;               
                    ui4_consume_size += (UINT32)i4_offset_app_hdr;           
                    i4_append_len -= i4_offset_app_hdr;             
                    i4_buffer_len -= i4_offset_app_hdr;         
                }                
                else              
                {                    
                    /* free buffer */       
                    x_mem_free(puc_extra_app_hdr_buffer);            
                    dlm_free_append_buffer(pt_append_data_block);   
                    pt_io->close_source (i4_file);                  
                    return DLM_READ_FILE_FAILED;              
                }                               
                /* to refresh IV, we need exec this no matter the data is need or not */             
                if ( t_append_data_tlv.uc_privacy[0] )          
                {
                	INT32 i4_ssss = 0;
                	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
						if (i4_extra_app_hdr_len > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
							i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
						}else {
							i4_ssss = i4_extra_app_hdr_len;
						}
					}
					if (i4_ssss > 0){
	                    dlm_decrypt_efuse(puc_extra_app_hdr_buffer, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
						i4_decrypted_len += i4_ssss;
						//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);
						//dlm_dump_buffer("data ", puc_extra_app_hdr_buffer, i4_decrypted_len);
					}
                }
                x_mem_free(puc_extra_app_hdr_buffer);  
            }
            i4_extra_app_hdr_len = 0;
            continue;
        }

        /* this append data does not have call back */
        if ( !pt_append_data_block )
        {
#if 0            
            DLM_TRACE(("No append data call back registed, skip this type %c%c%c%c\n", 
                       t_append_data_tlv.uc_type[0],
                       t_append_data_tlv.uc_type[1], 
                       t_append_data_tlv.uc_type[2], 
                       t_append_data_tlv.uc_type[3]));

            DLM_TRACE(("i4_buffer_len = %u, i4_append_len = %u\n", (unsigned int)i4_buffer_len, (unsigned int)i4_append_len));
#endif
            if ( i4_append_len > i4_buffer_len )
            {
                i4_append_len -= i4_buffer_len;
                i4_buffer_len = 0;
            }
            else
            {
                i4_buffer_len -= i4_append_len;
                p_buf_offset += i4_append_len;
                i4_append_len = 0;
            }
        }
        else
        {
            /* dispath this data to append data call back */
            
            if ( i4_append_buf_remain )
            {
                if ( i4_append_len > i4_append_buf_remain )
                {
                    /* more space need */
                    if ( i4_buffer_len >= i4_append_buf_remain )
                    {
                        /* fill whole remain space */
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T) i4_append_buf_remain);
                        p_buf_offset += i4_append_buf_remain;
                        i4_append_len -= i4_append_buf_remain;
                        i4_buffer_len -= i4_append_buf_remain;
                        i4_append_buf_content_size += i4_append_buf_remain;
                        i4_append_buf_remain = 0;
                    }
                    else
                    {
                        /* i4_buffer_len < i4_append_buf_remain */
                        /* move as much as possible */
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_buffer_len);
                        p_app_offset += i4_buffer_len;
                        i4_append_buf_remain -= i4_buffer_len;
                        i4_append_len -= i4_buffer_len;
                        i4_append_buf_content_size += i4_buffer_len;
                        i4_buffer_len = 0;
                    }
                }
                else
                {
                    /* i4_append_len < i4_append_buf_remain */
                    /* this is last apppend data in this block */
                    if ( i4_buffer_len >= i4_append_len )
                    {
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_append_len);
                        p_buf_offset += i4_append_len;
                        i4_buffer_len -= i4_append_len;
                        i4_append_buf_content_size += i4_append_len;
                        i4_append_len = 0;
                        i4_append_buf_remain = 0;
                    }
                    else
                    {
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T) i4_buffer_len);
                        p_app_offset += i4_buffer_len;
                        i4_append_len -= i4_buffer_len;
                        i4_append_buf_remain -= i4_buffer_len;
                        i4_append_buf_content_size += i4_buffer_len;
                        i4_buffer_len = 0;
                    }
                }
            }

            /* Turn on this to trace data status */
#if 0
            DLM_TRACE(("app_len = %u, buf_len = %u, appbuf_remain = %u, appbuf_content_size = %u\n", 
                       i4_append_len, 
                       i4_buffer_len, 
                       i4_append_buf_remain,
                       i4_append_buf_content_size));
#endif

            if ( i4_append_buf_remain == 0 )
            {
                /* if append data is scrambled then data buffer need decrypted */
                if ( t_append_data_tlv.uc_privacy[0] )
                {
                	INT32 i4_ssss = 0;
                	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
						if (i4_append_buf_content_size > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
							i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
						}else {
							i4_ssss = i4_append_buf_content_size;
						}
					}
					if (i4_ssss > 0){
	                    dlm_decrypt_efuse(pt_append_data_block->pauc_buffer, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
						i4_decrypted_len += i4_ssss;
						//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);

						//dlm_dump_buffer("data ", pt_append_data_block->pauc_buffer, i4_decrypted_len);
					}
                }
                if ( pt_append_data_block->pf_callback != NULL )
                {
                    i4_ret = pt_append_data_block->pf_callback(DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
                }
                if ( pt_append_data_block->pf_callback2 != NULL )
                {
                    i4_ret = pt_append_data_block->pf_callback2(pt_append_data_block->uc_type, DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
                }

                /* reset append data size */
                i4_append_buf_content_size = 0;

                if ( i4_ret != DLM_OK )
                { /* in case DLM_OK is set to '0' */
                    DLM_TRACE(("Call back return %d\n", (signed int) i4_ret));

                    /* free allocate buffer */
                    dlm_free_append_buffer(pt_append_data_block);
                    pt_io->close_source(i4_file);
                    return i4_ret;
                }

                /* renew buffer */
                i4_append_buf_remain = (INT32) pt_append_data_block->ui4_buf_len;
                p_app_offset = pt_append_data_block->pauc_buffer;
            }
        }
    }

    /* free allocate buffer */
    dlm_free_append_buffer(pt_append_data_block);

    pt_io->close_source (i4_file);
    return DLM_OK;
}
/*-----------------------------------------------------------------------------
 * Name: dlm_validate_digest
 *
 * Description: This API provide ability to validate codefile digest
 *
 * Inputs:  filename, the file need validate digest
 * Outputs: NA
 * Returns: DLM_OK, digest is correctly
 *          DLM_OPEN_FILE_FAILED, open specify file failed
 *          DLM_READ_FILE_FAILED, read specify file failed
 *          DLM_DIGEST_NOT_MATCH, the specify file may changed by someone
 ----------------------------------------------------------------------------*/


static INT32 dlm_validate_digest(CHAR *filename, DLM_IO_T *pt_user_io)
{
    INT32 i4_file, i4_ret;
    UCHAR auc_file_digest[32];
    UCHAR auc_new_digest[32];
    UCHAR *pauc_hdr = puac_internal_buf;
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;
    INT32 i4_len;
    DLM_IO_T *pt_io = pt_user_io;

    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    /* locate header */
    i4_ret = dlm_locate_codefile_header(filename, pt_user_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", filename));
        return i4_ret;
    }

    DLM_TRACE(("Validate codefile HMAC\n"));
    if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
    {
        DLM_TRACE(("open %s failed\n", filename));
        return DLM_OPEN_FILE_FAILED;
    }

    x_memset(auc_new_digest, 0, 32);

    dlm_digest_init();

    i4_len = pt_io->read_data(i4_file, (CHAR*)pauc_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset);
    if ( i4_len == -1 || i4_len < (INT32) (sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset) )
    {
        DLM_TRACE(("Error: read %s header failed\n", filename)); 
        pt_io->close_source (i4_file);
        return DLM_READ_FILE_FAILED;
    }

    /* reinitialize vector */
    ui4_iv = DLM_APPEND_DATA_IV;

    /* decrypted digest */
    /* dlm_dump_buffer("dlm_validate_digest key", puac_internal_buf, 4); */
    pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf + i4_codefile_header_offset);

    dlm_dump_buffer("Encrypted digest:", pt_file_hdr->uac_signature, 32);
    dlm_decrypt_efuse(pt_file_hdr->uac_signature, 32, *((UINT32 *) (puac_internal_buf+i4_codefile_header_offset)), &ui4_iv);

    /* back up digest */
    x_memcpy (auc_file_digest, pt_file_hdr->uac_signature, (SIZE_T)32);  
    dlm_dump_buffer("Download digest", auc_file_digest, 32);
    x_memset (pt_file_hdr->uac_signature, 0, 32);

    dlm_digest_update(puac_internal_buf+i4_codefile_header_offset, (UINT32) (i4_len-i4_codefile_header_offset));

    while ( !pt_io->eof_data(i4_file) )
    {
        i4_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);
        if ( i4_len == -1 || i4_len == 0 )
        {
            /* may reach end of file */
            if ( i4_len == -1 )
            {
                return DLM_READ_FILE_FAILED;
            }
            if ( i4_len == 0 )
            {
                continue;
            }
        }
        dlm_digest_update(puac_internal_buf, (UINT32)i4_len);
    }

    dlm_digest_final(auc_new_digest);

    dlm_dump_buffer("New HMAC", auc_new_digest, 32);

    if ( x_memcmp(auc_file_digest, auc_new_digest, 32) != 0 )
    {
        DLM_TRACE(("HMAC not matched\n"));
        pt_io->close_source (i4_file);
        return DLM_DIGEST_NOT_MATCH;
    }
    else
        DLM_TRACE(("Signature matched\n"));

    pt_io->close_source (i4_file);

    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_upload
 *
 * Description: Upload device internal data to USB flash for future use.
 *              During upload process, this API make proper header for internal
 *              data and add digest to header to provide codefile intergrity
 *              
 * Inputs:  
 *              pac_tag, which data to upload
 *              pt_user_io, user define I/O callback
 *              ps_path, output file and path  
 * Outputs: No
 * Returns: 
 *          DLM_OK                     : upload data done
 *          DLM_ALLOCATE_MEMORY_FAILED : Allocate memory failed
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write data failed
 *          DLM_NO_HELPER_REGISTERED, no such data exist
 ----------------------------------------------------------------------------*/
  #ifndef CC_LOADER_SHRINK_DLM_API
INT32 x_dlm_upload(CHAR *pac_tag, DLM_IO_T *pt_user_io, CHAR *ps_path)
{
    /*
        Algorithm:
        1. search tag in registered list, 
        2. if found, call register callback to get data 
        3. if received length less than register length, end of upload
        4. if received lebgth equal to register length, read more data from user, goto step 3
        5. compute file hash and encrypt it.
        6. output file.

        Note: 
            a) As Jerry.Fan's request, this file does not require ID/Modelname/Version check.
            b) Upload file the control flag is default to ignore
    */
    Append_DATA_TLV_T   t_append_data_tlv; ;
    DLM_APPEND_BLOCK_T *pt_append_data = NULL;
    INT32               i4_out_file;
    INT32               i4_read_len;
    INT32               i4_append_data_size;
    INT32               ui4_total_size = 0;
    INT32               i4_consumed = 0;
    UCHAR               uac_digest[32];
    UINT32              ui4_key;
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T*)puac_internal_buf;
    DLM_IO_T *pt_io = pt_user_io;

    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    /* testing.... dlm_write has problem */
    /*pt_io->write_data = test_w;*/

    x_memcpy(t_append_data_tlv.uc_type, pac_tag, (SIZE_T)4);
    pt_append_data = dlm_find_append_data_type(&t_append_data_tlv);
    if ( pt_append_data )
    {
        /* count data size */
        pt_append_data->pauc_buffer = (UCHAR*) x_mem_alloc(pt_append_data->ui4_buf_len);
        if ( pt_append_data->pauc_buffer == NULL )
        {
            DLM_TRACE(("Allocate buffer for upload data failed\n"));
            return DLM_ALLOCATE_MEMORY_FAILED;
        }

        DLM_TRACE(("Count append data size\n"));

        i4_append_data_size = 0;

        do
        {
            i4_read_len = pt_append_data->ui4_buf_len;
            if ( pt_append_data->pf_callback(DLM_READ, pt_append_data->pauc_buffer, &i4_read_len) == DLM_OK )
                i4_append_data_size += i4_read_len;
            else
            {
                DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                x_mem_free(pt_append_data->pauc_buffer);

                return DLM_UPLOAD_FAILED;
            }
        } while ( i4_read_len == pt_append_data->ui4_buf_len );

        ui4_total_size = (i4_append_data_size + 12 + sizeof (DEFAULT_CODE_FILE_HDR_T));

        /* Process codefile header */
        DLM_TRACE(("\nBuild header\n"));
        x_memset(pt_hdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T)); /* clear buffer */

        x_memcpy(pt_hdr, pt_myHdr, sizeof (DEFAULT_CODE_FILE_HDR_T));
        pt_hdr->ui4_total_size = ui4_total_size;  /* pt_hdr->ui4_time =  random assign */
        x_memcpy(pt_hdr->s_magic, DLM_HDR_MAGIC, (SIZE_T)8);  /* my magic */          

#if HDR_ENCRYPTED
        ui4_iv = DLM_HDR_IV;
        dlm_encrypt_efuse(puac_internal_buf, sizeof (DEFAULT_CODE_FILE_HDR_T)-32, DLM_HDR_KEY, &ui4_iv);
#endif
        ui4_key = *((UINT32 *) puac_internal_buf); /* the key to encrypt/decrypt append data */

        dlm_digest_init();
        dlm_digest_update(puac_internal_buf, (UINT32) sizeof (DEFAULT_CODE_FILE_HDR_T));

        x_memcpy(puac_internal_buf, (CHAR*)t_append_data_tlv.uc_type, (SIZE_T)4); /* tag */
        x_memset(puac_internal_buf + 4, 0, 4); /* no privacy */
        x_memcpy(puac_internal_buf + 8, (CHAR*) &i4_append_data_size, (SIZE_T)4); /* length */
        dlm_digest_update(puac_internal_buf, 12);

        /* compute append data digest */
        DLM_TRACE(("Calculate append data digest\n"));

        do
        {
            i4_read_len = pt_append_data->ui4_buf_len;
            if ( pt_append_data->pf_callback(DLM_READ, pt_append_data->pauc_buffer, &i4_read_len) == DLM_OK )
                dlm_digest_update(pt_append_data->pauc_buffer, (UINT32)i4_read_len);
            else
            {
                DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                x_mem_free(pt_append_data->pauc_buffer);

                return DLM_UPLOAD_FAILED;
            }
        }while ( i4_read_len == pt_append_data->ui4_buf_len );

        DLM_TRACE(("\ndigest\n"));
        /* output digest */
        dlm_digest_final(uac_digest);

        ui4_iv = DLM_APPEND_DATA_IV;
        dlm_dump_buffer("Digest", uac_digest, 32); /* debug */
        dlm_encrypt_efuse(uac_digest, 32, ui4_key, &ui4_iv);

        /* output file */
        i4_out_file = pt_io->open_source(ps_path, kDlm_Append);
        DLM_TRACE(("Output file = %s\n", ps_path));

        if ( i4_out_file == -1 )
        {
            DLM_TRACE(("Generate output file %s failed\n", ps_path));
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_OPEN_FILE_FAILED;
        }

        /* write default file header */
        x_memcpy(pt_hdr, pt_myHdr, sizeof (DEFAULT_CODE_FILE_HDR_T));
        /*pt_hdr->ui4_time =*/
        pt_hdr->ui4_total_size = ui4_total_size;
        x_memcpy(pt_hdr->s_magic, DLM_HDR_MAGIC, (SIZE_T)8);

#if HDR_ENCRYPTED
        ui4_iv = DLM_HDR_IV;
        dlm_encrypt_efuse(puac_internal_buf, sizeof (DEFAULT_CODE_FILE_HDR_T)-32, DLM_HDR_KEY, &ui4_iv);
#endif
        ui4_key = *((UINT32 *) puac_internal_buf);

        if ( pt_io->write_data(i4_out_file, (CHAR*)pt_hdr, sizeof (DEFAULT_CODE_FILE_HDR_T)-32) == -1 )
        {
            DLM_TRACE(("write header to file %s failed\n", ps_path));
            pt_io->close_source(i4_out_file);
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_WRITE_DATA_FAILED;
        }

        /* for progress */
        i4_consumed += sizeof (DEFAULT_CODE_FILE_HDR_T)-32;
        if ( dlm_notify_progress )
            dlm_notify_progress(i4_consumed *100 /ui4_total_size);
        else
            DLM_TRACE(("."));

        /* write digest */
        if ( pt_io->write_data(i4_out_file, (CHAR*)uac_digest, 32 ) == -1 )
        {
            DLM_TRACE(("write header to file %s failed\n", ps_path));
            pt_io->close_source(i4_out_file);
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_WRITE_DATA_FAILED;
        }

        i4_consumed += 32;
        if ( dlm_notify_progress )
            dlm_notify_progress(i4_consumed *100 /ui4_total_size);
        else
            DLM_TRACE(("."));

        /* write append data header */
        x_memcpy(puac_internal_buf, (CHAR*)t_append_data_tlv.uc_type, (SIZE_T)4); /* tag */
        x_memset(puac_internal_buf + 4, 0, 4); /* no privacy */
        x_memcpy(puac_internal_buf + 8, (CHAR*) &i4_append_data_size, (SIZE_T)4); /* length */
        if ( pt_io->write_data(i4_out_file, (CHAR*)puac_internal_buf, 12 ) == -1 )
        {
            DLM_TRACE(("write header to file %s failed\n", ps_path));
            pt_io->close_source(i4_out_file);
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_WRITE_DATA_FAILED;
        }

        i4_consumed += 12;
        if ( dlm_notify_progress )
            dlm_notify_progress(i4_consumed *100 /ui4_total_size);
        else
            DLM_TRACE(("."));

        /* write append data */
        do
        {
            i4_read_len = pt_append_data->ui4_buf_len;
            if ( pt_append_data->pf_callback(DLM_READ, pt_append_data->pauc_buffer, &i4_read_len) == DLM_OK )
            {
                if ( i4_read_len && pt_io->write_data(i4_out_file, (CHAR*)pt_append_data->pauc_buffer, i4_read_len) == -1 )
                {
                    DLM_TRACE(("write data to file %s failed\n", ps_path));
                    pt_io->close_source(i4_out_file);
                    DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                    x_mem_free(pt_append_data->pauc_buffer);

                    return DLM_WRITE_DATA_FAILED;
                }

                i4_consumed += i4_read_len;
                if ( dlm_notify_progress )
                    dlm_notify_progress(i4_consumed *100 /ui4_total_size);
                else
                    DLM_TRACE(("."));

            }
            else
            {
                DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                x_mem_free(pt_append_data->pauc_buffer);

                pt_io->close_source(i4_out_file);
                return DLM_UPLOAD_FAILED;
            }
        } while ( i4_read_len == pt_append_data->ui4_buf_len );

        pt_io->close_source(i4_out_file);

        DLM_TRACE(("Upload done, free buffer = %d\n", pt_append_data->ui4_buf_len));
        x_mem_free(pt_append_data->pauc_buffer);

        return DLM_OK;
    }
    else
    {
        DLM_TRACE(("dlm: no such data exist.\n"));
        return DLM_NO_HELPER_REGISTERED;
    }
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_dlm_download_by_tag
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ps_file,  codefile name
 *          puc_tag,   user defined tag
 *          pt_user_io, user specify I/O
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note: 
 *    Before this function is called, the codefile need pass integrity check
 ----------------------------------------------------------------------------*/
INT32 x_dlm_download_by_tag(CHAR *ps_file, UCHAR *puc_tag, DLM_IO_T *pt_user_io)
{
    INT32 i4_file;
    DLM_APPEND_BLOCK_T *pt_append_data_block = NULL;
    Append_DATA_TLV_T   t_append_data_tlv; 
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;

    INT32       i4_append_len = 0;
    INT32       i4_buffer_len = 0;
    INT32       i4_append_buf_remain = 0;
    INT32       i4_append_buf_content_size = 0;
    UCHAR      *p_buf_offset = NULL;
    UCHAR      *p_app_offset = NULL;
    INT32       i4_ret;
    DLM_IO_T*   pt_io = pt_user_io;
    UINT32      ui4_tag_size = 0;
    UINT32      ui4_tag_consume_size = 0;
    UINT32      ui4_tag_percentage = 0;
    INT32       i4_flag = 0;
    UCHAR       uac_ahdr[8];
	INT32       i4_decrypted_len = 0;

    /* append data may has extra hdr */
    INT32 i4_extra_app_hdr_len = 0;

    if ( pt_io == NULL )
        pt_io = x_dlm_get_default_io();


    /* locate header */
    i4_ret = dlm_locate_codefile_header(ps_file, pt_user_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", ps_file));
        return i4_ret;
    }

    /* check tag relation */
    x_memcpy(t_append_data_tlv.uc_type, puc_tag, (SIZE_T)4);
    pt_append_data_block = dlm_find_append_data_type(&t_append_data_tlv);
    if ( pt_append_data_block )
    {
        i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
        pt_append_data_block->pauc_buffer = (UCHAR *)x_mem_alloc (pt_append_data_block->ui4_buf_len);
        if ( pt_append_data_block->pauc_buffer == NULL )
        {
            return DLM_ALLOCATE_MEMORY_FAILED;
        }
        p_app_offset = pt_append_data_block->pauc_buffer;
    }
    else
    {
        return DLM_UNKONW_TAG;
    }

    /* open source */
    if ( (i4_file = pt_io->open_source(ps_file, kDlm_Read)) == -1 )
    {
        x_mem_free(pt_append_data_block->pauc_buffer);
        return DLM_OPEN_FILE_FAILED;
    }

    /* our internal header length */
    i4_ret = pt_io->read_data(i4_file, (CHAR *)puac_internal_buf, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset);
    if ( i4_ret == -1 || i4_ret < sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset )
    {
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_io->close_source(i4_file);
        return DLM_READ_FILE_FAILED;
    }

#if HDR_ENCRYPTED
    pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf + i4_codefile_header_offset);
    ui4_iv = DLM_HDR_IV;
    dlm_decrypt_efuse(puac_internal_buf + i4_codefile_header_offset, (UINT32)(sizeof(DEFAULT_CODE_FILE_HDR_T)-32), DLM_HDR_KEY, &ui4_iv);
#endif

    /* quick check, if magic not match, skip download */
    if ( x_memcmp(pt_hdr->s_magic, DLM_HDR_MAGIC, 8)!=0 )
    {
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_io->close_source(i4_file);
        return DLM_HDR_MAGIC_NOT_MATCH;
    }

    /* backup download hdr for future user */
    dlm_backup_download_hdr();

    /* locate tag */
    do
    {
        i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, 12);
        if ( i4_buffer_len != 12 )
        {
            DLM_TRACE(("locate tag failed\n"));
            pt_io->close_source(i4_file);
            x_mem_free(pt_append_data_block->pauc_buffer);
            return DLM_READ_FILE_FAILED;
        }
        /* compare tag */
        i4_buffer_len =  puac_internal_buf[11] * 256 * 256 * 256 + 
                         puac_internal_buf[10] * 256 * 256 + 
                         puac_internal_buf[9] * 256 + 
                         puac_internal_buf[8];

        DLM_TRACE(("Append file length = %d\n", i4_buffer_len));

        if ( x_memcmp(puac_internal_buf, puc_tag,4) != 0 )
        {
            /* seek to next tag */
            if ( pt_io->seek_source(i4_file, (UINT32)i4_buffer_len) != 0 )
            {
                DLM_TRACE(("Seek tag failed\n"));
                pt_io->close_source(i4_file);
                x_mem_free(pt_append_data_block->pauc_buffer);
                return DLM_READ_FILE_FAILED;
            }
        }
        else
        {
            /* tag match */
            i4_append_len = i4_buffer_len;

            /* reinitialize vector */
            ui4_iv = DLM_APPEND_DATA_IV; 

            /* for tag progress use */
            ui4_tag_size = i4_append_len;
            ui4_tag_consume_size = 0;
            ui4_tag_percentage = 0;

            i4_buffer_len = 0;
            x_memcpy(&t_append_data_tlv, puac_internal_buf, (SIZE_T)12);
            i4_flag = 1;
            break;
        }
    } while ( 1 );

    if ( i4_flag == 0 )
        return DLM_UNKONW_TAG;

    while ( i4_append_len )
    {
        /* renew buffer */
        if ( i4_buffer_len == 0 )
        {
            if ( !pt_io->eof_data(i4_file) )
                i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);
            else
                return DLM_READ_FILE_FAILED; /* ??? */

            /* error check here */
            if ( i4_buffer_len == -1 || i4_buffer_len == 0 )
            {
                if ( i4_buffer_len == -1 )
                {
                    return DLM_READ_FILE_FAILED;
                }
                else
                {
                    /* end of file? try again... */
                    continue;
                }
            }
            p_buf_offset = puac_internal_buf;
        }

        /**************************************
        * check if APPEND data has extra HDR *
        * the hdr is start with magic "iMtK",*
        * this magic is defned in dlm.h      *
        **************************************/
        if ( i4_flag )
        {
            x_memcpy(uac_ahdr, p_buf_offset, (SIZE_T)8);
            if ( t_append_data_tlv.uc_privacy[0] )
            {
                dlm_decrypt_efuse(uac_ahdr, 8, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
				i4_decrypted_len += 8;
            }

            if ( x_memcmp(uac_ahdr, DLM_APPEND_HDR_MAGIC, 4) == 0 )
            {
                i4_flag = 0; /* each append data only check once */
                /* the folllowing expresion is base on little endian */
                i4_extra_app_hdr_len = uac_ahdr[4] + uac_ahdr[5] * 256 + uac_ahdr[6] * 256 * 256 + uac_ahdr[7] * 256 * 256 * 256 ;

                if ( i4_extra_app_hdr_len % 4 )
                    i4_extra_app_hdr_len += (4 - (i4_extra_app_hdr_len %4));

                p_buf_offset  += 8; /* shift magic, length */
                i4_buffer_len -= 8;
                i4_append_len -= 8;
                ui4_tag_consume_size +=8;
                DLM_TRACE(("Extra append hdr length = %d\n", i4_extra_app_hdr_len));
            }
            else
            {
                ui4_iv = DLM_APPEND_DATA_IV;
            }
        }

        if ( i4_extra_app_hdr_len )
        {
            /* skip this hdr */
            if ( i4_buffer_len > i4_extra_app_hdr_len )
            {
                if ( t_append_data_tlv.uc_privacy[0] )
                {
                	INT32 i4_ssss = 0;
                	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
						if (i4_extra_app_hdr_len > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
							i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
						}else {
							i4_ssss = i4_extra_app_hdr_len;
						}
					}
					if (i4_ssss > 0){
	                    dlm_decrypt_efuse(p_buf_offset, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
						i4_decrypted_len += i4_ssss;
						//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);
						 //dlm_dump_buffer("data ", p_buf_offset, i4_decrypted_len);
					}
                }
                i4_buffer_len -= i4_extra_app_hdr_len;
                p_buf_offset += i4_extra_app_hdr_len;
                i4_append_len -= i4_extra_app_hdr_len;
                ui4_tag_consume_size += i4_extra_app_hdr_len;
            }
            else
            {
                if ( t_append_data_tlv.uc_privacy[0] )
                {
                	INT32 i4_ssss = 0;
                	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
						if (i4_buffer_len > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
							i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
						}else {
							i4_ssss = i4_buffer_len;
						}
					}
					if (i4_ssss > 0){
	                    dlm_decrypt_efuse(p_buf_offset, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
						i4_decrypted_len += i4_ssss;
						//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);
					   //dlm_dump_buffer("data ", p_buf_offset, i4_decrypted_len);
					}
                }
                i4_extra_app_hdr_len -= i4_buffer_len;
                ui4_tag_consume_size += i4_buffer_len;
                i4_append_len -= i4_extra_app_hdr_len;
                i4_buffer_len = 0;
                continue;
            }
            i4_extra_app_hdr_len = 0;
        }

        /* dispath this data to append data call back */
        if ( i4_append_buf_remain )
        {
            if ( i4_append_len > i4_append_buf_remain )
            {
                /* more space need */
                if ( i4_buffer_len >= i4_append_buf_remain )
                {
                    /* fill whole remain space */
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_append_buf_remain);
                    p_buf_offset += i4_append_buf_remain;
                    i4_append_len -= i4_append_buf_remain;
                    i4_buffer_len -= i4_append_buf_remain;
                    i4_append_buf_content_size += i4_append_buf_remain;
                    i4_append_buf_remain = 0;
                }
                else
                {
                    /* i4_buffer_len < i4_append_buf_remain */
                    /* move as much as possible */
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_buffer_len);
                    p_app_offset += i4_buffer_len;
                    i4_append_buf_remain -= i4_buffer_len;
                    i4_append_len -= i4_buffer_len;
                    i4_append_buf_content_size += i4_buffer_len;
                    i4_buffer_len = 0;
                }
            }
            else
            {
                /* i4_append_len < i4_append_buf_remain */
                /* this is last apppend data in this block */
                if ( i4_buffer_len >= i4_append_len )
                {
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_append_len);
                    p_buf_offset += i4_append_len;
                    i4_buffer_len -= i4_append_len;
                    i4_append_buf_content_size += i4_append_len;
                    i4_append_len = 0;
                    i4_append_buf_remain = 0;
                }
                else
                {
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_buffer_len);
                    p_app_offset += i4_buffer_len;
                    i4_append_len -= i4_buffer_len;
                    i4_append_buf_remain -= i4_buffer_len;
                    i4_append_buf_content_size += i4_buffer_len;
                    i4_buffer_len = 0;
                }
            }
        }

        /* Turn on this to trace date moving status */
#if 0
        DLM_TRACE(("app_len = %u, buf_len = %u, appbuf_remain = %u, appbuf_content_size = %u\n", 
                   i4_append_len, 
                   i4_buffer_len, 
                   i4_append_buf_remain,
                   i4_append_buf_content_size));
#endif

        if ( i4_append_buf_remain == 0 )
        {
            /* if append data is scrambled then data buffer need decrypted */
            if ( t_append_data_tlv.uc_privacy[0] )
            {
                	INT32 i4_ssss = 0;
                	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
						if (i4_append_buf_content_size > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
							i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
						}else {
							i4_ssss = i4_append_buf_content_size;
						}
					}
					if (i4_ssss > 0){
						dlm_decrypt_efuse(pt_append_data_block->pauc_buffer, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
						i4_decrypted_len += i4_ssss;
						//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);
						   //dlm_dump_buffer("data ", pt_append_data_block->pauc_buffer, i4_decrypted_len);
					}
            }
            if ( pt_append_data_block->pf_callback != NULL )
            {
                i4_ret = pt_append_data_block->pf_callback(DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
            }

            if ( pt_append_data_block->pf_callback2 != NULL )
            {
                i4_ret = pt_append_data_block->pf_callback2(pt_append_data_block->uc_type, DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
            }

            ui4_tag_consume_size += i4_append_buf_content_size;
            ui4_tag_percentage = ui4_tag_consume_size*100 / ui4_tag_size;

            /* current tag progress */
            if ( dlm_notify_tag_progress )
                dlm_notify_tag_progress(ui4_tag_percentage);
            else
                DLM_TRACE(("download %c%c%c%c percentage = %d\n", 
                           t_append_data_tlv.uc_type[0],
                           t_append_data_tlv.uc_type[1],
                           t_append_data_tlv.uc_type[2],
                           t_append_data_tlv.uc_type[3], 
                           ui4_tag_percentage));

            i4_append_buf_content_size = 0;

            if ( i4_ret != DLM_OK )
            { /* in case DLM_OK is set to '0' */
                DLM_TRACE(("Call back return %d\n", (signed int) i4_ret));

                /* free allocate buffer */
                if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
                {
                    x_mem_free(pt_append_data_block->pauc_buffer);
                    pt_append_data_block->pauc_buffer = NULL;
                }
                pt_io->close_source(i4_file);
                return i4_ret;
            }

            /* renew buffer */
            i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
            p_app_offset = pt_append_data_block->pauc_buffer;
        }
    }

    /* free allocate buffer */
    if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
    {
        DLM_TRACE(("\nfree memory size = %d\n", pt_append_data_block->ui4_buf_len));
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_append_data_block->pauc_buffer = NULL;
    }

    pt_io->close_source (i4_file);
    return DLM_OK;
}

INT32 x_dlm_download_by_division(UINT32 ui4_len, UCHAR*  puc_buffer, UCHAR* puc_tag)
{
    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_tag_test
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ps_file,  codefile name
 *          puac_tag, user specify tag name
 *          pac_buffer, the buffer to store downloaded data
 *          i4_buf_size, pac_buffer size
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note, 
 *      Due to crypher tool limit, the i4_buf_size must to be divisible by 4
 ----------------------------------------------------------------------------*/
INT32 x_dlm_tag_test(CHAR *ps_file, UCHAR *puac_tag, DLM_IO_T *pt_user_io, CHAR *pac_buffer, INT32 i4_buf_size)
{
    INT32 i4_file;
    INT32 i4_privacy = 0;
    INT32 i4_ret;
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;
    DLM_IO_T*   pt_io = pt_user_io;
	INT32 i4_decrypted_len = 0;

    if ( pt_io == NULL )
        pt_io = x_dlm_get_default_io();

    /* locate header */
    i4_ret = dlm_locate_codefile_header(ps_file, pt_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", ps_file));
        return i4_ret;
    }

    /* open source */
    if ( (i4_file = pt_io->open_source(ps_file, kDlm_Read)) == -1 )
    {
        return DLM_OPEN_FILE_FAILED;
    }

    /* our internal header length */
    i4_ret = pt_io->read_data(i4_file, (CHAR *)puac_internal_buf, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset);
    if ( i4_ret == -1 || i4_ret < sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset )
    {
        pt_io->close_source(i4_file);
        return DLM_READ_FILE_FAILED;
    }

#if HDR_ENCRYPTED
    pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf+i4_codefile_header_offset);
    ui4_iv = DLM_HDR_IV;
    dlm_decrypt_efuse(puac_internal_buf+i4_codefile_header_offset, (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32), DLM_HDR_KEY, &ui4_iv);
#endif

    /* quick check, if magic not match, skip download */
    if ( x_memcmp(pt_hdr->s_magic, DLM_HDR_MAGIC, 8)!=0 )
    {
        pt_io->close_source(i4_file);
        return DLM_HDR_MAGIC_NOT_MATCH;
    }

    /* backup download hdr for future user */
    dlm_backup_download_hdr();

    /* locate tag */
    do
    {
        i4_ret = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, 12);
        if ( i4_ret != 12 )
        {
            DLM_TRACE(("locate tag failed\n"));
            pt_io->close_source(i4_file);
            return DLM_READ_FILE_FAILED;
        }
        /* compare tag */
        i4_ret =  puac_internal_buf[11] * 256 * 256 * 256 + 
                  puac_internal_buf[10] * 256 * 256 + 
                  puac_internal_buf[9] * 256 + 
                  puac_internal_buf[8];

        DLM_TRACE(("Append file length = %d\n", i4_ret));

        if ( x_memcmp(puac_internal_buf, puac_tag,4) != 0 )
        {
            /* seek to next tag */
            if ( pt_io->seek_source(i4_file, (UINT32)i4_ret) != 0 )
            {
                DLM_TRACE(("Seek tag failed\n"));
                pt_io->close_source(i4_file);
                return DLM_READ_FILE_FAILED;
            }
        }
        else
        {
            /* tag match */
            if ( puac_internal_buf[4] )
            {
                i4_privacy = 1;
            }
            break;
        }
    } while ( 1 );


    i4_ret = pt_io->read_data(i4_file, (CHAR*)pac_buffer, i4_buf_size);
    if ( i4_privacy )
    {
    	INT32 i4_ssss = 0;
    	if (i4_decrypted_len < DLM_ENCRYPT_LENGTH){
			if (i4_ret > DLM_ENCRYPT_LENGTH - i4_decrypted_len){
				i4_ssss = DLM_ENCRYPT_LENGTH - i4_decrypted_len;
			}else {
				i4_ssss = i4_ret;
			}
		}
		if (i4_ssss > 0){
			ui4_iv = DLM_APPEND_DATA_IV;
			dlm_decrypt_efuse((UCHAR*)pac_buffer, (UINT32)i4_ssss, *((UINT32*)pt_myHdr->uac_id), &ui4_iv);
			i4_decrypted_len += i4_ssss;
			//x_dbg_stmt("jiangtao, current decrpted %d \r\n", i4_ssss);
			//dlm_dump_buffer("data ",pac_buffer, i4_decrypted_len);
			
		}
    }

    pt_io->close_source (i4_file);
    return DLM_OK;
}

/* To identify the code file header */
INT32 dlm_locate_codefile_header(CHAR *filename, DLM_IO_T *pt_user_io)
{
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)puac_internal_buf;
    INT32 i4_ret;

    i4_ret = dlm_read_code_header(filename, pt_user_io, sizeof (DEFAULT_CODE_FILE_HDR_T));
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Read locate codefile header failed - %s\n", filename));
        return  i4_ret;
    }

    /* check magic */
#if HDR_ENCRYPTED
    ui4_iv = DLM_HDR_IV;
    //UCHAR pt_file_hdlr[165];
    dlm_decrypt_efuse(puac_internal_buf, (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32), DLM_HDR_KEY, &ui4_iv);
    //dlm_dump_buffer("Encrypted header:",puac_internal_buf,sizeof(DEFAULT_CODE_FILE_HDR_T));//add dump msg.
#endif

    if ( x_memcmp(pt_file_hdr->s_magic, DLM_HDR_MAGIC, 8) == 0 )
    {
        i4_codefile_header_offset = 0;
        return DLM_OK;
    }

    /* has extra header in codefile */
    i4_ret = dlm_read_code_header(filename, pt_user_io, sizeof (DEFAULT_CODE_FILE_HDR_T) + DLM_PLAIN_TEXT_OFFSET_PHILIPS);
    if ( i4_ret != DLM_OK )
    {
        return  i4_ret;
    }

    /* check magic */
#if HDR_ENCRYPTED
    ui4_iv = DLM_HDR_IV;
    dlm_decrypt_efuse(puac_internal_buf + DLM_PLAIN_TEXT_OFFSET_PHILIPS, (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32), DLM_HDR_KEY, &ui4_iv);
#endif

    pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T*) (puac_internal_buf + DLM_PLAIN_TEXT_OFFSET_PHILIPS);

    if ( x_memcmp(pt_file_hdr->s_magic, DLM_HDR_MAGIC, 8) == 0 )
    {
        i4_codefile_header_offset = DLM_PLAIN_TEXT_OFFSET_PHILIPS;
        return DLM_OK;
    }

    return DLM_DIGEST_NOT_MATCH;
}

#else
/*-----------------------------------------------------------------------------
 * Name: x_dlm_integrity_check
 *
 * Description: This API will check file integrity as user defined
 *
 * Inputs:  filename
 * Outputs: no
 * Returns: DLM_OK check ok
 *          others failed
 ----------------------------------------------------------------------------*/

INT32 x_dlm_integrity_check(CHAR *filename, DLM_IO_T *pt_user_io)
{
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)puac_internal_buf;
    INT32 i4_file, i4_ret;
    UINT32 ui4_key = DLM_HDR_KEY;
    DLM_IO_T *pt_io = pt_user_io;
    UCHAR       uc_key[16];
    UCHAR       uc_vector[16];
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));

    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    if ( dlm_header_parse_fn )
    {
        DLM_TRACE(("Vendor has define its own integrity check API\n"));
        return dlm_header_parse_fn(filename);
    }
    else
    {
        DLM_TRACE(("Use internal integrity check API\n"));

        /* locate header */
        i4_ret = dlm_locate_codefile_header(filename, pt_io);
        if ( i4_ret != DLM_OK )
        {
            DLM_TRACE(("Locate codefile header failed - %s\n", filename));
            return i4_ret;
        }

        if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
        {
            DLM_TRACE(("Open file %s for integrity check failed\n", filename));
            return DLM_OPEN_FILE_FAILED;
        }

        i4_ret = pt_io->read_data(i4_file, (CHAR *)pt_file_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset);
        if ( i4_ret < (INT32) sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset || 
             i4_ret == -1 )
        {
            DLM_TRACE(("Read file %s header failed\n", filename));
            pt_io->close_source(i4_file);
            return DLM_READ_FILE_FAILED;
        }
        pt_io->close_source (i4_file);

#if HDR_ENCRYPTED
        pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)(puac_internal_buf + i4_codefile_header_offset);

        dlm_dump_buffer("Encrypted header:", (UCHAR*)pt_file_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32);
        
        dlm_aes(puac_internal_buf + i4_codefile_header_offset,
                NULL,
                (UINT32) (sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32),
                uc_key,
                uc_vector,
                AES_DECRYPT);

#endif
        dlm_dump_buffer("Header:", (UCHAR*)pt_file_hdr, sizeof( DEFAULT_CODE_FILE_HDR_T ) - 32);


        dlm_backup_download_hdr();

        /* 
         * verify control flag 
         * Current define 
         *    0x00000000            (only digest is verified)
         *    0x10000000           verify ID
         *    0x20000000           verify version
         *    0x40000000           verify Model Name
        */
        if ( pt_file_hdr->uac_control[0] & kValidateID )
        {
            if ( dlm_validate_id(puac_internal_buf + i4_codefile_header_offset) != DLM_OK )
            {
                return DLM_ID_NOT_MATCH;
            }
        }

        if ( pt_file_hdr->uac_control[0] & kValidateModelName )
        {
            if ( dlm_validate_model_name(puac_internal_buf + i4_codefile_header_offset) != DLM_OK )
            {
                return DLM_MODEL_NAME_NOT_MATCH;
            }
        }

        if ( pt_file_hdr->uac_control[0] & kValidateVersion )
        {
            if ( dlm_validate_version(puac_internal_buf + i4_codefile_header_offset) != DLM_OK )
            {
                return DLM_VERSION_NOT_MATCH;
            }
        }

        /* validate digest */
        return dlm_validate_digest(filename, pt_io);
    }
}


/*-----------------------------------------------------------------------------
 * Name: x_dlm_download
 *
 * Description: User can use this API to download codefile from USB flash
 *
 * Inputs:  filename, code file name
 * Outputs: NA
 * Returns: DLM_OK, download finished
 ----------------------------------------------------------------------------*/
INT32 x_dlm_download(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_check)
{
    INT32 i4_file;
    DLM_APPEND_BLOCK_T *pt_append_data_block = NULL;
    Append_DATA_TLV_T   t_append_data_tlv; 
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;

    INT32       i4_append_len = 0;
    INT32       i4_buffer_len = 0;
    INT32       i4_append_buf_remain = 0;
    INT32       i4_append_buf_content_size = 0;
    UCHAR      *p_buf_offset = NULL;
    UCHAR      *p_app_offset = NULL;
    INT32       i4_ret;
    DLM_IO_T*   pt_io = pt_user_io;
    UINT32      ui4_file_size = 0;
    UINT32      ui4_consume_size = 0;
    FLOAT       f_percent = 0;
    UINT32      ui4_percentage = 0;
    INT32       i4_adjust = 0;   
    UCHAR       uc_key[16];
    UCHAR       uc_vector[16];
    UCHAR       uc_digest[32];
    UINT32      ui4_key = DLM_HDR_KEY;
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    if ( pt_io == NULL )
        pt_io = x_dlm_get_default_io();

    /* locate header */
    i4_ret = dlm_locate_codefile_header(filename, pt_user_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", filename));
        return i4_ret;
    }

    /* open source */
    if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
    {
        DLM_TRACE(("Open download file %s failed\n", filename));
        return DLM_OPEN_FILE_FAILED;
    }

    /* Read code file header from storage */
    i4_ret = pt_io->read_data(i4_file, (CHAR *)puac_internal_buf, sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset);

    /* if no data or size not equal to code file header size, something wrong!!! */
    if ( i4_ret == -1 || i4_ret < (INT32) sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset )
    {
        pt_io->close_source (i4_file);
        return DLM_READ_FILE_FAILED;
    }
#if HDR_ENCRYPTED

    pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf + i4_codefile_header_offset);

    dlm_aes(puac_internal_buf + i4_codefile_header_offset,
            NULL,
            (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32),
            uc_key,
            uc_vector,
            AES_DECRYPT);

#endif
    ui4_file_size = pt_hdr->ui4_total_size;

    /* backup download hdr for future use */
    dlm_backup_download_hdr();

    ui4_consume_size = sizeof(DEFAULT_CODE_FILE_HDR_T);

    /**
     * main loop to iterator codefile tag
     * i4_buffer_len: unread buffer size in current block
     * i4_append_len: the counter to accumulate how many append data
     * is unread.
     */
    while ( !pt_io->eof_data(i4_file) || i4_buffer_len || i4_append_len )
    {
        /* renew buffer */
        if ( i4_buffer_len == 0 )
        {
            /* the counter to adjust i4_buffer_len if read data block is > i4_buffer_len */
            i4_adjust = 0;

            i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);

            /* error check here */
            if ( i4_buffer_len == -1 )
            {
                /* free buffer */
                dlm_free_append_buffer(pt_append_data_block);
                pt_io->close_source (i4_file);
                return DLM_READ_FILE_FAILED;
            }

            /* try again, our FM has issue before, left this code to make sure the file is end */
            if ( i4_buffer_len == 0 )
            {
                continue;
            }

            /* ui4_consume_size, the counter to record how many bytes is used */
            ui4_consume_size += i4_buffer_len;

            /* notify progress */
            f_percent = (FLOAT)((FLOAT)ui4_consume_size/(FLOAT)ui4_file_size);
            ui4_percentage = (UINT32)(f_percent * 100);
            
            if ( dlm_notify_progress )
                dlm_notify_progress((INT32)ui4_percentage);
            
            /* let ring buffer start again */
            p_buf_offset = puac_internal_buf;
        }
       
        /* start new append data block */
        if ( i4_append_len == 0 )
        {
            CHAR *pac_tlv = (CHAR *) &t_append_data_tlv;            /* 
             * if remain data is less than append data header size, 
             * need require more data...
             */
            if ( (UINT32)i4_buffer_len < APPEND_HEADER_SIZE)
            {
                i4_adjust = i4_buffer_len;

                /* consume remain data from internal buffer */
                x_memcpy(pac_tlv, p_buf_offset, (SIZE_T)i4_adjust);
                ui4_consume_size += i4_adjust;

                /* read new data from storage */
                i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);

                if ( i4_buffer_len )
                {
                    /* append remain data */
                    INT32 i4_offset = (INT32)APPEND_HEADER_SIZE - i4_adjust;

                    p_buf_offset = puac_internal_buf;
                    x_memcpy(pac_tlv + i4_adjust, p_buf_offset, (SIZE_T)i4_offset);
                    i4_adjust = i4_offset;

                    ui4_consume_size += i4_buffer_len;
                    /* notify progress */
                    f_percent = (FLOAT)((FLOAT)ui4_consume_size/(FLOAT)ui4_file_size);
                    ui4_percentage = (UINT32)(f_percent * 100);
                                

                    if ( dlm_notify_progress )
                        dlm_notify_progress((INT32)ui4_percentage);
                       
                }
                else
                {
                    /* free buffer */
                    dlm_free_append_buffer(pt_append_data_block);
                    pt_io->close_source (i4_file);
                    return DLM_READ_FILE_FAILED;
                }
            }
            else
            {
                x_memcpy(&t_append_data_tlv, p_buf_offset, sizeof(Append_DATA_TLV_T));
                i4_adjust = 0;
            }


            /* reinitialize vector */

            x_memset(uc_vector, 0, 16);
            x_memcpy(uc_key, pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + 2*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + 3*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));

        
            i4_append_len = t_append_data_tlv.uc_length[3] * 256 * 256 * 256 + 
                            t_append_data_tlv.uc_length[2] * 256 * 256 + 
                            t_append_data_tlv.uc_length[1] * 256 + 
                            t_append_data_tlv.uc_length[0];


            dlm_aes(t_append_data_tlv.uc_magic,
                    NULL,
                    16,
                    uc_key,
                    uc_vector,
                    AES_DECRYPT);
            if (x_memcmp(t_append_data_tlv.uc_magic, DLM_APPEND_MAGIC,16) != 0)
            {
                pt_io->close_source(i4_file);
                return DLM_HDR_MAGIC_NOT_MATCH;
            }
            dlm_aes(t_append_data_tlv.uc_digest,
                    NULL,
                    16,
                    uc_key,
                    uc_vector,
                    AES_DECRYPT);
            dlm_dump_buffer("Append", p_buf_offset, sizeof(Append_DATA_TLV_T));

            /* free allocate buffer */
            dlm_free_append_buffer(pt_append_data_block);

            pt_append_data_block = dlm_find_append_data_type(&t_append_data_tlv);

            if ( pt_append_data_block )
            {
                i4_append_buf_remain = (INT32) pt_append_data_block->ui4_buf_len;
                pt_append_data_block->pauc_buffer = (UCHAR *)x_mem_alloc (pt_append_data_block->ui4_buf_len);
                if ( pt_append_data_block->pauc_buffer == NULL )
                {
                    DLM_TRACE(("Allocate buffer for append data failed\n"));
                    pt_io->close_source(i4_file);
                    return DLM_ALLOCATE_MEMORY_FAILED;
                }
                p_app_offset = pt_append_data_block->pauc_buffer;

            }
            else
            {
                i4_append_buf_remain = 0;
                p_app_offset = NULL;
            }

            if ( i4_adjust )
            {
                i4_buffer_len -= i4_adjust;
                p_buf_offset += i4_adjust;
                i4_adjust = 0;
            }
            else
            {
                i4_buffer_len -= APPEND_HEADER_SIZE; /* append data header */
                p_buf_offset += APPEND_HEADER_SIZE;
            }
        }

        if ( i4_buffer_len <= 0 )
        {
            /* read data from storage */
            i4_buffer_len = 0;
            continue;
        }
        /* this append data does not have call back */
        if ( !pt_append_data_block )
        {
            if ( i4_append_len > i4_buffer_len )
            {
                i4_append_len -= i4_buffer_len;
                i4_buffer_len = 0;
            }
            else
            {
                i4_buffer_len -= i4_append_len;
                p_buf_offset += i4_append_len;
                i4_append_len = 0;
            }
        }
        else
        {
            /* dispath this data to append data call back */
            
            if ( i4_append_buf_remain )
            {
                if ( i4_append_len > i4_append_buf_remain )
                {
                    /* more space need */
                    if ( i4_buffer_len >= i4_append_buf_remain )
                    {
                        /* fill whole remain space */
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T) i4_append_buf_remain);
                        p_buf_offset += i4_append_buf_remain;
                        i4_append_len -= i4_append_buf_remain;
                        i4_buffer_len -= i4_append_buf_remain;
                        i4_append_buf_content_size += i4_append_buf_remain;
                        i4_append_buf_remain = 0;
                    }
                    else
                    {
                        /* i4_buffer_len < i4_append_buf_remain */
                        /* move as much as possible */
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_buffer_len);
                        p_app_offset += i4_buffer_len;
                        i4_append_buf_remain -= i4_buffer_len;
                        i4_append_len -= i4_buffer_len;
                        i4_append_buf_content_size += i4_buffer_len;
                        i4_buffer_len = 0;
                    }
                }
                else
                {
                    /* i4_append_len < i4_append_buf_remain */
                    /* this is last apppend data in this block */
                    if ( i4_buffer_len >= i4_append_len )
                    {
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_append_len);
                        p_buf_offset += i4_append_len;
                        i4_buffer_len -= i4_append_len;
                        i4_append_buf_content_size += i4_append_len;
                        i4_append_len = 0;
                        i4_append_buf_remain = 0;
                    }
                    else
                    {
                        x_memcpy(p_app_offset, p_buf_offset, (SIZE_T) i4_buffer_len);
                        p_app_offset += i4_buffer_len;
                        i4_append_len -= i4_buffer_len;
                        i4_append_buf_remain -= i4_buffer_len;
                        i4_append_buf_content_size += i4_buffer_len;
                        i4_buffer_len = 0;
                    }
                }
            }

            if ( i4_append_buf_remain == 0 )
            {
                /* if append data is scrambled then data buffer need decrypted */
                if ( t_append_data_tlv.uc_privacy[0] )
                {
                    dlm_aes(pt_append_data_block->pauc_buffer, 
                            NULL,
                            i4_append_buf_content_size,
                            uc_key,
                            uc_vector,
                            AES_DECRYPT);
                    if (i4_check)
                    {
                        dlm_digest_init();
                        dlm_digest_update(pt_append_data_block->pauc_buffer,i4_append_buf_content_size);
                        dlm_digest_final(uc_digest);
                        if (x_memcpy(uc_digest, t_append_data_tlv.uc_magic, 32) != 0)
                        {
                            dlm_free_append_buffer(pt_append_data_block);
                            pt_io->close_source(i4_file);
                            return DLM_DIGEST_NOT_MATCH;
                        }
                    }
                }
                if ( pt_append_data_block->pf_callback != NULL )
                {
                    i4_ret = pt_append_data_block->pf_callback(DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
                }
                if ( pt_append_data_block->pf_callback2 != NULL )
                {
                    i4_ret = pt_append_data_block->pf_callback2(pt_append_data_block->uc_type, DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
                }

                /* reset append data size */
                i4_append_buf_content_size = 0;

                if ( i4_ret != DLM_OK )
                { /* in case DLM_OK is set to '0' */
                    DLM_TRACE(("Call back return %d\n", (signed int) i4_ret));

                    /* free allocate buffer */
                    dlm_free_append_buffer(pt_append_data_block);
                    pt_io->close_source(i4_file);
                    return i4_ret;
                }

                /* renew buffer */
                i4_append_buf_remain = (INT32) pt_append_data_block->ui4_buf_len;
                p_app_offset = pt_append_data_block->pauc_buffer;
            }
        }
    }

    /* free allocate buffer */
    dlm_free_append_buffer(pt_append_data_block);

    pt_io->close_source (i4_file);
    return DLM_OK;
}
/*-----------------------------------------------------------------------------
 * Name: dlm_validate_digest
 *
 * Description: This API provide ability to validate codefile digest
 *
 * Inputs:  filename, the file need validate digest
 * Outputs: NA
 * Returns: DLM_OK, digest is correctly
 *          DLM_OPEN_FILE_FAILED, open specify file failed
 *          DLM_READ_FILE_FAILED, read specify file failed
 *          DLM_DIGEST_NOT_MATCH, the specify file may changed by someone
 ----------------------------------------------------------------------------*/


static INT32 dlm_validate_digest(CHAR *filename, DLM_IO_T *pt_user_io)
{
    INT32 i4_file, i4_ret;
    UCHAR auc_file_digest[32];
    UCHAR auc_new_digest[32];
    UCHAR *pauc_hdr = puac_internal_buf;
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;
    INT32 i4_len;
    DLM_IO_T *pt_io = pt_user_io;
    UCHAR uc_key[16];
    UCHAR uc_vector[16];
    UINT32 ui4_key = DLM_HDR_KEY;
    x_memcpy(uc_key,&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32),&ui4_key,sizeof(UINT32));

    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    /* locate header */
    i4_ret = dlm_locate_codefile_header(filename, pt_user_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", filename));
        return i4_ret;
    }

    DLM_TRACE(("Validate codefile HMAC\n"));
    if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
    {
        DLM_TRACE(("open %s failed\n", filename));
        return DLM_OPEN_FILE_FAILED;
    }

    x_memset(auc_new_digest, 0, 32);

    dlm_digest_init();

    i4_len = pt_io->read_data(i4_file, (CHAR*)pauc_hdr, sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset);
    if ( i4_len == -1 || i4_len < (INT32) (sizeof(DEFAULT_CODE_FILE_HDR_T) + i4_codefile_header_offset) )
    {
        DLM_TRACE(("Error: read %s header failed\n", filename)); 
        pt_io->close_source (i4_file);
        return DLM_READ_FILE_FAILED;
    }

    /* decrypted digest */
    /* dlm_dump_buffer("dlm_validate_digest key", puac_internal_buf, 4); */
    pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf + i4_codefile_header_offset);

    dlm_dump_buffer("Encrypted digest:", pt_file_hdr->uac_signature, 32);

    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key,&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    
    dlm_aes(pt_file_hdr->uac_signature,
            NULL,
            32,
            uc_key,
            uc_vector,
            AES_DECRYPT);

    /* back up digest */
    x_memcpy (auc_file_digest, pt_file_hdr->uac_signature, (SIZE_T)32);  
    dlm_dump_buffer("Download digest", auc_file_digest, 32);
    x_memset (pt_file_hdr->uac_signature, 0, 32);

    dlm_digest_update(puac_internal_buf+i4_codefile_header_offset, (UINT32) (i4_len-i4_codefile_header_offset));

    while ( !pt_io->eof_data(i4_file) )
    {
        i4_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);
        if ( i4_len == -1 || i4_len == 0 )
        {
            /* may reach end of file */
            if ( i4_len == -1 )
            {
                return DLM_READ_FILE_FAILED;
            }
            if ( i4_len == 0 )
            {
                continue;
            }
        }
        dlm_digest_update(puac_internal_buf, (UINT32)i4_len);
    }

    dlm_digest_final(auc_new_digest);

    dlm_dump_buffer("New HMAC", auc_new_digest, 32);

    if ( x_memcmp(auc_file_digest, auc_new_digest, 32) != 0 )
    {
        DLM_TRACE(("HMAC not matched\n"));
        pt_io->close_source (i4_file);
        return DLM_DIGEST_NOT_MATCH;
    }
    else
        DLM_TRACE(("Signature matched\n"));

    pt_io->close_source (i4_file);
    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_upload
 *
 * Description: Upload device internal data to USB flash for future use.
 *              During upload process, this API make proper header for internal
 *              data and add digest to header to provide codefile intergrity
 *              
 * Inputs:  
 *              pac_tag, which data to upload
 *              pt_user_io, user define I/O callback
 *              ps_path, output file and path  
 * Outputs: No
 * Returns: 
 *          DLM_OK                     : upload data done
 *          DLM_ALLOCATE_MEMORY_FAILED : Allocate memory failed
 *          DLM_OPEN_FILE_FAILED, open file failed
 *          DLM_WRITE_DATA_FAILED, write data failed
 *          DLM_NO_HELPER_REGISTERED, no such data exist
 ----------------------------------------------------------------------------*/
INT32 x_dlm_upload(CHAR *pac_tag, DLM_IO_T *pt_user_io, CHAR *ps_path)
{
    /*
        Algorithm:
        1. search tag in registered list, 
        2. if found, call register callback to get data 
        3. if received length less than register length, end of upload
        4. if received lebgth equal to register length, read more data from user, goto step 3
        5. compute file hash and encrypt it.
        6. output file.

        Note: 
            a) As Jerry.Fan's request, this file does not require ID/Modelname/Version check.
            b) Upload file the control flag is default to ignore
    */
    Append_DATA_TLV_T   t_append_data_tlv; ;
    DLM_APPEND_BLOCK_T *pt_append_data = NULL;
    INT32               i4_out_file;
    INT32               i4_read_len;
    INT32               i4_append_data_size;
    INT32               ui4_total_size = 0;
    INT32               i4_consumed = 0;
    UCHAR               uac_digest[32];
    UINT32              ui4_key = DLM_HDR_KEY;
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T*)puac_internal_buf;
    DLM_IO_T *pt_io = pt_user_io;
    UCHAR       uc_key[16];
    UCHAR       uc_vector[16];
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));

    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    /* testing.... dlm_write has problem */
    /*pt_io->write_data = test_w;*/

    x_memcpy(t_append_data_tlv.uc_type, pac_tag, (SIZE_T)4);
    pt_append_data = dlm_find_append_data_type(&t_append_data_tlv);
    if ( pt_append_data )
    {
        /* count data size */
        pt_append_data->pauc_buffer = (UCHAR*) x_mem_alloc(pt_append_data->ui4_buf_len);
        if ( pt_append_data->pauc_buffer == NULL )
        {
            DLM_TRACE(("Allocate buffer for upload data failed\n"));
            return DLM_ALLOCATE_MEMORY_FAILED;
        }

        DLM_TRACE(("Count append data size\n"));

        i4_append_data_size = 0;

        do
        {
            i4_read_len = pt_append_data->ui4_buf_len;
            if ( pt_append_data->pf_callback(DLM_READ, pt_append_data->pauc_buffer, &i4_read_len) == DLM_OK )
                i4_append_data_size += i4_read_len;
            else
            {
                DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                x_mem_free(pt_append_data->pauc_buffer);

                return DLM_UPLOAD_FAILED;
            }
        } while ( i4_read_len == pt_append_data->ui4_buf_len );

        ui4_total_size = (i4_append_data_size + 12 + sizeof (DEFAULT_CODE_FILE_HDR_T));

        /* Process codefile header */
        DLM_TRACE(("\nBuild header\n"));
        x_memset(pt_hdr, 0, sizeof(DEFAULT_CODE_FILE_HDR_T)); /* clear buffer */

        x_memcpy(pt_hdr, pt_myHdr, sizeof (DEFAULT_CODE_FILE_HDR_T));
        pt_hdr->ui4_total_size = ui4_total_size;  /* pt_hdr->ui4_time =  random assign */
        x_memcpy(pt_hdr->s_magic, DLM_HDR_MAGIC, (SIZE_T)8);  /* my magic */          

#if HDR_ENCRYPTED
        dlm_aes(puac_internal_buf,
                NULL,
                sizeof (DEFAULT_CODE_FILE_HDR_T)-32,
                uc_key,
                uc_vector,
                AES_ENCRYPT);
#endif
        ui4_key = *((UINT32 *) puac_internal_buf); /* the key to encrypt/decrypt append data */

        dlm_digest_init();
        dlm_digest_update(puac_internal_buf, (UINT32) sizeof (DEFAULT_CODE_FILE_HDR_T));

        x_memcpy(puac_internal_buf, (CHAR*)t_append_data_tlv.uc_type, (SIZE_T)4); /* tag */
        x_memset(puac_internal_buf + 4, 0, 4); /* no privacy */
        x_memcpy(puac_internal_buf + 8, (CHAR*) &i4_append_data_size, (SIZE_T)4); /* length */

        x_memcpy(puac_internal_buf + 12, t_append_data_tlv.uc_magic, (SIZE_T)16);
        x_memcpy(puac_internal_buf + 28, t_append_data_tlv.uc_digest,(SIZE_T)32);


        dlm_digest_update(puac_internal_buf, sizeof(Append_DATA_TLV_T));


        /* compute append data digest */
        DLM_TRACE(("Calculate append data digest\n"));

        do
        {
            i4_read_len = pt_append_data->ui4_buf_len;
            if ( pt_append_data->pf_callback(DLM_READ, pt_append_data->pauc_buffer, &i4_read_len) == DLM_OK )
                dlm_digest_update(pt_append_data->pauc_buffer, (UINT32)i4_read_len);
            else
            {
                DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                x_mem_free(pt_append_data->pauc_buffer);

                return DLM_UPLOAD_FAILED;
            }
        }while ( i4_read_len == pt_append_data->ui4_buf_len );

        DLM_TRACE(("\ndigest\n"));
        /* output digest */
        dlm_digest_final(uac_digest);
        
        x_memcpy(&ui4_key, pt_myHdr->uac_id, 4);
        x_memset(uc_vector,0,16);
        x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));
        dlm_dump_buffer("Digest", uac_digest, 32); /* debug */
        dlm_aes(uac_digest,
                NULL,
                32,
                uc_key,
                uc_vector,
                AES_ENCRYPT);

        /* output file */
        i4_out_file = pt_io->open_source(ps_path, kDlm_Append);
        DLM_TRACE(("Output file = %s\n", ps_path));

        if ( i4_out_file == -1 )
        {
            DLM_TRACE(("Generate output file %s failed\n", ps_path));
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_OPEN_FILE_FAILED;
        }

        /* write default file header */
        x_memcpy(pt_hdr, pt_myHdr, sizeof (DEFAULT_CODE_FILE_HDR_T));
        /*pt_hdr->ui4_time =*/
        pt_hdr->ui4_total_size = ui4_total_size;
        x_memcpy(pt_hdr->s_magic, DLM_HDR_MAGIC, (SIZE_T)8);

#if HDR_ENCRYPTED

        ui4_key = DLM_HDR_KEY;
        x_memset(uc_vector, 0, 16);
        x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
        x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));
        dlm_aes(puac_internal_buf,
                NULL,
                sizeof (DEFAULT_CODE_FILE_HDR_T)-32,
                uc_key,
                uc_vector,
                AES_ENCRYPT);
#endif
        ui4_key = *((UINT32 *) puac_internal_buf);

        if ( pt_io->write_data(i4_out_file, (CHAR*)pt_hdr, sizeof (DEFAULT_CODE_FILE_HDR_T)-32) == -1 )
        {
            DLM_TRACE(("write header to file %s failed\n", ps_path));
            pt_io->close_source(i4_out_file);
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_WRITE_DATA_FAILED;
        }

        /* for progress */
        i4_consumed += sizeof (DEFAULT_CODE_FILE_HDR_T)-32;
        if ( dlm_notify_progress )
            dlm_notify_progress(i4_consumed *100 /ui4_total_size);
        else
            DLM_TRACE(("."));

        /* write digest */
        if ( pt_io->write_data(i4_out_file, (CHAR*)uac_digest, 32 ) == -1 )
        {
            DLM_TRACE(("write header to file %s failed\n", ps_path));
            pt_io->close_source(i4_out_file);
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_WRITE_DATA_FAILED;
        }

        i4_consumed += 32;
        if ( dlm_notify_progress )
            dlm_notify_progress(i4_consumed *100 /ui4_total_size);
        else
            DLM_TRACE(("."));

        /* write append data header */
        x_memcpy(puac_internal_buf, (CHAR*)t_append_data_tlv.uc_type, (SIZE_T)4); /* tag */
        x_memset(puac_internal_buf + 4, 0, 4); /* no privacy */
        x_memcpy(puac_internal_buf + 8, (CHAR*) &i4_append_data_size, (SIZE_T)4); /* length */

        x_memcpy(puac_internal_buf + 12, t_append_data_tlv.uc_magic, (SIZE_T)16);
        x_memcpy(puac_internal_buf + 28, t_append_data_tlv.uc_digest,(SIZE_T)32);

        if ( pt_io->write_data(i4_out_file, (CHAR*)puac_internal_buf, sizeof(Append_DATA_TLV_T) ) == -1 )
        {
            DLM_TRACE(("write header to file %s failed\n", ps_path));
            pt_io->close_source(i4_out_file);
            DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
            x_mem_free(pt_append_data->pauc_buffer);
            return DLM_WRITE_DATA_FAILED;
        }

        i4_consumed += sizeof(Append_DATA_TLV_T);
        if ( dlm_notify_progress )
            dlm_notify_progress(i4_consumed *100 /ui4_total_size);
        else
            DLM_TRACE(("."));

        /* write append data */
        do
        {
            i4_read_len = pt_append_data->ui4_buf_len;
            if ( pt_append_data->pf_callback(DLM_READ, pt_append_data->pauc_buffer, &i4_read_len) == DLM_OK )
            {
                if ( i4_read_len && pt_io->write_data(i4_out_file, (CHAR*)pt_append_data->pauc_buffer, i4_read_len) == -1 )
                {
                    DLM_TRACE(("write data to file %s failed\n", ps_path));
                    pt_io->close_source(i4_out_file);
                    DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                    x_mem_free(pt_append_data->pauc_buffer);

                    return DLM_WRITE_DATA_FAILED;
                }

                i4_consumed += i4_read_len;
                if ( dlm_notify_progress )
                    dlm_notify_progress(i4_consumed *100 /ui4_total_size);
                else
                    DLM_TRACE(("."));

            }
            else
            {
                DLM_TRACE(("Upload failed, free buffer = %d\n", pt_append_data->ui4_buf_len));
                x_mem_free(pt_append_data->pauc_buffer);

                pt_io->close_source(i4_out_file);
                return DLM_UPLOAD_FAILED;
            }
        } while ( i4_read_len == pt_append_data->ui4_buf_len );

        pt_io->close_source(i4_out_file);

        DLM_TRACE(("Upload done, free buffer = %d\n", pt_append_data->ui4_buf_len));
        x_mem_free(pt_append_data->pauc_buffer);

        return DLM_OK;
    }
    else
    {
        DLM_TRACE(("dlm: no such data exist.\n"));
        return DLM_NO_HELPER_REGISTERED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_download_by_tag
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ps_file,  codefile name
 *          puc_tag,   user defined tag
 *          pt_user_io, user specify I/O
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note: 
 *    Before this function is called, the codefile need pass integrity check
 ----------------------------------------------------------------------------*/
INT32 x_dlm_download_by_tag(CHAR *ps_file, UCHAR *puc_tag, DLM_IO_T *pt_user_io)
{
    INT32 i4_file;
    DLM_APPEND_BLOCK_T *pt_append_data_block = NULL;
    Append_DATA_TLV_T   t_append_data_tlv; 
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;

    INT32       i4_append_len = 0;
    INT32       i4_buffer_len = 0;
    INT32       i4_append_buf_remain = 0;
    INT32       i4_append_buf_content_size = 0;
    UCHAR      *p_buf_offset = NULL;
    UCHAR      *p_app_offset = NULL;
    INT32       i4_ret;
    DLM_IO_T*   pt_io = pt_user_io;
    UINT32      ui4_tag_size = 0;
    UINT32      ui4_tag_consume_size = 0;
    UINT32      ui4_tag_percentage = 0;
    FLOAT       f_percent = 0;
    INT32       i4_flag = 0;
    UCHAR       uc_key[16];
    UCHAR       uc_vector[16];
    UINT32      ui4_key = DLM_HDR_KEY;
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    
    if ( pt_io == NULL )
        pt_io = x_dlm_get_default_io();


    /* locate header */
    i4_ret = dlm_locate_codefile_header(ps_file, pt_user_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", ps_file));
        return i4_ret;
    }

    /* check tag relation */
    x_memcpy(t_append_data_tlv.uc_type, puc_tag, (SIZE_T)4);
    pt_append_data_block = dlm_find_append_data_type(&t_append_data_tlv);
    if ( pt_append_data_block )
    {
        i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
        pt_append_data_block->pauc_buffer = (UCHAR *)x_mem_alloc (pt_append_data_block->ui4_buf_len);
        if ( pt_append_data_block->pauc_buffer == NULL )
        {
            return DLM_ALLOCATE_MEMORY_FAILED;
        }
        p_app_offset = pt_append_data_block->pauc_buffer;
    }
    else
    {
        return DLM_UNKONW_TAG;
    }

    /* open source */
    if ( (i4_file = pt_io->open_source(ps_file, kDlm_Read)) == -1 )
    {
        x_mem_free(pt_append_data_block->pauc_buffer);
        return DLM_OPEN_FILE_FAILED;
    }

    /* our internal header length */
    i4_ret = pt_io->read_data(i4_file, (CHAR *)puac_internal_buf, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset);
    if ( i4_ret == -1 || i4_ret < sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset )
    {
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_io->close_source(i4_file);
        return DLM_READ_FILE_FAILED;
    }

#if HDR_ENCRYPTED
    pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf + i4_codefile_header_offset);

    dlm_aes(puac_internal_buf + i4_codefile_header_offset,
            NULL,
            (UINT32)(sizeof(DEFAULT_CODE_FILE_HDR_T)-32),
            uc_key,
            uc_vector,
            AES_DECRYPT);
#endif
    /* quick check, if magic not match, skip download */
    if ( x_memcmp(pt_hdr->s_magic, DLM_HDR_MAGIC, 8)!=0 )
    {
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_io->close_source(i4_file);
        return DLM_HDR_MAGIC_NOT_MATCH;
    }

    /* backup download hdr for future user */
    dlm_backup_download_hdr();

    /* locate tag */
    do
    {
        i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, sizeof(Append_DATA_TLV_T));
        if ( i4_buffer_len != sizeof(Append_DATA_TLV_T) )
        {
            DLM_TRACE(("locate tag failed\n"));
            pt_io->close_source(i4_file);
            x_mem_free(pt_append_data_block->pauc_buffer);
            return DLM_READ_FILE_FAILED;
        }
        /* compare tag */
        i4_buffer_len =  puac_internal_buf[11] * 256 * 256 * 256 + 
                         puac_internal_buf[10] * 256 * 256 + 
                         puac_internal_buf[9] * 256 + 
                         puac_internal_buf[8];

        DLM_TRACE(("Append file length = %d\n", i4_buffer_len));

        if ( x_memcmp(puac_internal_buf, puc_tag,4) != 0 )
        { 
            /* seek to next tag */
            if ( pt_io->seek_source(i4_file, (UINT32)i4_buffer_len) != 0 )
            {
                DLM_TRACE(("Seek tag failed\n"));
                pt_io->close_source(i4_file);
                x_mem_free(pt_append_data_block->pauc_buffer);
                return DLM_READ_FILE_FAILED;
            }
        }
        else
        {
            /* tag match */
            i4_append_len = i4_buffer_len;
            /* for tag progress use */
            ui4_tag_size = i4_append_len;
            ui4_tag_consume_size = 0;
            ui4_tag_percentage = 0;

            i4_buffer_len = 0;
            x_memcpy(&t_append_data_tlv, puac_internal_buf, (SIZE_T)sizeof(Append_DATA_TLV_T));
            x_memset(uc_vector, 0, 16);
            x_memcpy(uc_key, pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + 2*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + 3*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            dlm_aes(t_append_data_tlv.uc_magic,
                    NULL,
                    16,
                    uc_key,
                    uc_vector,
                    AES_DECRYPT);
            if(x_memcmp(t_append_data_tlv.uc_magic, DLM_APPEND_MAGIC,16) != 0)
            {
                DLM_TRACE(("Append Magic check failed\n"));
                pt_io->close_source(i4_file);
                x_mem_free(pt_append_data_block->pauc_buffer);
                return DLM_HDR_MAGIC_NOT_MATCH;
            }
            dlm_aes(t_append_data_tlv.uc_digest,
                    NULL,
                    32,
                    uc_key,
                    uc_vector,
                    AES_DECRYPT);
            
            i4_flag = 1;
            break;
        }
    } while ( 1 );

    if ( i4_flag == 0 )
        return DLM_UNKONW_TAG;
    
    while ( i4_append_len )
    {
        /* renew buffer */
        if ( i4_buffer_len == 0 )
        {
            if ( !pt_io->eof_data(i4_file) )
                i4_buffer_len = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, ui4_internal_buf_size);
            else
                return DLM_READ_FILE_FAILED; /* ??? */

            /* error check here */
            if ( i4_buffer_len == -1 || i4_buffer_len == 0 )
            {
                if ( i4_buffer_len == -1 )
                {
                    return DLM_READ_FILE_FAILED;
                }
                else
                {
                    /* end of file? try again... */
                    continue;
                }
            }
            p_buf_offset = puac_internal_buf;
        }
        
        /* dispath this data to append data call back */
        if ( i4_append_buf_remain )
        {
            if ( i4_append_len > i4_append_buf_remain )
            {
                /* more space need */
                if ( i4_buffer_len >= i4_append_buf_remain )
                {
                    /* fill whole remain space */
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_append_buf_remain);
                    p_buf_offset += i4_append_buf_remain;
                    i4_append_len -= i4_append_buf_remain;
                    i4_buffer_len -= i4_append_buf_remain;
                    i4_append_buf_content_size += i4_append_buf_remain;
                    i4_append_buf_remain = 0;
                }
                else
                {
                    /* i4_buffer_len < i4_append_buf_remain */
                    /* move as much as possible */
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_buffer_len);
                    p_app_offset += i4_buffer_len;
                    i4_append_buf_remain -= i4_buffer_len;
                    i4_append_len -= i4_buffer_len;
                    i4_append_buf_content_size += i4_buffer_len;
                    i4_buffer_len = 0;
                }
            }
            else
            {
                /* i4_append_len < i4_append_buf_remain */
                /* this is last apppend data in this block */
                if ( i4_buffer_len >= i4_append_len )
                {
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_append_len);
                    p_buf_offset += i4_append_len;
                    i4_buffer_len -= i4_append_len;
                    i4_append_buf_content_size += i4_append_len;
                    i4_append_len = 0;
                    i4_append_buf_remain = 0;
                }
                else
                {
                    x_memcpy(p_app_offset, p_buf_offset, (SIZE_T)i4_buffer_len);
                    p_app_offset += i4_buffer_len;
                    i4_append_len -= i4_buffer_len;
                    i4_append_buf_remain -= i4_buffer_len;
                    i4_append_buf_content_size += i4_buffer_len;
                    i4_buffer_len = 0;
                }
            }
        }

        if ( i4_append_buf_remain == 0 )
        {
            /* if append data is scrambled then data buffer need decrypted */
            if ( t_append_data_tlv.uc_privacy[0] )
            {
                dlm_aes(pt_append_data_block->pauc_buffer,
                        NULL,
                        (UINT32)i4_append_buf_content_size,
                        uc_key,
                        uc_vector,
                        AES_DECRYPT);
            }
            if ( pt_append_data_block->pf_callback != NULL )
            {
                i4_ret = pt_append_data_block->pf_callback(DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
            }

            if ( pt_append_data_block->pf_callback2 != NULL )
            {
                i4_ret = pt_append_data_block->pf_callback2(pt_append_data_block->uc_type, DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
            }

            ui4_tag_consume_size += i4_append_buf_content_size;
            f_percent = (FLOAT)((FLOAT)ui4_tag_consume_size/(FLOAT)ui4_tag_size);
            ui4_tag_percentage = (UINT32)(f_percent*100);

            /* current tag progress */
            if ( dlm_notify_tag_progress )
                dlm_notify_tag_progress(ui4_tag_percentage);
            else
                DLM_TRACE(("download %c%c%c%c percentage = %d\n", 
                           t_append_data_tlv.uc_type[0],
                           t_append_data_tlv.uc_type[1],
                           t_append_data_tlv.uc_type[2],
                           t_append_data_tlv.uc_type[3], 
                           ui4_tag_percentage));

            i4_append_buf_content_size = 0;

            if ( i4_ret != DLM_OK )
            { /* in case DLM_OK is set to '0' */
                DLM_TRACE(("Call back return %d\n", (signed int) i4_ret));

                /* free allocate buffer */
                if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
                {
                    x_mem_free(pt_append_data_block->pauc_buffer);
                    pt_append_data_block->pauc_buffer = NULL;
                }
                pt_io->close_source(i4_file);
                return i4_ret;
            }

            /* renew buffer */
            i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
            p_app_offset = pt_append_data_block->pauc_buffer;
        }
    }

    /* free allocate buffer */
    if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
    {
        DLM_TRACE(("\nfree memory size = %d\n", pt_append_data_block->ui4_buf_len));
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_append_data_block->pauc_buffer = NULL;
    }

    pt_io->close_source (i4_file);
    return DLM_OK;
}

INT32 x_dlm_download_by_division(UINT32 ui4_len, UCHAR*  puc_buffer, UCHAR* puc_tag)
{
    DLM_APPEND_BLOCK_T *pt_append_data_block = NULL;
    Append_DATA_TLV_T   t_append_data_tlv; 
    INT32               i4_buffer_len = 0;
    INT32               i4_append_len = 0;
    INT32               i4_append_buf_remain = 0;
    INT32               i4_append_buf_content_size = 0;
    UINT32              ui4_tag_size;
    UINT32              ui4_tag_consume_size;
    UINT32              ui4_tag_percentage;
    FLOAT               f_percent = 0;
    UCHAR               uc_key[16];
    UCHAR               uc_vector[16];
    UCHAR               uc_vector_back[16];
    UCHAR               uc_digest[32];
    UCHAR               *p_buf_offset = NULL;
    UCHAR               *p_app_offset = NULL;
    INT32               i4_ret = DLM_OK;
    if (puc_buffer == NULL || puc_tag == NULL || ui4_len <= sizeof(Append_DATA_TLV_T))
    {
        return DLM_ARGV_INVALID;
    }
    if ( x_memcmp(puc_buffer, puc_tag,4) != 0 )
    {
        /* check tag */
        DLM_TRACE(("Seek tag failed\n"));
        return DLM_UNKONW_TAG;
    }
    
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, pt_myHdr->uac_id, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));

    p_buf_offset = puc_buffer;
    
    i4_buffer_len =  puc_buffer[11] * 256 * 256 * 256 + 
                     puc_buffer[10] * 256 * 256 + 
                     puc_buffer[9] * 256 + 
                     puc_buffer[8];
    if ((i4_buffer_len + sizeof(Append_DATA_TLV_T) != ui4_len))
    {
        /* Buffer length error */
        DLM_TRACE(("Seek tag failed\n"));
        return DLM_DIVISION_SIZE_ERROR;
    }   
    
    i4_append_len = i4_buffer_len;

    /* for tag progress use */
    ui4_tag_size = ui4_len;
    ui4_tag_consume_size = 0;
    ui4_tag_percentage = 0;

    x_memcpy(&t_append_data_tlv, puc_buffer, sizeof(Append_DATA_TLV_T));
    p_buf_offset += sizeof(Append_DATA_TLV_T);
    ui4_tag_consume_size += sizeof(Append_DATA_TLV_T);

    dlm_aes(t_append_data_tlv.uc_magic,
            NULL,
            16,
            uc_key,
            uc_vector,
            AES_DECRYPT);
    if(x_memcmp(t_append_data_tlv.uc_magic, DLM_APPEND_MAGIC,16) != 0)
    {
        DLM_TRACE(("Append Magic check failed\n"));
        return DLM_HDR_MAGIC_NOT_MATCH;
    }
    dlm_dump_buffer("[DLM]Append digest:", t_append_data_tlv.uc_digest, 32);
    dlm_aes(t_append_data_tlv.uc_digest,
            NULL,
            32,
            uc_key,
            uc_vector,
            AES_DECRYPT);
    x_memcpy(uc_vector_back, uc_vector, 16);
    dlm_dump_buffer("[DLM]Append Decrypt digest:", t_append_data_tlv.uc_digest, 32);
    dlm_digest_init();
    
    /* check tag relation */

    pt_append_data_block = dlm_find_append_data_type(&t_append_data_tlv);
    if ( pt_append_data_block )
    {
        i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
        pt_append_data_block->pauc_buffer = (UCHAR *)x_mem_alloc (pt_append_data_block->ui4_buf_len);
        if ( pt_append_data_block->pauc_buffer == NULL )
        {
            return DLM_ALLOCATE_MEMORY_FAILED;
        }
        p_app_offset = pt_append_data_block->pauc_buffer;
    }
    else
    {
        return DLM_UNKONW_TAG;
    }
    while(1)
    {
        if (i4_append_len == 0)
        {
            break;
        }
        if (i4_append_len <= i4_append_buf_remain)
        {
            x_memcpy(p_app_offset,p_buf_offset,i4_append_len);
            p_app_offset += i4_append_len;
            p_buf_offset += i4_append_len;
            i4_append_buf_content_size = i4_append_len;   
            i4_append_buf_remain = 0;
            i4_append_len = 0;
        }
        else
        {
            x_memcpy(p_app_offset,p_buf_offset,i4_append_buf_remain);
            i4_append_buf_content_size = i4_append_buf_remain;
            p_app_offset += i4_append_buf_remain;
            p_buf_offset += i4_append_buf_remain;
            i4_append_len -=  i4_append_buf_remain;
            i4_append_buf_remain = 0;
        }
        if ( i4_append_buf_remain == 0)
        {
            /* if append data is scrambled then data buffer need decrypted */

            
            if ( t_append_data_tlv.uc_privacy[0] )
            {
                dlm_aes(pt_append_data_block->pauc_buffer,
                        NULL,
                        (UINT32)i4_append_buf_content_size,
                        uc_key,
                        uc_vector,
                        AES_DECRYPT);
            }
            dlm_digest_update(pt_append_data_block->pauc_buffer,i4_append_buf_content_size);

            ui4_tag_consume_size += i4_append_buf_content_size;
            ui4_tag_percentage = ui4_tag_consume_size*100 / ui4_tag_size;
            i4_append_buf_content_size = 0;
            /* current tag progress */
            
            DLM_TRACE(("digest check %c%c%c%c percentage = %d\n", 
                       t_append_data_tlv.uc_type[0],
                       t_append_data_tlv.uc_type[1],
                       t_append_data_tlv.uc_type[2],
                       t_append_data_tlv.uc_type[3], 
                       ui4_tag_percentage));

            if ( i4_ret != DLM_OK )
            { /* in case DLM_OK is set to '0' */
                DLM_TRACE(("Call back return %d\n", (signed int) i4_ret));

                /* free allocate buffer */
                if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
                {
                    x_mem_free(pt_append_data_block->pauc_buffer);
                    pt_append_data_block->pauc_buffer = NULL;
                }
                return i4_ret;
            }

            /* renew buffer */
            i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
            p_app_offset = pt_append_data_block->pauc_buffer;
            
        }
    }
    
    dlm_digest_final(uc_digest);
    dlm_dump_buffer("[DLM]Append digest:", t_append_data_tlv.uc_digest, 32);
    if (x_memcmp(uc_digest, t_append_data_tlv.uc_digest, 32) !=0 )
    {
        x_dbg_stmt("[DLM]Append digest not match");
        return DLM_DIGEST_NOT_MATCH;
    }
    
    i4_append_len = i4_buffer_len;
    p_buf_offset = puc_buffer + sizeof(Append_DATA_TLV_T);
    ui4_tag_consume_size = 0;
    i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
    p_app_offset = pt_append_data_block->pauc_buffer;
    x_memcpy(uc_vector, uc_vector_back, 16);
    while(1)
    {
        if (i4_append_len == 0)
        {
            break;
        }
        if (i4_append_len <= i4_append_buf_remain)
        {
            x_memcpy(p_app_offset,p_buf_offset,i4_append_len);
            p_app_offset += i4_append_len;
            p_buf_offset += i4_append_len;
            i4_append_buf_content_size = i4_append_len;   
            i4_append_buf_remain = 0;
            i4_append_len = 0;
        }
        else
        {
            x_memcpy(p_app_offset,p_buf_offset,i4_append_buf_remain);
            i4_append_buf_content_size = i4_append_buf_remain;
            p_app_offset += i4_append_buf_remain;
            p_buf_offset += i4_append_buf_remain;
            i4_append_len -=  i4_append_buf_remain;
            i4_append_buf_remain = 0;
        }
        if ( i4_append_buf_remain == 0)
        {
            /* if append data is scrambled then data buffer need decrypted */
            if ( t_append_data_tlv.uc_privacy[0] )
            {
                dlm_aes(pt_append_data_block->pauc_buffer,
                        NULL,
                        (UINT32)i4_append_buf_content_size,
                        uc_key,
                        uc_vector,
                        AES_DECRYPT);
            }
            if ( pt_append_data_block->pf_callback != NULL )
            {
                i4_ret = pt_append_data_block->pf_callback(DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
            }

            if ( pt_append_data_block->pf_callback2 != NULL )
            {
                i4_ret = pt_append_data_block->pf_callback2(pt_append_data_block->uc_type, DLM_WRITE, pt_append_data_block->pauc_buffer, &i4_append_buf_content_size);
            }

            ui4_tag_consume_size += i4_append_buf_content_size;
            f_percent = (FLOAT)((FLOAT)ui4_tag_consume_size / (FLOAT)ui4_tag_size);
            ui4_tag_percentage = (UINT32)(f_percent*100);
            i4_append_buf_content_size = 0;
            /* current tag progress */
            if ( dlm_notify_tag_progress )
                dlm_notify_tag_progress(ui4_tag_percentage);
            else
                DLM_TRACE(("download %c%c%c%c percentage = %d\n", 
                           t_append_data_tlv.uc_type[0],
                           t_append_data_tlv.uc_type[1],
                           t_append_data_tlv.uc_type[2],
                           t_append_data_tlv.uc_type[3], 
                           ui4_tag_percentage));

            if ( i4_ret != DLM_OK )
            { /* in case DLM_OK is set to '0' */
                DLM_TRACE(("Call back return %d\n", (signed int) i4_ret));

                /* free allocate buffer */
                if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
                {
                    x_mem_free(pt_append_data_block->pauc_buffer);
                    pt_append_data_block->pauc_buffer = NULL;
                }
                return i4_ret;
            }

            /* renew buffer */
            i4_append_buf_remain = pt_append_data_block->ui4_buf_len;
            p_app_offset = pt_append_data_block->pauc_buffer;
            
        }
    }
    /* free allocate buffer */
    if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
    {
        DLM_TRACE(("\nfree memory size = %d\n", pt_append_data_block->ui4_buf_len));
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_append_data_block->pauc_buffer = NULL;
    }
    return DLM_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_tag_test
 *
 * Description: This API is use to download specify append data
 *
 * Inputs:  ps_file,  codefile name
 *          puac_tag, user specify tag name
 *          pac_buffer, the buffer to store downloaded data
 *          i4_buf_size, pac_buffer size
 * Outputs: NA
 * Returns: DLM_OK, 
 * 
 * Note, 
 *      Due to crypher tool limit, the i4_buf_size must to be divisible by 4
 ----------------------------------------------------------------------------*/
INT32 x_dlm_tag_test(CHAR *ps_file, UCHAR *puac_tag, DLM_IO_T *pt_user_io, CHAR *pac_buffer, INT32 i4_buf_size)
{
    INT32 i4_file;
    INT32 i4_privacy = 0;
    INT32 i4_ret;
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puac_internal_buf;
    DLM_IO_T*   pt_io = pt_user_io;
    UCHAR       uc_key[16];
    UCHAR       uc_vector[16];
    UCHAR       uc_magic[16];
    UCHAR       uc_digest[32];
    UINT32      ui4_key = DLM_HDR_KEY;
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key, &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32), &ui4_key, sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32), &ui4_key, sizeof(UINT32));


    if ( pt_io == NULL )
        pt_io = x_dlm_get_default_io();

    /* locate header */
    i4_ret = dlm_locate_codefile_header(ps_file, pt_io);
    if ( i4_ret != DLM_OK )
    {
        DLM_TRACE(("Locate codefile header failed - %s\n", ps_file));
        return i4_ret;
    }

    /* open source */
    if ( (i4_file = pt_io->open_source(ps_file, kDlm_Read)) == -1 )
    {
        return DLM_OPEN_FILE_FAILED;
    }

    /* our internal header length */
    i4_ret = pt_io->read_data(i4_file, (CHAR *)puac_internal_buf, sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset);
    if ( i4_ret == -1 || i4_ret < sizeof(DEFAULT_CODE_FILE_HDR_T)+i4_codefile_header_offset )
    {
        pt_io->close_source(i4_file);
        return DLM_READ_FILE_FAILED;
    }

#if HDR_ENCRYPTED
    pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) (puac_internal_buf+i4_codefile_header_offset);

    dlm_aes(puac_internal_buf+i4_codefile_header_offset,
            NULL,
            (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32),
            uc_key,
            uc_vector,
            AES_DECRYPT);
#endif
    /* quick check, if magic not match, skip download */
    if ( x_memcmp(pt_hdr->s_magic, DLM_HDR_MAGIC, 8)!=0 )
    {
        pt_io->close_source(i4_file);
        return DLM_HDR_MAGIC_NOT_MATCH;
    }

    /* backup download hdr for future user */
    dlm_backup_download_hdr();

    /* locate tag */
    do
    {
        i4_ret = pt_io->read_data(i4_file, (CHAR*)puac_internal_buf, 12);
        if ( i4_ret != 12 )
        {
            DLM_TRACE(("locate tag failed\n"));
            pt_io->close_source(i4_file);
            return DLM_READ_FILE_FAILED;
        }
        /* compare tag */
        i4_ret =  puac_internal_buf[11] * 256 * 256 * 256 + 
                  puac_internal_buf[10] * 256 * 256 + 
                  puac_internal_buf[9] * 256 + 
                  puac_internal_buf[8];

        DLM_TRACE(("[DLM]Append tag %c%c%c%c length = %d\n",
                    puac_internal_buf[0],
                    puac_internal_buf[1],
                    puac_internal_buf[2],
                    puac_internal_buf[3], 
                    i4_ret));

        if ( x_memcmp(puac_internal_buf, puac_tag,4) != 0 )
        {   
            if ( pt_io->seek_source(i4_file, (UINT32)48) != 0 )
            {
                DLM_TRACE(("Seek tag failed\n"));
                pt_io->close_source(i4_file);
                return DLM_READ_FILE_FAILED;
            }
            /* seek to next tag */
            if ( pt_io->seek_source(i4_file, (UINT32)i4_ret) != 0 )
            {
                DLM_TRACE(("Seek tag failed\n"));
                pt_io->close_source(i4_file);
                return DLM_READ_FILE_FAILED;
            }
        }
        else
        {
            /* tag match */
            if ( puac_internal_buf[4] )
            {
                i4_privacy = 1;
            }

            i4_ret = pt_io->read_data(i4_file, (CHAR*)uc_magic, 16);
            x_memcpy(uc_key, pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + 2*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            x_memcpy(uc_key + 3*sizeof(UINT32), pt_myHdr->uac_id, sizeof(UINT32));
            dlm_aes(uc_magic,
                    NULL,
                    16,
                    uc_key,
                    uc_vector,
                    AES_DECRYPT);
            if (x_memcmp(uc_magic,DLM_APPEND_MAGIC,16)!=0)
            {
                DLM_TRACE(("Compare Append magic failed\n"));
                pt_io->close_source(i4_file);
                return DLM_READ_FILE_FAILED;
            }
            i4_ret = pt_io->read_data(i4_file, (CHAR*)uc_digest, 32);
            dlm_aes(uc_digest,
                    NULL,
                    32,
                    uc_key,
                    uc_vector,
                    AES_DECRYPT);
            break;
        }
    } while ( 1 );


    i4_ret = pt_io->read_data(i4_file, (CHAR*)pac_buffer, i4_buf_size);
    if ( i4_privacy )
    {
        dlm_aes((UCHAR*)pac_buffer,
                NULL,
                i4_ret,
                uc_key,
                uc_vector,
                AES_DECRYPT);
    }

    pt_io->close_source (i4_file);
    return DLM_OK;
}

/* To identify the code file header */
INT32 dlm_locate_codefile_header(CHAR *filename, DLM_IO_T *pt_user_io)
{
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)puac_internal_buf;
    INT32 i4_ret;
    UCHAR uc_key[16];
    UCHAR uc_vector[16];
    UINT32 ui4_key = DLM_HDR_KEY;
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key,&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32),&ui4_key,sizeof(UINT32));


    i4_ret = dlm_read_code_header(filename, pt_user_io, sizeof (DEFAULT_CODE_FILE_HDR_T));
    if ( i4_ret != DLM_OK )
    {
        return  i4_ret;
    }

    /* check magic */
#if HDR_ENCRYPTED
    x_dbg_stmt("Header encrypt by AES\n");
	dlm_dump_buffer("Header :",puac_internal_buf,(UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32));
    dlm_aes(puac_internal_buf,
            NULL,
            (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32),
            uc_key,
            uc_vector,
            AES_DECRYPT);
    dlm_dump_buffer("Header :",puac_internal_buf,(UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32));

#endif
    if ( x_memcmp(pt_file_hdr->s_magic, DLM_HDR_MAGIC, 8) == 0 )
    {
        i4_codefile_header_offset = 0;
        return DLM_OK;
    }
    
    x_dbg_stmt("File Header Magic check failed \n");
    /* has extra header in codefile */
    i4_ret = dlm_read_code_header(filename, pt_user_io, sizeof (DEFAULT_CODE_FILE_HDR_T) + DLM_PLAIN_TEXT_OFFSET_PHILIPS);
    if ( i4_ret != DLM_OK )
    {
        return  i4_ret;
    }

    /* check magic */
#if HDR_ENCRYPTED
    x_memset(uc_vector, 0, 16);
    x_memcpy(uc_key,&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 2*sizeof(UINT32),&ui4_key,sizeof(UINT32));
    x_memcpy(uc_key + 3*sizeof(UINT32),&ui4_key,sizeof(UINT32));

    dlm_aes(puac_internal_buf + DLM_PLAIN_TEXT_OFFSET_PHILIPS,
            NULL,
            (UINT32) (sizeof(DEFAULT_CODE_FILE_HDR_T)-32),
            uc_key,
            uc_vector,
            AES_DECRYPT);

#endif
    pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T*) (puac_internal_buf + DLM_PLAIN_TEXT_OFFSET_PHILIPS);

    if ( x_memcmp(pt_file_hdr->s_magic, DLM_HDR_MAGIC, 8) == 0 )
    {
        i4_codefile_header_offset = DLM_PLAIN_TEXT_OFFSET_PHILIPS;
        return DLM_OK;
    }

    return DLM_DIGEST_NOT_MATCH;
}

#endif
/*-----------------------------------------------------------------------------
 * Name: dlm_parser_new
 *
 * Description: This API provide user to create DLM_APPEND_BLOCK_T to store
 *              append data information
 *
 * Inputs:  NA
 * Outputs: NA
 * Returns: new DLM_APPEND_BLOCK_T
 ----------------------------------------------------------------------------*/
static DLM_APPEND_BLOCK_T* dlm_parser_new(VOID)
{
    DLM_APPEND_BLOCK_T* pt_new;

    if ( (pt_new = (DLM_APPEND_BLOCK_T*) x_mem_alloc (sizeof (DLM_APPEND_BLOCK_T))) == NULL )
    {
        DLM_TRACE(("Allocate memory failed\n"));
        return NULL;
    }
    x_memset(pt_new, 0, sizeof (DLM_APPEND_BLOCK_T));

    return pt_new;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_find_append_data_type
 *
 * Description: The API use user specify tag to find out they registered callback
 *
 * Inputs:  pt_append_data_tlv, user defined tag
 * Outputs: NA
 * Returns: NULL, not found register tag,
 *          others, the append data callback
 ----------------------------------------------------------------------------*/
static DLM_APPEND_BLOCK_T *dlm_find_append_data_type(Append_DATA_TLV_T *pt_append_data_tlv)
{
    DLM_APPEND_BLOCK_T *pt_entry = pt_append_data_callback_list;

    while ( pt_entry )
    {
        if ( x_memcmp(pt_entry->uc_type, pt_append_data_tlv->uc_type, 4) == 0 )
            return pt_entry;
        pt_entry = pt_entry->pt_next;
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_validate_id
 *
 * Description: This API provide code file ID validate
 *
 * Inputs:  Code file Header start address
 * Outputs: no
 * Returns: DLM_OK, the download file has same ID as we defined
 *          DLM_ID_NOT_MATCH, the download file ID not match 
 ----------------------------------------------------------------------------*/
static INT32 dlm_validate_id(UCHAR *puc_hdr)
{
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puc_hdr;

    /* if disable ID check */
    if ( uc_g_flag & kValidateID )
    {
        return DLM_OK;
    }

    DLM_TRACE(("Validate ID "));
    if ( x_memcmp(pt_hdr->uac_id, pt_myHdr->uac_id, 4) == 0 )
    {
        DLM_TRACE(("- ok\n"));
        return DLM_OK;
    }

    DLM_TRACE(("- failed\n"));

    return DLM_ID_NOT_MATCH;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_validate_version
 *
 * Description: This API provide validate the firmware version
 *              The value is store as big endian
 * Inputs:  Code file Header start address
 * Outputs: no
 * Returns: DLM_OK, the download firmware is more fresh than current running
 *          DLM_VERSION_NOT_MATCH, firmware version is more old than current running
 * note:
 *       firmware version like this "CQAM_IDTV_001005_1"
 ----------------------------------------------------------------------------*/
static INT32  dlm_validate_version(UCHAR *puc_hdr)
{
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puc_hdr;
    INT32 i4_upgrade;
    INT32 i4_ver_small;
    CHAR s_my_version[64];
    INT32 i4_version;
    CHAR *ps_token;

    struct version_s
    {
        INT32 i4_version;
        struct version_s *pt_next;
    };

    struct version_s *pt_my_ver = NULL, *pt_dl_ver = NULL, *pt_my_tmp, *pt_new_tmp;

    /* if disable version check */
    if ( uc_g_flag & kValidateVersion )
    {
        return DLM_OK;
    }

    /* if user defined compare API */
    if ( dlm_ver_compare_fn )
    {
        return dlm_ver_compare_fn((CHAR*)pt_hdr->s_version);
    }

    /* get my version */
#ifdef NO_MIDDLEWARE
    x_strcpy(s_my_version, "CQAM_IDTV_001027_006_1_3_100");
#else
    x_strcpy(s_my_version, x_sys_get_version());
#endif

    DLM_TRACE(("Current  firmware version - %s\n", s_my_version));
    DLM_TRACE(("Download firmware version - %s\n", pt_hdr->s_version)); /* ha, its better wiht '0' terminal */

    /* build my version list */
    ps_token = dlm_strtok(s_my_version, "_\n,.");
    while ( ps_token )
    {
        struct version_s *pt_new;
#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
        i4_version = simple_strtoul(ps_token, NULL, 10);
#else /* CC_UBOOT */
#ifdef CC_MTK_LOADER
        i4_version = StrToInt(ps_token);
#else /* CC_MTK_LOADER */
        i4_version =  atoi(ps_token);
#endif /* CC_MTK_LOADER */
#endif /* CC_UBOOT */
#else
        i4_version =  (INT32) x_strtoll(ps_token, NULL, 10);
#endif        
        pt_new = (struct version_s*) x_mem_alloc(sizeof(struct version_s));
        if (pt_new == NULL)
        {
            /* should be error, just return not match let system continue running */
            return DLM_VERSION_NOT_MATCH;
        }

        pt_new->i4_version = i4_version;
        pt_new->pt_next = NULL;
        if ( pt_my_ver )
        {
            struct version_s *pt_last = pt_my_ver;
            while ( pt_last )
            {
                if ( pt_last->pt_next == NULL )
                    break;
                pt_last = pt_last->pt_next;
            }
            pt_last->pt_next = pt_new;
        }
        else
            pt_my_ver = pt_new;

        ps_token = dlm_strtok(NULL, "_\n,.");
    }

    /* build download version list */
    x_strcpy(s_my_version, pt_hdr->s_version);
    ps_token = dlm_strtok(s_my_version, "_\n,.");
    while ( ps_token )
    {
        struct version_s *pt_new;
#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
        i4_version = simple_strtoul(ps_token, NULL, 10);
#else /* CC_UBOOT */
#ifdef CC_MTK_LOADER
        i4_version = StrToInt(ps_token);
#else /* CC_MTK_LOADER */
        i4_version =  atoi(ps_token);
#endif /* CC_MTK_LOADER */
#endif /* CC_UBOOT */
#else        
        i4_version =  (INT32) x_strtoll(ps_token, NULL, 10);
#endif        
        pt_new = (struct version_s*) x_mem_alloc(sizeof(struct version_s));
        if (pt_new == NULL)
        {
            return DLM_VERSION_NOT_MATCH;
        }
        pt_new->i4_version = i4_version;
        pt_new->pt_next = NULL;
        if ( pt_dl_ver )
        {
            struct version_s *pt_last = pt_dl_ver;

            while ( pt_last )
            {
                if ( pt_last->pt_next == NULL )
                    break;
                pt_last = pt_last->pt_next;
            }
            pt_last->pt_next = pt_new;
        }
        else
            pt_dl_ver = pt_new;

        ps_token = dlm_strtok(NULL, "_\n,.");
    }

    /* do version compare */
    pt_my_tmp = pt_my_ver; pt_new_tmp = pt_dl_ver; i4_upgrade = 0; i4_ver_small = 0;
    while ( pt_my_tmp && pt_new_tmp )
    {
        if ( pt_new_tmp->i4_version > pt_my_tmp->i4_version )
        {
            if ( i4_ver_small == 0 )
            {
                i4_upgrade = 1;
                break;
            }
        }
        else if ( pt_new_tmp->i4_version < pt_my_tmp->i4_version )
        {
            i4_ver_small = 1;
        }

        DLM_TRACE(("my %d, downlaod %d, small = %d, upgrade = %d\n", 
                   (int)pt_my_tmp->i4_version, 
                   (int)pt_new_tmp->i4_version,
                   (int)i4_ver_small,
                   (int)i4_upgrade));

        pt_my_tmp = pt_my_tmp->pt_next;
        pt_new_tmp = pt_new_tmp->pt_next;
    }

    if ( i4_upgrade == 0 && i4_ver_small == 0 && pt_new_tmp )
    {
        /* ah... download version has more branch version */
        i4_upgrade = 1;
    }

    if ( i4_upgrade )
    {
        DLM_TRACE(("Validate firmware version - ok\n"));
        i4_version = DLM_OK;
    }
    else
    {
        DLM_TRACE(("Only accept more newer version firmware\n"));
        i4_version = DLM_VERSION_NOT_MATCH;
    }

    /* free allocated memory */
    while ( pt_my_ver )
    {
        pt_my_tmp = pt_my_ver->pt_next;
        x_mem_free(pt_my_ver);
        pt_my_ver = pt_my_tmp;
    }
    while ( pt_dl_ver )
    {
        pt_my_tmp = pt_dl_ver->pt_next;
        x_mem_free(pt_dl_ver);
        pt_dl_ver = pt_my_tmp;
    }

    DLM_TRACE(("small = %d, upgrade = %d\n", (int)i4_ver_small, (int)i4_upgrade));

    return i4_version;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_validate_model_name
 *
 * Description: This API provide validate the model name
 *
 * Inputs:  Code file Header start address
 * Outputs: no
 * Returns: DLM_OK, the download file has same model name as we defined
 *          DLM_MODEL_NAME_NOT_MATCH, the downloaded file's model name not match
 ----------------------------------------------------------------------------*/
static INT32  dlm_validate_model_name(UCHAR *puc_hdr)
{
    DEFAULT_CODE_FILE_HDR_T *pt_hdr = (DEFAULT_CODE_FILE_HDR_T *) puc_hdr;

    /* if disable model check */
    if ( uc_g_flag & kValidateModelName )
    {
        return DLM_OK;
    }

    DLM_TRACE(("Validate model name "));
    if ( x_memcmp(pt_hdr->s_model, pt_myHdr->s_model, 32) == 0 )
    {
        DLM_TRACE(("- ok\n"));
        return DLM_OK;
    }

    DLM_TRACE(("- failed\n"));
    return DLM_MODEL_NAME_NOT_MATCH;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_dump_register_append_data
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
 #ifndef CC_LOADER_SHRINK_DLM_API
VOID dlm_dump_register_append_data(VOID)
{
    DLM_APPEND_BLOCK_T *pt_block = pt_append_data_callback_list;

#if (DEBUG_DLM_TRACE == 1)
    if ( pt_block )
        DLM_TRACE(("[DLM] supported data tag\n\n"));

    while ( pt_block )
    {
        DLM_TRACE(("%c%c%c%c buffer = %p, size = %u\n", 
                   pt_block->uc_type[0],
                   pt_block->uc_type[1],
                   pt_block->uc_type[2],
                   pt_block->uc_type[3],
                   pt_block->pauc_buffer,
                   (unsigned int) pt_block->ui4_buf_len));
        pt_block = pt_block->pt_next;
    }
#endif    
}
#endif
/*-----------------------------------------------------------------------------
 * Name: x_dlm_unreg_append_callback
 *
 * Description: This API is use to remove installed append data call back 
 *
 * Inputs:  call back tag
 * Outputs: NA
 * Returns: DLM_OK, uninstall ok
 *          DLM_NO_HELPER_REGISTERED, no such tag exist
 ----------------------------------------------------------------------------*/
INT32 x_dlm_unreg_append_callback(UCHAR *pc_type)
{
    DLM_APPEND_BLOCK_T* pt_match = NULL;
    DLM_APPEND_BLOCK_T* pt_prev = NULL;

    /* look up list */
    pt_match = pt_append_data_callback_list;
    while ( pt_match )
    {
        if ( x_memcmp(pt_match->uc_type, pc_type, 4) == 0 )
        {
            /* remove this tag */
            break;
        }
        pt_prev = pt_match;
        pt_match = pt_match->pt_next;
    }

    if ( pt_prev && pt_match )
    {
        pt_prev->pt_next = pt_match->pt_next;
        x_mem_free(pt_match);
        return DLM_OK;
    }
    else if ( pt_match )
    {
        pt_append_data_callback_list = pt_match->pt_next;
        x_mem_free(pt_match);
        return DLM_OK;
    }
    else
    {
        return DLM_NO_HELPER_REGISTERED;
    }
}

/*-----------------------------------------------------------------------------
 * Name: x_dlm_set_ignore_check_item
 *
 * Description: This API is use to disable some integrity check item
 *
 * Inputs:  check flag
 * Outputs: NA
 * Returns: DLM_OK, 
 ----------------------------------------------------------------------------*/
INT32 x_dlm_set_ignore_check_item(UCHAR uc_flag)
{
    /**
     * #define kValidateID             (UCHAR) (0x10)
     * #define kValidateVersion        (UCHAR) (0x20)
     * #define kValidateModelName      (UCHAR) (0x40)
     * #define kValidateDigestAtTheEnd (UCHAR) (0x80)
     */
    uc_g_flag = uc_flag;

    return DLM_OK;
}

/**
 * x_dlm_reg_version_compare_callback
 * 
 * Register user defined version compare API
 * @param p_fn
 * 
 * @return VOID
 */
VOID x_dlm_reg_version_compare_callback(dlm_ver_chk_fn p_fn)
{
    dlm_ver_compare_fn = p_fn;
}

static VOID dlm_free_append_buffer(DLM_APPEND_BLOCK_T *pt_append_data_block)
{
    if ( pt_append_data_block && pt_append_data_block->pauc_buffer )
    {
        DLM_TRACE(("    free tag \"%c%c%c%c\" memory size = %d\n\n", 
                   pt_append_data_block->uc_type[0],
                   pt_append_data_block->uc_type[1],
                   pt_append_data_block->uc_type[2],
                   pt_append_data_block->uc_type[3],
                   pt_append_data_block->ui4_buf_len));
        x_mem_free(pt_append_data_block->pauc_buffer);
        pt_append_data_block->pauc_buffer = NULL;
    }
}   

/** 
 * dlm_read_code_header 
 *  
 * Read specify data from codefile  
 * 
 * @param filename
 * @param pt_user_io
 * @param i4_size
 * 
 * @return INT32 
 * DLM_OPEN_FILE_FAILED, 
 * DLM_READ_FILE_FAILED, 
 * DLM_OK 
 */
static INT32 dlm_read_code_header(CHAR *filename, DLM_IO_T *pt_user_io, INT32 i4_size)
{
    DEFAULT_CODE_FILE_HDR_T *pt_file_hdr = (DEFAULT_CODE_FILE_HDR_T *)puac_internal_buf;
    INT32 i4_file, i4_ret;
    DLM_IO_T *pt_io = pt_user_io;

    /* prepare IO set */
    if ( pt_io == NULL )
    {
        pt_io = x_dlm_get_default_io();
    }

    /* Read default header size */
    if ( (i4_file = pt_io->open_source(filename, kDlm_Read)) == -1 )
    {
        DLM_TRACE(("Open file %s for integrity check failed\n", filename));
        return DLM_OPEN_FILE_FAILED;
    }

    i4_ret = pt_io->read_data(i4_file, (CHAR *)pt_file_hdr, i4_size);

    if ( i4_ret < i4_size || i4_ret == -1 )
    {
        DLM_TRACE(("Read file %s header failed\n", filename));
        pt_io->close_source(i4_file);
        return DLM_READ_FILE_FAILED;
    }
    pt_io->close_source (i4_file);

    return DLM_OK;
}



