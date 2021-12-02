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
 * $RCSfile: imgrz_dif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_dif.h
 *  image resizer driver data interface
 *  include common interface of hardware and software engine
 *  get/set register base address
 *  get/set idle state
 *  init modules
 */
 
#ifndef IMGRZ_DIF_H
#define IMGRZ_DIF_H

#ifndef CC_MT5363
    #ifdef  CC_MT5365
    #define CC_MT5363
    #define JPG_CLK_FROM_GRAPHIC
    #endif

    #ifdef  CC_MT5395
    #define CC_MT5363
    #define JPG_CLK_FROM_GRAPHIC
    #define JPG_CLK_FROM_GRAPHIC_5395_PLL
    #endif

    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    #define CC_MT5363
    #define RZ_CLK_FROM_IMGRZ_CKCFG
    #endif      
#endif

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "imgrz_dif_reg.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// u4Config
#define MA_DIF_HAVE_IMGRZ_SW              \
    (IMGRZ_DifGetData(u4Which)->u4Config & D_RZ_HAVE_SW_MOD)
    
#define MA_DIF_HAVE_IMGRZ_HW          \
    (IMGRZ_DifGetData(u4Which)->u4Config & D_RZ_HAVE_HW_MOD)

// u4ModInit
#define MA_DIF_IMGRZ_SW_MOD_OK()          \
    (IMGRZ_DifGetData(u4Which)->u4ModInit |= D_RZ_HAVE_SW_MOD)
    
#define MA_DIF_IMGRZ_HW_MOD_OK()      \
    (IMGRZ_DifGetData(u4Which)->u4ModInit |= D_RZ_HAVE_HW_MOD)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

// system capabilities
//  uiConfig - SW_MOD, HW_MOD, etc
//  uiModInit - init bit flag
typedef struct _IMGRZ_DIF_T
{
    UINT32 u4Config;        // configuration bits, modules
    UINT32 u4ImgrzMode;     // operation mode / module
    UINT32 u4ModInit;       // configuration bits, modules status
    UINT32 *pu4CrBase;      // control register base address
} IMGRZ_DIF_T;


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern void IMGRZ_DifInit(UINT32 u4Which);

extern void IMGRZ_DifSetRegBase(UINT32 u4Which,UINT32 *pu4Base);

extern void IMGRZ_DifGetRegBase(UINT32 u4Which,UINT32 *pu4Base);

extern void IMGRZ_DifReset(UINT32 u4Which);

extern void IMGRZ_DifSetMode(UINT32 u4Which, UINT32 u4Mode);

extern void IMGRZ_DifSetNotify(UINT32 u4Which, void (*pfnNotify)(UINT32));

extern IMGRZ_DIF_T *IMGRZ_DifGetData(UINT32 u4Which);

//-------------------------------------------------------------------------
/** pfnIMGRZ_DifAction
 *  command queue action function pointer
 */
//-------------------------------------------------------------------------
typedef void (*CB_IMGRZ_DifAction)(INT32 u4Which);
//-------------------------------------------------------------------------
/** pfnIMGRZ_DifFinNotify
 *  complete notification function pointer
 */
//-------------------------------------------------------------------------
typedef void (*CB_IMGRZ_DifFinNotify)(UINT32);
extern CB_IMGRZ_DifAction _pfnIMGRZ_DifActions[IMGRZ_NUM];
extern CB_IMGRZ_DifFinNotify _pfnIMGRZ_DifFinNotifys[IMGRZ_NUM];
#define pfnIMGRZ_DifAction      _pfnIMGRZ_DifActions[u4Which]
#define pfnIMGRZ_DifFinNotify   _pfnIMGRZ_DifFinNotifys[u4Which]

#endif // IMGRZ_DIF_H


