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
 * $RCSfile: u_os.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains exported OS data definitions.
 *
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_os.h
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *           This header file contains exported OS data definitions.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_OS_H_
#define _U_OS_H_
/*!
 * @addtogroup    groupMW_OSAI
 * @brief    This header file contains OSAI exported constants, macros, and types.
 * 
 * @see groupMW_OSAI
 *
 * @{
 * */

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* OS API return values */

#define OSR_THREAD_ACTIVE         ((INT32)   3)
#define OSR_MEM_NOT_FREE          ((INT32)   2)
#define OSR_WOULD_BLOCK           ((INT32)   1)
#define OSR_OK                    ((INT32)   0)
#define OSR_EXIST                 ((INT32)  -1)
#define OSR_INV_ARG               ((INT32)  -2)
#define OSR_TIMEOUT               ((INT32)  -3)
#define OSR_NO_RESOURCE           ((INT32)  -4)
#define OSR_NOT_EXIST             ((INT32)  -5)
#define OSR_NOT_FOUND             ((INT32)  -6)
#define OSR_INVALID               ((INT32)  -7)
#define OSR_NOT_INIT              ((INT32)  -8)
#define OSR_DELETED               ((INT32)  -9)
#define OSR_TOO_MANY              ((INT32) -10)
#define OSR_TOO_BIG               ((INT32) -11)
#define OSR_DUP_REG               ((INT32) -12)
#define OSR_NO_MSG                ((INT32) -13)
#define OSR_INV_HANDLE            ((INT32) -14)
#define OSR_FAIL                  ((INT32) -15)
#define OSR_OUT_BOUND             ((INT32) -16)
#define OSR_NOT_SUPPORT           ((INT32) -17)
#define OSR_BUSY                  ((INT32) -18)
#define OSR_OUT_OF_HANDLES        ((INT32) -19)
#define OSR_AEE_OUT_OF_RESOURCES  ((INT32) -20)
#define OSR_AEE_NO_RIGHTS         ((INT32) -21)
#define OSR_CANNOT_REG_WITH_CLI   ((INT32) -22)
#define OSR_DUP_KEY               ((INT32) -23)
#define OSR_KEY_NOT_FOUND         ((INT32) -24)

/* handle types for OS modules */
#define HT_GROUP_OS_MSGQ            ((HANDLE_TYPE_T) HT_GROUP_OS)
#define HT_GROUP_OS_BIN_SEMA        ((HANDLE_TYPE_T) HT_GROUP_OS + 1)
#define HT_GROUP_OS_MUTEX_SEMA      ((HANDLE_TYPE_T) HT_GROUP_OS + 2)
#define HT_GROUP_OS_COUNTING_SEMA   ((HANDLE_TYPE_T) HT_GROUP_OS + 3)
#define HT_GROUP_OS_THREAD          ((HANDLE_TYPE_T) HT_GROUP_OS + 4)
#define HT_GROUP_OS_TIMER           ((HANDLE_TYPE_T) HT_GROUP_OS + 5)
#define HT_GROUP_OS_MEMORY          ((HANDLE_TYPE_T) HT_GROUP_OS + 6)
#define HT_GROUP_OS_EVENT_GROUP     ((HANDLE_TYPE_T) HT_GROUP_OS + 7)


#define X_SEMA_STATE_LOCK   ((UINT32) 0)
#define X_SEMA_STATE_UNLOCK ((UINT32) 1)

typedef UINT32 CRIT_STATE_T;

/*------------------------------------------------------------------*/
/*! @enum   SEMA_OPTION_T
 *  @brief  Enumeration for semaphore lock option.
 *  @code
 *  typedef enum                
 *  {                          
 *      X_SEMA_OPTION_WAIT = 1,
 *      X_SEMA_OPTION_NOWAIT   
 *  }   SEMA_OPTION_T;         
 *  @endcode
 *  @li@c  X_SEMA_OPTION_WAIT       - Indicates that accessing to a semaphore can 
 *                                    be blocked.
 *  @li@c  X_SEMA_OPTION_NOWAIT     - Indicates that accessing to a semaphore cannot 
 *                                    be blocked.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_SEMA_OPTION_WAIT = 1,
    X_SEMA_OPTION_NOWAIT
}   SEMA_OPTION_T;

/*------------------------------------------------------------------*/
/*! @enum   SEMA_TYPE_T
 *  @brief  Enumeration for semaphore type.
 *  @code
 *  typedef enum               
 *  {                          
 *      X_SEMA_TYPE_BINARY = 1,
 *      X_SEMA_TYPE_MUTEX,     
 *      X_SEMA_TYPE_COUNTING   
 *  }   SEMA_TYPE_T;           
 *  @endcode
 *  @li@c  X_SEMA_TYPE_BINARY       - Indicates that the semaphore to be created is 
 *                                    a binary semaphore.
 *  @li@c  X_SEMA_TYPE_MUTEX        - Indicates that the semaphore to be created is 
 *                                    a mutex semaphore.
 *  @li@c  X_SEMA_TYPE_COUNTING     - Indicates that the semaphore to be created is 
 *                                    a counting semaphore.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_SEMA_TYPE_BINARY = 1,
    X_SEMA_TYPE_MUTEX,
    X_SEMA_TYPE_COUNTING
}   SEMA_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum   MSGQ_OPTION_T
 *  @brief  Enumeration for Message Queue lock option.
 *  @code
 *  typedef enum                
 *  {                          
 *      X_MSGQ_OPTION_WAIT = 1,
 *      X_MSGQ_OPTION_NOWAIT   
 *  }   MSGQ_OPTION_T;         
 *  @endcode
 *  @li@c  X_MSGQ_OPTION_WAIT       - Indicates that accessing to a message queue 
 *                                    can be blocked.
 *  @li@c  X_MSGQ_OPTION_NOWAIT     - Indicates that accessing to a message queue 
 *                                    cannot be blocked.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_MSGQ_OPTION_WAIT = 1,
    X_MSGQ_OPTION_NOWAIT
}   MSGQ_OPTION_T;

/*------------------------------------------------------------------*/
/*! @enum   TIMER_FLAG_T
 *  @brief  Enumeration for Timer Flag option.
 *  @code
 *  typedef enum                 
 *  {                           
 *      X_TIMER_FLAG_ONCE   = 1,
 *      X_TIMER_FLAG_REPEAT     
 *  }   TIMER_FLAG_T;           
 *  @endcode
 *  @li@c  X_TIMER_FLAG_ONCE        - Indicates that the timer facility is one shot.
 *  @li@c  X_TIMER_FLAG_REPEAT      - IIndicates that the timer facility is repeated.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_TIMER_FLAG_ONCE   = 1,
    X_TIMER_FLAG_REPEAT
}   TIMER_FLAG_T;


typedef UINT32 EV_GRP_EVENT_T;

/*------------------------------------------------------------------*/
/*! @enum   TIMER_FLAG_T
 *  @brief  Enumeration for Event Groupt Operation.
 *  @code
 *  typedef enum                
 *  {                           
 *      X_EV_OP_CLEAR       = 1,
 *      X_EV_OP_AND,            
 *      X_EV_OP_OR,             
 *      X_EV_OP_AND_CONSUME,    
 *      X_EV_OP_OR_CONSUME      
 *  }   EV_GRP_OPERATION_T;     
 *  @endcode
 *  @li@c  X_EV_OP_CLEAR            - 
 *  @li@c  X_EV_OP_AND              - 
 *  @li@c  X_EV_OP_OR               - 
 *  @li@c  X_EV_OP_AND_CONSUME      - 
 *  @li@c  X_EV_OP_OR_CONSUME       - 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    X_EV_OP_CLEAR       = 1,
    X_EV_OP_AND,
    X_EV_OP_OR,
    X_EV_OP_AND_CONSUME,
    X_EV_OP_OR_CONSUME
}   EV_GRP_OPERATION_T;


/*---------------------------------------------------------------------------*/
/*! @brief The callback routine is invoked when the specified timer h_timer_hdl 
 *         is expired. Please refer to x_timer_start() for scheduling a timer. 
 *         The pv_tag is only significant to the callback routine and is passed 
 *         in as the second argument.
 *
 *  @param [in] pt_tm_handle    - handle returned from x_timer_create().
 *  @param [in] pv_tag          - argument to this callback routine.
 *
 *  @note
 *  @see   c_timer_start
 *
 *  @return
 *  @retval
 */
/*---------------------------------------------------------------------------*/
typedef VOID (*x_os_timer_cb_fct) (HANDLE_T  pt_tm_handle,
                                   VOID*     pv_tag);

/*---------------------------------------------------------------------------*/
/*! @brief The routine is executed when a thread is created in the system. 
 *         Please refer to x_thread_create() on creating a thread.
 *
 *  @param [in] pv_tag          - argument to this callback routine.
 *
 *  @note
 *  @see   x_thread_create
 *
 *  @return
 *  @retval
 */
/*---------------------------------------------------------------------------*/
typedef VOID (*x_os_thread_main_fct) (VOID*  pv_arg);

/*---------------------------------------------------------------------------*/
/*! @brief This callback function is executed when OS wants to delete thread 
 *         private data 
 *
 *  @param [in] ui4_key         - key associated with the private data
 *  @param [in] pv_pvt          - private data pointer
 *
 *  @note
 *  @see
 *
 *  @return
 *  @retval
 */
/*---------------------------------------------------------------------------*/
typedef VOID (*x_os_thread_pvt_del_fct) (UINT32  ui4_key,
                                         VOID*   pv_pvt);

/*---------------------------------------------------------------------------*/
/*! @brief This interrupt service routine (ISR) is executed when an interrupt 
 *         interrupts the system. This ISR is registered with a vector id. 
 *
 *  @param [in] ui2_vector_id   - vector id for this ISR
 *
 *  @note
 *  @see   x_reg_isr
 *
 *  @return
 *  @retval
 */
/*---------------------------------------------------------------------------*/
typedef VOID (*x_os_isr_fct) (UINT16  ui2_vector_id);

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_OSAI*/
/*----------------------------------------------------------------------------*/

#endif /* _U_OS_H_ */
