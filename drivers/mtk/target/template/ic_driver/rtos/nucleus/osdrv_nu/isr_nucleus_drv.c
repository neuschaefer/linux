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
 * Description:
 *---------------------------------------------------------------------------*/

/* driver-OS header files */
#include "driver_os.h"
#include "isr_nucleus_drv.h"
#include "nucleus.h"
#include "x_hal_926.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_printf.h"

#include <string.h>

//---------------------------------------------------------------------------

#define MAX_VECTOR_ID       31                  // Max vector ID
#define ISR_VECTOR_SIZE     (MAX_VECTOR_ID + 1) // Max number of registered vectors
#define HISR_STACK_SIZE     4096UL              // Stack size for the common HISR
#define HISR_PRIORITY       2                   // Priority of the common HISR

// ISR control block of OS driver
typedef struct
{
    NU_HISR             hisr;       // Must be the first element in the structure
    x_os_drv_isr_fct    pfIsr;
    void*               pvStack;
    UINT16              u2Vector;
} OS_DRV_ISR_T;

// List of ISR control block
static OS_DRV_ISR_T     _arIsrList[ISR_VECTOR_SIZE];

//---------------------------------------------------------

//============================================================================

/*----------------------------------------------------------------------------
 * Function: DriverCommonLISR
 *
 * Description:
 *      The common LISR for registered user ISRs. Note that the user ISRs are
 *      run in HISR context.
 *      1. Disable interrupt of current vector
 *      2. Activate the common HISR
 *
 * Inputs:
 *      i4Vector: The interrupt vector
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static void DriverCommonLISR(INT i4Vector)
{
    ASSERT((i4Vector >= 0) && (i4Vector <= MAX_VECTOR_ID));
    ASSERT(_arIsrList[i4Vector].u2Vector == (UINT16)i4Vector);
    ASSERT(_arIsrList[i4Vector].pfIsr != NULL);

    if (i4Vector==VECTOR_T1)
    {
        // Invoke user ISR
        _arIsrList[i4Vector].pfIsr((UINT16)i4Vector);
        // _GPIO_DimmingIsr((UINT16)i4Vector);
    }
    else
    {
        // Disable interrupt for this vector. Since the user ISR is
        // deferred to run in HISR context with interrupt enabled. If
        // this vector wasn't disabled prior the HISR, enabling interrupt
        // will cause this LISR to be triggered again immediately.
        //
        VERIFY(BIM_DisableIrq((UINT32)i4Vector));

        // Activate the corresponding HISR
        VERIFY(NU_Activate_HISR(&_arIsrList[i4Vector].hisr) == NU_SUCCESS);
    }
}

/*----------------------------------------------------------------------------
 * Function: DriverCommonHISR
 *
 * Description:
 *      The common HISR for registered user ISR.
 *      1. Find out the ISR control block for current vector
 *      2. Invoke user ISR
 *      3. Re-enable interrupt for current vector
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static void DriverCommonHISR(void)
{
    OS_DRV_ISR_T* pDrvIsr;
    NU_HISR* phisrSelf;

    // Get current HISR pointer
    phisrSelf = NU_Current_HISR_Pointer();
    ASSERT(phisrSelf != NULL);

    // Get corresponding OS_DRV_ISR_T pointer by casting directly
    pDrvIsr = (OS_DRV_ISR_T*)phisrSelf;
    ASSERT(pDrvIsr->pfIsr != NULL);
    ASSERT(pDrvIsr->u2Vector <= MAX_VECTOR_ID);

    // Invoke user ISR
    pDrvIsr->pfIsr(pDrvIsr->u2Vector);

    // Re-enable interrupt of this vector
    VERIFY(BIM_EnableIrq(pDrvIsr->u2Vector));
}

//============================================================================


/*----------------------------------------------------------------------------
 * Function: x_os_drv_reg_isr()
 *
 * Description:
 *      this API registers an ISR with its vector id. it performs
 *      1. check is vector table is established.
 *      2. locate vector table entry from vector id.
 *      3. save the new ISR if the table entry is not occupied or is
 *         occupied by the same vector, otherwise, search forward to
 *         find one until entire table is searched.
 *
 * Inputs:
 *      ui2_vec_id: an vector id to register an ISR.
 *      pf_isr: pointer to a ISR to set.
 *      ppf_old_isr: pointer to hold the current ISR setting.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_NOT_INIT: invalid argument.
 *      OSR_DRV_FAIL: OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_reg_isr(UINT16             ui2_vec_id,
                 x_os_drv_isr_fct   pf_isr,
                 x_os_drv_isr_fct   *ppf_old_isr)
{
    UINT32 u4Flags;
    INT32 i4Ret = OSR_DRV_FAIL;
    x_os_drv_isr_fct pf_old_isr;

    ASSERT(ppf_old_isr != NULL);

    if (ui2_vec_id > MAX_VECTOR_ID)
    {
        return OSR_DRV_FAIL;
    }

    u4Flags = HalCriticalStart();

    pf_old_isr = _arIsrList[ui2_vec_id].pfIsr;

    if (pf_isr != NULL)
    {
        if (_arIsrList[ui2_vec_id].pfIsr == NULL)
        {
            // Register ISR for the first time
            void (*pfnOldLisr)(INT);
            void* pvStack;
            CHAR szName[8];

            // Allocate HISR stack
            pvStack = DriverMemAlloc(HISR_STACK_SIZE);
            if (pvStack == NULL)
            {
                goto _Done;
            }
            memset(pvStack, 0, HISR_STACK_SIZE);    // Zero stack
            SPrintf(szName, "ISR_%u", ui2_vec_id);

            // Create corresponding HISR
            memset(&_arIsrList[ui2_vec_id].hisr, 0, sizeof(NU_HISR));
            if (NU_Create_HISR(&_arIsrList[ui2_vec_id].hisr, (CHAR*)szName,
                DriverCommonHISR, HISR_PRIORITY, pvStack, HISR_STACK_SIZE)
                != NU_SUCCESS)
            {
                DriverMemFree(pvStack);
                goto _Done;
            }

            // Register LISR
            if (NU_Register_LISR((INT)ui2_vec_id, DriverCommonLISR,
                &pfnOldLisr) != NU_SUCCESS)
            {
                DriverMemFree(pvStack);
                goto _Done;
            }

            _arIsrList[ui2_vec_id].u2Vector = ui2_vec_id;
            _arIsrList[ui2_vec_id].pvStack = pvStack;
            _arIsrList[ui2_vec_id].pfIsr = pf_isr;

            // Turn on interrupt
            VERIFY(BIM_EnableIrq(ui2_vec_id));
        }
        else
        {
            // Already installed for this vector, replace it
            _arIsrList[ui2_vec_id].pfIsr = pf_isr;
        }

        ASSERT(_arIsrList[ui2_vec_id].u2Vector == ui2_vec_id);
    }
    else
    {
        // De-register ISR
        if (_arIsrList[ui2_vec_id].pfIsr != NULL)
        {
            void (*pfnOldLisr)(INT);

            // Turn off interrupt
            VERIFY(BIM_DisableIrq(ui2_vec_id));

            // De-register LISR
            VERIFY(NU_Register_LISR((INT)ui2_vec_id, NULL, &pfnOldLisr)
                == NU_SUCCESS);

            // Delete HISR
            VERIFY(NU_Delete_HISR(&_arIsrList[ui2_vec_id].hisr) == NU_SUCCESS);

            // Free stack memory
            DriverMemFree(_arIsrList[ui2_vec_id].pvStack);

            _arIsrList[ui2_vec_id].u2Vector = MAX_VECTOR_ID + 1;
            _arIsrList[ui2_vec_id].pvStack = NULL;
            _arIsrList[ui2_vec_id].pfIsr = pf_isr;
        }
    }

    *ppf_old_isr = pf_old_isr;

    i4Ret = OSR_DRV_OK;

_Done:

    HalCriticalEnd(u4Flags);

    return i4Ret;
}


/*----------------------------------------------------------------------------
 * Function: os_drv_isr_init()
 *
 * Description:
 *      this API registers an ISR with its vector id. it performs
 *      1. allocate vector table
 *      2. initialize each table entry.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
os_drv_isr_init(void)
{
    UINT32 i;

    for (i = 0; i < ISR_VECTOR_SIZE; i++)
    {
        _arIsrList[i].pfIsr = NULL;
        _arIsrList[i].u2Vector = MAX_VECTOR_ID + 1;
        _arIsrList[i].pvStack = NULL;
        memset(&_arIsrList[i].hisr, 0, sizeof(NU_HISR));
    }

    return OSR_DRV_OK;
}

