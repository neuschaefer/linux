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
#include "x_bim.h"
#define __USE_XBIM_IRQS        // UGLY HACK! Make sure we don't include mach/irqs.h from kernel.

#include "x_linux.h"
#include "driver_os.h"
#include "isr_linux_drv.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_timeprofile.h"

#include <linux/version.h>
#include <linux/interrupt.h>

#ifndef SA_SHIRQ		/* depricated naming*/
#define SA_SHIRQ		IRQF_SHARED
#define SA_INTERRUPT		IRQF_DISABLED
#endif

//---------------------------------------------------------------------------

#if defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
    #define MAX_VECTOR_ID       72                  // Max vector ID
#else
    #define MAX_VECTOR_ID       MAX_IRQ_VECTOR
#endif

#if defined(CC_MT5396)||defined(CC_MT5368)
    #define SPI_OFFSET          32
#else
    #define SPI_OFFSET          0
#endif

#define ISR_VECTOR_SIZE     (MAX_VECTOR_ID + 1) // Max number of registered vectors

typedef struct
{
    x_os_isr_fct    pf_isr;
    char            ucName[32];
} IRQ_ARRAY_T;

#if defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389)
IRQ_ARRAY_T rIrqs[ISR_VECTOR_SIZE] =
{
    {NULL, "PDWNC"},
    {NULL, "SERIAL"},
    {NULL, "NAND"},
    {NULL, "T0"},
    {NULL, "T1"},
    {NULL, "T2"},
    {NULL, "SMARTCARD"},
    {NULL, "WDT"},
    {NULL, "ENET"},
    {NULL, "MJC"},
    {NULL, "PSCAN"},
    {NULL, "USB1"},
    {NULL, "IMGRZ"},
    {NULL, "VENC"},
    {NULL, "SPDIF_PSR"},
    {NULL, "USB0"},
    {NULL, "AUDIO"},
    {NULL, "RS232"},
    {NULL, "LED"},
    {NULL, "OSD"},
    {NULL, "VDOIN"},
    {NULL, "BLK2RS"},
    {NULL, "FLASH"},
    {NULL, "POST_PROC"},
    {NULL, "VDEC"},
    {NULL, "GFX"},
    {NULL, "DEMUX"},
    {NULL, "DEMOD"},
    {NULL, "FCI"},
    {NULL, "APWM"},
    {NULL, "MISC2"},
    {NULL, "MISC"},
    {NULL, "DRAMC"},
    {NULL, "EXT1"},
    {NULL, "POD"},
    {NULL, "PMU1"},
    {NULL, "PMU0"},
    {NULL, "DRAMC_CHB"},
    {NULL, "TVE"},
    {NULL, "PCIE"},
    {NULL, "APWM2"},
    {NULL, "SPI"},
    {NULL, "JPGDEC"},
    {NULL, "P"},
    {NULL, "AES"},
    {NULL, "TCON_TCH"},
    {NULL, "TCON_EH"},
    {NULL, "OD"},
    {NULL, "USB2"},
    {NULL, "USB3"},
    {NULL, "GDMA"},
    {NULL, "EPHY"},
    {NULL, "TCPIP"},
    {NULL, "GFX3D_GP"},
    {NULL, "GFX3D_PP"},
    {NULL, "GFX3D_PMU"},
    {NULL, "GFX3D_GPMMU"},
    {NULL, "GFX3D_PPMMU"},
    {NULL, "MMU_GFX"},
    {NULL, "MMU_GCPU"},
    {NULL, "MMU_IMGRZ"},
    {NULL, "MMU_JPG"},
    {NULL, "MMU_GDMA"},
    {NULL, "DDI"},
    {NULL, "LZHS"},
    {NULL, "L2C"},
    {NULL, "IMGRZ2"},
    {NULL, "MMU_IMGRZ2"},
    {NULL, "MSDC2"},
    {NULL, "PNG1"},
    {NULL, "PNG2"},
    {NULL, "PNG3"},
    {NULL, "MMU_PNG1"},
};
#else
#error rIrqs  not defined.
#endif


#if (TIMEPROFILE_LOG == 1)
EXETIME_MEASURE ExeTime[40];
#endif
irqreturn_t _interrupt(int irq, void *dev_id)
{
    irqreturn_t ret;

    irq -= SPI_OFFSET;

    if (irq > MAX_VECTOR_ID)
    {
        return IRQ_NONE;
    }

    if (rIrqs[irq].pf_isr)
    {
#if (TIMEPROFILE_LOG == 1)
    ExeTimeRecordStart(&ExeTime[irq].RawTimeStart);
#endif
        rIrqs[irq].pf_isr((UINT16)irq);
#if (TIMEPROFILE_LOG == 1)
    ExeTimeRecordStop(&ExeTime[irq] );
#endif
        ret = IRQ_HANDLED;
    }
    else
    {
        ret = IRQ_NONE;
    }
    
    return ret;
}

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
    int result;
    INT32 ret = OSR_DRV_OK;
    unsigned long flags;

    if (ui2_vec_id> MAX_VECTOR_ID)
    {
        return OSR_DRV_FAIL;
    }

    // disable IRQ.
    local_irq_save(flags);

#ifdef CONFIG_SMP
    // For SMP, also need to disable specific IRQ to prevent it run from other cores.
    // We can't use x_crit_start(), otherwise it might deadlock when:
    //  1. core 2 running reg_isr with lock held
    //  2. core 1 got IRQ, in ISR it need to enter critical section, wait for core2.
    //  3. core 2 running free_irq, which need to wait for core 1 ISR to finish.
    {
    int irqen = BIM_IsIrqEnabled(ui2_vec_id);
    if (irqen)
        BIM_DisableIrq(ui2_vec_id);
#endif

    *ppf_old_isr = rIrqs[ui2_vec_id].pf_isr;
    rIrqs[ui2_vec_id].pf_isr = pf_isr;

    if (*ppf_old_isr)
    {
        free_irq(ui2_vec_id+SPI_OFFSET, &rIrqs[ui2_vec_id].ucName);
    }

    if (pf_isr != NULL)
    {
        unsigned long irq_flags;

        irq_flags = SA_SHIRQ;

        if ((ui2_vec_id == VECTOR_SERIAL) ||
            (ui2_vec_id == VECTOR_GFX) ||
            (ui2_vec_id == VECTOR_PDWNC))
        {
            irq_flags |= IRQF_SAMPLE_RANDOM;
        }
        
        result = request_irq(ui2_vec_id+SPI_OFFSET, _interrupt, irq_flags, rIrqs[ui2_vec_id].ucName, &rIrqs[ui2_vec_id].ucName);
        if (result)
        {
            ret = OSR_DRV_FAIL;
            goto out;
        }
    }

out:

#ifdef CONFIG_SMP
    if (irqen)
        BIM_EnableIrq(ui2_vec_id);
    }
#endif

    local_irq_restore(flags);

    return ret;
}

INT32
x_os_drv_reg_isr_ex(UINT16             ui2_vec_id,
                    x_os_drv_isr_fct   pf_isr,
                    x_os_drv_isr_fct   *ppf_old_isr)
{
    int result;

    if (ui2_vec_id > MAX_VECTOR_ID)
    {
        return OSR_DRV_FAIL;
    }
    
    *ppf_old_isr = rIrqs[ui2_vec_id].pf_isr;
    rIrqs[ui2_vec_id].pf_isr = pf_isr;

    if (*ppf_old_isr)
    {
        free_irq(ui2_vec_id+SPI_OFFSET, NULL);
    }
    
    if (pf_isr != NULL)
    {
        result = request_irq(ui2_vec_id+SPI_OFFSET, _interrupt, SA_INTERRUPT, rIrqs[ui2_vec_id].ucName, NULL);
        if (result)
        {
            return OSR_DRV_FAIL;
        }
    }

    return OSR_DRV_OK;
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
    return OSR_DRV_OK;
}
