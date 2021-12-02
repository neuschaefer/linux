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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: cb_if.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file cb_if.c
 *  This file implements exported APIs of callback library.
 */


/*-----------------------------------------------------------------------------
 Include files
-----------------------------------------------------------------------------*/
#include "mw_adpt_cb_if.h"
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "os/inc/x_os.h"
#include "handle/c_handle.h"
#include "include/ioctl_inc/ioctl_param.h"
/*#include <semaphore.h>*/
#include "x_rm.h"
#include "include/cb_inc/cb_param.h"

#ifndef PRT_DBG
#define PRT_DBG(x, ...)  
#endif

#define CB_STRUCT_MAX_SIZE (1024)

/*-----------------------------------------------------------------------------
 Type definitions
-----------------------------------------------------------------------------*/
/* Callback function item. Used to maintain callback function list.*/
typedef struct _CB_ADPT_FCT_ITEM_T
{
    SLIST_ENTRY_T(_CB_ADPT_FCT_ITEM_T)   rLink;

    CB_FCT                          pfnCbFct;

    int                             isSync;
} CB_ADPT_FCT_ITEM_T;


/*-----------------------------------------------------------------------------
 Static variables
-----------------------------------------------------------------------------*/

static HANDLE_T hReadMutex;
/* If callback library initialized.*/
static int          _i4Init = 0;

/* Callback thread id.*/
static pthread_t    _thdId = 0;

static int fdioctrl;
/* Callback function list array.*/
static SLIST_T(_CB_ADPT_FCT_ITEM_T)   arFctList[CB_FCT_NUM];

/* Mutex for shared resources locking*/
static pthread_mutex_t _rCbMutex;

/* Mutex attributes*/
static pthread_mutexattr_t _rCbMutexAttr;

/*-----------------------------------------------------------------------------
 Static functions
-----------------------------------------------------------------------------*/
static int _i4MutexInit(void)
{
    int i4Ret;

    i4Ret = pthread_mutexattr_init(&_rCbMutexAttr);
    if (i4Ret)
    {
        return i4Ret;
    }

    i4Ret = pthread_mutexattr_settype(&_rCbMutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
    if (i4Ret)
    {
        return i4Ret;
    }
    
    return (pthread_mutex_init(&_rCbMutex, &_rCbMutexAttr));
}


static int _i4MutexLock(void)
{
    return (pthread_mutex_lock(&_rCbMutex));
}


static int _i4MutexUnlock(void)
{
    return (pthread_mutex_unlock(&_rCbMutex));
}


/*-----------------------------------------------------------------------------
 ** Callback thread.
 *  @param  ptr     Thread parameter.
 *  @return Never return.
-----------------------------------------------------------------------------*/
//static void * p_last_cb_func;
static CB_FCT p_last_cb_func;
static void _cbThd(void *ptr)
{
    CB_ADPT_FCT_ITEM_T*        prFctItem = NULL;
    CB_GET_MWADPT_CALLBACK_T*  prGetCb; 
    /*for callback output case, modify 3-->5: pv_Output, i4_OutSize */
    int i4_data[5]; 
    int i4_ret;

    PRT_DBG("\nmw: Trace _cbThd OK!\n");

    prGetCb = (CB_GET_MWADPT_CALLBACK_T *)malloc(sizeof(CB_GET_MWADPT_CALLBACK_T) + CB_STRUCT_MAX_SIZE);
    if (prGetCb == NULL)
    {
        PRT_DBG("Out of memory!\n");
        return ;
    }
    
    PRT_DBG("\nmw Trace fdioctrl %d  OK!\n", fdioctrl);
    
    while (1)
    {
        if (fdioctrl < 0)
        {
            PRT_DBG("CB Thd: Invalid callback device node!\n");
            break;
        }

        if (ioctl(fdioctrl, CB_GET_CALLBACK_ADPT, prGetCb) >= 0)
        {
            PRT_DBG("\nmw: user get callback from kernel.\n"); 
            if (_i4MutexLock())
            {
                PRT_DBG("CB Thd: pthread mutex lock failed!\n");
            }
            
            if ( prGetCb->eFctId >= CB_FCT_NUM )
            {
                PRT_DBG( "\nmw: prGetCb->eFctId(%d) is overflow!\n", prGetCb->eFctId );
                continue;
            }
            
            SLIST_FOR_EACH(prFctItem, &(arFctList[prGetCb->eFctId]), rLink)
            {
                PRT_DBG("\nmw: SLIST_FOR_EACH.\n"); 
                p_last_cb_func = prFctItem->pfnCbFct;
                i4_ret = prFctItem->pfnCbFct((void *)((unsigned int)prGetCb +
                                             sizeof(CB_GET_MWADPT_CALLBACK_T)));
                

                {
                    i4_data[0] = i4_ret;
                    i4_data[1] = (int)(prGetCb->pvSync);
                    i4_data[2] = prGetCb->fg_sync;
                    i4_data[3] = ( INT32 )( prGetCb->pv_Output ); /*user address is not than 3G*/
                    i4_data[4] = prGetCb->i4_OutSize;
                    ioctl(fdioctrl, CB_FIN_CALLBACK_ADPT, i4_data);
                }
                if (1 == prFctItem->isSync)
                {
                    PRT_DBG("\nmw: one Synchro cb is called.\n");
                }
                else
                {
                    PRT_DBG("\nmw: one Asynchro cb is called.\n");
                }
            }

            if (_i4MutexUnlock())
            {
                PRT_DBG("CB Thd: pthread mutex unlock failed!\n");
            }
        }
    }

    free(prGetCb);
}


/*-----------------------------------------------------------------------------
 Public functions
-----------------------------------------------------------------------------
*/
/*-----------------------------------------------------------------------------
 ** Initialize callback library.
 *  @retval CBR_OK          Success.
 *  @retval CBR_ERR_DEV     Fail to open device node.
 *  @retval CBR_ERR_SYS     Fail to create thread.
 *-----------------------------------------------------------------------------*/

int CB_InitMwAdptLib(void)
{
    int i, i4Ret = CBR_OK;
    /*pthread_attr_t rAttr;
    struct sched_param rParam;
    int i4Pri = 20;

    void * start_addr;*/

    if (_i4Init)
    {
        return CBR_OK;
    }

    if (_i4MutexInit())
    {
        return CBR_ERR_SYS;
    }

    fdioctrl = open("/dev/rmmgr", O_RDWR);
    if (fdioctrl == -1)
    {
        PRT_DBG("open cb dev error\n\n");
        return -1;
    }
    
    for (i = 0; i < CB_FCT_NUM; i++)
    {
        SLIST_INIT(&arFctList[i]);
    }


    
    if (x_thread_create (&_thdId,
                         "cb thread",
                         0x1000,
                         128,
                         _cbThd,
                         0,
                         NULL) != 0)
    {
        i4Ret = CBR_ERR_SYS;
        goto _CB_InitLib_Err;
    }

    _i4Init = 1;
    if (x_sema_create(&hReadMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        printf("call back create error\n\n");
    }
    
    /*close(fdioctrl);*/
    PRT_DBG("\nmw: CB_InitMwAdptLib OK.\n\n");
    return i4Ret;

_CB_InitLib_Err:
    if (fdioctrl >= 0)
    {
        /*close(fdioctrl);*/
    }
    return i4Ret;
}

/*-----------------------------------------------------------------------------
 * Register a callback function.
 *  @param  eFctId      Callback function ID.
 *  @param  i4SubId     Callback function Sub ID.
 *  @param  pfnCbFct    Callback function.
 *  @retval CBR_OK          Success.
 *  @retval CBR_NO_MEM      Out of memory.
 *  @retval CBR_ERR_REG     Couldn't register the callback function.
 * -----------------------------------------------------------------------------*/
int CB_RegMwAdptCbFunc(CB_FCT_ID_T eFctId, int i4SubId, CB_FCT pfnCbFct)
{
    CB_SET_MWADPT_CALLBACK_T rSetCb;
    CB_ADPT_FCT_ITEM_T *prFctItem;


    if (!_i4Init)
    {
        return CBR_ERR_REG;
    }
    
    prFctItem = (CB_ADPT_FCT_ITEM_T *)malloc(sizeof(CB_ADPT_FCT_ITEM_T));
    if (prFctItem == NULL)
    {
        return CBR_NO_MEM;
    }

    fdioctrl = open("/dev/rmmgr", O_RDWR);
    if (fdioctrl == -1)
    {
        PRT_DBG("open cb dev error\n\n");
        free(prFctItem);
        return -1;
    }

    prFctItem->pfnCbFct = pfnCbFct;
    prFctItem->isSync   = 0;

    if (_i4MutexLock())
    {
        PRT_DBG("CB_RegMwAdptCbFunc: pthread mutex lock failed!\n");
    }

    rSetCb.eFctId  = eFctId;
    rSetCb.i4SubId = i4SubId;
    if (ioctl(fdioctrl, CB_SET_CALLBACK_ADPT, &rSetCb) < 0)
    {
        free(prFctItem);
        if (_i4MutexUnlock())
        {
            PRT_DBG("CB_RegMwAdptCbFunc: pthread mutex unlock failed\n");
        }

        /*close(fdioctrl);*/
        return CBR_ERR_REG;
    }

    SLIST_INSERT_HEAD(prFctItem, &(arFctList[eFctId]), rLink);

    if (_i4MutexUnlock())
    {
        PRT_DBG("CB_RegMwAdptCbFunc: pthread mutex unlock failed\n");
    }
    
    /*close(fdioctrl);*/
    PRT_DBG("\nmw: CB_RegMwAdptCbFunc OK.\n\n");
    return CBR_OK;
}

/*-----------------------------------------------------------------------------
 * Unregister a callback function.
 *  @param  eFctId      Callback function ID.
 *  @param  pfnCbFct    Callback function.
 *  @retval CBR_OK          Success.
 *  @retval CBR_NOT_EXIST   The specified function is not registered..
-----------------------------------------------------------------------------*/
int CB_UnregMwAdptCbFunc(CB_FCT_ID_T eFctId, CB_FCT pfnCbFct)
{
    CB_ADPT_FCT_ITEM_T *prFctItem;

    if (!_i4Init)
    {
        return CBR_ERR_REG;
    }

    fdioctrl= open("/dev/rmmgr", O_RDWR);
    if (fdioctrl == -1)
    {
        PRT_DBG("open cb dev error\n\n");
        return -1;
    }

    if (_i4MutexLock())
    {
        PRT_DBG("CB_UnregMwAdptCbFunc: pthread mutex lock failed!\n");
    }
    
    SLIST_FOR_EACH(prFctItem, &(arFctList[eFctId]), rLink)
    {
        if (prFctItem->pfnCbFct == pfnCbFct)
        {
            /* found */
            break;
        }
    }

    if (prFctItem == NULL)
    {
        if (_i4MutexUnlock())
        {
            PRT_DBG("CB_UnregMwAdptCbFunc: pthread mutex unlock failed\n");
        }

        /*close(fdioctrl);*/

        return CBR_NOT_EXIST;
    }
    else
    {
        SLIST_REMOVE(prFctItem, rLink);
        free(prFctItem);

        if (_i4MutexUnlock())
        {
            PRT_DBG("CB_UnregMwAdptCbFunc: pthread mutex unlock failed\n");
        }

        /*close(fdioctrl);*/
        PRT_DBG("\nmw: CB_UnregMwAdptCbFunc OK.\n\n");
        return CBR_OK;
    }
}

/*-----------------------------------------------------------------------------
 * Register a callback function.
 *  @param  eFctId      Callback function ID.
 *  @param  i4SubId     Callback function Sub ID.
 *  @param  pfnCbFct    Callback function.
 *  @retval CBR_OK          Success.
 *  @retval CBR_NO_MEM      Out of memory.
 *  @retval CBR_ERR_REG     Couldn't register the callback function.
 * -----------------------------------------------------------------------------*/
int CB_RegMwAdptCBFunc_Sync(CB_FCT_ID_T eFctId, int i4SubId, CB_FCT pfnCbFct)
{
    CB_SET_MWADPT_CALLBACK_T rSetCb;
    CB_ADPT_FCT_ITEM_T *prFctItem;


    if (!_i4Init)
    {
        return CBR_ERR_REG;
    }
    
    prFctItem = (CB_ADPT_FCT_ITEM_T *)malloc(sizeof(CB_ADPT_FCT_ITEM_T));
    if (prFctItem == NULL)
    {
        return CBR_NO_MEM;
    }

    fdioctrl  =open("/dev/rmmgr", O_RDWR);
    if (fdioctrl == -1)
    {
        PRT_DBG("open cb dev error\n\n");
        free(prFctItem);
        return -1;
    }

    prFctItem->pfnCbFct = pfnCbFct;
    prFctItem->isSync   = 1;

    if (_i4MutexLock())
    {
        PRT_DBG("CB_RegMwAdptCBFunc_Sync: pthread mutex lock failed!\n");
    }

    rSetCb.eFctId  = eFctId;
    rSetCb.i4SubId = i4SubId;
    if (ioctl(fdioctrl, CB_SET_CALLBACK_ADPT, &rSetCb) < 0)
    {
        free(prFctItem);
        if (_i4MutexUnlock())
        {
            PRT_DBG("CB_RegMwAdptCBFunc_Sync: pthread mutex unlock failed\n");
        }

        /*close(fdioctrl);*/
        return CBR_ERR_REG;
    }

    SLIST_INSERT_HEAD(prFctItem, &(arFctList[eFctId]), rLink);

    if (_i4MutexUnlock())
    {
        PRT_DBG("CB_RegMwAdptCBFunc_Sync: pthread mutex unlock failed\n");
    }
    
    /*close(fdioctrl);*/
    PRT_DBG("\nmw: CB_RegMwAdptCBFunc_Sync is waiting semphore.\n\n");
    /*sem_wait(&sem);*/
    PRT_DBG("\nmw: CB_RegMwAdptCBFunc_Sync OK.\n\n");
    CB_UnregMwAdptCbFunc(eFctId, pfnCbFct);
    return CBR_OK;
}

