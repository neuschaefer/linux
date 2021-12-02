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

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_bim_mt82.h"
#include "x_ckgen_mt82.h"
#else
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_ckgen.h"
#endif
#include "x_os.h"
#include "drvcust_if.h"
#include "x_lint.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_entry.h"
#include "pdwnc_gpio.h"
#include "pdwnc_t8032.h"
#include "pdwnc_srvad.h"
#include "pdwnc_hw.h"
#include "pdwnc_drvif.h"
//#include "pdwnc_if.h"

//#undef PDWNC_WRITE32
//#undef PDWNC_READ32
EXTERN UINT8 _au1CecData[];

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DEFAULT_RESET_COUNTER       (DRVCUST_InitGet(eSysResetTimer))


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
EXTERN void HAL_Delay_us(UINT32 u4Micros);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT32 PDWNC_IsBootByWakeup(void)
{
	return (IO32ReadFldAlign(PDWNC_WKRSC, FLD_WKRSC) == DEFAULT_RESET_COUNTER);
}
void PDWNC_SetupPowerDown(void)
{
    UINT32 u4Val;

//for current reduction
#ifdef MT5396_TODO
//    vIO32Write4B(PDWNC_XTALCFG, 0x0e0000f0);
	vIO32Write4B(PDWNC_CONCFG0, 0x01ee0000);//default is 0x01ee0000//bit21: 27MHz clock to VGA enable
#endif

    // Disable watchdog timer.
    vIO32Write4B(PDWNC_WDTCTL, 0);

    // Set reset count from 16/27 seconds to 5/27 seconds
    vIO32WriteFldAlign(PDWNC_WKRSC, DEFAULT_RESET_COUNTER, FLD_WKRSC);

    // Turn off all Interrupt.
    BIM_WRITE32(REG_RW_IRQCL, 0xffffffff);
    BIM_WRITE32(REG_RW_IRQEN, 0);
    BIM_WRITE32(REG_RW_FIQCL, 0xffffffff);
    BIM_WRITE32(REG_RW_FIQEN, 0);

    _PDWNC_SetupStrapping();
    /* wait more than 1.5 IR clock cycle for IR busy bit clear */
    for (u4Val=0; u4Val<512; u4Val++) { }
    /* Do BIM clear IR busy bit. */
    vIO32WriteFldAlign(PDWNC_IRCLR, 0x1, FLD_IRCLR);
//david: Notify VideoSlt to change string parser mode from mt53xx to mt822x
   // HAL_Delay_us(150 * 1000);
    Printf("SWITCH_T8032\n");
  //  HAL_Delay_us(150 * 1000);

#ifdef CC_MT8223
  BIM_WRITE32(0xe04, 0);// switch uart to arm
  IO_WRITE32(PDWNC_BASE, 4, 0x6); // CLOCK FOR 8032 RUN NORMAL
#endif
#ifndef CC_UP8032_ATV
    // Enter Power Down mode by T8032 if uP is enabled.

    if( (DRVCUST_InitGet(eT8032uPOffset) == 0)  || !_PDWNC_SetT8032(PDWNC_T8032_SET_STANDBY))
    {
#ifdef CC_MT8223
		//while(1);
		x_thread_delay(0x10);
    	BIM_WRITE32(0xe04, 0xe2);// switch uart to arm    	
		Printf("--------in pdwnc_entry  ->8032 not run!!!\n");
		//HAL_Delay_us(150 * 1000);
#else		
        Printf("Error!! ARM enter standby state by itself!!\n");
#endif
        // Enter Power Down mode by ARM if uP is disabled.
        vIO32WriteFldAlign(PDWNC_PDCODE, PWRDWN_CODE1, FLD_PDCODE);
        vIO32WriteFldAlign(PDWNC_PDCODE, PWRDWN_CODE2, FLD_PDCODE);
    }
    PDWNC_Reboot();
    while (1) { }
#endif	
}

