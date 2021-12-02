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
 * $RCSfile: iom_key_gen.c,v $ 
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/7 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: 0fe25a3cee92c773cf2ba8a3e9469fd8 $
 *
 * Description:
 *         This is I/O Manager key generator source file.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "os/drv/x_driver_os.h"
#include "dbg/x_dbg.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_irrc_btn_def.h"
#include "cli/x_cli.h"
#include "io_mngr/_iom.h"
#include "io_mngr/iom_lock.h"
#include "io_mngr/iom.h"
#include "io_mngr/iom_key_gen.h"
#include "io_mngr/x_iom.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static HANDLE_T         h_key_gen_timer;
static IOM_KEY_GEN_T    t_key_gen_state = IOM_KEY_GEN_STOP;
static UINT8            ui1_key_record_cnt;
static UINT8            ui1_key_gen_stop_idx;
static UINT32           ui4_key_num;
static KEY_GEN_T        at_key_buf[KEY_BUF_SIZE];
static UINT32           aui4_hot_key_buf[HOT_KEY_BUF_SIZE];
static BOOL             b_get_1st_tick = FALSE;
static UINT32           ui4_prv_tick;
static UINT32           ui4_tick_period;

#ifdef CLI_LVL_ALL
static UINT32           ui4_key_idx;
static BOOL             b_key_gen_enabled = FALSE;
#endif

/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
#ifdef CLI_LVL_ALL 
/*-----------------------------------------------------------------------------
 * Name: _iom_key_gen_timer_cb
 *
 * Description: This is timer callback for key generator.
 *
 * Inputs:  h_handle        A handle referencing the timer.
 *          pv_tag          Contains the handle private tag.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _iom_key_gen_timer_cb(HANDLE_T h_timer,
                                  VOID*    pv_tag)
{
    INP_EVT_T   t_inp_evt;
    HANDLE_T*   ph_iom_inp_msgq;

    ph_iom_inp_msgq = iom_op_get_in_msgq_hdl();

    t_inp_evt.e_evt_type = INP_EVT_TYPE_TIMER_KEY_GEN;
    t_inp_evt.pv_tag = NULL;
    t_inp_evt.e_cond = IOM_NFY_COND_NONE;
    
    x_msg_q_send(*ph_iom_inp_msgq,
                 &t_inp_evt,
                 sizeof(INP_EVT_T),
                 IOM_MSGQ_DEFAULT_PRIORITY);
}
#endif


/*-----------------------------------------------------------------------------
 * Name: _calculate_elasped_time
 *
 * Description: This API calculates the elasped time between two events.
 *
 * Inputs:  ui4_key_idx     Contians the key buffer index.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _calculate_elasped_time(UINT32 ui4_key_idx)
{
    UINT32  ui4_cur_tick;
    UINT32  ui4_tick_diff = 0;

    /* Get initial system tick */
    if (!b_get_1st_tick)
    {
        at_key_buf[ui4_key_idx].ui4_sec = 0;
        at_key_buf[ui4_key_idx].ui4_msec = 0;
        ui4_prv_tick = os_get_sys_tick();
        ui4_tick_period = x_os_drv_get_tick_period();
        b_get_1st_tick = TRUE;
        return;
    }

    ui4_cur_tick = os_get_sys_tick();
    if (ui4_cur_tick < ui4_prv_tick)
    {
        /* Take care of loop back */
        ui4_tick_diff = 0xFFFFFFFF - ui4_prv_tick + ui4_cur_tick + 1;
    }
    else
    {
        /* Calculate the elasped time */
        ui4_tick_diff = ui4_cur_tick - ui4_prv_tick;
    }

    at_key_buf[ui4_key_idx].ui4_sec = ui4_tick_diff * ui4_tick_period / 1000;
    at_key_buf[ui4_key_idx].ui4_msec = ui4_tick_diff * ui4_tick_period % 1000;

    if (at_key_buf[ui4_key_idx].ui4_sec == 0 && at_key_buf[ui4_key_idx].ui4_msec < 100)
    {
        at_key_buf[ui4_key_idx].ui4_msec = at_key_buf[ui4_key_idx].ui4_msec;
    }

    ui4_prv_tick = ui4_cur_tick;
}


/*-----------------------------------------------------------------------------
 * Name: iom_key_gen_get_status
 *
 * Description: This API gets the current status of key generator.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: TRUE        The key generator is enabled.
 *          FALSE       The key generator is disabled.
 ----------------------------------------------------------------------------*/
BOOL iom_key_gen_get_status(VOID)
{
#ifdef CLI_LVL_ALL
    return b_key_gen_enabled;
#else
    return FALSE;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: iom_key_gen_set_status
 *
 * Description: This API sets the status of key generator.
 *
 * Inputs:  b_enable        New status (on/off) of key generator.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_key_gen_set_status(BOOL b_enable)
{
#ifdef CLI_LVL_ALL    
    if (!b_enable)
    {
        t_key_gen_state = IOM_KEY_GEN_STOP;
        x_timer_delete(h_key_gen_timer);
    }

    b_key_gen_enabled = b_enable;
#endif    
}


/*-----------------------------------------------------------------------------
 * Name: iom_key_gen_signal
 *
 * Description: This API generates keys to applications according to the
 *              information in key buffer when triggered.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_key_gen_signal(VOID)
{
#ifdef CLI_LVL_ALL    
    DEV_INFO_T*         pt_dev;
    EVT_FILTR_T*        pt_evt_filtr;
    UINT64              ui8_key_mask;
    IOM_NFY_COND_T      e_nfy_cond;
    UINT32              ui4_key_to_signal;
    UINT8               ui1_evt_filtr_cnt;
    UINT32              ui4_delay;
    UINT32              ui4_i;
    UINT32              ui4_j;

    /* Check if key generator is in right state */
    if (t_key_gen_state != IOM_KEY_GEN_START)
    {
        return;
    }

    /* Get key info to signal */
    e_nfy_cond = at_key_buf[ui4_key_idx].e_nfy_cond;
    ui4_key_to_signal = at_key_buf[ui4_key_idx].ui4_key;
    ui4_key_idx = (ui4_key_idx + 1) % ui4_key_num;
    ui8_key_mask = IOM_EVT_GRP_TO_MASK(ui4_key_to_signal);
    pt_dev = iom_op_get_in_dev_info_struct();

    /* Dispatch events to applications having the right of receiving them */
    iom_lock_db_wr();

    for (ui4_i = 0; ui4_i < (UINT32)IOM_MAX_DEV; ui4_i++)
    {
        if (pt_dev[ui4_i].e_dev_type == IOM_DEV_TYPE_IN_IRRC)
        {
            pt_evt_filtr = DLIST_HEAD(&(pt_dev[ui4_i].t_evt_filtr_list));
            ui1_evt_filtr_cnt = pt_dev[ui4_i].ui1_evt_filtr_cnt;

            for (ui4_j = 0; ui4_j < ui1_evt_filtr_cnt; ui4_j++)
            {
                /* Dispatch the event */
                if (ui8_key_mask & pt_evt_filtr->u.s_inp.t_irrc_setting.ui8_evt_grp_mask)
                {
                    (pt_evt_filtr->pf_nfy)(pt_evt_filtr->pv_tag,
                                           e_nfy_cond,
                                           ui4_key_to_signal,
                                           0);

                    pt_evt_filtr->u.s_inp.ui4_evt_code = ui4_key_to_signal;
                    x_memset(&pt_evt_filtr->u.s_inp.t_data, 0, sizeof(IRRC_DATA_T));
                }

                pt_evt_filtr = DLIST_NEXT(pt_evt_filtr, t_link);
                
                /* Start timer to trigger next key signal */
                if (ui4_key_idx == 0)
                {
                    ui4_delay = DELAY_BETWEEN_TWO_KEY_LOOPS;
                }
                else
                {
                    if (at_key_buf[ui4_key_idx].ui4_sec == 0 &&
                        at_key_buf[ui4_key_idx].ui4_msec == 0)
                    {
                        ui4_delay = DEFAULT_ELASPED_TIME;
                    }
                    else
                    {
                        ui4_delay = at_key_buf[ui4_key_idx].ui4_sec * 1000 +
                                    at_key_buf[ui4_key_idx].ui4_msec;
                    }
                }

                if (x_timer_start(h_key_gen_timer,
                                  ui4_delay,
                                  X_TIMER_FLAG_ONCE,
                                  _iom_key_gen_timer_cb,
                                  NULL) != OSR_OK)
                {
                    DBG_ERROR(("{IOM} ERR: x_timer_start() failed!\n\r"));
                    dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_TIMER);
                }                
            }
        }
    }

    iom_unlock_db();
#endif    
}


/*-----------------------------------------------------------------------------
 * Name: iom_key_gen_record
 *
 * Description: This API records user-input keys and also maintains key
 *              generator state.
 *
 * Inputs:  pt_inp_evt      Points to input event structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_key_gen_record(INP_EVT_T* pt_inp_evt)
{
    UINT8       ui1_idx;
            
    /* Skip button-down event if not in IOM_KEY_GEN_RECORD state */
    if (pt_inp_evt->e_evt_type != INP_EVT_TYPE_IRRC)
    {
        return;
    }

    if ((t_key_gen_state != IOM_KEY_GEN_RECORD) &&
        (pt_inp_evt->e_cond == IOM_NFY_COND_BTN_DOWN))
    {
        return;
    }

    /* Perform corresponding operations */
    switch (t_key_gen_state)
    {
        case IOM_KEY_GEN_RECORD:
            /* Record input key into key buffer */
            _calculate_elasped_time(ui4_key_num);
            at_key_buf[ui4_key_num].ui4_key = pt_inp_evt->u.s_irrc.ui4_evt_code;
            at_key_buf[ui4_key_num].e_nfy_cond = pt_inp_evt->e_cond;
            ui4_key_num = (ui4_key_num + 1) % KEY_BUF_SIZE;
                        
            if (iom_key_gen_get_status()) /* Keygen */
            {                                 
                if (pt_inp_evt->u.s_irrc.ui4_evt_code == KEY_CODE_TO_STOP_RECORDING)
                {
                    /* Skip button-down event */
                    if (pt_inp_evt->e_cond == IOM_NFY_COND_BTN_DOWN)
                    {
                        return;
                    }
    
                    if (++ui1_key_record_cnt == KEY_RECORD_COUNT)
                    {
                        ui4_key_num -= (KEY_RECORD_COUNT*2);
                        ui1_key_record_cnt = 0;
                        ui1_key_gen_stop_idx = 0;                        
                                                    
                        if (ui4_key_num == 0)
                        {
                            x_dbg_stmt("\n\r{IOM} Key generator stopped!\n\r");
                            t_key_gen_state = IOM_KEY_GEN_STOP;                       
                        }
                        else
                        {
                            /* Stop key recording */
                            x_dbg_stmt("\n\r{IOM} ... Stop key recording, key generator started!\n\r");
                            t_key_gen_state = IOM_KEY_GEN_START;
    
                            /* Create timer for key generator */
                            if (x_timer_create(&h_key_gen_timer) != OSR_OK)
                            {
                                DBG_ERROR(("\n\r{IOM} ERR: x_timer_create() failed!\n\r"));
                                dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_TIMER);
                            }
    
                            iom_key_gen_signal();
                        }
                    }
                }
                else
                {
                    ui1_key_record_cnt = 0;
                }     
            }     
            else /* Hotkey */
            {                                        
                if ((ui4_key_num >> 1) >= HOT_KEY_BUF_SIZE)
                {
                    ui1_key_record_cnt = 0;
                    ui1_key_gen_stop_idx = 0; 
                    
                    x_dbg_stmt("... Hotkey record ends\n\r");
                    t_key_gen_state = IOM_KEY_GEN_STOP;
                    
                    /* Extract 4-digit hot keys */
                    if (ui4_key_num >= (HOT_KEY_BUF_SIZE*2))
                    {     
                        x_dbg_stmt("{IOM} Hotkeys = ");
                        for (ui1_idx = 0; ui1_idx < 4; ui1_idx++)
                        {
                            aui4_hot_key_buf[ui1_idx] = at_key_buf[ui1_idx*2].ui4_key;
                            if ((aui4_hot_key_buf[ui1_idx] >= BTN_DIGIT_0) && 
                                (aui4_hot_key_buf[ui1_idx] <= BTN_DIGIT_9))
                            {                                        
                                x_dbg_stmt("%d", (aui4_hot_key_buf[ui1_idx] - BTN_DIGIT_0));
                            }
                            else
                            {
                                x_dbg_stmt("x");
                            }                                                                                                                            
                        }
                        
                        x_dbg_stmt("\n\r");
                        x_cli_ui_send_hotkeys(aui4_hot_key_buf, HOT_KEY_BUF_SIZE);
                    }                                            
                } 
            }                                                                 
            break;

        case IOM_KEY_GEN_START:
            if (iom_key_gen_get_status()) /* Keygen */  
            {                          
                /* Disable key generator whenever KEY_CODE_TO_START_RECORDING and
                   KEY_CODE_TO_STOP_RECORDING IRRC events are received sequentially */
                if (pt_inp_evt->u.s_irrc.ui4_evt_code == KEY_CODE_TO_START_RECORDING)
                {
                    ui1_key_gen_stop_idx = 1;
                    break;
                }
    
                if ((pt_inp_evt->u.s_irrc.ui4_evt_code == KEY_CODE_TO_STOP_RECORDING) &&
                    (ui1_key_gen_stop_idx == 1))
                {
                    ui1_key_gen_stop_idx = 0;
                }
                else
                {
                    ui1_key_gen_stop_idx = 0;
                    break;
                }
    
                x_dbg_stmt("\n\r{IOM} Key generator stopped!\n\r");
                t_key_gen_state = IOM_KEY_GEN_STOP;
    
                /* Delete key generator timer */
                if (x_timer_delete(h_key_gen_timer) != OSR_OK)
                {
                    DBG_ERROR(("{IOM} ERR: x_timer_delete() failed!\n\r"));
                    dbg_abort(DBG_MOD_IO_MNGR | DBG_CAT_TIMER);
                }
            }
            break;

        case IOM_KEY_GEN_STOP:
            if (pt_inp_evt->u.s_irrc.ui4_evt_code == KEY_CODE_TO_START_RECORDING)
            {
                if (++ui1_key_record_cnt == KEY_RECORD_COUNT)
                {
                    /* Begin key recording */
                    if (iom_key_gen_get_status()) /* Keygen */  
                    {                     
                        x_dbg_stmt("\n\r{IOM} Start key recording ...\n\r");
                    }
                    else /* Hotkey */
                    {
                        x_dbg_stmt("\n\r{IOM} Hotkey record begins ...");
                    }                 
                                                   
                    t_key_gen_state = IOM_KEY_GEN_RECORD;
                    ui1_key_record_cnt = 0;
                    ui4_key_num = 0;
                    b_get_1st_tick = FALSE;                    
#ifdef CLI_LVL_ALL                      
                    ui4_key_idx = 0;
#endif                    
                }
            }
            else
            {
                /* Reset key count */
                ui1_key_record_cnt = 0;
            }
            break;

        default:
            break;
    }
}


/*-----------------------------------------------------------------------------
 * Name: iom_key_gen_show_log
 *
 * Description: This API shows the log information of recorded IRRC keys.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_key_gen_show_log(VOID)
{
#ifdef CLI_LVL_ALL    
    UINT32  ui4_i;

    if (ui4_key_num == 0)
    {
        x_dbg_stmt("{IOM} Log is empty!\n\r");
    }
    else
    {
        for (ui4_i = 0; ui4_i < ui4_key_num; ui4_i++)
        {
            x_dbg_stmt("{IOM} [%03d:%03d] 0x%04x%04x %s\n\r",
                       (UINT16)(at_key_buf[ui4_i].ui4_sec),
                       (UINT16)(at_key_buf[ui4_i].ui4_msec),
                       (UINT16)(at_key_buf[ui4_i].ui4_key >> 16),
                       (UINT16)(at_key_buf[ui4_i].ui4_key),
                       at_key_buf[ui4_i].e_nfy_cond == IOM_NFY_COND_BTN_UP ? "BTN UP" : "BTN DN");
        }
    }
#endif    
}


/*-----------------------------------------------------------------------------
 * Name: iom_key_gen_clear_log
 *
 * Description: This API clears the log information of recorded IRRC keys.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID iom_key_gen_clear_log(VOID)
{
#ifdef CLI_LVL_ALL    
    ui4_key_num = 0;
    ui4_key_idx = 0;
    b_get_1st_tick = FALSE;
#endif    
}

