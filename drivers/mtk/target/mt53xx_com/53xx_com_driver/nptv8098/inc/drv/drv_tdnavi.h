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
 * $RCSfile: drv_tdnavi.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_TD_NAVI_H_
#define _DRV_TD_NAVI_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "typedef.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "video_timing.h"


EXTERN UINT8 g_u1TdNaviDetInFrmNum;
EXTERN UINT8 g_u1TdNaviDetOutFrmNum;

EXTERN UINT8 g_u1In3DSimilarityRatio; 
EXTERN UINT8 g_u1In3DSimilarThr;
EXTERN UINT8 g_u1In3DDecisionType;
EXTERN UINT8 g_u1Out3DSimilarityRatio;
EXTERN UINT8 g_u1Out3DSimilarThr;
EXTERN UINT8 g_u1Out3DDecisionType;
EXTERN UINT8 g_u1TdNaviDetNew;
EXTERN UINT8 g_u1TdNaviDetCurr;
EXTERN UINT8 g_u1TdNaviDet2DCnt;
EXTERN UINT8 g_u1TdNaviDetSbsCnt;
EXTERN UINT8 g_u1TdNaviDetTabCnt;
EXTERN UINT8 g_u1TdNaviAdaptOnOff;
EXTERN UINT8 g_u1TdNaviPrint; 
EXTERN UINT8 g_u1TdNaviTrigger;

EXTERN UINT8 g_u1RealDInDiffThr[2];
EXTERN UINT8 g_u1RealDOutDiffThr[2];
EXTERN UINT8 g_u1RealDInFrmThr[2];
EXTERN UINT8 g_u1RealDOutFrmThr[2];
EXTERN UINT16 g_u2RealDInQuaThr[2];
EXTERN UINT16 g_u2RealDOutQuaThr[2];
EXTERN UINT16 g_u2RealDInQuaThrRatio;
EXTERN UINT16 g_u2RealDOutQuaThrRatio;

EXTERN UINT8 g_u1RealDAdaptEn[2];

EXTERN UINT16 g_u2RealDPrint;
EXTERN UINT8 g_u1RealDTrigger;

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
#if 0
#define vIO32TdNaviWriteFldAlign(reg32,val,fld) /*lint -save -e506 -e504 -e514 -e62 -e737 -e572 -e961 -e648 -e701 -e732 -e571 */ \
    (Fld_ac(fld)==AC_FULLDW)?vIO32Write4B((reg32),(val)):( \
     vIO32Write4BMsk((reg32),((UINT32)(val)<<Fld_shft(fld)),Fld2Msk32(fld))) /*lint -restore */

#define TD_NAVI_WRITE_4BMSK(u4Addr, u4Value, u4Msk) vIO32Write4BMsk(((u4Addr) + IO_ADDR_OFST), (u4Value), (u4Msk))
#define TD_NAVI_WRITE_FLD(u4Addr, u4Value, u4Fld) vIO32TdNaviWriteFldAlign(((u4Addr) + IO_ADDR_OFST), (u4Value), (u4Fld))
#define TD_NAVI_READ_FLD(u4Addr, u4Fld) IO32ReadFldAlign(((u4Addr) + IO_ADDR_OFST), (u4Fld))
#endif

#define FW_NAVI_WINDOW_WIDTH 256
#define FW_NAVI_WINDOW_HEIGHT 128

#define TDNVAI_REGTBL_END 0xffffffff
#define REALD_REGTBL_END 0xffffffff
#define TD22D_REGTBL_END 0xffffffff

typedef enum
{
    TD_NAVI_DETECT_2D = 0,
    TD_NAVI_DETECT_SBS,
    TD_NAVI_DETECT_TAB,
    TD_NAVI_DETECT_CB,    
    TD_NAVI_DETECT_UNKNOWN,
#if 0 // For Interface , Should not return unkown status to other module
    TD_NAVI_DETECT_RESERVED,
    TD_NAVI_DETECT_TOO_FEW,
    TD_NAVI_DETECT_AMBIGUO,
    TD_NAVI_DETECT_NEIGHBO,
#endif
    TD_NAVI_DETECT_MAX
}TD_NAVI_DETECT;

enum
{
    REALD_DETECT_2D = 0,
    REALD_DETECT_RD,
    REALD_DETECT_MAX
};

enum
{
    TD22D_MODE_FRAME_SEQUENTIAL = 0,
    TD22D_MODE_SIDE_BY_SIDE,
    TD22D_MODE_TOP_AND_BOTTOM,
    TD22D_MODE_LINE_INTERLEAVE,
    TD22D_MODE_MAX
};

enum
{
    TD22D_FS_LEFT = 0,
    TD22D_FS_RIGHT
};

enum
{
    TD22D_SBS_RIGHT = 0,
    TD22D_SBS_LEFT
};

enum
{
    TD22D_TB_BOTTOM = 0,
    TD22D_TB_TOP
};

enum
{
    TD22D_LI_EVEN_LINE = 0,
    TD22D_LI_ODD_LINE
};


typedef struct
{
    UINT32  u2Addr;
    UINT32  u4Value;
    UINT32  u4Mask;
} TD_NAVI_REGTBL_T;

typedef struct
{
    UINT32  u2Addr;
    UINT32  u4Value;
    UINT32  u4Mask;
} REALD_REGTBL_T;

typedef struct
{
    UINT32  u2Addr;
    UINT32  u4Value;
    UINT32  u4Mask;
} TD22D_REGTBL_T;

typedef struct
{
    UINT16 u2Width;
    UINT16 u2Height;
    UINT8 u1TD22DEn;
    UINT8 u1MaskSel;
    UINT8 u1ModeSel;
    UINT8 u1IsIntr;
}TD22D_INFO;

EXTERN TD22D_INFO gMain3DTo2DInfo;
EXTERN TD22D_INFO gPip3DTo2DInfo;

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/

// 3D NAVI
void vDrvTdNaviLoadRegTbl(TD_NAVI_REGTBL_T * prRegTbl);
void vDrvTdNaviSetPicSize(void);
UINT8 u1DrvTdNaviModeChange(void);
void vDrvTdNaviInit(void);
void vDrvTdNaviMainloop(void);
UINT8 vDrvTdNaviGetSbsCnt(void);
UINT8 vDrvTdNaviGetTabCnt(void);
UINT8 vDrvTdNaviGetSbsVldBlk(void);
UINT8 vDrvTdNaviGetTabVldBlk(void);
UINT8 vDrvTdNaviGetNewDetType(void);
UINT8 vDrvTdNaviGetCurrDetType(void);
void vDrvTdNaviSetSimilarityRatio(UINT8 u1Ratio);
void vDrvTdNaviSetSimilarityThr(UINT8 u1Thr);
void vDrvTdNaviSetDecisionType(UINT8 u1Type);
void vDrvTdNaviSetDbgType(UINT8 u1Type);

// REALD TAG
void vDrvRealDLoadRegTbl(REALD_REGTBL_T * prRegTbl);
void vDrvRealDSetSourceMode(UINT8 u1VdpId);
UINT8 u1DrvRealDModeChange(UINT8 u1VdpId);
void vDrvRealDInit(void);
void vDrvRealDMainloop(UINT8 u1VdpId);
void vDrvRealDSetDetectMode(UINT8 u1VdpId, UINT8 u1DetMode);
UINT8 u1DrvRealDGetDetectMode(UINT8 u1VdpId);
void vDrvRealDSetInDiffThr(UINT8 u1VdpId, UINT8 u1InThr);
void vDrvRealDSetOutDiffThr(UINT8 u1VdpId, UINT8 u1OutThr);
UINT8 u1DrvRealDGetCurrDetType(UINT8 u1VdpId);
UINT16 u2DrvRealDGetQuaSta(UINT8 u1VdpId);
void u2DrvRealDQuery(void);

// 3D-to-2D
void vDrvTD22DLoadRegTbl(TD22D_REGTBL_T * prRegTbl);
UINT8 u1DrvTD22DSetMode(UINT8 u1VdpId, UINT8 u1TD22DEn, UINT8 u1ModeSel, UINT8 u1MaskSel);
void vDrvTD22DInit(void);

UINT32 vDrvTDIsFldInv(UINT32 u1VdpId);

TD_NAVI_DETECT eDrvSwNaviDetectedFmtQuery(void);
void vDrvTdNaviFWDetectionInit(void);
void vDrvTdNaviFWDetectionProc(void);


#endif //#ifndef _DRV_TD_NAVI_H_
