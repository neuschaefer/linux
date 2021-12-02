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
 * $RCSfile: imgrz_dif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_dif.c
 *  image resizer driver data interface layer
 *  
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "imgrz_dif.h"
#include "imgrz_hw.h"
#include "imgrz_sw.h"
#include "imgrz_cmdque.h"
#include "imgrz_debug.h"


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

static IMGRZ_DIF_T _rRzDifData =
{
    (UINT32)(D_RZ_HAVE_SW_MOD + D_RZ_HAVE_HW_MOD),
    (UINT32)E_RZ_HW_MOD,
    (UINT32)0,
    (UINT32 *)NULL
};


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** pfnIMGRZ_DifAction
 *  command queue action function pointer
 */
//-------------------------------------------------------------------------
void (*pfnIMGRZ_DifAction)(void);


//-------------------------------------------------------------------------
/** pfnIMGRZ_DifFinNotify
 *  complete notification function pointer
 */
//-------------------------------------------------------------------------
void (*pfnIMGRZ_DifFinNotify)(UINT32);


//-------------------------------------------------------------------------
/** IMGRZ_DifGetData
 *  get dif data (pointer)
 */
//-------------------------------------------------------------------------
IMGRZ_DIF_T *IMGRZ_DifGetData(void)
{
    return &_rRzDifData;
}


//-------------------------------------------------------------------------
/** IMGRZ_DifSetNotify
 *  set notification function
 */
//-------------------------------------------------------------------------
void IMGRZ_DifSetNotify(void (*pfnNotify)(UINT32))
{
    pfnIMGRZ_DifFinNotify = pfnNotify;
}


//-------------------------------------------------------------------------
/** IMGRZ_DifInit
 *  dif init
 *  init software module
 *  init hardware module
 *  set initial state (idle)
 */
//-------------------------------------------------------------------------
void IMGRZ_DifInit(void)
{

#if defined(IMGRZ_ENABLE_SW_MODE)
//-----------------------------------------------------------
    IMGRZ_SwInit();

    IMGRZ_DifSetMode((UINT32)E_RZ_SW_MOD);
    
    MA_DIF_IMGRZ_SW_MOD_OK();
//-----------------------------------------------------------
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)


    // setup ISR
    // reset hw and all counters
    // status check
    if (MA_DIF_HAVE_IMGRZ_HW)
    {
        IMGRZ_HwInit();
        
        MA_DIF_IMGRZ_HW_MOD_OK();
        
        IMGRZ_DifSetMode((UINT32)E_RZ_HW_MOD);
    }
}


//-------------------------------------------------------------------------
/** IMGRZ_DifSetRegBase
 *  set dif register base address
 */
//-------------------------------------------------------------------------
void IMGRZ_DifSetRegBase(UINT32 *pu4Base)
{
    IMGRZ_ASSERT(pu4Base != NULL);

    _rRzDifData.pu4CrBase = pu4Base;
}


//-------------------------------------------------------------------------
/** IMGRZ_DifGetRegBase
 *  get dif register base address
 */
//-------------------------------------------------------------------------
void IMGRZ_DifGetRegBase(UINT32 *pu4Base)
{
    IMGRZ_ASSERT(pu4Base != NULL);

    *pu4Base = (UINT32)(_rRzDifData.pu4CrBase);
}


//-------------------------------------------------------------------------
/** IMGRZ_DifReset
 *  dif reset
 *  reset hardware module and software module
 */
//-------------------------------------------------------------------------
void IMGRZ_DifReset(void)
{
    // if HW engine is available, just reset it
    if (MA_DIF_HAVE_IMGRZ_HW)
    {
        IMGRZ_HwReset();
    }
    else
    {
        // else erase software control register file
        IMGRZ_UNUSED_RET(x_memset(_rRzDifData.pu4CrBase, 0, 
                (sizeof(UINT32) * IMGRZ_REG_FILE_SIZE)))
    }
}


//-------------------------------------------------------------------------
/** IMGRZ_DifSetMode
 *  set operation mode (hardware/software)
 *  change function pointers and re-init command queue
 */
//-------------------------------------------------------------------------
void IMGRZ_DifSetMode(UINT32 u4Mode)
{
    UINT32 u4RzRegBase;
    UINT32 *pu4RzRegBase = &u4RzRegBase;

    _rRzDifData.u4ImgrzMode = u4Mode;
    IMGRZ_DBG_REC_ENG_EXE_MOD((INT32)u4Mode);   // for debug use
        
    // hw mode setting
    if ((UINT32)E_RZ_HW_MOD == u4Mode)
    {
        pfnIMGRZ_DifAction = IMGRZ_HwAction;
        
        IMGRZ_HwGetRegBase(&pu4RzRegBase);
        
        IMGRZ_DifSetRegBase(pu4RzRegBase);
    }


#if defined(IMGRZ_ENABLE_SW_MODE)
    // sw mode setting
    if ((UINT32)E_RZ_SW_MOD == u4Mode)
    {
        pfnIMGRZ_DifAction = IMGRZ_SwAction;
        
        IMGRZ_SwGetRegBase(&pu4RzRegBase);
        
        IMGRZ_DifSetRegBase(pu4RzRegBase);
    }

    // for emu-cli use
    IMGRZ_CmdQueInit();
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)

}


