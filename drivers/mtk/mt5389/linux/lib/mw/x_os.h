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
 * $RCSfile: x_os.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains OS function prototypes, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_OS_H_
#define _X_OS_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_os.h"
#include "x_driver_os.h"
#include <linux/irqflags.h>
#include <linux/string.h>
#include <linux/kernel.h>


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#if defined(__KERNEL__)
// HAL_CRIT_FORCE_FUNCTION:
// Force using function base implementation for HalCriticalStart/HalCriticalEnd.
// This is for debug/time benchmark feature.
//
// HAL_CRIT_FORCE_INLINE_IRQ_RESTORE: 
// Force to inline local_irq_restore() in SMP. This can make oprofile report
// more precisely. Turn on by default for NDEBUG. Must not use together with
// HAL_CRIT_FORCE_FUNCTION.
//
// Remember, you must change both vm_linux/chiling/driver/linux/lib/mw/x_os.h
// and vm_linux/project_x/middleware/os/inc/x_os.h
//#define HAL_CRIT_FORCE_FUNCTION
//#define HAL_CRIT_FORCE_INLINE_IRQ_RESTORE

#ifdef CC_UBOOT
#define HAL_CRIT_FORCE_FUNCTION
#endif

#ifdef CONFIG_SMP
#if !defined(HAL_CRIT_FORCE_FUNCTION) && !defined(NDEBUG)
#define HAL_CRIT_FORCE_INLINE_IRQ_RESTORE
#else
#define HAL_CRIT_FORCE_FUNCTION
#endif

#ifdef HAL_CRIT_FORCE_INLINE_IRQ_RESTORE
#define HAL_CRIT_NEED_DO_CRITICAL_END
extern UINT32 HalCriticalStart(void);
extern BOOL DoHalCriticalEnd(void);

static inline void HalCriticalEnd(UINT32 u4Flags)
{
    if (DoHalCriticalEnd())
    {
        unsigned long flags = (unsigned long)u4Flags;
        local_irq_restore(flags);
    }
}
#endif
#else // CONFIG_SMP
#ifndef HAL_CRIT_FORCE_FUNCTION
// inlined version for non-smp
static inline UINT32 HalCriticalStart(void)
{
    unsigned long flags;
    local_irq_save(flags);
    return (UINT32)flags;
}

static inline void HalCriticalEnd(UINT32 u4Flags)
{
    unsigned long flags = (unsigned long)u4Flags;
    local_irq_restore(flags);
}
#endif
#endif /* CONFIG_SMP */

#ifdef HAL_CRIT_FORCE_FUNCTION
// For nucleus, uboot, SMP NON-DEBUG or uP DEBUG.
extern UINT32 HalCriticalStart(void);
extern void HalCriticalEnd(UINT32 u4Flags);
#endif

#define x_crit_start()     (CRIT_STATE_T)HalCriticalStart()
#define x_crit_end(old)    HalCriticalEnd((UINT32)old)
#else
#error Should not be used by non-kernel
#endif


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* MsgQ API's */
extern INT32 x_msg_q_create (HANDLE_T*    ph_msg_q,
                             const CHAR*  ps_name,
                             SIZE_T       z_msg_size,
                             UINT16       ui2_msg_count);

extern INT32 x_msg_q_attach (HANDLE_T*    ph_msg_q,
                             const CHAR*  ps_name);

extern INT32 x_msg_q_delete (HANDLE_T  h_msg_q);

extern INT32 x_msg_q_send (HANDLE_T     h_msg_q,
                           const VOID*  pv_msg,
                           SIZE_T       z_size,
                           UINT8        ui1_priority);

extern INT32 x_msg_q_receive (UINT16*        pui2_index,
                              VOID*          pv_msg,
                              SIZE_T*        pz_size,
                              HANDLE_T*      ph_msg_q_mon_list,
                              UINT16         ui2_msg_q_mon_count,
                              MSGQ_OPTION_T  e_options);

extern INT32 x_msg_q_receive_timeout (UINT16*    pui2_index,
                                      VOID*      pv_msg,
                                      SIZE_T*    pz_size,
                                      HANDLE_T*  ph_msg_q_mon_list,
                                      UINT16     ui2_msg_q_mon_count,
                                      UINT32     ui4_time);

extern INT32 x_msg_q_num_msgs (HANDLE_T  h_msg_q,
                               UINT16*   pui2_num_msgs);

extern INT32 x_msg_q_get_max_msg_size (HANDLE_T  h_msg_q,
                                       SIZE_T*   pz_maxsize);

extern INT32 x_msg_q_health_check (const CHAR*  ps_name,
                                   BOOL         b_enable);

extern INT32 x_msg_q_health_check_start (VOID);

/* Event_group API's */
extern INT32 x_ev_group_create (HANDLE_T         *ph_hdl,
                                const CHAR       *ps_name,
                                EV_GRP_EVENT_T   e_init_events);

extern INT32 x_ev_group_attach (HANDLE_T     *ph_hdl,
                                const CHAR   *ps_name);

extern INT32 x_ev_group_delete (HANDLE_T  h_hdl);

extern INT32 x_ev_group_set_event (HANDLE_T             h_hdl,
                                   EV_GRP_EVENT_T       e_events,
                                   EV_GRP_OPERATION_T   e_operation);

extern INT32 x_ev_group_wait_event (HANDLE_T            h_hdl,
                                    EV_GRP_EVENT_T      e_events_req,
                                    EV_GRP_EVENT_T      *pe_events_got,
                                    EV_GRP_OPERATION_T  e_operation);

extern INT32 x_ev_group_wait_event_timeout(HANDLE_T           h_hdl,
                                           EV_GRP_EVENT_T     e_events_req,
                                           EV_GRP_EVENT_T     *pe_events_got,
                                           EV_GRP_OPERATION_T e_operation,
                                           UINT32             ui4_time);

extern INT32 x_ev_group_get_info (HANDLE_T          h_hdl,
                                  EV_GRP_EVENT_T    *pe_cur_events,
                                  UINT8             *pui1_num_thread_waiting,
                                  CHAR              *ps_ev_group_name,
                                  CHAR              *ps_first_wait_thread);


/* Thread API's */
extern INT32 x_thread_create (HANDLE_T*             ph_th_hdl,
                              const CHAR*           ps_name,
                              SIZE_T                z_stack_size,
                              UINT8                 ui1_priority,
                              x_os_thread_main_fct  pf_main_rtn,
                              SIZE_T                z_arg_size,
                              VOID*                 pv_arg);

extern VOID x_thread_exit (VOID);

extern INT32 x_thread_delete (HANDLE_T  h_th_hdl);

extern VOID x_thread_delay (UINT32  ui4_delay);

extern INT32 x_thread_set_pri (HANDLE_T  h_th_hdl,
                               UINT8     ui1_new_pri);

extern INT32 x_thread_get_pri (HANDLE_T  h_th_hdl,
                               UINT8*    pui1_pri);

/*----------------------------------------------------------------------------
 * Function: x_thread_set_affinity()
 *
 * Description:
 *      this API sets a thread's affinity mask
 *      the thread will only run on specified cpu.
 *
 * Inputs:
 *      h_th_hdl: thread handle returned via x_thread_create().
 *      cpu_mask: new mask specify CPU to run
 *
 * Outputs: -
 *
 * NOTES:
 *      This function is only avaliable in Linux kernel mode.
 *      Empty function in Nucleus.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : thread module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_FAIL: OS driver related errors.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
extern INT32 x_thread_set_affinity (HANDLE_T  h_th_hdl,
                                    UINT32    cpu_mask);

extern VOID x_thread_suspend (VOID);

extern INT32 x_thread_resume (HANDLE_T  h_th_hdl);

extern INT32 x_thread_self (HANDLE_T*  ph_th_hdl);

extern INT32 x_thread_stack_stats (HANDLE_T  h_th_hdl,
                                   SIZE_T*   pz_alloc_stack,
                                   SIZE_T*   pz_max_used_stack);

extern INT32 x_thread_set_pvt (UINT32                   ui4_key,
                               x_os_thread_pvt_del_fct  pf_pvt_del,
                               VOID*                    pv_pvt);
extern INT32 x_thread_get_pvt (UINT32  ui4_key,
                               VOID**  ppv_pvt);
extern INT32 x_thread_del_pvt (UINT32  ui4_key);

/* Semaphhore API's */
extern INT32 x_sema_create (HANDLE_T*    ph_sema_hdl,
                            SEMA_TYPE_T  e_types,
                            UINT32       ui4_init_value);

extern INT32 x_sema_delete (HANDLE_T  h_sema_hdl);

extern INT32 x_sema_force_delete (HANDLE_T  h_sema_hdl);

extern INT32 x_sema_lock (HANDLE_T       h_sema_hdl,
                          SEMA_OPTION_T  e_options);

extern INT32 x_sema_lock_timeout (HANDLE_T  h_sema_hdl,
                                  UINT32    ui4_time);

extern INT32 x_sema_unlock (HANDLE_T  h_sema_hdl);


/* ISR API's */
extern INT32 x_reg_isr (UINT16         ui2_vector_id,
                        x_os_isr_fct   pf_isr,
                        x_os_isr_fct*  ppf_old_isr);


/* Timer API's */
extern INT32 x_timer_create (HANDLE_T*  ph_timer);

extern INT32 x_timer_start (HANDLE_T           h_timer,
                            UINT32             ui4_delay,
                            TIMER_FLAG_T       e_flags,
                            x_os_timer_cb_fct  pf_callback,
                            VOID*              pv_tag);

extern INT32 x_timer_stop (HANDLE_T  h_timer);

extern INT32 x_timer_delete (HANDLE_T  h_timer);

extern INT32 x_timer_resume (HANDLE_T  h_timer);

/* Memory API's */
extern VOID* x_mem_alloc (SIZE_T  z_size);
extern VOID* x_mem_alloc_virtual (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*  pv_mem_block,
                            SIZE_T z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);

extern INT32 x_mem_part_create (HANDLE_T*    ph_part_hdl,
                                const CHAR*  ps_name,
                                VOID*        pv_addr,
                                SIZE_T       z_size,
                                SIZE_T       z_alloc_size);

extern INT32 x_mem_part_delete (HANDLE_T);

extern INT32 x_mem_part_attach (HANDLE_T*    ph_part_hdl,
                                const CHAR*  ps_name);

extern VOID* x_mem_part_alloc (HANDLE_T  h_part_hdl,
                               SIZE_T    z_size);

extern VOID* x_mem_part_calloc (HANDLE_T  h_part_hdl,
                                UINT32    ui4_num_element,
                                SIZE_T    z_size_element);

extern VOID* x_mem_part_realloc (HANDLE_T     h_part_hdl,
                                 VOID*        pv_mem_block,
                                 SIZE_T       z_new_size);


/* Utility API's */
#define x_memcpy            memcpy
/*
extern VOID* x_memcpy (VOID*        pv_to,
                       const VOID*  pv_from,
                       SIZE_T       z_len);
*/

#define x_memcmp            memcmp
/*
extern INT32 x_memcmp (const VOID*  pv_mem1,
                       const VOID*  pv_mem2,
                       SIZE_T       z_len);
*/

#define x_memmove           memmove
/*
extern VOID* x_memmove (VOID*        pv_to,
                        const VOID*  pv_from,
                        SIZE_T       z_len);
*/

#define x_memset            memset
/*
extern VOID* x_memset (VOID*   pui1_mem,
                       UINT8   ui1_c,
                       SIZE_T  z_len);
*/
extern VOID* x_memchr (const VOID*  pv_mem,
                       UINT8        ui1_c,
                       SIZE_T       z_len);

extern VOID* x_memrchr (const VOID*  pv_mem,
                        UINT8        ui1_c,
                        SIZE_T       z_len);


#define x_strdup            strdup
/*
extern CHAR* x_strdup (const CHAR*  ps_str);
*/

#define x_strcpy            strcpy
/*
extern CHAR* x_strcpy (CHAR*        ps_to,
                       const CHAR*  ps_from);
*/

#define x_strncpy           strncpy
/*
extern CHAR* x_strncpy (CHAR*        ps_to,
                        const CHAR*  ps_from,
                        SIZE_T       z_len);
*/

#define x_strcmp            strcmp
/*
extern INT32 x_strcmp (const CHAR*  ps_str1,
                       const CHAR*  ps_str2);
*/

#define x_strncmp           strncmp
/*
extern INT32 x_strncmp (const CHAR*  ps_str1,
                        const CHAR*  ps_str2,
                        SIZE_T       z_len);
*/


extern INT32 x_strcasecmp (const CHAR*  ps_str1,
                           const CHAR*  ps_str2);



extern INT32 x_strncasecmp (const CHAR*  ps_str1,
                            const CHAR*  ps_str2,
                            SIZE_T       z_len);

#define x_strcat            strcat
/*
extern CHAR* x_strcat (CHAR*        ps_to,
                       const CHAR*  ps_append);
*/

#define x_strncat           strncat
/*
extern CHAR* x_strncat (CHAR*        ps_to,
                        const CHAR*  ps_append,
                        SIZE_T       z_len);
*/

#define x_strchr            strchr
/*
extern CHAR* x_strchr (const CHAR*  ps_str,
                       CHAR         c_char);
*/

#define x_strrchr           strrchr
/*
extern CHAR* x_strrchr (const CHAR*  ps_str,
                        CHAR         c_char);
*/

#define x_strstr            strstr
/*
extern CHAR* x_strstr (const CHAR*  ps_str,
                       const CHAR*  ps_find);
*/

#define x_strtoull          strtoull
/*
extern UINT64 x_strtoull (const CHAR*  pc_beg_ptr,
                          CHAR**       ppc_end_ptr,
                          UINT8        ui1_base);
*/

#define x_strtoll           strtoll
/*
extern INT64 x_strtoll (const CHAR*  pc_beg_ptr,
                        CHAR**       ppc_end_ptr,
                        UINT8        ui1_base);
*/

//#define x_strlen            strlen
extern SIZE_T x_strlen (const CHAR*  ps_str);

#define x_strspn            strspn
/*
extern SIZE_T x_strspn (const CHAR*  ps_str,
                        const CHAR*  ps_accept);
*/

#define x_strcspn           strcspn
/*
extern SIZE_T x_strcspn (const CHAR*  ps_str,
                         const CHAR*  ps_reject);
*/

extern CHAR* x_str_toupper (CHAR*  ps_str);

extern CHAR* x_str_tolower (CHAR*  ps_str);

extern CHAR x_toupper (CHAR  c_char);

extern CHAR x_tolower (CHAR  c_char);

extern CHAR* x_strtok (CHAR*        ps_str,
                       const CHAR*  ps_delimiters,
                       CHAR**       pps_str,
                       SIZE_T*      pz_token_len);

#define x_sprintf           sprintf
/*
extern INT32 x_sprintf (CHAR*        ps_str,
                        const CHAR*  ps_format,
                        ...);
*/

#define x_vsprintf          vsprintf
/*
extern INT32 x_vsprintf (CHAR*        ps_str,
                         const CHAR*  ps_format,
                         VA_LIST      va_list);
*/
/*
extern INT32 x_snprintf (CHAR*        ps_str,
                         SIZE_T       z_size,
                         const CHAR*  ps_format,
                         ...);
*/
#define x_snprintf          snprintf

/*
extern INT32 x_vsnprintf (CHAR*        ps_str,
                          SIZE_T       z_size,
                          const CHAR*  ps_format,
                          VA_LIST      va_list);
*/
#define x_vsnprintf         vsnprintf

extern INT32 x_sscanf    (const CHAR *ps_buf,
                          const CHAR *ps_fmt,
                          ...);

extern INT32 x_vsscanf (const CHAR*        ps_buf,
                        const CHAR*  ps_fmt,
                        VA_LIST      t_ap);

/* DMA Memory API's */
extern VOID* x_memcpy_dma (VOID*        pv_to,
                           const VOID*  pv_from,
                           SIZE_T       z_len);

/*
  System clock tick function.
*/
extern UINT32 x_os_get_sys_tick (VOID) ;

extern UINT32 x_os_get_sys_tick_period (VOID);

#endif /* _X_OS_H_ */
