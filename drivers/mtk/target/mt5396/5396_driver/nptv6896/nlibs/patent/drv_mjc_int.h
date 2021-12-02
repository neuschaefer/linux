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
 * $RCSfile: drv_mjc_int.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_INT_H_
#define _DRV_MJC_INT_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "hw_mjc.h"
#include "drv_mjc.h"

#include "typedef.h"
#include "video_def.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#define MAX_FBCK_LVL 0x40

#define MJC_FM_VDO              0
#define MJC_FM_32               1
#define MJC_FM_22               2
#define MJC_FM_2332             3
#define MJC_FM_2224             4
#define MJC_FM_32322            5

#define AbsDiff(a,b) ((a)>=(b) ? (a)-(b) : (b)-(a))
#define u2Abs9(u2Val) (((u2Val)& 0x100)? (((~(u2Val))+1)&0x1FF) : (u2Val))
#define u1Abs8(u1Val) (((u1Val)&  0x80)? ((~(u1Val))+1) : (u1Val))

typedef enum
{
    E_ME_POS_F = 0, // forward
    E_ME_POS_C = 1, // center (bi-directional)
    E_ME_POS_B = 2, // backward
    E_ME_POS_A = 3  // auto switch (programmable)
} E_MJC_ME_POS_OTHR;

typedef struct
{
    UINT32 u4Cfg0;  // MJC_FM_10
    UINT32 u4Cfg1;  // MJC_FM_11
} MJC_ME_POS_OTHR_REG_T;

typedef enum
{
    E_ME_LBOX_SA_AUTO = 0,
    E_ME_LBOX_SA_MANUAL = 1,
} E_MJC_LBOX_ME_SA;

typedef struct
{
    UINT16 u2PosUp;
    UINT16 u2PosDn;
    UINT16 u2PosLf;
    UINT16 u2PosRt;
    BOOL fgIsYSmall;
} MJC_LBOX_STA_T;

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
EXTERN UINT16 u2MajorCnt0;
EXTERN UINT8  u1MajorMvX0;
EXTERN UINT8  u1MajorMvY0;

EXTERN UINT16 u2StaNrmvsi;
EXTERN UINT16 u2StaBdrvsi;
EXTERN UINT16 u2MaxMvErr;
EXTERN UINT16 u2BadMotCnt;
EXTERN UINT16 u2OsdErr;
EXTERN UINT16 u2LrgV;
EXTERN UINT16 u2StaMEBec;
EXTERN UINT16 u2StaCustVal;
EXTERN UINT16 u2TotalCnt;
EXTERN UINT16 u2TotalMotCnt;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void vDrvMJCInitQuality(void);
void vDrvMJCSetQualityISR(void);

void vDrvMJCSetMEScnChgTh_Mthd1(UINT32 u4Slope, UINT32 u4LowerBnd, UINT32 u4UpperBnd);
void vDrvMJCSetMEScnChgTh_Mthd2(UINT32 u4Slope, UINT32 u4LowerBnd, UINT32 u4SelTh);
UINT32 u4DrvGetStaNormLbox(UINT32 u4Val);
UINT32 u4DrvGetStaNormTotalCnt(UINT32 u4Val);

void vDrvMJCUpdateIntpCtrl(void);
void vDrvMJCMEStaPolling(void *pvArgv);

void vDrvMJCBadEdit(void);
void vDrvMJCSetMVDS(UINT8 u1MvdsVal);

void vDrvMJCSetClearMVTrig(void);

void vDrvMJCSetOSDIntp(UINT32 u4OnOff, UINT32 u4IntpCtrl);

UINT8 u1DrvMJCGetFilmFlag(void);
UINT8 u1DrvMJCGetOutLR(void);
void vDrvMJCSetOSDMask(UINT32 u4Mask, UINT32 u4Left, UINT32 u4Top, UINT32 u4Right, UINT32 u4Bottom);
void vDrvMJCSetDemoBar(UINT8 fgOnOff, UINT8 u1R, UINT8 u1G, UINT8 u1B);
void vDrvMJCSetMCOnOff(UINT8 fgOnOff);

#endif //#ifndef _DRV_MJC_H_

