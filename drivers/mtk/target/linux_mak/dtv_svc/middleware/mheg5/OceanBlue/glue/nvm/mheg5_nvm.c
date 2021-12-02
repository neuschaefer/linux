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
 * $RCSfile: mheg5_os.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all os related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

 
#ifdef MHEG5_NVM_SUPPORT
#include "dbg/u_dbg.h"
#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "handle/u_handle.h"
#include "file_mngr/u_fm.h"
#include "file_mngr/x_fm.h"

#include "os/inc/x_os.h"
#include "os/inc/u_os.h"

#include "mheg5/u_mheg5.h"
#include "mheg5/x_mheg5.h"
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"
#include "mheg5/OceanBlue/glue/nvm/mheg5_nvm.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_nvm.h"

/*-----------------------------------------------------------------------------
                    macro, structure, enum declarations
----------------------------------------------------------------------------*/
#ifndef LINUX_TURNKEY_SOLUTION
#define MHEG5_TPS_ROOT_WITH_SLASH        "/mnt/mh5tps/"
#define MHEG5_TPS_ROOT_WITH_SLASH_LEN    (13)
#define MHEG5_TPS_ROOT_WITHOUT_SLASH     "/mnt/mh5tps"
#define MHEG5_TPS_ROOT_WITHOUT_SLASH_LEN (12)
#else
#define MHEG5_TPS_ROOT_WITH_SLASH        "/3rd/mh5n/"
#define MHEG5_TPS_ROOT_WITH_SLASH_LEN    (11)
#define MHEG5_TPS_ROOT_WITHOUT_SLASH     "/3rd/mh5n"
#define MHEG5_TPS_ROOT_WITHOUT_SLASH_LEN (10)
#endif
/*-----------------------------------------------------------------------------
                    data declarations
----------------------------------------------------------------------------*/
#ifndef LINUX_TURNKEY_SOLUTION
static CHAR* _ps_tps_dev = NULL;
#endif


/*-----------------------------------------------------------------------------
                    functions prototypes
 ----------------------------------------------------------------------------*/
static CHAR* _mheg5_nvm_get_full_path(const char* ps_name);

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static CHAR* _mheg5_nvm_get_full_path(const char* ps_name)
{
    CHAR* ps_full_path = NULL;
    
    if (ps_name == NULL)
    {
        return ps_full_path;
    }
    else if (ps_name[0] == '/')
    {
        ps_full_path = x_mem_alloc(MHEG5_TPS_ROOT_WITHOUT_SLASH_LEN + x_strlen(ps_name));
        if (ps_full_path != NULL)
        {
            x_strcpy(ps_full_path, MHEG5_TPS_ROOT_WITHOUT_SLASH);
            x_strcat(ps_full_path, ps_name);
        }
        return ps_full_path;
    }
    else
    {
        ps_full_path = x_mem_alloc(MHEG5_TPS_ROOT_WITH_SLASH_LEN + x_strlen(ps_name));
        if (ps_full_path != NULL)
        {
            x_strcpy(ps_full_path, MHEG5_TPS_ROOT_WITH_SLASH);
            x_strcat(ps_full_path, ps_name);
        }
        return ps_full_path;
    }
}


tmMHEG5Err_t DVP_FileSize( const char* name, U32BIT* pSize )
{
    INT32 i4_ret;
    CHAR* ps_full_path = NULL;
    FM_FILE_INFO_T t_file_info;

    x_dbg_stmt("[MHEG5 NVM] DVP_FileSize(%s, %p)\n", name, pSize);

    if (pSize == NULL || name == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    ps_full_path = _mheg5_nvm_get_full_path(name);

    if (ps_full_path == NULL)
    {
        return MHEG5_ERR_OTHER;
    }

    i4_ret = x_fm_get_info_by_name(FM_ROOT_HANDLE, ps_full_path, &t_file_info);
    
    x_mem_free(ps_full_path);
    ps_full_path = NULL;

    if (i4_ret == FMR_NO_ENTRY)
    {
        return MHEG5_ERR_FILE_NOT_FOUND;
    }
    else if (i4_ret != FMR_OK)
    {
        return MHEG5_ERR_OTHER;
    }
    else
    {
        *pSize = (U32BIT)t_file_info.ui8_size;
        
        x_dbg_stmt("[MHEG5 NVM] DVP_FileSize(%s) -> %lu\n", name, *pSize);

        return MHEG5_OK;
    }
}

void* DVP_FileOpen( const char* name, E_FILE_MODE mode )
{
    INT32 i4_ret;
    CHAR* ps_full_path = NULL;
    HANDLE_T h_file = NULL_HANDLE;

    x_dbg_stmt("[MHEG5 NVM] DVP_FileOpen(%s, %d)\n", name, mode);

    if (name == NULL)
    {
        return NULL;
    }

    ps_full_path = _mheg5_nvm_get_full_path(name);

    if (ps_full_path == NULL)
    {
        return NULL;
    }

    /* FM_READ: Open for Reading: file must exist */
    /* FM_WRITE: Open for Reading & Writing: file must exist */
    /* FM_OVERWRITE: Open for Writing: if file exists, it's overwritten; if not, it's created */
    if (mode == FM_READ)
    {
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_full_path, FM_READ_ONLY | FM_OPEN_APPEND, FM_MODE_USR_READ, FALSE, &h_file);
    }
    else if (mode == FM_WRITE)
    {
        i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_full_path, FM_READ_WRITE | FM_OPEN_APPEND, FM_MODE_USR_READ | FM_MODE_USR_WRITE, FALSE, &h_file);
    }
    else /*FM_OVERWRITE*/
    {
        /* When DVP_FileOpen is called with FM_OVERWRITE mode, it expects that the file and 
                 any directories in the path will be created if they do not exist. The TPS only uses a 
                 single directory which is expected to be created by the first write operation. Calls to 
                 DVP_FileOpen in any other modes (or calls to DVP_DirectoryOpen) should return NULL 
                 if the file or directory doesn't exist. */
        UINT32 ui4_len = x_strlen(ps_full_path);
        UINT32 i = 0;
        CHAR*  pc_i = NULL;
        
        i4_ret = FMR_OK;

        for (i = MHEG5_TPS_ROOT_WITHOUT_SLASH_LEN; i < ui4_len; i++)
        {
            pc_i = ps_full_path + i;

            if (*pc_i == '/')
            {
                *pc_i = '\0';
                i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, ps_full_path, 0777);
                if (i4_ret == FMR_EXIST)
                {
                    i4_ret = FMR_OK;
                    *pc_i = '/';
                }
                else if (i4_ret != FMR_OK)
                {
                    *pc_i = '/';
                    break;
                }
                else
                {
                    x_dbg_stmt("[MHEG5 NVM]%s is created\n", ps_full_path);
                    *pc_i = '/';
                }
            }
        }
        if (i4_ret == FMR_OK)
        {
            i4_ret = x_fm_open(FM_ROOT_HANDLE, ps_full_path, FM_READ_WRITE | FM_OPEN_CREATE, FM_MODE_USR_READ | FM_MODE_USR_WRITE, FALSE, &h_file);
        }
    }
    x_mem_free(ps_full_path);
    ps_full_path = NULL;
    
    x_dbg_stmt("[MHEG5 NVM] DVP_FileOpen(%s, %d) -> %p\n", name, mode, (void*)h_file);

    if (i4_ret == FMR_OK)
    {
        return ((void*)h_file);
    }
    else
    {
        return NULL;
    }   
}

void DVP_FileClose( void* file )
{
    x_dbg_stmt("[MHEG5 NVM] DVP_FileClose(%p)\n", file);
    
    x_fm_close((HANDLE_T)file);
}

U32BIT DVP_FileRead( void* file, U8BIT* data, U32BIT size)
{
    HANDLE_T h_file = (HANDLE_T)file;
    INT32    i4_ret = 0;
    UINT32   ui4_read = 0;
    
    i4_ret = x_fm_read(h_file, data, size, &ui4_read);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[MHEG5 NVM] DVP_FileRead(%p, %p, %lu) failed as %ld\n", file, data, size, i4_ret);
    }
    
    x_dbg_stmt("[MHEG5 NVM] DVP_FileRead(%p, %p, %lu) -> %lu\n", file, data, size, ui4_read);

    return ui4_read;
}

U32BIT DVP_FileWrite( void* file, U8BIT* data, U32BIT size )
{
    HANDLE_T h_file = (HANDLE_T)file;
    INT32    i4_ret = 0;
    UINT32   ui4_write = 0;
    
    i4_ret = x_fm_write(h_file, data, size, &ui4_write);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[MHEG5 NVM] DVP_FileWrite(%p, %p, %lu) failed as %ld\n", file, data, size, i4_ret);
    }
    
    x_dbg_stmt("[MHEG5 NVM] DVP_FileWrite(%p, %p, %lu) -> %lu\n", file, data, size, ui4_write);

    return ui4_write;
}


tmMHEG5Err_t DVP_FileDelete( const char* name )
{
    INT32 i4_ret;
    CHAR* ps_full_path = NULL;

    x_dbg_stmt("[MHEG5 NVM] DVP_FileDelete(%s)\n", name);

    if (name == NULL)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }

    ps_full_path = _mheg5_nvm_get_full_path(name);

    if (ps_full_path == NULL)
    {
        return MHEG5_ERR_OTHER;
    }

    i4_ret = x_fm_delete_file(FM_ROOT_HANDLE, ps_full_path);
    x_mem_free(ps_full_path);
    ps_full_path = NULL;   
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[MHEG5 NVM] DVP_FileDelete(%s) failed as %ld\n", name, i4_ret);
        return MHEG5_ERR_OTHER;
    }
    else
    {
        return MHEG5_OK;
    }
}

void* DVP_DirectoryOpen( const char* path )
{
    INT32 i4_ret;
    CHAR* ps_full_path = NULL;
    HANDLE_T h_dir = NULL_HANDLE;

    x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryOpen(%s)\n", path);

    if (path == NULL)
    {
        return NULL;
    }

    ps_full_path = _mheg5_nvm_get_full_path(path);
    if (ps_full_path == NULL)
    {
        return NULL;
    }

    i4_ret = x_fm_open_dir(FM_ROOT_HANDLE, ps_full_path, &h_dir);
    x_mem_free(ps_full_path);
    ps_full_path = NULL;   
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryOpen(%s) failed as %ld\n", path, i4_ret);
        return NULL;
    }
    else
    {
        x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryOpen(%s) -> %p\n", path, (void*)h_dir);
        return ((void*)h_dir);
    }
}

E_DIR_ENTRY_TYPE DVP_DirectoryRead( void* dir, char* name )
{
    HANDLE_T h_dir = (HANDLE_T)dir;
    INT32    i4_ret = 0;
    FM_DIR_ENTRY_T t_dir_entry;
    UINT32 ui4_entries;
    E_DIR_ENTRY_TYPE e_entry_type = ENTRY_TYPE_ERROR;

    x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryRead(%p)\n", dir);

    while (1)
    {
        i4_ret = x_fm_read_dir_entries(h_dir, &t_dir_entry, 1, &ui4_entries);
        if (i4_ret == FMR_OK)
        {
            if (x_strcmp(".", t_dir_entry.s_name) == 0 ||
                x_strcmp("..", t_dir_entry.s_name) == 0 )
            {
                continue;
            }
            
            name[0] = '\0';
            x_strcpy(name, t_dir_entry.s_name);

            if (t_dir_entry.t_file_info.ui4_mode & FM_MODE_TYPE_FILE)
            {
                x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryRead(%p) got SUB_FILE: %s\n", dir, name);
                e_entry_type = ENTRY_TYPE_NORMAL_FILE;
            }
            else if (t_dir_entry.t_file_info.ui4_mode & FM_MODE_TYPE_DIR)
            {
                x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryRead(%p) got SUB_DIR: %s\n", dir, name);
                e_entry_type = ENTRY_TYPE_SUB_DIRECTORY;
            }
            else
            {
                x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryRead(%p) got UNKNOWN_TYPE: %s\n", dir, name);
                e_entry_type = ENTRY_TYPE_OTHER;
            }
            break;
        }
        else
        {
            name[0] = '\0';
            x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryRead(%p) failed as %ld\n", dir, i4_ret);
            e_entry_type = ENTRY_TYPE_ERROR;
            break;
        }
    }

    return e_entry_type;
}

void DVP_DirectoryClose( void* dir )
{
    x_dbg_stmt("[MHEG5 NVM] DVP_DirectoryClose(%p)\n", dir);
    
    x_fm_close((HANDLE_T)dir);
}


INT32 nvm_init_tps(
	#ifdef LINUX_TURNKEY_SOLUTION
		VOID
	#else
		const CHAR* ps_tps_dev
	#endif
)
{	
#ifndef LINUX_TURNKEY_SOLUTION    
    INT32 i4_ret;

    x_dbg_stmt("MHEG5 TPS start to setup with device %s\n", ps_tps_dev);

    if (NULL == ps_tps_dev)
    {
        return MHEG5R_INV_ARG;
    }
    
    _ps_tps_dev = x_mem_realloc(_ps_tps_dev, (x_strlen(ps_tps_dev) + 3));
    if (NULL != _ps_tps_dev)
    {
        x_strcpy(_ps_tps_dev, ps_tps_dev);
        x_strcat(_ps_tps_dev, "_0");
    }
    else
    {
        return MHEG5R_INTERNAL_ERROR;
    }
	
    i4_ret = x_fm_parse_drive(FM_ROOT_HANDLE, ps_tps_dev, NULL);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("MHEG5 TPS setup failed to parse drive(%ld)\n", i4_ret);
        x_mem_free(_ps_tps_dev);
        _ps_tps_dev = NULL;
        return MHEG5R_INTERNAL_ERROR;
    }
    
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH, 0666);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("MHEG5 TPS setup failed to create dir(%ld)\n", i4_ret);
        x_mem_free(_ps_tps_dev);
        _ps_tps_dev = NULL;
        return MHEG5R_INTERNAL_ERROR;
    }
    
    i4_ret = x_fm_mount(FM_ROOT_HANDLE, _ps_tps_dev, FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("MHEG5 TPS setup failed to mount(%ld)\n", i4_ret);
        x_mem_free(_ps_tps_dev);
        _ps_tps_dev = NULL;
        return MHEG5R_INTERNAL_ERROR;
    }
 #else
	/* mkdir /3rd/mh5n */
    INT32 i4_ret;

    x_dbg_stmt("MHEG5 TPS start to setup\n");

 #if 0
 	CHAR s_cmd[128];
	x_sprintf(s_cmd, "mkdir -p %s", MHEG5_TPS_ROOT_WITHOUT_SLASH);
	x_dbg_stmt("exec:%s\n", s_cmd);
	system(s_cmd);
 #else
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH, 0666);
    if (i4_ret != FMR_OK && i4_ret != FMR_EXIST)
    {
        x_dbg_stmt("MHEG5 TPS setup failed to create dir(%ld)\n", i4_ret);
        return MHEG5R_INTERNAL_ERROR;
    }
 #endif
	 
 #endif
    x_dbg_stmt("MHEG5 TPS setup OK\n");
    return MHEG5R_OK;        
}

INT32 nvm_clear_tps(VOID)
{
    /* user do factory reset */
    /* Clears all NVM data (and deletes all related files) held by MHEG5 component.*/
    tmMHEG5NotifyNvmFilesChanged();
	
#ifndef LINUX_TURNKEY_SOLUTION
{
    INT32 i4_ret;
	
    x_dbg_stmt("MHEG5 TPS start to clear tps device %s\n", _ps_tps_dev);

    i4_ret = x_fm_umount(FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("MHEG5 TPS failed to umount(%ld)\n", i4_ret);
    }

    i4_ret = x_fm_create_fs(FM_ROOT_HANDLE, _ps_tps_dev, FM_TYPE_FAT, NULL, 0);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("MHEG5 TPS failed to format(%ld)\n", i4_ret);
    }

    i4_ret = x_fm_mount(FM_ROOT_HANDLE, _ps_tps_dev, FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH);
    if (FMR_OK != i4_ret)
    {
        x_dbg_stmt("MHEG5 TPS failed to mount(%ld)\n", i4_ret);
    }
	
    x_dbg_stmt("MHEG5 TPS device %s cleared...\n", _ps_tps_dev);
}
#else
{
    INT32 i4_ret;

    x_dbg_stmt("MHEG5 TPS start to clear tps root directory %s\n", MHEG5_TPS_ROOT_WITHOUT_SLASH);

#if 0
    CHAR s_cmd[128];
	/* delete all files under /3rd/mh5n */
	x_sprintf(s_cmd, "rm -fvr %s", MHEG5_TPS_ROOT_WITHOUT_SLASH);
	x_dbg_stmt("exec:%s\n", s_cmd);
	system(s_cmd);

	
	/* mkdir /3rd/mh5n */
	x_sprintf(s_cmd, "mkdir -p %s", MHEG5_TPS_ROOT_WITHOUT_SLASH);
	x_dbg_stmt("exec:%s\n", s_cmd);
	system(s_cmd);
#else
    /* delete all files under /3rd/mh5n */
    i4_ret = x_fm_delete_dir_ex(FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH);
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("MHEG5 TPS failed to delete /3rd/mh5n(%ld)\n", i4_ret);
    }
    /* mkdir /3rd/mh5n */
    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, MHEG5_TPS_ROOT_WITHOUT_SLASH, 0666);
    if (i4_ret != FMR_OK && i4_ret != FMR_EXIST)
    {
        x_dbg_stmt("MHEG5 TPS setup failed to create dir(%ld)\n", i4_ret);
    }

#endif
	
    x_dbg_stmt("MHEG5 TPS device %s cleared...\n", MHEG5_TPS_ROOT_WITHOUT_SLASH);
}
#endif


    return MHEG5R_OK;        
}

#else /*MHEG5_NVM_SUPPORT*/

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_nvm.h"

tmMHEG5Err_t DVP_FileSize( const char* name, U32BIT* pSize )
{
    return MHEG5_ERR_OTHER;
}

void* DVP_FileOpen( const char* name, E_FILE_MODE mode )
{
    return NULL;
}

void DVP_FileClose( void* file )
{
    return ;
}

U32BIT DVP_FileRead( void* file, U8BIT* data, U32BIT size)
{
    return 0;
}

U32BIT DVP_FileWrite( void* file, U8BIT* data, U32BIT size )
{
    return 0;
}
tmMHEG5Err_t DVP_FileDelete( const char* name )
{
	return MHEG5_ERR_OTHER;
}

void* DVP_DirectoryOpen( const char* path )
{
	return NULL;
}

E_DIR_ENTRY_TYPE DVP_DirectoryRead( void* dir, char* name )
{
    return ENTRY_TYPE_ERROR;
}

void DVP_DirectoryClose( void* dir )
{
    return ;
}

#endif

