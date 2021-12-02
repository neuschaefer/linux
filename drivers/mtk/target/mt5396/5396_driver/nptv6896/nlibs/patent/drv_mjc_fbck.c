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
 * $RCSfile: drv_mjc_fbck.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "hw_mjc.h"
#include "hw_sw.h"
#include "hw_vdoin.h"
#include "drv_mjc.h"
#include "drv_mjc_int.h"
#include "drv_mjc_fbck.h"
#include "drv_mjc_qty.h"
#include "drv_mjc_if.h"
#include "nptv_debug.h"
#include "drv_tdtv_drvif.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
#define INTP_RDCTYPE(gain) ((gain >> 6) & 0x02)

typedef enum
{
    MJC_FBK_MTHD_SPRT = 0,          // separate
    MJC_FBK_MTHD_TWD0 = 1,          // toward 0
    MJC_FBK_MTHD_32_TWD0 = 2,       // 32 fallback + toward 0
    MJC_FBK_MTHD_00224_32 = 3,      // 00224+32 fallback
    MJC_FBK_MTHD_00224_32_NSFT = 4, // 00224+32 fallback, no phase shift
    MJC_FBK_MTHD_32_TWD0_2 = 5,     // 32 fallback + toward 0 (w/o fallback to 60Hz)
    MJC_FBK_MTHD_32_FORCE_VDO = 6,  // force film mode as video
    MJC_FBK_MTHD_SPRT_2 = 7
} MJC_64_FBK_MTHD_TYPE;

typedef enum
{
    MJC_22_FBK_MTHD_TWD0 = 0,
    MJC_22_FBK_MTHD_31_TWD0 = 1,
    MJC_22_FBK_MTHD_TWD0_2 = 2,     // w/o fallback to 60Hz
    MJC_22_FBK_MTHD_FORCE_VDO = 3   // force film mode as video
} MJC_22_FBK_MTHD_TYPE;

typedef enum
{
    MJC_240_FBK_MTHD_SPRT = 0,      // separate
    MJC_240_FBK_MTHD_55_TWD0 = 1,   // 240 -> 120 -> 55 -> total 10:10
    MJC_240_FBK_MTHD_DEMO = 2
} MJC_240_FBK_MTHD_TYPE;

typedef struct
{
    UINT8 u1Pos0B5;
    UINT8 u1Pos2A5;
    UINT8 u1Pos2B5;
    UINT8 u1Pos4A5;
    UINT8 u1Pos4B5;
    UINT8 u1Pos1A5;
    UINT8 u1Pos1B5;
    UINT8 u1Pos3A5;
    UINT8 u1Pos3B5;
} MJC_64_FBK_INTP_POS_T;

typedef struct
{
    UINT8 u1Pos15;
    UINT8 u1Pos25;
    UINT8 u1Pos35;
    UINT8 u1Pos45;
} MJC_32_FBK_INTP_POS_T;

typedef struct
{
    UINT8 u1Pos[20];
} MJC_32_55P2_INTP_POS_T;

typedef struct
{
    UINT8 u1Pos[8];
} MJC_22_8X_INTP_POS_T;

typedef struct
{
    UINT16 u2ThHi;
    UINT16 u2ThLo;
    UINT16 u2CntThHi;
    UINT16 u2CntThLo;
    UINT8 u1Delta;
    UINT8 u1DeltaDec;
    UINT8 u1TargetLvl;
    UINT8 u1SubSample;
} MJC_FBK_STAGE_T;

typedef struct
{
    UINT8 u1State;
    UINT16 u2Count;
    UINT16 u2CountMaxTh;
} MJC_FBK_GEAR_T;

typedef struct
{
    UINT8 u1Mthd;
    UINT8 u1Mthd22;
    UINT8 u1Thr1;
    UINT8 u1Thr2;
    UINT8 u1Thr1_2;   
} MJC_FBK_THR_120_T;

typedef struct
{
    UINT8 u1Mthd;
    UINT8 u1Thr0;
    UINT8 u1Thr1;
    UINT8 u1Thr2;
    UINT8 u1Thr1_2;   
} MJC_FBK_THR_240_T;


MJC_AVG_T  arMJCAvg[MJC_FB_NUM];
MJC_ERFB_T arMJCErfb;

static MJC_FBK_THR_120_T _rFbkThr120 =
{
    MJC_FBK_MTHD_SPRT, MJC_22_FBK_MTHD_TWD0, 0x10, 0x20, 0x18
};

static MJC_FBK_THR_240_T _rFbkThr240 =
{
    MJC_240_FBK_MTHD_55_TWD0, 0x10, 0x20, 0x30, 0x18
};

static PFN_FBCK_REMAP _pfnRemap = NULL;

static UINT8 _u1FbckQnty22[65] =
{
    0 ,4 ,8 ,12,16,20,24,28,32,36,40,44,48,52,56,60,
    64,1 ,2 ,4 ,5 ,6 ,8 ,9 ,10,12,13,14,16,17,18,20,
    21,22,24,25,26,28,29,30,32,33,34,36,37,38,40,41,
    42,44,45,46,48,49,50,52,53,54,56,57,58,60,61,62,
    64
};

static UINT8 _u1FbckQnty32[65] =
{
    0 ,3 ,6 ,9 ,12,15,19,22,25,28,31,35,38,41,44,47,
    51,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,15,
    17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
    34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,
    51
};

static MJC_64_FBK_INTP_POS_T _r64FbkPos =
{
    0x33, 0x66, 0x99, 0xcc, 0xff, 0x33, 0x66, 0x99, 0xcc
};

static MJC_32_FBK_INTP_POS_T _r32FbkPos =
{
    0x33, 0x66, 0x99, 0xcc
};

static MJC_32_55P2_INTP_POS_T _r32Fbk55p2Pos =
{
    {0x00, 0x19, 0x33, 0x4c, 0x66, 0x80, 0x99, 0xb2, 0xcc, 0xe5,
     0x00, 0x19, 0x33, 0x4c, 0x66, 0x80, 0x99, 0xb2, 0xcc, 0xe5}
};

static MJC_22_8X_INTP_POS_T _r22FbkPos =
{
    {0x0, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0}
};

static MJC_FBK_STAGE_T _rStage[5] =
{
    {1500,  200, 100,  30,  8, 1,  0, 2},   // config for state 0
    {2000,  800, 200,  60,  8, 1, 16, 3},   // config for state 1
    {4000, 1400, 300, 160, 10, 2, 24, 4},   // config for state 2
    {6000, 2700, 400, 260, 12, 2, 32, 5},   // config for state 3
    {8000, 4350, 500, 360,  8, 3, 64, 5}    // config for state 4
};

static MJC_FBK_GEAR_T _rGear =
{
    0,                              // initial state
    0,                              // initial counter
    500,                            // max counter threshold
};

static UINT8 _rGaussW[33] =
{
	32, 31, 28, 24, 19, 14, 9, 6, 4, 2, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0
};

static UINT8 _rGmvWtTbl[16] =
{
    16, 16, 16, 16, 15, 15, 15, 14, 14, 13, 13, 12, 12, 11, 10, 10
};

EXTERN UINT8 _u1AbsGmvYPdct; // integer pixel, predicted gmv for next frame
EXTERN UINT8 _u1AbsGmvXPdct; // integer pixel, predicted gmv for next frame

EXTERN BOOL fgDisableHf;

EXTERN BOOL fgDrvMJCIsMEScnChg(void);

/*----------------------------------------------------------------------------*
 * Functions
 *----------------------------------------------------------------------------*/
UINT8 u1DrvMJCGetIntpMode(void)
{
    if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_2D)
    {
        switch (arMJCPrm.arFRCConfig.u1PanelType)
        {
        case E_MJC_240HZ:
            return E_MJC_2D_240;
        case E_MJC_60HZ:
            return E_MJC_2D_60;
        default:
        case E_MJC_120HZ:
            return E_MJC_2D_120;
        }
    }
    else if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_TTD)
    {
        switch (arMJCPrm.arFRCConfig.u1PanelType)
        {
        case E_MJC_240HZ:
            return E_MJC_TTD_240;
        default:
        case E_MJC_120HZ:
            return E_MJC_TTD_120;
        }
    }
    else
    {
        switch (arMJCPrm.arFRCConfig.u1PanelType)
        {
        case E_MJC_240HZ:
            return E_MJC_3D_120;
        case E_MJC_120HZ:
            return E_MJC_3D_60;
        default:
            return E_MJC_INTP_UNKNOWN;
        }
    }
}

static void _vDrvMJCCalFbckQntyArr(void)
{
    //CRIT_STATE_T csState;

    UINT16 i;
    UINT8 u1Mthd = IO32ReadFldAlign(MJC_FBCK_13, FBCK_MTHD_32);
    UINT8 u1Mthd22 = IO32ReadFldAlign(MJC_FBCK_13, FBCK_MTHD_22);
    UINT8 u1FbckThr = IO32ReadFldAlign(MJC_FBCK_13, FBCK_THR1);
    UINT8 u1FbckThr2 = IO32ReadFldAlign(MJC_FBCK_13, FBCK_THR2);
    UINT8 u1FbckThr1_2 = IO32ReadFldAlign(MJC_FBCK_13, FBCK_THR1_2);

    u1FbckThr = MIN(MAX(u1FbckThr, 0), 64);

    //csState = x_crit_start();
    _rFbkThr120.u1Mthd = u1Mthd;
    _rFbkThr120.u1Mthd22 = u1Mthd22;
    _rFbkThr120.u1Thr1 = u1FbckThr;
    _rFbkThr120.u1Thr2 = u1FbckThr2;
    _rFbkThr120.u1Thr1_2 = u1FbckThr1_2;

    for (i = 0; i < 65; i++)
    {
        if (i <= u1FbckThr)
        {
            _u1FbckQnty22[i] = (UINT8)(((UINT16) i * 0x40) / u1FbckThr);
            _u1FbckQnty32[i] = (UINT8)(((UINT16) i * 0x33) / u1FbckThr);
        }
        else
        {
            switch (u1Mthd22)
            {
            default:
            case MJC_22_FBK_MTHD_TWD0:
                _u1FbckQnty22[i] = (((UINT16) (i - u1FbckThr)) * 0x80) / (0x40 - u1FbckThr);
                break;
            case MJC_22_FBK_MTHD_31_TWD0:
                if (i <= u1FbckThr2)
                {
                    _u1FbckQnty22[i] = (((UINT16) (i - u1FbckThr)) * 0x80) / (u1FbckThr2 - u1FbckThr);
                }
                else
                {
                    _u1FbckQnty22[i] = ((UINT16) 128*(i-u1FbckThr2) / (64-u1FbckThr2));
                }
                break;
            }

            switch (u1Mthd)
            {
            default:
            case MJC_FBK_MTHD_SPRT:
            case MJC_FBK_MTHD_TWD0:
                _u1FbckQnty32[i] = (((UINT16) (i - u1FbckThr)) * 0x33) / (0x40 - u1FbckThr);
                break;
            case MJC_FBK_MTHD_32_TWD0:
                if (i <= u1FbckThr2)
                {
                    _u1FbckQnty32[i] = (((UINT16) (i - u1FbckThr)) * 0x33) / (u1FbckThr2 - u1FbckThr);
                }
                else
                {
                    _u1FbckQnty32[i] = ((UINT16) 128*(i-u1FbckThr2) / (64-u1FbckThr2));
                }
                break;
            case MJC_FBK_MTHD_00224_32:
            case MJC_FBK_MTHD_00224_32_NSFT:
                if (i <= u1FbckThr1_2)
                {
                    _u1FbckQnty32[i] = (((UINT16) (i - u1FbckThr)) * 0x33) / (u1FbckThr1_2 - u1FbckThr);
                }
                else if (i <= u1FbckThr2)
                {
                    _u1FbckQnty32[i] = (((UINT16) (i - u1FbckThr1_2)) * 0x33) / (u1FbckThr2 - u1FbckThr1_2);
                }
                else
                {
                    _u1FbckQnty32[i] = ((UINT16) 128*(i-u1FbckThr2) / (64-u1FbckThr2));
                }
                break;
            }
        }
    }

    LOG(3, "MJC Fallback Qnty Arr 120Hz Updated!\n");
 
    //x_crit_end(csState);
}

static void _vDrvMJCCalFbckQntyArr_240(void)
{
    UINT16 i;

    _rFbkThr240.u1Mthd = IO32ReadFldAlign(MJC_FBCK_37, FBCK_240_MTHD_32);
    _rFbkThr240.u1Thr0 = IO32ReadFldAlign(MJC_FBCK_37, FBCK_240_THR0);
    _rFbkThr240.u1Thr1 = IO32ReadFldAlign(MJC_FBCK_37, FBCK_240_THR1);
    _rFbkThr240.u1Thr2 = IO32ReadFldAlign(MJC_FBCK_37, FBCK_240_THR2);
    _rFbkThr240.u1Thr1_2 = IO32ReadFldAlign(MJC_FBCK_37, FBCK_240_THR1_2);

    for (i = 0; i < 65; i++)
    {
        if (i <= _rFbkThr240.u1Thr0)
        {
            // HF toward 120Hz
            _u1FbckQnty32[i] = (UINT8)(((UINT16) i * 0x19) / _rFbkThr240.u1Thr0);
            _u1FbckQnty22[i] = (UINT8)(((UINT16) i * 0x20) / _rFbkThr240.u1Thr0);
        }
        else if (i <= _rFbkThr240.u1Thr1)
        {
            // toward 55 rpt or 60Hz
            _u1FbckQnty32[i] = (UINT8)(((UINT16) (i-_rFbkThr240.u1Thr0) * 0x33) / (_rFbkThr240.u1Thr1 - _rFbkThr240.u1Thr0));
            _u1FbckQnty22[i] = (UINT8)(((UINT16) (i-_rFbkThr240.u1Thr0) * 0x40) / (_rFbkThr240.u1Thr1 - _rFbkThr240.u1Thr0));
        }
        else
        {
            _u1FbckQnty22[i] = (((UINT16) (0x40 - i)) * 0x80) / (0x40 - _rFbkThr240.u1Thr1);
            switch (_rFbkThr240.u1Mthd)
            {
            default:
            case MJC_240_FBK_MTHD_SPRT:
            case MJC_240_FBK_MTHD_DEMO:
                _u1FbckQnty32[i] = (((UINT16) (i - _rFbkThr240.u1Thr1)) * 0x33) / (0x40 - _rFbkThr240.u1Thr1);
                break;
            case MJC_240_FBK_MTHD_55_TWD0:
                _u1FbckQnty32[i] = _u1FbckQnty22[i];
                break;
            }
        }
    }

    LOG(3, "MJC Fallback Qnty Arr 240Hz Updated!\n");
}


static void _vDrvMJCLoadStageCfg(void)
{
    CRIT_STATE_T csState;
    UINT32 u1Idx = 0;

    csState = x_crit_start();
    
    for (u1Idx = 0; u1Idx < 5; u1Idx ++)
    {
        _rStage[u1Idx].u2ThHi = IO32ReadFldAlign(MJC_FBCK_21 + 4*u1Idx, VAL_TH_HI_0);
        _rStage[u1Idx].u2ThLo = IO32ReadFldAlign(MJC_FBCK_21 + 4*u1Idx, VAL_TH_LO_0);
        _rStage[u1Idx].u2CntThHi = IO32ReadFldAlign(MJC_FBCK_16 + 4*u1Idx, CNT_TH_HI_0);
        _rStage[u1Idx].u2CntThLo = IO32ReadFldAlign(MJC_FBCK_16 + 4*u1Idx, CNT_TH_LO_0);
    }
    _rStage[0].u1Delta = IO32ReadFldAlign(MJC_FBCK_15, CNT_DELTA_0);
    _rStage[0].u1TargetLvl = IO32ReadFldAlign(MJC_FBCK_15, TGT_LVL_0);
    _rStage[1].u1Delta = IO32ReadFldAlign(MJC_FBCK_15, CNT_DELTA_1);
    _rStage[1].u1TargetLvl = IO32ReadFldAlign(MJC_FBCK_14, TGT_LVL_1);
    _rStage[2].u1Delta = IO32ReadFldAlign(MJC_FBCK_15, CNT_DELTA_2);
    _rStage[2].u1TargetLvl = IO32ReadFldAlign(MJC_FBCK_14, TGT_LVL_2);
    _rStage[3].u1Delta = IO32ReadFldAlign(MJC_FBCK_15, CNT_DELTA_3);
    _rStage[3].u1TargetLvl = IO32ReadFldAlign(MJC_FBCK_14, TGT_LVL_3);
    _rStage[4].u1Delta = IO32ReadFldAlign(MJC_FBCK_15, CNT_DELTA_4);
    _rStage[4].u1TargetLvl = IO32ReadFldAlign(MJC_FBCK_14, TGT_LVL_4);

    _rGear.u2CountMaxTh = IO32ReadFldAlign(MJC_FBCK_26, CNT_TH_MAX);

    x_crit_end(csState);
}

static void _vDrvMJCReadWriteTable(void)
{
    // x axis write    
    if (IO32ReadFldAlign(MJC_FBCK_01, CUST_QT_RELOAD))
    {
        UINT32 i = 0;
        CRIT_STATE_T csState = x_crit_start();
        UINT8 u1TblIdx = IO32ReadFldAlign(MJC_FBCK_01, CUST_QT_TBL_IDX);
        for (i = 0; i < 8; i++)
        {
            UINT32 u4Val = u4IO32Read4B(MJC_FBCK_02 + i*4);
            arMJCFbQuantTbl[u1TblIdx][(i*2)+0x0] = R_Fld(u4Val, CUST_QT_LVL_0);
            arMJCFbQuantTbl[u1TblIdx][(i*2)+0x1] = R_Fld(u4Val, CUST_QT_LVL_1);
        }
        x_crit_end(csState);
        vIO32WriteFldAlign(MJC_FBCK_01, 0, CUST_QT_RELOAD);
    }

    // x axis read
    if (IO32ReadFldAlign(MJC_FBCK_01, CUST_QT_READ))
    {
        UINT32 i = 0;
        CRIT_STATE_T csState = x_crit_start();
        UINT8 u1TblIdx = IO32ReadFldAlign(MJC_FBCK_01, CUST_QT_TBL_IDX);
        for (i = 0; i < 8; i++)
        {
            vIO32Write4B(MJC_FBCK_02 + i*4,
                (((UINT32)arMJCFbQuantTbl[u1TblIdx][(i*2)+0x1]) << 16) | arMJCFbQuantTbl[u1TblIdx][(i*2)+0x0]);
        }
        x_crit_end(csState);
        vIO32WriteFldAlign(MJC_FBCK_01, 0, CUST_QT_READ);
    }

    // y axis write
    if (IO32ReadFldAlign(MJC_FBCK_01, CUST_TGT_MAP_WRITE))
    {
        UINT32 i = 0;
        CRIT_STATE_T csState = x_crit_start();
        UINT8 u1TblIdx = IO32ReadFldAlign(MJC_FBCK_01, CUST_TGT_TBL_IDX);
        for (i = 0; i < 4; i++)
        {
            UINT32 u4Val = u4IO32Read4B(MJC_FBCK_32 + i*4);
            arMJCQuantMap[u1TblIdx][i*4 + 0x0] = R_Fld(u4Val, CUST_TGT_LVL_0);
            arMJCQuantMap[u1TblIdx][i*4 + 0x1] = R_Fld(u4Val, CUST_TGT_LVL_1);
            arMJCQuantMap[u1TblIdx][i*4 + 0x2] = R_Fld(u4Val, CUST_TGT_LVL_2);
            arMJCQuantMap[u1TblIdx][i*4 + 0x3] = R_Fld(u4Val, CUST_TGT_LVL_3);
        }
        x_crit_end(csState);
        vIO32WriteFldAlign(MJC_FBCK_01, 0, CUST_TGT_MAP_WRITE);
    }

    // y axis read
    if (IO32ReadFldAlign(MJC_FBCK_01, CUST_TGT_MAP_READ))
    {
        UINT32 i = 0;
        CRIT_STATE_T csState = x_crit_start();
        UINT8 u1TblIdx = IO32ReadFldAlign(MJC_FBCK_01, CUST_TGT_TBL_IDX);
        for (i = 0; i < 4; i++)
        {
            vIO32Write4B(MJC_FBCK_32 + i*4, 
                (arMJCQuantMap[u1TblIdx][i*4 + 0x3] << 24)|
                (arMJCQuantMap[u1TblIdx][i*4 + 0x2] << 16)|
                (arMJCQuantMap[u1TblIdx][i*4 + 0x1] << 8)|
                (arMJCQuantMap[u1TblIdx][i*4 + 0x0]));
        }
        x_crit_end(csState);
        vIO32WriteFldAlign(MJC_FBCK_01, 0, CUST_TGT_MAP_READ);
    }

    // Error feedback cfg write
    if (IO32ReadFldAlign(MJC_FBCK_36, ERFB_TRIG_WRITE))
    {
        CRIT_STATE_T csState = x_crit_start();
        arMJCErfb.u1IncOfst = IO32ReadFldAlign(MJC_FBCK_36, ERFB_INC_OFST);
        arMJCErfb.i2IncTh = IO32ReadFldAlign(MJC_FBCK_36, ERFB_INC_TH);
        arMJCErfb.i2DecTh = - ((INT16) IO32ReadFldAlign(MJC_FBCK_36, ERFB_DEC_TH));
        x_crit_end(csState);
        vIO32WriteFldAlign(MJC_FBCK_36, 0, ERFB_TRIG_WRITE);
    }

    // Error feedback cfg read
    if (IO32ReadFldAlign(MJC_FBCK_36, ERFB_TRIG_READ))
    {
        CRIT_STATE_T csState = x_crit_start();
        vIO32WriteFldAlign(MJC_FBCK_36, arMJCErfb.u1IncOfst, ERFB_INC_OFST);
        vIO32WriteFldAlign(MJC_FBCK_36, arMJCErfb.i2IncTh, ERFB_INC_TH);
        vIO32WriteFldAlign(MJC_FBCK_36, ((UINT16)(-arMJCErfb.i2DecTh)), ERFB_DEC_TH);
        x_crit_end(csState);
        vIO32WriteFldAlign(MJC_FBCK_36, 0, ERFB_TRIG_READ);
    }
}

BOOL fgDrvMJCSetFbckQTable(UINT8 u1TblIdx, const UINT16 u2Array[MJC_FB_QUANT_TBL_NUM])
{
    if (u1TblIdx >= MJC_FB_NUM)
        return SV_FALSE;

    x_memcpy(&arMJCFbQuantTbl[u1TblIdx][0], u2Array, sizeof(UINT16)*MJC_FB_QUANT_TBL_NUM);

    return SV_TRUE;
}

BOOL fgDrvMJCSetFbckQMap(UINT8 u1TblIdx, const UINT8 u1MapArray[MJC_FB_QUANT_TBL_NUM])
{
    if (u1TblIdx >= 2)
        return SV_FALSE;

    x_memcpy(&arMJCQuantMap[u1TblIdx][0], u1MapArray, sizeof(UINT8)*MJC_FB_QUANT_TBL_NUM);

    return SV_TRUE;
}

void vDrvInitFbck(void)
{
    arMJCErfb.i2DiffAccm = 0;
    arMJCErfb.i2IncTh = 8;
    arMJCErfb.i2DecTh = -8;
    arMJCErfb.u1SysSta = 0;
    arMJCErfb.u1IncOfst = 1; //(arMJCPrm.arFRCConfig.u1Is120HzPanel)? 1 : 2;
    arMJCErfb.u1StaMax = MAX_FBCK_LVL;

    vDrvInitAvgFilter();
    
    arMJCPrm.u1EffectParam = 0x00;

    vIO32Write4B(MJC_FBCK_00, 0x20002A00);
    vIO32Write4B(MJC_FBCK_01, 0x25C0EF40);
    
    vIO32Write4B(MJC_FBCK_11, 0x61084210);
    vIO32Write4B(MJC_FBCK_12, 0x00084210);
    vIO32Write4B(MJC_FBCK_13, 0xBA11810B);
    
    vIO32Write4B(MJC_FBCK_14, 0x40201810);
    vIO32Write4B(MJC_FBCK_15, 0x8CA88000);
    
    vIO32Write4B(MJC_FBCK_16, 0x0064001E);
    vIO32Write4B(MJC_FBCK_17, 0x00C8003C);
    vIO32Write4B(MJC_FBCK_18, 0x012C00A0);
    vIO32Write4B(MJC_FBCK_19, 0x01900104);
    vIO32Write4B(MJC_FBCK_20, 0x01F40168);
    
    vIO32Write4B(MJC_FBCK_21, 0x05DC00C8);
    vIO32Write4B(MJC_FBCK_22, 0x07D00320);
    vIO32Write4B(MJC_FBCK_23, 0x0FA00578);
    vIO32Write4B(MJC_FBCK_24, 0x17700A8C);
    vIO32Write4B(MJC_FBCK_25, 0x1F4010FE);
    vIO32Write4B(MJC_FBCK_26, 0x01F40080);

    vIO32Write4B(MJC_FBCK_36, 0x18080064);
    vIO32Write4B(MJC_FBCK_37, 0x0418C201);
    vIO32Write4B(MJC_FBCK_38, 0x2584003C);
    vIO32Write4B(MJC_FBCK_39, 0x0000171F);
}

void vDrvSetFallbackRemapFunctor(PFN_FBCK_REMAP pfn)
{
    _pfnRemap = pfn;
}

static UINT8 _u1DrvGetMappingLvl(UINT16 u2AvgVal, UINT16 u2QuantTbl[MJC_FB_QUANT_TBL_NUM], UINT8 u1QuantMap[MJC_FB_QUANT_TBL_NUM])
{
    UINT8   u1LowIndex = 0;
    UINT8   u1CurIndex;
    UINT8   u1HighIndex = MJC_FB_QUANT_TBL_NUM - 1;

    u2AvgVal = (u2AvgVal >= u2QuantTbl[MJC_FB_QUANT_TBL_NUM - 1])? u2QuantTbl[MJC_FB_QUANT_TBL_NUM - 1] : u2AvgVal;

    if (u2AvgVal >= u2QuantTbl[MJC_FB_QUANT_TBL_NUM - 1])
    {
        return u1QuantMap[MJC_FB_QUANT_TBL_NUM - 1];
    }

    do
    {
        u1CurIndex = (u1LowIndex + u1HighIndex) >> 1;
        if (u2AvgVal < u2QuantTbl[u1CurIndex])
        {
            u1HighIndex = u1CurIndex;
        }
        else
        {
            u1LowIndex = u1CurIndex;
        }
    } while (u1HighIndex != (u1LowIndex + 1));

    return u1QuantMap[u1LowIndex];
}

void vDrvInitErrorFeedBack(void)
{
    arMJCErfb.i2DiffAccm = 0;
    arMJCErfb.u1SysSta = 0;
}

UINT8 u1DrvErrorFeedBack(UINT8 u1Value)
{
    UINT32 u4Tmp = u4IO32Read4B(MJC_FBCK_38);
    
    INT16 i2DecTh = arMJCErfb.i2DecTh;
    
    if (R_Fld(u4Tmp, ERFB_DEC_TH_CURVE_EN))
    {
        UINT32 u4DecTh = ((UINT32) R_Fld(u4Tmp, ERFB_DEC_TH_SLOPE)*arMJCErfb.u1SysSta) >> 4;
        u4DecTh = u4DecTh + R_Fld(u4Tmp, ERFB_DEC_TH_MIN);
        u4DecTh = MIN(u4DecTh, R_Fld(u4Tmp, ERFB_DEC_TH_MAX));
        i2DecTh = - (INT16)u4DecTh;
    }

    arMJCErfb.i2DiffAccm += ((INT16)u1Value - (INT16)arMJCErfb.u1SysSta);

    if ((arMJCErfb.i2DiffAccm > arMJCErfb.i2IncTh) &&
        (arMJCErfb.u1SysSta < arMJCErfb.u1StaMax))
    {
        if ((arMJCErfb.u1StaMax - arMJCErfb.u1SysSta) >= arMJCErfb.u1IncOfst)
        {
            arMJCErfb.u1SysSta += arMJCErfb.u1IncOfst;
        }
        else
        {
            arMJCErfb.u1SysSta = arMJCErfb.u1StaMax;
        }

        arMJCErfb.i2DiffAccm = 0;
    }
    else if ((arMJCErfb.i2DiffAccm < i2DecTh) && arMJCErfb.u1SysSta > 0)
    {
        if (arMJCErfb.u1SysSta >= arMJCErfb.u1IncOfst)
        {
            arMJCErfb.u1SysSta -= arMJCErfb.u1IncOfst;
        }
        else
        {
            arMJCErfb.u1SysSta = 0;
        }

        arMJCErfb.i2DiffAccm = 0;
    }

    return arMJCErfb.u1SysSta;
}

UINT32 u4DrvBiltIIR(UINT32 u4X, UINT32 u4Min)
{
    static UINT32 _u4Y = 0;
    UINT32 u4A;
    UINT32 u4Diff, u4Tmp, u4Step;

    // A
    if (u4X >= _u4Y)
    {
        u4Diff = u4X - _u4Y;
        u4Tmp = _u4Y;
    }
    else
    {
        u4Diff = _u4Y - u4X;
        u4Tmp = u4X;
    }

    u4Step = MAX(u4Min, u4Tmp);

    if (u4Diff >= (8*u4Step))
        u4A = 8;
    else if (u4Diff >= (7*u4Step))
        u4A = 7;
    else if (u4Diff >= (6*u4Step))
        u4A = 6;
    else if (u4Diff >= (5*u4Step))
        u4A = 5;
    else if (u4Diff >= (4*u4Step))
        u4A = 4;
    else if (u4Diff >= (3*u4Step))
        u4A = 3;
    else if (u4Diff >= (2*u4Step))
        u4A = 2;
    else if (u4Diff >= u4Step)
        u4A = 1;
    else
        u4A = 0;     

    u4A = 8+u4A;
    
    // IIR
    _u4Y = ((u4A*_u4Y + (16-u4A)*u4X) + 8) >> 4;

    return _u4Y;
}


#define FIFO_LEN 8
UINT32 _rFifoBilt[2][FIFO_LEN] = {{0},{0}};
UINT8 _rFifoIdx[2] = {0};

UINT32 u4DrvBiltFIR(BOOL fgBox, UINT8 id, UINT32 u4X)
{	
    UINT8 i;
    UINT32 u4Max = 0, u4Min = 0xffffffff;
    UINT32 u4Sum = 0;
    UINT32 u4WeightSum = 0, u4WeightMax = 0, u4WeightMin = 0;

	_rFifoBilt[id][_rFifoIdx[id]] = u4X;

	for (i = 0; i < FIFO_LEN; i++)
	{
	    UINT32 u4Weight = 0;
	    UINT32 u4Neb = _rFifoBilt[id][(_rFifoIdx[id]+i) & (FIFO_LEN-1)];
        UINT8 u1DiffIdx =((AbsDiff(u4X, u4Neb) + 128) >> 8);
		u4Weight = fgBox ? 32 : _rGaussW[MIN((UINT8)32, u1DiffIdx)];
		u4Sum += (u4Weight * u4Neb);
		u4WeightSum += u4Weight;

		if (u4Neb > u4Max)
		{
			u4Max = u4Neb;
			u4WeightMax = u4Weight;
		}
		if (u4Neb < u4Min)
		{
			u4Min = u4Neb;
			u4WeightMin = u4Weight;
		}
	}

	u4WeightSum = u4WeightSum - (u4WeightMax + u4WeightMin);
	u4Sum = u4Sum - (u4Max*u4WeightMax + u4Min*u4WeightMin);

	_rFifoIdx[id] = (_rFifoIdx[id] + 1) & (FIFO_LEN-1);

	if (u4WeightSum)
		return (u4Sum + (u4WeightSum >> 1)) / u4WeightSum;
	else
		return 0;
}


UINT32 u4DrvNormByGMV(UINT32 u4X)
{
    UINT8 u1GmvLvl = MAX(_u1AbsGmvYPdct, _u1AbsGmvXPdct);
    u1GmvLvl = (u1GmvLvl + 2) >> 2;
    u1GmvLvl = MIN(u1GmvLvl, 15);
    
    return (((UINT32) _rGmvWtTbl[u1GmvLvl] * u4X + 8) >> 4);
}

EXTERN UINT8 _u1AbsGmvX;
UINT8 u1DrvCalPartialFbckAG(void)
{
    static UINT8 _u1TmpIdx = 1; // reset at 1
    static UINT16 _u2FiltVal = 0;
    UINT16 u2DCVal;
    UINT16 u2CustValue;
    UINT8 u1State;
    BOOL fgScnChg;
    
    u2CustValue = ((UINT32)u2StaCustVal * IO32ReadFldAlign(MJC_FBCK_26, SCALE_FACT) + 128) >> 8;
    //u2CustValue = u4DrvNormByGMV(u2CustValue);
    u2CustValue = MIN(u2CustValue, 8192);

    u2DCVal = u4DrvBiltFIR(1, 0, u2CustValue);

    vIO32WriteFldAlign(MJC_FBCK_27, u2DCVal, STA_NOFILT_VAL);

    fgScnChg = (AbsDiff(u2DCVal, u2CustValue) >= 3000); //fgDrvMJCIsMEScnChg();

    u1State = MIN(_rGear.u1State, 4);

    if ((_u1TmpIdx >= _rStage[u1State].u1SubSample) && (!fgScnChg))
    {       
        switch (IO32ReadFldAlign(MJC_FBCK_15, FILT_MODE))
        {
        default:
        case 0:
            _u2FiltVal = u4DrvBiltFIR(0, 1, u2CustValue);
            break;
        case 1:
            _u2FiltVal = u2DrvAvgFilter(MJC_FB_CUST, u2CustValue);
            break;
        case 2:
            _u2FiltVal = u4DrvBiltIIR(u2CustValue, 400);
            break;
        }

        _u1TmpIdx = 0;
        vIO32WriteFldAlign(MJC_FBCK_27, _u2FiltVal, STA_FILT_VAL);
    }

    if (!fgScnChg)
        _u1TmpIdx = ((UINT16) _u1TmpIdx + 1) & 0xff;

    //static UINT8 _u1PrevAbsGmvX = 0;
    //BOOL fgNoDec = (AbsDiff(_u1PrevAbsGmvX, _u1AbsGmvX) < 5) && (_u1AbsGmvX >= 3);
    //_u1PrevAbsGmvX = _u1AbsGmvX;
    
    if (u2CustValue >= _rStage[u1State].u2ThHi)
    {
		UINT16 u2Diff = u2CustValue - _rStage[u1State].u2ThHi;
		u2Diff = MIN((UINT16)1024, u2Diff);
		u2Diff = u2Diff >> 6;
		u2Diff = MAX((UINT16)MIN(u2Diff, (UINT16)_rStage[u1State].u1Delta), (UINT16)1);
        if (_rGear.u2Count < (_rGear.u2CountMaxTh - u2Diff /*_rStage[_rGear.u1State].u1Delta*/))
            _rGear.u2Count += u2Diff; //_rStage[_rGear.u1State].u1Delta;
        else
            _rGear.u2Count = _rGear.u2CountMaxTh;
    }
    else if ((u2CustValue < _rStage[u1State].u2ThLo) /*&& (!fgNoDec)*/)
    {
		//UINT16 u2Diff = _rStage[_rGear.u1State].u2ThLo - u2CustValue;
		//u2Diff = MIN((UINT16)1024, u2Diff);
		//u2Diff = u2Diff >> 6;
		//u2Diff = MAX(MIN(u2Diff, (UINT16)(_rStage[_rGear.u1State].u1Delta >> 1)), (UINT16)1);
        if (_rGear.u2Count >= _rStage[u1State].u1DeltaDec)
            _rGear.u2Count -= _rStage[u1State].u1DeltaDec;
        else
            _rGear.u2Count = 0;
    }

    if (_rGear.u2Count >= _rStage[u1State].u2CntThHi)
    {
        if (u1State < 4)
        {
            u1State ++;
            _u1TmpIdx = 1;
        }
    }
    else if (_rGear.u2Count < _rStage[u1State].u2CntThLo)
    {
        if (u1State > 0)
        {
            u1State --;
            _u1TmpIdx = 1;
        }
    }

    _rGear.u1State = u1State;
    return _rStage[u1State].u1TargetLvl;
}

UINT8 u1DrvCalPartialFbck(BOOL fgIsFilm)
{
    UINT8   u1FbckLvlFinal = 0x0;

    UINT32  u4MJCFbckCtrl;

    UINT16  u2AvgNrmvsi;
    UINT8   u1FbckLvl1;

    UINT16  u2AvgBdrvsi;
    UINT8   u1FbckLvl2;

    UINT16  u2AvgMvErr;
    UINT8   u1FbckLvl3;

    UINT16  u2AvgBadMotCnt;
    UINT8   u1FbckLvl4;

    UINT16  u2AvgOsdErr;
    UINT8   u1FbckLvl5;

    UINT8   u1FbckLvl6;

    UINT16  u2AvgBmval;
    UINT8   u1FbckLvl7;

    UINT16  u2CustValue;
    UINT8   u1FbckLvl8;
    UINT16  u2Vlarge_TH;
    UINT16  u2Vlarge_WDV_TH;
    UINT8   u1Vlarge_Speed;

    UINT8   u1TgtTblIdx;

    u4MJCFbckCtrl = fgIsFilm ? IO32ReadFldAlign(MJC_FBCK_00, FBCK_CTRL) : IO32ReadFldAlign(MJC_FBCK_00, FBCK_CTRL_VDO);
    
    u1TgtTblIdx = fgIsFilm ? 1 : 0;
    
    u2Vlarge_TH = IO32ReadFldAlign(MJC_FBCK_39, FBCK_VLRG_TH);
    u2Vlarge_WDV_TH = IO32ReadFldAlign(MJC_FBCK_39, VLRG_WDV_TH);
    u1Vlarge_Speed = IO32ReadFldAlign(MJC_FBCK_39, FBCK_VLRG_SPEED);

    
    //----------------------------------------------------------------
    // Meter 1: blk vsi
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_NRMVSI)
    {
        u2AvgNrmvsi = u2DrvAvgFilter(MJC_FB_NRMVSI, u2StaNrmvsi);
        u1FbckLvl1 = _u1DrvGetMappingLvl(u2AvgNrmvsi, arMJCFbQuantTbl[MJC_FB_NRMVSI], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl1);
    }

    //----------------------------------------------------------------
    // Meter 2: bdr vsi
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_BDRVSI)
    {
        u2AvgBdrvsi = u2DrvAvgFilter(MJC_FB_BDRVSI, u2StaBdrvsi);
        u1FbckLvl2 = _u1DrvGetMappingLvl(u2AvgBdrvsi, arMJCFbQuantTbl[MJC_FB_BDRVSI], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl2);
    }

    //----------------------------------------------------------------
    // Meter 3: MV error
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_MVERR)
    {
        u2AvgMvErr = u2DrvAvgFilter(MJC_FB_MVERR, u2MaxMvErr);
        u1FbckLvl3 = _u1DrvGetMappingLvl(u2AvgMvErr, arMJCFbQuantTbl[MJC_FB_MVERR], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl3);
    }

    //----------------------------------------------------------------
    // Meter 4: Bad motion count - OCC_SAD_ERR
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_BADMOTCNT)
    {
        u2AvgBadMotCnt = u2DrvAvgFilter(MJC_FB_BADMOTCNT, u2BadMotCnt);
        u1FbckLvl4 = _u1DrvGetMappingLvl(u2AvgBadMotCnt, arMJCFbQuantTbl[MJC_FB_BADMOTCNT], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl4);
    }

    //----------------------------------------------------------------
    // Meter 5: OSD error
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_OSDERR)
    {
        u2AvgOsdErr = u2DrvAvgFilter(MJC_FB_OSDERR, u2OsdErr);
        u1FbckLvl5 = _u1DrvGetMappingLvl(u2AvgOsdErr, arMJCFbQuantTbl[MJC_FB_OSDERR], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl5);
    }

    //----------------------------------------------------------------
    // Meter 6: Large vertical motion
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_LRGV)
    {
        if (u2MajorCnt0 >= 0x4800)
        {
            u1FbckLvl6 = (u1MajorMvY0 >= u2Vlarge_TH) ? (0x10 + ((u1MajorMvY0 - u2Vlarge_TH) << u1Vlarge_Speed)) : 0;
            u1FbckLvl6= MIN(u1FbckLvl6,64);

            if (u1MajorMvY0 >= u2Vlarge_WDV_TH)
            {
                MJC_WRITE_FLD(MJC_ME_00, 0, MJC_ME_WDV_BLK);
            }
            else
            {
                MJC_WRITE_FLD(MJC_ME_00, 1, MJC_ME_WDV_BLK);
            }
            
        }
        else
        {
            u1FbckLvl6 = 0;
        }

        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl6);
    }

    //----------------------------------------------------------------
    // Meter 7: Burst Error Count
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_BEC)
    {
        u2AvgBmval = u2DrvAvgFilter(MJC_FB_BEC, u2StaMEBec);
        u1FbckLvl7 = _u1DrvGetMappingLvl(u2AvgBmval, arMJCFbQuantTbl[MJC_FB_BEC], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl7);
    }

    //----------------------------------------------------------------
    // Meter 8: User defined
    //----------------------------------------------------------------
    if (u4MJCFbckCtrl & MJC_FB_FLAG_CUST)
    {
        u2CustValue = u2DrvAvgFilter(MJC_FB_CUST, u2StaCustVal);
        u1FbckLvl8  = _u1DrvGetMappingLvl(u2CustValue, arMJCFbQuantTbl[MJC_FB_CUST], arMJCQuantMap[u1TgtTblIdx]);
        u1FbckLvlFinal = MAX(u1FbckLvlFinal, u1FbckLvl8);
    }

    return u1FbckLvlFinal;
}

void vDrvInitAvgFilter(void)
{
    UINT8 i;
    for (i = 0; i < MJC_FB_NUM; i ++)
        x_memset((void *) &arMJCAvg[i], 0, sizeof(MJC_AVG_T));
}

UINT16 u2DrvAvgFilter(UINT8 u1Index, UINT16 u2AddOnData)
{
    if (arMJCAvg[u1Index].u4Sum >= ((UINT32)arMJCAvg[u1Index].u2Fifo[arMJCAvg[u1Index].u1Cursor]))
    {
        arMJCAvg[u1Index].u4Sum -= (UINT32) arMJCAvg[u1Index].u2Fifo[arMJCAvg[u1Index].u1Cursor];
        arMJCAvg[u1Index].u4Sum += (UINT32) u2AddOnData;
    }

    arMJCAvg[u1Index].u2Fifo[arMJCAvg[u1Index].u1Cursor] = u2AddOnData;
    arMJCAvg[u1Index].u1Cursor = ((arMJCAvg[u1Index].u1Cursor + 1) & MJC_AVG_FIFO_MOD_MSK);

    return (arMJCAvg[u1Index].u4Sum >> MJC_AVG_FIFO_WD);
}

static void _vDrvMJCCalc22FbkPosForceVdo(UINT8 u1Level)
{
    _r22FbkPos.u1Pos[0] = 0;

    _r22FbkPos.u1Pos[1] = 0;
    _r22FbkPos.u1Pos[2] = 0x80 - (u1Level<<1);
    _r22FbkPos.u1Pos[3] = 0xFF;
}

static void _vDrvMJCCalc22FbkPosNoBlur(UINT8 u1Level)
{
    _r22FbkPos.u1Pos[0] = 0;

    _r22FbkPos.u1Pos[1] = 0x40 - u1Level;
    _r22FbkPos.u1Pos[2] = 0x80 - (u1Level<<1);
    _r22FbkPos.u1Pos[3] = 0xC0 + (u1Level >= 0x3f ? 0x3f : u1Level);
}

static void _vDrvMJCCalc22FbkPos(UINT8 u1Level)
{
    UINT8 u1FbckQuantity;
	if(64 < u1Level)
	{
        LOG(3, "_vDrvMJCCalc22FbkPos u1Level: %d \n", u1Level); 
		return;
	}
    u1FbckQuantity = _u1FbckQnty22[u1Level];

    _r22FbkPos.u1Pos[0] = 0;    

    if (u1Level <= _rFbkThr120.u1Thr1)
    {
        _r22FbkPos.u1Pos[1] = 0x40 - u1FbckQuantity;
        _r22FbkPos.u1Pos[2] = 0x80;
        _r22FbkPos.u1Pos[3] = 0xC0 - u1FbckQuantity;
    }
    else
    {
        switch (_rFbkThr120.u1Mthd22)
        {
        default:
        case MJC_22_FBK_MTHD_TWD0:
            _r22FbkPos.u1Pos[1] = 0;
            _r22FbkPos.u1Pos[2] = 0x80 - u1FbckQuantity;
            _r22FbkPos.u1Pos[3] = 0x80 - u1FbckQuantity;
            break;
        case MJC_22_FBK_MTHD_31_TWD0:
            if (u1Level <= _rFbkThr120.u1Thr2)
            {
                _r22FbkPos.u1Pos[1] = 0;
                _r22FbkPos.u1Pos[2] = 0x80 - u1FbckQuantity;
                _r22FbkPos.u1Pos[3] = 0x80;
            }
            else
            {
                _r22FbkPos.u1Pos[1] = 0;
                _r22FbkPos.u1Pos[2] = 0;
                _r22FbkPos.u1Pos[3] = 0x80 - u1FbckQuantity;
            }
            break;
        }
    }
}

static void _vDrvMJCCalc55FbkPos(UINT8 u1Level)
{
    UINT8 u1FbckQuantity;

    // 5:5 mode
    u1FbckQuantity = (UINT8)(((UINT16) u1Level * 0x33 + 32) >> 6);
    _r32FbkPos.u1Pos15 = 0x33 - (u1FbckQuantity);
    _r32FbkPos.u1Pos25 = 0x66 - (u1FbckQuantity << 1);
    _r32FbkPos.u1Pos35 = 0x99 + (u1FbckQuantity << 1);
    _r32FbkPos.u1Pos45 = 0xcc + (u1FbckQuantity);
}

static void _vDrvMJCCalc64FbkPosForceVdo(UINT8 u1Level)
{
    UINT8 u1TmpLvl = MIN(((UINT32)0x80 + (u1Level<<1)), 255);
    _r64FbkPos.u1Pos0B5 = 0;
    _r64FbkPos.u1Pos2A5 = 0;
    _r64FbkPos.u1Pos2B5 = 0;
    _r64FbkPos.u1Pos4A5 = u1TmpLvl;
    _r64FbkPos.u1Pos4B5 = 0xFF;
    
    _r64FbkPos.u1Pos1A5 = 0; //_r64FbkPos.u1Pos0B5;
    _r64FbkPos.u1Pos1B5 = 0; //_r64FbkPos.u1Pos2A5;
    _r64FbkPos.u1Pos3A5 = 0; //_r64FbkPos.u1Pos2B5;
    _r64FbkPos.u1Pos3B5 = 0; //_r64FbkPos.u1Pos4A5;
}

static void _vDrvMJCCalc64FbkPosSprtNoBlur(UINT8 u1Level)
{
    UINT8 u1TmpLvl;
    // 24->60, 24->120
    u1TmpLvl = (UINT8)(((UINT16) 51*u1Level + 32) >> 6);
    _r64FbkPos.u1Pos0B5 = (0x33 - u1TmpLvl);
    _r64FbkPos.u1Pos2A5 = (0x66 - (u1TmpLvl<<1));
    _r64FbkPos.u1Pos2B5 = (0x99 + (u1TmpLvl<<1));
    _r64FbkPos.u1Pos4A5 = (0xcc + u1TmpLvl);
    
    _r64FbkPos.u1Pos4B5 = 0xFF;
    _r64FbkPos.u1Pos1A5 = _r64FbkPos.u1Pos0B5;
    _r64FbkPos.u1Pos1B5 = _r64FbkPos.u1Pos2A5;
    _r64FbkPos.u1Pos3A5 = _r64FbkPos.u1Pos2B5;
    _r64FbkPos.u1Pos3B5 = _r64FbkPos.u1Pos4A5;
}

static void _vDrvMJCCalc64FbkPosNoBlur(UINT8 u1Level)
{
    UINT8 u1TmpLvl;
    
    if (u1Level < 32)
    {
        u1TmpLvl = (UINT8)(((UINT32) 102*u1Level + 32) >> 6);
        _r64FbkPos.u1Pos0B5 = 0x33 - u1TmpLvl;
        _r64FbkPos.u1Pos2A5 = 0x66 + (u1TmpLvl>>1);
        _r64FbkPos.u1Pos2B5 = 0x99 - (u1TmpLvl>>1);
        _r64FbkPos.u1Pos4A5 = 0xCC + u1TmpLvl;
    }
    else
    {
        u1TmpLvl = 0x80 - ((u1Level - 32) << 2);
        _r64FbkPos.u1Pos0B5 = 0;
        _r64FbkPos.u1Pos2A5 = u1TmpLvl;
        _r64FbkPos.u1Pos2B5 = u1TmpLvl;
        _r64FbkPos.u1Pos4A5 = 0xFF;        
    }
    
    _r64FbkPos.u1Pos4B5 = 0xFF;
    _r64FbkPos.u1Pos1A5 = _r64FbkPos.u1Pos0B5;
    _r64FbkPos.u1Pos1B5 = _r64FbkPos.u1Pos2A5;
    _r64FbkPos.u1Pos3A5 = _r64FbkPos.u1Pos2B5;
    _r64FbkPos.u1Pos3B5 = _r64FbkPos.u1Pos4A5;
}

static void _vDrvMJCCalc64FbkPos(UINT8 u1Level)
{
    UINT8 u1FbckQuantity;
    if(64 < u1Level)
	{
        LOG(3, "_vDrvMJCCalc64FbkPos u1Level: %d \n", u1Level); 
		return;
	}
    u1FbckQuantity = _u1FbckQnty32[u1Level];

    if (u1Level <= _rFbkThr120.u1Thr1)
    {
        _r64FbkPos.u1Pos0B5 = 0x33 - u1FbckQuantity;
        _r64FbkPos.u1Pos2A5 = 0x66;
        _r64FbkPos.u1Pos2B5 = 0x99 - u1FbckQuantity;
        _r64FbkPos.u1Pos4A5 = 0xCC;
        _r64FbkPos.u1Pos4B5 = 0xFF - u1FbckQuantity;
        _r64FbkPos.u1Pos1A5 = 0x33;
        _r64FbkPos.u1Pos1B5 = 0x66 - u1FbckQuantity;
        _r64FbkPos.u1Pos3A5 = 0x99;
        _r64FbkPos.u1Pos3B5 = 0xCC - u1FbckQuantity;
    }
    else
    {
        switch (_rFbkThr120.u1Mthd)
        {
        default:
        case MJC_FBK_MTHD_SPRT:
            // Total fallback is 6:4 repeat
            _r64FbkPos.u1Pos0B5 = 0x00;
            _r64FbkPos.u1Pos2A5 = 0x66 - (u1FbckQuantity << 1);
            _r64FbkPos.u1Pos2B5 = 0x66 - (u1FbckQuantity << 1);
            _r64FbkPos.u1Pos4A5 = 0xCC + (u1FbckQuantity);
            _r64FbkPos.u1Pos4B5 = 0xCC + (u1FbckQuantity);
            _r64FbkPos.u1Pos1A5 = 0x33 - (u1FbckQuantity);
            _r64FbkPos.u1Pos1B5 = 0x33 - (u1FbckQuantity);
            _r64FbkPos.u1Pos3A5 = IO32ReadFldAlign(MJC_FBCK_01, FBCK_55_MODE) ?
                (0x99 - ((u1FbckQuantity << 1) + u1FbckQuantity)) : (0x99 + (u1FbckQuantity << 1));
            _r64FbkPos.u1Pos3B5 = 0x99 + (u1FbckQuantity << 1);
            break;
        case MJC_FBK_MTHD_TWD0:
            // Total fallback is 6:4 repeat
            _r64FbkPos.u1Pos0B5 = 0x00;
            _r64FbkPos.u1Pos2A5 = 0x66 - (u1FbckQuantity << 1);
            _r64FbkPos.u1Pos2B5 = 0x66 - (u1FbckQuantity << 1);
            _r64FbkPos.u1Pos4A5 = 0xCC - (u1FbckQuantity << 2);
            _r64FbkPos.u1Pos4B5 = IO32ReadFldAlign(MJC_FBCK_01, FBCK_55_MODE) ?
                (0xCC + u1FbckQuantity) : (0xCC - (u1FbckQuantity << 2));
            _r64FbkPos.u1Pos1A5 = 0x33 - (u1FbckQuantity);
            _r64FbkPos.u1Pos1B5 = 0x33 - (u1FbckQuantity);
            _r64FbkPos.u1Pos3A5 = 0x99 - ((u1FbckQuantity << 1) + u1FbckQuantity);
            _r64FbkPos.u1Pos3B5 = 0x99 - ((u1FbckQuantity << 1) + u1FbckQuantity);
            break;
        case MJC_FBK_MTHD_32_TWD0:
            // Total fallback is 5:5 repeat
            if (u1Level <= _rFbkThr120.u1Thr2)
            {
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x66 - (u1FbckQuantity << 1);
                _r64FbkPos.u1Pos2B5 = 0x66 + (u1FbckQuantity >> 1);
                //_r64FbkPos.u1Pos4A5 = 0xCC - (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos4A5 = 0xCC - (u1FbckQuantity + (u1FbckQuantity >> 1));
                _r64FbkPos.u1Pos4B5 = 0xCC + (u1FbckQuantity);
                _r64FbkPos.u1Pos1A5 = 0x33 - (u1FbckQuantity);
                _r64FbkPos.u1Pos1B5 = 0x33 - (u1FbckQuantity);
                _r64FbkPos.u1Pos3A5 = 0x99 - (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos3B5 = 0x99 - (u1FbckQuantity >> 1);
            }
            else
            {
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x00;
                _r64FbkPos.u1Pos2B5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos4A5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos4B5 = 0xFF;
                _r64FbkPos.u1Pos1A5 = 0x00;
                _r64FbkPos.u1Pos1B5 = 0x00;
                _r64FbkPos.u1Pos3A5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos3B5 = 0x80 - u1FbckQuantity;
            }
            break;
        case MJC_FBK_MTHD_00224_32:
            // Total fallback is 5:5 repeat
            if (u1Level <= _rFbkThr120.u1Thr1_2)
            {   // toward 00224
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x66;
                _r64FbkPos.u1Pos2B5 = 0x66;
                _r64FbkPos.u1Pos4A5 = 0xCC;
                _r64FbkPos.u1Pos4B5 = 0xCC + (u1FbckQuantity);
                _r64FbkPos.u1Pos1A5 = 0x33 - (u1FbckQuantity);
                _r64FbkPos.u1Pos1B5 = 0x33 + (u1FbckQuantity);
                _r64FbkPos.u1Pos3A5 = 0x99 - (u1FbckQuantity);
                _r64FbkPos.u1Pos3B5 = 0x99 + (u1FbckQuantity);
            }
            else if (u1Level <= _rFbkThr120.u1Thr2)
            {   // toward 32
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x66 + (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos2B5 = 0x66 + (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos4A5 = 0xCC + (u1FbckQuantity);
                _r64FbkPos.u1Pos4B5 = 0xFF;
                _r64FbkPos.u1Pos1A5 = 0x00;
                _r64FbkPos.u1Pos1B5 = 0x66 + (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos3A5 = 0x66 + (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos3B5 = 0xCC + (u1FbckQuantity);
            }
            else
            {   // toward 55
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos2B5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos4A5 = 0xFF;
                _r64FbkPos.u1Pos4B5 = 0xFF;
                _r64FbkPos.u1Pos1A5 = 0x00;
                _r64FbkPos.u1Pos1B5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos3A5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos3B5 = 0xFF;
            }
            break;
        case MJC_FBK_MTHD_00224_32_NSFT:
            // Total fallback is 5:5 repeat
            if (u1Level <= _rFbkThr120.u1Thr1_2)
            {   // toward 00224
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x66;
                _r64FbkPos.u1Pos2B5 = 0x66;
                _r64FbkPos.u1Pos4A5 = 0xCC;
                _r64FbkPos.u1Pos4B5 = 0xCC + (u1FbckQuantity);
                _r64FbkPos.u1Pos1A5 = 0x33 - (u1FbckQuantity);
                _r64FbkPos.u1Pos1B5 = 0x33 + (u1FbckQuantity);
                _r64FbkPos.u1Pos3A5 = 0x99 - (u1FbckQuantity);
                _r64FbkPos.u1Pos3B5 = 0x99 + (u1FbckQuantity);
            }
            else if (u1Level <= _rFbkThr120.u1Thr2)
            {
                // toward 32
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x66 - (u1FbckQuantity << 1);
                _r64FbkPos.u1Pos2B5 = 0x66 + (u1FbckQuantity >> 1);
                _r64FbkPos.u1Pos4A5 = 0xCC - (u1FbckQuantity + (u1FbckQuantity >> 1));
                _r64FbkPos.u1Pos4B5 = 0xFF;
                _r64FbkPos.u1Pos1A5 = 0x00;
                _r64FbkPos.u1Pos1B5 = _r64FbkPos.u1Pos2A5;
                _r64FbkPos.u1Pos3A5 = _r64FbkPos.u1Pos2B5;
                _r64FbkPos.u1Pos3B5 = _r64FbkPos.u1Pos4A5;
            } 
            else
            {
                // toward 55
                _r64FbkPos.u1Pos0B5 = 0x00;
                _r64FbkPos.u1Pos2A5 = 0x00;
                _r64FbkPos.u1Pos2B5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos4A5 = 0x80 - u1FbckQuantity;
                _r64FbkPos.u1Pos4B5 = 0xFF;
                _r64FbkPos.u1Pos1A5 = 0x00;
                _r64FbkPos.u1Pos1B5 = 0x00;
                _r64FbkPos.u1Pos3A5 = _r64FbkPos.u1Pos2B5;
                _r64FbkPos.u1Pos3B5 = _r64FbkPos.u1Pos4A5;
            }
            break;
        }
    }
}

static void _vDrvMJCSetParFbck22(UINT8 u1Level)
{
    switch (_rFbkThr120.u1Mthd22)
    {
    case MJC_22_FBK_MTHD_FORCE_VDO:
        _vDrvMJCCalc22FbkPosForceVdo(u1Level);
        break;
    case MJC_22_FBK_MTHD_TWD0_2:
        _vDrvMJCCalc22FbkPosNoBlur(u1Level);
        break;
    default:
        _vDrvMJCCalc22FbkPos(u1Level);
        break;
    }

    MJC_WRITE_FLD_MULTI(MJC_MC_21,
        P_Fld(_r22FbkPos.u1Pos[1], MJC_INTP_POS_1_8)|
        P_Fld(_r22FbkPos.u1Pos[2], MJC_INTP_POS_2_8)|
        P_Fld(_r22FbkPos.u1Pos[3], MJC_INTP_POS_3_8));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[1]), MJC_PROG_RDCTYPE_1_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[2]), MJC_PROG_RDCTYPE_2_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[3]), MJC_PROG_RDCTYPE_3_8));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_1_8)|P_Fld(0, MJC_PROG_RDCTYPE_2_8)|P_Fld(0, MJC_PROG_RDCTYPE_3_8));
    }
}

static void _vDrvMJCSetParFbck22_60(UINT8 u1Level)
{
    MJC_WRITE_FLD(MJC_MC_21, (0x80 - (u1Level*2)), MJC_INTP_POS_1_8);
    // rdctype
    MJC_WRITE_FLD(MJC_FM_08,
        (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL?INTP_RDCTYPE(_r22FbkPos.u1Pos[1]):0), MJC_PROG_RDCTYPE_1_8);
}

static void _vDrvMJCSetParFbck32_55(UINT8 u1Level)
{
    _vDrvMJCCalc55FbkPos(u1Level);

    MJC_WRITE_FLD_MULTI(MJC_MC_23,
        P_Fld(_r32FbkPos.u1Pos15, MJC_INTP_POS_1_10) |
        P_Fld(_r32FbkPos.u1Pos25, MJC_INTP_POS_2_10) |
        P_Fld(_r32FbkPos.u1Pos35, MJC_INTP_POS_3_10));
    MJC_WRITE_FLD(MJC_MC_24, _r32FbkPos.u1Pos45, MJC_INTP_POS_4_10);

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(INTP_RDCTYPE(_r32FbkPos.u1Pos15), MJC_PROG_RDCTYPE_1_10)|
            P_Fld(INTP_RDCTYPE(_r32FbkPos.u1Pos25), MJC_PROG_RDCTYPE_2_10)|
            P_Fld(INTP_RDCTYPE(_r32FbkPos.u1Pos35), MJC_PROG_RDCTYPE_3_10));
        MJC_WRITE_FLD(MJC_FM_13, INTP_RDCTYPE(_r32FbkPos.u1Pos45), MJC_PROG_RDCTYPE_4_10);
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_1_10)|P_Fld(0, MJC_PROG_RDCTYPE_2_10)|P_Fld(0, MJC_PROG_RDCTYPE_3_10));
        MJC_WRITE_FLD(MJC_FM_13, 0, MJC_PROG_RDCTYPE_4_10);
    }
}

static void _vDrvMJCSetParFbck32(UINT8 u1Level)
{
    switch (_rFbkThr120.u1Mthd)
    {
    case MJC_FBK_MTHD_32_FORCE_VDO:
        _vDrvMJCCalc64FbkPosForceVdo(u1Level);
        break;
    case MJC_FBK_MTHD_32_TWD0_2:
        _vDrvMJCCalc64FbkPosNoBlur(u1Level);
        break;
    case MJC_FBK_MTHD_SPRT_2:
        _vDrvMJCCalc64FbkPosSprtNoBlur(u1Level);
        break;
    default:
        _vDrvMJCCalc64FbkPos(u1Level);    
        break;
    }

    MJC_WRITE_FLD_MULTI(MJC_MC_26,
        P_Fld(_r64FbkPos.u1Pos0B5, MJC_INTP_POS_01_20)|
        P_Fld(_r64FbkPos.u1Pos1A5, MJC_INTP_POS_02_20)|
        P_Fld(_r64FbkPos.u1Pos1B5, MJC_INTP_POS_03_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_27,
        P_Fld(_r64FbkPos.u1Pos2A5, MJC_INTP_POS_04_20)|
        P_Fld(_r64FbkPos.u1Pos2B5, MJC_INTP_POS_05_20)|
        P_Fld(_r64FbkPos.u1Pos3A5, MJC_INTP_POS_06_20)|
        P_Fld(_r64FbkPos.u1Pos3B5, MJC_INTP_POS_07_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_28,
        P_Fld(_r64FbkPos.u1Pos4A5, MJC_INTP_POS_08_20)|
        P_Fld(_r64FbkPos.u1Pos4B5, MJC_INTP_POS_09_20));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos0B5), MJC_PROG_RDCTYPE_1_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos1A5), MJC_PROG_RDCTYPE_2_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos1B5), MJC_PROG_RDCTYPE_3_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos2A5), MJC_PROG_RDCTYPE_4_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos2B5), MJC_PROG_RDCTYPE_5_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos3A5), MJC_PROG_RDCTYPE_6_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos3B5), MJC_PROG_RDCTYPE_7_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos4A5), MJC_PROG_RDCTYPE_8_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos4B5), MJC_PROG_RDCTYPE_9_20));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(0, MJC_PROG_RDCTYPE_1_20)|P_Fld(0, MJC_PROG_RDCTYPE_2_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_3_20)|P_Fld(0, MJC_PROG_RDCTYPE_4_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_5_20)|P_Fld(0, MJC_PROG_RDCTYPE_6_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_7_20)|P_Fld(0, MJC_PROG_RDCTYPE_8_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_9_20));
    }
}

static void _vDrvMJCSetParFbckVdo_120(UINT8 u1Level)
{
    UINT8 u1FbkPos = (0x80 - (u1Level*2));
    MJC_WRITE_FLD(MJC_MC_20, u1FbkPos, MJC_INTP_POS_1_4);

    // rdctype
    MJC_WRITE_FLD(MJC_FM_08,
        (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL?INTP_RDCTYPE(u1FbkPos):0), MJC_PROG_RDCTYPE_1_4);
}

static void _vDrvMJCSetParFbckVdo_240(UINT8 u1Level)
{
    UINT8 u1FbkQty;
    UINT8 u1Pos[3];
    
    if (u1Level <= 32)
    {
        u1FbkQty = u1Level * 0x40 >> 5;
        u1Pos[0] = 0x40 - u1FbkQty;
        u1Pos[1] = 0x80;
        u1Pos[2] = 0xc0 - u1FbkQty;
    }
    else
    {
        u1FbkQty = (64 - u1Level) * 0x80 >> 5;
        u1Pos[0] = 0x0;
        u1Pos[1] = u1Pos[2] = u1FbkQty;
    }

    MJC_WRITE_FLD_MULTI(MJC_MC_20,
        P_Fld(u1Pos[0], MJC_INTP_POS_1_4) |
        P_Fld(u1Pos[1], MJC_INTP_POS_2_4) |
        P_Fld(u1Pos[2], MJC_INTP_POS_3_4));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(INTP_RDCTYPE(u1Pos[0]), MJC_PROG_RDCTYPE_1_4)|
            P_Fld(INTP_RDCTYPE(u1Pos[1]), MJC_PROG_RDCTYPE_2_4)|
            P_Fld(INTP_RDCTYPE(u1Pos[2]), MJC_PROG_RDCTYPE_3_4));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08, P_Fld(0, MJC_PROG_RDCTYPE_1_4)|P_Fld(0, MJC_PROG_RDCTYPE_2_4)|P_Fld(0, MJC_PROG_RDCTYPE_3_4));
    }
}

static void _vDrvMJCSetParFbck22_240_8X(UINT8 u1Level)
{
    UINT8 u1FbkQty;
    if(64 < u1Level)
	{
        LOG(3, "_vDrvMJCSetParFbck22_240_8X u1Level: %d \n", u1Level); 
		return;
	}
    u1FbkQty = _u1FbckQnty22[u1Level];

    if (u1Level <= _rFbkThr240.u1Thr0)
    {
        // HF toward 120Hz/100Hz
        _r22FbkPos.u1Pos[0] = 0;
        _r22FbkPos.u1Pos[1] = 0x20 - u1FbkQty;
        _r22FbkPos.u1Pos[2] = 0x40;
        _r22FbkPos.u1Pos[3] = 0x60 - u1FbkQty;
        _r22FbkPos.u1Pos[4] = 0x80;
        _r22FbkPos.u1Pos[5] = 0xa0 - u1FbkQty;
        _r22FbkPos.u1Pos[6] = 0xc0;
        _r22FbkPos.u1Pos[7] = 0xe0 - u1FbkQty;
    }
    else if (u1Level <= _rFbkThr240.u1Thr1)
    {
        // QF toward 44 rpt
        _r22FbkPos.u1Pos[0] = _r22FbkPos.u1Pos[1] = 0;
        _r22FbkPos.u1Pos[2] = _r22FbkPos.u1Pos[3] = 0x40 - u1FbkQty;
        _r22FbkPos.u1Pos[4] = _r22FbkPos.u1Pos[5] = 0x80;
        _r22FbkPos.u1Pos[6] = _r22FbkPos.u1Pos[7] = 0xc0 - u1FbkQty;
    }
    else
    {
        // toward total fallback 8:8 rpt
        _r22FbkPos.u1Pos[0] = _r22FbkPos.u1Pos[1] = 0;
        _r22FbkPos.u1Pos[2] = _r22FbkPos.u1Pos[3] = 0;
        _r22FbkPos.u1Pos[4] = _r22FbkPos.u1Pos[5] = u1FbkQty;
        _r22FbkPos.u1Pos[6] = _r22FbkPos.u1Pos[7] = u1FbkQty;
    }

    MJC_WRITE_FLD_MULTI(MJC_MC_21,
        P_Fld(_r22FbkPos.u1Pos[1], MJC_INTP_POS_1_8) |
        P_Fld(_r22FbkPos.u1Pos[2], MJC_INTP_POS_2_8) |
        P_Fld(_r22FbkPos.u1Pos[3], MJC_INTP_POS_3_8));
    MJC_WRITE_FLD_MULTI(MJC_MC_22,
        P_Fld(_r22FbkPos.u1Pos[4], MJC_INTP_POS_4_8) |
        P_Fld(_r22FbkPos.u1Pos[5], MJC_INTP_POS_5_8) |
        P_Fld(_r22FbkPos.u1Pos[6], MJC_INTP_POS_6_8) |
        P_Fld(_r22FbkPos.u1Pos[7], MJC_INTP_POS_7_8));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[1]), MJC_PROG_RDCTYPE_1_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[2]), MJC_PROG_RDCTYPE_2_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[3]), MJC_PROG_RDCTYPE_3_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[4]), MJC_PROG_RDCTYPE_4_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[5]), MJC_PROG_RDCTYPE_5_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[6]), MJC_PROG_RDCTYPE_6_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[7]), MJC_PROG_RDCTYPE_7_8));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_1_8)|P_Fld(0, MJC_PROG_RDCTYPE_2_8)|P_Fld(0, MJC_PROG_RDCTYPE_3_8)|P_Fld(0, MJC_PROG_RDCTYPE_4_8)|
            P_Fld(0, MJC_PROG_RDCTYPE_5_8)|P_Fld(0, MJC_PROG_RDCTYPE_6_8)|P_Fld(0, MJC_PROG_RDCTYPE_7_8));
    }
}

static void _vDrvMJCSetParFbck32_240_55(UINT8 u1Level)
{
    UINT8 u1FbkQty;
    if(64 < u1Level)
	{
        LOG(3, "_vDrvMJCSetParFbck32_240_55 u1Level: %d \n", u1Level); 
		return;
	}
    u1FbkQty = _u1FbckQnty32[u1Level];

    if (u1Level <= _rFbkThr240.u1Thr0)
    {
        // HF toward 120Hz
        _r32Fbk55p2Pos.u1Pos[0] = 0;
        _r32Fbk55p2Pos.u1Pos[1] = 0x19 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[2] = 0x33;
        _r32Fbk55p2Pos.u1Pos[3] = 0x4c - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[4] = 0x66;
        _r32Fbk55p2Pos.u1Pos[5] = 0x80 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[6] = 0x99;
        _r32Fbk55p2Pos.u1Pos[7] = 0xb2 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[8] = 0xcc;
        _r32Fbk55p2Pos.u1Pos[9] = 0xe5 - u1FbkQty;
    }
    else if (u1Level <= _rFbkThr240.u1Thr1)
    {
        // toward 55 rpt
        _r32Fbk55p2Pos.u1Pos[0] = 0;
        _r32Fbk55p2Pos.u1Pos[1] = 0;
        _r32Fbk55p2Pos.u1Pos[2] = 0x33 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[3] = 0x33 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[4] = 0x66 - (u1FbkQty<<1);
        _r32Fbk55p2Pos.u1Pos[5] = 0x66 + (u1FbkQty>>1);
        _r32Fbk55p2Pos.u1Pos[6] = 0x99 - (u1FbkQty>>1);
        _r32Fbk55p2Pos.u1Pos[7] = 0x99 - (u1FbkQty>>1);
        _r32Fbk55p2Pos.u1Pos[8] = 0xcc - (u1FbkQty + (u1FbkQty>>1));
        _r32Fbk55p2Pos.u1Pos[9] = 0xcc - (u1FbkQty + (u1FbkQty>>1));
    }
    else
    {
        // toward total fallback 10:10 rpt
        _r32Fbk55p2Pos.u1Pos[0]  = 0;
        _r32Fbk55p2Pos.u1Pos[1]  = 0;
        _r32Fbk55p2Pos.u1Pos[2]  = 0;
        _r32Fbk55p2Pos.u1Pos[3]  = 0;        
        _r32Fbk55p2Pos.u1Pos[4]  = 0;
        _r32Fbk55p2Pos.u1Pos[5]  = u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[6]  = u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[7]  = u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[8]  = u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[9]  = u1FbkQty;
    }

    MJC_WRITE_FLD_MULTI(MJC_MC_23,
        P_Fld(_r32Fbk55p2Pos.u1Pos[1], MJC_INTP_POS_1_10) |
        P_Fld(_r32Fbk55p2Pos.u1Pos[2], MJC_INTP_POS_2_10) |
        P_Fld(_r32Fbk55p2Pos.u1Pos[3], MJC_INTP_POS_3_10));
    MJC_WRITE_FLD_MULTI(MJC_MC_24,
        P_Fld(_r32Fbk55p2Pos.u1Pos[4], MJC_INTP_POS_4_10) |
        P_Fld(_r32Fbk55p2Pos.u1Pos[5], MJC_INTP_POS_5_10) |
        P_Fld(_r32Fbk55p2Pos.u1Pos[6], MJC_INTP_POS_6_10) |
        P_Fld(_r32Fbk55p2Pos.u1Pos[7], MJC_INTP_POS_7_10));
    MJC_WRITE_FLD_MULTI(MJC_MC_25,
        P_Fld(_r32Fbk55p2Pos.u1Pos[8], MJC_INTP_POS_8_10) |
        P_Fld(_r32Fbk55p2Pos.u1Pos[9], MJC_INTP_POS_9_10));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[1]), MJC_PROG_RDCTYPE_1_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[2]), MJC_PROG_RDCTYPE_2_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[3]), MJC_PROG_RDCTYPE_3_10));
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[4]), MJC_PROG_RDCTYPE_4_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[5]), MJC_PROG_RDCTYPE_5_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[6]), MJC_PROG_RDCTYPE_6_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[7]), MJC_PROG_RDCTYPE_7_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[8]), MJC_PROG_RDCTYPE_8_10)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[9]), MJC_PROG_RDCTYPE_9_10));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_1_10)|P_Fld(0, MJC_PROG_RDCTYPE_2_10)|P_Fld(0, MJC_PROG_RDCTYPE_3_10));
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(0, MJC_PROG_RDCTYPE_4_10)|P_Fld(0, MJC_PROG_RDCTYPE_5_10)|P_Fld(0, MJC_PROG_RDCTYPE_6_10)|
            P_Fld(0, MJC_PROG_RDCTYPE_7_10)|P_Fld(0, MJC_PROG_RDCTYPE_8_10)|P_Fld(0, MJC_PROG_RDCTYPE_9_10));
    }
}

static void _vDrvMJCSetParFbck22_240_HF(UINT8 u1Level)
{
    _vDrvMJCCalc22FbkPos(u1Level);

    MJC_WRITE_FLD_MULTI(MJC_MC_21,
        P_Fld(0, MJC_INTP_POS_1_8) |
        P_Fld(_r22FbkPos.u1Pos[1], MJC_INTP_POS_2_8) |
        P_Fld(_r22FbkPos.u1Pos[1], MJC_INTP_POS_3_8));
    MJC_WRITE_FLD_MULTI(MJC_MC_22,
        P_Fld(_r22FbkPos.u1Pos[2], MJC_INTP_POS_4_8) |
        P_Fld(_r22FbkPos.u1Pos[2], MJC_INTP_POS_5_8) |
        P_Fld(_r22FbkPos.u1Pos[3], MJC_INTP_POS_6_8) |
        P_Fld(_r22FbkPos.u1Pos[3], MJC_INTP_POS_7_8));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[1]), MJC_PROG_RDCTYPE_1_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[2]), MJC_PROG_RDCTYPE_2_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[3]), MJC_PROG_RDCTYPE_3_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[4]), MJC_PROG_RDCTYPE_4_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[5]), MJC_PROG_RDCTYPE_5_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[6]), MJC_PROG_RDCTYPE_6_8)|
            P_Fld(INTP_RDCTYPE(_r22FbkPos.u1Pos[7]), MJC_PROG_RDCTYPE_7_8));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_1_8)|P_Fld(0, MJC_PROG_RDCTYPE_2_8)|P_Fld(0, MJC_PROG_RDCTYPE_3_8)|P_Fld(0, MJC_PROG_RDCTYPE_4_8)|
            P_Fld(0, MJC_PROG_RDCTYPE_5_8)|P_Fld(0, MJC_PROG_RDCTYPE_6_8)|P_Fld(0, MJC_PROG_RDCTYPE_7_8));
    }
}

static void _vDrvMJCSetParFbck32_240_64HF(UINT8 u1Level)
{
    _vDrvMJCCalc64FbkPos(u1Level);

    MJC_WRITE_FLD_MULTI(MJC_MC_26,
        P_Fld(0, MJC_INTP_POS_01_20)|
        P_Fld(_r64FbkPos.u1Pos0B5, MJC_INTP_POS_02_20)|
        P_Fld(_r64FbkPos.u1Pos0B5, MJC_INTP_POS_03_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_27,
        P_Fld(_r64FbkPos.u1Pos1A5, MJC_INTP_POS_04_20)|
        P_Fld(_r64FbkPos.u1Pos1A5, MJC_INTP_POS_05_20)|
        P_Fld(_r64FbkPos.u1Pos1B5, MJC_INTP_POS_06_20)|
        P_Fld(_r64FbkPos.u1Pos1B5, MJC_INTP_POS_07_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_28,
        P_Fld(_r64FbkPos.u1Pos2A5, MJC_INTP_POS_08_20)|
        P_Fld(_r64FbkPos.u1Pos2A5, MJC_INTP_POS_09_20)|
        P_Fld(_r64FbkPos.u1Pos2B5, MJC_INTP_POS_10_20)|
        P_Fld(_r64FbkPos.u1Pos2B5, MJC_INTP_POS_11_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_29,
        P_Fld(_r64FbkPos.u1Pos3A5, MJC_INTP_POS_12_20)|
        P_Fld(_r64FbkPos.u1Pos3A5, MJC_INTP_POS_13_20)|
        P_Fld(_r64FbkPos.u1Pos3B5, MJC_INTP_POS_14_20)|
        P_Fld(_r64FbkPos.u1Pos3B5, MJC_INTP_POS_15_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_2A,
        P_Fld(_r64FbkPos.u1Pos4A5, MJC_INTP_POS_16_20)|
        P_Fld(_r64FbkPos.u1Pos4A5, MJC_INTP_POS_17_20)|
        P_Fld(_r64FbkPos.u1Pos4B5, MJC_INTP_POS_18_20)|
        P_Fld(_r64FbkPos.u1Pos4B5, MJC_INTP_POS_19_20));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(0, MJC_PROG_RDCTYPE_1_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos0B5), MJC_PROG_RDCTYPE_2_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos0B5), MJC_PROG_RDCTYPE_3_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos1A5), MJC_PROG_RDCTYPE_4_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos1A5), MJC_PROG_RDCTYPE_5_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos1B5), MJC_PROG_RDCTYPE_6_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos1B5), MJC_PROG_RDCTYPE_7_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos2A5), MJC_PROG_RDCTYPE_8_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos2A5), MJC_PROG_RDCTYPE_9_20));
        MJC_WRITE_FLD_MULTI(MJC_FM_14,
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos2B5), MJC_PROG_RDCTYPE_10_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos2B5), MJC_PROG_RDCTYPE_11_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos3A5), MJC_PROG_RDCTYPE_12_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos3A5), MJC_PROG_RDCTYPE_13_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos3B5), MJC_PROG_RDCTYPE_14_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos3B5), MJC_PROG_RDCTYPE_15_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos4A5), MJC_PROG_RDCTYPE_16_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos4A5), MJC_PROG_RDCTYPE_17_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos4B5), MJC_PROG_RDCTYPE_18_20)|
            P_Fld(INTP_RDCTYPE(_r64FbkPos.u1Pos4B5), MJC_PROG_RDCTYPE_19_20));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(0, MJC_PROG_RDCTYPE_1_20)|P_Fld(0, MJC_PROG_RDCTYPE_2_20)|P_Fld(0, MJC_PROG_RDCTYPE_3_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_4_20)|P_Fld(0, MJC_PROG_RDCTYPE_5_20)|P_Fld(0, MJC_PROG_RDCTYPE_6_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_7_20)|P_Fld(0, MJC_PROG_RDCTYPE_8_20)|P_Fld(0, MJC_PROG_RDCTYPE_9_20));
        MJC_WRITE_FLD_MULTI(MJC_FM_14,
            P_Fld(0, MJC_PROG_RDCTYPE_10_20)|P_Fld(0, MJC_PROG_RDCTYPE_11_20)|P_Fld(0, MJC_PROG_RDCTYPE_12_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_13_20)|P_Fld(0, MJC_PROG_RDCTYPE_14_20)|P_Fld(0, MJC_PROG_RDCTYPE_15_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_16_20)|P_Fld(0, MJC_PROG_RDCTYPE_17_20)|P_Fld(0, MJC_PROG_RDCTYPE_18_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_19_20));
    }
}

static void _vDrvMJCSetParFbck32_240_55p2(UINT8 u1Level)
{
    UINT8 u1FbkQty;
    if(64 < u1Level)
	{
        LOG(3, "_vDrvMJCSetParFbck32_240_55p2 u1Level: %d \n", u1Level); 
		return;
	}
    u1FbkQty = _u1FbckQnty32[u1Level];

    if (u1Level >= _rFbkThr240.u1Thr0)
        fgDisableHf = 1;

    if (u1Level <= _rFbkThr240.u1Thr0)
    {
        // HF toward 120Hz
        _r32Fbk55p2Pos.u1Pos[0] = _r32Fbk55p2Pos.u1Pos[10] = 0;
        _r32Fbk55p2Pos.u1Pos[1] = _r32Fbk55p2Pos.u1Pos[11] = 0x19 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[2] = _r32Fbk55p2Pos.u1Pos[12] = 0x33;
        _r32Fbk55p2Pos.u1Pos[3] = _r32Fbk55p2Pos.u1Pos[13] = 0x4c - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[4] = _r32Fbk55p2Pos.u1Pos[14] = 0x66;
        _r32Fbk55p2Pos.u1Pos[5] = _r32Fbk55p2Pos.u1Pos[15] = 0x80 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[6] = _r32Fbk55p2Pos.u1Pos[16] = 0x99;
        _r32Fbk55p2Pos.u1Pos[7] = _r32Fbk55p2Pos.u1Pos[17] = 0xb2 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[8] = _r32Fbk55p2Pos.u1Pos[18] = 0xcc;
        _r32Fbk55p2Pos.u1Pos[9] = _r32Fbk55p2Pos.u1Pos[19] = 0xe5 - u1FbkQty;
    }
    else if (u1Level <= _rFbkThr240.u1Thr1)
    {
        // QF toward 60Hz
        _r32Fbk55p2Pos.u1Pos[0] = 0;
        _r32Fbk55p2Pos.u1Pos[1] = 0x33 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[2] = 0x33 + u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[3] = 0x66;
        _r32Fbk55p2Pos.u1Pos[4] = 0x66;
        _r32Fbk55p2Pos.u1Pos[5] = 0x99 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[6] = 0x99 + u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[7] = 0xcc;
        _r32Fbk55p2Pos.u1Pos[8] = 0xcc;
        _r32Fbk55p2Pos.u1Pos[9] = 0xff - u1FbkQty;

        _r32Fbk55p2Pos.u1Pos[10] = 0 + u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[11] = 0x33;
        _r32Fbk55p2Pos.u1Pos[12] = 0x33;
        _r32Fbk55p2Pos.u1Pos[13] = 0x66 - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[14] = 0x66 + u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[15] = 0x99;
        _r32Fbk55p2Pos.u1Pos[16] = 0x99;
        _r32Fbk55p2Pos.u1Pos[17] = 0xcc - u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[18] = 0xcc + u1FbkQty;
        _r32Fbk55p2Pos.u1Pos[19] = 0xff;
    }
    else
    {
        _r32Fbk55p2Pos.u1Pos[0] = _r32Fbk55p2Pos.u1Pos[1] = 0;
        _r32Fbk55p2Pos.u1Pos[2] = _r32Fbk55p2Pos.u1Pos[3] =
        _r32Fbk55p2Pos.u1Pos[4] = _r32Fbk55p2Pos.u1Pos[5] = 0x66 - (u1FbkQty << 1);
        _r32Fbk55p2Pos.u1Pos[6] = _r32Fbk55p2Pos.u1Pos[7] =
        _r32Fbk55p2Pos.u1Pos[8] = _r32Fbk55p2Pos.u1Pos[9] = 0xCC + (u1FbkQty);
        _r32Fbk55p2Pos.u1Pos[10] = _r32Fbk55p2Pos.u1Pos[11] =
        _r32Fbk55p2Pos.u1Pos[12] = _r32Fbk55p2Pos.u1Pos[13] = 0x33 - (u1FbkQty);
        _r32Fbk55p2Pos.u1Pos[14] = _r32Fbk55p2Pos.u1Pos[15] =
        _r32Fbk55p2Pos.u1Pos[16] = _r32Fbk55p2Pos.u1Pos[17] = (0x99 + (u1FbkQty << 1));
        _r32Fbk55p2Pos.u1Pos[18] = _r32Fbk55p2Pos.u1Pos[19] = 0xff;
    }
    
    MJC_WRITE_FLD_MULTI(MJC_MC_26,
        P_Fld(_r32Fbk55p2Pos.u1Pos[1], MJC_INTP_POS_01_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[2], MJC_INTP_POS_02_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[3], MJC_INTP_POS_03_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_27,
        P_Fld(_r32Fbk55p2Pos.u1Pos[4], MJC_INTP_POS_04_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[5], MJC_INTP_POS_05_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[6], MJC_INTP_POS_06_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[7], MJC_INTP_POS_07_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_28,
        P_Fld(_r32Fbk55p2Pos.u1Pos[8], MJC_INTP_POS_08_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[9], MJC_INTP_POS_09_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[10], MJC_INTP_POS_10_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[11], MJC_INTP_POS_11_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_29,
        P_Fld(_r32Fbk55p2Pos.u1Pos[12], MJC_INTP_POS_12_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[13], MJC_INTP_POS_13_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[14], MJC_INTP_POS_14_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[15], MJC_INTP_POS_15_20));
    MJC_WRITE_FLD_MULTI(MJC_MC_2A,
        P_Fld(_r32Fbk55p2Pos.u1Pos[16], MJC_INTP_POS_16_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[17], MJC_INTP_POS_17_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[18], MJC_INTP_POS_18_20)|
        P_Fld(_r32Fbk55p2Pos.u1Pos[19], MJC_INTP_POS_19_20));

    // rdctype
    if (arMJCDemo.eDemoMode == E_MJC_DEMO_FULL)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[1]), MJC_PROG_RDCTYPE_1_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[2]), MJC_PROG_RDCTYPE_2_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[3]), MJC_PROG_RDCTYPE_3_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[4]), MJC_PROG_RDCTYPE_4_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[5]), MJC_PROG_RDCTYPE_5_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[6]), MJC_PROG_RDCTYPE_6_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[7]), MJC_PROG_RDCTYPE_7_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[8]), MJC_PROG_RDCTYPE_8_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[9]), MJC_PROG_RDCTYPE_9_20));
        MJC_WRITE_FLD_MULTI(MJC_FM_14,
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[10]), MJC_PROG_RDCTYPE_10_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[11]), MJC_PROG_RDCTYPE_11_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[12]), MJC_PROG_RDCTYPE_12_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[13]), MJC_PROG_RDCTYPE_13_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[14]), MJC_PROG_RDCTYPE_14_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[15]), MJC_PROG_RDCTYPE_15_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[16]), MJC_PROG_RDCTYPE_16_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[17]), MJC_PROG_RDCTYPE_17_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[18]), MJC_PROG_RDCTYPE_18_20)|
            P_Fld(INTP_RDCTYPE(_r32Fbk55p2Pos.u1Pos[19]), MJC_PROG_RDCTYPE_19_20));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_13,
            P_Fld(0, MJC_PROG_RDCTYPE_1_20)|P_Fld(0, MJC_PROG_RDCTYPE_2_20)|P_Fld(0, MJC_PROG_RDCTYPE_3_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_4_20)|P_Fld(0, MJC_PROG_RDCTYPE_5_20)|P_Fld(0, MJC_PROG_RDCTYPE_6_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_7_20)|P_Fld(0, MJC_PROG_RDCTYPE_8_20)|P_Fld(0, MJC_PROG_RDCTYPE_9_20));
        MJC_WRITE_FLD_MULTI(MJC_FM_14,
            P_Fld(0, MJC_PROG_RDCTYPE_10_20)|P_Fld(0, MJC_PROG_RDCTYPE_11_20)|P_Fld(0, MJC_PROG_RDCTYPE_12_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_13_20)|P_Fld(0, MJC_PROG_RDCTYPE_14_20)|P_Fld(0, MJC_PROG_RDCTYPE_15_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_16_20)|P_Fld(0, MJC_PROG_RDCTYPE_17_20)|P_Fld(0, MJC_PROG_RDCTYPE_18_20)|
            P_Fld(0, MJC_PROG_RDCTYPE_19_20));
    }
}

void vDrvSetPartialFbck(BOOL fgIsFilm, UINT8 u1Level)
{
    static BOOL _fgDisableHfPrev = 1;
    
    _fgDisableHfPrev = fgDisableHf;
    fgDisableHf = 0;
    
    if (IO32ReadFldAlign(MJC_FBCK_00, FBCK_DEBUG))
    {
        u1Level = IO32ReadFldAlign(MJC_FBCK_00, FBCK_LEVEL);
    }

    u1Level = MIN(u1Level, MAX_FBCK_LVL /*+ u1MJCPartialFbckThr_240_HF*/);

    vIO32WriteFldAlign(MJC_FBCK_10, u1Level, STA_FBCK_LEVEL);

    if (_pfnRemap != NULL)
    {
        u1Level = (*_pfnRemap)(u1Level, arMJCPrm.u1EffectParam);
    }
    
    vIO32WriteFldAlign(MJC_FBCK_10, u1Level, STA_FBCK_RMP_LEVEL);

    switch (u1DrvMJCGetIntpMode())
    {
    case E_MJC_2D_240:
        if (fgIsFilm == 0)
        {
            _vDrvMJCSetParFbckVdo_240(u1Level);
        }
        else
        {
            _vDrvMJCSetParFbck22_240_8X(u1Level);

            if (_rFbkThr240.u1Mthd == MJC_240_FBK_MTHD_DEMO)
                _vDrvMJCSetParFbck32_240_55p2(u1Level);
            else
                _vDrvMJCSetParFbck32_240_55(u1Level);
        }
        break;
    case E_MJC_2D_120:
    case E_MJC_3D_120:
    case E_MJC_TTD_120:
        if (fgIsFilm == 0)
        {
            // vdo
            _vDrvMJCSetParFbckVdo_120(u1Level);
        }
        else
        {
            // 25->100, 30->120
            _vDrvMJCSetParFbck22(u1Level);

            // 24->60, 24->120
            _vDrvMJCSetParFbck32(u1Level);
        }
        break;
    case E_MJC_2D_60:
    case E_MJC_3D_60:
        if (fgIsFilm == 0)
        {
            // vdo
            //MJC_WRITE_FLD(MJC_MC_20, (0x80 - (u1Level*2)), MJC_INTP_POS_1_4);
        }
        else
        {
            // 25->50, 30->60
            _vDrvMJCSetParFbck22_60(u1Level);

            // 24->60
            _vDrvMJCSetParFbck32_55(u1Level);
        }
        break;
    case E_MJC_TTD_240:
        if (fgIsFilm == 0)
        {
            // no video memc effect in TTD
            _vDrvMJCSetParFbckVdo_240(0);
        }
        else
        {
            _vDrvMJCSetParFbck22_240_HF(u1Level);

            _vDrvMJCSetParFbck32_240_64HF(u1Level);
        }
        break;
    }

    if (_fgDisableHfPrev != fgDisableHf)
        vDrvMJCUpdateIntpCtrl();
}

void vDrvMJCFbckModeChg(void)
{
    UINT8 u1Mthd120 = IO32ReadFldAlign(MJC_FBCK_13, FBCK_MTHD_32);
    UINT8 u1Mthd120_22 = IO32ReadFldAlign(MJC_FBCK_13, FBCK_MTHD_22);
    UINT8 u1Mthd240 = IO32ReadFldAlign(MJC_FBCK_37, FBCK_240_MTHD_32);
    if (IO32ReadFldAlign(MJC_FBCK_13, FBCK_THR_TRIG) ||
        (_rFbkThr120.u1Mthd != u1Mthd120) || (_rFbkThr240.u1Mthd != u1Mthd240) ||
        (_rFbkThr120.u1Mthd22 != u1Mthd120_22))
    {
        switch (u1DrvMJCGetIntpMode())
        {
        case E_MJC_2D_240:
            _vDrvMJCCalFbckQntyArr_240();
            break;
        case E_MJC_2D_60:
        case E_MJC_2D_120:
        case E_MJC_3D_60:
        case E_MJC_3D_120:
        case E_MJC_TTD_120:
        case E_MJC_TTD_240:
            _vDrvMJCCalFbckQntyArr();
            break;
        }
        
        _vDrvMJCLoadStageCfg();

        vIO32MJCWriteFldAlign(MJC_FBCK_13, 0, FBCK_THR_TRIG);
    }

    _vDrvMJCReadWriteTable();
}

