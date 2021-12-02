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
 * $RCSfile: dlm_fm.c,v $
 * $Revision: #2 $
 * $Date: 2012/08/22 $
 * $Author: muran.xiang $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#include "x_dlm.h"
#include "dlm_fm.h"


#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
    #include <linux/string.h>
#else
#ifdef CC_MTK_LOADER
    #include "x_dbg.h"
    #include "x_fm.h"
#else
    #include <stdio.h>
    #include <stdlib.h>
#endif /* CC_MTK_LOADER */
    #include <string.h>
#endif /* CC_UBOOT */
#else
    #include "os/inc/x_os.h"
    #include "dbg/x_dbg.h"
    #include "file_mngr/x_fm.h"
#endif

static DLM_IO_T t_my_io;

/*-----------------------------------------------------------------------------
 * Name: x_dlm_get_default_io
 *
 * Description: return default I/O APIs
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
DLM_IO_T* x_dlm_get_default_io(VOID)
{
    t_my_io.open_source = dlm_fopen;
    t_my_io.close_source = dlm_fclose;
    t_my_io.read_data = dlm_fread;
    t_my_io.write_data = dlm_fwrite;
    t_my_io.eof_data = dlm_feof;
    t_my_io.data_size = dlm_file_size;
    t_my_io.seek_source = dlm_fseek; 

    return &t_my_io;
}

#ifdef CC_MTK_LOADER
extern UINT32 LDR_GetOADPkgPhyAddress(void);
extern UINT32 LDR_GetOADPkgSize(void);
extern UINT8 LDR_IsOADUpgradeEnabled(void);
UINT32 u4OADPkgOffset = 0;
#define DLM_DEUG_EN 0
#endif
/*-----------------------------------------------------------------------------
 * Name: dlm_fopen
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_fopen(CHAR *ps_filename, DLM_OPEN_MODE_T e_mode)
{
#if defined(NO_MIDDLEWARE) && !defined(CC_MTK_LOADER)
#ifdef CC_UBOOT
    ux_file_open(ps_filename);
    return (ps_filename);
#else /* CC_UBOOT */
    FILE *file;
    CHAR s_mode[3] = "rb";
    int offset = 0;

    if (e_mode == kDlm_Write)
        strcpy(s_mode, "wb");
    else if (e_mode == kDlm_Append)
        strcpy(s_mode, "ab");

    if (*ps_filename == '/')
        offset = 8;

    if ((file = fopen(ps_filename+offset, s_mode)) == NULL)
    {
        printf("Open file %s failed\n", ps_filename+offset);
        return -1;
    }

    return(INT32) file;
#endif /* CC_UBOOT */
#else
    HANDLE_T h_file;
    UINT32 ui4_mode = FM_READ_ONLY;
    INT32 i4_ret;
    INT32 i4_cache = FALSE;

#ifdef CC_MTK_LOADER

#if DLM_DEUG_EN
    Printf("dlm_fopen: %s, 0x%x\n", *ps_filename, e_mode);
#endif
    if(LDR_IsOADUpgradeEnabled() == 1)
    {
        return LDR_GetOADPkgPhyAddress();
    }
#endif
    if (e_mode == kDlm_Write)
    {
        i4_cache = TRUE;
        ui4_mode = (FM_WRITE_ONLY | FM_OPEN_CREATE | FM_OPEN_TRUNC);

    }
    else if (e_mode == kDlm_Append)
    {
        ui4_mode = (FM_WRITE_ONLY | FM_OPEN_CREATE | FM_OPEN_APPEND);
    }

    x_dbg_stmt("open File = %s\n", ps_filename);

    i4_ret = x_fm_open( FM_ROOT_HANDLE, ps_filename, ui4_mode, 0777, i4_cache , &h_file);

    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open %s error[%d]!\n", ps_filename, i4_ret);
        return -1;
    }
    return(INT32) h_file;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: dlm_fclose
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_fclose(INT32 i4_fd)
{
#if defined(NO_MIDDLEWARE) && !defined(CC_MTK_LOADER)
#ifdef CC_UBOOT
    return 0;
#else /* CC_UBOOT */
    INT32 i4_ret;
    i4_ret = fclose((FILE *) i4_fd);
    if (i4_ret != 0 )
    {
        printf("Close file failed\n");
        return -1;
    }
#endif /* CC_UBOOT */
#else
#ifdef CC_MTK_LOADER

#if DLM_DEUG_EN
    Printf("dlm_fclose: 0x%x\n", i4_fd);
#endif

    if(LDR_IsOADUpgradeEnabled() == 1)
    {
        u4OADPkgOffset = 0;
        return 0;
    }
#endif

    if (x_fm_close((HANDLE_T) i4_fd) != FMR_OK)
    {
        x_dbg_stmt("Close file failed\n");
        return -1;
    }
#endif

    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_fread
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
/* return -1 for failed else return read cnt */
INT32 dlm_fread(INT32 i4_fd, CHAR *pc_buf, UINT32 ui4_buf_size)
{
    INT32 i4_read;
#ifdef CC_MTK_LOADER
    UINT32 i = 0;
    UINT8 * buffer;
#endif

#if defined(NO_MIDDLEWARE) && !defined(CC_MTK_LOADER)
#ifdef CC_UBOOT
    return ux_file_read(i4_fd, pc_buf, ui4_buf_size);
#else /* CC_UBOOT */
    i4_read = fread(pc_buf, sizeof(CHAR), ui4_buf_size, (FILE *) i4_fd);
    return i4_read;
#endif /* CC_UBOOT */
#else
    INT32  i4_ret;
    i4_ret = x_fm_read( (HANDLE_T) i4_fd, pc_buf, ui4_buf_size, (UINT32*) &i4_read);
#ifdef CC_MTK_LOADER
#if DLM_DEUG_EN

    Printf("dlm_freaddd: 0x%x, 0x%x, i4_read = 0x%x, i4_ret= 0x%x\n", i4_fd, ui4_buf_size, i4_read, i4_ret);
#endif
    if(LDR_IsOADUpgradeEnabled() == 1)
    {
        buffer = LDR_GetOADPkgPhyAddress()+u4OADPkgOffset;
        
        if((u4OADPkgOffset + ui4_buf_size) >= LDR_GetOADPkgSize())
        {
            ui4_buf_size = LDR_GetOADPkgSize() - u4OADPkgOffset;
            u4OADPkgOffset = LDR_GetOADPkgSize();
        }
        else
        {
            u4OADPkgOffset += ui4_buf_size;
        }
        for(i = 0; i<ui4_buf_size; i++)
        {
            *(pc_buf+i) = *(buffer+i);
        }
        

        return ui4_buf_size;
    }
#endif

    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("Read data from file failed(%d)\n", i4_ret);
        return -1;
    }
    return i4_read;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: dlm_feof
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_feof(INT32 i4_fd)
{
#if defined(NO_MIDDLEWARE) && !defined(CC_MTK_LOADER)
#ifdef CC_UBOOT
    return ux_file_eof(i4_fd);
#else /* CC_UBOOT */
    return feof((FILE *) i4_fd);
#endif /* CC_UBOOT */
#else
    BOOL b_flag;  /* true =1, false =0 */
    INT32 retval = 0;

    retval = x_fm_feof((HANDLE_T) i4_fd, &b_flag);
#ifdef CC_MTK_LOADER
#if DLM_DEUG_EN
    Printf("dlm_feof: 0x%x, 0x%x, pkgsize: 0x%x, 0x%x\n", i4_fd, b_flag, LDR_GetOADPkgSize(), u4OADPkgOffset);
#endif
    if(LDR_IsOADUpgradeEnabled() == 1)
    {
        if(u4OADPkgOffset >= LDR_GetOADPkgSize())
        {
            return 1;
        }
        return 0;
    }
#endif
    if (retval != FMR_OK)
    {
        x_dbg_stmt("Error in x_fm_feof\n");
        return -1;
    }
    

    return (INT32) b_flag;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: dlm_fwrite
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_fwrite(INT32 i4_fd, CHAR *pc_buffer, UINT32 ui4_buf_size)
{
#ifdef CC_MTK_LOADER
#if DLM_DEUG_EN
    Printf("dlm_fwrite: 0x%x, 0x%s, 0x%x\n", i4_fd, *pc_buffer, ui4_buf_size);
#endif
#endif
#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
    return -1;
#else /* CC_UBOOT */
#ifdef CC_MTK_LOADER
    return -1;
#else
    if(ui4_buf_size != fwrite(pc_buffer, sizeof(CHAR), ui4_buf_size, (FILE *) i4_fd))
    {
        printf("Write data into file failed\n");
        return -1;
    }
    return ui4_buf_size;
#endif /* CC_MTK_LOADER */
#endif /* CC_UBOOT */
#else
    UINT32 ui4_write;

    if (x_fm_write((HANDLE_T) i4_fd, pc_buffer, ui4_buf_size, &ui4_write) != FMR_OK)
    {
        x_dbg_stmt("Write data into file failed\n");
        return -1;
    }
    return (INT32) ui4_buf_size;
#endif
} 

/*-----------------------------------------------------------------------------
 * Name: dlm_file_size
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
UINT32 dlm_file_size(CHAR *filename)
{
    UINT32 ui4_sum = 0;
#if defined(NO_MIDDLEWARE) && !defined(CC_MTK_LOADER)
#ifdef CC_UBOOT
    return -1;
#else /* CC_UBOOT */
    FILE *file;
    UINT32 ui4_len = 0;
    
    char buf[1024];
    
    if ((file=fopen(filename, "rb")) != NULL)
    {
       while (!feof(file))
       {
          ui4_len = fread(buf, 1, 1024, file);
          ui4_sum += ui4_len;
        }
       fclose (file);       
    }
#endif /* CC_UBOOT */
#else
    HANDLE_T h_file;
    UINT32 ui4_mode = FM_READ_ONLY;
    INT32 i4_ret;
    FM_FILE_INFO_T  t_info;
#ifdef CC_MTK_LOADER
#if DLM_DEUG_EN

    Printf("dlm_file_size: %s\n", filename);
#endif

    if(LDR_IsOADUpgradeEnabled() == 1)
    {
        return LDR_GetOADPkgSize();
    }
#endif
    i4_ret = x_fm_open( FM_ROOT_HANDLE, filename, ui4_mode, 0777, FALSE, &h_file);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Open %s error[%d]!\n", filename, i4_ret);
        return 0;
    }

    if (x_fm_get_info_by_handle(h_file, &t_info) != FMR_OK)
    {
        x_dbg_stmt("Get information %s error[%d]!\n", filename, i4_ret);
        return 0;
    }

    if (x_fm_close(h_file)!=FMR_OK)
    {
        x_dbg_stmt("Get information %s error[%d]!\n", filename, i4_ret);
        return 0;
    }
    ui4_sum = (UINT32) t_info.ui8_size;
#endif

    x_dbg_stmt("file %s size = %d\n", filename, (int)ui4_sum);
    return ui4_sum;
}

/*-----------------------------------------------------------------------------
 * Name: dlm_delete_file
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_delete_file(CHAR *ps_filename)
{
#ifdef CC_MTK_LOADER
#if DLM_DEUG_EN
    Printf("dlm_delete_file: %s\n", *ps_filename);
#endif
#endif
#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
    return -1;
#else /* CC_UBOOT */
#ifdef CC_MTK_LOADER
    return -1;
#else
    UCHAR str[128];
    INT32 offset = 0;

    if (*ps_filename == '/')
        offset = 8;

    #ifdef _WIN32_
        sprintf(str, "del %s", ps_filename+offset);
    #else
        sprintf(str, "rm %s", ps_filename+offset);
    #endif

    return system(str);
#endif /* CC_MTK_LOADER */
#endif /* CC_UBOOT */
#else
    INT32 i4_ret;

    x_dbg_stmt("Remove File = %s\n", ps_filename);
    i4_ret = x_fm_delete_file(FM_ROOT_HANDLE, ps_filename);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Deletion Error[%d].\n", i4_ret);
        return -1;
    }
    return 0;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: dlm_rename_file
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_rename_file(CHAR *ps_old_file, CHAR *ps_new_file)
{
#ifdef NO_MIDDLEWARE
#ifdef CC_UBOOT
    return -1;
#else /* CC_UBOOT */
#ifdef CC_MTK_LOADER
    return -1;
#else
    UCHAR str[128];
    INT32 offset1 = 0;
    INT32 offset2 = 0;

    if (*ps_old_file == '/')
        offset1 = 8;

    if (*ps_new_file == '/')
        offset2 = 8;

    #ifdef _WIN32_
        sprintf(str, "ren %s %s", ps_old_file+offset1, ps_new_file+offset2);
    #else
        sprintf(str, "mv %s %s", ps_old_file+offset1, ps_new_file+offset2);
    #endif

    return system (str);
#endif /* CC_MTK_LOADER */
#endif /* CC_UBOOT */
#else
    INT32 i4_ret;
    x_dbg_stmt("Rename %s to %s\n",ps_old_file,ps_new_file);
    i4_ret = x_fm_rename(FM_ROOT_HANDLE, ps_old_file, FM_ROOT_HANDLE, ps_new_file);
    if ( i4_ret != FMR_OK)
    {
        x_dbg_stmt("Rename Error[%d].\n", i4_ret);
        return -1;
    }
    return 0;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: dlm_fseek
 *
 * Description: 
 *
 * Inputs:  
 * Outputs: 
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 dlm_fseek(INT32 i4_file, UINT32 ui4_offset)
{
#if defined(NO_MIDDLEWARE) && !defined(CC_MTK_LOADER)
#ifdef CC_UBOOT
    return -1;
#else /* CC_UBOOT */
    if (fseek((FILE *) i4_file, (long) ui4_offset, SEEK_CUR)!=0)
    {
        printf("seek file to other position failed\n");
        return -1;
    }
#endif /* CC_UBOOT */
#else
    UINT64 ui4_dummy;
    UINT64 ui8_offset = (UINT64) ui4_offset;
    INT32 i4_ret;
#ifdef CC_MTK_LOADER
#if DLM_DEUG_EN
    Printf("dlm_fseek:0x%x, 0x%x\n", i4_file, ui4_offset);
#endif
#endif
    i4_ret = x_fm_lseek((HANDLE_T) i4_file, (INT64) ui8_offset, FM_SEEK_CUR, &ui4_dummy);
    if (i4_ret !=FMR_OK)
    {
        x_dbg_stmt("Seek file Error[%d].\n", i4_ret);
        return -1;
    }
#endif
    return 0;
}
