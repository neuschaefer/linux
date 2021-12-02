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
 * $RCSfile: fbm_cs.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_cs.c
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_cs.h"
#include "fbm_drvif.h"
#include "fbm_debug.h"
#include "x_assert.h"
#include "x_os.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


/******************************************************************************
* Local Function, Counting Semaphore
******************************************************************************/

//-------------------------------------------------------------------------
/** _FBM_csema_create
 *  Create counting semaphore
 *  @param  prSemaphore   semaphore pointer.
 *  @param  hMutex   mutex.
 *  @param  i4InitCount   count.
 *  @return OSR_OK.
 *          If fail, return != OSR_OK.
 */
//-------------------------------------------------------------------------
INT32 _FBM_csema_create(COUNTING_SEMAPHORE_T* prSemaphore, HANDLE_T hMutex, INT32 i4InitCount)
{
    if (prSemaphore == NULL)
    {
        return OSR_INV_ARG;
    }

    prSemaphore->i4Count = i4InitCount;
    prSemaphore->hMutex = hMutex;        // special design, external mutex

    // create semaphore
    VERIFY(x_sema_create(&(prSemaphore->hBinary), 
            X_SEMA_TYPE_BINARY, 
            X_SEMA_STATE_LOCK) == OSR_OK);

    return OSR_OK;
}


//-------------------------------------------------------------------------
/** _FBM_csema_reset
 *  Reset counting semaphore
 *  @param  prSemaphore   semaphore pointer.
 *  @param  hMutex   mutex.
 *  @param  i4InitCount   count.
 *  @return OSR_OK.
 *          If fail, return != OSR_OK.
 */
//-------------------------------------------------------------------------
INT32 _FBM_csema_reset(COUNTING_SEMAPHORE_T* prSemaphore, HANDLE_T hMutex, INT32 i4InitCount)
{
    if (prSemaphore == NULL)
    {
        return OSR_INV_ARG;
    }

    prSemaphore->i4Count = i4InitCount;
    prSemaphore->hMutex = hMutex;        // special design, external mutex

    // reset prSemaphore->hBinary to X_SEMA_STATE_LOCK
    if (x_sema_lock(prSemaphore->hBinary, X_SEMA_OPTION_NOWAIT) == OSR_OK)
    {
    }
    else
    {
    }

    return OSR_OK;
}


//-------------------------------------------------------------------------
/** _FBM_csema_delete
 *  Delete counting semaphore
 *  @param  prSemaphore   semaphore pointer.
 *  @return OSR_OK.
 *          If fail, return != OSR_OK.
 */
//-------------------------------------------------------------------------
INT32 _FBM_csema_delete(const COUNTING_SEMAPHORE_T* prSemaphore)
{
    if (prSemaphore == NULL)
    {
        return OSR_INV_ARG;
    }

    // delete semaphore
    VERIFY(x_sema_delete(prSemaphore->hBinary) == OSR_OK);

    return OSR_OK;
}


//-------------------------------------------------------------------------
/** _FBM_csema_unlock
 *  Unlock counting semaphore
 *  @param  prSemaphore   semaphore pointer.
 *  @return OSR_OK.
 *          If fail, return != OSR_OK.
 */
//-------------------------------------------------------------------------
INT32 _FBM_csema_unlock(COUNTING_SEMAPHORE_T* prSemaphore)
{
    if (prSemaphore == NULL)
    {
        return OSR_INV_ARG;
    }

    prSemaphore->i4Count++;

    if (prSemaphore->i4Count <= 0)
    {
        VERIFY(x_sema_unlock(prSemaphore->hBinary) == OSR_OK);  // wake
    }

    return OSR_OK;
}


//-------------------------------------------------------------------------
/** _FBM_csema_lock
 *  Lock counting semaphore
 *  @param  prSemaphore   semaphore pointer.
 *  @param  e_options   wait/no_wait option.
 *  @param  u4Time   wait time.
 *  @return OSR_OK.
 *          If fail, return != OSR_OK.
 */
//-------------------------------------------------------------------------
INT32 _FBM_csema_lock(COUNTING_SEMAPHORE_T* prSemaphore, SEMA_OPTION_T e_options, UINT32 u4Time)
{
    INT32 i4ReturnValue;

    if (prSemaphore == NULL)
    {
        return OSR_INV_ARG;
    }

    if ((e_options == X_SEMA_OPTION_WAIT) && (u4Time != 0))
    {
        prSemaphore->i4Count--;

        if (prSemaphore->i4Count < 0)
        {
#ifdef CC_VDP_FULL_ISR
            x_crit_end(*prSemaphore->prState);
#else
            VERIFY(x_sema_unlock(prSemaphore->hMutex) == OSR_OK);
#endif

            i4ReturnValue = x_sema_lock_timeout(prSemaphore->hBinary, u4Time);

#ifdef CC_VDP_FULL_ISR
            *prSemaphore->prState = x_crit_start();
#else
            VERIFY(x_sema_lock(prSemaphore->hMutex, 
                X_SEMA_OPTION_WAIT) == OSR_OK);
#endif

            if (i4ReturnValue == OSR_TIMEOUT)
            {
                if (prSemaphore->i4Count >= 0)
                {
                    // 20041229 mutex is NOT well protected!
                    LOG(3, "{CS}\n");

                    VERIFY(x_sema_lock(prSemaphore->hBinary, 
                        X_SEMA_OPTION_NOWAIT) == OSR_OK);

                    i4ReturnValue = OSR_OK;

#ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('c');
#endif
                }
                else
                {
                    // Get semaphore timeout, put it back
                    prSemaphore->i4Count++;        

                    LOG(3, "{CS} Timeout (%d)\n", prSemaphore->i4Count);


#ifdef VDP_RACE_LOG
                    VDP_FrcRacingLog('C');
#endif
                    return OSR_TIMEOUT;
                }
            }

            ASSERT(i4ReturnValue == OSR_OK);
        }
    }
    else
    {
        if (prSemaphore->i4Count <= 0)
        {
            return OSR_WOULD_BLOCK;
        }

        prSemaphore->i4Count--;    
    }
    ASSERT(prSemaphore->i4Count >= 0);
    
    return OSR_OK;
}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


