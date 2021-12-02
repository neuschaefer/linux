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

/** @file musb_misc.c
 *  This C file implements the musb misc functions.
 * 
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_typedef.h"
#include "mu_stdint_drvif.h"
#include "brd_cnf.h"
#include "mu_none.h"

#ifdef MUSB_LOADER
#include "x_irq.h"
#include "x_ckgen.h"
#endif

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef struct{
    x_os_isr_fct pf_isr;
    UINT16 ui2_vec_id;
}MUSB_ISR_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define MUSB_MAX_HW_TIMER	(MUSB_MAX_TIMERS * MUSB_MAX_CONTROLLERS)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern size_t x_strlen(const CHAR *ps_s);

UINT32 u4UsbTick = 0;

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
void vUsbTimer(void);
void vUsbIrq(UINT32 vector_id);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT32 _u4USBISRHandlerNo = 0;
static MUSB_ISR_T _arUSBISRHandler[MUSB_MAX_CONTROLLERS];
static MGC_NoneTimerWrapper *_aprTimer[MUSB_MAX_HW_TIMER];

#ifdef MUSB_LOADER
static UINT32 _u4UsbTimerIsrInit = 0;
static PFN_IRQ_HANDLER pfnTimerNextIsr = NULL;
static PFN_IRQ_HANDLER pfnUsbNextIsr = NULL;

typedef enum
{
    IRQ_START = 0,
    IRQ_TIMEOUT,
    IRQ_ONLY_ONE,
    IRQ_NOT_ONE
}   IRQ_OFF_STATE;

static UINT8 _u1IrqOff = IRQ_START;

#endif
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifdef MUSB_LOADER
static void _NonOSTimerOn(void)
{
    UINT32 u4Val;

    // Enable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val |= ((TMR0_CNTDWN_EN) | (TMR0_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);
}

static void _NonOSTimerOff(void)
{
    UINT32 u4Val;

    // disable timer 0
    u4Val = BIM_READ32(REG_RW_TIMER_CTRL);
    u4Val &= ~((TMR0_CNTDWN_EN) | (TMR0_AUTOLD_EN));
    BIM_WRITE32(REG_RW_TIMER_CTRL, u4Val);            
}

static void _NonOSTimerIsr(void)
{
    if (BIM_IsIrqPending(VECTOR_T0))
    {
        vUsbTimer();
        BIM_ClearIrq(VECTOR_T0);        
    }
    
    if (pfnTimerNextIsr) { pfnTimerNextIsr(); }
}

static void _NonOSUsbIsr(void)
{

#ifdef MUSB_PORT_CONFIG
    UINT8 bIndex;
    for (bIndex = 0; bIndex < (uint8_t)MUSB_MAX_CONTROLLERS; bIndex++)
    {
    	if(MGC_bUsbPortDef[bIndex])
	{
 	    if (BIM_IsIrqPending(MUSB_aNoneController[bIndex].dwInfo))
	    {
		vUsbIrq(MUSB_aNoneController[bIndex].dwInfo);
		BIM_ClearIrq(MUSB_aNoneController[bIndex].dwInfo);		 
	    }
	}
    }
#else

    if (BIM_IsIrqPending(VECTOR_USB))
    {
        vUsbIrq(VECTOR_USB);
        BIM_ClearIrq(VECTOR_USB);        
    }

    if (BIM_IsIrqPending(VECTOR_USB1))
    {
        vUsbIrq(VECTOR_USB1);
        BIM_ClearIrq(VECTOR_USB1);        
    }
    /*
    if (BIM_IsIrqPending(VECTOR_USB2))
    {
        vUsbIrq(VECTOR_USB2);
        BIM_ClearIrq(VECTOR_USB2);        
    }
	if (BIM_IsIrqPending(VECTOR_USB3))
    {
        vUsbIrq(VECTOR_USB3);
        BIM_ClearIrq(VECTOR_USB3);        
    }
    */
  #endif
    if (pfnUsbNextIsr) { pfnUsbNextIsr(); }
}

static void _NonOSSetupUsbTimerIsr(void)
{
    UINT32 u4BusClock;
    #ifdef MUSB_PORT_CONFIG
    UINT8  bIndex;
    #endif
    if (_u4UsbTimerIsrInit)
    {
        return;
    }

    pfnUsbNextIsr = RegisterIrqHandler(_NonOSUsbIsr);
    pfnTimerNextIsr = RegisterIrqHandler(_NonOSTimerIsr);
    _u4UsbTimerIsrInit = 1;

    u4BusClock = BSP_GetDomainClock(SRC_BUS_CLK);

    // Setup Timer 0 and triger 1st interrupt.
    BIM_WRITE32(REG_RW_TIMER0_LLMT, (u4BusClock/800));
    BIM_WRITE32(REG_RW_TIMER0_HLMT, 0);
    BIM_WRITE32(REG_RW_TIMER0_LOW , (u4BusClock/800));
    BIM_WRITE32(REG_RW_TIMER0_HIGH, 0);

    _NonOSTimerOff();
    
    BIM_EnableIrq(VECTOR_T0);
    #ifdef MUSB_PORT_CONFIG
    for (bIndex = 0; bIndex < (uint8_t)MUSB_MAX_CONTROLLERS; bIndex++)
    {
    	if(MGC_bUsbPortDef[bIndex])
	{
 	    BIM_EnableIrq(MUSB_aNoneController[bIndex].dwInfo);
	}
    }
    #else
    BIM_EnableIrq(VECTOR_USB);
    BIM_EnableIrq(VECTOR_USB1);
    //BIM_EnableIrq(VECTOR_USB2);
    //BIM_EnableIrq(VECTOR_USB3);
    #endif
    HalEnableIRQ();
}

#else

static void _NonOSTimerOn(void)
{

}

static void _NonOSTimerOff(void)
{

}

#endif
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** vUsbTimer(): usb timer handler which is called by h/w handler every 10 ms.
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void vUsbTimer(void)
{
    MGC_NoneTimerWrapper *prTimer;
    UINT32 i;

    // 10 ms per tick.
    u4UsbTick ++;
    
    _u1IrqOff = IRQ_START;
    for (i=0; i<MUSB_MAX_HW_TIMER; i++)
    {
        if (_aprTimer[i])
        {
            prTimer = _aprTimer[i];
            if ((prTimer->pfExpired != NULL) && (prTimer->dwTime > 0))
            {
                if (++prTimer->dwCount >= prTimer->dwTime)
                {
                    _aprTimer[i] = NULL;
                    prTimer->dwTimeOut = TRUE;                    
                    if(_u1IrqOff == IRQ_START)
                        _u1IrqOff = IRQ_TIMEOUT;
                    //_NonOSTimerOff();
                }
                if(_u1IrqOff == IRQ_TIMEOUT)
                    _u1IrqOff = IRQ_ONLY_ONE;
                else
                    _u1IrqOff = IRQ_NOT_ONE;
                }
            }            
        }
    if(_u1IrqOff == IRQ_ONLY_ONE)
    {
        _NonOSTimerOff();
        _u1IrqOff = IRQ_START;
    }    
}
/*----------------------------------------------------------------------------
 * Function: x_timer_start()
 *
 * Description:
 *      this API starts a timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. parse argumets.
 *      3. check handle validity.
 *      4. fill up timer struct and put timer struct in the global timer
 *         doubly linked list.
 *
 * Inputs: -
 *      h_timer_hdl: timer handle returned from x_timer_create().
 *      ui4_delay: timer pops after this delay has passed.
 *      e_flags: either X_TIMER_FLAG_ONCE or X_TIMER_FLAG_REPEAT.
 *      pf_cb: callback function to be executed after "ui4_delay" had passed.
 *      *pv_tag: argument to callback function pf_cb.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_timer_start (HANDLE_T h_timer, UINT32 ui4_delay, TIMER_FLAG_T e_flags,
               x_os_timer_cb_fct pf_callback, VOID* pv_tag)
{
    UINT32 i;    
    MGC_NoneTimerWrapper *prTimer = (MGC_NoneTimerWrapper *)h_timer;
    
    UNUSED(ui4_delay);
    UNUSED(e_flags);
    UNUSED(pf_callback);
    UNUSED(pv_tag);
    
    // save timer handle
    for (i=0; i<MUSB_MAX_HW_TIMER; i++)
    {
        if (!_aprTimer[i])
        {
            _NonOSTimerOn();
            
            _aprTimer[i] = prTimer;
            return OSR_OK;
        }
    }
    return -1;
}


/*----------------------------------------------------------------------------
 * Function: x_timer_stop()
 *
 * Description:
 *      this API stops a timer. it performs
 *      1. check if timer mudule is initiated.
 *      2. check handle validity.
 *      3. remove timer struct from the global timer doubly linked list.
 *
 * Inputs: -
 *      h_timer_hdl: timer handle returned from x_timer_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : timer module has not been initiated.
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
x_timer_stop(HANDLE_T h_timer_hdl)
{
    UINT32 i;
    MGC_NoneTimerWrapper *prTimer = (MGC_NoneTimerWrapper *)h_timer_hdl;
    
    // save timer handle
    for (i=0; i<MUSB_MAX_HW_TIMER; i++)
    {
        if (_aprTimer[i] == prTimer) 
        {
            _aprTimer[i] = NULL;

            _NonOSTimerOff();
        }
    }

    return OSR_OK;
}

//-----------------------------------------------------------------------------
/** vUsbIrq(): usb interrupt handler.
 *  @param vector_id: an vector id to register an ISR.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void vUsbIrq(UINT32 vector_id)
{
    UINT32 i;
    MUSB_ISR_T *pISR = _arUSBISRHandler;
    
    for (i=0; i<_u4USBISRHandlerNo; i++)
    {
        if (vector_id == (UINT32)pISR->ui2_vec_id)
        {
            if (pISR->pf_isr)
            {
                pISR->pf_isr(pISR->ui2_vec_id);
            }
            break;
        }
        pISR ++;
    }
}

/*----------------------------------------------------------------------------
 * Function: x_reg_isr()
 *
 * Description:
 *      this API registers an ISR with its vector id. it performs
 *      1. parse argument.
 *      2. guard isr.
 *      3. call OS driver reg isr API.
 *
 * Inputs:
 *      ui2_vec_id: an vector id to register an ISR.
 *      pf_isr: pointer to a ISR to set.
 *      ppf_old_isr: pointer to hold the current ISR setting.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_INV_ARG: invalid argument.
 *      OSR_FAIL: OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
x_reg_isr(UINT16         ui2_vec_id,
          x_os_isr_fct   pf_isr,
          x_os_isr_fct   *ppf_old_isr)
{
    MUSB_ISR_T *pISR;
    
    UNUSED(ppf_old_isr);
    MUSB_ASSERT(_u4USBISRHandlerNo < MUSB_MAX_CONTROLLERS);

    pISR = &_arUSBISRHandler[_u4USBISRHandlerNo++];
    pISR->pf_isr = pf_isr;
    pISR->ui2_vec_id = ui2_vec_id;
    ppf_old_isr = NULL;

#ifdef MUSB_LOADER
    _NonOSSetupUsbTimerIsr();
#endif    
    return OSR_OK;
}
