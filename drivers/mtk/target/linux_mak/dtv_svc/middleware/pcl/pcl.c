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
 * $RCSfile: pcl.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         This is Power Control Library API source file.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "dbg/def_dbg_level_mw.h"
#include "dt/x_dt.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "res_mngr/drv/x_pwr_ctrl.h"
#include "res_mngr/drv/x_rtc.h"
#include "util/x_lnk_list.h"
#include "io_mngr/x_iom.h"
#include "pcl/pcl.h"
#include "pcl/x_pcl.h"
#include "pcl/pcl_cli.h"
#include "pcl/pcl_lock.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define PCL_GPIO_MONITOR_START      ((UINT32)   0)
#define PCL_GPIO_MONITOR_STOP       ((UINT32)  55)
#define PCL_OPCTRL_MONITOR_START    ((UINT32)   0)
#define PCL_OPCTRL_MONITOR_STOP     ((UINT32)   7)
#define PCL_OPCTRL_OFFSET           ((UINT32) 200)

/* Definition of handle type */
#define PCL_HANDLE_TYPE_NFY_REC         (HT_GROUP_PCL + ((HANDLE_TYPE_T) 0))
#define PCL_HANDLE_TYPE_WAKE_UP_EVT     (HT_GROUP_PCL + ((HANDLE_TYPE_T) 1))

/* Notify record */
typedef struct _PCL_NFY_REC_T
{
    DLIST_ENTRY_T(_PCL_NFY_REC_T)   t_link;
    HANDLE_T                        h_nfy;
    VOID*                           pv_tag;
    x_pcl_nfy_fct                   pf_nfy;
}   PCL_NFY_REC_T;

/* Wake-up event */
typedef struct _PCL_WAKE_UP_EVT_T
{
    DLIST_ENTRY_T(_PCL_WAKE_UP_EVT_T)   t_link;
    HANDLE_T                            h_wakeup_evt;
    PCL_WAKE_UP_CTRL_T                  t_wakeup_ctrl;
}   PCL_WAKE_UP_EVT_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static BOOL                             b_pcl_init = FALSE;
static HANDLE_T                         h_pcd = NULL_HANDLE;
static HANDLE_T                         h_power_down_timer = NULL_HANDLE;
static HANDLE_T                         h_wakeup_evt_pcl = NULL_HANDLE;
/*static UINT16                           ui2_pcl_dbg_level;*/
static UINT32                           ui4_pcl_wakeup_cap = 0;
static UINT32                           ui4_pcl_wakeup_ctrl_cap = 0;
static TIME_T                           t_power_down_time = NULL_TIME;
static PCL_GPIO_MONITOR_CTRL_T          t_gpio_monitor_ctrl;
/* static PCL_GPIO_MONITOR_CTRL_EX_T       t_gpio_monitor_ctrl_ex; */
static UINT64                           ui8_pcl_cur_gpio_data;
static UINT16                           aui2_res_cnt[PCL_RES_TYPE_MAX];
static UINT32                           ui4_pcl_log_level;
static DLIST_T(_PCL_NFY_REC_T)          t_pcl_nfy_lst;
static DLIST_T(_PCL_WAKE_UP_EVT_T)      t_pcl_wakeup_lst;

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
static VOID _pcl_rm_nfy_fct(HANDLE_T  h_obj,
                            UINT8     ui1_port,
                            RM_COND_T e_nfy_cond,
                            VOID*     pv_tag,
                            UINT32    ui4_data);

static BOOL _pcl_handle_free_nfy_fct(HANDLE_T      h_handle,
                                     HANDLE_TYPE_T e_dev_type,
                                     VOID*         pv_obj,
                                     VOID*         pv_tag,
                                     BOOL          b_req_handle);

static VOID _pcl_pc_nfy_fct(VOID*     pv_nfy_tag,
                            PC_COND_T e_nfy_cond,
                            UINT32    ui4_data);

static VOID _pcl_power_down_timer_cb(HANDLE_T h_timer,
                                     VOID*    pv_tag);

static INT32 _pcl_get_wakeup_cap(UINT32* pui4_wakeup_cap,
                                 UINT32* pui4_wakeup_ctrl_cap);

static INT32 _pcl_update_gpio_data(VOID);

/*-----------------------------------------------------------------------------
 * Name: _pcl_rm_nfy_fct
 *
 * Description: This notify function is called by the Resource Manager.
 *
 * Inputs:
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _pcl_rm_nfy_fct(HANDLE_T  h_obj,
                            UINT8     ui1_port,
                            RM_COND_T e_nfy_cond,
                            VOID*     pv_tag,
                            UINT32    ui4_data)
{
    /* To be implemented */
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_handle_free_nfy_fct
 *
 * Description: This is handle free notify function.
 *
 * Inputs:  h_handle        A handle referencing the registration.
 *          e_dev_type      Contains the handle type.
 *          pv_obj          References the data object.
 *          pv_tag          Contains the handle private tag.
 *          b_req_handle    If this argument is set to TRUE then the free
 *                          operation is specifically requested on this handle.
 *                          If this argument is set to FALSE, the handle is
 *                          being freed but this operation occurred due some
 *                          other effect.
 *
 * Outputs: -
 *
 * Returns: TRUE            Successful.
 *          FALSE           Failed.
 ----------------------------------------------------------------------------*/
static BOOL _pcl_handle_free_nfy_fct(HANDLE_T      h_handle,
                                     HANDLE_TYPE_T e_dev_type,
                                     VOID*         pv_obj,
                                     VOID*         pv_tag,
                                     BOOL          b_req_handle)
{
    INT32       i4_return;

    switch (e_dev_type)
    {
        case PCL_HANDLE_TYPE_NFY_REC:
        case PCL_HANDLE_TYPE_WAKE_UP_EVT:
            x_mem_free(pv_obj);
            i4_return = TRUE;
            break;

        default:
            i4_return = FALSE;
            break;
    }

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_pc_nfy_fct
 *
 * Description: This notify function is called by the Power Control driver in
 *              response to condition changes.
 *
 * Inputs:  pv_nfy_tag          Private tag value.
 *          e_nfy_cond          The notify condition.
 *          ui4_data            The additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _pcl_pc_nfy_fct(VOID*     pv_nfy_tag,
                            PC_COND_T e_nfy_cond,
                            UINT32    ui4_data)
{
    PCL_NFY_REC_T*  pt_nfy_rec = NULL;
    #if 0
    UINT32          ui4_gpio_id;
    UINT32          ui4_gpio_status;
    UINT64          ui8_gpio_ctrl;
    #endif

    switch (e_nfy_cond)
    {
        case PC_COND_WAKE_UP_EXPIRED:
            DLIST_FOR_EACH(pt_nfy_rec, &t_pcl_nfy_lst, t_link)
            {
                pt_nfy_rec->pf_nfy(pt_nfy_rec->h_nfy,
                                   pt_nfy_rec->pv_tag,
                                   PCL_COND_WAKE_UP_EXPIRED,
                                   0);
            }
            break;

        case PC_COND_GPIO_UPDATED:
            #if 0
            ui4_gpio_status = (ui4_data >> 16);
            ui4_gpio_id = (ui4_data & 0xffff);

            if ((ui4_gpio_id <= PCL_GPIO_MONITOR_STOP) ||
                ((ui4_gpio_id >= (PCL_OPCTRL_MONITOR_START + PCL_OPCTRL_OFFSET)) && (ui4_gpio_id <= (PCL_OPCTRL_MONITOR_STOP + PCL_OPCTRL_OFFSET))))
            {
                /* Update GPIO/OPCTRL data */
                if (ui4_gpio_id < PCL_OPCTRL_OFFSET) /* GPIO */
                {
                    ui8_gpio_ctrl = ((UINT64)1 << ui4_gpio_id);
                }
                else /* OPCTRL */
                {
                    ui8_gpio_ctrl = ((UINT64)1 << (ui4_gpio_id - PCL_OPCTRL_OFFSET + PCL_GPIO_MONITOR_STOP + 1));
                }

                /* Check if the monitoring GPIO condition matched */
                if ((ui8_gpio_ctrl & t_gpio_monitor_ctrl.ui8_ctrl) == ui8_gpio_ctrl)
                {
                    if (ui4_gpio_status)
                    {
                        ui8_pcl_cur_gpio_data |= ui8_gpio_ctrl;
                    }
                    else
                    {
                        ui8_pcl_cur_gpio_data &= !ui8_gpio_ctrl;
                    }

                    if (t_gpio_monitor_ctrl.b_monitor_both_chg)
                    {
                        DLIST_FOR_EACH(pt_nfy_rec, &t_pcl_nfy_lst, t_link)
                        {
                            pt_nfy_rec->pf_nfy(pt_nfy_rec->h_nfy,
                                               pt_nfy_rec->pv_tag,
                                               PCL_COND_MONITORING_GPIO_CHANGED,
                                               ui4_gpio_status);
                        }
                    }
                    else
                    {
                        if ((ui8_pcl_cur_gpio_data & t_gpio_monitor_ctrl.ui8_data) == ui8_pcl_cur_gpio_data)
                        {
                            DLIST_FOR_EACH(pt_nfy_rec, &t_pcl_nfy_lst, t_link)
                            {
                                pt_nfy_rec->pf_nfy(pt_nfy_rec->h_nfy,
                                                   pt_nfy_rec->pv_tag,
                                                   PCL_COND_MONITORING_GPIO_MATCHED,
                                                   ui4_gpio_status);
                            }
                        }
                    }
                }
            }
            #else
            DLIST_FOR_EACH(pt_nfy_rec, &t_pcl_nfy_lst, t_link)
            {
                pt_nfy_rec->pf_nfy(pt_nfy_rec->h_nfy,
                                   pt_nfy_rec->pv_tag,
                                   PCL_COND_MONITORING_GPIO_CHANGED,
                                   ui4_data);
            }
            #endif
            break;

        default:
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_power_down_timer_cb
 *
 * Description: This is the callback function of the PCL power-down timer.
 *
 * Inputs:  h_timer         Contains the timer handle.
 *          pv_tag          References a private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _pcl_power_down_timer_cb(HANDLE_T h_timer,
                                     VOID*    pv_tag)
{
    DBG_INFO(("{PCL} _pcl_power_down_timer_cb \n\r"));
#ifndef LINUX_TURNKEY_SOLUTION_PHASE2
    x_iom_send_evt(BTN_POWER, 0);
#else
    PCL_NFY_REC_T*  pt_nfy_rec = NULL;
    DLIST_FOR_EACH(pt_nfy_rec, &t_pcl_nfy_lst, t_link)
    {
        pt_nfy_rec->pf_nfy(pt_nfy_rec->h_nfy,
                           pt_nfy_rec->pv_tag,
                           PCL_COND_POWER_DOWN_TIMEOUT,
                           0);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _pcl_get_wakeup_cap
 *
 * Description: This API returns the wake-up capability info that the system
 *              can support and clients can control.
 *
 * Inputs:  -
 *
 * Outputs: pui4_wakeup_cap         Contains the bit-mask of all wake-up
 *                                  capabilities that the system can support.
 *          pui4_wakeup_ctrl_cap    Contains the bit-mask of the wake-up
 *                                  capabilities that clients can control.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_get_wakeup_cap(UINT32* pui4_wakeup_cap,
                                 UINT32* pui4_wakeup_ctrl_cap)
{
    INT32       i4_return;
    SIZE_T      z_info_len;

    if (h_pcd == NULL_HANDLE)
    {
        *pui4_wakeup_cap = 0;
        *pui4_wakeup_ctrl_cap = 0;
    }
    else
    {
        /* Get the wake-up capabilities that the hardware supports */
        z_info_len = sizeof(UINT32);

        i4_return = rm_get(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_GET_TYPE_WAKE_UP_CAP,
                           ((VOID*)pui4_wakeup_cap),
                           &z_info_len,
                           NULL);

        if (i4_return != RMR_OK)
        {
            return PCLR_DEV_GET_FAILED;
        }

        /* Get the wake-up capabilities that a client can control */
        z_info_len = sizeof(UINT32);

        i4_return = rm_get(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_GET_TYPE_WAKE_UP_CTRL_CAP,
                           ((VOID*)pui4_wakeup_ctrl_cap),
                           &z_info_len,
                           NULL);

        if (i4_return != RMR_OK)
        {
            return PCLR_DEV_GET_FAILED;
        }
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _pcl_update_gpio_data
 *
 * Description: This API gathers current GPIO and OPCTRL pin data.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
static INT32 _pcl_update_gpio_data(VOID)
{
    UINT32              ui4_idx;
    PCL_GPIO_STATUS_T   e_gpio_status;

    pcl_lock_wr_db();

    /* Get GPIO 0~55 data */
    for (ui4_idx = PCL_GPIO_MONITOR_START; ui4_idx <= PCL_GPIO_MONITOR_STOP; ui4_idx++)
    {
        x_pcl_get_gpio(ui4_idx, PCL_GPIO_MODE_INPUT, &e_gpio_status);

        if (e_gpio_status == PCL_GPIO_STATUS_LOW)
        {
            ui8_pcl_cur_gpio_data &= !(1 << ui4_idx);
        }
        else if (e_gpio_status == PCL_GPIO_STATUS_HIGH)
        {
            ui8_pcl_cur_gpio_data |= (1 << ui4_idx);
        }
        else
        {
#if 0 /* Remove it whenever PCD can support this feature */
            pcl_unlock_db();
            return PCLR_ERROR;
#endif
        }
    }

    /* Get OPCTRL 0~7 data */
    for (ui4_idx = PCL_OPCTRL_MONITOR_START; ui4_idx <= PCL_OPCTRL_MONITOR_STOP; ui4_idx++)
    {
        x_pcl_get_gpio((ui4_idx + PCL_OPCTRL_OFFSET), PCL_GPIO_MODE_INPUT, &e_gpio_status);

        if (e_gpio_status == PCL_GPIO_STATUS_LOW)
        {
            ui8_pcl_cur_gpio_data &= !(1 << (ui4_idx + PCL_GPIO_MONITOR_STOP + 1));
        }
        else if (e_gpio_status == PCL_GPIO_STATUS_HIGH)
        {
            ui8_pcl_cur_gpio_data |= (1 << (ui4_idx + PCL_GPIO_MONITOR_STOP + 1));
        }
        else
        {
#if 0 /* Remove it whenever PCD can support this feature */
            pcl_unlock_db();
            return PCLR_ERROR;
#endif
        }
    }

    pcl_unlock_db();
    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: pcl_get_wakeup_event_hdl_pcl
 *
 * Description: This API gets the PCL (i.e. internal) wake-up event handle.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: NULL_HANDLE     A wake-up event by index is not existing.
 *          Others          The handle of a wake-up event.
 ----------------------------------------------------------------------------*/
HANDLE_T pcl_get_wakeup_event_hdl_pcl(VOID)
{
    return (h_wakeup_evt_pcl);
}


/*-----------------------------------------------------------------------------
 * Name: pcl_get_wakeup_evt_hdl_by_idx
 *
 * Description: This API gets an wake-up event handle by index.
 *
 * Inputs:  ui2_idx     Index for a wake-up event.
 *
 * Outputs: -
 *
 * Returns: NULL_HANDLE     A wake-up event by index is not existing.
 *          Others          The handle of a wake-up event.
 ----------------------------------------------------------------------------*/
HANDLE_T pcl_get_wakeup_evt_hdl_by_idx(UINT16 ui2_idx)
{
    PCL_WAKE_UP_EVT_T*  pt_evt;
    UINT16              ui2_cnt = 0;

    pcl_lock_rd_db();

    if (DLIST_IS_EMPTY(&t_pcl_wakeup_lst))
    {
        pcl_unlock_db();
        return (NULL_HANDLE);
    }

    DLIST_FOR_EACH(pt_evt, &t_pcl_wakeup_lst, t_link)
    {
        if (ui2_idx == ui2_cnt)
        {
            pcl_unlock_db();
            return (pt_evt->h_wakeup_evt);
        }
        else
        {
            ui2_cnt++;
        }
    }

    pcl_unlock_db();

    return (NULL_HANDLE);
}


/*-----------------------------------------------------------------------------
 * Name: pcl_show_wakeup_list
 *
 * Description: This API shows the current status of wake-up list.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID pcl_show_wakeup_list(VOID)
{
    PCL_WAKE_UP_EVT_T*  pt_evt;
    UINT16              ui2_idx = 0;

    pcl_lock_rd_db();

    if (DLIST_IS_EMPTY(&t_pcl_wakeup_lst))
    {
        x_dbg_stmt("Wake-up list is empty\n\r");
    }

    DLIST_FOR_EACH(pt_evt, &t_pcl_wakeup_lst, t_link)
    {
        if (pt_evt->t_wakeup_ctrl.ui4_misc_ctrl == PCL_MISC_CTRL_IGNORE)
        {
            x_dbg_stmt("[%d] [0x%08x] wake-up_time=0x%08x%08x, misc_ctrl=ignore ",
                       ui2_idx++,
                       pt_evt->h_wakeup_evt,
                       (UINT32)(pt_evt->t_wakeup_ctrl.t_time >> 32),
                       (UINT32)pt_evt->t_wakeup_ctrl.t_time);
        }
        else
        {
            x_dbg_stmt("[%d] [0x%08x] wake-up_time=0x%08x%08x, misc_ctrl=0x%08x ",
                       ui2_idx++,
                       pt_evt->h_wakeup_evt,
                       (UINT32)(pt_evt->t_wakeup_ctrl.t_time >> 32),
                       (UINT32)pt_evt->t_wakeup_ctrl.t_time,
                       pt_evt->t_wakeup_ctrl.ui4_misc_ctrl);
        }

        if (pt_evt->h_wakeup_evt == h_wakeup_evt_pcl)
        {
            x_dbg_stmt("*\n\r"); /* Mark as PCL wake-up event */
        }
        else
        {
            x_dbg_stmt("\n\r");
        }
    }

    x_dbg_stmt("\n\r");

    pcl_unlock_db();
}


/*-----------------------------------------------------------------------------
 * Name: pcl_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 pcl_get_dbg_level(VOID)
{
    if (b_pcl_init)
    {
        /* return ui2_pcl_dbg_level; */
        return (UINT16)ui4_pcl_log_level;
    }
    else
    {
        return 0;
    }
}


/*-----------------------------------------------------------------------------
 * Name: pcl_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
BOOL pcl_set_dbg_level(UINT16 ui2_level)
{
    if (b_pcl_init)
    {
        /* ui2_pcl_dbg_level = ui2_level; */
        ui4_pcl_log_level = ui4_pcl_log_level & 0xff00;
        ui4_pcl_log_level = ui4_pcl_log_level | ui2_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*-----------------------------------------------------------------------------
 * Name: pcl_get_log_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
 UINT32 pcl_get_log_level(VOID)
{
    if(b_pcl_init)
    {
        return ui4_pcl_log_level;
    }
    else
    {
        return 0;
    }
}
 /*-----------------------------------------------------------------------------
 * Name: pcl_set_log_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/
BOOL    pcl_set_log_level(UINT32    ui4_level)
 {
    if(b_pcl_init)
    {
        ui4_pcl_log_level = ui4_level;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
 }

/*-----------------------------------------------------------------------------
 * Name: pcl_init
 *
 * Description: This API initializes the Power Control Library.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_ALREADY_INIT       The Power Control Library has already
 *                                  been initialized.
 *          PCLR_DEV_OPEN_FAILED    Device open operation is failed.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 *          PCLR_TIMER_ERROR        A timer error occurred.
 *          PCLR_ERROR              A Power Control Library error occurred.
 *          PCLR_CLI_FAILED         CLI init is failed.
 ----------------------------------------------------------------------------*/
INT32 pcl_init(VOID)
{
    INT32                   i4_return;
    PCL_WAKE_UP_CTRL_T      t_wakeup_ctrl;
    PC_NFY_INFO_T           t_pc_nfy_info;
    RM_COND_T               e_cond;
    UINT16                  ui1_idx;

    /* Check arguments */
    if (b_pcl_init)
    {
        return PCLR_ALREADY_INIT;
    }

    /* Initialize PCL lock module */
    pcl_lock_init();

    /* Initialize Power Control Library debug level */
    /* ui2_pcl_dbg_level = DBG_INIT_LEVEL_MW_PCL; */
    ui4_pcl_log_level = DBG_INIT_LEVEL_MW_PCL;

    /* Open Power Control driver */
    i4_return = rm_open_comp(NULL_HANDLE,
                             OBJ_COMP_REL_IGNORE,
                             DRVT_PWR_CTRL,
                             ANY_COMP_ID,
                             NULL,
                             CTRL_TYPE_SINGLE,
                             NULL,
                             NULL,
                             NULL,
                             &_pcl_rm_nfy_fct,
                             &h_pcd,
                             &e_cond);

    if (i4_return != RMR_OK)
    {
        return PCLR_DEV_OPEN_FAILED;
    }

    /* Set notify function to Power Control driver */
    t_pc_nfy_info.pv_tag = NULL;
    t_pc_nfy_info.pf_nfy = &_pcl_pc_nfy_fct;

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_NFY_FCT,
                       (VOID*)&t_pc_nfy_info,
                       sizeof(PC_NFY_INFO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        return PCLR_DEV_SET_FAILED;
    }

    /* Create power-down timer */
    i4_return = x_timer_create(&h_power_down_timer);

    if (i4_return != OSR_OK)
    {
        return PCLR_TIMER_ERROR;
    }

    /* Initialize internal variables */
    t_gpio_monitor_ctrl.ui8_ctrl = 0;
    t_gpio_monitor_ctrl.ui8_data = 0;
    
    ui8_pcl_cur_gpio_data = 0;
    for (ui1_idx = 0; ui1_idx < PCL_RES_TYPE_MAX; ui1_idx++)
    {
        aui2_res_cnt[ui1_idx] = 0;
    }
    DLIST_INIT(&t_pcl_nfy_lst);
    DLIST_INIT(&t_pcl_wakeup_lst);

    /* Update all GPIO status */
    i4_return = _pcl_update_gpio_data();

    if (i4_return != PCLR_OK)
    {
        return PCLR_ERROR;
    }

    /* Initialize Power Control Library CLI component */
    i4_return = pcl_cli_init();

    if (i4_return != PCLR_OK)
    {
        return PCLR_CLI_FAILED;
    }

    b_pcl_init = TRUE;

    /* Create PCL wake-up event */
    t_wakeup_ctrl.t_time = NULL_TIME;
    t_wakeup_ctrl.ui4_misc_ctrl = PCL_MISC_CTRL_IGNORE;

    i4_return = x_pcl_add_wakeup_event(&t_wakeup_ctrl,
                                       &h_wakeup_evt_pcl);

    if (i4_return != PCLR_OK)
    {
        return (i4_return);
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_wakeup_capability
 *
 * Description: This API returns the wake-up capability info that the system
 *              can support and clients can control.
 *
 * Inputs:  -
 *
 * Outputs: pui4_wakeup_cap         References the bit-mask of all wake-up
 *                                  capabilities that the system can support.
 *          pui4_wakeup_ctrl_cap    References the bit-mask of the wake-up
 *                                  capabilities that clients can control.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_wakeup_capability(UINT32* pui4_wakeup_cap,
                                  UINT32* pui4_wakeup_ctrl_cap)
{
    DBG_API(("{PCL} x_pcl_get_wakeup_capability\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    /* Get the wake-up capabilities and wake-up control capabilities */
    return (_pcl_get_wakeup_cap(pui4_wakeup_cap,
                                pui4_wakeup_ctrl_cap));
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_wakeup_setup
 *
 * Description: This API gets the current wake-up setup.
 *
 * Inputs:  -
 *
 * Outputs: pui4_wakeup_setup       References the wake-up setup in bit-mask
 *                                  fashion.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_wakeup_setup(UINT32* pui4_wakeup_setup)
{
    INT32       i4_return;
    SIZE_T      z_info_len;

    DBG_API(("{PCL} x_pcl_get_wakeup_setup\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the current wake-up setup */
    z_info_len = sizeof(UINT32);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_WAKE_UP_SETUP,
                       ((VOID*)pui4_wakeup_setup),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_WAKE_UP_SETUP) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_wakeup_setup
 *
 * Description: This API sets the wake-up setup.
 *              Note that PCL_WAKE_UP_REASON_IRRC and PCL_WAKE_UP_REASON_FP
 *              are always set and cannot be disabled by clients.
 *
 * Inputs:  ui4_wakeup_setup        Contains the wake-up setup in bit-mask
 *                                  fashion.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_wakeup_setup(UINT32 ui4_wakeup_setup)
{
    INT32       i4_return;
    UINT32      ui4_setup;

    DBG_API(("{PCL} x_pcl_set_wakeup_setup: 0x%x\n\r", ui4_wakeup_setup));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the wake-up capabilities */
    pcl_lock_wr_db();

    i4_return = _pcl_get_wakeup_cap(&ui4_pcl_wakeup_cap,
                                    &ui4_pcl_wakeup_ctrl_cap);

    if (i4_return != PCLR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: _pcl_get_wakeup_cap failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    ui4_setup = ui4_wakeup_setup;
    ui4_setup |= (~ui4_pcl_wakeup_ctrl_cap & ui4_pcl_wakeup_cap);

    /* Set the wake-up setup */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_WAKE_UP_SETUP,
                       (VOID*)ui4_setup,
                       sizeof(UINT32),
                       NULL);

    if (i4_return != RMR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_WAKE_UP_SETUP) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_wakeup_time
 *
 * Description: This API gets the wake-up time configured by
 *              x_pcl_set_wakeup_time().
 *
 * Inputs:  -
 *
 * Outputs: pt_time         References the wake-up time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If the wake-up time was never set, NULL_TIME will
 *                          be returned.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_ERROR              A Power Control Library error occurred.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_wakeup_time(TIME_T* pt_time)
{
    INT32                   i4_return;
    PCL_WAKE_UP_CTRL_T      t_wakeup_ctrl;

    DBG_API(("{PCL} x_pcl_get_wakeup_time\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (pt_time == NULL)
    {
        return PCLR_INV_ARG;
    }

    i4_return = x_pcl_get_wakeup_event(h_wakeup_evt_pcl, &t_wakeup_ctrl);

    if (i4_return == PCLR_OK)
    {
        *pt_time = t_wakeup_ctrl.t_time;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_wakeup_time
 *
 * Description: This API sets a wake-up time.
 *
 * Inputs:  t_time          Contains the wake-up time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If a client would like to cancel the wake-up time,
 *                          it simply calls this API with ui4_time set to
 *                          NULL_TIME.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_MEM_ALLOC_FAILED   Memory allocation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_wakeup_time(TIME_T t_time)
{
    INT32                   i4_return;
    PCL_WAKE_UP_CTRL_T      t_wakeup_ctrl;

    DBG_API(("{PCL} x_pcl_set_wakeup_time: 0x%08x%08x\n\r",
            (UINT32)(t_time >> 32),
            (UINT32)t_time));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    t_wakeup_ctrl.t_time = t_time;

    i4_return = x_pcl_get_misc_ctrl(&t_wakeup_ctrl.ui4_misc_ctrl);

    if (i4_return != PCLR_OK)
    {
        return (i4_return);
    }

    return (x_pcl_set_wakeup_event(h_wakeup_evt_pcl,
                                   &t_wakeup_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_power_down_time
 *
 * Description: This API gets the current power-down time.
 *
 * Inputs:  -
 *
 * Outputs: pt_time         References the power-down time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If the wake-up time was never set, NULL_TIME will
 *                          be returned.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_power_down_time(TIME_T* pt_time)
{
    DBG_API(("{PCL} x_pcl_get_power_down_time\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    pcl_lock_rd_db();

    *pt_time = t_power_down_time;

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_power_down_time
 *
 * Description: This API sets the power-down time. If the time is already
 *              passed, this API will reject with PCLR_INV_ARG.
 *
 * Inputs:  t_time          Contains the power-down time (UTC time count as
 *                          number of seconds since 00:00:00Z January 1, 1970).
 *                          If a client would like to cancel the power-down
 *                          time, it simply calls this API with ui4_time set
 *                          to NULL_TIME.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_TIMER_ERROR        A timer error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_power_down_time(TIME_T t_time)
{
    INT32       i4_return;
    TIME_T      t_cur_time = NULL_TIME;

    DBG_API(("{PCL} x_pcl_set_power_down_time: 0x%08x%08x\n\r",
            (UINT32)(t_time >> 32),
            (UINT32)t_time));
    DBG_INFO(("{PCL} x_pcl_set_power_down_time: 0x%08x%08x\n\r",
            (UINT32)(t_time >> 32),
            (UINT32)t_time));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    /* Check if the new time is valid */
    pcl_lock_wr_db();

    if (t_time != NULL_TIME)
    {
        t_cur_time = x_dt_get_utc(NULL, NULL);
        if (t_time <= t_cur_time)
        {
            pcl_unlock_db();
            return PCLR_INV_ARG;
        }
    }

    i4_return = x_timer_stop(h_power_down_timer);

    if (i4_return != OSR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: x_timer_stop() failed\n\r"));
        return PCLR_TIMER_ERROR;
    }

    if ((t_power_down_time = t_time) != NULL_TIME)
    {
        i4_return = x_timer_start(h_power_down_timer,
                                  (UINT32)((t_power_down_time - t_cur_time) * 1000),
                                  X_TIMER_FLAG_ONCE,
                                  _pcl_power_down_timer_cb,
                                  NULL);

        if (i4_return != OSR_OK)
        {
            pcl_unlock_db();
            DBG_ERROR(("{PCL} ERR: x_timer_start() failed\n\r"));
            return PCLR_TIMER_ERROR;
        }
    }

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_misc_ctrl
 *
 * Description: This API gets the status of miscellaneous controls.
 *
 * Inputs:  -
 *
 * Outputs: pui4_ctrl       References the bit-mask of miscellaneous controls.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_misc_ctrl(UINT32* pui4_ctrl)
{
    INT32                   i4_return;
    PCL_WAKE_UP_CTRL_T      t_wakeup_ctrl;

    DBG_API(("{PCL} x_pcl_get_misc_ctrl\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (pui4_ctrl == NULL)
    {
        return PCLR_INV_ARG;
    }

    i4_return = x_pcl_get_wakeup_event(h_wakeup_evt_pcl, &t_wakeup_ctrl);

    if (i4_return == PCLR_OK)
    {
        *pui4_ctrl = t_wakeup_ctrl.ui4_misc_ctrl;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_misc_ctrl
 *
 * Description: This API sets the miscellaneous controls.
 *
 * Inputs:  ui4_ctrl        Contains the bit-mask of miscellaneous controls.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_misc_ctrl(UINT32 ui4_ctrl)
{
    INT32                   i4_return;
    PCL_WAKE_UP_CTRL_T      t_wakeup_ctrl;

    DBG_API(("{PCL} x_pcl_set_misc_ctrl: 0x%x\n\r", ui4_ctrl));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    t_wakeup_ctrl.ui4_misc_ctrl = ui4_ctrl;

    i4_return = x_pcl_get_wakeup_time(&t_wakeup_ctrl.t_time);

    if (i4_return != PCLR_OK)
    {
        return (i4_return);
    }

    return (x_pcl_set_wakeup_event(h_wakeup_evt_pcl,
                                   &t_wakeup_ctrl));
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_wakeup_reason
 *
 * Description: This API gets the reason of current wake-up.
 *
 * Inputs:  -
 *
 * Outputs: pe_wakeup_reason        References the wake-up reason.
 *          pv_data                 References the additional information of
 *                                  wakeup reason.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_wakeup_reason(PCL_WAKE_UP_REASON_T* pe_wakeup_reason,
                              VOID*                 pv_data)
{
    INT32               i4_return;
    SIZE_T              z_info_len;
    PC_WAKE_UP_INFO_T   t_wakeup_info;
    UINT32              ui4_sub_reason;

    DBG_API(("{PCL} x_pcl_get_wakeup_reason\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the wake-up reason */
    t_wakeup_info.pv_data = (VOID*)&ui4_sub_reason;
    z_info_len = sizeof(PC_WAKE_UP_INFO_T);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_WAKE_UP_REASON,
                       ((VOID*)&t_wakeup_info),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_WAKE_UP_REASON) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    *pe_wakeup_reason = (PCL_WAKE_UP_REASON_T)(t_wakeup_info.e_reason);

    if (pv_data)
    {
        *(UINT32*)pv_data =  (UINT32) t_wakeup_info.pv_data;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_wakeup_reason
 *
 * Description: This API sets the reason of current wake-up.
 *
 * Inputs:  e_wakeup_reason         Contains the wake-up reason.
 *          pv_data                 References the additional information of
 *                                  wakeup reason.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_SET_DEV_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_wakeup_reason(PCL_WAKE_UP_REASON_T e_wakeup_reason,
                              VOID*                pv_data)
{
    INT32               i4_return;
    PC_WAKE_UP_INFO_T   t_wakeup_info;

    DBG_API(("{PCL} x_pcl_set_wakeup_reason: 0x%x\n\r", e_wakeup_reason));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Set the wake-up reason */
    t_wakeup_info.e_reason = (PC_WAKE_UP_REASON_T)e_wakeup_reason;
    t_wakeup_info.pv_data = pv_data;

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_WAKE_UP_REASON,
                       (VOID*)&t_wakeup_info,
                       sizeof(PC_WAKE_UP_INFO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_WAKE_UP_REASON) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_misc_status
 *
 * Description: This API gets the Power Control driver miscellaneous status.
 *
 * Inputs:  -
 *
 * Outputs: pui4_status       References the bit-mask of miscellaneous status.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_misc_status(UINT32* pui4_status)
{
    INT32       i4_return;
    SIZE_T      z_info_len;

    DBG_API(("{PCL} x_pcl_get_misc_status\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the current miscellaneous status */
    z_info_len = sizeof(UINT32);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_MISC_STATUS,
                       ((VOID*)pui4_status),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_MISC_STATUS) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_power_down_reason
 *
 * Description: This API gets the power-down reason.
 *
 * Inputs:  -
 *
 * Outputs: pe_power_down_reason    References the power-down reason.
 *
 *          pv_data                 References the additional information of
 *                                  power-down reason.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_power_down_reason(PCL_POWER_DOWN_REASON_T* pe_power_down_reason,
                                  VOID*                    pv_data)
{
    INT32                   i4_return;
    SIZE_T                  z_info_len;
    PC_POWER_DOWN_INFO_T    t_power_down_info;

    DBG_API(("{PCL} x_pcl_get_power_down_reason\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the power-down reason */
    z_info_len = sizeof(PC_POWER_DOWN_INFO_T);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_POWER_DOWN_REASON,
                       ((VOID*)&t_power_down_info),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_POWER_DOWN_REASON) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    *pe_power_down_reason = (PCL_POWER_DOWN_REASON_T)(t_power_down_info.e_reason);
    if (pv_data)
    {
        *(UINT32 *)pv_data = (UINT32)t_power_down_info.pv_data; /* button code */
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_power_down_reason
 *
 * Description: This API sets the power-down reason.
 *
 * Inputs:  e_power_down_reason     Contains the power-down reason.
 *          pv_data               - References the additional information of
 *                                       wakeup reason.
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_power_down_reason(PCL_POWER_DOWN_REASON_T e_power_down_reason,
                                  VOID                    *pv_data)
{
    INT32                   i4_return;
    PC_POWER_DOWN_INFO_T    t_power_down_info;

    DBG_API(("{PCL} x_pcl_set_power_down_reason: 0x%x\n\r", e_power_down_reason));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Set the power-down reason */
    t_power_down_info.e_reason = (PC_POWER_DOWN_REASON_T)e_power_down_reason;
    t_power_down_info.pv_data = pv_data; /* button code, not the address */

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_POWER_DOWN_REASON,
                       (VOID*)&t_power_down_info,
                       sizeof(PC_POWER_DOWN_INFO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_POWER_DOWN_REASON) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_power_down
 *
 * Description: This API sets a wake-up time into driver if available and then
 *              turns the system into power-down mode.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_power_down(VOID)
{
    INT32               i4_return;
    PCL_WAKE_UP_EVT_T*  pt_evt;
    UINT32              ui4_wakeup_setup;
    BOOL                b_set = FALSE;

    DBG_API(("{PCL} x_pcl_set_power_down\n\r"));
    DBG_INFO(("{PCL} x_pcl_set_power_down\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    pcl_lock_rd_db();

    /* Search for the first valid wake-up control from wake-up list and then set it into driver */
    DLIST_FOR_EACH(pt_evt, &t_pcl_wakeup_lst, t_link)
    {
        if (pt_evt->t_wakeup_ctrl.t_time == NULL_TIME)
        {
            /* If the wake-up time of a PCL wake-up event is equal to NULL_TIME,
               only set its misc control to driver if applicable */
            if (pt_evt->t_wakeup_ctrl.ui4_misc_ctrl != PCL_MISC_CTRL_IGNORE)
            {
                /* Set miscellaneous controls */
                i4_return = rm_set(h_pcd,
                                   FALSE,
                                   ANY_PORT_NUM,
                                   PC_SET_TYPE_MISC_CTRL,
                                   (VOID*)pt_evt->t_wakeup_ctrl.ui4_misc_ctrl,
                                   sizeof(UINT32),
                                   NULL);

                if (i4_return != RMR_OK)
                {
                    DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_MISC_CTRL) failed\n\r"));
                    return PCLR_DEV_SET_FAILED;
                }
                else
                {
                    DBG_INFO(("{PCL} rm_set(PC_SET_TYPE_MISC_CTRL): 0x%x\n\r",
                             (UINT32)(pt_evt->t_wakeup_ctrl.ui4_misc_ctrl)));
                }
            }
        }
        else
        {
            if (pt_evt->t_wakeup_ctrl.t_time > x_dt_get_utc(NULL, NULL))
            {
                /* Set wake-up time */
                i4_return = rm_set(h_pcd,
                                   FALSE,
                                   ANY_PORT_NUM,
                                   PC_SET_TYPE_WAKE_UP_TIME,
                                   (VOID*)&pt_evt->t_wakeup_ctrl.t_time,
                                   sizeof(TIME_T),
                                   NULL);

                if (i4_return != RMR_OK)
                {
                    DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_WAKE_UP_TIME) failed\n\r"));
                    return PCLR_DEV_SET_FAILED;
                }
                else
                {
                    DBG_INFO(("{PCL} rm_set(PC_SET_TYPE_WAKE_UP_TIME): 0x%08x%08x\n\r",
                             (UINT32)(pt_evt->t_wakeup_ctrl.t_time >> 32),
                             (UINT32)pt_evt->t_wakeup_ctrl.t_time));
                }

                /* Set miscellaneous controls */
                if (pt_evt->t_wakeup_ctrl.ui4_misc_ctrl != PCL_MISC_CTRL_IGNORE)
                {
                    i4_return = rm_set(h_pcd,
                                       FALSE,
                                       ANY_PORT_NUM,
                                       PC_SET_TYPE_MISC_CTRL,
                                       (VOID*)pt_evt->t_wakeup_ctrl.ui4_misc_ctrl,
                                       sizeof(UINT32),
                                       NULL);

                    if (i4_return != RMR_OK)
                    {
                        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_MISC_CTRL) failed\n\r"));
                        return PCLR_DEV_SET_FAILED;
                    }
                    else
                    {
                        DBG_INFO(("{PCL} rm_set(PC_SET_TYPE_MISC_CTRL): 0x%x\n\r",
                                 (UINT32)(pt_evt->t_wakeup_ctrl.ui4_misc_ctrl)));
                    }
                }

                b_set = TRUE;
                break;
            }
        }
    }

    pcl_unlock_db();

    if (!b_set)
    {
        if (x_pcl_get_wakeup_setup(&ui4_wakeup_setup) != PCLR_OK)
        {
            return PCLR_ERROR;
        }

        ui4_wakeup_setup &= ~PCL_WAKE_UP_CAP_RTC;

        if (x_pcl_set_wakeup_setup(ui4_wakeup_setup) != PCLR_OK)
        {
            return PCLR_ERROR;
        }

        DBG_INFO(("{PCL} None of wake-up controls is valid\n\r"));
    }

    /* Power down the system */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_POWER_DOWN,
                       NULL,
                       0,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_POWER_DOWN) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_power_state
 *
 * Description: This API changes the system power state.
 *
 * Inputs:  pe_pwr_state_ctrl       Contains power state setting.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_power_state(PCL_PWR_STATE_CTRL_T* pe_pwr_state_ctrl)
{
    INT32       i4_return;
    SIZE_T      z_info_len;

    DBG_API(("{PCL} x_pcl_get_power_state: 0x%x\n\r", pe_pwr_state_ctrl));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    z_info_len = sizeof(UINT32);
    
    /* Get power state */
    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_POWER_STATE,
                       (VOID*)pe_pwr_state_ctrl,
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_SET_TYPE_POWER_STATE) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_power_state
 *
 * Description: This API changes the system power state.
 *
 * Inputs:  e_pwr_state_ctrl        Contains power state setting.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_power_state(PCL_PWR_STATE_CTRL_T e_pwr_state_ctrl)
{
    INT32       i4_return;

    DBG_API(("{PCL} x_pcl_set_power_state: 0x%x\n\r", e_pwr_state_ctrl));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Set the system to power-down mode */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_POWER_STATE,
                       (VOID*)&e_pwr_state_ctrl,
                       sizeof(PC_PWR_STATE_CTRL_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_POWER_STATE) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_reboot
 *
 * Description: This API reboots the systems.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_reboot(VOID)
{
    INT32       i4_return;

    DBG_API(("{PCL} x_pcl_set_reboot\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Reboot the system */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_REBOOT,
                       NULL,
                       0,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_REBOOT) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_gpio
 *
 * Description: This API gets the status of specific GPIO pin (for either input
 *              or output mode).
 *
 * Inputs:  ui4_gpio_id             Contains the GPIO ID.
 *          e_gpio_mode             Contains the GPIO mode.
 *
 * Outputs: pe_gpio_status          References the GPIO status.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_gpio(UINT32             ui4_gpio_id,
                     PCL_GPIO_MODE_T    e_gpio_mode,
                     PCL_GPIO_STATUS_T* pe_gpio_status)
{
    INT32                   i4_return;
    SIZE_T                  z_info_len;
    PC_GPIO_MODE_T          e_mode;
    PC_GPIO_MODE_INFO_T     t_gpio_mode;
    PC_GPIO_STATUS_INFO_T   t_gpio_status;

    DBG_API(("{PCL} x_pcl_get_gpio: id=0x%x, mode=%d\n\r", ui4_gpio_id, e_gpio_mode));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get current GPIO mode */
    z_info_len = sizeof(PC_GPIO_MODE_INFO_T);
    t_gpio_mode.ui4_id = ui4_gpio_id;

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_GPIO_MODE,
                       ((VOID*)&t_gpio_mode),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_GPIO_MODE) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    e_mode = t_gpio_mode.e_mode;

    /* Change GPIO mode if required */
    if (e_gpio_mode != (PCL_GPIO_MODE_T)e_mode)
    {
        t_gpio_mode.ui4_id = ui4_gpio_id;
        t_gpio_mode.e_mode = (PC_GPIO_MODE_T)e_gpio_mode;

        i4_return = rm_set(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_SET_TYPE_GPIO_MODE,
                           (VOID*)&t_gpio_mode,
                           sizeof(PC_GPIO_MODE_INFO_T),
                           NULL);

        if (i4_return != RMR_OK)
        {
            DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_GPIO_MODE) failed\n\r"));
            return PCLR_DEV_SET_FAILED;
        }
    }

    /* Get specific GPIO status */
    z_info_len = sizeof(PC_GPIO_STATUS_INFO_T);
    t_gpio_status.ui4_id = ui4_gpio_id;

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_GPIO_STATUS,
                       ((VOID*)&t_gpio_status),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_GPIO_STATUS) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    *pe_gpio_status = (PCL_GPIO_STATUS_T)t_gpio_status.e_status;

    /* Restore GPIO mode back if required */
    if (e_gpio_mode != (PCL_GPIO_MODE_T)e_mode)
    {
        t_gpio_mode.ui4_id = ui4_gpio_id;
        t_gpio_mode.e_mode = e_mode;

        i4_return = rm_set(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_SET_TYPE_GPIO_MODE,
                           (VOID*)&t_gpio_mode,
                           sizeof(PC_GPIO_MODE_INFO_T),
                           NULL);

        if (i4_return != RMR_OK)
        {
            DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_GPIO_MODE) failed\n\r"));
            return PCLR_DEV_SET_FAILED;
        }
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_gpio
 *
 * Description: This API changes the status of specific GPIO pin output (output
 *              mode only).
 *
 * Inputs:  ui4_gpio_id         Contains the GPIO ID.
 *          e_gpio_status       Contains the GPIO status.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_gpio(UINT32            ui4_gpio_id,
                     PCL_GPIO_STATUS_T e_gpio_status)
{
    INT32                   i4_return;
    SIZE_T                  z_info_len;
    PC_GPIO_MODE_INFO_T     t_gpio_mode;
    PC_GPIO_STATUS_INFO_T   t_gpio_status;

    DBG_API(("{PCL} x_pcl_set_gpio: id=0x%x, status=%d\n\r", ui4_gpio_id, e_gpio_status));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get current GPIO mode */
    z_info_len = sizeof(PC_GPIO_MODE_INFO_T);
    t_gpio_mode.ui4_id = ui4_gpio_id;

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_GPIO_MODE,
                       ((VOID*)&t_gpio_mode),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_GPIO_MODE) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    /* Change GPIO mode to output */
    if (t_gpio_mode.e_mode != PC_GPIO_MODE_OUTPUT)
    {
        t_gpio_mode.ui4_id = ui4_gpio_id;
        t_gpio_mode.e_mode = PC_GPIO_MODE_OUTPUT;

        i4_return = rm_set(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_SET_TYPE_GPIO_MODE,
                           (VOID*)&t_gpio_mode,
                           sizeof(PC_GPIO_MODE_INFO_T),
                           NULL);

        if (i4_return != RMR_OK)
        {
            DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_GPIO_MODE) failed\n\r"));
            return PCLR_DEV_SET_FAILED;
        }
    }

    /* Change specific GPIO status */
    t_gpio_status.ui4_id = ui4_gpio_id;
    t_gpio_status.e_status = (PC_GPIO_STATUS_T)e_gpio_status;

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_GPIO_STATUS,
                       (VOID*)&t_gpio_status,
                       sizeof(PC_GPIO_STATUS_INFO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_GPIO_STATUS) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_gpio_monitor_ctrl
 *
 * Description: This API gets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  -
 *
 * Outputs: pt_gpio_monitor_ctrl    References GPIO monitoring control info.
 *                                  [55:0] => GPIO 0~55 monitoring control.
 *                                  [63:56] => OPCTRL 0~7 monitoring control.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_gpio_monitor_ctrl(PCL_GPIO_MONITOR_CTRL_T* pt_gpio_monitor_ctrl)
{
    DBG_API(("{PCL} x_pcl_get_gpio_monitor_ctrl\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    pcl_lock_rd_db();

    *pt_gpio_monitor_ctrl = t_gpio_monitor_ctrl;

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_gpio_monitor_ctrl
 *
 * Description: This API sets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  pt_gpio_monitor_ctrl    References GPIO monitoring control info.
 *                                  [55:0] => GPIO 0~55 monitoring control.
 *                                  [63:56] => OPCTRL 0~7 monitoring control.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_gpio_monitor_ctrl(PCL_GPIO_MONITOR_CTRL_T* pt_gpio_monitor_ctrl)
{
    INT32       i4_return;

    DBG_API(("{PCL} x_pcl_set_gpio_monitor_ctrl\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    pcl_lock_wr_db();

    t_gpio_monitor_ctrl = *pt_gpio_monitor_ctrl;

    /* Set GPIO monitoring control */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_GPIO_MONITOR_CTRL,
                       (VOID*)(&t_gpio_monitor_ctrl.ui8_ctrl),
                       sizeof(UINT64),
                       NULL);

    if (i4_return != RMR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_GPIO_MONITOR_CTRL) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    pcl_unlock_db();

    return PCLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_gpio_monitor_ctrl_ex
 *
 * Description: This API gets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  -
 *
 * Outputs: pt_gpio_monitor_ctrl    References GPIO monitoring control info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_gpio_monitor_ctrl_ex(PCL_GPIO_MONITOR_CTRL_EX_T* pt_gpio_monitor_ctrl_ex)
{
    INT32   i4_return;
    SIZE_T  z_info_len;
    PC_GPIO_MONITOR_CTRL_EX_T   t_gpio_monitor_ctrl_ex;
        
    DBG_API(("{PCL} x_pcl_get_gpio_monitor_ctrl_ex\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    z_info_len = sizeof(PC_GPIO_MONITOR_CTRL_EX_T);
    
    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_GPIO_QUERY,
                       ((VOID*)&t_gpio_monitor_ctrl_ex),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_GPIO_QUERY) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    pt_gpio_monitor_ctrl_ex->ui4_gpio = t_gpio_monitor_ctrl_ex.ui4_gpio;
    pt_gpio_monitor_ctrl_ex->b_monitor = t_gpio_monitor_ctrl_ex.b_monitor;

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_gpio_monitor_ctrl_ex
 *
 * Description: This API sets GPIO/OPCTRL monitoring control.
 *
 * Inputs:  pt_gpio_monitor_ctrl    References GPIO monitoring control info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_gpio_monitor_ctrl_ex(PCL_GPIO_MONITOR_CTRL_EX_T* pt_gpio_monitor_ctrl_ex)
{
    INT32       i4_return;
    PC_GPIO_MONITOR_CTRL_EX_T   t_gpio_monitor_ctrl_ex;

    DBG_API(("{PCL} x_pcl_set_gpio_monitor_ctrl\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    pcl_lock_wr_db();

    t_gpio_monitor_ctrl_ex = *(PC_GPIO_MONITOR_CTRL_EX_T*)pt_gpio_monitor_ctrl_ex;
    
    /* Set GPIO monitoring control */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_GPIO_REG,
                       (VOID*)&t_gpio_monitor_ctrl_ex,
                       sizeof(PC_GPIO_MONITOR_CTRL_EX_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_GPIO_MONITOR_CTRL_EX) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    pcl_unlock_db();

    return PCLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_sif
 *
 * Description: This API gets the data from a SIF address.
 *
 * Inputs:  ui4_sif_addr            Contains the SIF address.
 *          ui4_sif_offset          Contains the SIF offset.
 *
 * Outputs: pui1_sif_data           References the SIF data.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_sif(UINT32 ui4_sif_addr,
                    UINT32 ui4_sif_offset,
                    UINT8* pui1_sif_data)
{
    INT32               i4_return;
    SIZE_T              z_info_len;
    PC_SIF_INFO_T       t_sif_info;

    DBG_API(("{PCL} x_pcl_get_sif: addr=0x%x, offset=0x%x\n\r", ui4_sif_addr, ui4_sif_offset));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get specific SIF data */
    z_info_len = sizeof(PC_SIF_INFO_T);
    t_sif_info.ui4_addr = ui4_sif_addr;
    t_sif_info.ui4_offset = ui4_sif_offset;

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_SIF,
                       ((VOID*)&t_sif_info),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_SIF) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    *pui1_sif_data = t_sif_info.ui1_data;

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_sif
 *
 * Description: This API sets the data to a SIF address.
 *
 * Inputs:  ui4_sif_addr            Contains the SIF address.
 *          ui4_sif_offset          Contains the SIF offset.
 *          ui1_sif_data            Contains the SIF data.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_sif(UINT32 ui4_sif_addr,
                    UINT32 ui4_sif_offset,
                    UINT8  ui1_sif_data)
{
    INT32               i4_return;
    PC_SIF_INFO_T       t_sif_info;

    DBG_API(("{PCL} x_pcl_set_sif: addr=0x%x, offset=0x%x, data=0x%x\n\r",
            ui4_sif_addr, ui4_sif_offset, ui1_sif_data));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Change specific SIF data */
    t_sif_info.ui4_addr = ui4_sif_addr;
    t_sif_info.ui4_offset = ui4_sif_offset;
    t_sif_info.ui1_data = ui1_sif_data;

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_SIF,
                       (VOID*)&t_sif_info,
                       sizeof(PC_SIF_INFO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_SIF) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_sif_advanced
 *
 * Description: This API gets the data from a SIF address (advanced operation).
 *
 * Inputs:  pt_sif_info             References the SIF info.
 *
 * Outputs: pt_sif_info             References the SIF info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_sif_advanced(PCL_SIF_INFO_T* pt_sif_info)
{
    INT32                   i4_return;
    SIZE_T                  z_info_len;
    PC_SIF_INFO_ADVANCED_T  t_sif_info_advanced;

    DBG_API(("{PCL} x_pcl_get_sif_advanced\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get specific SIF data */
    z_info_len = sizeof(PC_SIF_INFO_ADVANCED_T);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_SIF_ADVANCED,
                       ((VOID*)&t_sif_info_advanced),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_SIF_ADVANCED) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    pt_sif_info->e_channel_id = (PCL_SIF_CH_ID_T)t_sif_info_advanced.e_channel_id;
    pt_sif_info->pui1_data = t_sif_info_advanced.pui1_data;
    pt_sif_info->ui1_data_len = t_sif_info_advanced.ui1_data_len;
    pt_sif_info->ui1_offset_len = t_sif_info_advanced.ui1_offset_len;
    pt_sif_info->ui4_addr = t_sif_info_advanced.ui4_addr;
    pt_sif_info->ui4_offset = t_sif_info_advanced.ui4_offset;

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_sif_advanced
 *
 * Description: This API sets the data to a SIF address (advanced operation).
 *
 * Inputs:  pt_sif_info             References the SIF info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_sif_advanced(PCL_SIF_INFO_T* pt_sif_info)
{
    INT32                   i4_return;
    PC_SIF_INFO_ADVANCED_T  t_sif_info_advanced;

    DBG_API(("{PCL} x_pcl_set_sif_advanced\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Change specific SIF data */
    t_sif_info_advanced = *(PC_SIF_INFO_ADVANCED_T*)pt_sif_info;

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_SIF_ADVANCED,
                       (VOID*)&t_sif_info_advanced,
                       sizeof(PC_SIF_INFO_ADVANCED_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_SIF_ADVANCED) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_sif_by_gpio
 *
 * Description: This API gets the data of specific SIF address via GPIO pins.
 *
 * Inputs:  pt_sif_info_by_gpio     References the SIF info.
 *
 * Outputs: pt_sif_info_by_gpio     References the SIF info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_sif_by_gpio(PCL_SIF_INFO_BY_GPIO_T* pt_sif_info_by_gpio)
{
    INT32                   i4_return;
    SIZE_T                  z_info_len;
    PC_SIF_INFO_BY_GPIO_T   t_sif_info_by_gpio;

    DBG_API(("{PCL} x_pcl_get_sif_by_gpio\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get specific SIF data */
    z_info_len = sizeof(PC_SIF_INFO_BY_GPIO_T);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_SIF_BY_GPIO,
                       ((VOID*)&t_sif_info_by_gpio),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_SIF_BY_GPIO) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    pt_sif_info_by_gpio->pui1_data = t_sif_info_by_gpio.pui1_data;
    pt_sif_info_by_gpio->ui1_data_len = t_sif_info_by_gpio.ui1_data_len;
    pt_sif_info_by_gpio->ui1_offset_len = t_sif_info_by_gpio.ui1_offset_len;
    pt_sif_info_by_gpio->ui4_addr = t_sif_info_by_gpio.ui4_addr;
    pt_sif_info_by_gpio->ui4_addr_gpio_id = t_sif_info_by_gpio.ui4_addr_gpio_id;
    pt_sif_info_by_gpio->ui4_data_gpio_id = t_sif_info_by_gpio.ui4_data_gpio_id;
    pt_sif_info_by_gpio->ui4_offset = t_sif_info_by_gpio.ui4_offset;

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_sif_by_gpio
 *
 * Description: This API sets the data to specific SIF address via GPIO pins.
 *
 * Inputs:  pt_sif_info_by_gpio     References the SIF info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_sif_by_gpio(PCL_SIF_INFO_BY_GPIO_T* pt_sif_info_by_gpio)
{
    INT32                   i4_return;
    PC_SIF_INFO_BY_GPIO_T   t_sif_info_by_gpio;

    DBG_API(("{PCL} x_pcl_set_sif_by_gpio\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Change specific SIF data */
    t_sif_info_by_gpio = *(PC_SIF_INFO_BY_GPIO_T*)pt_sif_info_by_gpio;

    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_SIF_BY_GPIO,
                       (VOID*)&t_sif_info_by_gpio,
                       sizeof(PC_SIF_INFO_BY_GPIO_T),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_SIF_BY_GPIO) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_upgrade_ctrl
 *
 * Description: This API gets the status of upgrade controls.
 *
 * Inputs:  -
 *
 * Outputs: pui4_ctrl       References the bit-mask of upgrade controls.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_upgrade_ctrl(UINT32* pui4_ctrl)
{
    INT32       i4_return;
    SIZE_T      z_info_len;

    DBG_API(("{PCL} x_pcl_get_upgrade_ctrl\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the current upgrade controls */
    z_info_len = sizeof(UINT32);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_UPGRADE_CTRL,
                       ((VOID*)pui4_ctrl),
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_UPGRADE_CTRL) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_upgrade_ctrl
 *
 * Description: This API sets the upgrade controls.
 *
 * Inputs:  ui4_ctrl        Contains the bit-mask of upgrade controls.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_upgrade_ctrl(UINT32 ui4_ctrl)
{
    INT32       i4_return;

    DBG_API(("{PCL} x_pcl_set_upgrade_ctrl: 0x%x\n\r", ui4_ctrl));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Set the wake-up setup */
    i4_return = rm_set(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_SET_TYPE_UPGRADE_CTRL,
                       (VOID*)ui4_ctrl,
                       sizeof(UINT32),
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_UPGRADE_CTRL) failed\n\r"));
        return PCLR_DEV_SET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_servo_ad
 *
 * Description: This API gets the status of specific servo AD.
 *
 * Inputs:  pt_servo_ad_info        References the servo AD info.
 *
 * Outputs: pt_servo_ad_info        References the servo AD info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_servo_ad(PCL_SERVO_AD_INFO_T* pt_servo_ad_info)
{
    INT32       i4_return;
    SIZE_T      z_info_len;

    DBG_API(("{PCL} x_pcl_get_servo_ad\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (h_pcd == NULL_HANDLE)
    {
        return PCLR_HW_NOT_SUPPORT;
    }

    /* Get the current servo AD */
    z_info_len = sizeof(PCL_SERVO_AD_INFO_T);

    i4_return = rm_get(h_pcd,
                       FALSE,
                       ANY_PORT_NUM,
                       PC_GET_TYPE_SERVO_AD,
                       (VOID*)pt_servo_ad_info,
                       &z_info_len,
                       NULL);

    if (i4_return != RMR_OK)
    {
        DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_SERVO_AD) failed\n\r"));
        return PCLR_DEV_GET_FAILED;
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_reg_nfy_fct
 *
 * Description: This API registers a callback function with the Power Control
 *              Library in order to receive condition changes.
 *
 * Inputs:  pv_tag          Private tag value which must be returned in the
 *                          notify function. It can be set to NULL.
 *          pf_nfy          References the caller's notify function.
 *
 * Outputs: ph_nfy          A handle referencing a notification record.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_MEM_ALLOC_FAILED   Memory allocation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_reg_nfy_fct(VOID*         pv_tag,
                        x_pcl_nfy_fct pf_nfy,
                        HANDLE_T*     ph_nfy)
{
    INT32               i4_return;
    PCL_NFY_REC_T*      pt_nfy_rec;
    CRIT_STATE_T        t_cs;

    DBG_API(("{PCL} x_pcl_reg_nfy_fct\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (ph_nfy == NULL)
    {
        return PCLR_INV_ARG;
    }

    pcl_lock_wr_db();

    /* Allocate a notify record and insert it into notify list */
    pt_nfy_rec = (PCL_NFY_REC_T*)x_mem_alloc(sizeof(PCL_NFY_REC_T));

    if (pt_nfy_rec != NULL)
    {
        x_memset(pt_nfy_rec, 0, sizeof(PCL_NFY_REC_T));
        pt_nfy_rec->pv_tag = pv_tag;
        pt_nfy_rec->pf_nfy = pf_nfy;
    }
    else
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: x_mem_alloc() failed\n\r"));
        return PCLR_MEM_ALLOC_FAILED;
    }

    i4_return = handle_alloc(PCL_HANDLE_TYPE_NFY_REC,
                             (VOID*)pt_nfy_rec,
                             pv_tag,
                             &_pcl_handle_free_nfy_fct,
                             ph_nfy);

    if (i4_return != HR_OK)
    {
        x_mem_free((VOID*)pt_nfy_rec);
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: handle_alloc() failed\n\r"));
        return PCLR_HANDLE_LIB_ERROR;
    }

    pt_nfy_rec->h_nfy = *ph_nfy;

    t_cs = x_crit_start();

    DLIST_INSERT_TAIL(pt_nfy_rec, &t_pcl_nfy_lst, t_link);

    x_crit_end(t_cs);

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_dereg_nfy_fct
 *
 * Description: This API de-registers a callback function from the Power
 *              Control Library.
 *
 * Inputs:  h_nfy       A handle referencing a notification record.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_dereg_nfy_fct(HANDLE_T h_nfy)
{
    INT32               i4_return;
    PCL_NFY_REC_T*      pt_nfy_rec;
    CRIT_STATE_T        t_cs;

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    pcl_lock_wr_db();

    /* Get the object reference */
    i4_return = handle_get_obj(h_nfy, (VOID*)&pt_nfy_rec);

    if (i4_return != HR_OK)
    {
        pcl_unlock_db();
        return PCLR_HANDLE_LIB_ERROR;
    }

    t_cs = x_crit_start();

    DLIST_REMOVE(pt_nfy_rec, &t_pcl_nfy_lst, t_link);

    x_crit_end(t_cs);

    i4_return = handle_free(h_nfy, TRUE);

    if (i4_return != HR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCLR} ERR: handle_free() failed\n\r"));
        return PCLR_HANDLE_LIB_ERROR;
    }

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_add_wakeup_event
 *
 * Description: This API adds a wake-up event into wake-up list. Note that the
 *              list is sorted by wake-up time.
 *
 * Inputs:  pt_wakeup_ctrl      References wake-up control info.
 *
 * Outputs: ph_wakeup_evt       A handle referencing the newly added wake-up
 *                              event.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_MEM_ALLOC_FAILED   Memory allocation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_add_wakeup_event(PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl,
                             HANDLE_T*           ph_wakeup_evt)
{
    INT32                   i4_return;
    PCL_WAKE_UP_EVT_T*      pt_evt_new;
    PCL_WAKE_UP_EVT_T*      pt_evt_old;
    BOOL                    b_inserted = FALSE;

    DBG_API(("{PCL} x_pcl_add_wakeup_event\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if ((pt_wakeup_ctrl == NULL) ||
        ((pt_wakeup_ctrl->t_time == NULL_TIME) && (ph_wakeup_evt != &h_wakeup_evt_pcl)))
    {
        return PCLR_INV_ARG;
    }

    pcl_lock_wr_db();

    /* Allocate a wake-up event and insert it into wake-up list */
    pt_evt_new = (PCL_WAKE_UP_EVT_T*)x_mem_alloc(sizeof(PCL_WAKE_UP_EVT_T));

    if (pt_evt_new != NULL)
    {
        x_memset(pt_evt_new, 0, sizeof(PCL_WAKE_UP_EVT_T));
        pt_evt_new->t_wakeup_ctrl = *pt_wakeup_ctrl;
    }
    else
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: x_mem_alloc() failed\n\r"));
        return PCLR_MEM_ALLOC_FAILED;
    }

    i4_return = handle_alloc(PCL_HANDLE_TYPE_WAKE_UP_EVT,
                             (VOID*)pt_evt_new,
                             NULL,
                             &_pcl_handle_free_nfy_fct,
                             ph_wakeup_evt);

    if (i4_return != HR_OK)
    {
        x_mem_free((VOID*)pt_evt_new);
        pcl_unlock_db();
        DBG_ERROR(("{PCL} ERR: handle_alloc() failed\n\r"));
        return PCLR_HANDLE_LIB_ERROR;
    }

    pt_evt_new->h_wakeup_evt = *ph_wakeup_evt;

    /* Insert wake-up event into the list and sort it by wake-up time */
    if (DLIST_IS_EMPTY(&t_pcl_wakeup_lst))
    {
        DLIST_INSERT_HEAD(pt_evt_new, &t_pcl_wakeup_lst, t_link);
    }
    else
    {
        DLIST_FOR_EACH(pt_evt_old, &t_pcl_wakeup_lst, t_link)
        {
            if (pt_evt_new->t_wakeup_ctrl.t_time > pt_evt_old->t_wakeup_ctrl.t_time)
            {
                continue;
            }
            else
            {
                DLIST_INSERT_BEFORE(pt_evt_new, pt_evt_old, &t_pcl_wakeup_lst, t_link);
                b_inserted = TRUE;
                break;
            }
        }

        if (!b_inserted)
        {
            DLIST_INSERT_TAIL(pt_evt_new, &t_pcl_wakeup_lst, t_link);
        }
    }

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_remove_wakeup_event
 *
 * Description: This API removes a wake-up event from wake-up list.
 *
 * Inputs:  h_wakeup_evt        A handle referencing a wake-up event.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_remove_wakeup_event(HANDLE_T h_wakeup_evt)
{
    INT32                   i4_return;
    PCL_WAKE_UP_EVT_T*      pt_evt;

    DBG_API(("{PCL} x_pcl_remove_wakeup_event\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    pcl_lock_wr_db();

    /* Get the object reference */
    i4_return = handle_get_obj(h_wakeup_evt, (VOID*)&pt_evt);

    if (i4_return != HR_OK)
    {
        pcl_unlock_db();
        return PCLR_HANDLE_LIB_ERROR;
    }

    DLIST_REMOVE(pt_evt, &t_pcl_wakeup_lst, t_link);

    i4_return = handle_free(h_wakeup_evt, TRUE);

    if (i4_return != HR_OK)
    {
        pcl_unlock_db();
        DBG_ERROR(("{PCLR} ERR: handle_free() failed\n\r"));
        return PCLR_HANDLE_LIB_ERROR;
    }

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_wakeup_event
 *
 * Description: This API obtains the wake-up control info of a wake-up event.
 *
 * Inputs:  h_wakeup_evt        A handle referencing a wake-up event.
 *
 * Outputs: pt_wakeup_ctrl      References wake-up control info.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_wakeup_event(HANDLE_T            h_wakeup_evt,
                             PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    INT32                   i4_return;
    PCL_WAKE_UP_EVT_T*      pt_evt;

    DBG_API(("{PCL} x_pcl_get_wakeup_event\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (pt_wakeup_ctrl == NULL)
    {
        return PCLR_INV_ARG;
    }

    /* Special handle for PCL wake-up event for backward compatibility */
    if (h_wakeup_evt == h_wakeup_evt_pcl)
    {
        SIZE_T      z_info_len;

        /* Get wake-up time from Power Control driver */
        z_info_len = sizeof(TIME_T);

        i4_return = rm_get(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_GET_TYPE_WAKE_UP_TIME,
                           (VOID*)&pt_wakeup_ctrl->t_time,
                           &z_info_len,
                           NULL);

        if (i4_return != RMR_OK)
        {
            DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_WAKE_UP_TIME) failed\n\r"));
            return PCLR_DEV_GET_FAILED;
        }

        /* Get miscellaneous controls from Power Control driver */
        z_info_len = sizeof(UINT32);

        i4_return = rm_get(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_GET_TYPE_MISC_CTRL,
                           (VOID*)&pt_wakeup_ctrl->ui4_misc_ctrl,
                           &z_info_len,
                           NULL);

        if (i4_return != RMR_OK)
        {
            DBG_ERROR(("{PCL} ERR: rm_get(PC_GET_TYPE_MISC_CTRL) failed\n\r"));
            return PCLR_DEV_GET_FAILED;
        }
    }
    else
    {
        pcl_lock_rd_db();

        /* Get the object reference */
        i4_return = handle_get_obj(h_wakeup_evt, (VOID*)&pt_evt);

        if (i4_return != HR_OK)
        {
            pcl_unlock_db();
            return PCLR_HANDLE_LIB_ERROR;
        }

        *pt_wakeup_ctrl = pt_evt->t_wakeup_ctrl;

        pcl_unlock_db();
    }

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_wakeup_event
 *
 * Description: This API changes the control info of a existing wake-up event
 *              in wake-up list.
 *
 * Inputs:  h_wakeup_evt        A handle referencing a wake-up event.
 *          pt_wakeup_ctrl      References wake-up control info.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_ERROR              Operation error.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_wakeup_event(HANDLE_T            h_wakeup_evt,
                             PCL_WAKE_UP_CTRL_T* pt_wakeup_ctrl)
{
    INT32                   i4_return;
    PCL_WAKE_UP_EVT_T*      pt_evt_new;
    PCL_WAKE_UP_EVT_T*      pt_evt_old;
    BOOL                    b_inserted = FALSE;

    DBG_API(("{PCL} x_pcl_set_wakeup_event\n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if ((pt_wakeup_ctrl == NULL) ||
        ((pt_wakeup_ctrl->t_time == NULL_TIME) && (h_wakeup_evt != h_wakeup_evt_pcl)))
    {
        return PCLR_INV_ARG;
    }

    /* Special handle for PCL wake-up event for backward compatibility */
    if (h_wakeup_evt == h_wakeup_evt_pcl)
    {
        UINT32      ui4_wakeup_setup;

        /* Set wake-up time to Power Control driver */
        if (pt_wakeup_ctrl->t_time == NULL_TIME)
        {
            if (x_pcl_get_wakeup_setup(&ui4_wakeup_setup) != PCLR_OK)
            {
                return PCLR_ERROR;
            }

            ui4_wakeup_setup &= ~PCL_WAKE_UP_CAP_RTC;

            if (x_pcl_set_wakeup_setup(ui4_wakeup_setup) != PCLR_OK)
            {
                return PCLR_ERROR;
            }
        }
        else
        {
            i4_return = rm_set(h_pcd,
                               FALSE,
                               ANY_PORT_NUM,
                               PC_SET_TYPE_WAKE_UP_TIME,
                               (VOID*)&pt_wakeup_ctrl->t_time,
                               sizeof(TIME_T),
                               NULL);

            if (i4_return != RMR_OK)
            {
                DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_WAKE_UP_TIME) failed\n\r"));
                return PCLR_DEV_SET_FAILED;
            }
        }

        /* Set miscellaneous control to Power Control driver */
        i4_return = rm_set(h_pcd,
                           FALSE,
                           ANY_PORT_NUM,
                           PC_SET_TYPE_MISC_CTRL,
                           (VOID*)pt_wakeup_ctrl->ui4_misc_ctrl,
                           sizeof(UINT32),
                           NULL);

        if (i4_return != RMR_OK)
        {
            DBG_ERROR(("{PCL} ERR: rm_set(PC_SET_TYPE_MISC_CTRL) failed\n\r"));
            return PCLR_DEV_SET_FAILED;
        }
    }

    pcl_lock_wr_db();

    /* Get the object reference */
    i4_return = handle_get_obj(h_wakeup_evt, (VOID*)&pt_evt_new);

    if (i4_return != HR_OK)
    {
        pcl_unlock_db();
        return PCLR_HANDLE_LIB_ERROR;
    }

   DLIST_REMOVE(pt_evt_new, &t_pcl_wakeup_lst, t_link);
   pt_evt_new->t_wakeup_ctrl = *pt_wakeup_ctrl;

    /* Insert a wake-up event into the list and sort it by wake-up time */
    if (DLIST_IS_EMPTY(&t_pcl_wakeup_lst))
    {
        DLIST_INSERT_HEAD(pt_evt_new, &t_pcl_wakeup_lst, t_link);
    }
    else
    {
        DLIST_FOR_EACH(pt_evt_old, &t_pcl_wakeup_lst, t_link)
        {
            if (pt_evt_new->t_wakeup_ctrl.t_time > pt_evt_old->t_wakeup_ctrl.t_time)
            {
                continue;
            }
            else
            {
                DLIST_INSERT_BEFORE(pt_evt_new, pt_evt_old, &t_pcl_wakeup_lst, t_link);
                b_inserted = TRUE;
                break;
            }
        }

        if (!b_inserted)
        {
            DLIST_INSERT_TAIL(pt_evt_new, &t_pcl_wakeup_lst, t_link);
        }
    }

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_res_count
 *
 * Description: This API obtains the count of a specific resource.
 *
 * Inputs:  e_res_type          Contains the resource type.
 *
 * Outputs: pui2_count          References a count of the resource.
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_res_count(PCL_RES_TYPE_T e_res_type,
                          UINT16*        pui2_count)
{
    DBG_API(("{PCL} x_pcl_get_res_count: %d\n\r", (UINT16)e_res_type));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (pui2_count == NULL)
    {
        return PCLR_INV_ARG;
    }

    if (e_res_type >= PCL_RES_TYPE_MAX)
    {
        return PCLR_INV_ARG;
    }

    pcl_lock_rd_db();

    *pui2_count = aui2_res_cnt[(UINT16)e_res_type];

    pcl_unlock_db();

    return PCLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_res_count
 *
 * Description: This API increase the count of a specific resource.
 *
 * Inputs:  e_res_type          Contains the resource type.
 *          b_increase          Indicates the counter should be increased or
 *                              decreased by one.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_HANDLE_LIB_ERROR   A handle library error occurred.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_res_count(PCL_RES_TYPE_T e_res_type,
                          BOOL           b_increase)
{
    PCL_NFY_REC_T*      pt_nfy_rec = NULL;
    PCL_COND_T          e_nfy_cond;

    DBG_API(("{PCL} x_pcl_set_res_count: %d\n\r", (UINT16)e_res_type));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    if (e_res_type >= PCL_RES_TYPE_MAX)
    {
        return PCLR_INV_ARG;
    }

    pcl_lock_wr_db();

    /* Modify the counter */
    if (b_increase)
    {
        if (aui2_res_cnt[(UINT16)e_res_type] != 0xFFFF)
        {
            aui2_res_cnt[(UINT16)e_res_type] += 1;
        }
        else
        {
            pcl_unlock_db();
            return PCLR_ERROR;
        }
    }
    else
    {
        if (aui2_res_cnt[(UINT16)e_res_type] > 0)
        {
            aui2_res_cnt[(UINT16)e_res_type] -= 1;
        }
        else
        {
            pcl_unlock_db();
            return PCLR_ERROR;
        }
    }

    /* Notify clients */
    switch(e_res_type)
    {
        case PCL_RES_TYPE_PANEL:
            e_nfy_cond = PCL_COND_RES_CNT_UPDATE_PANEL;
            break;

        case PCL_RES_TYPE_CPU:
            e_nfy_cond = PCL_COND_RES_CNT_UPDATE_CPU;
            break;

        case PCL_RES_TYPE_CUSTOM_1:
            e_nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_1;
            break;

        case PCL_RES_TYPE_CUSTOM_2:
            e_nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_2;
            break;

        case PCL_RES_TYPE_CUSTOM_3:
            e_nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_3;
            break;

        case PCL_RES_TYPE_CUSTOM_4:
            e_nfy_cond = PCL_COND_RES_CNT_UPDATE_CUSTOM_4;
            break;

        default:
            pcl_unlock_db();
            return PCLR_INV_ARG;
    }

    DLIST_FOR_EACH(pt_nfy_rec, &t_pcl_nfy_lst, t_link)
    {
        pt_nfy_rec->pf_nfy(pt_nfy_rec->h_nfy,
                           pt_nfy_rec->pv_tag,
                           e_nfy_cond,
                           (UINT32)(aui2_res_cnt[(UINT16)e_res_type]));
    }

    pcl_unlock_db();

    return PCLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_dual_bank_index
 *
 * Description: This API gets the dual bank index for the specified dual bank
 *              type.
 *
 * Inputs:  e_dual_bank             Type of the dual bank index.
 *
 * Outputs: pui1_index              References the dual bank index (0/1).
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_dual_bank_index(PCL_DUAL_BANK_TYPE_T    e_dual_bank,
                                UINT8 *                 pui1_index)
{
    INT32                   i4_return;
    UINT32                  ui4_misc_ctrl;
    UINT32                  ui4_bit_mask;

    DBG_API(("{PCL} x_pcl_get_dual_bank_index\n\r"));

    if (pui1_index == NULL)
    {
        return PCLR_INV_ARG;
    }

    do
    {
        i4_return = x_pcl_get_misc_ctrl(&ui4_misc_ctrl);
        if (i4_return != PCLR_OK)
        {
            break;
        }

        switch (e_dual_bank)
        {
            case PCL_DUAL_BANK_SYSTEM:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_SYSTEM;
                break;

            case PCL_DUAL_BANK_ROOTFS:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_ROOTFS;
                break;

            case PCL_DUAL_BANK_KERNEL:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_KERNEL;
                break;

            default:
                i4_return = PCLR_INV_ARG;
                ui4_bit_mask = 0;
                break;
        }
        if (i4_return != PCLR_OK)
        {
            break;
        }

        *pui1_index = ((ui4_misc_ctrl & ui4_bit_mask) != 0);
    } while (0);

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_dual_bank_index
 *
 * Description: This API sets the dual bank index for the specified dual bank
 *              type.
 *
 * Inputs:  e_dual_bank             Type of the dual bank index.
 *          ui1_index               Dual bank index value (0/1).
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_dual_bank_index(PCL_DUAL_BANK_TYPE_T    e_dual_bank,
                                UINT8                   ui1_index)
{
    INT32                   i4_return;
    UINT32                  ui4_misc_ctrl;
    UINT32                  ui4_bit_mask;

    DBG_API(("{PCL} x_pcl_set_dual_bank_index\n\r"));

    do
    {
        i4_return = x_pcl_get_misc_ctrl(&ui4_misc_ctrl);
        if (i4_return != PCLR_OK)
        {
            break;
        }

        switch (e_dual_bank)
        {
            case PCL_DUAL_BANK_SYSTEM:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_SYSTEM;
                break;

            case PCL_DUAL_BANK_ROOTFS:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_ROOTFS;
                break;

            case PCL_DUAL_BANK_SYSTEM_AND_ROOTFS:
                ui4_bit_mask = (PCL_MISC_CTRL_DUAL_BANK_SYSTEM |
                                PCL_MISC_CTRL_DUAL_BANK_ROOTFS);
                break;

            case PCL_DUAL_BANK_KERNEL:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_KERNEL;
                break;

            default:
                i4_return = PCLR_INV_ARG;
                ui4_bit_mask = 0;
                break;
        }
        if (i4_return != PCLR_OK)
        {
            break;
        }

        if (ui1_index == 0)
        {
            ui4_misc_ctrl &= ~ui4_bit_mask;
        }
        else if (ui1_index == 1)
        {
            ui4_misc_ctrl |= ui4_bit_mask;
        }
        else
        {
            i4_return = PCLR_INV_ARG;
            break;
        }

        i4_return = x_pcl_set_misc_ctrl(ui4_misc_ctrl);
    } while (0);

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_toggle_dual_bank_index
 *
 * Description: This API toggles the dual bank index for the specified dual bank
 *              type, changing from 0 to 1 or 1 to 0.
 *
 * Inputs:  e_dual_bank             Type of the dual bank index.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_toggle_dual_bank_index(PCL_DUAL_BANK_TYPE_T    e_dual_bank)
{
    INT32                   i4_return;
    UINT32                  ui4_misc_ctrl;
    UINT32                  ui4_bit_mask;

    DBG_API(("{PCL} x_pcl_toggle_dual_bank_index\n\r"));

    do
    {
        i4_return = x_pcl_get_misc_ctrl(&ui4_misc_ctrl);
        if (i4_return != PCLR_OK)
        {
            break;
        }

        switch (e_dual_bank)
        {
            case PCL_DUAL_BANK_SYSTEM:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_SYSTEM;
                break;

            case PCL_DUAL_BANK_ROOTFS:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_ROOTFS;
                break;

            case PCL_DUAL_BANK_SYSTEM_AND_ROOTFS:
                ui4_bit_mask = (PCL_MISC_CTRL_DUAL_BANK_SYSTEM |
                                PCL_MISC_CTRL_DUAL_BANK_ROOTFS);
                break;

            case PCL_DUAL_BANK_KERNEL:
                ui4_bit_mask = PCL_MISC_CTRL_DUAL_BANK_KERNEL;
                break;

            default:
                i4_return = PCLR_INV_ARG;
                ui4_bit_mask = 0;
                break;
        }
        if (i4_return != PCLR_OK)
        {
            break;
        }

        ui4_misc_ctrl ^= ui4_bit_mask;

        i4_return = x_pcl_set_misc_ctrl(ui4_misc_ctrl);
    } while (0);

    return i4_return;
}


#ifdef MW_FAST_INIT
/*-----------------------------------------------------------------------------
 * Name: x_pcl_get_boot_type
 *
  * Description: This API gets the system boot type.
 *
 * Inputs:  None             
 *
 * Outputs: pe_boot_type            References the boot type which must be one of 
 *                                  (PCL_MISC_CTRL_FAST_BOOT, PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND,
 *                                  PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND).
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_GET_FAILED     Device get operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_get_boot_type(PCL_BOOT_TYPE_T    *pe_boot_type)
{
    INT32                   i4_return;
    UINT32                  ui4_misc_ctrl;
    UINT32                  ui4_bit_mask = PCL_MISC_CTRL_FAST_BOOT 
                                           | PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND
                                           | PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND;

    DBG_API(("{PCL} x_pcl_get_boot_type\n\r"));

    if (pe_boot_type == NULL)
    {
        return PCLR_INV_ARG;
    }

    i4_return = x_pcl_get_misc_ctrl(&ui4_misc_ctrl);
    if (i4_return != PCLR_OK)
    {
        DBG_ERROR(("{PCL} ERR: x_pcl_get_misc_ctrl failed\n\r"));
        return i4_return;
    }

    switch (ui4_misc_ctrl & ui4_bit_mask)
    {
        case PCL_MISC_CTRL_FAST_BOOT:
            *pe_boot_type = PCL_BOOT_TYPE_FAST_BOOT;
            break;

        case PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND:
            *pe_boot_type = PCL_BOOT_TYPE_NORMAL_BOOT_WITH_SUSPEND;
            break;

        case PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND:
            *pe_boot_type = PCL_BOOT_TYPE_NORMAL_BOOT_WO_SUSPEND;
            break;

        default:
            i4_return = PCLR_INV_ARG;
            break;
    }

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_boot_type
 *
 * Description: This API sets the system boot type.
 *
 * Inputs:  e_boot_type             References the boot type. Only one of 
 *                                  (PCL_MISC_CTRL_FAST_BOOT, PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND,
 *                                  PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND) can be set.
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_INV_ARG            One or more of the arguments contain
 *                                  invalid data.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_boot_type(PCL_BOOT_TYPE_T    e_boot_type)
{
    INT32                   i4_return;
    UINT32                  ui4_misc_ctrl;
    UINT32                  ui4_bit_mask = PCL_MISC_CTRL_FAST_BOOT 
                                           | PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND
                                           | PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND;

    DBG_API(("{PCL} x_pcl_set_boot_type\n\r"));

    i4_return = x_pcl_get_misc_ctrl(&ui4_misc_ctrl);
    if (i4_return != PCLR_OK)
    {
        DBG_ERROR(("{PCL} ERR: x_pcl_get_misc_ctrl failed\n\r"));
        return i4_return;
    }    

    /* reset fast boot mask */
    ui4_misc_ctrl &= ~ui4_bit_mask;
    
    switch (e_boot_type)
    {         
        case PCL_BOOT_TYPE_FAST_BOOT:
            ui4_misc_ctrl |= PCL_MISC_CTRL_FAST_BOOT;
            break;

        case PCL_BOOT_TYPE_NORMAL_BOOT_WITH_SUSPEND:
            ui4_misc_ctrl |= PCL_MISC_CTRL_NORMAL_BOOT_WITH_SUSPEND;
            break;

        case PCL_BOOT_TYPE_NORMAL_BOOT_WO_SUSPEND:
            ui4_misc_ctrl |= PCL_MISC_CTRL_NORMAL_BOOT_WITHOUT_SUSPEND;
            break;
            
        default:
            return PCLR_INV_ARG;
    }

    i4_return = x_pcl_set_misc_ctrl(ui4_misc_ctrl);
    if (i4_return != PCLR_OK)
    {
        DBG_ERROR(("{PCL} ERR: x_pcl_set_misc_ctrl failed\n\r"));
        return i4_return;
    }

    return i4_return;
}

/*-----------------------------------------------------------------------------
 * Name: x_pcl_set_power_suspend
 *
 * Description: This API sets the power suspend.
 *
 * Inputs:       
 *
 * Outputs: -
 *
 * Returns: PCLR_OK                 The routine was successful.
 *          PCLR_NOT_INIT           The Power Control Library is not
 *                                  initialized.
 *          PCLR_HW_NOT_SUPPORT     The hardware is not supported.
 *          PCLR_DEV_SET_FAILED     Device set operation is failed.
 ----------------------------------------------------------------------------*/
INT32 x_pcl_set_power_suspend(VOID)
{
#ifdef LINUX_TURNKEY_SOLUTION    
    INT32                   i4_return;
    PCL_BOOT_TYPE_T         e_boot_type;

    DBG_API(("{PCL} x_pcl_set_power_suspend \n\r"));

    /* Check arguments */
    if (!b_pcl_init)
    {
        return PCLR_NOT_INIT;
    }

    i4_return = x_pcl_get_boot_type(&e_boot_type);
    if (i4_return != PCLR_OK)
    {
        DBG_ERROR(("{PCL} ERR: x_pcl_get_boot_type failed\n\r"));
        return i4_return;
    }

    if (e_boot_type == PCL_BOOT_TYPE_NORMAL_BOOT_WO_SUSPEND)
    {
        /* do nothing */
        return PCLR_OK;
    }
    
    /* set suspend */
    #if 1
    /* tux-on-ice */
    system("sync");
    system("sync");
    system("sync");
    system("echo 1 > /proc/sys/vm/drop_caches");
    system("echo 3 > /proc/sys/vm/drop_caches");
    system("mkswap /dev/"SN_MTD_SUSPEND);
    system("swapon /dev/"SN_MTD_SUSPEND);
    system("echo swap:/dev/"SN_MTD_SUSPEND" > /sys/power/tuxonice/resume");
    system("echo 1 > /sys/power/tuxonice/reboot");
    system("echo 1 > /sys/power/tuxonice/no_pageset2");
    system("echo 0 > /sys/power/tuxonice/user_interface/enabled");
    system("echo 0 > /proc/sys/vm/drop_caches");
    system("echo > /sys/power/tuxonice/do_hibernate");
    #else
    /* software suspend */
    system("mkswap /dev/mtdblock11");
    system("swapon /dev/mtdblock11");
    system("echo reboot > /sys/power/disk");
    system("echo disk > /sys/power/state");
    #endif
#endif
    return PCLR_OK;
}
#endif /* MW_FAST_INIT */

