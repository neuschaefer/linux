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
#ifndef _OSAI_API_H_
#define _OSAI_API_H_

#include "x_typedef.h"
#include "u_handle.h" //need data type HANDLE_T
#include "u_os.h"     //need x_os_thread_main_fct, OS API return values
/*************************************************************************/
//
//				thread APIs 
/*************************************************************************/
//-------------------------------------------------------------------------
/**
 *      This API creates a thread. it performs
 *      1. check if thread module is initiated.
 *      2. parse argumets.
 *      3. allocate memory for thread struct and for the specified argument.
 *      4. check existence of the thread (based on msg Q name).
 *      5. initialize, allocate, and link a messaage haandle.
 *      6. call OS driver thread create routine passing the middleware
 *         generic main function "thread_main".
 *
 * Inputs:
 *  @param    ps_name 		name of this thread, null ("\0") terminated, up to 16
 *               		characters excluding "\0" will be used.
 *  @param    z_stacksize 	stack size this thread will run on.
 *  @param    ui1_pri 		priority of this thread, 1- highest, 255 - lowest.
 *  @param    pf_main 		main entry of this thread.
 *  @param    z_arg_size 	size in byte of argument of pf_main.
 *  @param    pv_arg 		argument of pf_main.
 *
 * Outputs:
 *  @param    *ph_th_hdl	pointer to a handle to be returned by this API.
 *                  		the handle is valid only if this API returns OSR_OK.
 *
 *  @retval    OSR_OK		routine was successful. 
 *  @retval    OSR_NOT_INIT  	thread module has not been initiated.
 *  @retval    OSR_INV_ARG 	an error in the arguments of this API.
 *  @retval    OSR_NO_RESOURCE	no resources (memory).
 *  @retval    OSR_FAIL 	OS driver related errors.
 *  @retval    OSR_EXIST 	thread exists (ps_name collision).
 *  @retval    OSR_INV_HANDLE 	invalid handle.
 *  @retval    OSR_OUT_OF_HANDLES no handle.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_thread_create(HANDLE_T    *ph_th_hdl,         /* thread handle */
                const CHAR  *ps_name,
                SIZE_T       z_stacksize,
                UINT8        ui1_pri,           /* thread priority */
                x_os_thread_main_fct pf_main,   /* thread's main function */
                SIZE_T       z_arg_size,
                VOID         *pv_arg);

//-------------------------------------------------------------------------
/**
 *      This API exits a thread. it performs
 *      1. check if thread mudule is initiated.
 *      2. get thread handle, thread struct, and call handle_free().
 *      3. call OS driver thread exit routine.
 *  @retval   void 
 */
//-------------------------------------------------------------------------
EXTERN VOID x_thread_exit(VOID);

//-------------------------------------------------------------------------
/**
 *      This API suspends a thread. it performs
 *      1. check if thread mudule is initiated.
 *      2. call OS driver thread suspend routine.
 *  @retval   void 
 *
 */
//-------------------------------------------------------------------------
EXTERN VOID x_thread_suspend(VOID);

//-------------------------------------------------------------------------
/**
 *      This API resumes a thread. it performs
 *      1. check if thread mudule is initiated. 
 *      2. parse argumets.
 *      3. check validity of thread handle.
 *      4. call OS driver thread resume routine.
 *
 * Inputs:
 *  @param  	h_th_hdl	thread handle returned via x_thread_create().
 *
 *  @retval	OSR_OK		routine was successful.
 *  @retval     OSR_NOT_INIT	thread module has not been initiated.
 *  @retval     OSR_FAIL	OS driver related errors.
 *  @retval     OSR_INV_HANDLE	an error in handle operation.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_thread_resume(HANDLE_T h_th_hdl);

//-------------------------------------------------------------------------
/**
 *  This API allows a thread to give up CPU for a duration. it performs
 *  1. check if thread mudule is initiated.
 *  2. call OS driver thread delay routine.
 *  @param  ui4_delay   in millisecond (ms) the thread gives up CPU.
 *  @retval   void
 */
//-------------------------------------------------------------------------
EXTERN VOID x_thread_delay(UINT32 ui4_delay);

/*************************************************************************/
//
//				semaphore APIs 
/*************************************************************************/
//-------------------------------------------------------------------------
/**
 *      this API creates a semaphore and its handle. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. parse argumets.
 *      3. allocate memory for sema struct.
 *      4. call OS driver semaphore create and returns a driver sema id.
 *      5. allocate a handle for semaphore.
 *
 * Inputs:
 *  @param    e_type		either X_SEMA_TYPE_BINARY or X_SEMA_TYPE_MUTEX.
 *  @param    ui4_init_value	inital value of this semaphore.
 *
 * Outputs:
 *  @param    *ph_sema_hdl	pointer to hold returned semaphore handle.
 *                   		the handle is valid only if this API returns OSR_OK.
 *
 *  @retval    OSR_OK		routine was successful. 
 *  @retval    OSR_NOT_INIT	semaphore module has not been initiated. 
 *  @retval    OSR_INV_ARG	an error in the arguments of this API.
 *  @retval    OSR_NO_RESOURCE	no resources (memory).
 *  @retval    OSR_FAIL		OS driver related errors.
 *  @retval    OSR_INV_HANDLE	invalid handle.
 *  @retval    OSR_OUT_OF_HANDLES  no handle.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_sema_create(HANDLE_T      *ph_sema_hdl,        /* semaphore handle */
              SEMA_TYPE_T   e_type,
              UINT32        ui4_init_value);

//-------------------------------------------------------------------------
/**
 *      This API deletes a semaphore and its handle. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. parse argumets.
 *      3. call OS driver semaphore delete.
 *      4. free sema handle.
 *
 *  @param     h_sema_hdl	semaphore handle returned via x_sema_create().
 *
 * Returns:
 *  @retval    OSR_OK		routine was successful. 
 *  @retval    OSR_INV_ARG	an error in the arguments of this API.
 *  @retval    OSR_FAIL		OS driver related errors.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 *  @retval    OSR_DELETED	semaphore has been deleted.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_sema_delete(HANDLE_T h_sema_hdl);          /* semaphore handle */

//-------------------------------------------------------------------------
/**
 *      This API locks (acquires) a sema. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. parse argumets.
 *      3. get handle type and check its validity.
 *      4. get sema struct and if sema is type of MUTEX, check if the
 *         calling thread owns the sema.
 *      5. call OS driver semaphore lock.
 *      6. if sema is of type MUTEX, register as the owner of sema if no
 *         ownership is established.
 *
 * Inputs:
 *  @param     h_sema_hdl	semaphore handle returned via x_sema_create().
 *  @param     e_option		either X_SEMA_OPTION_WAIT or X_SEMA_OPTION_NOWAIT.
 *
 * Returns:
 *  @retval    OSR_OK		routine was successful. 
 *  @retval    OSR_NOT_INIT	semaphore module has not been initiated. 
 *  @retval    OSR_INV_ARG	an error in the arguments of this API.
 *  @retval    OSR_FAIL		OS driver related errors.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 *  @retval    OSR_WOULD_BLOCK	if eoption is X_SEMA_OPTION_NOWAIT and the calling
 *                       	thread would have been blocked to lock the sema.
 *  @retval    OSR_DELETED	semaphore has been deleted.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_sema_lock(HANDLE_T h_sema_hdl,        /* semaphore handle */
            SEMA_OPTION_T e_option);

//-------------------------------------------------------------------------
/**
 *      This API locks (acquires) a sema. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. get handle type and check its validity.
 *      3. get sema struct and if sema is type of MUTEX, check if the
 *         calling thread owns the sema.
 *      4. call OS driver semaphore lock timeout API.
 *      5. if sema is of type MUTEX, register as the owner of sema if no
 *         ownership is established.
 *
 * Inputs:
 *  @param    h_sema_hdl	semaphore handle returned via x_sema_create().
 *  @param    ui4_time		time duration in ms before thie API bails out.
 *
 *  @retval    OSR_OK		routine was successful. 
 *  @retval    OSR_NOT_INIT	semaphore module has not been initiated. 
 *  @retval    OSR_FAIL		OS driver related errors.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 *  @retval    OSR_TIMEOUT	failed to lock the semaphore within "ui4_time" duration.
 *  @retval    OSR_DELETED	semaphore has been deleted.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_sema_lock_timeout(HANDLE_T h_sema_hdl,        /* semaphore handle */
                    UINT32   ui4_time);

//-------------------------------------------------------------------------
/**
 *      This API unlocks (releases) a semaphore. it performs
 *      1. check if semaphore mudule is initiated. 
 *      2. get handle type and check its validity.
 *      3. get sema struct and if sema is type of MUTEX, check if the
 *         calling thread owns the sema and decrement refcount if so and return.
 *      5. call OS driver semaphore unlock if refcount reaches 0 on MUTEX sema.
 *      6. if sema is of type MUTEX, de-register as thalling thread as owner of
 *         sema.
 *
 * Inputs:
 *  @param     h_sema_hdl	semaphore handle returned via x_sema_create().
 *
 * Returns:
 *  @retval    OSR_OK		routine was successful. 
 *  @retval    OSR_NOT_INIT	semaphore module has not been initiated. 
 *  @retval    OSR_INV_ARG	an error in the arguments of this API.
 *  @retval    OSR_FAIL		OS driver related errors.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_sema_unlock(HANDLE_T h_sema_hdl);          /* semaphore handle */

/*************************************************************************/
//
//				timer APIs 
/*************************************************************************/
//-------------------------------------------------------------------------
/**
 *  This API returns the system tick count.
 *  @retval   The system tick value 
 */
//-------------------------------------------------------------------------
EXTERN UINT32 x_os_get_sys_tick (VOID);

//-------------------------------------------------------------------------
/** 
 *  This API returns the period (in ms) between system tick.
 *  @retval   The period (in ms) between system tick. 
 */
//-------------------------------------------------------------------------
EXTERN UINT32 x_os_get_sys_tick_period (VOID);

//-------------------------------------------------------------------------
/**
 *      this API creates timer and its handle. it performs
 *      1. check if timer mudule is initiated.
 *      2. parse argumets.
 *      3. allocate memory for timer struct.
 *      4. initialize, allocate, and link a messaage haandle.
 *      4. initialize timer struct.
 *
 * Inputs: -
 *
 * Outputs:
 *  @param    *ph_timer_hdl	pointer to hold returned timer handle.
 *                     		the handle is valid only if this API returns OSR_OK.
 *                     		the handle is used to access the created timer.
 *
 *  @retval    OSR_OK		routine was successful.
 *  @retval    OSR_NOT_INIT	timer module has not been initiated.
 *  @retval    OSR_INV_ARG	an error in the arguments of this API.
 *  @retval    OSR_NO_RESOURCE	no resources (memory).
 *  @retval    OSR_INV_HANDLE	invalid handle.
 *  @retval    OSR_OUT_OF_HANDLES no handle.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_timer_create(HANDLE_T *ph_timer_hdl);      /* timer handle */

//-------------------------------------------------------------------------
/**
 *      This API starts a timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. parse argumets.
 *      3. check handle validity.
 *      4. fill up timer struct and put timer struct in the global timer
 *         doubly linked list.
 *
 *  @param    h_timer_hdl	timer handle returned from x_timer_create().
 *  @param    ui4_delay	timer pops after this delay has passed.
 *  @param    e_flags		either X_TIMER_FLAG_ONCE or X_TIMER_FLAG_REPEAT.
 *  @param    pf_cb		callback function to be executed after "ui4_delay" had passed.
 *  @param    *pv_tag		argument to callback function pf_cb.
 *
 *  @retval    OSR_OK		routine was successful.
 *  @retval    OSR_NOT_INIT	timer module has not been initiated.
 *  @retval    OSR_INV_ARG	an error in the arguments of this API.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 */
//-------------------------------------------------------------------------
INT32 x_timer_start(HANDLE_T          h_timer_hdl,    /* timer handle */
                    UINT32            ui4_delay,
                    TIMER_FLAG_T      e_flags,
                    x_os_timer_cb_fct pf_cb,    /* callback function */
                    VOID*             pv_tag);

//-------------------------------------------------------------------------
/**
 *      This API stops a timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. check handle validity.
 *      3. remove timer struct from the global timer doubly linked list.
 *
 *  @param    h_timer_hdl	timer handle returned from x_timer_create().
 *
 *  @retval    OSR_OK		routine was successful.
 *  @retval    OSR_NOT_INIT	timer module has not been initiated.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_timer_stop(HANDLE_T h_timer_hdl);

//-------------------------------------------------------------------------
/** 
 *      This API deleted a timer and its handle. it performs
 *      1. check if timer mudule is initiated.
 *      2. check handle validity.
 *      3. remove timer struct from the global timer doubly linked list.
 *      4. free timer handle.
 *
 *  @param    h_timer_hdl	timer handle returned from x_timer_create().
 *
 *  @retval    OSR_OK		routine was successful.
 *  @retval    OSR_NOT_INIT	timer module has not been initiated.
 *  @retval    OSR_INV_HANDLE	an error in handle operation.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_timer_delete (HANDLE_T h_timer_hdl);

/*************************************************************************/
//
//				memory APIs 
/*************************************************************************/
//-------------------------------------------------------------------------
/**
 *      This API allocates memory from default partition. it performs
 *      1. check if default partition exists (created via os_main()).
 *      2. call partition allocate funtiocn pf_alloc().
 *
 *  @param  z_size   	requested memory size.
 *  @retval non NULL: 	routine was successful.
 *  @retval NULL: 	routine failed.
 */
//-------------------------------------------------------------------------
EXTERN VOID *x_mem_alloc(SIZE_T z_size);

//-------------------------------------------------------------------------
/**
 *      This API frees a memory block. it performs
 *      1. check validity of pv_mem.
 *      2. call os_mem_free() to actual free the memory block.
 *
 *  @param  pv_mem   	pointer to a memory block to be freed.
 *  @retval   void
 */
//-------------------------------------------------------------------------
EXTERN VOID x_mem_free(VOID *pv_mem);

//-------------------------------------------------------------------------
/**
 *      This API copies ui1_c to pv_to up to z_l bytes. it performs
 *      1. parse argument and return ps_to on error.
 *      2. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies. 
 *  @param  pv_to   	memory block to be copied to.
 *  @param  ui1_c   	data to populate to ps_to.
 *  @param  z_l		bytes to be copied 
 *  @retval pointer to pv_to 
 */
//-------------------------------------------------------------------------
EXTERN VOID *x_memset(VOID *pv_to, UINT8 ui1_c, SIZE_T z_l);

//-------------------------------------------------------------------------
/**
 *      This API copies pv_from to pv_to up to z_l bytes. it performs
 *      1. parse argument and return pv_to on error.
 *      2. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies. 
 *
 *  @param  pv_to   	memory block to be copied to.
 *  @param  pv_from   	memory block to be copied from.
 *  @param  z_l   	bytes to be copied.
 *  @retval pointer to pv_to 
 */
//-------------------------------------------------------------------------
EXTERN VOID *x_memcpy(VOID *pv_to, const VOID *pv_from, SIZE_T z_l);


/*************************************************************************/
//
//				string APIs
/*************************************************************************/

//-------------------------------------------------------------------------
/**
 *      This API compares 2 strings ps_s1 and ps_s2.
 *
 *  @param  ps_s1	null ("\0") terminated character string.
 *  @param  ps_s2	null ("\0") terminated character string.
 *  @retval 0		ps_s1 is identical to ps_s2. 
 *  @retval 1		ps_s1 is greater than ps_s2. 
 *  @retval -1 		ps_s1 is less than ps_s2.
 */
//-------------------------------------------------------------------------
EXTERN INT32 x_strcmp(const CHAR *ps_s1, const CHAR *ps_s2);

//-------------------------------------------------------------------------
/**
 *      This API returns length of a character string. it performs
 *      1. parse argument and return 0 if it is a NULL string.
 *      2. move temp pointer to null character.
 *      3. return distance between temp pointer and original pointer.
 *
 *  @param  ps_s	null ("\0") terminated character string.
 *  @retval 		length of character string excluding null ("\0") character. 
 */
//-------------------------------------------------------------------------
EXTERN SIZE_T x_strlen(const CHAR *ps_s);

#endif /*_OSAI_API_H_*/
