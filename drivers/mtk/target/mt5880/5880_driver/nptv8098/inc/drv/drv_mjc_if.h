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
 * $RCSfile: drv_mjc_if.h $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

#ifndef _DRV_MJC_IF_H_
#define _DRV_MJC_IF_H_

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "typedef.h"
#include "video_def.h"
#include "drv_mjc.h"

/*----------------------------------------------------------------------------*
 * Definitions
 *----------------------------------------------------------------------------*/
enum
{
    MJC_DEMO_OFF,
    MJC_DEMO_SIDEBYSIDE,
    MJC_DEMO_WINDOW
};

enum
{
    MJC_INTP_OFF,
    MJC_INTP_HALF,
    MJC_INTP_FULL
};

typedef enum
{
    E_NEW_DEMO_OFF = 0,
    E_NEW_DEMO_COL_LEFT_120,
    E_NEW_DEMO_COL_RIGHT_120,
    E_NEW_DEMO_COL_LEFT_240,
    E_NEW_DEMO_COL_RIGHT_240,
    E_NEW_DEMO_ROW_TOP_120,
    E_NEW_DEMO_ROW_BOT_120,
    E_NEW_DEMO_ROW_TOP_240,
    E_NEW_DEMO_ROW_BOT_240,
    E_NEW_DEMO_WINDOW_INSIDE,
    E_NEW_DEMO_WINDOW_OUTSIDE,
    E_NEW_DEMO_MAX
} E_MJC_NEW_DEMO_MODE;


typedef enum
{
    E_MJC_EFFECT_MODE_NO_REMAP = 0,
    E_MJC_EFFECT_MODE_DFT,
    E_MJC_EFFECT_MODE_2SEGMENT,
    E_MJC_EFFECT_MODE_LUT
} E_MJC_EFFECT_REMAP_MODE;

#define MJC_DRAM_VERF_MODE_NUM      17
#define MJC_DRAM_ALL_MODE_NUM       34
#define MJC_DRAM_MODE_QUERY         MJC_DRAM_ALL_MODE_NUM

#define NFRM_MAX                    2
#define RDCRSCN_MAX                 4
#define FLPMRR_MAX                  4
#define BIT_MAX                     6
#define COLOR_MAX                   4

// default fallback remap curve
typedef struct
{
    UINT8 u1Coring;
    UINT8 u1Offset;
    UINT8 u1Weight;
} MJC_FBCK_REMAP_DFT_T;

// fallback remap curve by 2 segments
// 1st segment => (0, y0), (x, y1)
// 2nd segment => (x, y2), (64, y3)
typedef struct
{
    UINT8 u1Y0, u1Y1, u1Y2, u1Y3;
    UINT8 u1X;
} MJC_FBCK_REMAP_2SEGMENT_T;


/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
extern MJC_FBCK_REMAP_DFT_T _rRemapDft;
extern MJC_FBCK_REMAP_2SEGMENT_T _rRemap2Seg;

/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
void MJC_SetOnOff(UINT8 u1OnOff);
void MJC_SetEffect(UINT32 u4Effect);
void MJC_SetClearLCD(UINT32 u4OnOff);
void MJC_SetDemo(UINT32 u4Mode, UINT32 u4Param1, UINT32 u4Param2);
void MJC_SetDemoNew(E_MJC_NEW_DEMO_MODE eMode, UINT32 u4Param);

void MJC_SetEffectRemapMode(E_MJC_EFFECT_REMAP_MODE eMode);
void MJC_SetFilmMode(BOOL fgOnOff);
void MJC_SetFilmModeTypeOnOff(UINT8 u1Path, UINT8 u1FilmType, UINT8 u1OnOff);
BOOL MJC_Set_FbckQTable(UINT8 u1TblIdx, const UINT16 u2Array[16]);
BOOL MJC_Set_FbckQMap(UINT8 u1TblIdx, const UINT8 u1MapArray[16]);
void MJC_SetBypassWindow(UINT8 u1Window, UINT32 u4Width, UINT32 u4Height, UINT32 u4StartX, UINT32 u4StartY);
void MJC_SetDemoBar(UINT8 fgOnOff, UINT8 r, UINT8 g, UINT8 b);
void MJC_SetMCOnOff(UINT8 fgOnOff);
void MJC_SetLRSwitch(BOOL fgSwap);

void MJC_SetDejudderLvl(UINT32 u4Lvl);
void MJC_SetDeblurLvl(UINT32 u4Lvl);

void MJC_SetPicSizeDbg(const MJC_PIC_SIZE_T* prMjcPicSize);
void MJC_SetFRCDbg(MJC_FRC_CONFIG_T* prMJCFRC);
void MJC_SetFrmSchDbg(UINT32 u4Mode);
void MJC_SetBufDbg(const MJC_DRAM_ALLOC_T* prMJCDram);
void MJC_SetDramModeDbg(UINT32 u4DramModeIdx);
void MJC_SetRemapDFTDbg(UINT8 u1Coring, UINT8 u1Offset, UINT8 u1Weight);

void MJC_VrfDramMode(UINT32 u4DramModeIdx);
void MJC_SetSystemModeAutoTest(void);
void vDrvMJCSetOutMuteOnOff(BOOL fgOnOff);

#endif //#ifndef _DRV_MJC_IF_H_

