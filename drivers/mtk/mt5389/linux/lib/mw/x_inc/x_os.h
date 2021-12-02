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

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)  && !defined(__KERNEL__)
#include <stdlib.h>
#endif

#if defined(__KERNEL__)
#include <linux/string.h>
#ifndef CC_UBOOT
#include <linux/irqflags.h>
#include <linux/kernel.h>
#endif
#endif

/*!
 * @defgroup    groupMW_OSAI OS API
 * @ingroup     groupMW
 * @brief    This header file declares OSAI exported APIs.
 * 
 * @see groupMW
 *
 * @{
 * */

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
#define x_crit_start  x_os_drv_crit_start
#define x_crit_end    x_os_drv_crit_end
#endif /* __KERNEL__ */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
//extern VOID* x_mem_alloc_nocache (SIZE_T z_size);
#endif

#if !defined(__KERNEL__)
#define x_mem_alloc_virtual x_mem_alloc
#else
extern VOID* x_mem_alloc_virtual (SIZE_T  z_size);
#endif

#ifndef SYS_MEM_CHK
#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION) && !defined(__KERNEL__)
#ifndef MEM_WITH_OSAI_HDR
#define x_mem_alloc(z_size) (((z_size) > 0) ? malloc((z_size)) : malloc(0))

#define x_mem_calloc(ui4_num_element, z_size_element) ((((ui4_num_element)*(z_size_element)) > 0) ? calloc((ui4_num_element), (z_size_element)) : calloc(1, (z_size_element)))

#define x_mem_realloc(pv_mem_block, z_new_size) (((pv_mem_block) != NULL) ? realloc((pv_mem_block), (z_new_size)) : malloc((z_new_size)))
#else
extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*        pv_mem_block,
                            SIZE_T       z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);
#endif
#endif
#endif

/* MsgQ API's */
#ifdef SYS_MEM_CHK
#ifdef CC_MTK_LOADER
extern INT32 x_msg_q_create (HANDLE_T*    ph_msg_q,
                             const CHAR*  ps_name,
                             SIZE_T       z_msg_size,
                             UINT16       ui2_msg_count);

extern INT32 x_msg_q_delete (HANDLE_T  h_msg_q);
#else
extern VOID x_msg_q_dbg_log_start(VOID);

extern VOID x_msg_q_dbg_log_stop(VOID);

extern VOID x_msg_q_dbg_log_dump(VOID);

extern INT32 x_msg_q_dbg_create (HANDLE_T*   ph_msg_q,
                                 const CHAR* ps_name,
                                 SIZE_T      z_msg_size,
                                 UINT16      ui2_msg_count,
                                 const CHAR* s_file, 
                                 UINT32      ui4_line);

extern INT32 x_msg_q_dbg_delete (HANDLE_T    h_msg_q,
                                 const CHAR* s_file, 
                                 UINT32      ui4_line);

#define x_msg_q_create(ph_msg_q, ps_name, z_msg_size, ui2_msg_count)        \
    x_msg_q_dbg_create(ph_msg_q, ps_name, z_msg_size, ui2_msg_count, __FILE__, __LINE__)

#define x_msg_q_delete(h_msg_q)     \
    x_msg_q_dbg_delete(h_msg_q, __FILE__, __LINE__)
#endif
#else
/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. allocate memory for msgq struct and for the specified number of
 *             messages, each with the specified size.
 *          4. link the message buffers.
 *          5. check existence of the msg Q (based on msg Q name) and enqueue
 *             the msgq struct in the global msgq list.
 *          6. initialize, allocate, and link a messaage haandle.
 *
 *  @param  [in] ps_name - name of this msg Q, null ('\0') terminated, up to 16
 *                         characters excluding '\0' will be used.
 *  @param  [in] z_msg_size - this msg Q accepts a message up to this size of bytes.
 *  @param  [in] ui2_msg_count - this msg Q accespts this many messaages.
 *  @param  [out] ph_msg_hdl - pointer to hold returned msg Q handle.
 *                             the handle is valid only if this API returns OSR_OK.
 *
 *  
 *  @see    x_msg_q_create
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                       
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.    
 *  @retval OSR_NO_RESOURCE     - no resources (memory).                
 *  @retval OSR_FAIL            - OS driver related errors.                    
 *  @retval OSR_EXIST           - msg Q exists (ps_name collision).           
 *  @retval OSR_INV_HANDLE      - invalid handle.                        
 *  @retval OSR_OUT_OF_HANDLES  - no handle.                         
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_create (HANDLE_T*    ph_msg_q,
                             const CHAR*  ps_name,
                             SIZE_T       z_msg_size,
                             UINT16       ui2_msg_count);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. check validity of msg Q handle.
 *          3. mark internaal close flag and call handle_free().
 *          4. remove the msgq struct from the global msgq link list.
 *
 *  @param  [in] h_msg_hdl - msg Q handle returned via c_msg_q_create() or
 *                           c_msg_q_attach().
 *  
 *  @see    x_msg_q_delete
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                       
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_FAIL            - OS driver related errors.    
 *  @retval OSR_INV_HANDLE      - invalid handle.    
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_delete (HANDLE_T  h_msg_q);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. check existence of the msg Q (based on msg Q name).
 *          4. initialize, allocate, and link a messaage haandle.
 *
 *  @param  [in] ps_name - name of this msg Q, null ('\0') terminated, up to 16
 *                         characters excluding '\0' will be used.
 *  @param  [out] ph_msg_hdl - pointer to hold returned msg Q handle.
 *                             the handle is valid only if this API returns OSR_OK.
 *
 *  
 *  @see    x_msg_q_attach
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                       
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.  
 *  @retval OSR_FAIL            - OS driver related errors.                    
 *  @retval OSR_NOT_EXIST       - msg Q not exist.           
 *  @retval OSR_INV_HANDLE      - invalid handle.                        
 *  @retval OSR_OUT_OF_HANDLES  - no handle.                         
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_attach (HANDLE_T*    ph_msg_q,
                             const CHAR*  ps_name);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. allocate memory for msg Q struct and the specified number of
 *              messages, each with the specified size.
 *          4. link the message buffers.
 *          5. initialize, allocate, and link a messaage haandle.
 *
 *  @param  [in] h_msg_hdl - msg Q handle returned via c_msg_q_create() or
 *                           c_msg_q_attach().
 *  @param  [in] pv_msg - pointer to a memory holding the message to be sent.
 *  @param  [in] z_size - size in bytes of "pv_msg".
 *  @param  [in] ui1_pri - priority of this sending message, 0 (highest) - 255 (lowest).
 *
 *  @see    x_msg_q_send
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_TOO_BIG         - the sending message is bigger than allowed, specified
 *                                in c_msg_q_create().
 *  @retval OSR_TOO_MANY        - too many messages queued in the msg Q, more that allowed,
 *                                specified in c_msg_q_create().
 *  @retval OSR_INV_HANDLE: an error in handle operation.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_send (HANDLE_T     h_msg_q,
                           const VOID*  pv_msg,
                           SIZE_T       z_size,
                           UINT8        ui1_priority);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. allocate memory for msg Q struct and the specified number of
 *              messages, each with the specified size.
 *          4. link the message buffers.
 *          5. initialize, allocate, and link a messaage haandle to the front of queue.
 *
 *  @param  [in] h_msg_hdl - msg Q handle returned via c_msg_q_create() or
 *                           c_msg_q_attach().
 *  @param  [in] pv_msg - pointer to a memory holding the message to be sent.
 *  @param  [in] z_size - size in bytes of "pv_msg".
 *  @param  [in] ui1_pri - priority of this sending message, 0 (highest) - 255 (lowest).
 *
 *  @see    x_msg_q_send_to_front
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_TOO_BIG         - the sending message is bigger than allowed, specified
 *                                in c_msg_q_create().
 *  @retval OSR_TOO_MANY        - too many messages queued in the msg Q, more that allowed,
 *                                specified in c_msg_q_create().
 *  @retval OSR_INV_HANDLE: an error in handle operation.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_send_to_front (HANDLE_T     h_msg_q,
                                    const VOID*  pv_msg,
                                    SIZE_T       z_size);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. allocate memory for polling in case of no message available.
 *          4. check msg Q handles' validity in "ph_msgq_hdl_list".
 *          5. call msg_q_get_msg() to go through each msg Q to get a message.
 *
 *  @param  [in] pv_msg - pointer to memory buffer.
 *  @param  [in] z_size - size in bytes of "pv_msg".
 *  @param  [in] ph_msgq_hdl_list - array of message handles.
 *  @param  [in] ui2_msgq_hdl_count - array entry count.
 *  @param  [in] e_option - either X_MSGQ_OPTION_WAIT or X_MSGQ_OPTION_NOWAIT.
 *  @param  [out] pui2_index - index to array "ph_msgq_hdl_list", indicating which
 *                   msg Q handle returns a message or has an error.
 *  @param  [out] pv_msg - pointer to a memory holding the received message.
 *  @param  [out] z_size - size in bytes of the received message in "pv_msg".
 *
 *  @see    x_msg_q_receive
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_NO_RESOURCE     - no resources (memory).
 *  @retval OSR_FAIL            - OS driver related errors.
 *  @retval OSR_NO_MSG          - no message to receive.
 *  @retval OSR_BUSY            - already a receiver waiting for a message.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_receive (UINT16*        pui2_index,
                              VOID*          pv_msg,
                              SIZE_T*        pz_size,
                              HANDLE_T*      ph_msg_q_mon_list,
                              UINT16         ui2_msg_q_mon_count,
                              MSGQ_OPTION_T  e_options);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. allocate memory for polling in case of no message available.
 *          4. check msg Q handles' validity in "ph_msgq_hdl_list".
 *          5. call msg_q_get_msg() to go through each msg Q to get a message.
 *
 *  @param  [in] pv_msg - pointer to memory buffer.
 *  @param  [in] z_size - size in bytes of "pv_msg".
 *  @param  [in] ph_msgq_hdl_list - array of message handles.
 *  @param  [in] ui2_msgq_hdl_count - array entry count.
 *  @param  [in] ui4_time - time in milliseconds before returning OSR_TIMEOUT
 *  @param  [out] pui2_index - index to array "ph_msgq_hdl_list", indicating which
 *                   msg Q handle returns a message or has an error.
 *  @param  [out] pv_msg - pointer to a memory holding the received message.
 *  @param  [out] z_size - size in bytes of the received message in "pv_msg".
 *
 *  @see    x_msg_q_receive_timeout
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_NO_RESOURCE     - no resources (memory).
 *  @retval OSR_FAIL            - OS driver related errors.
 *  @retval OSR_TIMEOUT         - receive timed out.
 *  @retval OSR_BUSY            - already a receiver waiting for a message.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_receive_timeout (UINT16*    pui2_index,
                                      VOID*      pv_msg,
                                      SIZE_T*    pz_size,
                                      HANDLE_T*  ph_msg_q_mon_list,
                                      UINT16     ui2_msg_q_mon_count,
                                      UINT32     ui4_time);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. return message count.
 *
 *  @param  [in] h_msg_hdl - msg Q handle returned via c_msg_q_create() or
 *                           c_msg_q_attach().
 *  @param  [out] pui2_num_msgs - pointer which contains message counts.
 *
 *  @see    x_msg_q_num_msgs
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_num_msgs (HANDLE_T  h_msg_q,
                               UINT16*   pui2_num_msgs);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a msg Q. it performs
 *          1. check if msg Q mudule is initiated. 
 *          2. parse argumets.
 *          3. return max. message size.
 *
 *  @param  [in] h_msg_hdl - msg Q handle returned via c_msg_q_create() or
 *                           c_msg_q_attach().
 *  @param  [out] pz_maxsize - pointer which contains max. msg size.
 *
 *  @see    x_msg_q_get_max_msg_size
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - message Q module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_msg_q_get_max_msg_size (HANDLE_T  h_msg_q,
                                       SIZE_T*   pz_maxsize);

extern INT32 x_msg_q_health_check (const CHAR*  ps_name,
                                   BOOL         b_enable);

extern INT32 x_msg_q_health_check_start (VOID);

/* Event_group API's */
/*---------------------------------------------------------------------------*/
/*! @brief  API creates an event group and sets its current events according
 *
 *  @param  [in] ps_name - name of an event group, 16 bytes excluding '\0'
 *  @param  [in] e_init_events - nitial current events of an event group
 *
 *  @param  [out] ph_hdl - handle references the created event group
 *
 *  @see    x_ev_group_create
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_EXIST           - event group exists.
 *  @retval OSR_NO_RESOURCE     - out of resource such as memory, etc.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_create (HANDLE_T         *ph_hdl,
                                const CHAR       *ps_name,
                                EV_GRP_EVENT_T   e_init_events);

/*---------------------------------------------------------------------------*/
/*! @brief  API attaches the calling thread to an event group
 *
 *  @param  [in] ps_name - name of an event group, 16 bytes excluding '\0'
 *
 *  @param  [out] ph_hdl - handle references the created event group
 *
 *  @see    x_ev_group_attach
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_EXIST           - event group exists.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_attach (HANDLE_T     *ph_hdl,
                                const CHAR   *ps_name);

/*---------------------------------------------------------------------------*/
/*! @brief  API deletes an event group
 *
 *  @param  [in] h_hdl - handle of an event group to memory buffer.
 *
 *  @see    x_ev_group_delete
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_delete (HANDLE_T  h_hdl);

/*---------------------------------------------------------------------------*/
/*! @brief  API requests an events to be received (satisfied) from an event group.
 *
 *  @param  [in] h_hdl - handle of an event group to memory buffer.
 *  @param  [in] e_events - events to be set into current events of an event group
 *  @param  [in] e_op - peration over "e_events".
 *                      one of X_EV_OP_CLEAR, X_EV_OP_AND, X_EV_OP_OR
 *
 *  @see    x_ev_group_set_event
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_DELETED         - event_group was closed.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_set_event (HANDLE_T             h_hdl,
                                   EV_GRP_EVENT_T       e_events,
                                   EV_GRP_OPERATION_T   e_operation);

/*---------------------------------------------------------------------------*/
/*! @brief  API requests an events to be received (satisfied) from an event group.
 *
 *  @param  [in] h_hdl - handle of an event group to memory buffer.
 *  @param  [in] e_events - events to be set into current events of an event group
 *  @param  [in] e_op - operation over "e_events_req".
 *                      one of X_EV_OP_AND, X_EV_OP_AND_CONSUME, X_EV_OP_OR,
 *                      X_EV_OP_OR_CONSUME
 *
 *  @param  [out] pe_events_got - actual events received
 *
 *  @see    x_ev_group_wait_event
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_DELETED         - event_group was closed.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_wait_event (HANDLE_T            h_hdl,
                                    EV_GRP_EVENT_T      e_events_req,
                                    EV_GRP_EVENT_T      *pe_events_got,
                                    EV_GRP_OPERATION_T  e_operation);

/*---------------------------------------------------------------------------*/
/*! @brief  API requests an events to be received (satisfied) from an event group.
 *
 *  @param  [in] h_hdl - handle of an event group to memory buffer.
 *  @param  [in] e_events - events to be set into current events of an event group
 *  @param  [in] e_op - operation over "e_events_req".
 *                      one of X_EV_OP_AND, X_EV_OP_AND_CONSUME, X_EV_OP_OR,
 *                      X_EV_OP_OR_CONSUME
 *  @param  [in] ui4_time - time delay in milliseconds before bailing out
 *
 *  @param  [out] pe_events_got - actual events received
 *
 *  @see    x_ev_group_wait_event
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_DELETED         - event_group was closed.
 *  @retval OSR_TIMEOUT         - bail out, time delay has passed.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_wait_event_timeout(HANDLE_T           h_hdl,
                                           EV_GRP_EVENT_T     e_events_req,
                                           EV_GRP_EVENT_T     *pe_events_got,
                                           EV_GRP_OPERATION_T e_operation,
                                           UINT32             ui4_time);

/*---------------------------------------------------------------------------*/
/*! @brief  API requests an events to be received (satisfied) from an event group.
 *
 *  @param  [in] h_hdl - handle of an event group to memory buffer.
 *
 *  @param  [out] pe_events_got - actual events received
 *  @param  [out] pe_cur_events - current events in an event group
 *  @param  [out] pui1_num_thread_waiting - no. of threads waiting to receive events
 *                                          with an event group
 *  @param  [out] ps_ev_group_name - name of an event group, 16 bytes excluding '\0'
 *  @param  [out] ps_first_wait_thread - name of the first thread waiting,
 *                                       16 bytes excluding '\0'
 *
 *  @see    x_ev_group_get_info
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - event group has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 *  @retval OSR_DELETED         - event_group was closed.
 *
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_ev_group_get_info (HANDLE_T          h_hdl,
                                  EV_GRP_EVENT_T    *pe_cur_events,
                                  UINT8             *pui1_num_thread_waiting,
                                  CHAR              *ps_ev_group_name,
                                  CHAR              *ps_first_wait_thread);


/* Thread API's */
#ifdef SYS_MEM_CHK
#ifdef CC_MTK_LOADER
extern INT32 x_thread_create (HANDLE_T*             ph_th_hdl,
                              const CHAR*           ps_name,
                              SIZE_T                z_stack_size,
                              UINT8                 ui1_priority,
                              x_os_thread_main_fct  pf_main_rtn,
                              SIZE_T                z_arg_size,
                              VOID*                 pv_arg);

extern VOID x_thread_exit (VOID);
#else
extern VOID x_thread_dbg_log_start(VOID);

extern VOID x_thread_dbg_log_stop(VOID);

extern VOID x_thread_dbg_file_log_dump(VOID);

extern INT32 x_thread_dbg_create(HANDLE_T    *ph_th_hdl,
                                 const CHAR  *ps_name,
                                 SIZE_T       z_stacksize,
                                 UINT8        ui1_pri,
                                 x_os_thread_main_fct pf_main,
                                 SIZE_T       z_arg_size,
                                 VOID         *pv_arg,
                                 const CHAR*  s_file, 
                                 UINT32       ui4_line);

extern VOID x_thread_dbg_exit(const CHAR* s_file, 
                              UINT32      ui4_line);
                  
#define x_thread_create(ph_th_hdl, ps_name, z_stacksize, ui1_pri, pf_main, z_arg_size, pv_arg)  \
    x_thread_dbg_create(ph_th_hdl, ps_name, z_stacksize, ui1_pri, pf_main, z_arg_size, pv_arg, __FILE__, __LINE__)

#define x_thread_exit()     \
    x_thread_dbg_exit(__FILE__, __LINE__)
#endif
#else
/*---------------------------------------------------------------------------*/
/*! @brief  this API allows a thread to give up CPU for a duration. it performs
 *
 *          1. check if thread mudule is initiated. 
 *
 *          2. call OS driver thread delay routine.
 *
 *  @param  [in] ui4_delay - in millisecond (ms) the thread gives up CPU.
 *
 *  
 *  @see    x_thread_create
 *
 *  @return VOID
 *  @retval -
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_thread_create (HANDLE_T*             ph_th_hdl,
                              const CHAR*           ps_name,
                              SIZE_T                z_stack_size,
                              UINT8                 ui1_priority,
                              x_os_thread_main_fct  pf_main_rtn,
                              SIZE_T                z_arg_size,
                              VOID*                 pv_arg);

/*---------------------------------------------------------------------------*/
/*! @brief  this API exits a thread. it performs
 *          1. check if thread mudule is initiated. 
 *          2. get thread handle, thread struct, and call handle_free().
 *          3. call OS driver thread exit routine.
 *
 *  @param  [in]
 *
 *  
 *  @see    x_thread_exit
 *
 *  @return VOID
 *  @retval -
 */
/*----------------------------------------------------------------------------*/
extern VOID x_thread_exit (VOID);
#endif

extern INT32 x_thread_delete (HANDLE_T  h_th_hdl);

/*---------------------------------------------------------------------------*/
/*! @brief  this API allows a thread to give up CPU for a duration. it performs
 *
 *          1. check if thread mudule is initiated. 
 *
 *          2. call OS driver thread delay routine.
 *
 *  @param  [in] ui4_delay - in millisecond (ms) the thread gives up CPU.
 *
 *  
 *  @see    x_thread_delay
 *
 *  @return VOID
 *  @retval -
 */
/*----------------------------------------------------------------------------*/
extern VOID x_thread_delay (UINT32  ui4_delay);

/*---------------------------------------------------------------------------*/
/*! @brief  This API sets some private data, associated with a thread and key.
 *          Such private data can again be extracted using API
 *          "x_thread_get_pvt ()". Note that the key must have the flag
 *          PVT_KEY_FLAG_APPL_ACCESS_ALLOWED set.
 *
 *  @param  [in] ui4_key - Contains the key associated with the private data.
 *  @param  [in] pf_pvt_del - References a callback function which is called when the
 *                            private data is being deleted or the thread is being
 *                            terminated.
 *  @param  [in] pv_pvt - Contains or references the private data.
 *
 *  
 *  @see    x_thread_set_pri
 *
 *  @return INT32
 *  @retval OSR_OK           - Routine was successful.
 *  @retval OSR_INV_ARG      - Invalid argument.
 *  @retval OSR_NOT_INIT     - Thread module has not been initiated.
 *  @retval OSR_NOT_EXIST    - Invalid or unknown OS thread.
 *  @retval OSR_DUP_KEY      - Duplicate key.
 *  @retval OSR_NO_RESOURCE  - Out of resources (e.g. memory etc.).
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_thread_set_pri (HANDLE_T  h_th_hdl,
                               UINT8     ui1_new_pri);

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

/*---------------------------------------------------------------------------*/
/*! @brief  This API gets some private data, associated with a thread and key.
 *          Such private data was set using API "x_thread_set_pvt ()". Note that
 *          the key must have the flag PVT_KEY_FLAG_APPL_ACCESS_ALLOWED set.
 *
 *  @param  [in] ui4_key - Contains the key associated with the private data.
 *  @param  [out] ppv_pvt - Contains or references the private data.
 *
 *  
 *  @see    x_thread_get_pri
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                   
 *  @retval OSR_INV_ARG         - Invalid argument.
 *  @retval OSR_NOT_INIT        - Thread module has not been initiated.
 *  @retval OSR_NOT_EXIST       - Invalid or unknown OS thread.
 *  @retval OSR_KEY_NOT_FOUND   - The specified key in argument "ui4_key" was not
 *                                found.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_thread_get_pri (HANDLE_T  h_th_hdl,
                               UINT8*    pui1_pri);

/*---------------------------------------------------------------------------*/
/*! @brief  this API suspends a thread. it performs
 *          1. check if thread mudule is initiated. 
 *          2. call OS driver thread suspend routine.
 *
 *  @param  [in]
 *
 *  @see    x_thread_suspend
 *
 *  @return VOID
 */
/*----------------------------------------------------------------------------*/
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
                               
/*---------------------------------------------------------------------------*/
/*! @brief  This API deletes some private data, associated with a thread and key.
 *          Such private data was set using API "x_thread_set_pvt ()". Note that
 *          the key must have the flag PVT_KEY_FLAG_APPL_ACCESS_ALLOWED set.
 *
 *  @param  [in] ui4_key - Contains the key associated with the private data.
 *
 *  
 *  @see    x_thread_del_pvt
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.
 *  @retval OSR_NOT_INIT        - Thread module has not been initiated.
 *  @retval OSR_NOT_EXIST       - Invalid or unknown OS thread.
 *  @retval OSR_KEY_NOT_FOUND   - The specified key in argument "ui4_key" was not
 *                                found.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_thread_del_pvt (UINT32  ui4_key);

/* Semaphhore API's */
/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a semaphore and return a handle to the semaphore.
 *
 *  @param  [in] e_type - can be X_SEMA_TYPE_BINARY, X_SEMA_TYPE_MUTEX
 *                        or X_SEMA_TYPE_COUNTING
 *  @param  [in] ui4_init_value - inital value of this semaphore.
 *  @param  [out] ph_sema_hdl - pointer to hold returned semaphore handle.
 *                              the handle is valid only if this API returns OSR_OK.
 *
 *  
 *  @see    x_sema_create
 *
 *  @return INT32
 *  @retval OSR_OK                      - routine was successful. 
 *  @retval OSR_NOT_INIT                - semaphore module has not been initiated. 
 *  @retval OSR_INV_ARG                 - an error in the arguments of this API.
 *  @retval OSR_NO_RESOURCE             - no resources (memory).
 *  @retval OSR_FAIL                    - OS driver related errors.
 *  @retval OSR_INV_HANDLE              - invalid handle.
 *  @retval OSR_OUT_OF_HANDLES          - no handle.
 *  @retval OSR_AEE_OUT_OF_RESOURCES    - can not allocate AEE resources
 *                                        for the semaphore.
 *  @retval OSR_AEE_NO_RIGHTS           - No permission to create the semaphore.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_sema_create (HANDLE_T*    ph_sema_hdl,
                            SEMA_TYPE_T  e_types,
                            UINT32       ui4_init_value);

/*---------------------------------------------------------------------------*/
/*! @brief  this API deletes a semaphore and its handle. 
 *
 *  @param  [in] h_sema_hdl - semaphore handle returned via c_sema_create().
 *
 *  
 *  @see    c_sema_delete
 *
 *  @return INT32
 *  @retval OSR_OK                      - routine was successful. 
 *  @retval OSR_NOT_INIT                - semaphore module has not been initiated. 
 *  @retval OSR_INV_ARG                 - an error in the arguments of this API.
 *  @retval OSR_FAIL                    - OS driver related errors.
 *  @retval OSR_INV_HANDLE              - invalid handle.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_sema_delete (HANDLE_T  h_sema_hdl);

extern INT32 x_sema_force_delete (HANDLE_T  h_sema_hdl);

/*---------------------------------------------------------------------------*/
/*! @brief  this API locks (acquires) a sema. 
 *
 *  @param  [in] h_sema_hdl - semaphore handle returned via c_sema_create().
 *  @param  [in] e_option - either X_SEMA_OPTION_WAIT or X_SEMA_OPTION_NOWAIT.
 *
 *  
 *  @see    x_sema_lock
 *
 *  @return INT32
 *  @retval OSR_OK                      - routine was successful. 
 *  @retval OSR_NOT_INIT                - semaphore module has not been initiated. 
 *  @retval OSR_INV_ARG                 - an error in the arguments of this API.
 *  @retval OSR_FAIL                    - OS driver related errors.
 *  @retval OSR_INV_HANDLE              - invalid handle.
 *  @retval OSR_WOULD_BLOCK             - if eoption is X_SEMA_OPTION_NOWAIT and the calling
 *                                        thread would have been blocked to lock the sema.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_sema_lock (HANDLE_T       h_sema_hdl,
                          SEMA_OPTION_T  e_options);

/*---------------------------------------------------------------------------*/
/*! @brief  this API attempts to lock (acquires) a semaphore,  if the semaphore
 *          has been acquired by others, then this API will only block for a
 *          specified amount of time before returning. 
 *
 *  @param  [in] h_sema_hdl - semaphore handle returned via c_sema_create().
 *  @param  [in] ui4_time - time duration in ms before thie API bails out.
 *
 *  
 *  @see    x_sema_lock_timeout
 *
 *  @return INT32
 *  @retval OSR_OK                      - routine was successful. 
 *  @retval OSR_NOT_INIT                - semaphore module has not been initiated.
 *  @retval OSR_FAIL                    - OS driver related errors.
 *  @retval OSR_INV_HANDLE              - invalid handle.
 *  @retval OSR_TIMEOUT                 - failed to lock the semaphore within "ui4_time" duration.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_sema_lock_timeout (HANDLE_T  h_sema_hdl,
                                  UINT32    ui4_time);

/*---------------------------------------------------------------------------*/
/*! @brief  this API unlocks (releases) a semaphore. 
 *
 *  @param  [in] h_sema_hdl - semaphore handle returned via c_sema_create().
 *
 *  
 *  @see    x_sema_unlock
 *
 *  @return INT32
 *  @retval OSR_OK                      - routine was successful. 
 *  @retval OSR_NOT_INIT                - semaphore module has not been initiated.
 *  @retval OSR_INV_ARG                 - an error in the arguments of this API.
 *  @retval OSR_FAIL                    - OS driver related errors.
 *  @retval OSR_INV_HANDLE              - invalid handle.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_sema_unlock (HANDLE_T  h_sema_hdl);


/* ISR API's */
extern INT32 x_reg_isr (UINT16         ui2_vector_id,
                        x_os_isr_fct   pf_isr,
                        x_os_isr_fct*  ppf_old_isr);


/* Timer API's */
/*---------------------------------------------------------------------------*/
/*! @brief  this API creates timer and a handle to the timer.
 *
 *  @param  [out] ph_timer  - pointer to hold returned timer handle.
 *                            the handle is valid only if this API returns OSR_OK.
 *                            the handle is used to access the created timer.
 *
 *  
 *  @see    x_timer_create
 *
 *  @return INT32
 *  @retval OSR_OK                      - routine was successful.
 *  @retval OSR_NOT_INIT                - timer module has not been initiated.
 *  @retval OSR_INV_ARG                 - an error in the arguments of this API.
 *  @retval OSR_NO_RESOURCE             - no resources (memory).
 *  @retval OSR_INV_HANDLE              - invalid handle.
 *  @retval OSR_OUT_OF_HANDLES          - no handle.
 *  @retval OSR_AEE_OUT_OF_RESOURCES    - can not allocate AEE resources for the timer.
 *  @retval OSR_AEE_NO_RIGHTS           - No permission to create the timer.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_timer_create (HANDLE_T*  ph_timer);

/*---------------------------------------------------------------------------*/
/*! @brief  this API starts a timer.
 *
 *  @param  [in] h_timer - timer handle returned from x_timer_create().
 *  @param  [in] ui4_delay - timer pops after this delay has passed.
 *  @param  [in] e_flags - either X_TIMER_FLAG_ONCE or X_TIMER_FLAG_REPEAT.
 *  @param  [in] pf_callback - callback function to be executed after "ui4_delay"
 *                 had passed.
 *  @param  [in] pv_tag - argument to callback function pf_cb.
 *
 *  
 *  @see    x_timer_start
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                   
 *  @retval OSR_NOT_INIT        - timer module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_timer_start (HANDLE_T           h_timer,
                            UINT32             ui4_delay,
                            TIMER_FLAG_T       e_flags,
                            x_os_timer_cb_fct  pf_callback,
                            VOID*              pv_tag);

/*---------------------------------------------------------------------------*/
/*! @brief  this API stops a timer.
 *
 *  @param  [in] h_timer - timer handle returned from x_timer_create().
 *
 *  
 *  @see    x_timer_stop
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                   
 *  @retval OSR_NOT_INIT        - timer module has not been initiated.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_timer_stop (HANDLE_T  h_timer);

/*---------------------------------------------------------------------------*/
/*! @brief  this API delete a timer.
 *
 *  @param  [in] h_timer - this API deleted a timer and its handle.
 *
 *  
 *  @see    x_timer_delete
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                   
 *  @retval OSR_NOT_INIT        - timer module has not been initiated.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_timer_delete (HANDLE_T  h_timer);

/*---------------------------------------------------------------------------*/
/*! @brief  this API resumes a previoulsy stopped timer.
 *
 *  @param  [in] h_timer - this API deleted a timer and its handle.
 *
 *  
 *  @see    x_timer_resume
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.                   
 *  @retval OSR_NOT_INIT        - timer module has not been initiated.
 *  @retval OSR_INV_HANDLE      - an error in handle operation.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_timer_resume (HANDLE_T  h_timer);

/* Memory API's */
#ifdef SYS_MEM_CHK
#ifdef CC_MTK_LOADER
extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*  pv_mem_block,
                            SIZE_T z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);
#else
extern VOID x_mem_dbg_log_start(VOID);

extern VOID x_mem_dbg_log_stop(VOID);

extern VOID x_mem_dbg_log_dump(VOID);

extern VOID* x_mem_dbg_alloc (SIZE_T  z_size, const CHAR* s_file, UINT32      ui4_line );

extern VOID* x_mem_dbg_calloc (UINT32  ui4_num_element,
                               SIZE_T  z_size_element, const CHAR* s_file, UINT32      ui4_line);

extern VOID* x_mem_dbg_realloc (VOID*        pv_mem_block,
                                SIZE_T       z_new_size, const CHAR* s_file, UINT32      ui4_line);

extern VOID x_mem_dbg_free (VOID*  pv_mem_block, const CHAR* s_file, UINT32      ui4_line);

#if defined(__KERNEL__)
extern VOID* x_mem_alloc (SIZE_T  z_size);
#else
#define x_mem_alloc(z_size)                              \
    x_mem_dbg_alloc( z_size, __FILE__, __LINE__)
#endif

#define x_mem_calloc(ui4_num_element, z_size_element)    \
    x_mem_dbg_calloc( ui4_num_element, z_size_element, __FILE__, __LINE__)

#define x_mem_realloc(pv_mem_block,z_new_size)           \
    x_mem_dbg_realloc( pv_mem_block, z_new_size, __FILE__, __LINE__)

#define x_mem_free( pv_mem_block )                       \
    x_mem_dbg_free( pv_mem_block, __FILE__, __LINE__)
#endif


#else

#if defined(OSAI_FOR_NUCLEUS) || !defined(LINUX_TURNKEY_SOLUTION)
extern VOID* x_mem_alloc (SIZE_T  z_size);
extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);
extern VOID* x_mem_realloc (VOID*  pv_mem_block,
                            SIZE_T z_new_size);
#endif


/*---------------------------------------------------------------------------*/
/*! @brief  This API frees a memory block. it performs
 *
 *          1. check validity of pv_mem.
 *
 *          2. call os_mem_free() to actual free the memory block.
 *
 *  @param  [in] pv_mem - pointer to a memory block to be freed.
 *
 *  
 *  @see    x_mem_free
 *
 *  @return VOID
 *  @retval -
 */
/*----------------------------------------------------------------------------*/
extern VOID x_mem_free (VOID*  pv_mem_block);

#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a memory partition. it performs
 *          1. check if (default) memory is initiated.
 *          2. parse argumets.
 *          3. call sem lock to guard partition table.
 *          4. call os_mem_part_create() to actually create a partition.
 *
 *  @param  [in] ps_name - an unique partition name, null ('\0') terminated. Length can
 *                         be up to 16 characters, excluding '\0'.
 *  @param  [in] pv_addr - starting memory address for this partition.
 *  @param  [in] z_size - size of this partition.
 *  @param  [in] z_alloc_size - 0 for dynamic size allocation. non-zero for fixed size
 *                              allocation (z_alloc_size is the size).
 *
 *  @param  [out] ph_part_hdl - pointer to hold returned partition handle.
 *                              the handle is valid only if this API returns OSR_OK.
 *  
 *  @see    x_mem_part_create
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.        
 *  @retval OSR_NOT_INIT        - memory module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_NO_RESOURCE     - no resources (memory).
 *  @retval OSR_FAIL            - OS driver related errors.
 *  @retval OSR_EXIST           - partition exists (ps_name collision).
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_mem_part_create (HANDLE_T*    ph_part_hdl,
                                const CHAR*  ps_name,
                                VOID*        pv_addr,
                                SIZE_T       z_size,
                                SIZE_T       z_alloc_size);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a memory partition. it performs
 *          1. check if (default) memory is initiated. 
 *          2. check argument.
 *          3. call sem lock to guard partition table.
 *          4. call os_mem_part_delete() to actually delete the partition.
 *
 *  @param  [in] h_part_hdl - memory partition handle.
 *  
 *  @see    x_mem_part_delete
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.        
 *  @retval OSR_NOT_INIT        - memory module has not been initiated.
 *  @retval OSR_INV_HANDLE      - invalid handle.
 *  @retval OSR_NOT_EXIST       - partition does not exists.
 *  @retval OSR_MEM_NOT_FREE    - partition is deleted but its memory has not freed.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_mem_part_delete (HANDLE_T);

/*---------------------------------------------------------------------------*/
/*! @brief  this API creates a memory partition. note that this API returns the
 *          same handle as the one returned by x_mem_part_create().
 *          it performs
 *          1. check if (default) memory is initiated. 
 *          2. parse argumets.
 *          3. call sem lock to guard partition table.
 *          4. call os_mem_part_check() to check existence of partition based on
 *              ps_name.
 *          5. return partition handle created in x_mem_part_create().
 *
 *  @param  [in] ps_name - an unique partition name, null ('\0') terminated. Length can
 *                         be up to 16 characters, excluding '\0'.
 *
 *  @param  [out] ph_part_hdl - pointer to hold returned partition handle.
 *                              the handle is valid only if this API returns OSR_OK.
 *  
 *  @see    x_mem_part_attach
 *
 *  @return INT32
 *  @retval OSR_OK              - routine was successful.        
 *  @retval OSR_NOT_INIT        - memory module has not been initiated.
 *  @retval OSR_INV_ARG         - an error in the arguments of this API.
 *  @retval OSR_FAIL            - OS driver related errors.
 *  @retval OSR_NOT_EXIST       - partition does not exist.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_mem_part_attach (HANDLE_T*    ph_part_hdl,
                                const CHAR*  ps_name);

/*---------------------------------------------------------------------------*/
/*! @brief  this API allocates memory from specified partition. it performs
 *          1. check if memory was initialized.
 *          2. check validity of partition handle.
 *          3. check if partition exists (created via x_mem_part_create()).
 *          4. call partition allocate funtiocn pf_alloc().
 *
 *  @param  [in] h_part_hdl - memory partition handle.
 *  @param  [in] z_size - requested memory size.
 *
 *  
 *  @see    x_mem_part_alloc
 *
 *  @return VOID*
 *  @retval non NULL            - routine was successful.        
 *  @retval NULL                - routine failed.
 */
/*----------------------------------------------------------------------------*/
extern VOID* x_mem_part_alloc (HANDLE_T  h_part_hdl,
                               SIZE_T    z_size);

/*---------------------------------------------------------------------------*/
/*! @brief  this API allocates memory from specified partition. it performs
 *          1. check if memory was initialized.
 *          2. check validity of partition handle.
 *          3. check if partition exists (created via x_mem_part_create()).
 *          4. adjust to have at least one unit if needed.
 *          5. call partition allocate funtiocn pf_alloc().
 *          6. zero out the allocated memory space.
 *
 *  @param  [in] h_part_hdl - memory partition handle.
 *  @param  [in] ui4_num_unit - number of memory units
 *  @param  [in] z_unit_size - size of each unit
 *
 *  
 *  @see    x_mem_part_calloc
 *
 *  @return VOID*
 *  @retval non NULL            - routine was successful.        
 *  @retval NULL                - routine failed.
 */
/*----------------------------------------------------------------------------*/
extern VOID* x_mem_part_calloc (HANDLE_T  h_part_hdl,
                                UINT32    ui4_num_element,
                                SIZE_T    z_size_element);

/*---------------------------------------------------------------------------*/
/*! @brief  this API allocates memory block of "z_new_size" from specified partition
 *          to replace the existing memory blcok "pv_mem". it performs
 *          1. check if memory was initialized.
 *          2. check validity of partition handle.
 *          3. check validity of pv_mem.
 *          4. check if default partition exists (created via os_main()).
 *          5. if pv_mem is NULL, it is equivalent to x_mem_alloc().
 *          6. if z_new_size is 0, it is equivalent to x_mem_free().
 *          7. call os_mem_realloc() to allocate memory.
 *
 *  @param  [in] h_part_hdl - memory partition handle.
 *  @param  [in] pv_mem - existing, allocated memory block.
 *  @param  [in] z_new_size - size of new memory block.
 *
 *  
 *  @see    x_mem_part_realloc
 *
 *  @return VOID*
 *  @retval non NULL            - routine was successful.        
 *  @retval NULL                - routine failed.
 */
/*----------------------------------------------------------------------------*/
extern VOID* x_mem_part_realloc (HANDLE_T     h_part_hdl,
                                 VOID*        pv_mem_block,
                                 SIZE_T       z_new_size);

extern VOID* x_share_mem_alloc(SIZE_T z_size);

extern VOID x_share_mem_free(VOID* pv_phy_addr, SIZE_T z_size);

extern VOID* x_mmap_share_mem(VOID* pv_phy_addr, SIZE_T z_size);
extern VOID* x_mmap_share_mem_rmmgr(VOID* pv_phy_addr, SIZE_T z_size);

extern VOID x_munmap_share_mem(VOID* pv_virt_addr, SIZE_T z_size);

#if !defined(OSAI_FOR_NUCLEUS) && defined(LINUX_TURNKEY_SOLUTION)
extern BOOL x_get_fbm_info(UINT32* pui4_phy_addr, UINT32* pui4_virt_addr, UINT32* pui4_size);
#endif

/* Utility API's */
/*---------------------------------------------------------------------------*/
/*! @brief  this API copies pv_from to pv_to up to z_l bytes. 
 *
 *  @param  [in] pv_to - memory block to be copied to.
 *  @param  [in] pv_from - memory block to be copied from 
 *  @param  [in] z_len - bytes to be copied
 *
 *  
 *  @see    x_memcpy
 *
 *  @return VOID*
 *  @retval pointer to pv_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_memcpy            memcpy
#else

extern VOID* x_memcpy (VOID*        pv_to,
                       const VOID*  pv_from,
                       SIZE_T       z_len);
#endif


/*---------------------------------------------------------------------------*/
/*! @brief  this API compares 2 memory blocks pv_mem1 and pv_mem2 up to z_len
 *          bytes.
 *
 *          1. if both are NULL, return 0.
 *
 *          2. if pv_mem1 is NULL and pv_mem2 is not NULL, return -1.
 *
 *          3. if pv_mem1 is not NULL and pv_mem2 is NULL, return 1.
 *
 *          4. compute the difference between pv_mem1 and pv_mem2 side by side.
 *
 *          5. if difference is less than zero, return -1; if difference is
 *              positive, return 1; if diffenece is 0, move both pointers, repeat
 *              steps 4, 5.
 *
 *          6. stop when z_len bytes have met.
 *
 *  @param  [in] pv_mem1 - pointer to the first memory block.
 *  @param  [in] pv_mem2 - pointer to the second memory block.
 *  @param  [in] z_len - byte count to compare.
 *
 *  
 *  @see    x_memcmp
 *
 *  @return INT32
 *  @retval 0   - pv_mem1 is identical to pv_mem2.
 *  @retval 1   - pv_mem1 is greater than pv_mem2.
 *  @retval -1  - pv_mem1 is less than pv_mem2.
 */
/*----------------------------------------------------------------------------*/

#if defined(__KERNEL__)
#define x_memcmp            memcmp
#else
extern INT32 x_memcmp(const VOID*  pv_mem1,
                           const VOID*  pv_mem2,
                           SIZE_T       z_len);
#endif
/*---------------------------------------------------------------------------*/
/*! @brief  this API copies pv_from to pv_to up to z_len bytes. 
 *
 *  @param  [in] pv_to - memory block to be copied to.
 *  @param  [in] pv_from - memory block to be copied from 
 *  @param  [in] z_len - bytes to be copied
 *
 *  
 *  @see    x_memmove
 *
 *  @return VOID*
 *  @retval pointer to pv_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_memmove           memmove
#else
extern VOID* x_memmove (VOID*        pv_to,
                        const VOID*  pv_from,
                        SIZE_T       z_len);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API copies ui1_c to pv_to up to z_len bytes. 
 *
 *  @param  [in] pui1_mem - memory block to write to.
 *  @param  [in] ui1_c - data to populate to pui1_mem.
 *  @param  [in] z_len - number of bytes in 'pui1_mem' to write to.
 *
 *  
 *  @see    x_memset
 *
 *  @return VOID*
 *  @retval pointer to pv_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_memset            memset
#else
extern VOID* x_memset (VOID*   pui1_mem,
                       UINT8   ui1_c,
                       SIZE_T  z_len);
#endif

extern VOID* x_memchr (const VOID*  pv_mem,
                       UINT8        ui1_c,
                       SIZE_T       z_len);

extern VOID* x_memrchr (const VOID*  pv_mem,
                        UINT8        ui1_c,
                        SIZE_T       z_len);

/*---------------------------------------------------------------------------*/
/*! @brief  this API returns a pointer to a memory block that duplicates ps_str.
 *
 *          1. parse argument and return NULL if it is a NULL string.
 *
 *          2. get string ps_str length plus 1.
 *
 *          3. allocate memory block of such size.
 *
 *          4. memory copy from ps_str to the allocated memory.
 *
 *  @param  [in] ps_s - null ('\0') terminated character string.
 *
 *  
 *  @see    x_strdup
 *
 *  @return CHAR*
 *  @retval pointer to copied string. - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strdup            strdup
#else
extern CHAR* x_strdup (const CHAR*  ps_str);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API copies ps-from to ps_to. it performs
 *
 *          1. parse argument and return NULL if either is NULL.
 *
 *          2. copy character by character from ps_from to ps_to until
 *              '\0' is encountered.
 *
 *
 *  @param  [in] ps_to - memory block to be copied to.
 *  @param  [in] ps_from - null ('\0') terminated character string.
 *
 *  
 *  @see    x_strcpy
 *
 *  @return CHAR*
 *  @retval pointer to ps_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strcpy            strcpy
#else
extern CHAR* x_strcpy (CHAR*        ps_to,
                       const CHAR*  ps_from);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API copies ps-from to ps_to up to z_l bytes. it performs
 *
 *          1. parse argument and return ps_to on error.
 *
 *          2. process to make the most UINT32 copies, then UINT16 copies, then
 *              UINT8 copies. 
 *
 *
 *  @param  [in] ps_to - memory block to be copied to.
 *  @param  [in] ps_from - null ('\0') terminated character string.
 *  @param  [in] z_l - bytes to be copied
 *
 *  
 *  @see    x_strncpy
 *
 *  @return CHAR*
 *  @retval pointer to ps_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strncpy           strncpy
#else
extern CHAR* x_strncpy (CHAR*        ps_to,
                        const CHAR*  ps_from,
                        SIZE_T       z_len);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API compares 2 strings ps_str1 and ps_str2. it performs
 *
 *          1. if both are NULL, return 0.
 *
 *          2. if ps_s1 is NULL and ps_s2 is not NULL, return -1.
 *
 *          3. if ps_s1 is not NULL and ps_s2 is NULL, return 1.
 *
 *          4. compute the difference between ps_str1 and ps_str2 side by side.
 *
 *          5. if difference is less than zero, return -1; if difference is
 *          positive, return 1; if diffenece is 0, move both pointers, repeat
 *              steps 4, 5.
 *
 *          6. stop when either one moves to end of string.
 *
 *  @param  [in] ps_str1 - null ('\0') terminated character string.
 *  @param  [in] ps_str2 - null ('\0') terminated character string.
 *
 *  
 *  @see    x_strcmp
 *
 *  @return INT32
 *  @retval 0   - ps_str1 is identical to ps_str2.
 *  @retval 1   - ps_str1 is greater than ps_str2.
 *  @retval -1  - ps_str1 is less than ps_str2.
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strcmp            strcmp
#else
extern INT32 x_strcmp (const CHAR*  ps_str1,
                       const CHAR*  ps_str2);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API compares 2 strings ps_str1 and ps_str2 up to z_len bytes.
 *          it performs
 *
 *          1. if both are NULL, return 0.
 *
 *          2. if ps_str1 is NULL and ps_str2 is not NULL, return -1.
 *
 *          3. if ps_str1 is not NULL and ps_str2 is NULL, return 1.
 *
 *          4. compute the difference between ps_str1 and ps_str2 side by side.
 *
 *          5. if difference is less than zero, return -1; if difference is
 *              positive, return 1; if diffenece is 0, move both pointers, repeat
 *              steps 4, 5.
 *
 *          6. stop when either one moves to end of string or z_len bytes have met.
 *
 *  @param  [in] ps_str1 - null ('\0') terminated character string.
 *  @param  [in] ps_str2 - null ('\0') terminated character string.
 *  @param  [in] z_len - byte count to compare.
 *
 *  
 *  @see    x_strncmp
 *
 *  @return INT32
 *  @retval 0   - ps_str1 is identical to ps_str2.
 *  @retval 1   - ps_str1 is greater than ps_str2.
 *  @retval -1  - ps_str1 is less than ps_str2.
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strncmp           strncmp
#else
extern INT32 x_strncmp (const CHAR*  ps_str1,
                        const CHAR*  ps_str2,
                        SIZE_T       z_len);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  Similar to c_strcmp, but ignoring the case of the characters.
 *
 *  @param  [in] ps_s1 - null ('\0') terminated character string.
 *  @param  [in] ps_s2 - null ('\0') terminated character string.
 *
 *  
 *  @see    x_strcasecmp
 *
 *  @return INT32
 *  @retval 0   - ps_s1 is identical to ps_s2.
 *  @retval 1   - ps_s1 is greater than ps_s2.
 *  @retval -1  - ps_s1 is less than ps_s2.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_strcasecmp (const CHAR*  ps_str1,
                           const CHAR*  ps_str2);

/*---------------------------------------------------------------------------*/
/*! @brief  Similar to c_strncmp, but ignoring the case of the characters.
 *
 *  @param  [in] ps_s1 - null ('\0') terminated character string.
 *  @param  [in] ps_s2 - null ('\0') terminated character string.
 *  @param  [in] z_len - byte count to compare.
 *
 *  
 *  @see    x_strncasecmp
 *
 *  @return INT32
 *  @retval 0   - ps_s1 is identical to ps_s2.
 *  @retval 1   - ps_s1 is greater than ps_s2.
 *  @retval -1  - ps_s1 is less than ps_s2.
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_strncasecmp (const CHAR*  ps_str1,
                            const CHAR*  ps_str2,
                            SIZE_T       z_len);

/*---------------------------------------------------------------------------*/
/*! @brief  this API concatenates ps_append to ps_to. it performs
 *
 *          1. parse argument and return ps_to if either is NULL.
 *
 *          2. move temp pointer to the end of ps_to.
 *
 *          3. copy character by character from ps_append to temp pointer until
 *              '\0' is encountered.
 *
 *  @param  [in] ps_to - null ('\0') terminated character string.
 *  @param  [in] ps_append - null ('\0') terminated character string.
 *
 *  
 *  @see    x_strcat
 *
 *  @return CHAR*
 *  @retval pointer to ps_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strcat            strcat
#else
extern CHAR* x_strcat (CHAR*        ps_to,
                       const CHAR*  ps_append);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API concatenates ps_append to ps_to up to z_len bytes. it performs
 *
 *          1. parse argument and return ps_to if either is NULL.
 *
 *          2. move temp pointer to the end of ps_to.
 *
 *          3. call x_strncpy() to copy z_len bytes.
 *
 *  @param  [in] ps_to - null ('\0') terminated character string.
 *  @param  [in] ps_append - null ('\0') terminated character string.
 *  @param  [in] z_l - bytes to be concaatenated from ps_append.
 *
 *  
 *  @see    x_strncat
 *
 *  @return CHAR*
 *  @retval pointer to ps_to - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strncat           strncat
#else
extern CHAR* x_strncat (CHAR*        ps_to,
                        const CHAR*  ps_append,
                        SIZE_T       z_len);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API locates the first occurrence of c_c in ps_s. it performs
 *
 *          1. parse argument and return NULL if it is a NULL string.
 *
 *          2. move along ps_s until c_c is found and return.
 *
 *          3. if not found, return NULL.
 *
 *  @param  [in] ps_s - null ('\0') terminated character string.
 *  @param  [in] c_c - character to locate.
 *
 *  
 *  @see    x_strchr
 *
 *  @return CHAR*
 *  @retval pointer to the first occurrence of c_c in ps_s. - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strchr            strchr
#else
extern CHAR* x_strchr (const CHAR*  ps_str,
                       CHAR         c_char);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  This API locates the last occurrence of a character in a string. It
 *          returns NULL if the character does not exist in the string.
 *
 *  @param  [in] ps_str - null ('\0') terminated character string.
 *  @param  [in] c_cchar - character to locate.
 *
 *  
 *  @see    x_strrchr
 *
 *  @return CHAR*
 *  @retval pointer to the last occurrence of c_char in ps_str, or NULL. - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strrchr           strrchr
#else
extern CHAR* x_strrchr (const CHAR*  ps_str,
                        CHAR         c_char);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API locates the first occurrence of ps_find in ps_s. it performs
 *
 *          1. parse arguments and return NULL if either is NULL string.
 *
 *          2. if ps_find contains '\0', returns ps_s.
 *
 *          3. move both ps_s and ps_find as long as they are identical, otherwise,
 *              strat over ps_find and continue to compare.
 *
 *          4. if not found, return NULL.
 *
 *  @param  [in] ps_str - null ('\0') terminated character string.
 *  @param  [in] ps_find - null terminated character string to locate.
 *
 *  
 *  @see    x_strstr
 *
 *  @return CHAR*
 *  @retval pointer to the first occurrence of ps_find in ps_s. - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strstr            strstr
#else
extern CHAR* x_strstr (const CHAR*  ps_str,
                       const CHAR*  ps_find);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  This API converts the initial part of the string in 'pc_beg_ptr' to an
 *          unsigned 64-bit integer value according to the given base. This API is
 *          the equivalent of the ANSI C strtoull API.
 *
 *  @param  [in] pc_beg_ptr - null ('\0') terminated character string.
 *  @param  [in] ui1_base - The base of numeric conversion, which must be between 2
 *                          and 36 inclusive, or be the special value 0.
 *
 *  
 *  @see    x_strtoull
 *
 *  @return UINT64
 *  @retval Unsigned INT64 value - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strtoull          strtoull
#else
extern UINT64 x_strtoull (const CHAR*  pc_beg_ptr,
                          CHAR**       ppc_end_ptr,
                          UINT8        ui1_base);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  This API converts the initial part of the string in 'pc_beg_ptr' to a
 *          signed 64-bit integer value according to the given base. This API is
 *          the equivalent of the ANSI C strtoll API.
 *
 *  @param  [in] pc_beg_ptr - null ('\0') terminated character string.
 *  @param  [in] ui1_base - The base of numeric conversion, which must be between 2
 *                          and 36 inclusive, or be the special value 0.
 *  @param  [out] ppc_end_ptr - If ppc_end_ptr is not NULL, *ppc_end_ptr contains the
 *                              address of the first invalid character.
 *
 *  
 *  @see    x_strtoll
 *
 *  @return INT64
 *  @retval Signed INT64 value - 
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strtoll           strtoll
#else
extern INT64 x_strtoll (const CHAR*  pc_beg_ptr,
                        CHAR**       ppc_end_ptr,
                        UINT8        ui1_base);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API returns length of a character string. it performs
 *
 *      1. parse argument and return 0 if it is a NULL string.
 *
 *      2. move temp pointer to null character.
 *
 *      3. return distance between temp pointer and original pointer.
 *
 *  @param  [in] ps_s - null ('\0') terminated character string.
 *
 *  
 *  @see    x_strlen
 *
 *  @return SIZE_T
 *  @retval length of character string excluding null ('\0') character. - 
 */
/*----------------------------------------------------------------------------*/
extern SIZE_T x_strlen (const CHAR*  ps_str);

/*---------------------------------------------------------------------------*/
/*! @brief  This API calculates the length of the initial segment of a string which
 *          consists entirely of characters in another string.
 *
 *  @param  [in] ps_str - Null-terminated character string.
 *  @param  [in] ps_accept - Character string containing the accepted character set.
 *
 *  
 *  @see    x_strspn
 *
 *  @return SIZE_T
 *  @retval Length of the segment. If either ps_str or ps_accept is NULL, the API returns 0. -
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strspn            strspn
#else
extern SIZE_T x_strspn (const CHAR*  ps_str,
                        const CHAR*  ps_accept);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  This API calculates the length of the initial segment of a string which
 *          consists entirely of characters not in another string.
 *
 *  @param  [in] ps_str - Null-terminated character string.
 *  @param  [in] ps_reject - Character string containing the rejected character set.
 *
 *  
 *  @see    x_strcspn
 *
 *  @return SIZE_T
 *  @retval Length of the segment. If either ps_str or ps_reject is NULL, the API returns 0. -
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_strcspn           strcspn
#else
extern SIZE_T x_strcspn (const CHAR*  ps_str,
                         const CHAR*  ps_reject);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  This API converts a string to uppercase.
 *
 *  @param  [in] ps_str - null ('\0') terminated character string.
 *
 *  
 *  @see    x_str_toupper
 *
 *  @return CHAR*
 *  @retval the pointer passed in ps_str. - 
 */
/*----------------------------------------------------------------------------*/
extern CHAR* x_str_toupper (CHAR*  ps_str);

/*---------------------------------------------------------------------------*/
/*! @brief  This API converts a string to lowercase.
 *
 *  @param  [in] ps_str - null ('\0') terminated character string.
 *
 *  
 *  @see    x_str_tolower
 *
 *  @return CHAR*
 *  @retval the pointer passed in ps_str. - 
 */
/*----------------------------------------------------------------------------*/
extern CHAR* x_str_tolower (CHAR*  ps_str);

extern CHAR x_toupper (CHAR  c_char);

extern CHAR x_tolower (CHAR  c_char);

/*---------------------------------------------------------------------------*/
/*! @brief  This API returns a pointer to the next 'token' in ps_str. Tokens are
 *          separated by delimiters (specified by ps_delimiter). This API returns
 *          NULL if no token is found (including if the string is empty).
 *
 *  @param  [in] ps_str - null ('\0') terminated character string.
 *  @param  [in] ps_delimiter - null ('\0') terminated character string.
 *  @param  [out] pps_str - Will point to the next string to parse.
 *  @param  [out] pz_token_len - Will contain the length of the found token. If
 *                               pz_token_len is NULL then the passed string is
 *                               modified so it will contain a NULL character at the
 *                               end of the token.
 *
 *  
 *  @see    x_strtok
 *
 *  @return CHAR*
 *  @retval pointer to the first token in ps_str. -
 */
/*----------------------------------------------------------------------------*/
extern CHAR* x_strtok (CHAR*        ps_str,
                       const CHAR*  ps_delimiters,
                       CHAR**       pps_str,
                       SIZE_T*      pz_token_len);

/*---------------------------------------------------------------------------*/
/*! @brief  this API is duplicate Middleware's version of x_sprintf().
 *
 *  @param  [in] ps_buf - output buffer.
 *  @param  [in] ps_fmt - format string.
 *  @param  [in] ...    - variable arguments
 *
 *  
 *  @see    x_sprintf
 *
 *  @return INT32
 *  @retval length of ps_buf. -
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_sprintf           sprintf
#else
extern INT32 x_sprintf (CHAR*        ps_str,
                        const CHAR*  ps_format,
                        ...);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API implement the vsprintf() functionality.
 *
 *  @param  [in] ps_buf - output buffer.
 *  @param  [in] ps_fmt - format string.
 *  @param  [in] va_list - variable arguments
 *
 *  
 *  @see    x_vsprintf
 *
 *  @return INT32
 *  @retval length of ps_buf. -
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_vsprintf          vsprintf
#else
extern INT32 x_vsprintf (CHAR*        ps_str,
                         const CHAR*  ps_format,
                         VA_LIST      va_list);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API is duplicate Middleware's version of x_snprintf().
 *
 *  @param  [in] ps_buf - output buffer.
 *  @param  [in] z_size - size of the output buffer.
 *  @param  [in] ps_fmt - format string.
 *  @param  [in] ...    - variable arguments
 *
 *  
 *  @see    x_snprintf
 *
 *  @return INT32
 *  @retval length of ps_buf. -
 */
/*----------------------------------------------------------------------------*/
#if defined(__KERNEL__)
#define x_snprintf          snprintf
#else
extern INT32 x_snprintf (CHAR*        ps_str,
                         SIZE_T       z_size,
                         const CHAR*  ps_format,
                         ...);
#endif

#if defined(__KERNEL__)
#define x_vsnprintf         vsnprintf
#else
/*---------------------------------------------------------------------------*/
/*! @brief  this API implement the vsnprintf() functionality.
 *
 *  @param  [in] ps_buf - output buffer.
 *  @param  [in] z_size - size of the output buffer.
 *  @param  [in] ps_fmt - format string.
 *  @param  [in] va_list - variable arguments
 *
 *  
 *  @see    x_vsnprintf
 *
 *  @return INT32
 *  @retval length of ps_buf. -
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_vsnprintf (CHAR*        ps_str,
                          SIZE_T       z_size,
                          const CHAR*  ps_format,
                          VA_LIST      va_list);
#endif

/*---------------------------------------------------------------------------*/
/*! @brief  this API implement the vsnprintf() functionality.
 *
 *  @param  [in] ps_buf - References the output buffer.
 *  @param  [in] ps_fmt - References the format string.
 *  @param  [in] ...    - Additional parameters (i.e. elements to output).
 *
 *  
 *  @see    x_sscanf
 *
 *  @return INT32
 *  @retval The number of successful assignment performed. -
 */
/*----------------------------------------------------------------------------*/
extern INT32 x_sscanf    (const CHAR *ps_buf,
                          const CHAR *ps_fmt,
                          ...);

/*---------------------------------------------------------------------------*/
/*! @brief  This API calls vsscanf_generic, specifying that there is no
 *          limit on the output buffer (i.e. b_snprintf is FALSE).
 *
 *  @param  [in] ps_buf - References the output buffer.
 *  @param  [in] ps_fmt - References the format string.
 *  @param  [in] t_ap   - Contains the list of parameters.
 *
 *  
 *  @see    x_vsscanf
 *
 *  @return INT32
 *  @retval The number of characters written, not including the NULL character. -
 */
/*----------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------*/
/*! @brief  This API returns the system tick count.
 *
 *  @param  [in]
 *  @param  [out]
 *
 *  
 *  @see    x_os_get_sys_tick
 *
 *  @return UINT32
 *  @retval The system tick value -
 */
/*----------------------------------------------------------------------------*/
extern UINT32 x_os_get_sys_tick (VOID) ;

/*---------------------------------------------------------------------------*/
/*! @brief  This API returns the period (in ms) between system tick.
 *
 *  @param  [in]
 *  @param  [out]
 *
 *  
 *  @see    x_os_get_sys_tick_period
 *
 *  @return UINT32
 *  @retval The period (in ms) between system tick. -
 */
/*----------------------------------------------------------------------------*/
extern UINT32 x_os_get_sys_tick_period (VOID);

/*---------------------------------------------------------------------------*/
/*! @brief  to set system boot up time.
 *
 *  @param  [in]
 *  @param  [out]
 *
 *  
 *  @see    x_os_set_timestamp
 *
 *  @return VOID
 *  @retval -
 */
/*----------------------------------------------------------------------------*/
extern VOID x_os_set_timestamp (CHAR *szString);

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_OSAI*/
/*----------------------------------------------------------------------------*/

#endif /* _X_OS_H_ */
