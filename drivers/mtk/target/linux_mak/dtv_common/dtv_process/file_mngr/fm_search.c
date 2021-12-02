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
/*----------------------------------------------------------------------------*
 * $RCSfile: fm_search.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains File Manager search related functions.
 *---------------------------------------------------------------------------*/

#ifndef ANDROID

#include <stdio.h>
#include <string.h>
#include <glob.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "x_fm.h"
#include "fm_search.h"
#include "os/inc/x_os.h"
#include "fm_util.h"


#define FM_PATTERN_LEN 128


typedef struct _FM_SEARCH_STACK_ELEM_T
{
    CHAR *ps_dir_path;
    struct _FM_SEARCH_STACK_ELEM_T *pt_next;
} FM_SEARCH_STACK_ELEM;

typedef struct _FM_SEARCH_STACK_T
{
    FM_SEARCH_STACK_ELEM *pt_head;
} FM_SEARCH_STACK;

#define STACK_INIT(pt_stack)            \
{                                       \
    pt_stack->pt_head = NULL;           \
}


#define PUSH(pt_stack, pt_elem)                         \
{                                                       \
    if(pt_stack != NULL && pt_elem != NULL)             \
    {                                                   \
        pt_elem->pt_next = pt_stack->pt_head;           \
        pt_stack->pt_head = pt_elem;                    \
    }                                                   \
}

#define POP(pt_stack, pt_elem)                          \
{                                                       \
    if(pt_stack != NULL && pt_stack->pt_head != NULL)   \
    {                                                   \
        pt_elem = pt_stack->pt_head;                    \
        pt_stack->pt_head = pt_elem->pt_next;           \
    }                                                   \
}

#define FREE_STACK(pt_stack)                            \
{                                                       \
    if(pt_stack != NULL)                                \
    {                                                   \
        FM_SEARCH_STACK_ELEM *pt_st_elem = NULL;        \
        while(pt_stack->pt_head != NULL)                \
        {                                               \
            POP(pt_stack, pt_st_elem);                  \
            if(pt_st_elem != NULL)                      \
            {                                           \
                x_mem_free(pt_st_elem);                 \
                pt_st_elem = NULL;                      \
            }                                           \
        }                                               \
    }                                                   \
}

#define STACK_EMPTY(pt_stack) (pt_stack->pt_head == NULL)


typedef struct __FM_SEARCH_ASYNC_ARG
{
    CHAR s_dir_path[FM_MAX_PATH_LEN + 1];
    CHAR s_keyword[FM_MAX_FILE_LEN + 1];
    FM_SEARCH_RESULT_T *pt_result;
} _FM_SEARCH_ASYNC_ARG;

static BOOL gb_continue_search = FALSE;
x_fm_search_nfy gpf_search_nfy = NULL;

static int _fm_search_errfunc(const char *pathname, int err)
{
    fprintf(stderr, "error accessing %s: %s\n", pathname, strerror(err));
    return 0;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_search_result_init
 *
 * Description: Initialize search result structure
 *
 * Inputs: pt_result    the pointer point to search result structure
 *
 * Outputs: -
 *
 * Returns: FMR_OK                  Success.
 *          FMR_ARG                 pt_result is NULL.
 *          FMR_NOT_ENOUGH_SPACE    alloc memory fail.
 ----------------------------------------------------------------------------*/
static INT32 _fm_search_result_init(FM_SEARCH_RESULT_T *pt_result)
{
    DBG_INFO(("%s start.\n", __func__));
    
    pt_result->pps_pathv = NULL;
    pt_result->ui4_matched_pathc = 0;
    DBG_INFO(("%s end.\n", __func__));

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fm_search_result_free
 *
 * Description: Release the memory of search result structure
 *
 * Inputs: pt_result    the pointer point to search result structure
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fm_search_result_free(FM_SEARCH_RESULT_T *pt_result)
{
    DBG_INFO(("%s enter.\n", __func__));
    if (pt_result == NULL)
    {
        return;
    }

    if (pt_result->pps_pathv != NULL)
    {
        int i;
        for (i = 0; i < pt_result->ui4_matched_pathc; i++)
        {
            x_mem_free(pt_result->pps_pathv[i]);
            pt_result->pps_pathv[i] = NULL;
        }
        
        x_mem_free(pt_result->pps_pathv);
        pt_result->pps_pathv = NULL;
        pt_result->ui4_matched_pathc = 0;
    }

    DBG_INFO(("%s end.\n", __func__));
}


/*-----------------------------------------------------------------------------
 * Name: _fm_search_str_endwith
 *
 * Description: Check if string is end with a specific character
 *
 * Inputs: pt_str   the address of a string which is to be check
 *         ch       the specific charactor
 *
 * Outputs: -
 *
 * Returns: TRUE    string is end with specific character
 *          FALSE   string is not end with specific character
 ----------------------------------------------------------------------------*/
static BOOL _fm_search_str_endwith(const CHAR *ps_str, CHAR ch)
{
    if(ps_str == NULL)
    {
        return FALSE;
    }

    UINT32 ui4_len = strlen(ps_str);
    if(ui4_len > 0 && ps_str[ui4_len-1] == ch)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _fm_search_copy_glob_result
 *
 * Description: Copy the matched file names to search result structure
 *
 * Inputs:  ps_dir_path     the searched directory path 
 *          pt_result       the pointer point to search result structure
 *          pt_glob         the pointer point to search result
 *
 * Outputs: -
 *
 * Returns: FMR_OK                  Success.
 *          FMR_ARG                 Paramter is NULL.
 *          FMR_NOT_ENOUGH_SPACE    Alloc memory fail.
 ----------------------------------------------------------------------------*/

static INT32 _fm_search_copy_glob_result(
    const CHAR *ps_dir_path,
    FM_SEARCH_RESULT_T *pt_result,
    glob_t *pt_glob)
{
    DBG_INFO(("%s enter.\n", __func__));
    if(ps_dir_path == NULL || pt_result == NULL || pt_glob == NULL)
    {
        return FMR_ARG;
    }

    DBG_INFO(("There are %d matched file in directory (%s).\n", pt_glob->gl_pathc, ps_dir_path));
    
    UINT32 i = 0;
    UINT32 ui4_dir_len = strlen(ps_dir_path);
    UINT32 ui4_len = 0;
    UINT32 ui4_total_len = 0;

 
    /*calculate the number of byte need to realloc*/
    UINT32 ui4_count = pt_result->ui4_matched_pathc +  pt_glob->gl_pathc;
        
    CHAR **pps = (CHAR **)realloc(pt_result->pps_pathv, ui4_count*sizeof(CHAR*));
    if(pps == NULL)
    {
        return FMR_NOT_ENOUGH_SPACE;
    }
    pt_result->pps_pathv = pps;
    
    /*store pointers of new matched file*/
    for(i = 0; i < pt_glob->gl_pathc; i++)
    {
        /*calculate the length of absolute matched file path*/
        ui4_len = strlen(pt_glob->gl_pathv[i]);
        ui4_total_len = (ui4_dir_len + 1) + ui4_len;
        CHAR *ps = (CHAR *)x_mem_alloc((ui4_total_len + 1) * sizeof(CHAR));
        if (ps == NULL)
        {
            /*prepare for free memory of result*/
            pt_result->ui4_matched_pathc += i;             
            return FMR_NOT_ENOUGH_SPACE;
        }
            
        /*store absolute matched file path*/
        strncpy(ps, ps_dir_path, ui4_dir_len);
        if(_fm_search_str_endwith(ps_dir_path, '/'))
        {
            strncpy(ps + ui4_dir_len, pt_glob->gl_pathv[i], ui4_len);
            ps[ui4_total_len-1] = '\0'; 
        }
        else
        {
            ps[ui4_dir_len] = '/';
            strncpy(ps + ui4_dir_len + 1, pt_glob->gl_pathv[i], ui4_len);
            ps[ui4_total_len] = '\0'; 
        }
            
        pps[pt_result->ui4_matched_pathc + i]= ps;
    }

    pt_result->ui4_matched_pathc += pt_glob->gl_pathc;

    DBG_INFO(("%s end.\n", __func__));
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_search_get_subdir_name
 *
 * Description: get absolute subdir path and push to stack
 *
 * Inputs:  ps_dir_path     the searched directory path 
 *          pt_stack        the head of stack
 *
 * Outputs: -
 *
 * Returns: FMR_OK      Success.
 *          FMR_ARG     Paramter is NULL.
 *          FMR_ACCESS  Call system interface fail.
 ----------------------------------------------------------------------------*/

static INT32 _fm_search_get_subdir_name(const CHAR *ps_dir_path, FM_SEARCH_STACK *pt_stack)
{
    DBG_INFO(("%s enter.\n", __func__));
    
    if(ps_dir_path == NULL || pt_stack == NULL)
    {
        DBG_ERROR(("Arguments are valid !\n"));
        return FMR_ARG;
    }
    INT32 i4_ret = chdir(ps_dir_path);
    if(i4_ret == -1)
    {
        DBG_ERROR(("Change dir failed. Error: %s\n", strerror(errno)));
        return FMR_ACCESS;
    }

    /*open directory*/
    DIR *dir = opendir(ps_dir_path);
    if(dir == NULL)
    {
        DBG_ERROR(("Open directory failed. Error: %s\n", strerror(errno)));
        return FMR_ACCESS;
    }

    /*read directory*/
    struct dirent *dirent = NULL;
    struct stat stat_buf;
    while((dirent = readdir(dir)) != NULL)
    {
        /*judge file name is equel with "." or "..", if yes, continue*/
        if(!(strcmp(dirent->d_name, ".") && strcmp(dirent->d_name, "..")))
        {
            continue;
        }

        /*Get the stat of a file*/
        i4_ret = lstat(dirent->d_name, &stat_buf);
        if(i4_ret == -1)
        {
            closedir(dir);
            return FMR_ACCESS;
        }

        /*whether the file is a drectory*/
        if(S_ISDIR(stat_buf.st_mode) && !S_ISLNK(stat_buf.st_mode))
        {
            /*calculate the length of sub-directory path, the former '1' is for '/', the later '1' for '\0'*/
            UINT32 ui4_len = strlen(ps_dir_path) + 1 + strlen(dirent->d_name) + 1;

            /*alloc memory for FM_SEARCH_STACK_ELEM and sub-directroy path name, the address of sub-directory name is after 
              the end of FM_SEARCH_STACK_ELEM*/
            FM_SEARCH_STACK_ELEM *pt_st_elem = (FM_SEARCH_STACK_ELEM *)x_mem_alloc(sizeof(FM_SEARCH_STACK_ELEM) + ui4_len * sizeof(CHAR));
            if(pt_st_elem == NULL)
            {
                closedir(dir);
                return FMR_NOT_ENOUGH_SPACE;
            }
            pt_st_elem->ps_dir_path = (CHAR *)pt_st_elem + sizeof(FM_SEARCH_STACK_ELEM);

            /*get absolute directory path and push to stack*/
            x_strcpy(pt_st_elem->ps_dir_path, ps_dir_path);
            if(!_fm_search_str_endwith(ps_dir_path, '/'))
            {
                x_strncat(pt_st_elem->ps_dir_path, "/", x_strlen("/"));
            }
            x_strncat(pt_st_elem->ps_dir_path, dirent->d_name, x_strlen(dirent->d_name));
            pt_st_elem->pt_next = NULL;
            PUSH(pt_stack, pt_st_elem);
        }
    }
    
    closedir(dir);

    DBG_INFO(("%s end.\n", __func__));
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_search_dir
 *
 * Description: get matched files in a directory
 *
 * Inputs:  ps_dir_path     the searched directory path 
 *          ps_pattern      the string to matched
 *          pt_result       the pointer of search result structure
 *
 * Outputs: -
 *
 * Returns: FMR_OK      Success.
 *          FMR_ARG     Paramter is NULL.
 *          FMR_ACCESS  Call system interface fail.
 ----------------------------------------------------------------------------*/

static INT32 _fm_search_dir(
    const CHAR *ps_dir_path, 
    const CHAR *ps_pattern, 
    FM_SEARCH_RESULT_T *pt_result)
{
    DBG_INFO(("%s enter.\n", __func__));
    if(ps_dir_path == NULL || ps_pattern == NULL || pt_result == NULL)
    {
        return FMR_ARG;
    }
    DBG_INFO(("Search file in directory: %s.\n", ps_dir_path));
    glob_t glob_result;
    
    INT32 i4_ret = chdir(ps_dir_path);
    if (i4_ret == -1)
    {
        DBG_ERROR(("Change dir failed. Error: %s\n", strerror(errno)));
        return FMR_ACCESS;
    }
    
    i4_ret = glob(ps_pattern, 0 , _fm_search_errfunc, &glob_result);
    if(i4_ret == -1)
    {
        DBG_ERROR(("Search directory. Error: %s\n", strerror(errno)));
        return FMR_ACCESS;
    }

    i4_ret = FMR_OK;
    
    /*whether find matched file*/
    if(glob_result.gl_pathc > 0)
    {
        i4_ret = _fm_search_copy_glob_result(ps_dir_path, pt_result, &glob_result);
    }
    
    globfree(&glob_result);
    DBG_INFO(("%s end.\n", __func__));

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_search
 *
 * Description: get all matched files in a directory and its sub-directory
 *
 * Inputs:  ps_dir_path     the searched directory path 
 *          ps_keyword      the string to matched
 *          pt_result       the pointer of search result structure
 *
 * Outputs: -
 *
 * Returns: FMR_OK      Success.
 *          FMR_ARG     Paramter is NULL.
 ----------------------------------------------------------------------------*/
static INT32 _fm_search(
    const char *ps_dir_path, 
    const char *ps_keyword,
    FM_SEARCH_RESULT_T *pt_result)
{
    if(ps_dir_path == NULL || ps_keyword == NULL || pt_result == NULL)
    {
        return FMR_ARG;
    }
    
    INT32 i4_ret = FMR_OK;
    CHAR ps_pattern[FM_PATTERN_LEN];
    FM_SEARCH_STACK st;
    FM_SEARCH_STACK *pt_stack = &st;

    x_strcpy(ps_pattern, "*");
    x_strncat(ps_pattern, ps_keyword, FM_PATTERN_LEN - 2 * strlen("*") - 1);
    x_strncat(ps_pattern, "*", 2);

    i4_ret = _fm_search_result_init(pt_result);
    STACK_INIT(pt_stack);
    
    UINT32 ui4_len = strlen(ps_dir_path) + 1;

    /*alloc memory for FM_SEARCH_STACK_ELEM and sub-directroy path name, the address of sub-directory name is after 
      the end of FM_SEARCH_STACK_ELEM*/
    FM_SEARCH_STACK_ELEM *pt_st_elem = (FM_SEARCH_STACK_ELEM *) x_mem_alloc(sizeof(FM_SEARCH_STACK_ELEM) + ui4_len);
    if(pt_st_elem == NULL)
    {
        return FMR_NOT_ENOUGH_SPACE;
    }
    pt_st_elem->ps_dir_path = (CHAR *)pt_st_elem + sizeof(FM_SEARCH_STACK_ELEM);

    /*push directory path which need to be search*/
    x_strcpy(pt_st_elem->ps_dir_path, ps_dir_path);
    pt_st_elem->pt_next = NULL;
    
    PUSH(pt_stack, pt_st_elem);

    /*loop until stack is empty*/
    while(!STACK_EMPTY(pt_stack) && gb_continue_search)
    {
        POP(pt_stack, pt_st_elem);
        if(pt_st_elem != NULL)
        {   
            /*search matched files in directory*/
            i4_ret = _fm_search_dir(pt_st_elem->ps_dir_path, ps_pattern, pt_result);
            if(i4_ret != FMR_OK)
            {
                fm_search_result_free(pt_result);
                FREE_STACK(pt_stack);
                return i4_ret;
            }

            /*get sub-directory and push to stack*/
            i4_ret = _fm_search_get_subdir_name(pt_st_elem->ps_dir_path, pt_stack);
            if(i4_ret != FMR_OK)
            {
                fm_search_result_free(pt_result);
                FREE_STACK(pt_stack);
                return i4_ret;
            }

            x_mem_free(pt_st_elem);
            pt_st_elem = NULL;
        }
    }

    if(!gb_continue_search)
    {
        fm_search_result_free(pt_result);
    }
    return FMR_OK; 
}

VOID _fm_search_async(VOID* pv_data)
{
    if(pv_data == NULL)
    {
        DBG_ERROR(("[FM]%s #%d argument is invalid\n", __FUNCTION__,__LINE__));
        return;
    }

    _FM_SEARCH_ASYNC_ARG* pt_arg = (_FM_SEARCH_ASYNC_ARG*)pv_data;
    if(pt_arg == NULL)
    {
        DBG_ERROR(("[FM]%s #%d paramter is invalid\n", __FUNCTION__,__LINE__));
        return;
    }

    INT32 i4_ret = _fm_search(pt_arg->s_dir_path, pt_arg->s_keyword, pt_arg->pt_result);
    if(i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM]%s #%d search process occur error\n", __FUNCTION__,__LINE__));
    }
    else if(!gb_continue_search)
    {
        DBG_ERROR(("[FM]%s #%d search process was stopped\n", __FUNCTION__,__LINE__));
    }
    else
    {
        gpf_search_nfy(pt_arg->pt_result);
    }
     
    gb_continue_search = FALSE;
    gpf_search_nfy = NULL;

    return;
}


INT32 fm_search(
    const char *ps_dir_path, 
    const char *ps_keyword,
    FM_SEARCH_RESULT_T *pt_result)
{
    gb_continue_search = TRUE;
    return _fm_search(ps_dir_path, ps_keyword, pt_result);
}



INT32 fm_search_async(
    const char *ps_dir_path, 
    const char *ps_keyword,
    x_fm_search_nfy pf_search_nfy,
    FM_SEARCH_RESULT_T *pt_result)
{
    if(ps_dir_path == NULL || ps_keyword == NULL || pt_result == NULL || pf_search_nfy == NULL)
    {
        DBG_ERROR(("[FM]%s #%d argument is invalid\n", __FUNCTION__,__LINE__));
        return FMR_ARG;
    }

    gb_continue_search = TRUE;
    gpf_search_nfy = pf_search_nfy;

    _FM_SEARCH_ASYNC_ARG* pt_arg = x_mem_alloc(sizeof(_FM_SEARCH_ASYNC_ARG));
    if(pt_arg == NULL)
    {
        DBG_ERROR(("[FM]%s #%d alloc memery fail\n", __FUNCTION__,__LINE__));
        return FMR_NOT_ENOUGH_SPACE;
    }
    x_memset(pt_arg, 0, sizeof(_FM_SEARCH_ASYNC_ARG));
    x_strcpy(pt_arg->s_dir_path, ps_dir_path);
    x_strcpy(pt_arg->s_keyword, ps_keyword);
    pt_arg->pt_result = pt_result;

    HANDLE_T h_thread = NULL_HANDLE;
    INT32 i4_ret = FMR_OK;
    i4_ret = x_thread_create(&h_thread, 
                             "async_search", 
                             4096,
                             128, 
                             _fm_search_async,
                             sizeof(_FM_SEARCH_ASYNC_ARG),
                             (VOID*)pt_arg);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM] %s #%d: return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        i4_ret = FMR_CORE;
    }

    if(pt_arg != NULL)
    {
        x_mem_free(pt_arg);
    }

    return i4_ret;
}

VOID fm_search_stop(VOID)
{
    gb_continue_search = FALSE;
}

#endif
