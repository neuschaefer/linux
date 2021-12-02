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
 * $RCSfile: dm.c,v $
 * $Revision: #4 $
 * $Date: 2012/08/28 $
 * $Author: xiuqin.wang $
 *
 * Description: 
 *         This file contains Device manager internal functions.  
 *---------------------------------------------------------------------------*/

#include "dm.h"
#include "dm_cli.h"
#include "dm_util.h"
#include "dbg/x_dbg.h"
#include "handle/handle.h"
#include "handle/x_handle.h"
#include "inc/common.h"
#include "util/x_lnk_list.h"
#include "os/inc/x_os.h"
#include "rwlock/x_rwlock.h"
#include "file_mngr/x_fm.h"

#include "stdio.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <scsi/scsi.h>
#include <linux/netlink.h>
#include <errno.h>
#include <time.h>

#include "u_dm.h"
#include "dm_mtp.h"
#include "dm_coredump.h"

#ifndef SCSI_IOCTL_GET_DEV_SIZE
#define SCSI_IOCTL_GET_DEV_SIZE   0x5388
#endif
#ifndef SCSI_IOCTL_GET_SECTOR_SIZE
#define SCSI_IOCTL_GET_SECTOR_SIZE 0x5389
#endif
#ifndef SCSI_IOCTL_GET_SECTOR_NUMBER
#define SCSI_IOCTL_GET_SECTOR_NUMBER 0x5390
#endif


#ifdef DLNA_SUPPORT
#include "inet/dlna/x_dlna_dmp_api.h"
#include "inet/dlna/x_dlna_api.h"
#include "dm/x_dm_dlna.h"
#endif

#ifdef SMB_SUPPORT
#include "res_mngr/drv/u_socket_dev.h"
#include "inet/x_net_smb.h"
#endif

static DM_CORE_T t_dm_core;

static HANDLE_T h_dev_root;

int dm_usb_notify_unknown_inteface_change(char *kernel_message);

/*Use netlink as notifier*/
#define UEVENT_BUFFER_SIZE  2048

#if 0
static VOID _dm_check_device_changed(const CHAR* ps_phy_dev_path);
#endif

#if 0
static int get_nl_sock(void);

static int get_nl_sock(void)
{
    struct sockaddr_nl snl;
    const int buffersize = 16 * 1024;
    int ret;
    int sock;

    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    snl.nl_groups = 1;
    
    sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);

    if (sock == -1) 
    {
        printf("Error getting socket: %s", strerror(errno));
        return -1;
    }

    /* set receive buffersize */
    setsockopt(sock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    ret = bind(sock, (struct sockaddr *) &snl, sizeof(struct sockaddr_nl));

    if (ret < 0) 
    {
        printf("Bind failed: %s", strerror(errno));
        close(sock);
        sock = -1;        
    }

    return sock;
}
#endif

static void _dm_get_full_path(
    char *path,
    size_t path_size,
    const char *dir,
    const char *name)
{
    strncpy(path, dir, path_size - 1);
    path[path_size -1] = '\0';
    
    strncat(path, name, path_size - strlen(path) - 1);
}

int dm_mount(char *dev_name, char *mnt_path)
{
    int ret;
    char dir_buf[MAX_MP_PATH_LEN];
    char dev_buf[MAX_DEV_PATH_LEN];
#if defined(DLNA_SUPPORT) || defined(SMB_SUPPORT)
    char temp[32];
#endif
    static UINT16 ui2_unit_id = 0;
    
    x_dbg_stmt("start dm_mount %s. \n", dev_name);
    if (dev_name == NULL || mnt_path == NULL)
    {
        return 0;
    }

    _dm_get_full_path(dir_buf, sizeof(dir_buf), USB_MNT_DIR, dev_name);
    
#if 0
    mkdir(dir_buf, 777);
#else
    ret = x_fm_create_dir(NULL_HANDLE, dir_buf, 777);
#endif

    _dm_get_full_path(dev_buf, sizeof(dev_buf), "/dev/", dev_name);

#ifdef DLNA_SUPPORT
   if (strncmp("dlnadms_", dev_name, strlen("dlnadms_")) == 0)
   {
      x_strcpy(temp,dev_name+ strlen("dlnadms_"));
      ret = x_fm_make_entry(NULL_HANDLE, dev_buf, (FM_MODE_TYPE_DLNA_DMS|FM_MODE_DEV_VIRT|0660), FM_DRVT_DLNA_DMS, atoi(temp));
   }
   else
#endif

#ifdef SMB_SUPPORT
    if (strncmp("smb_", dev_name, strlen("smb_")) == 0)
    {
        strncpy(temp, dev_name+ strlen("smb_"), sizeof(temp) - 1);
        ret = x_fm_make_entry(NULL_HANDLE, dev_buf, (FM_MODE_TYPE_SMB|FM_MODE_DEV_VIRT|0660), FM_DRVT_SMB, atoi(temp));
    }
    else
#endif

    ret = x_fm_make_entry(NULL_HANDLE, dev_buf, (FM_MODE_TYPE_BLK|FM_MODE_DEV_VIRT|0660), FM_DRVT_USB_MASS_STORAGE, ui2_unit_id++);

    ret = x_fm_mount(NULL_HANDLE, dev_buf, NULL_HANDLE, dir_buf);

    x_dbg_stmt("Finish dm_mount %s.\n", dev_name);
    if(ret == 0)
    {
        x_strcpy(mnt_path, dir_buf);
        return 1;
    }
    else
    {
        /*mnt_path[0] = '\0';*/
        x_strcpy(mnt_path, dir_buf);
        return 0;
    }
}

void dm_umount(char * dev_name)
{
    INT32 i4_ret;
    char dir_buf[MAX_MP_PATH_LEN];
    char dev_buf[MAX_DEV_PATH_LEN];

    /* mp:/mnt/usb/xxxx */
    _dm_get_full_path(dir_buf, sizeof(dir_buf), USB_MNT_DIR, dev_name);

    /* device node:/dev/xxxx */
    _dm_get_full_path(dev_buf, sizeof(dev_buf), "/dev/", dev_name);
    
    i4_ret = x_fm_umount(NULL_HANDLE, dir_buf);

    x_dbg_stmt("[DM]umount: %s, ret:%d\n", dir_buf, i4_ret);

    /* Delete the mount point */
    i4_ret = x_fm_delete_dir(NULL_HANDLE, dir_buf);
    #if 0
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[DM]x_fm_delete_dir(%s), ret:%d\n",dir_buf,i4_ret);
    }
    #endif

    /* Delete the device node */
    if (strncmp("dlnadms_", dev_name, strlen("dlnadms_")) == 0)
    {
        i4_ret = x_fm_delete_entry(NULL_HANDLE, dev_buf, (FM_MODE_TYPE_DLNA_DMS | FM_MODE_DEV_VIRT));
    }
    else if (strncmp("smb_", dev_name, strlen("smb_")) == 0)
    {
        i4_ret = x_fm_delete_entry(NULL_HANDLE, dev_buf, (FM_MODE_TYPE_SMB | FM_MODE_DEV_VIRT));
    }
    else
    {
        i4_ret = x_fm_delete_entry(NULL_HANDLE, dev_buf, (FM_MODE_TYPE_BLK | FM_MODE_DEV_VIRT));
    }
    #if 0
    if (i4_ret != FMR_OK)
    {
        x_dbg_stmt("[DM]x_fm_delete_entry(%s), ret:%d\n",dev_buf,i4_ret);
    }
    #endif

}

/* usbfs should be mounted on /proc/bus/usb
 * After usb driver is loaded and before any operation on /proc/bus/usb
 */
int dm_mount_usbfs_once(VOID)
{
    INT32 i4_ret;
    static BOOL b_usbfs_mounted = FALSE;

    if (b_usbfs_mounted == TRUE)
    {
        /* Already mounted. Do Nothing. So the usbfs is mounted only once. */
        return DMR_OK;
    }

    i4_ret = system("mount -t usbfs none /proc/bus/usb");
    if (i4_ret == -1)
    {
        x_dbg_stmt("[DM] Error: Can not mount usbfs on /proc/bus/usb\n");
        return DMR_USB_DRV_NOT_INIT;
    }
    
    b_usbfs_mounted = TRUE;
    x_dbg_stmt("[DM] usbfs mounted.\n");
    return DMR_OK;
}

#if 0
static char *_get_dev_changed(char *input, BOOL *padded)
{
    static char dev_name[MAX_DEV_PATH_LEN];
    char * p;
    x_memset(dev_name, 0 , sizeof(dev_name));
    
#ifdef DLNA_SUPPORT
    if(strncmp("add@/dlna/dlnadms_", input, strlen("add@/dlna/dlnadms_")) == 0)
    {
        x_strcpy(dev_name, input + strlen("add@/dlna/"));
        x_dbg_stmt("[DM]..... ..... dev_name is %s\n", dev_name);

        *padded = TRUE;
        return dev_name;
    }
    else if(strncmp("remove@/dlna/dlnadms_", input, strlen("remove@/dlna/dlnadms_")) == 0)
    {
        x_strcpy(dev_name, input + strlen("remove@/dlna/"));
        x_dbg_stmt("[DM]..... ..... dev_name is %s\n", dev_name);

        *padded = FALSE;
        return dev_name;
    }
    else
#endif
#ifdef SMB_SUPPORT
    if (strncmp ("add@/smb/smb_", input, strlen("add@/smb/smb_")) == 0)
    {
        strncpy(dev_name, input + strlen("add@/smb/"), MAX_DEV_PATH_LEN - 1);
        x_dbg_stmt("[DM]..... ..... dev_name is %s\n", dev_name);

        *padded = TRUE;
        return dev_name;
    }
    else if(strncmp("remove@/smb/smb_", input, strlen("remove@/smb/smb_")) == 0)
    {
        strncpy(dev_name, input + strlen("remove@/smb/"), MAX_DEV_PATH_LEN - 1);
        x_dbg_stmt("[DM]..... ..... dev_name is %s\n", dev_name);

        *padded = FALSE;
        return dev_name;
    }
    else
#endif
    if(strncmp("add@/block/sd", input, strlen("add@/block/sd")) == 0)
    {
        x_strcpy(dev_name, input + strlen("add@/block/"));
        x_dbg_stmt("[DM]..... ..... dev_name is %s\n", dev_name);
        /*To eat the sdX/ if there is /sdX/sdX1...*/
        for(p = dev_name; *p ; p ++)
        {
            if(*p == '/')
            {
                *padded = TRUE;
                x_dbg_stmt(" ..... ..... p + 1 is %s\n", p + 1);
                return p + 1;                
            }
        }
        *padded = TRUE;
        return dev_name;
    }
    else if(strncmp("remove@/block/sd", input, strlen("remove@/block/sd")) == 0)
    {
        x_strcpy(dev_name, input + strlen("remove@/block/"));
        /*To eat the sdX/ if there is /sdX/sdX1...*/
        for(p = dev_name; *p ; p ++)
        {
            if(*p == '/')
            {
                *padded = FALSE;
                return p + 1;                
            }
        }
        *padded = FALSE;
        return dev_name;
    }
    else if(strncmp("change@/", input, strlen("change@/")) == 0)
    {
        char* ps_dev_path = NULL;
        size_t bufpos = 0;
        
        /* skip header */
        bufpos = strlen(input) + 1;
        if (bufpos >= MAX_UEVENT_PAYLOAD) 
        {
            x_dbg_stmt("[DM]invalid message length \n");
            return NULL;
        }
        if (strstr(input, "change@/") == NULL) 
        {
            x_dbg_stmt("[DM]unrecognized message header \n");
            return NULL;
        }

        while (bufpos < MAX_UEVENT_PAYLOAD) 
        {
            char *key;
            size_t keylen;
            key = &input[bufpos];
            keylen = strlen(key);
            if (keylen == 0)
                return NULL;
            bufpos += keylen + 1;

            if (strncmp(key, "DEVPATH=", 8) == 0) 
            {
                ps_dev_path = &key[8];
                x_dbg_stmt("[DM]status changed:device path:%s\n", ps_dev_path);

                _dm_check_device_changed(ps_dev_path);

                return NULL;
            }
        }
        return NULL;
    }
    return NULL;
}
#endif

static BOOL _dm_handle_free_fct(
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID            *pv_obj,
    VOID            *pv_tag,
    BOOL            b_req_handle)
{
    if (b_req_handle == TRUE)
    {
        switch (e_type)
        {
            case DMT_DEV_HANDLE:
            {
                DM_DEV_DESC_T *pt_dev_desc = (DM_DEV_DESC_T *) pv_obj;

                if (pt_dev_desc != NULL)
                {
                    if (pt_dev_desc->ps_phys_dev_path != NULL)
                    {
                        free(pt_dev_desc->ps_phys_dev_path);
                    }
                    x_mem_free(pt_dev_desc);
                }

                break;
            }
            case DMT_NFY_HANDLE:
            {
                DM_NFY_DESC_T *pt_nfy_desc = (DM_NFY_DESC_T *) pv_obj;

                if (pt_nfy_desc != NULL)
                {
                    if (pt_nfy_desc->pt_evt_filtr != NULL)
                    {
                        x_mem_free(pt_nfy_desc->pt_evt_filtr);
                    }

                    x_mem_free(pt_nfy_desc);
                }

                break;
            }
            default:
                DM_ASSERT(0);
        }
    }

    return TRUE;
}



static BOOL _dm_dev_approve(
    DM_COND_T       *pt_cond,
    DM_HW_TYPE_T    t_hw_type,
    DM_DEV_TYPE_T   t_dev_type)
{
    BOOL b_hw = FALSE;
    BOOL b_dev = FALSE;

    if ((pt_cond->t_hw_type == DM_HW_TYPE_UNKNOWN) ||
        (pt_cond->t_hw_type == t_hw_type))
    {
        b_hw = TRUE;
    }

    if ((pt_cond->t_dev_type == DM_DEV_UKW_TYPE_UNKNOWN)  ||
        ((pt_cond->t_dev_type == DM_DEV_SKT_TYPE_UNKNOWN) 
        && (DM_DEV_CHR(t_dev_type) == DM_DEV_CHR_SOCKET)) ||
        ((pt_cond->t_dev_type == DM_DEV_MED_TYPE_UNKNOWN) 
        && (DM_DEV_CHR(t_dev_type) == DM_DEV_CHR_MEDIUM)) ||
        (pt_cond->t_dev_type == t_dev_type))
    {
        b_dev = TRUE;
    }

    return (b_hw && b_dev);
}

static VOID _dm_check_nfy(
    DM_EVT_T        t_evt,
    DM_NFY_DESC_T   *pt_nfy_desc,
    DM_HW_TYPE_T    t_hw_type,
    DM_DEV_TYPE_T   t_dev_type,
    VOID            *pv_data)
{
#if 0
    printf(" CHUN LOG %s, #%d, %s\n", __FILE__, __LINE__, __FUNCTION__);
    if ((DM_EVT_CAT(t_evt) & pt_nfy_desc->t_evt_cat) != 0)
    {
        UINT32 ui4_cnt;
        BOOL b_approve = FALSE;
        printf(" CHUN LOG %s, #%d, %s\n", __FILE__, __LINE__, __FUNCTION__);
        /*for (ui4_cnt = 0; ui4_cnt < pt_nfy_desc->ui4_evt_filtr_ns; ui4_cnt++)
        {
            DM_COND_T *pt_cond = &(pt_nfy_desc->pt_evt_filtr[ui4_cnt]);

            if (_dm_dev_approve(pt_cond, t_hw_type, t_dev_type) == TRUE)
            {
                b_approve = TRUE;
                break;
            }
        }*/
#endif

        BOOL b_approve = FALSE;
        /*TMp */
        b_approve = TRUE;
        /*printf(" CHUN LOG %s, #%d, %s\n", __FILE__, __LINE__, __FUNCTION__);*/
        if ((b_approve == TRUE) && (pt_nfy_desc->pf_nfy != NULL))
        {
            /*printf("[ Fake DM ] #%d, %s\n", __LINE__, __FUNCTION__);*/
            /*printf("[ Fake DM ] Call nfy %x\n", pt_nfy_desc->pf_nfy);*/
            pt_nfy_desc->pf_nfy(t_evt, pv_data, pt_nfy_desc->pv_tag);
            /*printf("[ Fake DM ] nfy %x return\n", pt_nfy_desc->pf_nfy);*/
        }
#if 0
    }
#endif
}


static INT32 _dm_nfy_all(
        DM_EVT_T    t_evt,
        HANDLE_T    h_dev)
{
    DM_HW_TYPE_T  t_hw_type;
    DM_DEV_TYPE_T t_dev_type;
    DM_FS_EVT_DATA_T * pt_req_data;
    HANDLE_TYPE_T t_handle_type;
    DM_DEV_DESC_T *pt_dev_desc = NULL;
    DM_NFY_DESC_T *pt_nfy_desc;

    INT32 i4_ret;

    pt_req_data = (VOID *) x_mem_alloc(sizeof(DM_FS_EVT_DATA_T));
    if (pt_req_data == NULL)
    {
        x_dbg_stmt("Not enough Memory.\n");
        return DMR_NO_MEM;
    }
    
    x_memset(pt_req_data, 0, sizeof(DM_FS_EVT_DATA_T));
    
    x_rwl_read_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT);

    i4_ret = handle_get_type_obj(h_dev, &t_handle_type, (VOID **) &pt_dev_desc);
    if (i4_ret != HR_OK)
    {
        goto EXIT;
    }
     
    x_strcpy(pt_req_data->ps_dev_name, pt_dev_desc->ps_dev_name);
    x_strcpy(pt_req_data->ps_mnt_path, pt_dev_desc->ps_mnt_path);
    x_strcpy(pt_req_data->ps_part_name, pt_dev_desc->ps_part_name);
    
    x_dbg_stmt("Notify with part name :%s\n", pt_req_data->ps_part_name);

    t_hw_type = pt_dev_desc->t_hw_type;
    t_dev_type = pt_dev_desc->t_dev_type;

    x_rwl_release_lock(t_dm_core.h_dev_rwlock);

    pt_req_data->h_dev = h_dev;
    pt_req_data->h_dev_root_lbl = h_dev_root;

    /* When event is MOUNTED/UMOUNTED, set the ui1_usb_port to indicate 
     * which port's status is changed. 
     */
    if (t_evt == DM_FS_EVT_MOUNTED || t_evt == DM_FS_EVT_UNMOUNTED)
    {
    #ifdef LINUX_TURNKEY_SOLUTION
        dm_get_changed_usb_port(pt_dev_desc->ps_phys_dev_path,
                                         &(pt_req_data->ui1_usb_port));
    #endif
    }

    if (x_rwl_read_lock(t_dm_core.h_nfy_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        DM_FATAL("_dm_nfy_req_thread lock error.");
    }

    SLIST_FOR_EACH(pt_nfy_desc, &(t_dm_core.t_nfy_list), t_link)
    {   
        _dm_check_nfy(t_evt, 
                      pt_nfy_desc, 
                      t_hw_type, 
                      t_dev_type, 
                      pt_req_data);
    }
    
    x_rwl_release_lock(t_dm_core.h_nfy_rwlock);

EXIT:
    x_mem_free(pt_req_data);
    return i4_ret;
    
}

#if 0
static INT32 _dm_notify_mounting(DM_EVT_T t_evt)
{
    DM_NFY_DESC_T *pt_nfy_desc = NULL;

    INT32 i4_ret = DMR_OK;

   
    if (t_evt == DM_FS_EVT_MOUNT_START)
    {
        DBG_INFO(("[DM] Mounting started.\n"));
    }
    else if (t_evt == DM_FS_EVT_MOUNT_END)
    {
        DBG_INFO(("[DM] Mounting finished.\n"));
    }

    if (x_rwl_read_lock(t_dm_core.h_nfy_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        DM_FATAL("_dm_nfy_req_thread lock error.");
    }

    SLIST_FOR_EACH(pt_nfy_desc, &(t_dm_core.t_nfy_list), t_link)
    {   
        _dm_check_nfy(t_evt, 
                      pt_nfy_desc, 
                      DM_HW_TYPE_UNKNOWN, 
                      DM_DEV_UKW_TYPE_UNKNOWN, 
                      NULL);
    }
    
    i4_ret = x_rwl_release_lock(t_dm_core.h_nfy_rwlock);

    return i4_ret;
}
#endif

static VOID _dm_nfy_new_reg(DM_NFY_DESC_T *pt_nfy_desc)
{
    DM_HW_TYPE_T  t_hw_type;
    DM_DEV_TYPE_T t_dev_type;
    
    DM_FS_EVT_DATA_T * pt_req_data;
    
    DM_DEV_DESC_T *pt_dev_desc;

    pt_req_data = (VOID *) x_mem_alloc(sizeof(DM_FS_EVT_DATA_T));
    if (pt_req_data == NULL)
    {
        x_dbg_stmt("Not enough Memory.\n");
        return;
    }
     
    x_rwl_read_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT);
    
    SLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
    {
        x_memset(pt_req_data, 0, sizeof(DM_FS_EVT_DATA_T));

        x_strcpy(pt_req_data->ps_dev_name, pt_dev_desc->ps_dev_name);
        x_strcpy(pt_req_data->ps_mnt_path, pt_dev_desc->ps_mnt_path);
        x_strcpy(pt_req_data->ps_part_name, pt_dev_desc->ps_part_name);

        pt_req_data->h_dev = pt_dev_desc->h_dev;
        pt_req_data->h_dev_root_lbl = h_dev_root;

        t_hw_type = pt_dev_desc->t_hw_type;
        t_dev_type = pt_dev_desc->t_dev_type;

        #if 0
        _dm_check_nfy(DM_DEV_EVT_ATTACH, 
                      pt_nfy_desc, 
                      t_hw_type, 
                      t_dev_type, 
                      pt_req_data);
        #endif
        if(pt_dev_desc->fg_mnted)
        {
            /*Auto mounted, notify with mounted*/
            _dm_check_nfy(DM_FS_EVT_MOUNTED, 
                          pt_nfy_desc, 
                          t_hw_type, 
                          t_dev_type, 
                          pt_req_data);
        }
    
    }

    x_rwl_release_lock(t_dm_core.h_dev_rwlock);

    x_mem_free(pt_req_data); 
    
}

#ifdef SMB_SUPPORT
extern VOID* x_dm_smb_get_device(UINT32 ui4_id);
#endif

/*
 *  For that dev is auto mounted, the dev is always added with a mount path.
 */
INT32 dm_add_dev(
    CHAR *          s_kernel_msg,
    CHAR *          s_dev_name,
    CHAR *          s_part_name,
    CHAR *          s_mnt_path,
    DM_HW_TYPE_T    t_hw_type,
    DM_DEV_TYPE_T   t_dev_type)    
{
    DM_DEV_DESC_T *pt_dev_desc;

#if defined(DLNA_SUPPORT) || defined(SMB_SUPPORT)
    char temp[32];
#endif
    INT32 i4_ret;

    if((strlen(s_dev_name)  >= MAX_DEV_PATH_LEN) &&
       (strlen(s_part_name) >= MAX_MP_PATH_LEN) &&
       (strlen(s_mnt_path)  >= MAX_MP_PATH_LEN))
    {
        return DMR_ERR;
    }
    
    if (x_rwl_write_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    SLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
    {
        if (x_strcmp(pt_dev_desc->ps_dev_name, s_dev_name) == 0)
        {
            x_dbg_stmt("Existed dev!\n");
            x_rwl_release_lock(t_dm_core.h_dev_rwlock);    
            return 0;
        }
    }
    
    /*New added device, create it and notify*/

    pt_dev_desc = x_mem_alloc(sizeof(DM_DEV_DESC_T));
    if (pt_dev_desc == NULL)
    {
        x_rwl_release_lock(t_dm_core.h_dev_rwlock);    
        return DMR_NO_MEM;
    }
    x_memset(pt_dev_desc, 0 , sizeof(DM_DEV_DESC_T));

    x_strncpy(pt_dev_desc->ps_dev_name, s_dev_name, DM_MAX_LABEL_NAME_LEN - 1);
    if(s_mnt_path != NULL)
    {
        x_strcpy(pt_dev_desc->ps_mnt_path, s_mnt_path);
    }
    x_strcpy(pt_dev_desc->ps_part_name, s_part_name);

    if(s_mnt_path == NULL)
    {
        x_dbg_stmt("[ DM ] Create device mnted dev:%s, part:%s\n",
               s_dev_name,
               s_part_name);
    }
    else
    {
        x_dbg_stmt("[ DM ] Create device mnted dev:%s, mnt:%s, part:%s\n",
               s_dev_name,
               s_mnt_path,
               s_part_name);
    }
    pt_dev_desc->t_hw_type = t_hw_type;
    pt_dev_desc->t_dev_type = t_dev_type;
    pt_dev_desc->h_dev_root_lbl = h_dev_root;
    pt_dev_desc->fg_found = TRUE;
    pt_dev_desc->fg_mnted = ((s_mnt_path != NULL && t_dev_type != DM_DEV_SKT_TYPE_CARD_READER)?TRUE:FALSE);

    if (t_hw_type == DM_HW_TYPE_ETHERNET && t_dev_type == DM_DEV_MED_TYPE_DLNA_DMS)
    {
#ifdef DLNA_SUPPORT
        if (strncmp("dlnadms_", s_dev_name, strlen("dlnadms_")) == 0)
        {
            x_strcpy(temp,s_dev_name+ strlen("dlnadms_"));
            pt_dev_desc->ui4_unit_id = atoi(temp);
            pt_dev_desc->h_drv = (HANDLE_T)x_dm_dlna_get_device(pt_dev_desc->ui4_unit_id);
        }
#endif
    }
#ifdef SMB_SUPPORT
    if (t_hw_type == DM_HW_TYPE_ETHERNET && t_dev_type == DM_DEV_MED_TYPE_SMB)
    {
        if (strncmp("smb_", s_dev_name, strlen("smb_")) == 0)
        {
            strncpy(temp,s_dev_name+ strlen("smb_"), sizeof(temp) - 1);
            pt_dev_desc->ui4_unit_id = atoi(temp);
            pt_dev_desc->h_drv = (HANDLE_T)x_dm_smb_get_device(pt_dev_desc->ui4_unit_id);
        }
    }
#endif
 
    i4_ret = handle_alloc((HANDLE_TYPE_T)DMT_DEV_HANDLE,
                          pt_dev_desc,
                          NULL,
                          _dm_handle_free_fct,
                          &(pt_dev_desc->h_dev));
    
    if(i4_ret != HR_OK)
    {
        x_rwl_release_lock(t_dm_core.h_dev_rwlock);  
        return DMR_HANDLE;
    }

    do
    {
        if (s_kernel_msg != NULL)
        {
            size_t bufpos = 0;
            
            /* skip header */
            bufpos = strlen(s_kernel_msg) + 1;
            if (bufpos >= MAX_UEVENT_PAYLOAD) 
            {
                x_dbg_stmt("[DM]invalid message length \n");
                break;
            }
            if (strstr(s_kernel_msg, "add@/") == NULL) 
            {
                x_dbg_stmt("[DM]unrecognized message header \n");
                break;
            }

            while (bufpos < MAX_UEVENT_PAYLOAD) 
            {
                char *key;
                size_t keylen;
                key = &s_kernel_msg[bufpos];
                keylen = strlen(key);
                if (keylen == 0)
                    break;
                bufpos += keylen + 1;

                if (strncmp(key, "PHYSDEVPATH=", 12) == 0) 
                {
                    pt_dev_desc->ps_phys_dev_path = strdup(&key[12]);
                    x_dbg_stmt("%s's physical device path:%s\n", s_part_name, pt_dev_desc->ps_phys_dev_path);
                    break;
                }
             }
        }
    } while(0);

    SLIST_INSERT_HEAD(pt_dev_desc, &(t_dm_core.t_dev_list), t_link);

    x_rwl_release_lock(t_dm_core.h_dev_rwlock);    

    if(pt_dev_desc->fg_mnted)
    {
        _dm_nfy_all(
                DM_FS_EVT_MOUNTED,
                pt_dev_desc->h_dev);
    }        
    return 0;
}

INT32 dm_remove_dev(CHAR *s_dev_name)
{
    DM_DEV_DESC_T *pt_dev_desc; 
    if (x_rwl_write_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    SLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
    {
        if (x_strcmp(pt_dev_desc->ps_dev_name, s_dev_name) == 0)
        {
            x_dbg_stmt("[DM]Remove existed dev!\n");
            SLIST_REMOVE(pt_dev_desc, t_link);
            x_rwl_release_lock(t_dm_core.h_dev_rwlock);  

            /*
             * For automounted devices, notify unmounted no matter it was successfully unmounted
             */
            if(pt_dev_desc->fg_mnted)
            {
                _dm_nfy_all(
                        DM_FS_EVT_UNMOUNTED,
                        pt_dev_desc->h_dev);
            }
            #if 0
            _dm_nfy_all(
                    DM_DEV_EVT_DETACH,
                    pt_dev_desc->h_dev);
            #endif

            x_handle_free(pt_dev_desc->h_dev);
            
            return 0;
        }
    }
    
    x_rwl_release_lock(t_dm_core.h_dev_rwlock);
    return 0;
}

#if 0
/*
 * Get devs when system is inited.
 */
static void _dm_update_devs()
{
    FILE *  fp;
    char    read_buf[1024];
    char    s_dev_name[MAX_DEV_PATH_LEN];
    char    s_mnt_name[MAX_MP_PATH_LEN];
    
    fp = fopen("/proc/diskstats", "r");
    if (fp == NULL)
    {
       return;
    }
    
    while(!feof(fp))
    {
        if(!fgets(read_buf, 1023 , fp))
        {
            break;
        }
        sscanf(read_buf, "%*d %*d %32s %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u", 
               s_dev_name);
            
        /*printf("[DM] From /proc/diskstats %s\n", s_dev_name);*/
        /*Just look for sdX*/
        if(strncmp("sd", s_dev_name, 2) == 0)
        {
            int mount_ok = 0;
            mount_ok = dm_mount(s_dev_name, s_mnt_name);
            if(x_strlen(s_mnt_name) > 0)
            {
                /*Simply ignored diff with part name and dev name*/
                if (mount_ok == 1)
                {
                    dm_add_dev(
                            NULL,
                            s_dev_name,
                            s_dev_name,
                            s_mnt_name,
                            DM_HW_TYPE_USB,
                            DM_DEV_MED_TYPE_MASS_STRG);

                    dm_core_file_enable(s_mnt_name);
                }
                else
                {
                    dm_add_dev(
                            NULL,
                            s_dev_name,
                            s_dev_name,
                            s_mnt_name,
                            DM_HW_TYPE_USB,
                            DM_DEV_SKT_TYPE_CARD_READER);
                }
            }
        }
    }
    fclose(fp);
    
    /*printf("[DM] %s exit\n", __FUNCTION__);*/
}
#endif

#if 0
static VOID _dm_open_dev_thread(
    VOID *pv_data)
{
    INT32 i4_ret;
    INT32 i4_fd;
    DM_DEV_DESC_T *pt_dev_desc; 
    char dev_buf[256];
    
    x_dbg_stmt("[DM] %s: entered\n", __FUNCTION__);

    while(1)
    {
        if (x_rwl_read_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
        {
            i4_ret = DMR_OS;
            goto EXIT;
        }

        SLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
        {
            if ((pt_dev_desc->ps_dev_name != NULL) && 
                (pt_dev_desc->t_hw_type == DM_HW_TYPE_USB))
            {
                _dm_get_full_path(dev_buf, sizeof(dev_buf), 
                                   "/dev/", pt_dev_desc->ps_dev_name);
                i4_fd = open(dev_buf, O_RDONLY | O_NONBLOCK);

                if (i4_fd >= 0)
                {
                    close(i4_fd);
                }
                else
                {
                    /*x_dbg_stmt("[DM] %s: open %s fail:%s\n", __FUNCTION__, dev_buf, strerror(errno));*/
                }

                dev_buf[0] = '\0';
            }
        }

        x_rwl_release_lock(t_dm_core.h_dev_rwlock);
        #if 0
        x_thread_delay(1000);
        #else
        {
            struct timespec req;
            
            req.tv_sec = 1;
            req.tv_nsec = 0;
            
            nanosleep(&req, NULL);
        }
        #endif
    }

EXIT:
    x_thread_exit();
}
#endif

#if 0
static VOID _dm_check_device_changed(
    const CHAR* ps_phy_dev_path)
{
    INT32 i4_ret;
    INT32 i4_fd;
    DM_DEV_DESC_T *pt_dev_desc = NULL;
    DM_DEV_DESC_T *pt_next_dev_desc = NULL;
    char dev_buf[256];
    /* This flag make the code more complex. But the patch is cleaner for MP code. */
    BOOL rwl_is_released = FALSE;
    
    if (ps_phy_dev_path != NULL)
    {
        x_dbg_stmt("[DM]%s entered(changed device path:%s)...\n", __func__, ps_phy_dev_path);
        do
        {
            if (x_rwl_write_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
            {
                i4_ret = DMR_OS;
                return;
            }
            rwl_is_released = FALSE;

            for (pt_dev_desc = t_dm_core.t_dev_list.pt_head;
                 pt_dev_desc != NULL;
                 pt_dev_desc = pt_next_dev_desc)
            {
                pt_next_dev_desc = pt_dev_desc->t_link.pt_next;
            
                if (pt_dev_desc->ps_dev_name != NULL && 
                    /*(pt_dev_desc->t_dev_type == DM_DEV_SKT_TYPE_CARD_READER) &&*/
                    pt_dev_desc->ps_phys_dev_path != NULL &&
                    !strcmp(pt_dev_desc->ps_phys_dev_path, ps_phy_dev_path) )
                {
                    _dm_get_full_path(dev_buf, sizeof(dev_buf), 
                                      "/dev/", pt_dev_desc->ps_dev_name);
                    i4_fd = open(dev_buf, O_RDONLY);

                    if (i4_fd >= 0)
                    {
                        x_dbg_stmt("[DM] %s: open %s is OK\n", __FUNCTION__, dev_buf);
                        close(i4_fd);

                        x_strcpy(dev_buf, pt_dev_desc->ps_dev_name);

                        DLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
                        {
                            if (strstr(pt_dev_desc->ps_dev_name, dev_buf)/* &&
                                strcmp(pt_dev_desc->ps_dev_name, dev_buf)*/)
                            {
                                if (!pt_dev_desc->fg_mnted)
                                {
                                    char s_mnt_path[MAX_MP_PATH_LEN];
                                    int mount_ok = 0;
                                    
                                    x_dbg_stmt("[DM]%s, Add %s\n", __func__, pt_dev_desc->ps_dev_name);
                                    
                                    s_mnt_path[0] = '\0';
                                    mount_ok = dm_mount(pt_dev_desc->ps_dev_name, s_mnt_path);
                                    
                                    pt_dev_desc->fg_mnted = ((mount_ok == 1)?TRUE:FALSE);

                                    if (pt_dev_desc->fg_mnted)
                                    {
                                        pt_dev_desc->t_dev_type = DM_DEV_MED_TYPE_MASS_STRG;

                                        /* only do corefile dump on real USB disk, no dlna/smb path */
                                        if(strncmp("sd", pt_dev_desc->ps_dev_name, 2) == 0)
                                        {
                                            dm_core_file_enable(s_mnt_path);
                                        }
                                    }
                                    
                                    #if 0
                                    _dm_nfy_all(
                                            DM_DEV_EVT_ATTACH,
                                            pt_dev_desc->h_dev);
                                    #endif
                                    x_rwl_release_lock(t_dm_core.h_dev_rwlock);
                                    rwl_is_released = TRUE;
                                    if(pt_dev_desc->fg_mnted)
                                    {
                                        _dm_nfy_all(
                                                DM_FS_EVT_MOUNTED,
                                                pt_dev_desc->h_dev);
                                    }  
                                }                                 
                            }
                        }
                    }
                    else
                    {
                        x_dbg_stmt("[DM] %s: open %s fail:%s\n", __FUNCTION__, dev_buf, strerror(errno));

                        x_strcpy(dev_buf, pt_dev_desc->ps_dev_name);
                        {
                            DLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
                            {
                                if (strstr(pt_dev_desc->ps_dev_name, dev_buf)/* &&
                                    strcmp(pt_dev_desc->ps_dev_name, dev_buf)*/)
                                {
                                    if (pt_dev_desc->fg_mnted)
                                    {
                                        x_dbg_stmt("[DM]%s, Remove %s\n", __func__, pt_dev_desc->ps_dev_name);
                                        
                                    /*
                                                           * For automounted devices, notify unmounted no matter it was successfully unmounted
                                                           */
                                        if(strncmp("sd", pt_dev_desc->ps_dev_name, 2) == 0)
                                        {
                                            dm_core_file_disable();
                                        }
                                        dm_umount(pt_dev_desc->ps_dev_name);

                                        _dm_nfy_all(
                                                DM_FS_EVT_UNMOUNTED,
                                                pt_dev_desc->h_dev);

                                        pt_dev_desc->fg_mnted = FALSE;

                                        #if 0
                                        _dm_nfy_all(
                                                DM_DEV_EVT_DETACH,
                                                pt_dev_desc->h_dev);
                                        #endif

                                    }                                 
                                }
                            }
                        }
                            
                    }
                    dev_buf[0] = '\0';
                }
            }
            if (rwl_is_released == FALSE)
            {
                x_rwl_release_lock(t_dm_core.h_dev_rwlock);
            }

        }while(0);
        
        x_dbg_stmt("[DM]%s exited(changed device path:%s)...\n", __func__, ps_phy_dev_path);
    }
}
#endif

#if 0
static VOID _dm_nfy_req_thread(
    VOID *pv_data)
{
    INT32 i4_ret;
    char* kernelmsg=NULL;
    int   sock;
    
    struct sockaddr_nl dest_addr;
    struct iovec iov;
    struct msghdr msg;

    kernelmsg = (char *)x_mem_alloc(MAX_UEVENT_PAYLOAD);
    if (kernelmsg == NULL)
    {
        x_dbg_stmt("Not enough Memory.\n");
        return;
    }
    memset(kernelmsg, 0, MAX_UEVENT_PAYLOAD);

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&iov, 0, sizeof(iov));
    memset(&msg, 0, sizeof(msg));
    
    iov.iov_base = (void *)kernelmsg;
    iov.iov_len = MAX_UEVENT_PAYLOAD;
    
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    
    x_dbg_stmt("[DM] %s enter\n", __FUNCTION__);

    /*
     * Fetch information from pluged devices 
     */
    _dm_update_devs();

    sock = get_nl_sock();
    x_dbg_stmt("[DM] %s get_nl_sock:%d\n", __FUNCTION__, sock);

    while (1)
    {
        i4_ret = recvmsg(sock, &msg, 0);
        if (i4_ret < 0)
        {
            DBG_ERROR(("[USER] Socket recvmsg failed.\n"));
            break;
        }

        DBG_INFO(("[DM uevent]: %s\n", kernelmsg));

        dm_usb_notify_unknown_inteface_change(kernelmsg);

        /* send msg */
        if (strncmp("add@/block/sd", kernelmsg, strlen("add@/block/sd")) == 0 ||
            strncmp("remove@/block/sd", kernelmsg, strlen("remove@/block/sd")) == 0 ||
            strncmp("change@/", kernelmsg, strlen("change@/")) == 0)
            
        {
            i4_ret = x_msg_q_send(t_dm_core.h_cb_msg_q, 
                                  (const VOID *) kernelmsg, 
                                  MAX_UEVENT_PAYLOAD,
                                  DM_CB_MSG_PRIORITY);

            if (i4_ret != OSR_OK)
            {
                DBG_ERROR(("[DM]%s: send msg fail(%d).\n",__FUNCTION__,i4_ret));
            }
            #if 0
            else
            {
                x_dbg_stmt("[DM]:%s, kernelmsg:%s is sent\n",__FUNCTION__, kernelmsg);
            }
            #endif
        }
    }

    x_mem_free(kernelmsg);
    x_dbg_stmt("[DM] %s exit\n", __FUNCTION__);

    x_thread_exit();
}
#endif 

#if 0
static VOID _dm_cb_msg_thread(
    VOID *pv_data)
{
    char *ps_buf = NULL;
    char s_mnt_path[MAX_MP_PATH_LEN];
    UINT16 ui2_q_idx;
    SIZE_T z_msg_size;
    BOOL added = FALSE;
    char * s_dev_name;
    INT32 i4_ret;

    x_dbg_stmt("[DM] %s enter\n", __FUNCTION__);

    ps_buf = (char *)x_mem_alloc(MAX_UEVENT_PAYLOAD);
    if (ps_buf == NULL)
    {
        x_dbg_stmt("Not enough Memory.\n");
        return;
    }
    memset(ps_buf, 0, MAX_UEVENT_PAYLOAD);

    while (1)
    {    
        z_msg_size = MAX_UEVENT_PAYLOAD;
        i4_ret = x_msg_q_receive(&ui2_q_idx,
                                 ps_buf,
                                 &z_msg_size,
                                 &t_dm_core.h_cb_msg_q,
                                 1,
                                 X_MSGQ_OPTION_WAIT);

        if (i4_ret != OSR_OK)
        {
            x_dbg_stmt("_dm_cb_msg_thread receive msg error.\n");
            goto fail;
        }
        added = FALSE;

        s_dev_name = _get_dev_changed(ps_buf, &added);
        if(s_dev_name == NULL)
        {
            x_dbg_stmt("[DM]No dev name found.\n");
            continue;
        }
        x_dbg_stmt("[DM]Dev changed:%s %s\n",added?"Add":"remove", s_dev_name);
        
        if(added == FALSE)
        {
            if(strncmp("sd", s_dev_name, 2) == 0)
            {
                dm_core_file_disable();
            }

            dm_umount(s_dev_name);
            dm_remove_dev(s_dev_name);
        }
        else
        {
            int mount_ok = 0;
            
            s_mnt_path[0] = '\0';

            _dm_notify_mounting(DM_FS_EVT_MOUNT_START);
            mount_ok = dm_mount(s_dev_name,s_mnt_path);
            if (x_strlen(s_mnt_path) == 0)
            {
                x_dbg_stmt("[DM]Mnt path empty.\n");
                _dm_notify_mounting(DM_FS_EVT_MOUNT_END);
                continue;
            }
            
            /*Simply ignored diff with part name and dev name*/
            if(strncmp("sd", s_dev_name, 2) == 0)
            {
                if (mount_ok == 1)
                {
                    dm_add_dev(
                            ps_buf,
                            s_dev_name,
                            s_dev_name,
                            s_mnt_path,
                            DM_HW_TYPE_USB,
                            DM_DEV_MED_TYPE_MASS_STRG);

                    dm_core_file_enable(s_mnt_path);
                }
                else
                {
                    dm_add_dev(
                            ps_buf,
                            s_dev_name,
                            s_dev_name,
                            s_mnt_path,
                            DM_HW_TYPE_USB,
                            DM_DEV_SKT_TYPE_CARD_READER);
                }
            }
            else if ((strncmp("dlnadms", s_dev_name, 7) == 0) && (mount_ok == 1))
            {
                dm_add_dev(
                        NULL,
                        s_dev_name,
                        s_dev_name,
                        s_mnt_path,
                        DM_HW_TYPE_ETHERNET,
                        DM_DEV_MED_TYPE_DLNA_DMS);
            }
            else if ((strncmp("smb", s_dev_name, 3) == 0) && (mount_ok == 1))
            {
                dm_add_dev(
                        NULL,
                        s_dev_name,
                        s_dev_name,
                        s_mnt_path,
                        DM_HW_TYPE_ETHERNET,
                        DM_DEV_MED_TYPE_SMB);
            }

            _dm_notify_mounting(DM_FS_EVT_MOUNT_END);
        }

    }
fail:
    x_mem_free(ps_buf);
    x_dbg_stmt("[DM] %s exit\n", __FUNCTION__);

    x_thread_exit();
}
#endif


INT32 dm_reg_nfy_fct(
    DM_EVT_CAT_T    t_evt_cat,
    x_dm_nfy_fct    pf_nfy,
    DM_EVT_FILTR_T  *pt_filtr,
    VOID            *pv_tag, 
    HANDLE_T        *ph_nfy)
{
    INT32 i4_ret;
    DM_NFY_DESC_T *pt_nfy_desc = NULL;

    if (t_dm_core.b_init == FALSE)
    {
        return DMR_NOT_INIT;
    }

    if ((pf_nfy == NULL) || (pt_filtr == NULL) || (ph_nfy == NULL))
    {
        return DMR_INVALID_PARM;
    }

    if (x_rwl_write_lock(t_dm_core.h_nfy_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    pt_nfy_desc = (DM_NFY_DESC_T *)x_mem_alloc(sizeof(DM_NFY_DESC_T));

    if (pt_nfy_desc == NULL)
    {
        i4_ret = DMR_NO_MEM;
        goto fail;
    }

    pt_nfy_desc->pf_nfy = pf_nfy;
    pt_nfy_desc->fg_new = TRUE;

    pt_nfy_desc->pt_evt_filtr = (DM_COND_T *)x_mem_alloc(
                                sizeof(DM_COND_T) * pt_filtr->ui4_evt_filtr_ns);
    if (pt_nfy_desc->pt_evt_filtr == NULL)
    {
        i4_ret = DMR_NO_MEM;
        goto fail;
    }

    x_memcpy((VOID *) pt_nfy_desc->pt_evt_filtr,
             (const VOID *) pt_filtr->pt_evt_filtr,
             sizeof(DM_COND_T) * pt_filtr->ui4_evt_filtr_ns);

    pt_nfy_desc->pv_tag = pv_tag;
    pt_nfy_desc->t_evt_cat = t_evt_cat;
    pt_nfy_desc->ui4_evt_filtr_ns = pt_filtr->ui4_evt_filtr_ns;

    i4_ret = handle_alloc((HANDLE_TYPE_T)DMT_NFY_HANDLE,
                          pt_nfy_desc,
                          NULL,
                          _dm_handle_free_fct,
                          &(pt_nfy_desc->h_nfy));

    if (i4_ret != HR_OK)
    {
        i4_ret = DMR_HANDLE;
        goto fail;
    }

    *ph_nfy = pt_nfy_desc->h_nfy;

    SLIST_INSERT_HEAD(pt_nfy_desc, &(t_dm_core.t_nfy_list), t_link);

    /*TODO:Get the state and notify*/
    /*if (pt_nfy_desc->t_evt_cat == DM_EVT_CAT_DEV)
    {
        i4_ret = _dm_dev_level_trig(pt_nfy_desc);
    }
    else if (pt_nfy_desc->t_evt_cat == DM_EVT_CAT_FS)
    {
        i4_ret = _dm_fs_level_trig(pt_nfy_desc);
    }
    else if (pt_nfy_desc->t_evt_cat == DM_EVT_CAT_LDR)
    {
        i4_ret = _dm_ldr_level_trig(pt_nfy_desc);
    }*/

    x_rwl_release_lock(t_dm_core.h_nfy_rwlock);

    /*Do "Level" notify */
    _dm_nfy_new_reg(pt_nfy_desc);

    if (i4_ret != DMR_OK)
    {
        goto fail_ext;
    }

    return DMR_OK;

fail:

    if (pt_nfy_desc != NULL)
    {
        if (pt_nfy_desc->h_nfy != NULL_HANDLE)
        {
            handle_free(pt_nfy_desc->h_nfy, FALSE);
        }

        if (pt_nfy_desc->pt_evt_filtr != NULL)
        {
            x_mem_free((VOID *) pt_nfy_desc->pt_evt_filtr);
        }
        x_mem_free((VOID *) pt_nfy_desc);
    }

fail_ext:

    x_rwl_release_lock(t_dm_core.h_nfy_rwlock);

    return i4_ret;
}

INT32 dm_unreg_nfy_fct(
    HANDLE_T h_nfy)
{
    INT32 i4_ret;
    DM_NFY_DESC_T *pt_nfy_desc = NULL;
    HANDLE_TYPE_T t_handle_type;

    if (x_rwl_write_lock(t_dm_core.h_nfy_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    i4_ret = handle_get_type_obj(h_nfy, 
                                 &t_handle_type, 
                                 (VOID **) &pt_nfy_desc);

    if ((i4_ret != HR_OK) || 
        (pt_nfy_desc == NULL) || 
        ((t_handle_type != DMT_NFY_HANDLE)))
    {
        i4_ret = DMR_INVALID_HANDLE;
        goto fail;
    }

    SLIST_REMOVE(pt_nfy_desc, t_link);

    i4_ret = x_handle_free(h_nfy);

    if (i4_ret != HR_OK)
    {
        i4_ret = DMR_INVALID_HANDLE;
        goto fail;
    }

    x_rwl_release_lock(t_dm_core.h_nfy_rwlock);

    return DMR_OK;

fail:

    x_rwl_release_lock(t_dm_core.h_nfy_rwlock);

    return i4_ret;
}


static INT32 _dm_fill_dev_info(char * s_dev_name, DM_DEV_INF_T *pt_inf)
{
    #define SCSI_INFO_READ_BUF_LEN 256
    #define SCSI_NAME_PRODUCT "      Product:"
    
    char path[256];
    char read_buf[SCSI_INFO_READ_BUF_LEN];
    unsigned long long dev_size;
    unsigned  sec_size;
    unsigned long long sec_num;
    
    int fd;
    int bus;
    FILE *fp = NULL;
    int ret;
    int name_len_product = strlen(SCSI_NAME_PRODUCT);

    _dm_get_full_path(path, sizeof(path), "/dev/", s_dev_name);

    fd = open(path, O_RDONLY|O_NONBLOCK);
    if(fd < 0)
    {
        x_dbg_stmt("[FakeDM]Error for open DEV to get scsi ID %s\n", path);
        ret = DMR_ERR;
        goto EXIT;
    }
    if(ioctl(fd, SCSI_IOCTL_GET_BUS_NUMBER, &bus) < 0)
    {
        x_dbg_stmt("[FakeDM]Error to ioctl SCSI_IOCTL_GET_BUS_NUMBER\n");
        ret = DMR_ERR;
        goto EXIT;
    }
    
    if(ioctl(fd, SCSI_IOCTL_GET_DEV_SIZE, &dev_size) < 0)
    {
        x_dbg_stmt("[FakeDM]Error to ioctl SCSI_IOCTL_GET_DEV_SIZE\n");
        ret = DMR_ERR;
        goto EXIT;
    }
    if(ioctl(fd, SCSI_IOCTL_GET_SECTOR_SIZE, &sec_size) < 0)
    {
        x_dbg_stmt("[FakeDM]Error to ioctl SCSI_IOCTL_GET_DEV_SIZE\n");
        ret = DMR_ERR;
        goto EXIT;
    }
    if(ioctl(fd, SCSI_IOCTL_GET_SECTOR_NUMBER, &sec_num) < 0)
    {
        x_dbg_stmt("[FakeDM]Error to ioctl SCSI_IOCTL_GET_DEV_SIZE\n");
        ret = DMR_ERR;
        goto EXIT;
    }
    

    pt_inf->ui4_mem_align = 32;
    pt_inf->u.t_med_inf.ui8_total_size = dev_size*1000*1000;/*dev_size is MB*/
    pt_inf->u.t_med_inf.u.t_blk_inf.ui8_sec_ns = sec_num;
    pt_inf->u.t_med_inf.u.t_blk_inf.ui4_sec_size = sec_size;
    pt_inf->u.t_med_inf.u.t_blk_inf.ui4_max_tx_sec_ns = 2;
    pt_inf->u.t_med_inf.u.t_blk_inf.ui4_best_tx_sec_ns = 2;
    
    x_dbg_stmt("[FakeDM]Successful to get scsi bus id:%d\n", bus);

    x_sprintf(path, "/proc/scsi/usb-storage/%d", bus);
    
    fp = fopen(path, "r");
    if (fp == NULL)
    {
        close(fd);
        return DMR_ERR;
    }
    while(!feof(fp))
    {
        if(!fgets(read_buf, SCSI_INFO_READ_BUF_LEN , fp))
        {
            break;
        }
        if(strncmp(SCSI_NAME_PRODUCT, read_buf, name_len_product) == 0)
        {      
            strncpy((CHAR *)pt_inf->aui1_model_str, read_buf + name_len_product, DM_DEV_MAX_MODEL_NAME_LEN-1); 
            pt_inf->aui1_model_str[DM_DEV_MAX_MODEL_NAME_LEN-1] = '\0';
            UINT32 ui4_len = strlen((CHAR*)(pt_inf->aui1_model_str));
            INT32 i = ui4_len - 1;
            for(; i >=0; i--)
            {
                if(pt_inf->aui1_model_str[i] >= 0 && pt_inf->aui1_model_str[i] < 0x20)
                {
                    //x_dbg_stmt("index: %d, char: %d\n", i, pt_inf->aui1_model_str[i]);
                    pt_inf->aui1_model_str[i] = '\0';
                }
                else
                {
                    break;
                }
            }
        }
        /*
         * Fill other information
         */
    }
    ret = DMR_OK;
    
EXIT:
    close(fd);
    if (fp != NULL)
    {
        fclose(fp);
    }

    return ret;
}


INT32 dm_get_device_info(
    HANDLE_T        h_dev, 
    DM_DEV_INF_T    *pt_inf)
{
    INT32 i4_ret;
    DM_DEV_DESC_T *pt_dev_desc = NULL;

    HANDLE_TYPE_T t_handle_type;

    if (pt_inf == NULL)
    {
        return DMR_INVALID_PARM;
    }

    if (x_rwl_read_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    i4_ret = handle_get_type_obj(h_dev, 
                                 &t_handle_type,
                                 (VOID **) &pt_dev_desc);

    if ((i4_ret != HR_OK) || 
        (pt_dev_desc == NULL) ||
        (t_handle_type != DMT_DEV_HANDLE))
    {
        i4_ret = DMR_INVALID_HANDLE;
        x_rwl_release_lock(t_dm_core.h_dev_rwlock);
        return i4_ret;
    }

    x_memset((VOID *) pt_inf, 0, sizeof(DM_DEV_INF_T));
    
    pt_inf->t_hw_type   = pt_dev_desc->t_hw_type;
    pt_inf->t_dev_type  = pt_dev_desc->t_dev_type;
    pt_inf->b_avail     = TRUE;
    pt_inf->ui4_unit_id = pt_dev_desc->ui4_unit_id;
    pt_inf->ui4_skt_no  = pt_dev_desc->ui4_skt_no;

    pt_inf->pv_data = pt_dev_desc->pv_data;
    pt_inf->ui4_data_size = pt_dev_desc->ui4_data_size;
    
    pt_inf->u.t_med_inf.h_dev_root_lbl = h_dev_root;
    x_strcpy(pt_inf->u.t_med_inf.ps_dev_name, pt_dev_desc->ps_dev_name);

    if (pt_dev_desc->t_hw_type == DM_HW_TYPE_ETHERNET)
    {
        if (pt_dev_desc->t_dev_type == DM_DEV_MED_TYPE_DLNA_DMS)
        {
#ifdef DLNA_SUPPORT
        CHAR* sc_model_name;
        
        i4_ret = x_dlna_get_device_name((DLNA_DMP_DEVICE_T)pt_dev_desc->h_drv, &sc_model_name);

        if (DLNAR_DMP_OK == i4_ret)
        {
            x_strncpy((CHAR*)pt_inf->aui1_model_str, sc_model_name, DM_DEV_MAX_MODEL_NAME_LEN-1);

        }
        else
        {
            x_sprintf((CHAR*)pt_inf->aui1_model_str, "DLNA_DMS_%X", pt_dev_desc->h_drv);

        }
#endif
        }
        if (pt_dev_desc->t_dev_type == DM_DEV_MED_TYPE_SMB)
        {
#ifdef SMB_SUPPORT
        if (pt_dev_desc->h_drv)
        {
            x_strncpy((CHAR*)pt_inf->aui1_model_str, (CHAR*) pt_dev_desc->h_drv, DM_DEV_MAX_MODEL_NAME_LEN-1);

        }
        else
        {
            x_sprintf((CHAR*)pt_inf->aui1_model_str, "SMB_%X", pt_dev_desc->h_drv);

        }
        /*x_dbg_stmt("%s %d model name:%s\n", __FUNCTION__, __LINE__, (CHAR*)pt_inf->aui1_model_str);*/
#endif
        }
    }
    else
    {
        _dm_fill_dev_info(pt_dev_desc->ps_dev_name, pt_inf);
    }

    x_rwl_release_lock(t_dm_core.h_dev_rwlock);

    return DMR_OK;
}

INT32 dm_get_devices(
    DM_DEV_FILTR_T *pt_filtr, 
    DM_DEV_STAT_T *pt_stat,
    UINT32 *pui4_dev_ns)
{
    DM_DEV_DESC_T *pt_dev_desc = NULL;

    UINT32 ui4_stat_ns;
 
    UINT32 ui4_cnt;
    UINT32 ui4_ns = 0;

    if ((pt_filtr == NULL) || (pt_stat == NULL) || (pui4_dev_ns == NULL))
    {
        return DMR_INVALID_PARM;
    }

    ui4_stat_ns = *pui4_dev_ns;

    if (x_rwl_read_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    SLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
    {
        for (ui4_cnt = 0; ui4_cnt < pt_filtr->ui4_dev_filtr_ns; ui4_cnt++)
        {
            DM_COND_T *pt_cond = &(pt_filtr->pt_dev_filtr[ui4_cnt]);

            if (_dm_dev_approve(pt_cond, 
                                pt_dev_desc->t_hw_type, 
                                pt_dev_desc->t_dev_type) == TRUE)
            {
                pt_stat[ui4_ns].t_hw_type = pt_dev_desc->t_hw_type;
                pt_stat[ui4_ns].t_dev_type = pt_dev_desc->t_dev_type;
                pt_stat[ui4_ns].b_avail = pt_dev_desc->b_avail;
                pt_stat[ui4_ns].ui4_unit_id = pt_dev_desc->ui4_unit_id;
                pt_stat[ui4_ns].ui4_skt_no = pt_dev_desc->ui4_skt_no;
                pt_stat[ui4_ns].h_dev = pt_dev_desc->h_dev;
    
                ui4_ns++;

                break;
            }
        }

        if (ui4_ns == ui4_stat_ns)
        {
            break;
        }
    }

    *pui4_dev_ns = ui4_ns;

    x_rwl_release_lock(t_dm_core.h_dev_rwlock);

    return DMR_OK;
}

INT32 dm_get_device(
    DM_DEV_SPEC_T *pt_spec,
    HANDLE_T *ph_dev)
{
    DM_DEV_DESC_T *pt_dev_desc = NULL;
    BOOL b_found = FALSE;

    if ((pt_spec == NULL) || (ph_dev == NULL))
    {
        return DMR_INVALID_PARM;
    }

    if ((pt_spec->t_hw_type == DM_HW_TYPE_UNKNOWN) || 
        (pt_spec->t_dev_type == DM_DEV_UKW_TYPE_UNKNOWN) ||
        (pt_spec->t_dev_type == DM_DEV_SKT_TYPE_UNKNOWN) ||
        (pt_spec->t_dev_type == DM_DEV_MED_TYPE_UNKNOWN))
    {
        return DMR_INVALID_PARM;
    }

    if (x_rwl_read_lock(t_dm_core.h_dev_rwlock, RWL_OPTION_WAIT) != RWLR_OK)
    {
        return DMR_OS;
    }

    SLIST_FOR_EACH(pt_dev_desc, &(t_dm_core.t_dev_list), t_link)
    {
        if ((pt_dev_desc->t_hw_type == pt_spec->t_hw_type) &&
            (pt_dev_desc->t_dev_type == pt_spec->t_dev_type) &&
            (pt_dev_desc->ui4_unit_id == pt_spec->ui4_unit_id))
        {
            b_found = TRUE;
            *ph_dev = pt_dev_desc->h_dev;
            break;
        }
    }

    x_rwl_release_lock(t_dm_core.h_dev_rwlock);

    if (b_found == FALSE)
    {
        return DMR_ERR;
    }

    return DMR_OK;
}


INT32 dm_get_usb_port_number(UINT8 *pui1_usb_port_num)
{
    INT32 i4_ret;
    FILE *fp_usbports;
    UINT8 ui1_port_num_char;

    if (pui1_usb_port_num == NULL)
    {
        return DMR_INVALID_PARM;
    }

    if (t_dm_core.ui1_usb_port_num != 0)
    {
        *pui1_usb_port_num = t_dm_core.ui1_usb_port_num;
        return DMR_OK;
    }

    i4_ret = dm_mount_usbfs_once();
    if (i4_ret != DMR_OK)
    {
        return i4_ret;
    }

    system("cat /proc/bus/usb/devices |grep -c Lev=00 > /tmp/usbports");
    
    fp_usbports = fopen("/tmp/usbports", "r");
    if (fp_usbports == NULL)
    {
        return DMR_ERR;
    }
    
    fscanf(fp_usbports, "%c", &ui1_port_num_char);
    *pui1_usb_port_num = ui1_port_num_char - '0';
    

    fclose(fp_usbports);

    t_dm_core.ui1_usb_port_num = *pui1_usb_port_num;

    return DMR_OK;
}


INT32 dm_get_changed_usb_port(
        const CHAR* ps_phy_path,
        UINT8 *pui1_changed_usb_port)
{
    const CHAR *ps_current_pos = ps_phy_path;

    if (ps_phy_path == NULL)
    {
        x_dbg_stmt("[DM]Physical path is empty.\n");
        return DMR_INVALID_PHY_PATH;
    }

    while (*ps_current_pos != '\0')
    {
        if (strncmp(ps_current_pos, "/usb", 4) == 0 &&
            *(ps_current_pos + 5) == '/')
        {
            /* First usb port is Port 0. */
            *pui1_changed_usb_port = *(ps_current_pos + 4) - '1';
            x_dbg_stmt("[DM] Status of usb port %x changed.\n", *pui1_changed_usb_port);
            return DMR_OK;
        }
        ps_current_pos++;
    }

    return DMR_INVALID_PHY_PATH; 
}


INT32 dm_umount_remove(CHAR* ps_dev_name)
{
    if(strncmp("sd", ps_dev_name, 2) == 0)
    {
        dm_core_file_disable();
    }
   
    dm_umount(ps_dev_name);      
    INT32 i4_ret = dm_remove_dev(ps_dev_name);
    return i4_ret;
}

UINT16 dm_get_dbg_level(VOID)
{
    return t_dm_core.ui2_dbg_lvl;
}

BOOL   dm_set_dbg_level(
    UINT16 ui2_level)
{
    t_dm_core.ui2_dbg_lvl = ui2_level;
    return TRUE;
}


static INT32 _dm_init_fm(VOID)
{
    INT32 i4_ret;

    i4_ret = x_fm_create_dir(FM_ROOT_HANDLE, "/dev", 0777);

    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/dev is created.");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            x_dbg_stmt("/dev is already existed.  That is OK.\n");
        }
        else
        {
            x_dbg_stmt("make /dev fail.\n");
            i4_ret = DMR_FM_ERR;
            goto fail;
        }
    }

    i4_ret = x_fm_create_dir(NULL_HANDLE, "/mnt", 777);
    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/mnt is created.");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            x_dbg_stmt("/mnt is already existed.  That is OK.\n");
        }
        else
        {
            x_dbg_stmt("create /mnt fail.\n");
            i4_ret = DMR_FM_ERR;
            goto fail;
        }
    }

    i4_ret = x_fm_create_dir(NULL_HANDLE, "/mnt/usb", 777);
    if (i4_ret == FMR_OK)
    {
        x_dbg_stmt("/mnt/usb is created.");
    }
    else if (i4_ret != FMR_OK)
    {
        if (i4_ret == FMR_EXIST)
        {
            x_dbg_stmt("/mnt/usb is already existed.  That is OK.\n");
        }
        else
        {
            x_dbg_stmt("create /mnt/usb fail.\n");
            i4_ret = DMR_FM_ERR;
            goto fail;
        }
    }
    return DMR_OK;

fail:

    return i4_ret;
}


#ifdef DLNA_SUPPORT

static VOID _dm_dlna_dev_nfy_fct(
    VOID              *pv_nfy_tag,
    SKTDEV_STATUS_T   e_nfy_st,
    VOID              *pv_data)
{
    char ps_buf[MAX_DLNA_PAYLOAD];
    char *ps_ptr = ps_buf;
    SKTDEV_DEV_INFO_T *pt_sktdev_info = (SKTDEV_DEV_INFO_T *) pv_data;
    DM_CB_MSG_T t_cb_msg;
    INT32 i4_ret;

    DM_ASSERT(pt_sktdev_info != NULL);    
    DM_ASSERT(e_nfy_st == pt_sktdev_info->eDevSt);

    t_cb_msg.pv_tag = pv_nfy_tag;
    t_cb_msg.t_cb_type = (e_nfy_st == SKTDEV_STATUS_DEV_ATTACH) ? DM_CB_DEV_ATTACH : DM_CB_DEV_DETACH;
    t_cb_msg.ui4_skt_no = pt_sktdev_info->u4SocketNo;
    t_cb_msg.t_dev_type = DM_DEV_MED_TYPE_DLNA_DMS;
    t_cb_msg.ui4_unit_id = pt_sktdev_info->u_info.rSupDev.u4CompId;
    t_cb_msg.ui4_dev_flag = pt_sktdev_info->u_info.rSupDev.u4DevFlag;
    t_cb_msg.pv_data = NULL;
    t_cb_msg.ui4_data_sz = 0;

    /*printf("[FakeDM]_dm_dlna_dev_nfy_fct: general event:0x%x device type:0x%x socket #:%d\n", 
                          t_cb_msg.t_cb_type,
                          t_cb_msg.t_dev_type,
                          t_cb_msg.ui4_skt_no);*/

    if (t_cb_msg.t_cb_type == DM_CB_DEV_ATTACH)
    {
        x_strcpy(ps_ptr, "add@/dlna/dlnadms_");
        x_sprintf(ps_buf, "%s%d", ps_ptr, t_cb_msg.ui4_unit_id);
    }
    else
    {
        x_strcpy(ps_ptr, "remove@/dlna/dlnadms_");
        x_sprintf(ps_buf, "%s%d", ps_ptr, t_cb_msg.ui4_unit_id);
    }

    /* send msg */
    i4_ret = x_msg_q_send(t_dm_core.h_cb_msg_q, 
                          (const VOID *) &ps_buf, 
                          sizeof(ps_buf),
                          DM_CB_MSG_PRIORITY);

    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("[FakeDM]%s: send msg fail(%d).\n",__FUNCTION__,i4_ret);
    }

    x_dbg_stmt("[FakeDM]:%s, ps_buf:%s is sent.\n",__FUNCTION__, ps_buf);
}

static void _dm_init_dlna (VOID)
{
    INT32 i4_ret;
    
    /*printf("[FakeDM] Before x_dm_dlna_init()\n");*/
    x_dm_dlna_init(); 
    /*printf("[FakeDM] After x_dm_dlna_init()\n");

    printf("[FakeDM] Before x_dm_dlna_register_notify()\n");*/
    i4_ret = x_dm_dlna_register_notify(_dm_dlna_dev_nfy_fct, NULL);
    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("[FakeDM]:%s,%d x_dm_dlna_register_notify fail(%d)\n",__FUNCTION__,__LINE__, i4_ret);
    }
    /*printf("[FakeDM] After x_dm_dlna_register_notify()\n");

    printf("[FakeDM] Before x_dm_dlna_dmp_start()\n");*/
    /* DTV00213986, x_dm_dlna_dmp_start() should be called by AP */
    /*x_dm_dlna_dmp_start();*/
    /*printf("[FakeDM] After x_dm_dlna_dmp_start()\n");*/
}
#endif

#ifdef SMB_SUPPORT

static VOID _dm_smb_dev_nfy_fct(
    VOID              *pv_nfy_tag,
    SKTDEV_STATUS_T   e_nfy_st,
    VOID              *pv_data)
{
    char ps_buf[MAX_SMB_PAYLOAD] = "";
    SKTDEV_DEV_INFO_T *pt_sktdev_info = (SKTDEV_DEV_INFO_T *) pv_data;
    DM_CB_MSG_T t_cb_msg;
    INT32 i4_ret;

    DM_ASSERT(pt_sktdev_info != NULL);    
    DM_ASSERT(e_nfy_st == pt_sktdev_info->eDevSt);

    t_cb_msg.pv_tag = pv_nfy_tag;
    t_cb_msg.t_cb_type = (e_nfy_st == SKTDEV_STATUS_DEV_ATTACH) ? DM_CB_DEV_ATTACH : DM_CB_DEV_DETACH;
    t_cb_msg.ui4_skt_no = pt_sktdev_info->u4SocketNo;
    t_cb_msg.t_dev_type = DM_DEV_MED_TYPE_SMB;
    t_cb_msg.ui4_unit_id = pt_sktdev_info->u_info.rSupDev.u4CompId;
    t_cb_msg.ui4_dev_flag = pt_sktdev_info->u_info.rSupDev.u4DevFlag;
    t_cb_msg.pv_data = NULL;
    t_cb_msg.ui4_data_sz = 0;

    if (t_cb_msg.t_cb_type == DM_CB_DEV_ATTACH)
    {
        x_sprintf(ps_buf, "add@/smb/smb_%d", t_cb_msg.ui4_unit_id);
    }
    else
    {
        x_sprintf(ps_buf, "remove@/smb/smb_%d", t_cb_msg.ui4_unit_id);
    }

    /* send msg */
    i4_ret = x_msg_q_send(t_dm_core.h_cb_msg_q, 
                          (const VOID *) &ps_buf, 
                          sizeof(ps_buf),
                          DM_CB_MSG_PRIORITY);

    if (i4_ret != OSR_OK)
    {
        x_dbg_stmt("[FakeDM]%s: send msg fail(%d).\n",__FUNCTION__,i4_ret);
    }

    x_dbg_stmt("[FakeDM]:%s, ps_buf:%s is sent.\n",__FUNCTION__, ps_buf);
}

extern VOID x_dm_smb_init(VOID);
extern INT32 x_dm_smb_register_notify(x_sktdev_st_nfy_fct pf_sktdev_st_nfy, VOID*  pv_tag);
static void _dm_init_smb (VOID)
{
    INT32 i4_ret = 0;
    
    x_dm_smb_init(); 

    i4_ret = x_dm_smb_register_notify(_dm_smb_dev_nfy_fct, NULL);
    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("[FakeDM]:%s,%d x_dm_smb_register_notify fail(%d)\n",__FUNCTION__,__LINE__, i4_ret);
    }
}
#endif


INT32 dm_init(
    DM_INIT_PARM_T *pt_init)
{
    INT32 i4_ret;
    if (t_dm_core.b_init == TRUE)
    {
        i4_ret = DMR_ALREADY_INIT;
        goto fail;
    }
    x_dbg_stmt("[ FakeDM ] Init...\n");
    t_dm_core.b_automnt_init = FALSE;
    t_dm_core.ui1_usb_port_num = 0;

    i4_ret = dm_cli_init();
    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("_dm_init init dm cli error.\n");
        goto fail;
    }

    /*t_dm_core.ui2_dbg_lvl = DBG_INIT_LEVEL;*/
    x_dbg_stmt("[FakeDM] Init FM...\n");

    i4_ret = _dm_init_fm();

    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("_dm_init init fm error.\n");
        goto fail;
    }
    x_dbg_stmt("[FakeDM] FM inited...\n");

    /*For fill fake stucture*/
    i4_ret = x_fm_open_dir(
        NULL_HANDLE,
        "/dev",
        &h_dev_root);
    
    if(i4_ret != FMR_OK)
    {
        x_dbg_stmt("open /dev fail.\n");
        goto fail;
    }

    i4_ret = x_rwl_create_lock(&t_dm_core.h_nfy_rwlock);

    if (i4_ret != RWLR_OK)
    {
        i4_ret = DMR_OS;
        goto fail;
    }

    i4_ret = x_rwl_create_lock(&t_dm_core.h_dev_rwlock);

    if (i4_ret != RWLR_OK)
    {
        i4_ret = DMR_OS;
        goto fail;
    }   

    SLIST_INIT(&(t_dm_core.t_nfy_list));
    SLIST_INIT(&(t_dm_core.t_dev_list));

    i4_ret = x_msg_q_create(&t_dm_core.h_cb_msg_q, 
                            DM_CB_MSG_Q_NAME,
                            MAX_UEVENT_PAYLOAD,
                            DM_CB_MSG_Q_NUM);

    if (i4_ret != OSR_OK)
    {
        i4_ret = DMR_OS;
        goto fail;
    }

    x_dbg_stmt("%s: line:%d, disable the MW-DM thread for auto mount.\n", __FUNCTION__, __LINE__);
#if 0
/* remove the middleware auto mount for Vestel*/
   i4_ret = x_thread_create(&t_dm_core.h_cb_msg_thrd,
                              DM_CB_MSG_THREAD_NAME, 
                              DM_CB_MSG_THREAD_DEFAULT_STACK_SIZE, 
                              DM_CB_MSG_THREAD_DEFAULT_PRIORITY,
                              _dm_cb_msg_thread,
                              0, 
                              NULL);

    if (i4_ret != OSR_OK)
    {
        i4_ret = DMR_OS;
        goto fail;
    }

    i4_ret = x_thread_create(&t_dm_core.h_nfy_req_thrd,
                             DM_NFY_REQ_THREAD_NAME,
                             DM_NFY_REQ_THREAD_DEFAULT_STACK_SIZE,
                             DM_NFY_REQ_THREAD_DEFAULT_PRIORITY,
                             _dm_nfy_req_thread,
                             0,
                             NULL);

    if (i4_ret != OSR_OK)
    {
        i4_ret = DMR_OS;
        goto fail;
    }
    
    i4_ret = x_thread_create(&t_dm_core.h_open_dev_thrd,
                             "dm_open_dev_thrd",
                             4096,
                             128,
                             _dm_open_dev_thread,
                             0,
                             NULL);
#endif 
    if (i4_ret != OSR_OK)
    {
        i4_ret = DMR_OS;
        goto fail;
    }

    if (pt_init != NULL)
    {
        #if 0
        t_dm_core.ui4_automnt_cond_ns = pt_init->ui4_automnt_cond_ns;
        t_dm_core.pt_automnt_cond = (DM_COND_T *)x_mem_alloc(pt_init->ui4_automnt_cond_ns * 
                                                             sizeof(DM_COND_T));

        if (t_dm_core.pt_automnt_cond == NULL)
        {
            i4_ret = DMR_OS;
            goto fail;
        }

        x_memcpy((VOID *) t_dm_core.pt_automnt_cond, 
                 (const VOID *) pt_init->pt_automnt_cond, 
                 pt_init->ui4_automnt_cond_ns * sizeof(DM_COND_T));
        #else
        t_dm_core.ui4_automnt_cond_ns = 0;
        t_dm_core.pt_automnt_cond = NULL;
        #endif
    }
#ifdef DLNA_SUPPORT
    x_dbg_stmt("[FakeDM] Init DLNA...\n");
    _dm_init_dlna();
    x_dbg_stmt("[FakeDM] DLNA inited...\n");
#endif
#ifdef SMB_SUPPORT
    x_dbg_stmt("[FakeDM] Init SMB...\n");
    _dm_init_smb();
    x_dbg_stmt("[FakeDM] SMB inited...\n");
#endif
    t_dm_core.b_init = TRUE;

    return DMR_OK;

fail:


    if (t_dm_core.h_nfy_rwlock != NULL_HANDLE)
    {
        x_rwl_delete_lock(t_dm_core.h_nfy_rwlock);
    }

    if (t_dm_core.pt_automnt_cond != NULL)
    {
        x_mem_free((VOID *) t_dm_core.pt_automnt_cond);
    }

    return i4_ret;
}

int dm_usb_has_unknown_interface(void)
{
    INT32 i4_ret;
    FILE *fp_mass_storage_num = NULL;
    int  unknown_interface_num = 0;

    i4_ret = dm_mount_usbfs_once();
    if (i4_ret != DMR_OK)
    {
        return 0;
    }

    /* I:* Match the active interface,
     * -v Cls=0[89] Filter out Mass storage and hub. 
     */
    system("cat /proc/bus/usb/devices |grep ^'I:\\*' | grep -v Cls=0[89] -c > /tmp/unknown_interface");
    
    fp_mass_storage_num = fopen("/tmp/unknown_interface", "r");
    if (fp_mass_storage_num == NULL)
    {
        return 0;
    }
    
    fscanf(fp_mass_storage_num, "%d", &unknown_interface_num);

    fclose(fp_mass_storage_num);

    return (unknown_interface_num);
}

int dm_usb_notify_unknown_inteface_change(char *kernel_message)
{
    static int total_unknown_dev = 0;
    int unknown_dev = 0;
    int mtp_dev = 0;
    char ps_buf[32];
    
    dm_mount_usbfs_once();
    if(strncmp("add@/class/usb_device", kernel_message, strlen("add@/class/usb_device")) == 0 )
    {
        unknown_dev = dm_usb_has_unknown_interface();
        if (unknown_dev > 0)
        {
            /* check MTP */
            _dm_check_mtp_change(kernel_message);
            mtp_dev = _dm_get_total_mtp_dev_num();
            unknown_dev -= mtp_dev;
        }
        
        for ( ;total_unknown_dev < unknown_dev; total_unknown_dev++)
        {
            x_sprintf(ps_buf, "unknown_device_%d", total_unknown_dev);
            dm_add_dev(
                    NULL,
                    ps_buf,
                    ps_buf,
                    NULL,
                    DM_HW_TYPE_UNKNOWN,
                    DM_DEV_UKW_TYPE_UNKNOWN);
            ps_buf[0] = '\0';
        }
    }

    if(strncmp("remove@/class/usb_device", kernel_message, strlen("remove@/class/usb_device")) == 0) 
    {
        unknown_dev = dm_usb_has_unknown_interface();
        /* check MTP */
        _dm_check_mtp_change(kernel_message);
        mtp_dev = _dm_get_total_mtp_dev_num();
        unknown_dev -= mtp_dev;

        for ( ;total_unknown_dev > unknown_dev; total_unknown_dev--) 
        {
            x_sprintf(ps_buf, "unknown_device_%d", total_unknown_dev-1);
            dm_remove_dev(ps_buf);
            ps_buf[0] = '\0';
        }
    }

    return 0;
}

