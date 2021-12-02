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
 * $RCSfile: pod_isr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pod_isr.c
 *  This file implements ISR of POD.
 */

#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_END

#include "pcmcia_if.h"
#include "pcmcia_ctrl.h"
#include "pcmcia_hw.h"
#include "pcmcia_isr.h"

#include "CableCard/podci_ctrl.h"

#ifdef PCMCIA_ENABLED

// Just Avoid Compile Error.
#ifndef VECTOR_PCMCIA
#define VECTOR_PCMCIA 0
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define CHECK_CMD_STS_COUNT         50
//#define BIT_VALUE(data, pos)            (((data) & (pos)) ? 1 : 0)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static BOOL             fgPODIsrOn = FALSE;

/** No access protection to the following two vailables since value change
    simultaneously is not possible
*/

static x_os_isr_fct         _pf_ireq_nfy_fct = NULL;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** This API signals the module state to the Middleware if requested.
 */
//-----------------------------------------------------------------------------
static BOOL _PCMCIA_IreqNfyFct(UINT32 ui4_ireq_state)
{
    if ( _pf_ireq_nfy_fct != NULL )
    {
        _pf_ireq_nfy_fct(ui4_ireq_state);
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** This is PCMCIA/POD/CI ISR.
 */
//-----------------------------------------------------------------------------
static void _PCMCIA_Isr(UINT16 u2Vector)
{   
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        return ;
    }
    else
    {
        UINT32              u4Ctrl;
        UINT32              u4IntSts;
        UINT32              u4PbusSts;
        UINT32              u4CmdSts;

        u4IntSts = PCMCIAHW_READ32(REG_INT_STS);
        while (u4IntSts)
        {
            u4Ctrl = PCMCIAHW_READ32(REG_CTRL);
            u4PbusSts = PCMCIAHW_READ32(REG_PBUS_STS);

            // CD Interrupt. (should be handled before clear interrupt bits.)
            if ((u4Ctrl & PC_CD_IE) && (u4IntSts & PC_CD_INT))
            {
                PCMCIA_CDPinHandler();
            }
            
            PCMCIAHW_WRITE32(REG_INT_STS, u4IntSts); /* Clear interrupt bits */
            UNUSED(BIM_ClearIrq(VECTOR_PCMCIA));

            // Command done interrupt
            if ((u4Ctrl & GCMD_IE) && (u4IntSts & CMD_DONE_INT))
            {
                u4CmdSts = PCMCIAHW_READ32(REG_CMD_STS);
                if ((u4CmdSts & (CMD_STS_DONE | CMD_STS_DONE_OK)) != (CMD_STS_DONE | CMD_STS_DONE_OK))
                {
                    /* INPACK error */
                    if (u4CmdSts & CMD_STS_INPACK_ERR)
                    {
                        LOG(0, "_PCMCIA_Isr(): INPACK error\n");
                    }

                    /* Read/write address is invalid to memory map */
                    if (u4CmdSts & CMD_STS_RWANM)
                    {
                        if (u4PbusSts & PC_WP)
                        {
                            LOG(1, "_PCMCIA_Isr(): The module is write-protected\n");
                        }
                        else
                        {
                            LOG(0, "_PCMCIA_Isr(): Invalid address\n");
                        }
                    }

                    /* Command is withdrawn by software */
                    if (u4CmdSts & CMD_STS_CW)
                    {
                        LOG(1, "_PCMCIA_Isr(): Command is withdrawn by software\n");
                    }
                }

                PCMCIA_SetCmdStatusDone();
            }

            // POD/CI READY/IREQ change interrupt
            if ((u4Ctrl & PC_RDY_IRQ_IE) && (u4IntSts & PC_RDY_IRQ_INT))
            {
                // check only callback upper layer only in pod/ci mode.
                if (((PCMCIAHW_READ32(REG_CTRL) & ((UINT32)3 << 30)) == (UINT32)PODCI))
                {
                    // check only callback upper layer only when cam connected.
                    // (avoid bug : sometimes there are some interrupts when cam removing.)
                    if ( ((u4PbusSts&PC_CD1) == 0) && ((u4PbusSts&PC_CD2) == 0) )
                    {
                        LOG(5, "PCMCIA IREQ pin status change: %d\n", u4PbusSts&PC_RDY_IRQ?TRUE:FALSE);
                        if ( u4PbusSts & PC_RDY_IRQ )
                        {
                            VERIFY(_PCMCIA_IreqNfyFct(1));
                        }
                        else
                        {
                            VERIFY(_PCMCIA_IreqNfyFct(0));
                        }
                    }
                }
            }

            u4IntSts = PCMCIAHW_READ32(REG_INT_STS);
        }
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** This API enables PCMCIA/POD/CI ISR.
 */
//-----------------------------------------------------------------------------
INT32 PCMCIA_EnableISR(void)
{
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        return 0;
    }
    else
    {
        INT32  i4Return;
        x_os_isr_fct fctNoUse;

        i4Return = POD_OK;

        /* Register PCMCIA/POD/CI ISR */
        if (!fgPODIsrOn)
        {
            if (x_reg_isr(VECTOR_PCMCIA, _PCMCIA_Isr, &fctNoUse) != OSR_OK)
            {
                i4Return = POD_REGISTER_ISR_FAILED;
            }
            else
            {
                /* Enable POD interrupt */
                UNUSED(BIM_ClearIrq(VECTOR_PCMCIA));
                fgPODIsrOn = TRUE;
            }
        }
        return i4Return;
    }
}

void PCMCIA_RegIreqCallback(x_os_isr_fct pfnIsr)
{
    _pf_ireq_nfy_fct = pfnIsr;
}

#endif

#endif //CC_MTK_LOADER
