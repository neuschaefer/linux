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
 * $RCSfile: x_driver_os.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains OS driver specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_DRIVER_OS_H_
#define _X_DRIVER_OS_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_os.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* OS driver return values */

#define OSR_DRV_THREAD_ACTIVE  ((INT32)   2)
#define OSR_DRV_WOULD_BLOCK    ((INT32)   1)
#define OSR_DRV_OK             ((INT32)   0)
#define OSR_DRV_EXIST          ((INT32)  -1)
#define OSR_DRV_INV_ARG        ((INT32)  -2)
#define OSR_DRV_TIMEOUT        ((INT32)  -3)
#define OSR_DRV_NO_RESOURCE    ((INT32)  -4)
#define OSR_DRV_NOT_EXIT       ((INT32)  -5)
#define OSR_DRV_NOT_FOUND      ((INT32)  -6)
#define OSR_DRV_INVALID        ((INT32)  -7)
#define OSR_DRV_NOT_INIT       ((INT32)  -8)
#define OSR_DRV_DELETED        ((INT32)  -9)
#define OSR_DRV_TOO_MANY       ((INT32) -10)
#define OSR_DRV_FAIL           ((INT32) -11)


typedef VOID (*x_os_drv_thread_main_fct) (const CHAR* ps_name);

typedef VOID (*x_os_drv_isr_fct) (UINT16 ui2_vector_id);

typedef VOID* (*x_os_drv_memcpy) (VOID*        pv_to,
                                  const VOID*  pv_from,
                                  SIZE_T       z_len);

typedef VOID* (*x_os_drv_memmove) (VOID*        pv_to,
                                   const VOID*  pv_from,
                                   SIZE_T       z_len);

typedef VOID* (*x_os_drv_memset) (VOID*   pv_mem,
                                  UINT8   ui1_c,
                                  SIZE_T  z_len);

typedef struct
{
    x_os_drv_memcpy   pf_drv_memcpy;
    x_os_drv_memmove  pf_drv_memmove;
    x_os_drv_memset   pf_drv_memset;
}   DRV_MEM_FUNCTIONS_T;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/* Thread API's */
extern INT32 x_os_drv_thread_create (VOID**                    ppv_th_id,
                                     const CHAR*               ps_name,
                                     SIZE_T                    z_stacksize,
                                     UINT8                     ui1_pri,
                                     UINT16                    ui2_flags,
                                     VOID*                     pv_tag,
                                     x_os_drv_thread_main_fct  pf_main);

extern INT32 x_os_drv_thread_delete (VOID*  pv_th_id);

extern VOID x_os_drv_thread_exit (VOID);

extern VOID x_os_drv_thread_delay (UINT32  ui4_delay);

extern INT32 x_os_drv_thread_get_pri (VOID*   pv_th_id,
                                      UINT8*  pui1_pri);

extern INT32 x_os_drv_thread_set_pri (VOID*  pv_th_id,
                                      UINT8  ui1_pri);

extern INT32 x_os_drv_thread_set_affinity (VOID   *pv_th_id,
                                           UINT32  cpu_mask);


extern VOID x_os_drv_thread_suspend (VOID);

extern INT32 x_os_drv_thread_resume (VOID*  pv_th_id);

extern INT32 x_os_drv_thread_self (VOID**  pv_th_id,
                                   VOID**  ppv_tag);

extern VOID * x_os_drv_thread_self_id (VOID);

extern INT32 x_os_drv_thread_stack_stats (VOID*    pv_th_id,
                                          SIZE_T*  pz_alloc_stack,
                                          SIZE_T*  pz_max_used_stack);


/* Semaphore API's */
extern INT32 x_os_drv_sema_create (VOID**  ppv_sema_id,
                                   UINT32  ui4_init_value);

extern INT32 x_os_drv_sema_delete (VOID*  pv_sema_id);

extern INT32 x_os_drv_sema_lock (VOID*          pv_sema_id,
                                 SEMA_OPTION_T  e_option);

extern INT32 x_os_drv_sema_lock_timeout (VOID*   pv_sema_id,
                                         UINT32  ui4_time);

extern INT32 x_os_drv_sema_unlock (VOID*  pv_sema_id);


/* Critical section API's */
extern CRIT_STATE_T x_os_drv_crit_start (VOID);

extern VOID x_os_drv_crit_end (CRIT_STATE_T  t_old_level);


/* Timer / tick API's */
extern UINT32 x_os_drv_get_tick_period (VOID);

extern UINT32 x_os_drv_get_fine_tick (VOID);

extern VOID x_os_drv_tick (VOID);

extern VOID x_os_drv_get_time (UINT32 *pui4Seconds, UINT32 *pui4Micros);


/* ISR API's */
extern INT32 x_os_drv_reg_isr(UINT16             ui2_vector_id,
                              x_os_drv_isr_fct   pf_isr,
                              x_os_drv_isr_fct*  ppf_old_isr);

/* To regiser driver-specific memory functions */
extern INT32
x_os_drv_reg_mem_functions (const DRV_MEM_FUNCTIONS_T*  pt_functions);

extern VOID* x_os_drv_memcpy_dma (VOID*        pv_to,
                                  const VOID*  pv_from,
                                  SIZE_T       z_len);

extern VOID x_os_drv_set_timestamp(CHAR *szString);

#endif /* _X_DRIVER_OS_H_ */

