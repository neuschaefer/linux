/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *---------------------------------------------------------------------------
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

#define _rRzDifData (rRzDifData[u4Which])

static IMGRZ_DIF_T rRzDifData[IMGRZ_NUM] =\
{\
    { (UINT32)(D_RZ_HAVE_SW_MOD + D_RZ_HAVE_HW_MOD),
    (UINT32)E_RZ_HW_MOD,
    (UINT32)0,
    (UINT32 *)NULL }
#if (defined(CC_MT5396))
    ,
    { (UINT32)(D_RZ_HAVE_SW_MOD + D_RZ_HAVE_HW_MOD),
    (UINT32)E_RZ_HW_MOD,
    (UINT32)0,
    (UINT32 *)NULL }
#endif
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

CB_IMGRZ_DifAction _pfnIMGRZ_DifActions[IMGRZ_NUM];
CB_IMGRZ_DifFinNotify _pfnIMGRZ_DifFinNotifys[IMGRZ_NUM];
//-------------------------------------------------------------------------
/** IMGRZ_DifGetData
 *  get dif data (pointer)
 */
//-------------------------------------------------------------------------
IMGRZ_DIF_T *IMGRZ_DifGetData(UINT32 u4Which)
{
    return &_rRzDifData;
}


//-------------------------------------------------------------------------
/** IMGRZ_DifSetNotify
 *  set notification function
 */
//-------------------------------------------------------------------------
void IMGRZ_DifSetNotify(UINT32 u4Which, void (*pfnNotify)(UINT32))
{
    pfnIMGRZ_DifFinNotify = (CB_IMGRZ_DifFinNotify)pfnNotify;
}


//-------------------------------------------------------------------------
/** IMGRZ_DifInit
 *  dif init
 *  init software module
 *  init hardware module
 *  set initial state (idle)
 */
//-------------------------------------------------------------------------
void IMGRZ_DifInit(UINT32 u4Which)
{

#if defined(IMGRZ_ENABLE_SW_MODE)
//-----------------------------------------------------------
    IMGRZ_SwInit(u4Which);

    IMGRZ_DifSetMode(u4Which, (UINT32)E_RZ_SW_MOD);
    
    MA_DIF_IMGRZ_SW_MOD_OK();
//-----------------------------------------------------------
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)


    // setup ISR
    // reset hw and all counters
    // status check
    if (MA_DIF_HAVE_IMGRZ_HW)
    {
        IMGRZ_HwInit(u4Which);
        
        MA_DIF_IMGRZ_HW_MOD_OK();
        
        IMGRZ_DifSetMode(u4Which, (UINT32)E_RZ_HW_MOD);
    }
}


//-------------------------------------------------------------------------
/** IMGRZ_DifSetRegBase
 *  set dif register base address
 */
//-------------------------------------------------------------------------
void IMGRZ_DifSetRegBase(UINT32 u4Which, UINT32 *pu4Base)
{
    IMGRZ_ASSERT(pu4Base != NULL);

    _rRzDifData.pu4CrBase = pu4Base;
}


//-------------------------------------------------------------------------
/** IMGRZ_DifGetRegBase
 *  get dif register base address
 */
//-------------------------------------------------------------------------
void IMGRZ_DifGetRegBase(UINT32 u4Which, UINT32 *pu4Base)
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
void IMGRZ_DifReset(UINT32 u4Which)
{
    // if HW engine is available, just reset it
    if (MA_DIF_HAVE_IMGRZ_HW)
    {
        IMGRZ_HwReset(u4Which);
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
void IMGRZ_DifSetMode(UINT32 u4Which, UINT32 u4Mode)
{
    UINT32 u4RzRegBase;
    UINT32 *pu4RzRegBase = &u4RzRegBase;

    _rRzDifData.u4ImgrzMode = u4Mode;
    IMGRZ_DBG_REC_ENG_EXE_MOD(u4Which, (INT32)u4Mode);   // for debug use
        
    // hw mode setting
    if ((UINT32)E_RZ_HW_MOD == u4Mode)
    {
        pfnIMGRZ_DifAction = (CB_IMGRZ_DifAction)IMGRZ_HwAction;
        
        IMGRZ_HwGetRegBase(u4Which, &pu4RzRegBase);
        
        IMGRZ_DifSetRegBase(u4Which, pu4RzRegBase);
    }


#if defined(IMGRZ_ENABLE_SW_MODE)
    // sw mode setting
    if ((UINT32)E_RZ_SW_MOD == u4Mode)
    {
        pfnIMGRZ_DifAction = (CB_IMGRZ_DifAction)IMGRZ_SwAction;
        
        IMGRZ_SwGetRegBase(u4Which, &pu4RzRegBase);
        
        IMGRZ_DifSetRegBase(u4Which, pu4RzRegBase);
    }

    // for emu-cli use
    IMGRZ_CmdQueInit(u4Which);
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)

}


