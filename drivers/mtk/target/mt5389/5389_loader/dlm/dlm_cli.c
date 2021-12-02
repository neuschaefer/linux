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
 * $RCSfile: dlm_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "cli/x_cli.h"
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "file_mngr/x_fm.h"
#include "inc/u_sys_name.h"
#include "res_mngr/x_rm_dev_types.h"
#include "file_mngr/fm_util.h"
#include "file_mngr/fm_ufs.h"
#include "file_mngr/fm_blkdev.h"
#include "x_dlm.h"
#include "dlm_fm.h"
#include "dlm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#ifdef CLI_LVL_ALL

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static INT32 dir_path(INT32 i4_argc, const CHAR ** pps_argv);
static INT32 download_file (INT32 i4_argc, const CHAR**  pps_argv);
static INT32 dump_dlm_tag (INT32 i4_argc, const CHAR**  pps_argv);
static INT32 del_dlm_tag (INT32 i4_argc, const CHAR**  pps_argv);
static INT32 upload_file (INT32 i4_argc, const CHAR**  pps_argv);
static INT32 dlm_set_unchk_item (INT32 i4_argc, const CHAR**  pps_argv);
static INT32 dlm_cli_get_codefile_tag (INT32 i4_argc, const CHAR**  pps_argv);

static INT32 dlm_cli_tag_download(INT32 i4_argc, const CHAR**  pps_argv);
static INT32 dlm_cli_tag_tst(INT32 i4_argc, const CHAR**  pps_argv);
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/* Download module command table */
static CLI_EXEC_T dlm_sub_cmd_tbl[] = 
{
    {
        "dir",
        "d",
        dir_path,
        NULL,
        "mount usb flash and list file in flash",
        CLI_SUPERVISOR
    },
    {
        "download",
        "dl",
        download_file,
        NULL,
        "download file from usb to device flash",
        CLI_SUPERVISOR
    },
    {
        "list",
        "l",
        dump_dlm_tag,
        NULL,
        "list current supported tag",
        CLI_SUPERVISOR
    },
    {
        "del",
        "del",
        del_dlm_tag,
        NULL,
        "del tag",
        CLI_SUPERVISOR
    },
    {
        "un_chk",
        "uchk",
        dlm_set_unchk_item,
        NULL,
        "uchk id model version",
        CLI_SUPERVISOR
    },
    {
        "upload",
        "ul",
        upload_file,
        NULL,
        "upload data to usb flash",
        CLI_SUPERVISOR
    },
    {
        "c_tag",
        "c",
        dlm_cli_get_codefile_tag,
        NULL,
        "dump codefile tag",
        CLI_SUPERVISOR
    },
    {
        "tag_dl",
        "tag_dl",
        dlm_cli_tag_download,
        NULL,
        "tag_dl file_name tag_name",
        CLI_SUPERVISOR
    },
    {
        "tag_tst",
        "tag_tst",
        dlm_cli_tag_tst,
        NULL,
        "tag_tst file_name tag",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};


/* Download module root command table */
static CLI_EXEC_T dlm_root_cmd_tbl[] =
{
    {
        "dlm",
        NULL,
        NULL,
        dlm_sub_cmd_tbl,
        "download command",
        CLI_SUPERVISOR
    },
    END_OF_CLI_CMD_TBL
};

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/* 
   mount usb flash command 
   mw.fm.mount /dev/usb_mass_3_p0 /mnt/ms
*/
static INT32 test_type_1(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pui4_len)
{
    INT32  out_file, in_file;
    static UINT32 cnt = 0;
    static UINT32 ui4_offset =0;

    if (e_flag == DLM_WRITE)
    {
#if 1
        out_file = dlm_fopen ("/mnt/ms/type_1.bin", kDlm_Append);
        dlm_fwrite(out_file, (CHAR*)pauc_buffer, *pui4_len );
        /*x_dump_buffer("", pauc_buffer, ui4_len, eHex);*/
        dlm_fclose(out_file);
        cnt += *pui4_len;
        /*x_dbg_stmt("Add %u into type_1\n", cnt);*/
#endif
    }
    else
    {
        /* e_flag == DLM_READ */
        in_file = dlm_fopen("/mnt/ms/type_1.bin", kDlm_Read);
        dlm_fseek(in_file, ui4_offset);
        if (!dlm_feof(in_file))
        {
            *pui4_len = dlm_fread(in_file, (CHAR*)pauc_buffer, 5000); /* 8192 is my buffer size */
            ui4_offset += *pui4_len;
            /*x_dbg_stmt("Total %u, read %u\n",ui4_offset,*pui4_len);*/
            x_dbg_stmt(".");
        }
        else
            ui4_offset = 0; /* renew for next time use */
        dlm_fclose(in_file);
    }
    return 0;
}

/* return DLM_OK */
static INT32 do_firmware(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pui4_len)
{
    static INT32 in_file = -1, out_file = -1;  /* must be -1 */
    static INT32 i4_size = 0;

    if (e_flag == DLM_WRITE)
    {
        if (out_file == -1)
        {
            out_file = dlm_fopen (SN_DEV_PATH "/" SN_NOR_FLASH_1, kDlm_Append);
            if (out_file == -1)
            {
                return DLM_OPEN_FILE_FAILED;
            }
        }

        if (dlm_fwrite(out_file, (CHAR*)pauc_buffer, *pui4_len ) == -1)
           return DLM_WRITE_DATA_FAILED;

        x_dbg_stmt("+");

        if (*pui4_len != 5000) /* my buffer size, if this statement is true, this is last packet */
        {
            dlm_fclose(out_file);
            out_file = -1; /* back to init state */
        }
    }
    else
    {
        /* e_flag == DLM_READ */
        if (in_file == -1)
        {
            i4_size = dlm_file_size("/mnt/ms/type_2.bin");
            if (i4_size == 0)
            {
                /* reach last packet */
                i4_size = -1;
                *pui4_len = 0;
            }
            else
            {
                in_file = dlm_fopen("/mnt/ms/type_2.bin", kDlm_Read);
                if (in_file == -1)
                {
                    return DLM_OPEN_FILE_FAILED;
                }
                x_dbg_stmt("open file\n");
            }
        }

        if (!dlm_feof(in_file))
            *pui4_len = dlm_fread(in_file, (CHAR*)pauc_buffer, 5000); 

        if (dlm_feof(in_file))
        {
            if (*pui4_len != 5000) /* if last packet size same as buffer size, keep this state */
            {
                dlm_fclose(in_file);
                x_dbg_stmt("Close file\n");
                in_file = -1;
            }
        }
    }
    return DLM_OK;
}
#if 0
static INT32 do_firmware_2usb(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pui4_len)
{
    static INT32 in_file = -1, out_file = -1;  /* must be -1 */
    static INT32 i4_size = 0;

    if (e_flag == DLM_WRITE)
    {
        if (out_file == -1)
        {
            out_file = dlm_fopen ("/mnt/ms/firm.bin", kDlm_Append);
            if (out_file == -1)
            {
                return DLM_OPEN_FILE_FAILED;
            }
        }

        if (dlm_fwrite(out_file, (CHAR*)pauc_buffer, *pui4_len ) == -1)
           return DLM_WRITE_DATA_FAILED;

        x_dbg_stmt("+");

        if (*pui4_len != 5000) /* my buffer size, if this statement is true, this is last packet */
        {
            dlm_fclose(out_file);
            out_file = -1; /* back to init state */
        }
    }
    else
    {
        /* e_flag == DLM_READ */
        if (in_file == -1)
        {
            i4_size = dlm_file_size("/mnt/ms/type_2.bin");
            if (i4_size == 0)
            {
                /* reach last packet */
                i4_size = -1;
                *pui4_len = 0;
            }
            else
            {
                in_file = dlm_fopen("/mnt/ms/type_2.bin", kDlm_Read);
                if (in_file == -1)
                {
                    return DLM_OPEN_FILE_FAILED;
                }
                x_dbg_stmt("open file\n");
            }
        }

        if (!dlm_feof(in_file))
            *pui4_len = dlm_fread(in_file, (CHAR*)pauc_buffer, 5000); 

        if (dlm_feof(in_file))
        {
            if (*pui4_len != 5000) /* if last packet size same as buffer size, keep this state */
            {
                dlm_fclose(in_file);
                x_dbg_stmt("Close file\n");
                in_file = -1;
            }
        }
    }
    return DLM_OK;
}
#endif
/* return DLM_OK */
static INT32 do_boot(DLM_ACCESS_T e_flag, UCHAR *pauc_buffer, INT32 *pui4_len)
{
    static INT32 out_file = -1;  /* must be -1 */

    if (e_flag == DLM_WRITE)
    {
        if (out_file == -1)
        {
            out_file = dlm_fopen (SN_DEV_PATH "/" SN_NOR_FLASH_0, kDlm_Append);
            if (out_file == -1)
            {
                return DLM_OPEN_FILE_FAILED;
            }
        }

        if (dlm_fwrite(out_file, (CHAR*)pauc_buffer, *pui4_len ) == -1)
           return DLM_WRITE_DATA_FAILED;

        x_dbg_stmt("+");

        if (*pui4_len != 5000) /* my buffer size, if this statement is true, this is last packet */
        {
            dlm_fclose(out_file);
            out_file = -1; /* back to init state */
        }
    }
    else
    {
        *pui4_len = 0;
    }
    return DLM_OK;
}

VOID Install_DLM_CallBack()
{
    static INT32 i4_init = 0;
    HANDLE_T     h_dev_dir;
    UCHAR        boot[] = "boot";    
    UCHAR        nvrm[] = "nvrm";    
    UCHAR        firm[] = "firm";
    if (i4_init)
        return ;

    /* install NOR flash firmware partition */

    if ( x_fm_open_dir(FM_ROOT_HANDLE, SN_DEV_PATH, &h_dev_dir) == FMR_OK)
    {
        /*
            nor partition 0 = loader,
	        nor partition 1 = image app.
        */
        x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_NOR_FLASH_1, (FM_MODE_TYPE_CHR | 0666), DRVT_NOR_FLASH, 1);
        x_fm_make_entry (FM_ROOT_HANDLE, SN_DEV_PATH "/" SN_NOR_FLASH_0, (FM_MODE_TYPE_CHR | 0666), DRVT_NOR_FLASH, 0);
    }

    x_dlm_reg_append_callback(boot, 5000, do_boot); /* boot code */
    x_dlm_reg_append_callback(nvrm, 5000, test_type_1);
    /*x_dlm_reg_append_callback("firm", 5000, do_firmware_2usb);*/
    x_dlm_reg_append_callback(firm, 5000, do_firmware);
    i4_init = 1;
}

static INT32 download_file (INT32 i4_argc, const CHAR**  pps_argv)
{
    INT32 i4_ret;

    if (i4_argc == 1)
    {
        x_dbg_stmt("Usage: download filename\n");

        return(CLIR_OK);
    }

    Install_DLM_CallBack();
    /*
    x_strcpy(s_file, "/mnt/ms/");
    x_strcat(s_file + 8, pps_argv[1]);

    i4_ret = x_dlm_integrity_check(s_file, NULL);
    if (i4_ret == 0)
    {
        x_dbg_stmt("Do firmware upgrade\n");
        x_dlm_download(s_file, NULL, 0);
    }
    else
        x_dbg_stmt("No proper firmware exist\n");
    */

    i4_ret = x_dlm_integrity_check((CHAR*)pps_argv[1], NULL);
    if (i4_ret == 0)
    {
        x_dbg_stmt("Do firmware upgrade\n");
        x_dlm_download((CHAR*)pps_argv[1], NULL, 0);
    }
    else
        x_dbg_stmt("No proper firmware exist\n");

/*    x_dlm_download(s_file, NULL, 1);*/

    return(CLIR_OK);
}

static INT32 dump_dlm_tag (INT32 i4_argc, const CHAR**  pps_argv)
{
    static int i4_flag = 0;

    if (i4_flag == 0)
    {
        Install_DLM_CallBack();
        i4_flag = 1;
    }

    dlm_dump_register_append_data();
    return(CLIR_OK);
}

static INT32 del_dlm_tag (INT32 i4_argc, const CHAR**  pps_argv)
{
    if (i4_argc != 2)
    {
        x_dbg_stmt("use \"del tag_name\"\n");
        return(CLIR_OK);
    }

    if (x_dlm_unreg_append_callback((UCHAR*)pps_argv[1]) == DLM_OK)
    {
        x_dbg_stmt("Remove tag %s done\n", pps_argv[1]);
    }
    else
    {
        x_dbg_stmt("No tag %s exist\n", pps_argv[1]);
    }

    return(CLIR_OK);
}

static INT32 upload_file (INT32 i4_argc, const CHAR**  pps_argv)
{
    if (i4_argc == 1)
    {
        x_dbg_stmt("Usage: upload tag\n use 'list' to get possible tag");

        return(CLIR_OK);
    }

    Install_DLM_CallBack();
    x_dlm_upload((CHAR*)pps_argv[1], NULL, "/mnt/ms/upload");

    return(CLIR_OK);
}

static INT32 dir_path(INT32 i4_argc, const CHAR ** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T    h_dir;
    FM_DIR_ENTRY_T at_dir_entry[512];
    UINT32  ui4_num_entries;
    static int i4_init = FALSE;

    if (i4_init != TRUE)
    {
        i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/mnt/ms", 0777);
        if ( (i4_ret != FMR_OK) && (i4_ret != FMR_EXIST))
        {
            x_dbg_stmt("Make dir %s Fail!: %d\n",pps_argv[2], i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }

        i4_ret = x_fm_mount(FM_ROOT_HANDLE, "/dev/usb_mass_0_p0", FM_ROOT_HANDLE, "/mnt/ms");
        if ( i4_ret != FMR_OK)
        {
            x_dbg_stmt("Mount %s fail: %d\n", "/dev/usb_mass_0_p0", i4_ret);
            return CLIR_CMD_EXEC_ERROR;
        }

        x_dbg_stmt("Mount %s on %s\n", "/dev/usb_mass_0_p0", "/mnt/ms");
        i4_init = TRUE;
    }

    /* list file */
    if ( FMR_OK != x_fm_open_dir(FM_ROOT_HANDLE, "/mnt/ms", &h_dir))
    {
        x_dbg_stmt("Open this directory fail!\n");
        return CLIR_CMD_EXEC_ERROR;
    }

    /* looply get directories under current directory */
    x_fm_read_dir_entries(h_dir, at_dir_entry, 512, &ui4_num_entries);

    i4_ret = 0; 
    while ( ui4_num_entries-- != 0)
    {
        x_dbg_stmt("%s\n", at_dir_entry[i4_ret++].s_name);
    }

    x_fm_close(h_dir);
    return CLIR_OK;
}

static INT32 dlm_set_unchk_item (INT32 i4_argc, const CHAR**  pps_argv)
{
    UCHAR uc_flag = 0;
    INT32 i4_i;

    for (i4_i =1; i4_i<i4_argc; i4_i++)
    {
        if (x_strcmp(pps_argv[i4_i], "id") == 0)
        {
            uc_flag |= kValidateID;
            continue;
        }

        if (x_strcmp(pps_argv[i4_i], "version") == 0)
        {
            uc_flag |= kValidateVersion;
            continue;
        }

        if (x_strcmp(pps_argv[i4_i], "model") == 0)
        {
            uc_flag |= kValidateModelName;
            continue;
        }
    }

    x_dlm_set_ignore_check_item(uc_flag);
    return CLIR_OK;
}

static INT32 dlm_cli_get_codefile_tag (INT32 i4_argc, const CHAR**  pps_argv)
{
    Append_T *pt_tag_list;

    if (i4_argc!=2)
    {
        x_dbg_stmt("Use c_tag filename\n");
        return CLIR_OK;
    }

    pt_tag_list = x_dlm_get_codefile_tag_list((CHAR *)pps_argv[1]);
    if (pt_tag_list)
    {
        while (pt_tag_list)
        {
            x_dbg_stmt("Tag -> %c%c%c%c, privacy = %d, lenght = %d\n", 
                       pt_tag_list->uc_type[0],
                       pt_tag_list->uc_type[1],
                       pt_tag_list->uc_type[2],
                       pt_tag_list->uc_type[3],
                       pt_tag_list->uac_privacy[0],
                       pt_tag_list->ui4_length);
                                                                                            
            pt_tag_list = pt_tag_list->pt_next;
        }
        x_dbg_stmt("\n");
    }
    else
    {
        x_dbg_stmt("No tag exist in file %s \n", pps_argv[1]);
    }

    return CLIR_OK;
}

/* tag_dl file_name tag_name */
static INT32 dlm_cli_tag_download(INT32 i4_argc, const CHAR**  pps_argv)
{
    INT32 i4_ret;
    UCHAR tag[] = "firm";
    
    if (i4_argc!=3)
    {
        x_dbg_stmt("Use tag_dl file_name tag_name\n");
        goto QUIT_DLM_CLI_TAG_DOWNLOAD;
    }
    
    Install_DLM_CallBack();

    i4_ret = x_dlm_integrity_check((CHAR*) pps_argv[1], NULL);
    if (i4_ret == DLM_OK)
    {
        i4_ret = x_dlm_download_by_tag((CHAR *)pps_argv[1], tag, NULL);
        if (i4_ret != DLM_OK)
        {
            x_dbg_stmt("Tag download failed\n");
        }
        else
        {
            x_dbg_stmt("Tag download ok\n");
        }
    }
    else
    {
        x_dbg_stmt("Integrity check failed --> %s\n", pps_argv[1]);
    }

QUIT_DLM_CLI_TAG_DOWNLOAD:
    return CLIR_OK;
}

/* tag_tst file_name tag */
static INT32 dlm_cli_tag_tst(INT32 i4_argc, const CHAR**  pps_argv)
{
    CHAR ac_buf[20];
    INT32 i4_ret;
    UCHAR tag[] = "firm";

    if (i4_argc!=3)
    {
        x_dbg_stmt("Use tag_tst file_name tag\n");
        goto QUIT_DLM_CLI_TAG_TST;
    }
    
    Install_DLM_CallBack();

    i4_ret = x_dlm_integrity_check((CHAR*) pps_argv[1], NULL);
    if (i4_ret == DLM_OK)
    {
        i4_ret = x_dlm_tag_test((CHAR *)pps_argv[1], tag, NULL, ac_buf, 20);
        if (i4_ret != DLM_OK)
        {
            x_dbg_stmt("Tag tst failed\n");
        }
        else
        {
            x_dbg_stmt("Tag tst ok\n");
        }
    }
    else
    {
        x_dbg_stmt("Integrity check failed --> %s\n", pps_argv[1]);
    }

QUIT_DLM_CLI_TAG_TST:
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
INT32 dlm_cli_init( VOID )
{
    INT32   i4_ret;

    i4_ret = x_cli_attach_cmd_tbl( dlm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_PIPE );
    if ( i4_ret != CLIR_OK && i4_ret != CLIR_NOT_INIT )
    {
        return CLIR_NOT_INIT;
    }

    return CLIR_OK;
}

#endif /* CLI_LVL_ALL */

