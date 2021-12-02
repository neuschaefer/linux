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
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mjc_fbck.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_FBCK_H_
#define _DRV_MJC_FBCK_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "typedef.h"
#include "video_def.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#define MJC_FB_FLAG_NRMVSI      (1<<0)
#define MJC_FB_FLAG_BDRVSI      (1<<1)
#define MJC_FB_FLAG_MVERR       (1<<2)
#define MJC_FB_FLAG_BADMOTCNT   (1<<3)
#define MJC_FB_FLAG_OSDERR      (1<<4)
#define MJC_FB_FLAG_LRGV        (1<<5)
#define MJC_FB_FLAG_BEC         (1<<6)
#define MJC_FB_FLAG_CUST        (1<<7)

#define MJC_FB_QUANT_TBL_NUM    16

#define MJC_AVG_FIFO_WD         4
#define MJC_AVG_FIFO_NUM        (1<<MJC_AVG_FIFO_WD)
#define MJC_AVG_FIFO_MOD_MSK    (MJC_AVG_FIFO_NUM-1)

typedef UINT8 (*PFN_FBCK_REMAP)(UINT8 u1Lvl, UINT8 u1Effect);

enum
{
    MJC_FB_NRMVSI = 0,
    MJC_FB_BDRVSI,
    MJC_FB_MVERR,
    MJC_FB_BADMOTCNT,
    MJC_FB_OSDERR,
    MJC_FB_LRGV,
    MJC_FB_BEC,
    MJC_FB_CUST,
    MJC_FB_NUM
};

typedef struct
{
    UINT8   u1StaMax;
    UINT8   u1SysSta;
    UINT8   u1IncOfst;
    INT16   i2DiffAccm;
    INT16   i2IncTh;
    INT16   i2DecTh;
} MJC_ERFB_T;

typedef struct
{
    UINT16  u2Fifo[MJC_AVG_FIFO_NUM];
    UINT32  u4Sum;
    UINT8   u1Cursor;
} MJC_AVG_T;


/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
extern UINT8 u1MJCFbckCtrl;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void   vDrvInitFbck(void);
UINT8  u1DrvCalPartialFbck(BOOL fgIsFilm);
UINT8  u1DrvCalPartialFbckAG(void);
void   vDrvInitAvgFilter(void);
UINT16 u2DrvAvgFilter(UINT8 u1Index, UINT16 u2AddOnData);
void vDrvInitErrorFeedBack(void);
UINT8  u1DrvErrorFeedBack(UINT8 u1Value);
void   vDrvSetPartialFbck(BOOL fgIsFilm, UINT8 u1Level);
//void   vDrvSetVdoPartialFbck(UINT8 u1Level);
void vDrvMJCFbckModeChg(void);

void vDrvSetFallbackRemapFunctor(PFN_FBCK_REMAP pfn);

#endif //#ifndef _DRV_MJC_FBCK_H_
