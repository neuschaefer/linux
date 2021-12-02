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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: cb_low.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file cb_low.c
 *  This file implements low-layer support of callback interface.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "cb_tim.h"
#include "cb_low.h"
#include "x_linux.h"
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

DECLARE_WAIT_QUEUE_HEAD(_rWq);
static DLIST_T(_CB_CALLBACK_EVENT_T) _qCbEvent;

/* Event type to callback thread ownership array */
static pid_t _arCbId[CB_FCT_NUM];

static BOOL fgInit=FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Add callback event into event queue.
 *  @param  prCbEv  Pointer to callback event.
 *  @retval CBR_OK          Success.
 *  @retval CBR_ERR_SYS     Internal system error.
 */
//-----------------------------------------------------------------------------
static INT32 _CbPutEvent(CB_CALLBACK_EVENT_T *prCbEv)
{
    CRIT_STATE_T rState;

    rState = x_crit_start();
    DLIST_INSERT_TAIL(prCbEv, &_qCbEvent, rLink);
    x_crit_end(rState);

    wake_up_interruptible_all(&_rWq);

    return CBR_OK;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Initialize callback support.
 */
//-----------------------------------------------------------------------------
#if defined(CC_FAST_INIT)
extern int  mt53xx_drvset_init(void);
#endif
void _CB_Init(void)
{
    CRIT_STATE_T rState;

    #if defined(CC_FAST_INIT)
    mt53xx_drvset_init();
    #endif
    #if defined(CC_SUPPORT_STR)
    mt53xx_STR_init();
    #endif
    if (fgInit)
    {
        return;
    }
    
    rState = x_crit_start();
    fgInit = TRUE;
    DLIST_INIT(&_qCbEvent);
    x_crit_end(rState);
}

//-----------------------------------------------------------------------------
/** To enable/register a callback function with the related ID.
 *  @param  eFctId  Callback function ID to enable.
 *  @param  i4SubId Callback function Sub ID.
 *  @return The return value of related callback registration function.
 */
//-----------------------------------------------------------------------------
INT32 _CB_RegCbFct(CB_FCT_ID_T eFctId, INT32 i4SubId, pid_t rCbPid)
{
    BOOL fgWakeUp=FALSE;
    CRIT_STATE_T rState;

    rState = x_crit_start();
    if (eFctId < CB_FCT_NUM)
    {
        if ((_arCbId[eFctId] != (pid_t)0) &&
            (_arCbId[eFctId] != rCbPid))
        {
            fgWakeUp = TRUE;
        }
            
        _arCbId[eFctId] = rCbPid;
    }
    x_crit_end(rState);
    
    if (apfnCbRegIf[eFctId] != NULL)
    {
        return apfnCbRegIf[eFctId](i4SubId);
    }

    if (fgWakeUp)
    {
        wake_up_interruptible_all(&_rWq);
    }
    
    /*
    printk("*** Thread 0x%x registers callback (0x%x, 0x%x)\n",
           (INT32)current, (INT32)eFctId, (INT32)i4SubId);
    */

    return CBR_OK;
}

pid_t _CB_GetPid(CB_FCT_ID_T eFctId)
{
    if(eFctId < CB_FCT_NUM)
    {
        return _arCbId[eFctId];
    }
    else
    {
        return 0;
    }
}


//-----------------------------------------------------------------------------
/** To unregister a callback function with the related ID.
 *  @param  eFctId  Callback function ID to enable.
 *  @param  i4SubId Callback function Sub ID.
 *  @return The return value of related callback unregistration function.
 */
//-----------------------------------------------------------------------------
INT32 _CB_UnRegCbFct(CB_FCT_ID_T eFctId, INT32 i4SubId, pid_t rCbPid)
{
    CRIT_STATE_T rState;

    rState = x_crit_start();
    if ((eFctId < CB_FCT_NUM) && (rCbPid == _arCbId[eFctId]))
    {
        _arCbId[eFctId] = 0;
    }
    x_crit_end(rState);
    
    return CBR_OK;
}


//-----------------------------------------------------------------------------
/** Add callback event into event queue.
 *  @param  eFctId      Callback function ID to notify.
 *  @param  i4TagSize   Data size to callback.
 *  @param  pvTag       Data to callback.
 *  @retval CBR_OK          Success.
 *  @retval CBR_NO_MEM      Out of memory.
 */
//-----------------------------------------------------------------------------
INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag)
{
    CB_CALLBACK_EVENT_T *prCbEv;
    UINT32 flags;

    if (!fgInit)
    {
        printk("Error! CB not yet initialized!\n");
        return CBR_ERR_SYS;
    }

    if (in_interrupt())
    {
        flags = GFP_ATOMIC;
    }
    else
    {
        flags = GFP_KERNEL;
    }

    prCbEv = (CB_CALLBACK_EVENT_T *)
             kmalloc(sizeof(CB_CALLBACK_EVENT_T) + i4TagSize, flags);

    if (prCbEv == NULL)
    {
        return CBR_NO_MEM;
    }

    prCbEv->rGetCb.eFctId = eFctId;
    prCbEv->rGetCb.i4TagSize = i4TagSize;
    memcpy((void *)((UINT32)prCbEv + sizeof(CB_CALLBACK_EVENT_T)), pvTag,
           i4TagSize);

    _CbPutEvent(prCbEv);

    return CBR_OK;
}

//-----------------------------------------------------------------------------
/** Get the first event from event queue.
 *  @return If success and has event in queue, pointer of the callback event.\n
 *          Otherwise, NULL.
 */
//-----------------------------------------------------------------------------
CB_CALLBACK_EVENT_T *_CB_GetEvent(void)
{
    CB_CALLBACK_EVENT_T *prLast=NULL;
    CRIT_STATE_T rState;
    BOOL fgFound=FALSE;

    rState = x_crit_start();
    DLIST_FOR_EACH(prLast, &_qCbEvent, rLink)
    {
        if (!prLast)
        {
            break;
        }

        if (prLast->rGetCb.eFctId >= CB_FCT_NUM)
        {
            break;
        }
        
        if (current->tgid == _arCbId[prLast->rGetCb.eFctId])
        {
            fgFound = TRUE;
            break;
        }

        if (prLast->rLink.pt_next == NULL)
        {
            break;
        }
    }
    if (fgFound && (prLast != NULL))
    {
        DLIST_REMOVE(prLast, &_qCbEvent, rLink);
    }
    x_crit_end(rState);

    if (fgFound)
    {
        return prLast;
    }
    else
    {
        return NULL;
    }
}

//-----------------------------------------------------------------------------
/** Put the current thread into wait queue.
 */
//-----------------------------------------------------------------------------
void _CB_PutThdIntoWQ(void)
{
    DECLARE_WAITQUEUE(wait, current);

    add_wait_queue(&_rWq, &wait);
    set_current_state(TASK_INTERRUPTIBLE);
    schedule();
    set_current_state(TASK_RUNNING);
    remove_wait_queue(&_rWq, &wait);
}

//-----------------------------------------------------------------------------
/** Clear specified PID entries in event list
 */
//-----------------------------------------------------------------------------
void _CB_ClearPendingEventEntries(void)
{
    CB_CALLBACK_EVENT_T *prLast=NULL;
    CRIT_STATE_T rState;
    BOOL fgFound;

    do {
        fgFound = FALSE;
        
        rState = x_crit_start();
        DLIST_FOR_EACH(prLast, &_qCbEvent, rLink)
        {
            if (!prLast)
            {
                break;
            }
            
            if (prLast->rGetCb.eFctId >= CB_FCT_NUM)
            {
                break;
            }
            
            if (current->tgid == _arCbId[prLast->rGetCb.eFctId])
            {
                fgFound = TRUE;
                break;
            }
        
            if (prLast->rLink.pt_next == NULL)
            {
                break;
            }
        }
        if (fgFound && (prLast != NULL))
        {
            DLIST_REMOVE(prLast, &_qCbEvent, rLink);
        }
        x_crit_end(rState);
    } while (fgFound);
}

//-----------------------------------------------------------------------------
/** Clear entries with specified eFctId and PID in event list
 */
//-----------------------------------------------------------------------------
void _CB_ClearPendingFctEventEntries(CB_FCT_ID_T eFctId)
{
    CB_CALLBACK_EVENT_T *prLast=NULL;
    CRIT_STATE_T rState;
    BOOL fgFound;

    do {
        fgFound = FALSE;
        
        rState = x_crit_start();
        DLIST_FOR_EACH(prLast, &_qCbEvent, rLink)
        {
            if (!prLast)
            {
                break;
            }
            
            if (prLast->rGetCb.eFctId >= CB_FCT_NUM)
            {
                break;
            }
            
            if (current->tgid == _arCbId[prLast->rGetCb.eFctId])
            {
                if (prLast->rGetCb.eFctId == eFctId)
                {
                    fgFound = TRUE;
                    break;
                }
            }
        
            if (prLast->rLink.pt_next == NULL)
            {
                break;
            }
        }
        if (fgFound && (prLast != NULL))
        {
            DLIST_REMOVE(prLast, &_qCbEvent, rLink);
        }
        x_crit_end(rState);
    } while (fgFound);
}

//-----------------------------------------------------------------------------
/** Clear specified PID entries in CB PID table
 */
//-----------------------------------------------------------------------------
void _CB_ClearCbIdArray(void)
{
    INT32 i;
    CRIT_STATE_T rState;

    rState = x_crit_start();
    for (i=0; i<CB_FCT_NUM; i++)
    {
        if (current->tgid == _arCbId[i])
        {
            _arCbId[i] = (pid_t)0;
        }
    }
    x_crit_end(rState);
}
