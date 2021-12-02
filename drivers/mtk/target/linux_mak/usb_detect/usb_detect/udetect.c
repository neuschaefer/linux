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
/* usb_detect.c Main file for usb status detect and notify. */

#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h> 
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/netlink.h>

#include "mylist.h"
#include "udetect.h"
#include "os/inc/x_os.h"

handle_t h_dev_list = NULL;

pthread_mutex_t t_dev_netlink_mutex;

static bool gb_quit = false ;
static bool g_is_inited = false;

typedef struct CALL_BACK_NOTIFY_LIST_T
{
    unsigned int index;
    Usb_Callback_Fun pf_notify;
    struct CALL_BACK_NOTIFY_LIST_T *pt_next;
} CALL_BACK_NOTIFY_LIST_T;

static struct CALL_BACK_NOTIFY_LIST_T *p_root_callback_list = NULL;
static unsigned int callback_index = 0;
static unsigned int callback_num = 0;

static handle_t h_dev_watch_list = NULL;
struct _DEV_PRE {
    char dev[64];
    int try;
    int port;
};

static void* _usb_mon_pre (void *pvtag);
static void* _usb_mon (void *pvtag);
int _get_usb_label_uuid (const char* dev, char* label, char* uuid);

#define PRINTF(...) do { printf ("[udetect] %s.%d: ", __FUNCTION__, __LINE__); printf(__VA_ARGS__); } while (0)

void udetect_notify(
        CALLBACK_EVENT_T event,
        const char *dev,
        const char *path,
        const char *label,
        int port)
{
    
    MNT_DEV_T mount_dev_info;
    x_strcpy(mount_dev_info.dev, dev);
    x_strcpy(mount_dev_info.dev_mnt_path, path);
    x_strcpy(mount_dev_info.label, label);
    mount_dev_info.port = port;

    PRINTF("start.\n");    
    struct CALL_BACK_NOTIFY_LIST_T * p_callback_notify = p_root_callback_list;
    
    while(p_callback_notify != NULL)
    { 
        if(p_callback_notify->pf_notify != NULL)
        {
             p_callback_notify->pf_notify(event, &mount_dev_info);
        }
        p_callback_notify = p_callback_notify->pt_next;
    }
    PRINTF("End.\n");    
}

int udetect_register_callback(Usb_Callback_Fun callback_fun)
{
    pthread_mutex_lock(&t_dev_netlink_mutex);
        
    struct CALL_BACK_NOTIFY_LIST_T *p_notify_tmp = p_root_callback_list;
    
    while(p_notify_tmp != NULL)//check if such callback function has alread registered.
    {
        if(p_notify_tmp->pf_notify == callback_fun)
        {
            printf("############  such callback function has alread registered!\n");
            pthread_mutex_unlock(&t_dev_netlink_mutex);
            return -1;
        }
        p_notify_tmp = p_notify_tmp->pt_next;
    }
    
    p_notify_tmp = p_root_callback_list;
    callback_index++;

    struct CALL_BACK_NOTIFY_LIST_T * p_notify_add =  (struct CALL_BACK_NOTIFY_LIST_T * )malloc(sizeof(struct CALL_BACK_NOTIFY_LIST_T));
    if (p_notify_add == NULL)
    {
        printf("############  net_register_callback malloc memory failed!\n");
        pthread_mutex_unlock(&t_dev_netlink_mutex);
        return -1;
    }
    p_notify_add->index = callback_index;
    p_notify_add->pf_notify = callback_fun;
    p_notify_add->pt_next = NULL;   

    if(p_notify_tmp == NULL)
    {

        printf("############## %s ,%d \n",__FUNCTION__,__LINE__);
        p_root_callback_list = p_notify_add;
    }
    else
    {
        while (p_notify_tmp != NULL)
        {
            if(p_notify_tmp->pt_next == NULL)
            {
                printf("############## %s ,%d \n",__FUNCTION__,__LINE__);
                p_notify_tmp->pt_next = p_notify_add;
                break ;
            }
            p_notify_tmp = p_notify_tmp->pt_next;
        }
        
    }

    pthread_mutex_unlock(&t_dev_netlink_mutex);
    callback_num++;
    printf("%s: current callback num is : %d\n",__FUNCTION__,callback_num);
    return 0;

}

int  udetect_unregister_callback(Usb_Callback_Fun callback_fun)
{
    struct CALL_BACK_NOTIFY_LIST_T * p_notify_index = p_root_callback_list;
    struct CALL_BACK_NOTIFY_LIST_T * p_notify_tmp = p_root_callback_list;
    
    pthread_mutex_lock(&t_dev_netlink_mutex);
    
    p_notify_tmp = p_root_callback_list;
    if(p_notify_index == NULL)
    {
        printf("#############  There are no registered callback function!\n");
        pthread_mutex_unlock(&t_dev_netlink_mutex);
        return -1;
    }
    while(p_notify_index != NULL)
    {
         if(p_notify_index->pf_notify == callback_fun)
         {
             if(p_notify_index == p_root_callback_list)//first node.
             {
                 printf("############## %s ,%d \n",__FUNCTION__,__LINE__);
                 p_root_callback_list = p_notify_index->pt_next;
             }
             else
             {
                 p_notify_tmp->pt_next = p_notify_index->pt_next;
             }
             free(p_notify_index);
             p_notify_index = NULL;

             pthread_mutex_unlock(&t_dev_netlink_mutex);
             callback_num--;
             printf(">>>>>>>>>>>>>>>> %s: current callback num is : %d\n",__FUNCTION__,callback_num);
             return 0;
         }
         p_notify_tmp = p_notify_index;
         p_notify_index = p_notify_index->pt_next;
    }
    printf("#############  %s: no such callback function!\n",__FUNCTION__);
    pthread_mutex_unlock(&t_dev_netlink_mutex);
    return -1;

}

bool udetect_init(void)
{
    int ret;

    pthread_t t_thread;
    pthread_t t_thread_pre;

    pthread_attr_t t_attr;

    pthread_attr_init (&t_attr);
    pthread_attr_setdetachstate (&t_attr, PTHREAD_CREATE_DETACHED);

    if(!g_is_inited)
    {         
        gb_quit = false;
        pthread_mutex_init(&t_dev_netlink_mutex ,NULL);

        ret = pthread_create (&t_thread, &t_attr, _usb_mon, NULL);
        if (ret != 0)
        {
            printf("[udetect]create _usb_mon failed,ret = %d\n", ret);
            return false;  
        }

        ret = pthread_create (&t_thread_pre, &t_attr, _usb_mon_pre, NULL);
        if (ret !=0)
        {
            printf("[udetect]create _usb_mon_pre failed,ret = %d\n", ret);
            return false;  
        }

        PRINTF("return.\n");      
        g_is_inited = true;
    }
    return true;
}

bool udetect_exit(void)
{
     if(g_is_inited == true)
     {     
         printf("%s\n", __func__);
         gb_quit = true;
         g_is_inited = false;
     }
     return true;
}

int _get_usb_label_uuid (const char* dev, char* label, char* uuid)
{
    FILE *f = NULL;
    int rc = 0;
    int rc2 = -1;
    char cmd[256] = "";
    char buf[513] = "";

    if (dev == NULL || (label == NULL && uuid == NULL))
    {
        return -1;
    }

    if (label) x_strcpy (label, "");
    if (uuid) x_strcpy (uuid, "");

    x_sprintf (cmd, "busybox blkid | grep '^/dev/%s: '", dev);
    /* /dev/sda1: LABEL="HAIFEI" UUID="F036-81EC" */
    /* may hove no LABEL or UUID :) */
    f = popen (cmd, "r");
    if (f == NULL)
    {
        return -1;
    }

    rc = fread (buf, 1, 512, f);
    if (rc > 0)
    {
        char* pl = NULL;
        char* pu = NULL;
        buf[512] = '\0'; 
        if (uuid)
        {
            pu = strstr (buf, "\" UUID=\"");
            if (pu != NULL && pu[8] != '\0')
            {
                pu += 8;
                pu [strlen(pu) - 1] = '\0';
                x_strcpy (uuid, pu);
                rc2 = 0;
            }
        }
        if (label)
        {
            pl = strstr (buf, ": LABEL=\"");
            if (pl != NULL && pl[9] != '\0')
            {
                char* pt = NULL;
                pl += 9;
                pt = strstr (pl, "\" UUID=\"");
                if (pt == NULL) /* have no UUID */
                {
                    pt = strstr (pl, "\"");
                }
                /*if (pt != NULL && pt[8] != '\0')*/
                if (pt)
                {
                    *pt = '\0';
                    x_strcpy (label, pl);
                    rc2 = 0;
                }
            }
        }
    }

    rc = pclose (f);
    if (rc != 0)
    {
        rc = (char)((rc >> 8) & 0xFF);
        if (rc != 0)
        {
            return -1;
        }
    }

    return rc2;
}


void _free_proc_devs (char **devs)
{
    if (devs == NULL) return;

    if (devs[0] != NULL)
    {
        free (devs[0]);
        devs[0] = NULL;
    }

    free (devs);
}

char** _get_proc_devs (void)
{
    FILE *f = NULL;
    char *buf = NULL;
    char **devs = {NULL};
    int rc = 0;

    buf = malloc (513);
    if (buf == NULL)
    {
        return NULL;
    }
    devs = malloc (20 * sizeof(char*));
    if (devs == NULL)
    {
        free (buf);
        free (devs);
        return NULL;
    }

    f = popen ("grep ' sd[a-z][0-9]* ' /proc/diskstats | awk '{print $3}'", "r");
    if (f == NULL)
    {
        free (devs);
        free (buf);
        return NULL;
    }

    rc = fread (buf, 1, 512, f);
    if (rc > 0)
    {
        char *pb = buf;
        int end = 0;
        int idx = 1;
        devs[0] = buf;
        while (*pb != 0)
        {
            if (end)
            {
                end = 0;
                devs[idx++] = pb;
                if (idx == 20)
                {
                    break;
                }
            }
            if (*pb == '\r')
            {
                *pb = 0;
            }
            if (*pb == '\n')
            {
                *pb = 0;
                end = 1;
            }
            pb++;
        }
    }
    else
    {
        free (devs);
        free (buf);
        pclose (f);
        return NULL;
    }
    pclose (f);

    return devs;
}

int _is_mount2 (const char* dev)
{
    FILE *f = NULL;
    char buf[512] = "";
    int rc = 0;

    x_sprintf (buf, "df | grep '/dev/%s .* /tmp/mnt/usb/%s'", dev, dev);

    f = popen (buf, "r");
    if (f == NULL) return 0;

    rc = fread (buf, 1, 16, f);
    pclose (f);

    if (rc > 3) return 1;
    return 0;
}

int _is_mount (char c)
{
    FILE *f = NULL;
    char buf[512] = "";
    int rc = 0;

    x_sprintf (buf, "df | grep '/dev/sd[a-z][0-9]* .*/tmp/usb/%c:'", c);

    f = popen (buf, "r");
    if (f == NULL) return 0;

    rc = fread (buf, 1, 16, f);
    pclose (f);

    if (rc > 3) return 1;
    return 0;
}

int _get_cur_dev (void)
{
    char i = 'c';

    for (i = 'c'; i <= 'z'; i++)
    {
        if (!_is_mount (i)) return i;
    }
    return 0;
}


int _get_cur_usb_ports (const char* dev, int *port)
{
    FILE* f = NULL;
    char cmd[512] = "";
    char ndev[4] = "";
    int rc = 0;

    if (port == NULL) return -1;
    if (dev == NULL || strlen (dev) < 3) return -1;

    x_strncpy (ndev, dev, 3);
    ndev[3] = '\0';

    x_sprintf (cmd, "grep '^PHYSDEVPATH=/devices/platform/MtkUsbHcd' /sys/block/%s/device/block:%s/uevent", ndev, ndev);

    f = popen (cmd, "r");

    if (f == NULL) return -1;

    memset (cmd, 0, sizeof(cmd));
    rc = fread (cmd, 1, sizeof(cmd), f);
    cmd[511] = '\0';
    pclose (f);

    if (rc < 50) return -1;

    *port = cmd[40] - '0' + 1;

    PRINTF (">>> USB PORTS: %s, PORT: %d\n", cmd, *port);
    return 0;
}

int _get_usb_mnt_by_idx (int idx, char* dev, char* path)
{
    struct _MNT_DEV** ppdev = NULL;
    struct _MNT_DEV** ppd = NULL;
    int i = 0;

    if (idx < 0) return -1;
    if (dev == NULL && path == NULL) return -1;

    ppdev = (struct _MNT_DEV**) my_list_toarray (h_dev_list, NULL);
    if (ppdev == NULL) return -1;

    for (i = 0, ppd = ppdev; ppd != NULL; i++, ppd++)
    {
        /*PRINTF ("+++++++ %d (0x%08x): %s, %d, %s\n", i, (int) *ppd, (*ppd)->dev, (*ppd)->mp, (*ppd)->dev_mnt_path);*/
        if (i == idx) break;
    }

    if ((i == idx) && (ppd != NULL) && ((*ppd) != NULL))
    {
        if (dev) x_strcpy (dev, (*ppd)->dev);
        if (path)
        {
            if ((*ppd)->mp != 0)
            {
                x_sprintf (path, "/tmp/usb/%c:/", (*ppd)->mp);
            }
            else
            {
                x_strcpy (path, (*ppd)->dev_mnt_path);
            }
        }
        free (ppdev);
        return 0;
    }

    free (ppdev);

    return -1;
}

static int _search_device_port_cmp (const void* pv1, const void* pv2)
{
    struct _MNT_DEV *pdev1 = (struct _MNT_DEV*) pv1;
    struct _MNT_DEV *pdev2 = (struct _MNT_DEV*) pv2;

    if (pdev1 == NULL || pdev2 == NULL) return -1;

    if (pdev1->port != 0 && strncmp (pdev1->dev, pdev2->dev, 3) == 0)
    {
        return 0;
    }

    return -1;
}

static int _add_mnt_dev (const char* dev, const char* dev_mnt_path, int mp, int port)
{
    struct _MNT_DEV *pdev = malloc (sizeof(*pdev));
    int rc;

    if (pdev == NULL) return -1;
    memset (pdev, 0, sizeof(*pdev));

    if (h_dev_list == NULL) my_list_create (&h_dev_list);
    if (h_dev_list == NULL)
    {
        free (pdev);
        return -1;
    }

    PRINTF ("%s, %s, %c (%d), %d\n", dev, dev_mnt_path, mp, mp, port);
    x_strcpy (pdev->dev, dev);
    if (dev_mnt_path) x_strcpy (pdev->dev_mnt_path, dev_mnt_path);
    else pdev->mp = mp;

    if (port == 0)
    {
        struct _MNT_DEV *pdev_cmp = my_list_search2 (h_dev_list, pdev, _search_device_port_cmp);
        if (pdev_cmp != NULL)
        {
            port = pdev_cmp->port;
        }
    }

    pdev->port = port;

    if (port < 0) port = 0 - port;

    /* label */
    {
        if (_get_usb_label_uuid (pdev->dev, pdev->label, NULL) != 0)
        {
            x_strcpy (pdev->label, "");
        }
    }

    rc = my_list_add_tail (h_dev_list, pdev);
    if (dev_mnt_path)
    {
        PRINTF ("(%s, %s) ret: %d, size: %d\n", dev, dev_mnt_path, rc, my_list_size(h_dev_list));
        udetect_notify (USB_DEVICE_MOUNTED, dev, dev_mnt_path, pdev->label, port);
    }
    else
    {
        char path[256] = "";
        x_sprintf (path, "/tmp/usb/%c:/", mp);
        PRINTF ("(%s, %c, %d) ret: %d, size: %d\n", dev, mp, mp, rc, my_list_size(h_dev_list));
        udetect_notify (USB_DEVICE_MOUNTED, dev, path, pdev->label, port);
    }

    return rc;
}

static int _cmp_mnt_dev (const void* pv1, const void* pv2)
{
    struct _MNT_DEV *pdev = (struct _MNT_DEV*) pv1;
    const char* dev = (const char*) pv2;

    if (pdev == NULL || dev == NULL) return -1;

    if (pdev->dev == dev || strcmp (pdev->dev, dev) == 0) return 0;

    return -1;
}

static int _del_mnt_dev (const char* dev, char *dev_mnt_path, char *pmp, char* label, int* port)
{
    struct _MNT_DEV *pdev = my_list_delete2 (h_dev_list, dev, _cmp_mnt_dev);

    if (pdev == NULL) return -1;

    if (port && *port <= 0) *port = pdev->port;
    if (pdev->label && label) x_strcpy (label, pdev->label);

    if (dev_mnt_path && pdev->dev_mnt_path[0] != 0)
    {
        x_strcpy (dev_mnt_path, pdev->dev_mnt_path);
    }
    else
    {
        if (pmp) *pmp = pdev->mp;
    }

    free (pdev);

    return 0;
}

static int _stop_dev_apply (void* pv_data, void* pv_tag)
{
    struct _MNT_DEV *pdev = (struct _MNT_DEV*) pv_data;
    char mntp [256] = "";
    char* pm = mntp;

    if (pdev == NULL) return 0;

    if (pdev->mp)
    {
        x_sprintf (mntp, "/tmp/usb/%c:/", pdev->mp);
    }
    else
    {
        pm = pdev->dev_mnt_path;
    }
    PRINTF ("stop all tasks on dev %s (%s), port %d\n", pdev->dev, pm, pdev->port);

    return 0;
}

int _stop_dev_all (void)
{
    my_list_map (h_dev_list, _stop_dev_apply, NULL);
    return 0;
}

static int _delete_dev_apply (void* pv_data, void* pv_tag)
{
    struct _MNT_DEV *pdev = (struct _MNT_DEV*) pv_data;
    char mntp [256] = "";
    char* pm = mntp;

    if (pdev == NULL) return 0;

    if (pdev->mp)
    {
        x_sprintf (mntp, "/tmp/usb/%c:/", pdev->mp);
    }
    else
    {
        pm = pdev->dev_mnt_path;
    }
    PRINTF ("delete all tasks on dev %s (%s), port %d\n", pdev->dev, pm, pdev->port);

    return 0;
}

int _delete_dev_all (void)
{
    my_list_map (h_dev_list, _delete_dev_apply, NULL);
    return 0;
}

static int _restart_dev_apply (void* pv_data, void* pv_tag)
{
    struct _MNT_DEV *pdev = (struct _MNT_DEV*) pv_data;
    char mntp [256] = "";
    char* pm = mntp;

    if (pdev == NULL) return 0;

    if (pdev->mp)
    {
        x_sprintf (mntp, "/tmp/usb/%c:/", pdev->mp);
    }
    else
    {
        pm = pdev->dev_mnt_path;
    }
    PRINTF ("restart all tasks on dev %s (%s), port %d\n", pdev->dev, pm, pdev->port);

    return 0;
}

int _restart_dev_all (void)
{
    my_list_map (h_dev_list, _restart_dev_apply, NULL);
    return 0;
}

static int _search_device_pre_port_cmp (const void* pv1, const void* pv2)
{
    struct _DEV_PRE *pdev1 = (struct _DEV_PRE*) pv1;
    struct _DEV_PRE *pdev2 = (struct _DEV_PRE*) pv2;

    if (pdev1 == NULL || pdev2 == NULL) return -1;

    if (pdev1->port != 0 && strncmp (pdev1->dev, pdev2->dev, 3) == 0)
    {
        return 0;
    }

    return -1;
}

static int _add_device_pre (const char* dev, int port)
{
    struct _DEV_PRE *pdev = malloc (sizeof(*pdev));
    int rc;

    if (pdev == NULL) return -1;

    if (h_dev_watch_list == NULL) 
        my_list_create (&h_dev_watch_list);

    if (h_dev_watch_list == NULL)
    {
        free (pdev);
        return -1;
    }

    x_strcpy (pdev->dev, dev);
    pdev->try = 0;

    if (port == 0)
    {
        struct _DEV_PRE *pdev_cmp = my_list_search2 (h_dev_watch_list, pdev, _search_device_pre_port_cmp);
        if (pdev_cmp != NULL)
        {
            port = pdev_cmp->port;
        }
    }

    pdev->port = port;

    rc = my_list_add_tail (h_dev_watch_list, pdev);
    PRINTF ("(%s, %d) ret: %d, size: %d\n", dev, port, rc, my_list_size(h_dev_watch_list));

    return rc;
}

int _add_device (const char* dev, int port)
{
    char cmd[1024] = "";

    PRINTF ("dev: %s, port: %d\n", dev, port);

    x_sprintf (cmd, "/mnt/usb/%s/", dev);

    _add_mnt_dev (dev, cmd, 0, port);

    return 0;
}

static int _cmp_dev_pre (const void* pv1, const void* pv2)
{
    struct _DEV_PRE *pdev = (struct _DEV_PRE*) pv1;
    const char* dev = (const char*) pv2;

    if (pdev == NULL || dev == NULL) return -1;

    if (pdev->dev == dev || strcmp (pdev->dev, dev) == 0) return 0;

    return -1;
}

int _del_device_pre (const char* dev)
{
    struct _DEV_PRE *pdev = NULL;

    while ((pdev = my_list_delete2(h_dev_watch_list, dev, _cmp_dev_pre)) != NULL)
        free (pdev);

    return 0;
}

int _del_device (const char* dev, int port)
{
    char cmd[1024] = "";
    char dev_mnt_path[256] = "";
    char label[256] = "";
    char mp = 0;

    /* delete pre list */
    _del_device_pre (dev);

    if (_del_mnt_dev (dev, dev_mnt_path, &mp, label, &port) != 0)
        return -1;

    /* now, udetect deal with both mounting & un-mounting */
    /*if (_is_running_iptv12 ()) return 0;*/

    PRINTF ("del device %s, mount -> %c\n", dev, mp);

    if (mp)
    {
        x_sprintf (dev_mnt_path, "/tmp/usb/%c:/", mp);

        x_sprintf (cmd, "umount -l %s", dev_mnt_path);
        system (cmd);
        PRINTF ("cmd: %s\n", cmd);
        rmdir (dev_mnt_path);
        PRINTF ("rmdir: %s\n", dev_mnt_path);
    }

    udetect_notify (USB_DEVICE_UNMOUNTED, dev, dev_mnt_path, label, port);

    return 0;
}

static int _cmp_pre_timeout (const void* pv1, const void* pv2)
{
    struct _DEV_PRE *pdev = (struct _DEV_PRE*) pv1;

    if (pdev == NULL) return -1;

    if (pdev->try > 500) return 0;

    return -1;
}

static int _del_dev_pre_timeout (void)
{
    struct _DEV_PRE *pdev = NULL;
    while ((pdev = my_list_delete2 (h_dev_watch_list, NULL, _cmp_pre_timeout)) != NULL)
    {
        PRINTF (">>>>> %s is remove from pre-list, try: %d\n", pdev->dev, pdev->try);
        free (pdev);
    }
    return 0;
}

static int _usb_pre_apply (void* pv_data, void* pv_tag)
{
    struct _DEV_PRE *pdev = (struct _DEV_PRE*) pv_data;

    if (pdev == NULL) return 0;

    ++pdev->try;

    if (_is_mount2 (pdev->dev))
    {
        PRINTF (">>>>> %s is mounted: /mnt/usb/%s, port %d\n", pdev->dev, pdev->dev, pdev->port);
        pdev->try = 99999999;
        _add_device (pdev->dev, pdev->port);
    }

    return 0;
}

static void* _usb_mon_pre (void *pvtag)
{
    /*h_dev_watch_list*/
    while (!gb_quit) {
        my_list_map (h_dev_watch_list, _usb_pre_apply, NULL);
        _del_dev_pre_timeout ();
        usleep (500000);
    }
    return NULL;
}

static void* _usb_mon (void *pvtag)
{
    struct sockaddr_nl servaddr;
    int fd = -1;
    char buf[1024] = "";
    /*int bsize = 1024;*/
    int reuse = 1;

    /*PRINTF (">>> start\n");*/

    {
        char **pps_devs = _get_proc_devs ();
        if (pps_devs != NULL)
        {
            char *dev = NULL;
            int i;
            for (i = 0; ; i++)
            {
                int port = 0;

                if ((dev = pps_devs[i]) == NULL) 
                    break;

                if (strncmp("sd", dev, strlen("sd")) != 0)
                {
                    PRINTF("Skip %s.\n", dev);
                    break;
                }
                _get_cur_usb_ports (dev, &port);
                _add_device_pre (dev, port);
            }
        }
        _free_proc_devs (pps_devs);
    }

    fd = socket (PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (fd < 0)
    {
        perror ("[udetect] socket");
        return NULL;
    }

    memset (&servaddr, 0, sizeof(servaddr));
    servaddr.nl_family = AF_NETLINK;
    servaddr.nl_pid = getpid();
    servaddr.nl_groups = 1;

    setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    if (bind (fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) != 0)
    {
        perror ("[udetect] bind2");
        close (fd);
        return NULL;
    }

    while (!gb_quit) {
        int add = 0, del = 0;
        char *ptmp = NULL;

        memset (buf, 0, sizeof(buf));

        recv (fd, buf, sizeof(buf), 0);
        
        if (buf[5] != 'b' && buf[8] != 'b')
            continue;

        PRINTF (">>>>> >>>>> %s <<<<<\n", buf);

        if (strstr (buf, "add@/block/sd") != NULL)
        {
            add = 1;
        }
        else if (strstr (buf, "remove@/block/sd") != NULL)
        {
            del = 1;
        }
        else 
            continue;

        ptmp = buf + strlen(buf);
        while (*(--ptmp) != '/')
            ;

        if (strlen(ptmp+1) < 3)
        {
            continue;
        }

        PRINTF (" == usb notify: %s %s\n", add ? "add" : "del", ptmp+1);

        if (add)
        {
            int port = 0;
            _get_cur_usb_ports (ptmp+1, &port);
            _add_device_pre (ptmp+1, port);
        }
        else if (del)
        {
            _del_device (ptmp+1, 0);
        }
    }

    close (fd);

    return NULL;
}

