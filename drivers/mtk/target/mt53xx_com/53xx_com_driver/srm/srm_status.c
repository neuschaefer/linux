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
 * $RCSfile: srm_status.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file srm_status.c
 *  Brief of file srm_status.c
 *  Details of file srm_status.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "srm_debug.h"
#include "aud_if.h"
#include "osd_drvif.h"
#include "hw_di.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "x_timer.h"
#include "x_os.h"
#include "x_hal_io.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void AudSetAoutTest(void);
extern void AudResetAoutStatus(void);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define CHECK_ERROR(error, mod)                                                \
    do                                                                         \
    {                                                                          \
        if (error > 0)                                                         \
        {                                                                      \
            Printf("%s fifo error = %x\n", mod, error);                        \
        }                                                                      \
    }                                                                          \
    while (0)


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

VOID SRM_QuesyFifoStatus(VOID)
{
    UINT32 u4Error = 0;
#if 1
    // Audio
    AudSetAoutTest();
    u4Error = AudGetAoutStatus();
    CHECK_ERROR(u4Error, "AUD");
    
    // OSD
    u4Error = IO_READ32MSK(0, OSD_INT_STATUS_REG, 
                           (OSD1_CFUD_IID | OSD2_CFUD_IID | OSD3_CFUD_IID));
    CHECK_ERROR(u4Error, "OSD");

#ifdef MDDI_DA_13
    // NR
    u4Error = IO32ReadFldAlign(VDOIN_BASE + MDDI_DA_13, STA_NR_UNDERFLOW);
    CHECK_ERROR(u4Error, "NR_UNDERFLOW");

    // PSCAN
    u4Error = IO32ReadFldAlign(VDOIN_BASE + MDDI_DA_13, STA_FLAG_UNDERFLOW);
    CHECK_ERROR(u4Error, "PSCAN_FLAG_UNDERFLOW");
    u4Error = IO32ReadFldAlign(VDOIN_BASE + MDDI_DA_13, STA_FLAG_OVERFLOW);
    CHECK_ERROR(u4Error, "PSCAN_FLAG_OVERFLOW");
    u4Error = IO32ReadFldAlign(VDOIN_BASE + MDDI_DA_13, STA_UNDERFLOW);
    CHECK_ERROR(u4Error, "PSCAN_UNDERFLOW");
    u4Error = IO32ReadFldAlign(VDOIN_BASE + MDDI_DA_13, STA_OVERFLOW);
    CHECK_ERROR(u4Error, "PSCAN_OVERFLOW");
#endif /* MDDI_DA_13 */

#endif
}


VOID SRM_ResetFifoStatus(VOID)
{
#if 1
    // OSD
    IO_WRITE32MSK(0, OSD_INT_STATUS_REG, 0,
                  (OSD1_CFUD_IID | OSD2_CFUD_IID | OSD3_CFUD_IID));
    x_thread_delay(20);

#if defined(MDDI_DA_00) && defined(RG_STATUS_CLR)
    // NR & PSCAN
    vIO32WriteFldAlign(VDOIN_BASE + MDDI_DA_00, 1, RG_STATUS_CLR);
    x_thread_delay(20);
    vIO32WriteFldAlign(VDOIN_BASE + MDDI_DA_00, 0, RG_STATUS_CLR);
#endif /* defined(MDDI_DA_00) && defined(RG_STATUS_CLR) */
	// audio
	AudResetAoutStatus();
#endif
}
